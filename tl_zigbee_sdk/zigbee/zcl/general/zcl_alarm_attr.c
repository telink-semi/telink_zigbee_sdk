/********************************************************************************************************
 * @file     zcl_alarm_attr.c
 *
 * @brief	 attribute configuration for alarm cluster
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
#ifdef ZCL_ALARMS
u16 zcl_attr_alarmCount = 0;

/* Attribute record list */
const zclAttrInfo_t alarm_attrTbl[] =
{
#ifdef ZCL_ATTR_ALARM_COUNT_ENABLE
	{ ZCL_ATTRID_ALARM_COUNT,  ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,  (u8*)&zcl_attr_alarmCount},
#endif
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_alarm_attrNum = ( sizeof(alarm_attrTbl) / sizeof(zclAttrInfo_t) );

#endif    /* ZCL_ALARMS */

#endif
