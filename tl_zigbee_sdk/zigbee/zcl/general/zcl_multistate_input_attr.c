/********************************************************************************************************
 * @file    zcl_multistate_input_attr.c
 *
 * @brief   This is the source file for zcl_multistate_input_attr
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#ifdef ZCL_MULITSTATE_INPUT

#ifdef ZCL_MULTISTATE_INPUT_ATTR_STATE_TEXT_ENBLE
u8 zcl_attr_mulInput_stateText[ZCL_MULTISTATE_INPUT_MAX_LENGTH] = {0};
#endif
#ifdef ZCL_MULTISTATE_INPUT_ATTR_DESCRIPTION_ENBLE
u8 zcl_attr_mulInput_description[ZCL_MULTISTATE_INPUT_MAX_LENGTH] = {0};
#endif
#ifdef ZCL_MULTISTATE_INPUT_ATTR_NUM_OF_STATES_ENBLE
u16 zcl_attr_mulInput_numOfStates = 0;
#endif
#ifdef ZCL_MULTISTATE_INPUT_ATTR_OUT_OF_SERVICE_ENBLE
bool zcl_attr_mulInput_outOfService = FALSE;
#endif
#ifdef ZCL_MULTISTATE_INPUT_ATTR_PRESENT_VALUE_ENBLE
u16 zcl_attr_mulInput_presentValue = 0;
#endif
#ifdef ZCL_MULTISTATE_INPUT_ATTR_RELIABLILITY_ENBLE
u8 zcl_attr_mulInput_reliability = 0x00;
#endif
#ifdef ZCL_MULTISTATE_INPUT_ATTR_STATUS_FLAGS_ENBLE
u8 zcl_attr_mulInput_statusFlags = 0;
#endif
#ifdef ZCL_MULTISTATE_INPUT_ATTR_APP_TYPE_ENBLE
u32 zcl_attr_mulInput_appType = 0;
#endif



const zclAttrInfo_t multistate_input_attrTbl[] =
{
#ifdef ZCL_MULTISTATE_INPUT_ATTR_STATE_TEXT_ENBLE
	{ ZCL_MULTISTATE_INPUT_ATTRID_STATE_TEXT,		ZCL_DATA_TYPE_CHAR_STR,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)zcl_attr_mulInput_stateText},
#endif
#ifdef ZCL_MULTISTATE_INPUT_ATTR_DESCRIPTION_ENBLE
	{ ZCL_MULTISTATE_INPUT_ATTRID_DESCRIPTION,		ZCL_DATA_TYPE_CHAR_STR,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)zcl_attr_mulInput_description},
#endif
#ifdef ZCL_MULTISTATE_INPUT_ATTR_NUM_OF_STATES_ENBLE
	{ ZCL_MULTISTATE_INPUT_ATTRID_NUM_OF_STATES,	ZCL_DATA_TYPE_UINT16,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_mulInput_numOfStates},
#endif
#ifdef ZCL_MULTISTATE_INPUT_ATTR_OUT_OF_SERVICE_ENBLE
	{ ZCL_MULTISTATE_INPUT_ATTRID_OUT_OF_SERVICE,	ZCL_DATA_TYPE_BOOLEAN,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_mulInput_outOfService},
#endif
#ifdef ZCL_MULTISTATE_INPUT_ATTR_PRESENT_VALUE_ENBLE
	{ ZCL_MULTISTATE_INPUT_ATTRID_PRESENT_VALUE,	ZCL_DATA_TYPE_UINT16,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_mulInput_presentValue},
#endif
#ifdef ZCL_MULTISTATE_INPUT_ATTR_RELIABLILITY_ENBLE
	{ ZCL_MULTISTATE_INPUT_ATTRID_RELIABLILITY,		ZCL_DATA_TYPE_ENUM8,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_mulInput_reliability},
#endif
#ifdef ZCL_MULTISTATE_INPUT_ATTR_STATUS_FLAGS_ENBLE
	{ ZCL_MULTISTATE_INPUT_ATTRID_STATUS_FLAGS,		ZCL_DATA_TYPE_BITMAP8,	ACCESS_CONTROL_READ,                       	(u8*)&zcl_attr_mulInput_statusFlags},
#endif
#ifdef ZCL_MULTISTATE_INPUT_ATTR_APP_TYPE_ENBLE
	{ ZCL_MULTISTATE_INPUT_ATTRID_APP_TYPE,			ZCL_DATA_TYPE_UINT32,	ACCESS_CONTROL_READ,  						(u8*)&zcl_attr_mulInput_appType},
#endif
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,			ZCL_DATA_TYPE_UINT16,	ACCESS_CONTROL_READ,  						(u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_multistate_input_attrNum = ( sizeof(multistate_input_attrTbl) / sizeof(zclAttrInfo_t) );

#endif	/* ZCL_MULISTATE_INPUT */

#endif
