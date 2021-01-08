/********************************************************************************************************
 * @file	drv_pm.c
 *
 * @brief	This is the source file for drv_pm
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
#include "zb_common.h"


#define PM_STIMER_SLEEP_TIME_MAX			120//120s

#define PM_FC_SET_FLAG						0x5A
#define PM_FC_CLR_FLAG						0x00


#if defined(MCU_CORE_826x)
	#define PM_ANA_REG_FRAME_COUNTER_FLAG	DEEP_ANA_REG0
	#define PM_ANA_REG_FRAME_COUNTER		DEEP_ANA_REG4
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	#define PM_ANA_REG_FRAME_COUNTER_FLAG	DEEP_ANA_REG0
	#define PM_ANA_REG_FRAME_COUNTER		DEEP_ANA_REG6
#elif defined(MCU_CORE_B91)
	#define PM_ANA_REG_FRAME_COUNTER_FLAG	PM_ANA_REG_POWER_ON_CLR_BUF1
	#define PM_ANA_REG_FRAME_COUNTER		PM_ANA_REG_POWER_ON_CLR_BUF2
#endif


void drv_pm_deepSleep_frameCnt_set(u32 frameCounter)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	analog_write(PM_ANA_REG_FRAME_COUNTER_FLAG, PM_FC_SET_FLAG);

	analog_write(PM_ANA_REG_FRAME_COUNTER,     frameCounter);
	analog_write(PM_ANA_REG_FRAME_COUNTER + 1, frameCounter >> 8);
	analog_write(PM_ANA_REG_FRAME_COUNTER + 2, frameCounter >> 16);
	analog_write(PM_ANA_REG_FRAME_COUNTER + 3, frameCounter >> 24);
#elif defined(MCU_CORE_B91)
	analog_write_reg8(PM_ANA_REG_FRAME_COUNTER_FLAG, PM_FC_SET_FLAG);

	analog_write_reg8(PM_ANA_REG_FRAME_COUNTER,     frameCounter);
	analog_write_reg8(PM_ANA_REG_FRAME_COUNTER + 1, frameCounter >> 8);
	analog_write_reg8(PM_ANA_REG_FRAME_COUNTER + 2, frameCounter >> 16);
	analog_write_reg8(PM_ANA_REG_FRAME_COUNTER + 3, frameCounter >> 24);
#endif
}

u32 drv_pm_deepSleep_frameCnt_get(void)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	analog_write(PM_ANA_REG_FRAME_COUNTER_FLAG, PM_FC_CLR_FLAG);
	return ((analog_read(PM_ANA_REG_FRAME_COUNTER+3) << 24) | (analog_read(PM_ANA_REG_FRAME_COUNTER+2) << 16) | (analog_read(PM_ANA_REG_FRAME_COUNTER+1) << 8) | analog_read(PM_ANA_REG_FRAME_COUNTER) );
#elif defined(MCU_CORE_B91)
	analog_write_reg8(PM_ANA_REG_FRAME_COUNTER_FLAG, PM_FC_CLR_FLAG);
	return ((analog_read_reg8(PM_ANA_REG_FRAME_COUNTER+3) << 24) | (analog_read_reg8(PM_ANA_REG_FRAME_COUNTER+2) << 16) | (analog_read_reg8(PM_ANA_REG_FRAME_COUNTER+1) << 8) | analog_read_reg8(PM_ANA_REG_FRAME_COUNTER) );
#endif
}

bool drv_pm_deepSleep_flag_get(void)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	return ((pm_get_mcu_status() == MCU_STATUS_DEEP_BACK) && (analog_read(PM_ANA_REG_FRAME_COUNTER_FLAG) == PM_FC_SET_FLAG));
#elif defined(MCU_CORE_B91)
	return ((g_pm_status_info.mcu_status == MCU_STATUS_DEEP_BACK) && (analog_read_reg8(PM_ANA_REG_FRAME_COUNTER_FLAG) == PM_FC_SET_FLAG));
#endif
}

void drv_pm_sleep(drv_pm_sleep_mode_e mode, drv_pm_wakeup_src_e src, u32 durationMs)
{
#if defined(MCU_CORE_826x)
	u8 sleep_mode = 0;
	WakeupSrc_TypeDef srcType = 0;

	if(mode == PM_SLEEP_MODE_DEEPSLEEP){
		sleep_mode = 1;
	}

	if(src & PM_WAKEUP_SRC_PAD){
		srcType |= WAKEUP_SRC_PAD;
	}
	if(src & PM_WAKEUP_SRC_TIMER){
		srcType |= WAKEUP_SRC_TIMER;
	}

	PM_LowPwrEnter(sleep_mode, srcType, clock_time() + durationMs*1000*CLOCK_SYS_CLOCK_1US);
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	SleepMode_TypeDef sleep_mode = SUSPEND_MODE;
	SleepWakeupSrc_TypeDef srcType = 0;

	if(mode == PM_SLEEP_MODE_SUSPEND){
		sleep_mode = SUSPEND_MODE;
	}else if(mode == PM_SLEEP_MODE_DEEP_WITH_RETENTION){
		sleep_mode = DEEPSLEEP_MODE_RET_SRAM_LOW32K;
	}

	if(src & PM_WAKEUP_SRC_PAD){
		srcType |= PM_WAKEUP_PAD;
	}
	if(src & PM_WAKEUP_SRC_TIMER){
		srcType |= PM_WAKEUP_TIMER;
	}

	cpu_sleep_wakeup(sleep_mode, srcType, clock_time() + durationMs*1000*CLOCK_SYS_CLOCK_1US);

	/* reconfigure some module used */
	ZB_RADIO_INIT();

	u8 value;
	u8 len;
	tl_zbMacAttrGet(MAC_PHY_ATTR_CURRENT_CHANNEL, &value, &len);
	ZB_TRANSCEIVER_SET_CHANNEL(value);
#elif defined(MCU_CORE_B91)
	pm_sleep_mode_e sleep_mode = SUSPEND_MODE;
	pm_sleep_wakeup_src_e srcType = 0;

	if(mode == PM_SLEEP_MODE_SUSPEND){
		sleep_mode = SUSPEND_MODE;
	}else if(mode == PM_SLEEP_MODE_DEEP_WITH_RETENTION){
		sleep_mode = DEEPSLEEP_MODE_RET_SRAM_LOW64K;
	}

	if(src & PM_WAKEUP_SRC_PAD){
		srcType |= PM_WAKEUP_PAD;
	}
	if(src & PM_WAKEUP_SRC_TIMER){
		srcType |= PM_WAKEUP_TIMER;
	}

	pm_sleep_wakeup(sleep_mode, srcType, PM_TICK_STIMER_16M, clock_time() + durationMs*1000*CLOCK_SYS_CLOCK_1US);

	/* reconfigure some module used */
	ZB_RADIO_INIT();

	u8 value;
	u8 len;
	tl_zbMacAttrGet(MAC_PHY_ATTR_CURRENT_CHANNEL, &value, &len);
	ZB_TRANSCEIVER_SET_CHANNEL(value);
#endif
}

void drv_pm_longSleep(drv_pm_sleep_mode_e mode, drv_pm_wakeup_src_e src, u32 durationMs)
{
#if defined(MCU_CORE_826x)
	u8 sleep_mode = 0;
	WakeupSrc_TypeDef srcType = 0;

	if(mode == PM_SLEEP_MODE_DEEPSLEEP){
		sleep_mode = 1;
	}

	if(src & PM_WAKEUP_SRC_PAD){
		srcType |= WAKEUP_SRC_PAD;
	}
	if(src & PM_WAKEUP_SRC_TIMER){
		srcType |= WAKEUP_SRC_TIMER;
	}

	PM_LowPwrEnter2(sleep_mode, srcType, durationMs * 1000);
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	SleepMode_TypeDef sleep_mode = SUSPEND_MODE;
	SleepWakeupSrc_TypeDef srcType = 0;

	if(mode == PM_SLEEP_MODE_SUSPEND){
		sleep_mode = SUSPEND_MODE;
	}else if(mode == PM_SLEEP_MODE_DEEPSLEEP){
		sleep_mode = DEEPSLEEP_MODE;
	}

	if(src & PM_WAKEUP_SRC_PAD){
		srcType |= PM_WAKEUP_PAD;
	}
	if(src & PM_WAKEUP_SRC_TIMER){
		srcType |= PM_WAKEUP_TIMER;
	}

	pm_long_sleep_wakeup(sleep_mode, srcType, durationMs * 32);

	/* reconfigure some module used */
	ZB_RADIO_INIT();

	u8 value;
	u8 len;
	tl_zbMacAttrGet(MAC_PHY_ATTR_CURRENT_CHANNEL, &value, &len);
	ZB_TRANSCEIVER_SET_CHANNEL(value);
#elif defined(MCU_CORE_B91)
	//not support currently
	while(1);
#endif
}


static void pm_wakeup_pad_cfg(u32 pin, drv_pm_wakeup_level_e pol, int en)
{
#if defined(MCU_CORE_826x)
	PM_PadSet(pin, pol, en);
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	cpu_set_gpio_wakeup(pin, pol, en);
#elif defined(MCU_CORE_B91)
	pm_set_gpio_wakeup(pin, pol, en);
#endif
}

u8 drv_pm_wakeupPinValid(drv_pm_pinCfg_t *pinCfg, u32 pinNum)
{
	drv_pm_pinCfg_t *p = pinCfg;

	for(u32 i = 0; i < pinNum; i++){
		if((p->wakeupLevel == PM_WAKEUP_LEVEL_HIGH) && drv_gpio_read(p->wakeupPin)){
			return SUCCESS;
		}

		if((p->wakeupLevel == PM_WAKEUP_LEVEL_LOW) && !drv_gpio_read(p->wakeupPin)){
			return SUCCESS;
		}
		p++;
	}
	return FAILURE;
}

void drv_pm_wakeupPinConfig(drv_pm_pinCfg_t *pinCfg, u32 pinNum)
{
	drv_pm_pinCfg_t *p = pinCfg;

	for(u32 i = 0; i < pinNum; i++){
		pm_wakeup_pad_cfg(p->wakeupPin, p->wakeupLevel, 1);
		p++;
	}
}

static void pm_lowPowerEnter(drv_pm_sleep_mode_e mode, drv_pm_wakeup_src_e wakeUpSrc, u32 ms)
{
	/* If 'ms' is 0, use default value 120s for sleep. */
	u32 interval = (ms == 0) ? (PM_STIMER_SLEEP_TIME_MAX * 1000) : ms;

	interval = (ev_nearestInterval() <= (interval * 1000 * CLOCK_SYS_CLOCK_1US)) ? ev_nearestInterval() / (1000 * CLOCK_SYS_CLOCK_1US)
																				 : interval;
	if(interval){
		rf_paShutDown();

		if(mode == PM_SLEEP_MODE_DEEPSLEEP){
			drv_pm_deepSleep_frameCnt_set(ss_outgoingFrameCntGet());
		}

		drv_pm_sleep(mode, wakeUpSrc, interval);
	}
}

u8 drv_pm_suspendEnter(drv_pm_wakeup_src_e wakeUpSrc, u32 ms)
{
	if(tl_stackBusy() || !zb_isTaskDone()){
		return FAILURE;
	}

	u32 r = drv_disable_irq();

	pm_lowPowerEnter(PM_SLEEP_MODE_SUSPEND, wakeUpSrc, ms);

	drv_restore_irq(r);

	return SUCCESS;
}

u8 drv_pm_deepSleepEnter(drv_pm_wakeup_src_e wakeUpSrc, u32 ms)
{
	if(tl_stackBusy() || !zb_isTaskDone()){
		return FAILURE;
	}

	u32 r = drv_disable_irq();

#if defined(MCU_CORE_826x)
	pm_lowPowerEnter(PM_SLEEP_MODE_DEEPSLEEP, wakeUpSrc, ms);
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278) || defined(MCU_CORE_B91)
	pm_lowPowerEnter(PM_SLEEP_MODE_DEEP_WITH_RETENTION, wakeUpSrc, ms);
#endif

	drv_restore_irq(r);

	return SUCCESS;
}

u8 drv_pm_interleaveSleepEnter(drv_pm_wakeup_src_e wakeUpSrc, u32 ms)
{
	if(tl_stackBusy() || !zb_isTaskDone()){
		return FAILURE;
	}

	u32 r = drv_disable_irq();

	u32 interval = ev_nearestInterval();

	/* Go to long sleep mode if there is no timer event exist.
	 * Otherwise enter suspend or deep with retention mode according to the platform.
	 * 826x --> suspend mode.
	 * 8258 --> deep with retention mode.
	 */
	if((interval == 0xFFFFFFFF) && (ms >= (PM_STIMER_SLEEP_TIME_MAX * 1000))){
		rf_paShutDown();

		drv_pm_deepSleep_frameCnt_set(ss_outgoingFrameCntGet());

		drv_pm_longSleep(PM_SLEEP_MODE_DEEPSLEEP, wakeUpSrc, ms);
	}else{
#if defined(MCU_CORE_826x)
		drv_pm_sleep_mode_e mode = PM_SLEEP_MODE_SUSPEND;
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278) || defined(MCU_CORE_B91)
		drv_pm_sleep_mode_e mode = PM_SLEEP_MODE_DEEP_WITH_RETENTION;
#endif

		pm_lowPowerEnter(mode, wakeUpSrc, ms);
	}

	drv_restore_irq(r);

	return SUCCESS;
}


