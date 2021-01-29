/********************************************************************************************************
 * @file	zcl_multistate_output_attr.c
 *
 * @brief	This is the source file for zcl_multistate_output_attr
 *
 * @author	Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_STATE_TEXT_ENBLE
u8 zcl_attr_mulOutput_stateText[ZCL_MULTISTATE_OUTPUT_MAX_LENGTH] = {0};
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_DESCRIPTION_ENBLE
u8 zcl_attr_mulOutput_description[ZCL_MULTISTATE_OUTPUT_MAX_LENGTH] = {0};
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_NUM_OF_STATES_ENBLE
u16 zcl_attr_mulOutput_numOfStates = 0;
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_OUT_OF_SERVICE_ENBLE
bool zcl_attr_mulOutput_outOfService = FALSE;
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_PRESENT_VALUE_ENBLE
u16 zcl_attr_mulOutput_presentValue = 0;
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_PRIORITY_ARRAY_ENBLE
zcl_mulOutput_priorityArray_t zcl_attr_mulOutput_priorityArray[16] = {0};
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_RELIABLILITY_ENBLE
u8 zcl_attr_mulOutput_reliability = 0;
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_RELINQUISH_DEFAULT_ENBLE
u16 zcl_attr_mulOutput_relinquishdDfault = 0;
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_STATUS_FLAGS_ENBLE
u8 zcl_attr_mulOutput_statusFlags = 0;
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_APP_TYPE_ENBLE
u32 zcl_attr_mulOutput_appType = 0;
#endif



const zclAttrInfo_t multistate_output_attrTbl[] =
{
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_STATE_TEXT_ENBLE
	{ ZCL_MULTISTATE_OUTPUT_ATTRID_STATE_TEXT,			ZCL_DATA_TYPE_CHAR_STR,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,	(u8*)zcl_attr_mulOutput_stateText},
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_DESCRIPTION_ENBLE
	{ ZCL_MULTISTATE_OUTPUT_ATTRID_DESCRIPTION,			ZCL_DATA_TYPE_CHAR_STR,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,	(u8*)zcl_attr_mulOutput_description},
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_NUM_OF_STATES_ENBLE
	{ ZCL_MULTISTATE_OUTPUT_ATTRID_NUM_OF_STATES,		ZCL_DATA_TYPE_UINT16,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,	(u8*)&zcl_attr_mulOutput_numOfStates},
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_OUT_OF_SERVICE_ENBLE
	{ ZCL_MULTISTATE_OUTPUT_ATTRID_OUT_OF_SERVICE,		ZCL_DATA_TYPE_BOOLEAN,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_mulOutput_outOfService},
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_PRESENT_VALUE_ENBLE
	{ ZCL_MULTISTATE_OUTPUT_ATTRID_PRESENT_VALUE,		ZCL_DATA_TYPE_UINT16,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_mulOutput_presentValue},
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_PRIORITY_ARRAY_ENBLE
	{ ZCL_MULTISTATE_OUTPUT_ATTRID_PRIORITY_ARRAY,		ZCL_DATA_TYPE_ARRAY,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)zcl_attr_mulOutput_priorityArray},
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_RELIABLILITY_ENBLE
	{ ZCL_MULTISTATE_OUTPUT_ATTRID_RELIABLILITY,		ZCL_DATA_TYPE_ENUM8,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_mulOutput_reliability},
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_RELINQUISH_DEFAULT_ENBLE
	{ ZCL_MULTISTATE_OUTPUT_ATTRID_RELINQUISH_DEFAULT,	ZCL_DATA_TYPE_UINT16,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_mulOutput_relinquishdDfault},
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_STATUS_FLAGS_ENBLE
	{ ZCL_MULTISTATE_OUTPUT_ATTRID_STATUS_FLAGS,		ZCL_DATA_TYPE_BITMAP8,	ACCESS_CONTROL_READ,						(u8*)&zcl_attr_mulOutput_statusFlags},
#endif
#ifdef ZCL_MULTISTATE_OUTPUT_ATTR_APP_TYPE_ENBLE
	{ ZCL_MULTISTATE_OUTPUT_ATTRID_APP_TYPE,			ZCL_DATA_TYPE_UINT32,	ACCESS_CONTROL_READ,						(u8*)&zcl_attr_mulOutput_appType},
#endif
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,				ZCL_DATA_TYPE_UINT16,	ACCESS_CONTROL_READ,						(u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_multistate_output_attrNum = ( sizeof(multistate_output_attrTbl) / sizeof(zclAttrInfo_t) );

#endif	/* ZCL_MULISTATE_OUTPUT */

#endif
