/********************************************************************************************************
 * @file     zcl_metering.c
 *
 * @brief	 APIs for metering cluster
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


#ifdef ZCL_METERING
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
static status_t zcl_metering_cmdHandler(zclIncoming_t *pInMsg);


_CODE_ZCL_ status_t zcl_metering_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
	return zcl_registerCluster(endpoint, ZCL_CLUSTER_SE_METERING, attrNum, attrTbl, zcl_metering_cmdHandler, cb);
}


_CODE_ZCL_ status_t zcl_metering_getProfile(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_metering_getProfileCmd_t *pReq)
{
	u8 buf[6];
	u8 *pBuf = buf;

	*pBuf++ = pReq->intervalChannel;
	*pBuf++ = U32_BYTE0(pReq->endTime);
	*pBuf++ = U32_BYTE1(pReq->endTime);
	*pBuf++ = U32_BYTE2(pReq->endTime);
	*pBuf++ = U32_BYTE3(pReq->endTime);
	*pBuf++ = pReq->numberOfPeriods;

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SE_METERING, ZCL_CMD_GET_PROFILE, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, (u8)(pBuf - buf), buf);
}

_CODE_ZCL_ status_t zcl_metering_requestMirrorRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_metering_requestMirrorRspCmd_t *pRsp)
{
	u8 buf[2];
	u8 *pBuf = buf;

	*pBuf++ = LO_UINT16(pRsp->endPointId);
	*pBuf++ = HI_UINT16(pRsp->endPointId);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SE_METERING, ZCL_CMD_REQUEST_MIRROR_RESPONSE, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, (u8)(pBuf - buf), buf);
}

_CODE_ZCL_ status_t zcl_metering_mirrorRemoved(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_metering_mirrorRemovedCmd_t *pRsp)
{
	u8 buf[2];
	u8 *pBuf = buf;

	*pBuf++ = LO_UINT16(pRsp->removedEndPointId);
	*pBuf++ = HI_UINT16(pRsp->removedEndPointId);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SE_METERING, ZCL_CMD_MIRROR_REMOVED, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, (u8)(pBuf - buf), buf);
}

_CODE_ZCL_ status_t zcl_metering_requestFastPollMode(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_metering_requestFastPollModeCmd_t *pReq)
{
	u8 buf[2];
	u8 *pBuf = buf;

	*pBuf++ = pReq->fastPollUpdatePeriod;
	*pBuf++ = pReq->duration;

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SE_METERING, ZCL_CMD_REQUEST_FAST_POLL_MODE, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, (u8)(pBuf - buf), buf);
}

_CODE_ZCL_ status_t zcl_metering_getProfileRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_metering_getProfileRspCmd_t *pRsp)
{
	u8 len = 4 + 1 + 1 + 1 + pRsp->numberOfPeriodsDelived * 3;

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = U32_BYTE0(pRsp->endTime);
	*pBuf++ = U32_BYTE1(pRsp->endTime);
	*pBuf++ = U32_BYTE2(pRsp->endTime);
	*pBuf++ = U32_BYTE3(pRsp->endTime);
	*pBuf++ = pRsp->status;
	*pBuf++ = pRsp->profileIntervalPeriod;
	*pBuf++ = pRsp->numberOfPeriodsDelived;
	memcpy(pBuf, pRsp->intervals, pRsp->numberOfPeriodsDelived * 3);
	pBuf += (pRsp->numberOfPeriodsDelived * 3);

	zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SE_METERING, ZCL_CMD_GET_PROFILE_RESPONSE, TRUE,
				ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, (u8)(pBuf - buf), buf);

	ev_buf_free(buf);

	return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ status_t zcl_metering_requestMirror(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SE_METERING, ZCL_CMD_REQUEST_MIRROR, TRUE,
					ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_metering_removeMirror(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SE_METERING, ZCL_CMD_REMOVE_MIRROR, TRUE,
					ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_metering_requestFastPollModeRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_metering_requestFastPollModeRspCmd_t *pRsp)
{
	u8 buf[5];
	u8 *pBuf = buf;

	*pBuf++ = pRsp->appliedUpdatePeriod;
	*pBuf++ = U32_BYTE0(pRsp->fastPollModeEndTime);
	*pBuf++ = U32_BYTE1(pRsp->fastPollModeEndTime);
	*pBuf++ = U32_BYTE2(pRsp->fastPollModeEndTime);
	*pBuf++ = U32_BYTE3(pRsp->fastPollModeEndTime);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SE_METERING, ZCL_CMD_REQUEST_FAST_POLL_MODE_RESPONSE, TRUE,
					ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, (u8)(pBuf - buf), buf);
}


_CODE_ZCL_ static status_t zcl_metering_getProfilePrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	u8 *pData = pInMsg->pData;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

    if(pInMsg->clusterAppCb){
		zclIncomingAddrInfo_t addrInfo;
		addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
		addrInfo.profileId = pApsdeInd->indInfo.profile_id;
		addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
		addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
		addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
		addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

	    zcl_metering_getProfileCmd_t getProfileCmd;
	    getProfileCmd.intervalChannel = *pData++;
	    getProfileCmd.endTime = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
	    pData += 4;
	    getProfileCmd.numberOfPeriods = *pData++;

    	status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &getProfileCmd);
    }else{
		status = ZCL_STA_FAILURE;
	}

    return status;
}

_CODE_ZCL_ static status_t zcl_metering_requestMirrorRspPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	u8 *pData = pInMsg->pData;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

    if(pInMsg->clusterAppCb){
		zclIncomingAddrInfo_t addrInfo;
		addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
		addrInfo.profileId = pApsdeInd->indInfo.profile_id;
		addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
		addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
		addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
		addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

	    zcl_metering_requestMirrorRspCmd_t requestMirrorRspCmd;
	    requestMirrorRspCmd.endPointId = BUILD_U16(pData[0], pData[1]);

    	status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &requestMirrorRspCmd);
    }else{
		status = ZCL_STA_FAILURE;
	}

    return status;
}

_CODE_ZCL_ static status_t zcl_metering_mirrorRemovedPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	u8 *pData = pInMsg->pData;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

    if(pInMsg->clusterAppCb){
		zclIncomingAddrInfo_t addrInfo;
		addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
		addrInfo.profileId = pApsdeInd->indInfo.profile_id;
		addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
		addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
		addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
		addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

	    zcl_metering_mirrorRemovedCmd_t mirrorRemovedCmd;
	    mirrorRemovedCmd.removedEndPointId = BUILD_U16(pData[0], pData[1]);

    	status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &mirrorRemovedCmd);
    }else{
		status = ZCL_STA_FAILURE;
	}

    return status;
}

_CODE_ZCL_ static status_t zcl_metering_requestFastPollModePrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	u8 *pData = pInMsg->pData;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

    if(pInMsg->clusterAppCb){
		zclIncomingAddrInfo_t addrInfo;
		addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
		addrInfo.profileId = pApsdeInd->indInfo.profile_id;
		addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
		addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
		addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
		addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

	    zcl_metering_requestFastPollModeCmd_t requestFastPollModeCmd;
	    requestFastPollModeCmd.fastPollUpdatePeriod = *pData++;
	    requestFastPollModeCmd.duration = *pData++;

    	status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &requestFastPollModeCmd);
    }else{
		status = ZCL_STA_FAILURE;
	}

    return status;
}

_CODE_ZCL_ static status_t zcl_metering_getProfileRspPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	u8 *pData = pInMsg->pData;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

    if(pInMsg->clusterAppCb){
		zclIncomingAddrInfo_t addrInfo;
		addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
		addrInfo.profileId = pApsdeInd->indInfo.profile_id;
		addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
		addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
		addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
		addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

	    zcl_metering_getProfileRspCmd_t getProfileRspCmd;
	    getProfileRspCmd.endTime = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
	    pData += 4;
	    getProfileRspCmd.status = *pData++;
	    getProfileRspCmd.profileIntervalPeriod = *pData++;
	    getProfileRspCmd.numberOfPeriodsDelived = *pData++;
	    getProfileRspCmd.intervals = pData;

    	status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &getProfileRspCmd);
    }else{
		status = ZCL_STA_FAILURE;
	}

    return status;
}

_CODE_ZCL_ static status_t zcl_metering_requestMirrorPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	//u8 *pData = pInMsg->pData; //this command has no payload.
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

    if(pInMsg->clusterAppCb){
		zclIncomingAddrInfo_t addrInfo;
		addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
		addrInfo.profileId = pApsdeInd->indInfo.profile_id;
		addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
		addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
		addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
		addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

    	status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, NULL);
    }else{
		status = ZCL_STA_FAILURE;
	}

    return status;
}

_CODE_ZCL_ static status_t zcl_metering_removeMirrorPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	//u8 *pData = pInMsg->pData; //this command has no payload.
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

    if(pInMsg->clusterAppCb){
		zclIncomingAddrInfo_t addrInfo;
		addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
		addrInfo.profileId = pApsdeInd->indInfo.profile_id;
		addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
		addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
		addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
		addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

    	status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, NULL);
    }else{
		status = ZCL_STA_FAILURE;
	}

    return status;
}

_CODE_ZCL_ static status_t zcl_metering_requestFastPollModeRspPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	u8 *pData = pInMsg->pData;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

    if(pInMsg->clusterAppCb){
		zclIncomingAddrInfo_t addrInfo;
		addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
		addrInfo.profileId = pApsdeInd->indInfo.profile_id;
		addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
		addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
		addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
		addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

	    zcl_metering_requestFastPollModeRspCmd_t requestFastPollModeRspCmd;
	    requestFastPollModeRspCmd.appliedUpdatePeriod = *pData++;
	    requestFastPollModeRspCmd.fastPollModeEndTime = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);

    	status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &requestFastPollModeRspCmd);
    }else{
		status = ZCL_STA_FAILURE;
	}

    return status;
}

_CODE_ZCL_ static status_t zcl_metering_clientCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd){
		case ZCL_CMD_GET_PROFILE:
			status = zcl_metering_getProfilePrc(pInMsg);
			break;
		case ZCL_CMD_REQUEST_MIRROR_RESPONSE:
			status = zcl_metering_requestMirrorRspPrc(pInMsg);
			break;
		case ZCL_CMD_MIRROR_REMOVED:
			status = zcl_metering_mirrorRemovedPrc(pInMsg);
			break;
		case ZCL_CMD_REQUEST_FAST_POLL_MODE:
			status = zcl_metering_requestFastPollModePrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_metering_serverCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd){
		case ZCL_CMD_GET_PROFILE_RESPONSE:
			status = zcl_metering_getProfileRspPrc(pInMsg);
			break;
		case ZCL_CMD_REQUEST_MIRROR:
			status = zcl_metering_requestMirrorPrc(pInMsg);
			break;
		case ZCL_CMD_REMOVE_MIRROR:
			status = zcl_metering_removeMirrorPrc(pInMsg);
			break;
		case ZCL_CMD_REQUEST_FAST_POLL_MODE_RESPONSE:
			status = zcl_metering_requestFastPollModeRspPrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_metering_cmdHandler(zclIncoming_t *pInMsg)
{
	if(pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR){
		return zcl_metering_clientCmdHandler(pInMsg);
	}else{
		return zcl_metering_serverCmdHandler(pInMsg);
	}
}

#endif  /* ZCL_METERING */







