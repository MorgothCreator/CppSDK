/*
 * usb_dev_com_interface.c
 *
 *  Created on: Oct 17, 2015
 *      Author: John Smith
 */

#include "stdbool.h"
#include "usbd_cdc.h"
GI::Dev::UsbDCdc::UsbDCdc(unsigned int instance)
{

}

GI::Dev::UsbDCdc::~UsbDCdc()
{

}

unsigned int GI::Dev::UsbDCdc::rx(unsigned char* buff)
{
	return 0;
}

unsigned int GI::Dev::UsbDCdc::tx(unsigned char* buff, unsigned int nbytes)
{
		return nbytes;
}

