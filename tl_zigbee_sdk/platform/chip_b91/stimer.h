/********************************************************************************************************
 * @file    stimer.h
 *
 * @brief   This is the header file for B91
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
/**	@page STIMER
 *
 *	Introduction
 *	===============
 *	B91 stimer use 16M clock count, have stimer irq.
 *
 *	API Reference
 *	===============
 *	Header File: uart.h
 */
#ifndef STIMER_H_
#define STIMER_H_

#include <stdbool.h>
#include "compiler.h"
#include "reg_include/stimer_reg.h"

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/
#ifndef SYS_TIMER_AUTO_MODE
#define SYS_TIMER_AUTO_MODE     			1
#endif

/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/
/**
 * @brief define system clock tick per us/ms/s.
 */
enum{
	SYSTEM_TIMER_TICK_1US 		= 16,
	SYSTEM_TIMER_TICK_1MS 		= 16000,
	SYSTEM_TIMER_TICK_1S 		= 16000000,

	SYSTEM_TIMER_TICK_625US  	= 10000,  //625*16
	SYSTEM_TIMER_TICK_1250US 	= 20000,  //1250*16
};


/**
 * @brief	This enumeration defines how many 32k ticks are reached, update the tick value of the increased system timer into the register.
 * 			//32k_cnt = 2^(16-(cal_32k_mode>>4)), system_timer_cnt = 500*32k_cnt.(system timer is 16M).
 */
typedef enum{
	STIMER_TRACK_32KCNT_2		= 0xf0, /* 2 32k ticks, corresponds 0x3e8 system timer ticks. */
	STIMER_TRACK_32KCNT_4		= 0xe0, /* 4 32k ticks, corresponds 0x7d0 system timer ticks. */
	STIMER_TRACK_32KCNT_8		= 0xd0, /* 8 32k ticks, corresponds 0xfa0 system timer ticks. */
	STIMER_TRACK_32KCNT_16		= 0xc0, /* 16 32k ticks, corresponds 0x1f40 system timer ticks. */
	STIMER_TRACK_32KCNT_32		= 0xb0, /* 32 32k ticks, corresponds 0x3e80 system timer ticks. */
	STIMER_TRACK_32KCNT_64		= 0xa0, /* 64 32k ticks, corresponds 0x7d00 system timer ticks. */
	STIMER_TRACK_32KCNT_128		= 0x90, /* 128 32k ticks, corresponds 0xfa00 system timer ticks. */
	STIMER_TRACK_32KCNT_256		= 0x80, /* 256 32k ticks, corresponds 0x1f400 system timer ticks. */
	STIMER_TRACK_32KCNT_512		= 0x70, /* 512 32k ticks, corresponds 0x3e800 system timer ticks. */
	STIMER_TRACK_32KCNT_1024	= 0x60, /* 1024 32k ticks, corresponds 0x7d000 system timer ticks. */
	STIMER_TRACK_32KCNT_2048	= 0x50, /* 2048 32k ticks, corresponds 0xfa000 system timer ticks. */
	STIMER_TRACK_32KCNT_4096	= 0x40, /* 4096 32k ticks, corresponds 0x1f4000 system timer ticks. */
	STIMER_TRACK_32KCNT_8192	= 0x30, /* 8192 32k ticks, corresponds 0x3e8000 system timer ticks. */
	STIMER_TRACK_32KCNT_16384	= 0x20, /* 16384 32k ticks, corresponds 0x7d0000 system timer ticks. */
	STIMER_TRACK_32KCNT_32768	= 0x10, /* 32768 32k ticks, corresponds 0xfa0000 system timer ticks. */
	STIMER_TRACK_32KCNT_65536	= 0x00, /* 65536 32k ticks, corresponds 0x1f40000 system timer ticks. */
}stimer_track_cnt_e;

/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/
extern unsigned int g_track_32kcnt;

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/

/**
 * @brief This function servers to set stimer irq mask.
 * @param[in]	mask - the irq mask.
 * @return  	none.
 */
static inline void stimer_set_irq_mask(stimer_irq_e mask)
{
	reg_system_irq_mask |= mask;
}

/**
 * @brief This function servers to clear stimer irq mask.
 * @param[in] 	mask - the irq mask.
 * @return  	none.
 */
static inline void stimer_clr_irq_mask(stimer_irq_e mask)
{
	reg_system_irq_mask &= (~mask);
}

/**
 * @brief This function servers to clear stimer irq status.
 * @param[in] 	status - the irq status.
 * @return  	none.
 */
static inline void stimer_clr_irq_status(stimer_irq_e status)
{
	reg_system_cal_irq = (status);
}

/**
 * @brief This function servers to get stimer irq status.
 * @param[in] 	status - the irq status.
 * @retval	  non-zero   -  the interrupt occurred.
 * @retval	  zero  -  the interrupt did not occur.
 */
static inline unsigned char stimer_get_irq_status(stimer_irq_e status)
{
	return (reg_system_cal_irq & status);
}

/**
 * @brief This function servers to set tick irq capture.
 * @param[in] tick - the value of irq tick.
 * @return    none.
 */
static inline void stimer_set_irq_capture(unsigned int tick)
{
	reg_system_irq_level = (tick);
}

/**
 * @brief This function servers to set stimer tick.
 * @param[in] tick - the value of tick.
 * @return    none.
 */
static inline void stimer_set_tick(unsigned int tick)
{
	reg_system_tick = (tick);
}

/*
 * @brief     This function performs to get system timer tick.
 * @return    system timer tick value.
**/
static inline unsigned int stimer_get_tick(void)
{
	return reg_system_tick;
}

/**
 * @brief     This function serves to set timeout by us.
 * @param[in] ref  - reference tick of system timer .
 * @param[in] us   - count by us.
 * @return    true - timeout, false - not timeout
 */
static inline _Bool clock_time_exceed(unsigned int ref, unsigned int us)
{
	return ((unsigned int)(stimer_get_tick() - ref) > us * SYSTEM_TIMER_TICK_1US);
}
/**
 * @brief     This function performs to set delay time by us.
 * @param[in] microsec - need to delay.
 * @return    none
 */
_attribute_ram_code_sec_noinline_   void delay_us(unsigned int microsec);

/**
 * @brief     This function performs to set delay time by ms.
 * @param[in] millisec - need to delay.
 * @return    none
 */
_attribute_ram_code_sec_noinline_  void  delay_ms(unsigned int millisec);

/**********************************************************************************************************************
 *                          Internal interface, provided only for driver use.                                         *
 *********************************************************************************************************************/

/**
 * @brief	This interface is used to configure the 32k track mode.
 * 			(2^(16-(cnt>>4)))cycles of 32k clock indicates the tick number of the system timer.
 * @return	none.
 */
static inline void stimer_set_32k_track_cnt(stimer_track_cnt_e cnt)
{
	reg_system_ctrl = (reg_system_ctrl & 0x0f) | cnt;
	g_track_32kcnt = (1 << (16 - (cnt >> 4)));
}

/**
 * @brief	This interface is used to obtain 32k track count.
 * @return	32k track count.
 */
static inline unsigned char stimer_get_32k_track_cnt(void)
{
	return g_track_32kcnt;
}

/**
 * @brief     This function performs to get the number of cycles of the RC 32k clock converted to system timer clock.
 * @return    none.
 */
static inline unsigned int stimer_get_tracking_32k_value(void)
{
	return reg_system_cal_latch_32k;
}

#endif /* STIMER_H_ */
