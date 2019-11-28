/********************************************************************************************************
 * @file     zcl_basic.c
 *
 * @brief	 APIs for basic cluster
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


#ifdef ZCL_BASIC
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
static status_t zcl_basic_cmdHandler(zclIncoming_t *pInMsg);


_CODE_ZCL_ status_t zcl_basic_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
	return zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_BASIC, attrNum, attrTbl, zcl_basic_cmdHandler, cb);
}

_CODE_ZCL_ status_t zcl_basic_reset(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
	return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_BASIC, ZCL_CMD_BASIC_RESET_FAC_DEFAULT, TRUE,
						ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ static status_t zcl_basic_clientCmdHandler(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t*)pInMsg->msg;

	switch(pInMsg->hdr.cmd)
	{
		case ZCL_CMD_BASIC_RESET_FAC_DEFAULT:
			if(pInMsg->clusterAppCb){
				zclIncomingAddrInfo_t addrInfo;
				addrInfo.dirCluster = pInMsg->hdr.frmCtrl.bf.dir;
				addrInfo.profileId = pApsdeInd->indInfo.profile_id;
				addrInfo.srcAddr = pApsdeInd->indInfo.src_short_addr;
				addrInfo.dstAddr = pApsdeInd->indInfo.dst_addr;
				addrInfo.srcEp = pApsdeInd->indInfo.src_ep;
				addrInfo.dstEp = pApsdeInd->indInfo.dst_ep;

				status = pInMsg->clusterAppCb(&addrInfo, ZCL_CMD_BASIC_RESET_FAC_DEFAULT, NULL);
			}else{
				status = ZCL_STA_FAILURE;
			}
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_basic_cmdHandler(zclIncoming_t *pInMsg)
{
	if(pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR){
		return zcl_basic_clientCmdHandler(pInMsg);
	}else{
		return ZCL_STA_UNSUP_CLUSTER_COMMAND;
	}
}

#endif  /* ZCL_BASIC */







