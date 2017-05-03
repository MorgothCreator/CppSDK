/*
 * defs.cpp
 */
#if defined(SmartRf06)

#include "defs.h"
#include <stdio.h>

#include <api/gpio.h>
#include <api/spi.h>
#include <api/i2c.h>
#include <api/uart.h>

/* IO initialization table */
ioSetCfgGpio(led_0,     "led-0",    pin_mux_convert_to_pin(IOA, 25),CfgGpio::GPIO_OUT_PUSH_PULL,    false, false, 0);
ioSetCfgGpio(led_1,     "led-1",    pin_mux_convert_to_pin(IOA, 27),CfgGpio::GPIO_OUT_PUSH_PULL,    false, false, 0);
ioSetCfgGpio(led_2,     "led-2",    pin_mux_convert_to_pin(IOA,  7),CfgGpio::GPIO_OUT_PUSH_PULL,    false, false, 0);
ioSetCfgGpio(led_3,     "led-3",    pin_mux_convert_to_pin(IOA,  6),CfgGpio::GPIO_OUT_PUSH_PULL,    false, false, 0);
ioSetCfgGpio(hbtn_0,    "hbtn-0",   pin_mux_convert_to_pin(IOA, 19),CfgGpio::GPIO_IN_PULL_UP,       false, false, 0);
ioSetCfgGpio(hbtn_1,    "hbtn-1",   pin_mux_convert_to_pin(IOA, 12),CfgGpio::GPIO_IN_PULL_UP,       false, false, 0);
ioSetCfgGpio(hbtn_2,    "hbtn-2",   pin_mux_convert_to_pin(IOA, 15),CfgGpio::GPIO_IN_PULL_UP,       false, false, 0);
ioSetCfgGpio(hbtn_3,    "hbtn-3",   pin_mux_convert_to_pin(IOA, 18),CfgGpio::GPIO_IN_PULL_UP,       false, false, 0);
ioSetCfgGpio(hbtn_4,    "hbtn-4",   pin_mux_convert_to_pin(IOA, 11),CfgGpio::GPIO_IN_PULL_UP,       false, false, 0);

ioSetCfgGpio(lv33_en,   "lv33-en",  pin_mux_convert_to_pin(IOA, 13),CfgGpio::GPIO_OUT_PUSH_PULL,   false, false, 1);
ioSetCfgGpio(acc_pwr,   "acc-pwr",  pin_mux_convert_to_pin(IOA, 20),CfgGpio::GPIO_OUT_PUSH_PULL,   false, false, 1);
ioSetCfgGpio(accint_1,  "accint-1", pin_mux_convert_to_pin(IOA, 28),CfgGpio::GPIO_IN_PULL_UP,      false, false, 0);
ioSetCfgGpio(accint_2,  "accint-2", pin_mux_convert_to_pin(IOA, 29),CfgGpio::GPIO_IN_PULL_UP,      false, false, 0);
ioSetCfgGpio(als_pwr,   "als-pwr",  pin_mux_convert_to_pin(IOA, 26),CfgGpio::GPIO_OUT_PUSH_PULL,   false, false, 0);
ioSetCfgGpio(als_out,   "als-out",  pin_mux_convert_to_pin(IOA, 23),CfgGpio::GPIO_IN_FLOATING,     false, false, 0);
ioSetCfgGpio(lcd_rst,   "lcd-rst",  pin_mux_convert_to_pin(IOA,  5),CfgGpio::GPIO_OUT_PUSH_PULL,   false, false, 1);
ioSetCfgGpio(lcd_cd,    "lcd-cd",   pin_mux_convert_to_pin(IOA,  4),CfgGpio::GPIO_OUT_PUSH_PULL,   false, false, 0);

ioSetCfgUart(uart_0,    "uart-0",   pin_mux_convert_to_pin(IOA,  3) , pin_mux_convert_to_pin(IOA, 2)    , 115200    , CfgUart::WORD_LEN_8   , CfgUart::STOP_BITS_ONE    , CfgUart::PAR_NONE , CfgUart::MODE_ASYNC);

ioSetCfgI2c(i2c_0,      "i2c-0",    pin_mux_convert_to_pin(IOA,  8) , pin_mux_convert_to_pin(IOC, 9), 100000);

ioSetCfgSpi(spi_0_0,    "spi-0.0",  pin_mux_convert_to_pin(IOA, 10) , pin_mux_convert_to_pin(IOA, 9), pin_mux_convert_to_pin(IOA, 8), pin_mux_convert_to_pin(IOA,14), CfgSpi::spiMode0,20000000, false);//LCD
ioSetCfgSpi(spi_0_1,    "spi-0.1",  pin_mux_convert_to_pin(IOA, 10) , pin_mux_convert_to_pin(IOA, 9), pin_mux_convert_to_pin(IOA, 8), pin_mux_convert_to_pin(IOA,30), CfgSpi::spiMode0,25000000, false);//uSD
ioSetCfgSpi(spi_0_2,    "spi-0.2",  pin_mux_convert_to_pin(IOA, 10) , pin_mux_convert_to_pin(IOA, 9), pin_mux_convert_to_pin(IOA, 8), pin_mux_convert_to_pin(IOA,24), CfgSpi::spiMode0, 5000000, false);//Accelerometer

ioSetCfgEnd(ioTableEnd);

ioSettings *ioSetCfg[] = {
        &led_0,
        &led_1,
        &led_2,
        &led_3,
        &hbtn_0,
        &hbtn_1,
        &hbtn_2,
        &hbtn_3,
        &hbtn_4,
        &lv33_en,
        &acc_pwr,
        &accint_1,
        &accint_2,
        &als_pwr,
        &als_out,
        &lcd_rst,
        &lcd_cd,
        &uart_0,
        //&i2c_0,
        &spi_0_0,
        &spi_0_1,
        &spi_0_2,
        &ioTableEnd,
        (ioSettings *)-1/* This is an artifice to force linker to put this structure into '.data' section, to be secure that this will be initiated before the call of init() function. */
};

#endif/*!STM32F769I_DISCO*/
