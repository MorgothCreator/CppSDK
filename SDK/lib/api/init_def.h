/*
 * init_def.h
 */

#ifndef LIB_API_INIT_DEF_H_
#define LIB_API_INIT_DEF_H_


typedef struct {
    struct info_s{
        enum ioType_s{
            ioType_NONE = 0,
            ioType_GPIO,
            ioType_UART,
            ioType_I2C,
            ioType_SPI,
            ioType_SCREEN,
            ioType_TOUCH,
            ioType_CAM,
            ioType_MMCSD,
            ioType_MMCSD_SPI,
        }ioType;
        char name[12];
    } info;
	void *cfg;
	void *ioConstruct;
}ioSettings;

#define ioSetCfgGpio(cfgName, name, pin, mode, multipin, reversed, defaultValue)\
	CfgGpio cfgName##_gpioCfg = {\
	    /* cfgName, name, pin, mode, multipin, reversed, defaultValue */\
	    \
	    /* See: <api/gpio.h>.*/\
        /* Some properties depend on the platform. */\
	    /* GPIO name. */\
		name,\
        /* GPIO pin. */\
		pin,\
        /* GPIO is inverted. */\
		reversed,\
        /* GPIO mode: CfgGpio::GPIO_OUT_PUSH_PULL, CfgGpio::GPIO_IN_FLOATING, CfgGpio::GPIO_AIN, CfgGpio::GPIO_IN_PULL_DOWN, CfgGpio::GPIO_IN_PULL_UP, CfgGpio::GPIO_OUT_OPEN_DRAIN, CfgGpio::GPIO_ALTERNATIVE_OPEN_DRINE, CfgGpio::GPIO_ALTERNATIVE_PUSH_PULL. */\
		mode,\
        /* GPIO controll is multipin : false/true. */\
		multipin,\
        /* GPIO default value 1/0 this value is reversed if 'reversed' is true. */\
		defaultValue\
	};\
	ioSettings cfgName = {\
	    /* IO type. */\
		ioSettings::info_s::ioType_GPIO,\
        /* IO Name. */\
		name,\
        /* IO config struct. */\
		(void *)&cfgName##_gpioCfg,\
        /* IO dev variables. */\
		NULL\
}

#define ioSetCfgUart(cfgName, name, tx, rx, speed, wordLen, stopBits, parity, uartMode)\
	CfgUart cfgName##_uartCfg = {\
	    /* cfgName, name, tx, rx, speed, wordLen, stopBits, parity, uartMode */\
        \
	    /* See: <api/uart.h>. */\
        /* Some properties depend on the platform. */\
        /* UART name. */\
		name,\
        /* UART tx pin. */\
		tx,\
        /* UART rx pin. */\
		rx,\
        /* UART baud rate in Hz. */\
		speed,\
        /* UART word len: CfgUart::WORD_LEN_5, CfgUart::WORD_LEN_6, CfgUart::WORD_LEN_7, CfgUart::WORD_LEN_8, CfgUart::WORD_LEN_9. */\
		wordLen,\
        /* UART stop bits: CfgUart::STOP_BITS_ONE, CfgUart::STOP_BITS_ONE_AND_HALF, CfgUart::STOP_BITS_TWO. */\
		stopBits,\
        /* UART parity : CfgUart::PAR_NONE, CfgUart::PAR_ODD, CfgUart::PAR_EVEN, CfgUart::PAR_MASK, CfgUart::PAR_ZERO, CfgUart::PAR_ONE. */\
		parity,\
        /* UART Mode: CfgUart::MODE_ASYNC, CfgUart::MODE_SYNC, CfgUart::MODE_IR, CfgUart::MODE_SPI. */\
		uartMode\
	};\
	ioSettings cfgName = {\
	    /* IO type. */\
		ioSettings::info_s::ioType_UART,\
        /* IO Name. */\
		name,\
        /* IO config struct. */\
		(void *)&cfgName##_uartCfg,\
        /* IO dev variables. */\
		NULL\
}

#define ioSetCfgI2c(cfgName, name, scl, sda, speed)\
	CfgI2c cfgName##_i2cCfg = {\
	    /* cfgName, name, scl, sda, speed */\
        \
	    /* See: <api/i2c.h>. */\
        /* Some properties depend on the platform. */\
	    /* I2C name. */\
		name,\
        /* I2C scl pin. */\
		scl,\
        /* I2C sda pin. */\
		sda,\
        /* I2C speed in Hz. */\
		speed,\
	};\
	ioSettings cfgName = {\
	    /* IO type. */\
		ioSettings::info_s::ioType_I2C,\
        /* IO Name. */\
		name,\
        /* IO config struct. */\
		(void *)&cfgName##_i2cCfg,\
        /* IO dev variables. */\
		NULL\
}

#define ioSetCfgSpi(cfgName, name, sck, mosi, miso, cs, spiMode, speed, lsbFirst)\
	CfgSpi cfgName##_spiCfg = {\
	    /* cfgName, name, sck, mosi, miso, cs, spiMode, speed, lsbFirst */\
        \
	    /* See: <api/spi.h>. */\
        /* Some properties depend on the platform. */\
	    /* SPI name. */\
		name,\
        /* SPI sck pin nr. */\
		sck,\
        /* SPI mosi pin nr. */\
		mosi,\
        /* SPI miso pin nr. */\
		miso,\
        /* SPI cs pin nr. */\
		cs,\
        /* SPI speed in Hz. */\
		speed,\
        /* SPI mode: CfgSpi::spiMode0, CfgSpi::spiMode1, CfgSpi::spiMode2, CfgSpi::spiMode3. */\
		spiMode,\
        /* SPI msb false/true. */\
		lsbFirst\
	};\
	ioSettings cfgName = {\
	    /* IO type. */\
		ioSettings::info_s::ioType_SPI,\
        /* IO Name. */\
		name,\
        /* IO config struct. */\
		(void *)&cfgName##_spiCfg,\
        /* IO dev variables. */\
		NULL\
}

#define ioSetCfgScreen(cfgName, name, timings, screenNr)\
	CfgLcd cfgName##_lcdCfg = {\
	    /* cfgName, name, timings, screenNr */\
        \
	    /* See: <api/lcd_def.h>, <api/lcd_def.cpp>, <api/lcd.cpp>. */\
	    /* Some properties depend on the platform. */\
	    /* Screen name. */\
		name,\
        /* Screen timings. */\
		timings,\
        /* Screen position. */\
		screenNr\
	};\
	ioSettings cfgName = {\
	    /* IO type. */\
		ioSettings::info_s::ioType_SCREEN,\
        /* IO Name. */\
		name,\
        /* IO config struct. */\
		(void *)&cfgName##_lcdCfg,\
        /* IO dev variables. */\
		NULL\
}

#define ioSetCfgEnd(cfgName)\
	ioSettings cfgName = {\
	    /* cfgName */\
        \
	    /* This is the end of table, is a NULL structure to indicate the end of table. */\
	    /* IO type. */\
		ioSettings::info_s::ioType_NONE,\
        /* IO Name. */\
		#cfgName,\
        /* IO config struct. */\
		NULL,\
        /* IO dev variables. */\
		NULL\
}



#endif /* LIB_API_INIT_DEF_H_ */
