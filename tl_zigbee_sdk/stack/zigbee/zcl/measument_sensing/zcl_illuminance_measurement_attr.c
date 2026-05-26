/********************************************************************************************************
 * @file    zcl_illuminance_measurement_attr.c
 *
 * @brief   This is the source file for zcl_illuminance_measurement_attr
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
#ifdef ZCL_ILLUMINANCE_MEASUREMENT

u16 zcl_attr_measuredVal = 0;
u16 zcl_attr_minMeasuredVal = 0;
u16 zcl_attr_maxMeasuredVal = 0;
#ifdef ZCL_ATTR_TOLERANCE_ENABLE
u16 zcl_attr_tolerance = 0;
#endif
#ifdef ZCL_ATTR_LIGHT_SENSOR_TYPE_ENABLE
u8 zcl_attr_lightSensorType = 0;
#endif

/* Attribute record list */
const zclAttrInfo_t illuminanceMeasure_attrTbl[] = {
    { ZCL_ATTRID_MEASURED_VALUE,          ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&zcl_attr_measuredVal },
    { ZCL_ATTRID_MIN_MEASURED_VALUE,      ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&zcl_attr_minMeasuredVal },
    { ZCL_ATTRID_MAX_MEASURED_VALUE,      ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&zcl_attr_maxMeasuredVal },
#ifdef ZCL_ATTR_TOLERANCE_ENABLE
    { ZCL_ATTRID_TOLERANCE,               ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&zcl_attr_tolerance },
#endif

#ifdef ZCL_ATTR_LIGHT_SENSOR_TYPE_ENABLE
    { ZCL_ATTRID_LIGHT_SENSOR_TYPE,       ZCL_DATA_TYPE_ENUM8,  ACCESS_CONTROL_READ, (u8*)&zcl_attr_lightSensorType },
#endif
    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&zcl_attr_global_clusterRevision },
};

const u8 zcl_illuminanceMeasure_attrNum = (sizeof(illuminanceMeasure_attrTbl) / sizeof(zclAttrInfo_t));

#endif    /* ZCL_ILLUMINANCE_MEASUREMENT */

#endif
