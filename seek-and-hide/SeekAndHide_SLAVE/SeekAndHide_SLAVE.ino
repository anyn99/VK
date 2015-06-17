
#include <RFduinoGZLL.h>


device_t role = DEVICE0;

char state = 0;

int speakerPin = 5;
int vibratorPin = 3;
int ledPin = 2;

void setup() 
{
  RFduinoGZLL.begin(role);
  Serial.begin(9600);
  pinMode(vibratorPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

//int signalStrength = -40;
unsigned long ct;
unsigned long lastVibrateTime = 0;
unsigned long lastPongTime = 0;
int vibrateFrequenzy = 2000;  

void loop() 
{
  ct = millis();  
  state = ! state;
  RFduinoGZLL.sendToHost(state);

  if(vibrateFrequenzy > 0 && (lastVibrateTime + vibrateFrequenzy) < ct)
  {
    digitalWrite(vibratorPin, HIGH);
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(vibratorPin, LOW);
    digitalWrite(ledPin, LOW);
    lastVibrateTime = ct;
  }

  if((millis() - lastPongTime) > 3000)
  {
    vibrateFrequenzy = 2000;
  } 
  
  delay(100);
}


void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len)
{
  lastPongTime = millis();
  char state = data[0];
  int remoteRssi =   atoi(data);
  int avgRssi = rssi;
  if(remoteRssi != 0){ avgRssi = (remoteRssi + rssi) / 2; }

  float A = -55.0; // Reference RSSI value at 1 meter
  float n = 2.0;
  float distance = pow(10, ((A - (float) rssi)/(10.0*n))); 
  //Serial.println(distance);
  //signalStrength = avgRssi;
  vibrateFrequenzy = distance * 100;  
} 
