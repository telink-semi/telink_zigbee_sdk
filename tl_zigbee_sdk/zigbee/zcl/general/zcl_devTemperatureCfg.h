/********************************************************************************************************
 * @file	zcl_devTemperatureCfg.h
 *
 * @brief	This is the header file for zcl_devTemperatureCfg
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
#ifndef ZCL_DEV_TEMP_CFG_H
#define ZCL_DEV_TEMP_CFG_H



/** @addtogroup  TELINK_ZIGBEE_STACK TELINK ZigBee Stack
 *  @{
 */

/** @addtogroup  ZCL_Module ZCL
 *  @{
 */
 
/** @addtogroup  ZCL_CLUSTER_DEVICE_TEMPERATURE DEVICE TEMPERATURE Cluster
 *  @{
 */

/** @addtogroup  ZCL_DEVICE_TEMPERATURE_Constant DEVICE TEMPERATURE Cluster Constants
 *  @{
 */

/** @addtogroup zcl_dev_temp_cfg_attrId DEVICE TEMPERATURE Attribute Ids
 * @{
 */
#define ZCL_ATTRID_DEV_TEMP_CURR_TEMP                     0x0000
#define ZCL_ATTRID_DEV_TEMP_MIN_TEMP_EXP                  0x0001
#define ZCL_ATTRID_DEV_TEMP_MAX_TEMP_EXP                  0x0002
#define ZCL_ATTRID_DEV_TEMP_OVER_TEMP_TOTAL               0x0003
#define ZCL_ATTRID_DEV_TEMP_ALARM_MASK                    0x0010
#define ZCL_ATTRID_DEV_TEMP_LOW_THRE                      0x0011
#define ZCL_ATTRID_DEV_TEMP_HIGHT_THRE                    0x0012
#define ZCL_ATTRID_DEV_TEMP_LOW_TRIP                      0x0013
#define ZCL_ATTRID_DEV_TEMP_HIGHT_TRIP                    0x0014

/** @} end of group zcl_dev_temp_cfg_attrId */


/** @} end of group ZCL_DEVICE_TEMPERATURE_Constant */

#if 0
/** @addtogroup  ZCL_DEVICE_TEMPERATUR_Variables DEVICE TEMPERATUR Cluster Variables
 *  @{
 */

/**
 *  @brief  External variable for device temperature Attribute
 */
extern s16 zcl_attr_curTemp;
extern s16 zcl_attr_minTempExp;
extern s16 zcl_attr_maxTempExp;
extern u16 zcl_attr_overTempTotal;
extern u8  zcl_attr_tempAlarmMask;
extern s16 zcl_attr_lowTempThreshold;
extern s16 zcl_attr_highTempThreshold;
extern u24 zcl_attr_lowTripPoint;
extern u24 zcl_attr_highTripPoint;

/**
 *  @brief  External variable for device temperature Attribute table
 */
extern const zclAttrInfo_t dev_temp_cfg_attrTbl[];

extern const u8 zcl_dev_temp_cfg_attrNum;

/** @} end of group ZCL_DEVICE_TEMPERATUR_Variables */
#endif

/** @addtogroup  ZCL_DEVICE_TEMPERATURE_Fountions DEVICE_TEMPERATURE Cluster APIs
 *  @{
 */

/**
 * @brief       This function register DEVICE_TEMPERATURE cluster server to ZCL. Only after registered,
 *              dev_temp_cfg attributes could be used.
 *
 * @param[in]   attrNum  - used attribute number
 * @param[in]   pAttrTbl - pointer to server side attribute table. It is implented in zcl_device_temperature_attr.c
 * @param[in]   cb       - pointer to the App layer command callbacks
 *
 * @return      Status
 */
status_t zcl_devTemperatureCfg_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);



/** @} end of group ZCL_DEVICE_TEMPERATURE_Fountions */

/** @} end of group ZCL_CLUSTER_DEVICE_TEMPERATURE */

/** @} end of group ZCL_Module */

/** @} end of group TELINK_ZIGBEE_STACK */

#endif	/* ZCL_DEV_TEMP_CFG_H */
