/********************************************************************************************************
 * @file	zcl_identify.c
 *
 * @brief	This is the source file for zcl_identify
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


_CODE_ZCL_ void zcl_identify_commissioningIdentify(apsdeDataInd_t *pApsdeInd, u16 identifyTime)
{
	clusterInfo_t *pCluster = zcl_findCluster(pApsdeInd->indInfo.dst_ep, ZCL_CLUSTER_GEN_IDENTIFY);
	if(pCluster && pCluster->clusterAppCb){
		zclIncomingAddrInfo_t addrInfo;
		addrInfo.dirCluster = ZCL_FRAME_CLIENT_SERVER_DIR;
		addrInfo.profileId = pApsdeInd->indInfo.profile_id;
		addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
		addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
		addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
		addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

		zcl_identifyCmd_t identify;
		identify.identifyTime = identifyTime;

		pCluster->clusterAppCb(&addrInfo, ZCL_CMD_IDENTIFY, &identify);
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
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

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
			zclIncomingAddrInfo_t addrInfo;
			addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
			addrInfo.profileId = pApsdeInd->indInfo.profile_id;
			addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
			addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
			addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
			addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

			status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmdPayload);
		}else{
			status = ZCL_STA_FAILURE;
		}
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_identify_serverCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

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
			zclIncomingAddrInfo_t addrInfo;
			addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
			addrInfo.profileId = pApsdeInd->indInfo.profile_id;
			addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
			addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
			addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
			addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

			status = pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &cmdPayload);
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

