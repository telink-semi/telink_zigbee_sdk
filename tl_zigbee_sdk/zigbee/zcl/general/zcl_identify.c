/********************************************************************************************************
 * @file    zcl_identify.c
 *
 * @brief   This is the source file for zcl_identify
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


#ifdef ZCL_IDENTIFY
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
static status_t zcl_identify_cmdHandler(zclIncoming_t *pInMsg);


_CODE_ZCL_ status_t zcl_identify_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
    return zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_IDENTIFY, manuCode, attrNum, attrTbl, zcl_identify_cmdHandler, cb);
}


_CODE_ZCL_ void zcl_identify_commissioningIdentify(zclIncoming_t *pInMsg, u16 identifyTime)
{
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

	clusterInfo_t *pCluster = zcl_findCluster(pApsdeInd->indInfo.dst_ep, ZCL_CLUSTER_GEN_IDENTIFY);
	if(pCluster && pCluster->clusterAppCb){
		zcl_identifyCmd_t identify;
		identify.identifyTime = identifyTime;

		pCluster->clusterAppCb(&(pInMsg->addrInfo), ZCL_CMD_IDENTIFY, &identify);
	}
}


_CODE_ZCL_ status_t zcl_identify_identify(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u16 identifyTime, u32 manuCode)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_IDENTIFY, ZCL_CMD_IDENTIFY, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, manuCode, seqNo, 2, (u8*)&identifyTime);
}

_CODE_ZCL_ status_t zcl_identify_identifyQuery(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_IDENTIFY, ZCL_CMD_IDENTIFY_QUERY, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_identify_triggerEffect(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 effectId, u8 effectVariant)
{
	u8 buf[2];

	buf[0] = effectId;
	buf[1] = effectVariant;

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_IDENTIFY, ZCL_CMD_TRIGGER_EFFECT, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 2, buf);
}

_CODE_ZCL_ status_t zcl_identify_identifyQueryRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u16 timeout)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_IDENTIFY, ZCL_CMD_IDENTIFY_QUERY_RSP, TRUE,
					ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, 2, (u8*)&timeout);
}

_CODE_ZCL_ static status_t zcl_identifyQueryPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;
	u16 clusterId = pApsdeInd->indInfo.cluster_id;
	u8 endpoint = pApsdeInd->indInfo.dst_ep;

	u16 identifyTime = 0;
	u16 len = 0;

	if(zcl_getAttrVal(endpoint, clusterId, ZCL_ATTRID_IDENTIFY_TIME, &len, (u8 *)&identifyTime) == ZCL_STA_SUCCESS){
		if(identifyTime){
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

		    zcl_identify_identifyQueryRsp(endpoint, &dstEp, TRUE, pInMsg->hdr.seqNum, identifyTime);

			status = ZCL_STA_CMD_HAS_RESP;
		}
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_identify_clientCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	zcl_identify_cmdPayload_t cmdPayload;
	memset((u8 *)&cmdPayload, 0, sizeof(zcl_identify_cmdPayload_t));

	switch(pInMsg->hdr.cmd)
	{
		case ZCL_CMD_IDENTIFY:
			//status = zcl_identifyPrc(pInMsg);
			cmdPayload.identify.identifyTime = BUILD_U16(pInMsg->pData[0], pInMsg->pData[1]);
			break;
		case ZCL_CMD_IDENTIFY_QUERY:
			status = zcl_identifyQueryPrc(pInMsg);
			break;
		case ZCL_CMD_TRIGGER_EFFECT:
			//status = zcl_identifyTriggerEffectPrc(pInMsg);
			cmdPayload.triggerEffect.effectId = pInMsg->pData[0];
			cmdPayload.triggerEffect.effectVariant = pInMsg->pData[1];
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	if(status == ZCL_STA_SUCCESS){
		if(pInMsg->clusterAppCb){
			status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &cmdPayload);
		}else{
			status = ZCL_STA_FAILURE;
		}
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_identify_serverCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	zcl_identify_cmdPayload_t cmdPayload;
	memset((u8 *)&cmdPayload, 0, sizeof(zcl_identify_cmdPayload_t));

	switch(pInMsg->hdr.cmd)
	{
		case ZCL_CMD_IDENTIFY_QUERY_RSP:
			//status = zcl_identifyQueryRspPrc(pInMsg);
			cmdPayload.identifyRsp.timeout = BUILD_U16(pInMsg->pData[0], pInMsg->pData[1]);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	if(status == ZCL_STA_SUCCESS){
		if(pInMsg->clusterAppCb){
			status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &cmdPayload);
		}else{
			status = ZCL_STA_FAILURE;
		}
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_identify_cmdHandler(zclIncoming_t *pInMsg)
{
	if(pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR){
		return zcl_identify_clientCmdHandler(pInMsg);
	}else{
		return zcl_identify_serverCmdHandler(pInMsg);
	}
}

#endif  /* ZCL_IDENTIFY */

