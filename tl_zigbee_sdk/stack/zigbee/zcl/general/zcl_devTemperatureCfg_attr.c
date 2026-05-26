/********************************************************************************************************
 * @file    zcl_devTemperatureCfg_attr.c
 *
 * @brief   This is the source file for zcl_devTemperatureCfg_attr
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
#ifdef ZCL_DEV_TEMPERATURE_CFG
s16 zcl_attr_curTemp = 0;
s16 zcl_attr_minTempExp = -199;
s16 zcl_attr_maxTempExp = 200;
u16 zcl_attr_overTempTotal = 0;
u8  zcl_attr_tempAlarmMask = 0;
s16 zcl_attr_lowTempThreshold = -199;
s16 zcl_attr_highTempThreshold = 200;
u24 zcl_attr_lowTripPoint = 0x000001;
u24 zcl_attr_highTripPoint = 0x000001;

/* Attribute record list */
const zclAttrInfo_t dev_temp_cfg_attrTbl[] = {
#ifdef ZCL_ATTR_CUTTRNT_TEMP_ENABLE
    { ZCL_ATTRID_DEV_TEMP_CURR_TEMP,       ZCL_DATA_TYPE_INT16,   ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_curTemp},
#endif
#ifdef ZCL_ATTR_MIN_TEMP_EXP_ENABLE
    { ZCL_ATTRID_DEV_TEMP_MIN_TEMP_EXP,    ZCL_DATA_TYPE_INT16,   ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_minTempExp},
#endif
#ifdef ZCL_ATTR_MAX_TEMP_EXP_ENABLE
    { ZCL_ATTRID_DEV_TEMP_MAX_TEMP_EXP,    ZCL_DATA_TYPE_INT16,   ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_maxTempExp},
#endif
#ifdef ZCL_ATTR_OVER_TEMP_TOTAL_ENABLE
    { ZCL_ATTRID_DEV_TEMP_OVER_TEMP_TOTAL, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_overTempTotal},
#endif
#ifdef ZCL_ATTR_OVER_DEV_TEMP_ALARM_MASK_ENABLE
    { ZCL_ATTRID_DEV_TEMP_ALARM_MASK,      ZCL_DATA_TYPE_BITMAP8, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_tempAlarmMask},
#endif
#ifdef ZCL_ATTR_OVER_LOW_TEMP_THRESHOLD_ENABLE
    { ZCL_ATTRID_DEV_TEMP_LOW_THRES,       ZCL_DATA_TYPE_INT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_lowTempThreshold},
#endif
#ifdef ZCL_ATTR_OVER_HIGH_TEMP_THERSHOLD_ENABLE
    { ZCL_ATTRID_DEV_TEMP_HIGH_THRES,      ZCL_DATA_TYPE_INT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_highTempThreshold},
#endif
#ifdef ZCL_ATTR_OVER_LOW_TEMP_TRIP_ENABLE
    { ZCL_ATTRID_DEV_TEMP_LOW_TRIP,        ZCL_DATA_TYPE_UINT24,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_lowTripPoint},
#endif
#ifdef ZCL_ATTR_OVER_HIGH_TEMP_TRIP_ENABLE
    { ZCL_ATTRID_DEV_TEMP_HIGH_TRIP,       ZCL_DATA_TYPE_UINT24,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_highTripPoint},
#endif
    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,  ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_dev_temp_cfg_attrNum = (sizeof(dev_temp_cfg_attrTbl) / sizeof(zclAttrInfo_t));
 
#endif    /* ZCL_DEV_TEMPERATURE_CFG */

#endif
