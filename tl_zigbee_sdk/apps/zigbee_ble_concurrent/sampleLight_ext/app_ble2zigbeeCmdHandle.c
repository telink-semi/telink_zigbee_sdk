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
#if (__PROJECT_TL_DIMMABLE_LIGHT__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zcl_include.h"
#include "sampleLight.h"

enum{
    /*
     * the command identifier to configure the zigbee network
     * */
    APP_BLE_CMD_ZB_NETWORK_JOIN         =   0xFF00,
    APP_BLE_CMD_ZB_NETWORK_KEY_SET      =   0xFF01,
    APP_BLE_CMD_ZB_LINK_KEY_SET         =   0xFF02,
    APP_BLE_CMD_ZB_NWK_MANAGEMENT_SET   =   0xFF03,
    APP_BLE_CMD_ZB_NETWORK_JOIN_DIRECT  =   0xFF04,

    /*
     * the command identifier to change some ble setting
     * */
    APP_BLE_CMD_INTERVAL_SET        =   0xFF10
};

typedef struct {
    u16 panId;
    u16 nwkAddr;
    u8  channel;
} joinNetworkInfo_t;

joinNetworkInfo_t  g_joinNetworkInfo = {0};

extern void sampleLight_moveToLevelProcess(u8 cmdId, moveToLvl_t *cmd);

s32 sampleLight_bdbNetworkJoinDirect(void *arg)
{
    u8 extPanId[] = {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa};
    u8 nwkKey[] = {0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89, 0x9a, 0xab, 0xbc, 0xcd, 0xde, 0xef, 0xf0, 0x01};

    if (SUCCESS == bdb_join_direct(g_joinNetworkInfo.channel, g_joinNetworkInfo.panId, g_joinNetworkInfo.nwkAddr, extPanId, nwkKey, SS_SEMODE_DISTRIBUTED, 1, NULL)) {
        return -1;
    }

    return 0;
}

int zb_ble_hci_cmd_handler(u16 clusterId, u8 len, u8 *payload)
{
    u8 cmdId = payload[0];
    u8 *pCmd = &payload[1];

    int ret = 0;

    if (clusterId == APP_BLE_CMD_ZB_NWK_MANAGEMENT_SET) {
        u16 dstAddr = 0xfffd;
        u8 sn = 0;
        zdo_mgmt_nwk_update_req_t req;

        req.nwk_manager_addr = 0x1234;
        req.scan_ch = 0x07fff800;
        req.scan_duration = ZDO_NWK_MANAGER_ATTRIBUTES_CHANGE;
        req.nwk_update_id = 0x00;

        zb_mgmtNwkUpdateReq(dstAddr, &req, &sn, NULL);
    } else if (clusterId == ZCL_CLUSTER_GEN_ON_OFF) {
        sampleLight_onOffUpdate(cmdId);
    } else if (clusterId == ZCL_CLUSTER_GEN_LEVEL_CONTROL) {
        if (cmdId == ZCL_CMD_LEVEL_MOVE_TO_LEVEL) {
            sampleLight_moveToLevelProcess(cmdId, (moveToLvl_t *)pCmd);
        }
    } else if (clusterId == APP_BLE_CMD_INTERVAL_SET) {
        extern void app_bleConnIntervalSet(u8 interval, u16 latency);
        app_bleConnIntervalSet(payload[0], ((payload[1] << 8) | payload[2]));
    } else if(clusterId == APP_BLE_CMD_ZB_NETWORK_JOIN_DIRECT) {
        g_joinNetworkInfo.channel = payload[0];
        g_joinNetworkInfo.panId = ((u16)payload[1] | ((u16)payload[2] << 8));
        g_joinNetworkInfo.nwkAddr = ((u16)payload[3] | ((u16)payload[4] << 8));

        TL_ZB_TIMER_SCHEDULE(sampleLight_bdbNetworkJoinDirect, NULL, 50);
    } else {
        ret = -1;
    }
    return ret;
}

#endif  /* __PROJECT_TL_DIMMABLE_LIGHT__ */
