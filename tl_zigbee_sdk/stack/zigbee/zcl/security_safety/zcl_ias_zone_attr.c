/********************************************************************************************************
 * @file    zcl_ias_zone_attr.c
 *
 * @brief   This is the source file for zcl_ias_zone_attr
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
#ifdef ZCL_IAS_ZONE

/* Attribute default */
u8  zcl_attr_zoneState = ZONE_STATE_NOT_ENROLLED;
u16 zcl_attr_zoneType = 0;
u16 zcl_attr_zoneStatus = 0;
u8  zcl_attr_iasCieAddr[8] = {0};
u8  zcl_attr_zoneID = 0xFF;

/* Attribute record list */
const zclAttrInfo_t iasZone_attrTbl[] = {
    { ZCL_ATTRID_ZONE_STATE,              ZCL_DATA_TYPE_ENUM8,     ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_zoneState},
    { ZCL_ATTRID_ZONE_TYPE,               ZCL_DATA_TYPE_ENUM16,    ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_zoneType},
    { ZCL_ATTRID_ZONE_STATUS,             ZCL_DATA_TYPE_BITMAP16,  ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_zoneStatus},
    { ZCL_ATTRID_IAS_CIE_ADDR,            ZCL_DATA_TYPE_IEEE_ADDR, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, zcl_attr_iasCieAddr},
    { ZCL_ATTRID_ZONE_ID,                 ZCL_DATA_TYPE_UINT8,     ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_zoneID},
    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,    ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_iasZone_attrNum = (sizeof(iasZone_attrTbl) / sizeof(zclAttrInfo_t));

#endif    /* ZCL_IAS_ZONE */
#endif
