/********************************************************************************************************
 * @file     zcl_alarm.h
 *
 * @brief	 APIs statement for alarm cluster
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
 
/** @addtogroup  ZCL_CLUSTER_ALARMS ALARMS Cluster
 *  @{
 */

/** @addtogroup  ZCL_Alarm_Constant ALARMS Cluster Constants
 *  @{
 */

/**
 *  @brief  Size of ZCL alarm table 
 */
#ifndef ZCL_ALARM_TABLE_NUM
#define ZCL_ALARM_TABLE_NUM                            5
#endif

/** @addtogroup zcl_alarm_cmdId ALARMS Command Ids
 * @{
 */
#define ZCL_CMD_ALARM_RESET_ALARM                      0x00
#define ZCL_CMD_ALARM_RESET_ALL                        0x01
#define ZCL_CMD_ALARM_GET_ALARM                        0x02
#define ZCL_CMD_ALARM_RESET_ALARM_LOG                  0x03

#define ZCL_CMD_ALARM_ALARM                            0x00
#define ZCL_CMD_ALARM_GET_ALARM_RSP                    0x01
/** @} end of group zcl_alarm_cmdId */

/** @addtogroup zcl_alarm_attrId ALARMS Attribute Ids
 * @{
 */
#define ZCL_ATTRID_ALARM_COUNT                       0x0000
/** @} end of group zcl_alarm_attrId */

/** @} end of group ZCL_Alarm_Constant */

/** @addtogroup  ZCL_ALARM_Types ALARMS Cluster Types
 *  @{
 */

/**
 *  @brief  Structure definition for reset alarm in alarm cluster
 */
typedef struct {
    u8  alarmCode;                       //!< alarm code
    u16 clusterId;                       //!< cluster id
} resetAlarm_t;

/**
 *  @brief  Structure definition for alarm in alarm cluster
 */
typedef struct {
    u8  alarmCode;                       //!< alarm code
    u16 clusterId;                       //!< cluster id
} alarm_t;

/**
 *  @brief  Structure definition for get alarm response command in alarm cluster
 */
typedef struct {
	u8  status;                          //!< status code
    u8  alarmCode;                       //!< Identifying code for the cause of the alarm
    u16 clusterId;                       //!< The identifier of the cluster whose attribute generated this alarm
    u32 timeStamp;                       //!< The time at which the alarm occurred
} getAlarmRsp_t;
/** @} end of group ZCL_ALARM_Types */


/**
 *  @brief  Structure definition for alarm table entry in alarm cluster
 */
typedef struct {
	u8  alarmCode;                       //!< Identifying code for the cause of the alarm
    u16 clusterId;                       //!< The identifier of the cluster whose attribute generated this alarm
    u32 timeStamp;                       //!< The time at which the alarm occurred
} zcl_alarmEntry_t;

/**
 *  @brief  Structure definition for alarm table
 */
typedef struct {
	u8 used;
	u8 endpoint;
	zcl_alarmEntry_t alarm;
} zcl_alarmTable_t;

/**
 *  @brief  External variable for alarm table
 */
extern zcl_alarmTable_t g_zcl_alarmTab[];



#if 0
/** @addtogroup  ZCL_ALARM_Callbacks ALARMS Cluster Callbacks
 *  @{
 */
typedef void (*zcl_alarmResetAlarmCb_t)(apsdeDataInd_t *pApsdeInd, resetAlarm_t *cmd);
typedef void (*zcl_alarmResetAllCb_t)(apsdeDataInd_t *pApsdeInd);
typedef void (*zcl_alarmGetAlarmCb_t)(apsdeDataInd_t *pApsdeInd);
typedef void (*zcl_alarmResetAlarmLogCb_t)(apsdeDataInd_t *pApsdeInd);
typedef void (*zcl_alarmRecvAlarmCb_t)(apsdeDataInd_t *pApsdeInd, alarm_t *cmd);
typedef void (*zcl_alarmGetAlarmRspCb_t)(apsdeDataInd_t *pApsdeInd, getAlarmRsp_t *cmd);

/**
 *  @brief  Structure definition for commands callback functions in Alarm cluster
 */
typedef struct {
	zcl_alarmResetAlarmCb_t       alarmResetCbFunc;
	zcl_alarmResetAllCb_t         alarmResetAllCbFunc;
	zcl_alarmGetAlarmCb_t		  alarmGetAlarmCbFunc;
	zcl_alarmResetAlarmLogCb_t    alarmResetLogCbFunc;
	zcl_alarmRecvAlarmCb_t        alarmRecvAlarmCbFunc;
	zcl_alarmGetAlarmRspCb_t      alarmGetAlarmRspFunc;
} zcl_alarm_AppCallbacks_t;

/** @} end of group ZCL_ALARM_Callbacks */


/** @addtogroup  ZCL_ALARM_Variables Alarms Cluster Variables
 *  @{
 */

/**
 *  @brief  External variable for alarm Attribute
 */
extern u16 zcl_attr_alarmCount;

/**
 *  @brief  External variable for alarm Attribute table
 */
extern const zclAttrInfo_t alarm_attrTbl[];

extern const u8 zcl_alarm_attrNum;

/** @} end of group ZCL_Scene_Variables */
#endif

/** @addtogroup  ZCL_Alarms_Fountions ALARMS Cluster APIs
 *  @{
 */

/**
 * @brief       This function register ALARMS cluster server to ZCL. Only after registered,
 *              alarms attributes could be used.
 *
 * @param[in]   cb       - pointer to the App layer command callbacks
 *
 * @return      Status
 */
status_t zcl_alarm_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);


/**
 * @brief       API to send alarm command in ALARM cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   pAlarm
 *
 * @return      status_t
 */
status_t zcl_alarm_alarm(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, alarm_t *pAlarm);
#define zcl_alarm_alarmCmd(a,b,c,d)		(zcl_alarm_alarm((a), (b), (c), ZCL_SEQ_NUM, (d)))

/**
 * @brief       API to send reset alarm command in ALARM cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   pResetAlarm
 *
 * @return      status_t
 */
status_t zcl_alarm_resetAlarm(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, resetAlarm_t *pResetAlarm);
#define zcl_alarm_resetAlarmCmd(a,b,c,d)		(zcl_alarm_resetAlarm((a), (b), (c), ZCL_SEQ_NUM, (d)))

/**
 * @brief       API to send reset all alarm command in ALARM cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 *
 * @return      status_t
 */
status_t zcl_alarm_resetAllAlarm(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_alarm_resetAllAlarmCmd(a,b,c)		(zcl_alarm_resetAllAlarm((a), (b), (c), ZCL_SEQ_NUM))

/**
 * @brief       API to send get alarm command in ALARM cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 *
 * @return      status_t
 */
status_t zcl_alarm_getAlarm(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_alarm_getAlarmCmd(a,b,c)		(zcl_alarm_getAlarm((a), (b), (c), ZCL_SEQ_NUM))

/**
 * @brief       API to send reset alarm log command in ALARM cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 *
 * @return      status_t
 */
status_t zcl_alarm_resetAlarmLog(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_alarm_resetAlarmLogCmd(a,b,c)		(zcl_alarm_resetAlarmLog((a), (b), (c), ZCL_SEQ_NUM))


/**
 * @brief       add alarm to alarm table
 *
 * @param[in]   endpoint  - endpoint
 * @param[in]   alarmCode - alarm code
 * @param[in]   clusterId - cluster id
 * @param[in]   timeStamp - time stamp
 *
 * @return      status code
 */
status_t zcl_alarm_AddAlarmTab(u8 endpoint, u8 alarmCode, u16 clusterId, u32 timeStamp);


/** @} end of group ZCL_Alarms_Fountions */

/** @} end of group ZCL_CLUSTER_ALARMS */

/** @} end of group ZCL_Module */

/** @} end of group TELINK_ZIGBEE_STACK */
