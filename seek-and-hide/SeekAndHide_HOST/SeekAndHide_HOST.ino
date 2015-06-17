#include <RFduinoGZLL.h>

device_t role = HOST;

int vibratorPin = 3;
int ledPin = 2;
int piezoPin = 6;

unsigned long ct;
unsigned long lastVibrateTime = 0;
unsigned long lastPingTime = 0;
int vibrateFrequenzy = 0;


void setup() 
{
  RFduinoGZLL.begin(role);
  Serial.begin(9600);
  pinMode(vibratorPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(piezoPin, INPUT);
  vibrate();
  blinkLed();
  delay(200);
  blinkLed();
  delay(200);
  blinkLed();
}


void loop() 
{
  ct = millis();  
  
  if(vibrateFrequenzy > 0 && (lastVibrateTime + vibrateFrequenzy) < ct)
  {
     vibrate();
  }
  
  if((millis() - lastPingTime) > 3000)
  {
     vibrateFrequenzy = 0;
  }
  
  int piezoVal = analogRead(piezoPin);
  
  if(piezoVal > 140 && millis() - lastVibrateTime > 500 )
  {
     vibrate();
     blinkLed();
  }
  
  Serial.println(piezoVal);
  delay(100);  
}
  
  
void blinkLed()
{
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
}

void vibrate()
{
  digitalWrite(vibratorPin, HIGH);
  delay(200);
  digitalWrite(vibratorPin, LOW);
  lastVibrateTime = millis();
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len)
{
  lastPingTime = millis();
  char state = data[0]; // this test is not needed for a single device
  //Serial.println("yo");
  //Serial.println("ao"); // no data to piggyback on the acknowledgement sent back to the Device
  char str[15];
  sprintf(str, "%d", rssi); 
  
  float A = -55.0; // Reference RSSI value at 1 meter
  float n = 2.0;
  float distance = pow(10, ((A - (float) rssi)/(10.0*n)));

  if(distance < 1.0)
  {
    vibrateFrequenzy = 500;
  }
  else
  {
    vibrateFrequenzy = 0;
  }  

RFduinoGZLL.sendToDevice(device, str);
}
