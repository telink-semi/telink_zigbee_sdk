/********************************************************************************************************
 * @file     zcl_powerCfg.h
 *
 * @brief	 APIs statement for power cluster
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


#define ZCL_ATTRID_MAINS_VOLTAGE						0x0000
#define ZCL_ATTRID_MAINS_FREQUENCY						0x0001
#define ZCL_ATTRID_MAINS_ALARM_MASK						0x0010
#define ZCL_ATTRID_MAINS_VOLTAGE_MIN_THRESHOLD			0x0011
#define ZCL_ATTRID_MAINS_VOLTAGE_MAX_THRESHOLD			0x0012
#define ZCL_ATTRID_MAINS_VOLTAGE_DWELL_TRIP_POINT		0x0013
#define ZCL_ATTRID_BATTERY_VOLTAGE						0x0020
#define ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING			0x0021
#define ZCL_ATTRID_BATTERY_MANUFACTURER					0x0030
#define ZCL_ATTRID_BATTERY_SIZE							0x0031
#define ZCL_ATTRID_BATTERY_A_HR_RATING					0x0032
#define ZCL_ATTRID_BATTERY_QUANTITY						0x0033
#define ZCL_ATTRID_BATTERY_RATED_VOLTAGE				0x0034
#define ZCL_ATTRID_BATTERY_ALARM_MASK					0x0035
#define ZCL_ATTRID_BATTERY_VOLTAGE_MIN_THRESHOLD		0x0036
#define ZCL_ATTRID_BATTERY_VOLTAGE_THRESHOLD_1			0x0037
#define ZCL_ATTRID_BATTERY_VOLTAGE_THRESHOLD_2			0x0038
#define ZCL_ATTRID_BATTERY_VOLTAGE_THRESHOLD_3			0x0039
#define ZCL_ATTRID_BATTERY_PERCENTAGE_MIN_THRESHOLD 	0x003A
#define ZCL_ATTRID_BATTERY_PERCENTAGE_THRESHOLD_1		0x003B
#define ZCL_ATTRID_BATTERY_PERCENTAGE_THRESHOLD_2		0x003C
#define ZCL_ATTRID_BATTERY_PERCENTAGE_THRESHOLD_3		0x003D
#define ZCL_ATTRID_BATTERY_ALARM_STATE					0x003E





status_t zcl_powerCfg_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);


