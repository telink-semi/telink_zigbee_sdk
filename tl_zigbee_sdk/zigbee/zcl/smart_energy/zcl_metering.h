/********************************************************************************************************
 * @file     zcl_metering.h
 *
 * @brief	 APIs statement for metering cluster
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
/**
 *  @brief	Attribute Sets
 */
#define READING_INFORMATION_SET									0x00
#define TOU_INFORMATION_SET										0x01
#define METER_STATUS											0x02
#define FORMATTING												0x03
#define HISTORICAL_CONSUMPTION									0x04
#define LOAD_PROFILE_CONFIGURATION								0x05
#define SUPPLY_LIMIT											0x06
#define BLOCK_INFORMATION										0x07
#define ALARMS													0x08

/**
 *  @brief	Metering cluster Attribute IDs
 */
//Reading Information Set
#define ZCL_ATTRID_CURRENT_SUMMATION_DELIVERD	            	0x0000
#define ZCL_ATTRID_CURRENT_SUMMATION_RECEIVED               	0x0001
#define ZCL_ATTRID_CURRENT_MAX_DEMAND_DELIVERD              	0x0002
#define ZCL_ATTRID_CURRENT_MAX_DEMAND_RECEIVED              	0x0003
#define ZCL_ATTRID_DFT_SUMMATION	                        	0x0004
#define ZCL_ATTRID_DAILY_FREEZE_TIME							0x0005
#define ZCL_ATTRID_METERING_POWER_FACTOR						0x0006
#define ZCL_ATTRID_READING_SNAP_SHORT_TIME						0x0007
#define ZCL_ATTRID_CURRENT_MAX_DEMAND_DELIVERD_TIME				0x0008
#define ZCL_ATTRID_CURRENT_MAX_DEMAND_RECEIVED_TIME				0x0009
#define ZCL_ATTRID_DEFAULT_UPDATE_PERIOD						0x000A
#define ZCL_ATTRID_FAST_POLL_UPDATE_PERIOD						0x000B
#define ZCL_ATTRID_CURRENT_BLOCK_PERIOD_CONSUMPTION_DELIVERD	0x000C
#define ZCL_ATTRID_DAILY_CONSUMPTION_TARGET						0x000D
#define ZCL_ATTRID_CURRENT_BLOCK								0x000E
#define ZCL_ATTRID_PROFILE_INTERVAL_PERIOD						0x000F
#define ZCL_ATTRID_INTERVAL_READ_REPORTING_PERIOD				0x0010
#define ZCL_ATTRID_PRESENT_READING_TIME							0x0011
#define ZCL_ATTRID_VOLUME_PER_REPORT							0x0012
#define ZCL_ATTRID_FLOW_RESTRICTION								0x0013
#define ZCL_ATTRID_SUPPLY_STATUS								0x0014
#define ZCL_ATTRID_CURRENT_INLET_ENERGY_CARRIER_SUMMATION		0x0015
#define ZCL_ATTRID_CURRENT_OUTLET_ENERGY_CARRIER_SUMMATION		0x0016
#define ZCL_ATTRID_INLET_TEMPERATURE							0x0017
#define ZCL_ATTRID_OUTLET_TEMPERATURE							0x0018
#define ZCL_ATTRID_CONTROL_TEMPERATURE							0x0019
#define ZCL_ATTRID_CURRENT_INLET_ENERGY_CARRIER_DEMAND			0x001A
#define ZCL_ATTRID_CURRENT_OUTLET_ENERGY_CARRIER_DEMAND			0x001B
#define ZCL_ATTRID_PREVIOUS_BLOCK_PERIOD_CONSUMPTION_DELIVERD	0x001C
//Summation TOU Information Set
#define ZCL_ATTRID_CURRENT_TIER_1_SUMMATION_DELIVERD			0x0100
#define ZCL_ATTRID_CURRENT_TIER_1_SUMMATION_RECEIVED			0x0101
#define ZCL_ATTRID_CURRENT_TIER_2_SUMMATION_DELIVERD			0x0102
#define ZCL_ATTRID_CURRENT_TIER_2_SUMMATION_RECEIVED			0x0103
#define ZCL_ATTRID_CURRENT_TIER_3_SUMMATION_DELIVERD			0x0104
#define ZCL_ATTRID_CURRENT_TIER_3_SUMMATION_RECEIVED			0x0105
#define ZCL_ATTRID_CURRENT_TIER_4_SUMMATION_DELIVERD			0x0106
#define ZCL_ATTRID_CURRENT_TIER_4_SUMMATION_RECEIVED			0x0107
#define ZCL_ATTRID_CURRENT_TIER_5_SUMMATION_DELIVERD			0x0108
#define ZCL_ATTRID_CURRENT_TIER_5_SUMMATION_RECEIVED			0x0109
#define ZCL_ATTRID_CURRENT_TIER_6_SUMMATION_DELIVERD			0x010A
#define ZCL_ATTRID_CURRENT_TIER_6_SUMMATION_RECEIVED			0x010B
#define ZCL_ATTRID_CURRENT_TIER_7_SUMMATION_DELIVERD			0x010C
#define ZCL_ATTRID_CURRENT_TIER_7_SUMMATION_RECEIVED			0x010D
#define ZCL_ATTRID_CURRENT_TIER_8_SUMMATION_DELIVERD			0x010E
#define ZCL_ATTRID_CURRENT_TIER_8_SUMMATION_RECEIVED			0x010F
#define ZCL_ATTRID_CURRENT_TIER_9_SUMMATION_DELIVERD			0x0110
#define ZCL_ATTRID_CURRENT_TIER_9_SUMMATION_RECEIVED			0x0111
#define ZCL_ATTRID_CURRENT_TIER_10_SUMMATION_DELIVERD			0x0112
#define ZCL_ATTRID_CURRENT_TIER_10_SUMMATION_RECEIVED			0x0113
#define ZCL_ATTRID_CURRENT_TIER_11_SUMMATION_DELIVERD			0x0114
#define ZCL_ATTRID_CURRENT_TIER_11_SUMMATION_RECEIVED			0x0115
#define ZCL_ATTRID_CURRENT_TIER_12_SUMMATION_DELIVERD			0x0116
#define ZCL_ATTRID_CURRENT_TIER_12_SUMMATION_RECEIVED			0x0117
#define ZCL_ATTRID_CURRENT_TIER_13_SUMMATION_DELIVERD			0x0118
#define ZCL_ATTRID_CURRENT_TIER_13_SUMMATION_RECEIVED			0x0119
#define ZCL_ATTRID_CURRENT_TIER_14_SUMMATION_DELIVERD			0x011A
#define ZCL_ATTRID_CURRENT_TIER_14_SUMMATION_RECEIVED			0x011B
#define ZCL_ATTRID_CURRENT_TIER_15_SUMMATION_DELIVERD			0x011C
#define ZCL_ATTRID_CURRENT_TIER_15_SUMMATION_RECEIVED			0x011D
//Meter Status Attribute Set
#define ZCL_ATTRID_STATUS										0x0200
#define ZCL_ATTRID_REMAINING_BATTERY_LIFE						0x0201
#define ZCL_ATTRID_HOURS_IN_OPERATION							0x0202
#define ZCL_ATTRID_HOURS_IN_FAULT								0x0203
//Formatting
#define ZCL_ATTRID_UNIT_OF_MEASURE								0x0300
#define ZCL_ATTRID_MULTIPLIER									0x0301
#define ZCL_ATTRID_DIVISOR										0x0302
#define ZCL_ATTRID_SUMMATION_FORMATTING							0x0303
#define ZCL_ATTRID_DEMAND_FORMATTING							0x0304
#define ZCL_ATTRID_HISTORICAL_CONSUMPTION_FORMATTING			0x0305
#define ZCL_ATTRID_METERING_DEVICE_TYPE							0x0306
#define ZCL_ATTRID_SITE_ID										0x0307
#define ZCL_ATTRID_METER_SERIAL_NUMBER							0x0308
#define ZCL_ATTRID_ENERGY_CARRIER_UNIT_OF_MEASURE				0x0309
#define ZCL_ATTRID_ENERGY_CARRIER_SUMMATION_FORMATTING			0x030A
#define ZCL_ATTRID_ENERGY_CARRIER_DEMAND_FORMATTING				0x030B
#define ZCL_ATTRID_TEMPERATURE_UNIT_OF_MEASURE					0x030C
#define ZCL_ATTRID_TEMPERATURE_FORMATTING						0x030D
//Historical Consumption Attribute
#define ZCL_ATTRID_INSTANTANEOUS_DEMAND							0x0400
#define ZCL_ATTRID_CURRENT_DAY_CONSUMPTION_DELIVERD				0x0401
#define ZCL_ATTRID_CURRENT_DAY_CONSUMPTION_RECEIVED				0x0402
#define ZCL_ATTRID_PREVIOUS_DAY_CONSUMPTION_DELIVERD			0x0403
#define ZCL_ATTRID_PREVIOUS_DAY_CONSUMPTION_RECEIVED			0x0404
#define ZCL_ATTRID_CUR_PARTITAL_PROFILE_INT_START_TIME_DELIVERD	0x0405
#define ZCL_ATTRID_CUR_PARTITAL_PROFILE_INT_START_TIME_REVEIVED	0x0406
#define ZCL_ATTRID_CUR_PARTITAL_PROFILE_INT_VALUE_DELIVERD		0x0407
#define ZCL_ATTRID_CUR_PARTITAL_PROFILE_INT_VALUE_REVEIVED		0x0408
#define ZCL_ATTRID_CURRENT_DAY_MAX_PRESSURE						0x0409
#define ZCL_ATTRID_CURRENT_DAY_MIN_PRESSURE						0x040A
#define ZCL_ATTRID_PREVIOUS_DAY_MAX_PRESSURE					0x040B
#define ZCL_ATTRID_PREVIOUS_DAY_MIN_PRESSURE					0x040C
#define ZCL_ATTRID_CURRENT_DAY_MAX_DEMAND						0x040D
#define ZCL_ATTRID_PREVIOUS_DAY_MAX_DEMAND						0x040E
#define ZCL_ATTRID_CURRENT_MONTH_MAX_DEMAND						0x040F
#define ZCL_ATTRID_CURRENT_YEAR_MAX_DEMAND						0x0410
#define ZCL_ATTRID_CURRENT_DAY_MAX_ENERGY_CARRIER_DEMAND		0x0411
#define ZCL_ATTRID_PREVIOUS_DAY_MAX_ENERGY_CARRIER_DEMAND		0x0412
#define ZCL_ATTRID_CURRENT_MONTH_MAX_ENERGY_CARRIER_DEMAND		0x0413
#define ZCL_ATTRID_CURRENT_MONTH_MIN_ENERGY_CARRIER_DEMAND		0x0414
#define ZCL_ATTRID_CURRENT_YEAR_MAX_ENERGY_CARRIER_DEMAND		0x0415
#define ZCL_ATTRID_CURRENT_YEAR_MIN_ENERGY_CARRIER_DEMAND		0x0416
//Load Profile Configuration
#define ZCL_ATTRID_MAX_NUMBER_OF_PERIODS_DELIVERD				0x0500
//Supply Limit Attributes
#define ZCL_ATTRID_CURRENT_DEMAND_DELIVERD						0x0600
#define ZCL_ATTRID_DEMAND_LIMIT									0x0601
#define ZCL_ATTRID_DEMAND_INTEGRATION_PERIOD					0x0602
#define ZCL_ATTRID_NUMBER_OF_DEMAND_SUB_INTERVALS				0x0603
//Block Information Set
#define ZCL_ATTRID_CURRENT_NO_TIER_BLOCK_1_SUMMATION_DELIVERD	0x0700
#define ZCL_ATTRID_CURRENT_TIER_1_BLOCK_1_SUMMATION_DELIVERD	0x0710
#define ZCL_ATTRID_CURRENT_TIER_2_BLOCK_1_SUMMATION_DELIVERD	0x0720
#define ZCL_ATTRID_CURRENT_TIER_3_BLOCK_1_SUMMATION_DELIVERD	0x0730
#define ZCL_ATTRID_CURRENT_TIER_4_BLOCK_1_SUMMATION_DELIVERD	0x0740
#define ZCL_ATTRID_CURRENT_TIER_5_BLOCK_1_SUMMATION_DELIVERD	0x0750
#define ZCL_ATTRID_CURRENT_TIER_6_BLOCK_1_SUMMATION_DELIVERD	0x0760
#define ZCL_ATTRID_CURRENT_TIER_7_BLOCK_1_SUMMATION_DELIVERD	0x0770
#define ZCL_ATTRID_CURRENT_TIER_8_BLOCK_1_SUMMATION_DELIVERD	0x0780
#define ZCL_ATTRID_CURRENT_TIER_9_BLOCK_1_SUMMATION_DELIVERD	0x0790
#define ZCL_ATTRID_CURRENT_TIER_10_BLOCK_1_SUMMATION_DELIVERD	0x07A0
#define ZCL_ATTRID_CURRENT_TIER_11_BLOCK_1_SUMMATION_DELIVERD	0x07B0
#define ZCL_ATTRID_CURRENT_TIER_12_BLOCK_1_SUMMATION_DELIVERD	0x07C0
#define ZCL_ATTRID_CURRENT_TIER_13_BLOCK_1_SUMMATION_DELIVERD	0x07D0
#define ZCL_ATTRID_CURRENT_TIER_14_BLOCK_1_SUMMATION_DELIVERD	0x07E0
#define ZCL_ATTRID_CURRENT_TIER_15_BLOCK_1_SUMMATION_DELIVERD	0x07F0
//Alarms Set
#define ZCL_ATTRID_GENERIC_ALARM_MASK							0x0800
#define ZCL_ATTRID_ELECTRICITY_ALARM_MASK						0x0801
#define ZCL_ATTRID_GENERIC_FLOW_PRESSURE_ALARM_MASK				0x0802
#define ZCL_ATTRID_WATER_SPEC_ALARM_MASK						0x0803
#define ZCL_ATTRID_HEAT_AND_COOLING_SPEC_ALARM_MASK				0x0804
#define ZCL_ATTRID_GAS_SPEC_ALARM_MASK							0x0805

/**
 *  @brief	metering command IDs
 */
//Server commands
#define ZCL_CMD_GET_PROFILE_RESPONSE	                        0x00
#define ZCL_CMD_REQUEST_MIRROR									0x01
#define ZCL_CMD_REMOVE_MIRROR									0x02
#define ZCL_CMD_REQUEST_FAST_POLL_MODE_RESPONSE					0x03
//client commands
#define ZCL_CMD_GET_PROFILE										0x00
#define ZCL_CMD_REQUEST_MIRROR_RESPONSE							0x01
#define ZCL_CMD_MIRROR_REMOVED									0x02
#define ZCL_CMD_REQUEST_FAST_POLL_MODE							0x03


/**
 *  @brief	Format of the Get Profile command
 */
typedef struct
{
	UTCTime	endTime;
	u8	intervalChannel;
	u8	numberOfPeriods;
}zcl_metering_getProfileCmd_t;

/**
 *  @brief	Format of the Request Mirror Response command
 */
typedef struct
{
	u16	endPointId;
}zcl_metering_requestMirrorRspCmd_t;

/**
 *  @brief	Format of the Mirror Removed command
 */
typedef struct
{
	u16	removedEndPointId;
}zcl_metering_mirrorRemovedCmd_t;

/**
 *  @brief	Format of the Request Fast Poll Mode command
 */
typedef struct
{
	u8	fastPollUpdatePeriod;
	u8	duration;
}zcl_metering_requestFastPollModeCmd_t;

/**
 *  @brief	Format of the Get Profile Response command
 */
typedef struct
{
	UTCTime	endTime;
	u8	status;
	u8	profileIntervalPeriod;
	u8	numberOfPeriodsDelived;
	u8	*intervals;
}zcl_metering_getProfileRspCmd_t;

/**
 *  @brief	Format of the Request Fast Poll Mode Response command
 */
typedef struct
{
	UTCTime	fastPollModeEndTime;
	u8	appliedUpdatePeriod;
}zcl_metering_requestFastPollModeRspCmd_t;

#if 0
typedef status_t (*zcl_metering_getProfileCb_t)(apsdeDataInd_t *pInd, zcl_metering_getProfileCmd_t *pCmd);
typedef status_t (*zcl_metering_requestMirrorRspCb_t)(apsdeDataInd_t *pInd, zcl_metering_requestMirrorRspCmd_t *pCmd);
typedef status_t (*zcl_metering_mirrorRemovedCb_t)(apsdeDataInd_t *pInd, zcl_metering_mirrorRemovedCmd_t *pCmd);
typedef status_t (*zcl_metering_requestFastPollModeCb_t)(apsdeDataInd_t *pInd, zcl_metering_requestFastPollModeCmd_t *pCmd);
typedef status_t (*zcl_metering_getProfileRspCb_t)(apsdeDataInd_t *pInd, zcl_metering_getProfileRspCmd_t *pCmd);
typedef status_t (*zcl_metering_requestMirrorCb_t)(apsdeDataInd_t *pInd);
typedef status_t (*zcl_metering_removeMirrorCb_t)(apsdeDataInd_t *pInd);
typedef status_t (*zcl_metering_requestFastPollModeRspCb_t)(apsdeDataInd_t *pInd, zcl_metering_requestFastPollModeRspCmd_t *pCmd);

/**
 *  @brief  Structure definition for commands callBack functions in metering cluster
 */
typedef struct
{
	zcl_metering_getProfileCb_t                     		getProfileCbFunc;
	zcl_metering_requestMirrorRspCb_t                     	requestMirrorRspCbFunc;
	zcl_metering_mirrorRemovedCb_t							mirrorRemovedCbFunc;
	zcl_metering_requestFastPollModeCb_t                    requestFastPollModeCbFunc;
	zcl_metering_getProfileRspCb_t							getProfileRspCbFunc;
	zcl_metering_requestMirrorCb_t							requestMirrorCbFunc;
	zcl_metering_removeMirrorCb_t							removeMirrorCbFunc;
	zcl_metering_requestFastPollModeRspCb_t					requestFastPollModeRspCbFunc;
}zcl_metering_AppCallbacks_t;



extern const zclAttrInfo_t metering_attrTbl[];
extern const u8 zcl_metering_attrNum;
#endif


status_t zcl_metering_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

status_t zcl_metering_getProfile(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_metering_getProfileCmd_t *pReq);
#define zcl_metering_getProfileCmd(a,b,c,d)	(zcl_metering_getProfile((a), (b), (c), ZCL_SEQ_NUM, (d)))

void zcl_metering_requestMirrorRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_metering_requestMirrorRspCmd_t *pRsp);
#define zcl_metering_requestMirrorRspCmd(a,b,c,d)	(zcl_metering_requestMirrorRsp((a), (b), (c), ZCL_SEQ_NUM, (d)))

void zcl_metering_mirrorRemoved(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_metering_mirrorRemovedCmd_t *pRsp);
#define zcl_metering_mirrorRemovedCmd(a,b,c,d)	(zcl_metering_mirrorRemoved((a), (b), (c), ZCL_SEQ_NUM, (d)))

void zcl_metering_requestFastPollMode(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_metering_requestFastPollModeCmd_t *pReq);
#define zcl_metering_requestFastPollModeCmd(a,b,c,d)	(zcl_metering_requestFastPollMode((a), (b), (c), ZCL_SEQ_NUM, (d)))

void zcl_metering_getProfileRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_metering_getProfileRspCmd_t *pRsp);
#define zcl_metering_getProfileRspCmd(a,b,c,d)	(zcl_metering_getProfileRsp((a), (b), (c), ZCL_SEQ_NUM, (d)))

void zcl_metering_requestMirror(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_metering_requestMirrorCmd(a,b,c)	(zcl_metering_requestMirror((a), (b), (c), ZCL_SEQ_NUM))

void zcl_metering_removeMirror(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_metering_removeMirrorCmd(a,b,c)	(zcl_metering_removeMirror((a), (b), (c), ZCL_SEQ_NUM))

void zcl_metering_requestFastPollModeRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_metering_requestFastPollModeRspCmd_t *pRsp);
#define zcl_metering_requestFastPollModeRspCmd(a,b,c,d)	(zcl_metering_requestFastPollModeRsp((a), (b), (c), ZCL_SEQ_NUM, (d)))

