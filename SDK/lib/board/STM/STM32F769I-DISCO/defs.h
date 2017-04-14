#ifndef __STM32F769I_DISCO__
#define __STM32F769I_DISCO__

#if defined(STM32F769I_DISCO)

#include "ffconf.h"
#include "lwipopts.h"
#include <api/uart.h>
#include <api/spi.h>
#include <api/i2c.h>
#include <api/gpio.h>


#define LED1_PORT			IOJ
#define LED1_PIN			13
#define LED1_INVERSE		false

#define LED2_PORT			IOJ
#define LED2_PIN			5
#define LED2_INVERSE		false

/*#define LED3_PORT			IOD
 #define LED3_PIN			5
 #define LED3_INVERSE		true

 #define LED4_PORT			IOK
 #define LED4_PIN			3
 #define LED4_INVERSE		true*/

#define LEDS_NR				2

#define HARDBTN1_PORT		IOA
#define HARDBTN1_PIN		0

#define HARDBTNS_NR			1

#define DEFAULT_UART_UNIT		0
#define DEFAULT_UART_TX_PORT	IOA
#define DEFAULT_UART_TX_PIN		9
#define DEFAULT_UART_RX_PORT	IOA
#define DEFAULT_UART_RX_PIN		10

#define DEFAULT_I2C_UNIT		0
#define DEFAULT_I2C_SCL_PORT	IOB
#define DEFAULT_I2C_SCL_PIN		8
#define DEFAULT_I2C_SDA_PORT	IOB
#define DEFAULT_I2C_SDA_PIN		9

#define AUDIO_LCD_I2C_UNIT		3
#define AUDIO_LCD_I2C_SCL_PORT	IOD
#define AUDIO_LCD_I2C_SCL_PIN	12
#define AUDIO_LCD_I2C_SDA_PORT	IOB
#define AUDIO_LCD_I2C_SDA_PIN	7

#define SPI_EXTENSION_UNIT		1
#define SPI_EXTENSION_SCK_PORT	IOA
#define SPI_EXTENSION_SCK_PIN	12
#define SPI_EXTENSION_MOSI_PORT	IOB
#define SPI_EXTENSION_MOSI_PIN	15
#define SPI_EXTENSION_MISO_PORT	IOB
#define SPI_EXTENSION_MISO_PIN	14
#define SPI_EXTENSION_CS1_PORT	IOA//SD_CD
#define SPI_EXTENSION_CS1_PIN	11

#define LCD_BACKLIGHT_PORT		IOI
#define LCD_BACKLIGHT_PIN		14
#define LCD_DISPLAY_RST_PORT	IOJ
#define LCD_DISPLAY_RST_PIN		15
#define LCD_TOUCH_INT_PORT		IOI
#define LCD_TOUCH_INT_PIN		13

#define SD_CD_PORT				IOI
#define SD_CD_PIN				15

/*
 * Processor choose
 */
//#define STM32F722xx
//#define STM32F723xx
//#define STM32F732xx
//#define STM32F733xx
//#define STM32F756xx
//#define STM32F746xx
//#define STM32F745xx
//#define STM32F765xx
//#define STM32F767xx
#define STM32F769xx
//#define STM32F777xx
//#define STM32F779xx
/*
 * !Processor choose
 */

/*
 * Processor clock selection.
 *
 * If EXTERNAL_CLOCK_VALUE id !0 the reference clock will be EXTERNAL_CLOCK_VALUE.
 * If HSE_VALUE value is 0 the reference clock will be the HSI_VALUE.
 */

#define HSE_VALUE						((uint32_t)25000000)	/*!< Value of the External oscillator in Hz*/
#define LSE_VALUE  						((uint32_t)32768)		/*!< Value of the External Low Speed oscillator in Hz */
#define EXTERNAL_CLOCK_VALUE    		((uint32_t)25000000)	/*!< Value of the External clock in Hz*/
#define CORE_CLOCK_DEFAULT				((uint32_t)216000000)	/*!< Value of the Processor default clock in Hz*/
//#define DATA_IN_ExtSDRAM
/*
 * Processor clock selection.
 */

/*
 * Debug interface setup.
 */
//#define STD_OUT_PATH			"UART:0"
//#define STD_OUT_PATH			"SCREEN<Terminal>"
//#define STD_IN_PATH				"UART:0"
//#define STD_ERR_PATH			"UART:0"
/*
 * !Debug interface setup.
 */

/*
 * Screen settings
 */

#define SCREEN_ENABLE					lcd_OTM8009A_PORTRAIT
#define USE_SCREEN_TYPE_DSI				1
/*
 * !Screen settings
 */
/*
 * Touch screen settings
 */

#define USE_CAP_TOUCHSCREEN				"touchirq"
#define CAP_TOUCHSCREEN_I2C_UNIT		"i2c-3"
#define CAP_TOUCHSCREEN_IRQ_PIN			pin_mux_convert_to_pin(IOI, 13)
/*
 * !Touch screen settings
 */

/*
 * Touch screen settings
 */

#define TOUCH_SCREEN_ENABLE				1
#define TOUCH_SCREEN_TYPE_ANALOG		0
#define TOUCH_SCREEN_TYPE_CAPACITIVE	1
/*
 * !Touch screen settings
 */

/*
 * USB settings
 */

#define USE_USB_DEV
#define USE_USB_HS
/* #define USE_USB_HS_IN_FS */

/*
 * GPIO settings
 */
/*
 * !GPIO settings
 */

/*
 * SPI settings
 */
/*
 * !SPI settings
 */

/*
 * I2C settings
 */
/*
 * !I2C settings
 */

/*
 * !UART settings
 */

#define USED_DEBUG_UART_PORT_NR		0
/*
 * UART settings
 */


/*
 * Used fonts
 */
#define USE_FONT_CMSS14B
//#define USE_ALL_FONTS
/*
 * !UART settings
 */


/*
 * Eth settings
 */
/*
 * Eth general settings
 */
#define LWIP_DEBUG_EN						0
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
 * Eth http server settings
 */
#define USE_RMII_INTERFACE					1
#define LWIP_DEBUG_EN						0



/*
 * Eth http server settings
 */

/*
 * !Eth settings
 */


/*
 * MmcSd settings
 */

#define USE_MMCSD_ENABLE					1
#define MMCSD_CARD_DETECT_PIN				"mmcsdcd"
#define MMCSD_STATUS_LED_PIN				"led-1"
/*
 * !MmcSd settings
 */





#endif/*!STM32F769I_DISCO*/
#endif