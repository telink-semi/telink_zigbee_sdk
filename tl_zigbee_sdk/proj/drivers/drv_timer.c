/********************************************************************************************************
 * @file	drv_timer.c
 *
 * @brief	This is the source file for drv_timer
 *
 * @author	Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#include "../tl_common.h"



#define TIMER_SAFE_BOUNDARY_IN_US(v)		(10 * v)
#define TIMER_OVERFLOW_VALUE        		(0xFFFFFFFE)


typedef struct{
    /* Expire time and callback parameters */
    ext_clk_t expireInfo;
    timerCb_t cb;
    void	  *arg;

    /* Flags for useful information */
    union{
        struct{
            u32 status:2;
            u32 reserved:30;
        }bf;
        u32 byteVal;
    }flags;
}hwTmr_info_t;

typedef struct{
    hwTmr_info_t timerInfo[TIMER_NUM];
}hwTmr_ctrl_t;


hwTmr_ctrl_t hwTmr_vars;


static void hwTimerInit(u8 tmrIdx, u8 mode)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	if(tmrIdx < TIMER_IDX_3){
		timer_set_init_tick(tmrIdx, 0);
		timer_set_mode(tmrIdx, mode);
		timer_irq_enable(tmrIdx);
	}else{
		stimer_irq_enable();
	}
#elif defined(MCU_CORE_B91)
	if(tmrIdx < TIMER_IDX_3){
		timer_set_init_tick(tmrIdx, 0);
		timer_set_mode(tmrIdx, mode);
		if(tmrIdx == TIMER_IDX_0){
			plic_interrupt_enable(IRQ4_TIMER0);
		}else if(tmrIdx == TIMER_IDX_1){
			plic_interrupt_enable(IRQ3_TIMER1);
		}
	}else{
		plic_interrupt_enable(IRQ1_SYSTIMER);
	}
#endif
}

static void hwTimerSet(u8 tmrIdx, u32 tick)
{
	/* Set capture tick value */
	if(tmrIdx < TIMER_IDX_3){
		timer_set_cap_tick(tmrIdx, tick);
	}else{
		stimer_set_irq_capture(tick + clock_time());
	}
}

static void hwTimerStart(u8 tmrIdx)
{
	if(tmrIdx < TIMER_IDX_3){
		timer_start(tmrIdx);
	}else{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
		stimer_set_irq_mask();
#elif defined(MCU_CORE_B91)
		stimer_set_irq_mask(FLD_SYSTEM_IRQ);
#endif
	}
}

static void hwTimerStop(u8 tmrIdx)
{
	if(tmrIdx < TIMER_IDX_3){
		timer_stop(tmrIdx);
	}else{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
		stimer_clr_irq_mask();
#elif defined(MCU_CORE_B91)
		stimer_clr_irq_mask(FLD_SYSTEM_IRQ);
#endif
	}
}


static hw_timer_sts_t hwTmr_setAbs(u8 tmrIdx, ext_clk_t *absTimer, timerCb_t func, void *arg)
{
    hwTmr_info_t *pTimer = &hwTmr_vars.timerInfo[tmrIdx];

    /* Validation */
    if(TIMER_IDLE != pTimer->flags.bf.status){
        return HW_TIMER_IS_RUNNING;
    }

    u32 r = drv_disable_irq();

    /* Set expire information */
    pTimer->expireInfo.high = absTimer->high;
    pTimer->expireInfo.low  = absTimer->low;
    pTimer->cb = func;
    pTimer->arg = arg;

    pTimer->flags.bf.status = pTimer->expireInfo.high ? TIMER_WOF : TIMER_WTO;

    /* Safety Check - If time is already past, set timer as Expired */
    if(!pTimer->expireInfo.high && pTimer->expireInfo.low < TIMER_SAFE_BOUNDARY_IN_US(TIMER_TICK_1US_GET(tmrIdx))){
    	drv_restore_irq(r);
        memset(pTimer, 0, sizeof(hwTmr_info_t));
		if(func){
            func(arg);
		}
        return HW_TIMER_SUCC;
    }else{
    	hwTimerSet(tmrIdx, pTimer->expireInfo.high ? TIMER_OVERFLOW_VALUE : pTimer->expireInfo.low);
    	hwTimerStart(tmrIdx);
    }

    drv_restore_irq(r);

    return HW_TIMER_SUCC;
}

static void drv_hwTmr_irq_process(u8 tmrIdx)
{
    hwTmr_info_t *pTimer = &hwTmr_vars.timerInfo[tmrIdx];

    if(TIMER_WTO == pTimer->flags.bf.status){
        /* Expired, callback */
        if(pTimer->cb){
        	int t;

        	t = pTimer->cb(pTimer->arg);

        	if(t < 0){
        		hwTimerStop(tmrIdx);
        		memset(pTimer, 0, sizeof(hwTmr_info_t));
        	}else if(t == 0){
        		if(tmrIdx < TIMER_IDX_3){
					/* do nothing, use the previous configuration. */
        		}else{
        			/* STimer need set capture tick again. */
					hwTimerSet(tmrIdx, pTimer->expireInfo.high ? TIMER_OVERFLOW_VALUE : pTimer->expireInfo.low);
        		}
        	}else{
        		pTimer->expireInfo.low = t * TIMER_TICK_1US_GET(tmrIdx);

        		hwTimerSet(tmrIdx, pTimer->expireInfo.high ? TIMER_OVERFLOW_VALUE : pTimer->expireInfo.low);
        	}
        }
    }else{
        if(--pTimer->expireInfo.high){
        	hwTimerSet(tmrIdx, TIMER_OVERFLOW_VALUE);
        }else{
        	hwTimerSet(tmrIdx, pTimer->expireInfo.low);

            pTimer->flags.bf.status = TIMER_WTO;
        }
    }
}

static void hwTimerInfoReset(u8 tmrIdx)
{
	memset((u8*)&hwTmr_vars.timerInfo[tmrIdx], 0, sizeof(hwTmr_info_t));
}

void drv_hwTmr_init(u8 tmrIdx, u8 mode)
{
	if(tmrIdx >= TIMER_NUM){
		return;
	}

	hwTimerInfoReset(tmrIdx);
	hwTimerInit(tmrIdx, mode);
}

void drv_hwTmr_cancel(u8 tmrIdx)
{
	if(tmrIdx >= TIMER_NUM){
		return;
	}

	u32 r = drv_disable_irq();

	hwTimerStop(tmrIdx);

	hwTmr_info_t *pTimer = &hwTmr_vars.timerInfo[tmrIdx];
	memset(pTimer, 0, sizeof(hwTmr_info_t));

	drv_restore_irq(r);
}

hw_timer_sts_t drv_hwTmr_set(u8 tmrIdx, u32 t_us, timerCb_t func, void *arg)
{
	if(tmrIdx >= TIMER_NUM){
		return HW_TIMER_INVALID;
	}

    ext_clk_t t;
    t.high = 0;
    t.low = t_us * TIMER_TICK_1US_GET(tmrIdx);

    return hwTmr_setAbs(tmrIdx, &t, func, arg);
}



void drv_timer_irq0_handler(void)
{
	drv_hwTmr_irq_process(TIMER_IDX_0);
}

void drv_timer_irq1_handler(void)
{
	drv_hwTmr_irq_process(TIMER_IDX_1);
}

void drv_timer_irq3_handler(void)
{
	drv_hwTmr_irq_process(TIMER_IDX_3);
}

