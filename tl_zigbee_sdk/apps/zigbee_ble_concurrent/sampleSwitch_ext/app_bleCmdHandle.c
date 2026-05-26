/********************************************************************************************************
 * @file    app_bleCmdHandle.c
 *
 * @brief   This is the source file for app_bleCmdHandle
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
 *******************************************************************************************************/
#if (__PROJECT_TL_SWITCH__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zcl_include.h"
#include "sampleSwitch.h"
#include "app_ui.h"

enum {
    /*
     * the command identifier to configure the zigbee network
     * */
    APP_BLE_CMD_ZB_NETWORK_JOIN     =   0xFF00,
    APP_BLE_CMD_ZB_FACTORY_RESET    =   0xFF01,
    APP_BLE_CMD_ZB_NETWORK_KEY_SET  =   0xFF02,
    APP_BLE_CMD_ZB_LINK_KEY_SET     =   0xFF03,
    APP_BLE_CMD_ZB_NETWORK_JOIN_DIRECT  =   0xFF04,
};

typedef struct {
    u16 panId;
    u16 nwkAddr;
    u8  channel;
} joinNetworkInfo_t;


volatile u8 T_sampleSwitch_bdbNetworkJoinDirect[4] = {0};
joinNetworkInfo_t  g_joinNetworkInfo = {0};

s32 sampleSwitch_bdbNetworkJoinDirect(void *arg)
{
    u8 extPanId[] = {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa};
    u8 nwkKey[] = {0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89, 0x9a, 0xab, 0xbc, 0xcd, 0xde, 0xef, 0xf0, 0x01};

    T_sampleSwitch_bdbNetworkJoinDirect[0]++;
    if (SUCCESS == bdb_join_direct(g_joinNetworkInfo.channel, g_joinNetworkInfo.panId, g_joinNetworkInfo.nwkAddr, extPanId, nwkKey, SS_SEMODE_DISTRIBUTED, 1, NULL)) {
        T_sampleSwitch_bdbNetworkJoinDirect[1];
        return -1;
    }

    T_sampleSwitch_bdbNetworkJoinDirect[2]++;
    return 0;
}

int zb_ble_hci_cmd_handler(u16 cmdId, u8 len, u8 *payload)
{
    int ret = 0;
    if (cmdId == APP_BLE_CMD_ZB_NETWORK_JOIN) {
        bdb_networkSteerStart();
    } else if (cmdId == APP_BLE_CMD_ZB_FACTORY_RESET) {
        zb_resetDevice2FN();
    } else if (cmdId == APP_BLE_CMD_ZB_NETWORK_JOIN_DIRECT) {
        g_joinNetworkInfo.channel = payload[0];
        g_joinNetworkInfo.panId = ((u16)payload[1] | ((u16)payload[2] << 8));
        g_joinNetworkInfo.nwkAddr = ((u16)payload[3] | ((u16)payload[4] << 8));

        TL_ZB_TIMER_SCHEDULE(sampleSwitch_bdbNetworkJoinDirect, NULL, 50);
    } else {
        ret = -1;
    }
    return ret;
}

#endif  /* __PROJECT_TL_SWITCH__ */

