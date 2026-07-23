/********************************************************************************************************
 * @file    pm_sys_internal.h
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
#ifndef TLKSTK_PM_INTERNAL_H_
#define TLKSTK_PM_INTERNAL_H_

#include "tl_common.h"
#include "common/types.h"
#include "controller/ll/ll_stack.h"

#if defined(TLK_ONLY_BLE_HOST)
#ifndef TLK_PM_ENABLE_CONFIG
#define TLK_PM_ENABLE_CONFIG        (TLK_PM_ENABLE && BLMS_PM_ENABLE)
#endif

#ifndef PM_DEEPSLEEP_RET_EN
#define PM_DEEPSLEEP_RET_EN         (0 && BLMS_PM_ENABLE && BLMS_PM_ENABLE) //TODO:
#endif

#ifndef PM_USER_TIMER_WAKEUP_EN
#define PM_USER_TIMER_WAKEUP_EN      1 //remove latter, not used
#endif

#ifndef PM_EVENT_FUNC_EN
#define PM_EVENT_FUNC_EN              1 //must open, can use PM event callback
#endif

/*
 * software 32k RC calculation algorithm
 */
#ifndef PM_32KRC_SW_CAL_EN
#define PM_32KRC_SW_CAL_EN      0 //not used now, TODO: use APIS: pm_ble_32k_xxx in ext_driver/ext_pm.c
#endif


#ifndef MCU_DUAL_CORE_ENABLE
#define MCU_DUAL_CORE_ENABLE   1
#endif

#if defined(TLK_ONLY_BLE_HOST)
#define TLK_PM_HOST_EN  1
#define TLK_PM_CONTROLLER_EN 0
#else
#define TLK_PM_HOST_EN  0
#define TLK_PM_CONTROLLER_EN 1
#endif

#ifndef TLK_PM_HOST_EN
#define TLK_PM_HOST_EN  1
#endif

#ifndef TLK_PM_CONTROLLER_EN
#define TLK_PM_CONTROLLER_EN   0
#endif

#ifndef TLK_PM_CHECK_SLEEP_READY_EN
#define TLK_PM_CHECK_SLEEP_READY__EN   1
#endif

#ifndef TLK_PM_CHECK_SLEEP_READY_NUM
#define TLK_PM_CHECK_SLEEP_READY_NUM  16
#endif
/*
 * If other IRQs like STIMER or RF not occur as a mistake, we use a HW timer to keep WFI can wake-up.
 * If the above situation definitely does not exist, no additional hardware timer is needed for WFI
 * wake-up, any IRQ can wake up MCU from WFI.
 */
#ifndef PM_MCU_STALL_USE_TIMER_WAKEUP_EN
#define PM_MCU_STALL_USE_TIMER_WAKEUP_EN   0
#endif


/*
 * Old code compatible for BLE ACL and Periodic ADV
 */
#ifndef PM_MIN_TOLERANCE_US
#define PM_MIN_TOLERANCE_US   40    //for every 80mS, 500ppm
#endif

/*
 * BLE ACL Peripheral connection latency used for low power mode
 */
#ifndef ACL_SLAVE_PM_LATENCY_EN
#define ACL_SLAVE_PM_LATENCY_EN (1 && BLMS_PM_ENABLE)
#endif

_attribute_aligned_(4) typedef struct
{
    u8 pm_inited        : 1;
    u8 sleep_allowed    : 1;
    u8 deepRt_en        : 1;
    u8 wfi_mode_used    : 1;
    u8 appWakeup_en     : 1;
    u8 appWakeup_flg    : 1;
    u8 pm_entered       : 1;

    u8 gpio_early_wkp   : 1;
    u8 sch_is_running   : 1;
    u8 resverd0         : 6;

    u8 wakeup_src;
    u8 slave_no_sleep;

    u16 sleep_enable;
    u8 wkpTsk_oft; //not used now
    u8 sys_ppm_index;

    u16 user_latency;
    u16 Wakeup_early_us; //default:30us

    u32 deepRet_type;
    u32 deepRet_thresTick;
    u32 deepRet_earlyWakeupTick;
    u32 next_task_tick;
    u32 current_wakeup_tick; //The system wake-up tick of the actual transfer of the cpu_sleep_wakeup function.

    u32 latency_wakeup_tick; //if BLE ACL latency wakeup used, select the nearest wakeup tick

    u32 appWakeup_tick;
    u32 wfi_wakeup_tick;

    u32 wakeup_lock_tick; //Only used for dual core suspend (like panther IC)

    sch_task_t wkpTsk_fifo; //latency wake_up task fifo

} st_llms_pm_t;

extern st_llms_pm_t blmsPm;


typedef int (*ll_pm_callback_t)(u32 sleep_mode,u32 tick); //sleep_mode refer to type: pm_sleep_mode_e
extern ll_pm_callback_t tlkstk_pm_sleep_cb;



typedef u32 (*ll_pm_checkSleepReadyCallback_t)(void);
u32 tlk_pm_registCheckSleepReadyCallback(ll_pm_checkSleepReadyCallback_t cb);

typedef void (*ll_pm_storeHwSettingCallback_t)(void);
u32 tlk_pm_registStoreHwSettingCallback(ll_pm_storeHwSettingCallback_t cb);

typedef void (*ll_pm_restoreHwSettingCallback_t)(void);
u32 tlk_pm_registRestoreHwSettingCallback(ll_pm_restoreHwSettingCallback_t cb);

/* pm 32k rc cal alg */
void         tlk_pm_32k_rc_cal_reset(void);
void         tlk_pm_32k_rc_cal_offset (int offset_tick, int rc32_cnt);
unsigned int tlk_pm_32k_rc_get_calib (void);
unsigned int tlk_pm_32k_rc_get_latest_offset_cal_time(void);
void         tlk_pm_32k_rc_update_sleep_tick (unsigned int tick_32k, unsigned int tick);

/*  */

void blc_pm_setWfiWakeupTick(u32 tick);

/* sleep api, internal used */
u32 tlk_pm_handler(pm_sleep_mode_e sleep_M, u32 nxt_task_wakeup_tick);
#endif

#endif /* TLKSTK_PM_INTERNAL_H_ */
