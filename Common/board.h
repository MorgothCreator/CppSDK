/*
 * board.h
 *
 *  Created on: Dec 9, 2016
 *      Author: John Smith
 */

#ifndef STM32F_BOARD_BOARD_H_
#define STM32F_BOARD_BOARD_H_

#if defined(STM32F429I_DISCO)
#include <board/STM32F429I-DISCO/defs.h>
#elif defined(STM32F469I_DISCO)
#include <board/STM32F469I-DISCO/defs.h>
#elif defined(STM32F769I_DISCO)
#include <board/STM32F769I-DISCO/defs.h>
#endif

#endif /* STM32F7_BOARD_BOARD_H_ */
