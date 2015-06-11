/*
 v0.1	06/10/2013
 
 TinyTouchLibrary Example
 
 Hardware:
	- Attiny 13A/25/45/85
	- LED on PB3
	- Touchbutton on PB4
 */ 

 
#include <avr/io.h>
#include <util/delay.h>
#include "tinytouchlib.h"

int main(void)
{
	CLKPR=_BV(CLKPCE);
	CLKPR=0;			// set clock prescaler to 1 (attiny 25/45/85/24/44/84/13/13A)		

	DDRB=_BV(PB3);		// Enable LED output pin
	
	tinytouch_init();
	
	uint8_t state;
	
	while(1) {
		state = tinytouch_sense();
		if (state == tt_on) {PORTB |= _BV(PB3);}      // Toggle LED on touch event
		else PORTB &= ~_BV(PB3);
			
	}	
}

