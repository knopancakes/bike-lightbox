#ifndef _UART_H
#define	_UART_H

#define F_CPU 16000000UL
#define BAUD 9600

void uart_putchar(char c, FILE *stream);
char uart_getchar(FILE *stream);

void uart_init(void);

extern FILE uart_output;
extern FILE uart_input;

#endif
