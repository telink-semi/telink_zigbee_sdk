/********************************************************************************************************
 * @file	sampleGateway.h
 *
 * @brief	This is the header file for sampleGateway
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
	u8 keyType; /* ERTIFICATION_KEY or MASTER_KEY key for touch-link or distribute network
	 	 	 	 SS_UNIQUE_LINK_KEY or SS_GLOBAL_LINK_KEY for distribute network */
	u8 key[16];	/* the key used */
}app_linkKeyType_t;

typedef struct{
	app_linkKeyType_t tcLinkKey;
	app_linkKeyType_t distributeLinkKey;
	app_linkKeyType_t touchlinkKey;
}app_linkkey_info_t;

typedef struct{
	ev_time_event_t *timerLedEvt;
	u32 keyPressedTime;

	u16 ledOnTime;
	u16 ledOffTime;
	u8 	oriSta;		//original state before blink
	u8 	sta;		//current state in blink
	u8 	times;		//blink times
	u8  state;
	u8	keyPressed;

	app_linkkey_info_t linkKey;
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
#ifdef ZCL_DOOR_LOCK
status_t sampleGW_doorLockCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
#endif
#ifdef ZCL_IAS_ZONE
status_t sampleGW_iasZoneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
#endif
#ifdef ZCL_POLL_CTRL
status_t sampleGW_pollCtrlCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
#endif

void sampleGW_devAnnHandler(zdo_device_annce_req_t *pDevAnnceReq);
void sampleGW_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf);
void sampleGW_leaveIndHandler(nlme_leave_ind_t *pLeaveInd);
bool sampleGW_nwkUpdateIndicateHandler(nwkCmd_nwkUpdate_t *pNwkUpdateCmd);
bool sampleGW_tcJoinIndHandler(zdo_tc_join_ind_t *pTcJoinInd);

void sampleGW_dataSendConfirm(void *arg);

#if AF_TEST_ENABLE
void afTest_rx_handler(void *arg);
void afTest_dataSendConfirm(void *arg);
#endif

#endif /* _SAMPLE_GW_H_ */
