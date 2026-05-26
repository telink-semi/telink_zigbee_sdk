/********************************************************************************************************
 * @file    zcl_zbd.c
 *
 * @brief   This is the source file for zcl_basic
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


#ifdef ZCL_ZBD_CONFIG
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
static status_t zcl_zbd_cmdHandler(zclIncoming_t *pInMsg);

_CODE_ZCL_ status_t zcl_zbd_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
    return zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_ZBD_CONFIG, manuCode, attrNum, attrTbl, zcl_zbd_cmdHandler, cb);
}

_CODE_ZCL_ status_t zcl_zbd_setIfState(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 state)
{
    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_ZBD_CONFIG, ZCL_CMD_ZBD_SET_IF_STATE, TRUE,
                       ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 1, (u8 *)&state);
}

_CODE_ZCL_ status_t zcl_zbd_setAnjTimeout(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u24 timeSet)
{
    u8 anjTime[3] = {0};

    anjTime[0] = U32_BYTE0(timeSet);
    anjTime[1] = U32_BYTE1(timeSet);
    anjTime[2] = U32_BYTE2(timeSet);

    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_ZBD_CONFIG, ZCL_CMD_ZBD_SET_AJOIN_TIMEOUT, TRUE,
                       ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, 3, anjTime);
}

_CODE_ZCL_ u8 zcl_zbd_setStateRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, getIntfStateRsp_t states)
{
    if (!ss_securityModeIsDistributed()) {
        pDstEpInfo->txOptions |= APS_TX_OPT_SECURITY_ENABLED;
    }
    return zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_ZBD_CONFIG, ZCL_CMD_ZBD_SET_IF_STATE_RSP, TRUE,
                       ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, sizeof(getIntfStateRsp_t), (u8 *)&states);
}

_CODE_ZCL_ static status_t zcl_zbd_clientCmdHandler(zclIncoming_t *pInMsg)
{
    u8 status = ZCL_STA_SUCCESS;
    u8 *pData = pInMsg->pData;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t *)pInMsg->msg;

    /* On receipt of the Configure Zigbee Direct Interface command via the groupcast or broadcast
     * transmission service, the client SHALL ignore the command and no processing SHALL be done.
     */
    if ((pInMsg->msg->indInfo.dst_addr_mode == APS_SHORT_GROUPADDR_NOEP) ||
       (ZB_NWK_IS_ADDRESS_BROADCAST(pInMsg->addrInfo.dstAddr))) {
        return ZCL_STA_FAILURE;
    }

    if ((!ss_securityModeIsDistributed()) &&
        ((!(pInMsg->msg->indInfo.security_status & SECURITY_IN_APSLAYER)) ||
        (pApsdeInd->indInfo.src_short_addr != 0x0000))) {
        return ZCL_STA_NOT_AUTHORIZED;
    }

    zcl_zbd_cmdPayload_t cmdPayload;
    memset((u8 *)&cmdPayload, 0, sizeof(zcl_zbd_cmdPayload_t));

    switch (pInMsg->hdr.cmd) {
        case ZCL_CMD_ZBD_SET_IF_STATE:
            cmdPayload.interface.state = pData[0];

            if ((cmdPayload.interface.state < ZCL_ATTRID_ZBD_INSTATE_MIN) ||
                (cmdPayload.interface.state > ZCL_ATTRID_ZBD_INSTATE_MAX)) {
                return ZCL_STA_INVALID_VALUE;
            }

            if (pInMsg->clusterAppCb) {
                status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), ZCL_CMD_ZBD_SET_IF_STATE, &cmdPayload);
            }

            getIntfStateRsp_t rspData;
            rspData.status = status;
            rspData.currState = cmdPayload.interface.state;

            epInfo_t dstEp;
            TL_SETSTRUCTCONTENT(dstEp, 0);

            dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
            dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;
            dstEp.dstEp = pApsdeInd->indInfo.src_ep;
            dstEp.profileId = pApsdeInd->indInfo.profile_id;

            zcl_zbd_setStateRsp(pApsdeInd->indInfo.dst_ep, &dstEp, TRUE, pInMsg->hdr.seqNum, rspData);
            break;
        case ZCL_CMD_ZBD_SET_AJOIN_TIMEOUT:
//          if((cmdPayload.anJoinTimeout.time < ZCL_ATTRID_ZBD_AJOIN_MIN)||(cmdPayload.anJoinTimeout.time > ZCL_ATTRID_ZBD_AJOIN_MAX)){
//              return ZCL_STA_INVALID_VALUE;
//          }

            cmdPayload.anJoinTimeout.time = BUILD_U24(pInMsg->pData[0], pInMsg->pData[1], pInMsg->pData[2]);
            if (pInMsg->clusterAppCb) {
                status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), ZCL_CMD_ZBD_SET_AJOIN_TIMEOUT, &cmdPayload);
            }
            break;
        default:
            status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
            break;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_zbd_serverCmdHandler(zclIncoming_t *pInMsg)
{
    u8 status = ZCL_STA_SUCCESS;

    switch (pInMsg->hdr.cmd) {
        case ZCL_CMD_ZBD_SET_IF_STATE_RSP:
            {
                getIntfStateRsp_t cmdPayload;
                memset((u8 *)&cmdPayload, 0, sizeof(getIntfStateRsp_t));

                cmdPayload.status = pInMsg->pData[0];
                cmdPayload.currState = pInMsg->pData[1];
                if (pInMsg->clusterAppCb) {
                    status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), ZCL_CMD_ZBD_SET_IF_STATE_RSP, &cmdPayload);
                }
            }
            break;
        default:
            status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
            break;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_zbd_cmdHandler(zclIncoming_t *pInMsg)
{
    if (pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR) {
        return zcl_zbd_clientCmdHandler(pInMsg);
    } else {
        return zcl_zbd_serverCmdHandler(pInMsg);
    }
}

#endif  /* ZCL_ZBD */
