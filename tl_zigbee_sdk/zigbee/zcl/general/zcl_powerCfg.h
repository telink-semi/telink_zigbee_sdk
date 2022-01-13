/********************************************************************************************************
 * @file    zcl_powerCfg.h
 *
 * @brief   This is the header file for zcl_powerCfg
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

#ifndef ZCL_POWER_CFG_H
#define ZCL_POWER_CFG_H




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





status_t zcl_powerCfg_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);


#endif	/* ZCL_POWER_CFG_H */
