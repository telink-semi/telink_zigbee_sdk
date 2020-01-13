/********************************************************************************************************
 * @file     drv_pm.c
 *
 * @brief    API for platform low power
 *
 * @author
 * @date     June. 20, 2018
 *
 * @par      Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
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
#include "drv_pm.h"
#include "platform_includes.h"
#include "zb_common.h"


void platform_wakeup_init(void){
	cpu_wakeup_init();
}

void platform_wakeup_pad_cfg(u32 pin, platform_wakeup_level_e pol, int en){
#if defined (MCU_CORE_826x) || defined (MCU_CORE_HAWK)
	PM_PadSet(pin, pol, en);
#else
	cpu_set_gpio_wakeup(pin, pol, en);
#endif
}

void platform_lowpower_enter(platform_mode_e mode, platform_wakeup_e src, u32 cycle_ms){
#if defined (MCU_CORE_826x) || defined (MCU_CORE_HAWK)
	u8 sleep_mode = 0;
	if(mode == PLATFORM_MODE_DEEPSLEEP){
		sleep_mode = 1;
	}

	WakeupSrc_TypeDef srcType = 0;
	if(src & PLATFORM_WAKEUP_PAD){
		srcType |= WAKEUP_SRC_PAD;
	}
	if(src & PLATFORM_WAKEUP_TIMER){
		srcType |= WAKEUP_SRC_TIMER;
	}
	PM_LowPwrEnter(sleep_mode, srcType, clock_time() + cycle_ms*1000*CLOCK_SYS_CLOCK_1US);
#else		//8258/8278
	SleepMode_TypeDef sleep_mode = SUSPEND_MODE;
	if(mode == PLATFORM_MODE_SUSPEND){
		sleep_mode = SUSPEND_MODE;
	}else if(mode == PLATFORM_MODE_DEEP_WITH_RETENTION){
		sleep_mode = DEEPSLEEP_MODE_RET_SRAM_LOW32K;
	}

	SleepWakeupSrc_TypeDef srcType = 0;
	if(src & PLATFORM_WAKEUP_PAD){
		srcType |= PM_WAKEUP_PAD;
	}
	if(src & PLATFORM_WAKEUP_TIMER){
		srcType |= PM_WAKEUP_TIMER;
	}

#if defined(MCU_CORE_8278)
	blc_pm_select_internal_32k_crystal();
#endif
	cpu_sleep_wakeup(sleep_mode, srcType, clock_time() + cycle_ms*1000*CLOCK_SYS_CLOCK_1US);

	/* reconfigure some module used */
	ZB_RADIO_INIT();

	u8 value;
	u8 len;
	tl_zbMacAttrGet(MAC_PHY_ATTR_CURRENT_CHANNEL, &value, &len);
	ZB_TRANSCEIVER_SET_CHANNEL(value);
#endif
}

void platform_longLowpower_enter(platform_mode_e mode, platform_wakeup_e src, u32 durationMs)
{
#if defined (MCU_CORE_826x) || defined (MCU_CORE_HAWK)
	u8 sleep_mode = 0;
	if(mode == PLATFORM_MODE_DEEPSLEEP){
		sleep_mode = 1;
	}

	WakeupSrc_TypeDef srcType = 0;
	if(src & PLATFORM_WAKEUP_PAD){
		srcType |= WAKEUP_SRC_PAD;
	}
	if(src & PLATFORM_WAKEUP_TIMER){
		srcType |= WAKEUP_SRC_TIMER;
	}
	PM_LowPwrEnter2(sleep_mode, srcType, durationMs * 1000);
#else
	SleepMode_TypeDef sleep_mode = SUSPEND_MODE;
	if(mode == PLATFORM_MODE_SUSPEND){
		sleep_mode = SUSPEND_MODE;
	}else if(mode == PLATFORM_MODE_DEEPSLEEP){
		sleep_mode = DEEPSLEEP_MODE;
	}

	SleepWakeupSrc_TypeDef srcType = 0;
	if(src & PLATFORM_WAKEUP_PAD){
		srcType |= PM_WAKEUP_PAD;
	}
	if(src & PLATFORM_WAKEUP_TIMER){
		srcType |= PM_WAKEUP_TIMER;
	}

	pm_long_sleep_wakeup(sleep_mode, srcType, durationMs * 1000);

	/* reconfigure some module used */
	ZB_RADIO_INIT();

	u8 value;
	u8 len;
	tl_zbMacAttrGet(MAC_PHY_ATTR_CURRENT_CHANNEL, &value, &len);
	ZB_TRANSCEIVER_SET_CHANNEL(value);
#endif
}


void deep_sleep_flag_set(unsigned int a){
	analog_write(REG_DEEP_FLAG, DATA_STORE_FLAG);

	analog_write(REG_FRAMECOUNT,   (a));
	analog_write(REG_FRAMECOUNT+1, (a)>>8);
	analog_write(REG_FRAMECOUNT+2, (a)>>16);
	analog_write(REG_FRAMECOUNT+3, (a)>>24);
}

u8 deep_sleep_flag_get(void){
	return ((analog_read(REG_DEEP_FLAG) == DATA_STORE_FLAG) && (analog_read(REG_DEEP_BACK_FLAG) == DATA_STORE_FLAG));
}

u32 deep_sleep_framecount_get(void){
	return ((analog_read(REG_FRAMECOUNT+3) << 24) | (analog_read(REG_FRAMECOUNT+2) << 16) | (analog_read(REG_FRAMECOUNT+1) << 8) | analog_read(REG_FRAMECOUNT) );
}

