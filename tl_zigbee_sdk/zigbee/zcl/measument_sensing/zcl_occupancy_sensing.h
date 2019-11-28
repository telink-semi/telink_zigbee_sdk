/********************************************************************************************************
 * @file     zcl_occupancy_sensing.h
 *
 * @brief	 APIs statement for occupancy sensing cluster
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

/** @addtogroup  TELINK_ZIGBEE_STACK TELINK ZigBee Stack
 *  @{
 */

/** @addtogroup  ZCL_Module ZCL
 *  @{
 */
 
/** @addtogroup  ZCL_CLUSTER_OCCUPANCY OCCUPANCY SENSING Cluster
 *  @{
 */

/** @addtogroup  ZCL_OCCUPANCY_Constant OCCUPANCY SENSING Cluster Constants
 *  @{
 */

/** @addtogroup zcl_occupancy_attrId OCCUPANCY SENSING Attribute Ids
 * @{
 */
#define ZCL_ATTRID_OCCUPANCY                          0x0000
#define ZCL_ATTRID_OCCUPANCY_SENSOR_TYPE              0x0001
#define ZCL_ATTRID_PIR_OCCU2UNOCCU_DELAY              0x0010
#define ZCL_ATTRID_PIR_UNOCCU2OCCU_DELAY              0x0011
#define ZCL_ATTRID_PIR_UNOCCU2OCCU_THRES              0x0012
#define ZCL_ATTRID_ULTRA_OCCU2UNOCCU_DELAY            0x0020
#define ZCL_ATTRID_ULTRA_UNOCCU2OCCU_DELAY            0x0021
#define ZCL_ATTRID_ULTRA_UNOCCU2OCCU_THRES            0x0022

/** @} end of group zcl_occupancy_attrId */

/** @} end of group ZCL_OCCUPANCY_Constant */



/** @addtogroup  ZCL_OCCUPANCY_Fountions OCCUPANCY SENSING Cluster APIs
 *  @{
 */

/**
 * @brief       This function register OCCUPANCY SENSING cluster server to ZCL. Only after registered,
 *              occupancy sensing attributes could be used.
 *
 * @param[in]   None
 *
 * @return      Status
 */
status_t zcl_occupancySensing_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);


/** @} end of group ZCL_OCCUPANCY_Fountions */

/** @} end of group ZCL_CLUSTER_OCCUPANCY */

/** @} end of group ZCL_Module */

/** @} end of group TELINK_ZIGBEE_STACK */
