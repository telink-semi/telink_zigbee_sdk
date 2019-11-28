/********************************************************************************************************
 * @file     zcl_ias_ace.c
 *
 * @brief	 APIs for Ancillary Control Equipment of the IAS system.
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

#ifdef ZCL_IAS_ACE
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
static status_t zcl_iasAce_cmdHandler(zclIncoming_t *pInMsg);

_CODE_ZCL_ status_t zcl_iasAce_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
	return zcl_registerCluster(endpoint, ZCL_CLUSTER_SS_IAS_ACE, attrNum, attrTbl, zcl_iasAce_cmdHandler, cb);
}


_CODE_ZCL_ status_t zcl_iasAce_armRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 armNotification)
{
	u8 buf[1];

	buf[0] = armNotification;

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SS_IAS_ACE, ZCL_CMD_ARM_RESPONSE, TRUE,
					ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, 1, buf);
}

_CODE_ZCL_ status_t zcl_iasAce_getZoneIdMapRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 *pZoneIdMapSection)
{
	u8 buf[32];
	u8 *pBuf = buf;

    for(u8 i = 0; i < 16; i++){
    	*pBuf++ = pZoneIdMapSection[1];
    	*pBuf++ = pZoneIdMapSection[0];
    	pZoneIdMapSection += 2;
    }

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SS_IAS_ACE, ZCL_CMD_GET_ZONE_ID_MAP_RESPONSE, TRUE,
					ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, 32, (u8*)buf);
}

_CODE_ZCL_ status_t zcl_iasAce_getZoneInfoRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, getZoneInfoRsp_t *pGetZoneInfoRsp)
{
	u8 buf[36];
	u8 *pBuf = buf;

	*pBuf++ = pGetZoneInfoRsp->zoneId;
	*pBuf++ = LO_UINT16(pGetZoneInfoRsp->zoneType);
	*pBuf++ = HI_UINT16(pGetZoneInfoRsp->zoneType);
	memcpy(pBuf, pGetZoneInfoRsp->ieeeAddr, 8);
	pBuf += 8;
	*pBuf++ = pGetZoneInfoRsp->zoneLabel.strLen;
	if(pGetZoneInfoRsp->zoneLabel.strLen){
		memcpy(pBuf, pGetZoneInfoRsp->zoneLabel.pStr, pGetZoneInfoRsp->zoneLabel.strLen);
		pBuf += pGetZoneInfoRsp->zoneLabel.strLen;
	}

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SS_IAS_ACE, ZCL_CMD_GET_ZONE_INFORMATION_RESPONSE, TRUE,
					ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_iasAce_zoneStatusChanged(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zoneStatusChanged_t *pZoneStatusChanged)
{
	u8 buf[29];
	u8 *pBuf = buf;

	*pBuf++ = pZoneStatusChanged->zoneId;
	*pBuf++ = LO_UINT16(pZoneStatusChanged->zoneStatus);
	*pBuf++ = HI_UINT16(pZoneStatusChanged->zoneStatus);
	*pBuf++ = pZoneStatusChanged->audibleNotification;
	*pBuf++ = pZoneStatusChanged->zoneLabel.strLen;
	if(pZoneStatusChanged->zoneLabel.strLen){
		memcpy(pBuf, pZoneStatusChanged->zoneLabel.pStr, pZoneStatusChanged->zoneLabel.strLen);
		pBuf += pZoneStatusChanged->zoneLabel.strLen;
	}

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SS_IAS_ACE, ZCL_CMD_ZONE_STATUS_CHANGED, TRUE,
					ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_iasAce_panelStatusChanged(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, panelStatusChanged_t *pPanelStatusChanged)
{
	u8 buf[4];

	buf[0] = pPanelStatusChanged->panelStatus;
    buf[1] = pPanelStatusChanged->secondsRemaining;
    buf[2] = pPanelStatusChanged->audibleNotification;
    buf[3] = pPanelStatusChanged->alarmStatus;

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SS_IAS_ACE, ZCL_CMD_PANEL_STATUS_CHANGED, TRUE,
					ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, 4, buf);
}

_CODE_ZCL_ status_t zcl_iasAce_getPanelStatusRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, getPanelStatusRsp_t *pGetPanelStatusRsp)
{
	u8 buf[4];

	buf[0] = pGetPanelStatusRsp->panelStatus;
    buf[1] = pGetPanelStatusRsp->secondsRemaining;
    buf[2] = pGetPanelStatusRsp->audibleNotification;
    buf[3] = pGetPanelStatusRsp->panelStatus;

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SS_IAS_ACE, ZCL_CMD_GET_PANEL_STATUS_RESPONSE, TRUE,
					ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, 4, buf);
}

_CODE_ZCL_ status_t zcl_iasAce_setBypassedZoneList(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, setBypassedZoneList_t *pSetBypassedZoneList)
{
	u8 len = 1;

	if(pSetBypassedZoneList->numOfZones){
		len += pSetBypassedZoneList->numOfZones;
	}

	u8 *buf = ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = pSetBypassedZoneList->numOfZones;
	if(pSetBypassedZoneList->numOfZones){
		memcpy(pBuf, pSetBypassedZoneList->zoneIds, pSetBypassedZoneList->numOfZones);
		pBuf += pSetBypassedZoneList->numOfZones;
	}

	zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SS_IAS_ACE, ZCL_CMD_SET_BYPASSED_ZONE_LIST, TRUE,
				ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);

	ev_buf_free(buf);

	return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ status_t zcl_iasAce_bypassResp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, bypassRsp_t *pBypassRsp)
{
	u8 len = 1;

	if(pBypassRsp->numOfZones){
		len += pBypassRsp->numOfZones;
	}

	u8 *buf = ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = pBypassRsp->numOfZones;
	if(pBypassRsp->numOfZones){
		memcpy(pBuf, pBypassRsp->zoneIds, pBypassRsp->numOfZones);
		pBuf += pBypassRsp->numOfZones;
	}

	zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SS_IAS_ACE, ZCL_CMD_BYPASS_RESPONSE, TRUE,
				ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);

	ev_buf_free(buf);

	return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ status_t zcl_iasAce_getZoneStatusResp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, getZoneStatusRsp_t *pGetZoneStatusRsp)
{
	u8 len = 2;

	if(pGetZoneStatusRsp->numOfZones){
		len += (pGetZoneStatusRsp->numOfZones * 3);
	}

	u8 *buf = ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = pGetZoneStatusRsp->zoneStatusComplete;
	*pBuf++ = pGetZoneStatusRsp->numOfZones;
	if(pGetZoneStatusRsp->numOfZones){
		for(u8 i = 0; i < pGetZoneStatusRsp->numOfZones; i++){
			memcpy(pBuf, &pGetZoneStatusRsp->zoneStatusInfo[i], 3);
			pBuf += 3;
		}
	}

	zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SS_IAS_ACE, ZCL_CMD_GET_ZONE_STATUS_RESPONSE, TRUE,
				ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);

	ev_buf_free(buf);

	return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ status_t zcl_iasAce_arm(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, arm_t *pArm)
{
	u8 buf[11];
	bu *pBuf = buf;

	*pBuf++ = pArm->armMode;
	*pBuf++ = pArm->code.strLen;
	if(pArm->code.strLen){
		memcpy(pBuf, pArm->code.pStr, pArm->code.strLen);
		pBuf += pArm->code.strLen;
	}
	*pBuf++ = pArm->zoneId;

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SS_IAS_ACE, ZCL_CMD_ARM, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}

_CODE_ZCL_ status_t zcl_iasAce_bypass(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, bypass_t *pBypass)
{
	u8 len = 10;

	if(pBypass->numOfZones){
		len += pBypass->numOfZones;
	}

	u8 *buf = ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = pBypass->numOfZones;
	if(pBypass->numOfZones){
		memcpy(pBuf, pBypass->zoneIds, pBypass->numOfZones);
		pBuf += pBypass->numOfZones;
	}
	*pBuf++ = pBypass->code.strLen;
	if(pBypass->code.strLen){
		memcpy(pBuf, pBypass->code.pStr, pBypass->code.strLen);
		pBuf += pBypass->code.strLen;
	}
    
	zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SS_IAS_ACE, ZCL_CMD_BYPASS, TRUE,
				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);

	ev_buf_free(buf);

	return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ status_t zcl_iasAce_emergency(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SS_IAS_ACE, ZCL_CMD_EMERGENCY, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_iasAce_fire(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SS_IAS_ACE, ZCL_CMD_FIRE, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_iasAce_panic(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SS_IAS_ACE, ZCL_CMD_PANIC, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_iasAce_getZoneIdMap(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SS_IAS_ACE, ZCL_CMD_GET_ZONE_ID_MAP, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_iasAce_getZoneInfo(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 ZoneId)
{
    u8 buf[1];

    buf[0] = ZoneId;

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SS_IAS_ACE, ZCL_CMD_GET_ZONE_INFORMATION, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 1, buf);
}

_CODE_ZCL_ status_t zcl_iasAce_getPanelStatus(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SS_IAS_ACE, ZCL_CMD_GET_PANEL_STATUS, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_iasAce_getBypassedZoneList(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SS_IAS_ACE, ZCL_CMD_GET_BYPASSED_ZONE_LIST, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_iasAce_getZoneStatus(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, getZoneStatus_t *pGetZoneStatus)
{
    u8 buf[5];

    buf[0] = pGetZoneStatus->startingZoneId;
    buf[1] = pGetZoneStatus->maxNumOfZoneIds;
    buf[2] = pGetZoneStatus->zoneStatusMaskFlag;
    buf[3] = LO_UINT16(pGetZoneStatus->zoneStatusMask);
    buf[4] = HI_UINT16(pGetZoneStatus->zoneStatusMask);

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SS_IAS_ACE, ZCL_CMD_GET_ZONE_STATUS, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 5, buf);
}


_CODE_ZCL_ static u8 zcl_parseUTF8String(u8 *pData, UTF8String_t *pString, u8 maxLen)
{
	u8 len = 0;
	pString->strLen = *pData++;

	if(pString->strLen == 0xFF){
		pString->strLen = 0;
	}

	if(pString->strLen == 0){
		pString->pStr = NULL;
	}else{
		len = pString->strLen;

		pString->strLen = (len > maxLen) ? maxLen : len;
		pString->pStr = pData;
	}

	return (len + 1);
}

_CODE_ZCL_ static void zcl_armParse(u8 *pData, arm_t *pCmd)
{
	pCmd->armMode = *pData++;

	u8 len = zcl_parseUTF8String(pData, &pCmd->code, 8);
	pData += len;

	pCmd->zoneId = *pData++;
}

_CODE_ZCL_ static void zcl_getZoneInfoRspParse(u8 *pData, getZoneInfoRsp_t *pCmd)
{
	pCmd->zoneId = *pData++;
	pCmd->zoneType = BUILD_U16(pData[0], pData[1]);
	pData += 2;
	memcpy(pCmd->ieeeAddr, pData, 8);
	pData += 8;
	zcl_parseUTF8String(pData, &pCmd->zoneLabel, 24);
}

_CODE_ZCL_ static void zcl_zoneStatusChangedParse(u8 *pData, zoneStatusChanged_t *pCmd)
{
	pCmd->zoneId = *pData++;
	pCmd->zoneStatus = BUILD_U16(pData[0], pData[1]);
	pData += 2;
	pCmd->audibleNotification = *pData++;
	zcl_parseUTF8String(pData, &pCmd->zoneLabel, 24);
}

_CODE_ZCL_ static void zcl_bypassParse(u8 *pData, bypass_t *pCmd)
{
	pCmd->numOfZones = *pData++;
	pCmd->zoneIds = pData;
	pData += pCmd->numOfZones;

	zcl_parseUTF8String(pData, &pCmd->code, 8);
}

_CODE_ZCL_ static status_t zcl_iasAce_armPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

	if(pInMsg->clusterAppCb){
		u8 armNotification = 0xff;

		zclIncomingAddrInfo_t addrInfo;
		addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
		addrInfo.profileId = pApsdeInd->indInfo.profile_id;
		addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
		addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
		addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
		addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

		arm_t arm;
		TL_SETSTRUCTCONTENT(arm, 0);

		zcl_armParse(pInMsg->pData, &arm);

		armNotification = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &arm);

		if(armNotification != 0xff){
			epInfo_t dstEp;
			TL_SETSTRUCTCONTENT(dstEp, 0);

			dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
			dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
			dstEp.dstEp = pApsdeInd->indInfo.src_ep;
			dstEp.profileId = pApsdeInd->indInfo.profile_id;

			zcl_iasAce_armRsp(endpoint, &dstEp, TRUE, pInMsg->hdr.seqNum, armNotification);

			status = ZCL_STA_CMD_HAS_RESP;
		}else{
			status = ZCL_STA_INVALID_VALUE;
		}
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasAce_bypassPrc(zclIncoming_t *pInMsg)
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

		bypass_t bypass;
		TL_SETSTRUCTCONTENT(bypass, 0);

		zcl_bypassParse(pInMsg->pData, &bypass);

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &bypass);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasAce_emergencyPrc(zclIncoming_t *pInMsg)
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

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, NULL);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasAce_firePrc(zclIncoming_t *pInMsg)
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

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, NULL);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasAce_panicPrc(zclIncoming_t *pInMsg)
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

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, NULL);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasAce_getZoneIdMapPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 endpoint = pApsdeInd->indInfo.dst_ep;

	if(pInMsg->clusterAppCb){
		u16 zoneIdMapSection[16];
		u16 section = 0;

		for(u8 i = 0; i < 16; i++){
			section = 0;

			for(u8 j = 0; j < 16; j++){
				u8 zoneId = 16 * i + j;

				if(zcl_findZoneEntry(endpoint, zoneId)){
					section |= (0x01 << j);
				}
			}

			zoneIdMapSection[i] = section;
		}

		zclIncomingAddrInfo_t addrInfo;
		addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
		addrInfo.profileId = pApsdeInd->indInfo.profile_id;
		addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
		addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
		addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
		addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, NULL);

		epInfo_t dstEp;
		TL_SETSTRUCTCONTENT(dstEp, 0);

		dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
		dstEp.dstEp = pApsdeInd->indInfo.src_ep;
		dstEp.profileId = pApsdeInd->indInfo.profile_id;

		zcl_iasAce_getZoneIdMapRsp(endpoint, &dstEp, TRUE, pInMsg->hdr.seqNum, (u8 *)zoneIdMapSection);

		status = ZCL_STA_CMD_HAS_RESP;
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasAce_getZoneInformationPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u8 endpoint = pApsdeInd->indInfo.dst_ep;

	if(pInMsg->clusterAppCb){
		zclIncomingAddrInfo_t addrInfo;
		addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
		addrInfo.profileId = pApsdeInd->indInfo.profile_id;
		addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
		addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
		addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
		addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

		u8 zoneId = pInMsg->pData[0];

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &zoneId);

		zcl_zoneTable_t *pZoneEntry = zcl_findZoneEntry(endpoint, zoneId);
		if(pZoneEntry){
			epInfo_t dstEp;
			TL_SETSTRUCTCONTENT(dstEp, 0);

			dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
			dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
			dstEp.dstEp = pApsdeInd->indInfo.src_ep;
			dstEp.profileId = pApsdeInd->indInfo.profile_id;

			getZoneInfoRsp_t getZoneInfoRsp;
			TL_SETSTRUCTCONTENT(dstEp, 0);

			getZoneInfoRsp.zoneId = pZoneEntry->zone.zoneId;
			getZoneInfoRsp.zoneType = pZoneEntry->zone.zoneType;
			memcpy(getZoneInfoRsp.ieeeAddr, pZoneEntry->zone, 8);
			//TODO: zoneLabel ?

			zcl_iasAce_getZoneInfoRsp(endpoint, &dstEp, TRUE, pInMsg->hdr.seqNum, &getZoneInfoRsp);

			status = ZCL_STA_CMD_HAS_RESP;
		}else{
			status = ZCL_STA_INVALID_VALUE;
		}
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasAce_getPanelStatusPrc(zclIncoming_t *pInMsg)
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

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, NULL);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasAce_getBypassedZoneListPrc(zclIncoming_t *pInMsg)
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

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, NULL);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasAce_getZoneStatusPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
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

		getZoneStatus_t getZoneStatus;
		getZoneStatus.startingZoneId = *pData++;
		getZoneStatus.maxNumOfZoneIds = *pData++;
		getZoneStatus.zoneStatusMaskFlag = *pData++;
		getZoneStatus.zoneStatusMask = BUILD_U16(pData[0], pData[1]);

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &getZoneStatus);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasAce_armRspPrc(zclIncoming_t *pInMsg)
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

		u8 armNotification = pInMsg->pData[0];

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &armNotification);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasAce_getZoneIdMapRspPrc(zclIncoming_t *pInMsg)
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

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, pInMsg->pData);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasAce_getZoneInfoRspPrc(zclIncoming_t *pInMsg)
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

		getZoneInfoRsp_t getZoneInfoRsp;
		TL_SETSTRUCTCONTENT(getZoneInfoRsp, 0);

		zcl_getZoneInfoRspParse(pInMsg->pData, &getZoneInfoRsp);

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &getZoneInfoRsp);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasAce_zoneStatusChangedPrc(zclIncoming_t *pInMsg)
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

		zoneStatusChanged_t zoneStatusChanged;
		TL_SETSTRUCTCONTENT(zoneStatusChanged, 0);

		zcl_zoneStatusChangedParse(pInMsg->pData, &zoneStatusChanged);

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &zoneStatusChanged);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasAce_panelStatusChangedPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
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

		panelStatusChanged_t panelStatusChanged;
		panelStatusChanged.panelStatus = *pData++;
		panelStatusChanged.secondsRemaining = *pData++;
		panelStatusChanged.audibleNotification = *pData++;
		panelStatusChanged.alarmStatus = *pData++;

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &panelStatusChanged);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasAce_getPanelStatusRspPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
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

		getPanelStatusRsp_t getPanelStatusRsp;
		getPanelStatusRsp.panelStatus = *pData++;
		getPanelStatusRsp.secondsRemaining = *pData++;
		getPanelStatusRsp.audibleNotification = *pData++;
		getPanelStatusRsp.alarmStatus = *pData++;

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &getPanelStatusRsp);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasAce_setBypassedZoneListPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
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

		setBypassedZoneList_t setBypassedZoneList;
		setBypassedZoneList.numOfZones = *pData++;
		setBypassedZoneList.zoneIds = setBypassedZoneList.numOfZones ? pData : NULL;

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &setBypassedZoneList);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasAce_bypassRspPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
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

		bypassRsp_t bypassRsp;
		bypassRsp.numOfZones = *pData++;
		bypassRsp.zoneIds = bypassRsp.numOfZones ? pData : NULL;

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &bypassRsp);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasAce_getZoneStatusRspPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
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

		getZoneStatusRsp_t getZoneStatusRsp;
		getZoneStatusRsp.zoneStatusComplete = *pData++;
		getZoneStatusRsp.numOfZones = *pData++;
		getZoneStatusRsp.zoneStatusInfo = getZoneStatusRsp.numOfZones ? pData : NULL;

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &getZoneStatusRsp);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasAce_clientCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd)
	{
		case ZCL_CMD_ARM:
			status = zcl_iasAce_armPrc(pInMsg);
			break;
		case ZCL_CMD_BYPASS:
			status = zcl_iasAce_bypassPrc(pInMsg);
			break;
		case ZCL_CMD_EMERGENCY:
			status = zcl_iasAce_emergencyPrc(pInMsg);
			break;
		case ZCL_CMD_FIRE:
			status = zcl_iasAce_firePrc(pInMsg);
			break;
		case ZCL_CMD_PANIC:
			status = zcl_iasAce_panicPrc(pInMsg);
			break;
		case ZCL_CMD_GET_ZONE_ID_MAP:
			status = zcl_iasAce_getZoneIdMapPrc(pInMsg);
			break;
		case ZCL_CMD_GET_ZONE_INFORMATION:
			status = zcl_iasAce_getZoneInformationPrc(pInMsg);
			break;
		case ZCL_CMD_GET_PANEL_STATUS:
			status = zcl_iasAce_getPanelStatusPrc(pInMsg);
			break;
		case ZCL_CMD_GET_BYPASSED_ZONE_LIST:
			status = zcl_iasAce_getBypassedZoneListPrc(pInMsg);
			break;
		case ZCL_CMD_GET_ZONE_STATUS:
			status = zcl_iasAce_getZoneStatusPrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasAce_serverCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd)
	{
		case ZCL_CMD_ARM_RESPONSE:
			status = zcl_iasAce_armRspPrc(pInMsg);
			break;
		case ZCL_CMD_GET_ZONE_ID_MAP_RESPONSE:
			status = zcl_iasAce_getZoneIdMapRspPrc(pInMsg);
			break;
		case ZCL_CMD_GET_ZONE_INFORMATION_RESPONSE:
			status = zcl_iasAce_getZoneInfoRspPrc(pInMsg);
			break;
		case ZCL_CMD_ZONE_STATUS_CHANGED:
			status = zcl_iasAce_zoneStatusChangedPrc(pInMsg);
			break;
		case ZCL_CMD_PANEL_STATUS_CHANGED:
			status = zcl_iasAce_panelStatusChangedPrc(pInMsg);
			break;
		case ZCL_CMD_GET_PANEL_STATUS_RESPONSE:
			status = zcl_iasAce_getPanelStatusRspPrc(pInMsg);
			break;
		case ZCL_CMD_SET_BYPASSED_ZONE_LIST:
			status = zcl_iasAce_setBypassedZoneListPrc(pInMsg);
			break;
		case ZCL_CMD_BYPASS_RESPONSE:
			status = zcl_iasAce_bypassRspPrc(pInMsg);
			break;
		case ZCL_CMD_GET_ZONE_STATUS_RESPONSE:
			status = zcl_iasAce_getZoneStatusRspPrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasAce_cmdHandler(zclIncoming_t *pInMsg)
{
	if(pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR){
		return zcl_iasAce_clientCmdHandler(pInMsg);
	}else{
		return zcl_iasAce_serverCmdHandler(pInMsg);
	}
}

#endif
