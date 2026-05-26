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
#include "app.h"

#define TICK_ONE_MIN    (60 * SYSTEM_TIMER_TICK_1S)

extern void user_init(bool isRetention);

void app_init(startup_state_e state)
{
    u8 isRetention = (state == SYSTEM_DEEP_RETENTION) ? 1 : 0;
    u8 isBoot = (state == SYSTEM_BOOT) ? 1 : 0;

    printf("[APP]: state = %d, boot = %d, retention = %d\n", state, isBoot, isRetention);

    ev_init(!isRetention);
    tl_zbTaskInit(!isRetention);

    user_init(isRetention);
}


volatile u32 T_DBG_total_min = 0;
void app_proc(void)
{
    ev_main();
    tl_zbTaskProcedure();

#if (ZB_ED_ROLE || ZB_ROUTER_ROLE)
    static u32 pre_timer = 0;
    static u32 intv_min = 0;
    if ((clock_time() - pre_timer) >= TICK_ONE_MIN) {
    	if (intv_min++ >= (60 * 24)) {
    		bdb_updateTCLK(TRUE);
    		intv_min = 0;
    	}
    	T_DBG_total_min++;
        pre_timer = clock_time();
    }
#endif
}
