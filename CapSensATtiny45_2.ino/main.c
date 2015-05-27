
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

//define F_CPU
#ifndef F_CPU
#define F_CPU 8000000UL
#warning F_CPU
#endif

#include <util/delay.h>

//Include project files

#include "uart.h"

#define nop() __asm volatile ("nop")


#define LCD 0b01110100


int main(void)
{
    DDRB |= (1 << 1);
    uart_init();
    sei();
    for(;;)
    {
        uart_putc("H");
        _delay_ms(1000);
        PORTB ^= (1 << 1);
    }
}
