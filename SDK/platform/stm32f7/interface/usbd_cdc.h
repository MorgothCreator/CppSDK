/*
 * usb_dev_com_interface.h
 *
 *  Created on: Nov 23, 2014
 *      Author: Morgoth
 */

#ifndef USB_DEV_COM_INTERFACE_H_
#define USB_DEV_COM_INTERFACE_H_

namespace GI
{
namespace Dev
{
class UsbDCdc
{
public:
	UsbDCdc(unsigned int instance);
	~UsbDCdc();
	unsigned int rx(unsigned char* buff);
	unsigned int tx(unsigned char* buff, unsigned int nbytes);
};
}
}



#endif /* USB_DEV_COM_INTERFACE_H_ */
