/********************************************************************************************************
 * @file	zcl_thermostat.c
 *
 * @brief	This is the source file for zcl_thermostat
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


_CODE_ZCL_ status_t zcl_thermostat_register(u8 endpoint, u16 manuCode, u8 arrtNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
	return zcl_registerCluster(endpoint, ZCL_CLUSTER_HAVC_THERMOSTAT, manuCode, arrtNum, attrTbl, zcl_thermostat_cmdHandler, cb);
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
