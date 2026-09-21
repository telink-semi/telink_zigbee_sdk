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
#ifndef DRIVERS_TL721X_DRIVER_EXT_EXT_PM_H_
#define DRIVERS_TL721X_DRIVER_EXT_EXT_PM_H_

#include "../lib/include/pm/pm.h"
#include "types.h"


/**
 * @brief       This function serves to set the working mode of MCU based on 32k crystal,e.g. suspend mode, deep sleep mode, deep sleep with SRAM retention mode and shutdown mode.
 * @param[in]   sleep_mode          - sleep mode type select.
 * @param[in]   wakeup_src          - wake up source select.
 * @param[in]   wakeup_tick         - The tick value at the time of wake-up.
                                      If the wakeup_tick_type is PM_TICK_STIMER, then wakeup_tick is converted to 24M. The range of tick that can be set is approximately:
                                      current tick value + (18352~0xe0000000), and the corresponding sleep time is approximately: 2ms~234.88s.It cannot go to sleep normally when it exceeds this range.
                                      If the wakeup_tick_type is PM_TICK_32K, then wakeup_tick is converted to 32K. The range of tick that can be set is approximately:
                                      64~0xffffffff, and the corresponding sleep time is approximately: 2ms~37hours.It cannot go to sleep normally when it exceeds this range.
 * @note        There are two things to note when using LPC wake up:
 *              1.After the LPC is configured, you need to wait 100 seconds before you can go to sleep. After the LPC is opened, 1-2 32k tick is needed to calculate the result.
 *                Before this, the data in the result register is random. If you enter the sleep function at this time,
 *                you may not be able to sleep normally because the data in the result register is abnormal.
 *              2.When entering sleep, keep the input voltage and reference voltage difference must be greater than 30mV, otherwise can not enter sleep normally, crash occurs.
 * @return      indicate whether the cpu is wake up successful.
 */
typedef int (*cpu_pm_handler_t)(pm_sleep_mode_e sleep_mode,  pm_sleep_wakeup_src_e wakeup_src, unsigned int  wakeup_tick);
extern  cpu_pm_handler_t            cpu_sleep_wakeup;


/**
 * @brief   deepsleep wakeup by external xtal
 */
typedef struct __attribute__((packed)) {
    unsigned char pad32k_en;
    unsigned char pm_enter_en;
    unsigned char adc_efuse_calib_flag;
    unsigned char rfu;    //Not applicable
}misc_para_t;
extern  _attribute_aligned_(4) misc_para_t              blt_miscParam;



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
 * @brief      This function serves to determine whether mcu is waked up from deep retention.
 * @param[in]  none.
 * @return     1- yes , 0- no.
 */
static inline int pm_is_MCU_deepRetentionWakeup(void)
{
    return (g_pm_status_info.mcu_status & MCU_STATUS_DEEPRET_BACK);
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


/**
 * @brief      This function serves to get the status of mcu.
 * @param[in]  none.
 * @return     mcu_status.
 */
static inline unsigned char pm_get_mcu_status(void)
{
    return g_pm_status_info.mcu_status;
}


#define cpu_set_gpio_wakeup             pm_set_gpio_wakeup

/**
 * @brief      This function serves to set the working mode of MCU based on 32k crystal,e.g. suspend mode, deepsleep mode, deepsleep with SRAM retention mode and shutdown mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select.
 * @param[in]  wakeup_tick - the time of short sleep, which means MCU can sleep for less than 5 minutes.
 * @return     indicate whether the cpu is wake up successful.
 */
int  cpu_sleep_wakeup_32k_rc(pm_sleep_mode_e sleep_mode,  pm_sleep_wakeup_src_e wakeup_src, unsigned int  wakeup_tick);

#endif /* DRIVERS_TL721X_DRIVER_EXT_EXT_PM_H_ */
