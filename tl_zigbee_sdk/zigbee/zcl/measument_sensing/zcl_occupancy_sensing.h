/********************************************************************************************************
 * @file	zcl_occupancy_sensing.h
 *
 * @brief	This is the header file for zcl_occupancy_sensing
 *
 * @author	Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
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
