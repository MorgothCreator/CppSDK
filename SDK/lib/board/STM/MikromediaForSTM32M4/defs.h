#ifndef __MikromediaForSTM32M4__
#define __MikromediaForSTM32M4__

#if defined(MikromediaForSTM32M4)

#include "../MikromediaForSTM32M4/ffconf.h"
#include "../MikromediaForSTM32M4/lwipopts.h"
#include <api/lcd_def.h>


#define LED1_PORT			IOG
#define LED1_PIN			6
#define LED1_INVERSE		true

#define LED2_PORT			IOD
#define LED2_PIN			4
#define LED2_INVERSE		true

#define LED3_PORT			IOD
#define LED3_PIN			5
#define LED3_INVERSE		true

#define LED4_PORT			IOK
#define LED4_PIN			3
#define LED4_INVERSE		true

#define LEDS_NR				4

#define HARDBTN1_PORT		IOA
#define HARDBTN1_PIN		0

#define HARDBTNS_NR			1

#define DEFAULT_UART_UNIT		2
#define DEFAULT_UART_TX_PORT	IOB
#define DEFAULT_UART_TX_PIN		10
#define DEFAULT_UART_RX_PORT	IOB
#define DEFAULT_UART_RX_PIN		11

#define DEFAULT_I2C_UNIT		0
#define DEFAULT_I2C_SCL_PORT	IOB
#define DEFAULT_I2C_SCL_PIN	8
#define DEFAULT_I2C_SDA_PORT	IOB
#define DEFAULT_I2C_SDA_PIN	9

#define AUDIO_I2C_UNIT		1
#define AUDIO_I2C_SCL_PORT	IOH
#define AUDIO_I2C_SCL_PIN	4
#define AUDIO_I2C_SDA_PORT	IOH
#define AUDIO_I2C_SDA_PIN	5

#define SPI_EXTENSION_UNIT		0
#define SPI_EXTENSION_SCK_PORT	IOA
#define SPI_EXTENSION_SCK_PIN	5
#define SPI_EXTENSION_MOSI_PORT	IOB
#define SPI_EXTENSION_MOSI_PIN	5
#define SPI_EXTENSION_MISO_PORT	IOB
#define SPI_EXTENSION_MISO_PIN	4
#define SPI_EXTENSION_CS1_PORT	IOA//SD_CD
#define SPI_EXTENSION_CS1_PIN	15

#define LCD_BACKLIGHT_PORT		IOA
#define LCD_BACKLIGHT_PIN		3
#define LCD_DISPLAY_RST_PORT	IOH
#define LCD_DISPLAY_RST_PIN		7
#define LCD_TOUCH_INT_PORT		IOJ
#define LCD_TOUCH_INT_PIN		5

#define SD_CD_PORT				IOG
#define SD_CD_PIN				2


/*
 * Processor choose
 */
//#define STM32F405xx
//#define STM32F415xx
#define STM32F407xx
//#define STM32F417xx
//#define STM32F427xx
//#define STM32F437xx
//#define STM32F429xx
//#define STM32F439xx
//#define STM32F401xC
//#define STM32F401xE
//#define STM32F410Tx
//#define STM32F410Cx
//#define STM32F410Rx
//#define STM32F411xE
//#define STM32F446xx
//#define STM32F469xx
//#define STM32F479xx

/*
 * !Processor choose
 */

/*
 * Processor clock selection.
 *
 * If EXTERNAL_CLOCK_VALUE id !0 the reference clock will be EXTERNAL_CLOCK_VALUE.
 * If HSE_VALUE value is 0 the reference clock will be the HSI_VALUE.
 */

#define HSE_VALUE						((uint32_t)8000000)	/*!< Value of the External oscillator in Hz*/
#define LSE_VALUE  						((uint32_t)32768)		/*!< Value of the External Low Speed oscillator in Hz */
#define EXTERNAL_CLOCK_VALUE    		((uint32_t)0000000)	/*!< Value of the External clock in Hz*/
#define CORE_CLOCK_DEFAULT				((uint32_t)180000000)	/*!< Value of the Processor default clock in Hz*/
#define USE_EXT_RAM						0
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

//#define SCREEN_ENABLE					lcd_ILI9341_PORTRAIT
/*
 * !Screen settings
 */
/*
 * Touch screen settings
 */

#define USE_CAP_TOUCHSCREEN				1
#define CAP_TOUCHSCREEN_I2C_UNIT		"i2c-0"
#define CAP_TOUCHSCREEN_IRQ_PIN			"touchirq"
/*
 * !Touch screen settings
 */

/*
 * Touch screen settings
 */

#define TOUCH_SCREEN_ENABLE				1
#define TOUCH_SCREEN_TYPE_ANALOG		1
#define TOUCH_SCREEN_TYPE_CAPACITIVE	0
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
#if (USE_LWIP == 1)
#define USE_RMII_INTERFACE					1
#define LWIP_DEBUG_EN						0


#define USE_LWIP_HTTPD_SERVER				1
#endif
/* HTTP settings */
#define LWIP_HTTPD_SERVER_DRIVE_PATH		"SD1:"
//#define LWIP_HTTPD_SERVER_DRIVE_PATH		"USB1:"
#define LWIP_HTTPD_SERVER_PORT				80

/* FTP settings */
#define TFTPD_PATH_SERVER_DRIVE_PATH		"SD1:"
//#define TFTPD_PATH_SERVER_DRIVE_PATH	"USB1:"

/*
 * Eth http server settings
 */

/*
 * !Eth settings
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





#endif/*!STM32F769I_DISCO*/
#endif
