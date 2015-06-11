
#include <SoftwareSerial.h>

// Pin to connect to your drawing
int SensePin = 3;
int RefPin = 4;

// This is how high the sensor needs to read in order
//  to trigger a touch.  You'll find this number
//  by trial and error, or you could take readings at 
//  the start of the program to dynamically calculate this.

SoftwareSerial mySerial =  SoftwareSerial(0, 1); // 0=RX 1=TX

unsigned long  leastTotal = 0x0FFFFFFFL;   // input large value for autocalibrate begin

const unsigned int  CS_Timeout_Millis = 60000;

unsigned long  lastCal = millis();         // set millis for start
unsigned int  total;


// Public Methods //////////////////////////////////////////////////////////////

unsigned int readCap(byte samples)
{
	total = 0;
	if (samples == 0) return 0;
        pinMode(RefPin, OUTPUT); //set RefPin as Output

	for (byte i = 0; i < samples; i++) {    // loop for samples parameter - simple lowpass filter
		if (SenseOneCycle() < 0)  return -2;   // variable over timeout
}

		// only calibrate if time is greater than CS_AutocaL_Millis and total is less than 10% of baseline
		// this is an attempt to keep from calibrating when the sensor is seeing a "touched" signal

		if ( (millis() - lastCal > 20000) && abs(total  - leastTotal) < (int)(.15 * (float)leastTotal) ) {

	

			leastTotal = 0x0FFFFFFFL;          // reset for "autocalibrate"
			lastCal = millis();
		}
	

	// routine to subtract baseline (non-sensed capacitance) from sensor return
	if (total < leastTotal) leastTotal = total;                 // set floor value to subtract from sensed value
	return(total - leastTotal);

}


// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

int SenseOneCycle(void)
{
    noInterrupts();
	digitalWrite(RefPin, LOW);	// RefPin low
	//pinMode(SensePin, INPUT);	// SensePin to input (pullups are off)
	pinMode(SensePin, OUTPUT);      // SensePin to OUTPUT
	digitalWrite(SensePin, LOW);	// pin is now LOW AND OUTPUT
	delayMicroseconds(10);
	pinMode(SensePin, INPUT);	// SensePin to input (pullups are off)
	digitalWrite(RefPin, HIGH);	// RefPin High
    interrupts();

	while ( !digitalRead(SensePin) && (total < CS_Timeout_Millis) ) {  // while SensePin is LOW AND total is less than timeout
		total++;
	}
	//Serial.print("SenseOneCycle(1): ");
	//Serial.println(total);

	if (total > CS_Timeout_Millis) {
		return -2;         //  total variable over timeout
	}

	// set receive pin HIGH briefly to charge up fully - because the while loop above will exit when pin is ~ 2.5V
    noInterrupts();
	//digitalWrite(SensePin, HIGH);
	pinMode(SensePin, OUTPUT);        // SensePin to OUTPUT - pin is now HIGH AND OUTPUT
	digitalWrite(SensePin, HIGH);
	pinMode(SensePin, INPUT);	// SensePin to INPUT (pullup is off)
	digitalWrite(RefPin, LOW);	// RefPin LOW
    interrupts();

	while ( digitalRead(SensePin) && (total < CS_Timeout_Millis) ) {  // while SensePin is HIGH  AND total is less than timeout
		total++;
	}
	//Serial.print("SenseOneCycle(2): ");
	//Serial.println(total);

	if (total >= CS_Timeout_Millis) {
		return -2;     // total variable over timeout
	} else {
		return 1;
	}
}




void setup(){
  
  pinMode(2, OUTPUT);
  mySerial.begin(9600);
  
}

void loop(){
  // If the capacitive sensor reads above a certain threshold,
  //  turn on the LED
  unsigned int value = readCap(60);
  
  if (value > 100) digitalWrite(2, HIGH);
    else digitalWrite(2, LOW);
    delay (10);
  
  
    mySerial.println(value);

}



