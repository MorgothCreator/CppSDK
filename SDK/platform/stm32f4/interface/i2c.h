/*
 * twi_interface.h
 *
 * Created: 2/8/2013 5:00:12 PM
 *  Iulian Gheorghiu <morgoth.creator@gmail.com>
 */
/*#####################################################*/
#ifndef TWI_INTERFACE_H_
#define TWI_INTERFACE_H_
/*#####################################################*/
#include <stdbool.h>
#include "stdlib.h"
#include "string.h"
#include <include/global.h>
#include <api/gpio.h>
//#include "stm32f7xx_conf.h"
/*#####################################################*/
#define TWI_INTERFACE_COUNT	6
/*#####################################################*/
/*! Transaction status defines. */
#define TWIM_STATUS_READY              0
#define TWIM_STATUS_BUSY               1
#define sEE_DIRECTION_TX                 0
#define sEE_DIRECTION_RX                 1
//#####################################################
#define sEE_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define sEE_LONG_TIMEOUT         ((uint32_t)(10 * sEE_FLAG_TIMEOUT))
//#####################################################
//#define TWIM_WRITE_BUFFER_SIZE         8
//#define TWIM_READ_BUFFER_SIZE          8

typedef enum TWIM_RESULT_enum
{
	TWIM_RESULT_UNKNOWN = (0x00 << 0),
	TWIM_RESULT_OK = (0x01 << 0),
	TWIM_RESULT_BUFFER_OVERFLOW = (0x02 << 0),
	TWIM_RESULT_ARBITRATION_LOST = (0x03 << 0),
	TWIM_RESULT_BUS_ERROR = (0x04 << 0),
	TWIM_RESULT_NACK_RECEIVED = (0x05 << 0),
	TWIM_RESULT_FAIL = (0x06 << 0),
} TWIM_RESULT_t;
/*#####################################################*/
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "twi_interface.c"
#endif
/*#####################################################*/
#endif /* TWI_INTERFACE_H_ */
/*#####################################################*/
