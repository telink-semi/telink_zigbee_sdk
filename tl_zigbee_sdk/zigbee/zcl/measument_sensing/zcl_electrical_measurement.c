/********************************************************************************************************
 * @file    zcl_electrical_measurement.c
 *
 * @brief   This is the source file for zcl_electrical_measurement
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
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


_CODE_ZCL_ status_t zcl_electricalMeasure_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
	return zcl_registerCluster(endpoint, ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT, manuCode, attrNum, attrTbl, zcl_electricalMeasure_cmdHandler, cb);
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
    if(pInMsg->clusterAppCb){
    	status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, NULL);
    }else{
		status = ZCL_STA_FAILURE;
	}

    return status;
}

_CODE_ZCL_ status_t zcl_electricalMeasure_getMeasurementProfilePrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	u8 *pData = pInMsg->pData;

    if(pInMsg->clusterAppCb){
	    zcl_electricalMeasure_getMeasurementProfileCmd_t getMeasurementProfileCmd;
	    getMeasurementProfileCmd.attrID = BUILD_U16(pData[0], pData[1]);
	    pData += 2;
	    getMeasurementProfileCmd.startTime = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
	    pData += 4;
	    getMeasurementProfileCmd.numberOfIntervals = *pData++;

    	status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &getMeasurementProfileCmd);
    }else{
		status = ZCL_STA_FAILURE;
	}

    return status;
}

_CODE_ZCL_ status_t zcl_electricalMeasure_getProfileInfoRspPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	u8 *pData = pInMsg->pData;

    if(pInMsg->clusterAppCb){
	    zcl_electricalMeasure_getProfileInfoRspCmd_t getProfileInfoRspCmd;
	    getProfileInfoRspCmd.profileCnt = *pData++;
	    getProfileInfoRspCmd.profileIntervalPeriod = *pData++;
	    getProfileInfoRspCmd.maxNumberOfIntervals = *pData++;
	    getProfileInfoRspCmd.listOfAttributes = pData;

    	status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &getProfileInfoRspCmd);
    }else{
		status = ZCL_STA_FAILURE;
	}

    return status;
}

_CODE_ZCL_ status_t zcl_electricalMeasure_getMeasurementProfileRspPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	u8 *pData = pInMsg->pData;

    if(pInMsg->clusterAppCb){
	    zcl_electricalMeasure_getMeasurementProfileRspCmd_t getMeasurementProfileRspCmd;
	    getMeasurementProfileRspCmd.startTime = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
	    pData += 4;
	    getMeasurementProfileRspCmd.status = *pData++;
	    getMeasurementProfileRspCmd.profileIntervalPeriod = *pData++;
	    getMeasurementProfileRspCmd.numberOfIntervals = *pData++;
	    getMeasurementProfileRspCmd.attributeId = *pData++;
	    getMeasurementProfileRspCmd.intervals = pData;

    	status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &getMeasurementProfileRspCmd);
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







