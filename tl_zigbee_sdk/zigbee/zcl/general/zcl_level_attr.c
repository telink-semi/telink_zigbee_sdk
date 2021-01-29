/********************************************************************************************************
 * @file	zcl_level_attr.c
 *
 * @brief	This is the source file for zcl_level_attr
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
#ifdef ZCL_LEVEL_CTRL

zcl_levelAttr_t zcl_attr_level = {
#ifdef ZCL_ATTR_CURRENT_LEVEL_ENABLE
		.curLevel 			= ZCL_LEVEL_ATTR_CURRENT_LEVEL_DEFAULT,	//default level
#endif
#ifdef ZCL_ATTR_REMAINING_TIME_ENABLE
		.remainingTime 		= 0,
#endif
#ifdef ZCL_ATTR_ONOFF_TRANS_TIME_ENABLE
		.onOffTransTime		= 0,
#endif
#ifdef ZCL_ATTR_ON_LEVEL_ENABLE
		.onLevel  			= ZCL_LEVEL_ATTR_ON_LEVEL_NO_EFFECT,	//no effect
#endif
#ifdef ZCL_ATTR_ON_TRANS_ENABLE
		.onTransitionTime 	= 0xFFFF,
#endif
#ifdef ZCL_ATTR_OFF_TRANS_ENABLE
		.offTransitionTime 	= 0xFFFF,
#endif
#ifdef ZCL_ATTR_DEFAULT_MOVE_RATE_ENABLE
		.defaultMoveRate	= 0,
#endif
#ifdef ZCL_ATTR_OPTIONS_ENABLE
		.options			= 0,
#endif
#ifdef ZCL_ATTR_START_UP_CURRENT_LEVEL_ENABLE
		.startUpCurrentLevel = ZCL_START_UP_CURRENT_LEVEL_TO_PREVIOUS,
#endif
};

/* Attribute record list */
const zclAttrInfo_t level_attrTbl[] =
{
#ifdef ZCL_ATTR_CURRENT_LEVEL_ENABLE
	{ ZCL_ATTRID_LEVEL_CURRENT_LEVEL,  ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ|ACCESS_CONTROL_REPORTABLE,  (u8*)&zcl_attr_level.curLevel },
#endif

#ifdef ZCL_ATTR_REMAINING_TIME_ENABLE
	{ ZCL_ATTRID_LEVEL_REMAINING_TIME, ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,  (u8*)&zcl_attr_level.remainingTime },
#endif

#ifdef ZCL_ATTR_ONOFF_TRANS_TIME_ENABLE
	{ ZCL_ATTRID_LEVEL_ON_OFF_TRANSITION_TIME,  ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_level.onOffTransTime },
#endif

#ifdef ZCL_ATTR_ON_LEVEL_ENABLE
	{ ZCL_ATTRID_LEVEL_ON_LEVEL,       ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_level.onLevel },
#endif

#ifdef ZCL_ATTR_ON_TRANS_ENABLE
	{ ZCL_ATTRID_LEVEL_ON_TRANSITION_TIME,       ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_level.onTransitionTime },
#endif

#ifdef ZCL_ATTR_OFF_TRANS_ENABLE
	{ ZCL_ATTRID_LEVEL_OFF_TRANSITION_TIME,       ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_level.offTransitionTime },
#endif

#ifdef ZCL_ATTR_DEFAULT_MOVE_RATE_ENABLE
	{ ZCL_ATTRID_LEVEL_DEFAULT_MOVE_RATE,       ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_level.defaultMoveRate },
#endif

#ifdef ZCL_ATTR_OPTIONS_ENABLE
	{ ZCL_ATTRID_LEVEL_OPTIONS,       ZCL_DATA_TYPE_BITMAP8,   ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_level.options },
#endif

#ifdef ZCL_ATTR_START_UP_CURRENT_LEVEL_ENABLE
	{ ZCL_ATTRID_LEVEL_START_UP_CURRENT_LEVEL,  ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_level.startUpCurrentLevel },
#endif

	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_level_attrNum = ( sizeof(level_attrTbl) / sizeof(zclAttrInfo_t) );

nv_sts_t zcl_level_saveAttr(void)
{
	return zcl_attr_levelParams_save();
}

nv_sts_t zcl_level_loadAttr(void)
{
	return zcl_attr_levelParams_restore();
}

#endif    /* ZCL_LEVEL_CTRL */

#endif
