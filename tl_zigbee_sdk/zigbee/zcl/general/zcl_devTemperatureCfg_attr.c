/********************************************************************************************************
 * @file	zcl_devTemperatureCfg_attr.c
 *
 * @brief	This is the source file for zcl_devTemperatureCfg_attr
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
#ifdef ZCL_DEV_TEMPERATURE_CFG
s16 zcl_attr_curTemp = 0;
s16 zcl_attr_minTempExp = -199;
s16 zcl_attr_maxTempExp = 200;
u16 zcl_attr_overTempTotal = 0;
u8  zcl_attr_tempAlarmMask = 0;
s16 zcl_attr_lowTempThreshold = -199;
s16 zcl_attr_highTempThreshold = 200;
u24 zcl_attr_lowTripPoint = 0x000001;
u24 zcl_attr_highTripPoint = 0x000001;

/* Attribute record list */
const zclAttrInfo_t dev_temp_cfg_attrTbl[] =
{
#ifdef ZCL_ATTR_CUTTRNT_TEMP_ENABLE
	{ ZCL_ATTRID_DEV_TEMP_CURR_TEMP,       ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ, (u8*)&zcl_attr_curTemp},
#endif
#ifdef ZCL_ATTR_MIN_TEMP_EXP_ENABLE
	{ ZCL_ATTRID_DEV_TEMP_MIN_TEMP_EXP,    ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ, (u8*)&zcl_attr_minTempExp},
#endif
#ifdef ZCL_ATTR_MAX_TEMP_EXP_ENABLE
	{ ZCL_ATTRID_DEV_TEMP_MAX_TEMP_EXP,    ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ, (u8*)&zcl_attr_maxTempExp},
#endif
#ifdef ZCL_ATTR_OVER_TEMP_TOTAL_ENABLE
	{ ZCL_ATTRID_DEV_TEMP_OVER_TEMP_TOTAL, ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ, (u8*)&zcl_attr_overTempTotal},
#endif
#ifdef ZCL_ATTR_OVER_DEV_TEMP_ALARM_MASK_ENABLE
	{ ZCL_ATTRID_DEV_TEMP_ALARM_MASK,      ZCL_DATA_TYPE_BITMAP8,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_tempAlarmMask},
#endif
#ifdef ZCL_ATTR_OVER_LOW_TEMP_THRESHOLD_ENABLE
	{ ZCL_ATTRID_DEV_TEMP_LOW_THRE,        ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_lowTempThreshold},
#endif
#ifdef ZCL_ATTR_OVER_HIGH_TEMP_THERSHOLD_ENABLE
	{ ZCL_ATTRID_DEV_TEMP_HIGHT_THRE,      ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_highTempThreshold},
#endif
#ifdef ZCL_ATTR_OVER_LOW_TEMP_TRIP_ENABLE
	{ ZCL_ATTRID_DEV_TEMP_LOW_TRIP,        ZCL_DATA_TYPE_UINT24,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_lowTripPoint},
#endif
#ifdef ZCL_ATTR_OVER_HIGH_TEMP_TRIP_ENABLE
	{ ZCL_ATTRID_DEV_TEMP_HIGHT_TRIP,      ZCL_DATA_TYPE_UINT24,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_highTripPoint},
#endif
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_dev_temp_cfg_attrNum = ( sizeof(dev_temp_cfg_attrTbl) / sizeof(zclAttrInfo_t) );
 
#endif    /* ZCL_DEV_TEMPERATURE_CFG */

#endif
