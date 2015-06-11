// demo of Qtouch - isTouch
// this demo will use isTouch function
// this function can the state of certain key
// such as you can get state of KEY0 with this function:
// isTouch(0), if return 1 meas KEY0 got touched while
// return 0 no touch.
// besides, this demo also print the touch time

#include <Wire.h>
#include "Seeed_QTouch.h"

void setup()
{   
    Wire.beginOnPins(3, 2);  // SCL on GPIO 3 and SDA on GPIO 2
    Serial.begin(9600);
    QTouch.reset(); //resets touch sensor
   QTouch.calibrate(); //calibrates sensor
   QTouch.setNTHRForKey(255, 3); //sets negative threshold
   QTouch.setNTHRForKey(20, 4); //sets negative threshold   //Head-Sensor
   QTouch.setNTHRForKey(255, 5); //sets negative threshold
   //QTouch.setAKSForKey(0, 3);
   //QTouch.setAKSForKey(0, 4);
   //QTouch.setAKSForKey(0, 5);
}

void loop()
{
    char stat = QTouch.getState();
  
   if (0x28&stat)
    {
        Serial.println("Hug recognized!");
    }
    
    if(0x10&stat)
    {
        long time2 = millis();                          // get origin time
        Serial.print("Head touched: ");
        while(QTouch.isTouch(4));                       // unless key released
        Serial.print(millis() - time2);                 
        Serial.println(" ms");
    }
    
    
    
    delay(10);
}
