#ifndef UART_CONTROL_H
#define UART_CONTROL_H

#include <MKL46Z4.h>

/* UART communication functions */
void init_uart(void);
void uart_putc(char ch);
void uart_puts(char *ptr_str);
void short_delay(void);

#endif /* UART_CONTROL_H */
