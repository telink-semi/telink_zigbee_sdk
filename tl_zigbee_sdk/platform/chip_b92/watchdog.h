/********************************************************************************************************
 * @file    watchdog.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/**	@page WATCHDOG
 *
 *	Introduction
 *	===============
 *	 -----------------------------------------------------------------------------------------------------------------------------------------------
 *	|  watchdog mode | timer source |            Usage scenarios                         |                          note                            |
 *  |-----------------------------------------------------------------------------------------------------------------------------------------------
 *  |                |              |                                                    |the timer watchdog does not work while sleep              |
 *  | timer watchdog | system clock | only reset exceptions that occur during active     |(because its clock source is no longer available)         |
 *  |                |              |                                                    |                                                          |
 *  |-----------------------------------------------------------------------------------------------------------------------------------------------
 *  |                |              |                                                    |1.If want to use 32K watchdog to handle sleep exceptions, |
 *  |                |              |                                                    |need to pay attention:if there is no timer as the wake-up |
 *  |                |              |                                                    |source in the sleep state,32K watchdog cannot be enabled. |
 *  |                |              |                                                    |                                                          |
 *  |                |              | 1. reset exceptions that occur during active       |                                                          |                                                         |
 *  | 32k watchdog   | 32k timer    | 2. reset exceptions that occur during sleep wakeup |2.Because the 32K clock source may also be used by other  |
 *  |                |              |                                                    |modules,the 32K watchdog has no action to clear watchdog, |
 *  |                |              |                                                    |and can only feed the dog by resetting the capture value. |
 *  |                |              |                                                    |The correct operation process is:                         |
 *  |                |              |                                                    |wd_32k_stop->wd_32k_set_interval_ms->wd_32k_start;        |
 *  |                |              |                                                    |(If set the capture value without stopping,there will be  |
 *  |                |              |                                                    |some intermediate values that can cause an abnormal reset)|
 *   ------------------------------------------------------------------------------------------------------------------------------------------------
 *	API Reference
 *	===============
 *	Header File: watchdog.h
 */
#ifndef WATCHDOG_H_
#define WATCHDOG_H_
#include "analog.h"
#include "clock.h"
#include "compiler.h"

#define wd_clear_cnt		wd_clear

/**
 * @brief     start watchdog.
 * @return    none
 */
static inline void wd_start(void){

	BM_SET(reg_tmr_ctrl2, FLD_TMR_WD_EN);
}


/**
 * @brief     stop watchdog.
 * @return    none
 */
static inline void wd_stop(void){
	BM_CLR(reg_tmr_ctrl2, FLD_TMR_WD_EN);
}

/**
 * @brief     get watchdog overflow status.
 * @return    watchdog overflow status.
 * @note      -# After the timer watchdog reboot returns, the status is set to 1,before the mcu enters the next state, wd_clear_status() must be used to clear the status,
 *               otherwise, the next status judgment of the mcu will be affected;
 *            -# When the status is set to 1, if it is not cleared by calling wd_clear_status():
 *                - software reboot(sys_reboot()) come back,the interface status remains;
 *                - deep/deepretation/power cyele/reset pin/32k watchdog/vbus detect come back, the status of the interface is lost;
 */
static inline unsigned char wd_get_status(void)
{
	return (reg_tmr_sta & FLD_TMR_STA_WD);
}

/**
 * @brief     Clear the reset state caused by the watchdog overflow.
 * @return    none.
 * @note      the interface sys_init() must be called before this interface can be invoked.
 */
static inline void wd_clear_status(void)
{
	reg_tmr_sta = FLD_TMR_STA_WD;
}

/**
 * @brief     Feed the dog. Clear watchdog timer tick count.
 * @return    none.
 */
static inline void wd_clear(void)
{
	reg_tmr_sta = FLD_TMR_WD_CNT_CLR;
}

/**
 * @brief     This function set the watchdog trigger time.
 * 			  Because the lower 8bit of the wd timer register will always be 0, there will be an error ,
			  The time error = (0x00~0xff)/(APB clock frequency)
 * @param[in] period_ms - The watchdog trigger time. Unit is  millisecond
 * @return    none
 * @attention The clock source of watchdog comes from pclk, when pclk changes it needs to be reconfigured.
 */
static inline void wd_set_interval_ms(unsigned int period_ms)
{
	static unsigned int tmp_period_ms = 0;
	tmp_period_ms=period_ms*sys_clk.pclk*1000;
	reg_wt_target=tmp_period_ms;
}

/**
 * @brief     start 32k watchdog.
 * @return    none.
 */
_attribute_ram_code_sec_noinline_ void wd_32k_start(void);

/**
 * @brief     stop 32k watchdog.
 * @return    none.
 * @note      -# the interface sys_init() must be called before this interface can be invoked.
 *            -# after calling this interface, wd_32k_get_status() will be cleared.
 */
_attribute_ram_code_sec_noinline_ void wd_32k_stop(void);

/**
 * @brief     get 32k watchdog overflow status.
 * @return    watchdog overflow status.
 * @note      -# After the 32k watchdog reboot returns, the status is set to 1,before the mcu enters the next state, wd_32k_clear_status() must be used to clear the status,
 *               otherwise, the next status judgment of the mcu will be affected;
 *            -# When the status is set to 1, if it is not cleared by calling wd_32k_clear_status():
 *                - software reboot(sys_reboot())/deep/deepretation/32k watchdog come back,the interface status remains;
 *                - power cyele/reset pin/vbus detect come back, the status of the interface is lost;
 */
_attribute_ram_code_sec_noinline_ unsigned char wd_32k_get_status(void);

/**
 * @brief     Clear the reset state caused by the 32k watchdog overflow.
 * @return    none.
 * @note      -# the interface sys_init() must be called before this interface can be invoked.
 */
_attribute_ram_code_sec_noinline_ void wd_32k_clear_status(void);

/**
 * @brief     This function set the watchdog trigger time.
 * @param[in] period_ms - The watchdog trigger time. Unit is  millisecond,the lower 8bit of the wd timer register is invalid,so ranges from 8~134,217,720ms.
 * @return    none
 */
_attribute_ram_code_sec_noinline_ void wd_32k_set_interval_ms(unsigned int period_ms);


/**
 * @brief      This function is used to turn off the 8s vbus timer.
 * @attention  When using the vbus (not vbat) power supply, you must turn off the vbus timer,
 *             otherwise the MCU will be reset after 8s.
 * @return     none.
 */
static inline void wd_turn_off_vbus_timer(void)
{
	analog_write_reg8(0x69, 0x40);
}
#endif
