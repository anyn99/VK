

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
  

byte command;
byte color = 7;  //color white for startup
byte note = 1;   //sound 1 for startup
byte touchstate;
byte LSBhead;
byte MSBhead;

byte received[20];
boolean recv= false;

int timer0 = millis();
int timer1 = millis();
int rtimer;

void setup(){
  //Serial.begin(9600);  //for debugging
  //Serial.println("Start HOST");
  
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
  
  RFduinoGZLL.begin(role); //start GZLL communication;  

  
}


void loop(){
    
   
   touchstate = QTouch.getState();
    delay(8);
   
   MSBhead =  (QTouch.readReg(12)); //MSB value of Headsensor
     delay(8);
   LSBhead =  (QTouch.readReg(13));  //LSB value of Headsensor
    delay(8);
   
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
          
          timer1=millis();
          
       }
 
      timer0=millis();
    }
    else  timer1= millis();
   
   if (recv==true)
   {
           /*
           for (int i=0;i<=(sizeof(received)-1);i++)
           {
           Serial.print("received[");
           Serial.print(i);
           Serial.print("]: ");
           Serial.println(received[i]);
           }
           Serial.println(sizeof(received));
           */
           
          int i = 1;
          while (i<=sizeof(received))
          {
            RGBchange(color);     //LED on with current color
            sound(received[i]);   //play note i=1,6,11,16
            i++;
            rtimer= millis();
            while( (millis()-rtimer) < (((received[i+1]<<8) + received[i])-60) ); //duration led on - combine values from LSB and MSB from data stream, i=2,7,13,18; -60ms for playing sound!
            //Serial.print("Press duration: ");
            //Serial.println((received[i+1]<<8) + received[i]);
            
            RGBchange(0);  //LED off
            i=i+2;
            rtimer= millis();
            while( (millis()-rtimer) < ((received[i+1]<<8) + received[i]) ); //wait pause - combine values from LSB and MSB from data stream, i=4,9,15
            //Serial.print("Pause: ");
            //Serial.println((received[i+1]<<8) + received[i]);
            
            i=i+2;   
          }   
          recv = false; //reset state for new data over GZLL interrupt
    }
    
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len)
{
        //store received data in array//store received data in array
        if (data[0]==0x0F)
        {
        for (int i=0;i<=(len-1);i++)
           {
           received[i]=data[i];
           }
        recv = true;
        }  
        RFduinoGZLL.sendToDevice(device, data[0]);
        

}





