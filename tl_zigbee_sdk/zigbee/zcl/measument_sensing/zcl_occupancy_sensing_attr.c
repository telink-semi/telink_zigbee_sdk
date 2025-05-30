/********************************************************************************************************
 * @file    zcl_occupancy_sensing_attr.c
 *
 * @brief   This is the source file for zcl_occupancy_sensing_attr
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
#ifdef ZCL_OCCUPANCY_SENSING
u8  zcl_attr_occup = 0;
u8  zcl_attr_occupSensorType = 0;
u16 zcl_attr_pirOccu2UncoouD = 0;
u16 zcl_attr_pirUnoccu2OccuD = 0;
u8  zcl_attr_pirUnoccu2OccuT = 1;
u16 zcl_attr_ultraOccu2UncoouD = 0;
u16 zcl_attr_ultraUnoccu2OccuD = 0;
u8  zcl_attr_ultraUnoccu2OccuT = 1;

/* Attribute record list */
const zclAttrInfo_t occupancy_attrTbl[] = {
#ifdef ZCL_ATTR_OCCUPANCY_ENABLE
    { ZCL_ATTRID_OCCUPANCY,               ZCL_DATA_TYPE_BITMAP8, ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_occup},
#endif
#ifdef ZCL_ATTR_OCCUPANCY_SENSOR_TYPE_ENABLE
    { ZCL_ATTRID_OCCUPANCY_SENSOR_TYPE,   ZCL_DATA_TYPE_ENUM8,   ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_occupSensorType},
#endif

#ifdef ZCL_ATTR_PIR_OCCU2UNOCCU_DELAY_ENABLE
    { ZCL_ATTRID_PIR_OCCU2UNOCCU_DELAY,   ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_pirOccu2UncoouD},
#endif
#ifdef ZCL_ATTR_PIR_UNOCCU2OCCU_DELAY_ENABLE
    { ZCL_ATTRID_PIR_UNOCCU2OCCU_DELAY,   ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_pirUnoccu2OccuD},
#endif
#ifdef ZCL_ATTR_PIR_UNOCCU2OCCU_THRES_ENABLE
    { ZCL_ATTRID_PIR_UNOCCU2OCCU_THRES,   ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_pirUnoccu2OccuT},
#endif

#ifdef ZCL_ATTR_ULTRA_OCCU2UNOCCU_DELAY_ENABLE
    { ZCL_ATTRID_ULTRA_OCCU2UNOCCU_DELAY, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_ultraOccu2UncoouD},
#endif
#ifdef ZCL_ATTR_ULTRA_UNOCCU2OCCU_DELAY_ENABLE
    { ZCL_ATTRID_ULTRA_UNOCCU2OCCU_DELAY, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_ultraUnoccu2OccuD},
#endif
#ifdef ZCL_ATTR_ULTRA_UNOCCU2OCCU_THRES_ENABLE
    { ZCL_ATTRID_ULTRA_UNOCCU2OCCU_THRES, ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_ultraUnoccu2OccuT},
#endif
    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_occupancy_attrNum = (sizeof(occupancy_attrTbl) / sizeof(zclAttrInfo_t));

#endif    /* ZCL_OCCUPANCY_SENSING */

#endif
