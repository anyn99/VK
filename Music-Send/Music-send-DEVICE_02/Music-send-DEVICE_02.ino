#include <RFduinoBLE.h>
#include <RFduinoGZLL.h>
#include <Wire.h>
#include <Seeed_QTouch.h>


#define HEAD_SENSOR_SENSITIVITY 35




// Pin for the LED
int RLEDPin = 6;
int GLEDPin = 5;
int BLEDPin = 4;

// I2C-Bus on Pins 2 & 3!

int wav0 = 1;
int wav1 = 0;

device_t role = DEVICE0;
byte sendarray[32];     //array to send over GZLL to Device0
byte ct = 1;

char btdata[8];

byte command;
byte color = 4;  //color white for startup
byte note = 1;   //sound 1 for startup
byte touchstate;
byte LSBhead;
byte MSBhead;


int presstimer;
int longpress;
int lastpress;
int wait;

boolean ble_on = false;
boolean gzll_on = false;


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
  
  QTouch.writeReg(36, HEAD_SENSOR_SENSITIVITY); //sets threshold key4  //Head-Sensor
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
  
  //BLE stack is initiated at startup 
  RFduinoBLE.advertisementData = "test";
  RFduinoBLE.advertisementInterval = 500;
  RFduinoBLE.deviceName = "Avakai";
  RFduinoBLE.txPowerLevel = +4;    //possible values:  (-20, -16, -12, -8, -4, 0, +4)
  
  // start the BLE stack
  RFduinoBLE.begin();
  ble_on = true;
  
 
} //end of setup


void loop(){

   MSBhead =  (QTouch.readReg(12)); //MSB value of Headsensor
     delay(8);
   LSBhead =  (QTouch.readReg(13));  //LSB value of Headsensor
     delay(8);
   
   touchstate = QTouch.getState();
     
     
   if (0x10&touchstate)  //if Head-Sensor is touched
    {      
       
       if (millis()-presstimer>50)
       {  
         int pause = millis() - lastpress;
         
         RGBchange(color);        //led on
         note = random(3) + 1;
         sound(note);             //play note
         delay(300);
         RGBchange(0);
         
        if (color == 2)
        {
         
         if (ct > 1) //if not first note
         {
         sendarray[ct] = pause & 0xFF; //pause since last call
         sendarray[ct+1] = pause >> 8;
         
         Serial.print("Pause first byte ");
         Serial.println(sendarray[ct]);
         Serial.print("Pause second byte ");
         Serial.println(sendarray[ct+1]);
         
         ct = ct + 2;
         }
         
         sendarray[ct] = note;
         Serial.print("Note ");
         Serial.print(ct);
         Serial.print(" is ");
         Serial.println(note);
         ct++;
          
         lastpress = millis();
         
         sendarray[0] = 0x0F; //mark as ready to send
        }
        
        
       }
      

       if (millis() - longpress > 1500) //changing color/mode
       {         
           if (color == 4) color = 2;
           else color = 4;
           sound(note);
           RGBchange(color);
           delay(300);
           RGBchange(0);
           
           longpress = millis();
       }

       presstimer=millis();
    }
   else  longpress= millis();
       
   bthandler(); //handles connection over Bluetooth
  
  
} //end of loop


void bthandler()
{
            if (color !=2 && ble_on == true) BLEsendstatus(); //if BLE is on, send status to app

            if (color == 2 && gzll_on == true && millis() - presstimer > 2000 && sendarray[0] == 0x0F) SendToOther(); //if green and GZLL radio on
            
            
           if (color == 2 && ble_on == true && gzll_on == false)
           {
             BLEsendstatus();
             delay(30);
             while(RFduinoBLE.radioActive);
             RFduinoBLE.end();
            
             
             RFduinoGZLL.begin(role); //start GZLL communication;
             ble_on = false;
             gzll_on = true;
           }
           
           if (color != 2 && gzll_on == true && ble_on == false)  
           {
             RFduinoGZLL.end(); // end GZLL stack;
             
             // start the BLE stack
             RFduinoBLE.begin();
             gzll_on = false;
             ble_on = true;
           }
}


void SendToOther()
{
        // send data over GZLL to other doll
         
         Serial.println("Sending!");

         RFduinoGZLL.sendToHost((const char *)&sendarray, sizeof(sendarray));
         
         for (int i=0;i<=sizeof(sendarray);i++)
           {
           Serial.print("sendarray[");
           Serial.print(i);
           Serial.print("]: ");
           Serial.println(sendarray[i]);
           } 
         
         ct = 1; //reset array counter
         memset(sendarray, 0, sizeof(sendarray)); //resetting array
         

}


void BLEsendstatus()
{
  btdata[0] = color;
  btdata[1] = touchstate;
  btdata[2] = MSBhead;
  btdata[3] = LSBhead;
  
  btdata[5] = note;
  btdata[6] = command;
  
  RFduinoBLE.send(btdata, 8);

}


void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len)
{
    byte *received = (byte*)data; //store received data in array
    
    if (data[0]==0x0F) //if acknowledged by other Avakai
    {
      
    }
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

void RFduinoBLE_onReceive(char *data, int len)
{

        color = data[0]; //store color value
        note = data[1]; //store sound value
        command = data[2]; //store command

        if (command==1)
        {
          sound(note);
          RGBchange(color);
          delay(300);
          RGBchange(0);
        }
	

}

