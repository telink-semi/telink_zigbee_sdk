/********************************************************************************************************
 * @file     zcl_scene.h
 *
 * @brief	 APIs statement for scene cluster
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
 
/** @addtogroup  ZCL_CLUSTER_SCENE SCENE Cluster
 *  @{
 */

/** @addtogroup  ZCL_Scene_Constant SCENE Cluster Constants
 *  @{
 */

/**
 *  @brief  Size of ZCL scene table
 */
#ifndef ZCL_SCENE_TABLE_NUM
#define ZCL_SCENE_TABLE_NUM								8
#endif

/**
 *  @brief  Max size for ZCL scene name
 */
#define ZCL_MAX_SCENE_NAME_LEN                          16

/**
 *  @brief  Max size for ZCL scene extension field
 */
#define ZCL_MAX_SCENE_EXT_FIELD_SIZE                    20






/** @addtogroup zcl_scene_cmdId SCENE Command Ids
 * @{
 */
#define ZCL_CMD_SCENE_ADD_SCENE                           0x00
#define ZCL_CMD_SCENE_VIEW_SCENE                          0x01
#define ZCL_CMD_SCENE_REMOVE_SCENE                        0x02
#define ZCL_CMD_SCENE_REMOVE_ALL_SCENE                    0x03
#define ZCL_CMD_SCENE_STORE_SCENE                         0x04
#define ZCL_CMD_SCENE_RECALL_SCENE                        0x05
#define ZCL_CMD_SCENE_GET_SCENE_MEMSHIP                   0x06
#define ZCL_CMD_SCENE_ENHANCED_ADD_SCENE                  0x40
#define ZCL_CMD_SCENE_ENHANCED_VIEW_SCENE                 0x41
#define ZCL_CMD_SCENE_COPY_SCENE                          0x42

#define ZCL_CMD_SCENE_ADD_SCENE_RSP						  0x00
#define	ZCL_CMD_SCENE_VIEW_SCENE_RSP					  0x01
#define ZCL_CMD_SCENE_REMOVE_SCENE_RSP					  0x02
#define ZCL_CMD_SCENE_REMOVE_ALL_SCENE_RSP				  0x03
#define ZCL_CMD_SCENE_STORE_SCENE_RSP					  0x04
#define ZCL_CMD_SCENE_GET_SCENE_MEMSHIP_RSP				  0x06
#define ZCL_CMD_SCENE_ENHANCED_ADD_SCENE_RSP			  0x40
#define ZCL_CMD_SCENE_ENHANCED_VIEW_SCENE_RSP			  0x41
#define ZCL_CMD_SCENE_COPY_SCENE_RSP					  0x42

/** @} end of group zcl_SCENE_cmdId */

/** @addtogroup zcl_scene_attrId SCENE Attribute Ids
 * @{
 */
#define ZCL_ATTRID_SCENE_SCENE_COUNT                      0x0000
#define ZCL_ATTRID_SCENE_CURRENT_SCENE                    0x0001
#define ZCL_ATTRID_SCENE_CURRENT_GROUP                    0x0002
#define ZCL_ATTRID_SCENE_SCENE_VALID                      0x0003
#define ZCL_ATTRID_SCENE_NAME_SUPPORT                     0x0004
#define ZCL_ATTRID_SCENE_LAST_CONFIG_BY                   0x0005
/** @} end of group zcl_scene_attrId */

/** @} end of group ZCL_Scene_Constant */

/** @addtogroup  ZCL_SCENE_Types SCENE Cluster Types
 *  @{
 */

#define ZB_SCENE_CLUSTER_COPY_SCENE_MODE_ALL(mode) ( ((mode) & 0x01) == 0x01 )

/**
 *  @brief  Structure definition for scene table entry in scene cluster
 */
typedef struct {
    u16 groupId;                                //!< The group ID for which this scene applies
    u8 sceneId;                                 //!< The identifier, unique within this group, which is used to identify this scene.
    u8 sceneName[ZCL_MAX_SCENE_NAME_LEN];       //!< The name of the scene
    u16 transTime;                              //!< Seconds it will take for the device to change from its current state to the requested scene.
    u16 transTime100ms;							//!< todo - check if this causes any problems
    u8 extFieldLen;                             //!< The length of extension field
    u8 extField[ZCL_MAX_SCENE_EXT_FIELD_SIZE];  //!< The extension field
} zcl_sceneEntry_t;

/**
 *  @brief  Structure definition for add scene command in scene cluster
 */
typedef struct {
	zcl_sceneEntry_t scene;
} addScene_t;

/**
 *  @brief  Structure definition for view scene command in scene cluster
 */
typedef struct {
    u16 groupId;                        //!< group id
    u8 sceneId;                         //!< scene id
} viewScene_t;

/**
 *  @brief  Structure definition for remove scene command in scene cluster
 */
typedef struct {
    u16 groupId;                        //!< group id
    u8 sceneId;                         //!< scene id
} removeScene_t;

/**
 *  @brief  Structure definition for remove all scene command in scene cluster
 */
typedef struct {
    u16 groupId;                        //!< group id
} removeAllScene_t;

/**
 *  @brief  Structure definition for store scene command in scene cluster
 */
typedef struct {
    u16 groupId;                        //!< group id
    u8 sceneId;                         //!< scene id
} storeScene_t;

/**
 *  @brief  Structure definition for recall scene command in scene cluster
 */
typedef struct {
    u16 groupId;                        //!< group id
    u16 transTime;						//!< transition time
    u8 sceneId;                         //!< scene id
} recallScene_t;

/**
 *  @brief  Structure definition for get scene member ship command in scene cluster
 */
typedef struct {
    u16 groupId;                        //!< group id
} getSceneMemship_t;

/**
 *  @brief  Structure definition for copy scene  command in scene cluster
 */
typedef struct {
    u8 mode;                        //!< mode (all scenes in group or particular group )
    u16 groupIdFrom;                //!< group id from
    u8 sceneIdFrom;                 //!< scene id from
    u16 groupIdTo;                  //!< group id to
    u8 sceneIdTo;                   //!< scene id to
} copyScene_t;


/**
 *  @brief  Structure definition for add scene response in scene cluster
 */
typedef struct {
    u8 status;                          //!< status
    u16 groupId;                        //!< group id
    u8 sceneId;                         //!< scene id
} addSceneRsp_t;

/**
 *  @brief  Structure definition for view scene response in scene cluster
 */
typedef struct {
    u8 status;                          //!< status
    zcl_sceneEntry_t scene;				//!< scene
} viewSceneRsp_t;

/**
 *  @brief  Structure definition for remove scene response in scene cluster
 */
typedef struct {
    u8 status;                          //!< status
    u16 groupId;                        //!< group id
    u8 sceneId;                         //!< scene id
} removeSceneRsp_t;

/**
 *  @brief  Structure definition for remove all scene response in scene cluster
 */
typedef struct {
    u8 status;                          //!< status
    u16 groupId;                        //!< group id
} removeAllSceneRsp_t;

/**
 *  @brief  Structure definition for store scene response in scene cluster
 */
typedef struct {
    u8 status;                          //!< status
    u16 groupId;                        //!< group id
    u8 sceneId;                         //!< scene id
} storeSceneRsp_t;

/**
 *  @brief  Structure definition for get scene membership response in scene cluster
 */
typedef struct {
    u8 status;                          //!< status
    u8 capacity;                        //!< capacity
    u16 groupId;                        //!< group id
    u8 sceneCnt;                        //!< scene count
    u8 *sceneList;                    	//!< scene list
} getSceneMemRsp_t;

/**
 *  @brief  Structure definition for copy scene response in scene cluster
 */
typedef struct {
    u8 status;                          //!< status
    u16 groupIdFrom;                    //!< group id from
    u8 sceneIdFrom;                     //!< scene id from
} copySceneRsp_t;


/**
 *  @brief  Definition format for scene command payload
 */
typedef union{
	addSceneRsp_t 		addSceneRsp;
	viewSceneRsp_t 		viewSceneRsp;
	removeSceneRsp_t 	removeSceneRsp;
	removeAllSceneRsp_t removeAllSceneRsp;
	storeSceneRsp_t 	storeSceneRsp;
	getSceneMemRsp_t 	getSceneMembershipRsp;
	copySceneRsp_t 		copySceneRsp;
}zcl_scene_cmdPayload_t;


/** @} end of group ZCL_SCENE_Types */

#if 0
/** @addtogroup  ZCL_SCENE_Callbacks SCENE Cluster Callbacks
 *  @{
 */
typedef void (*zcl_storeSceneCb_t)(apsdeDataInd_t *pApsdeInd, zcl_sceneEntry_t *pScene);
typedef void (*zcl_recallSceneCb_t)(apsdeDataInd_t *pApsdeInd, zcl_sceneEntry_t *pScene);
typedef void (*zcl_addSceneRspCb_t)(apsdeDataInd_t *pApsdeInd, u8 cmdId, addSceneRsp_t *pAddSceneRsp);
typedef void (*zcl_viewSceneRspCb_t)(apsdeDataInd_t *pApsdeInd, u8 cmdId, viewSceneRsp_t *pViewSceneRsp);
typedef void (*zcl_removeSceneRspCb_t)(apsdeDataInd_t *pApsdeInd, removeSceneRsp_t *pRemoveSceneRsp);
typedef void (*zcl_removeAllSceneRspCb_t)(apsdeDataInd_t *pApsdeInd, removeAllSceneRsp_t *pRemoveAllSceneRsp);
typedef void (*zcl_storeSceneRspCb_t)(apsdeDataInd_t *pApsdeInd, storeSceneRsp_t *pStoreSceneRsp);
typedef void (*zcl_getSceneMembershipRspCb_t)(apsdeDataInd_t *pApsdeInd, getSceneMemRsp_t *pGetSceneMembershipRsp);
typedef void (*zcl_copySceneRspCb_t)(apsdeDataInd_t *pApsdeInd, copySceneRsp_t *pCopySceneRsp);

/**
 *  @brief  Structure definition for commands callback functions in Scene cluster
 */
typedef struct {
	zcl_storeSceneCb_t     			storeSceneCbFunc;
	zcl_recallSceneCb_t     		recallSceneCbFunc;
	zcl_addSceneRspCb_t     		addSceneRspCbFunc;
	zcl_viewSceneRspCb_t     		viewSceneRspCbFunc;
	zcl_removeSceneRspCb_t     		removeSceneRspCbFunc;
	zcl_removeAllSceneRspCb_t     	removeAllSceneRspCbFunc;
	zcl_storeSceneRspCb_t     		storeSceneRspCbFunc;
	zcl_getSceneMembershipRspCb_t   getSceneMembershipRspCbFunc;
	zcl_copySceneRspCb_t     		copySceneRspCbFunc;
} zcl_scene_AppCallbacks_t;

/** @} end of group ZCL_SCENE_Callbacks */


/** @addtogroup  ZCL_SCENE_Variables Scene Cluster Variables
 *  @{
 */
/**
 *  @brief  Structure definition for scene management information
 */
typedef struct {
    u8 sceneCnt;                                //!< the number of  scenes currently in the device's scene table
    u8 currentScene;                            //!< the Scene ID of the scene last invoked
    u16 currentGroup;                           //!< the Group ID  of the scene last invoked
    u8 sceneValid;                              //!< indicates whether the state of the device
    u8 nameSupport;                             //!< indicates whether or not scene names are supported
    addrExt_t lastConfigBy;                     //!< Specifies the IEEE address of the device that last configured the scene table
    zcl_sceneEntry_t scene_table[ZCL_SCENE_TABLE_NUM];   //!< Store information for each scene capable of being invoked on a device
} zcl_sceneAttr_t;
/**
 *  @brief  External variable for scene Attribute
 */
extern zcl_sceneAttr_t zcl_attr_scene;

#define zcl_sceneAttrGet()					&zcl_attr_scene

/**
 *  @brief  External variable for scene Attribute table
 */
extern const zclAttrInfo_t scene_attrTbl[];

extern const u8 zcl_scene_attrNum;

/** @} end of group ZCL_Scene_Variables */
#endif

/**
 *  @brief  Structure definition for scene table
 */
typedef struct {
	u8 used;
	u8 endpoint;
	zcl_sceneEntry_t scene;
} zcl_sceneTable_t;

extern zcl_sceneTable_t g_zcl_sceneTab[];

/** @addtogroup  ZCL_Scene_Fountions SCENE Cluster APIs
 *  @{
 */
/**
 * @brief       This function register SCENE cluster server to ZCL. Only after registered,
 *              scene attributes could be used.
 *
 * @param[in]   cb       - pointer to the App layer command callbacks
 *
 * @return      Status
 */
status_t zcl_scene_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);


/**
 * @brief       API to send add scene command in SCENE cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   cmdId - ZCL_CMD_SCENE_ADD_SCENE / ZCL_CMD_SCENE_ENHANCED_ADD_SCENE
 * @param[in]   pAddScene
 *
 * @return      status_t
 */
status_t zcl_scene_addScene(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 cmdId, addScene_t *pAddScene);
#define zcl_scene_addSceneCmd(a,b,c,d)				(zcl_scene_addScene((a), (b), (c), ZCL_SEQ_NUM, ZCL_CMD_SCENE_ADD_SCENE, (d)))
#define zcl_scene_enhancedAddSceneCmd(a,b,c,d)		(zcl_scene_addScene((a), (b), (c), ZCL_SEQ_NUM, ZCL_CMD_SCENE_ENHANCED_ADD_SCENE, (d)))

/**
 * @brief       API to send view scene command in SCENE cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   cmdId -  ZCL_CMD_SCENE_VIEW_SCENE / ZCL_CMD_SCENE_ENHANCED_VIEW_SCENE
 * @param[in]   pViewScene
 *
 * @return      status_t
 */
status_t zcl_scene_viewScene(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 cmdId, viewScene_t *pViewScene);
#define zcl_scene_viewSceneCmd(a,b,c,d)				(zcl_scene_viewScene((a), (b), (c), ZCL_SEQ_NUM, ZCL_CMD_SCENE_VIEW_SCENE, (d)))
#define zcl_scene_enhancedViewSceneCmd(a,b,c,d)		(zcl_scene_viewScene((a), (b), (c), ZCL_SEQ_NUM, ZCL_CMD_SCENE_ENHANCED_VIEW_SCENE, (d)))

/**
 * @brief       API to send remove scene command in SCENE cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   pRemoveScene
 *
 * @return      status_t
 */
status_t zcl_scene_removeScene(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, removeScene_t *pRemoveScene);
#define zcl_scene_removeSceneCmd(a,b,c,d)		(zcl_scene_removeScene((a), (b), (c), ZCL_SEQ_NUM, (d)))

/**
 * @brief       API to send remove all scene command in SCENE cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   pRemoveAllScene
 *
 * @return      status_t
 */
status_t zcl_scene_removeAllScene(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, removeAllScene_t *pRemoveAllScene);
#define zcl_scene_removeAllSceneCmd(a,b,c,d)		(zcl_scene_removeAllScene((a), (b), (c), ZCL_SEQ_NUM, (d)))

/**
 * @brief       API to send store scene command in SCENE cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   pStoreScene
 *
 * @return      status_t
 */
status_t zcl_scene_storeScene(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, storeScene_t *pStoreScene);
#define zcl_scene_storeSceneCmd(a,b,c,d)		(zcl_scene_storeScene((a), (b), (c), ZCL_SEQ_NUM, (d)))

/**
 * @brief       API to send recall scene command in SCENE cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   pRecallScene
 *
 * @return      status_t
 */
status_t zcl_scene_recallScene(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, recallScene_t *pRecallScene);
#define zcl_scene_recallSceneCmd(a,b,c,d)		(zcl_scene_recallScene((a), (b), (c), ZCL_SEQ_NUM, (d)))

/**
 * @brief       API to send get scene membership command in SCENE cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   pGetMembership
 *
 * @return      status_t
 */
status_t zcl_scene_getSceneMemship(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, getSceneMemship_t *pGetMembership);
#define zcl_scene_getSceneMemshipCmd(a,b,c,d)		(zcl_scene_getSceneMemship((a), (b), (c), ZCL_SEQ_NUM, (d)))

/**
 * @brief       API to send copy scene command in SCENE cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   pCopyScene
 *
 * @return      status_t
 */
status_t zcl_scene_copyScene(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, copyScene_t *pCopyScene);
#define zcl_scene_copySceneCmd(a,b,c,d)		(zcl_scene_copyScene((a), (b), (c), ZCL_SEQ_NUM, (d)))



extern void zcl_scene_removeAllSceneEntry(u8 endpoint, u16 groupId, bool updateNV);

/** @} end of group ZCL_SCENE_Fountions */

/** @} end of group ZCL_CLUSTER_SCENE */

/** @} end of group ZCL_Module */

/** @} end of group TELINK_ZIGBEE_STACK */
