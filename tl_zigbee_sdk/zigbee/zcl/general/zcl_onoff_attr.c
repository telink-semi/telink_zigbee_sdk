/********************************************************************************************************
 * @file	zcl_onoff_attr.c
 *
 * @brief	This is the source file for zcl_onoff_attr
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
#ifdef ZCL_ON_OFF

zcl_onoff_attr_t g_zclOnOffAttr = {
#ifdef ZCL_ATTR_ON_TIME_ENABLE
		.onTime = 0,
#endif
#ifdef ZCL_ATTR_OFF_WAIT_TIME_ENABLE
		.offWaitTime = 0,
#endif
#ifdef ZCL_ATTR_ONOFF_ENABLE
		.onOff = 0,
#endif
#ifdef ZCL_ATTR_GLOBAL_SCENE_CONTROL_ENABLE
		.globalSceneControl = TRUE,
#endif
#ifdef ZCL_ATTR_START_UP_ONOFF_ENABLE
		.startUpOnOff = ZCL_START_UP_ONOFF_SET_DEFAULT,
#endif
};

/* Attribute record list */
const zclAttrInfo_t onoff_attrTbl[] =
{
#ifdef ZCL_ATTR_ONOFF_ENABLE
	{ ZCL_ATTRID_ONOFF,  				ZCL_DATA_TYPE_BOOLEAN,  ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE,  (u8*)&g_zclOnOffAttr.onOff},
#endif
#ifdef ZCL_ATTR_GLOBAL_SCENE_CONTROL_ENABLE
	{ ZCL_ATTRID_GLOBAL_SCENE_CONTROL, 	ZCL_DATA_TYPE_BOOLEAN, 	ACCESS_CONTROL_READ, 							  (u8*)&g_zclOnOffAttr.globalSceneControl},
#endif
#ifdef ZCL_ATTR_ON_TIME_ENABLE
	{ ZCL_ATTRID_ON_TIME, 				ZCL_DATA_TYPE_UINT16, 	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_zclOnOffAttr.onTime},
#endif
#ifdef ZCL_ATTR_OFF_WAIT_TIME_ENABLE
	{ ZCL_ATTRID_OFF_WAIT_TIME, 		ZCL_DATA_TYPE_UINT16, 	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_zclOnOffAttr.offWaitTime},
#endif
#ifdef ZCL_ATTR_START_UP_ONOFF_ENABLE
	{ ZCL_ATTRID_START_UP_ONOFF, 		ZCL_DATA_TYPE_ENUM8, 	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_zclOnOffAttr.startUpOnOff},
#endif
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_onOff_attrNum = ( sizeof(onoff_attrTbl) / sizeof(zclAttrInfo_t) );

nv_sts_t zcl_onOff_saveAttr(void)
{
	return zcl_attr_onOffParams_save();
}

nv_sts_t zcl_onOff_loadAttr(void)
{
	return zcl_attr_onOffParams_restore();
}

#endif    /* ZCL_ON_OFF */

#endif
