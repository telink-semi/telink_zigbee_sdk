/********************************************************************************************************
 * @file    zcl_greenPower.c
 *
 * @brief   This is the source file for zcl_greenPower
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

#ifdef ZCL_GREEN_POWER
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
static status_t zcl_gp_cmdHandler(zclIncoming_t *pInMsg);


_CODE_ZCL_ status_t zcl_gp_register(u8 endpoint, u16 manuCode, u8 arrtNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
    return zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_GREEN_POWER, manuCode, arrtNum, attrTbl, zcl_gp_cmdHandler, cb);
}

_CODE_ZCL_ status_t zcl_gp_notificationCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_notificationCmd_t *pCmd)
{
    u8 len = 11;//options + gpdSecCnt + cmdID + payloadLen + gppShortAddr + link

    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        len += 4;//srcID
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        len += 9;//ieeeAddr + endpoint
    }

    len += pCmd->payloadLen;//cmd length

    u8 *buf = (u8 *)ev_buf_allocate(len);
    if (!buf) {
        return ZCL_STA_INSUFFICIENT_SPACE;
    }

    u8 *pBuf = buf;

    //options
    *pBuf++ = LO_UINT16(pCmd->options.opts);
    *pBuf++ = HI_UINT16(pCmd->options.opts);
    //GPD ID and Endpoint
    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        *pBuf++ = U32_BYTE0(pCmd->gpdId.srcId);
        *pBuf++ = U32_BYTE1(pCmd->gpdId.srcId);
        *pBuf++ = U32_BYTE2(pCmd->gpdId.srcId);
        *pBuf++ = U32_BYTE3(pCmd->gpdId.srcId);
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(pBuf, pCmd->gpdId.gpdIeeeAddr);
        pBuf += EXT_ADDR_LEN;
        *pBuf++ = pCmd->endpoint;
    }
    //GPD security frame counter
    *pBuf++ = U32_BYTE0(pCmd->gpdSecFrameCnt);
    *pBuf++ = U32_BYTE1(pCmd->gpdSecFrameCnt);
    *pBuf++ = U32_BYTE2(pCmd->gpdSecFrameCnt);
    *pBuf++ = U32_BYTE3(pCmd->gpdSecFrameCnt);
    //GPD CommandID
    *pBuf++ = pCmd->gpdCmdID;
    *pBuf++ = pCmd->payloadLen;
    if (pCmd->payloadLen) {
        memcpy(pBuf, pCmd->pGpdCmdPayload, pCmd->payloadLen);
        pBuf += pCmd->payloadLen;
    }
    if (pCmd->options.bits.proxyInfoPresent) {
        //GPP shortAddr
        *pBuf++ = LO_UINT16(pCmd->gppShortAddr);
        *pBuf++ = HI_UINT16(pCmd->gppShortAddr);
        //GPP-GPD link
        *pBuf++ = pCmd->gppGpdLink.link;
    }

    zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GREEN_POWER, ZCL_CMD_GP_NOTIFICATION, TRUE,
                ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, len, buf);

    ev_buf_free(buf);

    return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ status_t zcl_gp_commissioningNotificationCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_commissioningNotificationCmd_t *pCmd)
{
    u8 len = 11;//options + gpdSecCnt + cmdID + payloadLen + gppShortAddr + link

    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        len += 4;//srcID
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        len += 9;//ieeeAddr + endpoint
    }

    if (pCmd->options.bits.secPrcFailed) {
        len += 4;//MIC
    }

    len += pCmd->payloadLen;//cmd length

    u8 *buf = (u8 *)ev_buf_allocate(len);
    if (!buf) {
        return ZCL_STA_INSUFFICIENT_SPACE;
    }

    u8 *pBuf = buf;

    //options
    *pBuf++ = LO_UINT16(pCmd->options.opts);
    *pBuf++ = HI_UINT16(pCmd->options.opts);
    //GPD ID and Endpoint
    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        *pBuf++ = U32_BYTE0(pCmd->gpdId.srcId);
        *pBuf++ = U32_BYTE1(pCmd->gpdId.srcId);
        *pBuf++ = U32_BYTE2(pCmd->gpdId.srcId);
        *pBuf++ = U32_BYTE3(pCmd->gpdId.srcId);
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(pBuf, pCmd->gpdId.gpdIeeeAddr);
        pBuf += EXT_ADDR_LEN;
        *pBuf++ = pCmd->endpoint;
    }
    //GPD security frame counter
    *pBuf++ = U32_BYTE0(pCmd->gpdSecFrameCnt);
    *pBuf++ = U32_BYTE1(pCmd->gpdSecFrameCnt);
    *pBuf++ = U32_BYTE2(pCmd->gpdSecFrameCnt);
    *pBuf++ = U32_BYTE3(pCmd->gpdSecFrameCnt);
    //GPD CommandID
    *pBuf++ = pCmd->gpdCmdID;
    *pBuf++ = pCmd->payloadLen;
    if (pCmd->payloadLen) {
        memcpy(pBuf, pCmd->pGpdCmdPayload, pCmd->payloadLen);
        pBuf += pCmd->payloadLen;
    }
    //GPP shortAddr
    *pBuf++ = LO_UINT16(pCmd->gppShortAddr);
    *pBuf++ = HI_UINT16(pCmd->gppShortAddr);
    //GPP link
    *pBuf++ = pCmd->gppGpdLink.link;
    //MIC
    if (pCmd->options.bits.secPrcFailed) {
        *pBuf++ = U32_BYTE0(pCmd->mic);
        *pBuf++ = U32_BYTE1(pCmd->mic);
        *pBuf++ = U32_BYTE2(pCmd->mic);
        *pBuf++ = U32_BYTE3(pCmd->mic);
    }

    zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GREEN_POWER, ZCL_CMD_GP_COMMISSIONING_NOTIFICATION, TRUE,
                ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, len, buf);

    ev_buf_free(buf);

    return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ status_t zcl_gp_sinkTabReqCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_sinkTabReqCmd_t *pCmd)
{
    u8 len = 1;//options

    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        len += 4;//srcID
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        len += 9;//ieeeAddr + endpoint
    }
    if (pCmd->options.bits.reqType == REQUEST_TABLE_ENTRIES_BY_INDEX) {
        len++;//index
    }

    u8 *buf = (u8 *)ev_buf_allocate(len);
    if (!buf) {
        return ZCL_STA_INSUFFICIENT_SPACE;
    }

    u8 *pBuf = buf;

    //options
    *pBuf++ = pCmd->options.opts;
    //GPD ID and Endpoint
    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        *pBuf++ = U32_BYTE0(pCmd->gpdId.srcId);
        *pBuf++ = U32_BYTE1(pCmd->gpdId.srcId);
        *pBuf++ = U32_BYTE2(pCmd->gpdId.srcId);
        *pBuf++ = U32_BYTE3(pCmd->gpdId.srcId);
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(pBuf, pCmd->gpdId.gpdIeeeAddr);
        pBuf += EXT_ADDR_LEN;
        *pBuf++ = pCmd->endpoint;
    }
    //Index
    if (pCmd->options.bits.reqType == REQUEST_TABLE_ENTRIES_BY_INDEX) {
        *pBuf++ = pCmd->index;
    }

    zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GREEN_POWER, ZCL_CMD_GP_SINK_TABLE_REQUEST, TRUE,
                ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, len, buf);

    ev_buf_free(buf);

    return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ status_t zcl_gp_proxyTableRspCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_proxyTabRspCmd_t *pCmd)
{
    u8 len = 4;//status + total number + start index + entries count

    len += pCmd->entriesLen;

    u8 *buf = (u8 *)ev_buf_allocate(len);
    if (!buf) {
        return ZCL_STA_INSUFFICIENT_SPACE;
    }

    u8 *pBuf = buf;

    *pBuf++ = pCmd->status;
    *pBuf++ = pCmd->totalTabEntries;
    *pBuf++ = pCmd->startIdx;
    *pBuf++ = pCmd->entriesCnt;
    if (pCmd->entriesLen) {
        memcpy(pBuf, pCmd->proxyTabEntry, pCmd->entriesLen);
    }

    zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GREEN_POWER, ZCL_CMD_GP_PROXY_TABLE_RESPONSE, TRUE,
                ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, len, buf);

    ev_buf_free(buf);

    return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ status_t zcl_gp_pairingCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_pairingCmd_t *pCmd)
{
    u8 len = 3;//options

    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        len += 4;//srcID
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        len += 9;//ieeeAddr + endpoint
    }

    if (!pCmd->options.bits.removeGPD) {
        if ((pCmd->options.bits.commMode == GPS_COMM_MODE_FULL_UNICAST) ||
            (pCmd->options.bits.commMode == GPS_COMM_MODE_LIGHTWEIGHT_UNICAST)) {
            len += 10;//sink ieee address + sink nwk address
        } else if ((pCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_DGROUPID) ||
                   (pCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_PRE_COMMISSIONED_GROUPID)) {
            len += 2;//sink group id
        }

        if (pCmd->options.bits.addSink) {
            len += 1;//device id

            if (pCmd->options.bits.gpdSecFrameCntPresent) {
                len += 4;//GPD security frame counter
            }

            if (pCmd->options.bits.gpdSecKeyPresent) {
                len += SEC_KEY_LEN;//GPD key
            }

            if (pCmd->options.bits.assignedAliasPresent) {
                len += 2;//assigned alias
            }

            if (pCmd->options.bits.groupcastRadiusPresent) {
                len += 1;//groupcast radius
            }
        }
    }

    u8 *buf = (u8 *)ev_buf_allocate(len);
    if (!buf) {
        return ZCL_STA_INSUFFICIENT_SPACE;
    }

    u8 *pBuf = buf;

    //options
    *pBuf++ = U24_BYTE0(pCmd->options.opts);
    *pBuf++ = U24_BYTE1(pCmd->options.opts);
    *pBuf++ = U24_BYTE2(pCmd->options.opts);
    //GPD ID and Endpoint
    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        *pBuf++ = U32_BYTE0(pCmd->gpdId.srcId);
        *pBuf++ = U32_BYTE1(pCmd->gpdId.srcId);
        *pBuf++ = U32_BYTE2(pCmd->gpdId.srcId);
        *pBuf++ = U32_BYTE3(pCmd->gpdId.srcId);
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(pBuf, pCmd->gpdId.gpdIeeeAddr);
        pBuf += EXT_ADDR_LEN;
        *pBuf++ = pCmd->endpoint;
    }

    if (!pCmd->options.bits.removeGPD) {
        //Sink IEEE address and Sink NWK address; or Sink GroupID
        if ((pCmd->options.bits.commMode == GPS_COMM_MODE_FULL_UNICAST) ||
            (pCmd->options.bits.commMode == GPS_COMM_MODE_LIGHTWEIGHT_UNICAST)) {
            ZB_64BIT_ADDR_COPY(pBuf, pCmd->sinkIeeeAddr);
            pBuf += EXT_ADDR_LEN;
            *pBuf++ = LO_UINT16(pCmd->sinkNwkAddr);
            *pBuf++ = HI_UINT16(pCmd->sinkNwkAddr);
        } else if ((pCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_DGROUPID) ||
                   (pCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_PRE_COMMISSIONED_GROUPID)) {
            *pBuf++ = LO_UINT16(pCmd->sinkGroupID);
            *pBuf++ = HI_UINT16(pCmd->sinkGroupID);
        }

        if (pCmd->options.bits.addSink) {
            //Device ID
            *pBuf++ = pCmd->deviceID;
            //GPD security frame counter
            if (pCmd->options.bits.gpdSecFrameCntPresent) {
                *pBuf++ = U32_BYTE0(pCmd->gpdSecFrameCnt);
                *pBuf++ = U32_BYTE1(pCmd->gpdSecFrameCnt);
                *pBuf++ = U32_BYTE2(pCmd->gpdSecFrameCnt);
                *pBuf++ = U32_BYTE3(pCmd->gpdSecFrameCnt);
            }
            //GPD key
            if (pCmd->options.bits.gpdSecKeyPresent) {
                memcpy(pBuf, pCmd->gpdKey, SEC_KEY_LEN);
                pBuf += SEC_KEY_LEN;
            }
            //Assigned alias
            if (pCmd->options.bits.assignedAliasPresent) {
                *pBuf++ = LO_UINT16(pCmd->assignedAlias);
                *pBuf++ = HI_UINT16(pCmd->assignedAlias);
            }
            //Groupcast radius
            if (pCmd->options.bits.groupcastRadiusPresent) {
                *pBuf++ = pCmd->groupcastRadius;
            }
        }
    }

    zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GREEN_POWER, ZCL_CMD_GP_PAIRING, TRUE,
                ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, len, buf);

    ev_buf_free(buf);

    return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ status_t zcl_gp_proxyCommissioningModeCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_proxyCommissioningModeCmd_t *pCmd)
{
    u8 len = 1;//options

    if (pCmd->options.bits.action) {
        if (pCmd->options.bits.commWindowPresent) {
            len += 2;
        }
        if (pCmd->options.bits.channelPresent) {
            len++;
        }
    }

    u8 *buf = (u8 *)ev_buf_allocate(len);
    if (!buf) {
        return ZCL_STA_INSUFFICIENT_SPACE;
    }

    u8 *pBuf = buf;

    //Options
    *pBuf++ = pCmd->options.opts;
    //Commissioning window and channel
    if (pCmd->options.bits.action) {
        if (pCmd->options.bits.commWindowPresent) {
            *pBuf++ = LO_UINT16(pCmd->commissioningWindow);
            *pBuf++ = HI_UINT16(pCmd->commissioningWindow);
        }
        if (pCmd->options.bits.channelPresent) {
            *pBuf++ = pCmd->channel;
        }
    }

    zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GREEN_POWER, ZCL_CMD_GP_PROXY_COMMISSIONING_MODE, TRUE,
                ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, len, buf);

    ev_buf_free(buf);

    return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ status_t zcl_gp_responseCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_responseCmd_t *pCmd)
{
    u8 len = 1 + 2 + 1 + 1;//options + tempMasterShortAddr + tempMasterTxChannel + gpdCommandId

    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        len += 4;//srcID
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        len += 9;//ieeeAddr + endpoint
    }

    len++;//gpdCommandPayload, first byte is payload length.

    if ((pCmd->payloadLen != 0xff) && (pCmd->payloadLen != 0x0)) {
        len += pCmd->payloadLen;
    }

    u8 *buf = (u8 *)ev_buf_allocate(len);
    if (!buf) {
        return ZCL_STA_INSUFFICIENT_SPACE;
    }

    u8 *pBuf = buf;

    //Options
    *pBuf++ = pCmd->options.opts;
    //TempMaster short address
    *pBuf++ = LO_UINT16(pCmd->tempMasterShortAddr);
    *pBuf++ = HI_UINT16(pCmd->tempMasterShortAddr);
    //TempMaster Tx channel
    *pBuf++ = pCmd->tempMasterTxChannel;
    //GPD ID and Endpoint
    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        *pBuf++ = U32_BYTE0(pCmd->gpdId.srcId);
        *pBuf++ = U32_BYTE1(pCmd->gpdId.srcId);
        *pBuf++ = U32_BYTE2(pCmd->gpdId.srcId);
        *pBuf++ = U32_BYTE3(pCmd->gpdId.srcId);
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(pBuf, pCmd->gpdId.gpdIeeeAddr);
        pBuf += EXT_ADDR_LEN;
        *pBuf++ = pCmd->endpoint;
    }
    //GPD CommandID
    *pBuf++ = pCmd->gpdCmdID;
    //GPD Command payload length
    *pBuf++ = pCmd->payloadLen;
    if ((pCmd->payloadLen != 0xff) && (pCmd->payloadLen != 0x0)) {
        memcpy(pBuf, pCmd->pGpdCmdPayload, pCmd->payloadLen);
        pBuf += pCmd->payloadLen;
    }

    zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GREEN_POWER, ZCL_CMD_GP_RESPONSE, TRUE,
                ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, len, buf);

    ev_buf_free(buf);

    return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ status_t zcl_gp_sinkTableRspCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_sinkTabRspCmd_t *pCmd)
{
    u8 len = 4;//status + total number + start index + entries count

    len += pCmd->entriesLen;

    u8 *buf = (u8 *)ev_buf_allocate(len);
    if (!buf) {
        return ZCL_STA_INSUFFICIENT_SPACE;
    }

    u8 *pBuf = buf;

    *pBuf++ = pCmd->status;
    *pBuf++ = pCmd->totalTabEntries;
    *pBuf++ = pCmd->startIdx;
    *pBuf++ = pCmd->entriesCnt;
    if (pCmd->entriesLen) {
        memcpy(pBuf, pCmd->sinkTabEntry, pCmd->entriesLen);
    }

    zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GREEN_POWER, ZCL_CMD_GP_SINK_TABLE_RESPONSE, TRUE,
                ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, len, buf);

    ev_buf_free(buf);

    return ZCL_STA_SUCCESS;
}

status_t zcl_gp_transTableRspCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_transTabRspCmd_t *pCmd)
{
    u8 len = 5;//status + options + totalNumOfEntries + startIdx + entriesCnt

    len += pCmd->bufLen;

    u8 *buf = (u8 *)ev_buf_allocate(len);
    if (!buf) {
        return ZCL_STA_INSUFFICIENT_SPACE;
    }

    u8 *pBuf = buf;

    *pBuf++ = pCmd->status;
    *pBuf++ = pCmd->options.opts;
    *pBuf++ = pCmd->totalNumOfEntries;
    *pBuf++ = pCmd->startIdx;
    *pBuf++ = pCmd->entriesCnt;
    if (pCmd->bufLen) {
        memcpy(pBuf, pCmd->pBuf, pCmd->bufLen);
    }

    zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GREEN_POWER, ZCL_CMD_GP_TRANSLATION_TABLE_RESPONSE, TRUE,
                ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, len, buf);

    ev_buf_free(buf);

    return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ status_t zcl_gp_proxyTableReqCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_proxyTabReqCmd_t *pCmd)
{
    u8 len = 1;//options

    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        len += 4;//srcID
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        len += 9;//ieeeAddr + endpoint
    }
    if (pCmd->options.bits.reqType == REQUEST_TABLE_ENTRIES_BY_INDEX) {
        len++;//index
    }

    u8 *buf = (u8 *)ev_buf_allocate(len);
    if (!buf) {
        return ZCL_STA_INSUFFICIENT_SPACE;
    }

    u8 *pBuf = buf;

    //options
    *pBuf++ = pCmd->options.opts;
    //GPD ID and Endpoint
    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        *pBuf++ = U32_BYTE0(pCmd->gpdId.srcId);
        *pBuf++ = U32_BYTE1(pCmd->gpdId.srcId);
        *pBuf++ = U32_BYTE2(pCmd->gpdId.srcId);
        *pBuf++ = U32_BYTE3(pCmd->gpdId.srcId);
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(pBuf, pCmd->gpdId.gpdIeeeAddr);
        pBuf += EXT_ADDR_LEN;
        *pBuf++ = pCmd->endpoint;
    }
    //Index
    if (pCmd->options.bits.reqType == REQUEST_TABLE_ENTRIES_BY_INDEX) {
        *pBuf++ = pCmd->index;
    }

    zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GREEN_POWER, ZCL_CMD_GP_PROXY_TABLE_REQUEST, TRUE,
                ZCL_FRAME_SERVER_CLIENT_DIR, disableDefaultRsp, 0, seqNo, len, buf);

    ev_buf_free(buf);

    return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ status_t zcl_gp_sinkCommissioningModeCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_sinkCommissioningModeCmd_t *pCmd)
{
    u8 len = 1 + 2 + 2 + 1;//options + gpmAddrForSec + gpmAddrForPairing + sinkEndpoint;

    u8 *buf = (u8 *)ev_buf_allocate(len);
    if (!buf) {
        return ZCL_STA_INSUFFICIENT_SPACE;
    }

    u8 *pBuf = buf;

    //options
    *pBuf++ = pCmd->options.opts;
    //GPM address for security
    *pBuf++ = LO_UINT16(pCmd->gpmAddrForSec);
    *pBuf++ = HI_UINT16(pCmd->gpmAddrForSec);
    //GPM address for pairing
    *pBuf++ = LO_UINT16(pCmd->gpmAddrForPairing);
    *pBuf++ = HI_UINT16(pCmd->gpmAddrForPairing);
    //Sink Endpoint
    *pBuf++ = pCmd->sinkEndpoint;

    zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GREEN_POWER, ZCL_CMD_GP_SINK_COMMISSIONING_MODE, TRUE,
                ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, len, buf);

    ev_buf_free(buf);

    return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ status_t zcl_gp_pairingCfgCmd(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_gp_pairingConfigurationCmd_t *pCmd)
{
    u8 len = 6;//actions + options + deviceId + groupcastRadius + number of paired endpoints

    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        len += 4;//srcID
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        len += 9;//ieeeAddr + endpoint
    }

    if (pCmd->actions.bits.action != REMOVE_GPD) {
        if (pCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_PRE_COMMISSIONED_GROUPID) {
            len++;//numOfGroupList
            if (pCmd->pGroupList[0]) {
                len += pCmd->pGroupList[0] * sizeof(gpSinkGroupListItem_t);
            }
        }

        if (pCmd->options.bits.assignedAlias) {
            len += sizeof(u16);
        }

        if (pCmd->options.bits.secUse) {
            len += 21;//secOptions + frameCounter + key
        } else if (pCmd->options.bits.seqNumCap) {
            len += 4;//frameCounter
        }
    }

    if ((pCmd->numOfPairedEndpoints != 0x00) && (pCmd->numOfPairedEndpoints != 0xfd) &&
       ((pCmd->numOfPairedEndpoints != 0xfe) && (pCmd->numOfPairedEndpoints != 0xff))) {
        len += pCmd->numOfPairedEndpoints;
    }

    if (pCmd->actions.bits.action != REMOVE_GPD) {
        if (pCmd->options.bits.appInfoPresent) {
            len++;//appInfo

            if (pCmd->appInfo.bits.manuIdPresent) {
                len += 2;
            }
            if (pCmd->appInfo.bits.modelIdPresent) {
                len += 2;
            }
            if (pCmd->appInfo.bits.gpdCmdsPresent) {
                len++;//number of GPD commands
                if (pCmd->numOfGpdCmds) {
                    len += pCmd->numOfGpdCmds;
                }
            }
            if (pCmd->appInfo.bits.clusterListPresent) {
                len++;//length of cluster list
                if (pCmd->pClusterList[0]) {
                    len += ((pCmd->pClusterList[0] & 0x0f) + (pCmd->pClusterList[0] & 0xf0)) * sizeof(u16);
                }
            }
            if (pCmd->appInfo.bits.switchInfoPresent) {
                len++;//switch info length
                len += pCmd->switchInfo.switchInfoLen;
            }
        }

        if (pCmd->actions.bits.action == APPLICATION_DESCRIPTION) {
            len += pCmd->reportDescLen;//total number + number of reports + list
        }
    }

    u8 *buf = (u8 *)ev_buf_allocate(len);
    if (!buf) {
        return ZCL_STA_INSUFFICIENT_SPACE;
    }

    u8 *pBuf = buf;

    //options
    *pBuf++ = pCmd->actions.acts;
    //options
    *pBuf++ = LO_UINT16(pCmd->options.opts);
    *pBuf++ = HI_UINT16(pCmd->options.opts);
    //GPD ID and Endpoint
    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        *pBuf++ = U32_BYTE0(pCmd->gpdId.srcId);
        *pBuf++ = U32_BYTE1(pCmd->gpdId.srcId);
        *pBuf++ = U32_BYTE2(pCmd->gpdId.srcId);
        *pBuf++ = U32_BYTE3(pCmd->gpdId.srcId);
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(pBuf, pCmd->gpdId.gpdIeeeAddr);
        pBuf += EXT_ADDR_LEN;
        *pBuf++ = pCmd->endpoint;
    }
    //device ID
    *pBuf++ = pCmd->deviceId;

    if (pCmd->actions.bits.action != REMOVE_GPD) {
        //groupList
        if (pCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_PRE_COMMISSIONED_GROUPID) {
            *pBuf++ = pCmd->pGroupList[0];
            if (pCmd->pGroupList[0]) {
                memcpy(pBuf, &pCmd->pGroupList[1], pCmd->pGroupList[0] * sizeof(gpSinkGroupListItem_t));
                pBuf += pCmd->pGroupList[0] * sizeof(gpSinkGroupListItem_t);
            }
        }
        //assigned alias
        if (pCmd->options.bits.assignedAlias) {
            *pBuf++ = LO_UINT16(pCmd->gpdAssignedAlias);
            *pBuf++ = HI_UINT16(pCmd->gpdAssignedAlias);
        }
    }

    //groupcast radius
    *pBuf++ = pCmd->groupcastRadius;

    if (pCmd->actions.bits.action != REMOVE_GPD) {
        //security
        if (pCmd->options.bits.secUse) {
            *pBuf++ = pCmd->secOptions.opts;

            *pBuf++ = U32_BYTE0(pCmd->gpdSecFrameCnt);
            *pBuf++ = U32_BYTE1(pCmd->gpdSecFrameCnt);
            *pBuf++ = U32_BYTE2(pCmd->gpdSecFrameCnt);
            *pBuf++ = U32_BYTE3(pCmd->gpdSecFrameCnt);

            memcpy(pBuf, pCmd->gpdSecKey, SEC_KEY_LEN);
            pBuf += SEC_KEY_LEN;
        } else if (pCmd->options.bits.seqNumCap) {
            *pBuf++ = U32_BYTE0(pCmd->gpdSecFrameCnt);
            *pBuf++ = U32_BYTE1(pCmd->gpdSecFrameCnt);
            *pBuf++ = U32_BYTE2(pCmd->gpdSecFrameCnt);
            *pBuf++ = U32_BYTE3(pCmd->gpdSecFrameCnt);
        }
    }

    //number of paired endpoint
    *pBuf++ = pCmd->numOfPairedEndpoints;
    //paired endpoint
    if ((pCmd->numOfPairedEndpoints != 0x00) && (pCmd->numOfPairedEndpoints != 0xfd) &&
        (pCmd->numOfPairedEndpoints != 0xfe) && (pCmd->numOfPairedEndpoints != 0xff)) {
        memcpy(pBuf, pCmd->pPairedEndpoints, pCmd->numOfPairedEndpoints);
        pBuf += pCmd->numOfPairedEndpoints;
    }

    if (pCmd->actions.bits.action != REMOVE_GPD) {
        //application information
        if (pCmd->options.bits.appInfoPresent) {
            //appInfo
            *pBuf++ = pCmd->appInfo.info;
            //manuId
            if (pCmd->appInfo.bits.manuIdPresent) {
                *pBuf++ = LO_UINT16(pCmd->manufacturerId);
                *pBuf++ = HI_UINT16(pCmd->manufacturerId);
            }
            //modelId
            if (pCmd->appInfo.bits.modelIdPresent) {
                *pBuf++ = LO_UINT16(pCmd->modelId);
                *pBuf++ = HI_UINT16(pCmd->modelId);
            }
            if (pCmd->appInfo.bits.gpdCmdsPresent) {
                //number of GPD commands
                *pBuf++ = pCmd->numOfGpdCmds;
                //command list
                if (pCmd->numOfGpdCmds) {
                    memcpy(pBuf, pCmd->pGpdCmdList, pCmd->numOfGpdCmds);
                    pBuf += pCmd->numOfGpdCmds;
                }
            }
            //cluster list
            if (pCmd->appInfo.bits.clusterListPresent) {
                *pBuf++ = pCmd->pClusterList[0];
                if (pCmd->pClusterList[0]) {
                    u8 clusterLen = ((pCmd->pClusterList[0] & 0x0f) + (pCmd->pClusterList[0] & 0xf0)) * sizeof(u16);
                    memcpy(pBuf, &pCmd->pClusterList[1], clusterLen);
                    pBuf += clusterLen;
                }
            }
            //switchInfo
            if (pCmd->appInfo.bits.switchInfoPresent) {
                *pBuf++ = pCmd->switchInfo.switchInfoLen;
                *pBuf++ = pCmd->switchInfo.switchCfg.cfg;
                *pBuf++ = pCmd->switchInfo.contactStatus;
            }
        }
        //reportInfo
        if (pCmd->actions.bits.action == APPLICATION_DESCRIPTION) {
            memcpy(pBuf, pCmd->pReportDescriptor, pCmd->reportDescLen);
            pBuf += pCmd->reportDescLen;
        }
    }

    zcl_sendCmd(srcEp, pDstEpInfo, ZCL_CLUSTER_GEN_GREEN_POWER, ZCL_CMD_GP_PAIRING_CONFIGURATION, TRUE,
                ZCL_FRAME_CLIENT_SERVER_DIR, disableDefaultRsp, 0, seqNo, len, buf);

    ev_buf_free(buf);

    return ZCL_STA_SUCCESS;
}

_CODE_ZCL_ static void zcl_gp_notificationParse(u8 *pData, zcl_gp_notificationCmd_t *pCmd)
{
    pCmd->options.opts = BUILD_U16(pData[0], pData[1]);
    pData += 2;

    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        pCmd->gpdId.srcId = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
        pData += 4;
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(pCmd->gpdId.gpdIeeeAddr, pData);
        pData += EXT_ADDR_LEN;
        pCmd->endpoint = *pData++;
    }

    pCmd->gpdSecFrameCnt = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
    pData += 4;

    pCmd->gpdCmdID = *pData++;

    pCmd->payloadLen = *pData++;
    pCmd->pGpdCmdPayload = pData;
    pData += pCmd->payloadLen;

    if (pCmd->options.bits.proxyInfoPresent) {
        pCmd->gppShortAddr = BUILD_U16(pData[0], pData[1]);
        pData += 2;
        pCmd->gppGpdLink.link = *pData++;
    }
}

_CODE_ZCL_ static void zcl_gp_commNotificationParse(u8 *pData, zcl_gp_commissioningNotificationCmd_t *pCmd)
{
    pCmd->options.opts = BUILD_U16(pData[0], pData[1]);
    pData += 2;

    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        pCmd->gpdId.srcId = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
        pData += 4;
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(pCmd->gpdId.gpdIeeeAddr, pData);
        pData += EXT_ADDR_LEN;
        pCmd->endpoint = *pData++;
    }

    pCmd->gpdSecFrameCnt = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
    pData += 4;

    pCmd->gpdCmdID = *pData++;

    pCmd->payloadLen = *pData++;
    pCmd->pGpdCmdPayload = pData;
    pData += pCmd->payloadLen;

    if (pCmd->options.bits.proxyInfoPresent) {
        pCmd->gppShortAddr = BUILD_U16(pData[0], pData[1]);
        pData += 2;
        pCmd->gppGpdLink.link = *pData++;
    }

    if (pCmd->options.bits.secPrcFailed) {
        pCmd->mic = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
        pData += 4;
    }
}

_CODE_ZCL_ static void zcl_gp_sinkCommModeParse(u8 *pData, zcl_gp_sinkCommissioningModeCmd_t *pCmd)
{
    pCmd->options.opts = *pData++;

    pCmd->gpmAddrForSec = BUILD_U16(pData[0], pData[1]);
    pData += 2;

    pCmd->gpmAddrForPairing = BUILD_U16(pData[0], pData[1]);
    pData += 2;

    pCmd->sinkEndpoint = *pData++;
}

_CODE_ZCL_ static void zcl_gp_pairingCfgParse(u8 *pData, u16 dataLen, zcl_gp_pairingConfigurationCmd_t *pCmd)
{
    u8 *pBuf = pData;

    pCmd->actions.acts = *pBuf++;

    pCmd->options.opts = BUILD_U16(pBuf[0], pBuf[1]);
    pBuf += 2;

    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        pCmd->gpdId.srcId = BUILD_U32(pBuf[0], pBuf[1], pBuf[2], pBuf[3]);
        pBuf += 4;
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(pCmd->gpdId.gpdIeeeAddr, pBuf);
        pBuf += EXT_ADDR_LEN;
        pCmd->endpoint = *pBuf++;
    }

    pCmd->deviceId = *pBuf++;

    u8 len = 0;
    if (pCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_PRE_COMMISSIONED_GROUPID) {
        pCmd->pGroupList = pBuf;
        pBuf += 1;
        if (pCmd->pGroupList[0]) {
            len = pCmd->pGroupList[0] * sizeof(gpSinkGroupListItem_t);
            pBuf += len;
        }
    }

    if (pCmd->options.bits.assignedAlias) {
        pCmd->gpdAssignedAlias = BUILD_U16(pBuf[0], pBuf[1]);
        pBuf += 2;
    }

    pCmd->groupcastRadius = *pBuf++;

    if (pCmd->options.bits.secUse) {
        pCmd->secOptions.opts = *pBuf++;

        pCmd->gpdSecFrameCnt = BUILD_U32(pBuf[0], pBuf[1], pBuf[2], pBuf[3]);
        pBuf += 4;

        memcpy(pCmd->gpdSecKey, pBuf, SEC_KEY_LEN);
        pBuf += SEC_KEY_LEN;
    } else if (pCmd->options.bits.seqNumCap) {
        pCmd->gpdSecFrameCnt = BUILD_U32(pBuf[0], pBuf[1], pBuf[2], pBuf[3]);
        pBuf += 4;
    }

    pCmd->numOfPairedEndpoints = *pBuf++;

    if ((pCmd->numOfPairedEndpoints != 0x00) && (pCmd->numOfPairedEndpoints != 0xfe) &&
        (pCmd->numOfPairedEndpoints != 0xfd) && (pCmd->numOfPairedEndpoints != 0xff)) {
        pCmd->pPairedEndpoints = pBuf;
        pBuf += pCmd->numOfPairedEndpoints;
    }

    if (pCmd->options.bits.appInfoPresent) {
        pCmd->appInfo.info = *pBuf++;

        if (pCmd->appInfo.bits.manuIdPresent) {
            pCmd->manufacturerId = BUILD_U16(pBuf[0], pBuf[1]);
            pBuf += 2;
        }
        if (pCmd->appInfo.bits.modelIdPresent) {
            pCmd->modelId = BUILD_U16(pBuf[0], pBuf[1]);
            pBuf += 2;
        }
        if (pCmd->appInfo.bits.gpdCmdsPresent) {
            pCmd->numOfGpdCmds = *pBuf++;
            if (pCmd->numOfGpdCmds) {
                pCmd->pGpdCmdList = pBuf;
                pBuf += pCmd->numOfGpdCmds;
            }
        }
        if (pCmd->appInfo.bits.clusterListPresent) {
            pCmd->pClusterList = pBuf;
            pBuf += 1;
            if (pCmd->pClusterList[0]) {
                len = ((pCmd->pClusterList[0] & 0x0f) + (pCmd->pClusterList[0] & 0xf0)) * sizeof(u16);
                pBuf += len;
            }
        }
        if (pCmd->appInfo.bits.switchInfoPresent) {
            pCmd->switchInfo.switchInfoLen = *pBuf++;

            if (pCmd->switchInfo.switchInfoLen == 2) {
                pCmd->switchInfo.switchCfg.cfg = *pBuf++;
                pCmd->switchInfo.contactStatus = *pBuf++;
            } else {
                pBuf += pCmd->switchInfo.switchInfoLen;
            }
        }
    }

    if (pCmd->actions.bits.action == APPLICATION_DESCRIPTION) {
        if (dataLen > pBuf - pData) {
            pCmd->reportDescLen = dataLen - (pBuf - pData);
            pCmd->pReportDescriptor = pBuf;
        }
    }
}

_CODE_ZCL_ static void zcl_gp_sinkTabReqParse(u8 *pData, zcl_gp_sinkTabReqCmd_t *pCmd)
{
    pCmd->options.opts = *pData++;

    if (pCmd->options.bits.reqType == REQUEST_TABLE_ENTRIES_BY_GPD_ID) {
        if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
            pCmd->gpdId.srcId = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
            pData += 4;
        } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
            ZB_64BIT_ADDR_COPY(pCmd->gpdId.gpdIeeeAddr, pData);
            pData += EXT_ADDR_LEN;
            pCmd->endpoint = *pData++;
        }
    } else if (pCmd->options.bits.reqType == REQUEST_TABLE_ENTRIES_BY_INDEX) {
        pCmd->index = *pData++;
    }
}

_CODE_ZCL_ static void zcl_gp_proxyTabRspParse(u8 *pData, u16 dataLen, zcl_gp_proxyTabRspCmd_t *pCmd)
{
    u8 *ptr = pData;

    pCmd->status = *pData++;
    pCmd->totalTabEntries = *pData++;
    pCmd->startIdx = *pData++;
    pCmd->entriesCnt = *pData++;
    pCmd->entriesLen = dataLen - (pData - ptr);
    if (pCmd->entriesLen) {
        pCmd->proxyTabEntry = pData;
    }
}

_CODE_ZCL_ static void zcl_gp_transTabUpdateParse(u8 *pData, u16 dataLen, zcl_gp_transTabUpdateCmd_t *pCmd)
{
    u8 *ptr = pData;

    pCmd->options.opts = BUILD_U16(pData[0], pData[1]);
    pData += 2;

    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        pCmd->gpdId.srcId = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
        pData += 4;
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(pCmd->gpdId.gpdIeeeAddr, pData);
        pData += EXT_ADDR_LEN;
        pCmd->endpoint = *pData++;
    }

    pCmd->bufLen = dataLen - (pData - ptr);
    if (pCmd->bufLen) {
        pCmd->pBuf = pData;
    }
}

_CODE_ZCL_ static void zcl_gp_pairingParse(u8 *pData, zcl_gp_pairingCmd_t *pCmd)
{
    //options
    pCmd->options.opts = BUILD_U24(pData[0], pData[1], pData[2]);
    pData += 3;

    //GPD ID and Endpoint
    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        pCmd->gpdId.srcId = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
        pData += 4;
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(pCmd->gpdId.gpdIeeeAddr, pData);
        pData += EXT_ADDR_LEN;
        pCmd->endpoint = *pData++;
    }

    if (!pCmd->options.bits.removeGPD) {
        if ((pCmd->options.bits.commMode == GPS_COMM_MODE_FULL_UNICAST) ||
            (pCmd->options.bits.commMode == GPS_COMM_MODE_LIGHTWEIGHT_UNICAST)) {
            ZB_64BIT_ADDR_COPY(pCmd->sinkIeeeAddr, pData);
            pData += EXT_ADDR_LEN;
            pCmd->sinkNwkAddr = BUILD_U16(pData[0], pData[1]);
            pData += 2;
        } else if ((pCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_DGROUPID) ||
                   (pCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_PRE_COMMISSIONED_GROUPID)) {
            pCmd->sinkGroupID = BUILD_U16(pData[0], pData[1]);
            pData += 2;
        }
    } else {
        //remove GPD
        return;
    }

    if (pCmd->options.bits.addSink) {
        pCmd->deviceID = *pData++;

        if (pCmd->options.bits.gpdSecFrameCntPresent) {
            pCmd->gpdSecFrameCnt = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
            pData += 4;
        }

        if (pCmd->options.bits.gpdSecKeyPresent) {
            memcpy(pCmd->gpdKey, pData, SEC_KEY_LEN);
            pData += SEC_KEY_LEN;
        }

        if (pCmd->options.bits.assignedAliasPresent) {
            pCmd->assignedAlias = BUILD_U16(pData[0], pData[1]);
            pData += 2;
        }

        if (pCmd->options.bits.groupcastRadiusPresent) {
            pCmd->groupcastRadius = *pData++;
        }
    }
}

_CODE_ZCL_ static void zcl_gp_sinkTabRspParse(u8 *pData, u16 dataLen, zcl_gp_sinkTabRspCmd_t *pCmd)
{
    pCmd->status = *pData++;
    pCmd->totalTabEntries = *pData++;
    pCmd->startIdx = *pData++;
    pCmd->entriesCnt = *pData++;
    pCmd->sinkTabEntry = pData;
    pCmd->entriesLen = dataLen - 4;
}

_CODE_ZCL_ static void zcl_gp_proxyTabReqParse(u8 *pData, zcl_gp_proxyTabReqCmd_t *pCmd)
{
    //options
    pCmd->options.opts = *pData++;

    //GPD ID and Endpoint
    if (pCmd->options.bits.reqType == REQUEST_TABLE_ENTRIES_BY_GPD_ID) {
        if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
            pCmd->gpdId.srcId = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
        } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
            ZB_64BIT_ADDR_COPY(pCmd->gpdId.gpdIeeeAddr, pData);
            pData += EXT_ADDR_LEN;
            pCmd->endpoint = *pData;
        }
    } else if (pCmd->options.bits.reqType == REQUEST_TABLE_ENTRIES_BY_INDEX) {
        pCmd->index = *pData;
    }
}

_CODE_ZCL_ static void zcl_gp_proxyCommModeParse(u8 *pData, zcl_gp_proxyCommissioningModeCmd_t *pCmd)
{
    //options
    pCmd->options.opts = *pData++;

    if (pCmd->options.bits.action) {
        if (pCmd->options.bits.commWindowPresent) {
            pCmd->commissioningWindow = BUILD_U16(pData[0], pData[1]);
            pData += 2;
        }
        if (pCmd->options.bits.channelPresent) {
            pCmd->channel = *pData++;
        }
    }
}

_CODE_ZCL_ static void zcl_gp_responseParse(u8 *pData, u16 dataLen, zcl_gp_responseCmd_t *pCmd)
{
    //options
    pCmd->options.opts = *pData++;

    pCmd->tempMasterShortAddr = BUILD_U16(pData[0], pData[1]);
    pData += 2;

    pCmd->tempMasterTxChannel = *pData++;

    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        pCmd->gpdId.srcId = BUILD_U32(pData[0], pData[1], pData[2], pData[3]);
        pData += 4;
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(pCmd->gpdId.gpdIeeeAddr, pData);
        pData += EXT_ADDR_LEN;
        pCmd->endpoint = *pData++;
    }

    pCmd->gpdCmdID = *pData++;

    pCmd->payloadLen = *pData++;

    pCmd->pGpdCmdPayload = pData;
}

_CODE_ZCL_ static status_t zcl_gp_notificationPrc(zclIncoming_t *pInMsg)
{
    status_t status = ZCL_STA_SUCCESS;

    if (pInMsg->clusterAppCb) {
        zcl_gp_notificationCmd_t cmd;
        TL_SETSTRUCTCONTENT(cmd, 0);

        zcl_gp_notificationParse(pInMsg->pData, &cmd);

        status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &cmd);
    } else {
        status = ZCL_STA_FAILURE;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_gp_commissioningNotificationPrc(zclIncoming_t *pInMsg)
{
    status_t status = ZCL_STA_SUCCESS;

    if (pInMsg->clusterAppCb) {
        zcl_gp_commissioningNotificationCmd_t cmd;
        TL_SETSTRUCTCONTENT(cmd, 0);

        zcl_gp_commNotificationParse(pInMsg->pData, &cmd);

        status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &cmd);
    } else {
        status = ZCL_STA_FAILURE;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_gp_sinkCommissioningModePrc(zclIncoming_t *pInMsg)
{
    status_t status = ZCL_STA_SUCCESS;

    if (pInMsg->clusterAppCb) {
        zcl_gp_sinkCommissioningModeCmd_t cmd;
        TL_SETSTRUCTCONTENT(cmd, 0);

        zcl_gp_sinkCommModeParse(pInMsg->pData, &cmd);

        status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &cmd);
    } else {
        status = ZCL_STA_FAILURE;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_gp_pairingCfgPrc(zclIncoming_t *pInMsg)
{
    status_t status = ZCL_STA_SUCCESS;

    if (pInMsg->clusterAppCb) {
        zcl_gp_pairingConfigurationCmd_t cmd;
        TL_SETSTRUCTCONTENT(cmd, 0);

        zcl_gp_pairingCfgParse(pInMsg->pData, pInMsg->dataLen, &cmd);

        status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &cmd);
    } else {
        status = ZCL_STA_FAILURE;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_gp_sinkTabReqPrc(zclIncoming_t *pInMsg)
{
    status_t status = ZCL_STA_SUCCESS;

    if (pInMsg->clusterAppCb) {
        zcl_gp_sinkTabReqCmd_t cmd;
        TL_SETSTRUCTCONTENT(cmd, 0);

        zcl_gp_sinkTabReqParse(pInMsg->pData, &cmd);

        status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &cmd);
    } else {
        status = ZCL_STA_FAILURE;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_gp_proxyTabRspPrc(zclIncoming_t *pInMsg)
{
    status_t status = ZCL_STA_SUCCESS;

    if (pInMsg->clusterAppCb) {
        zcl_gp_proxyTabRspCmd_t cmd;
        TL_SETSTRUCTCONTENT(cmd, 0);

        zcl_gp_proxyTabRspParse(pInMsg->pData, pInMsg->dataLen, &cmd);

        status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &cmd);
    } else {
        status = ZCL_STA_FAILURE;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_gp_transTabReqPrc(zclIncoming_t *pInMsg)
{
    status_t status = ZCL_STA_SUCCESS;

    if (pInMsg->clusterAppCb) {
        zcl_gp_transTabReqCmd_t cmd;
        TL_SETSTRUCTCONTENT(cmd, 0);

        cmd.startIdx = pInMsg->pData[0];

        status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &cmd);
    } else {
        status = ZCL_STA_FAILURE;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_gp_transTabUpdatePrc(zclIncoming_t *pInMsg)
{
    status_t status = ZCL_STA_SUCCESS;

    if (pInMsg->clusterAppCb) {
        zcl_gp_transTabUpdateCmd_t cmd;
        TL_SETSTRUCTCONTENT(cmd, 0);

        zcl_gp_transTabUpdateParse(pInMsg->pData, pInMsg->dataLen, &cmd);

        status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &cmd);
    } else {
        status = ZCL_STA_FAILURE;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_gp_pairingPrc(zclIncoming_t *pInMsg)
{
    status_t status = ZCL_STA_SUCCESS;

    if (pInMsg->clusterAppCb) {
        zcl_gp_pairingCmd_t cmd;
        TL_SETSTRUCTCONTENT(cmd, 0);

        zcl_gp_pairingParse(pInMsg->pData, &cmd);

        status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &cmd);
    } else {
        status = ZCL_STA_FAILURE;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_gp_proxyCommissioningModePrc(zclIncoming_t *pInMsg)
{
    status_t status = ZCL_STA_SUCCESS;

    if (pInMsg->clusterAppCb) {
        zcl_gp_proxyCommissioningModeCmd_t cmd;
        TL_SETSTRUCTCONTENT(cmd, 0);

        zcl_gp_proxyCommModeParse(pInMsg->pData, &cmd);

        status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &cmd);
    } else {
        status = ZCL_STA_FAILURE;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_gp_ResponsePrc(zclIncoming_t *pInMsg)
{
    status_t status = ZCL_STA_SUCCESS;

    if (pInMsg->clusterAppCb) {
        zcl_gp_responseCmd_t cmd;
        TL_SETSTRUCTCONTENT(cmd, 0);

        zcl_gp_responseParse(pInMsg->pData, pInMsg->dataLen, &cmd);

        status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &cmd);
    } else {
        status = ZCL_STA_FAILURE;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_gp_sinkTabRspPrc(zclIncoming_t *pInMsg)
{
    status_t status = ZCL_STA_SUCCESS;

    if (pInMsg->clusterAppCb) {
        zcl_gp_sinkTabRspCmd_t cmd;
        TL_SETSTRUCTCONTENT(cmd, 0);

        zcl_gp_sinkTabRspParse(pInMsg->pData, pInMsg->dataLen, &cmd);

        status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &cmd);
    } else {
        status = ZCL_STA_FAILURE;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_gp_proxyTabReqPrc(zclIncoming_t *pInMsg)
{
    status_t status = ZCL_STA_SUCCESS;

    if (pInMsg->clusterAppCb) {
        zcl_gp_proxyTabReqCmd_t cmd;
        TL_SETSTRUCTCONTENT(cmd, 0);

        zcl_gp_proxyTabReqParse(pInMsg->pData, &cmd);

        status = pInMsg->clusterAppCb(&(pInMsg->addrInfo), pInMsg->hdr.cmd, &cmd);
    } else {
        status = ZCL_STA_FAILURE;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_gp_clientCmdHandler(zclIncoming_t *pInMsg)
{
    u8 status = ZCL_STA_SUCCESS;

    switch (pInMsg->hdr.cmd) {
    case ZCL_CMD_GP_NOTIFICATION:
        status = zcl_gp_notificationPrc(pInMsg);
        break;
    case ZCL_CMD_GP_COMMISSIONING_NOTIFICATION:
        status = zcl_gp_commissioningNotificationPrc(pInMsg);
        break;
    case ZCL_CMD_GP_SINK_COMMISSIONING_MODE:
        status = zcl_gp_sinkCommissioningModePrc(pInMsg);
        break;
    case ZCL_CMD_GP_PAIRING_CONFIGURATION:
        status = zcl_gp_pairingCfgPrc(pInMsg);
        break;
    case ZCL_CMD_GP_SINK_TABLE_REQUEST:
        status = zcl_gp_sinkTabReqPrc(pInMsg);
        break;
    case ZCL_CMD_GP_PROXY_TABLE_RESPONSE:
        status = zcl_gp_proxyTabRspPrc(pInMsg);
        break;
    case ZCL_CMD_GP_TRANSLATION_TABLE_REQUEST:
        status = zcl_gp_transTabReqPrc(pInMsg);
        break;
    case ZCL_CMD_GP_TRANSLATION_TABLE_UPDATE_COMMAND:
        status = zcl_gp_transTabUpdatePrc(pInMsg);
        break;
    default:
        status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
        break;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_gp_serverCmdHandler(zclIncoming_t *pInMsg)
{
    u8 status = ZCL_STA_SUCCESS;

    switch (pInMsg->hdr.cmd) {
    case ZCL_CMD_GP_PAIRING:
        status = zcl_gp_pairingPrc(pInMsg);
        break;
    case ZCL_CMD_GP_PROXY_COMMISSIONING_MODE:
        status = zcl_gp_proxyCommissioningModePrc(pInMsg);
        break;
    case ZCL_CMD_GP_RESPONSE:
        status = zcl_gp_ResponsePrc(pInMsg);
        break;
    case ZCL_CMD_GP_SINK_TABLE_RESPONSE:
        status = zcl_gp_sinkTabRspPrc(pInMsg);
        break;
    case ZCL_CMD_GP_PROXY_TABLE_REQUEST:
        status = zcl_gp_proxyTabReqPrc(pInMsg);
        break;
    default:
        status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
        break;
    }

    return status;
}

_CODE_ZCL_ static status_t zcl_gp_cmdHandler(zclIncoming_t *pInMsg)
{
    if (pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR) {
        return zcl_gp_clientCmdHandler(pInMsg);
    } else {
        return zcl_gp_serverCmdHandler(pInMsg);
    }
}

#endif	/* ZCL_GREEN_POWER */
