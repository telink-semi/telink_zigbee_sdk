/********************************************************************************************************
 * @file     ext_pm.h
 *
 * @brief   This is the header file for B91
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#ifndef DRIVERS_B91_DRIVER_EXT_EXT_PM_H_
#define DRIVERS_B91_DRIVER_EXT_EXT_PM_H_

#include "lib/include/pm.h"
#include "types.h"

#ifndef	PM_32k_RC_CALIBRATION_ALGORITHM_EN
#define PM_32k_RC_CALIBRATION_ALGORITHM_EN	1
#endif


typedef int (*suspend_handler_t)(void);
typedef void (*check_32k_clk_handler_t)(void);
typedef unsigned int (*pm_get_32k_clk_handler_t)(void);
typedef unsigned int (*pm_tim_recover_handler_t)(unsigned int);


extern  check_32k_clk_handler_t  	pm_check_32k_clk_stable;


/**
 * @brief   deepsleep wakeup by external xtal
 */
typedef struct{
	unsigned char ext_cap_en;    //24xtal  cap
	unsigned char pad32k_en;
	unsigned char pm_enter_en;
	unsigned char rsvd;
}misc_para_t;

extern  _attribute_aligned_(4) misc_para_t 				blt_miscParam;

//#define SYS_NEED_REINIT_EXT32K			    BIT(1)
#define WAKEUP_STATUS_TIMER_CORE     	    ( WAKEUP_STATUS_TIMER | WAKEUP_STATUS_CORE)
#define WAKEUP_STATUS_TIMER_PAD		        ( WAKEUP_STATUS_TIMER | WAKEUP_STATUS_PAD)


/**
 * @brief analog register below can store Information when MCU in deepsleep mode
 * 	      store your information in these ana_regs before deepsleep by calling analog_write function
 * 	      when MCU wakeup from deepsleep, read the information by by calling analog_read function
 * 	      Reset these analog registers only by power cycle
 */
#define DEEP_ANA_REG0                       0x39 //initial value =0x00
#define DEEP_ANA_REG1                       0x3a //initial value =0x00
#define DEEP_ANA_REG2                       0x3b //initial value =0x00
#define DEEP_ANA_REG3                      	0x3c //initial value =0x00
#define DEEP_ANA_REG4                       0x3d //initial value =0x00
#define DEEP_ANA_REG5                       0x3e //initial value =0x00
#define DEEP_ANA_REG6                       0x3f //initial value =0x0f

/**
 * @brief these analog register can store data in deepsleep mode or deepsleep with SRAM retention mode.
 * 	      Reset these analog registers by watchdog, chip reset, RESET Pin, power cycle
 */

#define DEEP_ANA_REG7                       0x38 //initial value =0xff

//ana3e system used, user can not use
#define SYS_DEEP_ANA_REG 					PM_ANA_REG_POWER_ON_CLR_BUF0

extern void check_32k_clk_stable(void);

/**
 * @brief      This function serves to determine whether wake up source is external 32k RC.
 * @param[in]  none.
 * @return     none.
 */
static inline void blc_pm_select_external_32k_crystal(void)
{
	pm_check_32k_clk_stable = check_32k_clk_stable;
	blt_miscParam.pad32k_en 	= 1; // set '1': 32k clk src use external 32k crystal
}


/**
 * @brief      This function serves to determine whether mcu is waked up by pad.
 * @param[in]  none.
 * @return     1- yes , 0- no.
 */
static inline int pm_is_deepPadWakeup(void)
{
	return g_pm_status_info.is_pad_wakeup;
}
#endif /* DRIVERS_B91_DRIVER_EXT_EXT_PM_H_ */
