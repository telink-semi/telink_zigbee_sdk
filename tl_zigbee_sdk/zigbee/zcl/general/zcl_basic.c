/********************************************************************************************************
 * @file    zcl_basic.c
 *
 * @brief   This is the source file for zcl_basic
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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


_CODE_ZCL_ status_t zcl_basic_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
    return zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_BASIC, manuCode, attrNum, attrTbl, zcl_basic_cmdHandler, cb);
}

_CODE_ZCL_ status_t zcl_basic_reset(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo)
{
    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_BASIC, ZCL_CMD_BASIC_RESET_FAC_DEFAULT, TRUE,
                       ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 0, NULL);
}

_CODE_ZCL_ static status_t zcl_basic_clientCmdHandler(zclIncoming_t *pInMsg)
{
    u8 status = ZCL_STA_SUCCESS;

    switch (pInMsg->hdr.cmd) {
    case ZCL_CMD_BASIC_RESET_FAC_DEFAULT:
        if (pInMsg->clusterAppCb) {
            status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), ZCL_CMD_BASIC_RESET_FAC_DEFAULT, NULL);
        } else {
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
    if (pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR) {
        return zcl_basic_clientCmdHandler(pInMsg);
    } else {
        return ZCL_STA_UNSUP_CLUSTER_COMMAND;
    }
}

#endif  /* ZCL_BASIC */
