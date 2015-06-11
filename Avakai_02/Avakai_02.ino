

#include <RFduinoBLE.h>
#include <Wire.h>
#include "QTouchI2C.h"

// Pin for the LED
int RLEDPin = 6;
int GLEDPin = 5;
int BLEDPin = 4;

// I2C-Bus on Pins 2 & 3!

//int vibro = 1;




char btdata[8];
//btdata[0] = State of RGB-LED
//btdata[1] = State of Sensors, first bit = Head Sensor

int timer0 = millis();



void setup(){
  //Serial.begin(9600);  //for debugging
  
  Wire.beginOnPins(3, 2);  // SCL on GPIO 3 and SDA on GPIO 2
  
  // Set up the RGB-LED - all on - white color
  pinMode(RLEDPin, OUTPUT);
  analogWrite(RLEDPin, 220);
  pinMode(GLEDPin, OUTPUT);
  analogWrite(GLEDPin, 220);
  pinMode(BLEDPin, OUTPUT);
  analogWrite(BLEDPin, 220);
  
  //pinMode(vibro, OUTPUT);
  
  // configure the RFduino BLE properties
  RFduinoBLE.advertisementData = "test";
  RFduinoBLE.advertisementInterval = 500;
  RFduinoBLE.deviceName = "Avakai";
  RFduinoBLE.txPowerLevel = +4;    //possible values:  (-20, -16, -12, -8, -4, 0, +4)
  
  // start the BLE stack
  RFduinoBLE.begin();
  
  QTouch.reset(); //resets touch sensor
  delay(250);
  QTouch.calibrate(); //calibrates sensor
  delay(250);
  
  QTouch.writeReg(36, 25); //sets negative threshold key4  //Head-Sensor
  delay(8);
  QTouch.writeReg(35, 255); //key3 big threshold
  delay(8);
  QTouch.writeReg(37, 255); //key5 big threshold
  delay(8);
  
  //QTouch.setAKSForKey(0,1); //removes key1 from AKS-groups
  //delay(8);
  
  //Averaging Factors, 0 to turn channel off
  QTouch.writeReg(39, 0); //key0
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
  
  
}

void loop(){
    
   
   btdata[1] = QTouch.getState();
    delay(8);
   
   btdata[2] =  (QTouch.readReg(12)); //MSB value of key1
     delay(8);
   btdata[3] =  (QTouch.readReg(13));  //LSB value of key1
    delay(8);
   
   /*
   Serial.print("Key 4 reads: ");
   Serial.println((int)(value1));
   Serial.print("Status reads: ");
   Serial.println(stat, HEX);
   */
    
    if (0x10&btdata[1]){      //if Head-Sensor is touched
       if (millis()-timer0>100)
       {
         btdata[0]++;
         if (btdata[0] == 8) btdata[0] = 0;
         RGBchange(btdata[0]);
       }
 
      timer0=millis();
    }
  
  /*
  if (0x02&stat){    
    
     Serial.println("HUG");
    }
  */ 
  
  BTsendstatus();
  delay(5);
}

/*
void vibrate(char mil){
  int timer = millis();
  digitalWrite(vibro, HIGH);
  while (millis()-timer<mil);
  digitalWrite(vibro, LOW);
}
*/

void BTsendstatus(){
  RFduinoBLE.send(btdata, 8);
}

void RFduinoBLE_onReceive(char *data, int len)
{
	
	RGBchange(data[0]);
        btdata[0] = data[0]; //store RGBstate
       
	BTsendstatus();
}

