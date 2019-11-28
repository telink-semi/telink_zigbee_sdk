/********************************************************************************************************
 * @file     zcl_devTemperatureCfg.h
 *
 * @brief	 APIs statement for temperature cluster
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
