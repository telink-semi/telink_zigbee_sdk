/********************************************************************************************************
 * @file     zcl_identify.h
 *
 * @brief	 APIs statement for group cluster
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
 
/** @addtogroup  ZCL_CLUSTER_IDENTIFY IDENTIFY Cluster
 *  @{
 */

/** @addtogroup  ZCL_IDENTIFY_Constant IDENTIFY Cluster Constants
 *  @{
 */

/** @addtogroup zcl_identify_cmdId IDENTIFY Command Ids
 * @{
 */

// Client generated command
#define ZCL_CMD_IDENTIFY                                  0x00
#define ZCL_CMD_IDENTIFY_QUERY                            0x01
#define ZCL_CMD_TRIGGER_EFFECT                            0x40

// Server generated command
#define ZCL_CMD_IDENTIFY_QUERY_RSP                        0x00

/** @} end of group zcl_identify_cmdId */


/** @addtogroup zcl_identify_attrId IDENTIFY Attribute Ids
 * @{
 */
#define ZCL_ATTRID_IDENTIFY_TIME                          0x0000
/** @} end of group zcl_identify_attrId */

#define DEFAULT_IDENTIFY_DURATION						  5
/** @} end of group ZCL_IDENTIFY_Constant */


// Values of the trigger effect identifier Field
typedef enum {
    IDENTIFY_EFFECT_BLINK            = 0x00,
    IDENTIFY_EFFECT_BREATHE          = 0x01,
    IDENTIFY_EFFECT_OKAY             = 0x02,
    IDENTIFY_EFFECT_CHANNEL_CHANGE   = 0x0b,
    IDENTIFY_EFFECT_FINISH_EFFECT    = 0xfe,
    IDENTIFY_EFFECT_STOP_EFFECT      = 0xff
} effectId_t;

/**
 *  @brief  Definition format for trigger effect command
 */
typedef struct {
	u8 effectId;           //!< effect identifier
	u8 effectVariant;      //!< effect Variant
} zcl_triggerEffect_t;

/**
 *  @brief  Definition format for identify command
 */
typedef struct {
	u16 identifyTime;
} zcl_identifyCmd_t;

/**
 *  @brief  Definition format for identify response command
 */
typedef struct {
	u16 timeout;
} zcl_identifyRspCmd_t;


/**
 *  @brief  Definition format for identify command payload
 */
typedef union{
	zcl_identifyCmd_t				identify;
	zcl_identifyRspCmd_t			identifyRsp;
	zcl_triggerEffect_t				triggerEffect;
}zcl_identify_cmdPayload_t;


#if 0
/** @addtogroup  ZCL_IDENTIFY_Callbacks IDENTIFY Cluster Callbacks
 *  @{
 */

/**
 *  @brief  Definition for identify command callback format
 */
typedef void (*zcl_identifyCb_t)(u8 endpoint, u16 srcAddr, u16 identifyTime);

/**
 *  @brief  Definition for trigger command callback format
 */
typedef void (*zcl_triggerEffectCb_t)(apsdeDataInd_t *pApsdeInd, zcl_triggerEffect_t *pTriggerEffect);

/**
 *  @brief  Definition for identify command callback format
 */
typedef void (*zcl_identifyQueryRspCb_t)(apsdeDataInd_t *pApsdeInd, u16 timeout);



/**
 *  @brief  Structure definition for commands callback functions in Level cluster
 */
typedef struct {
	zcl_identifyCb_t    		identifyCbFunc;
	zcl_triggerEffectCb_t	    triggerEffectCbFunc;
	zcl_identifyQueryRspCb_t	identifyQueryRspCbFunc;
} zcl_identify_AppCallbacks_t;

/** @} end of group ZCL_IDENTIFY_Callbacks */


/** @addtogroup  ZCL_IDENTIFY_Variables IDENTIFY Cluster Variables
 *  @{
 */

/**
 *  @brief  External variable for identifyTime Attribute
 */
extern u16 zcl_attr_identifyTime;
/**
 *  @brief  External variable for identify Attribute Table
 */
extern const zclAttrInfo_t identify_attrTbl[];

extern const u8 zcl_identify_attrNum;

/** @} end of group ZCL_IDENTIFY_Variables */
#endif


/** @addtogroup  ZCL_IDENTIFY_Fountions IDENTIFY Cluster APIs
 *  @{
 */

/**
 * @brief       This function register IDENTIFY cluster server to ZCL. Only after registered,
 *              identify attributes could be used.
 *
 * @param[in]   cb       - pointer to the App layer command callbacks
 *
 * @return      Status
 */
status_t zcl_identify_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

/**
 * @brief       API to send Identify command in IDENTIFY cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   identifyTime - specified the identify last for
 *
 * @return      None
 */
status_t zcl_identify_identify(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u16 identifyTime, u32 manuCode);
#define zcl_identify_identifyCmd(a,b,c,d,e)	(zcl_identify_identify((a), (b), (c), ZCL_SEQ_NUM,(d), (e)))

/**
 * @brief       API to send Identify Query command in IDENTIFY cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 *
 * @return      None
 */
status_t zcl_identify_identifyQuery(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_identify_identifyQueryCmd(a,b,c)	(zcl_identify_identifyQuery((a), (b), (c), ZCL_SEQ_NUM))

/**
 * @brief       API to send trigger command in IDENTIFY cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   effectId - effect identifier
 * @param[in]   effectVariant - effect variant
 *
 * @return      None
 */
status_t zcl_identify_triggerEffect(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 effectId, u8 effectVariant);
#define zcl_identify_sendTriggerCmd(a,b,c,d,e)	zcl_identify_triggerEffect((a), (b), (c), ZCL_SEQ_NUM, (d), (e))


extern void zcl_identify_commissioningIdentify(apsdeDataInd_t *pApsdeInd, u16 identifyTime);

/** @} end of group ZCL_IDENTIFY_Fountions */

/** @} end of group ZCL_CLUSTER_IDENTIFY */

/** @} end of group ZCL_Module */

/** @} end of group TELINK_ZIGBEE_STACK */
