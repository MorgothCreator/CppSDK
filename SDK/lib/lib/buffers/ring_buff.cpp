/*
 *  buffers/ring_buff.cpp
 *
 *  Copyright (C) 2016  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of CppSDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __FiFo__
#define __FiFo__
//###########################################################################
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#ifdef _UTIL_ATOMIC_H_
#include <util/atomic.h>
#endif
#include "ring_buff.h"
//###########################################################################
GI::Buff::RingBuff::RingBuff(u32 elements, u32 elementSize)
{
	memset(this, 0, sizeof(*this));
	if ((buff = (u8*) malloc(elements * elementSize)) == 0)
		return;
	this->elements = elements;
	this->elementSize = elementSize;
}
//###########################################################################
GI::Buff::RingBuff::RingBuff(u32 elements)
{
	memset(this, 0, sizeof(*this));
	if ((buff = (u8*) malloc(elements)) == 0)
		return;
	this->elements = elements;
	this->elementSize = 1;
}
//###########################################################################
GI::Buff::RingBuff::~RingBuff()
{
	if(buff)
		free(buff);
}
//###########################################################################
bool GI::Buff::RingBuff::push(u8 data)
{
	if(elementSize != 1)
		return false;
	bool ret = false;
#ifdef _UTIL_ATOMIC_H_
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
#endif
	unsigned int BuffSize = elements;
	unsigned int BytesToBuff = elemenmtsOnBuff;
	if (BytesToBuff < BuffSize)
	{
		unsigned int PushPtr = pushPtr;
		((u8 *)buff)[pushPtr] = data;
		PushPtr++;
		if (PushPtr == BuffSize)
			PushPtr = 0;
		BytesToBuff++;
		pushPtr = PushPtr;
		elemenmtsOnBuff = BytesToBuff;
		ret = true;
	}
#ifdef _UTIL_ATOMIC_H_
}
#endif
	return ret;
}
//###########################################################################
bool GI::Buff::RingBuff::push(u16 data)
{
	if(elementSize != 2)
		return false;
	bool ret = false;
#ifdef _UTIL_ATOMIC_H_
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
#endif
	unsigned int BuffSize = elements;
	unsigned int BytesToBuff = elemenmtsOnBuff;
	if (BytesToBuff < BuffSize)
	{
		unsigned int PushPtr = pushPtr;
		((u16 *)(buff))[pushPtr] = data;
		PushPtr++;
		if (PushPtr == BuffSize)
			PushPtr = 0;
		BytesToBuff++;
		pushPtr = PushPtr;
		elemenmtsOnBuff = BytesToBuff;
		ret = true;
	}
#ifdef _UTIL_ATOMIC_H_
}
#endif
	return ret;
}
//###########################################################################
bool GI::Buff::RingBuff::push(u32 data)
{
	if(elementSize != 4)
		return false;
	bool ret = false;
#ifdef _UTIL_ATOMIC_H_
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
#endif
	unsigned int BuffSize = elements;
	unsigned int BytesToBuff = elemenmtsOnBuff;
	if (BytesToBuff < BuffSize)
	{
		unsigned int PushPtr = pushPtr;
		((u32 *)buff)[pushPtr] = data;
		PushPtr++;
		if (PushPtr == BuffSize)
			PushPtr = 0;
		BytesToBuff++;
		pushPtr = PushPtr;
		elemenmtsOnBuff = BytesToBuff;
		ret = true;
	}
#ifdef _UTIL_ATOMIC_H_
}
#endif
	return ret;
}
//###########################################################################
bool GI::Buff::RingBuff::push(u64 data)
{
	if(elementSize != 8)
		return false;
	bool ret = false;
#ifdef _UTIL_ATOMIC_H_
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
#endif
	unsigned int BuffSize = elements;
	unsigned int BytesToBuff = elemenmtsOnBuff;
	if (BytesToBuff < BuffSize)
	{
		unsigned int PushPtr = pushPtr;
		((u64 *)buff)[pushPtr] = data;
		PushPtr++;
		if (PushPtr == BuffSize)
			PushPtr = 0;
		BytesToBuff++;
		pushPtr = PushPtr;
		elemenmtsOnBuff = BytesToBuff;
		ret = true;
	}
#ifdef _UTIL_ATOMIC_H_
}
#endif
	return ret;
}
//###########################################################################
s32 GI::Buff::RingBuff::push(u8 *buff, s32 len)
{
	s32 cnt = 0;
	for (; cnt < len; cnt++)
	{
		if (!push(buff[cnt]))
		{
			return cnt;
		}
	}
	return cnt;
}
//###########################################################################
bool GI::Buff::RingBuff::pop(u8 *data)
{
	if(elementSize != 1)
		return false;
	bool res;
#ifdef _UTIL_ATOMIC_H_
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
#endif
	unsigned int BytesToBuff = elemenmtsOnBuff;
	if (BytesToBuff == 0)
		res = false;
	else
	{
		unsigned int PopPtr = popPtr;
		*data = ((u8 *)buff)[PopPtr];
		PopPtr++;
		if (PopPtr == elements)
			PopPtr = 0;
		BytesToBuff--;
		elemenmtsOnBuff = BytesToBuff;
		popPtr = PopPtr;
		res = true;
	}
#ifdef _UTIL_ATOMIC_H_
}
#endif
	return res;
}
//###########################################################################
bool GI::Buff::RingBuff::pop(u16 *data)
{
	if(elementSize != 2)
		return false;
	bool res;
#ifdef _UTIL_ATOMIC_H_
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
#endif
	unsigned int BytesToBuff = elemenmtsOnBuff;
	if (BytesToBuff == 0)
		res = false;
	else
	{
		unsigned int PopPtr = popPtr;
		*data = ((u16 *)buff)[PopPtr];
		PopPtr++;
		if (PopPtr == elements)
			PopPtr = 0;
		BytesToBuff--;
		elemenmtsOnBuff = BytesToBuff;
		popPtr = PopPtr;
		res = true;
	}
#ifdef _UTIL_ATOMIC_H_
}
#endif
	return res;
}
//###########################################################################
bool GI::Buff::RingBuff::pop(u32 *data)
{
	if(elementSize != 4)
		return false;
	bool res;
#ifdef _UTIL_ATOMIC_H_
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
#endif
	unsigned int BytesToBuff = elemenmtsOnBuff;
	if (BytesToBuff == 0)
		res = false;
	else
	{
		unsigned int PopPtr = popPtr;
		*data = ((u32 *)buff)[PopPtr];
		PopPtr++;
		if (PopPtr == elements)
			PopPtr = 0;
		BytesToBuff--;
		elemenmtsOnBuff = BytesToBuff;
		popPtr = PopPtr;
		res = true;
	}
#ifdef _UTIL_ATOMIC_H_
}
#endif
	return res;
}
//###########################################################################
bool GI::Buff::RingBuff::pop(u64 *data)
{
	if(elementSize != 8)
		return false;
	bool res;
#ifdef _UTIL_ATOMIC_H_
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
#endif
	unsigned int BytesToBuff = elemenmtsOnBuff;
	if (BytesToBuff == 0)
		res = false;
	else
	{
		unsigned int PopPtr = popPtr;
		*data = ((u64 *)buff)[PopPtr];
		PopPtr++;
		if (PopPtr == elements)
			PopPtr = 0;
		BytesToBuff--;
		elemenmtsOnBuff = BytesToBuff;
		popPtr = PopPtr;
		res = true;
	}
#ifdef _UTIL_ATOMIC_H_
}
#endif
	return res;
}
//###########################################################################
s32 GI::Buff::RingBuff::pop(u8 *buff, s32 len)
{
	s32 cnt = 0;
	for (; cnt < len; cnt++)
	{
		if (!pop(&buff[cnt]))
			return cnt;
	}
	return cnt;
}
//###########################################################################
SysErr GI::Buff::RingBuff::reinit(u32 elements, u32 elementSize)
{
	deinit();
	if ((buff = (u8*) malloc(elements * elementSize)) == 0)
	{
		return SYS_ERR_OUT_OF_MEMORY;
	}
	this->elements = elements;
	this->elementSize = elementSize;
	return SYS_ERR_OK;
}
//###########################################################################
SysErr GI::Buff::RingBuff::reinit(u32 elements)
{
	deinit();
	if ((buff = (u8*) malloc(elements)) == 0)
	{
		return SYS_ERR_OUT_OF_MEMORY;
	}
	this->elements = elements;
	this->elementSize = 1;
	return SYS_ERR_OK;
}
//###########################################################################
void GI::Buff::RingBuff::deinit()
{
	if(buff)
		free(buff);
}
//###########################################################################
#endif
