
        short  leastTotal = 0x0FFF;   // input large value for autocalibrate begin
        short   loopTimingFactor = 310;   // determined empirically -  a hack
        unsigned long  CS_Timeout_Millis = (2000 * (float)loopTimingFactor * (float)F_CPU) / 16000000;
        unsigned long  CS_AutocaL_Millis = 20000;
        unsigned long  lastCal = millis();         // set millis for start
        short  total = 0;
       

short CapSens(byte sendPin, byte receivePin, uint8_t samples)
{
	total = 0;

        //init
        DDRB |=     (1<<sendPin);						// sendpin to OUTPUT
        PORTB &=  ~(1<<sendPin);          //sendPin LOW;
        
	DDRB &= ~(1<<receivePin);    			                        // receivePin to INPUT
	PORTB &= ~(1<<receivePin);      // Pullup off  
	
        if (samples == 0) return 0;
	
	for (uint8_t i = 0; i < samples; i++) {    // loop for samples parameter - simple lowpass filter


	        noInterrupts();
	        PORTB &= ~(1<<sendPin);   	// sendPin low
	        DDRB &= ~(1<<receivePin);	// receivePin to input (pullup already off)
                DDRB |= (1<<receivePin);        // receivePin to OUTPUT 
	        PORTB &= ~(1<<receivePin); 	// pin is now LOW AND OUTPUT
	        for (byte i=0; i<80; i++);     //aprox on 8Mhz 100x125nS = 10microseconds delay - or more
	        DDRB &= ~(1<<receivePin);	// receivePin to input (pullups are off)
	        PORTB |= (1<<sendPin); 	        // sendPin High
                interrupts();

	        while ( !(PINB & (1 << receivePin)) && (total < CS_Timeout_Millis) ) {  // while receive pin is LOW AND total is positive value
		total++;
	        }
	

	        if (total > CS_Timeout_Millis) {
		return -2;         //  total variable over timeout
	        }

	        // set receive pin HIGH briefly to charge up fully - because the while loop above will exit when pin is ~ 2.5V
                noInterrupts();
	        PORTB |= (1<<receivePin);        //recievePin HIGH
	        DDRB |= (1<<receivePin);  // receivePin to OUTPUT - pin is now HIGH AND OUTPUT
	        PORTB &= ~(1<<receivePin);       //Pullup off
	        DDRB &= ~(1<<receivePin);	// receivePin to INPUT (pullup is off)
	        PORTB &= ~(1<<sendPin);	// sendPin LOW
                interrupts();


	        while ( (PINB & (1 << receivePin)) && (total < CS_Timeout_Millis) ) {  // while receive pin is HIGH  AND total is less than timeout
		total++;
	        }

	        if (total >= CS_Timeout_Millis) {
		return -2;     // total variable over timeout
	        } else {
		return 1;
	        }
         }

		// only calibrate if time is greater than CS_AutocaL_Millis and total is less than 10% of baseline
		// this is an attempt to keep from calibrating when the sensor is seeing a "touched" signal

		if ( (millis() - lastCal > CS_AutocaL_Millis) && abs(total  - leastTotal) < (int)(.10 * (float)leastTotal) ) {

			// Serial.println();               // debugging
			// Serial.println("auto-calibrate");
			// Serial.println();
			// delay(2000); */

			leastTotal = 0x0FFF;          // reset for "autocalibrate"
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



void CapSens_reset_CS_AutoCal(void){
	leastTotal = 0x0FFFFFFFL;
}

void CapSens_set_CS_AutocaL_Millis(unsigned long autoCal_millis){
	CS_AutocaL_Millis = autoCal_millis;
}

void CapSens_set_CS_Timeout_Millis(unsigned long timeout_millis){
	CS_Timeout_Millis = (timeout_millis * (float)loopTimingFactor * (float)F_CPU) / 16000000;  // floats to deal with large numbers
}



