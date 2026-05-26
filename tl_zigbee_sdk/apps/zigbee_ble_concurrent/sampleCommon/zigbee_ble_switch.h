/********************************************************************************************************
 * @file    zigbee_ble_switch.h
 *
 * @brief   This is the header file for zigbee_ble_switch
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
#ifndef ZIGBEE_BLE_SWITCH_H
#define ZIGBEE_BLE_SWITCH_H

#define ZIGBEE_AFTER_TIME       (S_TIMER_CLOCK_1US * 1000 * 4) //4ms
#define BLE_IDLE_TIME           (S_TIMER_CLOCK_1US * 1000 * 4) //4ms

typedef enum {
    DUALMODE_SLOT_BLE,
    DUALMODE_SLOT_ZIGBEE,
} app_currentSlot_e;

typedef struct {
    u32 bleTaskTick;
    u8 bleState;
    volatile app_currentSlot_e slot;
} app_dualModeInfo_t;

typedef void (*master_service_t)(void);
typedef void (*master_update_t)(void);

typedef struct {
    master_service_t serviceCb;
    master_update_t updateCb;
} ble_master_cb_t;

extern app_dualModeInfo_t g_dualModeInfo;

#define CURRENT_SLOT_GET()              (g_dualModeInfo.slot)
#define CURRENT_SLOT_SET(s)             (g_dualModeInfo.slot = s)

#if defined(MCU_CORE_8258) || defined(MCU_CORE_B91)
#define APP_BLE_STATE_SET(state)        (g_dualModeInfo.bleState = state)
#define APP_BLE_STATE_GET()             (g_dualModeInfo.bleState)
#define APP_BLE_STATE_IDLE()            (APP_BLE_STATE_GET() == BLS_LINK_STATE_IDLE)
#elif defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL323X)
#define APP_BLE_STATE_IDLE()            (blc_ll_isBleTaskIdle())
#endif

#define ZB_RF_ISR_RECOVERY              do {  \
                                            if (CURRENT_SLOT_GET() == DUALMODE_SLOT_ZIGBEE) { \
                                                rf_set_irq_mask(FLD_RF_IRQ_RX | FLD_RF_IRQ_TX);  \
                                            }   \
                                        } while(0)

void switch_to_zb_context(void);
_attribute_ram_code_ void switch_to_ble_context(void);
int is_switch_to_ble(void);
int is_switch_to_zigbee(void);
u8 ble_task_stop(void);
u8 ble_task_restart(void);
void zb_ble_switch_proc(void);
void concurrent_mode_main_loop(void);

#if BLE_MASTER_ROLE_ENABLE
void ble_master_serviceCbRegister(master_service_t cb);
void ble_master_updateIndCbRegister(master_update_t cb);
#endif

#endif /* ZIGBEE_BLE_SWITCH_H */
