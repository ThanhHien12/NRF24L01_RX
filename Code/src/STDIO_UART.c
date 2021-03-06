/*
 * STDIO_UART.c
 *
 *  Created on: Apr 29, 2022
 *      Author: Admin
 */
#include <stdio.h>
#include <avr/io.h>
#include <avr/iom128.h>
#include "STDIO_UART.h"

#define MYUBRR (((F_CPU / (BAUD * 16UL))) - 1)

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
static FILE mystdin = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

void uart_init(void)
{
    UBRR0H = MYUBRR >> 8;
    UBRR0L = MYUBRR;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);

    stdout = &mystdout;
    stdin  = &mystdin;
}

// Redirect stdout to UART
int uart_putchar(char c, FILE *stream) {
	if (c == '\n') {
		uart_putchar('\r', stream);
	}
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	return 0;
}

// Redirect stdin to UART
int uart_getchar(FILE *stream) {
	loop_until_bit_is_set(UCSR0A, RXC0);
	return UDR0;
}

