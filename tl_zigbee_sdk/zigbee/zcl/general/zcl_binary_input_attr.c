/********************************************************************************************************
 * @file	zcl_binary_input_attr.c
 *
 * @brief	This is the source file for zcl_binary_input_attr
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
#ifdef ZCL_BINARY_INPUT

#ifdef ZCL_BINARY_INPUT_ATTR_ACTIVE_TEXT_ENABLE
u8 zcl_attr_binInput_activeText[ZCL_BINARY_INPUT_MAX_LENGTH] = {0};
#endif
#ifdef ZCL_BINARY_INPUT_ATTR_DESCRIPTIONT_ENABLE
u8 zcl_attr_binInput_description[ZCL_BINARY_INPUT_MAX_LENGTH] = {0};
#endif
#ifdef ZCL_BINARY_INPUT_ATTR_INACTIVE_TEXT_ENABLE
u8 zcl_attr_binInput_inactiveText[ZCL_BINARY_INPUT_MAX_LENGTH] = {0};
#endif
#ifdef ZCL_BINARY_INPUT_ATTR_OUT_OF_SERVICE_ENABLE
bool zcl_attr_binInput_outOfService = FALSE;
#endif
#ifdef ZCL_BINARY_INPUT_ATTR_POLARITY_ENABLE
u8 zcl_attr_binInput_polarity = 0;
#endif
#ifdef ZCL_BINARY_INPUT_ATTR_PRESENT_VALUE_ENABLE
bool zcl_attr_binInput_presentValue = 0;
#endif
#ifdef ZCL_BINARY_INPUT_ATTR_RELIABILITY_ENABLE
u8 zcl_attr_binInput_reliability = 0;
#endif
#ifdef ZCL_BINARY_INPUT_ATTR_STATUS_FLAGS_ENABLE
u8 zcl_attr_binInput_statusFlags = 0;
#endif
#ifdef ZCL_BINARY_INPUT_ATTR_APP_TYPE_ENABLE
u32 zcl_attr_binInput_appType = 0;
#endif

const zclAttrInfo_t binary_input_attrTbl[] =
{
#ifdef ZCL_BINARY_INPUT_ATTR_ACTIVE_TEXT_ENABLE
	{ ZCL_BINARY_INPUT_ATTRID_ACTIVE_TEXT,		ZCL_DATA_TYPE_CHAR_STR,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)zcl_attr_binInput_activeText},
#endif
#ifdef ZCL_BINARY_INPUT_ATTR_DESCRIPTIONT_ENABLE
	{ ZCL_BINARY_INPUT_ATTRID_DESCRIPTION,		ZCL_DATA_TYPE_CHAR_STR,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)zcl_attr_binInput_description},
#endif
#ifdef ZCL_BINARY_INPUT_ATTR_INACTIVE_TEXT_ENABLE
	{ ZCL_BINARY_INPUT_ATTRID_INACTIVE_TEXT,	ZCL_DATA_TYPE_CHAR_STR,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)zcl_attr_binInput_inactiveText},
#endif
#ifdef ZCL_BINARY_INPUT_ATTR_OUT_OF_SERVICE_ENABLE
	{ ZCL_BINARY_INPUT_ATTRID_OUT_OF_SERVICE,	ZCL_DATA_TYPE_BOOLEAN,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_binInput_outOfService},
#endif
#ifdef ZZCL_BINARY_INPUT_ATTR_POLARITY_ENABLE
	{ ZCL_BINARY_INPUT_ATTRID_POLARITY,			ZCL_DATA_TYPE_ENUM8,	ACCESS_CONTROL_READ, 						(u8*)&zcl_attr_binInput_polarity},
#endif
#ifdef ZCL_BINARY_INPUT_ATTR_PRESENT_VALUE_ENABLE
	{ ZCL_BINARY_INPUT_ATTRID_PRESENT_VALUE,	ZCL_DATA_TYPE_BOOLEAN,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_binInput_presentValue},
#endif
#ifdef ZCL_BINARY_INPUT_ATTR_RELIABILITY_ENABLE
	{ ZCL_BINARY_INPUT_ATTRID_RELIABILITY,		ZCL_DATA_TYPE_ENUM8,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_binInput_reliability},
#endif
#ifdef ZCL_BINARY_INPUT_ATTR_STATUS_FLAGS_ENABLE
	{ ZCL_BINARY_INPUT_ATTRID_STATUS_FLAGS,		ZCL_DATA_TYPE_BITMAP8,	ACCESS_CONTROL_READ, 						(u8*)&zcl_attr_binInput_statusFlags},
#endif
#ifdef ZCL_BINARY_INPUT_ATTR_APP_TYPE_ENABLE
	{ ZCL_BINARY_INPUT_ATTRID_APP_TYPE,			ZCL_DATA_TYPE_UINT32,	ACCESS_CONTROL_READ, 						(u8*)&zcl_attr_binInput_appType},
#endif
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,		ZCL_DATA_TYPE_UINT16,	ACCESS_CONTROL_READ,  						(u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_binary_input_attrNum = ( sizeof(binary_input_attrTbl) / sizeof(zclAttrInfo_t) );

#endif	/* ZCL_BINARY_INPUT */

#endif
