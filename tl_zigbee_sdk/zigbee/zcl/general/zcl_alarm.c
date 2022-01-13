/********************************************************************************************************
 * @file    zcl_alarm.c
 *
 * @brief   This is the source file for zcl_alarm
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


#ifdef ZCL_ALARMS
/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * LOCAL TYPES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */
zcl_alarmTable_t g_zcl_alarmTab[ZCL_ALARM_TABLE_NUM];


/**********************************************************************
 * LOCAL FUNCTIONS
 */
static status_t zcl_alarm_cmdHandler(zclIncoming_t *pInMsg);


_CODE_ZCL_ status_t zcl_alarm_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
    return zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_ALARMS, manuCode, attrNum, attrTbl, zcl_alarm_cmdHandler, cb);
}


_CODE_ZCL_ void zcl_alarm_clearAlarmEntry(zcl_alarmTable_t *pAlarmTab)
{
	if(pAlarmTab){
		pAlarmTab->used = 0;
		pAlarmTab->endpoint = 0xfe;
		memset((u8 *)&pAlarmTab->alarm, 0, sizeof(zcl_alarmEntry_t));
	}
}

_CODE_ZCL_ void zcl_alarm_alarmTabClear(void)
{
	for(u8 i = 0; i < ZCL_ALARM_TABLE_NUM; i++){
		zcl_alarmTable_t *pAlarmTab = &g_zcl_alarmTab[i];

		zcl_alarm_clearAlarmEntry(pAlarmTab);
	}
}

_CODE_ZCL_ zcl_alarmTable_t *zcl_alarm_findEarliestAlarmEntry(u8 endpoint)
{
	zcl_alarmTable_t *pAlarmTab = NULL;
	u32 t = 0xffffffff;

	for(u8 i = 0; i < ZCL_ALARM_TABLE_NUM; i++){
		zcl_alarmTable_t *pEntry = &g_zcl_alarmTab[i];

		if(pEntry->used && (pEntry->endpoint == endpoint)){
			if(pEntry->alarm.timeStamp < t){
				pAlarmTab = pEntry;
				t = pEntry->alarm.timeStamp;
			}
		}
	}

	return pAlarmTab;
}

_CODE_ZCL_ void zcl_alarm_removeAllAlarmEntries(u8 endpoint)
{
	for(u8 i = 0; i < ZCL_ALARM_TABLE_NUM; i++){
		zcl_alarmTable_t *pAlarmTab = &g_zcl_alarmTab[i];

		if(pAlarmTab->used && (pAlarmTab->endpoint == endpoint)){
			zcl_alarm_clearAlarmEntry(pAlarmTab);
		}
	}
}

_CODE_ZCL_ zcl_alarmTable_t *zcl_alarm_getFreeAlarmTabEntry(void)
{
	for(u8 i = 0; i < ZCL_ALARM_TABLE_NUM; i++){
		zcl_alarmTable_t *pAlarmTab = &g_zcl_alarmTab[i];

		if(!pAlarmTab->used){
			return pAlarmTab;
		}
	}

	return NULL;
}

_CODE_ZCL_ zcl_alarmTable_t *zcl_alarm_findAlarmTab(u8 endpoint, u8 alarmCode, u16 clusterId)
{
	for(u8 i = 0; i < ZCL_ALARM_TABLE_NUM; i++){
		zcl_alarmTable_t *pAlarmTab = &g_zcl_alarmTab[i];

		if( pAlarmTab->used && (pAlarmTab->endpoint == endpoint) &&
			(pAlarmTab->alarm.alarmCode == alarmCode) && (pAlarmTab->alarm.clusterId == clusterId) ){
			return pAlarmTab;
		}
	}

	return NULL;
}

_CODE_ZCL_ status_t zcl_alarm_AddAlarmTab(u8 endpoint, u8 alarmCode, u16 clusterId, u32 timeStamp)
{
	zcl_alarmTable_t *pAlarmTab = zcl_alarm_getFreeAlarmTabEntry();
	if(pAlarmTab){
		pAlarmTab->used = 1;
		pAlarmTab->endpoint = endpoint;
		pAlarmTab->alarm.alarmCode = alarmCode;
		pAlarmTab->alarm.clusterId = clusterId;
		pAlarmTab->alarm.timeStamp = timeStamp;
	}else{
		return ZCL_STA_INSUFFICIENT_SPACE;
	}

	return ZCL_STA_SUCCESS;
}


_CODE_ZCL_ u8 zcl_alarm_getAlarmRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, getAlarmRsp_t *pGetAlarmRsp)
{
	buf[8];

	u8 *pBuf = buf;

	*pBuf++ = pGetAlarmRsp->status;
	if(pGetAlarmRsp->status == ZCL_STA_SUCCESS){
		*pBuf++ = pGetAlarmRsp->alarmCode;
	    *pBuf++ = LO_UINT16(pGetAlarmRsp->clusterId);
	    *pBuf++ = HI_UINT16(pGetAlarmRsp->clusterId);
		*pBuf++ = U32_BYTE0(pGetAlarmRsp->timeStamp);
		*pBuf++ = U32_BYTE1(pGetAlarmRsp->timeStamp);
		*pBuf++ = U32_BYTE2(pGetAlarmRsp->timeStamp);
		*pBuf++ = U32_BYTE3(pGetAlarmRsp->timeStamp);
	}

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_ALARMS, ZCL_CMD_ALARM_GET_ALARM_RSP, TRUE,
					ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, pBuf - buf, buf);
}


_CODE_ZCL_ status_t zcl_alarm_alarm(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, alarm_t *pAlarm)
{
    u8 buf[3];

    buf[0] = pAlarm->alarmCode;
    buf[1] = LO_UINT16(pAlarm->clusterId);
    buf[2] = HI_UINT16(pAlarm->clusterId);

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_ALARMS, ZCL_CMD_ALARM_ALARM, TRUE,
    				ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, 3, buf);
}

_CODE_ZCL_ status_t zcl_alarm_resetAlarm(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, resetAlarm_t *pResetAlarm)
{
    u8 buf[3];

    buf[0] = pResetAlarm->alarmCode;
    buf[1] = LO_UINT16(pResetAlarm->clusterId);
    buf[2] = HI_UINT16(pResetAlarm->clusterId);

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_ALARMS, ZCL_CMD_ALARM_RESET_ALARM, TRUE,
    				ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 3, buf);
}

_CODE_ZCL_ status_t zcl_alarm_resetAllAlarm(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_ALARMS, ZCL_CMD_ALARM_RESET_ALL, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_alarm_getAlarm(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_ALARMS, ZCL_CMD_ALARM_GET_ALARM, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_alarm_resetAlarmLog(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_ALARMS, ZCL_CMD_ALARM_RESET_ALARM_LOG, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}


_CODE_ZCL_ static status_t zcl_alarm_resetAlarmPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	u8 *pData = pInMsg->pData;

	if(pInMsg->clusterAppCb){
		resetAlarm_t resetAlarm;
		resetAlarm.alarmCode = *pData++;
		resetAlarm.clusterId = BUILD_U16(pData[0], pData[1]);
		pData += 2;

		pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &resetAlarm);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_alarm_resetAllAlarmsPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	if(pInMsg->clusterAppCb){
		pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, NULL);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_alarm_getAlarmPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

	if(pInMsg->clusterAppCb){
		pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, NULL);

		getAlarmRsp_t getAlarmRsp;
		TL_SETSTRUCTCONTENT(getAlarmRsp, 0);

		zcl_alarmTable_t *pEntry = zcl_alarm_findEarliestAlarmEntry(endpoint);
		if(pEntry){
			getAlarmRsp.status = ZCL_STA_SUCCESS;
			getAlarmRsp.alarmCode = pEntry->alarm.alarmCode;
			getAlarmRsp.clusterId = pEntry->alarm.clusterId;
			getAlarmRsp.timeStamp = pEntry->alarm.timeStamp;
		}else{
			getAlarmRsp.status = ZCL_STA_NOT_FOUND;
		}

		epInfo_t dstEp;
		TL_SETSTRUCTCONTENT(dstEp, 0);

		dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
		dstEp.dstEp = pApsdeInd->indInfo.src_ep;
		dstEp.profileId = pApsdeInd->indInfo.profile_id;

		zcl_alarm_getAlarmRsp(pApsdeInd->indInfo.dst_ep, &dstEp, TRUE, pInMsg->hdr.seqNum, &getAlarmRsp);

		if(getAlarmRsp.status == ZCL_STA_SUCCESS){
			/* remove entry */
			zcl_alarm_clearAlarmEntry(pEntry);
		}

		status = ZCL_STA_CMD_HAS_RESP;
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_alarm_resetAlarmLogPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

	if(pInMsg->clusterAppCb){
		pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, NULL);

		/* remove all entries */
		zcl_alarm_removeAllAlarmEntries(pApsdeInd->indInfo.dst_ep);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_alarm_alarmPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	u8 *pData = pInMsg->pData;

	if(pInMsg->clusterAppCb){
		alarm_t alarm;
		alarm.alarmCode = *pData++;
		alarm.clusterId = BUILD_U16(pData[0], pData[1]);
		pData += 2;

		pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &alarm);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_alarm_getAlarmRspPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	u8 *pData = pInMsg->pData;

	if(pInMsg->clusterAppCb){
		getAlarmRsp_t getAlarmRsp;
		getAlarmRsp.status = *pData++;
		if(getAlarmRsp.status == ZCL_STA_SUCCESS){
			getAlarmRsp.alarmCode = *pData++;
			getAlarmRsp.clusterId = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			getAlarmRsp.timeStamp = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
			pData += 4;
		}

		pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &getAlarmRsp);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_alarm_clientCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd){
		case ZCL_CMD_ALARM_RESET_ALARM:
			status = zcl_alarm_resetAlarmPrc(pInMsg);
			break;
		case ZCL_CMD_ALARM_RESET_ALL:
			status = zcl_alarm_resetAllAlarmsPrc(pInMsg);
			break;
		case ZCL_CMD_ALARM_GET_ALARM:
			status = zcl_alarm_getAlarmPrc(pInMsg);
			break;
		case ZCL_CMD_ALARM_RESET_ALARM_LOG:
			status = zcl_alarm_resetAlarmLogPrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

    return status;
}

_CODE_ZCL_ static status_t zcl_alarm_serverCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd){
		case ZCL_CMD_ALARM_ALARM:
			status = zcl_alarm_alarmPrc(pInMsg);
			break;
		case ZCL_CMD_ALARM_GET_ALARM_RSP:
			status = zcl_alarm_getAlarmRspPrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

    return status;
}

_CODE_ZCL_ static status_t zcl_alarm_cmdHandler(zclIncoming_t *pInMsg)
{
    if(pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR){
        return zcl_alarm_clientCmdHandler(pInMsg);
    }else{
        return zcl_alarm_serverCmdHandler(pInMsg);
    }
}

#endif  /* ZCL_ALARMS */
