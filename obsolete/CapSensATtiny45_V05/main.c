#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	
	DDRB=_BV(PB0);		// Enable LED output pin
	sei();
	
	
	while(1) {
		if (GPIOR0 & _BV(0)) {PORTB ^= _BV(PB4);}
		if (tinytouch_sense()==tt_push) {PORTB ^= _BV(PB4);}      // Toggle LED on touch event
		
		_delay_ms(10);	
	}	
}
