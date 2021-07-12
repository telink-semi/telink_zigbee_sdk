/********************************************************************************************************
 * @file	zcl_wwah.c
 *
 * @brief	This is the source file for zcl_wwah
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
wwah_apsLinkKeyAuthInfo_t g_zcl_apsLinkKeyAuthInfo;
wwah_apsAcksRequireInfo_t g_zcl_apsAcksRequireInfo;
wwah_useTCForClusterInfo_t g_zcl_useTCForClusterInfo;

bool g_wwah_RouterCheckedFlag = TRUE;

/**********************************************************************
 * LOCAL FUNCTIONS
 */
static void zcl_wwah_apsLinkKeyAuthInfoClear(void);
static nv_sts_t zcl_wwah_apsLinkKeyAuthInfoRestore(void);
static void zcl_wwah_apsAcksRequireInfoClear(void);
static nv_sts_t zcl_wwah_apsAcksRequireInfoRestore(void);
static void zcl_wwah_useTCForClusterInfoClear(void);
static nv_sts_t zcl_wwah_useTCForClusterInfoRestore(void);
static status_t zcl_wwah_cmdHandler(zclIncoming_t *pInMsg);


_CODE_ZCL_ status_t zcl_wwah_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
    if(zcl_wwah_apsLinkKeyAuthInfoRestore() != NV_SUCC){
    	zcl_wwah_apsLinkKeyAuthInfoClear();
    }
    if(zcl_wwah_apsAcksRequireInfoRestore() != NV_SUCC){
    	zcl_wwah_apsAcksRequireInfoClear();
    }
	if(zcl_wwah_useTCForClusterInfoRestore() != NV_SUCC){
		zcl_wwah_useTCForClusterInfoClear();
	}

	return zcl_registerCluster(endpoint, ZCL_CLUSTER_WWAH, manuCode, attrNum, attrTbl, zcl_wwah_cmdHandler, cb);
}

_CODE_ZCL_ static void zcl_wwah_apsLinkKeyAuthInfoClear(void)
{
	g_zcl_apsLinkKeyAuthInfo.status = APS_LINK_KEY_AUTH_DISABLE;
	g_zcl_apsLinkKeyAuthInfo.num = 0;
	memset((u8 *)g_zcl_apsLinkKeyAuthInfo.clusterID, 0, sizeof(u16) * CLUSTER_ID_MAX_NUM);
}

_CODE_ZCL_ static nv_sts_t zcl_wwah_apsLinkKeyAuthInfoRestore(void)
{
	return zcl_apsLinkKeyAuthInfo_restore();
}

_CODE_ZCL_ static void zcl_wwah_apsLinkKeyAuthInfoSave(void *arg)
{
	zcl_apsLinkKeyAuthInfo_save();
}

_CODE_ZCL_ static void zcl_wwah_apsAcksRequireInfoClear(void)
{
	g_zcl_apsAcksRequireInfo.num = 0;
	memset((u8 *)g_zcl_apsAcksRequireInfo.clusterID, 0, sizeof(u16) * CLUSTER_ID_MAX_NUM);
}

_CODE_ZCL_ static nv_sts_t zcl_wwah_apsAcksRequireInfoRestore(void)
{
	return zcl_apsAcksRequireInfo_restore();
}

_CODE_ZCL_ static void zcl_wwah_apsAcksRequireInfoSave(void *arg)
{
	zcl_apsAcksRequireInfo_save();
}

_CODE_ZCL_ static void zcl_wwah_useTCForClusterInfoClear(void)
{
	g_zcl_useTCForClusterInfo.num = 0;
	memset((u8 *)g_zcl_useTCForClusterInfo.clusterID, 0, sizeof(u16) * CLUSTER_ID_MAX_NUM);
}

_CODE_ZCL_ static nv_sts_t zcl_wwah_useTCForClusterInfoRestore(void)
{
	return zcl_useTCForClusterInfo_restore();
}

_CODE_ZCL_ static void zcl_wwah_useTCForClusterInfoSave(void *arg)
{
	zcl_useTCForClusterInfo_save();
}


_CODE_ZCL_ static apsLinkKeyAuthStatus_t zcl_wwah_apsLinkKeyAuthSearch(u16 clusterID)
{
	u8 find = 0;
	for(u8 i = 0; i < g_zcl_apsLinkKeyAuthInfo.num; i++){
		if(g_zcl_apsLinkKeyAuthInfo.clusterID[i] == clusterID){
			find = 1;
			break;
		}
	}

	return (find ? (!g_zcl_apsLinkKeyAuthInfo.status) : g_zcl_apsLinkKeyAuthInfo.status);
}

_CODE_ZCL_ static bool zcl_wwah_apsLinkKeyAuthCheck(u16 clusterID, bool apsUseSec)
{
	bool accept;

	if(g_zcl_apsLinkKeyAuthInfo.status == APS_LINK_KEY_AUTH_DISABLE){
		accept = TRUE;

		for(u8 i = 0; i < g_zcl_apsLinkKeyAuthInfo.num; i++){
			if((g_zcl_apsLinkKeyAuthInfo.clusterID[i] == clusterID) && !apsUseSec){
				accept = FALSE;
				break;
			}
		}
	}else{
		if(apsUseSec){
			return TRUE;
		}

		accept = FALSE;

		for(u8 i = 0; i < g_zcl_apsLinkKeyAuthInfo.num; i++){
			if(g_zcl_apsLinkKeyAuthInfo.clusterID[i] == clusterID){
				accept = TRUE;
				break;
			}
		}
	}

	return accept;
}

_CODE_ZCL_ static bool zcl_wwah_touchlinkEnabled(u8 ep)
{
	bool enable = TRUE;
	u16 len;

	if(zcl_getAttrVal(ep, ZCL_CLUSTER_WWAH, ZCL_ATTRID_WWAH_TOUCHLINK_INTERPAN_ENABLED, &len, (u8 *)&enable) == ZCL_STA_SUCCESS){
		return enable;
	}
	return TRUE;
}

_CODE_ZCL_ static void zcl_wwah_periodicRouterChecked(void)
{
	g_wwah_RouterCheckedFlag = TRUE;
}

_CODE_ZCL_ bool zcl_wwah_getRouterCheckedFlag(void)
{
	return g_wwah_RouterCheckedFlag;
}

_CODE_ZCL_ void zcl_wwah_clearRouterCheckedFlag(void)
{
	g_wwah_RouterCheckedFlag = FALSE;
}

_CODE_ZCL_ status_t zcl_wwah_acceptCheck(zclIncoming_t *pInMsg)
{
	apsdeDataInd_t *pApsdeInd = pInMsg->msg;

	/* During the router check-in period, if a message from TC is received. */
	if(pApsdeInd->indInfo.src_short_addr == 0x0000){
		zcl_wwah_periodicRouterChecked();
	}

	/* Check the ZCL_ATTRID_WWAH_TOUCHLINK_INTERPAN_ENABLED when received ZLL messages. */
	if(pApsdeInd->indInfo.cluster_id == ZCL_CLUSTER_TOUCHLINK_COMMISSIONING){
		if(!zcl_wwah_touchlinkEnabled(pApsdeInd->indInfo.dst_ep)){
			return ZCL_STA_NOT_AUTHORIZED;
		}
	}

	/* Check APS link key authorization. */
	if(zcl_wwah_apsLinkKeyAuthCheck(pApsdeInd->indInfo.cluster_id, pApsdeInd->indInfo.security_status & SECURITY_IN_APSLAYER) == FALSE){
		return ZCL_STA_NOT_AUTHORIZED;
	}

	return ZCL_STA_SUCCESS;
}



_CODE_ZCL_ status_t zcl_wwah_enableApsLinkKeyAuthCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, clustersToExempt_t *pCmd)
{
	u8 status = ZCL_STA_SUCCESS;
	u8 len = 1 + pCmd->numOfClustersToExempt * sizeof(u16);

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = pCmd->numOfClustersToExempt;
	for(u8 i = 0; i < pCmd->numOfClustersToExempt; i++){
		*pBuf++ = LO_UINT16(pCmd->pClusterID[i]);
		*pBuf++ = HI_UINT16(pCmd->pClusterID[i]);
	}

	status = zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_ENABLE_APS_LINK_KEY_AUTH, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, len, buf);

    ev_buf_free(buf);

    return status;
}

_CODE_ZCL_ status_t zcl_wwah_disableApsLinkKeyAuthCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, clustersToExempt_t *pCmd)
{
	u8 status = ZCL_STA_SUCCESS;
	u8 len = 1 + pCmd->numOfClustersToExempt * sizeof(u16);

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = pCmd->numOfClustersToExempt;
	for(u8 i = 0; i < pCmd->numOfClustersToExempt; i++){
		*pBuf++ = LO_UINT16(pCmd->pClusterID[i]);
		*pBuf++ = HI_UINT16(pCmd->pClusterID[i]);
	}

	status = zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_DISABLE_APS_LINK_KEY_AUTH, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, len, buf);

    ev_buf_free(buf);

    return status;
}

_CODE_ZCL_ status_t zcl_wwah_apsLinkKeyAuthQueryCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_apsLinkKeyAuthQueryCmd_t *pCmd)
{
	u8 buf[2];
	u8 *pBuf = buf;

	*pBuf++ = LO_UINT16(pCmd->clusterID);
	*pBuf++ = HI_UINT16(pCmd->clusterID);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_APS_LINK_KEY_AUTH_QUERY, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 2, buf);
}

_CODE_ZCL_ status_t zcl_wwah_requestNewApsLinkKeyCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_REQUEST_NEW_APS_LINK_KEY, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_enableAppEvtRetryAlgorithmCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_enableAppEvtRetryAlgorithmCmd_t *pCmd)
{
	u8 buf[7];
	u8 *pBuf = buf;

	*pBuf++ = pCmd->firstBackoffTimeInSeconds;
	*pBuf++ = pCmd->backoffSequenceCommonRatio;
	*pBuf++ = U32_BYTE0(pCmd->maxBackoffTimeInSeconds);
	*pBuf++ = U32_BYTE1(pCmd->maxBackoffTimeInSeconds);
	*pBuf++ = U32_BYTE2(pCmd->maxBackoffTimeInSeconds);
	*pBuf++ = U32_BYTE3(pCmd->maxBackoffTimeInSeconds);
	*pBuf++ = pCmd->maxReDeliveryAttepts;

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_ENABLE_APP_EVT_RETRY_ALGORITHM, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 7, buf);
}

_CODE_ZCL_ status_t zcl_wwah_disableAppEvtRetryAlgorithmCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_DISABLE_APP_EVT_RETRY_ALGORITHM, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_requestTimeCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_REQUEST_TIME, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_enableRejoinAlgorithmCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_enableRejoinAlgorithmCmd_t *pCmd)
{
	u8 buf[10];
	u8 *pBuf = buf;

	*pBuf++ = LO_UINT16(pCmd->fastRejoinTimeoutInSeconds);
	*pBuf++ = HI_UINT16(pCmd->fastRejoinTimeoutInSeconds);
	*pBuf++ = LO_UINT16(pCmd->durationBetweenEachRejoinInSeconds);
	*pBuf++ = HI_UINT16(pCmd->durationBetweenEachRejoinInSeconds);
	*pBuf++ = LO_UINT16(pCmd->fastRejoinFirstBackoffInSeconds);
	*pBuf++ = HI_UINT16(pCmd->fastRejoinFirstBackoffInSeconds);
	*pBuf++ = LO_UINT16(pCmd->maxBackoffTimeInSeconds);
	*pBuf++ = HI_UINT16(pCmd->maxBackoffTimeInSeconds);
	*pBuf++ = LO_UINT16(pCmd->maxBackoffIterations);
	*pBuf++ = HI_UINT16(pCmd->maxBackoffIterations);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_ENABLE_REJOIN_ALGORITHM, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 10, buf);
}

_CODE_ZCL_ status_t zcl_wwah_disableRejoinAlgorithmCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_DISABLE_REJOIN_ALGORITHM, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_setIasZoneEnrollmentMethodCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_setIasZoneEnrollmentMethodCmd_t *pCmd)
{
	u8 buf[1];

	buf[0] = (u8)pCmd->enrollmentMode;

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_SET_IAS_ZONE_ENROLLMENT_METHOD, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 1, buf);
}

_CODE_ZCL_ status_t zcl_wwah_clearBindingTableCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_CLEAR_BINDING_TABLE, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_enablePeriodicRouterCheckInsCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_enablePeriodicRouterCheckInsCmd_t *pCmd)
{
	u8 buf[2];
	u8 *pBuf = buf;

	*pBuf++ = LO_UINT16(pCmd->checkInInterval);
	*pBuf++ = HI_UINT16(pCmd->checkInInterval);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_ENABLE_PERIODIC_ROUTER_CHECK_INS, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 2, buf);
}

_CODE_ZCL_ status_t zcl_wwah_disablePeriodicRouterCheckInsCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_DISABLE_PERIODIC_ROUTER_CHECK_INS, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_setMacPollFailureWaitTimeCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_setMacPollFailureWaitTimeCmd_t *pCmd)
{
	u8 buf[1];

	buf[0] = pCmd->waitTime;

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_SET_MAC_POLL_FAILURE_WAIT_TIME, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 1, buf);
}

_CODE_ZCL_ status_t zcl_wwah_setPendingNwkUpdateCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_setPendingNwkUpdateCmd_t *pCmd)
{
	u8 buf[3];
	u8 *pBuf = buf;

	*pBuf++ = pCmd->channel;
	*pBuf++ = LO_UINT16(pCmd->panID);
	*pBuf++ = HI_UINT16(pCmd->panID);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_SET_PENDING_NWK_UPDATE, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 3, buf);
}

_CODE_ZCL_ status_t zcl_wwah_requireApsAcksOnUnicastsCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, clustersToExempt_t *pCmd)
{
	u8 status = ZCL_STA_SUCCESS;
	u8 len = 1 + pCmd->numOfClustersToExempt * sizeof(u16);

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = pCmd->numOfClustersToExempt;
	for(u8 i = 0; i < pCmd->numOfClustersToExempt; i++){
		*pBuf++ = LO_UINT16(pCmd->pClusterID[i]);
		*pBuf++ = HI_UINT16(pCmd->pClusterID[i]);
	}

	status = zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_REQUIRE_APS_ACKS_ON_UNICASTS, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, len, buf);

    ev_buf_free(buf);

    return status;
}

_CODE_ZCL_ status_t zcl_wwah_removeApsAcksOnUnicastsRequirementCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_REMOVE_APS_ACKS_ON_UNICASTS_REQUIREMENT, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_apsAckRequirementQueryCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_APS_ACK_REQUIREMENT_QUERY, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_debugReportQueryCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_debugReportQueryCmd_t *pCmd)
{
	u8 buf[1];

	buf[0] = pCmd->debugReportID;

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_DEBUG_REPORT_QUERY, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 1, buf);
}

_CODE_ZCL_ status_t zcl_wwah_surveyBeaconsCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_surveyBeaconsCmd_t *pCmd)
{
	u8 buf[1];

	buf[0] = pCmd->standardBeacons;

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_SURVEY_BEACONS, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 1, buf);
}

_CODE_ZCL_ status_t zcl_wwah_disableOtaDowngradesCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_DISABLE_OTA_DOWNGRADES, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_disableMgmtLeaveWithoutRejoinCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_DISABLE_MGMT_LEAVE_WITHOUT_REJOIN, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_disableTouchlinkInterpanMsgCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_DISABLE_TOUCHLINK_INTERPAN_MSG_SUPPORT, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_enableParentClassificationCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_ENABLE_PARENT_CLASSIFICATION, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_disableParentClassificationCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_DISABLE_PARENT_CLASSIFICATION, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_enableTcSecurityOnNwkKeyRotationCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_ENABLE_TC_SECURITY_ON_NWK_KEY_ROTATION, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_enableBadParentRecoveryCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_ENABLE_BAD_PARENT_RECOVERY, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_disableBadParentRecoveryCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_DISABLE_BAD_PARENT_RECOVERY, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_enableConfigurationModeCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_ENABLE_CONFIGURATION_MODE, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_disableConfigurationModeCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_DISABLE_CONFIGURATION_MODE, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_useTrustCenterForClusterCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_useTrustCenterForClusterCmd_t *pCmd)
{
	u8 status = ZCL_STA_SUCCESS;
	u8 len = 1 + pCmd->numOfClusters * sizeof(u16);

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = pCmd->numOfClusters;
	for(u8 i = 0; i < pCmd->numOfClusters; i++){
		*pBuf++ = LO_UINT16(pCmd->pClusterID[i]);
		*pBuf++ = HI_UINT16(pCmd->pClusterID[i]);
	}

	status = zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_USE_TRUST_CENTER_FOR_CLUSTER, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, len, buf);

    ev_buf_free(buf);

    return status;
}

_CODE_ZCL_ status_t zcl_wwah_trustCenterForClusterServerQueryCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_TRUST_CENTER_FOR_CLUSTER_SERVER_QUERY, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_wwah_apsLinkKeyAuthQueryRspCmd(u8 srcEp, epInfo_t *pDstEpInfo, u16 manuCode, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_apsLinkKeyAuthQueryRspCmd_t *pCmd)
{
	u8 buf[3];
	u8 *pBuf = buf;

	*pBuf++ = LO_UINT16(pCmd->clusterID);
	*pBuf++ = HI_UINT16(pCmd->clusterID);
	*pBuf++ = pCmd->status;

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_APS_LINK_KEY_AUTH_QUERY_RSP, TRUE,
    		ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, manuCode, seqNo, 3, buf);
}

_CODE_ZCL_ status_t zcl_wwah_poweringOffNotificationCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_poweringNotificationCmd_t *pCmd)
{
	u8 status = ZCL_STA_SUCCESS;
	u8 len = 4 + pCmd->manuPowerNotiReasonLen;

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = (u8)pCmd->powerNotiReason;
	*pBuf++ = LO_UINT16(pCmd->manuID);
	*pBuf++ = HI_UINT16(pCmd->manuID);
	*pBuf++ = pCmd->manuPowerNotiReasonLen;
	if(pCmd->manuPowerNotiReasonLen){
		memcpy(pBuf, pCmd->pManuPowerNotiReason, pCmd->manuPowerNotiReasonLen);
		pBuf += pCmd->manuPowerNotiReasonLen;
	}

	status = zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_POWERING_OFF_NOTIFICATION, TRUE,
    		ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, len, buf);

    ev_buf_free(buf);

    return status;
}

_CODE_ZCL_ status_t zcl_wwah_poweringOnNotificationCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_poweringNotificationCmd_t *pCmd)
{
	u8 status = ZCL_STA_SUCCESS;
	u8 len = 4 + pCmd->manuPowerNotiReasonLen;

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = (u8)pCmd->powerNotiReason;
	*pBuf++ = LO_UINT16(pCmd->manuID);
	*pBuf++ = HI_UINT16(pCmd->manuID);
	*pBuf++ = pCmd->manuPowerNotiReasonLen;
	if(pCmd->manuPowerNotiReasonLen){
		memcpy(pBuf, pCmd->pManuPowerNotiReason, pCmd->manuPowerNotiReasonLen);
		pBuf += pCmd->manuPowerNotiReasonLen;
	}

	status = zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_POWERING_ON_NOTIFICATION, TRUE,
    		ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, len, buf);

    ev_buf_free(buf);

    return status;
}

_CODE_ZCL_ status_t zcl_wwah_shortAddrChangeCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_shortAddrChangeCmd_t *pCmd)
{
	u8 buf[10];
	u8 *pBuf = buf;

	ZB_64BIT_ADDR_COPY(pBuf, pCmd->deviceEUI64);
	pBuf += EXT_ADDR_LEN;
	*pBuf++ = LO_UINT16(pCmd->deviceShort);
	*pBuf++ = HI_UINT16(pCmd->deviceShort);

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_SHORT_ADDRESS_CHANGE, TRUE,
    		ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, 10, buf);
}

_CODE_ZCL_ status_t zcl_wwah_apsAckRequirementQueryRspCmd(u8 srcEp, epInfo_t *pDstEpInfo, u16 manuCode, u8 disableDefaultRsp, u8 seqNo, clustersToExempt_t *pCmd)
{
	u8 status = ZCL_STA_SUCCESS;
	u8 len = 1 + pCmd->numOfClustersToExempt * sizeof(u16);

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = pCmd->numOfClustersToExempt;
	for(u8 i = 0; i < pCmd->numOfClustersToExempt; i++){
		*pBuf++ = LO_UINT16(pCmd->pClusterID[i]);
		*pBuf++ = HI_UINT16(pCmd->pClusterID[i]);
	}

	status = zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_APS_ACK_REQUIREMENT_QUERY_RSP, TRUE,
    		ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, manuCode, seqNo, len, buf);

    ev_buf_free(buf);

    return status;
}

_CODE_ZCL_ status_t zcl_wwah_powerDescriptorChangeCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_powerDescChangeCmd_t *pCmd)
{
	u8 buf[2];
	u8 *pBuf = buf;

	*pBuf++ = (pCmd->currentPowerMode << 4) + pCmd->availablePowerSrcs;
	*pBuf++ = (pCmd->currentPowerSrc << 4) + pCmd->currentPowerSrcLevel;

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_POWER_DESCRIPTOR_CHANGE, TRUE,
    		ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, 2, buf);
}

_CODE_ZCL_ status_t zcl_wwah_newDebugReportNotificationCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_newDbgReportNotiCmd_t *pCmd)
{
	u8 buf[5];
	u8 *pBuf = buf;

	*pBuf++ = pCmd->dbgReportID;
	*pBuf++ = U32_BYTE0(pCmd->sizeOfReport);
	*pBuf++ = U32_BYTE1(pCmd->sizeOfReport);
	*pBuf++ = U32_BYTE2(pCmd->sizeOfReport);
	*pBuf++ = U32_BYTE3(pCmd->sizeOfReport);

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_NEW_DEBUG_REPORT_NOTIFICATION, TRUE,
    		ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, 5, buf);
}

_CODE_ZCL_ status_t zcl_wwah_debugReportQueryRspCmd(u8 srcEp, epInfo_t *pDstEpInfo, u16 manuCode, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_dbgReportQueryRspCmd_t *pCmd)
{
	u8 status = ZCL_STA_SUCCESS;
	u8 len = 1 + pCmd->reportDataLen;//reportID + dataLen

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = pCmd->dbgReportID;
	if(pCmd->reportDataLen){
		memcpy(pBuf, pCmd->pReportData, pCmd->reportDataLen);
		pBuf += pCmd->reportDataLen;
	}

	status = zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_DEBUG_REPORT_QUERY_RSP, TRUE,
    		ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, manuCode, seqNo, len, buf);

    ev_buf_free(buf);

    return status;
}

_CODE_ZCL_ status_t zcl_wwah_trustCenterForClusterServerQueryRspCmd(u8 srcEp, epInfo_t *pDstEpInfo, u16 manufCode, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_tcForClusterServerQueryRspCmd_t *pCmd)
{
	u8 status = ZCL_STA_SUCCESS;
	u8 len = 1 + pCmd->numOfClusters * sizeof(u16);

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = pCmd->numOfClusters;
	for(u8 i = 0; i < pCmd->numOfClusters; i++){
		*pBuf++ = LO_UINT16(pCmd->pClusterID[i]);
		*pBuf++ = HI_UINT16(pCmd->pClusterID[i]);
	}

	status = zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_TRUST_CENTER_FOR_CLUSTER_SERVER_QUERY_RSP, TRUE,
    		ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, manufCode, seqNo, len, buf);

    ev_buf_free(buf);

    return status;
}

_CODE_ZCL_ status_t zcl_wwah_surveyBeaconsRspCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_wwah_surveyBeaconsRspCmd_t *pCmd)
{
	u8 status = ZCL_STA_SUCCESS;
	u8 len = 1 + pCmd->numOfBeacons * 4;

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = pCmd->numOfBeacons;
	for(u8 i = 0; i < pCmd->numOfBeacons; i++){
		*pBuf++ = LO_UINT16(pCmd->pBeacon[i].deviceShort);
		*pBuf++ = HI_UINT16(pCmd->pBeacon[i].deviceShort);
		*pBuf++ = pCmd->pBeacon[i].rssi;
		*pBuf++ = pCmd->pBeacon[i].classificationMask;
	}

	status = zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_WWAH, ZCL_CMD_WWAH_SURVEY_BEACONS_RSP, TRUE,
    		ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, len, buf);

    ev_buf_free(buf);

    return status;
}



_CODE_ZCL_ static u8 zcl_wwah_clustersToExemptPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
    u8 *pData = pInMsg->pData;

    if( (pInMsg->hdr.cmd == ZCL_CMD_WWAH_ENABLE_APS_LINK_KEY_AUTH) ||
    	(pInMsg->hdr.cmd == ZCL_CMD_WWAH_DISABLE_APS_LINK_KEY_AUTH) ){
		if(!(pInMsg->msg->indInfo.security_status & SECURITY_IN_APSLAYER)){
			return ZCL_STA_NOT_AUTHORIZED;
		}
    }

	if( (pInMsg->hdr.cmd == ZCL_CMD_WWAH_ENABLE_APS_LINK_KEY_AUTH) ||
		(pInMsg->hdr.cmd == ZCL_CMD_WWAH_DISABLE_APS_LINK_KEY_AUTH)	){
		g_zcl_apsLinkKeyAuthInfo.status = (pInMsg->hdr.cmd == ZCL_CMD_WWAH_ENABLE_APS_LINK_KEY_AUTH) ? APS_LINK_KEY_AUTH_ENABLE
																									 : APS_LINK_KEY_AUTH_DISABLE;
		g_zcl_apsLinkKeyAuthInfo.num = *pData++;
		g_zcl_apsLinkKeyAuthInfo.num = (g_zcl_apsLinkKeyAuthInfo.num > CLUSTER_ID_MAX_NUM) ? CLUSTER_ID_MAX_NUM
																						   : g_zcl_apsLinkKeyAuthInfo.num;
		for(u8 i = 0; i < g_zcl_apsLinkKeyAuthInfo.num; i++){
			g_zcl_apsLinkKeyAuthInfo.clusterID[i] = BUILD_U16(pData[0], pData[1]);
			pData += 2;
		}

		TL_SCHEDULE_TASK(zcl_wwah_apsLinkKeyAuthInfoSave, NULL);
	}else if(pInMsg->hdr.cmd == ZCL_CMD_WWAH_REQUIRE_APS_ACKS_ON_UNICASTS){
		g_zcl_apsAcksRequireInfo.num = *pData++;
		g_zcl_apsAcksRequireInfo.num = (g_zcl_apsAcksRequireInfo.num > CLUSTER_ID_MAX_NUM) ? CLUSTER_ID_MAX_NUM
																						   : g_zcl_apsAcksRequireInfo.num;
		for(u8 i = 0; i < g_zcl_apsAcksRequireInfo.num; i++){
			g_zcl_apsAcksRequireInfo.clusterID[i] = BUILD_U16(pData[0], pData[1]);
			pData += 2;
		}

		TL_SCHEDULE_TASK(zcl_wwah_apsAcksRequireInfoSave, NULL);
	}

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_apsLinkKeyAuthQueryPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
    u8 *pData = pInMsg->pData;

    if(UNICAST_MSG(pApsdeInd)){
		zcl_wwah_apsLinkKeyAuthQueryRspCmd_t queryRsp;

		queryRsp.clusterID = BUILD_U16(pData[0], pData[1]);
		queryRsp.status = (bool)zcl_wwah_apsLinkKeyAuthSearch(queryRsp.clusterID);

		epInfo_t dstEp;
		TL_SETSTRUCTCONTENT(dstEp, 0);

		dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
		dstEp.dstEp = pApsdeInd->indInfo.src_ep;
		dstEp.profileId = pApsdeInd->indInfo.profile_id;
		dstEp.txOptions |= APS_TX_OPT_ACK_TX;
		if(pInMsg->msg->indInfo.security_status & SECURITY_IN_APSLAYER){
			dstEp.txOptions |= APS_TX_OPT_SECURITY_ENABLED;
		}

		status = zcl_wwah_apsLinkKeyAuthQueryRspCmd(pApsdeInd->indInfo.dst_ep, &dstEp, pInMsg->hdr.manufCode, TRUE, pInMsg->hdr.seqNum, &queryRsp);

		if(status == ZCL_STA_SUCCESS){
			status = ZCL_STA_CMD_HAS_RESP;
		}
    }

	return status;
}

_CODE_ZCL_ static u8 zcl_cmdNoPayloadPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

    if( (pInMsg->hdr.cmd == ZCL_CMD_WWAH_DISABLE_PERIODIC_ROUTER_CHECK_INS) ||
    	(pInMsg->hdr.cmd == ZCL_CMD_WWAH_DISABLE_MGMT_LEAVE_WITHOUT_REJOIN) ){
		if(!(pInMsg->msg->indInfo.security_status & SECURITY_IN_APSLAYER)){
			return ZCL_STA_NOT_AUTHORIZED;
		}
    }

    if(UNICAST_MSG(pApsdeInd)){
    	if(pInMsg->clusterAppCb){
    		zclIncomingAddrInfo_t addrInfo;
    		addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
    		addrInfo.profileId = pApsdeInd->indInfo.profile_id;
    		addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
    		addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
    		addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
    		addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

    		//no payload.

    		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, NULL);
    	}else{
    		status = ZCL_STA_FAILURE;
    	}
    }

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_enableAppEvtRetryAlgorithmPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
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

		zcl_wwah_enableAppEvtRetryAlgorithmCmd_t cmd;
		TL_SETSTRUCTCONTENT(cmd, 0);

		cmd.firstBackoffTimeInSeconds = *pData++;
		cmd.backoffSequenceCommonRatio = *pData++;
		cmd.maxBackoffTimeInSeconds = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
		pData += 4;
		cmd.maxReDeliveryAttepts = *pData++;

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmd);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_enableRejoinAlgorithmPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
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

		zcl_wwah_enableRejoinAlgorithmCmd_t cmd;
		TL_SETSTRUCTCONTENT(cmd, 0);

		cmd.fastRejoinTimeoutInSeconds = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		cmd.durationBetweenEachRejoinInSeconds = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		cmd.fastRejoinFirstBackoffInSeconds = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		cmd.maxBackoffTimeInSeconds = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		cmd.maxBackoffIterations = BUILD_U16(pData[0], pData[1]);
		pData += 2;

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmd);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_setIasZoneEnrollmentMethodPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
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

		zcl_wwah_setIasZoneEnrollmentMethodCmd_t cmd;
		TL_SETSTRUCTCONTENT(cmd, 0);

		cmd.enrollmentMode = *pData++;

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmd);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_clearBindingTablePrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

    //TODO: need check this?
//	if(!(pInMsg->msg->indInfo.security_status & SECURITY_IN_APSLAYER)){
//		return ZCL_STA_NOT_AUTHORIZED;
//	}

    if(UNICAST_MSG(pApsdeInd)){
    	aps_bindingTab_clear();
    }

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_enablePeriodicRouterCheckInsPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
    u8 *pData = pInMsg->pData;

	if(!(pInMsg->msg->indInfo.security_status & SECURITY_IN_APSLAYER)){
		return ZCL_STA_NOT_AUTHORIZED;
	}

    if(UNICAST_MSG(pApsdeInd)){
    	if(pInMsg->clusterAppCb){
    		zclIncomingAddrInfo_t addrInfo;
    		addrInfo.apsSec = pInMsg->msg->indInfo.security_status & SECURITY_IN_APSLAYER ? TRUE : FALSE;
    		addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
    		addrInfo.profileId = pApsdeInd->indInfo.profile_id;
    		addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
    		addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
    		addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
    		addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

        	zcl_wwah_enablePeriodicRouterCheckInsCmd_t cmd;

        	cmd.checkInInterval = BUILD_U16(pData[0], pData[1]);

        	status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmd);
    	}else{
    		status = ZCL_STA_FAILURE;
    	}
    }

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_setMacPollFailureWaitTimePrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
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

		zcl_wwah_setMacPollFailureWaitTimeCmd_t cmd;
		TL_SETSTRUCTCONTENT(cmd, 0);

		cmd.waitTime = *pData++;

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmd);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_setPendingNwkUpdatePrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
    u8 *pData = pInMsg->pData;

	if(!(pInMsg->msg->indInfo.security_status & SECURITY_IN_APSLAYER)){
		return ZCL_STA_NOT_AUTHORIZED;
	}

	if(UNICAST_MSG(pApsdeInd)){
		if(pInMsg->clusterAppCb){
			zclIncomingAddrInfo_t addrInfo;
			addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
			addrInfo.profileId = pApsdeInd->indInfo.profile_id;
			addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
			addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
			addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
			addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

			zcl_wwah_setPendingNwkUpdateCmd_t cmd;
			TL_SETSTRUCTCONTENT(cmd, 0);

			cmd.channel = *pData++;
			cmd.panID = BUILD_U16(pData[0], pData[1]);

			status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmd);
		}else{
			status = ZCL_STA_FAILURE;
		}
	}

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_removeApsAcksOnUnicastsRequirementPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

    if(UNICAST_MSG(pApsdeInd)){
    	zcl_wwah_apsAcksRequireInfoClear();
    	TL_SCHEDULE_TASK(zcl_wwah_apsAcksRequireInfoSave, NULL);
    }

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_apsAckRequirementQueryPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

    if(UNICAST_MSG(pApsdeInd)){
    	clustersToExempt_t clustersToExempt;
    	TL_SETSTRUCTCONTENT(clustersToExempt, 0);

    	clustersToExempt.numOfClustersToExempt = g_zcl_apsAcksRequireInfo.num;
    	clustersToExempt.pClusterID = g_zcl_apsAcksRequireInfo.clusterID;

		epInfo_t dstEp;
		TL_SETSTRUCTCONTENT(dstEp, 0);

		dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
		dstEp.dstEp = pApsdeInd->indInfo.src_ep;
		dstEp.profileId = pApsdeInd->indInfo.profile_id;
		dstEp.txOptions |= APS_TX_OPT_ACK_TX;
		if(pInMsg->msg->indInfo.security_status & SECURITY_IN_APSLAYER){
			dstEp.txOptions |= APS_TX_OPT_SECURITY_ENABLED;
		}

    	status = zcl_wwah_apsAckRequirementQueryRspCmd(pApsdeInd->indInfo.dst_ep, &dstEp, pInMsg->hdr.manufCode, TRUE, pInMsg->hdr.seqNum, &clustersToExempt);

		if(status == ZCL_STA_SUCCESS){
			status = ZCL_STA_CMD_HAS_RESP;
		}
    }

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_debugReportQueryPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
    u8 *pData = pInMsg->pData;

    if(UNICAST_MSG(pApsdeInd)){
    	u8 curDbgReportID;
    	u16 len;

    	if(zcl_getAttrVal(pApsdeInd->indInfo.dst_ep, ZCL_CLUSTER_WWAH, ZCL_ATTRID_WWAH_CURRENT_DEBUG_REPORT_ID, &len, (u8 *)&curDbgReportID) != ZCL_STA_SUCCESS){
    		return ZCL_STA_NOT_FOUND;
    	}

    	u8 debugReportID = *pData++;

    	if(debugReportID != curDbgReportID){
    		return ZCL_STA_NOT_FOUND;
    	}

    	u8 reportBuf[] = "TEST\0";
		zcl_wwah_dbgReportQueryRspCmd_t dbgReportQueryRspCmd;
		TL_SETSTRUCTCONTENT(dbgReportQueryRspCmd, 0);

		dbgReportQueryRspCmd.dbgReportID = debugReportID;
		dbgReportQueryRspCmd.reportDataLen = sizeof(reportBuf);
		dbgReportQueryRspCmd.pReportData = reportBuf;

		epInfo_t dstEp;
		TL_SETSTRUCTCONTENT(dstEp, 0);

		dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
		dstEp.dstEp = pApsdeInd->indInfo.src_ep;
		dstEp.profileId = pApsdeInd->indInfo.profile_id;
		dstEp.txOptions |= APS_TX_OPT_ACK_TX;
		if(pInMsg->msg->indInfo.security_status & SECURITY_IN_APSLAYER){
			dstEp.txOptions |= APS_TX_OPT_SECURITY_ENABLED;
		}

		zcl_wwah_debugReportQueryRspCmd(pApsdeInd->indInfo.dst_ep, &dstEp, pInMsg->hdr.manufCode, TRUE, pInMsg->hdr.seqNum, &dbgReportQueryRspCmd);

		if(status == ZCL_STA_SUCCESS){
			status = ZCL_STA_CMD_HAS_RESP;
		}
    }

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_surveyBeaconsPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
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

		zcl_wwah_surveyBeaconsCmd_t cmd;
		TL_SETSTRUCTCONTENT(cmd, 0);

		cmd.standardBeacons = *pData++;

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmd);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}




#if 0
_CODE_ZCL_ static u8 zcl_wwah_disableMgmtLeaveWithoutRejoinPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

	if(!(pInMsg->msg->indInfo.security_status & SECURITY_IN_APSLAYER)){
		return ZCL_STA_NOT_AUTHORIZED;
	}

    if(UNICAST_MSG(pApsdeInd)){
    	if(pInMsg->clusterAppCb){
    		zclIncomingAddrInfo_t addrInfo;
    		addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
    		addrInfo.profileId = pApsdeInd->indInfo.profile_id;
    		addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
    		addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
    		addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
    		addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

    		//no payload

    		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, NULL);
    	}else{
    		status = ZCL_STA_FAILURE;
    	}
    }

	return status;
}
#endif

_CODE_ZCL_ static u8 zcl_wwah_useTrustCenterForClusterPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
    u8 *pData = pInMsg->pData;

    if(UNICAST_MSG(pApsdeInd)){
    	wwah_useTCForClusterInfo_t useTCForClusterInfo;
    	TL_SETSTRUCTCONTENT(useTCForClusterInfo, 0);

    	u8 numOfCluster = *pData++;
    	numOfCluster = (numOfCluster > CLUSTER_ID_MAX_NUM) ? CLUSTER_ID_MAX_NUM : numOfCluster;

    	clusterInfo_t *pCluster = NULL;
    	for(u8 i = 0; i < numOfCluster; i++){
    		pCluster = zcl_findCluster(pApsdeInd->indInfo.dst_ep, BUILD_U16(pData[0], pData[1]));
    		if(pCluster){
    			useTCForClusterInfo.clusterID[useTCForClusterInfo.num++] = BUILD_U16(pData[0], pData[1]);
    		}
    		pData += 2;
    	}

    	g_zcl_useTCForClusterInfo.num = useTCForClusterInfo.num;
		for(u8 i = 0; i < useTCForClusterInfo.num; i++){
			g_zcl_useTCForClusterInfo.clusterID[i] = useTCForClusterInfo.clusterID[i];
		}

		TL_SCHEDULE_TASK(zcl_wwah_useTCForClusterInfoSave, NULL);

		if(pInMsg->clusterAppCb && useTCForClusterInfo.num){
			zclIncomingAddrInfo_t addrInfo;
			addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
			addrInfo.profileId = pApsdeInd->indInfo.profile_id;
			addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
			addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
			addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
			addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

			status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &g_zcl_useTCForClusterInfo);
		}
    }

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_trustCenterForClusterServerQueryPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

    if(UNICAST_MSG(pApsdeInd)){
    	zcl_wwah_tcForClusterServerQueryRspCmd_t tcForClusterServerQueryRspCmd;
    	TL_SETSTRUCTCONTENT(tcForClusterServerQueryRspCmd, 0);

    	tcForClusterServerQueryRspCmd.numOfClusters = g_zcl_useTCForClusterInfo.num;
    	tcForClusterServerQueryRspCmd.pClusterID = g_zcl_useTCForClusterInfo.clusterID;

		epInfo_t dstEp;
		TL_SETSTRUCTCONTENT(dstEp, 0);

		dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
		dstEp.dstEp = pApsdeInd->indInfo.src_ep;
		dstEp.profileId = pApsdeInd->indInfo.profile_id;
		dstEp.txOptions |= APS_TX_OPT_ACK_TX;
		if(pInMsg->msg->indInfo.security_status & SECURITY_IN_APSLAYER){
			dstEp.txOptions |= APS_TX_OPT_SECURITY_ENABLED;
		}

		status = zcl_wwah_trustCenterForClusterServerQueryRspCmd(pApsdeInd->indInfo.dst_ep, &dstEp, pInMsg->hdr.manufCode, TRUE, pInMsg->hdr.seqNum, &tcForClusterServerQueryRspCmd);

		if(status == ZCL_STA_SUCCESS){
			status = ZCL_STA_CMD_HAS_RESP;
		}
    }

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_apsLinkKeyAuthQueryRspPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
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

		zcl_wwah_apsLinkKeyAuthQueryRspCmd_t cmd;
		TL_SETSTRUCTCONTENT(cmd, 0);

		cmd.clusterID = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		cmd.status = *pData++;

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmd);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_poweringNotificationPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
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

		zcl_wwah_poweringNotificationCmd_t cmd;
		TL_SETSTRUCTCONTENT(cmd, 0);

		cmd.powerNotiReason = *pData++;
		cmd.manuID = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		cmd.manuPowerNotiReasonLen = *pData++;
		cmd.pManuPowerNotiReason = cmd.manuPowerNotiReasonLen ? pData : NULL;

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmd);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_shortAddressChangePrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
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

		zcl_wwah_shortAddrChangeCmd_t cmd;
		TL_SETSTRUCTCONTENT(cmd, 0);

		ZB_64BIT_ADDR_COPY(cmd.deviceEUI64, pData);
		pData += EXT_ADDR_LEN;
		cmd.deviceShort = BUILD_U16(pData[0], pData[1]);
		pData += 2;

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmd);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_apsAckRequirementQueryRspPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
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

		clustersToExempt_t cmd;
		TL_SETSTRUCTCONTENT(cmd, 0);

		cmd.numOfClustersToExempt = *pData++;
		cmd.pClusterID = cmd.numOfClustersToExempt ? (u16 *)pData : NULL;

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmd);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_powerDescriptorChangePrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
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

		zcl_wwah_powerDescChangeCmd_t cmd;
		TL_SETSTRUCTCONTENT(cmd, 0);

		cmd.currentPowerMode = (pData[0] & 0xf0) >> 4;
		cmd.availablePowerSrcs = pData[0] & 0x0f;
		cmd.currentPowerSrc = (pData[1] & 0xf0) >> 4;
		cmd.currentPowerSrcLevel = pData[1] & 0x0f;

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmd);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_newDebugReportNotificationPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
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

		zcl_wwah_newDbgReportNotiCmd_t cmd;
		TL_SETSTRUCTCONTENT(cmd, 0);

		cmd.dbgReportID = *pData++;
		cmd.sizeOfReport = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
		pData += 4;

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmd);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_debugReportQueryRspPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
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

		zcl_wwah_dbgReportQueryRspCmd_t cmd;
		TL_SETSTRUCTCONTENT(cmd, 0);

		cmd.dbgReportID = *pData++;
		cmd.reportDataLen = *pData++;
		cmd.pReportData = cmd.reportDataLen ? pData : NULL;

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmd);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_trustCenterForClusterServerQueryRspPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
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

		zcl_wwah_tcForClusterServerQueryRspCmd_t cmd;
		TL_SETSTRUCTCONTENT(cmd, 0);

		cmd.numOfClusters = *pData++;
		cmd.pClusterID = cmd.numOfClusters ? (u16 *)pData : NULL;

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmd);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_surveyBeaconsRspPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
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

		zcl_wwah_surveyBeaconsRspCmd_t cmd;
		TL_SETSTRUCTCONTENT(cmd, 0);

		cmd.numOfBeacons = *pData++;
		cmd.pBeacon = cmd.numOfBeacons ? (beaconSurvey_t *)pData : NULL;

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmd);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static u8 zcl_wwah_clientCmdHandler(zclIncoming_t *pInMsg)
{
    u8 status = ZCL_STA_SUCCESS;

    switch(pInMsg->hdr.cmd)
    {
		case ZCL_CMD_WWAH_ENABLE_APS_LINK_KEY_AUTH:
		case ZCL_CMD_WWAH_DISABLE_APS_LINK_KEY_AUTH:
		case ZCL_CMD_WWAH_REQUIRE_APS_ACKS_ON_UNICASTS:
			status = zcl_wwah_clustersToExemptPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_APS_LINK_KEY_AUTH_QUERY:
			status = zcl_wwah_apsLinkKeyAuthQueryPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_REQUEST_NEW_APS_LINK_KEY:
			status = zcl_cmdNoPayloadPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_ENABLE_APP_EVT_RETRY_ALGORITHM:
			status = zcl_wwah_enableAppEvtRetryAlgorithmPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_DISABLE_APP_EVT_RETRY_ALGORITHM:
			status = zcl_cmdNoPayloadPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_REQUEST_TIME:
			status = zcl_cmdNoPayloadPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_ENABLE_REJOIN_ALGORITHM:
			status = zcl_wwah_enableRejoinAlgorithmPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_DISABLE_REJOIN_ALGORITHM:
			status = zcl_cmdNoPayloadPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_SET_IAS_ZONE_ENROLLMENT_METHOD:
			status = zcl_wwah_setIasZoneEnrollmentMethodPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_CLEAR_BINDING_TABLE:
			status = zcl_wwah_clearBindingTablePrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_ENABLE_PERIODIC_ROUTER_CHECK_INS:
			status = zcl_wwah_enablePeriodicRouterCheckInsPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_DISABLE_PERIODIC_ROUTER_CHECK_INS:
			status = zcl_cmdNoPayloadPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_SET_MAC_POLL_FAILURE_WAIT_TIME:
			status = zcl_wwah_setMacPollFailureWaitTimePrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_SET_PENDING_NWK_UPDATE:
			status = zcl_wwah_setPendingNwkUpdatePrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_REMOVE_APS_ACKS_ON_UNICASTS_REQUIREMENT:
			status = zcl_wwah_removeApsAcksOnUnicastsRequirementPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_APS_ACK_REQUIREMENT_QUERY:
			status = zcl_wwah_apsAckRequirementQueryPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_DEBUG_REPORT_QUERY:
			status = zcl_wwah_debugReportQueryPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_SURVEY_BEACONS:
			status = zcl_wwah_surveyBeaconsPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_DISABLE_OTA_DOWNGRADES:
		case ZCL_CMD_WWAH_DISABLE_MGMT_LEAVE_WITHOUT_REJOIN:
		case ZCL_CMD_WWAH_DISABLE_TOUCHLINK_INTERPAN_MSG_SUPPORT:
		case ZCL_CMD_WWAH_ENABLE_PARENT_CLASSIFICATION:
		case ZCL_CMD_WWAH_DISABLE_PARENT_CLASSIFICATION:
		case ZCL_CMD_WWAH_ENABLE_TC_SECURITY_ON_NWK_KEY_ROTATION:
		case ZCL_CMD_WWAH_ENABLE_BAD_PARENT_RECOVERY:
		case ZCL_CMD_WWAH_DISABLE_BAD_PARENT_RECOVERY:
		case ZCL_CMD_WWAH_ENABLE_CONFIGURATION_MODE:
		case ZCL_CMD_WWAH_DISABLE_CONFIGURATION_MODE:
			status = zcl_cmdNoPayloadPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_USE_TRUST_CENTER_FOR_CLUSTER:
			status = zcl_wwah_useTrustCenterForClusterPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_TRUST_CENTER_FOR_CLUSTER_SERVER_QUERY:
			status = zcl_wwah_trustCenterForClusterServerQueryPrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
    }

    return status;
}

_CODE_ZCL_ static u8 zcl_wwah_serverCmdHandler(zclIncoming_t *pInMsg)
{
    u8 status = ZCL_STA_SUCCESS;

    switch(pInMsg->hdr.cmd)
    {
		case ZCL_CMD_WWAH_APS_LINK_KEY_AUTH_QUERY_RSP:
			status = zcl_wwah_apsLinkKeyAuthQueryRspPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_POWERING_OFF_NOTIFICATION:
		case ZCL_CMD_WWAH_POWERING_ON_NOTIFICATION:
			status = zcl_wwah_poweringNotificationPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_SHORT_ADDRESS_CHANGE:
			status = zcl_wwah_shortAddressChangePrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_APS_ACK_REQUIREMENT_QUERY_RSP:
			status = zcl_wwah_apsAckRequirementQueryRspPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_POWER_DESCRIPTOR_CHANGE:
			status = zcl_wwah_powerDescriptorChangePrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_NEW_DEBUG_REPORT_NOTIFICATION:
			status = zcl_wwah_newDebugReportNotificationPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_DEBUG_REPORT_QUERY_RSP:
			status = zcl_wwah_debugReportQueryRspPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_TRUST_CENTER_FOR_CLUSTER_SERVER_QUERY_RSP:
			status = zcl_wwah_trustCenterForClusterServerQueryRspPrc(pInMsg);
			break;
		case ZCL_CMD_WWAH_SURVEY_BEACONS_RSP:
			status = zcl_wwah_surveyBeaconsRspPrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_wwah_cmdHandler(zclIncoming_t *pInMsg)
{
	if (pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR) {
		return zcl_wwah_clientCmdHandler(pInMsg);
	} else {
		return zcl_wwah_serverCmdHandler(pInMsg);
	}
}

#endif /* ZCL_WWAH */
