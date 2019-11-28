/********************************************************************************************************
 * @file     clock.h
 *
 * @brief    system clock interface header for tlsr8258
 *
 * @author   jian.zhang@telink-semi.com
 * @date     Oct. 8, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#pragma once

#include "register.h"
#include "compiler.h"
#include "bit.h"
// constants
// system clock



//system timer clock source is constant 16M, never change
enum{
	CLOCK_16M_SYS_TIMER_CLK_1S =  16000000,
	CLOCK_16M_SYS_TIMER_CLK_1MS = 16000,
	CLOCK_16M_SYS_TIMER_CLK_1US = 16,
};

#define 	sys_tick_per_us			16
extern unsigned long CLOCK_SYS_CLOCK_1US;
extern unsigned long tickPerUs;

#define MASTER_CLK_FREQ				tickPerUs


//#define SET_SYS_TICK_IRQ(tick)   	(reg_system_tick_irq = (tick) - pm_systick_offset)


static inline unsigned long get_system_tick(void)
{
	return reg_system_tick;
}


#define _ASM_NOP_				asm("tnop")

//  delay precisely
#define		CLOCK_DLY_1_CYC    _ASM_NOP_
#define		CLOCK_DLY_2_CYC    _ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_3_CYC    _ASM_NOP_;_ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_4_CYC    _ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_5_CYC    _ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_6_CYC    _ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_7_CYC    _ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_8_CYC    _ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_9_CYC    _ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_
#define		CLOCK_DLY_10_CYC   _ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_;_ASM_NOP_

#if (CLOCK_SYS_CLOCK_HZ == 30000000 || CLOCK_SYS_CLOCK_HZ == 32000000)
	#define		CLOCK_DLY_100NS		CLOCK_DLY_3_CYC							// 100,  94
	#define		CLOCK_DLY_200NS		CLOCK_DLY_6_CYC							// 200, 188
	#define 	CLOCK_DLY_600NS 	CLOCK_DLY_10_CYC;CLOCK_DLY_10_CYC 		// 200, 188
#elif (CLOCK_SYS_CLOCK_HZ == 24000000)
	#define 	CLOCK_DLY_63NS 		CLOCK_DLY_3_CYC 		//  63 ns
	#define		CLOCK_DLY_100NS		CLOCK_DLY_4_CYC			//  100 ns
	#define		CLOCK_DLY_200NS		CLOCK_DLY_8_CYC			//  200 ns
	#define 	CLOCK_DLY_600NS 	CLOCK_DLY_10_CYC 		//	600 ns
#elif (CLOCK_SYS_CLOCK_HZ == 12000000 || CLOCK_SYS_CLOCK_HZ == 16000000)
	#define 	CLOCK_DLY_63NS 		CLOCK_DLY_1_CYC 		//  63 ns
	#define		CLOCK_DLY_100NS		CLOCK_DLY_2_CYC			//  128 ns
	#define		CLOCK_DLY_200NS		CLOCK_DLY_4_CYC			//  253 ns
	#define 	CLOCK_DLY_600NS 	CLOCK_DLY_10_CYC 		//	253 ns
#elif (CLOCK_SYS_CLOCK_HZ == 48000000)
	#define		CLOCK_DLY_100NS		CLOCK_DLY_5_CYC			// 104
	#define		CLOCK_DLY_200NS		CLOCK_DLY_10_CYC		// 208
	#define 	CLOCK_DLY_600NS 	CLOCK_DLY_10_CYC;CLOCK_DLY_10_CYC;CLOCK_DLY_10_CYC		//	600 ns
#elif (CLOCK_SYS_CLOCK_HZ == 6000000 || CLOCK_SYS_CLOCK_HZ == 8000000)
	#define		CLOCK_DLY_100NS		CLOCK_DLY_1_CYC			//  125 ns
	#define		CLOCK_DLY_200NS		CLOCK_DLY_2_CYC			//  250
	#define 	CLOCK_DLY_600NS 	CLOCK_DLY_5_CYC 		//  725
#else
#define		CLOCK_DLY_100NS		CLOCK_DLY_1_CYC			//  125 ns
#define		CLOCK_DLY_200NS		CLOCK_DLY_2_CYC			//  250
#define 	CLOCK_DLY_600NS 	CLOCK_DLY_5_CYC 		//  725
#endif

typedef enum{
	SYS_CLK_12M_Crystal,
	SYS_CLK_16M_Crystal,
	SYS_CLK_24M_Crystal,
	SYS_CLK_32M_Crystal,
	SYS_CLK_48M_Crystal,
	SYS_CLK_24M_RC,
	SYS_CLK_24M_Crystal_directly = 0x10,
}SYS_CLK_TYPEDEF;

void clock_init(SYS_CLK_TYPEDEF SYS_CLK);

_attribute_ram_code_ void sleep_us (unsigned long microsec);		//  use register counter to delay


#define WaitUs				sleep_us
#define WaitMs(t)			sleep_us((t)*1000)


static inline unsigned int clock_time(void)
{
	return reg_system_tick;
}


#define ClockTime				 clock_time


static inline unsigned int clock_time_exceed(unsigned int ref, unsigned int span_us){
	return ((unsigned int)(clock_time() - ref) > span_us * 16);
}

// we use clock insteady of timer, to differentiate OS timers utility
static inline void clock_enable_clock(int tmr, int en){
	if(0 == tmr){
		SET_FLD_V(reg_tmr_ctrl, FLD_TMR0_EN, en);
	}else if(1 == tmr){
		SET_FLD_V(reg_tmr_ctrl, FLD_TMR1_EN, en);
	}else{
		SET_FLD_V(reg_tmr_ctrl, FLD_TMR2_EN, en);
	}
}

static inline void clock_set_tmr_mode(int tmr, unsigned int m){
	if(0 == tmr){
		SET_FLD_V(reg_tmr_ctrl16, FLD_TMR0_MODE, m);
	}else if(1 == tmr){
		SET_FLD_V(reg_tmr_ctrl16, FLD_TMR1_MODE, m);
	}else{
		SET_FLD_V(reg_tmr_ctrl16, FLD_TMR2_MODE, m);
	}
}

static inline unsigned int clock_get_tmr_status(int tmr){
	if(0 == tmr){
		return reg_tmr_ctrl & FLD_TMR0_STA;
	}else if(1 == tmr){
		return reg_tmr_ctrl & FLD_TMR1_STA;
	}else{
		return reg_tmr_ctrl & FLD_TMR2_STA;
	}
}

#define TIMER_STATE_CLEAR(idx) 			reg_tmr_sta |= (1 << tmrIdx)
#define TIMER_STOP(idx)					clock_enable_clock(idx, 0)
#define TIMER_START(idx)				clock_enable_clock(idx, 1)
#define TIMER_TICK_CLEAR(idx)   		reg_tmr_tick(tmrIdx) = 0
#define TIMER_INTERVAL_SET(idx, cyc)	reg_tmr_capt(idx) = cyc
#define TIMER_INIT(tmrIdx, mode)			do{ \
										   clock_set_tmr_mode(tmrIdx, mode);\
										   reg_tmr_sta |= 1 << tmrIdx;	\
										   reg_irq_src |= 1 << tmrIdx;	\
										   reg_irq_mask |= 1 << tmrIdx;	\
									   }while(0)

#if 0
reg_irq_mask |= FLD_IRQ_TMR0_EN;
	reg_tmr0_tick = 0; //clear counter
	reg_tmr0_capt = 10 * CLOCK_SYS_CLOCK_1MS;
	reg_tmr_sta = FLD_TMR_STA_TMR0; //clear irq status
	reg_tmr_ctrl &= (~FLD_TMR0_MODE);
	reg_tmr_ctrl |= (TIMER_MODE_SYSCLK<<1);
	reg_tmr_ctrl |= FLD_TMR0_EN;reg_irq_mask |= FLD_IRQ_TMR0_EN;
	reg_tmr0_tick = 0; //clear counter
	reg_tmr0_capt = 10 * CLOCK_SYS_CLOCK_1MS;
	reg_tmr_sta = FLD_TMR_STA_TMR0; //clear irq status
	reg_tmr_ctrl &= (~FLD_TMR0_MODE);
	reg_tmr_ctrl |= (TIMER_MODE_SYSCLK<<1);
	reg_tmr_ctrl |= FLD_TMR0_EN;
#endif
