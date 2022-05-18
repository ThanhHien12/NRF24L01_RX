/*
 * main.c
 *
 *  Created on: Apr 29, 2022
 *      Author: Admin
 */
#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//	Set up UART for printf();
#ifndef BAUD
#define BAUD 9600
#endif
#include "STDIO_UART.h"

//	Include nRF24L01+ library
#include "nrf24l01.h"
#include "nrf24l01-mnemonics.h"
#include "spi.h"
#include "myLCD.h"
void print_config(void);

//	Used in IRQ ISR
volatile bool message_received = false;
volatile bool status = false;

int main(void) {
	//	Set cliche message to send (message cannot exceed 32 characters)

	char tx_message[32];			// Define string array
	strcpy(tx_message, "1");

	//	Initialize UART
	uart_init();

	//	Initialize nRF24L01+ and print configuration info
	nrf24_init();
	print_config();
	nrf24_start_listening();

	//	Start listening to incoming messages

	init_LCD();
	while (1) {

		if (message_received) {

			//	Message received, print it
			message_received = false;

			clr_LCD();
			move_LCD(1, 1);
			printf_LCD("Received message:");

			move_LCD(2, 1);
			printf_LCD("%d", nrf24_read_message());
			//	Send message as response
			_delay_ms(1000);

		}
		_delay_ms(1000);

		status = nrf24_send_message(tx_message);
		if (status == true) {
			clr_LCD();
			move_LCD(1, 1);			//Set up row and column off LCD
			printf_LCD("Message success");
			move_LCD(2, 1);
			printf_LCD("%d", nrf24_read_message());
		}
	}
}


//	Interrupt on IRQ pin
ISR(INT0_vect) {
message_received = true;
}

void print_config(void) {
uint8_t data;
printf("Startup successful\n\n nRF24L01+ configured as:\n");
printf("-------------------------------------------\n");
nrf24_read(CONFIG, &data, 1);
printf("CONFIG		0x%x\n", data);
nrf24_read(EN_AA, &data, 1);
printf("EN_AA			0x%x\n", data);
nrf24_read(EN_RXADDR, &data, 1);
printf("EN_RXADDR		0x%x\n", data);
nrf24_read(SETUP_RETR, &data, 1);
printf("SETUP_RETR		0x%x\n", data);
nrf24_read(RF_CH, &data, 1);
printf("RF_CH			0x%x\n", data);
nrf24_read(RF_SETUP, &data, 1);
printf("RF_SETUP		0x%x\n", data);
nrf24_read(STATUS, &data, 1);
printf("STATUS		0x%x\n", data);
nrf24_read(FEATURE, &data, 1);
printf("FEATURE		0x%x\n", data);
printf("-------------------------------------------\n\n");
}

