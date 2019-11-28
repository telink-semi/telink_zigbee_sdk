/********************************************************************************************************
 * @file     zcl_timer_attr.c
 *
 * @brief	 attribute configuration for timer cluster
 *
 * @author
 * @date     June. 10, 2017
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
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
