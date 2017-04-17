/*
 *  buffers/ring_buff.h
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
#ifndef __RING_BUFF_H__
#define __RING_BUFF_H__
//###########################################################################
#include <stdbool.h>
#ifdef _UTIL_ATOMIC_H_
#include <util/atomic.h>
#endif
#include <include/global.h>
//###########################################################################
namespace GI
{
namespace Buff
{
class RingBuff {
public:
	RingBuff(u32 elements, u32 elementSize);
	RingBuff(u32 elements);
	~RingBuff();
	bool push(u8 data);
	bool push(u16 data);
	bool push(u32 data);
	bool push(u64 data);
	bool push(void *data);
	s32 push(u8 *buff, s32 len);
	bool pop(u8 *data);
	bool pop(u16 *data);
	bool pop(u32 *data);
	bool pop(u64 *data);
	s32 pop(u8 *buff, s32 len);
	SysErr reinit(u32 elements, u32 elementSize);
	SysErr reinit(u32 elements);
	void deinit();
	u32 elemenmtsOnBuff;
	u32 elements;
	u32 elementSize;
private:
	void* buff;
	u32 pushPtr;
	u32 popPtr;
};
}
}
//###########################################################################
//###########################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "ring_buff.c"
#endif
//###########################################################################
#endif
