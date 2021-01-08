/********************************************************************************************************
 * @file	zcl_time_attr.c
 *
 * @brief	This is the source file for zcl_time_attr
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
#ifdef ZCL_TIME
u32 zcl_attr_time = 0xffffffff;
u8 zcl_attr_timeStatus = 0;
s32 zcl_attr_timezone = 0;
u32 zcl_attr_dstStart = 0;
u32 zcl_attr_dstEnd = 0;
s32 zcl_attr_dstShift = 0;
u32 zcl_attr_standardTime = 0;
u32 zcl_attr_localTime = 0xffffffff;
u32 zcl_attr_validUntilTime = 0xffffffff;

/* Attribute record list */
const zclAttrInfo_t time_attrTbl[] =
{
#ifdef ZCL_ATTR_TIME_ENABLE
	{ ZCL_ATTRID_TIME,  ZCL_DATA_TYPE_UTC,   ACCESS_CONTROL_READ,  (u8*)&zcl_attr_time},
#endif
#ifdef ZCL_ATTR_TIME_STATUS_ENABLE
	{ ZCL_ATTRID_TIME_STATUS,  ZCL_DATA_TYPE_BITMAP8,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_timeStatus},
#endif
#ifdef ZCL_ATTR_TIMEZONE_ENABLE
	{ ZCL_ATTRID_TIMEZONE,  ZCL_DATA_TYPE_INT32,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_timezone},
#endif
#ifdef ZCL_ATTR_DST_START_ENABLE
	{ ZCL_ATTRID_DST_START,  ZCL_DATA_TYPE_UINT32,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_dstStart},
#endif
#ifdef ZCL_ATTR_DST_END_ENABLE
	{ ZCL_ATTRID_DST_END,  ZCL_DATA_TYPE_UINT32,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_dstEnd},
#endif
#ifdef ZCL_ATTR_DST_SHIFT_ENABLE
	{ ZCL_ATTRID_DST_SHIFT,  ZCL_DATA_TYPE_UINT32,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_dstShift},
#endif
#ifdef ZCL_ATTR_STANDARD_TIME_ENABLE
	{ ZCL_ATTRID_STANDARD_TIME,  ZCL_DATA_TYPE_UINT32,   ACCESS_CONTROL_READ,  (u8*)&zcl_attr_standardTime},
#endif
#ifdef ZCL_ATTR_LOCAL_TIME_ENABLE
	{ ZCL_ATTRID_LOCAL_TIME,  ZCL_DATA_TYPE_UINT32,   ACCESS_CONTROL_READ,  (u8*)&zcl_attr_localTime},
#endif
#ifdef ZCL_ATTR_LAST_SET_TIME_ENABLE
	{ ZCL_ATTRID_LAST_SET_TIME,  ZCL_DATA_TYPE_UTC,   ACCESS_CONTROL_READ,  (u8*)&zcl_attr_localTime},
#endif
#ifdef ZCL_ATTR_VALID_UNTIL_TIME_ENABLE
	{ ZCL_ATTRID_VALID_UNTIL_TIME,  ZCL_DATA_TYPE_UTC,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_validUntilTime},
#endif
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_time_attrNum = ( sizeof(time_attrTbl) / sizeof(zclAttrInfo_t) );

#endif    /* ZCL_TIME */
#endif
