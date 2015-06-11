// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.
// 

#include <Wire.h>

volatile int value =0;
#define I2C_SLAVE_ADDR  0x26

byte buffer[16];
void setup()
{
  memset(buffer,0,sizeof(buffer)); //set buffer to zero
  
  Wire.beginOnPins(5, 6);  // SCL on GPIO 5 and SDA on GPIO 6
  
  Serial.begin(9600);  // start serial for output
}

void loop()
{
 
 for(byte i=0; i <= 15; i++){
 Wire.requestFrom(I2C_SLAVE_ADDR,1);
 
 if (Wire.available()) {
 
   Serial.print ("Byte received: ");
   Serial.println ( i );
   buffer[i] = Wire.read();
   }
 }
 
 for(byte f=0; f <= 3; f++){
 Serial.print("Byte ");
 Serial.print(f);
 Serial.print(" = ");
 Serial.println(buffer[f],HEX);

 }
 
 delay(1500);
 
}
