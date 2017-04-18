/*
 * interface/usbd_cdc.c
 */

#include "stdbool.h"
#include "driver/USBD/Class/CDC/Inc/usbd_cdc.h"
#include "driver/USBD/Class/CDC/usbd_cdc_interface.h"
#include "driver/USBD/Class/CDC/usbd_cdc_desc.h"
#include "driver/USBD/Core/Inc/usbd_core.h"
#include "driver/stm32f7xx_hal_tim.h"
#include "lib/buffers/ring_buff.h"
#include "usbd_cdc.h"

GI::Buff::RingBuff *usb_cdc_dev_rx_fifo;
GI::Buff::RingBuff *usb_cdc_dev_tx_fifo;

int put_receive_char_ptr = 0;

USBD_HandleTypeDef  usb_cdc_dev_param;

GI::Dev::UsbDCdc::UsbDCdc(unsigned int instance)
{
	memset(&usb_cdc_dev_param, 0, sizeof(USBD_HandleTypeDef));
	usb_cdc_dev_rx_fifo = new GI::Buff::RingBuff(APP_RX_DATA_SIZE);
	usb_cdc_dev_tx_fifo = new GI::Buff::RingBuff(APP_RX_DATA_SIZE);
	/* Init Device Library */
	USBD_Init(&usb_cdc_dev_param, &VCP_Desc, 0);

	/* Add Supported Class */
	USBD_RegisterClass(&usb_cdc_dev_param, USBD_CDC_CLASS);

	/* Add CDC Interface Class */
	USBD_CDC_RegisterInterface(&usb_cdc_dev_param, &USBD_CDC_fops);

	/* Start Device Process */
	USBD_Start(&usb_cdc_dev_param);
}

GI::Dev::UsbDCdc::~UsbDCdc()
{

}

unsigned int GI::Dev::UsbDCdc::rx(u8* buff)
{
	return usb_cdc_dev_rx_fifo->pop(buff);
}

unsigned int GI::Dev::UsbDCdc::tx(unsigned char* buff, unsigned int nbytes)
{
	unsigned int cnt = 0;
    USBD_CDC_HandleTypeDef   *hcdc = (USBD_CDC_HandleTypeDef*) usb_cdc_dev_param.pClassData;
	for(; cnt < nbytes; cnt++)
	{
		if(usb_cdc_dev_tx_fifo->push(buff[cnt]) == false && hcdc && hcdc->TxBuffer && hcdc->RxBuffer)
			break;
	}
	return cnt;
}

