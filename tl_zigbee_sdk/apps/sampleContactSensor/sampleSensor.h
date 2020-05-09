/********************************************************************************************************
 * @file     sampleSensor.h
 *
 * @brief
 *
 * @author
 * @date     Jan. 4, 2018
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

#ifndef _SAMPLE_CONTACT_SENSOR_H_
#define _SAMPLE_CONTACT_SENSOR_H_


/**********************************************************************
 * CONSTANT
 */
#define SAMPLE_SENSOR_ENDPOINT  0x01

/**********************************************************************
 * TYPEDEFS
 */
typedef struct{
	ev_time_event_t *timerLedEvt;
	u32 keyPressedTime;
	u16 ledOnTime;
	u16 ledOffTime;
	u8 	oriSta;		//original state before blink
	u8 	sta;		//current state in blink
	u8 	times;		//blink times
	u8  state;
	u8  keyPressed;

	bool installCodeAvailable:1;	//flag indicates if support install code
	bool useInstallCodeFlg:1;		//flag indicates switch link key between install code and default link key
	bool reserved:6;
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

/**
 *  @brief Defined for ias zone cluster attributes
 */
typedef struct {
	u8  zoneState;
	u16 zoneType;
	u16 zoneStatus;
	extAddr_t  iasCieAddr;
	u8  zoneId;
}zcl_iasZoneAttr_t;

/**
 *  @brief  Defined for poll control cluster attributes
 */
typedef struct{
	u32	chkInInterval;
	u32	longPollInterval;
	u32	chkInIntervalMin;
	u32	longPollIntervalMin;
	u16	shortPollInterval;
	u16	fastPollTimeout;
	u16	fastPollTimeoutMax;
}zcl_pollCtrlAttr_t;

/**********************************************************************
 * GLOBAL VARIABLES
 */
extern app_ctx_t g_sensorAppCtx;

extern bdb_appCb_t g_zbDemoBdbCb;

extern bdb_commissionSetting_t g_bdbCommissionSetting;

extern u8 SAMPLE_SENSOR_CB_CLUSTER_NUM;
extern const zcl_specClusterInfo_t g_sampleSensorClusterList[];
extern const af_simple_descriptor_t sampleSensor_simpleDesc;

/* Attributes */
extern zcl_basicAttr_t g_zcl_basicAttrs;
extern zcl_identifyAttr_t g_zcl_identifyAttrs;
extern zcl_iasZoneAttr_t g_zcl_iasZoneAttrs;
extern zcl_pollCtrlAttr_t g_zcl_pollCtrlAttrs;

#define zcl_iasZoneAttrGet()	&g_zcl_iasZoneAttrs
#define zcl_pollCtrlAttrGet()	&g_zcl_pollCtrlAttrs

/**********************************************************************
 * FUNCTIONS
 */
void sampleSensor_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg);

status_t sampleSensor_basicCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t sampleSensor_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t sampleSensor_iasZoneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
#ifdef ZCL_POLL_CTRL
status_t sampleSensor_pollCtrlCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
void sampleSensor_zclCheckInStart(void);
#endif

void sampleSensor_leaveCnfHandler(void *p);
void sampleSensor_leaveIndHandler(void *p);
void sampleSensor_otaProcessMsgHandler(u8 evt, u8 status);


#endif /* _SAMPLE_CONTACT_SENSOR_H_ */
