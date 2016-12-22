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
#include <api/i2c_def.h>
#include <interface/gpio.h>
#include <interface/i2c_def.h>
#include "stdlib.h"
//#include "stm32f7xx_conf.h"
#include "include/stm32f7xx.h"
#include "main.h"
#include "include/stm32f7xx.h"
#include "driver/stm32f7xx_hal.h"
#include "driver/stm32f7xx_hal_i2c.h"
#include "driver/stm32f7xx_hal_i2c_ex.h"
#include "driver/stm32f7xx_hal_rcc.h"
#include <include/global.h>
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
namespace GI
{
namespace Dev
{
class I2c
{
public:
	I2c(const char *path);
	~I2c();
	int writeRead(unsigned char addr, unsigned char *buffSend,
			unsigned int lenSend, unsigned char *buffReceive,
			unsigned int lenReceive);
	int readBytes(unsigned char addr, unsigned char *buff, unsigned int len);
	int writeBytes(unsigned char addr, unsigned char *buff, unsigned int len);

	unsigned char unitNr;
	volatile unsigned char flag;
	volatile unsigned char error_flag;
	volatile unsigned char result;
	volatile unsigned char status;
	unsigned int tCount;
	unsigned int rCount;
	unsigned int numOfBytes;
	unsigned int bytesToWrite;
	unsigned int bytesToRead;
	unsigned long BusyTimeOut;
	bool noSendWriteOnRead;
	CfgI2c cfg;
	void *udata;
	SysErr err;
private:
	unsigned long WR(unsigned char addr, unsigned char *buff_send,
			unsigned int TransmitBytes, unsigned char *buff_receive,
			unsigned int ReceiveBytes);
	void SoftwareReset(I2C_HandleTypeDef *hi2c);
};
}
}
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "twi_interface.c"
#endif
/*#####################################################*/
#endif /* TWI_INTERFACE_H_ */
/*#####################################################*/
