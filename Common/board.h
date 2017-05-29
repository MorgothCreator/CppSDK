/*
 * board.h
 */

#ifndef _BOARD_BOARD_H_
#define _BOARD_BOARD_H_

#if defined(MikromediaForSTM32M4)
#include <board/STM/MikromediaForSTM32M4/defs.h>
#elif defined(STM32F429I_DISCO)
#include <board/STM/STM32F429I-DISCO/defs.h>
#elif defined(STM32F469I_DISCO)
#include <board/STM/STM32F469I-DISCO/defs.h>
#elif defined(STM32F769I_DISCO)
#include <board/STM/STM32F769I-DISCO/defs.h>
#elif defined(SmartRf06)
#include <board/TI/SmartRf06/defs.h>
#elif defined(FRDM_KEAZ128)
#include <board/NXP/FRDM-KEAZ128/defs.h>
#elif defined(Xmega_E5_Xplained)
#include <board/ATMEL/Xmega-E5_Xplained/defs.h>
#elif defined(FeatherAdaloggerM0)
#include <board/ATMEL/FeatherAdaloggerM0/defs.h>
#elif defined(mikroXmega)
#include <board/ATMEL/mikroXmega/defs.h>
#endif

#endif /* _BOARD_BOARD_H_ */
