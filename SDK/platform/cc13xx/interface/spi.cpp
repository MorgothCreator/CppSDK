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
GI::Dev::Spi::Spi(ioSettings *cfg)
{
    memset(this, 0, sizeof(*this));
    if(cfg->info.ioType != ioSettings::info_s::ioType_SPI)
        return;

	if(strncmp(cfg->info.name, (char *)"spi-", sizeof("spi-") - 1))
	{
		err = SYS_ERR_INVALID_PATH;
		return;
	}
	unsigned int _portNr = (unsigned int)-1;
	unsigned int _channel = (unsigned int)-1;
	char *port = strchr(cfg->info.name, '-');
	char *chan = strchr(cfg->info.name, '.');
	if(!port || !chan)
	{
		err = SYS_ERR_INVALID_PATH;
		return;
	}
	//int _nr_of_items = sscanf(port + 1, (const char *)"%u", &_portNr);
	//_nr_of_items += sscanf(chan + 1, (const char *)"%u", &_channel);
    _portNr = port[1] - '0';
    _channel = chan[1] - '0';
	if (/*_nr_of_items != 2 || */_portNr >= SPI_INTERFACE_COUNT || _channel >= SPI_CHANNELS_PER_INTERFACE_COUNT)
	{
		err = SYS_ERR_INVALID_PATH;
		return;
	}

	unitNr = _portNr;
	channel = _channel;
    this->cfg = cfg;
    CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;

	switch(_portNr)
	{
	case 0:
		SSIDisable(SSI0_BASE);
		IOCPortConfigureSet((int_cfg->sck % 32), IOC_PORT_MCU_SSI0_CLK, IOC_CURRENT_8MA | IOC_STRENGTH_MAX | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= (1 << (int_cfg->sck % 32));
		IOCPortConfigureSet((int_cfg->miSo % 32), IOC_PORT_MCU_SSI0_RX, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (int_cfg->miSo % 32));
		IOCPortConfigureSet((int_cfg->moSi % 32), IOC_PORT_MCU_SSI0_TX, IOC_CURRENT_8MA | IOC_STRENGTH_MAX | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= (1 << (int_cfg->moSi % 32));
		IOCPortConfigureSet((int_cfg->cs % 32), IOC_PORT_MCU_SSI0_FSS, IOC_CURRENT_8MA | IOC_STRENGTH_MAX | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= (1 << (int_cfg->cs % 32));
		SSIConfigSetExpClk(SSI0_BASE, CoreFreq, SSI_FRF_MOTO_MODE_3, int_cfg->spiMode, int_cfg->speed, 8);
		userData = (void *)SSI0_BASE;
		break;
	case 1:
		SSIDisable(SSI1_BASE);
		IOCPortConfigureSet((int_cfg->sck % 32), IOC_PORT_MCU_SSI1_CLK, IOC_CURRENT_8MA | IOC_STRENGTH_MAX | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= (1 << (int_cfg->sck % 32));
		IOCPortConfigureSet((int_cfg->miSo % 32), IOC_PORT_MCU_SSI1_RX, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (int_cfg->miSo % 32));
		IOCPortConfigureSet((int_cfg->moSi % 32), IOC_PORT_MCU_SSI1_TX, IOC_CURRENT_8MA | IOC_STRENGTH_MAX | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= (1 << (int_cfg->moSi % 32));
		IOCPortConfigureSet((int_cfg->cs % 32), IOC_PORT_MCU_SSI1_FSS, IOC_CURRENT_8MA | IOC_STRENGTH_MAX | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= (1 << (int_cfg->cs % 32));
		SSIConfigSetExpClk(SSI1_BASE, CoreFreq, SSI_FRF_MOTO_MODE_3, int_cfg->spiMode, int_cfg->speed, 8);
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
    CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	switch(unitNr)
	{
	case 0:
		IOCPortConfigureSet((int_cfg->sck % 32), IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (int_cfg->sck % 32));
		IOCPortConfigureSet((int_cfg->miSo % 32), IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (int_cfg->miSo % 32));
		IOCPortConfigureSet((int_cfg->moSi % 32), IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (int_cfg->moSi % 32));
		IOCPortConfigureSet((int_cfg->cs % 32), IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (int_cfg->cs % 32));
		SSIConfigSetExpClk(SSI0_BASE, CoreFreq, SSI_FRF_MOTO_MODE_0, int_cfg->spiMode, int_cfg->speed, 8);
		break;
	case 1:
		IOCPortConfigureSet((int_cfg->sck % 32), IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (int_cfg->sck % 32));
		IOCPortConfigureSet((int_cfg->miSo % 32), IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (int_cfg->miSo % 32));
		IOCPortConfigureSet((int_cfg->moSi % 32), IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (int_cfg->moSi % 32));
		IOCPortConfigureSet((int_cfg->cs % 32), IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (int_cfg->cs % 32));
		SSIConfigSetExpClk(SSI1_BASE, CoreFreq, SSI_FRF_MOTO_MODE_0, int_cfg->spiMode, int_cfg->speed, 8);
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
	while (spi_semaphore[unitNr])
		;
#endif
	setSpeed(speed);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[unitNr] = true;
#endif
    CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	HWREG(GPIO_BASE + GPIO_O_DOUTCLR31_0) = 1 << (int_cfg->cs % 32);
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
    CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	HWREG(GPIO_BASE + GPIO_O_DOUTSET31_0) = 1 << (int_cfg->cs % 32);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[unitNr] = false;
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
	if (spi_semaphore[unitNr])
		return SYS_ERR_BUSY;
    spi_semaphore[unitNr] = true;
#endif
    CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	if (!disableCsHandle)
		HWREG(GPIO_BASE + GPIO_O_DOUTCLR31_0) = 1 << (int_cfg->cs % 32);
	SysErr status = SYS_ERR_OK;
	unsigned int transfer_cnt = 0;
	for(; transfer_cnt < lenWrite; transfer_cnt++) {
		SSIDataPut((unsigned int)userData, buffWrite[transfer_cnt]);
        while(SSIBusy((unsigned int)userData));
        unsigned long ui32Dummy;
        while(SSIDataGetNonBlocking((unsigned int)userData, &ui32Dummy));
	}
	transfer_cnt = 0;
	for(; transfer_cnt < lenRead; transfer_cnt++) {
		SSIDataPut((unsigned int)userData, 0xFF);
		uint32_t pui32Data;
        while(SSIBusy((unsigned int)userData));
		SSIDataGet((unsigned int)userData, &pui32Data);
		buffRead[transfer_cnt] = pui32Data;
	}

	if (!disableCsHandle)
		HWREG(GPIO_BASE + GPIO_O_DOUTSET31_0) = 1 << (int_cfg->cs % 32);
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
	if (spi_semaphore[unitNr])
		return SYS_ERR_BUSY;
    spi_semaphore[unitNr] = true;
#endif
    CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	if (!disableCsHandle)
		HWREG(GPIO_BASE + GPIO_O_DOUTCLR31_0) = 1 << (int_cfg->cs % 32);
	SysErr status = SYS_ERR_OK;
	unsigned int transfer_cnt = 0;
	for(; transfer_cnt < len; transfer_cnt++) {
		SSIDataPut((unsigned int)userData, 0xFF);
        while(SSIBusy((unsigned int)userData));
		uint32_t pui32Data;
		SSIDataGet((unsigned int)userData, &pui32Data);
		buff[transfer_cnt] = pui32Data;
	}
	if (!disableCsHandle)
		HWREG(GPIO_BASE + GPIO_O_DOUTSET31_0) = 1 << (int_cfg->cs % 32);
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
	if (spi_semaphore[unitNr])
		return SYS_ERR_BUSY;
    spi_semaphore[unitNr] = true;
#endif
    CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	if (!disableCsHandle)
		HWREG(GPIO_BASE + GPIO_O_DOUTCLR31_0) = 1 << (int_cfg->cs % 32);
	SysErr status = SYS_ERR_OK;
	unsigned int transfer_cnt = 0;
	for(; transfer_cnt < len; transfer_cnt++) {
		SSIDataPut((unsigned int)userData, buff[transfer_cnt]);
	    while(SSIBusy((unsigned int)userData));
	    unsigned long ui32Dummy;
	    while(SSIDataGetNonBlocking((unsigned int)userData, &ui32Dummy));
	}

	if (!disableCsHandle)
		HWREG(GPIO_BASE + GPIO_O_DOUTSET31_0) = 1 << (int_cfg->cs % 32);
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
    CfgSpi *int_cfg = (CfgSpi *)cfg->cfg;
	SSIDisable((unsigned int)userData);
	SSIConfigSetExpClk((unsigned int)userData, CoreFreq, SSI_FRF_MOTO_MODE_0, int_cfg->spiMode, int_cfg->speed, 8);
	SSIEnable((unsigned int)userData);
	err = SYS_ERR_OK;
	return SYS_ERR_OK;
}
/*#####################################################*/
