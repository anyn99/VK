#ifndef _UART_H_
#define _UART_H_

#define SUART_TXD
#define SUART_RXD

extern void uart_init();

#ifdef SUART_TXD
    extern void uart_putc (const char);
#endif // SUART_RXD

#ifdef SUART_RXD
    extern int uart_getc_wait ();
    extern int uart_getc_nowait();
#endif // SUART_RXD

#endif /* _UART_H_ */
