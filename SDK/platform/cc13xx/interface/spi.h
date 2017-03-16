/*
 * mcspi_interface.h
 *
 * Created: 3/7/2013 6:40:42 PM
 *  Author: XxXx
 */

#ifndef MCSPI_INTERFACE_H_
#define MCSPI_INTERFACE_H_

#include <stdlib.h>
#include <api/gpio.h>
#include <include/global.h>
#include <board.h>

#define SPI_INTERFACE_COUNT					6
#define SPI_CHANNELS_PER_INTERFACE_COUNT	4

/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "mcspi_interface.c"
#endif

#endif /* MCSPI_INTERFACE_H_ */
