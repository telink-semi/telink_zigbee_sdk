/********************************************************************************************************
 * @file    pm_sys.h
 *
 * @brief   This is the header file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef TLKSTK_PM_H_
#define TLKSTK_PM_H_

#include "types.h"

#ifndef TLK_PM_ENABLE
#define TLK_PM_ENABLE              1
#endif

#if defined(TLK_ONLY_BLE_HOST)

#define PM_CHK_HCI_TASK_NUM      TSKNUM_MAX

typedef enum {
    PM_ESLEEP_SUCCESS = 0x00,
    PM_ESLEEP_NOT_ALLOWED,
    PM_ESLEEP_HOST_NOT_ALLOWED,
    PM_ESLEEP_CTRL_NOT_ALLOWED,
    PM_ESLEEP_RF_FSM_BUSY,
    PM_ESLEEP_TO_SHORT,
    PM_ESLEEP_TASKS_NOT_RDY,
    PM_ESLEEP_INVALID_PARAM,
    PM_ESLEEP_MCU_RUN,
} pm_sleep_error_t;


/**
 *  @brief
 */
typedef enum
{
    PM_SLEEP_DISABLE      = 0,
    PM_SLEEP_ENABLE       = 1,
} sleep_enable_t;

/**
 *  @brief
 */
/* DeepSleepRetention_Enable */
typedef enum
{
    PM_DeepRetn_Disable = 0x00,
    PM_DeepRetn_Enable  = 0x01,
} deep_retn_en_t;

typedef enum
{
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
    BLT_EV_FLAG_GPIO_EARLY_WAKEUP,
    BLT_EV_FLAG_SLEEP_ENTER,
    BLT_EV_FLAG_SUSPEND_EXIT,
    BLT_EV_FLAG_KEY_MISSING,

    BLT_EV_MAX_NUM,
} blt_ev_flag_t;


/**
 * @brief   for user to set low power mode mask
 * @param   mask - low power mode mask
 * @return  none
 */
void blc_pm_setSleepMask(sleep_mask_t mask);


/**
 * @brief   application wake up low power mode process callback function
 */
typedef void (*pm_appWakeupLowPower_callback_t)(int);
extern pm_appWakeupLowPower_callback_t pm_appWakeupLowPowerCb;

/**
 * @brief   Telink defined LinkLayer Event Callback
 */
typedef void (*pm_event_callback_t)(u8 e, u8 *p, int n);
extern pm_event_callback_t pm_event_callback;

typedef enum
{
    PM_EV_FLAG_SLEEP_ENTER,
    PM_EV_FLAG_SUSPEND_EXIT,
    PM_EV_FLAG_GPIO_EARLY_WAKEUP,
    PM_EV_FLAG_SLEEP_EXIT,
    PM_EV_MAX_NUM,
} pm_ev_flag_t;

/**
 * @brief   Register PM Event callBack
 * @param[in]   e - event number, must use element of "pm_ev_flag_t"
 * @param[in]   p - callBack function
 * @return  none
 */
void tlksdk_pm_registerPmEventCallback(u8 e, pm_event_callback_t p);
#define blc_ll_registerTelinkControllerEventCallback tlksdk_pm_registerPmEventCallback


/**
 * @brief   for user to initialize low power mode
 * @param   none
 * @return  none
 */
void tlksdk_pm_init(void);
#define blc_ll_initPowerManagement_module tlksdk_pm_init


/**
 * @brief   LinkLayer initialization after deepSleep retention wake_up
 * @param   none
 * @return  none
 */
void tlksdk_pm_recoverDeepRetention(void);
#define  blc_ll_recoverDeepRetention  tlksdk_pm_recoverDeepRetention


/**
 * @brief   for user to enable low power mode
 * @param   enable - TRUE: enable low power mode ; FALSE: disable low power mode
 * @return  none
 */
void tlksdk_pm_enableSleep(bool enable);


/**
 * @brief   for user to enable WFI mode(also called MCU STALL mode)
 * @param   enable - TRUE: enable WFI mode; FALSE: disable WFI mode
 * @return  none
 * @note    B92 do NOT support WFI, call this API will not effect
 */
void tlksdk_pm_enableWfiMode(bool enable);


/**
 * @brief   for user to check if WFI mode(also called MCU STALL mode) is enabled
 * @param   none
 * @return  TRUE: WFI mode enabled; FALSE: WFI mode disabled
 */
bool tlksdk_pm_isEnableWfiMode(void);


/**
 * @brief   for user to enable or disable deepSleep retention function
 * @param   en -  deepSleep retention enable, 1: enable; 0: disable
 * @return  none
 */
void tlksdk_pm_setDeepsleepRetentionEnable(deep_retn_en_t en);
#define blc_pm_setDeepsleepRetentionEnable tlksdk_pm_setDeepsleepRetentionEnable


/**
 * @brief   for user to set low power mode wake up source
 * @param   wakeup_src - low power mode wake_up source
 * @return  none
 */
void tlksdk_pm_setWakeupSource(pm_sleep_wakeup_src_e wakeup_src);
#define blc_pm_setWakeupSource  tlksdk_pm_setWakeupSource

/**
 * @brief   for user to get the nearest low power wake up tick
 * @param   none
 * @return  bltPm.current_wakeup_tick
 */
u32 tlksdk_pm_getWakeupSystemTick(void);


/**
 * @brief   for user to set the sleep latency enter us
 * @param   wakeup_early_us - low power early us (latency enter delay)
 *          minimum value: 30us
 * @return  none
 */
void tlksdk_pm_setWakeupEarlyUs(u16 wakeup_early_us);


/**
 * @brief   for user to get the sleep latency enter us
 * @param   none
 * @return  bltPm.Wakeup_early_us
 */
u32 tlksdk_pm_getWakeupWakeupEarlyUs(void);
#define blc_pm_getWakeupSystemTick  tlksdk_pm_getWakeupWakeupEarlyUs


/**
 * @brief   for user to set latency manually for acl_peripheral role
 * @param   latency
 * @return  none
 */
void bls_pm_setManualLatency(u16 latency);
#define bls_pm_setManualLatency  bls_pm_setManualLatency


/**
 * @brief   for user to set the threshold of sleep tick for entering deep retention mode
 * @param   threshold_ms - the threshold of time for suspend or deepsleep retention
 * @return  none.
 */
void tlksdk_pm_setDeepsleepRetentionThreshold(u32 threshold_ms);
#define blc_pm_setDeepsleepRetentionThreshold    tlksdk_pm_setDeepsleepRetentionThreshold


/**
 * @brief   for user to set early wake up tick for deep retention mode
 * @param   earlyWakeup_us - early wake up tick for deep retention mode
 * @return  none.
 */
void tlksdk_pm_setDeepsleepRetentionEarlyWakeupTiming(u32 earlyWakeup_us);
#define blc_pm_setDeepsleepRetentionEarlyWakeupTiming    tlksdk_pm_setDeepsleepRetentionEarlyWakeupTiming


/**
 * @brief   for user to set the type of deep retention mode
 * @param   sleep_type - the type of deep retention mode
 * @return  none.
 */
void tlksdk_pm_setDeepsleepRetentionType(pm_sleep_mode_e sleep_type);
#define    blc_pm_setDeepsleepRetentionType    tlksdk_pm_setDeepsleepRetentionType


/**
 * @brief   for user to set application wake up low power mode
 * @param   wakeup_tick - low power mode wake up time
 * @param   enable - low power mode application wake up enable
 * @return  none
 */
void tlksdk_pm_setAppWakeupLowPower(u32 wakeup_tick, u8 enable);
//void blc_pm_setAppWakeupLowPower(u32 wakeup_tick, u8 enable);
#define blc_pm_setAppWakeupLowPower(wakeup_tick, enable)    tlksdk_pm_setAppWakeupLowPower(wakeup_tick, enable)
/**
 * @brief   for user to register the callback for application wake up low power mode process
 * @param   cb - the pointer of callback function
 * @return  none.
 */
void tlksdk_pm_registerAppWakeupLowPowerCb(pm_appWakeupLowPower_callback_t cb);
//void blc_pm_registerAppWakeupLowPowerCb(pm_appWakeupLowPower_callback_t cb);
#define blc_pm_registerAppWakeupLowPowerCb(cb)  tlksdk_pm_registerAppWakeupLowPowerCb(cb)

/**
 * @brief   Check the system's readiness and put the CPU into sleep for entering Power Management (PM) mode.
 * @param   sleep_mode : refer to type: pm_sleep_mode_e in driver/pm.h
 * @param   nxt_task_wakeup_tick: Expected wake - up time.
 * @return 16 - bit unsigned int; 0 indicates successful entry into PM mode, non - zero means an issue prevented entry.
 */
u32 tlksdk_pm_enterSleep(u32 sleep_mode, u32 nxt_task_wakeup_tick);

/**
 * @brief  Function to get the wakeup source of mcu.
 * @param  none
 * @return refer to pm_suspend_wakeup_status_e[for TL721X/TL751X] or pm_wakeup_status_e [for B91\B92].
 */
u32 tlksdk_pm_getPMWakeupSRC(void);


/**
 * @brief  Function to enable or disable WFI (Wait For Interrupt) mode.
 * @param  enable - true to enable WFI mode, false to disable WFI mode
 * @return none
 */
void blc_pm_enableWfiMode(bool enable);

/**
 * @brief  Function to initialize delay parameters for WFI mode wakeup.
 *         This function configures timing parameters required for delayed wakeup
 *         when operating in WFI (Wait For Interrupt) mode.
 * @param  none
 * @return none
 */
void blc_pm_InitWfiDelayWakeup(void);


#endif //#if TLK_PM_ENABLE

#endif /* TLKSTK_PM_H_ */
