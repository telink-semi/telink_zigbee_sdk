/********************************************************************************************************
 * @file     sampleGateway.h
 *
 * @brief
 *
 * @author
 * @date     Dec. 1, 2016
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

void sampleGW_devAnnHandler(void *arg);
void sampleGW_leaveCnfHandler(void *arg);
void sampleGW_leaveIndHandler(void *arg);

void sampleGW_dataSendConfirm(void *arg);

u8 sampleGW_nwkUpdateIndicateHandler(void *arg);

#if AF_TEST_ENABLE
void afTest_rx_handler(void *arg);
void afTest_dataSendConfirm(void *arg);
#endif

#endif /* _SAMPLE_GW_H_ */
