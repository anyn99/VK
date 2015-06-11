#include <TinyWireS.h>                  // wrapper class for I2C slave routines

#define I2C_SLAVE_ADDR  0x26            // i2c slave address (38)

int pin = 3; //LED 
boolean state = false;
unsigned int cnt = 0;
boolean firstbyte = true;
byte lowByte;
byte highByte;
int BLNK = 60000;

void setup()
{
 pinMode(pin, OUTPUT);
 digitalWrite(pin, LOW);
 
 TinyWireS.begin(I2C_SLAVE_ADDR);      // init I2C Slave mode , PB0 is SDA , PB2 is SCL !Dont change SCL-Pin - doesnt work!
 TinyWireS.onRequest(requestEvent);

}

void loop()
{
  cnt++;
  if (cnt > BLNK){
    if (state == false) digitalWrite(pin,LOW);
    if (state == true) digitalWrite(pin,HIGH);
    state = !state;
    cnt = 0;
 }
}

void requestEvent()
{
  BLNK = 20000;
  lowByte = 1;
   highByte = 2;
 if(firstbyte == true){     // on the first byte we do the math
   firstbyte = false;      //so next time though we send the next byte    
   TinyWireS.send(lowByte);
 }
 else {
   TinyWireS.send(highByte);
   firstbyte = true;
 }
}


