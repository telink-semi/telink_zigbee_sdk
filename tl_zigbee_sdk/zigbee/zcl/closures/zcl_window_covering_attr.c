/********************************************************************************************************
 * @file    zcl_window_covering_attr.c
 *
 * @brief   This is the source file for zcl_window_covering_attr
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
#ifdef ZCL_WINDOW_COVERING

/* Attribute default */
u8 zcl_attr_WindowCoveringType = 0;
u8 zcl_attr_ConfigStatus = 0x03;
u8 zcl_attr_Mode = 0x04;


/* Attribute record list */
const zclAttrInfo_t windowCovering_attrTbl[] =
{
    { ZCL_ATTRID_WINDOW_COVERING_TYPE,	   	ZCL_DATA_TYPE_ENUM8,     	ACCESS_CONTROL_READ,  						(u8*)&zcl_attr_WindowCoveringType},
    { ZCL_ATTRID_CONFIG_STATUS,	   			ZCL_DATA_TYPE_BITMAP8,     	ACCESS_CONTROL_READ,  						(u8*)&zcl_attr_ConfigStatus},
	{ ZCL_ATTRID_MODE,	   					ZCL_DATA_TYPE_BITMAP8,     	ACCESS_CONTROL_READ| ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_Mode},
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	ZCL_DATA_TYPE_UINT16,  		ACCESS_CONTROL_READ,  						(u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_windowCovering_attrNum = ( sizeof(windowCovering_attrTbl) / sizeof(zclAttrInfo_t) );

#endif    /* ZCL_IAS_ZONE */
#endif
