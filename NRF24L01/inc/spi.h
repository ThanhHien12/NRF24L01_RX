/*
 * spi.h
 *
 *  Created on: Apr 29, 2022
 *      Author: Admin
 */

#ifndef SPI_H_
#define SPI_H_

#include <avr/common.h>

void spi_master_init( void );
void spi_bulk_send( uint8_t *send_buffer, uint8_t count );
void spi_send( uint8_t send_data );
void spi_bulk_exchange( uint8_t *send_buffer, uint8_t *receive_buffer, uint8_t count );
uint8_t spi_exchange( uint8_t send_data );



#endif /* SPI_H_ */
