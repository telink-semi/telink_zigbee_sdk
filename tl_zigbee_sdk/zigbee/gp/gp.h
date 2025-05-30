/********************************************************************************************************
 * @file    gp.h
 *
 * @brief   This is the header file for gp
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
#ifndef GP_H
#define GP_H


//if ZERO, means only BASIC PROXY include.
#ifndef GP_BASIC_COMBO
#define GP_BASIC_COMBO                                  0
#endif

#define GREEN_POWER_ENDPOINT                            0xF2//242

//for Green Power device frame
#define GP_ZB_PROTOCOL_VERSION                          0x03

//GP Shared Key
#define GP_SHARED_KEY                                   { 0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,\
                                                          0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF }

#define DEFAULT_TC_LINK_KEY                             { 0x5a,0x69,0x67,0x42,0x65,0x65,0x41,0x6c,\
                                                          0x6c,0x69,0x61,0x6e,0x63,0x65,0x30,0x39 }
//GP Link Key
#define GP_LINK_KEY                                     DEFAULT_TC_LINK_KEY

//A.3.6.1.2.1, the time the GP endpoint of the sink and the proxy keeps the information on the received GPDF, in order to filter out duplicates.
#define GP_DUPLICATE_TIMEOUT_DEFAULT                    2//2s

//A.3.6.3.2
#define GPP_COMMISSIONING_WINDOW_DEFAULT                180//180s

//A.3.6.2.4
#define GPS_MULTI_SENSOR_COMMISSIONING_TIMEOUT          20//20s

//A.3.9.1, step 8, time that the basic proxy device will be absent of the operational network due to bidirectional commissioning.
#define GP_TRANSMIT_CHANNEL_TIMEOUT                     5//5s


//GPD Device IDs
#define GPD_DEV_ID_SIMPLE_1STATE_SWITCH                 0x00
#define GPD_DEV_ID_SIMPLE_2STATE_SWITCH                 0x01
#define GPD_DEV_ID_ONOFF_SWITCH                         0x02
#define GPD_DEV_ID_LEVEL_CONTROL_SWITCH                 0x03
#define GPD_DEV_ID_GENERIC_8_CONTACT_SWITCH             0x07
#define GPD_DEV_ID_TEMPERATURE_SENSOR                   0x30
#define GPD_DEV_ID_NOT_SPECIFIC_FUNCTION                0xFE

//GPD Command IDs, A.4.1
#define GPDF_CMD_ID_ONOFF_OFF                           0x20
#define GPDF_CMD_ID_ONOFF_ON                            0x21
#define GPDF_CMD_ID_ONOFF_TOGGLE                        0x22
#define GPDF_CMD_ID_LEVEL_MOVE_UP                       0x30
#define GPDF_CMD_ID_LEVEL_MOVE_DOWN                     0x31
#define GPDF_CMD_ID_LEVEL_STEP_UP                       0x32
#define GPDF_CMD_ID_LEVEL_STEP_DOWN                     0x33
#define GPDF_CMD_ID_LEVEL_STOP                          0x34
#define GPDF_CMD_ID_LEVEL_MOVE_UP_WITH_ONOFF            0x35
#define GPDF_CMD_ID_LEVEL_MOVE_DOWN_WITH_ONOFF          0x36
#define GPDF_CMD_ID_LEVEL_STEP_UP_WITH_ONOFF            0x37
#define GPDF_CMD_ID_LEVEL_STEP_DOWN_WITH_ONOFF          0x38

#define GPDF_CMD_ID_8BIT_VECTOR_PRESS                   0x69
#define GPDF_CMD_ID_8BIT_VECTOR_RELEASE                 0x6A

#define GPDF_CMD_ID_ATTR_REPORT                         0xA0
#define GPDF_CMD_ID_MANU_ATTR_REPORT                    0xA1
#define GPDF_CMD_ID_MULTI_CLUSTER_REPORT                0xA2
#define GPDF_CMD_ID_MANU_MULTI_CLUSTER_REPORT           0xA3
#define GPDF_CMD_ID_ZCL_TUNNELING                       0xA6
#define GPDF_CMD_ID_COMPACT_ATTR_REPORT                 0xA8
#define GPDF_CMD_ID_ANY_SENSOR_REPORT_COMMAND           0xAF

#define GPDF_CMD_ID_COMMISSIONING                       0xE0
#define GPDF_CMD_ID_DECOMMISSIONING                     0xE1
#define GPDF_CMD_ID_SUCCESS                             0xE2
#define GPDF_CMD_ID_CHANNEL_REQ                         0xE3
#define GPDF_CMD_ID_APP_DESCRIPTION                     0xE4
#define GPDF_CMD_ID_COMMISSIONING_REPLY                 0xF0
#define GPDF_CMD_ID_CHANNEL_CONFIGURATION               0xF3


//GPP functionality
/************************************************************************************************************
b0    ||  GP feature                                                                              ||    0b1
b1    ||  Direct communication (reception of GPDF via GP stub)                                    ||    0b1
b2    ||  Derived groupcast communication                                                         ||    0b1
b3    ||  Pre-commissioned groupcast communication                                                ||    0b1
b4    ||  Full unicast communication                                                              ||    0b0
b5    ||  Lightweight unicast communication                                                       ||    0b1
b6    ||  Reserved                                                                                ||    0b0
b7    ||  Bidirectional operation                                                                 ||    0b0
b8    ||  Proxy Table maintenance (active and passive, for GPD mobility and GPP robustness)       ||    0b0
b9    ||  Reserved                                                                                ||    0b0
b10   ||  GP commissioning                                                                        ||    0b1
b11   ||  CT-based commissioning                                                                  ||    0b1
b12   ||  Maintenance of GPD (deliver channel/key during operation)                               ||    0b0
b13   ||  gpdSecurityLevel = 0b00                                                                 ||    0b1
b14   ||  Deprecated: gpdSecurityLevel = 0b01                                                     ||    0b0
b15   ||  gpdSecurityLevel = 0b10                                                                 ||    0b1
b16   ||  gpdSecurityLevel = 0b11                                                                 ||    0b1
b17   ||  Reserved                                                                                ||    0b0
b18   ||  Reserved                                                                                ||    0b0
b19   ||  GPD IEEE address                                                                        ||    0b1
b20   ||  Reserved                                                                                ||    0b0
      ||  b21 - b23 Reserved                                                                      ||    0b0
************************************************************************************************************/
#define GPP_FUNCTIONALITY                               0x09AC2F  // Table 42 - Format of the gppFunctionality attribute

//GPS functionality
/************************************************************************************************************
b0    ||  GP feature                                                                              ||    0b1
b1    ||  Direct communication (reception of GPDF via GP stub)                                    ||    0b1
b2    ||  Derived groupcast communication                                                         ||    0b1
b3    ||  Pre-commissioned groupcast communication                                                ||    0b1
b4    ||  Full unicast communication                                                              ||    0b0
b5    ||  Lightweight unicast communication                                                       ||    0b1
b6    ||  Proximity bidirectional operation                                                       ||    0b0
b7    ||  Multi-hop bidirectional operation                                                       ||    0b0
b8    ||  Proxy Table maintenance (active and passive, for GPD mobility and GPP robustness)       ||    0b0
b9    ||  Proximity commissioning (unidirectional and bidirectional)                              ||    0b1
b10   ||  Multi-hop commissioning (unidirectional and bidirectional)                              ||    0b1
b11   ||  CT-based commissioning                                                                  ||    0b1
b12   ||  Maintenance of GPD (deliver channel/key during operation)                               ||    0b0
b13   ||  gpdSecurityLevel = 0b00 in operation                                                    ||    0b1
b14   ||  Deprecated: gpdSecurityLevel = 0b01                                                     ||    0b0
b15   ||  gpdSecurityLevel = 0b10                                                                 ||    0b1
b16   ||  gpdSecurityLevel = 0b11                                                                 ||    0b1
b17   ||  Sink Table-based groupcast forwarding                                                   ||    0b0
b18   ||  Translation Table                                                                       ||    0b0
b19   ||  GPD IEEE address                                                                        ||    0b1
b20	  ||  Compact attribute reporting                                                         ||    0b0
      ||  b21 - b23 Reserved                                                                      ||    0b0
************************************************************************************************************/
#define GPS_FUNCTIONALITY                               0x09AE2F  // Table 28 - Format of the gpsFunctionality attribute

//GGP active functionality, A.3.4.2.8
#define GPP_ACTIVE_FUNCTIONALITY                        0xFFFFFF
#define GPS_ACTIVE_FUNCTIONALITY                        0xFFFFFF



/***************************************************************************
*  @brief	Define for GP Channel request command
*/
typedef union {
    u8 chTogglingBehavior;
    struct {
        u8 rxChNext:4;
        u8 rxChSecondNext:4;
    } bits;
} gpd_channelRequestCmd_payload_t;

/***************************************************************************
*  @brief	Define for GP Response
*/
typedef union {
    u8 channel;
    struct {
        u8 operationChannel:4;
        u8 basic:1;
        u8 reserved:3;
    } bits;
} gpd_channelConfigurationCmd_payload_t;

/***************************************************************************
* @brief	Define for commissioning command payload
*/
typedef union {
    u8 opts;
    struct {
        u8 macSeqNumCap:1;
        u8 rxOnCap:1;
        u8 appInfoPresent:1;
        u8 reserved:1;
        u8 panIdReq:1;
        u8 gpSecKeyReq:1;
        u8 fixedLocation:1;
        u8 extOpPresent:1;
    } bits;
} gpCommissioningOpt_t;

typedef union {
    u8 opts;
    struct {
        u8 secLevelCap:2;
        u8 keyType:3;
        u8 gpdKeyPresent:1;
        u8 gpdKeyEncrypt:1;
        u8 outgoingCounterPresent:1;
    } bits;
} gpCommissioningExtOpt_t;

typedef struct {
    u8 *commandList;
    u8 *clusterList;
    u8 payloadLen;
    u8 deviceId;
    gpCommissioningOpt_t options;
    gpCommissioningExtOpt_t extOptions;
    u8 gpdSecKey[SEC_KEY_LEN];
    u32 gpdMIC;
    u32 gpdOutGoingCounter;
    u16 manuId;
    u16 modelId;
    gpSwitchInfo_t switchInfo;
    gpAppInfo_t appInfo;
    u8 gpdCmdNum;
} gpd_commissioning_payload_t;

/***************************************************************************
*  @brief	Define for GP commissioning reply
*/
typedef union {
    u8 opts;
    struct {
        u8 panIdPresent:1;
        u8 gpdSecKeyPresent:1;
        u8 gpdKeyEncrypt:1;
        u8 securityLevel:2;
        u8  keyType:3;
    } bits;
} gpCommissioningReplyOpt_t;

typedef struct {
    u8  key[16];
    u32 keyMIC;
    u32 frameCounter;
    u16 panId;
    gpCommissioningReplyOpt_t options;
} gpd_commissioningReply_payload_t;

/***************************************************************************
*  @brief	Define for GP Application Description
*/
typedef union {
    u8 opts;
    struct {
        u8 timeoutPeriodPresent:1;
        u8 resv:7;
    } bits;
} gpReportOpt_t;

typedef union {
    u8 opts;
    struct {
        u8 numOfRecords:3;
        u8 side:1;
        u8 manuIdPresent:1;
        u8 resv:3;
    } bits;
} gpDataPointOpt_t;

typedef union {
    u8 opts;
    struct {
        u8 remainingLen:4;
        u8 reported:1;
        u8 attrValuePresent:1;
        u8 resv:2;
    } bits;
} gpAttrOpt_t;

typedef struct {
    u16 attrId;
    u8 attrDataType;
    gpAttrOpt_t attrOption;
    u8 attrOffsetWithinReport;
    u8 attrValue[8];
} gpAttrRecord_t;

typedef struct {
    u16 clusterId;
    u16 manuId;
    gpDataPointOpt_t dataPointOpt;
    gpAttrRecord_t attrRecord[];
} gpDataPointDesc_t;

typedef struct {
    u16 timeoutPeriod;
    u8 reportId;
    gpReportOpt_t reportOpt;
    u8 remainingLen;
    gpDataPointDesc_t dataPointDesc[];
} gpReportDesc_t;

typedef struct {
    u8 totalNumReport;
    u8 numOfReport;
    gpReportDesc_t reportDesc[];
} gpd_appDesc_payload_t;

/***************************************************************************
*  @brief	Define for GP Data
*/
typedef struct {
    //TODO
} gpd_data_payload_t;

/***************************************************************************
* @brief	Define for GP (Commissioning) Notification
*/
typedef struct {
    epInfo_t dstEpInfo;
    union {
        zcl_gp_notificationCmd_t gpNotificationCmd;
        zcl_gp_commissioningNotificationCmd_t gpCommissioningNotificationCmd;
    };
} gp_noti_t;

/***************************************************************************
* @brief	Define for APP layer
*/
typedef bool (*gpChangleChannelReqCb_t)(void);
typedef void (*gpCommissioningModeCb_t)(bool isInCommMode);

typedef struct {
    gpChangleChannelReqCb_t gpChangleChannelReqCb;
    gpCommissioningModeCb_t gpCommissioningModeCb;
} gp_appCb_t;


/**
 *  @brief  External variable for GP Attribute
 */
extern u8 zclGpAttr_gpsMaxSinkTabEntries;
extern u8 zclGpAttr_sinkTabEntry[];
extern u8 zclGpAttr_gpsCommMode;
extern u8 zclGpAttr_gpsCommExitMode;
extern u16 zclGpAttr_gpsCommWindow;
extern u8 zclGpAttr_gpsSecLevel;
extern u8 zclGpAttr_gpsFunc[];
extern u8 zclGpAttr_gpsActiveFunc[];
extern u8 zclGpAttr_gppMaxProxyTabEntries;
extern u16 zclGpAttr_clusterRev;
extern u8 zclGpAttr_proxyTabEntry[];
extern u8 zclGpAttr_gppFunc[];
extern u8 zclGpAttr_gppActiveFunc[];
extern u8 zclGpAttr_gpSharedSecKeyType;
extern u8 zclGpAttr_gpSharedSecKey[];
extern u8 zclGpAttr_gpLinkKey[];

extern u8 GP_CB_CLUSTER_NUM;
extern const zcl_specClusterInfo_t g_gpClusterList[];
extern const af_simple_descriptor_t gp_simpleDesc;
extern gp_appCb_t *gpAppCb;


void gpDataCnfPrc(gp_data_cnf_t *pGpDataCnf);
void gpDataIndPrc(void *arg);
void gpSecReqPrc(void *arg);

void gp_init(u8 endpoint);
void gp_registerAppCb(gp_appCb_t *cb);
status_t zcl_gpCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);

bool gpDevAnnceCheckCb(u16 ailasNwkAddr, addrExt_t ailasIeeeAddr);
bool gpChangleChannelReqCb(void);
void gpCommissioningModeCb(bool isInCommMode);
void gpCommissioningNotificationCmdFromLocalCb(zcl_gp_commissioningNotificationCmd_t *pCmd);

void gpsCommissionModeInvork(void);

#endif	/* GP_H */
