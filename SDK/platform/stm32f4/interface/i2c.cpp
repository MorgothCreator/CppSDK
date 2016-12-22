/*
 * twi_interface.c
 *
 * Created: 2/8/2013 5:00:27 PM
 *  Iulian Gheorghiu <morgoth.creator@gmail.com>
 */
/*#####################################################*/
#include <interface/i2c.h>
#include "api/init.h"

#define TIMING_CLEAR_MASK   ((uint32_t)0xF0FFFFFFU)  /*!<  I2C TIMING clear register Mask */
#define I2C_TIMEOUT_ADDR    ((uint32_t)10000U)       /*!< 10 s  */
#define I2C_TIMEOUT_BUSY    ((uint32_t)25U)          /*!< 25 ms */
#define I2C_TIMEOUT_DIR     ((uint32_t)25U)          /*!< 25 ms */
#define I2C_TIMEOUT_RXNE    ((uint32_t)25U)          /*!< 25 ms */
#define I2C_TIMEOUT_STOPF   ((uint32_t)25U)          /*!< 25 ms */
#define I2C_TIMEOUT_TC      ((uint32_t)25U)          /*!< 25 ms */
#define I2C_TIMEOUT_TCR     ((uint32_t)25U)          /*!< 25 ms */
#define I2C_TIMEOUT_TXIS    ((uint32_t)25U)          /*!< 25 ms */
#define I2C_TIMEOUT_FLAG    ((uint32_t)25U)          /*!< 25 ms */

#define MAX_NBYTE_SIZE      255U
#define SlaveAddr_SHIFT     7U
#define SlaveAddr_MSK       0x06U

#define USE_I2C_TX_DMA
/* I2C TIMING Register define when I2C clock source is APB1 (SYSCLK/4) */
/* I2C TIMING is calculated in case of the I2C Clock source is the APB1CLK = 50 MHz */
/* This example use TIMING to 0x40912732 to reach 100 kHz speed (Rise time = 700 ns, Fall time = 100 ns) */
#define I2C_TIMING      0x40912732
//#####################################################
#if (USE_DRIVER_SEMAPHORE == true)
volatile bool twi_semaphore[TWI_INTERFACE_COUNT];
#endif

I2C_TypeDef *sEE_I2C[4] =
{
#ifdef I2C1
		I2C1,
#endif
#ifdef I2C2
		I2C2,
#endif
#ifdef I2C3
		I2C3,
#endif
#ifdef I2C4
		I2C4,
#endif
#ifdef I2C5
		I2C5,
#endif
#ifdef I2C6
		I2C6
#endif
	};

/**
 * @brief  Start critical section: these callbacks should be typically used
 *         to disable interrupts when entering a critical section of I2C communication
 *         You may use default callbacks provided into this driver by uncommenting the
 *         define USE_DEFAULT_CRITICAL_CALLBACK.
 *         Or you can comment that line and implement these callbacks into your
 *         application.
 * @param  None.
 * @retval None.
 */
void sEE_EnterCriticalSection_UserCallback(void)
{
	__disable_irq();
}

/**
 * @brief  Start and End of critical section: these callbacks should be typically used
 *         to re-enable interrupts when exiting a critical section of I2C communication
 *         You may use default callbacks provided into this driver by uncommenting the
 *         define USE_DEFAULT_CRITICAL_CALLBACK.
 *         Or you can comment that line and implement these callbacks into your
 *         application.
 * @param  None.
 * @retval None.
 */
void sEE_ExitCriticalSection_UserCallback(void)
{
	__enable_irq();
}
/**
 * @brief  Enables or disables the specified I2C software reset.
 * @note   When software reset is enabled, the I2C IOs are released (this can
 *         be useful to recover from bus errors).
 * @param  I2Cx: where x can be 1, 2 or 3 to select the I2C peripheral.
 * @param  NewState: new state of the I2C software reset.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void GI::Dev::I2c::SoftwareReset(I2C_HandleTypeDef *hi2c)
{
	I2C_TypeDef* I2Cx = hi2c->Instance;
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	I2C_TypeDef I2CxBack;
	I2CxBack.CR1 = I2Cx->CR1;
	I2CxBack.CR2 = I2Cx->CR2;
	I2CxBack.OAR1 = I2Cx->OAR1;
	I2CxBack.OAR2 = I2Cx->OAR2;
	I2CxBack.CCR = I2Cx->CCR;
	I2CxBack.TRISE = I2Cx->TRISE;
	/* Enable the selected I2C peripheral */
	__HAL_I2C_DISABLE(hi2c);
	/* Enable the selected I2C peripheral */
	__HAL_I2C_ENABLE(hi2c);
	I2Cx->TRISE = I2CxBack.TRISE;
	I2Cx->CCR = I2CxBack.CCR;
	I2Cx->OAR2 = I2CxBack.OAR2;
	I2Cx->OAR1 = I2CxBack.OAR1;
	I2Cx->CR2 = I2CxBack.CR2;
	I2Cx->CR1 = I2CxBack.CR1;
}
void TWI_SendStop(I2C_HandleTypeDef *hi2c)
{

}
//#####################################################
//#include "int/int_twi.h"
/**
 * @brief  Reads a block of data from the EEPROM.
 * @param  pBuffer : pointer to the buffer that receives the data read from
 *         the EEPROM.
 * @param  ReadAddr : EEPROM's internal address to start reading from.
 * @param  NumByteToRead : pointer to the variable holding number of bytes to
 *         be read from the EEPROM.
 *
 *        @note The variable pointed by NumByteToRead is reset to 0 when all the
 *              data are read from the EEPROM. Application should monitor this
 *              variable in order know when the transfer is complete.
 *
 * @note When number of data to be read is higher than 1, this function just
 *       configures the communication and enable the DMA channel to transfer data.
 *       Meanwhile, the user application may perform other tasks.
 *       When number of data to be read is 1, then the DMA is not used. The byte
 *       is read in polling mode.
 *
 * @retval sEE_OK (0) if operation is correctly performed, else return value
 *         different from sEE_OK (0) or the timeout user callback.
 */
/*unsigned long TWI_MasterWriteRead(new_twi* TwiStruct, unsigned int TransmitBytes, unsigned int _Size)
 {
 I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)TwiStruct->udata;

 }*/
//#####################################################
/**
 * @brief  Initializes peripherals used by the I2C EEPROM driver.
 * @param  None
 * @retval None
 */
GI::Dev::I2c::I2c(const char *path)
{
	unsigned int item_nr = 0;
	while(1)
	{
		if(i2cCfg[item_nr].name == NULL)
		{
			err = SYS_ERR_INVALID_PATH;
			return;
		}
		if(!strcmp(i2cCfg[item_nr].name, path))
			break;
		item_nr++;
	}

	if(strncmp(path, (char *)"i2c-", sizeof("i2c-") - 1))
	{
		err = SYS_ERR_INVALID_PATH;
		return;
	}
	unsigned int dev_nr = 0;
	if(sscanf(path + (sizeof("i2c-") - 1), "%u", &dev_nr) != 1)
	{
		err = SYS_ERR_INVALID_PATH;
		return;
	}
	if(dev_nr >= TWI_INTERFACE_COUNT)
	{
		err = SYS_ERR_INVALID_PATH;
		return;
	}

	memset(this, 0, sizeof(*this));
	memcpy(&cfg, &i2cCfg[item_nr], sizeof(CfgI2c));

	I2C_HandleTypeDef *I2cHandle = (I2C_HandleTypeDef *) calloc(1, sizeof(I2C_HandleTypeDef));
	if (!I2cHandle)
		return;
	//I2C_InitTypeDef  I2C_InitStructure;
	udata = (void *) I2cHandle;
	I2cHandle->Instance = sEE_I2C[dev_nr];
	unitNr = dev_nr;

	GPIO_InitTypeDef GPIO_InitStructure;

	switch (dev_nr)
	{
#ifdef __HAL_RCC_I2C1_CLK_ENABLE
	case 0:
		__HAL_RCC_I2C1_CLK_ENABLE()
		;
		break;
#endif
#ifdef __HAL_RCC_I2C2_CLK_ENABLE
	case 1:
		__HAL_RCC_I2C2_CLK_ENABLE()
		;
		break;
#endif
#ifdef __HAL_RCC_I2C3_CLK_ENABLE
	case 2:
		__HAL_RCC_I2C3_CLK_ENABLE()
		;
		break;
#endif
#ifdef __HAL_RCC_I2C4_CLK_ENABLE
		case 3:
		__HAL_RCC_I2C4_CLK_ENABLE()
		;
		break;
#endif
#ifdef __HAL_RCC_I2C5_CLK_ENABLE
		case 4:
		__HAL_RCC_I2C5_CLK_ENABLE();
		break;
#endif
#ifdef __HAL_RCC_I2C6_CLK_ENABLE
		case 5:
		__HAL_RCC_I2C6_CLK_ENABLE();
		break;
#endif
	}
	/*!< GPIO configuration */
	/*!< Configure sEE_I2C pins: SCL */
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStructure.Alternate = GPIO_AF4_I2C1;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStructure.Pull = GPIO_PULLUP;

	GPIO_InitStructure.Pin = 1 << (cfg.scl % 32);
	HAL_GPIO_Init(GET_GPIO_PORT_BASE_ADDR[cfg.scl >> 5],
			&GPIO_InitStructure);

	GPIO_InitStructure.Pin = 1 << (cfg.sda % 32);
	HAL_GPIO_Init(GET_GPIO_PORT_BASE_ADDR[cfg.sda >> 5],
			&GPIO_InitStructure);

	/*##-1- Configure the I2C peripheral #######################################*/

	I2cHandle->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	I2cHandle->Init.ClockSpeed = 100000.;
	I2cHandle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	I2cHandle->Init.DutyCycle = I2C_DUTYCYCLE_2;
	I2cHandle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	I2cHandle->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	//I2cHandle->Init.OwnAddress1 = TwiStruct->MasterSlaveAddr;
	//I2cHandle.Init.OwnAddress2     = 0xFF;

	if (HAL_I2C_Init(I2cHandle) != HAL_OK)
	{
		/* Initialization Error */
		return;
	}
	return;
}
/*#####################################################*/
GI::Dev::I2c::~I2c()
{
	I2C_HandleTypeDef *I2cHandle = (I2C_HandleTypeDef *) udata;

	switch ((unsigned int) I2cHandle->Instance)
	{
#ifdef __HAL_RCC_I2C1_CLK_DISABLE
	case I2C1_BASE:
		__HAL_RCC_I2C1_CLK_DISABLE();
		break;
#endif
#ifdef __HAL_RCC_I2C2_CLK_DISABLE
	case I2C2_BASE:
		__HAL_RCC_I2C2_CLK_DISABLE();
		break;
#endif
#ifdef __HAL_RCC_I2C3_CLK_DISABLE
	case I2C3_BASE:
		__HAL_RCC_I2C3_CLK_DISABLE();
		break;
#endif
#ifdef __HAL_RCC_I2C4_CLK_DISABLE
		case I2C4_BASE:
		__HAL_RCC_I2C4_CLK_DISABLE();
		break;
#endif
#ifdef __HAL_RCC_I2C5_CLK_DISABLE
		case I2C5_BASE:
		__HAL_RCC_I2C5_CLK_DISABLE();
		break;
#endif
#ifdef __HAL_RCC_I2C6_CLK_DISABLE
		case I2C6_BASE:
		__HAL_RCC_I2C6_CLK_DISABLE();
		break;
#endif
	}
	HAL_GPIO_DeInit(GET_GPIO_PORT_BASE_ADDR[cfg.scl >> 5],
			(1 << (cfg.scl % 32)));

	HAL_GPIO_DeInit(GET_GPIO_PORT_BASE_ADDR[cfg.sda >> 5],
			(1 << (cfg.sda % 32)));
	if (udata)
		free(udata);
}
/*#####################################################*/
/*#####################################################*/
#define Timeout 10
#define I2C_TIMEOUT_BUSY_FLAG     ((uint32_t)10000)  /* 10 s  */

unsigned long GI::Dev::I2c::WR(unsigned char addr, unsigned char *buff_send,
		unsigned int TransmitBytes, unsigned char *buff_receive,
		unsigned int ReceiveBytes)
{
	//unsigned long sEETimeout;
	/* Set the pointer to the Number of data to be read. This pointer will be used
	 by the DMA Transfer Completer interrupt Handler in order to reset the
	 variable to 0. User should check on this variable in order to know if the
	 DMA transfer has been complete or not. */
	//I2C_TypeDef *I2Cx = sEE_I2C[property.unitNr];
	I2C_HandleTypeDef *hi2c = ((I2C_HandleTypeDef *) udata);
	/* If bus is freeze we will reset the unit and restore the settings. */
	/* Wait until BUSY flag is reset */
	if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET,
			I2C_TIMEOUT_BUSY_FLAG) != HAL_OK)
	{
		SoftwareReset(hi2c);
		return HAL_BUSY;
	}
	/*!< While the bus is busy */
	unsigned int cnt = 0;
	/* Disable Pos */
	hi2c->Instance->CR1 &= ~I2C_CR1_POS;

	hi2c->State = HAL_I2C_STATE_MEM_BUSY_RX;
	hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

	if (!noSendWriteOnRead)
	{

		/* Enable Acknowledge */
		hi2c->Instance->CR1 |= I2C_CR1_ACK;

		/* Generate Start */
		hi2c->Instance->CR1 |= I2C_CR1_START;

		/* Wait until SB flag is set */
		if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_SB, RESET, Timeout)
				!= HAL_OK)
		{
			__HAL_UNLOCK(hi2c);
			return HAL_TIMEOUT;
		}

		/* Send slave address */
		hi2c->Instance->DR = I2C_7BIT_ADD_WRITE(addr);

		/* Wait until ADDR flag is set */
		if (I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADDR,
				Timeout) != HAL_OK)
		{
			if (hi2c->ErrorCode == HAL_I2C_ERROR_AF)
			{
				__HAL_UNLOCK(hi2c);
				return HAL_TIMEOUT;
			}
			else
			{
				__HAL_UNLOCK(hi2c);
				return HAL_TIMEOUT;
			}
		}

		/* Clear ADDR flag */
		__HAL_I2C_CLEAR_ADDRFLAG(hi2c);

		/* Wait until TXE flag is set */
		if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_TXE, RESET, Timeout)
				!= HAL_OK)
		{
			__HAL_UNLOCK(hi2c);
			return HAL_TIMEOUT;
		}

		for (; cnt < TransmitBytes; cnt++)
		{

			/* Send MSB of Memory Address */
			hi2c->Instance->DR = buff_send[cnt];

			/* Wait until TXE flag is set */
			if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_TXE, RESET, Timeout)
					!= HAL_OK)
			{
				__HAL_UNLOCK(hi2c);
				return HAL_TIMEOUT;
			}
		}
	}

	if (!ReceiveBytes)
	{
		/* Generate Stop */
		hi2c->Instance->CR1 |= I2C_CR1_STOP;

		hi2c->State = HAL_I2C_STATE_READY;

		/* Process Unlocked */
		__HAL_UNLOCK(hi2c);

	}
	else
	{

		/* Generate Restart */
		hi2c->Instance->CR1 |= I2C_CR1_START;

		/* Wait until SB flag is set */
		if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_SB, RESET, Timeout)
				!= HAL_OK)
		{
			__HAL_UNLOCK(hi2c);
			return HAL_TIMEOUT;
		}

		/* Send slave address */
		hi2c->Instance->DR = I2C_7BIT_ADD_READ(addr);

		/* Wait until ADDR flag is set */
		if (I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADDR,
				Timeout) != HAL_OK)
		{
			__HAL_UNLOCK(hi2c);
			return HAL_TIMEOUT;
		}
		unsigned long data_cnt = 0;

		if (ReceiveBytes == 1)
		{
			/* Disable Acknowledge */
			hi2c->Instance->CR1 &= ~I2C_CR1_ACK;

			/* Clear ADDR flag */
			__HAL_I2C_CLEAR_ADDRFLAG(hi2c);

			/* Generate Stop */
			hi2c->Instance->CR1 |= I2C_CR1_STOP;
		}
		else if (ReceiveBytes == 2)
		{
			/* Disable Acknowledge */
			hi2c->Instance->CR1 &= ~I2C_CR1_ACK;

			/* Enable Pos */
			hi2c->Instance->CR1 |= I2C_CR1_POS;

			/* Clear ADDR flag */
			__HAL_I2C_CLEAR_ADDRFLAG(hi2c);
		}
		else
		{
			/* Enable Acknowledge */
			hi2c->Instance->CR1 |= I2C_CR1_ACK;

			/* Clear ADDR flag */
			__HAL_I2C_CLEAR_ADDRFLAG(hi2c);
		}

		while (ReceiveBytes > 0)
		{
			if (ReceiveBytes <= 3)
			{
				/* One byte */
				if (ReceiveBytes == 1)
				{
					/* Wait until RXNE flag is set */
					if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_RXNE, RESET,
							Timeout) != HAL_OK)
					{
						__HAL_UNLOCK(hi2c);
						return HAL_TIMEOUT;
					}

					/* Read data from DR */
					buff_receive[data_cnt++] = hi2c->Instance->DR;
					ReceiveBytes--;
				}
				/* Two bytes */
				else if (ReceiveBytes == 2)
				{
					/* Wait until BTF flag is set */
					if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BTF, RESET,
							Timeout) != HAL_OK)
					{
						__HAL_UNLOCK(hi2c);
						return HAL_TIMEOUT;
					}

					/* Generate Stop */
					hi2c->Instance->CR1 |= I2C_CR1_STOP;

					/* Read data from DR */
					buff_receive[data_cnt++] = hi2c->Instance->DR;
					ReceiveBytes--;

					/* Read data from DR */
					buff_receive[data_cnt++] = hi2c->Instance->DR;
					ReceiveBytes--;
				}
				/* 3 Last bytes */
				else
				{
					/* Wait until BTF flag is set */
					if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BTF, RESET,
							Timeout) != HAL_OK)
					{
						__HAL_UNLOCK(hi2c);
						return HAL_TIMEOUT;
					}

					/* Disable Acknowledge */
					hi2c->Instance->CR1 &= ~I2C_CR1_ACK;

					/* Read data from DR */
					buff_receive[data_cnt++] = hi2c->Instance->DR;
					ReceiveBytes--;

					/* Wait until BTF flag is set */
					if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BTF, RESET,
							Timeout) != HAL_OK)
					{
						__HAL_UNLOCK(hi2c);
						return HAL_TIMEOUT;
					}

					/* Generate Stop */
					hi2c->Instance->CR1 |= I2C_CR1_STOP;

					/* Read data from DR */
					buff_receive[data_cnt++] = hi2c->Instance->DR;
					ReceiveBytes--;

					/* Read data from DR */
					buff_receive[data_cnt++] = hi2c->Instance->DR;
					ReceiveBytes--;
				}
			}
			else
			{
				/* Wait until RXNE flag is set */
				if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_RXNE, RESET,
						Timeout) != HAL_OK)
				{
					__HAL_UNLOCK(hi2c);
					return HAL_TIMEOUT;
				}

				/* Read data from DR */
				buff_receive[data_cnt++] = hi2c->Instance->DR;
				ReceiveBytes--;

				if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF) == SET)
				{
					/* Read data from DR */
					buff_receive[data_cnt++] = hi2c->Instance->DR;
					ReceiveBytes--;
				}
			}
		}
	}
	/* Process Unlocked */
	hi2c->State = HAL_I2C_STATE_READY;

	/* Process Unlocked */
	__HAL_UNLOCK(hi2c);
	return HAL_OK;
}
/*#####################################################*/
int GI::Dev::I2c::writeRead(unsigned char addr, unsigned char *buffSend,
		unsigned int lenSend, unsigned char *buffReceive,
		unsigned int lenReceive)
{
	if (!this)
		return false;
#if (USE_DRIVER_SEMAPHORE == true)
	while (twi_semaphore[unitNr])
	;
	twi_semaphore[unitNr] = true;
#endif
	bool result = WR(addr << 1, buffSend, lenSend, buffReceive, lenReceive);

#if (USE_DRIVER_SEMAPHORE == true)
	twi_semaphore[unitNr] = false;
#endif
	if (result == HAL_OK)
		return true;
	else
		return false;
}
/*#####################################################*/
int GI::Dev::I2c::readBytes(unsigned char addr, unsigned char *buff,
		unsigned int len)
{
	if (!this)
		return false;
#if (USE_DRIVER_SEMAPHORE == true)
	while (twi_semaphore[unitNr])
	;
	twi_semaphore[unitNr] = true;
#endif
	bool result = WR(addr << 1, NULL, 0, buff, len);
#if (USE_DRIVER_SEMAPHORE == true)
	twi_semaphore[unitNr] = false;
#endif
	if (result == HAL_OK)
		return true;
	else
		return false;
}

/*#####################################################*/
int GI::Dev::I2c::writeBytes(unsigned char addr, unsigned char *buff,
		unsigned int len)
{
	if (!this)
		return false;
#if (USE_DRIVER_SEMAPHORE == true)
	while (twi_semaphore[unitNr])
	;
	twi_semaphore[unitNr] = true;
#endif
	bool result = WR(addr << 1, buff, len, NULL, 0);
#if (USE_DRIVER_SEMAPHORE == true)
	twi_semaphore[unitNr] = false;
#endif
	if (result == HAL_OK)
		return true;
	else
		return false;
}

/*#####################################################*/
