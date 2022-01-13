/********************************************************************************************************
 * @file    zcl_onoff.h
 *
 * @brief   This is the header file for zcl_onoff
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
 *******************************************************************************************************/

#ifndef ZCL_ON_OFF_H
#define ZCL_ON_OFF_H



/** @addtogroup  TELINK_ZIGBEE_STACK TELINK ZigBee Stack
 *  @{
 */

/** @addtogroup  ZCL_Module ZCL
 *  @{
 */
 
/** @addtogroup  ZCL_CLUSTER_ONOFF ONOFF Cluster
 *  @{
 */

/** @addtogroup  ZCL_ONOFF_Constant ONOFF Cluster Constants
 *  @{
 */

/** @addtogroup zcl_onoff_cmdId ONOFF Command Ids
 * @{
 */
#define ZCL_CMD_ONOFF_OFF                                 0x00
#define ZCL_CMD_ONOFF_ON                                  0x01
#define ZCL_CMD_ONOFF_TOGGLE                              0x02
#define ZCL_CMD_OFF_WITH_EFFECT                           0x40
#define ZCL_CMD_ON_WITH_RECALL_GLOBAL_SCENE               0x41
#define ZCL_CMD_ON_WITH_TIMED_OFF                         0x42
/** @} end of group zcl_onoff_cmdId */

/** @addtogroup zcl_onoff_attrId ONOFF Attribute Ids
 * @{
 */
#define ZCL_ATTRID_ONOFF                                  0x0000
#define ZCL_ATTRID_GLOBAL_SCENE_CONTROL                   0x4000
#define ZCL_ATTRID_ON_TIME                                0x4001
#define ZCL_ATTRID_OFF_WAIT_TIME                          0x4002
#define ZCL_ATTRID_START_UP_ONOFF						  0x4003
/** @} end of group zcl_onoff_attrId */



/** @addtogroup zcl_onoff_state ONOFF States
 * @{
 */
#define ZCL_ONOFF_STATUS_OFF                              0x00
#define ZCL_ONOFF_STATUS_ON                               0x01
#define ZCL_ONOFF_STATUS_DELAYED_OFF                      0x02
#define ZCL_ONOFF_STATUS_TIMED_ON                         0x03
/** @} end of group zcl_onoff_state */


/** @addtogroup zcl_off_with_effect effect identifiers
 * @{
 */
#define ZCL_OFF_EFFECT_DELAYED_ALL_OFF                    0x00
#define ZCL_OFF_EFFECT_DYING_LIGHT                        0x01
/** @} end of group zcl_off_with_effect */


#define ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF				  0x00
#define ZCL_START_UP_ONOFF_SET_ONOFF_TO_ON				  0x01
#define ZCL_START_UP_ONOFF_SET_ONOFF_TOGGLE				  0x02
#define ZCL_START_UP_ONOFF_SET_ONOFF_TO_PREVIOUS		  0xFF

/** @} end of group ZCL_ONOFF_Constant */



/**
 *  @brief  Definition format for ZCL_CMD_OFF_WITH_EFFECT command
 */
typedef struct{
	u8 effectId;
	u8 effectVariant;
}zcl_onoff_offWithEffectCmd_t;

typedef union{
	struct{
		u8 acceptOnlyWhenOn:1;
		u8 reserved:7;
	}bits;
	u8 onOffCtrl;
}zcl_onoffCtrl_t;

/**
 *  @brief  Definition format for command of "ZCL_CMD_ON_WITH_TIMED_OFF"
 */
typedef struct{
	zcl_onoffCtrl_t onOffCtrl;
	u16 onTime;
	u16 offWaitTime;
}zcl_onoff_onWithTimeOffCmd_t;

/**
 *  @brief  Definition format for on/off command payload
 */
typedef union{
	zcl_onoff_offWithEffectCmd_t	offWithEffect;
	zcl_onoff_onWithTimeOffCmd_t	onWithTimeOff;
}zcl_onoff_cmdPayload_t;

#if 0
/** @addtogroup  ZCL_ONOFF_Callbacks ONOFF Cluster Callbacks
 *  @{
 */

/**
 *  @brief  Definition for onoff command callback function type
 */
typedef void (*zcl_OnOffCb_t)(u8 cmdId, zcl_onoff_cmdPayload_t *cmdPayload);

/**
 *  @brief  Structure definition for commands callback functions in OnOff cluster
 */
typedef struct {
	zcl_OnOffCb_t      onOffCbFunc;
} zcl_onOff_AppCallbacks_t;


/** @} end of group ZCL_ONOFF_Callbacks */


/**
 *  @brief  onoff Attributes
 */
typedef struct{
#ifdef ZCL_ATTR_ON_TIME_ENABLE
	u16 onTime;
#endif
#ifdef ZCL_ATTR_OFF_WAIT_TIME_ENABLE
	u16 offWaitTime;
#endif
#ifdef ZCL_ATTR_ONOFF_ENABLE
	u8 onOff;
#endif
#ifdef ZCL_ATTR_GLOBAL_SCENE_CONTROL_ENABLE
	u8 globalSceneControl;
#endif
#ifdef ZCL_ATTR_START_UP_ONOFF_ENABLE
	u8 startUpOnOff;
#endif
}zcl_onoff_attr_t;

/** @addtogroup  ZCL_ONOFF_Variables ONOFF Cluster Variables
 *  @{
 */

/**
 *  @brief  External variable for onoff Attribute
 */
extern zcl_onoff_attr_t 					g_zclOnOffAttr;
#define zcl_onoffAttrGet()					&g_zclOnOffAttr

#define ZCL_ONOFF_ONTIME_SET(v)			g_zclOnOffAttr.onTime = (v)
#define ZCL_ONOFF_ONTIME_GET()			g_zclOnOffAttr.onTime

#define ZCL_ONOFF_GLBSCENEEN_SET(v)		g_zclOnOffAttr.globalSceneControl = (v)
#define ZCL_ONOFF_GLBSCENEEN_GET()		g_zclOnOffAttr.globalSceneControl

#define ZCL_ONOFF_ONOFF_SET(v)			g_zclOnOffAttr.onOff = (v)
#define ZCL_ONOFF_ONOFF_GET()			g_zclOnOffAttr.onOff

#define ZCL_ONOFF_OFFWAITTIME_SET(v)	g_zclOnOffAttr.offWaitTime = (v)
#define ZCL_ONOFF_OFFWAITTIME_GET()		g_zclOnOffAttr.offWaitTime

/**
 *  @brief  External variable for onoff Attribute table
 */
extern const zclAttrInfo_t onoff_attrTbl[];

extern const u8 zcl_onOff_attrNum;

/** @} end of group ZCL_ONOFF_Variables */
#endif

/** @addtogroup  ZCL_ONOFF_Fountions ONOFF Cluster APIs
 *  @{
 */

/**
 * @brief       This function register ONOFF cluster server to ZCL. Only after registered,
 *              onoff attributes could be used.
 *
 * @param[in]   cb       - pointer to the App layer command callbacks
 *
 * @return      Status
 */
status_t zcl_onOff_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);


/**
 * @brief       API to send ON command in ONOFF cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 *
 * @return      status_t
 */
status_t zcl_onOff_on(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_onOff_onCmd(a,b,c)	(zcl_onOff_on((a), (b), (c), ZCL_SEQ_NUM))

/**
 * @brief       API to send OFF command in ONOFF cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 *
 * @return      status_t
 */
status_t zcl_onOff_off(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_onOff_offCmd(a,b,c)	(zcl_onOff_off((a), (b), (c), ZCL_SEQ_NUM))

/**
 * @brief       API to send TOGGLE command in ONOFF cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 *
 * @return      status_t
 */
status_t zcl_onOff_toggle(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_onOff_toggleCmd(a,b,c)	(zcl_onOff_toggle((a), (b), (c), ZCL_SEQ_NUM))

/**
 * @brief       API to send OFF With Effect command in ONOFF cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]	pOffWithEffect
 * @return      status_t
 */
status_t zcl_onOff_offWithEffect(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_onoff_offWithEffectCmd_t *pOffWithEffect);
#define zcl_onOff_offWithEffectCmd(a,b,c,d)	(zcl_onOff_offWithEffect((a), (b), (c), ZCL_SEQ_NUM, (d)))

/**
 * @brief       API to send On with Recall Global Scene command in ONOFF cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 *
 * @return      None
 */
status_t zcl_onOff_onWithRecallGlobalScene(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_onOff_onWithRecallGlobalSceneCmd(a,b,c)	(zcl_onOff_onWithRecallGlobalScene((a), (b), (c), ZCL_SEQ_NUM))

/**
 * @brief       API to send On with Timed Off command in ONOFF cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   pOnWithTimedOff
 * @return      None
 */
status_t zcl_onOff_onWithTimedOff(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_onoff_onWithTimeOffCmd_t *pOnWithTimedOff);
#define zcl_onOff_onWithTimedOffCmd(a,b,c,d)	(zcl_onOff_onWithTimedOff((a), (b), (c), ZCL_SEQ_NUM, (d)))




/** @} end of group ZCL_ONOFF_Fountions */

/** @} end of group ZCL_CLUSTER_ONOFF */

/** @} end of group ZCL_Module */

/** @} end of group TELINK_ZIGBEE_STACK */

#endif	/* ZCL_ON_OFF_H */
