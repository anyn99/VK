

/*
PinOut ATtiny45:
Pin5 = Ardu 0 = PB0
Pin6 = Ardu 1 = PB1
Pin7 = Ardu 2 = PB2
Pin2 = Ardu 3 = PB3
Pin3 = Ardu 4 = PB4
*/

#include <SoftwareSerial.h>
// Definitions
#define rxPin 0
#define txPin 1
SoftwareSerial mySerial(rxPin, txPin);


unsigned short leastTotal1 = 0xFFFF;   // input large value for autocalibrate begin
unsigned short leastTotal1 = 0xFFFF;   // input large value for autocalibrate begin

unsigned short val1=0;
unsigned short val2=0;





unsigned short CapSens(byte sendPin, byte receivePin, uint8_t samples)
{
	unsigned short total = 0;

        //init
        DDRB |=     (1<<sendPin);						// sendpin to OUTPUT
        PORTB &=  ~(1<<sendPin);          //sendPin LOW;
        
	DDRB &= ~(1<<receivePin);    			                        // receivePin to INPUT
	PORTB &= ~(1<<receivePin);      // Pullup off  
	
        if (samples == 0) return 0;
	
	for (uint8_t i = 0; i < samples; i++) {    // loop for samples parameter - simple lowpass filter


	        noInterrupts();
	        PORTB &= ~(1<<sendPin);   	// sendPin LOW
	        DDRB &= ~(1<<receivePin);	// receivePin to input (pullup already off)
                DDRB |= (1<<receivePin);        // receivePin to OUTPUT 
	        PORTB &= ~(1<<receivePin); 	// pin is now LOW AND OUTPUT
	        for (byte i=0; i<80; i++);     //aprox on 8Mhz 100x125nS = 10microseconds delay - or more
	        DDRB &= ~(1<<receivePin);	// receivePin to input (pullups are off)
	        PORTB |= (1<<sendPin); 	        // sendPin High
                interrupts();

	        while ( !(PINB & (1 << receivePin)) /*&& (total < CS_Timeout_Millis)*/ ) {  // while receive pin is LOW AND total is positive value
		total++;
	        }
	

	        /*if (total >= CS_Timeout_Millis) {
		return 3;         //  total variable over timeout
	        }*/

	        // set receive pin HIGH briefly to charge up fully - because the while loop above will exit when pin is ~ 2.5V
                noInterrupts();
	        PORTB |= (1<<receivePin);        //recievePin HIGH
	        DDRB |= (1<<receivePin);  // receivePin to OUTPUT - pin is now HIGH AND OUTPUT
	        PORTB &= ~(1<<receivePin);  //receive Pin LOW
                DDRB &= ~(1<<receivePin);	// receivePin to INPUT 
	        PORTB &= ~(1<<sendPin);	// sendPin LOW
                interrupts();


	        while ( (PINB & (1 << receivePin)) /*&& (total < CS_Timeout_Millis)*/ ) {  // while receive pin is HIGH  AND total is less than timeout
		total++;
	        }

	        /*if (total >= CS_Timeout_Millis) {
		return 4;     // total variable over timeout
	        }*/
         }

		// only calibrate if time is greater than CS_AutocaL_Millis and total is less than 10% of baseline
		// this is an attempt to keep from calibrating when the sensor is seeing a "touched" signal

		/* if ( (millis() - lastCal > CS_AutocaL_Millis) && abs(total  - leastTotal) < (int)(leastTotal/10) ) {

			// Serial.println();               // debugging
			mySerial.println("auto-calibrate");
			mySerial.println();
			delay(2000);

			leastTotal = 0xFFFF;          // reset for "autocalibrate"
			//lastCal = millis();
		}*/
		/*else{                                // debugging
			mySerial.print("  total =  ");
			mySerial.println(total);

			mySerial.print(" leastTotal  =  ");
			mySerial.println(leastTotal);

			mySerial.print("total - leastTotal =  ");
			int x = total - leastTotal ;
			mySerial.print(x);
			mySerial.print("     .1 * leastTotal = ");
			x = (int)(leastTotal/10);
			mySerial.println(x);
		}*/
          
	// routine to subtract baseline (non-sensed capacitance) from sensor return
	
        if (total < leastTotal) leastTotal = total;                 // set floor value to subtract from sensed value
	return(total - leastTotal);


}



/*void CapSens_calib(void){
	leastTotal = 0xFFFF;
}*/


void setup()
{
    
    mySerial.begin(19200);

}



void loop() {
  
    
   mySerial.println("main loop");
     
   val1 =  CapSens(2,3,30); //(SendPin,ReceivePin,samples)
          mySerial.print("Floorvalue: ");
          mySerial.println(leastTotal);
          
   mySerial.print("Sensor 1: ");
   mySerial.println(val1);
   
   mySerial.print("Sensor 2: ");
   mySerial.println(val2); 
   
   
   delay(1000);           // 1 second delay

}
