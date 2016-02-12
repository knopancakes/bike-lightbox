#include <avr/io.h>
#include <stdio.h>
#include "uart.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef BAUD
#define BAUD 9600
#endif
#include <util/setbaud.h>

FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);


void uart_init(void) {
    UBRR1H = UBRRH_VALUE;
    UBRR1L = UBRRL_VALUE;

#if USE_2X
    UCSR1A |= _BV(U2X1);
#else
    UCSR1A &= ~(_BV(U2X1));
#endif

    UCSR1C = _BV(UCSZ11) | _BV(UCSZ10); /* 8-bit data */
    UCSR1B = _BV(RXEN1) | _BV(TXEN1);   /* Enable RX and TX */
    PORTD |= _BV(1) | _BV(2);
}

void uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        uart_putchar('\r', stream);
    }
    loop_until_bit_is_set(UCSR1A, UDRE1);
    UDR1 = c;
}

char uart_getchar(FILE *stream) {
    loop_until_bit_is_set(UCSR1A, RXC1);
    return UDR1;
}
