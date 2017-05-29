/*
 * 45db.cpp
 */

#include "45db.h"
#include <api/dev_request.h>
#include <api/timer.h>

GI::Device::Flash45db::Flash45db(string spiPath)
{
    memset(this, 0, sizeof(*this));
    GI::Dev::DevRequest::request(spiPath, &SPI);
}

GI::Device::Flash45db::~Flash45db()
{

}

SysErr GI::Device::Flash45db::readStatus(unsigned char *status)
{
    if(!SPI)
        return SYS_ERR_INVALID_HANDLER;
    unsigned char reg = 0x7D;
    return SPI->writeRead(&reg, 1, status, 1);
}

SysErr GI::Device::Flash45db::enterSleep()
{
    if(!SPI)
        return SYS_ERR_INVALID_HANDLER;
    unsigned char reg = 0xB9;
    if(SPI->writeBytes(&reg, 1) != 1)
        return SYS_ERR_UNKNOWN;
    else
        return SYS_ERR_OK;
}

SysErr GI::Device::Flash45db::resumeSleep()
{
    if(!SPI)
        return SYS_ERR_INVALID_HANDLER;
    unsigned char reg = 0xAB;
    if(SPI->writeBytes(&reg, 1) != 1)
        return SYS_ERR_UNKNOWN;
    else
        return SYS_ERR_OK;
}

SysErr GI::Device::Flash45db::writeBuff(unsigned char buffNr, unsigned int pageNr, unsigned char *data)
{
    if(!SPI)
        return SYS_ERR_INVALID_HANDLER;
    unsigned char *tmp_buff = (unsigned char *)malloc(5 + (1 << pageSize));
    if(!tmp_buff)
        return SYS_ERR_OUT_OF_PAGE;
    if(buffNr == 0)
        tmp_buff[0] = 0x84;
    else
        tmp_buff[0] = 0x87;
    tmp_buff[1] = 0x00;
    tmp_buff[2] = 0x00;
    tmp_buff[3] = 0x00;
    tmp_buff[4] = 0x00;
    memcpy(tmp_buff, data, 1 << pageSize);
    if(SPI->writeBytes(tmp_buff + 5, 5 + (1 << pageSize)) != 5 + (1 << pageSize))
    {
        free(tmp_buff);
        return SYS_ERR_OUT_OF_MEMORY;
    }
    else
    {
        free(tmp_buff);
        return SYS_ERR_OK;
    }
}

SysErr GI::Device::Flash45db::writeFromBuffer(unsigned char buffNr, unsigned int pageNr)
{
    if(!SPI)
        return SYS_ERR_INVALID_HANDLER;
    unsigned char tmp_buff[4];
    if(buffNr == 0)
        tmp_buff[0] = 0x83;
    else
        tmp_buff[0] = 0x86;
    unsigned long addr_tmp = ((unsigned long)pageNr) << pageSize;
    addr_tmp = addr_tmp & ~((1 << pageSize) - 1);
    tmp_buff[1] = addr_tmp>>16;
    tmp_buff[2] = addr_tmp>>8;
    tmp_buff[3] = addr_tmp;
    if(SPI->writeBytes(tmp_buff, 4) != 4)
        return SYS_ERR_OUT_OF_MEMORY;
    unsigned char tmp = 0;
    SysErr error = SYS_ERR_OK;
    GI::Sys::Timer timer_timeout = GI::Sys::Timer(10);
    do
    {
        if((error = readStatus(&tmp)) != SYS_ERR_OK)
            return error;
        if(timer_timeout.tick())
            return SYS_ERR_TIMEOUT;
    } while (!(tmp & (1<<DEVICE_45DB_RDY)));
    return SYS_ERR_OK;
}

SysErr GI::Device::Flash45db::pageErase(unsigned int pageNr)
{
    if(!SPI)
        return SYS_ERR_INVALID_HANDLER;
    unsigned char tmp_buff[4];
    tmp_buff[0] = 0x81;
    unsigned long addr_tmp = ((unsigned long)pageNr) << pageSize;
    addr_tmp = addr_tmp & ~((1 << pageSize) - 1);
    tmp_buff[1] = addr_tmp>>16;
    tmp_buff[2] = addr_tmp>>8;
    tmp_buff[3] = addr_tmp;
    if(SPI->writeBytes(tmp_buff, 4) != 4)
        return SYS_ERR_OUT_OF_MEMORY;
    unsigned char tmp = 0;
    SysErr error = SYS_ERR_OK;
    GI::Sys::Timer timer_timeout = GI::Sys::Timer(10);
    do
    {
        if((error = readStatus(&tmp)) != SYS_ERR_OK)
            return error;
        if(timer_timeout.tick())
            return SYS_ERR_TIMEOUT;
    } while (!(tmp & (1<<DEVICE_45DB_RDY)));
    return SYS_ERR_OK;
}

SysErr GI::Device::Flash45db::write(unsigned char buffNr, unsigned int pageNr, unsigned char *data)
{
    if(!SPI)
        return SYS_ERR_INVALID_HANDLER;
    SysErr error = SYS_ERR_OK;
    if((error = pageErase(pageNr)) != SYS_ERR_OK)
        return error;
    if((error = writeBuff(buffNr, pageNr, data)) != SYS_ERR_OK)
        return error;
    if((error = writeFromBuffer(buffNr, pageNr)) != SYS_ERR_OK)
        return error;
    return SYS_ERR_OK;
}

SysErr GI::Device::Flash45db::read(unsigned int pageNr, unsigned char *data, unsigned int size)
{
    if(!SPI)
        return SYS_ERR_INVALID_HANDLER;
    unsigned char tmp_buff[8];
    tmp_buff[0] = 0xE8;
    unsigned long addr_tmp = ((unsigned long)pageNr) << pageSize;
    addr_tmp = addr_tmp & ~((1 << pageSize) - 1);
    tmp_buff[1] = addr_tmp>>16;
    tmp_buff[2] = addr_tmp>>8;
    tmp_buff[3] = addr_tmp;
    tmp_buff[4] = 0xFF;
    tmp_buff[5] = 0xFF;
    tmp_buff[6] = 0xFF;
    tmp_buff[7] = 0xFF;
    return SPI->writeRead(tmp_buff, 8, data, size);
}

