/********************************************************************************************************
 * @file     zcl_wwah.c
 *
 * @brief	 APIs for WWAH cluster
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


#ifdef ZCL_WWAH
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
status_t zcl_wwah_cmdHandler(zclIncoming_t *pInMsg);

_CODE_ZCL_ status_t zcl_wwah_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
	return zcl_registerCluster(endpoint, ZCL_CLUSTER_WWAH, attrNum, attrTbl, zcl_wwah_cmdHandler, cb);
}

_CODE_ZCL_ status_t zcl_wwah_enableApsLinkKeyAuthSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, wwah_enableApsLinkKeyAuth_t *pReq)
{
	u8 status = ZCL_STA_SUCCESS;
	u8 len = 1 + pReq->numOfClustersToExempt * sizeof(u16);

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return RET_BUF_FULL;
	}

	u8 *pBuf = buf;

	*pBuf++ = pReq->numOfClustersToExempt;
	for(u8 i = 0; i < pReq->numOfClustersToExempt; i++){
		*pBuf++ = LO_UINT16(pReq->clusterID[i]);
		*pBuf++ = HI_UINT16(pReq->clusterID[i]);
	}

	status = zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_ENABLE_APS_LINK_KEY_AUTH, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, len, buf);

    ev_buf_free(buf);

    return status;
}

_CODE_ZCL_ status_t zcl_wwah_disableApsLinkKeyAuthSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, wwah_disableApsLinkKeyAuth_t *pReq)
{
	u8 status = ZCL_STA_SUCCESS;
	u8 len = 1 + pReq->numOfClustersToExempt * sizeof(u16);

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return RET_BUF_FULL;
	}

	u8 *pBuf = buf;

	*pBuf++ = pReq->numOfClustersToExempt;
	for(u8 i = 0; i < pReq->numOfClustersToExempt; i++){
		*pBuf++ = LO_UINT16(pReq->clusterID[i]);
		*pBuf++ = HI_UINT16(pReq->clusterID[i]);
	}

	status = zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_DISABLE_APS_LINK_KEY_AUTH, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, len, buf);

    ev_buf_free(buf);

    return status;
}

_CODE_ZCL_ status_t zcl_wwah_apsLinkKeyAuthQuerySend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, wwah_apsLinkKeyAuthQuery_t *pReq)
{
	u8 buf[2];
	u8 *pBuf = buf;

	*pBuf++ = LO_UINT16(pReq->clusterID);
	*pBuf++ = HI_UINT16(pReq->clusterID);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_APS_LINK_KEY_AUTH_QUERY, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 2, buf);
}

_CODE_ZCL_ status_t zcl_wwah_requestNewApsLinkKeySend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_REQUEST_NEW_APS_LINK_KEY, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_enableAppEvtRetryAlgorithmSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, wwah_enableAppEvtRetryAlgorithm_t *pReq)
{
	u8 buf[7];
	u8 *pBuf = buf;

	*pBuf++ = pReq->firstBackoffTimeInSeconds;
	*pBuf++ = pReq->backoffSequenceCommonRatio;
	*pBuf++ = U32_BYTE0(pReq->maxBackoffTimeInSeconds);
	*pBuf++ = U32_BYTE1(pReq->maxBackoffTimeInSeconds);
	*pBuf++ = U32_BYTE2(pReq->maxBackoffTimeInSeconds);
	*pBuf++ = U32_BYTE3(pReq->maxBackoffTimeInSeconds);
	*pBuf++ = pReq->maxReDeliveryAttepts;

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_ENABLE_APP_EVT_RETRY_ALGORITHM, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 7, buf);
}

_CODE_ZCL_ status_t zcl_wwah_disableAppEvtRetryAlgorithmSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_DISABLE_APP_EVT_RETRY_ALGORITHM, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_requestTimeSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_REQUEST_TIME, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_enableRejoinAlgorithmSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, wwah_enableRejoinAlgorithm_t *pReq)
{
	u8 buf[4];
	u8 *pBuf = buf;

	*pBuf++ = LO_UINT16(pReq->fastRejoinTimeoutInSeconds);
	*pBuf++ = HI_UINT16(pReq->fastRejoinTimeoutInSeconds);
	*pBuf++ = LO_UINT16(pReq->durationBetweenEachRejoinInSeconds);
	*pBuf++ = HI_UINT16(pReq->durationBetweenEachRejoinInSeconds);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_ENABLE_REJOIN_ALGORITHM, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 4, buf);
}

_CODE_ZCL_ status_t zcl_wwah_disableRejoinAlgorithmSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_DISABLE_REJOIN_ALGORITHM, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_setIasZoneEnrollmentMethodSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, wwah_setIasZoneEnrollmentMethod_t *pReq)
{
	u8 buf[1];

	buf[0] = pReq->enrollmentMode;

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_SET_IAS_ZONE_ENROLLMENT_METHOD, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 1, buf);
}

_CODE_ZCL_ status_t zcl_wwah_clearBindingTableSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_CLEAR_BINDING_TABLE, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_enablePeriodicRouterCheckInsSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, wwah_enablePeriodicRouterCheckIns_t *pReq)
{
	u8 buf[2];
	u8 *pBuf = buf;

	*pBuf++ = LO_UINT16(pReq->checkInInterval);
	*pBuf++ = HI_UINT16(pReq->checkInInterval);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_ENABLE_PERIODIC_ROUTER_CHECK_INS, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 2, buf);
}

_CODE_ZCL_ status_t zcl_wwah_disablePeriodicRouterCheckInsSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_DISABLE_PERIODIC_ROUTER_CHECK_INS, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_apsLinkKeyAuthQueryRspSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, wwah_apsLinkKeyAuthQueryRsp_t *pRsp)
{
	u8 buf[3];
	u8 *pBuf = buf;

	*pBuf++ = LO_UINT16(pRsp->clusterID);
	*pBuf++ = HI_UINT16(pRsp->clusterID);
	*pBuf++ = pRsp->status;

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_APS_LINK_KEY_AUTH_QUERY_RSP, TRUE,
    		ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, 3, buf);
}

_CODE_ZCL_ status_t zcl_wwah_poweringOffNotificationSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 reason)
{
    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_POWERING_OFF_NOTIFICATION, TRUE,
    		ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, 1, &reason);
}

_CODE_ZCL_ status_t zcl_wwah_poweringOnNotificationSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 reason)
{
    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_POWERING_ON_NOTIFICATION, TRUE,
    		ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, 1, &reason);
}

_CODE_ZCL_ status_t zcl_wwah_shortAddrChangeSend(u8 srcEp, epInfo_t* pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_SHORT_ADDRESS_CHANGE, TRUE,
    		ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

// Client side handler
_CODE_ZCL_ u8 zcl_wwah_serverCmdHandler(zclIncoming_t *pInMsg)
{
    u8 status = ZCL_STA_SUCCESS;
    u8 cmd = pInMsg->hdr.cmd;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

    switch(cmd){
		case ZCL_CMD_WWAH_APS_LINK_KEY_AUTH_QUERY_RSP:
			break;
		case ZCL_CMD_WWAH_POWERING_OFF_NOTIFICATION:
			break;
		case ZCL_CMD_WWAH_POWERING_ON_NOTIFICATION:
			break;
		case ZCL_CMD_WWAH_SHORT_ADDRESS_CHANGE:
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
    }

    return status;
}

// Server side handler
_CODE_ZCL_ u8 zcl_wwah_clientCmdHandler(zclIncoming_t *pInMsg)
{
    u8 status = ZCL_STA_SUCCESS;
    u8 cmd = pInMsg->hdr.cmd;
    epInfo_t dstEp;
    TL_SETSTRUCTCONTENT(dstEp, 0);
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
    u8 endpoint = pApsdeInd->indInfo.dst_ep;
    dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
    dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
    dstEp.dstEp = pApsdeInd->indInfo.src_ep;
    dstEp.profileId = pApsdeInd->indInfo.profile_id;
    dstEp.radius = 0;
    dstEp.txOptions = 0;

    switch(cmd){
		case ZCL_CMD_WWAH_ENABLE_APS_LINK_KEY_AUTH:
			break;
		case ZCL_CMD_WWAH_DISABLE_APS_LINK_KEY_AUTH:
			break;
		case ZCL_CMD_WWAH_APS_LINK_KEY_AUTH_QUERY:
			break;
		case ZCL_CMD_WWAH_REQUEST_NEW_APS_LINK_KEY:
			break;
		case ZCL_CMD_WWAH_ENABLE_APP_EVT_RETRY_ALGORITHM:
			break;
		case ZCL_CMD_WWAH_DISABLE_APP_EVT_RETRY_ALGORITHM:
			break;
		case ZCL_CMD_WWAH_REQUEST_TIME:
			break;
		case ZCL_CMD_WWAH_ENABLE_REJOIN_ALGORITHM:
			break;
		case ZCL_CMD_WWAH_DISABLE_REJOIN_ALGORITHM:
			break;
		case ZCL_CMD_WWAH_SET_IAS_ZONE_ENROLLMENT_METHOD:
			break;
		case ZCL_CMD_WWAH_CLEAR_BINDING_TABLE:
			break;
		case ZCL_CMD_WWAH_ENABLE_PERIODIC_ROUTER_CHECK_INS:
			break;
		case ZCL_CMD_WWAH_DISABLE_PERIODIC_ROUTER_CHECK_INS:
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
    }

    return status;
}

_CODE_ZCL_ status_t zcl_wwah_cmdHandler(zclIncoming_t *pInMsg)
{
	if (pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR) {
		return zcl_wwah_clientCmdHandler(pInMsg);
	} else {
		return zcl_wwah_serverCmdHandler(pInMsg);
	}
}

#endif /* ZCL_WWAH */
