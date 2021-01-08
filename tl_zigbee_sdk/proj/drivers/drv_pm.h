/********************************************************************************************************
 * @file	drv_pm.h
 *
 * @brief	This is the header file for drv_pm
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
#pragma once




typedef enum{
	PM_SLEEP_MODE_SUSPEND,
	PM_SLEEP_MODE_DEEPSLEEP,
	PM_SLEEP_MODE_DEEP_WITH_RETENTION,	//826x not support

	PM_SLEEP_MODE_MCU_STALL,
}drv_pm_sleep_mode_e;

typedef enum{
	PM_WAKEUP_SRC_PAD 	= BIT(0),
	PM_WAKEUP_SRC_TIMER = BIT(1),
}drv_pm_wakeup_src_e;

typedef enum{
	PM_WAKEUP_LEVEL_LOW,
	PM_WAKEUP_LEVEL_HIGH,
}drv_pm_wakeup_level_e;

typedef struct{
	u32 wakeupPin;
	drv_pm_wakeup_level_e wakeupLevel;
}drv_pm_pinCfg_t;

#if defined(MCU_CORE_826x)
	#define PM_CLOCK_INIT()
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	#define PM_CLOCK_INIT()					do{ \
												/* Initialize 32K for timer wakeup. */	\
												clock_32k_init(CLK_32K_RC);				\
												rc_32k_cal();							\
												pm_select_internal_32k_rc();			\
											}while(0)
#elif defined(MCU_CORE_B91)
	/* 24M RC is inaccurate, and it is greatly affected by temperature, so real-time calibration is required
	 * The 24M RC needs to be calibrated before the pm_sleep_wakeup function,
	 * because this clock will be used to kick 24m xtal start after wake up.
	 * The more accurate this time, the faster the crystal will start. Calibration cycle depends on usage
	 */
	#define PM_CLOCK_INIT()					do{ \
												clock_cal_24m_rc();						\
												/* Initialize 32K for timer wakeup. */	\
												clock_32k_init(CLK_32K_RC);				\
												clock_cal_32k_rc();/*6.68ms*/			\
											}while(0)
#endif


void drv_pm_deepSleep_frameCnt_set(u32 frameCounter);
u32 drv_pm_deepSleep_frameCnt_get(void);
bool drv_pm_deepSleep_flag_get(void);

void drv_pm_wakeupPinConfig(drv_pm_pinCfg_t *pinCfg, u32 pinNum);
u8 drv_pm_wakeupPinValid(drv_pm_pinCfg_t *pinCfg, u32 pinNum);

void drv_pm_sleep(drv_pm_sleep_mode_e mode, drv_pm_wakeup_src_e src, u32 durationMs);
void drv_pm_longSleep(drv_pm_sleep_mode_e mode, drv_pm_wakeup_src_e src, u32 durationMs);

u8 drv_pm_suspendEnter(drv_pm_wakeup_src_e wakeUpSrc, u32 ms);
u8 drv_pm_deepSleepEnter(drv_pm_wakeup_src_e wakeUpSrc, u32 ms);
u8 drv_pm_interleaveSleepEnter(drv_pm_wakeup_src_e wakeUpSrc, u32 ms);
