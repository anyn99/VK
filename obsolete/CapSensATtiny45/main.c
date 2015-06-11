
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "softuart.h"


#define MASK_REFPINS   (1<<PB4)
#define MASK_SENSEPINS (1<<PB3)

#define MAX_CYCLE       2000

uint16_t keys[3];


unsigned long  leastTotal = 0x0FFFFFFFL;   // input large value for autocalibrate begin


unsigned int  total;

char str[16];



//------------------------------functions start-----------------------------------------
// SensePin = PB3;
// byte RefPin = PB4;


int SenseOneCycle(void)
{
	
	cli();				 //Interrupts deaktivieren 	
    DDRB  &= ~_BV(PB3); //Beide Pins als Eingänge definieren
	DDRB  &= ~_BV(PB4);
    PORTB &= ~_BV(PB3); //Pullups deaktivieren
	PORTB &= ~_BV(PB4);
    
    DDRB  |= _BV(PB4); //Beide Pins als Ausgänge definieren
    DDRB  |= _BV(PB3); //Werden auf "LOW" gezogen durch PORT Befehl vorher?
	
    _delay_us(5);		//warten bis sicher "LOW"
    DDRB &= ~_BV(PB4);	//Beide Pins wieder als Eingänge definieren
    DDRB &= ~_BV(PB3);

    PORTB |= _BV(PB3);	//Pullup aktiviert nur am SensePin 
	
    while ( !(PINB & _BV(PB3))) { //Schleife bis SensePin "HIGH" wird
	
        DDRB |=  _BV(PB3);	//Setze SensePin als Ausgang
        _delay_us(5);		//Warte bis Touch-Kondensator geladen
        DDRB &= ~_BV(PB3);	//Setze SensePin als Eingang
        DDRB |=  _BV(PB4);	//Setze RefPin als Ausgang -> auf "LOW"-Level
        _delay_us(20);		//Warte -> Touch-Kondensator entlädt sich in Cs
        DDRB &= ~_BV(PB4);	//Setze RefPin als Eingang -> "tristate"
    
		total++;			//Zähler hochsetzen
	}
    DDRB |= _BV(PB4);		//Beide Pins als Ausgänge	
    DDRB |= _BV(PB3);	
    PORTB &= ~_BV(PB4); 	//Setze beide Pins auf "LOW"
	PORTB &= ~_BV(PB3);
	sei();					//Interrupts wieder aktivieren
	
	
    
		return 1;
}



unsigned int readCap(unsigned char samples)
{
	total = 0;
	if (samples == 0) return 0;

	for (unsigned char i = 0; i < samples; i++) {    // loop for samples parameter - simple lowpass filter
		if (SenseOneCycle() < 0)  return -2;   // variable over timeout
		/*
		softuart_puts_P("Schleife: ");
		itoa(i, str, 10);
		softuart_puts(str);
		softuart_puts_P("\n");
		*/
		
	}

		// only calibrate if total is less than 10% of baseline
		// this is an attempt to keep from calibrating when the sensor is seeing a "touched" signal
		
		if ( abs(total  - leastTotal) < (int)(.15 * (float)leastTotal) ) {

			leastTotal = 0x0FFFFFFFL;          // reset for "autocalibrate"
			
		}
	

	// routine to subtract baseline (non-sensed capacitance) from sensor return
	if (total < leastTotal) leastTotal = total;                 // set floor value to subtract from sensed value
	return(total/* - leastTotal*/);

}

//--------------------------functions end----------------------





int main(void)
{
	unsigned int c;
	unsigned int count = 0;

	softuart_init();
	//softuart_turn_rx_on(); /* redundant - on by default */
	
	sei();

	softuart_puts_P( "\r\nSoftuart Demo-Application\r\n" );    // "implicit" PSTR

	
	for (;;) {
		
		count++;
		
		if ( softuart_kbhit() ) {
			c = softuart_getchar();
			softuart_putchar( '[' );
			softuart_putchar( c );
			softuart_putchar( ']' );
		}
		
		
		if (count == 0xFFFF) {
		c = readCap(30);
		itoa(c, str, 10);
	
		softuart_putchar( '[' );
		softuart_puts(str);
		softuart_putchar( ']' );
		count = 0;
		}
		
	
		
	}
	
	return 0; /* never reached */
}