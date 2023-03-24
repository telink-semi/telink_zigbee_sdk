/********************************************************************************************************
 * @file    sampleGateway.h
 *
 * @brief   This is the header file for sampleGateway
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
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

#ifndef _SAMPLE_GW_H_
#define _SAMPLE_GW_H_


/**********************************************************************
 * CONSTANT
 */
#define SAMPLE_GW_ENDPOINT      0x01
#define SAMPLE_TEST_ENDPOINT	0x02

/**********************************************************************
 * TYPEDEFS
 */
typedef struct{
	ev_timer_event_t *timerLedEvt;
	u32 keyPressedTime;

	u16 ledOnTime;
	u16 ledOffTime;
	u8 	oriSta;		//original state before blink
	u8 	sta;		//current state in blink
	u8 	times;		//blink times
	u8  state;
	u8	keyPressed;
}app_ctx_t;

/**
 *  @brief Defined for basic cluster attributes
 */
typedef struct{
	u8 	zclVersion;
	u8	appVersion;
	u8	stackVersion;
	u8	hwVersion;
	u8	manuName[ZCL_BASIC_MAX_LENGTH];
	u8	modelId[ZCL_BASIC_MAX_LENGTH];
	u8	powerSource;
	u8	deviceEnable;
}zcl_basicAttr_t;

/**
 *  @brief Defined for identify cluster attributes
 */
typedef struct{
	u16	identifyTime;
}zcl_identifyAttr_t;


/**********************************************************************
 * GLOBAL VARIABLES
 */
extern app_ctx_t g_appGwCtx;

extern bdb_appCb_t g_zbDemoBdbCb;

extern u8 SAMPLE_GW_CB_CLUSTER_NUM;
extern const zcl_specClusterInfo_t g_sampleGwClusterList[];
extern const af_simple_descriptor_t sampleGW_simpleDesc;
#if AF_TEST_ENABLE
extern const af_simple_descriptor_t sampleTestDesc;
#endif

/* Attributes */
extern zcl_basicAttr_t g_zcl_basicAttrs;
extern zcl_identifyAttr_t g_zcl_identifyAttrs;


/**********************************************************************
 * FUNCTIONS
 */
void sampleGW_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg);

status_t sampleGW_basicCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t sampleGW_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t sampleGW_groupCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t sampleGW_sceneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t sampleGW_doorLockCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t sampleGW_iasZoneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t sampleGW_pollCtrlCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);

void sampleGW_devAnnHandler(zdo_device_annce_req_t *pDevAnnceReq);
void sampleGW_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf);
void sampleGW_leaveIndHandler(nlme_leave_ind_t *pLeaveInd);
bool sampleGW_nwkUpdateIndicateHandler(nwkCmd_nwkUpdate_t *pNwkUpdateCmd);
void sampleGW_permitJoinIndHandler(nlme_permitJoining_req_t *pPermitJoinReq);
bool sampleGW_tcJoinIndHandler(zdo_tc_join_ind_t *pTcJoinInd);
void sampleGW_tcFrameCntReachedHandler(void);

void sampleGW_dataSendConfirm(void *arg);

#if AF_TEST_ENABLE
void afTest_rx_handler(void *arg);
void afTest_dataSendConfirm(void *arg);
#endif

#endif /* _SAMPLE_GW_H_ */
