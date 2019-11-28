/********************************************************************************************************
 * @file     zcl_temperature_measurement_attr.c
 *
 * @brief	 attribute configuration for temperature measurement cluster
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
#ifdef ZCL_TEMPERATURE_MEASUREMENT

#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_MEASUREDVALUE_ENABLE
s16 zcl_attr_MeasuredValue = 0xFFFF;
#endif
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_MINMEASUREDVALUE_ENABLE
s16 zcl_attr_MinMeasuredValue = 0x8000;
#endif
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_MAXMEASUREDVALUE_ENABLE
s16 zcl_attr_MaxMeasuredValue = 0x8000;
#endif
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_TOLERANCE_ENABLE
u16 zcl_attr_Tolerance = 0;
#endif

const zclAttrInfo_t temperature_measurement_attrTbl[] =
{
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_MEASUREDVALUE_ENABLE
	{ ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MEASUREDVALUE,       ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zcl_attr_MeasuredValue},
#endif
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_MINMEASUREDVALUE_ENABLE
	{ ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MINMEASUREDVALUE,    ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ, (u8*)&zcl_attr_MinMeasuredValue},
#endif
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_MAXMEASUREDVALUE_ENABLE
	{ ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MAXMEASUREDVALUE,    ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ, (u8*)&zcl_attr_MaxMeasuredValue},
#endif
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_TOLERANCE_ENABLE
	{ ZCL_TEMPERATURE_MEASUREMENT_ATTRID_TOLERANCE,           ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ, (u8*)&zcl_attr_Tolerance},
#endif
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 					  ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_temperature_measurement_attrNum = ( sizeof(temperature_measurement_attrTbl) / sizeof(zclAttrInfo_t) );

#endif	/* ZCL_TEMPERATURE_MEASUREMENT */

#endif
