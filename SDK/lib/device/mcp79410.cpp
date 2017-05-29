/*
 * mcp79410.cpp
 */

#include <stdlib.h>
#include "mcp79410.h"
#include <api/dev_request.h>

GI::Device::Mcp79410::Mcp79410(char *i2cPath)
{
    memset(this, 0, sizeof(*this));
    GI::Dev::DevRequest::request(i2cPath, &I2C);
    if (!I2C)
        return;
    unsigned char tmp_buff[4];
    if (regRead(MCP73871_RTC_SecReg, tmp_buff, 4) == SYS_ERR_OK
            && (tmp_buff[0] & MCP73871_St_bm) != 0)
    {
        tmp_buff[0] |= MCP73871_St_bm;
        tmp_buff[3] |= MCP73871_VBatEn_bm;
        regWrite(MCP73871_RTC_SecReg, tmp_buff, 4);
        timeStr = new GI::String();
    }
}

GI::Device::Mcp79410::~Mcp79410()
{
    delete timeStr;
}

SysErr GI::Device::Mcp79410::regRead(unsigned char reg, unsigned char *data,
                                     unsigned char len)
{
    if (!I2C)
        return SYS_ERR_NO_ASSIGNED_DEVICE;
    if (I2C->writeRead(MCP73871_Rtc_DeviceAddr, &reg, 1, data, len))
        return SYS_ERR_BUS;
    return SYS_ERR_OK;
}

SysErr GI::Device::Mcp79410::regWrite(unsigned char reg, unsigned char *data,
                                      unsigned char len)
{
    if (!I2C)
        return SYS_ERR_NO_ASSIGNED_DEVICE;
    unsigned char *tmp = (unsigned char *) malloc(len + 1);
    if (!tmp)
        return SYS_ERR_OUT_OF_MEMORY;
    tmp[0] = reg;
    memcpy(tmp + 1, data, len);
    if (I2C->writeBytes(MCP73871_Rtc_DeviceAddr, tmp, 1 + len) != 1 + len)
    {
        free(tmp);
        return SYS_ERR_BUS;
    }
    free(tmp);
    return SYS_ERR_OK;
}

SysErr GI::Device::Mcp79410::ramRead(unsigned char addr, unsigned char *data,
                                     unsigned char len)
{
    if (!I2C)
        return SYS_ERR_NO_ASSIGNED_DEVICE;
    unsigned char tmp = addr + MCP73871_Sram_Start;
    if (I2C->writeRead(MCP73871_Sram_DeviceAddr, &tmp, 1, data, len))
        return SYS_ERR_BUS;
    return SYS_ERR_OK;
}

SysErr GI::Device::Mcp79410::ramWrite(unsigned char addr, unsigned char *data,
                                      unsigned char len)
{
    if (!I2C)
        return SYS_ERR_NO_ASSIGNED_DEVICE;
    unsigned char *tmp = (unsigned char *) malloc(len + 1);
    if (!tmp)
        return SYS_ERR_OUT_OF_MEMORY;
    tmp[0] = addr + MCP73871_Sram_Start;
    memcpy(tmp + 1, data, len);
    if (I2C->writeBytes(MCP73871_Sram_DeviceAddr, tmp, 1 + len) != 1 + len)
    {
        free(tmp);
        return SYS_ERR_BUS;
    }
    free(tmp);
    return SYS_ERR_OK;
}

SysErr GI::Device::Mcp79410::eepRead(unsigned char addr, unsigned char *data,
                                     unsigned char len)
{
    if (!I2C)
        return SYS_ERR_NO_ASSIGNED_DEVICE;
    unsigned char tmp = addr + MCP73871_Sram_Start;
    if (I2C->writeRead(MCP73871_Eep_DeviceAddr, &tmp, 1, data, len))
        return SYS_ERR_BUS;
    return SYS_ERR_OK;
}

SysErr GI::Device::Mcp79410::eepWrite(unsigned char addr, unsigned char *data,
                                      unsigned char len)
{
    if (!I2C)
        return SYS_ERR_NO_ASSIGNED_DEVICE;
    unsigned char *tmp = (unsigned char *) malloc(len + 1);
    if (!tmp)
        return SYS_ERR_OUT_OF_MEMORY;
    tmp[0] = addr + MCP73871_Sram_Start;
    memcpy(tmp + 1, data, len);
    if (I2C->writeBytes(MCP73871_Eep_DeviceAddr, tmp, 1 + len) != 1 + len)
    {
        free(tmp);
        return SYS_ERR_BUS;
    }
    free(tmp);
    return SYS_ERR_OK;
}

SysErr GI::Device::Mcp79410::checkEepReady()
{
    unsigned char tmp;
    return eepRead(0, &tmp, 1);
}

unsigned char GI::Device::Mcp79410::byteToBcd(unsigned char byte)
{
    uint8_t Tmp = 0;
    while (byte > 9)
        Tmp++;
    return ((Tmp << 4) | byte);
}

unsigned char GI::Device::Mcp79410::bcdToByte(unsigned char bcd)
{
    return ((((bcd >> 4) & 0x0F) * 10) | (bcd & 0x0F));
}

SysErr GI::Device::Mcp79410::writeRtc(unsigned char reg, unsigned char time)
{
    unsigned char tmp_reg = 0;
    SysErr err = SYS_ERR_OK;
    if ((err = regRead(reg, &tmp_reg, 1)) != SYS_ERR_OK)
        return err;
    switch (reg)
    {
    case MCP73871_RTC_SecReg:
        tmp_reg = (tmp_reg & ~(MCP73871_Seconds_gm | MCP73871_10Seconds_gm))
                | (byteToBcd(time) & (MCP73871_Seconds_gm | MCP73871_10Seconds_gm));
        break;
    case MCP73871_RTC_MinReg:
        tmp_reg = (tmp_reg & ~(MCP73871_Minutes_gm | MCP73871_10Minutes_gm))
                | (byteToBcd(time) & (MCP73871_Minutes_gm | MCP73871_10Minutes_gm));
        break;
    case MCP73871_RTC_HourReg:
        tmp_reg = (tmp_reg & ~(MCP73871_Hours_gm | MCP73871_10Hours_gm))
                | (byteToBcd(time) & (MCP73871_Hours_gm | MCP73871_10Hours_gm));
        break;
    case MCP73871_RTC_DateReg:
        tmp_reg = (tmp_reg & ~(MCP73871_Date_gm | MCP73871_10Date_gm))
                | (byteToBcd(time) & (MCP73871_Date_gm | MCP73871_10Date_gm));
        break;
    case MCP73871_RTC_MonthReg:
        tmp_reg = (tmp_reg & ~(MCP73871_Month_gm | MCP73871_10Month_bm))
                | (byteToBcd(time) & (MCP73871_Month_gm | MCP73871_10Month_bm));
        break;
    case MCP73871_RTC_YearReg:
        tmp_reg = (tmp_reg & ~(MCP73871_Year_gm | MCP73871_10Year_gm))
                | (byteToBcd(time) & (MCP73871_Year_gm | MCP73871_10Year_gm));
        break;
    }
    if ((err = regWrite(reg, &tmp_reg, 1)) != SYS_ERR_OK)
        return err;
    return err;
}

SysErr GI::Device::Mcp79410::readRtc(unsigned char reg, unsigned char *time)
{
    unsigned char tmp_reg = 0;
    SysErr err = SYS_ERR_OK;
    if ((err = regRead(reg, &tmp_reg, 1)) != SYS_ERR_OK)
        return err;
    switch (reg)
    {
    case MCP73871_RTC_SecReg:
        *time = bcdToByte(tmp_reg & (MCP73871_Seconds_gm | MCP73871_10Seconds_gm));
        break;
    case MCP73871_RTC_MinReg:
        *time = bcdToByte(tmp_reg & (MCP73871_Minutes_gm | MCP73871_10Minutes_gm));
        break;
    case MCP73871_RTC_HourReg:
        *time = bcdToByte(tmp_reg & (MCP73871_Hours_gm | MCP73871_10Hours_gm));
        break;
    case MCP73871_RTC_DateReg:
        *time = bcdToByte(tmp_reg & (MCP73871_Date_gm | MCP73871_10Date_gm));
        break;
    case MCP73871_RTC_MonthReg:
        *time = bcdToByte(tmp_reg & (MCP73871_Month_gm | MCP73871_10Month_bm));
        break;
    case MCP73871_RTC_YearReg:
        *time = bcdToByte(tmp_reg & (MCP73871_Year_gm | MCP73871_10Year_gm));
        break;
    }
    return err;
}
/*
 * Format can be "%02d:%02d:%02d-%02d:%02d:%04d" result = 01:01:01-01:01:2017 = Hour:Min:Sec-Day:Month:Year.
 * If a NULL is provided the result will be in the UP format.
 */
GI::String *GI::Device::Mcp79410::readTime(string format)
{
    unsigned char tmp_buff[7];
    timeStr->clear();
    if (regRead(MCP73871_RTC_SecReg, tmp_buff, 7) != SYS_ERR_OK)
    {
        return timeStr;
    }

    unsigned int sec = bcdToByte(tmp_buff[0] & (MCP73871_Seconds_gm | MCP73871_10Seconds_gm));
    unsigned int min = bcdToByte(tmp_buff[1] & (MCP73871_Minutes_gm | MCP73871_10Minutes_gm));
    unsigned int hour = bcdToByte(tmp_buff[2] & (MCP73871_Hours_gm | MCP73871_10Hours_gm));
    unsigned int date = bcdToByte(tmp_buff[4] & (MCP73871_Date_gm | MCP73871_10Date_gm));
    unsigned int month = bcdToByte(tmp_buff[5] & (MCP73871_Month_gm | MCP73871_10Month_bm));
    unsigned int year = bcdToByte(tmp_buff[6] & (MCP73871_Year_gm | MCP73871_10Year_gm));
    if(format)
        timeStr->setF(format, hour, min, sec, date, month, year);
    else
        timeStr->setF("%02d:%02d:%02d-%02d:%02d:%04d", hour, min, sec, date, month, year);
    return timeStr;
}
