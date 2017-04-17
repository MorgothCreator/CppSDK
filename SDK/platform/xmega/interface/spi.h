/*
 * spi.h
 *
 *  Created on: Jan 3, 2017
 *      Author: John Smith
 */

#ifndef NXP_KEAZ_INTERFACE_SPI_H_
#define NXP_KEAZ_INTERFACE_SPI_H_

#include <avr/io.h>

#if defined(__AVR_ATxmega8E5__) || defined(__AVR_ATxmega16E5__) || defined(__AVR_ATxmega32E5__)
#define SPI_INTERFACE_COUNT		1
#else
#define SPI_INTERFACE_COUNT		4
#endif

#define SPI_CHANNELS_PER_INTERFACE_COUNT	4



#endif /* NXP_KEAZ_INTERFACE_SPI_H_ */
