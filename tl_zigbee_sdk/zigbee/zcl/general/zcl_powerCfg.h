/********************************************************************************************************
 * @file	zcl_powerCfg.h
 *
 * @brief	This is the header file for zcl_powerCfg
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





status_t zcl_powerCfg_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);


#endif	/* ZCL_POWER_CFG_H */
