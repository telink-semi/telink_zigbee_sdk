/********************************************************************************************************
 * @file     pm_interface.c
 *
 * @brief    Power management interface
 *
 * @author
 * @date     Feb. 1, 2017
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
#include "tl_common.h"
#include "zb_common.h"
#include "pm_interface.h"

#if (PM_ENABLE)

void pm_wakeupPinConfig(pm_pinCfg_t *pmCfg, int pinNum){
	pm_pinCfg_t *p = pmCfg;
	for(s32 i = 0; i < pinNum; i++){
		platform_wakeup_pad_cfg(p->wakeupPin, p->wakeLevel, 1);
		p++;
	}
}

u8 pm_wakeupValid(pm_pinCfg_t *pmCfg, int pinNum){
	pm_pinCfg_t *p = pmCfg;
	for(s32 i = 0; i < pinNum; i++){
		if((p->wakeLevel == PLATFORM_WAKEUP_LEVEL_HIGH) && gpio_read(p->wakeupPin)){
			return 0;
		}

		if((p->wakeLevel == PLATFORM_WAKEUP_LEVEL_LOW) && !gpio_read(p->wakeupPin)){
			return 0;
		}
		p++;
	}
	return 1;
}

void pm_lowPowerEnter(platform_mode_e mode, int wakeUpSrc, u32 ms){
	/* If ms is 0, we use default value 120s for sleep mode. */
	u32 interval = (ms == 0) ? (120 * 1000) : ms;

	if(!tl_stackBusy() && zb_isTaskDone()){
		u32 r = disable_irq();

		interval = (ev_nearestInterval() <= (interval * CLOCK_SYS_CLOCK_1US * 1000)) ? ev_nearestInterval() / (CLOCK_SYS_CLOCK_1US * 1000)
																					 : interval;
		if(interval){
			extern void rf_paShutDown(void);
			rf_paShutDown();

			if(mode == PLATFORM_MODE_DEEPSLEEP || mode == PLATFORM_MODE_DEEP_WITH_RETENTION){
				deep_sleep_flag_set(ss_outgoingFrameCntGet());
			}
			platform_lowpower_enter(mode, wakeUpSrc, interval);
		}

		restore_irq(r);
	}
}

void pm_suspendEnter(int wakeUpSrc, u32 ms){
	pm_lowPowerEnter(PLATFORM_MODE_SUSPEND, wakeUpSrc, ms);
}

void pm_deepSleepEnter(int wakeUpSrc, u32 ms){
#if defined(MCU_CORE_826x)
	pm_lowPowerEnter(PLATFORM_MODE_DEEPSLEEP, wakeUpSrc, ms);
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278) || defined(MCU_CORE_B91)
	pm_lowPowerEnter(PLATFORM_MODE_DEEP_WITH_RETENTION, wakeUpSrc, ms);
#endif
}

u8 pm_interleaveSleepEnter(int wakeUpSrc, u32 ms){
	if(!tl_stackBusy() && zb_isTaskDone()){
		u32 r = disable_irq();

		u32 interval = ev_nearestInterval();

		/* Go to long sleep mode if there is no timer event exist.
		 * Otherwise enter suspend or deep with retention mode according to the platform.
		 * 826x --> suspend mode.
		 * 8258 --> deep with retention mode.
		 */
		if(interval == 0xFFFFFFFF){
			/* Set sleep flag */
			deep_sleep_flag_set(ss_outgoingFrameCntGet());

			platform_longLowpower_enter(PLATFORM_MODE_DEEPSLEEP, wakeUpSrc, ms);
		}else{
#if defined(MCU_CORE_826x)
			platform_mode_e mode = PLATFORM_MODE_SUSPEND;
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278) || defined(MCU_CORE_B91)
			platform_mode_e mode = PLATFORM_MODE_DEEP_WITH_RETENTION;

			/* Set sleep flag */
			deep_sleep_flag_set(ss_outgoingFrameCntGet());
#endif

			interval = interval / (CLOCK_SYS_CLOCK_1US * 1000);

			platform_lowpower_enter(mode, wakeUpSrc, interval);
		}

		restore_irq(r);
	}else{
		return FAILURE;
	}

	return SUCCESS;
}
#endif
