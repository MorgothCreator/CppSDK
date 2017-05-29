/*
 *  include/global.h
 */

#ifndef LIB_API_GLOBAL_DEF_HPP_
#define LIB_API_GLOBAL_DEF_HPP_
/*
 * Sys errors definitions.
 */
typedef enum {
	SYS_ERR_OK 						=   0,
	SYS_ERR_UNKNOWN					=  -1,
	SYS_ERR_INVALID_HANDLER			=  -2,
	SYS_ERR_OUT_OF_MEMORY			=  -3,
	SYS_ERR_OUT_OF_RANGE			=  -4,
	SYS_ERR_INVALID_COMMAND			=  -5,
	SYS_ERR_INVALID_PARAM			=  -6,
	SYS_ERR_INIT					=  -7,
	SYS_ERR_NOT_IMPLEMENTED			=  -8,
	SYS_ERR_NOT_INITIALIZED			=  -9,
	SYS_ERR_NO_PATH					= -10,
	SYS_ERR_INVALID_PATH			= -11,
	SYS_ERR_NOT_ACTIVE				= -12,
	SYS_ERR_NAK						= -13,
	SYS_ERR_BUSY					= -14,
	SYS_ERR_WRITE					= -15,
	SYS_ERR_READ					= -16,
	SYS_ERR_NO_REGISTERED_DEVICE	= -17,
	SYS_ERR_BUFF_NOT_ALLOCATED		= -18,
	SYS_ERR_BUS						= -19,
	SYS_ERR_NO_ASSIGNED_DEVICE		= -20,
	SYS_ERR_DEVICE_NOT_MATCH		= -21,
	SYS_ERR_DEVICE_NOT_FOUND		= -22,
	SYS_ERR_TIMEOUT					= -23,
	SYS_ERR_NOTHING_CHANGED			= -24,
    SYS_ERR_NOT_RECEIVED            = -25,
    SYS_ERR_TRANSMISSION            = -26,
    SYS_ERR_RECEPTION               = -27,
    SYS_ERR_TOO_LONG                = -28,
    SYS_ERR_OUT_OF_PAGE            =  -29,
    SYS_ERR_INVALID_SIZE           =  -30,
}SysErr;

typedef enum {
    size1 = 0,
    size2,
    size4,
    size8,
    size16,
    size32,
    size64,
    size128,
    size256,
    size512,
    size1024,
    size2048,
    size4096,
    size8192,
    size16384,
    size32768,
    size65536,
    size131072,
    size262144,
    size524288,
    size1048576,
    size2097152,
    size4194304,
    size8388608,
    size16777216,
}Sizes;

/*
 * Int definitions.
 */

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned long u32;
typedef signed long s32;
typedef unsigned int ui;
typedef signed int si;
typedef unsigned long long u64;
typedef signed long long s64;
typedef float ff;
typedef double dd;

typedef char* string;

/*
 * Graphic definitions.
 */

class Color {
public:
	typedef union {
		struct {
			u8 B;
			u8 G;
			u8 R;
			u8 A;
		};
		u32 ARGB;
		u32 RGB;
	}ARGB;
	static Color::ARGB fromArgb(u8 red, u8 green, u8 blue, u8 alpha);
	static Color::ARGB fromArgb(u8 red, u8 green, u8 blue);
};

typedef struct charDev_s
{

} charDev;

/*#####################################################*/
#ifdef USE_VISUAL_STUDIO
#include "global_color.cpp"
#endif
/*#####################################################*/


#endif /* LIB_API_GLOBAL_DEF_HPP_ */
