/********************************************************************************************************
 * @file    zcl_commissioning.c
 *
 * @brief   This is the source file for zcl_commissioning
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

/**********************************************************************
 * INCLUDES
 */
#include "../zcl_include.h"

#if 0
#define ZB_RANDOM_X(x) (x) ? (zb_random() % x) : 0

/**********************************************************************
 * LOCAL TYPES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */

#ifdef ZCL_COMMISSIONING

zcl_commissioning_AppCallbacks_t *zcl_commissioning_cb;

#endif    /* ZCL_COMMISSIONING */


/**********************************************************************
 * LOCAL FUNCTIONS
 */
u8 zcl_commissioning_cmdHandler(zclIncoming_t *pInMsg);


_CODE_ZCL_ static int zb_restart_device_func(void *arg)
{
    zcl_initialize_startup_attributes();
    /* TODO - */

    return -1;
}

/************************Client Command*************************************/
_CODE_ZCL_ void zcl_commissioning_restart_device(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo,
                                                 u8 options, u8 delay, u8 jitter)
{
    u8 buf[3];
    buf[0] = options;
    buf[1] = delay;
    buf[2] = jitter;

    zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_COMMISSIONING, ZCL_CMD_RESTART_DEVICE, TRUE,
                ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 3, buf);
}

// Server side handler
_CODE_ZCL_ u8 zcl_commissioning_clientCmdHandler(zclIncoming_t *pInMsg)
{
    u8 status = ZCL_STA_SUCCESS;
    u8 cmd = pInMsg->hdr.cmd;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t *)pInMsg->msg;
    //u8 endpoint = pApsdeInd->indInfo.dst_ep;

    epInfo_t dstEp;
    TL_SETSTRUCTCONTENT(dstEp, 0);

    dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
    dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
    dstEp.dstEp = pApsdeInd->indInfo.src_ep;
    dstEp.profileId = pApsdeInd->indInfo.profile_id;
    dstEp.radius = 0x0a;
    dstEp.txOptions = 0;

    switch (cmd) {
    case ZCL_CMD_RESTART_DEVICE:
        {
            zcl_restartDevice_t *restartDevice = (zcl_restartDevice_t *)pInMsg->pData;
            bool immediate = false;

            if (ZCL_RESTART_OPTIONS_GET_IMMEDIATE(restartDevice->options)) {
                if (restartDevice->delay == 0) {
                    /* Restart device immediately */
                    immediate = true;
                    zb_restart_device_func(&immediate);
                } else {
                    /* Restart device immediately after delay(s) + jitter(ms) */
                    u16 jitter_in_milliseconds = restartDevice->jitter ? ZB_RANDOM_X(restartDevice->jitter * 80) : 0;
                    u32 max_delay = (restartDevice->delay + (jitter_in_milliseconds / 1000)) * 1000;/* in microseconds */

                    immediate = true;
                    TL_ZB_TIMER_SCHEDULE(zb_restart_device_func, (void *)TRUE, max_delay);
                }
            } else {
                /* Restart device after (delay - jitter) + wait until pending frames have been transmitted */
                u16 jitter_in_milliseconds = restartDevice->jitter ? ZB_RANDOM_X(restartDevice->jitter * 80) : 0;
                u32 max_delay = 0;
                max_delay = (restartDevice->delay + (jitter_in_milliseconds / 1000)) * 1000;/* in microseconds */

                immediate = false;
                if (max_delay) {
                    TL_ZB_TIMER_SCHEDULE(zb_restart_device_func, (void *)TRUE, max_delay);
                } else {
                    zb_restart_device_func(&immediate);
                }
            }
        }
        break;
    case ZCL_CMD_SAVE_STARTUP_PARAMETERS:
        //TODO - handle command
        status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
        break;
    case ZCL_CMD_RESTORE_STARTUP_PARAMETERS:
        //TODO - handle command
        status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
        break;
    case ZCL_CMD_RESET_STARTUP_PARAMETERS:
        //TODO - handle command
        status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
        break;
    default:
        status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
        break;
    }

    return status;
}

// Client side handler
_CODE_ZCL_ u8 zcl_commissioning_serverCmdHandler(zclIncoming_t *pInMsg)
{
    u8 status = ZCL_STA_SUCCESS;
    u8 cmd = pInMsg->hdr.cmd;

    switch (cmd) {
    case ZCL_CMD_RESTART_DEVICE_RSP:
        //TODO - handle command
        status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
        break;
    case ZCL_CMD_SAVE_STARTUP_PARAMETERS_RSP:
        //TODO - handle command
        status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
        break;
    case ZCL_CMD_RESTORE_STARTUP_PARAMETERS_RSP:
        //TODO - handle command
        status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
        break;
    case ZCL_CMD_RESET_STARTUP_PARAMETERS_RSP:
        //TODO - handle command
        status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
        break;
    default:
        status = ZCL_STA_FAILURE;
        break;
    }

    return status;
}
#endif
