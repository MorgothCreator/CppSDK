/*
 *  lib/api/gpio_def.h
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GPIO_DEF_H_
#define GPIO_DEF_H_
/*#####################################################*/
#include <stdlib.h>
#include <stdbool.h>
#include <interface/gpio.h>
#include <include/global.h>
/*#####################################################*/
//#define GPIO_DIR_OUTPUT						0
//#define GPIO_DIR_INPUT						1
/*#####################################################*/
typedef enum
{
	IOA = 0x0u,
	IOB,
	IOC,
	IOD,
	IOE,
	IOF,
	IOG,
	IOH,
	IOI,
	IOJ,
	IOK,
	IOL,
	IOM,
	ION,
	IOO,
	IOP,
	IOQ,
	IOR,
	IOS,
	IOT,
	IOU,
	IOV,
	IOW,
	IOX,
	IOY,
	IOZ
} gpio_port_enum;
/*#####################################################*/
#define pin_mux_convert_to_pin(port, pin) ((port<<5) + (pin) % 32)

#define pin_mux_convert_to_port_pin(pin) (pin>>5), ((pin %32) - ((pin>>5)<<5))
/*#####################################################*/
typedef struct
{
	char name[9];
	unsigned int pin;
	bool reverse;
	enum gpioMode{
#ifdef HAVE_GPIO_OUT_PUSH_PULL
	GPIO_OUT_PUSH_PULL =0,
#endif
#ifdef HAVE_GPIO_IN_FLOATING
	GPIO_IN_FLOATING,
#endif
#ifdef HAVE_GPIO_AIN
	GPIO_AIN,
#endif
#ifdef HAVE_GPIO_IN_PULL_DOWN
	GPIO_IN_PULL_DOWN,
#endif
#ifdef HAVE_GPIO_IN_PULL_UP
	GPIO_IN_PULL_UP,
#endif
#ifdef HAVE_GPIO_OUT_OPEN_DRAIN
	GPIO_OUT_OPEN_DRAIN,
#endif
#ifdef HAVE_GPIO_ALTERNATIVE_OPEN_DRINE
	GPIO_ALTERNATIVE_OPEN_DRINE,
#endif
#ifdef HAVE_GPIO_ALTERNATIVE_PUSH_PULL
	GPIO_ALTERNATIVE_PUSH_PULL,
#endif
	}mode;
	bool multiPin;
} CfgGpio;
/*#####################################################*/
namespace GI
{
namespace Dev
{
class Gpio
{
public:
	Gpio(unsigned int pin, CfgGpio::gpioMode mode, bool multiPin);
	Gpio(CfgGpio *gpioPin);
	~Gpio();
	bool setOut(unsigned int value);
	signed int in();
	bool setMode(unsigned char mode);
	bool getState();
	void idle();
	SysErr err;

	CfgGpio cfg;

	bool lastState;
	struct
	{
		struct
		{
			void (*onStateChanged)(void *param, bool state);
			void *onStateChangedHandler;
			void (*onUp)(void *param);
			void *onUpHandler;
			void (*onDown)(void *param);
			void *onDownHandler;
		} callback;
		bool stateUp;
		bool stateDown;
		bool stateChanged;
	}events;
};
}
}
/*#####################################################*/
#endif /* GPIO_DEF_H_ */
