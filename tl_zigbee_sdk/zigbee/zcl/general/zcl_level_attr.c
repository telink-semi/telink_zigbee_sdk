/********************************************************************************************************
 * @file    zcl_level_attr.c
 *
 * @brief   This is the source file for zcl_level_attr
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
#ifdef ZCL_LEVEL_CTRL

zcl_levelAttr_t zcl_attr_level = {
#ifdef ZCL_ATTR_CURRENT_LEVEL_ENABLE
    .curLevel            = ZCL_LEVEL_ATTR_CURRENT_LEVEL_DEFAULT, //default level
#endif
#ifdef ZCL_ATTR_REMAINING_TIME_ENABLE
    .remainingTime       = 0,
#endif
#ifdef ZCL_ATTR_ONOFF_TRANS_TIME_ENABLE
    .onOffTransTime      = 0,
#endif
#ifdef ZCL_ATTR_ON_LEVEL_ENABLE
    .onLevel             = ZCL_LEVEL_ATTR_ON_LEVEL_NO_EFFECT, //no effect
#endif
#ifdef ZCL_ATTR_ON_TRANS_ENABLE
    .onTransitionTime    = 0xFFFF,
#endif
#ifdef ZCL_ATTR_OFF_TRANS_ENABLE
    .offTransitionTime   = 0xFFFF,
#endif
#ifdef ZCL_ATTR_DEFAULT_MOVE_RATE_ENABLE
    .defaultMoveRate     = 0,
#endif
#ifdef ZCL_ATTR_OPTIONS_ENABLE
    .options             = 0,
#endif
#ifdef ZCL_ATTR_START_UP_CURRENT_LEVEL_ENABLE
    .startUpCurrentLevel = ZCL_START_UP_CURRENT_LEVEL_TO_PREVIOUS,
#endif
};

/* Attribute record list */
const zclAttrInfo_t level_attrTbl[] = {
#ifdef ZCL_ATTR_CURRENT_LEVEL_ENABLE
    { ZCL_ATTRID_LEVEL_CURRENT_LEVEL,          ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zcl_attr_level.curLevel },
#endif

#ifdef ZCL_ATTR_REMAINING_TIME_ENABLE
    { ZCL_ATTRID_LEVEL_REMAINING_TIME,         ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,                             (u8*)&zcl_attr_level.remainingTime },
#endif

#ifdef ZCL_ATTR_ONOFF_TRANS_TIME_ENABLE
    { ZCL_ATTRID_LEVEL_ON_OFF_TRANSITION_TIME, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zcl_attr_level.onOffTransTime },
#endif

#ifdef ZCL_ATTR_ON_LEVEL_ENABLE
    { ZCL_ATTRID_LEVEL_ON_LEVEL,               ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zcl_attr_level.onLevel },
#endif

#ifdef ZCL_ATTR_ON_TRANS_ENABLE
    { ZCL_ATTRID_LEVEL_ON_TRANSITION_TIME,     ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zcl_attr_level.onTransitionTime },
#endif

#ifdef ZCL_ATTR_OFF_TRANS_ENABLE
    { ZCL_ATTRID_LEVEL_OFF_TRANSITION_TIME,    ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zcl_attr_level.offTransitionTime },
#endif

#ifdef ZCL_ATTR_DEFAULT_MOVE_RATE_ENABLE
    { ZCL_ATTRID_LEVEL_DEFAULT_MOVE_RATE,      ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zcl_attr_level.defaultMoveRate },
#endif

#ifdef ZCL_ATTR_OPTIONS_ENABLE
    { ZCL_ATTRID_LEVEL_OPTIONS,                ZCL_DATA_TYPE_BITMAP8, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zcl_attr_level.options },
#endif

#ifdef ZCL_ATTR_START_UP_CURRENT_LEVEL_ENABLE
    { ZCL_ATTRID_LEVEL_START_UP_CURRENT_LEVEL, ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&zcl_attr_level.startUpCurrentLevel },
#endif

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,      ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,                             (u8*)&zcl_attr_global_clusterRevision },
};

const u8 zcl_level_attrNum = (sizeof(level_attrTbl) / sizeof(zclAttrInfo_t));

nv_sts_t zcl_level_saveAttr(void)
{
    return zcl_attr_levelParams_save();
}

nv_sts_t zcl_level_loadAttr(void)
{
    return zcl_attr_levelParams_restore();
}

#endif    /* ZCL_LEVEL_CTRL */

#endif
