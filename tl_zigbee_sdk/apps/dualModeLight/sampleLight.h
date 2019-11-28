/********************************************************************************************************
 * @file     sampleLight.h
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

#ifndef _SAMPLE_LIGHT_H_
#define _SAMPLE_LIGHT_H_



/**********************************************************************
 * CONSTANT
 */
#define SAMPLE_LIGHT_ENDPOINT   0x01

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

	bool bdbFindBindFlg;
	bool lightAttrsChanged;
	u8	powerCntFacRst:1;
	u8  powerCntFacRst2SigMesh:7;

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
	u8	swBuildId[ZCL_BASIC_MAX_LENGTH];
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
 *  @brief Defined for group cluster attributes
 */
typedef struct{
	u8	nameSupport;
}zcl_groupAttr_t;

/**
 *  @brief Defined for scene cluster attributes
 */
typedef struct{
	u8	 sceneCount;
	u8	 currentScene;
	u8	 nameSupport;
	bool sceneValid;
	u16	 currentGroup;
}zcl_sceneAttr_t;

/**
 *  @brief Defined for on/off cluster attributes
 */
typedef struct{
	u16	 onTime;
	u16	 offWaitTime;
	u8	 startUpOnOff;
	bool onOff;
	bool globalSceneControl;
}zcl_onOffAttr_t;

/**
 *  @brief Defined for level cluster attributes
 */
typedef struct{
	u16	remainingTime;
	u8	curLevel;
	u8	startUpCurrentLevel;
}zcl_levelAttr_t;

/**
 *  @brief Defined for color control cluster attributes
 */
typedef struct{
	u8	colorMode;
	u8	options;
	u8	enhancedColorMode;
	u8	numOfPrimaries;
	u16 colorCapabilities;
#if COLOR_RGB_SUPPORT
	u8	currentHue;
	u8	currentSaturation;
	u8	colorLoopActive;
	u8	colorLoopDirection;
	u16	colorLoopTime;
	u16 colorLoopStartEnhancedHue;
	u16 colorLoopStoredEnhancedHue;
#elif COLOR_CCT_SUPPORT
	u16 colorTemperatureMireds;
	u16 colorTempPhysicalMinMireds;
	u16 colorTempPhysicalMaxMireds;
	u16 startUpColorTemperatureMireds;
#endif
}zcl_lightColorCtrlAttr_t;


/**
 *  @brief Defined for saving on/off attributes
 */
typedef struct {
	u8	onOff;
	u8	startUpOnOff;
}zcl_nv_onOff_t;

/**
 *  @brief Defined for saving level attributes
 */
typedef struct {
	u8	curLevel;
	u8	startUpCurLevel;
}zcl_nv_level_t;

/**
 *  @brief Defined for saving color control attributes
 */
typedef struct {
#if COLOR_RGB_SUPPORT
	u8	currentHue;
	u8	currentSaturation;
#elif COLOR_CCT_SUPPORT
	u16	colorTemperatureMireds;
	u16	startUpColorTemperatureMireds;
#endif
}zcl_nv_colorCtrl_t;

/**********************************************************************
 * GLOBAL VARIABLES
 */
extern app_ctx_t gLightCtx;

extern bdb_appCb_t g_zbDemoBdbCb;


extern u8 SAMPLELIGHT_CB_CLUSTER_NUM;
extern zcl_specClusterInfo_t g_sampleLightClusterList[];
extern const af_simple_descriptor_t sampleLight_simpleDesc;

/* Attributes */
extern zcl_basicAttr_t g_zcl_basicAttrs;
extern zcl_identifyAttr_t g_zcl_identifyAttrs;
extern zcl_groupAttr_t g_zcl_groupAttrs;
extern zcl_sceneAttr_t g_zcl_sceneAttrs;
extern zcl_onOffAttr_t g_zcl_onOffAttrs;
extern zcl_levelAttr_t g_zcl_levelAttrs;
extern zcl_lightColorCtrlAttr_t g_zcl_colorCtrlAttrs;

#define zcl_sceneAttrGet()		&g_zcl_sceneAttrs
#define zcl_onoffAttrGet()		&g_zcl_onOffAttrs
#define zcl_levelAttrGet()		&g_zcl_levelAttrs
#define zcl_colorAttrGet()		&g_zcl_colorCtrlAttrs

/**********************************************************************
 * FUNCTIONS
 */
void sampleLight_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg);

status_t sampleLight_basicCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t sampleLight_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t sampleLight_sceneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t sampleLight_onOffCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t sampleLight_levelCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t sampleLight_colorCtrlCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);

void sampleLight_leaveCnfHandler(void *p);
void sampleLight_leaveIndHandler(void *p);
void sampleLight_otaProcessMsgHandler(u8 evt, u8 status);

void sampleLight_onoff(u8 cmd);

void zcl_sampleLightAttrsInit(void);
nv_sts_t zcl_onOffAttr_save(void);
nv_sts_t zcl_levelAttr_save(void);
nv_sts_t zcl_colorCtrlAttr_save(void);


#if DUAL_MODE
void dualModeInit(void);
void dualModeRecovery(void);
void dualModeDisable(void);
#endif

#endif /* _SAMPLE_LIGHT_H_ */