/********************************************************************************************************
 * @file     zcl_pollCtrl.c
 *
 * @brief	 APIs for pollCtrl cluster
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


#ifdef ZCL_POLL_CTRL
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
static status_t zcl_pollCtrl_cmdHandler(zclIncoming_t *pInMsg);


_CODE_ZCL_ status_t zcl_pollCtrl_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
	return zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_POLL_CONTROL, attrNum, attrTbl, zcl_pollCtrl_cmdHandler, cb);
}


_CODE_ZCL_ status_t zcl_pollCtrl_chkIn(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_POLL_CONTROL, ZCL_CMD_CHK_IN, TRUE,
					ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_pollCtrl_chkInRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_chkInRsp_t *pCheckInRsp)
{
	u8 buf[3];

	buf[0] = pCheckInRsp->startFastPolling;
	buf[1] = LO_UINT16(pCheckInRsp->fastPollTimeout);
	buf[2] = HI_UINT16(pCheckInRsp->fastPollTimeout);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_POLL_CONTROL, ZCL_CMD_CHK_IN_RSP, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 3, buf);
}

_CODE_ZCL_ status_t zcl_pollCtrl_fastPollStop(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_POLL_CONTROL, ZCL_CMD_FAST_POLL_STOP, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_pollCtrl_setLongPollInterval(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_setLongPollInterval_t *pSetLongPollInterval)
{
	u8 buf[4];

	buf[0] = U32_BYTE0(pSetLongPollInterval->newLongPollInterval);
	buf[1] = U32_BYTE1(pSetLongPollInterval->newLongPollInterval);
	buf[2] = U32_BYTE2(pSetLongPollInterval->newLongPollInterval);
	buf[3] = U32_BYTE3(pSetLongPollInterval->newLongPollInterval);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_POLL_CONTROL, ZCL_CMD_SET_LONG_POLL_INTERVAL, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 4, buf);
}

_CODE_ZCL_ status_t zcl_pollCtrl_setShortPollInterval(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_setShortPollInterval_t *pSetShortPollInterval)
{
	u8 buf[2];

	buf[0] = LO_UINT16(pSetShortPollInterval->newShortPollInterval);
	buf[1] = HI_UINT16(pSetShortPollInterval->newShortPollInterval);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_POLL_CONTROL, ZCL_CMD_SET_LONG_POLL_INTERVAL, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 2, buf);
}

_CODE_ZCL_ static status_t zcl_pollCtrl_chkInRspPrc(zclIncoming_t *pInMsg)
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

		zcl_chkInRsp_t payload;
		payload.startFastPolling = pInMsg->pData[0];
		payload.fastPollTimeout = BUILD_U16(pInMsg->pData[1], pInMsg->pData[2]);

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &payload);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_pollCtrl_fastPollStopPrc(zclIncoming_t *pInMsg)
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

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, NULL);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_pollCtrl_setLongPollIntervalPrc(zclIncoming_t *pInMsg)
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

		zcl_setLongPollInterval_t payload;
		payload.newLongPollInterval = BUILD_U32(pInMsg->pData[0], pInMsg->pData[1], pInMsg->pData[2], pInMsg->pData[3]);

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &payload);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_pollCtrl_setShortPollIntervalPrc(zclIncoming_t *pInMsg)
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

		zcl_setShortPollInterval_t payload;
		payload.newShortPollInterval = BUILD_U16(pInMsg->pData[0], pInMsg->pData[1]);

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &payload);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_pollCtrl_chkInPrc(zclIncoming_t *pInMsg)
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

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, NULL);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_pollCtrl_clientCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd)
	{
		case ZCL_CMD_CHK_IN_RSP:
			status = zcl_pollCtrl_chkInRspPrc(pInMsg);
			break;
		case ZCL_CMD_FAST_POLL_STOP:
			status = zcl_pollCtrl_fastPollStopPrc(pInMsg);
			break;
		case ZCL_CMD_SET_LONG_POLL_INTERVAL:
			status = zcl_pollCtrl_setLongPollIntervalPrc(pInMsg);
			break;
		case ZCL_CMD_SET_SHORT_POLL_INTERVAL:
			status = zcl_pollCtrl_setShortPollIntervalPrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_pollCtrl_serverCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd)
	{
		case ZCL_CMD_CHK_IN:
			status = zcl_pollCtrl_chkInPrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_pollCtrl_cmdHandler(zclIncoming_t *pInMsg)
{
	if(pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR){
		return zcl_pollCtrl_clientCmdHandler(pInMsg);
	}else{
		return zcl_pollCtrl_serverCmdHandler(pInMsg);
	}
}

#endif    /* ZCL_POLL_CTRL */
