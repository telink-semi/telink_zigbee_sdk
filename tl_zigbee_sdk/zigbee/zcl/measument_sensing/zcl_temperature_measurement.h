/********************************************************************************************************
 * @file     zcl_temperature_measurement.h
 *
 * @brief	 APIs statement for temperature measurement cluster
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
#pragma once

#include "tl_common.h"
#include "../../zdo/zb_af.h"
#include "../zcl.h"

/*********************************************************************
 * CONSTANTS
 */

/**
 *  @brief	temperature measurement cluster Attribute IDs
 */
#define ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MEASUREDVALUE     0x0000
#define ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MINMEASUREDVALUE  0x0001
#define ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MAXMEASUREDVALUE  0x0002
#define ZCL_TEMPERATURE_MEASUREMENT_ATTRID_TOLERANCE         0x0003


#if 0
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_MEASUREDVALUE_ENABLE
extern s16 zcl_attr_MeasuredValue;
#endif
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_MINMEASUREDVALUE_ENABLE
extern s16 zcl_attr_MinMeasuredValue;
#endif
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_MAXMEASUREDVALUE_ENABLE
extern s16 zcl_attr_MaxMeasuredValue;
#endif
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_TOLERANCE_ENABLE
extern u16 zcl_attr_Tolerance;
#endif

extern const zclAttrInfo_t temperature_measurement_attrTbl[];
extern const u8 zcl_temperature_measurement_attrNum;
#endif


status_t zcl_temperature_measurement_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

