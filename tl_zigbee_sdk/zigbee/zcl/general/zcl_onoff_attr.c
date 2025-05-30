/********************************************************************************************************
 * @file    zcl_onoff_attr.c
 *
 * @brief   This is the source file for zcl_onoff_attr
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
#ifdef ZCL_ON_OFF

zcl_onoff_attr_t g_zclOnOffAttr = {
#ifdef ZCL_ATTR_ON_TIME_ENABLE
    .onTime             = 0,
#endif
#ifdef ZCL_ATTR_OFF_WAIT_TIME_ENABLE
    .offWaitTime        = 0,
#endif
#ifdef ZCL_ATTR_ONOFF_ENABLE
    .onOff              = 0,
#endif
#ifdef ZCL_ATTR_GLOBAL_SCENE_CONTROL_ENABLE
    .globalSceneControl = TRUE,
#endif
#ifdef ZCL_ATTR_START_UP_ONOFF_ENABLE
    .startUpOnOff       = ZCL_START_UP_ONOFF_SET_DEFAULT,
#endif
};

/* Attribute record list */
const zclAttrInfo_t onoff_attrTbl[] = {
#ifdef ZCL_ATTR_ONOFF_ENABLE
    { ZCL_ATTRID_ONOFF,                   ZCL_DATA_TYPE_BOOLEAN, ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&g_zclOnOffAttr.onOff},
#endif
#ifdef ZCL_ATTR_GLOBAL_SCENE_CONTROL_ENABLE
    { ZCL_ATTRID_GLOBAL_SCENE_CONTROL,    ZCL_DATA_TYPE_BOOLEAN, ACCESS_CONTROL_READ,                             (u8*)&g_zclOnOffAttr.globalSceneControl},
#endif
#ifdef ZCL_ATTR_ON_TIME_ENABLE
    { ZCL_ATTRID_ON_TIME,                 ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&g_zclOnOffAttr.onTime},
#endif
#ifdef ZCL_ATTR_OFF_WAIT_TIME_ENABLE
    { ZCL_ATTRID_OFF_WAIT_TIME,           ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&g_zclOnOffAttr.offWaitTime},
#endif
#ifdef ZCL_ATTR_START_UP_ONOFF_ENABLE
    { ZCL_ATTRID_START_UP_ONOFF,          ZCL_DATA_TYPE_ENUM8,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&g_zclOnOffAttr.startUpOnOff},
#endif
    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,                             (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_onOff_attrNum = (sizeof(onoff_attrTbl) / sizeof(zclAttrInfo_t));

nv_sts_t zcl_onOff_saveAttr(void)
{
    return zcl_attr_onOffParams_save();
}

nv_sts_t zcl_onOff_loadAttr(void)
{
    return zcl_attr_onOffParams_restore();
}

#endif    /* ZCL_ON_OFF */

#endif
