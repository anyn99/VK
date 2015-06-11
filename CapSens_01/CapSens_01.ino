
//#include <RFduinoBLE.h>

// Pin for the LED
int RLEDPin = 4;
int GLEDPin = 5;
int BLEDPin = 6;

int SensePin = 2;
int RefPin = 3;

byte state;

// This is how high the sensor needs to read in order
//  to trigger a touch.  You'll find this number
//  by trial and error, or you could take readings at 
//  the start of the program to dynamically calculate this.
int touchedCutoff = 180;
unsigned int Timer0 = millis();
byte ledstate;

void setup()
{
  Serial.begin(9600);
  // Set up the LED
  pinMode(RLEDPin, OUTPUT);
  digitalWrite(RLEDPin, LOW);
  pinMode(GLEDPin, OUTPUT);
  digitalWrite(GLEDPin, LOW);
  pinMode(BLEDPin, OUTPUT);
  digitalWrite(BLEDPin, LOW);
  
  
  // configure the RFduino BLE properties
  //RFduinoBLE.advertisementData = "test";
  //RFduinoBLE.advertisementInterval = 1000;
  //RFduinoBLE.deviceName = "Avakai";
  //RFduinoBLE.txPowerLevel = -20;
  
  // start the BLE stack
  //RFduinoBLE.begin();
}

void loop()
{
  // If the capacitive sensor reads above a certain threshold,
  //  turn on the LED
  //while (RFduinoBLE.radioActive);
  int value = readCap(RefPin,SensePin,60);
  
   if (value > touchedCutoff && (millis() - Timer0) >= 300)     //if Head-Sensor is touched
      {
        
      Timer0 = millis();
      
      RGBchange(state);
      
      state++;
      if (state == 8) state = 0;

      }
    
    Serial.print("Capacitive Sensor on Pin 2 reads: ");
    Serial.print(value);
    Serial.print("\t");
    Serial.print("state: ");
    Serial.println(ledstate);
     
  
}

/*
void RFduinoBLE_onReceive(char *data, int len)
{
	
	RGBchange(data[0]);
	
}
*/
