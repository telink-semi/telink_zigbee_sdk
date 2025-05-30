/********************************************************************************************************
 * @file    zcl_pollCtrl_attr.c
 *
 * @brief   This is the source file for zcl_pollCtrl_attr
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
#if 0
#include "tl_common.h"

/**********************************************************************
 * INCLUDES
 */
#include "../zcl_include.h"


/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * LOCAL TYPES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */
#ifdef ZCL_POLL_CTRL

/* Attribute default */
zcl_pollCtrl_attr_t gZclPollCtrlAttr = {
    .chkInInterval              = 0x3840, //1 hour
    .longPollInterval           = 0x14,   //5 sec
    .shortPollInterval          = 0x02,   //2 qs
    .fastPollTimeout            = 0x28,   //10 sec
    .chkInIntervalMin           = 0x28,   //10 sec
    .longPollIntervalMin        = 0x0c,   //3 sec
    .fastPollTimeoutMax         = 0xf0,   //60 sec
};

/* Attribute record list */
const zclAttrInfo_t pollCtrl_attrTbl[] = {
    { ZCL_ATTRID_CHK_IN_INTERVAL,         ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&gZclPollCtrlAttr.chkInInterval },
    { ZCL_ATTRID_LONG_POLL_INTERVAL,      ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ,                        (u8*)&gZclPollCtrlAttr.longPollInterval },
    { ZCL_ATTRID_SHORT_POLL_INTERVAL,     ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ,                        (u8*)&gZclPollCtrlAttr.shortPollInterval },
    { ZCL_ATTRID_FAST_POLL_TIMEOUT,       ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&gZclPollCtrlAttr.fastPollTimeout },
    { ZCL_ATTRID_CHK_IN_INTERVAL_MIN,     ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ,                        (u8*)&gZclPollCtrlAttr.chkInIntervalMin},
    { ZCL_ATTRID_LONG_POLL_INTERVAL_MIN,  ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ,                        (u8*)&gZclPollCtrlAttr.longPollIntervalMin },
    { ZCL_ATTRID_FAST_POLL_TIMEOUT_MAX,   ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ,                        (u8*)&gZclPollCtrlAttr.fastPollTimeoutMax },
    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_global_clusterRevision },
};

const u8 zcl_pollCtrl_attrNum = (sizeof(pollCtrl_attrTbl) / sizeof(zclAttrInfo_t));

#endif    /* ZCL_POLL_CTRL */
#endif
