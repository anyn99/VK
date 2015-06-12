

#include <RFduinoBLE.h>
#include <Wire.h>
#include <Seeed_QTouch.h>

// Pin for the LED
int RLEDPin = 6;
int GLEDPin = 5;
int BLEDPin = 4;

// I2C-Bus on Pins 2 & 3!

int wav0 = 1;
int wav1 = 0;



char btdata[8];
//btdata[0] = State of RGB-LED
//btdata[1] = State of Sensors, first bit = Head Sensor
//btdata[2] = MSB sensor value
//btdata[3] = LSB sensor value

//btdata[5] = Sound to play
//btdata[6] = command from app

int timer0 = millis();
int timer1 = millis();


void setup(){
  //Serial.begin(9600);  //for debugging
  Serial.println("Start");
  
  Wire.beginOnPins(3, 2);  // SCL on GPIO 3 and SDA on GPIO 2
  
  // Set up the RGB-LED - all on - white color
  pinMode(RLEDPin, OUTPUT);
  pinMode(GLEDPin, OUTPUT);
  pinMode(BLEDPin, OUTPUT);
  btdata[0]=7;
  RGBchange(btdata[7]);
  
  pinMode(wav0, OUTPUT); 
  digitalWrite(wav0, HIGH);
  pinMode(wav1, OUTPUT);
  digitalWrite(wav1, HIGH);
  
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
  
  btdata[5]=1;
  
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
   Serial.println((int)(btdata[3]));
   Serial.print("Status reads: ");
   Serial.println(btdata[1], HEX);
   */
    
    if (0x10&btdata[1]){      //if Head-Sensor is touched
       
       if (millis()-timer0>50)
       {  
         sound(btdata[5]);
         RGBchange(btdata[0]);
         delay(300);
         RGBchange(0);
       }
       
       if (millis()-timer1>1500)
       {
          btdata[0]++;
          if (btdata[0] == 8) btdata[0] = 1;
          sound(btdata[5]);
          RGBchange(btdata[0]);
          delay(300);
          RGBchange(0);
          timer1=millis();
       }
 
      timer0=millis();
    }
    else  timer1= millis();
 
  
  /*
  if (0x02&stat){    
    
     Serial.println("HUG");
    }
  */ 
  
  BTsendstatus();
  delay(5);
}

void sound (byte nr){
    
    unsigned int STimer;
    
    if (nr>3) nr -= 3; 
    
    //Serial.println(nr);
    
    if (nr == 1) {
      STimer=millis();
      digitalWrite(wav0,LOW); 
      while ( (millis() - STimer) < 60);
      digitalWrite(wav0,HIGH);
    }
    
    if (nr == 2) {
      STimer=millis();
      digitalWrite(wav1,LOW); 
      while ( (millis() - STimer) < 60);
      digitalWrite(wav1,HIGH);
    
    }
    
    if (nr == 3) {
      STimer=millis();
      digitalWrite(wav0,LOW); 
      digitalWrite(wav1,LOW); 
      while ( (millis() - STimer) < 60);
      digitalWrite(wav0,HIGH);
      digitalWrite(wav1,HIGH);
    
    }
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

        btdata[0] = data[0]; //store color value
        btdata[5] = data[1]; //store sound value
        btdata[6] = data[2]; //store command
        
        if (btdata[6]==1){
        sound(btdata[5]);
        RGBchange(btdata[0]);
        delay(300);
        RGBchange(0);
        }
        
        
        /*
        Serial.print("data[0]: ");
        Serial.println(data[0],HEX);
        Serial.print("data[1]: ");
        Serial.println(data[1],HEX);
        */
        
	BTsendstatus();
}

