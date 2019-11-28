/********************************************************************************************************
 * @file     zcl_level_attr.c
 *
 * @brief	 attribute configuration for level cluster
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
	{ ZCL_ATTRID_LEVLE_OPTIONS,       ZCL_DATA_TYPE_BITMAP8,   ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_level.options },
#endif

#ifdef ZCL_ATTR_START_UP_CURRENT_LEVEL_ENABLE
	{ ZCL_ATTRID_LEVLE_START_UP_CURRENT_LEVEL,  ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_level.startUpCurrentLevel },
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
