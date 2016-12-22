/*
 * mcspi_interface.h
 *
 * Created: 3/7/2013 6:40:42 PM
 *  Author: XxXx
 */

#ifndef MCSPI_INTERFACE_H_
#define MCSPI_INTERFACE_H_

#include <stdlib.h>
#include <api/gpio_def.h>
#include <api/spi_def.h>
#include <include/global.h>
#include <include/spidev.h>
#include <interface/gpio.h>
#include "include/stm32f7xx.h"
#include "main.h"
#include "driver/stm32f7xx_hal_conf.h"
#include "driver/stm32f7xx_hal_rcc_ex.h"
#include "driver/stm32f7xx_hal_rcc.h"
#include "driver/stm32f7xx_hal_spi.h"
#include "driver/stm32f7xx_hal_gpio.h"
#include "driver/stm32f7xx_hal_gpio_ex.h"
#include "board/board.h"
/*#####################################################*/
namespace GI
{
namespace Dev
{
class Spi
{
public:
	Spi(const char *path);
	~Spi();
	int ioctl(charDev *fp, int command, void *param);
	int assert();
	int deassert();
	int writeRead(unsigned char *buffWrite, unsigned char *buffRead,
			unsigned int len);
	int readBytes(unsigned char *buff, unsigned int len);
	int writeBytes(unsigned char *buff, unsigned int len);
	int _mcspi_set_baud(unsigned long baud);
	int writeReadByte(unsigned char *byte);
	int err;
	CfgSpi cfg;
private:
	unsigned char oldCsSelect;
	unsigned char unitNr;
	unsigned char channel;
	unsigned int speed;
	volatile bool useInterrupt;
	volatile unsigned char flag;
	volatile unsigned char Priority;
	volatile unsigned char Mode;
	volatile unsigned int BytesCnt;
	bool DisableCsHandle;
	bool cPol;bool cPha;
	bool lsbFirst;
	unsigned char wordSize;
	bool slave;
	void *userData;

};
}
}

#ifdef HEADER_INCLUDE_C_FILES
#include "mcspi_interface.c"
#endif

#endif /* MCSPI_INTERFACE_H_ */
