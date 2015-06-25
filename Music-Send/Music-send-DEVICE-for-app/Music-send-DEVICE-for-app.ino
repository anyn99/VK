

#include <RFduinoBLE.h>
#include <RFduinoGZLL.h>
#include <Wire.h>
#include <Seeed_QTouch.h>

// Pin for the LED
int RLEDPin = 6;
int GLEDPin = 5;
int BLEDPin = 4;

// I2C-Bus on Pins 2 & 3!

int wav0 = 1;
int wav1 = 0;

device_t role = DEVICE0;
  

byte command;
byte color = 7;  //color white for startup
byte note = 1;   //sound 1 for startup
byte touchstate;
byte LSBhead;
byte MSBhead;




int timer0 = millis();
int timer1 = millis();
int GZLLtimer = millis();

void setup(){
  // Serial.begin(9600);  //for debugging
  Serial.println("Start");
  
  Wire.beginOnPins(3, 2);  // SCL on GPIO 3 and SDA on GPIO 2
  
                              // Set up the RGB-LED 
  pinMode(RLEDPin, OUTPUT);
  pinMode(GLEDPin, OUTPUT);
  pinMode(BLEDPin, OUTPUT);
  RGBchange(color);               //initialized with white color
  
  
  pinMode(wav0, OUTPUT); 
  digitalWrite(wav0, HIGH);
  pinMode(wav1, OUTPUT);
  digitalWrite(wav1, HIGH);
  

  

  

  
  QTouch.reset(); //resets touch sensor
  delay(250);
  QTouch.calibrate(); //calibrates sensor
  delay(250);
  
  QTouch.writeReg(36, 25); //sets threshold key4  //Head-Sensor
  delay(8);
  QTouch.writeReg(35, 255); //key3 big threshold
  delay(8);
  QTouch.writeReg(37, 255); //key5 big threshold
  delay(8);
  
  QTouch.writeReg(50, 8); //set detection integrator for key4
  delay(8);
  
  //QTouch.setAKSForKey(0,1); //removes key1 from AKS-groups
  //delay(8);
  
  //Averaging Factors, 0 to turn channel off
  QTouch.writeReg(39, 8); //key0
  delay(8);
  QTouch.writeReg(40, 0); //key1
  delay(8);
  QTouch.writeReg(41, 0); //key2
  delay(8);
  QTouch.writeReg(42, 4); //key3
  delay(8);
  QTouch.writeReg(43, 8); // set key4 to averaging factor   
  delay(8);
  QTouch.writeReg(44, 4); //key5
  delay(8);
  QTouch.writeReg(45, 0); //key6
  delay(8);
  
  
  
  //just for testing! remove for final functionality!
  gzll_on();
  
}


void loop(){
    
   
   touchstate = QTouch.getState();
    delay(8);
   
   MSBhead =  (QTouch.readReg(12)); //MSB value of Headsensor
     delay(8);
   LSBhead =  (QTouch.readReg(13));  //LSB value of Headsensor
    delay(8);
   
   /*
   Serial.print("btdata[2]: ");
   Serial.println(btdata[2], HEX);
   Serial.print("Note     : ");
   Serial.println(note, HEX);
   delay(500);
   */
   
    if (0x10 & touchstate){      //if Head-Sensor is touched
       
       if (millis()-timer0>50)
       {  
         sound(note);
         RGBchange(color);
         delay(300);
         RGBchange(0);
       }
       
       if (millis()-timer1>1500)
       {
          color++;
          if (color == 8) color = 1;
          sound(note);
          RGBchange(color);
          delay(300);
          RGBchange(0);
          
          if (color == 2) gzll_on();
          if (color == 4) bt_on();
          
          timer1=millis();
          
       }
 
      timer0=millis();
    }
    else  timer1= millis();
   
   
    if (millis()-GZLLtimer > 1000)
    {
      RFduinoGZLL.sendToHost(NULL,0); //send to HOST to ask for music message
      GZLLtimer=millis();
    }
}

void bt_on()
{
  RFduinoGZLL.end(); //end gazelle stack
  delay(30);
    // configure the RFduino BLE properties
  RFduinoBLE.advertisementData = "test";
  RFduinoBLE.advertisementInterval = 1000;
  RFduinoBLE.deviceName = "Avakai";
  RFduinoBLE.txPowerLevel = 0;    //possible values:  (-20, -16, -12, -8, -4, 0, +4)
  
  RFduinoBLE.begin(); //start bluetooth stack
}


void gzll_on()
{
  RFduinoBLE.end(); //end bluetooth stack
  delay(30);
  RFduinoGZLL.txPowerLevel = 0;
  RFduinoGZLL.begin(role); //start GZLL communication;  
}


/*
void BTsendstatus(){
  byte btdata[6] = {command,color,note,touchstate,LSBhead,MSBhead};
  RFduinoBLE.send(btdata, 8);
}
*/

void RFduinoBLE_onReceive(char *data, int len)
{
        byte *received = (byte*)data; //store received data in array
        
        
        if (received[0]==0x1F)  //first received byte tells packet type, functionality with app
        {
          color = received[1]; //store color value
          note = received[2]; //store sound value
        
          sound(note);
          RGBchange(color);
          delay(300);
          RGBchange(0); 
        }
       
       
       
        if (received[0]==0x0F) //first byte 0F means its a music message
        {
          int i = 1;
          while (i<=len)
          {
            sound(received[i]);   //play note i=1,6,11,16
            i++;
            RGBchange(color);     //blink
   
            delay( (received[i+1]<<8) + received[i]); //duration led on - combine values from LSB and MSB from data stream, i=2,7,13,18
            Serial.print("Press duration: ");
            Serial.println((received[i+1]<<8) + received[i]);
            
            RGBchange(0);
            i=i+2;
            delay( (received[i+1]<<8) + received[i]); //wait pause - combine values from LSB and MSB from data stream, i=4,9,15
            
            Serial.print("Pause: ");
            Serial.println((received[i+1]<<8) + received[i]);
            
            i=i+2;        
          }
        }

        
}


void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len)
{
        byte *received = (byte*)data; //store received data in array
        
        
        if (received[0]==0x1F)  //first received byte tells packet type, 01 means functionality with app
        {
          color = received[1]; //store color value
          note = received[2]; //store sound value
        
          sound(note);
          RGBchange(color);
          delay(300);
          RGBchange(0); 
        }
       
       
       
        if (received[0]==0x0F) //first byte 0F means its a music message
        {
          int i = 1;
          while (i<=len)
          {
            sound(received[i]);   //play note i=1,6,11,16
            i++;
            RGBchange(color);     //blink
   
            delay( (received[i+1]<<8) + received[i]); //duration led on - combine values from LSB and MSB from data stream, i=2,7,13,18
            Serial.print("Press duration: ");
            Serial.println((received[i+1]<<8) + received[i]);
            
            RGBchange(0);
            i=i+2;
            delay( (received[i+1]<<8) + received[i]); //wait pause - combine values from LSB and MSB from data stream, i=4,9,15
            
            Serial.print("Pause: ");
            Serial.println((received[i+1]<<8) + received[i]);
            
            i=i+2;        
          }
        }
        
        RFduinoGZLL.sendToHost(data, len);

}


