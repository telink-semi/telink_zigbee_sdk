/********************************************************************************************************
 * @file    zcl_binary_output_attr.c
 *
 * @brief   This is the source file for zcl_binary_output_attr
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

#ifdef ZCL_BINARY_OUTPUT

#ifdef ZCL_BINARY_OUTPUT_ATTR_ACTIVE_TEXT_ENABLE
const u8 zcl_attr_binOutput_activeText[ZCL_BINARY_OUTPUT_MAX_LENGTH] = {0};
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_DESCRIPTIONT_ENABLE
const u8 zcl_attr_binOutput_description[ZCL_BINARY_OUTPUT_MAX_LENGTH] = {0};
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_INACTIVE_TEXT_ENABLE
const u8 zcl_attr_binOutput_inactiveText[ZCL_BINARY_OUTPUT_MAX_LENGTH] = {0};
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_MIN_OFF_TIME_ENABLE
u32 zcl_attr_binOutput_minOffTime = 0xFFFFFFFF;
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_MIN_ON_TIME_ENABLE
u32 zcl_attr_binOutput_minOnTime = 0xFFFFFFFF;
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_OUT_OF_SERVICE_ENABLE
bool zcl_attrr_binOutput_outOfService = FALSE;
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_POLARITY_ENABLE
u8 zcl_attr_binOutput_polarity = 0;
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_PRESENT_VALUE_ENABLE
bool zcl_attr_binOutput_presentValue = 0;
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_PRIORITY_ARRAY_ENABLE
zcl_binOutput_priorityArray_t zcl_attr_binOutput_priorityArray[16] = {0};
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_RELIABILITY_ENABLE
u8 zcl_attr_binOutput_reliability = 0;
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_RELINQUISH_DEFAULT_ENABLE
bool zcl_attr_binOutput_relinquishDefault = 0;
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_STATUS_FLAGS_ENABLE
u8 zcl_attr_binOutput_statusFlags = 0;
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_APP_TYPE_ENABLE
u32 zcl_attr_binOutput_appType = 0;
#endif

const zclAttrInfo_t binary_output_attrTbl[] =
{
#ifdef ZCL_BINARY_OUTPUT_ATTR_ACTIVE_TEXT_ENABLE
	{ ZCL_BINARY_OUTPUT_ATTRID_ACTIVE_TEXT,			ZCL_DATA_TYPE_CHAR_STR,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)zcl_attr_binOutput_activeText},
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_DESCRIPTIONT_ENABLE
	{ ZCL_BINARY_OUTPUT_ATTRID_DESCRIPTION,			ZCL_DATA_TYPE_CHAR_STR,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)zcl_attr_binOutput_description},
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_INACTIVE_TEXT_ENABLE
	{ ZCL_BINARY_OUTPUT_ATTRID_INACTIVE_TEXT,		ZCL_DATA_TYPE_CHAR_STR,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)zcl_attr_binOutput_inactiveText},
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_MIN_OFF_TIME_ENABLE
	{ ZCL_BINARY_OUTPUT_ATTRID_MIN_OFF_TIME,		ZCL_DATA_TYPE_UINT32,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_binOutput_minOffTime},
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_MIN_ON_TIME_ENABLE
	{ ZCL_BINARY_OUTPUT_ATTRID_MIN_ON_TIME,			ZCL_DATA_TYPE_UINT32,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_binOutput_minOnTime},
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_OUT_OF_SERVICE_ENABLE
	{ ZCL_BINARY_OUTPUT_ATTRID_OUT_OF_SERVICE,		ZCL_DATA_TYPE_BOOLEAN,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attrr_binOutput_outOfService},
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_POLARITY_ENABLE
	{ ZCL_BINARY_OUTPUT_ATTRID_POLARITY,			ZCL_DATA_TYPE_ENUM8,	ACCESS_CONTROL_READ,     					(u8*)&zcl_attr_binOutput_polarity},
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_PRESENT_VALUE_ENABLE
	{ ZCL_BINARY_OUTPUT_ATTRID_PRESENT_VALUE,		ZCL_DATA_TYPE_BOOLEAN,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_binOutput_presentValue},
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_PRIORITY_ARRAY_ENABLE
	{ ZCL_BINARY_OUTPUT_ATTRID_PRIORITY_ARRAY,		ZCL_DATA_TYPE_ARRAY,   	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)zcl_attr_binOutput_priorityArray},
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_RELIABILITY_ENABLE
	{ ZCL_BINARY_OUTPUT_ATTRID_RELIABLILITY,		ZCL_DATA_TYPE_ENUM8,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_binOutput_reliability},
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_RELINQUISH_DEFAULT_ENABLE
	{ ZCL_BINARY_OUTPUT_ATTRID_RELINQUISH_DEFAULT,	ZCL_DATA_TYPE_BOOLEAN, 	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_binOutput_relinquishDefault},
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_STATUS_FLAGS_ENABLE
	{ ZCL_BINARY_OUTPUT_ATTRID_STATUS_FLAGS,		ZCL_DATA_TYPE_BITMAP8,	ACCESS_CONTROL_READ, 						(u8*)&zcl_attr_binOutput_statusFlags},
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_APP_TYPE_ENABLE
	{ ZCL_BINARY_OUTPUT_ATTRID_APP_TYPE,			ZCL_DATA_TYPE_UINT32,	ACCESS_CONTROL_READ, 						(u8*)&zcl_attr_binOutput_appType},
#endif
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,			ZCL_DATA_TYPE_UINT16,	ACCESS_CONTROL_READ,  						(u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_binary_output_attrNum = ( sizeof(binary_output_attrTbl) / sizeof(zclAttrInfo_t) );

#endif	/* ZCL_BINARY_OUTPUT */

#endif
