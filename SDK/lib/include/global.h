/*
 * global.hpp
 *
 *  Created on: Dec 9, 2016
 *      Author: John Smith
 */

#ifndef LIB_API_GLOBAL_DEF_HPP_
#define LIB_API_GLOBAL_DEF_HPP_

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
}SysErr;

typedef struct charDev_s
{

} charDev;

#endif /* LIB_API_GLOBAL_DEF_HPP_ */
