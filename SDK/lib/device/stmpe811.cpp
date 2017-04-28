/*
 * device/stmpe811.cpp
 */

#include "stmpe811.h"
#include <api/dev_request.h>
#include <lib/util/converters.h>
#include <api/timer.h>

#define FILTER_TOUCH_CALIBRATE_SIZE		1

GI::Sensor::Stmpe811::Stmpe811(GI::Dev::Screen *pDisplay, char *i2cpath,
		char *irqPinPath) {
	if (!pDisplay)
		return;
	memset(this, 0, sizeof(*this));
	this->pDisplay = pDisplay;
	GI::Dev::DevRequest::request(i2cpath, &twiStruct);
	GI::Dev::DevRequest::request(irqPinPath, &irqHeandle);

	GI::Sys::Timer::delay(50);

	SysErr err;
	if ((err = reset()))
		return;
	unsigned char mode;

	/* Get the current register value */
	read(STMPE811_REG_SYS_CTRL2, &mode);

	/* Set the Functionalities to be Enabled */
	mode &= ~(STMPE811_IO_FCT);

	/* Write the new register value */
	write(STMPE811_REG_SYS_CTRL2, mode);

	/* Select TSC pins in TSC alternate mode */
	ioEnableAf(STMPE811_TOUCH_IO_ALL);

	/* Set the Functionalities to be Enabled */
	mode &= ~(STMPE811_TS_FCT | STMPE811_ADC_FCT);

	/* Set the new register value */
	write(STMPE811_REG_SYS_CTRL2, mode);

	/* Select Sample Time, bit number and ADC Reference */
	write(STMPE811_REG_ADC_CTRL1, 0x49);

	/* Wait for 2 ms */
	GI::Sys::Timer::delay(2);

	/* Select the ADC clock speed: 3.25 MHz */
	write(STMPE811_REG_ADC_CTRL2, 0x01);

	/* Select 2 nF filter capacitor */
	/* Configuration:
	 - Touch average control    : 8 samples
	 - Touch delay time         : 1000 uS
	 - Panel driver setting time: 500 uS
	 */
	write(STMPE811_REG_TSC_CFG, 0xDB);

	/* Configure the Touch FIFO threshold: single point reading */
	write(STMPE811_REG_FIFO_TH, 0x01);

	/* Clear the FIFO memory content. */
	write(STMPE811_REG_FIFO_STA, 0x01);

	/* Put the FIFO back into operation mode  */
	write(STMPE811_REG_FIFO_STA, 0x00);

	/* Set the range and accuracy pf the pressure measurement (Z) :
	 - Fractional part :7
	 - Whole part      :1
	 */
	write(STMPE811_REG_TSC_FRACT_XYZ, 0x01);

	/* Set the driving capability (limit) of the device for TSC pins: 50mA */
	write(STMPE811_REG_TSC_I_DRIVE, 0x01);

	/* Touch screen control configuration (enable TSC):
	 - No window tracking index
	 - XYZ acquisition mode
	 */
	write(STMPE811_REG_TSC_CTRL, 0x01);

	write(STMPE811_REG_INT_CTRL, 0x01);

	write(STMPE811_REG_INT_EN, 0x01);

	/*  Clear all the status pending bits if any */
	write(STMPE811_REG_INT_STA, 0xFF);
	GI::Sys::Timer::delay(2);
}

GI::Sensor::Stmpe811::~Stmpe811() {
	reset();
}

tControlCommandData *GI::Sensor::Stmpe811::idle() {
	unsigned char State1 = Cursor_NoAction;
	signed short TouchX = -1, TouchY = -1;
	readTouch(&TouchX, &TouchY, true);
	if (isTsPress) {
		State1 = Cursor_Move;
		TouchResponse.x1 = TouchX;
		TouchResponse.y1 = TouchY;
		if (pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE
				|| pDisplay->LcdTimings->orientation
						== LCD_ORIENTATION_LANDSCAPE_FLIP) {
			TouchResponse.y1 = pDisplay->LcdTimings->X - TouchResponse.y1;
			signed int x1_Back = TouchResponse.x1;
			TouchResponse.x1 = TouchResponse.y1;
			TouchResponse.y1 = x1_Back;
			TouchResponse.x1 = pDisplay->LcdTimings->X - TouchResponse.x1;
			TouchResponse.y1 = pDisplay->LcdTimings->Y - TouchResponse.y1;
		}
		if (pDisplay->LcdTimings->orientation == LCD_ORIENTATION_PORTRAIT_FLIP
				|| pDisplay->LcdTimings->orientation
						== LCD_ORIENTATION_LANDSCAPE_FLIP) {
			TouchResponse.x1 = pDisplay->LcdTimings->X - TouchResponse.x1;
			TouchResponse.y1 = pDisplay->LcdTimings->Y - TouchResponse.y1;
		}
		if (flip_x)
			TouchResponse.x1 = pDisplay->LcdTimings->X - TouchResponse.x1;
		if (flip_y)
			TouchResponse.y1 = pDisplay->LcdTimings->Y - TouchResponse.y1;
	}
	if (LastState1 == Cursor_NoAction && State1 == Cursor_Move) {
		LastState1 = Cursor_Down;
		TouchResponse.touch_event1 = Cursor_Down;
	} else if ((LastState1 == Cursor_Down || LastState1 == Cursor_Move)
			&& State1 == Cursor_Move) {
		LastState1 = Cursor_Move;
		TouchResponse.touch_event1 = Cursor_Move;
	} else if ((LastState1 == Cursor_Down || LastState1 == Cursor_Move)
			&& State1 == Cursor_NoAction) {
		LastState1 = Cursor_Up;
		TouchResponse.touch_event1 = Cursor_Up;
	} else if (LastState1 == Cursor_Up && State1 == Cursor_NoAction) {
		LastState1 = Cursor_NoAction;
		TouchResponse.touch_event1 = Cursor_NoAction;
		TouchResponse.x1 = -1;
		TouchResponse.y1 = -1;
	}
	memset(&cursor_ctl, 0, sizeof(tControlCommandData));
	cursor_ctl.Cursor = (CursorState) TouchResponse.touch_event1;
	cursor_ctl.X = TouchResponse.x1;
	cursor_ctl.Y = TouchResponse.y1;
	return &cursor_ctl;
}

SysErr GI::Sensor::Stmpe811::write(unsigned char reg, unsigned char data) {
	if (!twiStruct)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	unsigned char tmp[2];
	tmp[0] = reg;
	tmp[1] = data;
	if (twiStruct->writeBytes(STMPE811_ADDR, tmp, 2) != 2)
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Stmpe811::read(unsigned char reg, unsigned char *data) {
	if (!twiStruct)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	if (twiStruct->writeRead(STMPE811_ADDR, &reg, 1, data, 1))
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Stmpe811::readMultiple(unsigned char reg,
		unsigned char *data, unsigned char len) {
	if (!twiStruct)
		return SYS_ERR_NO_ASSIGNED_DEVICE;
	if (twiStruct->writeRead(STMPE811_ADDR, &reg, 1, data, len))
		return SYS_ERR_BUS;
	return SYS_ERR_OK;
}

SysErr GI::Sensor::Stmpe811::reset() {
	unsigned char data;
	SysErr err = SYS_ERR_OK;
	if ((err = read(STMPE811_SYS_CTRL1, &data)))
		return err;
	data |= 0x02;
	if ((err = write(STMPE811_SYS_CTRL1, data)))
		return err;
	data &= ~(0x02);
	GI::Sys::Timer::delay(10);
	if ((err = write(STMPE811_SYS_CTRL1, data)))
		return err;
	GI::Sys::Timer::delay(2);
	return err;
}

/**
 * @brief  Read the stmpe811 IO Expander device ID.
 * @param  DeviceAddr: Device address on communication Bus.
 * @retval The Device ID (two bytes).
 */
SysErr GI::Sensor::Stmpe811::readId(unsigned short id)
{
	/* Return the device ID value */
	unsigned char dataL;
	unsigned char dataH;
	SysErr err = SYS_ERR_OK;
	if ((err = read(STMPE811_REG_CHP_ID_LSB, &dataL)))
		return err;
	if ((err = read(STMPE811_REG_CHP_ID_MSB, &dataH)))
		return err;
	id = ((dataL << 8) | (dataH));
	return err;
}

/**
 * @brief  Enable the Global interrupt.
 * @param  DeviceAddr: Device address on communication Bus.
 * @retval None
 */
SysErr GI::Sensor::Stmpe811::enableGlobalIt()
{
	unsigned char tmp = 0;
	SysErr err;

	/* Read the Interrupt Control register  */
	if ((err = read(STMPE811_REG_INT_CTRL, &tmp)))
		return err;
	/* Set the global interrupts to be Enabled */
	tmp |= (unsigned char) STMPE811_GIT_EN;

	/* Write Back the Interrupt Control register */
	return write(STMPE811_REG_INT_CTRL, tmp);
}

/**
 * @brief  Disable the Global interrupt.
 * @param  DeviceAddr: Device address on communication Bus.
 * @retval None
 */
SysErr GI::Sensor::Stmpe811::disableGlobalIt()
{
	unsigned char tmp = 0;
	SysErr err;

	/* Read the Interrupt Control register  */
	if ((err = read(STMPE811_REG_INT_CTRL, &tmp)))
		return err;
	/* Set the global interrupts to be Disabled */
	tmp &= ~(unsigned char) STMPE811_GIT_EN;

	/* Write Back the Interrupt Control register */
	return write(STMPE811_REG_INT_CTRL, tmp);
}

/**
 * @brief  Enable the interrupt mode for the selected IT source
 * @param  DeviceAddr: Device address on communication Bus.
 * @param Source: The interrupt source to be configured, could be:
 *   @arg  STMPE811_GIT_IO: IO interrupt
 *   @arg  STMPE811_GIT_ADC : ADC interrupt
 *   @arg  STMPE811_GIT_FE : Touch Screen Controller FIFO Error interrupt
 *   @arg  STMPE811_GIT_FF : Touch Screen Controller FIFO Full interrupt
 *   @arg  STMPE811_GIT_FOV : Touch Screen Controller FIFO Overrun interrupt
 *   @arg  STMPE811_GIT_FTH : Touch Screen Controller FIFO Threshold interrupt
 *   @arg  STMPE811_GIT_TOUCH : Touch Screen Controller Touch Detected interrupt
 * @retval None
 */
SysErr GI::Sensor::Stmpe811::enableItSource(unsigned char source)
{
	unsigned char tmp = 0;
	SysErr err;

	/* Get the current value of the INT_EN register */
	if ((err = read(STMPE811_REG_INT_EN, &tmp)))
		return err;
	/* Set the interrupts to be Enabled */
	tmp |= source;

	/* Set the register */
	return write(STMPE811_REG_INT_EN, tmp);
}

/**
 * @brief  Disable the interrupt mode for the selected IT source
 * @param  DeviceAddr: Device address on communication Bus.
 * @param  Source: The interrupt source to be configured, could be:
 *   @arg  STMPE811_GIT_IO: IO interrupt
 *   @arg  STMPE811_GIT_ADC : ADC interrupt
 *   @arg  STMPE811_GIT_FE : Touch Screen Controller FIFO Error interrupt
 *   @arg  STMPE811_GIT_FF : Touch Screen Controller FIFO Full interrupt
 *   @arg  STMPE811_GIT_FOV : Touch Screen Controller FIFO Overrun interrupt
 *   @arg  STMPE811_GIT_FTH : Touch Screen Controller FIFO Threshold interrupt
 *   @arg  STMPE811_GIT_TOUCH : Touch Screen Controller Touch Detected interrupt
 * @retval None
 */
SysErr GI::Sensor::Stmpe811::disableItSource(unsigned char source)
{
	unsigned char tmp = 0;
	SysErr err;

	/* Get the current value of the INT_EN register */
	if ((err = read(STMPE811_REG_INT_EN, &tmp)))
		return err;
	/* Set the interrupts to be Enabled */
	tmp &= ~source;

	/* Set the register */
	return write(STMPE811_REG_INT_EN, tmp);
}

/**
 * @brief  Set the global interrupt Polarity.
 * @param  DeviceAddr: Device address on communication Bus.
 * @param  Polarity: the IT mode polarity, could be one of the following values:
 *   @arg  STMPE811_POLARITY_LOW: Interrupt line is active Low/Falling edge
 *   @arg  STMPE811_POLARITY_HIGH: Interrupt line is active High/Rising edge
 * @retval None
 */
SysErr GI::Sensor::Stmpe811::setItPolarity(unsigned char polarity)
{
	unsigned char tmp = 0;
	SysErr err;

	/* Get the current register value */
	if ((err = read(STMPE811_REG_INT_CTRL, &tmp)))
		return err;
	/* Mask the polarity bits */
	tmp &= ~(unsigned char) 0x04;

	/* Modify the Interrupt Output line configuration */
	tmp |= polarity;

	/* Set the new register value */
	return write(STMPE811_REG_INT_CTRL, tmp);
}

/**
 * @brief  Set the global interrupt Type.
 * @param  DeviceAddr: Device address on communication Bus.
 * @param  Type: Interrupt line activity type, could be one of the following values:
 *   @arg  STMPE811_TYPE_LEVEL: Interrupt line is active in level model
 *   @arg  STMPE811_TYPE_EDGE: Interrupt line is active in edge model
 * @retval None
 */
SysErr GI::Sensor::Stmpe811::setItType(unsigned char type)
{
	unsigned char tmp = 0;
	SysErr err;

	/* Get the current register value */
	if ((err = read(STMPE811_REG_INT_CTRL, &tmp)))
		return err;
	/* Mask the type bits */
	tmp &= ~(unsigned char) 0x02;

	/* Modify the Interrupt Output line configuration */
	tmp |= type;

	/* Set the new register value */
	return write(STMPE811_REG_INT_CTRL, tmp);
}

/**
 * @brief  Check the selected Global interrupt source pending bit
 * @param  DeviceAddr: Device address on communication Bus.
 * @param  Source: the Global interrupt source to be checked, could be:
 *   @arg  STMPE811_GIT_IO: IO interrupt
 *   @arg  STMPE811_GIT_ADC : ADC interrupt
 *   @arg  STMPE811_GIT_FE : Touch Screen Controller FIFO Error interrupt
 *   @arg  STMPE811_GIT_FF : Touch Screen Controller FIFO Full interrupt
 *   @arg  STMPE811_GIT_FOV : Touch Screen Controller FIFO Overrun interrupt
 *   @arg  STMPE811_GIT_FTH : Touch Screen Controller FIFO Threshold interrupt
 *   @arg  STMPE811_GIT_TOUCH : Touch Screen Controller Touch Detected interrupt
 * @retval The checked Global interrupt source status.
 */
SysErr GI::Sensor::Stmpe811::globalItStatus(unsigned char source, bool *status)
{
	unsigned char tmp = 0;
	SysErr err;
	/* Return the global IT source status */
	if ((err = read(STMPE811_REG_INT_STA, &tmp)))
		return err;
	if((tmp & source) == source)
		*status = true;
	else
		*status = false;
	return err;
}

/**
 * @brief  Return the Global interrupts status
 * @param  DeviceAddr: Device address on communication Bus.
 * @param  Source: the Global interrupt source to be checked, could be:
 *   @arg  STMPE811_GIT_IO: IO interrupt
 *   @arg  STMPE811_GIT_ADC : ADC interrupt
 *   @arg  STMPE811_GIT_FE : Touch Screen Controller FIFO Error interrupt
 *   @arg  STMPE811_GIT_FF : Touch Screen Controller FIFO Full interrupt
 *   @arg  STMPE811_GIT_FOV : Touch Screen Controller FIFO Overrun interrupt
 *   @arg  STMPE811_GIT_FTH : Touch Screen Controller FIFO Threshold interrupt
 *   @arg  STMPE811_GIT_TOUCH : Touch Screen Controller Touch Detected interrupt
 * @retval The checked Global interrupt source status.
 */
SysErr GI::Sensor::Stmpe811::getGlobalItStatus(unsigned char source, unsigned char *status)
{
	unsigned char tmp = 0;
	SysErr err;
	/* Return the global IT source status */
	if ((err = read(STMPE811_REG_INT_STA, &tmp)))
		return err;
	*status = tmp & source;
	return err;
}

/**
 * @brief  Clear the selected Global interrupt pending bit(s)
 * @param  DeviceAddr: Device address on communication Bus.
 * @param  Source: the Global interrupt source to be cleared, could be any combination
 *         of the following values:
 *   @arg  STMPE811_GIT_IO: IO interrupt
 *   @arg  STMPE811_GIT_ADC : ADC interrupt
 *   @arg  STMPE811_GIT_FE : Touch Screen Controller FIFO Error interrupt
 *   @arg  STMPE811_GIT_FF : Touch Screen Controller FIFO Full interrupt
 *   @arg  STMPE811_GIT_FOV : Touch Screen Controller FIFO Overrun interrupt
 *   @arg  STMPE811_GIT_FTH : Touch Screen Controller FIFO Threshold interrupt
 *   @arg  STMPE811_GIT_TOUCH : Touch Screen Controller Touch Detected interrupt
 * @retval None
 */
SysErr GI::Sensor::Stmpe811::clearGlobalIt(unsigned char source)
{
	/* Write 1 to the bits that have to be cleared */
	return write(STMPE811_REG_INT_STA, source);
}

/**
 * @brief  Start the IO functionality use and disable the AF for selected IO pin(s).
 * @param  DeviceAddr: Device address on communication Bus.
 * @param  IO_Pin: The IO pin(s) to put in AF. This parameter can be one
 *         of the following values:
 *   @arg  STMPE811_PIN_x: where x can be from 0 to 7.
 * @retval None
 */
SysErr GI::Sensor::Stmpe811::ioStart(unsigned char ioPin)
{
	unsigned char mode;
	SysErr err;

	/* Get the current register value */
	if ((err = read(STMPE811_REG_SYS_CTRL2, &mode)))
		return err;
	/* Set the Functionalities to be Disabled */
	mode &= ~(STMPE811_IO_FCT | STMPE811_ADC_FCT);

	/* Write the new register value */
	if ((err = write(STMPE811_REG_SYS_CTRL2, mode)))
			return err;
	/* Disable AF for the selected IO pin(s) */
	return ioDisableAf(ioPin);
}

/**
 * @brief  Configures the IO pin(s) according to IO mode structure value.
 * @param  DeviceAddr: Device address on communication Bus.
 * @param  IO_Pin: The output pin to be set or reset. This parameter can be one
 *         of the following values:
 *   @arg  STMPE811_PIN_x: where x can be from 0 to 7.
 * @param  IO_Mode: The IO pin mode to configure, could be one of the following values:
 *   @arg  IO_MODE_INPUT
 *   @arg  IO_MODE_OUTPUT
 *   @arg  IO_MODE_IT_RISING_EDGE
 *   @arg  IO_MODE_IT_FALLING_EDGE
 *   @arg  IO_MODE_IT_LOW_LEVEL
 *   @arg  IO_MODE_IT_HIGH_LEVEL
 * @retval 0 if no error, IO_Mode if error
 */
SysErr GI::Sensor::Stmpe811::ioConfig(unsigned char ioPin, GI::Sensor::Stmpe811::gpioMode_e ioMode)
{
	SysErr err;

	/* Configure IO pin according to selected IO mode */
	switch (ioMode) {
	case GPIO_IN_FLOATING: /* Input mode */
		if ((err = ioInitPin(ioPin, STMPE811_DIRECTION_IN)))
			return err;
		break;

	case GPIO_OUT_PUSH_PULL: /* Output mode */
		if ((err = ioInitPin(ioPin, STMPE811_DIRECTION_OUT)))
			return err;
		break;

	case GPIO_MODE_IT_RISING_EDGE: /* Interrupt rising edge mode */
		if ((err = ioEnableIt()))
			return err;
		if ((err = ioEnablePinIt(ioPin)))
			return err;
		if ((err = ioInitPin(ioPin, STMPE811_DIRECTION_IN)))
			return err;
		if ((err = setItType(STMPE811_TYPE_EDGE)))
			return err;
		if ((err = ioSetEdgeMode(ioPin, STMPE811_EDGE_RISING)))
			return err;
		break;

	case GPIO_MODE_IT_FALLING_EDGE: /* Interrupt falling edge mode */
		if ((err = ioEnableIt()))
			return err;
		if ((err = ioEnablePinIt(ioPin)))
			return err;
		if ((err = ioInitPin(ioPin, STMPE811_DIRECTION_IN)))
			return err;
		if ((err = setItType(STMPE811_TYPE_EDGE)))
			return err;
		if ((err = ioSetEdgeMode(ioPin, STMPE811_EDGE_FALLING)))
			return err;
		break;

	case GPIO_MODE_IT_LOW_LEVEL: /* Low level interrupt mode */
		if ((err = ioEnableIt()))
			return err;
		if ((err = ioEnablePinIt(ioPin)))
			return err;
		if ((err = ioInitPin(ioPin, STMPE811_DIRECTION_IN)))
			return err;
		if ((err = setItType(STMPE811_TYPE_LEVEL)))
			return err;
		if ((err = setItPolarity(STMPE811_POLARITY_LOW)))
			return err;
		break;

	case GPIO_MODE_IT_HIGH_LEVEL: /* High level interrupt mode */
		if ((err = ioEnableIt()))
			return err;
		if ((err = ioEnablePinIt(ioPin)))
			return err;
		if ((err = ioInitPin(ioPin, STMPE811_DIRECTION_IN)))
			return err;
		if ((err = setItType(STMPE811_TYPE_LEVEL)))
			return err;
		if ((err = setItPolarity(STMPE811_POLARITY_HIGH)))
			return err;
		break;

	default:
		err = SYS_ERR_NOT_IMPLEMENTED;
		break;
	}
	return err;
}

/**
 * @brief  Initialize the selected IO pin direction.
 * @param  DeviceAddr: Device address on communication Bus.
 * @param  IO_Pin: The IO pin to be configured. This parameter could be any
 *         combination of the following values:
 *   @arg  STMPE811_PIN_x: Where x can be from 0 to 7.
 * @param  Direction: could be STMPE811_DIRECTION_IN or STMPE811_DIRECTION_OUT.
 * @retval None
 */
SysErr GI::Sensor::Stmpe811::ioInitPin(unsigned char ioPin, unsigned char direction)
{
	unsigned char tmp = 0;
	SysErr err;

	/* Get all the Pins direction */
	if ((err = read(STMPE811_REG_IO_DIR, &tmp)))
		return err;
	/* Set the selected pin direction */
	if (direction != STMPE811_DIRECTION_IN) {
		tmp |= (unsigned char) ioPin;
	} else {
		tmp &= ~(unsigned char) ioPin;
	}

	/* Write the register new value */
	return write(STMPE811_REG_IO_DIR, tmp);
}

/**
 * @brief  Disable the AF for the selected IO pin(s).
 * @param  DeviceAddr: Device address on communication Bus.
 * @param  IO_Pin: The IO pin to be configured. This parameter could be any
 *         combination of the following values:
 *   @arg  STMPE811_PIN_x: Where x can be from 0 to 7.
 * @retval None
 */
SysErr GI::Sensor::Stmpe811::ioDisableAf(unsigned char ioPin)
{
	unsigned char tmp = 0;
	SysErr err;

	/* Get the current state of the IO_AF register */
	if ((err = read(STMPE811_REG_IO_AF, &tmp)))
		return err;
	/* Enable the selected pins alternate function */
	tmp |= ioPin;

	/* Write back the new value in IO AF register */
	return write(STMPE811_REG_IO_AF, tmp);

}

/**
 * @brief  Enable the AF for the selected IO pin(s).
 * @param  DeviceAddr: Device address on communication Bus.
 * @param  IO_Pin: The IO pin to be configured. This parameter could be any
 *         combination of the following values:
 *   @arg  STMPE811_PIN_x: Where x can be from 0 to 7.
 * @retval None
 */
SysErr GI::Sensor::Stmpe811::ioEnableAf(unsigned char ioPin)
{
	unsigned char tmp = 0;
	SysErr err;

	/* Get the current register value */
	if ((err = read(STMPE811_REG_IO_AF, &tmp)))
		return err;
	/* Enable the selected pins alternate function */
	tmp &= ~ioPin;

	/* Write back the new register value */
	return write(STMPE811_REG_IO_AF, tmp);
}

/**
 * @brief  Configure the Edge for which a transition is detectable for the
 *         selected pin.
 * @param  DeviceAddr: Device address on communication Bus.
 * @param  IO_Pin: The IO pin to be configured. This parameter could be any
 *         combination of the following values:
 *   @arg  STMPE811_PIN_x: Where x can be from 0 to 7.
 * @param  Edge: The edge which will be detected. This parameter can be one or
 *         a combination of following values: STMPE811_EDGE_FALLING and STMPE811_EDGE_RISING .
 * @retval None
 */
SysErr GI::Sensor::Stmpe811::ioSetEdgeMode(unsigned char ioPin, unsigned char Edge)
{
	unsigned char tmp1 = 0, tmp2 = 0;
	SysErr err;

	/* Get the current registers values */
	if ((err = read(STMPE811_REG_IO_FE, &tmp1)))
				return err;
	if ((err = read(STMPE811_REG_IO_RE, &tmp2)))
			return err;

	/* Disable the Falling Edge */
	tmp1 &= ~(unsigned char) ioPin;

	/* Disable the Falling Edge */
	tmp2 &= ~(unsigned char) ioPin;

	/* Enable the Falling edge if selected */
	if (Edge & STMPE811_EDGE_FALLING) {
		tmp1 |= (unsigned char) ioPin;
	}

	/* Enable the Rising edge if selected */
	if (Edge & STMPE811_EDGE_RISING) {
		tmp2 |= (unsigned char) ioPin;
	}

	/* Write back the new registers values */
	if ((err = write(STMPE811_REG_IO_FE, tmp1)))
			return err;
	return write(STMPE811_REG_IO_RE, tmp2);
}

/**
 * @brief  Write a new IO pin state.
 * @param  DeviceAddr: Device address on communication Bus.
 * @param IO_Pin: The output pin to be set or reset. This parameter can be one
 *        of the following values:
 *   @arg  STMPE811_PIN_x: where x can be from 0 to 7.
 * @param PinState: The new IO pin state.
 * @retval None
 */
SysErr GI::Sensor::Stmpe811::ioWritePin(unsigned char ioPin, unsigned char pinState)
{
	SysErr err = SYS_ERR_OK;
	/* Apply the bit value to the selected pin */
	if (pinState != 0) {
		/* Set the register */
		if ((err = write(STMPE811_REG_IO_SET_PIN, ioPin)))
			return err;
	} else {
		/* Set the register */
		if ((err = write(STMPE811_REG_IO_CLR_PIN, ioPin)))
		return err;
	}
	return err;
}

/**
 * @brief  Return the state of the selected IO pin(s).
 * @param  DeviceAddr: Device address on communication Bus.
 * @param IO_Pin: The output pin to be set or reset. This parameter can be one
 *        of the following values:
 *   @arg  STMPE811_PIN_x: where x can be from 0 to 7.
 * @retval IO pin(s) state.
 */
SysErr GI::Sensor::Stmpe811::ioReadPin(unsigned char ioPin, unsigned char *ioPinStatus)
{
	unsigned char tmp = 0;
	SysErr err;
	if ((err = read(STMPE811_REG_IO_MP_STA, &tmp)))
		return err;
	*ioPinStatus = tmp & ioPin;
	return err;
}

/**
 * @brief  Enable the global IO interrupt source.
 * @param  DeviceAddr: Device address on communication Bus.
 * @retval None
 */
SysErr GI::Sensor::Stmpe811::ioEnableIt()
{
	SysErr err;
	//IOE_ITConfig();

	/* Enable global IO IT source */
	if ((err = enableItSource(STMPE811_GIT_IO)))
			return err;

	/* Enable global interrupt */
	return enableGlobalIt();
}

/**
 * @brief  Disable the global IO interrupt source.
 * @param  DeviceAddr: Device address on communication Bus.
 * @retval None
 */
SysErr GI::Sensor::Stmpe811::ioDisableIt()
{
	SysErr err;
	/* Disable the global interrupt */
	if ((err = disableGlobalIt()))
			return err;

	/* Disable global IO IT source */
	return disableItSource(STMPE811_GIT_IO);
}

/**
 * @brief  Enable interrupt mode for the selected IO pin(s).
 * @param  DeviceAddr: Device address on communication Bus.
 * @param  IO_Pin: The IO interrupt to be enabled. This parameter could be any
 *         combination of the following values:
 *   @arg  STMPE811_PIN_x: where x can be from 0 to 7.
 * @retval None
 */
SysErr GI::Sensor::Stmpe811::ioEnablePinIt(unsigned char ioPin)
{
	unsigned char tmp = 0;
	SysErr err;

	/* Get the IO interrupt state */
	if ((err = read(STMPE811_REG_IO_INT_EN, &tmp)))
		return err;
	/* Set the interrupts to be enabled */
	tmp |= ioPin;

	/* Write the register new value */
	return write(STMPE811_REG_IO_INT_EN, tmp);
}

/**
 * @brief  Disable interrupt mode for the selected IO pin(s).
 * @param  DeviceAddr: Device address on communication Bus.
 * @param  IO_Pin: The IO interrupt to be disabled. This parameter could be any
 *         combination of the following values:
 *   @arg  STMPE811_PIN_x: where x can be from 0 to 7.
 * @retval None
 */
SysErr GI::Sensor::Stmpe811::ioDisablePinIt(unsigned char ioPin)
{
	unsigned char tmp = 0;
	SysErr err;

	/* Get the IO interrupt state */
	if ((err = read(STMPE811_REG_IO_INT_EN, &tmp)))
		return err;
	/* Set the interrupts to be Disabled */
	tmp &= ~ioPin;

	/* Write the register new value */
	return write(STMPE811_REG_IO_INT_EN, tmp);
}

/**
 * @brief  Check the status of the selected IO interrupt pending bit
 * @param  DeviceAddr: Device address on communication Bus.
 * @param  IO_Pin: The IO interrupt to be checked could be:
 *   @arg  STMPE811_PIN_x Where x can be from 0 to 7.
 * @retval Status of the checked IO pin(s).
 */
SysErr GI::Sensor::Stmpe811::ioItStatus(unsigned char ioPin, unsigned char *ioItPinStatus)
{
	unsigned char tmp = 0;
	SysErr err;
	/* Get the Interrupt status */
	if ((err = read(STMPE811_REG_IO_INT_STA, &tmp)))
		return err;
	*ioItPinStatus = tmp & ioPin;
	return err;
}

/**
 * @brief  Clear the selected IO interrupt pending bit(s).
 * @param  DeviceAddr: Device address on communication Bus.
 * @param  IO_Pin: the IO interrupt to be cleared, could be:
 *   @arg  STMPE811_PIN_x: Where x can be from 0 to 7.
 * @retval None
 */
SysErr GI::Sensor::Stmpe811::ioClearIt(unsigned char ioPin)
{
	SysErr err;
	/* Clear the global IO IT pending bit */
	if ((err = clearGlobalIt(STMPE811_GIT_IO)))
		return err;

	/* Clear the IO IT pending bit(s) */
	if ((err = write(STMPE811_REG_IO_INT_STA, ioPin)))
			return err;

	/* Clear the Edge detection pending bit*/
	if ((err = write(STMPE811_REG_IO_ED, ioPin)))
			return err;

	/* Clear the Rising edge pending bit */
	if ((err = write(STMPE811_REG_IO_RE, ioPin)))
					return err;

	/* Clear the Falling edge pending bit */
	return write(STMPE811_REG_IO_FE, ioPin);
}

SysErr GI::Sensor::Stmpe811::readTouch(signed short *x, signed short *y, bool applyScale)
{
	SysErr err;
	unsigned char st;

	if (!irqHeandle->in()) {
		if ((err = read(STMPE811_REG_TSC_CTRL, &st)))
			return err;
		if (st & STMPE811_TS_CTRL_STATUS) {
			/* Reset FIFO */
			if ((err = write(STMPE811_REG_FIFO_STA, 0x01)))
				return err;
			/* Enable the FIFO again */
			if ((err = write(STMPE811_REG_FIFO_STA, 0x00)))
				return err;
			intIsPressed = true;
		} else {
			intIsPressed = false;
			isTsPress = false;
		}
		/* Reset all interrupt */
		if ((err = write(STMPE811_REG_INT_STA, 0xFF)))
			return err;
	}
	if (intIsPressed) {
		if ((err = read(STMPE811_REG_FIFO_SIZE, &st)))
			return err;
		if (st > 0) {
			unsigned char dataXYZ[4];
			unsigned long uldataXYZ;

			if ((err = readMultiple(STMPE811_REG_TSC_DATA_NON_INC, dataXYZ, sizeof(dataXYZ))))
				return err;

			/* Calculate positions values */
			uldataXYZ = ((unsigned long)dataXYZ[0] << 24) | ((unsigned long)dataXYZ[1] << 16)
					| (dataXYZ[2] << 8) | (dataXYZ[3] << 0);
			*x = (uldataXYZ >> 20) & 0x00000FFF;
			*y = (uldataXYZ >> 8) & 0x00000FFF;
			double scaleX = 4095, scaleY = 4095;
			if (applyScale) {
				scaleX = (double) pDisplay->LcdTimings->X;
				scaleY = (double) pDisplay->LcdTimings->Y;
			}
			*x = (unsigned short) GUtil::Converters::toPercent(
					LcdTouch_L_Calibration_Value, LcdTouch_R_Calibration_Value,
					scaleX, (double) *x);
			*y = (unsigned short) GUtil::Converters::toPercent(
					LcdTouch_U_Calibration_Value, LcdTouch_D_Calibration_Value,
					scaleY, (double) *y);
			/* Reset FIFO */
			if ((err = write(STMPE811_REG_FIFO_STA, 0x01)))
				return err;
			/* Enable the FIFO again */
			if ((err = write(STMPE811_REG_FIFO_STA, 0x00)))
				return err;
			isTsPress = true;
		}
	}
	return SYS_ERR_OK;
}

#define STMPE811_TIME	20

void GI::Sensor::Stmpe811::touchCalibrate()
{
	GI::Sys::Timer TimerTouchCalibrate(25);
	pDisplay->drawRectangle(0, 0, pDisplay->LcdTimings->X,
			pDisplay->LcdTimings->Y, true, ClrBlack);
	pDisplay->cacheClean(0, 0, pDisplay->LcdTimings->X,
			pDisplay->LcdTimings->Y);
	signed short TouchX = 0, TouchY = 0;
	//double screen_max_x = 4095;
	//double screen_max_y = 4095;
	LcdTouch_L_Calibration_Value = 0;
	LcdTouch_R_Calibration_Value = 4095;
	LcdTouch_U_Calibration_Value = 0;
	LcdTouch_D_Calibration_Value = 4095;
	double LcdTouch_L_Calibration_Value;
	double LcdTouch_R_Calibration_Value;
	double LcdTouch_U_Calibration_Value;
	double LcdTouch_D_Calibration_Value;

	pDisplay->drawTouchPoint(
			(((double) pDisplay->LcdTimings->X * (double) 12.5) / (double) 100),
			(((double) pDisplay->LcdTimings->Y * (double) 12.5) / (double) 100),
			ClrRed);
	pDisplay->cacheClean(0, 0, pDisplay->LcdTimings->X,
			pDisplay->LcdTimings->Y);

	readTouch(&TouchX, &TouchY, false);
	while (!isTsPress) {
		readTouch(&TouchX, &TouchY, false);
		GI::Sys::Timer::delay(STMPE811_TIME);
	}
	LcdTouch_L_Calibration_Value = (double) TouchX;
	LcdTouch_U_Calibration_Value = (double) TouchY;
	readTouch(&TouchX, &TouchY, false);
	while (isTsPress) {
		readTouch(&TouchX, &TouchY, false);
		GI::Sys::Timer::delay(STMPE811_TIME);
	}
	pDisplay->drawRectangle(0, 0, pDisplay->LcdTimings->X,
			pDisplay->LcdTimings->Y, true, ClrBlack);
	pDisplay->cacheClean(0, 0, pDisplay->LcdTimings->X,
			pDisplay->LcdTimings->Y);

	pDisplay->drawTouchPoint(
			(double) pDisplay->LcdTimings->X
					- (((double) pDisplay->LcdTimings->X * (double) 12.5)
							/ (double) 100),
			(((double) pDisplay->LcdTimings->Y * (double) 12.5) / (double) 100),
			ClrRed);
	pDisplay->cacheClean(0, 0, pDisplay->LcdTimings->X,
			pDisplay->LcdTimings->Y);

	readTouch(&TouchX, &TouchY, false);
	while (!isTsPress) {
		readTouch(&TouchX, &TouchY, false);
		GI::Sys::Timer::delay(STMPE811_TIME);
	}

	LcdTouch_R_Calibration_Value = (double) TouchX;
	LcdTouch_U_Calibration_Value += (double) TouchY;
	readTouch(&TouchX, &TouchY, false);
	while (isTsPress) {
		readTouch(&TouchX, &TouchY, false);
		GI::Sys::Timer::delay(STMPE811_TIME);
	}
	pDisplay->drawRectangle(0, 0, pDisplay->LcdTimings->X,
			pDisplay->LcdTimings->Y, true, ClrBlack);
	pDisplay->cacheClean(0, 0, pDisplay->LcdTimings->X,
			pDisplay->LcdTimings->Y);

	pDisplay->drawTouchPoint(
			(double) pDisplay->LcdTimings->X
					- (((double) pDisplay->LcdTimings->X * (double) 12.5)
							/ (double) 100),
			(double) pDisplay->LcdTimings->Y
					- (((double) pDisplay->LcdTimings->Y * (double) 12.5)
							/ (double) 100), ClrRed);
	pDisplay->cacheClean(0, 0, pDisplay->LcdTimings->X,
			pDisplay->LcdTimings->Y);

	readTouch(&TouchX, &TouchY, false);
	while (!isTsPress) {
		readTouch(&TouchX, &TouchY, false);
		GI::Sys::Timer::delay(STMPE811_TIME);
	}

	LcdTouch_R_Calibration_Value += (double) TouchX;
	LcdTouch_D_Calibration_Value = (double) TouchY;
	readTouch(&TouchX, &TouchY, false);
	while (isTsPress) {
		readTouch(&TouchX, &TouchY, false);
		GI::Sys::Timer::delay(STMPE811_TIME);
	}
	pDisplay->drawRectangle(0, 0, pDisplay->LcdTimings->X,
			pDisplay->LcdTimings->Y, true, ClrBlack);
	pDisplay->cacheClean(0, 0, pDisplay->LcdTimings->X,
			pDisplay->LcdTimings->Y);

	pDisplay->drawTouchPoint(
			(((double) pDisplay->LcdTimings->X * (double) 12.5) / (double) 100),
			(double) pDisplay->LcdTimings->Y
					- (((double) pDisplay->LcdTimings->Y * (double) 12.5)
							/ (double) 100), ClrRed);
	pDisplay->cacheClean(0, 0, pDisplay->LcdTimings->X,
			pDisplay->LcdTimings->Y);

	readTouch(&TouchX, &TouchY, false);
	while (!isTsPress) {
		readTouch(&TouchX, &TouchY, false);
		GI::Sys::Timer::delay(STMPE811_TIME);
	}

	LcdTouch_L_Calibration_Value += (double) TouchX;
	LcdTouch_D_Calibration_Value += (double) TouchY;
	readTouch(&TouchX, &TouchY, false);
	while (isTsPress) {
		readTouch(&TouchX, &TouchY, false);
		GI::Sys::Timer::delay(STMPE811_TIME);
	}
	pDisplay->drawRectangle(0, 0, pDisplay->LcdTimings->X,
			pDisplay->LcdTimings->Y, true, ClrBlack);
	pDisplay->cacheClean(0, 0, pDisplay->LcdTimings->X,
			pDisplay->LcdTimings->Y);

	LcdTouch_L_Calibration_Value /= 2.0;
	LcdTouch_R_Calibration_Value /= 2.0;
	LcdTouch_U_Calibration_Value /= 2.0;
	LcdTouch_D_Calibration_Value /= 2.0;

	double X_middle =
			LcdTouch_L_Calibration_Value
					+ ((LcdTouch_R_Calibration_Value
							- LcdTouch_L_Calibration_Value) / 2);
	LcdTouch_L_Calibration_Value = X_middle
			- ((X_middle - LcdTouch_L_Calibration_Value) * 1.33);
	LcdTouch_R_Calibration_Value = X_middle
			+ ((LcdTouch_R_Calibration_Value - X_middle) * 1.33);
	double Y_middle =
			LcdTouch_U_Calibration_Value
					+ ((LcdTouch_D_Calibration_Value
							- LcdTouch_U_Calibration_Value) / 2);
	LcdTouch_U_Calibration_Value = Y_middle
			- ((Y_middle - LcdTouch_U_Calibration_Value) * 1.33);
	LcdTouch_D_Calibration_Value = Y_middle
			+ ((LcdTouch_D_Calibration_Value - Y_middle) * 1.33);

	this->LcdTouch_L_Calibration_Value = LcdTouch_L_Calibration_Value;
	this->LcdTouch_R_Calibration_Value = LcdTouch_R_Calibration_Value;
	this->LcdTouch_U_Calibration_Value = LcdTouch_U_Calibration_Value;
	this->LcdTouch_D_Calibration_Value = LcdTouch_D_Calibration_Value;
}

