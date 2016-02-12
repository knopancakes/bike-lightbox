#ifndef _USB_H
#define	_USB_H

#void uart_putchar(char c, FILE *stream);
#char uart_getchar(FILE *stream);

void usb_init(void);

#extern FILE uart_output;
#extern FILE uart_input;

#endif
