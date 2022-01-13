/********************************************************************************************************
 * @file    zcl_pollCtrl.c
 *
 * @brief   This is the source file for zcl_pollCtrl
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


#ifdef ZCL_POLL_CTRL
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
static status_t zcl_pollCtrl_cmdHandler(zclIncoming_t *pInMsg);


_CODE_ZCL_ status_t zcl_pollCtrl_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
	return zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_POLL_CONTROL, manuCode, attrNum, attrTbl, zcl_pollCtrl_cmdHandler, cb);
}


_CODE_ZCL_ status_t zcl_pollCtrl_chkIn(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_POLL_CONTROL, ZCL_CMD_CHK_IN, TRUE,
					ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_pollCtrl_chkInRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_chkInRsp_t *pCheckInRsp)
{
	u8 buf[3];

	buf[0] = pCheckInRsp->startFastPolling;
	buf[1] = LO_UINT16(pCheckInRsp->fastPollTimeout);
	buf[2] = HI_UINT16(pCheckInRsp->fastPollTimeout);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_POLL_CONTROL, ZCL_CMD_CHK_IN_RSP, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 3, buf);
}

_CODE_ZCL_ status_t zcl_pollCtrl_fastPollStop(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_POLL_CONTROL, ZCL_CMD_FAST_POLL_STOP, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ status_t zcl_pollCtrl_setLongPollInterval(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_setLongPollInterval_t *pSetLongPollInterval)
{
	u8 buf[4];

	buf[0] = U32_BYTE0(pSetLongPollInterval->newLongPollInterval);
	buf[1] = U32_BYTE1(pSetLongPollInterval->newLongPollInterval);
	buf[2] = U32_BYTE2(pSetLongPollInterval->newLongPollInterval);
	buf[3] = U32_BYTE3(pSetLongPollInterval->newLongPollInterval);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_POLL_CONTROL, ZCL_CMD_SET_LONG_POLL_INTERVAL, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 4, buf);
}

_CODE_ZCL_ status_t zcl_pollCtrl_setShortPollInterval(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_setShortPollInterval_t *pSetShortPollInterval)
{
	u8 buf[2];

	buf[0] = LO_UINT16(pSetShortPollInterval->newShortPollInterval);
	buf[1] = HI_UINT16(pSetShortPollInterval->newShortPollInterval);

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_POLL_CONTROL, ZCL_CMD_SET_LONG_POLL_INTERVAL, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 2, buf);
}

_CODE_ZCL_ static status_t zcl_pollCtrl_chkInRspPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	if(pInMsg->clusterAppCb){
		zcl_chkInRsp_t payload;
		payload.startFastPolling = pInMsg->pData[0];
		payload.fastPollTimeout = BUILD_U16(pInMsg->pData[1], pInMsg->pData[2]);

		status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &payload);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_pollCtrl_fastPollStopPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	if(pInMsg->clusterAppCb){
		status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, NULL);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_pollCtrl_setLongPollIntervalPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	if(pInMsg->clusterAppCb){
		zcl_setLongPollInterval_t payload;
		payload.newLongPollInterval = BUILD_U32(pInMsg->pData[0], pInMsg->pData[1], pInMsg->pData[2], pInMsg->pData[3]);

		status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &payload);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_pollCtrl_setShortPollIntervalPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	if(pInMsg->clusterAppCb){
		zcl_setShortPollInterval_t payload;
		payload.newShortPollInterval = BUILD_U16(pInMsg->pData[0], pInMsg->pData[1]);

		status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &payload);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_pollCtrl_chkInPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	if(pInMsg->clusterAppCb){
		status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, NULL);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_pollCtrl_clientCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd)
	{
		case ZCL_CMD_CHK_IN_RSP:
			status = zcl_pollCtrl_chkInRspPrc(pInMsg);
			break;
		case ZCL_CMD_FAST_POLL_STOP:
			status = zcl_pollCtrl_fastPollStopPrc(pInMsg);
			break;
		case ZCL_CMD_SET_LONG_POLL_INTERVAL:
			status = zcl_pollCtrl_setLongPollIntervalPrc(pInMsg);
			break;
		case ZCL_CMD_SET_SHORT_POLL_INTERVAL:
			status = zcl_pollCtrl_setShortPollIntervalPrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_pollCtrl_serverCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd)
	{
		case ZCL_CMD_CHK_IN:
			status = zcl_pollCtrl_chkInPrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_pollCtrl_cmdHandler(zclIncoming_t *pInMsg)
{
	if(pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR){
		return zcl_pollCtrl_clientCmdHandler(pInMsg);
	}else{
		return zcl_pollCtrl_serverCmdHandler(pInMsg);
	}
}

#endif    /* ZCL_POLL_CTRL */
