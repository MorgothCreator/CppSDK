/*
 * main.h
 *
 *  Created on: Apr 16, 2017
 *      Author: Cutare
 */

#ifndef MAIN_H_
#define MAIN_H_

#define USE_GPIO                        true
#define USE_UART                        true
#define USE_I2C                         false
#define USE_SPI                         false
#define USE_SCREEN                      false

#define CALIBRATE_TOUCH_ON_STARTUP		false

#if USE_SPI
#define USE_MMCSD_SPI                   false
#define USED_MMCSD_SPI_UNIT             "spi-0.0"
#endif


#define CONSOLE_UART_IN		            "uart-2"
#define CONSOLE_UART_OUT	            "uart-2"
#define CONSOLE_UART_ERR	            "uart-2"

#define USE_TERMINAL                    false

#define USE_LWIP						false

#define USE_DHCP						false

#define USE_TFTP						false

#define USE_LWIP_HTTPD_SERVER			false
/* HTTP settings */
#define LWIP_HTTPD_SERVER_DRIVE_PATH	"SD1:"
//#define LWIP_HTTPD_SERVER_DRIVE_PATH	"USB1:"
#define LWIP_HTTPD_SERVER_PORT			80

#define USE_TFTP						false
/* FTP settings */
#define TFTPD_PATH_SERVER_DRIVE_PATH	"SD1:"
//#define TFTPD_PATH_SERVER_DRIVE_PATH	"USB1:"
#define TFTP_SERVER_PORT				12000

#define USE_FTP							false
/* FTP settings */
#define FTPD_PATH_SERVER_DRIVE_PATH		"SD1:"
//#define TFTPD_PATH_SERVER_DRIVE_PATH	"USB1:"
#define FTP_SERVER_PORT					21

#define USE_SNTP						false
#define SNTP_SERVER_ADDRESS 			"82.197.164.46" /* google */
#define SNTP_UPDATE_DELAY 				90000 /* SNTP update every 90 seconds */

#define _USE_PASSWORD_PROTECTION		false

#define _USE_MPU60x0_9150				false
#define _USE_HIH613x					false
#define _USE_AK8975						false
#define _USE_BMP180						false
#define _USE_MPL3115A2					false
#define _USE_MPR121						false
#define _USE_L3GD20						false
#define _USE_MS5611						false
#define _USE_MHC5883					false
#define _USE_ADXL345					false
#define _USE_SHT11 						false
#define _USE_SX150x						false
#define _USE_SRF02						false
#define _USE_LEPTON_FLIR				false
#define _USE_nRF25l01					false

#define _USE_INT_ADC					false

#define USE_DRIVER_SEMAPHORE			true

#include <main_app.h>

#endif /* MAIN_H_ */
