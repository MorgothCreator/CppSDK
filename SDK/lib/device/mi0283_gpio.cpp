/*
 * mi0283_gpio.cpp
 *
 *  Created on: Apr 14, 2017
 *      Author: Cutare
 */

#include "mi0283_gpio.h"

GI::Device::Mi0283Gpio::Mi0283Gpio(char *dev_name)
{

    memset(this, 0, sizeof(*this));
    if(!dev_name)
        return;
    char tmp[12];
    int dev_name_len = strlen(dev_name);
    if(dev_name_len > 7 || dev_name_len == 0)
        return;
    strcpy(tmp, dev_name);
    strcat(tmp, "-bl");
    LCD_BACKLIGHT = new GI::IO(tmp);
    strcpy(tmp, dev_name);
    strcat(tmp, "-rst");
    LCD_DISPLAY_RST = new GI::IO(tmp);
    strcpy(tmp, dev_name);
    strcat(tmp, "-rs");
    LCD_DISPLAY_RS = new GI::IO(tmp);
    strcpy(tmp, dev_name);
    strcat(tmp, "-cs");
    LCD_DISPLAY_CS = new GI::IO(tmp);
    strcpy(tmp, dev_name);
    strcat(tmp, "-wr");
    LCD_DISPLAY_WR = new GI::IO(tmp);
    strcpy(tmp, dev_name);
    strcat(tmp, "-rd");
    LCD_DISPLAY_RD = new GI::IO(tmp);
    if(!LCD_DISPLAY_RS || !LCD_DISPLAY_CS || !LCD_DISPLAY_WR)
    {
        return;
    }
}

GI::Device::Mi0283Gpio::~Mi0283Gpio()
{

}

