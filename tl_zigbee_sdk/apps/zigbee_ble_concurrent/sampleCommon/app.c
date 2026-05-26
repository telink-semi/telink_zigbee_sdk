/********************************************************************************************************
 * @file    app.c
 *
 * @brief   This is the source file for app
 *
 * @author  Zigbee Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "zb_common.h"
#include "zigbee_ble_switch.h"
#include "ble.h"
#include "app.h"

extern void user_init(bool isRetention);
extern void user_ble_init(bool isRetention);

void app_init(startup_state_e state)
{
    u8 isRetention = (state == SYSTEM_DEEP_RETENTION) ? 1 : 0;

    ev_init(!isRetention);
    tl_zbTaskInit(!isRetention);
    user_init(isRetention);

    ble_radio_init();
    user_ble_init(isRetention);

#if PM_ENABLE
    if(CURRENT_SLOT_GET() == DUALMODE_SLOT_ZIGBEE){
    #if (!BLE_ACTIVE_BY_UI)
        if (APP_BLE_STATE_IDLE()) {
            ble_task_restart();
            ble_advertiseTickUpdate();
        }
    #else
        switch_to_zb_context();
    #endif
    }
#endif
}

void app_proc(void)
{
    concurrent_mode_main_loop();
}
