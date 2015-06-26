

//This is the Avakai receiving notes!

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

device_t role = HOST;
  


byte color = 2;  //color green for startup
byte note = 1;   //sound 1 for startup
byte touchstate;
byte LSBhead;
byte MSBhead;

byte received[32];
boolean recv = false;
int lenrecv;

int presstimer = millis();
int longpress = millis();
int rtimer;

void setup(){
  //Serial.begin(9600);  //for debugging
  Serial.println("Start HOST");
  
  Wire.beginOnPins(3, 2);  // SCL on GPIO 3 and SDA on GPIO 2
  
                              // Set up the RGB-LED 
  pinMode(RLEDPin, OUTPUT);
  pinMode(GLEDPin, OUTPUT);
  pinMode(BLEDPin, OUTPUT);
  RGBchange(0);               //initialize led color
  
  
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
  
  RFduinoGZLL.begin(role); //start GZLL communication;  

  
} //end of setup


void loop()
{

  
   MSBhead =  (QTouch.readReg(12)); //MSB value of Headsensor
     delay(8);
   LSBhead =  (QTouch.readReg(13));  //LSB value of Headsensor
    delay(8);
    
   touchstate = QTouch.getState();

    
    if (0x10&touchstate)  //if Head-Sensor is touched
    {      
       
       if (millis()-presstimer>50)
       {  
         
         RGBchange(color);        //led on
         note = random(3) + 1;
         sound(note);             //play note
         delay(300);
         RGBchange(0);
         
       }
      
       
       
       if (millis() - longpress > 1500) //changing color/mode
       {         
           color++;
           if (color == 8) color = 1;
           sound(note);
           RGBchange(color);
           delay(300);
           RGBchange(0);
           
           longpress = millis();
       }
       
       presstimer=millis();
    }
   else  longpress= millis();
   
   
   if (recv==true)
        {
           /*
           for (int i=0;i<=(len);i++)
           {
           Serial.print("data[");
           Serial.print(i);
           Serial.print("]: ");
           Serial.println(data[i]);
           }
           Serial.println(sizeof(data));
           */
           
          int i = 1;
          while (received[i] != 0)
          {
            RGBchange(color);     //LED on with current color
            sound(received[i]);   //play note 
            delay(300);
            RGBchange(0);  //LED off
            i++;
            
            if (received[i] != 0)
            {
            rtimer= millis();
            while( (millis()-rtimer) < ((received[i+1]<<8) + received[i]) ); //wait pause - combine values from LSB and MSB from data stream,
            Serial.print("Pause: ");
            Serial.println((received[i+1]<<8) + received[i]);
            i=i+2;
            }

          }   
          
          RFduinoGZLL.sendToDevice(DEVICE0, received[0]); //tell device we are done
          recv = false; //reset state for new data over GZLL interrupt
       }
   
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len)
{
        //store received data in array//store received data in array
        if (data[0]==0x0F)
        {
          for (int i=0;i<len;i++)
           {
             received[i] = data[i];
           }
           recv = true;
        }  
        
        
        
        
}





