/*
 * gpio.h
 *
 *  Created on: Jan 3, 2017
 *      Author: John Smith
 */

#ifndef NXP_KEAZ_INTERFACE_GPIO_H_
#define NXP_KEAZ_INTERFACE_GPIO_H_

#include <avr/io.h>
#include <avr/pgmspace.h>

#define HAVE_GPIO_IN_FLOATING
#define HAVE_GPIO_IN_PULL_DOWN
#define HAVE_GPIO_IN_PULL_UP
#define HAVE_GPIO_OUT_PUSH_PULL
#define HAVE_GPIO_IN_PULL_UP

#define _GPIO_FAST_SET_PIN(base_addr, pin)\
		((PORT_t*)base_addr)->OUTSET = 1 << (pin)

#define _GPIO_FAST_CLEAR_PIN(base_addr, pin)\
		((PORT_t*)base_addr)->OUTCLR = 1 << (pin)

#define _GPIO_FAST_WRITE_MULTI_PIN(base_addr, mask, pin_mask)\
		((PORT_t*)base_addr)->OUT = (((GPIO_TypeDef*)base_addr)->OUT & ~mask) | ((pin_mask) & mask)

extern const unsigned char BIT_MASK_TABLE[] PROGMEM;
extern PORT_t *GPIO_BASE_PTRS[];

#endif /* NXP_KEAZ_INTERFACE_GPIO_H_ */
