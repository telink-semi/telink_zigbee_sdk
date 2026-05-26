/********************************************************************************************************
 * @file    ll_pm.h
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
#ifndef LL_PM_H_
#define LL_PM_H_

#include "types.h"

/**
 *  @brief
 */
typedef enum
{
    PM_SLEEP_DISABLE     = 0,
    PM_SLEEP_LEG_ADV     = BIT(0),
    PM_SLEEP_LEG_SCAN    = BIT(1),
    PM_SLEEP_ACL_PERIPHR = BIT(2),
    PM_SLEEP_ACL_CENTRAL = BIT(3),

    PM_SLEEP_EXT_ADV = BIT(4),

    PM_SLEEP_PAWRS_SUB = BIT(6),

    PM_SLEEP_PAWRS_RSP = BIT(7),

    PM_SLEEP_CIS_PERIPHR = BIT(8),
    PM_SLEEP_CIS_CENTRAL = BIT(9),
} sleep_mask_t;

typedef enum
{
    WFI_DISABLE = 0,
    WFI_ENABLE      = 0xFFFF,
} wfi_mask_t;

/**
 *  @brief
 */
/* DeepSleepRetention_Enable */
typedef enum
{
    PM_DeepRetn_Disable = 0x00,
    PM_DeepRetn_Enable  = 0x01,
} deep_retn_en_t;

/**
 * @brief   for user to initialize low power mode
 * @param   none
 * @return  none
 */
void blc_ll_initPowerManagement_module(void);


/**
 * @brief   LinkLayer initialization after deepSleep retention wake_up
 * @param   none
 * @return  none
 */
void blc_ll_recoverDeepRetention(void);


/**
 * @brief   for user to set low power mode mask
 * @param   mask - low power mode mask
 * @return  none
 */
void blc_pm_setSleepMask(sleep_mask_t mask);


/**
 * @brief   Set the WFI (Wait For Interrupt) mask.
 * @param   mask    The WFI mask to be set.
 * @return  0x00: command succeeded; others is not supported.
 */
unsigned char blc_pm_setWfiMask(wfi_mask_t mask);


/**
 * @brief   for user to enable or disable deepSleep retention function
 * @param   en -  deepSleep retention enable, 1: enable; 0: disable
 * @return  none
 */
void blc_pm_setDeepsleepRetentionEnable(deep_retn_en_t en);

/**
 * @brief   for user to set low power mode wake up source
 * @param   wakeup_src - low power mode wake_up source
 * @return  none
 */
void blc_pm_setWakeupSource(pm_sleep_wakeup_src_e wakeup_src);


/**
 * @brief   for user to get low power mode wake up time
 * @param   none
 * @return  bltPm.current_wakeup_tick
 */
u32 blc_pm_getWakeupSystemTick(void);

/**
 * @brief   for user to set latency manually for acl_peripheral role
 * @param   latency
 * @return  none
 */
void bls_pm_setManualLatency(u16 latency);

/**
 * @brief   for user to set the threshold of sleep tick for entering deep retention mode
 * @param   threshold_ms - the threshold of time for suspend or deepsleep retention
 * @return  none.
 */
void blc_pm_setDeepsleepRetentionThreshold(u32 threshold_ms);

/**
 * @brief   for user to set early wake up tick for deep retention mode
 * @param   earlyWakeup_us - early wake up tick for deep retention mode
 * @return  none.
 */
void blc_pm_setDeepsleepRetentionEarlyWakeupTiming(u32 earlyWakeup_us);

/**
 * @brief   for user to set the type of deep retention mode
 * @param   sleep_type - the type of deep retention mode
 * @return  none.
 */
void blc_pm_setDeepsleepRetentionType(pm_sleep_mode_e sleep_type);


/**
 * @brief   application wake up low power mode process callback function
 */
typedef void (*pm_appWakeupLowPower_callback_t)(int);

/**
 * @brief   for user to set application wake up low power mode
 * @param   wakeup_tick - low power mode wake up time
 * @param   enable - low power mode application wake up enable
 * @return  none
 */
void blc_pm_setAppWakeupLowPower(u32 wakeup_tick, u8 enable);

/**
 * @brief   for user to register the callback for application wake up low power mode process
 * @param   cb - the pointer of callback function
 * @return  none.
 */
void blc_pm_registerAppWakeupLowPowerCb(pm_appWakeupLowPower_callback_t cb);

/**
 * @brief   This function enables low power mode in OS usage
 * @param   none
 * @return  none
 */
void blc_ll_enOsPowerManagement_module(void);
#endif /* LL_PM_H_ */
