/********************************************************************************************************
 * @file	zcl_ota.c
 *
 * @brief	This is the source file for zcl_ota
 *
 * @author	Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
/**********************************************************************
 * INCLUDES
 */
#include "../zcl_include.h"


#ifdef ZCL_OTA
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
static status_t zcl_ota_cmdHandler(zclIncoming_t *pInMsg);

_CODE_ZCL_ status_t zcl_ota_register(u8 endpoint, u8 arrtNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
	return zcl_registerCluster(endpoint, ZCL_CLUSTER_OTA, arrtNum, attrTbl, zcl_ota_cmdHandler, cb);
}

_CODE_ZCL_ status_t zcl_ota_imageNotifySend(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, ota_imageNotify_t *pReq)
{
	u8 buf[10];
	u8 *pBuf = buf;

	*pBuf++ = pReq->payloadType;
	*pBuf++ = pReq->queryJitter;
	if(pReq->payloadType >= IMAGE_NOTIFY_QUERT_JITTER_MFG){
		*pBuf++ = LO_UINT16(pReq->manuCode);
		*pBuf++ = HI_UINT16(pReq->manuCode);
	}
	if(pReq->payloadType >= IMAGE_NOTIFY_QUERY_JITTER_MFG_TYPE){
		*pBuf++ = LO_UINT16(pReq->imageType);
		*pBuf++ = HI_UINT16(pReq->imageType);
	}
	if(pReq->payloadType == IMAGE_NOTIFY_QUERY_JITTER_MFG_TYPE_VER){
		*pBuf++ = U32_BYTE0(pReq->newFileVer);
		*pBuf++ = U32_BYTE1(pReq->newFileVer);
		*pBuf++ = U32_BYTE2(pReq->newFileVer);
		*pBuf++ = U32_BYTE3(pReq->newFileVer);
	}

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_OTA, ZCL_CMD_OTA_IMAGE_NOTIFY, TRUE,
					ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, (u8)(pBuf - buf), buf);
}

_CODE_ZCL_ status_t zcl_ota_queryNextImageReqSend(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, ota_queryNextImageReq_t *pReq)
{
	u8 buf[11];
	u8 *pBuf = buf;

	*pBuf++ = pReq->fc;
	*pBuf++ = LO_UINT16(pReq->manuCode);
	*pBuf++ = HI_UINT16(pReq->manuCode);
	*pBuf++ = LO_UINT16(pReq->imageType);
	*pBuf++ = HI_UINT16(pReq->imageType);
	*pBuf++ = U32_BYTE0(pReq->curFileVer);
	*pBuf++ = U32_BYTE1(pReq->curFileVer);
	*pBuf++ = U32_BYTE2(pReq->curFileVer);
	*pBuf++ = U32_BYTE3(pReq->curFileVer);
	if(pReq->fc & IMAGE_FC_BITMASK_HARDWARE_VERSION_PRESENT){
		*pBuf++ = LO_UINT16(pReq->hdrwareVer);
		*pBuf++ = HI_UINT16(pReq->hdrwareVer);
	}

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_OTA, ZCL_CMD_OTA_QUERY_NEXT_IMAGE_REQ, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, (u8)(pBuf - buf), buf);
}

_CODE_ZCL_ status_t zcl_ota_queryNextImageRspSend(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, ota_queryNextImageRsp_t *pRsp)
{
	u8 buf[13];
	u8 *pBuf = buf;

	*pBuf++ = pRsp->st;
	if(pRsp->st == ZCL_STA_SUCCESS){
		*pBuf++ = LO_UINT16(pRsp->manuCode);
		*pBuf++ = HI_UINT16(pRsp->manuCode);
		*pBuf++ = LO_UINT16(pRsp->imageType);
		*pBuf++ = HI_UINT16(pRsp->imageType);
		*pBuf++ = U32_BYTE0(pRsp->fileVer);
		*pBuf++ = U32_BYTE1(pRsp->fileVer);
		*pBuf++ = U32_BYTE2(pRsp->fileVer);
		*pBuf++ = U32_BYTE3(pRsp->fileVer);
		*pBuf++ = U32_BYTE0(pRsp->imageSize);
		*pBuf++ = U32_BYTE1(pRsp->imageSize);
		*pBuf++ = U32_BYTE2(pRsp->imageSize);
		*pBuf++ = U32_BYTE3(pRsp->imageSize);
	}

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_OTA, ZCL_CMD_OTA_QUERY_NEXT_IMAGE_RSP, TRUE,
			    	ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, (u8)(pBuf - buf), buf);
}

_CODE_ZCL_ status_t zcl_ota_imageBlockReqSend(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, ota_imageBlockReq_t *pReq)
{
	u8 buf[24];
	u8 *pBuf = buf;

	*pBuf++ = pReq->fc;
	*pBuf++ = LO_UINT16(pReq->manuCode);
	*pBuf++ = HI_UINT16(pReq->manuCode);
	*pBuf++ = LO_UINT16(pReq->imageType);
	*pBuf++ = HI_UINT16(pReq->imageType);
	*pBuf++ = U32_BYTE0(pReq->fileVer);
	*pBuf++ = U32_BYTE1(pReq->fileVer);
	*pBuf++ = U32_BYTE2(pReq->fileVer);
	*pBuf++ = U32_BYTE3(pReq->fileVer);
	*pBuf++ = U32_BYTE0(pReq->fileOffset);
	*pBuf++ = U32_BYTE1(pReq->fileOffset);
	*pBuf++ = U32_BYTE2(pReq->fileOffset);
	*pBuf++ = U32_BYTE3(pReq->fileOffset);
	*pBuf++ = pReq->maxDataSize;
	if(pReq->fc & BLOCK_FC_BITMASK_NODE_IEEE_PRESENT){
		ZB_IEEE_ADDR_COPY(pBuf, pReq->reqNodeAddr);
		pBuf += EXT_ADDR_LEN;
	}
	if(pReq->fc & BLOCK_FC_BITMASK_MIN_PERIOD_PRESENT){
		*pBuf++ = LO_UINT16(pReq->blockReqDelay);
		*pBuf++ = HI_UINT16(pReq->blockReqDelay);
	}

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_OTA, ZCL_CMD_OTA_IMAGE_BLOCK_REQ, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, (u8)(pBuf - buf), buf);
}

_CODE_ZCL_ status_t zcl_ota_imagePageReqSend(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, ota_imagePageReq_t *pReq)
{
	u8 buf[26];
	u8 *pBuf = buf;

	*pBuf++ = pReq->fc;
	*pBuf++ = LO_UINT16(pReq->manuCode);
	*pBuf++ = HI_UINT16(pReq->manuCode);
	*pBuf++ = LO_UINT16(pReq->imageType);
	*pBuf++ = HI_UINT16(pReq->imageType);
	*pBuf++ = U32_BYTE0(pReq->fileVer);
	*pBuf++ = U32_BYTE1(pReq->fileVer);
	*pBuf++ = U32_BYTE2(pReq->fileVer);
	*pBuf++ = U32_BYTE3(pReq->fileVer);
	*pBuf++ = U32_BYTE0(pReq->fileOffset);
	*pBuf++ = U32_BYTE1(pReq->fileOffset);
	*pBuf++ = U32_BYTE2(pReq->fileOffset);
	*pBuf++ = U32_BYTE3(pReq->fileOffset);
	*pBuf++ = pReq->maxDataSize;
	*pBuf++ = LO_UINT16(pReq->pageSize);
	*pBuf++ = HI_UINT16(pReq->pageSize);
	*pBuf++ = LO_UINT16(pReq->rspSpacing);
	*pBuf++ = HI_UINT16(pReq->rspSpacing);
	if(pReq->fc & PAGE_FC_BITMASK_NODE_IEEE_PRESENT){
		ZB_IEEE_ADDR_COPY(pBuf, pReq->reqNodeAddr);
		pBuf += EXT_ADDR_LEN;
	}

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_OTA, ZCL_CMD_OTA_IMAGE_PAGE_REQ, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, (u8)(pBuf - buf), buf);
}

_CODE_ZCL_ status_t zcl_ota_imageBlockRspSend(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, ota_imageBlockRsp_t *pRsp)
{
	u8 len = 0;

	if(pRsp->st == ZCL_STA_SUCCESS){
		len = 14 + pRsp->rsp.success.dataSize;
	}else if(pRsp->st == ZCL_STA_WAIT_FOR_DATA){
		len = 11;
	}else{
		len = 1;
	}

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = pRsp->st;
	if(pRsp->st == ZCL_STA_SUCCESS){
		*pBuf++ = LO_UINT16(pRsp->rsp.success.manuCode);
		*pBuf++ = HI_UINT16(pRsp->rsp.success.manuCode);
		*pBuf++ = LO_UINT16(pRsp->rsp.success.imageType);
		*pBuf++ = HI_UINT16(pRsp->rsp.success.imageType);
		*pBuf++ = U32_BYTE0(pRsp->rsp.success.fileVer);
		*pBuf++ = U32_BYTE1(pRsp->rsp.success.fileVer);
		*pBuf++ = U32_BYTE2(pRsp->rsp.success.fileVer);
		*pBuf++ = U32_BYTE3(pRsp->rsp.success.fileVer);
		*pBuf++ = U32_BYTE0(pRsp->rsp.success.fileOffset);
		*pBuf++ = U32_BYTE1(pRsp->rsp.success.fileOffset);
		*pBuf++ = U32_BYTE2(pRsp->rsp.success.fileOffset);
		*pBuf++ = U32_BYTE3(pRsp->rsp.success.fileOffset);
		*pBuf++ = pRsp->rsp.success.dataSize;
		memcpy(pBuf, pRsp->rsp.success.data, pRsp->rsp.success.dataSize);
	}else if(pRsp->st == ZCL_STA_WAIT_FOR_DATA){
		*pBuf++ = U32_BYTE0(pRsp->rsp.wait.currTime);
		*pBuf++ = U32_BYTE1(pRsp->rsp.wait.currTime);
		*pBuf++ = U32_BYTE2(pRsp->rsp.wait.currTime);
		*pBuf++ = U32_BYTE3(pRsp->rsp.wait.currTime);
		*pBuf++ = U32_BYTE0(pRsp->rsp.wait.reqTime);
		*pBuf++ = U32_BYTE1(pRsp->rsp.wait.reqTime);
		*pBuf++ = U32_BYTE2(pRsp->rsp.wait.reqTime);
		*pBuf++ = U32_BYTE3(pRsp->rsp.wait.reqTime);
		*pBuf++ = LO_UINT16(pRsp->rsp.wait.blockReqDelay);
		*pBuf++ = HI_UINT16(pRsp->rsp.wait.blockReqDelay);
	}

	zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_OTA, ZCL_CMD_OTA_IMAGE_BLOCK_RSP, TRUE,
			    ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, len, buf);

	ev_buf_free(buf);

	return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ status_t zcl_ota_upgradeEndReqSend(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, ota_upgradeEndReq_t *pReq)
{
	u8 buf[9];
	u8 *pBuf = buf;

	*pBuf++ = pReq->st;
	*pBuf++ = LO_UINT16(pReq->manuCode);
	*pBuf++ = HI_UINT16(pReq->manuCode);
	*pBuf++ = LO_UINT16(pReq->imageType);
	*pBuf++ = HI_UINT16(pReq->imageType);
	*pBuf++ = U32_BYTE0(pReq->fileVer);
	*pBuf++ = U32_BYTE1(pReq->fileVer);
	*pBuf++ = U32_BYTE2(pReq->fileVer);
	*pBuf++ = U32_BYTE3(pReq->fileVer);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_OTA, ZCL_CMD_OTA_UPGRADE_END_REQ, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, (u8)(pBuf - buf), buf);
}

_CODE_ZCL_ status_t zcl_ota_upgradeEndRspSend(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, ota_upgradeEndRsp_t *pRsp)
{
	u8 buf[16];
	u8 *pBuf = buf;

	*pBuf++ = LO_UINT16(pRsp->manuCode);
	*pBuf++ = HI_UINT16(pRsp->manuCode);
	*pBuf++ = LO_UINT16(pRsp->imageType);
	*pBuf++ = HI_UINT16(pRsp->imageType);
	*pBuf++ = U32_BYTE0(pRsp->fileVer);
	*pBuf++ = U32_BYTE1(pRsp->fileVer);
	*pBuf++ = U32_BYTE2(pRsp->fileVer);
	*pBuf++ = U32_BYTE3(pRsp->fileVer);
	*pBuf++ = U32_BYTE0(pRsp->currentTime);
	*pBuf++ = U32_BYTE1(pRsp->currentTime);
	*pBuf++ = U32_BYTE2(pRsp->currentTime);
	*pBuf++ = U32_BYTE3(pRsp->currentTime);
	*pBuf++ = U32_BYTE0(pRsp->upgradeTime);
	*pBuf++ = U32_BYTE1(pRsp->upgradeTime);
	*pBuf++ = U32_BYTE2(pRsp->upgradeTime);
	*pBuf++ = U32_BYTE3(pRsp->upgradeTime);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_OTA, ZCL_CMD_OTA_UPGRADE_END_RSP, TRUE,
			    	ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, (u8)(pBuf - buf), buf);
}

_CODE_ZCL_ status_t zcl_ota_queryDevSpecFileReqSend(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, ota_querySpecFileReq_t *pReq)
{
	u8 buf[18];
	u8 *pBuf = buf;

	ZB_IEEE_ADDR_COPY(pBuf, pReq->reqNodeAddr);
	pBuf += EXT_ADDR_LEN;
	*pBuf++ = LO_UINT16(pReq->manuCode);
	*pBuf++ = HI_UINT16(pReq->manuCode);
	*pBuf++ = LO_UINT16(pReq->imageType);
	*pBuf++ = HI_UINT16(pReq->imageType);
	*pBuf++ = U32_BYTE0(pReq->fileVer);
	*pBuf++ = U32_BYTE1(pReq->fileVer);
	*pBuf++ = U32_BYTE2(pReq->fileVer);
	*pBuf++ = U32_BYTE3(pReq->fileVer);
	*pBuf++ = LO_UINT16(pReq->zbStackVer);
	*pBuf++ = HI_UINT16(pReq->zbStackVer);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_OTA, ZCL_CMD_OTA_QUERY_DEVICE_SPECIFIC_FILE_REQ, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, (u8)(pBuf - buf), buf);
}

_CODE_ZCL_ status_t zcl_ota_queryDevSpecFileRspSend(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, ota_querySpecFileRsp_t *pRsp)
{
	u8 buf[13];
	u8 *pBuf = buf;

	*pBuf++ = pRsp->st;
	if(pRsp->st == ZCL_STA_SUCCESS){
		*pBuf++ = LO_UINT16(pRsp->manuCode);
		*pBuf++ = HI_UINT16(pRsp->manuCode);
		*pBuf++ = LO_UINT16(pRsp->imageType);
		*pBuf++ = HI_UINT16(pRsp->imageType);
		*pBuf++ = U32_BYTE0(pRsp->fileVer);
		*pBuf++ = U32_BYTE1(pRsp->fileVer);
		*pBuf++ = U32_BYTE2(pRsp->fileVer);
		*pBuf++ = U32_BYTE3(pRsp->fileVer);
		*pBuf++ = U32_BYTE0(pRsp->imageSize);
		*pBuf++ = U32_BYTE1(pRsp->imageSize);
		*pBuf++ = U32_BYTE2(pRsp->imageSize);
		*pBuf++ = U32_BYTE3(pRsp->imageSize);
	}

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_OTA, ZCL_CMD_OTA_QUERY_DEVICE_SPECIFIC_FILE_RSP, TRUE,
				ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, (u8)(pBuf - buf), buf);
}

_CODE_ZCL_ static status_t zcl_ota_queryNextImageReqPrc(zclIncoming_t *pInMsg)
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

		ota_queryNextImageReq_t req;
		req.fc = *pData++;
		req.manuCode = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		req.imageType = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		req.curFileVer = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
		pData += 4;
		if(req.fc & IMAGE_FC_BITMASK_HARDWARE_VERSION_PRESENT){
			req.hdrwareVer = BUILD_U16(pData[0], pData[1]);
		}

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &req);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_ota_imageBlockReqPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;

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

		ota_imageBlockReq_t req;
		req.fc = *pData++;
		req.manuCode = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		req.imageType = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		req.fileVer = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
		pData += 4;
		req.fileOffset = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
		pData += 4;
		req.maxDataSize = *pData++;
		if(req.fc & BLOCK_FC_BITMASK_NODE_IEEE_PRESENT){
			ZB_IEEE_ADDR_COPY(req.reqNodeAddr, pData);
			pData += EXT_ADDR_LEN;
		}
		if(req.fc & BLOCK_FC_BITMASK_MIN_PERIOD_PRESENT){
			req.blockReqDelay = BUILD_U16(pData[0], pData[1]);
		}

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &req);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_ota_imagePageReqPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;

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

		ota_imagePageReq_t req;
		req.fc = *pData++;
		req.manuCode = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		req.imageType = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		req.fileVer = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
		pData += 4;
		req.fileOffset = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
		pData += 4;
		req.maxDataSize = *pData++;
		req.pageSize = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		req.rspSpacing = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		if(req.fc & PAGE_FC_BITMASK_NODE_IEEE_PRESENT){
			ZB_IEEE_ADDR_COPY(req.reqNodeAddr, pData);
			pData += EXT_ADDR_LEN;
		}

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &req);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_ota_upgradeEndReqPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;

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

		ota_upgradeEndReq_t req;
		req.st = *pData++;
		if(req.st == ZCL_STA_SUCCESS){
			req.manuCode = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			req.imageType = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			req.fileVer = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
		}

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &req);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_ota_queryDeviceSpecificFileReqPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;

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

		ota_querySpecFileReq_t req;
		ZB_IEEE_ADDR_COPY(req.reqNodeAddr, pData);
		pData += EXT_ADDR_LEN;
		req.manuCode = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		req.imageType = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		req.fileVer = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
		pData += 4;
		req.zbStackVer = BUILD_U16(pData[0], pData[1]);
		pData += 2;

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &req);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_ota_imageNotifyPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;

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

		ota_imageNotify_t imageNotify;
		imageNotify.payloadType = *pData++;
		imageNotify.queryJitter = *pData++;

		if(imageNotify.payloadType >= IMAGE_NOTIFY_QUERT_JITTER_MFG){
			imageNotify.manuCode = BUILD_U16(pData[0], pData[1]);
			pData += 2;
		}
		if(imageNotify.payloadType >= IMAGE_NOTIFY_QUERY_JITTER_MFG_TYPE){
			imageNotify.imageType = BUILD_U16(pData[0], pData[1]);
			pData += 2;
		}
		if(imageNotify.payloadType == IMAGE_NOTIFY_QUERY_JITTER_MFG_TYPE_VER){
			imageNotify.newFileVer = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
		}

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &imageNotify);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_ota_queryNextImageRspPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;

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

		ota_queryNextImageRsp_t rsp;
		rsp.st = *pData++;
		if(rsp.st == ZCL_STA_SUCCESS){
			rsp.manuCode = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			rsp.imageType = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			rsp.fileVer = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
			pData += 4;
			rsp.imageSize = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
		}

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &rsp);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_ota_imageBlockRspPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;

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

		ota_imageBlockRsp_t rsp;
		rsp.st = *pData++;
		if(rsp.st == ZCL_STA_SUCCESS){
			rsp.rsp.success.manuCode = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			rsp.rsp.success.imageType = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			rsp.rsp.success.fileVer = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
			pData += 4;
			rsp.rsp.success.fileOffset = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
			pData += 4;
			rsp.rsp.success.dataSize = *pData++;
			rsp.rsp.success.data = pData;
		}else if(rsp.st == ZCL_STA_WAIT_FOR_DATA){
			rsp.rsp.wait.currTime = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
			pData += 4;
			rsp.rsp.wait.reqTime = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
			pData += 4;
			rsp.rsp.wait.blockReqDelay = BUILD_U16(pData[0], pData[1]);
		}

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &rsp);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_ota_upgradeEndRspPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;

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

		ota_upgradeEndRsp_t rsp;
		rsp.manuCode = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		rsp.imageType = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		rsp.fileVer = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
		pData += 4;
		rsp.currentTime = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
		pData += 4;
		rsp.upgradeTime = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &rsp);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_ota_queryDeviceSpecificFileRspPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;

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

		ota_querySpecFileRsp_t rsp;
		rsp.st = *pData++;
		if(rsp.st == ZCL_STA_SUCCESS){
			rsp.manuCode = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			rsp.imageType = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			rsp.fileVer = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
			pData += 4;
			rsp.imageSize = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
		}

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &rsp);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_ota_clientCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd)
	{
		case ZCL_CMD_OTA_QUERY_NEXT_IMAGE_REQ:
			status = zcl_ota_queryNextImageReqPrc(pInMsg);
			break;
		case ZCL_CMD_OTA_IMAGE_BLOCK_REQ:
			status = zcl_ota_imageBlockReqPrc(pInMsg);
			break;
		case ZCL_CMD_OTA_IMAGE_PAGE_REQ:
			status = zcl_ota_imagePageReqPrc(pInMsg);
			break;
		case ZCL_CMD_OTA_UPGRADE_END_REQ:
			status = zcl_ota_upgradeEndReqPrc(pInMsg);
			break;
		case ZCL_CMD_OTA_QUERY_DEVICE_SPECIFIC_FILE_REQ:
			status = zcl_ota_queryDeviceSpecificFileReqPrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_ota_serverCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd)
	{
		case ZCL_CMD_OTA_IMAGE_NOTIFY:
			status = zcl_ota_imageNotifyPrc(pInMsg);
			break;
		case ZCL_CMD_OTA_QUERY_NEXT_IMAGE_RSP:
			status = zcl_ota_queryNextImageRspPrc(pInMsg);
			break;
		case ZCL_CMD_OTA_IMAGE_BLOCK_RSP:
			status = zcl_ota_imageBlockRspPrc(pInMsg);
			break;
		case ZCL_CMD_OTA_UPGRADE_END_RSP:
			status = zcl_ota_upgradeEndRspPrc(pInMsg);
			break;
		case ZCL_CMD_OTA_QUERY_DEVICE_SPECIFIC_FILE_RSP:
			status = zcl_ota_queryDeviceSpecificFileRspPrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_ota_cmdHandler(zclIncoming_t *pInMsg)
{
	if(pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR){
		return zcl_ota_clientCmdHandler(pInMsg);
	}else{
		return zcl_ota_serverCmdHandler(pInMsg);
	}
}

#endif	/* ZCL_OTA */

