/********************************************************************************************************
 * @file    gp_sink.h
 *
 * @brief   This is the header file for gp_sink
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
#ifndef GP_SINK_H
#define GP_SINK_H


#define GPS_MAX_SINK_TABLE_ENTRIES              5

//A4.2.1
#define GP_COMMISSIONING_PACKET_LEN_MAX         55

#define SINK_GROUP_LIST_NUM                     2

#define ZCL_GP_MAX_SINK_TABLE_ATTR_LEN          (45 * 2)

#define GPS_MULTI_BUF_SIZE_MAX                  3

typedef union {
    u16 opts;
    struct {
        u16 appId:3;
        u16 communicationMode:2;
        u16 seqNumCapabilities:1;
        u16 rxOnCapability:1;
        u16 fixedLocation:1;
        u16 assignedAlias:1;
        u16 securityUse:1;
        u16 reserved:6;
    } bits;
} gpSinkEntryOpt_t;

/**
 *  @brief	Sink table entry
 */
typedef struct _attribute_packed_ {
    gpdId_t gpdId;              //8-bytes
    u32 gpdSecFrameCnt;
    gpSinkEntryOpt_t options;
    u16 gpdAssignedAlias;       //8-bytes
    gpSinkGroupListItem_t groupList[SINK_GROUP_LIST_NUM]; //8-bytes
    u8 endpoint;
    u8 deviceId;
    u8 groupcastRadius;
    gpSecOpt_t secOptions;      //4-bytes
    u8 gpdKey[16];              //16-bytes
    u8 used:1;
    u8 sinkGroupListCnt:2;
    u8 complete:1;
    u8 replyIncludeKey:1;
    u8 replyEncrypt:1;
    u8 reserved:2;
} gpSinkTabEntry_t; //45-bytes

/***************************************************************************
* @brief	Define for GPs
*/
typedef struct {
    ev_timer_event_t *gpsCommWindowTimeoutEvt;
    ev_timer_event_t *gpsMultiSensorCommTimeoutEvt;
    u8 *pMultiSensorCommBuf[GPS_MULTI_BUF_SIZE_MAX];
    u8 multiSensorCommBufSize;
    u8 multiSensorCommTime;
    u8 multiSensorTotalNumReport;
    u8 multiSensorNumOfReport;
    u8 multiSensorCompleted:1;
    u8 multiSensorReportMatched:1;
    u8 resv:6;
    u8 gpsInCommMode;
    u8 gpsAppEndpoint;
} gps_ctx_t;

typedef struct {
    u8 devId;
    u8 gpdCommanadId;
    u16 clusterId;
    u8 zigEndpoint;
    u8 zigbeeCommandId;
    u8 payloadLen;
    u8 changeIndex;
    u8 payload[8];
} gp_gpdZigCommand_t;

//global value
typedef struct _attribute_packed_ {
    gpSinkTabEntry_t gpSinkTab[GPS_MAX_SINK_TABLE_ENTRIES];
    u8 gpSinkTabNum;
} gp_sinkTab_t;

extern gp_sinkTab_t g_gpSinkTab;
extern gps_ctx_t g_gpsCtx;

gpSecRsp_status_t gpSinkSecOperation(u8 appId, gpdId_t gpdId,
                                     u8 endpoint, u32 gpdSecFrameCnt,
                                     u8 gpdfSecurityLevel, u8 gpdfKeyType,
                                     u8 *pKeyType, u8 *pKey);

void gpsGpDataIndProcess(gp_data_ind_t *pGpDataInd);

void gpSinkTabInit(void);
void gpSinkTabUpdate(void);
gpSinkTabEntry_t *gp_sinkTabEntryFreeGet(void);
gpSinkTabEntry_t *gp_getSinkTabByGpdId(u8 appId, gpdId_t gpdId);
void gp_sinkRemoveEntries(u8 appId, gpdId_t gpdId, u8 endpoint);
void gp_sinkGroupListAdd(gpSinkTabEntry_t *pEntry, u8 endpoint);
u8 gp_getSinkTabEntryTotalNum(void);
u8 gp_getSinkTabEntryLen(gpSinkTabEntry_t *pEntry);
u8 gp_buildSinkTabEntryFormat(gpSinkTabEntry_t *pEntry, u8 *pBuf);
void gp_sinkInit(u8 endpoint);

void gpCommissioningNotificationCmdProcess(zcl_gp_commissioningNotificationCmd_t *pCmd);

u8 gpsCommissionModeSet(bool action, bool involveProxies);

status_t zcl_gpClientCmdHandler(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);

#endif	/* GP_SINK_H */
