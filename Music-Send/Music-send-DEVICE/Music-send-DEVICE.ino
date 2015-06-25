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
byte sendarray[20];     //array to send over GZLL to Device0
byte ct = 1;

char btdata[8];

byte command;
byte color = 7;  //color white for startup
byte note = 1;   //sound 1 for startup
byte touchstate;
byte LSBhead;
byte MSBhead;

boolean colorchanged = false;
int timerpressed;
int longpress;
int lastpress;



void setup(){
  //Serial.begin(9600);  //for debugging
  Serial.println("Start DEVICE0");
  
  Wire.beginOnPins(3, 2);  // SCL on GPIO 3 and SDA on GPIO 2
  
                              // Set up the RGB-LED 
  pinMode(RLEDPin, OUTPUT);
  pinMode(GLEDPin, OUTPUT);
  pinMode(BLEDPin, OUTPUT);
  RGBchange(0);               //initialized with led off
  
  
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
  
  RFduinoGZLL.txPowerLevel = 0;
  RFduinoGZLL.begin(role); //start GZLL communication;
  
  
}


void loop(){
    
   
   touchstate = QTouch.getState();
     delay(8);
   
   MSBhead =  (QTouch.readReg(12)); //MSB value of Headsensor
     delay(8);
   LSBhead =  (QTouch.readReg(13));  //LSB value of Headsensor
     delay(8);
   
   
    if (0x10&touchstate){      //if Head-Sensor is touched
       
       int pause = millis() - lastpress;
       
       timerpressed = millis();
       longpress = millis();
       colorchanged = false;                   //reset state for loop
       
       RGBchange(color);        //led on
       note = random(3) + 1;
       sound(note);             //play note
       
       while (0x10 & QTouch.getState())   //while pressed
       {
         if (millis() - longpress > 1500) //changing color/mode
         {         
           
           RGBchange(0); //switch off led
           color++;
           if (color == 8) color = 1;
           colorchanged = true;
           sound(note);
           delay(300);
           RGBchange(color);
           delay(500);
           RGBchange(0);
           
           switch (color) 
           {
             case 2:
             
             break;
             
             case 4:
             
             break;
             
             default:
             
             break;
           }
           
           longpress = millis();
         }
       }
       RGBchange(0);            //led off when touch released
       
       if (colorchanged == false)  //only do if color hasnt just been changed
       {
         
         timerpressed = millis() - timerpressed; //calculate how long press was
         //Serial.print("timerpressed: ");
         //Serial.println(timerpressed);
         
         if (ct > 1)            //if not first note send pause since last press
         {
           sendarray[ct] = pause & 0xFF;
           sendarray[ct+1] = pause >> 8;
           ct = ct + 2;
         }
         
         
         sendarray[ct] = note;
         sendarray[ct+1] = timerpressed & 0xFF;
         sendarray[ct+2] = timerpressed >> 8;
         ct = ct + 3;                            //count to next array variable
         
         
         if (ct >= 17)
         {
           
           sendarray[0] = 0x0F;   //mark as music message for GZLL handler to send array  

           RFduinoGZLL.sendToHost((const char *)&sendarray, sizeof(sendarray));
         
           ct = 1;
           sendarray[0] = 0; //reset first byte
         }
         
         lastpress = millis();
         
       }
    }
 
     
}



void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len)
{
    byte *received = (byte*)data; //store received data in array
    
    /*
    for (int i=0;i<=len;i++)
           {
           Serial.print("received[");
           Serial.print(i);
           Serial.print("]: ");
           Serial.println(received[i]);
           } 
    */
}


