/********************************************************************************************************
 * @file     timer.c
 *
 * @brief    Interface of hardware timer
 *
 * @author
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

#include "../tl_common.h"


#define TIMER_SAFE_BOUNDARY_IN_US(v)		(10 * v)
#define TIMER_OVERFLOW_VALUE        		(0xFFFFFFFE)

typedef struct {
    /* Expire time and callback parameters */
    ext_clk_t expireInfo;
    timerCb_t cb;
    void	  *arg;

    /* Flags for useful information */
    union {
        struct {
            u32 status:2;
            u32 reserved:30;
        }bf;
        u32 byteVal;
    } flags; 
} hwTmr_info_t;

typedef struct {
    hwTmr_info_t timerInfo[TIMER_NUM];
} hwTmr_ctrl_t;


hwTmr_ctrl_t hwTmr_vars;




static void drv_hwTmr_reset(u8 tmrIdx)
{
	if(tmrIdx < TIMER_NUM){
		memset((u8*)&hwTmr_vars.timerInfo[tmrIdx], 0, sizeof(hwTmr_info_t));
	}
}

static hw_timer_sts_t drv_hwTmr_setAbs(u8 tmrIdx, ext_clk_t *absTimer, timerCb_t func, void *arg)
{
	if(tmrIdx >= TIMER_NUM){
		return HW_TIMER_IS_RUNNING;
	}
    hwTmr_info_t *pTimer = &hwTmr_vars.timerInfo[tmrIdx];

    /* Validation */
    if(TIMER_IDLE != pTimer->flags.bf.status){
        return HW_TIMER_IS_RUNNING;
    }

    u32 r = disable_irq();

    /* Set expire information */
    pTimer->expireInfo.high = absTimer->high;
    pTimer->expireInfo.low  = absTimer->low;
    pTimer->cb = func;
    pTimer->arg = arg;

    pTimer->flags.bf.status = pTimer->expireInfo.high ? TIMER_WOF : TIMER_WTO;

    /* Safety Check - If time is already past, set timer as Expired */
    if(!pTimer->expireInfo.high && pTimer->expireInfo.low < TIMER_SAFE_BOUNDARY_IN_US(MASTER_CLK_FREQ)){
    	restore_irq(r);
        memset(pTimer, 0, sizeof(hwTmr_info_t));
		if(func){
            func(arg);
		}
        return HW_TIMER_SUCC;
    }else{
    	if(tmrIdx < TIMER_IDX_3){
			/* Set compare value */
			TIMER_TICK_CLEAR(tmrIdx);
			TIMER_INTERVAL_SET(tmrIdx, pTimer->expireInfo.high ? TIMER_OVERFLOW_VALUE : pTimer->expireInfo.low);

			/* Enable Timer */
			TIMER_START(tmrIdx);
    	}else{
    		u32 tick = clock_time() + (pTimer->expireInfo.high ? TIMER_OVERFLOW_VALUE : pTimer->expireInfo.low);
    		SYS_TIMER_INTERVAL_SET(tick);
    		SYS_TIMER_START();
    	}
    }

    restore_irq(r);

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
        		if(tmrIdx < TIMER_IDX_3){
        			TIMER_STOP(tmrIdx);
        		}else{
        			SYS_TIMER_STOP();
        		}
        		memset(pTimer, 0, sizeof(hwTmr_info_t));
        	}else if(t == 0){
        		if(tmrIdx < TIMER_IDX_3){
					/* do nothing, use the previous configuration. */
        		}else{
					u32 tick = clock_time() + (pTimer->expireInfo.high ? TIMER_OVERFLOW_VALUE : pTimer->expireInfo.low);
					SYS_TIMER_INTERVAL_SET(tick);
        		}
        	}else{
        		pTimer->expireInfo.low = t * MASTER_CLK_FREQ;

        		if(tmrIdx < TIMER_IDX_3){
					TIMER_INTERVAL_SET(tmrIdx, pTimer->expireInfo.high ? TIMER_OVERFLOW_VALUE : pTimer->expireInfo.low);
        		}else{
					u32 tick = clock_time() + (pTimer->expireInfo.high ? TIMER_OVERFLOW_VALUE : pTimer->expireInfo.low);
					SYS_TIMER_INTERVAL_SET(tick);
        		}
        	}
        }
    }else{
        if(--pTimer->expireInfo.high){
        	if(tmrIdx < TIMER_IDX_3){
        		TIMER_INTERVAL_SET(tmrIdx, TIMER_OVERFLOW_VALUE);
        	}else{
        		u32 tick = clock_time() + TIMER_OVERFLOW_VALUE;
        		SYS_TIMER_INTERVAL_SET(tick);
        	}
        }else{
        	if(tmrIdx < TIMER_IDX_3){
        		TIMER_INTERVAL_SET(tmrIdx, pTimer->expireInfo.low);
        	}else{
        		u32 tick = clock_time() + pTimer->expireInfo.low;
        		SYS_TIMER_INTERVAL_SET(tick);
        	}

            pTimer->flags.bf.status = TIMER_WTO;
        }
    }
}

void drv_hwTmr_init(u8 tmrIdx, u8 mode)
{
	if(tmrIdx < TIMER_NUM){
		drv_hwTmr_reset(tmrIdx);

		if(tmrIdx < TIMER_IDX_3){
			TIMER_INIT(tmrIdx, mode);
		}else{
			SYS_TIMER_INIT();
		}
	}
}

void drv_hwTmr_cancel(u8 tmrIdx)
{
	if(tmrIdx < TIMER_NUM){
		u32 r = disable_irq();

		if(tmrIdx < TIMER_IDX_3){
			/* Write 1 to clear, even if the timer is already set, cancel */
			TIMER_STATE_CLEAR(tmrIdx);
			TIMER_STOP(tmrIdx);
		}else{
			SYS_TIMER_STOP();
		}

		hwTmr_info_t *pTimer = &hwTmr_vars.timerInfo[tmrIdx];
		memset(pTimer, 0, sizeof(hwTmr_info_t));

		restore_irq(r);
	}
}

hw_timer_sts_t drv_hwTmr_set(u8 tmrIdx, u32 t_us, timerCb_t func, void *arg)
{
    ext_clk_t t;
    t.high = 0;
    if(tmrIdx < TIMER_IDX_3){
    	t.low = t_us * MASTER_CLK_FREQ;
    }else{
    	t.low = t_us * CLOCK_SYS_CLOCK_1US;
    }
    return drv_hwTmr_setAbs(tmrIdx, &t, func, arg);
}



void drv_timer_irq0_handler(void)
{
	/* Write 1 to clear */
	TIMER_STATE_CLEAR(TIMER_IDX_0);

	drv_hwTmr_irq_process(TIMER_IDX_0);
	//timer_irq0_process(TIMER_IDX_0);
}

void drv_timer_irq1_handler(void)
{
	/* Write 1 to clear */
	TIMER_STATE_CLEAR(TIMER_IDX_1);

	drv_hwTmr_irq_process(TIMER_IDX_1);
}

void drv_timer_irq3_handler(void)
{
	SYS_TIMER_STATE_CLEAR();

	drv_hwTmr_irq_process(TIMER_IDX_3);
}

