/*
 * io_handle.h
 *
 *  Created on: Dec 24, 2016
 *      Author: John Smith
 */

#ifndef LIB_API_IO_HANDLE_H_
#define LIB_API_IO_HANDLE_H_

#include <include/global.h>

typedef struct ioCtl_s{
	unsigned int slaveAddr;
	unsigned char *buff;
	unsigned int len;
	enum flags_e {
		FLAG_M_READ,/* Read bytes from the stream */
		FLAG_M_WRITE,/* Write bytes from the stream */
		FLAG_M_WRITE_READ,/* Write Read bytes from the stream */
	}flags;
}ioCtlMsg_t;

typedef struct ioCtlRwMsgs_s {
	int nMsgs;
	ioCtlMsg_t msgs[];
}ioCtlRwMsgs_t;


namespace GI
{
class IO
{
public:
	IO(char *path);
	~IO();
	int write(unsigned char *buff, unsigned int len);
	int read(unsigned char *buff, unsigned int len);

	enum ioCtl_e {
		IO_CTL_NONE,/* This is used for refresh calls*/
		IO_CTL_WR,/* Write Read data to/from the device */
		IO_CTL_R,/* Read data from device */
		IO_CTL_W,/* Write data to the device */
		IO_CTL_SET_SLAVE_ADDR,/* Set slave addr. */
		IO_CTL_GET_SLAVE_ADDR,/* Get slave addr. */
		IO_CTL_SET_MODE,/* Set mode bits. */
		IO_CTL_GET_MODE,/* Get mode bits. */
		IO_CTL_SET_SPEED, /* Set bus speed. */
		IO_CTL_GET_SPEED, /* Get bus speed. */
		IO_CTL_SET_WLEN,/* Set word length. */
		IO_CTL_GET_WLEN,/* Get word length. */
		IO_CTL_SET_SBITS,/* Set stop bits. */
		IO_CTL_GET_SBITS,/* Get stop bits. */
		IO_CTL_SET_PBITS,/* Set parity bits. */
		IO_CTL_GET_PBITS,/* Get parity bits. */
		IO_CTL_SET,/* Set word. */
		IO_CTL_GET,/* Get word. */
		IO_CTL_SET_BIT,/* Set bit. */
		IO_CTL_GET_BIT,/* Get bit. */
	}ioCtl;

	enum ioDevTypeMsg_e {
		IO_DEV_NULL,
		IO_DEV_GPIO,
		IO_DEV_UART,
		IO_DEV_I2C,
		IO_DEV_SPI,
		IO_DEV_MMCSD,
		IO_DEV_SCREEN,
		IO_DEV_RTC,
		IO_DEV_QSPI,
		IO_DEV_ADC,
		IO_DEV_ETH,
	}ioDevType;

	SysErr ctl(GI::IO::ioCtl_e cmd, unsigned long *data);

	void *devHandler;
	unsigned int slaveAddr;
};
}

#define newIo(name, path) GI::IO *name = new GI::IO::IO(path)
#define deleteIo(name) delete name

#endif /* LIB_API_IO_HANDLE_H_ */
