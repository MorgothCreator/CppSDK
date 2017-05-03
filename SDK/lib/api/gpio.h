/*
 *  api/gpio.h
 */

#ifndef GPIO_DEF_H_
#define GPIO_DEF_H_
/*#####################################################*/
#include <stdlib.h>
#include <stdbool.h>
#include <interface/gpio.h>
#include <include/global.h>
#include <api/init_def.h>
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
#define pin_mux_convert_to_pin(port, pin) ((port<<5) + ((pin) % 32))

#define pin_mux_convert_to_port_pin(pin) (pin>>5), ((pin %32) - ((pin>>5)<<5))
/*#####################################################*/
typedef struct
{
	char name[12];
	unsigned int pin;
	bool reverse;
	enum gpioMode_e{
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
	}gpioMode;
	bool multiPin;
	unsigned int defValue;
} CfgGpio;
/*#####################################################*/
namespace GI
{
namespace Dev
{
class Gpio
{
public:
	Gpio(ioSettings *cfg);
	~Gpio();
	SysErr setOut(unsigned int value);
	void setOut(void *base_addr, unsigned int mask, unsigned int pin_mask);
	void setOut(void *base_addr, unsigned int pin, bool value);
	signed int in();
	SysErr getIn(unsigned long *value);
	SysErr setMode(CfgGpio::gpioMode_e mode);
	SysErr setMode(CfgGpio::gpioMode_e mode, unsigned int mask);
	bool getState();
	void idle();
	SysErr err;

	ioSettings *cfg;
	void *baseAddr;
	unsigned char pinNr;
	unsigned int multiPinMask;;

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
