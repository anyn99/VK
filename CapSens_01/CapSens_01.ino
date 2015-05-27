

// Pin for the LED
int RLEDPin = 4;
int GLEDPin = 5;
int BLEDPin = 6;
// Pin to connect to your drawing
int SensePin = 2;
int RefPin = 3;

int wavPin0 = 0;
//int wavPin1 = 1;

// This is how high the sensor needs to read in order
//  to trigger a touch.  You'll find this number
//  by trial and error, or you could take readings at 
//  the start of the program to dynamically calculate this.
int touchedCutoff = 200;
unsigned int Timer0 = millis();
byte ledstate;

void setup(){
  //Serial.begin(9600);
  // Set up the LED
  pinMode(RLEDPin, OUTPUT);
  digitalWrite(RLEDPin, LOW);
  pinMode(GLEDPin, OUTPUT);
  digitalWrite(GLEDPin, LOW);
  pinMode(BLEDPin, OUTPUT);
  digitalWrite(BLEDPin, LOW);
  
  pinMode(wavPin0, OUTPUT);
  digitalWrite(wavPin0, HIGH);
  //pinMode(wavPin1, OUTPUT);
  //digitalWrite(wavPin1, HIGH);
  
  //Timer0= millis();
  
}

void loop(){
  // If the capacitive sensor reads above a certain threshold,
  //  turn on the LED
  int value = readCap(RefPin,SensePin,60);
  if (value > touchedCutoff && (millis() - Timer0) >= 300) {
    Timer0 = millis();
    ledstate = RGBchange();
    digitalWrite(wavPin0,LOW);
    delay(1000);
    digitalWrite(wavPin0,HIGH);
    
  }
   /*
    Serial.print("Capacitive Sensor on Pin 2 reads: ");
    Serial.print(value);
    Serial.print("\t");
    Serial.print("state: ");
    Serial.println(ledstate);
  */
  
}



