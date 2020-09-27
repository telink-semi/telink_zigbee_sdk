/********************************************************************************************************
 * @file     clock_826x.h
 *
 * @brief    system clock configuration for tlsr826x
 *
 * @author
 * @date     Dec. 1, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#pragma once

#ifdef WIN32
#include <time.h>
#endif

#include "compiler.h"
#include "bit.h"
#include "types.h"
#include "register_826x.h"


#define WATCHDOG_INIT_TIMEOUT		600

enum{
	CLOCK_SEL_32M_RC = 	0,
	CLOCK_SEL_HS_DIV = 	1,
	CLOCK_SEL_16M_PAD =	2,
	CLOCK_SEL_32M_PAD =	3,
	CLOCK_SEL_SPI  	  = 4,
	CLOCK_SEL_40M_INTERNAL = 5,
	CLOCK_SEL_32K_RC  =	6,
};


typedef enum{
	SYS_CLK_16M_PLL,
	SYS_CLK_32M_PLL,
	SYS_CLK_32M_RC,
	SYS_CLK_16M_RC,
	SYS_CLK_8M_RC,
}SYS_CLK_TYPEDEF;


extern unsigned long CLOCK_SYS_CLOCK_1US;
extern unsigned long tickPerUs;

#define MASTER_CLK_FREQ		tickPerUs


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


void clock_init(SYS_CLK_TYPEDEF SYS_CLK);

static inline u32 clock_time(){
	return reg_system_tick;
}

static inline u32 clock_time2(int tmr){
	return reg_tmr_tick(tmr);
}

// check if the current time is exceed span_us from ref time
static inline u32 clock_time_exceed(u32 ref, u32 span_us){
	return ((u32)(clock_time() - ref) > span_us * CLOCK_SYS_CLOCK_1US);
}

// more efficient than clock_set_interval
static inline void clock_set_tmr_interval(int tmr, u32 intv){
	reg_tmr_capt(tmr) = intv;
}

static inline void clock_set_tmr_mode(int tmr, u32 m){
	if(0 == tmr){
		SET_FLD_V(reg_tmr_ctrl16, FLD_TMR0_MODE, m);
	}else if(1 == tmr){
		SET_FLD_V(reg_tmr_ctrl16, FLD_TMR1_MODE, m);
	}else{
		SET_FLD_V(reg_tmr_ctrl16, FLD_TMR2_MODE, m);
	}
}

static inline u32 clock_get_tmr_status(int tmr){
	if(0 == tmr){
		return reg_tmr_ctrl & FLD_TMR0_STA;
	}else if(1 == tmr){
		return reg_tmr_ctrl & FLD_TMR1_STA;
	}else{
		return reg_tmr_ctrl & FLD_TMR2_STA;
	}
}


//  watchdog use timer 2
static inline void wd_setintervalms(u32 interval_ms)
{
	reg_tmr_ctrl |= MASK_VAL(FLD_TMR_WD_CAPT, (interval_ms * MASTER_CLK_FREQ * 1000 >> WATCHDOG_TIMEOUT_COEFF));
}

//  watchdog use timer 2
static inline void wd_start(void)
{
	SET_FLD(reg_tmr_ctrl, FLD_TMR_WD_EN);
}

static inline void wd_stop(void)
{
	CLR_FLD(reg_tmr_ctrl, FLD_TMR_WD_EN);
}

static inline void wd_clear(void)
{
	SET_FLD(reg_tmr_ctrl, FLD_CLR_WD);
}


#define TIMER_INIT(idx, mode)			do{ \
										   clock_set_tmr_mode(idx, mode);	\
										   reg_tmr_sta = 1 << idx;			\
										   reg_irq_src = 1 << idx;			\
										   reg_irq_mask |= 1 << idx;		\
									    }while(0)

#define TIMER_START(idx)				clock_enable_clock(idx, 1)
#define TIMER_STOP(idx)					clock_enable_clock(idx, 0)

#define TIMER_TICK_CLEAR(idx)   		reg_tmr_tick(idx) = 0
#define TIMER_INTERVAL_SET(idx, cyc)	reg_tmr_capt(idx) = cyc

#define TIMER_STATE_CLEAR(idx) 			reg_tmr_sta = (1 << idx)


#define SYS_TIMER_INIT()				do{	\
											reg_irq_src = FLD_IRQ_SYSTEM_TIMER;				\
											reg_system_tick_mode |= FLD_SYSTEM_TICK_IRQ_EN;	\
											reg_irq_mask &= ~(u32)FLD_IRQ_SYSTEM_TIMER;		\
										}while(0)

#define SYS_TIMER_START()				do{	\
											reg_irq_mask |= FLD_IRQ_SYSTEM_TIMER;			\
										}while(0)

#define SYS_TIMER_STOP()				do{	\
											reg_irq_mask &= ~(u32)FLD_IRQ_SYSTEM_TIMER;		\
										}while(0)

#define SYS_TIMER_INTERVAL_SET(cyc)		do{	\
											reg_system_tick_irq = cyc;	\
										}while(0)

#define SYS_TIMER_STATE_CLEAR()
