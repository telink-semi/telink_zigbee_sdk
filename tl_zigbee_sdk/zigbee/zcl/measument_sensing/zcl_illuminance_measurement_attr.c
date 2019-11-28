/********************************************************************************************************
 * @file     zcl_illuminance_measurement_attr.c
 *
 * @brief	 attribute configuration for illuminance measurement cluster
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
#ifdef ZCL_ILLUMINANCE_MEASUREMENT

u16 zcl_attr_measuredVal = 0;
u16 zcl_attr_minMeasuredVal = 0;
u16 zcl_attr_maxMeasuredVal = 0;

#ifdef ZCL_ATTR_TOLERANCE_ENABLE
u16  zcl_attr_tolerance = 0;
#endif

#ifdef ZCL_ATTR_LIGHT_SENSOR_TYPE_ENABLE
u8  zcl_attr_lightSensorType = 0;
#endif


/* Attribute record list */
const zclAttrInfo_t illuminanceMeasure_attrTbl[] =
{
	{ ZCL_ATTRID_MEASURED_VALUE,      ZCL_DATA_TYPE_UINT16,     ACCESS_CONTROL_READ,  (u8*)&zcl_attr_measuredVal },
	{ ZCL_ATTRID_MIN_MEASURED_VALUE,  ZCL_DATA_TYPE_UINT16,     ACCESS_CONTROL_READ,  (u8*)&zcl_attr_minMeasuredVal },
	{ ZCL_ATTRID_MAX_MEASURED_VALUE,  ZCL_DATA_TYPE_UINT16,     ACCESS_CONTROL_READ,  (u8*)&zcl_attr_maxMeasuredVal },
#ifdef ZCL_ATTR_TOLERANCE_ENABLE
	{ ZCL_ATTRID_TOLERANCE,           ZCL_DATA_TYPE_UINT16,     ACCESS_CONTROL_READ,  (u8*)&zcl_attr_tolerance },
#endif

#ifdef ZCL_ATTR_LIGHT_SENSOR_TYPE_ENABLE
	{ ZCL_ATTRID_LIGHT_SENSOR_TYPE,   ZCL_DATA_TYPE_ENUM8,      ACCESS_CONTROL_READ,  (u8*)&zcl_attr_lightSensorType },
#endif
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_illuminanceMeasure_attrNum = ( sizeof(illuminanceMeasure_attrTbl) / sizeof(zclAttrInfo_t) );

#endif    /* ZCL_ILLUMINANCE_MEASUREMENT */

#endif
