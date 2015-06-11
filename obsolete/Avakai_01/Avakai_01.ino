

#include <RFduinoBLE.h>
#include <Wire.h>
#include "Seeed_QTouch.h"

// Pin for the LED
int RLEDPin = 6;
int GLEDPin = 5;
int BLEDPin = 4;

int vibro = 1;

// I2C-Bus on Pins 2 & 3!

//int wavPin0 = 1;    //Sound-Board-Pins 
//int wavPin1 = 0;

byte state;


void setup(){
  //Serial.begin(9600);  //for debugging
  Wire.beginOnPins(3, 2);  // SCL on GPIO 3 and SDA on GPIO 2
  Serial.begin(9600);
  QTouch.reset(); //resets touch sensor
  QTouch.calibrate(); //calibrates sensor
  QTouch.setNTHRForKey(255, 3); //sets negative threshold
  QTouch.setNTHRForKey(20, 4); //sets negative threshold   //Head-Sensor
  QTouch.setNTHRForKey(255, 5); //sets negative threshold
  
  // Set up the RGB-LED - all on - white color
  pinMode(RLEDPin, OUTPUT);
  digitalWrite(RLEDPin, LOW);
  pinMode(GLEDPin, OUTPUT);
  digitalWrite(GLEDPin, LOW);
  pinMode(BLEDPin, OUTPUT);
  digitalWrite(BLEDPin, LOW);
  
  // Set up the communication with sound board 
  /* pinMode(wavPin0, OUTPUT); 
     digitalWrite(wavPin0, HIGH);
     pinMode(wavPin1, OUTPUT);
     digitalWrite(wavPin1, HIGH);  */
  
  
  // configure the RFduino BLE properties
  RFduinoBLE.advertisementData = "test";
  RFduinoBLE.advertisementInterval = 500;
  RFduinoBLE.deviceName = "Avakai";
  RFduinoBLE.txPowerLevel = -20;
  
  // start the BLE stack
  RFduinoBLE.begin();
  
  
}

void loop(){
      
    
   char stat = QTouch.getState();
  
   /* if (0x28&stat)
    {
        Serial.println("Hug recognized!");
    } */
    
    /* if(0x10&stat)
    {
        long time2 = millis();                          // get origin time
        Serial.print("Head touched: ");
        while(QTouch.isTouch(4));                       // unless key released
        Serial.print(millis() - time2);                 
        Serial.println(" ms");
    } */
    
    
    if (QTouch.isTouch(4)){      //if Head-Sensor is touched
      
      RGBchange(state);
      
      //sound(state);
      
      state++;
      if (state == 8) state = 0;
      delay(500);
    }
  
   
  
}

/* void sound (byte nr){
    
    unsigned int Timer0;
    
    if (nr>3) nr -= 3; 
    
    //Serial.println(nr);
    
    if (nr == 1) {
      Timer0=millis();
      digitalWrite(wavPin0,LOW); 
      while ( (millis() - Timer0) < 60);
      digitalWrite(wavPin0,HIGH);
    }
    
    if (nr == 2) {
      Timer0=millis();
      digitalWrite(wavPin1,LOW); 
      while ( (millis() - Timer0) < 60);
      digitalWrite(wavPin1,HIGH);
    
    }
    
    if (nr == 3) {
      Timer0=millis();
      digitalWrite(wavPin0,LOW); 
      digitalWrite(wavPin1,LOW); 
      while ( (millis() - Timer0) < 60);
      digitalWrite(wavPin0,HIGH);
      digitalWrite(wavPin1,HIGH);
    
    }
} */

void RFduinoBLE_onReceive(char *data, int len)
{
	
	RGBchange(data[0]);
	
}

