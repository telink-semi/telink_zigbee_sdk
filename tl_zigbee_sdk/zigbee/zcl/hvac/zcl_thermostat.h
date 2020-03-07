/********************************************************************************************************
 * @file     zcl_thermostat.h
 *
 * @brief	 APIs for thermostat cluster
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

/**
 *  @brief	zcl_thermostat_attrId Attribute Ids
 */
//Server attributes
#define ZCL_ATTRID_HVAC_THERMOSTAT_LOCAL_TEMPERATURE				0x0000
#define ZCL_ATTRID_HVAC_THERMOSTAT_OUTDOOR_TEMPERATURE				0x0001
#define ZCL_ATTRID_HVAC_THERMOSTAT_OCCUPANCY						0x0002
#define ZCL_ATTRID_HVAC_THERMOSTAT_ABS_MIN_HEAT_SETPOINT_LIMIT		0x0003
#define ZCL_ATTRID_HVAC_THERMOSTAT_ABS_MAX_HEAT_SETPOINT_LIMIT		0x0004
#define ZCL_ATTRID_HVAC_THERMOSTAT_ABS_MIN_COOL_SETPOINT_LIMIT		0x0005
#define ZCL_ATTRID_HVAC_THERMOSTAT_ABS_MAX_COOL_SETPOINT_LIMIT		0x0006
#define ZCL_ATTRID_HVAC_THERMOSTAT_PI_COOLING_DEMAND				0x0007
#define ZCL_ATTRID_HVAC_THERMOSTAT_PI_HEATING_DEMAND				0x0008
#define ZCL_ATTRID_HVAC_THERMOSTAT_SYS_TYPE_CONFIGURATION			0x0009
#define ZCL_ATTRID_HVAC_THERMOSTAT_LOCAL_TEMP_CALIBRATION			0x0010
#define ZCL_ATTRID_HVAC_THERMOSTAT_OCCUPIED_COOLING_SETPOINT		0x0011
#define ZCL_ATTRID_HVAC_THERMOSTAT_OCCUPIED_HEATING_SETPOINT		0x0012
#define ZCL_ATTRID_HVAC_THERMOSTAT_UNOCCUPIED_COOLING_SETPOINT		0x0013
#define ZCL_ATTRID_HVAC_THERMOSTAT_UNOCCUPIED_HEATING_SETPOINT		0x0014
#define ZCL_ATTRID_HVAC_THERMOSTAT_MIN_HEAT_SETPOINT_LIMIT			0x0015
#define ZCL_ATTRID_HVAC_THERMOSTAT_MAX_HEAT_SETPOINT_LIMIT			0x0016
#define ZCL_ATTRID_HVAC_THERMOSTAT_MIN_COOL_SETPOINT_LIMIT			0x0017
#define ZCL_ATTRID_HVAC_THERMOSTAT_MAX_COOL_SETPOINT_LIMIT			0x0018
#define ZCL_ATTRID_HVAC_THERMOSTAT_MIN_SETPOINT_DEAD_BAND			0x0019
#define ZCL_ATTRID_HVAC_THERMOSTAT_REMOTE_SENSING					0x001A
#define ZCL_ATTRID_HVAC_THERMOSTAT_CTRL_SEQUENCE_OF_OPERATION		0x001B
#define ZCL_ATTRID_HVAC_THERMOSTAT_SYS_MODE							0x001C
#define ZCL_ATTRID_HVAC_THERMOSTAT_ALARM_MASK						0x001D
#define ZCL_ATTRID_HVAC_THERMOSTAT_RUNNING_MODE						0x001E
#define ZCL_ATTRID_HVAC_THERMOSTAT_START_OF_WEEK					0x0020
#define ZCL_ATTRID_HVAC_THERMOSTAT_NUM_OF_WEEKLY_TRANSITIONS		0x0021
#define ZCL_ATTRID_HVAC_THERMOSTAT_NUM_OF_DAILY_TRANSITIONS			0x0022
#define ZCL_ATTRID_HVAC_THERMOSTAT_TEMP_SETPOINT_HOLD				0x0023
#define ZCL_ATTRID_HVAC_THERMOSTAT_TEMP_SETPOINT_HOLD_DURATION		0x0024
#define ZCL_ATTRID_HVAC_THERMOSTAT_PROGRAMMING_OPERATION_MODE		0x0025
#define ZCL_ATTRID_HVAC_THERMOSTAT_RUNNING_STATE					0x0029
#define ZCL_ATTRID_HVAC_THERMOSTAT_SETPOINT_CHANGE_SOURCE			0x0030
#define ZCL_ATTRID_HVAC_THERMOSTAT_SETPOINT_CHANGE_AMOUNT			0x0031
#define ZCL_ATTRID_HVAC_THERMOSTAT_SETPOINT_CHANGE_SRC_TIMESTAMP	0x0032
#define ZCL_ATTRID_HVAC_THERMOSTAT_OCCUPIED_SETBACK					0x0034
#define ZCL_ATTRID_HVAC_THERMOSTAT_OCCUPIED_SETBACK_MIN				0x0035
#define ZCL_ATTRID_HVAC_THERMOSTAT_OCCUPIED_SETBACK_MAX				0x0036
#define ZCL_ATTRID_HVAC_THERMOSTAT_UNOCCUPIED_SETBACK				0x0037
#define ZCL_ATTRID_HVAC_THERMOSTAT_UNOCCUPIED_SETBACK_MIN			0x0038
#define ZCL_ATTRID_HVAC_THERMOSTAT_UNOCCUPIED_SETBACK_MAX			0x0039
#define ZCL_ATTRID_HVAC_THERMOSTAT_EMERGENCY_HEAT_DELTA				0x003A
#define ZCL_ATTRID_HVAC_THERMOSTAT_AC_TYPE							0x0040
#define ZCL_ATTRID_HVAC_THERMOSTAT_AC_CAPACITY						0x0041
#define ZCL_ATTRID_HVAC_THERMOSTAT_AC_REFRIGERANT_TYPE				0x0042
#define ZCL_ATTRID_HVAC_THERMOSTAT_AC_COMPRESSOR_TYPE				0x0043
#define ZCL_ATTRID_HVAC_THERMOSTAT_AC_ERROR_CODE					0x0044
#define ZCL_ATTRID_HVAC_THERMOSTAT_AC_LOUVER_POSITION				0x0045
#define ZCL_ATTRID_HVAC_THERMOSTAT_AC_COIL_TEMP						0x0046
#define ZCL_ATTRID_HVAC_THERMOSTAT_AC_CAPACITY_FORMAT				0x0047

/**
 *  @brief	zcl thermostat command ids
 */
//Server side commands received or client side commands generated
#define ZCL_CMD_THERMOSTAT_SETPOINT_RAISE_LOWER						0x00
#define ZCL_CMD_THERMOSTAT_SET_WEEKLY_SCHEDULE						0x01
#define ZCL_CMD_THERMOSTAT_GET_WEEKLY_SCHEDULE						0x02
#define ZCL_CMD_THERMOSTAT_CLEAR_WEEKLY_SCHEDULE					0x03
#define ZCL_CMD_THERMOSTAT_GET_RELAY_STATUS_LOG						0x04
//Server side commands generated or client side commands received
#define ZCL_CMD_THERMOSTAT_GET_WEEKLY_SCHEDULE_RSP					0x00
#define ZCL_CMD_THERMOSTAT_GET_RELAY_STATUS_LOG_RSP					0x01


/**
 *  @brief	setpoint raise/lower command
 */
typedef enum
{
	ADJUST_HEAT_SETPOINT,
	ADJUST_COOL_SETPOINT,
	ADJUST_HEAT_COOL_SETPOINT,
}setpointMode_t;

typedef struct
{
	setpointMode_t mode;
	s8 amount;
}zcl_thermostat_setpointRaiseLowerCmd_t;

/**
 *  @brief	set weekly schedule command
 */
typedef struct
{
	u16 transTime;
	u16	heatSetpoint;
}heatMode_t;

typedef struct
{
	u16 transTime;
	u16	coolSetpoint;
}coolMode_t;

typedef struct
{
	u16 transTime;
	u16	heatSetpoint;
	u16	coolSetpoint;
}bothMode_t;

typedef union
{
	heatMode_t *pHeatMode;
	coolMode_t *pCoolMode;
	bothMode_t *pBothMode;
}sequenceMode_t;

#define HEAT_SERPOINT_FIELD_PRESENT		0x01
#define COOL_SETPOINT_FIELD_PRESENT		0x02
#define BOTH_SETPOINT_FIELD_PRESENT		0x03

typedef struct
{
	sequenceMode_t sequenceMode;
	u8 numOfTransForSequence;
	u8 dayOfWeekForSequence;
	u8 modeForSequence;
}zcl_thermostat_setWeeklyScheduleCmd_t;

/**
 *  @brief	get weekly schedule command
 */
typedef struct
{
	u8 daysToReturn;
	u8 modeToReturn;
}zcl_thermostat_getWeeklyScheduleCmd_t;

/**
 *  @brief	get weekly schedule response command
 */
typedef zcl_thermostat_setWeeklyScheduleCmd_t zcl_thermostat_getWeeklyScheduleRspCmd_t;

/**
 *  @brief	get relay status log response command
 */
typedef struct
{
	u16 timeOfDay;
	u16 relayStatus;
	s16 localTemp;
	u16 setpoint;
	u16 unreadEntries;
	u8	hunidityInPercentage;
}zcl_thermostat_getRelayStatusLogRspCmd_t;


status_t zcl_thermostat_register(u8 endpoint, u8 arrtNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

status_t zcl_thermostat_setpointRaiseLowerCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_thermostat_setpointRaiseLowerCmd_t *pCmd);
#define zcl_thermostat_setpointRaiseLowerCmdSend(a,b,c,d)	(zcl_thermostat_setpointRaiseLowerCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_thermostat_setWeeklyScheduleCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_thermostat_setWeeklyScheduleCmd_t *pCmd);
#define zcl_thermostat_setWeeklyScheduleCmdSend(a,b,c,d)	(zcl_thermostat_setWeeklyScheduleCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_thermostat_getWeeklyScheduleCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_thermostat_getWeeklyScheduleCmd_t *pCmd);
#define zcl_thermostat_getWeeklyScheduleCmdSend(a,b,c,d)	(zcl_thermostat_getWeeklyScheduleCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_thermostat_clearWeeklyScheduleCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_thermostat_clearWeeklyScheduleCmdSend(a,b,c)	(zcl_thermostat_clearWeeklyScheduleCmd((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_thermostat_getRelayStatusLogCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_thermostat_getRelayStatusLogCmdSend(a,b,c)		(zcl_thermostat_getRelayStatusLogCmd((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_thermostat_getWeeklyScheduleRspCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_thermostat_getWeeklyScheduleRspCmd_t *pCmd);
#define zcl_thermostat_getWeeklyScheduleRspCmdSend(a,b,c,d)	(zcl_thermostat_getWeeklyScheduleRspCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_thermostat_getRelayStatusLogRspCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_thermostat_getRelayStatusLogRspCmd_t *pCmd);
#define zcl_thermostat_getRelayStatusLogRspCmdSend(a,b,c,d)	(zcl_thermostat_getRelayStatusLogRspCmd((a), (b), (c), ZCL_SEQ_NUM, (d)))

