/********************************************************************************************************
 * @file    zcl_devTemperatureCfg.h
 *
 * @brief   This is the header file for zcl_devTemperatureCfg
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
#define ZCL_ATTRID_DEV_TEMP_LOW_THRES                     0x0011
#define ZCL_ATTRID_DEV_TEMP_HIGH_THRES					  0x0012
#define ZCL_ATTRID_DEV_TEMP_LOW_TRIP                      0x0013
#define ZCL_ATTRID_DEV_TEMP_HIGH_TRIP                     0x0014

/** @} end of group zcl_dev_temp_cfg_attrId */


/** @} end of group ZCL_DEVICE_TEMPERATURE_Constant */

#if 0
/** @addtogroup  ZCL_DEVICE_TEMPERATURE_Variables DEVICE TEMPERATURE Cluster Variables
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

/** @} end of group ZCL_DEVICE_TEMPERATURE_Variables */
#endif

/** @addtogroup  ZCL_DEVICE_TEMPERATURE_Fountions DEVICE_TEMPERATURE Cluster APIs
 *  @{
 */

/**
 * @brief       This function register DEVICE_TEMPERATURE cluster server to ZCL. Only after registered,
 *              dev_temp_cfg attributes could be used.
 *
 * @param[in]   attrNum  - used attribute number
 * @param[in]   pAttrTbl - pointer to server side attribute table. It is implemented in zcl_device_temperature_attr.c
 * @param[in]   cb       - pointer to the App layer command callbacks
 *
 * @return      Status
 */
status_t zcl_devTemperatureCfg_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);



/** @} end of group ZCL_DEVICE_TEMPERATURE_Fountions */

/** @} end of group ZCL_CLUSTER_DEVICE_TEMPERATURE */

/** @} end of group ZCL_Module */

/** @} end of group TELINK_ZIGBEE_STACK */

#endif	/* ZCL_DEV_TEMP_CFG_H */
