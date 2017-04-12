/*
 * main.h
 *
 *  Created on: Dec 9, 2016
 *      Author: John Smith
 */

#ifndef MAIN_H_
#define MAIN_H_



#if  defined(STM32F429I_DISCO)

#define _USE_PASSWORD_PROTECTION		0

#define _USE_SCREEN						0

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



#elif defined(STM32F469I_DISCO)

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



#elif  defined(STM32F769I_DISCO)

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

#endif


#endif /* MAIN_H_ */
