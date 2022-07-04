/*
 * mainRX.c
 *
 *  Created on: May 11, 2022
 *      Author: Admin
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <avr/iom128.h>
#include "STDIO_UART.h"
/* Set up UART for printf() */

#include "STDIO_UART.h"

/*	Include nRF24L01+ library */
#include "nrf24l01.h"
#include "nrf24l01-mnemonics.h"
#include "spi.h"
#include "myLCD.h"

// Define baud rate
#define USART0_BAUD         115200ul
#define USART0_UBBR_VALUE   ((F_CPU/(USART0_BAUD<<4))-1)

// Output Port pin LED_O
#define PORT_LED_O      PORTB
#define DDR_LED_O       DDRB
#define BIT_LED_O       6

//#define PORT_LED1_O      PORTC
//#define DDR_LED1_O       DDRC
//#define BIT_LED1_O       0

//#define PORT_LED2_O      PORTC
//#define DDR_LED2_O       DDRC
//#define BIT_LED2_O       1

// Output ENA
#define PORT_MOTOR_ENA      PORTE
#define DDR_MOTOR_ENA       DDRE
#define BIT_MOTOR_ENA        4
// Output motor
#define PORT_MOTOR_1      PORTE
#define DDR_MOTOR_1       DDRE
#define BIT_MOTOR_1         5

#define PORT_MOTOR_2      PORTE
#define DDR_MOTOR_2       DDRE
#define BIT_MOTOR_2         6
// Output Port pin LED_O
#define PORT_LED_O         PORTB
#define DDR_LED_O          DDRB
#define BIT_LED_O          6

// Define frequency
#define PWM_MAX_DUTY_CYCLE 0x3FF

void PWM_vInit(void) {
	/*
	 Start Timer 1 with clock prescaler CLK/8 and phase correct
	 10-bit PWM mode. Output on PB6 (OC1B). Resolution is 1.09 us.
	 Frequency is 450 Hz.
	 */

	TCCR3A = (0 << COM3A1) | (0 << COM3A0) | (1 << COM3B1) | (0 << COM3B0)
				| (0 << COM3C1) | (0 << COM3C0) | (1 << WGM31) | (1 << WGM30);

	TCCR3B = (0 << ICNC3) | (0 << ICES3) | (0 << WGM33) | (0 << WGM32)
				| (0 << CS32) | (1 << CS31) | (0 << CS30);

	// Reset counter
	TCNT3 = 0;//


	// Set duty cycle to 0%

	OCR3B = 0;

}

void PWM_vSetDutyCycle(uint16_t u16DutyCycle) {
	// Clip parameter to maximum value
	if (u16DutyCycle > PWM_MAX_DUTY_CYCLE) {
		u16DutyCycle = PWM_MAX_DUTY_CYCLE;
	}
	OCR3B = u16DutyCycle;
}

void thuan(){
	PORT_MOTOR_1 |= (1 << BIT_MOTOR_1);
	PORT_MOTOR_2 &=~ (1 << BIT_MOTOR_2);
}

void nghich()
{
	PORT_MOTOR_2 |= (1 << BIT_MOTOR_2);
	PORT_MOTOR_1 &=~ (1 << BIT_MOTOR_1);
}

//void print_config(void);
//void init_IO()
//{
//    DDR_LED_O |= (1<<BIT_LED_O);
//    DDR_MOTOR |= (1 << BIT_MOTOR);
//    DDR_MOTOR_1 |= (1 << BIT_MOTOR_1);
//    DDR_MOTOR_2 |= (1 << BIT_MOTOR_2);
//}

//	Used in IRQ ISR
volatile bool message_received = false;
volatile bool status = false;
//uint8_t rx_message[32];

void USART0_vInit(void)
{
    // Set baud rate
    UBRR0H = (uint8_t)(USART0_UBBR_VALUE>>8);
    UBRR0L = (uint8_t)USART0_UBBR_VALUE;

    // Set frame format to 8 data bits, no parity, 1 stop bit
    UCSR0C = (0<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);

    // Enable receiver and transmitter
    UCSR0B = (1<<RXEN)|(1<<TXEN);
}

uint8_t USART0_vReceiveByte()
{
    // Wait until a byte has been received
    while((UCSR0A&(1<<RXC0)) == 0)
    {
        ;
    }

    // Return received data
    return UDR0;
}


int main(void) {
	/* Init GPIO and LCD */
	init_IO();
	init_LCD();
	PWM_vInit();

	/*	Initialize UART */
	uart_init();

//	clr_LCD();
//	move_LCD(1, 1);
	printf_LCD("Start RX program");
	printf("\nStart RX program\n");

	/*	Initialize nRF24L01+ and print configuration info */
	nrf24_init();
	print_config();

	/*	Start listening to incoming messages */
	nrf24_start_listening();

	char rx_message[32];
	char str[15];
	memcpy(str,"a",1);
	int ret;

	while (1) {
		if (message_received) {
			message_received = 0;
			memcpy(rx_message,nrf24_read_message(),1);
			printf("received message: %s\n", rx_message);
			ret = memcmp(rx_message,str, 1);
			if(ret == 0)
			{
				for(int i = 0; i < 1024; i+=20)
				{
					printf("Motor");
					thuan();
					PWM_vSetDutyCycle(i);
					_delay_ms(500);

				}
			}


// PWM Motor
//			PORT_LED_O |= (1<<BIT_LED_O);// Led On

			move_LCD(1, 1); //Set up row and column off LCD
			printf_LCD("PWM: 1000 thuan" );// Print LCD
			_delay_ms(150);



//			clr_LCD();
//			move_LCD(2, 1);
//			printf_LCD("Start TX program");
		}
	}

}

/*	Interrupt handler function on IRQ pin */
ISR(INT0_vect) {
	message_received = 1;
	PORT_LED_O ^= (1 << BIT_LED_O);
}

void init_IO(void) {
	DDR_LED_O |= (1 << BIT_LED_O);
	DDR_MOTOR_ENA |= (1 << BIT_MOTOR_ENA);
	DDR_MOTOR_1 |= (1 << BIT_MOTOR_1);
	DDR_MOTOR_2 |= (1 << BIT_MOTOR_2);

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

