/*
 * mcspi_interface.c
 *
 * Created: 3/7/2013 6:41:33 PM
 *  Author: XxXx
 */

//#include "driver/spi.h"
extern unsigned long CoreFreq;

#include "inc/hw_types.h"
#include "driver/ssi.h"
#include <interface/spi.h>
#include "driver/ioc.h"
#include "api/init.h"

#if (USE_DRIVER_SEMAPHORE == true)
volatile bool spi_semaphore[SPI_INTERFACE_COUNT];
#endif

extern CfgSpi spiCfg[];

/**
 * @brief SPI MSP Initialization
 *        This function configures the hardware resources used in this example:
 *           - Peripheral's clock enable
 *           - Peripheral's GPIO Configuration
 *           - DMA configuration for transmission request by peripheral
 *           - NVIC configuration for DMA interrupt request enable
 * @param hspi: SPI handle pointer
 * @retval None
 */
GI::Dev::Spi::Spi(const char *path)
{
	memset(this, 0, sizeof(*this));
	unsigned int item_nr = 0;
	while(1)
	{
		if(spiCfg[item_nr].name == NULL)
		{
			err = SYS_ERR_INVALID_PATH;
			return;
		}
		if(!strcmp(spiCfg[item_nr].name, path))
			break;
		item_nr++;
	}

	if(strncmp(path, (char *)"spi-", sizeof("spi-") - 1))
	{
		err = SYS_ERR_INVALID_PATH;
		return;
	}
	unsigned int _portNr = (unsigned int)-1;
	unsigned int _channel = (unsigned int)-1;
	char *port = strchr(path, '-');
	char *chan = strchr(path, '.');
	if(!port || !chan)
	{
		err = SYS_ERR_INVALID_PATH;
		return;
	}
	int _nr_of_items = sscanf(port + 1, (const char *)"%u", &_portNr);
	_nr_of_items += sscanf(chan + 1, (const char *)"%u", &_channel);
	if (_nr_of_items != 2 || _portNr >= SPI_INTERFACE_COUNT || _channel >= SPI_CHANNELS_PER_INTERFACE_COUNT)
	{
		err = SYS_ERR_INVALID_PATH;
		return;
	}

	memset(this, 0, sizeof(*this));
	unitNr = _portNr;
	channel = _channel;
	memcpy(&cfg, &spiCfg[item_nr], sizeof(CfgSpi));

	switch(_portNr)
	{
	case 0:
		SSIDisable(SSI0_BASE);
		IOCPortConfigureSet((cfg.sck % 32), IOC_PORT_MCU_SSI0_CLK, IOC_CURRENT_8MA | IOC_STRENGTH_MAX | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= (1 << (cfg.sck % 32));
		IOCPortConfigureSet((cfg.miSo % 32), IOC_PORT_MCU_SSI0_RX, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (cfg.miSo % 32));
		IOCPortConfigureSet((cfg.moSi % 32), IOC_PORT_MCU_SSI0_TX, IOC_CURRENT_8MA | IOC_STRENGTH_MAX | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= (1 << (cfg.moSi % 32));
		IOCPortConfigureSet((cfg.cs % 32), IOC_PORT_MCU_SSI0_FSS, IOC_CURRENT_8MA | IOC_STRENGTH_MAX | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= (1 << (cfg.cs % 32));
		SSIConfigSetExpClk(SSI0_BASE, CoreFreq, SSI_FRF_MOTO_MODE_3, cfg.spiMode, cfg.speed, 8);
		userData = (void *)SSI0_BASE;
		break;
	case 1:
		SSIDisable(SSI1_BASE);
		IOCPortConfigureSet((cfg.sck % 32), IOC_PORT_MCU_SSI1_CLK, IOC_CURRENT_8MA | IOC_STRENGTH_MAX | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= (1 << (cfg.sck % 32));
		IOCPortConfigureSet((cfg.miSo % 32), IOC_PORT_MCU_SSI1_RX, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (cfg.miSo % 32));
		IOCPortConfigureSet((cfg.moSi % 32), IOC_PORT_MCU_SSI1_TX, IOC_CURRENT_8MA | IOC_STRENGTH_MAX | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= (1 << (cfg.moSi % 32));
		IOCPortConfigureSet((cfg.cs % 32), IOC_PORT_MCU_SSI1_FSS, IOC_CURRENT_8MA | IOC_STRENGTH_MAX | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= (1 << (cfg.cs % 32));
		SSIConfigSetExpClk(SSI1_BASE, CoreFreq, SSI_FRF_MOTO_MODE_3, cfg.spiMode, cfg.speed, 8);
		userData = (void *)SSI1_BASE;
		break;
	default:
		return;
	}
	SSIEnable((unsigned int)userData);
}

/**
 * @brief SPI MSP De-Initialization
 *        This function frees the hardware resources used in this example:
 *          - Disable the Peripheral's clock
 *          - Revert GPIO, DMA and NVIC configuration to their default state
 * @param hspi: SPI handle pointer
 * @retval None
 */
GI::Dev::Spi::~Spi()
{
	if (!this)
	{
		err = SYS_ERR_INVALID_HANDLER;
		return;
	}
	SSIDisable((unsigned int)userData);
	switch(unitNr)
	{
	case 0:
		IOCPortConfigureSet((cfg.sck % 32), IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (cfg.sck % 32));
		IOCPortConfigureSet((cfg.miSo % 32), IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (cfg.miSo % 32));
		IOCPortConfigureSet((cfg.moSi % 32), IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (cfg.moSi % 32));
		IOCPortConfigureSet((cfg.cs % 32), IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (cfg.cs % 32));
		SSIConfigSetExpClk(SSI0_BASE, CoreFreq, SSI_FRF_MOTO_MODE_0, cfg.spiMode, cfg.speed, 8);
		break;
	case 1:
		IOCPortConfigureSet((cfg.sck % 32), IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (cfg.sck % 32));
		IOCPortConfigureSet((cfg.miSo % 32), IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (cfg.miSo % 32));
		IOCPortConfigureSet((cfg.moSi % 32), IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (cfg.moSi % 32));
		IOCPortConfigureSet((cfg.cs % 32), IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (cfg.cs % 32));
		SSIConfigSetExpClk(SSI1_BASE, CoreFreq, SSI_FRF_MOTO_MODE_0, cfg.spiMode, cfg.speed, 8);
		break;
	}
}
/*#####################################################*/
/*#####################################################*/
int GI::Dev::Spi::assert()
{
	if (!this)
	{
		err = SYS_ERR_INVALID_HANDLER;
		return SYS_ERR_INVALID_HANDLER;
	}
#if (USE_DRIVER_SEMAPHORE == true)
	while (spi_semaphore[property.unitNr])
		;
#endif
	setSpeed(speed);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[property.unitNr] = true;
#endif
	HWREG(GPIO_BASE + GPIO_O_DOUTCLR31_0) = 1 << (cfg.cs % 32);
	err = SYS_ERR_OK;
	return SYS_ERR_OK;
}
/*#####################################################*/
int GI::Dev::Spi::ioctl(charDev *fp, int command, void *param)
{
	err = SYS_ERR_OK;
	return SYS_ERR_OK;
}
/*#####################################################*/
int GI::Dev::Spi::deassert()
{
	if (!this)
	{
		err = SYS_ERR_INVALID_HANDLER;
		return SYS_ERR_INVALID_HANDLER;
	}
	HWREG(GPIO_BASE + GPIO_O_DOUTSET31_0) = 1 << (cfg.cs % 32);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[property.unitNr] = false;
#endif
	err = SYS_ERR_OK;
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Spi::writeRead(unsigned char *buffWrite, unsigned int lenWrite,
		unsigned char *buffRead, unsigned int lenRead)
{
	if (!this)
	{
		err = SYS_ERR_INVALID_HANDLER;
		return SYS_ERR_INVALID_HANDLER;
	}
#if (USE_DRIVER_SEMAPHORE == true)
	if (!spi_semaphore[unitNr])
		return SYS_ERR_BUSY;
#endif
	if (!DisableCsHandle)
		HWREG(GPIO_BASE + GPIO_O_DOUTCLR31_0) = 1 << (cfg.cs % 32);
	SysErr status = SYS_ERR_OK;
	unsigned int transfer_cnt = 0;
	for(; transfer_cnt < lenWrite; transfer_cnt++) {
		SSIDataPut((unsigned int)userData, buffWrite[transfer_cnt]);
	}
	transfer_cnt = 0;
	for(; transfer_cnt < lenRead; transfer_cnt++) {
		SSIDataPut((unsigned int)userData, 0xFF);
		uint32_t pui32Data;
		SSIDataGet((unsigned int)userData, &pui32Data);
		buffRead[transfer_cnt] = pui32Data;
	}

	if (!DisableCsHandle)
		HWREG(GPIO_BASE + GPIO_O_DOUTSET31_0) = 1 << (cfg.cs % 32);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[unitNr] = false;
#endif
	return status;
}
/*#####################################################*/
int GI::Dev::Spi::readBytes(unsigned char *buff, unsigned int len)
{
	if (!this)
	{
		err = SYS_ERR_INVALID_HANDLER;
		return SYS_ERR_INVALID_HANDLER;
	}
#if (USE_DRIVER_SEMAPHORE == true)
	if (!spi_semaphore[unitNr])
		return SYS_ERR_BUSY;
#endif
	if (!DisableCsHandle)
		HWREG(GPIO_BASE + GPIO_O_DOUTCLR31_0) = 1 << (cfg.cs % 32);
	SysErr status = SYS_ERR_OK;
	unsigned int transfer_cnt = 0;
	for(; transfer_cnt < len; transfer_cnt++) {
		SSIDataPut((unsigned int)userData, 0xFF);
		uint32_t pui32Data;
		SSIDataGet((unsigned int)userData, &pui32Data);
		buff[transfer_cnt] = pui32Data;
	}
	if (!DisableCsHandle)
		HWREG(GPIO_BASE + GPIO_O_DOUTSET31_0) = 1 << (cfg.cs % 32);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[unitNr] = false;
#endif
	if(status == SYS_ERR_OK)
		return len;
	return status;
}
/*#####################################################*/
int GI::Dev::Spi::writeBytes(unsigned char *buff, unsigned int len)
{
	if (!this)
	{
		err = SYS_ERR_INVALID_HANDLER;
		return SYS_ERR_INVALID_HANDLER;
	}
#if (USE_DRIVER_SEMAPHORE == true)
	if (!spi_semaphore[unitNr])
		return SYS_ERR_BUSY;
#endif
	if (!DisableCsHandle)
		HWREG(GPIO_BASE + GPIO_O_DOUTCLR31_0) = 1 << (cfg.cs % 32);
	SysErr status = SYS_ERR_OK;
	unsigned int transfer_cnt = 0;
	for(; transfer_cnt < len; transfer_cnt++) {
		SSIDataPut((unsigned int)userData, buff[transfer_cnt]);
	}

	if (!DisableCsHandle)
		HWREG(GPIO_BASE + GPIO_O_DOUTSET31_0) = 1 << (cfg.cs % 32);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[unitNr] = false;
#endif
	if(status == SYS_ERR_OK)
		return len;
	return status;
}
/*#####################################################*/
SysErr GI::Dev::Spi::writeReadByte(unsigned char *byte)
{
	if (!this)
	{
		err = SYS_ERR_INVALID_HANDLER;
		return SYS_ERR_INVALID_HANDLER;
	}
	SSIDataPut((unsigned int)userData, *byte);
	uint32_t pui32Data;
	SSIDataGet((unsigned int)userData, &pui32Data);
	*byte = pui32Data;
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Spi::setSpeed(unsigned long baud)
{
	if (!this)
	{
		err = SYS_ERR_INVALID_HANDLER;
		return SYS_ERR_INVALID_HANDLER;
	}
#if (USE_DRIVER_SEMAPHORE == true)
	while (spi_semaphore[unitNr])
		;
#endif
	SSIDisable((unsigned int)userData);
	SSIConfigSetExpClk((unsigned int)userData, CoreFreq, SSI_FRF_MOTO_MODE_0, cfg.spiMode, cfg.speed, 8);
	SSIEnable((unsigned int)userData);
	err = SYS_ERR_OK;
	return SYS_ERR_OK;
}
/*#####################################################*/
