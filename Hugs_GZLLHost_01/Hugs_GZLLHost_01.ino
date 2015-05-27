#include <RFduinoGZLL.h>

#define PIN_LED     0
#define PIN_SENSOR  2
#define PIN_BUZZ    4
#define REACT_SPEED 0.05

device_t role = HOST;

int hugLevel = 0;
char hugSend = 0;

boolean reacting = false;
boolean growing = false;
float levelLED = 255.0;

char valReceived = 'Z';
char reactThreshold = 'G';

void setup()
{
  pinMode(PIN_LED,  OUTPUT); 
  pinMode(PIN_BUZZ, OUTPUT);
  digitalWrite(PIN_LED,  HIGH);
  digitalWrite(PIN_BUZZ, LOW);
  pinMode(PIN_SENSOR, INPUT);
 
  //Serial.begin(9600);
  
  RFduinoGZLL.begin(role);
}

void loop()
{
  int level = analogRead(PIN_SENSOR);  
  hugLevel = smooth(level);
  hugSend = (char)constrain(map(hugLevel, 0, 25, 'A', 'Z'), 'A', 'Z');

  //Serial.print(valReceived);
  if (valReceived <= reactThreshold) {
    reacting = true;
    //Serial.println('R');
  }
  else {
    reacting = false;
    //Serial.println('N');
  }
  updateReaction();
}

void updateReaction() {
  if (reacting || levelLED < 254) {
    if (growing) {
      levelLED += REACT_SPEED;
      if (levelLED > 254) {
        levelLED = 254;
        growing = false;
      }
    } 
    else {
      levelLED -= REACT_SPEED;
      if (levelLED < 1) {
        levelLED = 1;
        growing = true;
      }
    }
    analogWrite(PIN_LED,  (int)levelLED);
    analogWrite(PIN_BUZZ, 255-((int)levelLED));
  }
  else {
    levelLED = 254;
    growing = false;
    digitalWrite(PIN_BUZZ, LOW);
  }
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len)
{
  if (len > 0)
  {
    valReceived = data[0];
  }
  RFduinoGZLL.sendToDevice(device, hugSend);
}

