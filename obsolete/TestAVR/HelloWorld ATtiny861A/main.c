

#include <avr/io.h>          // (1)
 
int main (void) {            // (2)
 
   DDRB  = 0xFF;             // (3)
   PORTB = 0x04;             // (4)
 
   while(1) {                // (5)
     /* "leere" Schleife*/   // (6)
   }                         // (7)
 
   /* wird nie erreicht */
   return 0;                 // (8)
}