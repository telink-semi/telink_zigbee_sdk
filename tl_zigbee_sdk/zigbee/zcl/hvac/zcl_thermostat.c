/********************************************************************************************************
 * @file     zcl_thermostat.c
 *
 * @brief	 APIs for thermostat cluster
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

#ifdef ZCL_THERMOSTAT
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
static status_t zcl_thermostat_cmdHandler(zclIncoming_t *pInMsg);


_CODE_ZCL_ status_t zcl_thermostat_register(u8 endpoint, u8 arrtNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
	return zcl_registerCluster(endpoint, ZCL_CLUSTER_HAVC_THERMOSTAT, arrtNum, attrTbl, zcl_thermostat_cmdHandler, cb);
}


_CODE_ZCL_ status_t zcl_thermostat_setpointRaiseLowerCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_thermostat_setpointRaiseLowerCmd_t *pCmd)
{
	u8 buf[2];
	u8 *pBuf = buf;

	*pBuf++ = pCmd->mode;
	*pBuf++ = pCmd->amount;

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_HAVC_THERMOSTAT, ZCL_CMD_THERMOSTAT_SETPOINT_RAISE_LOWER, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 2, buf);
}

_CODE_ZCL_ status_t zcl_thermostat_setWeeklyScheduleCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_thermostat_setWeeklyScheduleCmd_t *pCmd)
{
	u8 status = ZCL_STA_SUCCESS;
	u8 len = 3;

	if( (pCmd->modeForSequence == HEAT_SERPOINT_FIELD_PRESENT) ||
		(pCmd->modeForSequence == COOL_SETPOINT_FIELD_PRESENT) ){
		len += pCmd->numOfTransForSequence * 4;
	}else if(pCmd->modeForSequence == BOTH_SETPOINT_FIELD_PRESENT){
		len += pCmd->numOfTransForSequence * 6;
	}else{
		return ZCL_STA_FAILURE;
	}

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = pCmd->numOfTransForSequence;
	*pBuf++ = pCmd->dayOfWeekForSequence;
	*pBuf++ = pCmd->modeForSequence;
	for(u8 i = 0; i < pCmd->numOfTransForSequence; i++){
		if(pCmd->modeForSequence == HEAT_SERPOINT_FIELD_PRESENT){
			*pBuf++ = LO_UINT16(pCmd->sequenceMode.pHeatMode[i].transTime);
			*pBuf++ = HI_UINT16(pCmd->sequenceMode.pHeatMode[i].transTime);
			*pBuf++ = LO_UINT16(pCmd->sequenceMode.pHeatMode[i].heatSetpoint);
			*pBuf++ = HI_UINT16(pCmd->sequenceMode.pHeatMode[i].heatSetpoint);
		}else if(pCmd->modeForSequence == COOL_SETPOINT_FIELD_PRESENT){
			*pBuf++ = LO_UINT16(pCmd->sequenceMode.pCoolMode[i].transTime);
			*pBuf++ = HI_UINT16(pCmd->sequenceMode.pCoolMode[i].transTime);
			*pBuf++ = LO_UINT16(pCmd->sequenceMode.pCoolMode[i].coolSetpoint);
			*pBuf++ = HI_UINT16(pCmd->sequenceMode.pCoolMode[i].coolSetpoint);
		}else if(pCmd->modeForSequence == BOTH_SETPOINT_FIELD_PRESENT){
			*pBuf++ = LO_UINT16(pCmd->sequenceMode.pBothMode[i].transTime);
			*pBuf++ = HI_UINT16(pCmd->sequenceMode.pBothMode[i].transTime);
			*pBuf++ = LO_UINT16(pCmd->sequenceMode.pBothMode[i].heatSetpoint);
			*pBuf++ = HI_UINT16(pCmd->sequenceMode.pBothMode[i].heatSetpoint);
			*pBuf++ = LO_UINT16(pCmd->sequenceMode.pBothMode[i].coolSetpoint);
			*pBuf++ = HI_UINT16(pCmd->sequenceMode.pBothMode[i].coolSetpoint);
		}else{
			status = ZCL_STA_FAILURE;
		}
	}

	if(status != ZCL_STA_FAILURE){
		status = zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_HAVC_THERMOSTAT, ZCL_CMD_THERMOSTAT_SET_WEEKLY_SCHEDULE, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, len, buf);
	}

    ev_buf_free(buf);

    return status;
}

_CODE_ZCL_ status_t zcl_thermostat_getWeeklyScheduleCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_thermostat_getWeeklyScheduleCmd_t *pCmd)
{
	u8 buf[2];
	u8 *pBuf = buf;

	*pBuf++ = pCmd->daysToReturn;
	*pBuf++ = pCmd->modeToReturn;

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_HAVC_THERMOSTAT, ZCL_CMD_THERMOSTAT_GET_WEEKLY_SCHEDULE, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 2, buf);
}

_CODE_ZCL_ status_t zcl_thermostat_clearWeeklyScheduleCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_HAVC_THERMOSTAT, ZCL_CMD_THERMOSTAT_CLEAR_WEEKLY_SCHEDULE, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_thermostat_getRelayStatusLogCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_HAVC_THERMOSTAT, ZCL_CMD_THERMOSTAT_GET_RELAY_STATUS_LOG, TRUE,
    					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_thermostat_getWeeklyScheduleRspCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_thermostat_getWeeklyScheduleRspCmd_t *pCmd)
{
	u8 status = ZCL_STA_SUCCESS;
	u8 len = 3;

	if( (pCmd->modeForSequence == HEAT_SERPOINT_FIELD_PRESENT) ||
		(pCmd->modeForSequence == COOL_SETPOINT_FIELD_PRESENT) ){
		len += pCmd->numOfTransForSequence * 4;
	}else if(pCmd->modeForSequence == BOTH_SETPOINT_FIELD_PRESENT){
		len += pCmd->numOfTransForSequence * 6;
	}else{
		return ZCL_STA_FAILURE;
	}

	u8 *buf = (u8 *)ev_buf_allocate(len);
	if(!buf){
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	u8 *pBuf = buf;

	*pBuf++ = pCmd->numOfTransForSequence;
	*pBuf++ = pCmd->dayOfWeekForSequence;
	*pBuf++ = pCmd->modeForSequence;
	for(u8 i = 0; i < pCmd->numOfTransForSequence; i++){
		if(pCmd->modeForSequence == HEAT_SERPOINT_FIELD_PRESENT){
			*pBuf++ = LO_UINT16(pCmd->sequenceMode.pHeatMode[i].transTime);
			*pBuf++ = HI_UINT16(pCmd->sequenceMode.pHeatMode[i].transTime);
			*pBuf++ = LO_UINT16(pCmd->sequenceMode.pHeatMode[i].heatSetpoint);
			*pBuf++ = HI_UINT16(pCmd->sequenceMode.pHeatMode[i].heatSetpoint);
		}else if(pCmd->modeForSequence == COOL_SETPOINT_FIELD_PRESENT){
			*pBuf++ = LO_UINT16(pCmd->sequenceMode.pCoolMode[i].transTime);
			*pBuf++ = HI_UINT16(pCmd->sequenceMode.pCoolMode[i].transTime);
			*pBuf++ = LO_UINT16(pCmd->sequenceMode.pCoolMode[i].coolSetpoint);
			*pBuf++ = HI_UINT16(pCmd->sequenceMode.pCoolMode[i].coolSetpoint);
		}else if(pCmd->modeForSequence == BOTH_SETPOINT_FIELD_PRESENT){
			*pBuf++ = LO_UINT16(pCmd->sequenceMode.pBothMode[i].transTime);
			*pBuf++ = HI_UINT16(pCmd->sequenceMode.pBothMode[i].transTime);
			*pBuf++ = LO_UINT16(pCmd->sequenceMode.pBothMode[i].heatSetpoint);
			*pBuf++ = HI_UINT16(pCmd->sequenceMode.pBothMode[i].heatSetpoint);
			*pBuf++ = LO_UINT16(pCmd->sequenceMode.pBothMode[i].coolSetpoint);
			*pBuf++ = HI_UINT16(pCmd->sequenceMode.pBothMode[i].coolSetpoint);
		}else{
			status = ZCL_STA_FAILURE;
		}
	}

	if(status != ZCL_STA_FAILURE){
		status = zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_HAVC_THERMOSTAT, ZCL_CMD_THERMOSTAT_GET_WEEKLY_SCHEDULE_RSP, TRUE,
					ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, len, buf);
	}

    ev_buf_free(buf);

    return status;
}

_CODE_ZCL_ status_t zcl_thermostat_getRelayStatusLogRspCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_thermostat_getRelayStatusLogRspCmd_t *pCmd)
{
	u8 buf[11];
	u8 *pBuf = buf;

	*pBuf++ = LO_UINT16(pCmd->timeOfDay);
	*pBuf++ = HI_UINT16(pCmd->timeOfDay);
	*pBuf++ = LO_UINT16(pCmd->relayStatus);
	*pBuf++ = HI_UINT16(pCmd->relayStatus);
	*pBuf++ = LO_UINT16(pCmd->localTemp);
	*pBuf++ = HI_UINT16(pCmd->localTemp);
	*pBuf++ = pCmd->hunidityInPercentage;
	*pBuf++ = LO_UINT16(pCmd->setpoint);
	*pBuf++ = HI_UINT16(pCmd->setpoint);
	*pBuf++ = LO_UINT16(pCmd->unreadEntries);
	*pBuf++ = HI_UINT16(pCmd->unreadEntries);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_HAVC_THERMOSTAT, ZCL_CMD_THERMOSTAT_GET_RELAY_STATUS_LOG_RSP, TRUE,
					ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, 11, buf);
}



_CODE_ZCL_ static status_t zcl_thermostat_setpointRaiseLowerPrc(zclIncoming_t *pInMsg)
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

		zcl_thermostat_setpointRaiseLowerCmd_t cmd;
		TL_SETSTRUCTCONTENT(cmd, 0);

		cmd.mode = *pData++;
		cmd.amount = *pData++;

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmd);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_thermostat_setWeeklySchedulePrc(zclIncoming_t *pInMsg)
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

		zcl_thermostat_setWeeklyScheduleCmd_t cmd;
		TL_SETSTRUCTCONTENT(cmd, 0);

		cmd.numOfTransForSequence = *pData++;
		cmd.dayOfWeekForSequence = *pData++;
		cmd.modeForSequence = *pData++;
		if(cmd.numOfTransForSequence){
			if(cmd.modeForSequence == HEAT_SERPOINT_FIELD_PRESENT){
				cmd.sequenceMode.pHeatMode = (heatMode_t *)pData;
			}else if(cmd.modeForSequence == COOL_SETPOINT_FIELD_PRESENT){
				cmd.sequenceMode.pCoolMode = (coolMode_t *)pData;
			}else if(cmd.modeForSequence == BOTH_SETPOINT_FIELD_PRESENT){
				cmd.sequenceMode.pBothMode = (bothMode_t *)pData;
			}
		}

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmd);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_thermostat_getWeeklySchedulePrc(zclIncoming_t *pInMsg)
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

		zcl_thermostat_getWeeklyScheduleCmd_t cmd;
		TL_SETSTRUCTCONTENT(cmd, 0);

		cmd.daysToReturn = *pData++;
		cmd.modeToReturn = *pData++;

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmd);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_thermostat_clearWeeklySchedulePrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

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

	return status;
}

_CODE_ZCL_ static status_t zcl_thermostat_getRelayStatusLogPrc(zclIncoming_t *pInMsg)
{
	status_t status = ZCL_STA_SUCCESS;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

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

	return status;
}

_CODE_ZCL_ static status_t zcl_thermostat_getWeeklyScheduleRspPrc(zclIncoming_t *pInMsg)
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

		zcl_thermostat_getWeeklyScheduleRspCmd_t cmd;
		TL_SETSTRUCTCONTENT(cmd, 0);

		cmd.numOfTransForSequence = *pData++;
		cmd.dayOfWeekForSequence = *pData++;
		cmd.modeForSequence = *pData++;
		if(cmd.numOfTransForSequence){
			if(cmd.modeForSequence == HEAT_SERPOINT_FIELD_PRESENT){
				cmd.sequenceMode.pHeatMode = (heatMode_t *)pData;
			}else if(cmd.modeForSequence == COOL_SETPOINT_FIELD_PRESENT){
				cmd.sequenceMode.pCoolMode = (coolMode_t *)pData;
			}else if(cmd.modeForSequence == BOTH_SETPOINT_FIELD_PRESENT){
				cmd.sequenceMode.pBothMode = (bothMode_t *)pData;
			}
		}

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmd);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_thermostat_getRelayStatusLogRspPrc(zclIncoming_t *pInMsg)
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

		zcl_thermostat_getRelayStatusLogRspCmd_t cmd;
		TL_SETSTRUCTCONTENT(cmd, 0);

		cmd.timeOfDay = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		cmd.relayStatus = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		cmd.localTemp = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		cmd.hunidityInPercentage = *pData++;
		cmd.setpoint = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		cmd.unreadEntries = BUILD_U16(pData[0], pData[1]);
		pData += 2;

		status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmd);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_thermostat_clientCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd)
	{
		case ZCL_CMD_THERMOSTAT_SETPOINT_RAISE_LOWER:
			status = zcl_thermostat_setpointRaiseLowerPrc(pInMsg);
			break;
		case ZCL_CMD_THERMOSTAT_SET_WEEKLY_SCHEDULE:
			status = zcl_thermostat_setWeeklySchedulePrc(pInMsg);
			break;
		case ZCL_CMD_THERMOSTAT_GET_WEEKLY_SCHEDULE:
			status = zcl_thermostat_getWeeklySchedulePrc(pInMsg);
			break;
		case ZCL_CMD_THERMOSTAT_CLEAR_WEEKLY_SCHEDULE:
			status = zcl_thermostat_clearWeeklySchedulePrc(pInMsg);
			break;
		case ZCL_CMD_THERMOSTAT_GET_RELAY_STATUS_LOG:
			status = zcl_thermostat_getRelayStatusLogPrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_thermostat_serverCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd)
	{
		case ZCL_CMD_THERMOSTAT_GET_WEEKLY_SCHEDULE_RSP:
			status = zcl_thermostat_getWeeklyScheduleRspPrc(pInMsg);
			break;
		case ZCL_CMD_THERMOSTAT_GET_RELAY_STATUS_LOG_RSP:
			status = zcl_thermostat_getRelayStatusLogRspPrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_thermostat_cmdHandler(zclIncoming_t *pInMsg)
{
	if(pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR){
		return zcl_thermostat_clientCmdHandler(pInMsg);
	}else{
		return zcl_thermostat_serverCmdHandler(pInMsg);
	}
}

#endif	/* ZCL_THERMOSTAT */
