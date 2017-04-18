/*
 * interface/spi.h
 */

#ifndef MCSPI_INTERFACE_H_
#define MCSPI_INTERFACE_H_

#include <stdlib.h>
#include <include/global.h>
/*#####################################################*/
#define SPI_INTERFACE_COUNT					6
#define SPI_CHANNELS_PER_INTERFACE_COUNT	4
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "mcspi_interface.c"
#endif

#endif /* MCSPI_INTERFACE_H_ */
