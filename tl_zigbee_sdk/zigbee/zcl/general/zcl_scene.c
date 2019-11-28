/********************************************************************************************************
 * @file     zcl_scene.c
 *
 * @brief	 APIs for scene cluster
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


#ifdef ZCL_SCENE
/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * LOCAL TYPES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */
zcl_sceneTable_t g_zcl_sceneTab[ZCL_SCENE_TABLE_NUM];

/**********************************************************************
 * LOCAL FUNCTIONS
 */
void zcl_scene_sceneTabClear(void);
nv_sts_t zcl_scenesRestore(void);
nv_sts_t zcl_scenesSave(u8 endpoint);
void zcl_scene_updateSceneCntAttr(u8 endpoint);
static status_t zcl_scene_cmdHandler(zclIncoming_t *pInMsg);


_CODE_ZCL_ status_t zcl_scene_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
	u8 status = ZCL_STA_SUCCESS;

    if(zcl_scenesRestore() != NV_SUCC){
    	zcl_scene_sceneTabClear();
    }

    status = zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_SCENES, attrNum, attrTbl, zcl_scene_cmdHandler, cb);

    if(status == ZCL_STA_SUCCESS){
    	zcl_scene_updateSceneCntAttr(endpoint);
    }

    return status;
}


_CODE_ZCL_ nv_sts_t zcl_scenesSave(u8 endpoint)
{
	zcl_scene_updateSceneCntAttr(endpoint);

	return zcl_sceneTable_save();
}

_CODE_ZCL_ nv_sts_t zcl_scenesRestore(void)
{
	return zcl_sceneTable_restore();
}


_CODE_ZCL_ void zcl_scene_clearEntry(zcl_sceneTable_t *pSceneTab)
{
	if(pSceneTab){
		pSceneTab->used = 0;
		pSceneTab->endpoint = 0xfe;
		memset((u8 *)&pSceneTab->scene, 0, sizeof(zcl_sceneEntry_t));
	}
}

_CODE_ZCL_ void zcl_scene_sceneTabClear(void)
{
    for(u8 i = 0; i < ZCL_SCENE_TABLE_NUM; i++){
    	zcl_sceneTable_t *pSceneTab = &g_zcl_sceneTab[i];

    	zcl_scene_clearEntry(pSceneTab);
    }
}

_CODE_ZCL_ zcl_sceneTable_t *zcl_scene_getFreeEntry(void)
{
    for(u8 i = 0; i < ZCL_SCENE_TABLE_NUM; i++){
    	zcl_sceneTable_t *pSceneTab = &g_zcl_sceneTab[i];

        if(!pSceneTab->used){
        	return pSceneTab;
        }
    }
    return NULL;
}

_CODE_ZCL_ zcl_sceneTable_t *zcl_scene_findEntry(u8 endpoint, u16 groupId, u8 sceneId)
{
    for(u8 i = 0; i < ZCL_SCENE_TABLE_NUM; i++){
    	zcl_sceneTable_t *pSceneTab = &g_zcl_sceneTab[i];

        if(pSceneTab->used && (pSceneTab->endpoint == endpoint)){
            if((pSceneTab->scene.groupId == groupId) && (pSceneTab->scene.sceneId == sceneId)){
            	return pSceneTab;
            }
        }
    }
    return NULL;
}

_CODE_ZCL_ u8 zcl_scene_getSceneUsedNum(void)
{
	u8 cnt = 0;

    for(u8 i = 0; i < ZCL_SCENE_TABLE_NUM; i++){
    	zcl_sceneTable_t *pSceneTab = &g_zcl_sceneTab[i];

    	if(pSceneTab->used){
    		cnt++;
    	}
    }

    return cnt;
}

_CODE_ZCL_ void zcl_scene_updateSceneCntAttr(u8 endpoint)
{
	u16 attrLen = 0;
	u8 sceneCnt = 0;

	if(zcl_getAttrVal(endpoint, ZCL_CLUSTER_GEN_SCENES, ZCL_ATTRID_SCENE_SCENE_COUNT, &attrLen, (u8 *)&sceneCnt) == ZCL_STA_SUCCESS){
		sceneCnt = zcl_scene_getSceneUsedNum();

		zcl_setAttrVal(endpoint, ZCL_CLUSTER_GEN_SCENES, ZCL_ATTRID_SCENE_SCENE_COUNT, (u8 *)&sceneCnt);
	}
}

_CODE_ZCL_ u8 zcl_scene_findAllSceneByGroup(u8 endpoint, u16 groupId, u8 *sceneList)
{
	u8 cnt = 0;

	for(u8 i = 0; i < ZCL_SCENE_TABLE_NUM; i++){
		zcl_sceneTable_t *pSceneTab = &g_zcl_sceneTab[i];

		if(pSceneTab->used && (pSceneTab->endpoint == endpoint) && (pSceneTab->scene.groupId == groupId)){
			sceneList[cnt++] = pSceneTab->scene.sceneId;
		}
	}

	return cnt;
}

_CODE_ZCL_ status_t zcl_scene_addSceneEntry(u8 endpoint, zcl_sceneEntry_t *pScene)
{
	zcl_sceneTable_t *pSceneEntry = zcl_scene_findEntry(endpoint, pScene->groupId, pScene->sceneId);
	if(!pSceneEntry){
		pSceneEntry = zcl_scene_getFreeEntry();
		if(!pSceneEntry){
			/* scene table full */
			return ZCL_STA_INSUFFICIENT_SPACE;
		}
	}

	/* update or add new */
	pSceneEntry->used = 1;
	pSceneEntry->endpoint = endpoint;
	memcpy((u8 *)&pSceneEntry->scene, (u8 *)pScene, sizeof(zcl_sceneEntry_t));

	/* scenes save */
	zcl_scenesSave(endpoint);

	return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ status_t zcl_scene_removeSceneEntry(u8 endpoint, u16 groupId, u8 sceneId)
{
	zcl_sceneTable_t *pSceneEntry = zcl_scene_findEntry(endpoint, groupId, sceneId);
	if(pSceneEntry){
		zcl_scene_clearEntry(pSceneEntry);

		/* remove success, update scene table */
		zcl_scenesSave(endpoint);

		return ZCL_STA_SUCCESS;
	}else{
		return ZCL_STA_NOT_FOUND;
	}
}

_CODE_ZCL_ void zcl_scene_removeAllSceneEntry(u8 endpoint, u16 groupId, bool updateNV)
{
	bool find = FALSE;
	for(u8 i = 0; i < ZCL_SCENE_TABLE_NUM; i++){
		zcl_sceneTable_t *pSceneTab = &g_zcl_sceneTab[i];

		if(pSceneTab->used && (pSceneTab->endpoint == endpoint) && (pSceneTab->scene.groupId == groupId)){
			zcl_scene_clearEntry(pSceneTab);

			find = TRUE;
		}
	}

	if(find && updateNV){
		/* remove success, update scene table */
		zcl_scenesSave(endpoint);
	}
}

_CODE_ZCL_ status_t zcl_scene_copySceneEntry(u8 endpoint, copyScene_t *pCopyScene)
{
	u8 sceneCnt = 0;
	u8 sceneList[ZCL_SCENE_TABLE_NUM] = {0};
	zcl_sceneTable_t *pSceneEntry = NULL;
	zcl_sceneTable_t *pSceneToEntry = NULL;

	if(ZB_SCENE_CLUSTER_COPY_SCENE_MODE_ALL(pCopyScene->mode)){
		sceneCnt = zcl_scene_findAllSceneByGroup(endpoint, pCopyScene->groupIdFrom, sceneList);
		if(sceneCnt == 0){
			/* scene not found */
			return ZCL_STA_INVALID_FIELD;
		}
	}else{
		pSceneEntry = zcl_scene_findEntry(endpoint, pCopyScene->groupIdFrom, pCopyScene->sceneIdFrom);
		if(pSceneEntry){
			sceneCnt = 1;
			sceneList[0] = pCopyScene->sceneIdFrom;
		}else{
			/* scene not found */
			return ZCL_STA_INVALID_FIELD;
		}
	}

	u8 i = 0;
	u8 addCnt = 0;

	for(i = 0; i < sceneCnt; i++){
		if(zcl_scene_findEntry(endpoint, pCopyScene->groupIdTo, sceneList[i]) == NULL){
			addCnt++;
		}
	}

	if(addCnt > (ZCL_SCENE_TABLE_NUM - zcl_scene_getSceneUsedNum())){
		/* space not enough to copy */
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	/* copy scenes */
	for(i = 0; i < sceneCnt; i++){
		/* find source scene */
		pSceneEntry = zcl_scene_findEntry(endpoint, pCopyScene->groupIdFrom, sceneList[i]);
		if(pSceneEntry){
			u8 addSceneId = ZB_SCENE_CLUSTER_COPY_SCENE_MODE_ALL(pCopyScene->mode) ? sceneList[i] : pCopyScene->sceneIdTo;

			pSceneToEntry = zcl_scene_findEntry(endpoint, pCopyScene->groupIdTo, addSceneId);
			if(!pSceneToEntry){
				pSceneToEntry = zcl_scene_getFreeEntry();
				if(!pSceneToEntry){
					return ZCL_STA_INSUFFICIENT_SPACE;
				}
			}

			/* add or update */
			pSceneToEntry->used = 1;
			pSceneToEntry->endpoint = endpoint;
			memcpy((u8 *)&pSceneToEntry->scene, (u8 *)&pSceneEntry->scene, sizeof(zcl_sceneEntry_t));
		}
	}

	/* scenes save */
	zcl_scenesSave(endpoint);

	return ZCL_STA_SUCCESS;
}



_CODE_ZCL_ status_t zcl_scene_addScene(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 cmdId, addScene_t *pAddScene)
{
	u8 len = 5;//group id + scene id + transition time
	len += (1 + pAddScene->scene.sceneName[0]);
	len += pAddScene->scene.extFieldLen;

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

    *pBuf++ = LO_UINT16(pAddScene->scene.groupId);
    *pBuf++ = HI_UINT16(pAddScene->scene.groupId);
    *pBuf++ = pAddScene->scene.sceneId;
    *pBuf++ = LO_UINT16(pAddScene->scene.transTime);
    *pBuf++ = HI_UINT16(pAddScene->scene.transTime);
    *pBuf++ = pAddScene->scene.sceneName[0];
    memcpy(pBuf, &pAddScene->scene.sceneName[1], pAddScene->scene.sceneName[0]);
    pBuf += pAddScene->scene.sceneName[0];
    if(pAddScene->scene.extFieldLen){
    	memcpy(pBuf, pAddScene->scene.extField, pAddScene->scene.extFieldLen);
    	pBuf += pAddScene->scene.extFieldLen;
    }

	zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_SCENES, cmdId, TRUE,
			ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);

	ev_buf_free(buf);

	return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ status_t zcl_scene_viewScene(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 cmdId, viewScene_t *pViewScene)
{
    u8 buf[3];

    buf[0] = LO_UINT16(pViewScene->groupId);
    buf[1] = HI_UINT16(pViewScene->groupId);
    buf[2] = pViewScene->sceneId;

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_SCENES, cmdId, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 3, buf);
}

_CODE_ZCL_ status_t zcl_scene_removeScene(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, removeScene_t *pRemoveScene)
{
    u8 buf[3];

    buf[0] = LO_UINT16(pRemoveScene->groupId);
    buf[1] = HI_UINT16(pRemoveScene->groupId);
    buf[2] = pRemoveScene->sceneId;

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_SCENES, ZCL_CMD_SCENE_REMOVE_SCENE, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 3, buf);
}

_CODE_ZCL_ status_t zcl_scene_removeAllScene(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, removeAllScene_t *pRemoveAllScene)
{
    u8 buf[2];

    buf[0] = LO_UINT16(pRemoveAllScene->groupId);
    buf[1] = HI_UINT16(pRemoveAllScene->groupId);

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_SCENES, ZCL_CMD_SCENE_REMOVE_ALL_SCENE, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 2, buf);
}

_CODE_ZCL_ status_t zcl_scene_storeScene(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, storeScene_t *pStoreScene)
{
    u8 buf[3];

    buf[0] = LO_UINT16(pStoreScene->groupId);
    buf[1] = HI_UINT16(pStoreScene->groupId);
    buf[2] = pStoreScene->sceneId;

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_SCENES, ZCL_CMD_SCENE_STORE_SCENE, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 3, buf);
}

_CODE_ZCL_ status_t zcl_scene_recallScene(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, recallScene_t *pRecallScene)
{
    u8 buf[5];

    u8 *pBuf = buf;

    *pBuf++ = LO_UINT16(pRecallScene->groupId);
    *pBuf++ = HI_UINT16(pRecallScene->groupId);
    *pBuf++ = pRecallScene->sceneId;
    if(pRecallScene->transTime){
        *pBuf++ = LO_UINT16(pRecallScene->transTime);
        *pBuf++ = HI_UINT16(pRecallScene->transTime);
    }

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_SCENES, ZCL_CMD_SCENE_RECALL_SCENE, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_scene_getSceneMemship(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, getSceneMemship_t *pGetMembership)
{
    u8 buf[2];

    buf[0] = LO_UINT16(pGetMembership->groupId);
    buf[1] = HI_UINT16(pGetMembership->groupId);

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_SCENES, ZCL_CMD_SCENE_GET_SCENE_MEMSHIP, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 2, buf);
}

_CODE_ZCL_ status_t zcl_scene_copyScene(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, copyScene_t *pCopyScene)
{
    u8 buf[7];

    /* If source and destination are same, don't send the command */
    if((pCopyScene->groupIdFrom == pCopyScene->groupIdTo) && (pCopyScene->sceneIdFrom == pCopyScene->sceneIdTo)){
    	return ZCL_STA_INVALID_FIELD;
    }

    buf[0] = pCopyScene->mode;
    buf[1] = LO_UINT16(pCopyScene->groupIdFrom);
    buf[2] = HI_UINT16(pCopyScene->groupIdFrom);
    buf[3] = pCopyScene->sceneIdFrom;
    buf[4] = LO_UINT16(pCopyScene->groupIdTo);
    buf[5] = HI_UINT16(pCopyScene->groupIdTo);
    buf[6] = pCopyScene->sceneIdTo;

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_SCENES, ZCL_CMD_SCENE_COPY_SCENE, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 7, buf);
}

_CODE_ZCL_ status_t zcl_scene_sceneRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 cmdId, u8 status, u16 groupId, u8 sceneId)
{
    u8 buf[4];

    u8 *pBuf = buf;

    *pBuf++ = status;
    *pBuf++ = LO_UINT16(groupId);
    *pBuf++ = HI_UINT16(groupId);
    if(cmdId != ZCL_CMD_SCENE_REMOVE_ALL_SCENE_RSP){
    	*pBuf++ = sceneId;
    }

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_SCENES, cmdId, TRUE,
    				ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_scene_viewSceneRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo,
										   u8 cmdId, u8 status, u16 groupId, u8 sceneId, zcl_sceneEntry_t *pEntry)
{
	u8 len = 4;

	if(status == ZCL_STA_SUCCESS){
		if(!pEntry){
			return ZCL_STA_FAILURE;
		}

		len += 3;//transTime + scene name 1 byte
		len += pEntry->sceneName[0];
		len += pEntry->extFieldLen;
	}

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = status;
	*pBuf++ = LO_UINT16(groupId);
	*pBuf++ = HI_UINT16(groupId);
	*pBuf++ = sceneId;
	if(status == ZCL_STA_SUCCESS){
		u16 transTime = pEntry->transTime;
		if(cmdId == ZCL_CMD_SCENE_ENHANCED_VIEW_SCENE_RSP){
			transTime *= 10;
			transTime += pEntry->transTime100ms;
		}
		*pBuf++ = LO_UINT16(transTime);
		*pBuf++ = HI_UINT16(transTime);
		*pBuf++ = pEntry->sceneName[0];
		memcpy(pBuf, &pEntry->sceneName[1], pEntry->sceneName[0]);
		pBuf += pEntry->sceneName[0];
		memcpy(pBuf, pEntry->extField, pEntry->extFieldLen);
		pBuf += pEntry->extFieldLen;
	}

	zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_SCENES, cmdId, TRUE,
			ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);

	ev_buf_free(buf);

	return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ status_t zcl_scene_getSceneMembershipRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, getSceneMemRsp_t *pGetSceneMembershipRsp)
{
	u8 len = 4;

	if(pGetSceneMembershipRsp->status == ZCL_STA_SUCCESS){
		len++;//scene count;
		len += pGetSceneMembershipRsp->sceneCnt;
	}

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = pGetSceneMembershipRsp->status;
	*pBuf++ = pGetSceneMembershipRsp->capacity;
	*pBuf++ = LO_UINT16(pGetSceneMembershipRsp->groupId);
	*pBuf++ = HI_UINT16(pGetSceneMembershipRsp->groupId);
	if(pGetSceneMembershipRsp->status == ZCL_STA_SUCCESS){
		*pBuf++ = pGetSceneMembershipRsp->sceneCnt;
		memcpy(pBuf, pGetSceneMembershipRsp->sceneList, pGetSceneMembershipRsp->sceneCnt);
		pBuf += pGetSceneMembershipRsp->sceneCnt;
	}

	zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_SCENES, ZCL_CMD_SCENE_GET_SCENE_MEMSHIP_RSP, TRUE,
			ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);

	ev_buf_free(buf);

	return ZCL_STA_SUCCESS;
}


_CODE_ZCL_ static void zcl_addSceneParse(u8 endpoint, u8 *pData, u16 dataLen, addScene_t *pCmd)
{
	u8 *ptr = pData;

	pCmd->scene.groupId = BUILD_U16(pData[0], pData[1]);
	pData += 2;
	pCmd->scene.sceneId = *pData++;
	pCmd->scene.transTime = BUILD_U16(pData[0], pData[1]);
	pData += 2;

	u8 nameLen = *pData++;

	bool nameSupport = FALSE;
	u16 attrLen = 0;
	zcl_getAttrVal(endpoint, ZCL_CLUSTER_GEN_SCENES, ZCL_ATTRID_SCENE_NAME_SUPPORT, &attrLen, (u8 *)&nameSupport);

	if(nameSupport){
		pCmd->scene.sceneName[0] = (nameLen > (ZCL_MAX_SCENE_NAME_LEN - 1)) ? (ZCL_MAX_SCENE_NAME_LEN - 1) : nameLen;
		memcpy(&pCmd->scene.sceneName[1], pData, pCmd->scene.sceneName[0]);
	}

	pData += nameLen;

	pCmd->scene.extFieldLen = dataLen - (pData - ptr);
	if(pCmd->scene.extFieldLen){
		pCmd->scene.extFieldLen = (pCmd->scene.extFieldLen > ZCL_MAX_SCENE_EXT_FIELD_SIZE) ? ZCL_MAX_SCENE_EXT_FIELD_SIZE
																						   : pCmd->scene.extFieldLen;
		memcpy(pCmd->scene.extField, pData, pCmd->scene.extFieldLen);
	}
}

_CODE_ZCL_ static void zcl_viewSceneRspParse(u8 *pData, u16 dataLen, viewSceneRsp_t *pCmd)
{
	u8 *ptr = pData;

	pCmd->status = *pData++;
	pCmd->scene.groupId = BUILD_U16(pData[0], pData[1]);
	pData += 2;
	pCmd->scene.sceneId = *pData++;
	if(pCmd->status == ZCL_STA_SUCCESS){
		pCmd->scene.transTime = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		u8 nameLen = *pData++;
		pCmd->scene.sceneName[0] = (nameLen > (ZCL_MAX_SCENE_NAME_LEN - 1)) ? (ZCL_MAX_SCENE_NAME_LEN - 1) : nameLen;
		memcpy(&pCmd->scene.sceneName[1], pData, pCmd->scene.sceneName[0]);

		pData += nameLen;

		pCmd->scene.extFieldLen = dataLen - (pData - ptr);
		if(pCmd->scene.extFieldLen){
			pCmd->scene.extFieldLen = (pCmd->scene.extFieldLen > ZCL_MAX_SCENE_EXT_FIELD_SIZE) ? ZCL_MAX_SCENE_EXT_FIELD_SIZE
																							   : pCmd->scene.extFieldLen;
			memcpy(pCmd->scene.extField, pData, pCmd->scene.extFieldLen);
		}
	}
}

_CODE_ZCL_ static void zcl_getSceneMembershipRspParse(u8 *pData, getSceneMemRsp_t *pCmd)
{
	pCmd->status = *pData++;
	pCmd->capacity = *pData++;
	pCmd->groupId = BUILD_U16(pData[0], pData[1]);
	pData += 2;
	if(pCmd->status == ZCL_STA_SUCCESS){
		pCmd->sceneCnt = *pData++;
		pCmd->sceneList = pData;
	}
}

_CODE_ZCL_ static status_t zcl_addScenePrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 endpoint = pApsdeInd->indInfo.dst_ep;
	u8 cmdId = pInMsg->hdr.cmd;

	addScene_t addScene;
	TL_SETSTRUCTCONTENT(addScene, 0);

	zcl_addSceneParse(endpoint, pInMsg->pData, pInMsg->dataLen, &addScene);

	if(cmdId == ZCL_CMD_SCENE_ENHANCED_ADD_SCENE){
		addScene.scene.transTime100ms = addScene.scene.transTime % 10;
		addScene.scene.transTime /= 10;
	}

	/* check if group id is valid */
	if((addScene.scene.groupId == 0) || aps_group_search(addScene.scene.groupId, endpoint)){
		status = zcl_scene_addSceneEntry(endpoint, &addScene.scene);
	}else{
		status = ZCL_STA_INVALID_FIELD;
	}

	if(UNICAST_MSG(pApsdeInd)){
		epInfo_t dstEp;
		TL_SETSTRUCTCONTENT(dstEp, 0);

		dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
		dstEp.dstEp = pApsdeInd->indInfo.src_ep;
		dstEp.profileId = pApsdeInd->indInfo.profile_id;

		zcl_scene_sceneRsp(endpoint, &dstEp, TRUE, pInMsg->hdr.seqNum, cmdId, status, addScene.scene.groupId, addScene.scene.sceneId);

		status = ZCL_STA_CMD_HAS_RESP;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_viewScenePrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 endpoint = pApsdeInd->indInfo.dst_ep;
	u8 cmdId = pInMsg->hdr.cmd;
	u8 *pData = pInMsg->pData;

	viewScene_t viewScene;
	viewScene.groupId = BUILD_U16(pData[0], pData[1]);
	pData += 2;
	viewScene.sceneId = *pData++;

	zcl_sceneTable_t *pSceneEntry = zcl_scene_findEntry(endpoint, viewScene.groupId, viewScene.sceneId);
	if(!pSceneEntry){
		if((viewScene.groupId != 0) && (aps_group_search(viewScene.groupId, endpoint) == NULL)){
			/* group not found */
			status = ZCL_STA_INVALID_FIELD;
		}else{
			status = ZCL_STA_NOT_FOUND;
		}
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

		zcl_sceneEntry_t *pScene = (status == ZCL_STA_SUCCESS) ? &pSceneEntry->scene : NULL;

		zcl_scene_viewSceneRsp(endpoint, &dstEp, TRUE, pInMsg->hdr.seqNum, cmdId, status, viewScene.groupId, viewScene.sceneId, pScene);

		status = ZCL_STA_CMD_HAS_RESP;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_removeScenePrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 endpoint = pApsdeInd->indInfo.dst_ep;
	u8 *pData = pInMsg->pData;

	removeScene_t removeScene;
	removeScene.groupId = BUILD_U16(pData[0], pData[1]);
	pData += 2;
	removeScene.sceneId = *pData++;

	status = zcl_scene_removeSceneEntry(endpoint, removeScene.groupId, removeScene.sceneId);
	if(status == ZCL_STA_NOT_FOUND){
		/* scene not found, check if group exist */
		if(aps_group_search(removeScene.groupId, endpoint) == NULL){
			status = ZCL_STA_INVALID_FIELD;
		}
	}

	if(UNICAST_MSG(pApsdeInd)){
		epInfo_t dstEp;
		TL_SETSTRUCTCONTENT(dstEp, 0);

		dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
		dstEp.dstEp = pApsdeInd->indInfo.src_ep;
		dstEp.profileId = pApsdeInd->indInfo.profile_id;

		zcl_scene_sceneRsp(endpoint, &dstEp, TRUE, pInMsg->hdr.seqNum, ZCL_CMD_SCENE_REMOVE_SCENE_RSP, status, removeScene.groupId, removeScene.sceneId);

		status = ZCL_STA_CMD_HAS_RESP;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_removeAllScenePrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 endpoint = pApsdeInd->indInfo.dst_ep;

	removeAllScene_t removeAllScene;
	removeAllScene.groupId = BUILD_U16(pInMsg->pData[0], pInMsg->pData[1]);

	if((removeAllScene.groupId == 0) || aps_group_search(removeAllScene.groupId, endpoint)){
		zcl_scene_removeAllSceneEntry(endpoint, removeAllScene.groupId, TRUE);
	}else{
		status = ZCL_STA_INVALID_FIELD;
	}

	if(UNICAST_MSG(pApsdeInd)){
		epInfo_t dstEp;
		TL_SETSTRUCTCONTENT(dstEp, 0);

		dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
		dstEp.dstEp = pApsdeInd->indInfo.src_ep;
		dstEp.profileId = pApsdeInd->indInfo.profile_id;

		zcl_scene_sceneRsp(endpoint, &dstEp, TRUE, pInMsg->hdr.seqNum, ZCL_CMD_SCENE_REMOVE_ALL_SCENE_RSP, status, removeAllScene.groupId, 0);

		status = ZCL_STA_CMD_HAS_RESP;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_storeScenePrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 endpoint = pApsdeInd->indInfo.dst_ep;
	u8 *pData = pInMsg->pData;

	storeScene_t storeScene;
	storeScene.groupId = BUILD_U16(pData[0], pData[1]);
	pData += 2;
	storeScene.sceneId = *pData++;

	if((storeScene.groupId == 0) || aps_group_search(storeScene.groupId, endpoint)){
		zcl_sceneTable_t *pSceneEntry = zcl_scene_findEntry(endpoint, storeScene.groupId, storeScene.sceneId);
		if(!pSceneEntry){
			pSceneEntry = zcl_scene_getFreeEntry();
			if(!pSceneEntry){
				/* scene table full */
				status = ZCL_STA_INSUFFICIENT_SPACE;
			}
		}

		if(status == ZCL_STA_SUCCESS){
			if(pInMsg->clusterAppCb){
				zclIncomingAddrInfo_t addrInfo;
				addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
				addrInfo.profileId = pApsdeInd->indInfo.profile_id;
				addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
				addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
				addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
				addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

				/* Notify APP to fill scenes' information */
				pInMsg->clusterAppCb(&addrInfo, ZCL_CMD_SCENE_STORE_SCENE, &pSceneEntry->scene);

				/* store scene */
				pSceneEntry->used = 1;
				pSceneEntry->endpoint = endpoint;
				pSceneEntry->scene.groupId = storeScene.groupId;
				pSceneEntry->scene.sceneId = storeScene.sceneId;

				/* update scene attributes */
				u16 attrLen = 0;
				u8 currentScene = 0;
				if(zcl_getAttrVal(endpoint, ZCL_CLUSTER_GEN_SCENES, ZCL_ATTRID_SCENE_CURRENT_SCENE, &attrLen, (u8 *)&currentScene) == ZCL_STA_SUCCESS){
					currentScene = storeScene.sceneId;
					zcl_setAttrVal(endpoint, ZCL_CLUSTER_GEN_SCENES, ZCL_ATTRID_SCENE_CURRENT_SCENE, (u8 *)&currentScene);
				}

				u16 currentGroup = 0;
				if(zcl_getAttrVal(endpoint, ZCL_CLUSTER_GEN_SCENES, ZCL_ATTRID_SCENE_CURRENT_GROUP, &attrLen, (u8 *)&currentGroup) == ZCL_STA_SUCCESS){
					currentGroup = storeScene.groupId;
					zcl_setAttrVal(endpoint, ZCL_CLUSTER_GEN_SCENES, ZCL_ATTRID_SCENE_CURRENT_GROUP, (u8 *)&currentGroup);
				}

				u8 sceneValid = 0;
				if(zcl_getAttrVal(endpoint, ZCL_CLUSTER_GEN_SCENES, ZCL_ATTRID_SCENE_SCENE_VALID, &attrLen, (u8 *)&sceneValid) == ZCL_STA_SUCCESS){
					sceneValid = 1;
					zcl_setAttrVal(endpoint, ZCL_CLUSTER_GEN_SCENES, ZCL_ATTRID_SCENE_SCENE_VALID, (u8 *)&sceneValid);
				}

				/* scenes save */
				zcl_scenesSave(endpoint);
			}else{
				status = ZCL_STA_FAILURE;
			}
		}
	}else{
		status = ZCL_STA_INVALID_FIELD;
	}

	if(UNICAST_MSG(pApsdeInd)){
		epInfo_t dstEp;
		TL_SETSTRUCTCONTENT(dstEp, 0);

		dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
		dstEp.dstEp = pApsdeInd->indInfo.src_ep;
		dstEp.profileId = pApsdeInd->indInfo.profile_id;

		zcl_scene_sceneRsp(endpoint, &dstEp, TRUE, pInMsg->hdr.seqNum, ZCL_CMD_SCENE_STORE_SCENE_RSP, status, storeScene.groupId, storeScene.sceneId);

		status = ZCL_STA_CMD_HAS_RESP;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_recallScenePrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 endpoint = pApsdeInd->indInfo.dst_ep;
	u8 *pData = pInMsg->pData;

	recallScene_t recallScene;
	recallScene.groupId = BUILD_U16(pData[0], pData[1]);
	pData += 2;
	recallScene.sceneId = *pData++;

	zcl_sceneTable_t *pSceneEntry = zcl_scene_findEntry(endpoint, recallScene.groupId, recallScene.sceneId);
	if(pSceneEntry){
		if(pInMsg->clusterAppCb){
			zclIncomingAddrInfo_t addrInfo;
			addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
			addrInfo.profileId = pApsdeInd->indInfo.profile_id;
			addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
			addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
			addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
			addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

			pInMsg->clusterAppCb(&addrInfo, ZCL_CMD_SCENE_RECALL_SCENE, &pSceneEntry->scene);

			/* update scene attributes */
			u16 attrLen = 0;
			u8 currentScene = 0;
			if(zcl_getAttrVal(endpoint, ZCL_CLUSTER_GEN_SCENES, ZCL_ATTRID_SCENE_CURRENT_SCENE, &attrLen, (u8 *)&currentScene) == ZCL_STA_SUCCESS){
				currentScene = recallScene.sceneId;
				zcl_setAttrVal(endpoint, ZCL_CLUSTER_GEN_SCENES, ZCL_ATTRID_SCENE_CURRENT_SCENE, (u8 *)&currentScene);
			}

			u16 currentGroup = 0;
			if(zcl_getAttrVal(endpoint, ZCL_CLUSTER_GEN_SCENES, ZCL_ATTRID_SCENE_CURRENT_GROUP, &attrLen, (u8 *)&currentGroup) == ZCL_STA_SUCCESS){
				currentGroup = recallScene.groupId;
				zcl_setAttrVal(endpoint, ZCL_CLUSTER_GEN_SCENES, ZCL_ATTRID_SCENE_CURRENT_GROUP, (u8 *)&currentGroup);
			}

			u8 sceneValid = 0;
			if(zcl_getAttrVal(endpoint, ZCL_CLUSTER_GEN_SCENES, ZCL_ATTRID_SCENE_SCENE_VALID, &attrLen, (u8 *)&sceneValid) == ZCL_STA_SUCCESS){
				sceneValid = 1;
				zcl_setAttrVal(endpoint, ZCL_CLUSTER_GEN_SCENES, ZCL_ATTRID_SCENE_SCENE_VALID, (u8 *)&sceneValid);
			}
		}
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_getSceneMembershipPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 endpoint = pApsdeInd->indInfo.dst_ep;

	getSceneMemship_t getSceneMembership;
	getSceneMembership.groupId = BUILD_U16(pInMsg->pData[0], pInMsg->pData[1]);

	u8 sceneCnt = 0;
	u8 sceneList[ZCL_SCENE_TABLE_NUM] = {0};

	if((getSceneMembership.groupId == 0) || aps_group_search(getSceneMembership.groupId, endpoint)){
		sceneCnt = zcl_scene_findAllSceneByGroup(endpoint, getSceneMembership.groupId, sceneList);
	}else{
		status = ZCL_STA_INVALID_FIELD;
	}

	if(UNICAST_MSG(pApsdeInd)){
		epInfo_t dstEp;
		TL_SETSTRUCTCONTENT(dstEp, 0);

		getSceneMemRsp_t getSceneMembershipRsp;
		getSceneMembershipRsp.status = status;
		getSceneMembershipRsp.capacity = ZCL_SCENE_TABLE_NUM - zcl_scene_getSceneUsedNum();
		getSceneMembershipRsp.groupId = getSceneMembership.groupId;
		getSceneMembershipRsp.sceneCnt = sceneCnt;
		getSceneMembershipRsp.sceneList = sceneList;

		dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
		dstEp.dstEp = pApsdeInd->indInfo.src_ep;
		dstEp.profileId = pApsdeInd->indInfo.profile_id;

		zcl_scene_getSceneMembershipRsp(endpoint, &dstEp, TRUE, pInMsg->hdr.seqNum, &getSceneMembershipRsp);

		status = ZCL_STA_CMD_HAS_RESP;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_copyScenePrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 endpoint = pApsdeInd->indInfo.dst_ep;
	u8 *pData = pInMsg->pData;

	copyScene_t copyScene;
	copyScene.mode = *pData++;
	copyScene.groupIdFrom = BUILD_U16(pData[0], pData[1]);
	pData += 2;
	copyScene.sceneIdFrom = *pData++;
	copyScene.groupIdTo = BUILD_U16(pData[0], pData[1]);
	pData += 2;
	copyScene.sceneIdTo = *pData++;

	if(aps_group_search(copyScene.groupIdFrom, endpoint) && aps_group_search(copyScene.groupIdTo, endpoint)){
		status = zcl_scene_copySceneEntry(endpoint, &copyScene);
	}else{
		/* group not found */
		status = ZCL_STA_INVALID_FIELD;
	}

	if(UNICAST_MSG(pApsdeInd)){
		epInfo_t dstEp;
		TL_SETSTRUCTCONTENT(dstEp, 0);

		dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
		dstEp.dstEp = pApsdeInd->indInfo.src_ep;
		dstEp.profileId = pApsdeInd->indInfo.profile_id;

		zcl_scene_sceneRsp(endpoint, &dstEp, TRUE, pInMsg->hdr.seqNum, ZCL_CMD_SCENE_COPY_SCENE_RSP, status, copyScene.groupIdFrom, copyScene.sceneIdFrom);

		status = ZCL_STA_CMD_HAS_RESP;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_scene_clientCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd)
	{
		case ZCL_CMD_SCENE_ADD_SCENE:
		case ZCL_CMD_SCENE_ENHANCED_ADD_SCENE:
			status = zcl_addScenePrc(pInMsg);
			break;
		case ZCL_CMD_SCENE_VIEW_SCENE:
		case ZCL_CMD_SCENE_ENHANCED_VIEW_SCENE:
			status = zcl_viewScenePrc(pInMsg);
			break;
		case ZCL_CMD_SCENE_REMOVE_SCENE:
			status = zcl_removeScenePrc(pInMsg);
			break;
		case ZCL_CMD_SCENE_REMOVE_ALL_SCENE:
			status = zcl_removeAllScenePrc(pInMsg);
			break;
		case ZCL_CMD_SCENE_STORE_SCENE:
			status = zcl_storeScenePrc(pInMsg);
			break;
		case ZCL_CMD_SCENE_RECALL_SCENE:
			status = zcl_recallScenePrc(pInMsg);
			break;
		case ZCL_CMD_SCENE_GET_SCENE_MEMSHIP:
			status = zcl_getSceneMembershipPrc(pInMsg);
			break;
		case ZCL_CMD_SCENE_COPY_SCENE:
			status = zcl_copyScenePrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_scene_serverCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 *pData = pInMsg->pData;

	zcl_scene_cmdPayload_t cmdPayload;
	memset((u8 *)&cmdPayload, 0, sizeof(zcl_scene_cmdPayload_t));

	switch(pInMsg->hdr.cmd)
	{
		case ZCL_CMD_SCENE_ADD_SCENE_RSP:
		case ZCL_CMD_SCENE_ENHANCED_ADD_SCENE_RSP:
			//status = zcl_addSceneRspPrc(pInMsg);
			cmdPayload.addSceneRsp.status = *pData++;
			cmdPayload.addSceneRsp.groupId = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			cmdPayload.addSceneRsp.sceneId = *pData++;
			break;
		case ZCL_CMD_SCENE_VIEW_SCENE_RSP:
		case ZCL_CMD_SCENE_ENHANCED_VIEW_SCENE_RSP:
			//status = zcl_viewSceneRspPrc(pInMsg);
			zcl_viewSceneRspParse(pInMsg->pData, pInMsg->dataLen, &cmdPayload.viewSceneRsp);
			break;
		case ZCL_CMD_SCENE_REMOVE_SCENE_RSP:
			//status = zcl_removeSceneRspPrc(pInMsg);
			cmdPayload.removeSceneRsp.status = *pData++;
			cmdPayload.removeSceneRsp.groupId = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			cmdPayload.removeSceneRsp.sceneId = *pData++;
			break;
		case ZCL_CMD_SCENE_REMOVE_ALL_SCENE_RSP:
			//status = zcl_removeAllSceneRspPrc(pInMsg);
			cmdPayload.removeAllSceneRsp.status = *pData++;
			cmdPayload.removeAllSceneRsp.groupId = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			break;
		case ZCL_CMD_SCENE_STORE_SCENE_RSP:
			//status = zcl_storeSceneRspPrc(pInMsg);
			cmdPayload.storeSceneRsp.status = *pData++;
			cmdPayload.storeSceneRsp.groupId = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			cmdPayload.storeSceneRsp.sceneId = *pData++;
			break;
		case ZCL_CMD_SCENE_GET_SCENE_MEMSHIP_RSP:
			//status = zcl_getSceneMembershipRspPrc(pInMsg);
			zcl_getSceneMembershipRspParse(pInMsg->pData, &cmdPayload.getSceneMembershipRsp);
			break;
		case ZCL_CMD_SCENE_COPY_SCENE_RSP:
			//status = zcl_copySceneRspPrc(pInMsg);
			cmdPayload.copySceneRsp.status = *pData++;
			cmdPayload.copySceneRsp.groupIdFrom = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			cmdPayload.copySceneRsp.sceneIdFrom = *pData++;
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	if(status == ZCL_STA_SUCCESS){
		if(pInMsg->clusterAppCb){
			zclIncomingAddrInfo_t addrInfo;
			addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
			addrInfo.profileId = pApsdeInd->indInfo.profile_id;
			addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
			addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
			addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
			addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

			pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmdPayload);
		}
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_scene_cmdHandler(zclIncoming_t *pInMsg)
{
    if(pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR){
        return zcl_scene_clientCmdHandler(pInMsg);
    }else{
        return zcl_scene_serverCmdHandler(pInMsg);
    }
}

#endif  /* ZCL_SCENE */







