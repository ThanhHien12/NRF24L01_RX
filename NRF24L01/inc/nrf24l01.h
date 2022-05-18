/*
 * nrf24l01.h
 *
 *  Created on: Apr 29, 2022
 *      Author: Admin
 */

#ifndef NRF24L01_H_
#define NRF24L01_H_

//	States
#define POWERUP		1
#define POWERDOWN	2
#define RECEIVE		3
#define TRANSMIT	4
#define STANDBY1	5
#define STANDBY2	6

//	Forward declarations
uint8_t nrf24_send_spi(uint8_t register_address, void *data, unsigned int bytes);
uint8_t nrf24_write(uint8_t register_address, uint8_t *data, unsigned int bytes);
uint8_t nrf24_read(uint8_t register_address, uint8_t *data, unsigned int bytes);
void nrf24_init(void);
void nrf24_state(uint8_t state);
void nrf24_start_listening(void);
unsigned int nrf24_available(void);
const char * nrf24_read_message(void);
uint8_t nrf24_send_message(const void *tx_message);

#endif /* NRF24L01_H_ */
