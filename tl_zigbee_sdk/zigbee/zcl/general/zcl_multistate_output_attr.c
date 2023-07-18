/********************************************************************************************************
 * @file    zcl_multistate_output_attr.c
 *
 * @brief   This is the source file for zcl_multistate_output_attr
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

#ifdef ZCL_MULTISTATE_OUTPUT

#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_STATE_TEXT_ENABLE
u8 zcl_attr_mulOutput_stateText[ZCL_MULTISTATE_OUTPUT_MAX_LENGTH] = {0};
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_DESCRIPTION_ENABLE
u8 zcl_attr_mulOutput_description[ZCL_MULTISTATE_OUTPUT_MAX_LENGTH] = {0};
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_NUM_OF_STATES_ENABLE
u16 zcl_attr_mulOutput_numOfStates = 0;
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_OUT_OF_SERVICE_ENABLE
bool zcl_attr_mulOutput_outOfService = FALSE;
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_PRESENT_VALUE_ENABLE
u16 zcl_attr_mulOutput_presentValue = 0;
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_PRIORITY_ARRAY_ENABLE
zcl_mulOutput_priorityArray_t zcl_attr_mulOutput_priorityArray[16] = {0};
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_RELIABLILITY_ENABLE
u8 zcl_attr_mulOutput_reliability = 0;
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_RELINQUISH_DEFAULT_ENABLE
u16 zcl_attr_mulOutput_relinquishdDfault = 0;
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_STATUS_FLAGS_ENABLE
u8 zcl_attr_mulOutput_statusFlags = 0;
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_APP_TYPE_ENABLE
u32 zcl_attr_mulOutput_appType = 0;
#endif



const zclAttrInfo_t multistate_output_attrTbl[] =
{
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_STATE_TEXT_ENABLE
	{ ZCL_MULTISTATE_OUTPUT_ATTRID_STATE_TEXT,			ZCL_DATA_TYPE_CHAR_STR,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,	(u8*)zcl_attr_mulOutput_stateText},
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_DESCRIPTION_ENABLE
	{ ZCL_MULTISTATE_OUTPUT_ATTRID_DESCRIPTION,			ZCL_DATA_TYPE_CHAR_STR,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,	(u8*)zcl_attr_mulOutput_description},
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_NUM_OF_STATES_ENABLE
	{ ZCL_MULTISTATE_OUTPUT_ATTRID_NUM_OF_STATES,		ZCL_DATA_TYPE_UINT16,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,	(u8*)&zcl_attr_mulOutput_numOfStates},
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_OUT_OF_SERVICE_ENABLE
	{ ZCL_MULTISTATE_OUTPUT_ATTRID_OUT_OF_SERVICE,		ZCL_DATA_TYPE_BOOLEAN,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_mulOutput_outOfService},
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_PRESENT_VALUE_ENABLE
	{ ZCL_MULTISTATE_OUTPUT_ATTRID_PRESENT_VALUE,		ZCL_DATA_TYPE_UINT16,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_mulOutput_presentValue},
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_PRIORITY_ARRAY_ENABLE
	{ ZCL_MULTISTATE_OUTPUT_ATTRID_PRIORITY_ARRAY,		ZCL_DATA_TYPE_ARRAY,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)zcl_attr_mulOutput_priorityArray},
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_RELIABLILITY_ENABLE
	{ ZCL_MULTISTATE_OUTPUT_ATTRID_RELIABLILITY,		ZCL_DATA_TYPE_ENUM8,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_mulOutput_reliability},
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_RELINQUISH_DEFAULT_ENABLE
	{ ZCL_MULTISTATE_OUTPUT_ATTRID_RELINQUISH_DEFAULT,	ZCL_DATA_TYPE_UINT16,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_mulOutput_relinquishdDfault},
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_STATUS_FLAGS_ENABLE
	{ ZCL_MULTISTATE_OUTPUT_ATTRID_STATUS_FLAGS,		ZCL_DATA_TYPE_BITMAP8,	ACCESS_CONTROL_READ,						(u8*)&zcl_attr_mulOutput_statusFlags},
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_APP_TYPE_ENABLE
	{ ZCL_MULTISTATE_OUTPUT_ATTRID_APP_TYPE,			ZCL_DATA_TYPE_UINT32,	ACCESS_CONTROL_READ,						(u8*)&zcl_attr_mulOutput_appType},
#endif
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,				ZCL_DATA_TYPE_UINT16,	ACCESS_CONTROL_READ,						(u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_multistate_output_attrNum = ( sizeof(multistate_output_attrTbl) / sizeof(zclAttrInfo_t) );

#endif	/* ZCL_MULISTATE_OUTPUT */

#endif
