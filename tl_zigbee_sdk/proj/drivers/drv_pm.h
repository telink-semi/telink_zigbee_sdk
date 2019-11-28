/********************************************************************************************************
 * @file     drv_pm.h
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

#pragma once

#include "../common/types.h"
#include "../common/compiler.h"

typedef enum{
	PLATFORM_MODE_SUSPEND,
	PLATFORM_MODE_DEEPSLEEP,
	PLATFORM_MODE_MCU_STALL,
	PLATFORM_MODE_DEEP_WITH_RETENTION,//826x not support
}platform_mode_e;

typedef enum{
	PLATFORM_WAKEUP_PAD = (1 << 0),
	PLATFORM_WAKEUP_TIMER = (1 << 1),
}platform_wakeup_e;

typedef enum{
	PLATFORM_WAKEUP_LEVEL_LOW,
	PLATFORM_WAKEUP_LEVEL_HIGH,
}platform_wakeup_level_e;

void platform_wakeup_init(void);

void platform_wakeup_pad_cfg(u32 pin, platform_wakeup_level_e pol, int en);

void platform_lowpower_enter(platform_mode_e mode, platform_wakeup_e src, u32 cycle_ms);

void platform_longLowpower_enter(platform_mode_e mode, platform_wakeup_e src, u32 durationMs);

void deep_sleep_flag_set(unsigned int a);
u8 deep_sleep_flag_get(void);
u32 deep_sleep_framecount_get(void);
