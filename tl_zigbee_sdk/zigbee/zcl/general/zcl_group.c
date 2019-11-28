/********************************************************************************************************
 * @file     zcl_group.c
 *
 * @brief	 APIs for group cluster
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


/**********************************************************************
 * INCLUDES
 */
#include "../zcl_include.h"


#ifdef ZCL_GROUP
/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * LOCAL TYPES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */
static status_t zcl_group_cmdHandler(zclIncoming_t *pInMsg);


_CODE_ZCL_ status_t zcl_group_register(u8 endpoint, u8 arrtNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
    return zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_GROUPS, arrtNum, attrTbl, zcl_group_cmdHandler, cb);
}


_CODE_ZCL_ status_t zcl_group_add(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u16 groupId, u8 *groupName)
{
	u8 buf[20];
	
	u8 *pBuf = buf;

	*pBuf++ = LO_UINT16(groupId);
	*pBuf++ = HI_UINT16(groupId);
	if(!groupName){
		*pBuf++ = 0;
	}else{
		u8 len = groupName[0];
		len = (len > 15) ? 15 : len;
		*pBuf++ = len;
		memcpy(pBuf, &groupName[1], len);
		pBuf += len;
	}
	
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GROUPS, ZCL_CMD_GROUP_ADD_GROUP, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_group_view(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u16 groupId)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GROUPS, ZCL_CMD_GROUP_VIEW_GROUP, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 2, (u8*)&groupId);
}

_CODE_ZCL_ status_t zcl_group_getMembership(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 groupCnt, u16 *groupList)
{
	u8 buf[40];
	buf[0] = groupCnt;
	memcpy(&buf[1], groupList, groupCnt*2);

	return  zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GROUPS, ZCL_CMD_GROUP_GET_MEMBERSHIP, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, groupCnt*2+1, buf);
}

_CODE_ZCL_ status_t zcl_group_remove(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u16 groupId)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GROUPS, ZCL_CMD_GROUP_REMOVE_GROUP, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 2, (u8*)&groupId);
}

_CODE_ZCL_ status_t zcl_group_removeAll(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GROUPS, ZCL_CMD_GROUP_REMOVE_ALL_GROUP, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_group_addIfIdentify(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u16 groupId, u8 *groupName)
{
	u8 buf[20];

	u8 *pBuf = buf;

	*pBuf++ = LO_UINT16(groupId);
	*pBuf++ = HI_UINT16(groupId);
	if(!groupName){
		*pBuf++ = 0;
	}else{
		u8 len = groupName[0];
		len = (len > 15) ? 15 : len;
		*pBuf++ = len;
		memcpy(pBuf, &groupName[1], len);
		pBuf += len;
	}

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GROUPS, ZCL_CMD_GROUP_ADD_GROUP_IF_IDF, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}



_CODE_ZCL_ status_t zcl_group_addGroupRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u16 groupId, u8 stauts)
{
	u8 buf[3];

	buf[0] = stauts;
	buf[1] = LO_UINT16(groupId);
	buf[2] = HI_UINT16(groupId);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GROUPS, ZCL_CMD_GROUP_ADD_GROUP_RSP, TRUE,
					ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, 3, buf);
}

_CODE_ZCL_ status_t zcl_group_viewGroupRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_viewGroupRsp_t *pViewGroupRsp)
{
	u8 buf[20];

	u8 *pBuf = buf;

	*pBuf++ = pViewGroupRsp->status;
	*pBuf++ = LO_UINT16(pViewGroupRsp->groupId);
	*pBuf++ = HI_UINT16(pViewGroupRsp->groupId);
	if(pViewGroupRsp->pGroupName == NULL){
		*pBuf++ = 0;//group name length set to zero
	}else{
		u8 len = pViewGroupRsp->pGroupName[0];
		len = (len > 15) ? 15 : len;
		*pBuf++ = len;
		memcpy(pBuf, &pViewGroupRsp->pGroupName[1], len);
		pBuf += len;
	}

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GROUPS, ZCL_CMD_GROUP_VIEW_GROUP_RSP, TRUE,
					ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_group_getGroupMembershipRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 capacity, u8 groupCnt, u16 *groupList)
{
	u8 buf[30];
	buf[0] = capacity;
	buf[1] = groupCnt;
	memcpy(&buf[2], groupList, groupCnt*2);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GROUPS, ZCL_CMD_GROUP_GET_MEMBERSHIP_RSP, TRUE,
					ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, groupCnt*2+2, buf);
}

_CODE_ZCL_ status_t zcl_group_removeGroupRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u16 groupId, u8 stauts)
{
	u8 buf[3];

	buf[0] = stauts;
	buf[1] = LO_UINT16(groupId);
	buf[2] = HI_UINT16(groupId);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GROUPS, ZCL_CMD_GROUP_REMOVE_GROUP_RSP, TRUE,
					ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, 3, buf);
}


_CODE_ZCL_ static status_t zcl_addGroupPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 endpoint = pApsdeInd->indInfo.dst_ep;

	aps_add_group_req_t addGroup;

	addGroup.group_addr = BUILD_U16(pInMsg->pData[0], pInMsg->pData[1]);
	addGroup.ep = pApsdeInd->indInfo.dst_ep;

	status = aps_me_group_add_req(&addGroup);
	if(status == APS_STATUS_DUPLICATE_ENTRY){
		status = ZCL_STA_DUPLICATE_EXISTS;
	}else if(status == APS_STATUS_TABLE_FULL){
		status = ZCL_STA_INSUFFICIENT_SPACE;
	}else{
		status = ZCL_STA_SUCCESS;
	}

	if(UNICAST_MSG(pApsdeInd)){
		epInfo_t dstEp;
		TL_SETSTRUCTCONTENT(dstEp, 0);

		dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
		dstEp.dstEp = pApsdeInd->indInfo.src_ep;
		dstEp.profileId = pApsdeInd->indInfo.profile_id;

		zcl_group_addGroupRsp(endpoint, &dstEp, TRUE, pInMsg->hdr.seqNum, addGroup.group_addr, status);

		status = ZCL_STA_CMD_HAS_RESP;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_viewGroupPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 endpoint = pApsdeInd->indInfo.dst_ep;

	if(UNICAST_MSG(pApsdeInd)){
		zcl_viewGroupRsp_t viewGroupRsp;

		viewGroupRsp.groupId = BUILD_U16(pInMsg->pData[0], pInMsg->pData[1]);

		aps_group_tbl_ent_t *pEntry = aps_group_search(viewGroupRsp.groupId, endpoint);
		if(!pEntry){
			status = ZCL_STA_NOT_FOUND;
		}

		viewGroupRsp.pGroupName = NULL;//not support
		viewGroupRsp.status = status;

		epInfo_t dstEp;
		TL_SETSTRUCTCONTENT(dstEp, 0);

		dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
		dstEp.dstEp = pApsdeInd->indInfo.src_ep;
		dstEp.profileId = pApsdeInd->indInfo.profile_id;

		zcl_group_viewGroupRsp(endpoint, &dstEp, TRUE, pInMsg->hdr.seqNum, &viewGroupRsp);

		status = ZCL_STA_CMD_HAS_RESP;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_getGroupMembershipPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 endpoint = pApsdeInd->indInfo.dst_ep;

	if(UNICAST_MSG(pApsdeInd)){
		u8 *pBuf = pInMsg->pData;
		u8 groupCnt = *pBuf++;
		u16 findGroupList[15];
		u8 findCnt = 0;

		if(groupCnt == 0){
			aps_group_list_get(&findCnt, findGroupList);
		}else{
			while(groupCnt--){
				u16 groupId = BUILD_U16(pBuf[0], pBuf[1]);

				aps_group_tbl_ent_t *pEntry = aps_group_search(groupId, endpoint);
				if(pEntry){
					findGroupList[findCnt++] = groupId;
				}
				pBuf += 2;
			}
		}

		epInfo_t dstEp;
		TL_SETSTRUCTCONTENT(dstEp, 0);

		dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
		dstEp.dstEp = pApsdeInd->indInfo.src_ep;
		dstEp.profileId = pApsdeInd->indInfo.profile_id;

		zcl_group_getGroupMembershipRsp(endpoint, &dstEp, TRUE, pInMsg->hdr.seqNum,
										(APS_GROUP_TABLE_SIZE - aps_group_entry_num_get()), findCnt, findGroupList);

		status = ZCL_STA_CMD_HAS_RESP;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_removeGroupPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 endpoint = pApsdeInd->indInfo.dst_ep;

	aps_delete_group_req_t removeGroup;
	removeGroup.ep = endpoint;
	removeGroup.group_addr = BUILD_U16(pInMsg->pData[0], pInMsg->pData[1]);

#ifdef ZCL_SCENE
	zcl_scene_removeAllSceneEntry(endpoint, removeGroup.group_addr, TRUE);
#endif

	status = aps_me_group_delete_req(&removeGroup);
	if(status == APS_STATUS_INVALID_GROUP){
		status = ZCL_STA_NOT_FOUND;
	}else{
		status = ZCL_STA_SUCCESS;
	}

	if(UNICAST_MSG(pApsdeInd)){
		epInfo_t dstEp;
		TL_SETSTRUCTCONTENT(dstEp, 0);

		dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
		dstEp.dstEp = pApsdeInd->indInfo.src_ep;
		dstEp.profileId = pApsdeInd->indInfo.profile_id;

		zcl_group_removeGroupRsp(endpoint, &dstEp, TRUE, pInMsg->hdr.seqNum, removeGroup.group_addr, status);

		status = ZCL_STA_CMD_HAS_RESP;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_removeAllGroupPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 endpoint = pApsdeInd->indInfo.dst_ep;

#ifdef ZCL_SCENE
	u8 groupCnt = 0;
	u16 groupList[15] = {0};

	aps_group_list_get(&groupCnt, groupList);

	for(u8 i = 0; i < groupCnt; i++){
		zcl_scene_removeAllSceneEntry(endpoint, groupList[i], ((i >= groupCnt) ? TRUE : FALSE));
	}
#endif

	aps_me_group_delete_all_req(endpoint);

	return status;
}

_CODE_ZCL_ static status_t zcl_addGroupIfIdentifyPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 endpoint = pApsdeInd->indInfo.dst_ep;
	
	u16 identifyTime = 0;
	u16 len = 0;

	if(zcl_getAttrVal(endpoint, ZCL_CLUSTER_GEN_IDENTIFY, ZCL_ATTRID_IDENTIFY_TIME, &len, (u8 *)&identifyTime) == ZCL_STA_SUCCESS){
		if(identifyTime){
			aps_add_group_req_t addGroup;

			addGroup.group_addr = BUILD_U16(pInMsg->pData[0], pInMsg->pData[1]);
			addGroup.ep = pApsdeInd->indInfo.dst_ep;

			status = aps_me_group_add_req(&addGroup);
			if(status == APS_STATUS_DUPLICATE_ENTRY){
				status = ZCL_STA_DUPLICATE_EXISTS;
			}else if(status == APS_STATUS_TABLE_FULL){
				status = ZCL_STA_INSUFFICIENT_SPACE;
			}else{
				status = ZCL_STA_SUCCESS;
			}
		}
	}

	return status;
}


_CODE_ZCL_ static status_t zcl_addGroupRspPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 *pData = pInMsg->pData;

	if(pInMsg->clusterAppCb){
		zclIncomingAddrInfo_t addrInfo;
		addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
		addrInfo.profileId = pApsdeInd->indInfo.profile_id;
		addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
		addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
		addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
		addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

		zcl_addGroupRsp_t addGroupRsp;
		addGroupRsp.status = *pData++;
		addGroupRsp.groupId = BUILD_U16(pData[0], pData[1]);
		
		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &addGroupRsp);
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_viewGroupRspPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 *pData = pInMsg->pData;

	if(pInMsg->clusterAppCb){
		zclIncomingAddrInfo_t addrInfo;
		addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
		addrInfo.profileId = pApsdeInd->indInfo.profile_id;
		addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
		addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
		addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
		addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

		zcl_viewGroupRsp_t viewGroupRsp;
		viewGroupRsp.status = *pData++;
		viewGroupRsp.groupId = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		if(viewGroupRsp.status == ZCL_STA_SUCCESS){
			viewGroupRsp.pGroupName = pData;
		}else{
			viewGroupRsp.pGroupName = NULL;
		}

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &viewGroupRsp);
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_removeGroupRspPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 *pData = pInMsg->pData;

	if(pInMsg->clusterAppCb){
		zclIncomingAddrInfo_t addrInfo;
		addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
		addrInfo.profileId = pApsdeInd->indInfo.profile_id;
		addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
		addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
		addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
		addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

		zcl_removeGroupRsp_t removeGroupRsp;
		removeGroupRsp.status = *pData++;
		removeGroupRsp.groupId = BUILD_U16(pData[0], pData[1]);

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &removeGroupRsp);
	}

	return status;
}



_CODE_ZCL_ static status_t zcl_getGroupMembershipRspPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 *pData = pInMsg->pData;

	if(pInMsg->clusterAppCb){
		zclIncomingAddrInfo_t addrInfo;
		addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
		addrInfo.profileId = pApsdeInd->indInfo.profile_id;
		addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
		addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
		addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
		addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

		zcl_getGroupMembershipRsp_t getGroupMembershipRsp;
		getGroupMembershipRsp.capacity = *pData++;
		getGroupMembershipRsp.groupCnt = *pData++;
		getGroupMembershipRsp.pGroupLsit = pData;

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &getGroupMembershipRsp);
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_group_clientCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd)
	{
		case ZCL_CMD_GROUP_ADD_GROUP:
			status = zcl_addGroupPrc(pInMsg);
			break;
		case ZCL_CMD_GROUP_VIEW_GROUP:
			status = zcl_viewGroupPrc(pInMsg);
			break;
		case ZCL_CMD_GROUP_GET_MEMBERSHIP:
			status = zcl_getGroupMembershipPrc(pInMsg);
			break;
		case ZCL_CMD_GROUP_REMOVE_GROUP:
			status = zcl_removeGroupPrc(pInMsg);
			break;
		case ZCL_CMD_GROUP_REMOVE_ALL_GROUP:
			status = zcl_removeAllGroupPrc(pInMsg);
			break;
		case ZCL_CMD_GROUP_ADD_GROUP_IF_IDF:
			status = zcl_addGroupIfIdentifyPrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}

_CODE_ZCL_ u8 zcl_group_serverCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd)
	{
		case ZCL_CMD_GROUP_ADD_GROUP_RSP:
			status = zcl_addGroupRspPrc(pInMsg);
			break;
		case ZCL_CMD_GROUP_VIEW_GROUP_RSP:
			status = zcl_viewGroupRspPrc(pInMsg);
			break;
		case ZCL_CMD_GROUP_REMOVE_GROUP_RSP:
			status = zcl_removeGroupRspPrc(pInMsg);
			break;
		case ZCL_CMD_GROUP_GET_MEMBERSHIP_RSP:
			status = zcl_getGroupMembershipRspPrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_group_cmdHandler(zclIncoming_t *pInMsg)
{
	if(pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR){
		return zcl_group_clientCmdHandler(pInMsg);
	}else{
		return zcl_group_serverCmdHandler(pInMsg);
	}
}

#endif  /* ZCL_GROUP */







