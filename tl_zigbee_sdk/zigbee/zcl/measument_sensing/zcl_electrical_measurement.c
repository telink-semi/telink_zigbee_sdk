/********************************************************************************************************
 * @file     zcl_electrical_measurement.c
 *
 * @brief	 APIs for electrical measurement cluster
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







