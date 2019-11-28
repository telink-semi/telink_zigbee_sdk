/********************************************************************************************************
 * @file     zcl_devTemperatureCfg_attr.c
 *
 * @brief	 attribute configuration for temperature cluster
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
