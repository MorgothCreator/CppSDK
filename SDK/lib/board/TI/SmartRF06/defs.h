#ifndef __STM32F429I_DISCO__
#define __STM32F429I_DISCO__

#if defined(SmartRf06)

#include <include/global.h>
#include "../SmartRF06/ffconf.h"
#include "../SmartRF06/lwipopts.h"
#include <api/lcd_def.h>

/*
 * !Processor choose
 */

/*
 * Processor clock selection.
 *
 * If EXTERNAL_CLOCK_VALUE id !0 the reference clock will be EXTERNAL_CLOCK_VALUE.
 * If HSE_VALUE value is 0 the reference clock will be the HSI_VALUE.
 */

#define HSE_VALUE						((uint32_t)24000000)	/*!< Value of the External oscillator in Hz*/
#define LSE_VALUE  						((uint32_t)32768)		/*!< Value of the External Low Speed oscillator in Hz */
#define EXTERNAL_CLOCK_VALUE    		((uint32_t)0000000)	/*!< Value of the External clock in Hz*/
#define CORE_CLOCK_DEFAULT				((uint32_t)48000000)	/*!< Value of the Processor default clock in Hz*/
#define USE_EXT_RAM						1
/*
 * Processor clock selection.
 */

/*
 * Debug interface setup.
 */
//#define STD_OUT_PATH			"uart-0"/* Is output is directed to serial port you need to provide the path of port */
//#define STD_OUT_PATH			"SCREEN<Terminal>"/* If output is directed to screen, need a screen name to be displayed on caption. */
//#define STD_IN_PATH				"UART:0"
//#define STD_ERR_PATH			"UART:0"
/*
 * !Debug interface setup.
 */

/*
 * Screen settings
 */

#define SCREENS_COUNT                   1
#define SCREEN_CONFIG_1                 lcd_DOGM128W_6_LANDSCAPE_FLIP
#define USED_SCREEN_CONTROLLER_1        St7565
#define OTHER_SCREEN_1_PARAMS           ((string)"spi-0.1")

/*
 * !Screen settings
 */
/*
 * Touch screen settings
 */

#define TOUCH_SCREEN_ENABLE				false
#define CAP_TOUCHSCREEN_I2C_UNIT		"i2c-0"
#define CAP_TOUCHSCREEN_IRQ_PIN			"touchirq"
//#define USED_TOUCHSCREEN				Stmpe811

/*
 * !Touch screen settings
 */

/*
 * USB settings
 */

#define USE_USB_DEV
//#define USE_USB_HS
//#define USE_USB_FS
/* #define USE_USB_HS_IN_FS */

/*
 * !USB settings
 */

/*
 * Used fonts
 */
#define USE_FONT_CMSS14B
//#define USE_ALL_FONTS
/*
 * !Used fonts
 */

/*
 * Eth general settings
 */
#define LWIP_DEBUG_EN						false
#define MAX_DHCP_TRIES  					4

/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0							(unsigned char) 192
#define IP_ADDR1							(unsigned char) 168
#define IP_ADDR2							(unsigned char) 0
#define IP_ADDR3							(unsigned char) 10

/*NETMASK*/
#define NETMASK_ADDR0						(unsigned char) 255
#define NETMASK_ADDR1						(unsigned char) 255
#define NETMASK_ADDR2						(unsigned char) 255
#define NETMASK_ADDR3						(unsigned char) 0

/*Gateway Address*/
#define GW_ADDR0							(unsigned char) 192
#define GW_ADDR1							(unsigned char) 168
#define GW_ADDR2							(unsigned char) 0
#define GW_ADDR3							(unsigned char) 1

/*
 * !Eth general settings
 */

/*
 * MmcSd settings
 */

#define USE_MMCSD_ENABLE					0
#define MMCSD_CARD_DETECT_PIN				"mmcsdcd"
#define MMCSD_STATUS_LED_PIN				"led-1"
/*
 * !MmcSd settings
 */

/*
 * Thread settings
 */

#define USE_DRIVER_SEMAPHORE			true

/*
 * !Thread settings
 */




#endif/*!STM32F769I_DISCO*/
#endif
