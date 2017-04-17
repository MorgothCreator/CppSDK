/*
 *  api/adc.h
 *
 *  Copyright (C) 2016  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of CppSDK.
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
/*#####################################################*/
#ifndef ADC_DEF_H_
#define ADC_DEF_H_
/*#####################################################*/
#include <interface/adc_def.h>
#include <stdbool.h>
/*#####################################################*/
typedef enum
{
	ADC_ALIGN_LEFT = 0, ADC_ALIGN_RIGHT
} ADC_ALIGN_en;
/*#####################################################*/
typedef enum
{
	ADC_MOD_INDEPENDENT = 0, ADC_MOD_DUAL
} ADC_MODE_en;
/*#####################################################*/
typedef enum
{
	ADC_CONV_MODE_SINGLE = 0, ADC_CONV_MODE_MULTICHANNEL
} ADC_CONV_MODE_en;
/*#####################################################*/
typedef enum
{
	ADC_CONTINUOUS_MODE_SINGLE = 0, ADC_CONTINUOUS_MODE_CONTINUOUS
} ADC_CONTINUOUS_MODE_en;
/*#####################################################*/
typedef enum
{
	ADC_SAMPLE_TIME_1 = 0,
	ADC_SAMPLE_TIME_2 = 1,
	ADC_SAMPLE_TIME_3 = 2,
	ADC_SAMPLE_TIME_4 = 3,
	ADC_SAMPLE_TIME_5 = 4,
	ADC_SAMPLE_TIME_6 = 5,
	ADC_SAMPLE_TIME_7 = 6,
	ADC_SAMPLE_TIME_8 = 7,
} ADC_SAMPLE_TIME_en;
/*#####################################################*/
typedef enum
{
	ADC_INT_REF_DISABLE = 0, ADC_INT_REF_ENABLE
} ADC_INT_REF_en;
/*#####################################################*/
typedef enum
{
	ADC_INT_TEMP_SENSOR_DISABLE = 0, ADC_INT_TEMP_SENSOR_ENABLE
} ADC_INT_TEMP_SENSOR_en;
/*#####################################################*/
typedef enum
{
	ADC_EXT_TRIG_NONE = 0,
	ADC_EXT_TRIG_1,
	ADC_EXT_TRIG_2,
	ADC_EXT_TRIG_3,
	ADC_EXT_TRIG_4,
	ADC_EXT_TRIG_5,
	ADC_EXT_TRIG_6,
	ADC_EXT_TRIG_7,
	ADC_EXT_TRIG_8,
	ADC_EXT_TRIG_9,
	ADC_EXT_TRIG_10,
	ADC_EXT_TRIG_11,
	ADC_EXT_TRIG_12,
	ADC_EXT_TRIG_13,
	ADC_EXT_TRIG_14,
	ADC_EXT_TRIG_15,
	ADC_EXT_TRIG_16,
} ADC_EXT_TRIG_en;
/*#####################################################*/
typedef enum
{
	ADC_EXT_TRIG_ADGE_NONE = 0,
	ADC_EXT_TRIG_ADGE_RISING,
	ADC_EXT_TRIG_ADGE_FALLING,
	ADC_EXT_TRIG_ADGE_RIS_FAL,
} ADC_EXT_TRIG_ADGE_en;
/*#####################################################*/
typedef struct Adc_s
{
	void *BaseAddr; /*!< Here will be the base address of ADC module. This will be write on ADC initialization  */

	unsigned char UnitNr; /*!< Specifies the unit number */

	ADC_MODE_en Mode; /*!< Configures the ADC to operate in independent or dual mode. ADC_MODE_INDEPENDENT and ADC_MODE_DUAL */

	ADC_CONV_MODE_en ConvMode; /*!< Specifies whether the conversion is performed in Scan (multichannels) or Single (one channel) mode. This parameter can be set to true = multichanel or false = single channel */

	ADC_CONTINUOUS_MODE_en ContinuousMode; /*!< Specifies whether the conversion is performed in Continuous or Single mode. This parameter can be set to ENABLE or DISABLE. */

	ADC_EXT_TRIG_en ExtTrig; /*!< Defines the external trigger used to start the analog to digital conversion of regular channels. */

	unsigned int ExtTrigEdge; /*!< Defines the trigger edge. */

	ADC_ALIGN_en Align; /*!< Specifies whether the ADC data alignment is ADC_ALIGN_LEFT or ADC_ALIGN_RIGHT. */

	char EnCh[ADC_CHANNELS_NR_PER_UNIT]; /*!< Specifies whether the channel will be enabled. Here is a table with order of registration, the rank will be assigned in order. ChEn[n] = 0 signify that the channel 'n' is disabled, ChEn[n] = 1 signify Channel 0*/

	unsigned int Prescaller; /*!< Specifies the prescaller divider value */

	bool IntRefEn;

	bool TempSensorEn;

	bool VBatEn;

	unsigned int DmaChannel;

	unsigned int ResolutionBits;

	unsigned char DmaUnitNr;

	void *DmaChBaseAddr;

	volatile unsigned int ChannelCount;

	volatile bool EndOfConversion;

	unsigned int EnabledChannelsNr;

	volatile unsigned int ConvResult[ADC_CHANNELS_NR_PER_UNIT];

	unsigned char SampleTime[ADC_CHANNELS_NR_PER_UNIT];/*!< Sample time, watch the ADC controller documentation */
} Adc_t;

#endif /* ADC_DEF_H_ */
/*#####################################################*/
