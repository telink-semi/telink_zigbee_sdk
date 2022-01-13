/********************************************************************************************************
 * @file    zcl_occupancy_sensing.h
 *
 * @brief   This is the header file for zcl_occupancy_sensing
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *******************************************************************************************************/

#ifndef ZCL_OCCUPANCY_SENSING_H
#define ZCL_OCCUPANCY_SENSING_H



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
status_t zcl_occupancySensing_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);


/** @} end of group ZCL_OCCUPANCY_Fountions */

/** @} end of group ZCL_CLUSTER_OCCUPANCY */

/** @} end of group ZCL_Module */

/** @} end of group TELINK_ZIGBEE_STACK */

#endif	/* ZCL_OCCUPANCY_SENSING_H */
