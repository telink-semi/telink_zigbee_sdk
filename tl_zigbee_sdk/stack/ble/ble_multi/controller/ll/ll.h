/********************************************************************************************************
 * @file    ll.h
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
#ifndef LL_H_
#define LL_H_

#include "tl_common.h"
#include "common/types.h"
#include "ble_common.h"

/**
 * @brief   Telink defined LinkLayer Event Callback
 */
typedef void (*blt_event_callback_t)(u8 e, u8 *p, int n);
typedef void (*user_irq_handler_cb_t)(void);
extern user_irq_handler_cb_t usr_irq_handler_cb;


typedef int (*ll_task_callback_0_t)(void);
extern ll_task_callback_0_t ll_cs_rawData_process_cb;
extern ll_task_callback_0_t ll_cs_hci_subevent_report_cb;

typedef enum
{
    BLT_EV_FLAG_RX_DATA_ABANDON,
    BLT_EV_FLAG_GPIO_EARLY_WAKEUP,
    BLT_EV_FLAG_SLEEP_ENTER,
    BLT_EV_FLAG_SUSPEND_EXIT,
    BLT_EV_FLAG_KEY_MISSING,
    BLT_EV_FLAG_WFI_ENTER,
    BLT_EV_FLAG_WFI_EXIT,
    BLT_EV_FLAG_CHECK_BEFORE_SLEEP_ENTER,// Final check before entering PM sleep
    BLT_EV_MAX_NUM,
} blt_ev_flag_t;

typedef enum
{
    BLE_STATUS_IDLE            = 0x0000, // Idle state
    BLE_STATUS_ADVERTISING     = 0x0001, // Advertising state (non-extended)
    BLE_STATUS_SCANNING        = 0x0002, // Scanning state
    BLE_STATUS_INITIATING      = 0x0004, // initiating state
    BLE_STATUS_CONNECTED       = 0x0008, // Connected state
    BLE_STATUS_EXT_ADVERTISING = 0x0010, // Extended Advertising state
    BLE_STATUS_UNKNOWN         = 0x8000, // Unknown state (to indicate undefined)
} ble_status_t;

/**
 * @brief   Telink defined LinkLayer Event callBack
 * @param[in]   e - event number, must use element of "blt_ev_flag_t"
 * @param[in]   p - callBack function
 * @return  none
 */
void blc_ll_registerTelinkControllerEventCallback(u8 e, blt_event_callback_t p);

/**
 * @brief   irq_handler for BLE stack, process system tick interrupt and RF interrupt
 * @param   none
 * @return  none
 */
void blc_sdk_irq_handler(void);

/**
 * @brief   main_loop for BLE stack, process data and event
 * @param   none
 * @return  none
 */
void blc_sdk_main_loop(void);


/**
 * @brief      for user to initialize MCU
 * @param      none
 * @return     none
 */
void blc_ll_initBasicMCU(void);


/**
 * @brief      for user to initialize link layer Standby state
 * @param      none
 * @return     none
 */
void blc_ll_initStandby_module(u8 *public_adr);


/**
 * @brief      this function is used to read MAC address
 * @param[in]  *addr -  The address where the read value(MAC address) prepare to write.
 * @return     status, 0x00:  succeed
 *                     other: failed
 */
ble_sts_t blc_ll_readBDAddr(u8 *addr);


/**
 * @brief      this function is used to set the LE Public Device Address in the Controller
 * @param[in]  *randomAddr -  Public Device Address
 * @return     status, 0x00:  succeed
 *                     other: failed
 */
ble_sts_t blc_ll_writeBDAddr(u8 *addr);


/**
 * @brief      this function is used to set the LE Random Device Address in the Controller
 * @param[in]  *randomAddr -  Random Device Address
 * @return     status, 0x00:  succeed
 *                     other: failed
 */
ble_sts_t blc_ll_setRandomAddr(u8 *randomAddr);


/**
 * @brief      this function is used check if any controller buffer initialized by application incorrect.
 *             attention: this function must be called at the end of BLE LinkLayer Initialization.
 * @param      none
 * @return     status, 0x00:  succeed, no buffer error
 *                     other: buffer error code
 */
init_err_t blc_contr_checkControllerInitialization(void);

#define blc_controller_check_appBufferInitialization blc_contr_checkControllerInitialization

/**
 * @brief      this function is used by the Host to specify a channel classification based on its local information,
 *             only the ACL Central role is valid.
 * @param[in]  *map - channel map
 * @return     status, 0x00:  succeed
 *                     other: failed
 */
ble_sts_t blc_ll_setHostChannel(u8 *chnMap);


/**
 * @brief      this function is used to reset module of all.
 * @param      none
 * @return     status, 0x00:  succeed, no buffer error
 *                     other: buffer error code
 */
ble_sts_t blc_hci_reset(void);


/**
 * @brief      this function checks whether the Bluetooth stack task is IDLE, not recommended
 * @param      none
 * @return     status, 0:  idle
 *                     1:  task
 */
u32 blc_ll_checkBleTaskIsIdle(void);


/**
 * @brief      this function checks whether the Bluetooth stack task is IDLE
 * @param      none
 * @return     bool, 0:  ble task running
 *                   1:  idle
 */
bool blc_ll_isBleTaskIdle(void);


/**
 * @brief      This function is used to set some other channel to replace advertising & scanning channel 37/38/39.
 * @param[in]  chn0 - channel to replace channel 37
 * @param[in]  chn1 - channel to replace channel 38
 * @param[in]  chn2 - channel to replace channel 39
 * @return     none
 */
void blc_ll_setCustomizedAdvertisingScanningChannel(u8 chn0, u8 chn1, u8 chn2);


/**
 * @brief   Check if the RF is currently busy.
 * This function checks if the RF module is currently in a busy state.
 * @return  true if the RF is busy, false otherwise.
 */
bool blc_ll_isRfBusy(void);


/**
 * @brief   Retrieves the current active states of the BLE stack, similar to `blc_ll_getCurrentState`.
 * @param   none
 * @return  A bitmask representing the active BLE states.
 */
ble_status_t blc_ll_getBleCurrentState(void);

//#ifdef BLC_ZIGBEE_INTEGRATION
typedef bool (*ll_module_pm_lowpowerCond_t)(void);

#define    BLS_LINK_STATE_IDLE        0

u32 get_ble_end_event_tick(void);
u32 get_ble_next_event_tick(void);
u32 get_ble_event_state(void);

void restore_ble_rf_context(void);
void backup_ble_rf_context(void);
void ble_radio_init(void);

void ble_rf_fast_settle_recover(void);

/**
 * @brief       condition function if the ble is allowed to enter low power mode
 * @param[in]   cb the condition function
 * @return      1: allowed to enter low power mode
 */
void bls_pm_conditionCbRegister(ll_module_pm_lowpowerCond_t cb);

/**
 * @brief       release the condition function, so that  the ble won't be allowed to enter low power mode
 * @param[in]   none
 * @return      none
 */
void bls_pm_conditionCbUnregister(void);


/**
 * @brief       if the callback for allowing to enter low power mode is valid
 * @param[in]   none
 * @return
 */
bool bls_pm_conditionCbIsValid(void);

/**
 * @brief       if the callback for allowing to enter software low power mode is valid
 * @param[in]   1:enable, 0:disable
 * @return
 */
void  bls_pm_disableHardwarePm(u8 en);
//#endif /* BLC_ZIGBEE_INTEGRATION */


#endif /* LL_H_ */
