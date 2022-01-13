/********************************************************************************************************
 * @file    zcl_ias_wd.c
 *
 * @brief   This is the source file for zcl_ias_wd
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

_CODE_ZCL_ status_t zcl_iasWd_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{

	return zcl_registerCluster(endpoint, ZCL_CLUSTER_SS_IAS_WD, manuCode, attrNum, attrTbl, zcl_iasWD_cmdHandler, cb);
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
	u8 *pData = pInMsg->pData;

	if(pInMsg->clusterAppCb){
		startWarning_t startWarning;
		startWarning.warning.byte = *pData++;
		startWarning.warningDuration = BUILD_U16(pData[0], pData[1]);
		pData += 2;
		startWarning.strobeDutyCycle = *pData++;
		startWarning.strobeLevel = *pData++;

		pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &startWarning);
	}else{
		status = ZCL_STA_FAILURE;
	}

	return status;
}

_CODE_ZCL_ static status_t zcl_squawkPrc(zclIncoming_t *pInMsg)
{
	u8 status = ZCL_STA_SUCCESS;

	if(pInMsg->clusterAppCb){
		squawk_t squawk;
		squawk.byte = pInMsg->pData[0];

		pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &squawk);
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
