/*
 * device/stmpe811.h
 */

#ifndef LIB_DEVICE_STMPE811_H_
#define LIB_DEVICE_STMPE811_H_

#include <stdbool.h>
#include <api/cursor_ctl_def.h>
#include <include/global.h>


/* Chip IDs */
#define STMPE811_ID                     0x0811


/* Private defines */
/* I2C address */
#define STMPE811_ADDR					0x41

/* STMPE811 Chip ID on reset */
#define STMPE811_CHIP_ID_VALUE			0x0811	/* Chip ID */

/* Registers */
#define STMPE811_CHIP_ID	0x00	/* STMPE811 Device identification */
#define STMPE811_ID_VER		0x02	/* STMPE811 Revision number */
#define STMPE811_SYS_CTRL1	0x03	/* Reset control */
#define STMPE811_SYS_CTRL2	0x04	/* Clock control */
#define STMPE811_SPI_CFG	0x08	/* SPI interface configuration */
#define STMPE811_INT_CTRL	0x09	/* Interrupt control register */
#define STMPE811_INT_EN		0x0A	/* Interrupt enable register */
#define STMPE811_INT_STA	0x0B	/* Interrupt status register */
#define STMPE811_GPIO_EN	0x0C	/* GPIO interrupt enable register */
#define STMPE811_GPIO_INT_STA	0x0D	/* GPIO interrupt status register */
#define STMPE811_ADC_INT_EN	0x0E	/* ADC interrupt enable register */
#define STMPE811_ADC_INT_STA	0x0F	/* ADC interface status register */
#define STMPE811_GPIO_SET_PIN	0x10	/* GPIO set pin register */
#define STMPE811_GPIO_CLR_PIN	0x11	/* GPIO clear pin register */
#define STMPE811_MP_STA		0x12	/* GPIO monitor pin state register */
#define STMPE811_GPIO_DIR	0x13	/* GPIO direction register */
#define STMPE811_GPIO_ED	0x14	/* GPIO edge detect register */
#define STMPE811_GPIO_RE	0x15	/* GPIO rising edge register */
#define STMPE811_GPIO_FE	0x16	/* GPIO falling edge register */
#define STMPE811_GPIO_AF	0x17	/* Alternate function register */
#define STMPE811_ADC_CTRL1	0x20	/* ADC control */
#define STMPE811_ADC_CTRL2	0x21	/* ADC control */
#define STMPE811_ADC_CAPT	0x22	/* To initiate ADC data acquisition */
#define STMPE811_ADC_DATA_CHO	0x30	/* ADC channel 0 */
#define STMPE811_ADC_DATA_CH1	0x32	/* ADC channel 1 */
#define STMPE811_ADC_DATA_CH2	0x34	/* ADC channel 2 */
#define STMPE811_ADC_DATA_CH3	0x36	/* ADC channel 3 */
#define STMPE811_ADC_DATA_CH4	0x38	/* ADC channel 4 */
#define STMPE811_ADC_DATA_CH5	0x3A	/* ADC channel 5 */
#define STMPE811_ADC_DATA_CH6	0x3C	/* ADC channel 6 */
#define STMPE811_ADC_DATA_CH7	0x3E	/* ADC channel 7 */
#define STMPE811_TSC_CTRL	0x40	/* 4-wire tsc setup */
#define STMPE811_TSC_CFG	0x41	/* Tsc configuration */
#define STMPE811_WDW_TR_X	0x42	/* Window setup for top right X */
#define STMPE811_WDW_TR_Y	0x44	/* Window setup for top right Y */
#define STMPE811_WDW_BL_X	0x46	/* Window setup for bottom left X */
#define STMPE811_WDW_BL_Y	0x48	/* Window setup for bottom left Y */
#define STMPE811_FIFO_TH	0x4A	/* FIFO level to generate interrupt */
#define STMPE811_FIFO_STA	0x4B	/* Current status of FIFO */
#define STMPE811_FIFO_SIZE	0x4C	/* Current filled level of FIFO */
#define STMPE811_TSC_DATA_X	0x4D	/* Data port for tsc data access */
#define STMPE811_TSC_DATA_Y	0x4F	/* Data port for tsc data access */
#define STMPE811_TSC_DATA_Z	0x51	/* Data port for tsc data access */
#define STMPE811_TSC_DATA_XYZ	0x52	/* Data port for tsc data access */
#define STMPE811_TSC_FRACTION_Z	0x56	/* Touchscreen controller FRACTION_Z */
#define STMPE811_TSC_DATA	0x57	/* Data port for tsc data access */
#define STMPE811_TSC_I_DRIVE	0x58	/* Touchscreen controller drivel */
#define STMPE811_TSC_SHIELD	0x59	/* Touchscreen controller shield */
#define STMPE811_TEMP_CTRL	0x60	/* Temperature sensor setup */
#define STMPE811_TEMP_DATA	0x61	/* Temperature data access port */
#define STMPE811_TEMP_TH	0x62	/* Threshold for temp controlled int */

#define STMPE811_SYS_CTRL2_ADC_OFF	(1 << 0)
#define STMPE811_SYS_CTRL2_TSC_OFF	(1 << 1)
#define STMPE811_SYS_CTRL2_GPIO_OFF	(1 << 2)
#define STMPE811_SYS_CTRL2_TS_OFF	(1 << 3)

#define STMPE811_TEMP_CTRL_EN		(1 << 0)
#define STMPE811_TEMP_CTRL_ACQ		(1 << 1)
#define STMPE811_TEMP_DATA_MSB_MASK	0x03

#define STMPE811_TSC_CTRL_EN		(1 << 0)

#define STMPE811_INT_EN_TOUCH_DET	(1 << 0)
#define STMPE811_INT_EN_FIFO_TH		(1 << 1)
#define STMPE811_INT_EN_FIFO_OFLOW	(1 << 2)
#define STMPE811_INT_EN_FIFO_FULL	(1 << 3)
#define STMPE811_INT_EN_FIFO_EMPTY	(1 << 4)
#define STMPE811_INT_EN_TEMP_SENS	(1 << 5)
#define STMPE811_INT_EN_ADC		(1 << 6)
#define STMPE811_INT_EN_GPIO		(1 << 7)

#define STMPE811_SYS_CTRL1_SOFT_RESET	(1 << 1)

#define STMPE811_FIFO_STA_TOUCH_DET	(1 << 0)

#define STMPE811_INT_CTRL_GLOBAL_INT	(1 << 0)

#define I2C_CR2_FREQ_MASK	0x3ff

#define I2C_CCR_CCRMASK	0xfff

#define I2C_TRISE_MASK	0x3f

/* Chip IDs */
#define STMPE811_ID                     0x0811

/* Identification registers & System Control */
#define STMPE811_REG_CHP_ID_LSB         0x00
#define STMPE811_REG_CHP_ID_MSB         0x01
#define STMPE811_REG_ID_VER             0x02

/* Global interrupt Enable bit */
#define STMPE811_GIT_EN                 0x01

/* IO expander functionalities */
#define STMPE811_ADC_FCT                0x01
#define STMPE811_TS_FCT                 0x02
#define STMPE811_IO_FCT                 0x04
#define STMPE811_TEMPSENS_FCT           0x08

/* Global Interrupts definitions */
#define STMPE811_GIT_IO                 0x80  /* IO interrupt                   */
#define STMPE811_GIT_ADC                0x40  /* ADC interrupt                  */
#define STMPE811_GIT_TEMP               0x20  /* Not implemented                */
#define STMPE811_GIT_FE                 0x10  /* FIFO empty interrupt           */
#define STMPE811_GIT_FF                 0x08  /* FIFO full interrupt            */
#define STMPE811_GIT_FOV                0x04  /* FIFO overflowed interrupt      */
#define STMPE811_GIT_FTH                0x02  /* FIFO above threshold interrupt */
#define STMPE811_GIT_TOUCH              0x01  /* Touch is detected interrupt    */
#define STMPE811_ALL_GIT                0x1F  /* All global interrupts          */
#define STMPE811_TS_IT                  (STMPE811_GIT_TOUCH | STMPE811_GIT_FTH |  STMPE811_GIT_FOV | STMPE811_GIT_FF | STMPE811_GIT_FE) /* Touch screen interrupts */

/* General Control Registers */
#define STMPE811_REG_SYS_CTRL1          0x03
#define STMPE811_REG_SYS_CTRL2          0x04
#define STMPE811_REG_SPI_CFG            0x08

/* Interrupt system Registers */
#define STMPE811_REG_INT_CTRL           0x09
#define STMPE811_REG_INT_EN             0x0A
#define STMPE811_REG_INT_STA            0x0B
#define STMPE811_REG_IO_INT_EN          0x0C
#define STMPE811_REG_IO_INT_STA         0x0D

/* IO Registers */
#define STMPE811_REG_IO_SET_PIN         0x10
#define STMPE811_REG_IO_CLR_PIN         0x11
#define STMPE811_REG_IO_MP_STA          0x12
#define STMPE811_REG_IO_DIR             0x13
#define STMPE811_REG_IO_ED              0x14
#define STMPE811_REG_IO_RE              0x15
#define STMPE811_REG_IO_FE              0x16
#define STMPE811_REG_IO_AF              0x17

/* ADC Registers */
#define STMPE811_REG_ADC_INT_EN         0x0E
#define STMPE811_REG_ADC_INT_STA        0x0F
#define STMPE811_REG_ADC_CTRL1          0x20
#define STMPE811_REG_ADC_CTRL2          0x21
#define STMPE811_REG_ADC_CAPT           0x22
#define STMPE811_REG_ADC_DATA_CH0       0x30
#define STMPE811_REG_ADC_DATA_CH1       0x32
#define STMPE811_REG_ADC_DATA_CH2       0x34
#define STMPE811_REG_ADC_DATA_CH3       0x36
#define STMPE811_REG_ADC_DATA_CH4       0x38
#define STMPE811_REG_ADC_DATA_CH5       0x3A
#define STMPE811_REG_ADC_DATA_CH6       0x3B
#define STMPE811_REG_ADC_DATA_CH7       0x3C

/* Touch Screen Registers */
#define STMPE811_REG_TSC_CTRL           0x40
#define STMPE811_REG_TSC_CFG            0x41
#define STMPE811_REG_WDM_TR_X           0x42
#define STMPE811_REG_WDM_TR_Y           0x44
#define STMPE811_REG_WDM_BL_X           0x46
#define STMPE811_REG_WDM_BL_Y           0x48
#define STMPE811_REG_FIFO_TH            0x4A
#define STMPE811_REG_FIFO_STA           0x4B
#define STMPE811_REG_FIFO_SIZE          0x4C
#define STMPE811_REG_TSC_DATA_X         0x4D
#define STMPE811_REG_TSC_DATA_Y         0x4F
#define STMPE811_REG_TSC_DATA_Z         0x51
#define STMPE811_REG_TSC_DATA_XYZ       0x52
#define STMPE811_REG_TSC_FRACT_XYZ      0x56
#define STMPE811_REG_TSC_DATA_INC       0x57
#define STMPE811_REG_TSC_DATA_NON_INC   0xD7
#define STMPE811_REG_TSC_I_DRIVE        0x58
#define STMPE811_REG_TSC_SHIELD         0x59

/* Touch Screen Pins definition */
#define STMPE811_TOUCH_YD               STMPE811_PIN_7
#define STMPE811_TOUCH_XD               STMPE811_PIN_6
#define STMPE811_TOUCH_YU               STMPE811_PIN_5
#define STMPE811_TOUCH_XU               STMPE811_PIN_4
#define STMPE811_TOUCH_IO_ALL           (unsigned char)(STMPE811_TOUCH_YD | STMPE811_TOUCH_XD | STMPE811_TOUCH_YU | STMPE811_TOUCH_XU)

/* IO Pins definition */
#define STMPE811_PIN_0                  0x01
#define STMPE811_PIN_1                  0x02
#define STMPE811_PIN_2                  0x04
#define STMPE811_PIN_3                  0x08
#define STMPE811_PIN_4                  0x10
#define STMPE811_PIN_5                  0x20
#define STMPE811_PIN_6                  0x40
#define STMPE811_PIN_7                  0x80
#define STMPE811_PIN_ALL                0xFF

/* IO Pins directions */
#define STMPE811_DIRECTION_IN           0x00
#define STMPE811_DIRECTION_OUT          0x01

/* IO IT types */
#define STMPE811_TYPE_LEVEL             0x00
#define STMPE811_TYPE_EDGE              0x02

/* IO IT polarity */
#define STMPE811_POLARITY_LOW           0x00
#define STMPE811_POLARITY_HIGH          0x04

/* IO Pin IT edge modes */
#define STMPE811_EDGE_FALLING           0x01
#define STMPE811_EDGE_RISING            0x02

/* TS registers masks */
#define STMPE811_TS_CTRL_ENABLE         0x01
#define STMPE811_TS_CTRL_STATUS         0x80


namespace GI
{
namespace Sensor
{
class Stmpe811 : public Cursor {
public:
	Stmpe811(GI::Dev::Screen *pDisplay, char *i2cpath, char *irqPinPath);
	~Stmpe811();
	tControlCommandData *idle();
	void touchCalibrate();

	enum gpioMode_e{
		GPIO_OUT_PUSH_PULL =0,
		GPIO_IN_FLOATING,
		GPIO_MODE_IT_RISING_EDGE,
		GPIO_MODE_IT_FALLING_EDGE,
		GPIO_MODE_IT_LOW_LEVEL,
		GPIO_MODE_IT_HIGH_LEVEL
	}gpioMode;

	SysErr reset();
	SysErr readId(unsigned short id);
	SysErr enableGlobalIt();
	SysErr disableGlobalIt();
	SysErr enableItSource(unsigned char source);
	SysErr disableItSource(unsigned char source);
	SysErr setItPolarity(unsigned char polarity);
	SysErr setItType(unsigned char type);
	SysErr globalItStatus(unsigned char source, bool *status);
	SysErr getGlobalItStatus(unsigned char source, unsigned char *status);
	SysErr clearGlobalIt(unsigned char source);
	SysErr ioStart(unsigned char ioPin);
	SysErr ioConfig(unsigned char ioPin, gpioMode_e ioMode);
	SysErr ioInitPin(unsigned char ioPin, unsigned char direction);
	SysErr ioDisableAf(unsigned char ioPin);
	SysErr ioEnableAf(unsigned char ioPin);
	SysErr ioSetEdgeMode(unsigned char ioPin, unsigned char Edge);
	SysErr ioWritePin(unsigned char ioPin, unsigned char pinState);
	SysErr ioReadPin(unsigned char ioPin, unsigned char *ioPinStatus);
	SysErr ioEnableIt();
	SysErr ioDisableIt();
	SysErr ioEnablePinIt(unsigned char ioPin);
	SysErr ioDisablePinIt(unsigned char ioPin);
	SysErr ioItStatus(unsigned char ioPin, unsigned char *ioItPinStatus);
	SysErr ioClearIt(unsigned char ioPin);

	SysErr write(unsigned char reg, unsigned char data);
	SysErr read(unsigned char reg, unsigned char *data);
	SysErr readMultiple(unsigned char reg, unsigned char *data, unsigned char len);
private:
	SysErr readX(unsigned short *x);
	SysErr readY(unsigned short *y);
	bool readTouch(signed short *x, signed short *y, bool applyScale);
	bool isTsPress;
	bool intIsPressed;
};
}
}


#endif /* LIB_DEVICE_STMPE811_H_ */
