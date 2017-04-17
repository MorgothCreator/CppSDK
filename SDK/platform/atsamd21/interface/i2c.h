/*
 * i2c.h
 *
 *  Created on: Jan 3, 2017
 *      Author: John Smith
 */

#ifndef NXP_KEAZ_INTERFACE_I2C_H_
#define NXP_KEAZ_INTERFACE_I2C_H_

#include "sam.h"

#if defined(__AVR_ATxmega8E5__) || defined(__AVR_ATxmega16E5__) || defined(__AVR_ATxmega32E5__)
#define TWI_INTERFACE_COUNT		1
#else
#define TWI_INTERFACE_COUNT		4
#endif


#endif /* NXP_KEAZ_INTERFACE_I2C_H_ */
