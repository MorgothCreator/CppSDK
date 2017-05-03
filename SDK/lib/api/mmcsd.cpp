/*
 * mmcsd.cpp
 *
 *  Created on: May 1, 2017
 *      Author: Cutare
 */

#include "mmcsd.h"

SysErr GI::Dev::MmcSd::ioctl(unsigned int command,
		unsigned int *buffer)
{
    if(ioctl_Ptr)
        ioctl_Ptr(driverHandler_Ptr, command, buffer);
    else
        return SYS_ERR_NOT_IMPLEMENTED;
    return SYS_ERR_OK;
}

SysErr GI::Dev::MmcSd::read(void *ptr,
		unsigned long block, unsigned int nblks)
{
    if(read_Ptr)
        read_Ptr(driverHandler_Ptr, ptr, block, nblks);
    else
        return SYS_ERR_NOT_IMPLEMENTED;
    return SYS_ERR_OK;
}

SysErr GI::Dev::MmcSd::write(void *ptr,
		unsigned long block, unsigned int nblks)
{
    if(write_Ptr)
        write_Ptr(driverHandler_Ptr, ptr, block, nblks);
    else
        return SYS_ERR_NOT_IMPLEMENTED;
    return SYS_ERR_OK;
}

SysErr GI::Dev::MmcSd::idle()
{
    if(idle_Ptr)
        idle_Ptr(driverHandler_Ptr);
    else
        return SYS_ERR_NOT_IMPLEMENTED;
    return SYS_ERR_OK;
}


