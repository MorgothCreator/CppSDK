/*
 *  lib/buffers/ring_buff.h
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

/*#####################################################*/
#ifdef USE_VISUAL_STUDIO
#include "ring_buff.cpp"
#endif
/*#####################################################*/
#endif
