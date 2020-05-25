/********************************************************************************************************
 * @file     zcl_door_lock.c
 *
 * @brief	 APIs for door lock cluster
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


#ifdef ZCL_DOOR_LOCK
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
static status_t zcl_doorLock_cmdHandler(zclIncoming_t *pInMsg);


_CODE_ZCL_ status_t zcl_doorLock_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
	return zcl_registerCluster(endpoint, ZCL_CLUSTER_CLOSURES_DOOR_LOCK, attrNum, attrTbl, zcl_doorLock_cmdHandler, cb);
}


_CODE_ZCL_ status_t zcl_doorLock_doorLockReq(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 cmdId, u8 *pCode)
{
	status_t status = ZCL_STA_SUCCESS;
	u8 len = pCode[0] + 1;

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	for(u8 i = 0; i < len; i++){
		buf[i] = pCode[i];
	}

	status = zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_CLOSURES_DOOR_LOCK, cmdId, TRUE,
		ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, len, buf);

	if(buf){
		ev_buf_free(buf);
	}
	return status;
}

_CODE_ZCL_ status_t zcl_doorLock_doorLockRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 cmdId, status_t rspStatus)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_CLOSURES_DOOR_LOCK, cmdId, TRUE,
		ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, 1, &rspStatus);
}


_CODE_ZCL_ static status_t zcl_doorLock_doorLockReqPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

    if(pInMsg->clusterAppCb){
		zclIncomingAddrInfo_t addrInfo;
		addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
		addrInfo.profileId = pApsdeInd->indInfo.profile_id;
		addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
		addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
		addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
		addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

	    zcl_doorlockReq_t reqCmd;
	    reqCmd.dataLen = pInMsg->dataLen;
	    reqCmd.pData = pInMsg->pData;

    	status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &reqCmd);
    }else{
    	status = ZCL_STA_FAILURE;
    }

	epInfo_t dstEp;
	TL_SETSTRUCTCONTENT(dstEp, 0);

	dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
	dstEp.dstEp = pApsdeInd->indInfo.src_ep;
	dstEp.profileId = pApsdeInd->indInfo.profile_id;

	zcl_doorLock_doorLockRsp(pApsdeInd->indInfo.dst_ep, &dstEp, TRUE, pInMsg->hdr.seqNum, pInMsg->hdr.cmd, status);

	status = ZCL_STA_CMD_HAS_RESP;

    return status;
}

_CODE_ZCL_ static status_t zcl_doorLock_doorLockRspPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

	if(pInMsg->clusterAppCb){
		zclIncomingAddrInfo_t addrInfo;
		addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
		addrInfo.profileId = pApsdeInd->indInfo.profile_id;
		addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
		addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
		addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
		addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

		zcl_doorlockRsp_t rspCmd;
		rspCmd.status = pInMsg->pData[0];
		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &rspCmd);
	}else{
		status = ZCL_STA_FAILURE;
	}

    return status;
}

_CODE_ZCL_ static status_t zcl_doorLock_clientCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd){
		case ZCL_CMD_LOCK_DOOR:
		case ZCL_CMD_UNLOCK_DOOR:
		case ZCL_CMD_DOOR_LOCK_TOGGLE:
			status = zcl_doorLock_doorLockReqPrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_doorLock_serverCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd){
		case ZCL_CMD_LOCK_DOOR_RESPONSE:
		case ZCL_CMD_UNLOCK_DOOR_RESPONSE:
		case ZCL_CMD_DOOR_LOCK_TOGGLE_RESPONSE:
			status = zcl_doorLock_doorLockRspPrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_doorLock_cmdHandler(zclIncoming_t *pInMsg)
{
	if(pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR){
		return zcl_doorLock_clientCmdHandler(pInMsg);
	}else{
		return zcl_doorLock_serverCmdHandler(pInMsg);
	}
}
#endif
