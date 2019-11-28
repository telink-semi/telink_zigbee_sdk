/********************************************************************************************************
 * @file     zcl_illuminance_measurement.h
 *
 * @brief	 APIs statement for illuminance measurement cluster
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


/* Attribute ID */
#define ZCL_ATTRID_MEASURED_VALUE                         0x0000
#define ZCL_ATTRID_MIN_MEASURED_VALUE                     0x0001
#define ZCL_ATTRID_MAX_MEASURED_VALUE                     0x0002
#define ZCL_ATTRID_TOLERANCE                              0x0003
#define ZCL_ATTRID_LIGHT_SENSOR_TYPE                      0x0004




status_t zcl_illuminanceMeasure_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);
