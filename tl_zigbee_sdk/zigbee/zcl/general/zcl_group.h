/********************************************************************************************************
 * @file     zcl_group.h
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
 
/** @addtogroup  ZCL_CLUSTER_GROUP GROUP Cluster
 *  @{
 */

/** @addtogroup  ZCL_GROUP_Constant GROUP Cluster Constants
 *  @{
 */

/** @addtogroup zcl_group_cmdId GROUP Command Ids
 * @{
 */
#define ZCL_CMD_GROUP_ADD_GROUP                           0x00
#define ZCL_CMD_GROUP_VIEW_GROUP                          0x01
#define ZCL_CMD_GROUP_GET_MEMBERSHIP                      0x02
#define ZCL_CMD_GROUP_REMOVE_GROUP                        0x03
#define ZCL_CMD_GROUP_REMOVE_ALL_GROUP                    0x04
#define ZCL_CMD_GROUP_ADD_GROUP_IF_IDF                    0x05

#define ZCL_CMD_GROUP_ADD_GROUP_RSP                       0x00
#define ZCL_CMD_GROUP_VIEW_GROUP_RSP                      0x01
#define ZCL_CMD_GROUP_GET_MEMBERSHIP_RSP                  0x02
#define ZCL_CMD_GROUP_REMOVE_GROUP_RSP                    0x03
/** @} end of group zcl_group_cmdId */

/** @addtogroup zcl_group_attrId GROUP Attribute Ids
 * @{
 */
#define ZCL_ATTRID_GROUP_NAME_SUPPORT                     0x0000
/** @} end of group zcl_group_attrId */

/** @} end of group ZCL_GROUP_Constant */


/** @addtogroup  ZCL_GROUP_Types GROUP Cluster Types
 *  @{
 */

/**
 *  @brief  Definition format for add group command
 */
typedef struct {
	u16 groupId;
	u8 *pGroupName;
} zcl_addGroup_t;

/**
 *  @brief  Definition format for add group response command
 */
typedef struct {
	u8 status;
	u16 groupId;
} zcl_addGroupRsp_t;

/**
 *  @brief  Definition format for remove group response command
 */
typedef zcl_addGroupRsp_t zcl_removeGroupRsp_t;

/**
 *  @brief  Definition format for view group response command
 */
typedef struct {
	u8 status;
	u16 groupId;
	u8 *pGroupName;
} zcl_viewGroupRsp_t;

/**
 *  @brief  Definition format for get group membership response command
 */
typedef struct {
	u8 capacity;
	u8 groupCnt;
	u8 *pGroupLsit;
} zcl_getGroupMembershipRsp_t;

#if 0
/** @addtogroup  ZCL_GROUP_Callbacks GROUP Cluster Callbacks
 *  @{
 */

/**
 *  @brief  Definition for group command callback format
 */
typedef void (*zcl_addGroupRspCb_t)(apsdeDataInd_t *pApsdeInd, zcl_addGroupRsp_t *pAddGroupRsp);
typedef void (*zcl_viewGroupRspCb_t)(apsdeDataInd_t *pApsdeInd, zcl_viewGroupRsp_t *pViewGroupRsp);
typedef void (*zcl_removeGroupRspCb_t)(apsdeDataInd_t *pApsdeInd, zcl_removeGroupRsp_t *pRemoveGroupRsp);
typedef void (*zcl_getGroupMembershipRspCb_t)(apsdeDataInd_t *pApsdeInd, zcl_getGroupMembershipRsp_t *pGetGroupMembershipRsp);

/**
 *  @brief  Structure definition for commands callback functions in Group cluster
 */
typedef struct {
	zcl_addGroupRspCb_t     			addGroupRspCbFunc;
	zcl_viewGroupRspCb_t				viewGroupRspCbFunc;
	zcl_removeGroupRspCb_t     			removeGroupRspCbFunc;
	zcl_getGroupMembershipRspCb_t     	getGroupMembershipRspCbFunc;
} zcl_group_AppCallbacks_t;

/** @} end of group ZCL_GROUP_Callbacks */


/** @addtogroup  ZCL_GROUP_Variables GROUP Cluster Variables
 *  @{
 */

/**
 *  @brief  External variable for groupNameSupport Attribute
 */
extern u8 zcl_attr_group_nameSupport;
/**
 *  @brief  External variable for group Attribute table
 */
extern const zclAttrInfo_t group_attrTbl[];

extern const u8 zcl_group_attrNum;

/** @} end of group ZCL_GROUP_Variables */
#endif

/** @addtogroup  ZCL_GROUP_Fountions GROUP Cluster APIs
 *  @{
 */

/**
 * @brief       This function register GROUP cluster server to ZCL. Only after registered,
 *              group attributes could be used.
 *
 * @param[in]   cb       - pointer to the App layer command callbacks
 *
 * @return      Status
 */
status_t zcl_group_register(u8 endpoint, u8 arrtNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

/**
 * @brief       API to send Add command in Group cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   groupId - specified the group ID the dest node should add to
 * @param[in]   groupName - specified the group name
 *
 * @return      None
 */
status_t zcl_group_add(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u16 groupId, u8 *groupName);
#define zcl_group_addCmd(a,b,c,d,e)		(zcl_group_add((a), (b), (c), ZCL_SEQ_NUM, (d), (e)))

/**
 * @brief       API to send View command in Group cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   groupId - specified the group ID the dest node should add to
 *
 * @return      None
 */
status_t zcl_group_view(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u16 groupId);
#define zcl_group_viewCmd(a,b,c,d)		(zcl_group_view((a), (b), (c), ZCL_SEQ_NUM, (d)))

/**
 * @brief       API to send Remove command in Group cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   groupId - specified the group ID the dest node should remove from
 *
 * @return      None
 */
status_t zcl_group_remove(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u16 groupId);
#define zcl_group_removeCmd(a,b,c,d)		(zcl_group_remove((a), (b), (c), ZCL_SEQ_NUM, (d)))

/**
 * @brief       API to send Remove All command in Group cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 *
 * @return      None
 */
status_t zcl_group_removeAll(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_group_removeAllCmd(a,b,c)		(zcl_group_removeAll((a), (b), (c), ZCL_SEQ_NUM))

/**
 * @brief       API to send Get Membership command in Group cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   groupCnt - specified the count in group list
 * @param[in]   groupList - specified the group list
 *
 * @return      None
 */
status_t zcl_group_getMembership(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 groupCnt, u16 *groupList);
#define zcl_group_getMembershipCmd(a,b,c,d,e)		(zcl_group_getMembership((a), (b), (c), ZCL_SEQ_NUM, (d), (e)))

/**
 * @brief       API to send View command in Group cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   groupId - specified the group ID the dest node should add to
 *
 * @return      None
 */
status_t zcl_group_addIfIdentify(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u16 groupId, u8 *groupName);
#define zcl_group_addIfIdentifyCmd(a,b,c,d,e)		(zcl_group_addIfIdentify((a), (b), (c), ZCL_SEQ_NUM, (d), (e)))










/** @} end of group ZCL_GROUP_Fountions */

/** @} end of group ZCL_CLUSTER_GROUP */

/** @} end of group ZCL_Module */

/** @} end of group TELINK_ZIGBEE_STACK */
