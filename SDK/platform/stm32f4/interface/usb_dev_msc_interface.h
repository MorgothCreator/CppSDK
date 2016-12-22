/*
 * usb_dev_msc_interface.h
 *
 *  Created on: Oct 18, 2015
 *      Author: Iulian Gheorghiu
 */

#ifndef USB_DEV_MSC_INTERFACE_H_
#define USB_DEV_MSC_INTERFACE_H_

#include <stdbool.h>
#include "lib/fs/fat/inc/diskio.h"

#define GET_SECTOR_COUNT 1
#define GET_SECTOR_SIZE 2

extern DRV_RW_FUNC usbd_drv_func[];

bool _usb_msc_dev_media_connected(unsigned int instance);
void _usb_msc_dev_media_change_state(unsigned int instance, bool media_is_present);
void _usb_msc_dev_init(unsigned int instance, void *slave_controls);


#endif /* USB_DEV_MSC_INTERFACE_H_ */
