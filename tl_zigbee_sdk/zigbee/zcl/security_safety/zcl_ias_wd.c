/********************************************************************************************************
 * @file     zcl_ias_wd.c
 *
 * @brief	 APIs for Warning Device equipment of the IAS system.
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


#ifdef ZCL_IAS_WD
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
static status_t zcl_iasWd_cmdHandler(zclIncoming_t *pInMsg);

_CODE_ZCL_ status_t zcl_iasWd_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{

	return zcl_registerCluster(endpoint, ZCL_CLUSTER_SS_IAS_WD, attrNum, attrTbl, zcl_iasWD_cmdHandler, cb);
}

_CODE_ZCL_ status_t zcl_iasWd_startWarning(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, startWarning_t *pStartWarning)
{
    u8 buf[5];
    
	buf[0] = pStartWarning->warning.byte;
	buf[1] = LO_UINT16(pStartWarning->warningDuration);
    buf[2] = HI_UINT16(pStartWarning->warningDuration);
    buf[3] = pStartWarning->strobeDutyCycle;
    buf[4] = pStartWarning->strobeLevel;

	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SS_IAS_WD, ZCL_CMD_START_WARNING, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 5, buf);
}


_CODE_ZCL_ status_t zcl_iasWd_squawk(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, squawk_t *pSquawk)
{
    u8 buf[1];

    buf[0] = pSquawk->byte;
	
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_SS_IAS_WD,ZCL_CMD_SQUAWK, TRUE,
					ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 1, buf);
}


_CODE_ZCL_ static status_t zcl_startWarningPrc(zclIncoming_t *pInMsg)
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

		startWarning_t startWarning;
		startWarning.warning.byte = *pData++;
		startWarning.warningDuration = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		startWarning.strobeDutyCycle = *pData++;
		startWarning.strobeLevel = *pData++;

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &startWarning);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_squawkPrc(zclIncoming_t *pInMsg)
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

		squawk_t squawk;
		squawk.byte = pInMsg->pData[0];

		pInMsg->clusterAppCb(&addrInfo, pInMsg->hdr.cmd, &squawk);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasWd_clientCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	switch(pInMsg->hdr.cmd)
	{
		case ZCL_CMD_START_WARNING:
			status = zcl_startWarningPrc(pInMsg);
			break;
		case ZCL_CMD_SQUAWK:
			status = zcl_squawkPrc(pInMsg);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_iasWd_cmdHandler(zclIncoming_t *pInMsg)
{
	if(pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR){
		return zcl_iasWd_clientCmdHandler(pInMsg);
	}else{
		return ZCL_STA_UNSUP_GENERAL_COMMAND;
	}
}

#endif  /* ZCL_IAS_WD */
