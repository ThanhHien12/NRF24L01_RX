/*
 * STDIO_UART.h
 *
 *  Created on: Apr 29, 2022
 *      Author: Admin
 */

#ifndef STDIO_UART_H_
#define STDIO_UART_H_

#define BAUD 115200

void uart_init(void);
int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);

#endif /* STDIO_UART_H_ */
