/*
 * 24c.cpp
 */

#include "24c.h"
#include <api/dev_request.h>

GI::Device::Eep24c::Eep24c(string i2cPath, unsigned char icNr, Sizes devSize)
{
    memset(this, 0, sizeof(*this));
    GI::Dev::DevRequest::request(i2cPath, &I2C);
    if(!I2C)
        return;
    switch((unsigned char)devSize)
    {
    case size128:
    case size256:
    case size512:
    case size1024:
    case size2048:
    case size4096:
    case size8192:
        if(icNr > 7)
        {
            err = SYS_ERR_OUT_OF_RANGE;
            return;
        }
        break;
    case size16384:
    case size32768:
    case size65536:
        if(icNr > 3)
        {
            err = SYS_ERR_OUT_OF_RANGE;
            return;
        }
        break;
    case size131072:
        if(icNr > 1)
        {
            err = SYS_ERR_OUT_OF_RANGE;
            return;
        }
        break;
    default:
        err = SYS_ERR_INVALID_SIZE;
    }
    this->IcNr = icNr & 0x07;
    this->devSize = devSize;
}

GI::Device::Eep24c::~Eep24c()
{

}

SysErr GI::Device::Eep24c::write(unsigned long addr, unsigned char *buff, unsigned long len)
{
    unsigned char ctrl_byte = 0x50;
    unsigned char low_addr = addr;
    unsigned char hi_addr = addr >> 8;
    unsigned short page_size = 0;
    unsigned char *tmp_buff;
    tmp_buff = (unsigned char *)malloc(len + 2);
    if(!tmp_buff)
        return SYS_ERR_OUT_OF_MEMORY;
    switch((unsigned char)devSize)
    {
    case size128:
        low_addr &= 0x7F;
    case size256:
        page_size = 8;
        ctrl_byte |= IcNr & 0x07;
        goto eep24c_dev_under_16kb;
    case size512:
        page_size = 16;
        ctrl_byte |= ((IcNr << 1) & 0xFE) | ((addr >> 8) & 0x01);
        goto eep24c_dev_under_16kb;
    case size1024:
        page_size = 16;
        ctrl_byte |= ((IcNr << 2) & 0xFC) | ((addr >> 8) & 0x03);
        goto eep24c_dev_under_16kb;
    case size2048:
        page_size = 16;
        ctrl_byte |= ((addr >> 8) & 0x07);
        goto eep24c_dev_under_16kb;
    case size4096:
        page_size = 32;
        hi_addr &= 0x0F;
        ctrl_byte &= 0x7B;
        goto eep24c_dev_bigger_than_16kb;
    case size8192:
        page_size = 32;
        hi_addr &= 0x1F;
        ctrl_byte &= 0x7B;
        goto eep24c_dev_bigger_than_16kb;
    case size16384:
        page_size = 64;
        hi_addr &= 0x3F;
        ctrl_byte |= IcNr & 0x07;
        goto eep24c_dev_bigger_than_16kb;
    case size32768:
        page_size = 64;
        hi_addr &= 0x7F;
        ctrl_byte |= IcNr & 0x07;
        goto eep24c_dev_bigger_than_16kb;
    case size65536:
        page_size = 128;
        ctrl_byte |= IcNr & 0x07;
        ctrl_byte &= 0x7B;
        goto eep24c_dev_bigger_than_16kb;
    case size131072:
        page_size = 256;
        ctrl_byte |= ((IcNr << 1) & 0x02) | ((addr >> 16) & 0x04);
        goto eep24c_dev_bigger_than_16kb;
    default:
        return SYS_ERR_INVALID_SIZE;
    }
eep24c_dev_under_16kb:
    if((addr % page_size) + len > page_size)
        return SYS_ERR_OUT_OF_PAGE;
    tmp_buff[0] = low_addr;
    memcpy(tmp_buff + 1, buff, len);
    if(I2C->writeBytes(ctrl_byte, tmp_buff, len + 1) != (int)len + 1)
    {
        free(tmp_buff);
        return SYS_ERR_UNKNOWN;
    }
    else
    {
        free(tmp_buff);
        return SYS_ERR_OK;
    }
eep24c_dev_bigger_than_16kb:
    if((addr % page_size) + len > page_size)
        return SYS_ERR_OUT_OF_PAGE;
    tmp_buff = (unsigned char *)malloc(len + 2);
    tmp_buff[0] = hi_addr;
    tmp_buff[1] = low_addr;
    memcpy(tmp_buff + 2, buff, len);
    if(I2C->writeBytes(ctrl_byte, tmp_buff, len + 2) != (int)len + 2)
    {
        free(tmp_buff);
        return SYS_ERR_UNKNOWN;
    }
    else
    {
        free(tmp_buff);
        return SYS_ERR_OK;
    }
}

SysErr GI::Device::Eep24c::read(unsigned long addr, unsigned char *buff, unsigned long len)
{
    unsigned char ctrl_byte = 0x50;
    unsigned char low_addr = addr;
    unsigned char hi_addr = addr >> 8;
    switch((unsigned char)devSize)
    {
    case size128:
        low_addr &= 0x7F;
    case size256:
        ctrl_byte |= IcNr & 0x07;
        goto eep24c_dev_under_16kb;
    case size512:
        ctrl_byte |= ((IcNr << 1) & 0xFE) | ((addr >> 8) & 0x01);
        goto eep24c_dev_under_16kb;
    case size1024:
        ctrl_byte |= ((IcNr << 2) & 0xFC) | ((addr >> 8) & 0x03);
        goto eep24c_dev_under_16kb;
    case size2048:
        ctrl_byte |= ((addr >> 8) & 0x07);
        goto eep24c_dev_under_16kb;
    case size4096:
        hi_addr &= 0x0F;
        ctrl_byte &= 0x7B;
        goto eep24c_dev_bigger_than_16kb;
    case size8192:
        hi_addr &= 0x1F;
        ctrl_byte &= 0x7B;
        goto eep24c_dev_bigger_than_16kb;
    case size16384:
        hi_addr &= 0x3F;
        ctrl_byte |= IcNr & 0x07;
        goto eep24c_dev_bigger_than_16kb;
    case size32768:
        hi_addr &= 0x7F;
        ctrl_byte |= IcNr & 0x07;
        goto eep24c_dev_bigger_than_16kb;
    case size65536:
        ctrl_byte |= IcNr & 0x07;
        ctrl_byte &= 0x7B;
        goto eep24c_dev_bigger_than_16kb;
    case size131072:
        ctrl_byte |= ((IcNr << 1) & 0x02) | ((addr >> 16) & 0x04);
        goto eep24c_dev_bigger_than_16kb;
    default:
        return SYS_ERR_INVALID_SIZE;
    }
    unsigned char buffSend[2];
eep24c_dev_under_16kb:
    buffSend[0] = low_addr;
    return I2C->writeRead(ctrl_byte, buffSend, 1, buff, len);
eep24c_dev_bigger_than_16kb:
    buffSend[0] = hi_addr;
    buffSend[1] = low_addr;
    return I2C->writeRead(ctrl_byte, buffSend, 2, buff, len);
}

