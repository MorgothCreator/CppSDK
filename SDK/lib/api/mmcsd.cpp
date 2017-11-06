/*
 * mmcsd.cpp
 *
 *  Created on: May 1, 2017
 *      Author: Cutare
 */

#include <stdlib.h>
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

SysErr GI::Dev::MmcSd::getRawCid(unsigned long *rawCid)
{
	if(getRawCid_Ptr)
	{
    	if(!rawCid)
    	{
    		rawCid = (unsigned long *)malloc(sizeof(unsigned long) * 4);
    		if(!rawCid)
    			return SYS_ERR_OUT_OF_MEMORY;
    	}
		getRawCid_Ptr(driverHandler_Ptr, rawCid);
	}
    else
        return SYS_ERR_NOT_IMPLEMENTED;
    return SYS_ERR_OK;
}

SysErr GI::Dev::MmcSd::getRawCsd(unsigned long *rawCsd)
{
    if(getRawCsd_Ptr)
    {
    	if(!rawCsd)
    	{
    		rawCsd = (unsigned long *)malloc(sizeof(unsigned long) * 4);
    		if(!rawCsd)
    			return SYS_ERR_OUT_OF_MEMORY;
    	}
    	getRawCsd_Ptr(driverHandler_Ptr, rawCsd);
    }
    else
        return SYS_ERR_NOT_IMPLEMENTED;
    return SYS_ERR_OK;
}


