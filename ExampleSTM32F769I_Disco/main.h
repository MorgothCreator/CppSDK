/*
 * main.h
 *
 *  Created on: Apr 16, 2017
 *      Author: Cutare
 */

#ifndef MAIN_H_
#define MAIN_H_

#define USE_GPIO                        1
#define USE_UART                        1
#define USE_I2C                         1
#define USE_SPI                         1

#define CONSOLE_UART_IN		            "uart-0"
#define CONSOLE_UART_OUT	            "uart-0"
#define CONSOLE_UART_ERR	            "uart-0"

#define USE_TERMINAL                    1

#define USE_DHCP						1

#define USE_LWIP						1

#define USE_LWIP_HTTPD_SERVER			1
/* HTTP settings */
#define LWIP_HTTPD_SERVER_DRIVE_PATH	"SD1:"
//#define LWIP_HTTPD_SERVER_DRIVE_PATH	"USB1:"
#define LWIP_HTTPD_SERVER_PORT			80

#define USE_TFTP						0
/* FTP settings */
#define TFTPD_PATH_SERVER_DRIVE_PATH	"SD1:"
//#define TFTPD_PATH_SERVER_DRIVE_PATH	"USB1:"
#define TFTP_SERVER_PORT				12000

#define USE_FTP							1
/* FTP settings */
#define FTPD_PATH_SERVER_DRIVE_PATH		"SD1:"
//#define TFTPD_PATH_SERVER_DRIVE_PATH	"USB1:"
#define FTP_SERVER_PORT					21

#define USE_SNTP						1
#define SNTP_SERVER_ADDRESS 			"82.197.164.46" /* google */
#define SNTP_UPDATE_DELAY 				90000 /* SNTP update every 90 seconds */

#define _USE_PASSWORD_PROTECTION		0

#define _USE_SCREEN						1

#define _USE_MPU60x0_9150				0
#define _USE_HIH613x					0
#define _USE_AK8975						0
#define _USE_BMP180						0
#define _USE_MPL3115A2					0
#define _USE_MPR121						0
#define _USE_L3GD20						0
#define _USE_MS5611						0
#define _USE_MHC5883					0
#define _USE_ADXL345					0
#define _USE_SHT11 						0
#define _USE_SX150x						0
#define _USE_SRF02						0
#define _USE_LEPTON_FLIR				0
#define _USE_nRF25l01					0

#define _USE_INT_ADC					0

#define USE_DRIVER_SEMAPHORE			true

#include <main_app.h>


#endif /* MAIN_H_ */
