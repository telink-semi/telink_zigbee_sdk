/********************************************************************************************************
 * @file    sampleLight.h
 *
 * @brief   This is the header file for sampleLight
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

#ifndef _SAMPLE_LIGHT_H_
#define _SAMPLE_LIGHT_H_



/**********************************************************************
 * CONSTANT
 */
#define SAMPLE_LIGHT_ENDPOINT   0x01
#define SAMPLE_TEST_ENDPOINT	0x02

/**********************************************************************
 * TYPEDEFS
 */
typedef struct{
	u8 keyType; /* ERTIFICATION_KEY or MASTER_KEY key for touch-link or distribute network
	 	 	 	 SS_UNIQUE_LINK_KEY or SS_GLOBAL_LINK_KEY for distribute network */
	u8 key[16];	/* the key used */
}app_linkKey_info_t;

typedef struct{
	ev_timer_event_t *timerLedEvt;
	u32 keyPressedTime;

	u16 ledOnTime;
	u16 ledOffTime;
	u8 	oriSta;		//original state before blink
	u8 	sta;		//current state in blink
	u8 	times;		//blink times
	u8  state;

	bool bdbFindBindFlg;
	bool lightAttrsChanged;

	app_linkKey_info_t tcLinkKey;
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
	u8	minLevel;
	u8	maxLevel;
	u8	options;
}zcl_levelAttr_t;

/**
 *  @brief Defined for color control cluster attributes
 */
typedef struct{
	u8	colorMode;
	u8	enhancedColorMode;
	u8	options;
	u8	numOfPrimaries;
	u16 colorCapabilities;
#if COLOR_RGB_SUPPORT
	u8	currentHue;
	u8	currentSaturation;
	u8	colorLoopActive;
	u8	colorLoopDirection;
#ifndef COLOR_X_Y_DISABLE
	u16 currentX;
	u16 currentY;
#endif
	u16	colorLoopTime;
	u16 colorLoopStartEnhancedHue;
	u16 colorLoopStoredEnhancedHue;
	u16 enhancedCurrentHue;
#endif
#if COLOR_CCT_SUPPORT
	u16 colorTemperatureMireds;
	u16 colorTempPhysicalMinMireds;
	u16 colorTempPhysicalMaxMireds;
	u16 coupleColorTempToLevelMinMireds;
	u16 startUpColorTemperatureMireds;
#endif
}zcl_lightColorCtrlAttr_t;

/**
 *  @brief Defined for saving on/off attributes
 */
typedef struct _attribute_packed_{
	u8	onOff;
	u8	startUpOnOff;
}zcl_nv_onOff_t;

/**
 *  @brief Defined for saving level attributes
 */
typedef struct _attribute_packed_{
	u8	curLevel;
	u8	startUpCurLevel;
}zcl_nv_level_t;

/**
 *  @brief Defined for saving color control attributes
 */
typedef struct _attribute_packed_{
	u8	colorMode;
	u8	enhancedColorMode;
#if COLOR_RGB_SUPPORT
	u8	currentHue;
	u8	currentSaturation;
#ifndef COLOR_X_Y_DISABLE
	u16 currentX;
	u16 currentY;
#endif
#endif
#if COLOR_CCT_SUPPORT
	u16	colorTemperatureMireds;
	u16	startUpColorTemperatureMireds;
#endif
}zcl_nv_colorCtrl_t;

/**********************************************************************
 * GLOBAL VARIABLES
 */
extern app_ctx_t gLightCtx;
extern bdb_commissionSetting_t g_bdbCommissionSetting;
extern bdb_appCb_t g_zbDemoBdbCb;


extern u8 SAMPLELIGHT_CB_CLUSTER_NUM;
extern const zcl_specClusterInfo_t g_sampleLightClusterList[];
extern const af_simple_descriptor_t sampleLight_simpleDesc;
#if AF_TEST_ENABLE
extern const af_simple_descriptor_t sampleTestDesc;
#endif

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

void sampleLight_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf);
void sampleLight_leaveIndHandler(nlme_leave_ind_t *pLeaveInd);
void sampleLight_otaProcessMsgHandler(u8 evt, u8 status);
bool sampleLight_nwkUpdateIndicateHandler(nwkCmd_nwkUpdate_t *pNwkUpdate);

void sampleLight_onOffUpdate(u8 cmd);

void zcl_sampleLightAttrsInit(void);
nv_sts_t zcl_onOffAttr_save(void);
nv_sts_t zcl_levelAttr_save(void);
nv_sts_t zcl_colorCtrlAttr_save(void);

#if AF_TEST_ENABLE
void afTest_rx_handler(void *arg);
void afTest_dataSendConfirm(void *arg);
#endif

#endif /* _SAMPLE_LIGHT_H_ */
