

unsigned long  leastTotal = 0x0FFFFFFFL;   // input large value for autocalibrate begin
unsigned int   loopTimingFactor = 310;   // determined empirically -  a hack
unsigned long  CS_Timeout_Millis = (2000 * (float)loopTimingFactor * (float)F_CPU) / 16000000;
unsigned long  CS_AutocaL_Millis = 20000;
unsigned long  lastCal = millis();         // set millis for start
unsigned long  total;


// Public Methods //////////////////////////////////////////////////////////////

long readCap(byte RefPin, byte SensePin, unsigned int samples)
{
	total = 0;
	if (samples == 0) return 0;
        pinMode(RefPin, OUTPUT); //set RefPin as Output

	for (unsigned int i = 0; i < samples; i++) {    // loop for samples parameter - simple lowpass filter
		if (SenseOneCycle() < 0)  return -2;   // variable over timeout
}

		// only calibrate if time is greater than CS_AutocaL_Millis and total is less than 10% of baseline
		// this is an attempt to keep from calibrating when the sensor is seeing a "touched" signal

		if ( (millis() - lastCal > CS_AutocaL_Millis) && abs(total  - leastTotal) < (int)(.15 * (float)leastTotal) ) {

			// Serial.println();               // debugging
			// Serial.println("auto-calibrate");
			// Serial.println();
			// delay(2000); */

			leastTotal = 0x0FFFFFFFL;          // reset for "autocalibrate"
			lastCal = millis();
		}
		/*else{                                // debugging
			Serial.print("  total =  ");
			Serial.print(total);

			Serial.print("   leastTotal  =  ");
			Serial.println(leastTotal);

			Serial.print("total - leastTotal =  ");
			x = total - leastTotal ;
			Serial.print(x);
			Serial.print("     .1 * leastTotal = ");
			x = (int)(.1 * (float)leastTotal);
			Serial.println(x);
		} */

	// routine to subtract baseline (non-sensed capacitance) from sensor return
	if (total < leastTotal) leastTotal = total;                 // set floor value to subtract from sensed value
	return(total - leastTotal);

}

long readCapRaw(unsigned int samples)
{
	total = 0;
	if (samples == 0) return 0;

	for (unsigned int i = 0; i < samples; i++) {    // loop for samples parameter - simple lowpass filter
		if (SenseOneCycle() < 0)  return -2;   // variable over timeout
	}

	return total;
}


void reset_AutoCal(void){
	leastTotal = 0x0FFFFFFFL;
}

void set_AutocaL_Millis(unsigned long autoCal_millis){
	CS_AutocaL_Millis = autoCal_millis;
}

void set_Timeout_Millis(unsigned long timeout_millis){
	CS_Timeout_Millis = (timeout_millis * (float)loopTimingFactor * (float)F_CPU) / 16000000;  // floats to deal with large numbers
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

