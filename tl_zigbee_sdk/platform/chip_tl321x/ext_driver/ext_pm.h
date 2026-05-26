/********************************************************************************************************
 * @file    ext_pm.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef DRIVERS_TL321X_DRIVER_EXT_EXT_PM_H_
#define DRIVERS_TL321X_DRIVER_EXT_EXT_PM_H_

#include "../lib/include/pm/pm.h"
#include "types.h"


#ifndef USE_DRIVER_PM
    #define USE_DRIVER_PM  0
#endif

#ifndef _2P4G_DEMO_
    #define _2P4G_DEMO_  0
#endif

#define DEEPSLEEP_RAM_SIZE_TO_MODE(ram_size)  ram_size==0x8000? DEEPSLEEP_MODE_RET_SRAM_LOW32K: (ram_size==0x10000)? DEEPSLEEP_MODE_RET_SRAM_LOW64K:DEEPSLEEP_MODE_RET_SRAM_LOW96K

typedef int (*cpu_pm_handler_t)(pm_sleep_mode_e sleep_mode,  pm_sleep_wakeup_src_e wakeup_src, unsigned int  wakeup_tick);
extern  cpu_pm_handler_t            cpu_sleep_wakeup;


#define DEFAULT_DEEPSLEEP_MODE_RET_SRAM_SIZE  DEEPSLEEP_MODE_RET_SRAM_LOW96K

/**
 * @brief   deepsleep wakeup by external xtal
 */
typedef struct __attribute__((packed)) {
    unsigned char ext_cap_en;    //24xtal  cap
    unsigned char pad32k_en;
    unsigned char pm_enter_en;
    unsigned char adc_efuse_calib_flag;
}misc_para_t;

extern  _attribute_aligned_(4) misc_para_t              blt_miscParam;

/**
 * @brief      This function registers a callback function to be executed before suspend.
 * @param[in]  suspend_handler_t func.
 * @return     none.
 */
typedef int (*suspend_handler_t)(void);
void bls_pm_registerFuncBeforeSuspend (suspend_handler_t func );

/**
 * @brief      This function serves to set the working mode of MCU based on 32k crystal,e.g. suspend mode, deepsleep mode, deepsleep with SRAM retention mode and shutdown mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select.
 * @param[in]  wakeup_tick - the time of short sleep, which means MCU can sleep for less than 5 minutes.
 * @return     indicate whether the cpu is wake up successful.
 */
int  cpu_sleep_wakeup_32k_rc(pm_sleep_mode_e sleep_mode,  pm_sleep_wakeup_src_e wakeup_src, unsigned int  wakeup_tick);

/**
 * @brief      This function serves to set the working mode of MCU based on 32k crystal,e.g. suspend mode, deepsleep mode, deepsleep with SRAM retention mode and shutdown mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select.
 * @param[in]  wakeup_tick - the time of short sleep, which means MCU can sleep for less than 5 minutes.
 * @return     indicate whether the cpu is wake up successful.
 */
int  cpu_sleep_wakeup_32k_xtal(pm_sleep_mode_e sleep_mode,  pm_sleep_wakeup_src_e wakeup_src, unsigned int  wakeup_tick);

/**
 * @brief   This function serves to reboot chip.
 * @param   none.
 * @return  none.
 */
void start_reboot(void);

/**
 * @brief   This function serves to recover system timer from tick of internal 32k RC.
 * @param   none.
 * @return  none.
 */
unsigned int pm_tim_recover_32k_rc(unsigned int now_tick_32k);

/**
 * @brief   This function serves to recover system timer from tick of external 32k crystal.
 * @param   none.
 * @return  none.
 */
unsigned int pm_tim_recover_32k_xtal(unsigned int now_tick_32k);

/**
 * @brief   This function serves to get the 32k tick.
 * @param   none
 * @return  tick of 32k .
 */
unsigned int clock_get_digital_32k_tick(void);

/**
 * @brief      This function serves to determine whether wake up source is internal 32k RC.
 *             attention: this function must called before "sys_init()" if using 32K RC for power management
 * @param[in]  none.
 * @return     none.
 */
void blc_pm_select_internal_32k_crystal(void);

/**
 * @brief      This function serves to determine whether wake up source is external 32k RC.
 * @param[in]  none.
 * @return     none.
 */
void blc_pm_select_external_32k_crystal(void);

/**
 * @brief      This function servers to wake up the cpu from sleep mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select.
 * @param[in]  wakeup_tick - the time of sleep,unit is 31.25us,1ms = 32.
 * @return     indicate whether the cpu is wake up successful.
 */
int cpu_long_sleep_wakeup_32k_rc(pm_sleep_mode_e sleep_mode,  pm_sleep_wakeup_src_e wakeup_src, unsigned int  wakeup_tick);

/**
 * @brief      This function serves to determine whether mcu is waked up from deep retention.
 * @param[in]  none.
 * @return     1- yes , 0- no.
 */
static _always_inline int pm_is_MCU_deepRetentionWakeup(void)
{
    return (g_pm_status_info.mcu_status & MCU_STATUS_DEEPRET_BACK);
}

/**
 * @brief      This function serves to determine whether mcu is waked up by pad.
 * @param[in]  none.
 * @return     1- yes , 0- no.
 */
static _always_inline int pm_is_deepPadWakeup(void)
{
    return g_pm_status_info.is_pad_wakeup;
}

/**
 * @brief      This function serves to get the status of mcu.
 * @param[in]  none.
 * @return     mcu_status.
 */
static _always_inline int pm_get_mcu_status(void)
{
    return g_pm_status_info.mcu_status;
}

#define cpu_set_gpio_wakeup             pm_set_gpio_wakeup

#endif /* DRIVERS_TL321X_DRIVER_EXT_EXT_PM_H_ */
