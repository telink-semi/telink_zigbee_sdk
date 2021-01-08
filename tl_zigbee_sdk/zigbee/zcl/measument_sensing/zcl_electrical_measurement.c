/********************************************************************************************************
 * @file	zcl_electrical_measurement.c
 *
 * @brief	This is the source file for zcl_electrical_measurement
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


#ifdef ZCL_ELECTRICAL_MEASUREMENT
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
status_t zcl_electricalMeasure_cmdHandler(zclIncoming_t *pInMsg);


_CODE_ZCL_ status_t zcl_electricalMeasure_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
	return zcl_registerCluster(endpoint, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, attrNum, attrTbl, zcl_electricalMeasure_cmdHandler, cb);
}


_CODE_ZCL_ status_t zcl_electricalMeasure_getProfileInfo(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, ZCL_CMD_GET_PROFILE_INFO, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_electricalMeasure_getMeasurementProfile(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_electricalMeasure_getMeasurementProfileCmd_t *pReq)
{
	u8 buf[7];
	u8 *pBuf = buf;

	*pBuf++ = LO_UINT16(pReq->attrID);
	*pBuf++ = HI_UINT16(pReq->attrID);
	*pBuf++ = U32_BYTE0(pReq->startTime);
	*pBuf++ = U32_BYTE1(pReq->startTime);
	*pBuf++ = U32_BYTE2(pReq->startTime);
	*pBuf++ = U32_BYTE3(pReq->startTime);
	*pBuf++ = pReq->numberOfIntervals;

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, ZCL_CMD_GET_MEASUREMENT_PROFILE, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, (u8)(pBuf - buf), buf);
}

_CODE_ZCL_ status_t zcl_electricalMeasure_getProfileInfoRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_electricalMeasure_getProfileInfoRspCmd_t *pRsp)
{
	u8 len = 1 + 1 + 1 + pRsp->maxNumberOfIntervals * 2;

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = pRsp->profileCnt;
	*pBuf++ = pRsp->profileIntervalPeriod;
	*pBuf++ = pRsp->maxNumberOfIntervals;
	if(pRsp->maxNumberOfIntervals){
		memcpy(pBuf, pRsp->listOfAttributes, pRsp->maxNumberOfIntervals * 2);
		pBuf += pRsp->maxNumberOfIntervals * 2;
	}

	zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, ZCL_CMD_GET_PROFILE_INFO_RESPONSE, TRUE,
				ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, (u8)(pBuf - buf), buf);

	ev_buf_free(buf);

	return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ status_t zcl_electricalMeasure_getMeasurementProfileRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_electricalMeasure_getMeasurementProfileRspCmd_t *pRsp)
{
	u8 len = 4 + 1 + 1 + 1 + 1 + pRsp->numberOfIntervals * 2;

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = U32_BYTE0(pRsp->startTime);
	*pBuf++ = U32_BYTE1(pRsp->startTime);
	*pBuf++ = U32_BYTE2(pRsp->startTime);
	*pBuf++ = U32_BYTE3(pRsp->startTime);
	*pBuf++ = pRsp->status;
	*pBuf++ = pRsp->profileIntervalPeriod;
	*pBuf++ = pRsp->numberOfIntervals;
	*pBuf++ = pRsp->attributeId;
	if(pRsp->numberOfIntervals){
		memcpy(pBuf, pRsp->intervals, pRsp->numberOfIntervals * 2);
		pBuf += pRsp->numberOfIntervals * 2;
	}

	zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, ZCL_CMD_GET_MEASUREMENT_PROFILE_RESPONSE, TRUE,
				ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, (u8)(pBuf - buf), buf);

	ev_buf_free(buf);

	return ZCL_STA_SUCCESS;
}


_CODE_ZCL_ status_t zcl_electricalMeasure_getProfileInfoPrc(zclIncoming_t *pInMsg)
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

_CODE_ZCL_ status_t zcl_electricalMeasure_getMeasurementProfilePrc(zclIncoming_t *pInMsg)
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

	    zcl_electricalMeasure_getMeasurementProfileCmd_t getMeasurementProfileCmd;
	    getMeasurementProfileCmd.attrID = BUILD_U16(pData[0], pData[1]);
	    pData += 2;
	    getMeasurementProfileCmd.startTime = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
	    pData += 4;
	    getMeasurementProfileCmd.numberOfIntervals = *pData++;

    	status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &getMeasurementProfileCmd);
    }else{
		status = ZCL_STA_FAILURE;
	}

    return status;
}

_CODE_ZCL_ status_t zcl_electricalMeasure_getProfileInfoRspPrc(zclIncoming_t *pInMsg)
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

	    zcl_electricalMeasure_getProfileInfoRspCmd_t getProfileInfoRspCmd;
	    getProfileInfoRspCmd.profileCnt = *pData++;
	    getProfileInfoRspCmd.profileIntervalPeriod = *pData++;
	    getProfileInfoRspCmd.maxNumberOfIntervals = *pData++;
	    getProfileInfoRspCmd.listOfAttributes = pData;

    	status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &getProfileInfoRspCmd);
    }else{
		status = ZCL_STA_FAILURE;
	}

    return status;
}

_CODE_ZCL_ status_t zcl_electricalMeasure_getMeasurementProfileRspPrc(zclIncoming_t *pInMsg)
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

	    zcl_electricalMeasure_getMeasurementProfileRspCmd_t getMeasurementProfileRspCmd;
	    getMeasurementProfileRspCmd.startTime = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
	    pData += 4;
	    getMeasurementProfileRspCmd.status = *pData++;
	    getMeasurementProfileRspCmd.profileIntervalPeriod = *pData++;
	    getMeasurementProfileRspCmd.numberOfIntervals = *pData++;
	    getMeasurementProfileRspCmd.attributeId = *pData++;
	    getMeasurementProfileRspCmd.intervals = pData;

    	status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &getMeasurementProfileRspCmd);
    }else{
		status = ZCL_STA_FAILURE;
	}

    return status;
}

_CODE_ZCL_ static status_t zcl_electricalMeasure_clientCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd){
		case ZCL_CMD_GET_PROFILE_INFO:
			status = zcl_electricalMeasure_getProfileInfoPrc(pInMsg);
			break;
		case ZCL_CMD_GET_MEASUREMENT_PROFILE:
			status = zcl_electricalMeasure_getMeasurementProfilePrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_electricalMeasure_serverCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd){
		case ZCL_CMD_GET_PROFILE_INFO_RESPONSE:
			status = zcl_electricalMeasure_getProfileInfoRspPrc(pInMsg);
			break;
		case ZCL_CMD_GET_MEASUREMENT_PROFILE_RESPONSE:
			status = zcl_electricalMeasure_getMeasurementProfileRspPrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_electricalMeasure_cmdHandler(zclIncoming_t *pInMsg)
{
	if(pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR){
		return zcl_electricalMeasure_clientCmdHandler(pInMsg);
	}else{
		return zcl_electricalMeasure_serverCmdHandler(pInMsg);
	}
}

#endif  /* ZCL_ELECTRICAL_MEASUREMENT */







