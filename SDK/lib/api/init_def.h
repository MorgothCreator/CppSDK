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
		name,\
		pin,\
		reversed,\
		mode,\
		multipin,\
		defaultValue\
	};\
	ioSettings cfgName = {\
		ioSettings::info_s::ioType_GPIO,\
		name,\
		(void *)&cfgName##_gpioCfg,\
		NULL\
}

#define ioSetCfgUart(cfgName, name, tx, rx, speed, wordLen, stopBits, parity, uartMode)\
	CfgUart cfgName##_uartCfg = {\
		name,\
		tx,\
		rx,\
		speed,\
		wordLen,\
		stopBits,\
		parity,\
		uartMode\
	};\
	ioSettings cfgName = {\
		ioSettings::info_s::ioType_UART,\
		name,\
		(void *)&cfgName##_uartCfg,\
		NULL\
}

#define ioSetCfgI2c(cfgName, name, scl, sda, i2cMode, speed)\
	CfgI2c cfgName##_i2cCfg = {\
		name,\
		scl,\
		sda,\
		speed,\
		i2cMode,\
	};\
	ioSettings cfgName = {\
		ioSettings::info_s::ioType_I2C,\
		name,\
		(void *)&cfgName##_i2cCfg,\
		NULL\
}

#define ioSetCfgSpi(cfgName, name, sck, mosi, miso, cs, spiMode, speed, lsbFirst)\
	CfgSpi cfgName##_spiCfg = {\
		name,\
		sck,\
		mosi,\
		miso,\
		cs,\
		speed,\
		spiMode,\
		lsbFirst\
	};\
	ioSettings cfgName = {\
		ioSettings::info_s::ioType_SPI,\
		name,\
		(void *)&cfgName##_spiCfg,\
		NULL\
}

#define ioSetCfgScreen(cfgName, name, timings, screenNr)\
	CfgLcd cfgName##_lcdCfg = {\
		name,\
		timings,\
		screenNr\
	};\
	ioSettings cfgName = {\
		ioSettings::info_s::ioType_SCREEN,\
		name,\
		(void *)&cfgName##_lcdCfg,\
		NULL\
}

#define ioSetCfgEnd(cfgName)\
	ioSettings cfgName = {\
		ioSettings::info_s::ioType_NONE,\
		#cfgName,\
		NULL,\
		NULL\
}



#endif /* LIB_API_INIT_DEF_H_ */
