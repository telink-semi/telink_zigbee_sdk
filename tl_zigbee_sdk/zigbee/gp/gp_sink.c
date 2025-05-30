/********************************************************************************************************
 * @file    gp_sink.c
 *
 * @brief   This is the source file for gp_sink
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
#include "../common/includes/zb_common.h"
#include "../zcl/zcl_include.h"
#include "gp_sec.h"
#include "gp_base.h"
#include "gp.h"
#include "gp_sink.h"
#include "gp_trans.h"

#if (GP_SUPPORT_ENABLE && GP_BASIC_COMBO)
/**********************************************************************
 * GLOBAL VARIABLES
 */
gps_ctx_t g_gpsCtx = {
    .gpsCommWindowTimeoutEvt            = NULL,
    .gpsMultiSensorCommTimeoutEvt       = NULL,
    .multiSensorCommBufSize             = 0,
    .multiSensorCommTime                = GPS_MULTI_SENSOR_COMMISSIONING_TIMEOUT,
    .multiSensorTotalNumReport          = 0,
    .multiSensorNumOfReport             = 0,
    .multiSensorCompleted               = 0,
    .multiSensorReportMatched           = 0,
    .gpsInCommMode                      = FALSE,
    .gpsAppEndpoint                     = 0,
};

/**********************************************************************
 * FUNCTIONS
 */
void gp_sinkInit(u8 endpoint)
{
    g_gpsCtx.gpsAppEndpoint = endpoint;

    gpSinkTabInit();

    gp_transTabInit();

    COPY_U24TOBUFFER(zclGpAttr_gpsFunc, GPS_FUNCTIONALITY);
    COPY_U24TOBUFFER(zclGpAttr_gpsActiveFunc, GPS_ACTIVE_FUNCTIONALITY);
}

gpSecRsp_status_t gpSinkSecOperation(u8 appId, gpdId_t gpdId,
                                     u8 endpoint, u32 gpdSecFrameCnt,
                                     u8 gpdfSecurityLevel, u8 gpdfKeyType,
                                     u8 *pKeyType, u8 *pKey)
{
    u8 secChkFail = FALSE;
    u8 epChkFail = FALSE;
    gpSecRsp_status_t status = GP_SEC_RSP_STATUS_MATCH;

    //A.3.7.3.1.2
    gpSinkTabEntry_t *pGpSinkTabEntry = gp_getSinkTabByGpdId(appId, gpdId);

    //printf("gpSinkSecOperation: sinkEntry = %x\n", pGpSinkTabEntry);

    if (pGpSinkTabEntry && pGpSinkTabEntry->complete) {
        if (gpdfSecurityLevel != GP_SEC_LEVEL_NO_SECURITY) {
            if (pGpSinkTabEntry->options.bits.securityUse) {
                //check security level and frame counter.
                if ((gpdfSecurityLevel != pGpSinkTabEntry->secOptions.bits.secLevel) ||
                    (gpdSecFrameCnt <= pGpSinkTabEntry->gpdSecFrameCnt)) {
                    secChkFail = TRUE;
                }
                //check security key type mapping.
                else if (gpSecKeyTypeMappingChk(pGpSinkTabEntry->secOptions.bits.secKeyType, gpdfKeyType)) {
                    secChkFail = TRUE;
                }
            } else {
                secChkFail = TRUE;
            }
        }

        if (secChkFail == TRUE) {
            return GP_SEC_RSP_STATUS_DROP_FRAME;
        } else {
            if (appId == GP_APP_ID_GPD) {
                if (pGpSinkTabEntry->endpoint == 0xff) {
                    epChkFail = FALSE;
                } else if ((endpoint != pGpSinkTabEntry->endpoint) &&
                           (endpoint != 0) && (endpoint != 0xFF)) {
                    epChkFail = TRUE;
                }
            }

            status = (epChkFail == TRUE) ? GP_SEC_RSP_STATUS_TX_THEN_DROP : GP_SEC_RSP_STATUS_MATCH;

            if (gpdfSecurityLevel == GP_SEC_LEVEL_NO_SECURITY) {
                return status;
            }

            return gpKeyRecovery(gpdfKeyType, pGpSinkTabEntry->secOptions.bits.secKeyType, pGpSinkTabEntry->gpdKey, status, pKeyType, pKey);
        }
    } else {
        if (gpdfKeyType == 0) {//shared key
            //if there is no shared key, then pass unprocessed
            if ((gpdfSecurityLevel != GP_SEC_LEVEL_NO_SECURITY) &&
                ZB_SEC_KEY_IS_NULL(zclGpAttr_gpSharedSecKey)) {
                return GP_SEC_RSP_STATUS_DROP_FRAME;
            }

            memcpy(pKey, zclGpAttr_gpSharedSecKey, SEC_KEY_LEN);
            *pKeyType = (gpdfSecurityLevel == GP_SEC_LEVEL_NO_SECURITY) ? GP_SEC_KEY_TYPE_NO_KEY : zclGpAttr_gpSharedSecKeyType;

            return GP_SEC_RSP_STATUS_MATCH;
        } else {
            if (pGpSinkTabEntry) {
                return gpKeyRecovery(gpdfKeyType, pGpSinkTabEntry->secOptions.bits.secKeyType, pGpSinkTabEntry->gpdKey, status, pKeyType, pKey);
            }

            return GP_SEC_RSP_STATUS_DROP_FRAME;
        }
    }
}

static gpSecRsp_status_t gpSinkTunneledSecOperation(u8 appId, gpdId_t gpdId,
                                                    u8 endpoint, u32 gpdSecFrameCnt,
                                                    u8 secLevel, u8 secKeyType,
                                                    u8 *pKeyType, u8 *pKey)
{
    u8 gpdfKeyType = (secKeyType >= GP_SEC_KEY_TYPE_OUT_OF_THE_BOX_GPD_KEY) ? 1 : 0;

    return gpSinkSecOperation(appId, gpdId, endpoint, gpdSecFrameCnt,
                              secLevel, gpdfKeyType, pKeyType, pKey);
}

static void gp_sinkTabNoCompleteEntryClear(void)
{
    //printf("sinkNoCompleteClear\n");

    for (u8 i = 0; i < zclGpAttr_gpsMaxSinkTabEntries; i++) {
        gpSinkTabEntry_t *pEntry = &g_gpSinkTab.gpSinkTab[i];

        if ((pEntry->used) && (!pEntry->complete)) {
            gp_transTabEntryRemove(pEntry->options.bits.appId, pEntry->gpdId, pEntry->endpoint);
            gp_sinkRemoveEntries(pEntry->options.bits.appId, pEntry->gpdId, pEntry->endpoint);
        }
    }
}

static void gpProxyCommissioningModeCmdSend(bool enter, bool involveProxies)
{
    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
    dstEpInfo.dstEp = GREEN_POWER_ENDPOINT;
    dstEpInfo.dstAddr.shortAddr = NWK_BROADCAST_RX_ON_WHEN_IDLE;
    dstEpInfo.profileId = GP_PROFILE_ID;
    dstEpInfo.txOptions = 0;
    dstEpInfo.radius = 0;

    zcl_gp_proxyCommissioningModeCmd_t commMode;
    TL_SETSTRUCTCONTENT(commMode, 0);

    commMode.options.bits.action = enter;//enter commissioning
    commMode.options.bits.exitMode = EXIT_ON_FIRST_PAIRING_SUCCESS;
    if (enter) {
        if ((zclGpAttr_gpsCommExitMode & GPS_EXIT_ON_COMM_WINDOW_EXPIRATION) && zclGpAttr_gpsCommWindow) {
            commMode.options.bits.commWindowPresent = 1;
            commMode.options.bits.exitMode |= EXIT_ON_GP_PROXY_COMMISSIONING_MODE;
            commMode.commissioningWindow = zclGpAttr_gpsCommWindow;
        }
    }

    if (involveProxies) {
        zcl_gp_proxyCommissioningModeCmdSend(GREEN_POWER_ENDPOINT, &dstEpInfo, TRUE, &commMode);
    }
}

static s32 gpMultiSensorCommTimeoutCb(void *arg)
{
    gpSinkTabEntry_t *pSinkEntry = (gpSinkTabEntry_t *)arg;

    //printf("gpMultiCommTimeoutCb: evt = %x\n", g_gpsCtx.gpsMultiSensorCommTimeoutEvt);

    for (u8 i = 0; i < GPS_MULTI_BUF_SIZE_MAX; i++) {
        if (g_gpsCtx.pMultiSensorCommBuf[i]) {
            ev_buf_free(g_gpsCtx.pMultiSensorCommBuf[i]);
            g_gpsCtx.pMultiSensorCommBuf[i] = NULL;
        }
    }
    g_gpsCtx.multiSensorCommBufSize = 0;
    g_gpsCtx.multiSensorTotalNumReport = 0;
    g_gpsCtx.multiSensorNumOfReport = 0;
    g_gpsCtx.multiSensorCompleted = 0;
    g_gpsCtx.multiSensorReportMatched = 0;

    //commissioning timeout, remove entry and translation table.
    gp_transTabEntryRemove(pSinkEntry->options.bits.appId, pSinkEntry->gpdId, pSinkEntry->endpoint);
    gp_sinkRemoveEntries(pSinkEntry->options.bits.appId, pSinkEntry->gpdId, pSinkEntry->endpoint);

    g_gpsCtx.gpsMultiSensorCommTimeoutEvt = NULL;
    return -1;
}

static void gpMultiSensorCommTimeoutStop(void)
{
    //printf("gpMultiCommEvtStop: evt = %x\n", g_gpsCtx.gpsMultiSensorCommTimeoutEvt);

    if (g_gpsCtx.gpsMultiSensorCommTimeoutEvt) {
        TL_ZB_TIMER_CANCEL(&g_gpsCtx.gpsMultiSensorCommTimeoutEvt);

        for (u8 i = 0; i < g_gpsCtx.multiSensorCommBufSize; i++) {
            if (g_gpsCtx.pMultiSensorCommBuf[i]) {
                ev_buf_free(g_gpsCtx.pMultiSensorCommBuf[i]);
                g_gpsCtx.pMultiSensorCommBuf[i] = NULL;
            }
        }

        g_gpsCtx.multiSensorCommBufSize = 0;
        g_gpsCtx.multiSensorTotalNumReport = 0;
        g_gpsCtx.multiSensorNumOfReport = 0;
        g_gpsCtx.multiSensorCompleted = 0;
        g_gpsCtx.multiSensorReportMatched = 0;
    }
}

static void gpsCommissioningModeExit(void)
{
    gpMultiSensorCommTimeoutStop();

    g_gpsCtx.gpsInCommMode = FALSE;

    gpTxQueueFree();
}

static void gpsCommissioningWindowTimeoutStop(void)
{
    if (g_gpsCtx.gpsCommWindowTimeoutEvt) {
        TL_ZB_TIMER_CANCEL(&g_gpsCtx.gpsCommWindowTimeoutEvt);
    }
}

static s32 gpsCommissioningWindowTimeoutCb(void *arg)
{
    //printf("gpsCommWinTimeout\n");

    gpsCommissioningModeExit();

    gp_sinkTabNoCompleteEntryClear();

    gpProxyCommissioningModeCmdSend(FALSE, TRUE);

    //notify up layer the current commissioning mode
    if (gpAppCb && gpAppCb->gpCommissioningModeCb) {
        gpAppCb->gpCommissioningModeCb(g_gpsCtx.gpsInCommMode);
    }

    g_gpsCtx.gpsCommWindowTimeoutEvt = NULL;
    return -1;
}

u8 gpsCommissionModeSet(bool action, bool involveProxies)
{
    if (zclGpAttr_gpsSecLevel & GPS_INVOLVE_TC) {
        return FAILURE;
    }

    //printf("gpsCommModeSet: action = %x, involve = %x\n", action, involveProxies);

    if (action) {
        g_gpsCtx.gpsInCommMode = TRUE;

        if (zclGpAttr_gpsCommExitMode & GPS_EXIT_ON_COMM_WINDOW_EXPIRATION) {
            gpsCommissioningWindowTimeoutStop();

            if (zclGpAttr_gpsCommWindow) {
                g_gpsCtx.gpsCommWindowTimeoutEvt = TL_ZB_TIMER_SCHEDULE(gpsCommissioningWindowTimeoutCb,
                                                                        NULL,
                                                                        zclGpAttr_gpsCommWindow * 1000);
            }
        }
    } else {
        gpsCommissioningWindowTimeoutStop();
        gpsCommissioningModeExit();
        gp_sinkTabNoCompleteEntryClear();
    }

    gpProxyCommissioningModeCmdSend(action, involveProxies);

    //notify up layer the current commissioning mode
    if (gpAppCb && gpAppCb->gpCommissioningModeCb) {
        gpAppCb->gpCommissioningModeCb(g_gpsCtx.gpsInCommMode);
    }

    return SUCCESS;
}

static status_t gpSinkCommModeCmdProcess(zcl_gp_sinkCommissioningModeCmd_t *pCmd)
{
    if ((pCmd->sinkEndpoint != 0xff) && (pCmd->sinkEndpoint != g_gpsCtx.gpsAppEndpoint)) {
        return ZCL_STA_NOT_FOUND;
    }

    if ((pCmd->options.bits.involveGpmInPairing) || (pCmd->options.bits.involveGpmInSec) ||
        (pCmd->gpmAddrForSec != 0xffff) || (pCmd->gpmAddrForPairing != 0xffff)) {
        return ZCL_STA_INVALID_FIELD;
    }

    gpsCommissionModeSet(pCmd->options.bits.action, pCmd->options.bits.involveProxies);

    return ZCL_STA_SUCCESS;
}

static void gpPairingCmdSend(zcl_gp_pairingCmd_t *pCmd)
{
    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.dstEp = GREEN_POWER_ENDPOINT;
    dstEpInfo.profileId = GP_PROFILE_ID;
    dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
    dstEpInfo.dstAddr.shortAddr = NWK_BROADCAST_RX_ON_WHEN_IDLE;
    dstEpInfo.txOptions = 0;
    dstEpInfo.radius = 0;

    zcl_gp_pairingCmdSend(GREEN_POWER_ENDPOINT, &dstEpInfo, TRUE, pCmd);
}

static void gpPairingCfgCmdSend(zcl_gp_pairingConfigurationCmd_t *pCmd)
{
    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.dstEp = GREEN_POWER_ENDPOINT;
    dstEpInfo.profileId = GP_PROFILE_ID;
    dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
    dstEpInfo.dstAddr.shortAddr = NWK_BROADCAST_RX_ON_WHEN_IDLE;
    dstEpInfo.txOptions = 0;
    dstEpInfo.radius = 0;

    zcl_gp_pairingCfgCmdSend(GREEN_POWER_ENDPOINT, &dstEpInfo, TRUE, pCmd);
}

static void gpDecommissioningHandler(u8 appId, gpdId_t gpdId, u8 endpoint)
{
    gpSinkTabEntry_t *pSinkEntry = gp_getSinkTabByGpdId(appId, gpdId);
    if (!pSinkEntry) {
        return;
    }

    if ((appId == GP_APP_ID_GPD) &&
        (endpoint != pSinkEntry->endpoint) &&
        (endpoint != 0) && (endpoint != 0xFF)) {
        return;
    }

    zcl_gp_pairingCmd_t *pPairingCmd = (zcl_gp_pairingCmd_t *)ev_buf_allocate(sizeof(zcl_gp_pairingCmd_t));
    if (!pPairingCmd) {
        return;
    }

    memset((u8 *)pPairingCmd, 0, sizeof(zcl_gp_pairingCmd_t));

    pPairingCmd->options.bits.appId = appId;
    if (pPairingCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        pPairingCmd->gpdId.srcId = gpdId.srcId;
    } else if (pPairingCmd->options.bits.appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(pPairingCmd->gpdId.gpdIeeeAddr, gpdId.gpdIeeeAddr);
        pPairingCmd->endpoint = endpoint;
    }
    pPairingCmd->options.bits.removeGPD = 1;

    gpPairingCmdSend(pPairingCmd);

    ev_buf_free((u8 *)pPairingCmd);

    if (g_gpsCtx.gpsInCommMode &&
        pSinkEntry->options.bits.communicationMode == GPS_COMM_MODE_GROUP_PRE_COMMISSIONED_GROUPID) {
        zcl_gp_pairingConfigurationCmd_t *pCfgCmd = (zcl_gp_pairingConfigurationCmd_t *)ev_buf_allocate(sizeof(zcl_gp_pairingConfigurationCmd_t));
        if (!pCfgCmd) {
            return;
        }

        memset((u8 *)pCfgCmd, 0, sizeof(zcl_gp_pairingConfigurationCmd_t));

        pCfgCmd->actions.bits.action = REMOVE_GPD;
        if (pCfgCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
            pCfgCmd->gpdId.srcId = pSinkEntry->gpdId.srcId;
        } else if (pCfgCmd->options.bits.appId == GP_APP_ID_GPD) {
            ZB_64BIT_ADDR_COPY(pCfgCmd->gpdId.gpdIeeeAddr, pSinkEntry->gpdId.gpdIeeeAddr);
            pCfgCmd->endpoint = pSinkEntry->endpoint;
        }
        pCfgCmd->deviceId = pSinkEntry->deviceId;
        pCfgCmd->groupcastRadius = pSinkEntry->groupcastRadius;
        pCfgCmd->numOfPairedEndpoints = 0xFE;

        gpPairingCfgCmdSend(pCfgCmd);

        ev_buf_free((u8 *)pCfgCmd);
    }

    gp_transTabEntryRemove(appId, gpdId, endpoint);
    gp_sinkRemoveEntries(appId, gpdId, endpoint);
}

static void gpdDataHandler(u8 appId, gpdId_t gpdId, u8 endpoint,
                           u8 gpdfSecLevel, u8 gpdfSecKeyType,
                           u32 gpdSecFrameCnt, u8 gpdCmdId,
                           u8 dataLen, u8 *pData)
{
    //printf("gpdDataHandler: gpdCmdId = %x, len = %d\n", gpdCmdId, dataLen);
    if (dataLen) {
        DEBUG_ARRAY(1, pData, dataLen);
    }

    gpSinkTabEntry_t *pSinkEntry = gp_getSinkTabByGpdId(appId, gpdId);
    if (!pSinkEntry) {
        return;
    }

    if ((appId == GP_APP_ID_GPD) &&
        (endpoint != pSinkEntry->endpoint) &&
        (pSinkEntry->endpoint != 0xFF) &&
        (endpoint != 0) && (endpoint != 0xFF)) {
        return;
    }

    if (gpdCmdId < GPDF_CMD_ID_COMMISSIONING) {
        if ((gpdfSecLevel != pSinkEntry->secOptions.bits.secLevel) ||
            (gpdfSecKeyType != pSinkEntry->secOptions.bits.secKeyType)) {
            return;
        }
    }

    //update GPD security frame counter.
    if (pSinkEntry->options.bits.securityUse || pSinkEntry->options.bits.seqNumCapabilities) {
        if (pSinkEntry->options.bits.securityUse &&
           (gpdSecFrameCnt <= pSinkEntry->gpdSecFrameCnt)) {
            return;
        }

        pSinkEntry->gpdSecFrameCnt = gpdSecFrameCnt;
        gpSinkTabUpdate();
    }

    gp_gpdfTranslate(appId, gpdId, endpoint, gpdCmdId, dataLen, pData);
}

static void gpNotificationCmdProcess(zcl_gp_notificationCmd_t *pCmd)
{
    //printf("gpsRcvNoti: secFc = %x\n", pCmd->gpdSecFrameCnt);

    if (g_gpsCtx.gpsInCommMode) {
        return;
    }

    if (((pCmd->options.bits.appId == GP_APP_ID_SRC_ID) && (pCmd->gpdId.srcId == 0)) ||
        ((pCmd->options.bits.appId == GP_APP_ID_GPD) && (ZB_IEEE_ADDR_IS_ZERO(pCmd->gpdId.gpdIeeeAddr)))) {
        return;
    }

    if (gpDataTunneledDuplicateCheck(pCmd->options.bits.appId, pCmd->gpdId, pCmd->gpdSecFrameCnt)) {
        //printf("tunneledDuplicated\n");
        return;
    }

    if (pCmd->gpdCmdID == GPDF_CMD_ID_COMMISSIONING) {
        return;
    } else if (pCmd->gpdCmdID == GPDF_CMD_ID_DECOMMISSIONING) {
        gpDecommissioningHandler(pCmd->options.bits.appId, pCmd->gpdId, pCmd->endpoint);
        return;
    }

    gpdDataHandler(pCmd->options.bits.appId, pCmd->gpdId, pCmd->endpoint,
                   pCmd->options.bits.secLevel, pCmd->options.bits.secKeyType,
                   pCmd->gpdSecFrameCnt, pCmd->gpdCmdID,
                   pCmd->payloadLen, pCmd->pGpdCmdPayload);
}

static void gpResponseCmdSend(zcl_gp_responseCmd_t *pRspCmd)
{
    //printf("gpRspSend: cmdId = %x\n", pRspCmd->gpdCmdID);

    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);
    dstEpInfo.dstEp = GREEN_POWER_ENDPOINT;
    dstEpInfo.profileId = GP_PROFILE_ID;
    dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
    dstEpInfo.dstAddr.shortAddr = NWK_BROADCAST_RX_ON_WHEN_IDLE;
    dstEpInfo.txOptions = 0;
    dstEpInfo.radius = 0;

    zcl_gp_responseCmdSend(GREEN_POWER_ENDPOINT, &dstEpInfo, TRUE, pRspCmd);

    if (pRspCmd->tempMasterShortAddr == NIB_NETWORK_ADDRESS()) {
        //clear for commissioning reply.
        if (pRspCmd->gpdCmdID == GPDF_CMD_ID_COMMISSIONING_REPLY) {
            gpTxQueueMaintenceClear();
        }

        gp_data_req_t gpDataReq;
        TL_SETSTRUCTCONTENT(gpDataReq, 0);

        gpDataReq.action = TRUE;
        gpDataReq.txOptions.useGpTxQueue = 1;
        gpDataReq.appId = pRspCmd->options.bits.appId;
        memcpy((u8 *)&gpDataReq.gpdId, (u8 *)&pRspCmd->gpdId, sizeof(gpdId_t));
        gpDataReq.endpoint = pRspCmd->endpoint;
        gpDataReq.gpdCmdId = pRspCmd->gpdCmdID;
        gpDataReq.gpdAsduLen = pRspCmd->payloadLen;
        gpDataReq.gpdAsdu = pRspCmd->pGpdCmdPayload;

        //printf("GPDF_push: cmdId = %x\n", pRspCmd->gpdCmdID);

        gpTranimitGPDF(&gpDataReq);
    }
}

static s32 switchToTxChannelDelayCb(void *arg)
{
    u8 tempMasterTxChannel = (u8)((u32)arg & 0xff);

    u8 operationChannel = g_zbMacPib.phyChannelCur - TL_ZB_MAC_CHANNEL_START;

    gpSwitchToTransmitChannel(operationChannel, tempMasterTxChannel);

    return -1;
}

static void gpChannelRequestHandler(u8 appId, gpdId_t gpdId, u8 endpoint,
                                    u16 tempMasterAddr, u8 tempMasterTxChn,
                                    gpd_channelConfigurationCmd_payload_t *pChnCfg)
{
    //printf("chnReqHandler\n");

    u8 len = sizeof(zcl_gp_responseCmd_t) + sizeof(gpd_channelConfigurationCmd_payload_t);

    zcl_gp_responseCmd_t *pRsp = (zcl_gp_responseCmd_t *)ev_buf_allocate(len);
    if (!pRsp) {
        return;
    }

    memset((u8 *)pRsp, 0, len);

    pRsp->options.bits.appId = appId;
    memcpy((u8 *)&pRsp->gpdId, (u8 *)&gpdId, sizeof(gpdId_t));
    pRsp->endpoint =  endpoint;
    pRsp->tempMasterShortAddr = tempMasterAddr;
    pRsp->tempMasterTxChannel = tempMasterTxChn;
    pRsp->gpdCmdID = GPDF_CMD_ID_CHANNEL_CONFIGURATION;
    pRsp->payloadLen = sizeof(gpd_channelConfigurationCmd_payload_t);
    pRsp->pGpdCmdPayload = (u8 *)pRsp + sizeof(zcl_gp_responseCmd_t);
    memcpy(pRsp->pGpdCmdPayload, (u8 *)&pChnCfg->channel, sizeof(gpd_channelConfigurationCmd_payload_t));

    gpResponseCmdSend(pRsp);

    ev_buf_free((u8*)pRsp);

    if (tempMasterAddr == NIB_NETWORK_ADDRESS()) {
        //wait broadcast send, switch to tx channel.
        u8 gppTunnelingDelay = gppTunnelingDelayGet(TRUE, 32, TRUE, FALSE);
        u32 txChn = (u32)tempMasterTxChn;
        //printf("delay: %d\n", gppTunnelingDelay);
        TL_ZB_TIMER_SCHEDULE(switchToTxChannelDelayCb, (void *)txChn, gppTunnelingDelay);
    }
}

static void gpChannelReqCmdHandler(zcl_gp_commissioningNotificationCmd_t *pCmd)
{
    if ((!g_gpsCtx.gpsInCommMode) || (!pCmd->options.bits.rxAfterTx)) {
        return;
    }

    gpd_channelRequestCmd_payload_t chnReq;
    TL_SETSTRUCTCONTENT(chnReq, 0);
    chnReq.chTogglingBehavior = pCmd->pGpdCmdPayload[0];

    gpd_channelConfigurationCmd_payload_t chnCfg;
    TL_SETSTRUCTCONTENT(chnCfg, 0);
    chnCfg.bits.operationChannel = g_zbMacPib.phyChannelCur - TL_ZB_MAC_CHANNEL_START;
    chnCfg.bits.basic = (!pCmd->options.bits.bidirectionalCap) ? 1 : 0;

    gpChannelRequestHandler(pCmd->options.bits.appId, pCmd->gpdId, pCmd->endpoint,
                            pCmd->gppShortAddr, chnReq.bits.rxChNext, &chnCfg);
}

static u8 gpCommissioningParse(gpd_commissioning_payload_t *pCmd, u8 *gpdPayload)
{
    u8 *pBuf = gpdPayload;

    pCmd->deviceId = *pBuf++;
    pCmd->options.opts = *pBuf++;
    if (pCmd->options.bits.extOpPresent) {
        pCmd->extOptions.opts = *pBuf++;
    }
    if (pCmd->extOptions.bits.gpdKeyPresent) {
        memcpy(pCmd->gpdSecKey, pBuf, 16);
        pBuf += 16;
    }
    if (pCmd->extOptions.bits.gpdKeyEncrypt) {
        pCmd->gpdMIC = BUILD_U32(pBuf[0], pBuf[1], pBuf[2], pBuf[3]);
        pBuf += 4;
    }
    if (pCmd->extOptions.bits.outgoingCounterPresent) {
        pCmd->gpdOutGoingCounter = BUILD_U32(pBuf[0], pBuf[1], pBuf[2], pBuf[3]);
        pBuf += 4;
    }
    if (pCmd->options.bits.appInfoPresent) {
        pCmd->appInfo.info = *pBuf++;
    }
    if (pCmd->appInfo.bits.manuIdPresent) {
        pCmd->manuId = BUILD_U16(pBuf[0], pBuf[1]);
        pBuf += 2;
    }
    if (pCmd->appInfo.bits.modelIdPresent) {
        pCmd->modelId = BUILD_U16(pBuf[0], pBuf[1]);
        pBuf += 2;
    }
    if (pCmd->appInfo.bits.gpdCmdsPresent) {
        pCmd->gpdCmdNum = *pBuf++;
        pCmd->commandList = pBuf;
        pBuf += pCmd->gpdCmdNum;
    }
    if (pCmd->appInfo.bits.clusterListPresent) {
        pCmd->clusterList = pBuf;
        pBuf += (((*pBuf) & 0x0f) + ((*pBuf) & 0xf0)) * sizeof(u16) + 1;
    }
    if (pCmd->appInfo.bits.switchInfoPresent) {
        pCmd->switchInfo.switchInfoLen = *pBuf++;
        pCmd->switchInfo.switchCfg.cfg = *pBuf++;
        pCmd->switchInfo.contactStatus = *pBuf++;
    }

    return (pBuf - gpdPayload);
}

static gpSinkTabEntry_t *gpSinkTableEntryCommUpdate(u8 appId, gpdId_t gpdId, u8 endpoint,
                                                    bool rxAfterTx, u32 gpdSecFrameCnt,
                                                    gpd_commissioning_payload_t *pCommissioning)
{
    gpSinkTabEntry_t *pSinkEntry = NULL;

    u8 sinkAttrMinSecLevel = zclGpAttr_gpsSecLevel & 0x03;
    u8 protectWithGpLinkkey = zclGpAttr_gpsSecLevel & GPS_PROTECTION_WITH_GPLINKKEY;

    if (pCommissioning->options.bits.extOpPresent) {
        if ((pCommissioning->extOptions.bits.secLevelCap == GP_SEC_LEVEL_RESERVED) ||
            (pCommissioning->extOptions.bits.secLevelCap < sinkAttrMinSecLevel)) {
            return NULL;
        }
    }

    if (protectWithGpLinkkey && !pCommissioning->extOptions.bits.gpdKeyEncrypt) {
        return NULL;
    }

    if ((pCommissioning->extOptions.bits.secLevelCap) &&
       (!pCommissioning->extOptions.bits.gpdKeyPresent &&
       (!rxAfterTx || !pCommissioning->options.bits.gpSecKeyReq))) {
        return NULL;
    }

    if (pCommissioning->extOptions.bits.gpdKeyEncrypt && pCommissioning->extOptions.bits.gpdKeyPresent) {
        if (SUCCESS != gpdKeyWithTCLK(appId, gpdId,
                                      SEC_KEY_LEN, pCommissioning->gpdSecKey,
                                      pCommissioning->gpdOutGoingCounter, (u8 *)&pCommissioning->gpdMIC,
                                      zclGpAttr_gpLinkKey, FALSE))
        {
            //printf("gpdKeyWithTCLK FAIL.\n");
            return NULL;
        }
    }

    pSinkEntry = gp_getSinkTabByGpdId(appId, gpdId);
    if (pSinkEntry) {
        if (appId == GP_APP_ID_GPD) {
            if ((endpoint != pSinkEntry->endpoint) &&
                (endpoint != 0) && (endpoint != 0xff)) {
                pSinkEntry = NULL;
            }
        }
    }
    if (!pSinkEntry) {
        pSinkEntry = gp_sinkTabEntryFreeGet();
        if (!pSinkEntry) {
            return NULL;
        }
    }

    //new entry.
    if (!pSinkEntry->used) {
        pSinkEntry->used = 1;
        g_gpSinkTab.gpSinkTabNum++;
    }

    pSinkEntry->options.bits.appId = appId;
    pSinkEntry->options.bits.communicationMode = zclGpAttr_gpsCommMode & 0x03;
    pSinkEntry->options.bits.seqNumCapabilities = pCommissioning->options.bits.macSeqNumCap;
    pSinkEntry->options.bits.rxOnCapability = pCommissioning->options.bits.rxOnCap;
    pSinkEntry->options.bits.fixedLocation = pCommissioning->options.bits.fixedLocation;
    pSinkEntry->options.bits.securityUse = pCommissioning->extOptions.bits.gpdKeyPresent ? 1 : 0;

    if (appId == GP_APP_ID_SRC_ID) {
        pSinkEntry->gpdId.srcId = gpdId.srcId;
    } else if (appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(pSinkEntry->gpdId.gpdIeeeAddr, gpdId.gpdIeeeAddr);
        pSinkEntry->endpoint = endpoint;
    }

    pSinkEntry->deviceId = pCommissioning->deviceId;
    pSinkEntry->groupcastRadius = 0;

    pSinkEntry->secOptions.bits.secKeyType = pCommissioning->extOptions.bits.keyType;
    pSinkEntry->secOptions.bits.secLevel = pCommissioning->extOptions.bits.secLevelCap;

    if (pSinkEntry->options.bits.securityUse) {
        memcpy(pSinkEntry->gpdKey, pCommissioning->gpdSecKey, SEC_KEY_LEN);
    }

    pSinkEntry->replyIncludeKey = 0;
    pSinkEntry->replyEncrypt = 0;
    if ((pCommissioning->extOptions.bits.keyType == GP_SEC_KEY_TYPE_OUT_OF_THE_BOX_GPD_KEY) &&
       (pCommissioning->options.bits.gpSecKeyReq) &&
       (rxAfterTx || pCommissioning->appInfo.bits.gpdAppDescCmdFollows)) {
        if ((zclGpAttr_gpSharedSecKeyType != GP_SEC_KEY_TYPE_NO_KEY) &&
           (!ZB_SEC_KEY_IS_NULL(zclGpAttr_gpSharedSecKey)) &&
           ((pCommissioning->extOptions.bits.keyType != zclGpAttr_gpSharedSecKeyType) ||
            (memcmp(pCommissioning->gpdSecKey, zclGpAttr_gpSharedSecKey, SEC_KEY_LEN)))) {
            switch (zclGpAttr_gpSharedSecKeyType) {
            case GP_SEC_KEY_TYPE_NWK_KEY:
                pSinkEntry->replyIncludeKey = TRUE;
                pSinkEntry->replyEncrypt = pCommissioning->extOptions.bits.gpdKeyEncrypt;
                pSinkEntry->secOptions.bits.secKeyType = GP_SEC_KEY_TYPE_NWK_KEY;
                memcpy(pSinkEntry->gpdKey, ss_ib.nwkSecurMaterialSet[ss_ib.activeSecureMaterialIndex].key, SEC_KEY_LEN);
                break;
            case GP_SEC_KEY_TYPE_GPD_GROUP_KEY:
                pSinkEntry->replyIncludeKey = TRUE;
                pSinkEntry->replyEncrypt = pCommissioning->extOptions.bits.gpdKeyEncrypt;
                pSinkEntry->secOptions.bits.secKeyType = GP_SEC_KEY_TYPE_GPD_GROUP_KEY;
                memcpy(pSinkEntry->gpdKey, zclGpAttr_gpSharedSecKey, SEC_KEY_LEN);
                break;
            default:
                break;
            }
        }
    }

    if (pCommissioning->extOptions.bits.outgoingCounterPresent) {
        pSinkEntry->gpdSecFrameCnt = pCommissioning->gpdOutGoingCounter;
    } else {
        pSinkEntry->gpdSecFrameCnt = gpdSecFrameCnt;
    }

    return pSinkEntry;
}

static void gpAppDescFollowHandler(gpSinkTabEntry_t *pSinkEntry)
{
    if (!g_gpsCtx.gpsMultiSensorCommTimeoutEvt) {
        g_gpsCtx.gpsMultiSensorCommTimeoutEvt = TL_ZB_TIMER_SCHEDULE(gpMultiSensorCommTimeoutCb,
                                                                     (void *)pSinkEntry,
                                                                     g_gpsCtx.multiSensorCommTime * 1000);

        //printf("gpMultiCommEvtCreat: evt = %x\n", g_gpsCtx.gpsMultiSensorCommTimeoutEvt);
    } else {
        ev_on_timer(g_gpsCtx.gpsMultiSensorCommTimeoutEvt, g_gpsCtx.multiSensorCommTime * 1000);

        //printf("gpMultiCommEvtChk: evt = %x\n", g_gpsCtx.gpsMultiSensorCommTimeoutEvt);
    }
}

static u8 gpCommissioningReplyCmdLenGet(gpd_commissioningReply_payload_t *pCmd)
{
    u8 len = 1;

    if (pCmd->options.bits.panIdPresent) {
        len += 2;
    }
    if (pCmd->options.bits.gpdSecKeyPresent) {
        len += SEC_KEY_LEN;

        if (pCmd->options.bits.gpdKeyEncrypt) {
            len += 4;

            if ((pCmd->options.bits.securityLevel == GP_SEC_LEVEL_4BFC_4BMIC) ||
                (pCmd->options.bits.securityLevel == GP_SEC_LEVEL_4BFC_4BMIC_ENCRYPTION)) {
                len += 4;
            }
        }
    }

    return len;
}

static void gpCommissioningReplyFormatBuild(gpd_commissioningReply_payload_t *pCmd, u8 *pBuf)
{
    u8 *ptr = pBuf;

    *ptr++ = pCmd->options.opts;

    if (pCmd->options.bits.panIdPresent) {
        *ptr++ = LO_UINT16(pCmd->panId);
        *ptr++ = HI_UINT16(pCmd->panId);
    }
    if (pCmd->options.bits.gpdSecKeyPresent) {
        memcpy(ptr, pCmd->key, SEC_KEY_LEN);
        ptr += SEC_KEY_LEN;

        if (pCmd->options.bits.gpdKeyEncrypt) {
            *ptr++ = U32_BYTE0(pCmd->keyMIC);
            *ptr++ = U32_BYTE1(pCmd->keyMIC);
            *ptr++ = U32_BYTE2(pCmd->keyMIC);
            *ptr++ = U32_BYTE3(pCmd->keyMIC);

            if ((pCmd->options.bits.securityLevel == GP_SEC_LEVEL_4BFC_4BMIC) ||
                (pCmd->options.bits.securityLevel == GP_SEC_LEVEL_4BFC_4BMIC_ENCRYPTION)) {
                *ptr++ = U32_BYTE0(pCmd->frameCounter);
                *ptr++ = U32_BYTE1(pCmd->frameCounter);
                *ptr++ = U32_BYTE2(pCmd->frameCounter);
                *ptr++ = U32_BYTE3(pCmd->frameCounter);
            }
        }
    }
}

static void gpCommissioningReplyCmdSend(gpSinkTabEntry_t *pSinkEntry, u16 gppShortAddr)
{
    gpd_commissioningReply_payload_t reply;
    TL_SETSTRUCTCONTENT(reply, 0);

    reply.options.bits.keyType = pSinkEntry->secOptions.bits.secKeyType;
    reply.options.bits.securityLevel = pSinkEntry->secOptions.bits.secLevel;
    reply.options.bits.panIdPresent = 0;
    reply.options.bits.gpdSecKeyPresent = pSinkEntry->replyIncludeKey;
    reply.options.bits.gpdKeyEncrypt = pSinkEntry->replyEncrypt;

    if (pSinkEntry->replyIncludeKey) {
        memcpy(reply.key, pSinkEntry->gpdKey, SEC_KEY_LEN);

        if (pSinkEntry->replyEncrypt &&
           ((pSinkEntry->secOptions.bits.secLevel == GP_SEC_LEVEL_4BFC_4BMIC) ||
            (pSinkEntry->secOptions.bits.secLevel == GP_SEC_LEVEL_4BFC_4BMIC_ENCRYPTION))) {
            reply.frameCounter = pSinkEntry->gpdSecFrameCnt + 1;

            gpdKeyWithTCLK(pSinkEntry->options.bits.appId, pSinkEntry->gpdId,
                           SEC_KEY_LEN, reply.key,
                           reply.frameCounter, (u8 *)&reply.keyMIC,
                           zclGpAttr_gpLinkKey, TRUE);
        }
    }

    u8 payloadLen = gpCommissioningReplyCmdLenGet(&reply);

    zcl_gp_responseCmd_t *pRsp = (zcl_gp_responseCmd_t *)ev_buf_allocate(sizeof(zcl_gp_responseCmd_t) + payloadLen);
    if (!pRsp) {
        return;
    }

    memset(pRsp, 0, sizeof(zcl_gp_responseCmd_t) + payloadLen);

    pRsp->options.bits.appId = pSinkEntry->options.bits.appId;
    memcpy((u8 *)&pRsp->gpdId, (u8 *)&pSinkEntry->gpdId, sizeof(gpdId_t));
    pRsp->endpoint = pSinkEntry->endpoint;
    pRsp->tempMasterShortAddr = gppShortAddr;
    pRsp->tempMasterTxChannel = rf_getChannel() - TL_ZB_MAC_CHANNEL_START;
    pRsp->gpdCmdID = GPDF_CMD_ID_COMMISSIONING_REPLY;
    pRsp->payloadLen = payloadLen;
    pRsp->pGpdCmdPayload = (u8 *)pRsp + sizeof(zcl_gp_responseCmd_t);
    gpCommissioningReplyFormatBuild(&reply, pRsp->pGpdCmdPayload);

    gpResponseCmdSend(pRsp);

    ev_buf_free((u8*)pRsp);
}

static void gpPairingAddSink(gpSinkTabEntry_t *pSinkEntry)
{
    zcl_gp_pairingCmd_t *pCmd = (zcl_gp_pairingCmd_t *)ev_buf_allocate(sizeof(zcl_gp_pairingCmd_t));
    if (!pCmd) {
        return;
    }

    memset((u8 *)pCmd, 0, sizeof(zcl_gp_pairingCmd_t));

    pCmd->options.bits.appId = pSinkEntry->options.bits.appId;
    pCmd->options.bits.addSink = 1;
    pCmd->options.bits.commMode = pSinkEntry->options.bits.communicationMode;
    pCmd->options.bits.gpdFixed = pSinkEntry->options.bits.fixedLocation;
    pCmd->options.bits.gpdMacSeqNumCap = pSinkEntry->options.bits.seqNumCapabilities;
    pCmd->options.bits.secLevel = pSinkEntry->secOptions.bits.secLevel;
    pCmd->options.bits.secKeyType = pSinkEntry->secOptions.bits.secKeyType;
    pCmd->options.bits.assignedAliasPresent = pSinkEntry->options.bits.assignedAlias;
    pCmd->options.bits.groupcastRadiusPresent = (pSinkEntry->groupcastRadius == 0xff) ? 0 : 1;

    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        pCmd->gpdId.srcId = pSinkEntry->gpdId.srcId;
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(pCmd->gpdId.gpdIeeeAddr, pSinkEntry->gpdId.gpdIeeeAddr);
        pCmd->endpoint = pSinkEntry->endpoint;
    }

    if (pCmd->options.bits.assignedAliasPresent) {
        pCmd->assignedAlias = pSinkEntry->gpdAssignedAlias;
    }

    if (pCmd->options.bits.groupcastRadiusPresent) {
        pCmd->groupcastRadius = pSinkEntry->groupcastRadius;
    }

    pCmd->deviceID = pSinkEntry->deviceId;

    if ((pCmd->options.bits.secLevel == GP_SEC_LEVEL_NO_SECURITY) &&
       (!pCmd->options.bits.gpdMacSeqNumCap)) {
        pCmd->options.bits.gpdSecFrameCntPresent = 0;
    } else {
        pCmd->options.bits.gpdSecFrameCntPresent = 1;
        pCmd->gpdSecFrameCnt = pSinkEntry->gpdSecFrameCnt;

        if ((pCmd->options.bits.secLevel == GP_SEC_LEVEL_4BFC_4BMIC) ||
            (pCmd->options.bits.secLevel == GP_SEC_LEVEL_4BFC_4BMIC_ENCRYPTION)) {
            pCmd->options.bits.gpdSecKeyPresent = 1;
            memcpy(pCmd->gpdKey, pSinkEntry->gpdKey, SEC_KEY_LEN);
        }
    }

    if ((pCmd->options.bits.commMode == GPS_COMM_MODE_FULL_UNICAST) ||
        (pCmd->options.bits.commMode == GPS_COMM_MODE_LIGHTWEIGHT_UNICAST)) {
        pCmd->sinkNwkAddr = NIB_NETWORK_ADDRESS();
        ZB_64BIT_ADDR_COPY(pCmd->sinkIeeeAddr, NIB_IEEE_ADDRESS());

        gpPairingCmdSend(pCmd);
    } else if (pCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_DGROUPID) {
        pCmd->sinkGroupID = gpAliasSrcAddrDerived(pCmd->options.bits.appId, pCmd->gpdId);

        gpPairingCmdSend(pCmd);
    } else if (pCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_PRE_COMMISSIONED_GROUPID) {
        for (u8 i = 0; i < pSinkEntry->sinkGroupListCnt; i++) {
            pCmd->sinkGroupID = pSinkEntry->groupList[i].groupId;

            gpPairingCmdSend(pCmd);
        }
    }

    ev_buf_free((u8 *)pCmd);
}

static void gpPairingCfgAddSink(gpSinkTabEntry_t *pSinkEntry)
{
    if (!pSinkEntry->sinkGroupListCnt) {
        return;
    }

    u8 cmdLen = sizeof(zcl_gp_pairingConfigurationCmd_t);
    u8 groupListLen = pSinkEntry->sinkGroupListCnt * sizeof(gpSinkGroupListItem_t) + 1;

    cmdLen += groupListLen;

    zcl_gp_pairingConfigurationCmd_t *pCmd = (zcl_gp_pairingConfigurationCmd_t *)ev_buf_allocate(sizeof(zcl_gp_pairingConfigurationCmd_t) + groupListLen);
    if (!pCmd) {
        return;
    }

    memset((u8 *)pCmd, 0, cmdLen);

    pCmd->pGroupList = (u8 *)pCmd + sizeof(zcl_gp_pairingConfigurationCmd_t);

    pCmd->actions.bits.action = EXTEND_SINK_TABLE_ENTRY;
    pCmd->actions.bits.sendGpPairing = 0;

    pCmd->options.opts = pSinkEntry->options.opts;
    pCmd->options.bits.appInfoPresent = 1;

    if (pSinkEntry->options.bits.appId == GP_APP_ID_SRC_ID) {
        pCmd->gpdId.srcId = pSinkEntry->gpdId.srcId;
    } else if (pSinkEntry->options.bits.appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(pCmd->gpdId.gpdIeeeAddr, pSinkEntry->gpdId.gpdIeeeAddr);
        pCmd->endpoint = pSinkEntry->endpoint;
    }

    pCmd->deviceId = pSinkEntry->deviceId;

    pCmd->pGroupList[0] = pSinkEntry->sinkGroupListCnt;
    if (pCmd->pGroupList[0]) {
        u8 *pData = &pCmd->pGroupList[1];
        for (u8 i = 0; i < pCmd->pGroupList[0]; i++) {
            *pData++ = LO_UINT16(pSinkEntry->groupList[i].groupId);
            *pData++ = HI_UINT16(pSinkEntry->groupList[i].groupId);
            *pData++ = LO_UINT16(pSinkEntry->groupList[i].alias);
            *pData++ = HI_UINT16(pSinkEntry->groupList[i].alias);
        }
    }

    if (pCmd->options.bits.assignedAlias) {
        pCmd->gpdAssignedAlias = pSinkEntry->gpdAssignedAlias;
    }

    pCmd->groupcastRadius = pSinkEntry->groupcastRadius;

    if (pCmd->options.bits.secUse) {
        pCmd->secOptions.opts = pSinkEntry->secOptions.opts;
        pCmd->gpdSecFrameCnt = pSinkEntry->gpdSecFrameCnt;
        memcpy(pCmd->gpdSecKey, pSinkEntry->gpdKey, SEC_KEY_LEN);
    }

    pCmd->numOfPairedEndpoints = 0xFE;

    if (pSinkEntry->deviceId == GPD_DEV_ID_GENERIC_8_CONTACT_SWITCH) {
        transTabEntry_t *pTransEntry = gp_transTabEntryFind(pSinkEntry->options.bits.appId, pSinkEntry->gpdId, pSinkEntry->endpoint,
                                                            GPDF_CMD_ID_8BIT_VECTOR_PRESS, 0xFF,
                                                            0xFFFF, HA_PROFILE_ID,
                                                            0xFF, 0xFFFF);
        if (pTransEntry && pTransEntry->optRecordUsed) {
            pCmd->appInfo.bits.switchInfoPresent = 1;

            pCmd->switchInfo.switchInfoLen = 2;
            pCmd->switchInfo.switchCfg.cfg = pTransEntry->optRecord.vectorCmd.switchCfg.cfg;
            pCmd->switchInfo.contactStatus = pTransEntry->optRecord.vectorCmd.cmd.contactStatus;
        }
    }

    if (g_gpsCtx.gpsMultiSensorCommTimeoutEvt) {
        pCmd->appInfo.bits.gpdAppDescCmdFollows = 1;
    }

    gpPairingCfgCmdSend(pCmd);

    ev_buf_free((u8 *)pCmd);
}

static void gpPairingCfgAppDesc(gpSinkTabEntry_t *pSinkEntry)
{
    if (!g_gpsCtx.multiSensorCommBufSize) {
        return;
    }

    zcl_gp_pairingConfigurationCmd_t *pCmd = (zcl_gp_pairingConfigurationCmd_t *)ev_buf_allocate(sizeof(zcl_gp_pairingConfigurationCmd_t));
    if (!pCmd) {
        return;
    }

    memset((u8 *)pCmd, 0, sizeof(zcl_gp_pairingConfigurationCmd_t));

    pCmd->actions.bits.action = APPLICATION_DESCRIPTION;
    pCmd->actions.bits.sendGpPairing = 0;

    pCmd->options.bits.appInfoPresent = 1;
    pCmd->options.bits.appId = pSinkEntry->options.bits.appId;

    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        pCmd->gpdId.srcId = pSinkEntry->gpdId.srcId;
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(pCmd->gpdId.gpdIeeeAddr, pSinkEntry->gpdId.gpdIeeeAddr);
        pCmd->endpoint = pSinkEntry->endpoint;
    }

    pCmd->deviceId = pSinkEntry->deviceId;

    pCmd->groupcastRadius = pSinkEntry->groupcastRadius;

    pCmd->numOfPairedEndpoints = 0xFE;

    pCmd->appInfo.info = 0;

    for (u8 i = 0; i < g_gpsCtx.multiSensorCommBufSize; i++) {
        u8 *pBuf = g_gpsCtx.pMultiSensorCommBuf[i];
        if (pBuf) {
            pCmd->reportDescLen = pBuf[0];
            pCmd->pReportDescriptor = &pBuf[1];
        }

        gpPairingCfgCmdSend(pCmd);

        if (pBuf) {
            ev_buf_free(pBuf);
            g_gpsCtx.pMultiSensorCommBuf[i] = NULL;
        }
    }

    ev_buf_free((u8 *)pCmd);
}

static void gpCommissioningFinalization(u8 appId, gpdId_t gpdId, u8 endpoint)
{
    gpSinkTabEntry_t *pSinkEntry = gp_getSinkTabByGpdId(appId, gpdId);
    if (!pSinkEntry) {
        return;
    }

    aps_add_group_req_t addGroupReq;
    TL_SETSTRUCTCONTENT(addGroupReq, 0);

    if (pSinkEntry->options.bits.communicationMode == GPS_COMM_MODE_GROUP_DGROUPID) {
        addGroupReq.ep = GREEN_POWER_ENDPOINT;
        addGroupReq.group_addr = gpAliasSrcAddrDerived(appId, gpdId);

        //printf("addGrp: ep = %x, grpId = %x\n", addGroupReq.ep, addGroupReq.group_addr);

        aps_me_group_add_req(&addGroupReq);
    } else if (pSinkEntry->options.bits.communicationMode == GPS_COMM_MODE_GROUP_PRE_COMMISSIONED_GROUPID) {
        gp_sinkGroupListAdd(pSinkEntry, g_gpsCtx.gpsAppEndpoint);

        //TODO: add group?
        for (u8 i = 0; i < pSinkEntry->sinkGroupListCnt; i++) {
            addGroupReq.ep = GREEN_POWER_ENDPOINT;
            addGroupReq.group_addr = pSinkEntry->groupList[i].groupId;

            aps_me_group_add_req(&addGroupReq);
        }
    }

    pSinkEntry->complete = 1;

    //store sink table.
    gpSinkTabUpdate();
    gp_transTabNvUpdate();

    //TODO: translation table.

    /*
     * When creating a pairing for a GPD supporting generic switch functionality (GPD CommandID 0x69 and/or 0x6a),
     * the sink SHOULD only send Device_annce when creating the Sink Table entry for a particular GPD (i.e. upon
     * successful commissioning of the first button of that GPD); it SHOULD NOT send the Device_annce upon successful
     * subsequent commissioning of the same GPD (i.e. when the Sink Table entry already exists), irrespective of whether
     * the subsequent commissioning procedure immediately follows the first commissioning exchange or the subsequent
     * commissioning is independently triggered.
     */
    if (pSinkEntry->options.bits.communicationMode != GPS_COMM_MODE_LIGHTWEIGHT_UNICAST) {
        u16 addrAlias = gpAliasSrcAddrDerived(appId, gpdId);
        gpDevAnnceAliasSend(addrAlias);
    }

    //send pairing command.
    gpPairingAddSink(pSinkEntry);

    if (pSinkEntry->options.bits.communicationMode == GPS_COMM_MODE_GROUP_PRE_COMMISSIONED_GROUPID) {
        gpPairingCfgAddSink(pSinkEntry);
        gpPairingCfgAppDesc(pSinkEntry);
    }

    gpMultiSensorCommTimeoutStop();

    //exit commissioning mode.
    if (zclGpAttr_gpsCommExitMode & GPS_EXIT_ON_FIRST_PAIRING_SUCCESS) {
        gpsCommissionModeSet(FALSE, TRUE);
    }
}

static void gpCommissioningHandler(u8 appId, gpdId_t gpdId, u8 endpoint,
                                   u8 gpdfSecKeyType, u8 gpdfSecLevel,
                                   u32 gpdSecFrameCnt, bool rxAfterTx,
                                   u16 tempMasterAddr, gpd_commissioning_payload_t *pCommissioning)
{
    //printf("gpCommissionHandler: gpdfKeyType = %x, gpdfSecLvl = %x, gpdfSecFC = %x, rx = %x, masterAddr = %x\n",
    //       gpdfSecKeyType, gpdfSecLevel, gpdSecFrameCnt, rxAfterTx, tempMasterAddr);

    if (!g_gpsCtx.gpsInCommMode) {
        return;
    }

    if (((appId == GP_APP_ID_SRC_ID) && (gpdId.srcId == 0)) ||
        ((appId == GP_APP_ID_GPD) && ZB_IS_64BIT_ADDR_ZERO(gpdId.gpdIeeeAddr))) {
        return;
    }

    if (gpdfSecLevel || (gpdfSecKeyType != GP_SEC_KEY_TYPE_NO_KEY)) {
        return;
    }

    gpSinkTabEntry_t *pSinkEntry = gpSinkTableEntryCommUpdate(appId, gpdId, endpoint,
                                                              rxAfterTx, gpdSecFrameCnt,
                                                              pCommissioning);

    if (!pSinkEntry) {
        return;
    }

    //printf("devId = %x\n", pCommissioning->deviceId);

    gp_transTabUpdateWithCfg(appId, gpdId, endpoint,
                             pCommissioning->deviceId, pCommissioning->gpdCmdNum,
                             pCommissioning->commandList, pCommissioning->clusterList);

    if (pCommissioning->options.bits.appInfoPresent) {
        if (pCommissioning->appInfo.bits.switchInfoPresent &&
           (pCommissioning->deviceId == GPD_DEV_ID_GENERIC_8_CONTACT_SWITCH)) {
            gp_transTabGenSwitchUpdate(appId, gpdId, endpoint, &pCommissioning->switchInfo);
        }
        if (pCommissioning->appInfo.bits.gpdAppDescCmdFollows) {
            gpAppDescFollowHandler(pSinkEntry);
            return;
        }
    }

    if (rxAfterTx) {
        gpCommissioningReplyCmdSend(pSinkEntry, tempMasterAddr);
    } else {
        gpCommissioningFinalization(appId, gpdId, endpoint);
    }
}

static void gpCommissioningCmdHandler(zcl_gp_commissioningNotificationCmd_t *pCmd)
{
    if (pCmd->options.bits.secPrcFailed) {
        return;
    }

    gpd_commissioning_payload_t commissioning;
    TL_SETSTRUCTCONTENT(commissioning, 0);

    commissioning.payloadLen = gpCommissioningParse(&commissioning, pCmd->pGpdCmdPayload);

    gpCommissioningHandler(pCmd->options.bits.appId, pCmd->gpdId, pCmd->endpoint,
                           pCmd->options.bits.secKeyType, pCmd->options.bits.secLevel,
                           pCmd->gpdSecFrameCnt, pCmd->options.bits.rxAfterTx,
                           pCmd->gppShortAddr, &commissioning);
}

static void gpSuccessHandler(u8 appId, gpdId_t gpdId, u8 endpoint,
                             u8 gpdfSecLevel, u8 gpdfSecKeyType, u32 gpdSecFrameCnt)
{
    //printf("gpSuccessHandler: level = %x, keyType = %x, fc = %x\n",
    //       gpdfSecLevel, gpdfSecKeyType, gpdSecFrameCnt);

    gpSinkTabEntry_t *pSinkEntry = gp_getSinkTabByGpdId(appId, gpdId);
    if (!pSinkEntry) {
        return;
    }

    //printf("pSinkEntry: %x\n", pSinkEntry);

    if ((appId == GP_APP_ID_GPD) &&
        (pSinkEntry->endpoint != 0xff) &&
        (pSinkEntry->endpoint != endpoint)) {
        return;
    }

    if ((gpdfSecLevel != pSinkEntry->secOptions.bits.secLevel) ||
        (gpdfSecKeyType != pSinkEntry->secOptions.bits.secKeyType) ||
        (gpdSecFrameCnt <= pSinkEntry->gpdSecFrameCnt)) {
        //printf("FAIL: secLvl = %x, secType = %x, secFc = %x\n",
                //pSinkEntry->secOptions.bits.secLevel,
                //pSinkEntry->secOptions.bits.secKeyType,
                //pSinkEntry->gpdSecFrameCnt);
        return;
    }

    //update security frame counter
    pSinkEntry->gpdSecFrameCnt = gpdSecFrameCnt;

    if (!g_gpsCtx.gpsMultiSensorCommTimeoutEvt || g_gpsCtx.multiSensorCompleted) {
        gpCommissioningFinalization(appId, gpdId, endpoint);
    }
}

static void gpSuccessCmdHandler(zcl_gp_commissioningNotificationCmd_t *pCmd)
{
    if (!g_gpsCtx.gpsInCommMode) {
        return;
    }

    if (((pCmd->options.bits.appId == GP_APP_ID_SRC_ID) && (pCmd->gpdId.srcId == 0)) ||
        ((pCmd->options.bits.appId == GP_APP_ID_GPD) && ZB_IS_64BIT_ADDR_ZERO(pCmd->gpdId.gpdIeeeAddr))) {
        return;
    }

    if (pCmd->options.bits.secPrcFailed) {
        return;
    }

    gpSuccessHandler(pCmd->options.bits.appId, pCmd->gpdId, pCmd->endpoint,
                     pCmd->options.bits.secLevel, pCmd->options.bits.secKeyType,
                     pCmd->gpdSecFrameCnt);
}

static void gpDecommissioningCmdHandler(zcl_gp_commissioningNotificationCmd_t *pCmd)
{
    gpDecommissioningHandler(pCmd->options.bits.appId, pCmd->gpdId, pCmd->endpoint);
}

static void gpAppDescHandler(u8 appId, gpdId_t gpdId, u8 endpoint,
                             bool rxAfterTx, u16 tempMasterAddr,
                             u8 appDescLen, u8 *pAppDesc, bool wait)
{
    //printf("appDescHandler: descLen = %d, rxAfterTx = %x, masterAddr = %x\n",
    //       appDescLen, rxAfterTx, tempMasterAddr);

    if (!g_gpsCtx.gpsMultiSensorCommTimeoutEvt) {
        return;
    }

    gpSinkTabEntry_t *pSinkEntry = gp_getSinkTabByGpdId(appId, gpdId);
    if (!pSinkEntry) {
        return;
    }

    if ((appId == GP_APP_ID_GPD) &&
        (endpoint != pSinkEntry->endpoint) &&
        (pSinkEntry->endpoint != 0xFF) &&
        (endpoint != 0) && (endpoint != 0xFF)) {
        return;
    }

    u8 *pData = pAppDesc;

    gpd_appDesc_payload_t appDesc;

    appDesc.totalNumReport = *pData++;
    appDesc.numOfReport = *pData++;

    if (!appDesc.totalNumReport || !appDesc.numOfReport) {
        return;
    }

    bool lastFrame = wait ? FALSE : TRUE;

    for (u8 i = 0; i < appDesc.numOfReport; i++) {
        gpReportDesc_t reportDesc;
        TL_SETSTRUCTCONTENT(reportDesc, 0);

        reportDesc.reportId = *pData++;
        reportDesc.reportOpt.opts = *pData++;
        if (reportDesc.reportOpt.bits.timeoutPeriodPresent) {
            reportDesc.timeoutPeriod = BUILD_U16(pData[0], pData[1]);
            pData += 2;
        }
        reportDesc.remainingLen = *pData++;

        //printf("reportId = %x, descRemainLen = %d\n", reportDesc.reportId, reportDesc.remainingLen);

        //the last appDesc frame
        if (reportDesc.reportId == (appDesc.totalNumReport - 1)) {
            lastFrame = TRUE;
        }

        if (reportDesc.remainingLen) {
            u8 *ptr = NULL;
            s32 remainLen = reportDesc.remainingLen;

            do {
                ptr = pData;

                gpDataPointDesc_t dataPoint;
                TL_SETSTRUCTCONTENT(dataPoint, 0);

                dataPoint.dataPointOpt.opts = *pData++;
                dataPoint.clusterId = BUILD_U16(pData[0], pData[1]);
                pData += 2;
                if (dataPoint.dataPointOpt.bits.manuIdPresent) {
                    dataPoint.manuId = BUILD_U16(pData[0], pData[1]);
                    pData += 2;
                }
                for (u8 j = 0; j < (dataPoint.dataPointOpt.bits.numOfRecords + 1); j++) {
                    gpAttrRecord_t attrRecord;
                    TL_SETSTRUCTCONTENT(attrRecord, 0);

                    attrRecord.attrId = BUILD_U16(pData[0], pData[1]);
                    pData += 2;
                    attrRecord.attrDataType = *pData++;
                    attrRecord.attrOption.opts = *pData++;

                    //TODO: the attrRecord.attrOption.bits.remainingLen field allows the sink
                    //		for skipping attribute records for attributeIDs it does not support.

                    if (attrRecord.attrOption.bits.reported) {
                        attrRecord.attrOffsetWithinReport = *pData++;
                    }
                    if (attrRecord.attrOption.bits.attrValuePresent) {
                        u8 valueLen = attrRecord.attrOption.bits.remainingLen + 1;
                        if (attrRecord.attrOption.bits.reported) {
                            valueLen -= 1;
                        }
                        memcpy(attrRecord.attrValue, pData, (valueLen > 8) ? 8 : valueLen);
                        pData += valueLen;
                    }

                    transTabEntry_t *pTransTabEntry = NULL;

                    pTransTabEntry = gp_transTabReportUpdate(appId, gpdId, endpoint,
                                                             reportDesc.reportId, dataPoint.dataPointOpt.bits.side,
                                                             dataPoint.clusterId, dataPoint.manuId,
                                                             &attrRecord);
                    if (pTransTabEntry) {
                        g_gpsCtx.multiSensorReportMatched = 1;
                    }
                }

                remainLen = remainLen - (pData - ptr);
                //printf("remainLen = %d\n", remainLen);
            } while (remainLen > 0);
        }
    }

    bool bufCached = FALSE;

    for (u8 i = 0; i < g_gpsCtx.multiSensorCommBufSize; i++) {
        u8 *pCachedBuf = g_gpsCtx.pMultiSensorCommBuf[i];
        if ((pCachedBuf[0] == appDescLen) && !memcmp(&pCachedBuf[1], pAppDesc, appDescLen)) {
            bufCached = TRUE;
        }
    }

    //cache multiSensorCommissioning
    //TODO: if clusterId not support, need cache?
    if (!bufCached) {
        if (g_gpsCtx.multiSensorCommBufSize < GPS_MULTI_BUF_SIZE_MAX) {
            u8 *pBuf = ev_buf_allocate(appDescLen + 1);
            if (!pBuf) {
                return;
            }

            memset(pBuf, 0, appDescLen + 1);

            pBuf[0] = appDescLen;
            memcpy(&pBuf[1], pAppDesc, appDescLen);

            g_gpsCtx.pMultiSensorCommBuf[g_gpsCtx.multiSensorCommBufSize] = pBuf;
            g_gpsCtx.multiSensorCommBufSize++;
        }

        if (g_gpsCtx.multiSensorTotalNumReport == 0) {
            g_gpsCtx.multiSensorTotalNumReport = appDesc.totalNumReport;
        }
        g_gpsCtx.multiSensorNumOfReport += appDesc.numOfReport;
    }

    if (lastFrame && (g_gpsCtx.multiSensorNumOfReport >= g_gpsCtx.multiSensorTotalNumReport)) {
        g_gpsCtx.multiSensorCompleted = 1;

        if (g_gpsCtx.multiSensorReportMatched) {
            if (rxAfterTx) {
                gpCommissioningReplyCmdSend(pSinkEntry, tempMasterAddr);
            } else {
                gpCommissioningFinalization(appId, gpdId, endpoint);
            }
        } else {
            gp_sinkRemoveEntries(appId, gpdId, endpoint);
        }
    }
}

static void gpApplicationDescCmdHandler(zcl_gp_commissioningNotificationCmd_t *pCmd)
{
    gpAppDescHandler(pCmd->options.bits.appId, pCmd->gpdId, pCmd->endpoint,
                     pCmd->options.bits.rxAfterTx, pCmd->gppShortAddr,
                     pCmd->payloadLen, pCmd->pGpdCmdPayload, TRUE);
}

u8 gpSinkCommNotiDecrypt(zcl_gp_commissioningNotificationCmd_t *pCmd)
{
    u8 gpdKey[16] = {0};
    u8 gpdKeyType = 0;
    u8 sta = SUCCESS;

    gpSecRsp_status_t status = gpSinkTunneledSecOperation(pCmd->options.bits.appId, pCmd->gpdId,
                                                          pCmd->endpoint, pCmd->gpdSecFrameCnt,
                                                          pCmd->options.bits.secLevel, pCmd->options.bits.secKeyType,
                                                          &gpdKeyType, gpdKey);

    if (status == GP_SEC_RSP_STATUS_MATCH) {
        if (pCmd->options.bits.secLevel == GP_SEC_LEVEL_NO_SECURITY) {
            return SUCCESS;
        }

        u8 *gpdAsdu = ev_buf_allocate(pCmd->payloadLen + 1);
        if (!gpdAsdu) {
            return FAILURE;
        }

        memset(gpdAsdu, 0, pCmd->payloadLen + 1);

        gpdAsdu[0] = pCmd->gpdCmdID;
        if (pCmd->payloadLen) {
            memcpy(&gpdAsdu[1], pCmd->pGpdCmdPayload, pCmd->payloadLen);
        }

        u8 gpdfSecKey = (pCmd->options.bits.secKeyType >= GP_SEC_KEY_TYPE_OUT_OF_THE_BOX_GPD_KEY) ? 1 : 0;

        sta = gpCcmStar(pCmd->options.bits.appId, pCmd->gpdId,
                        gpdfSecKey, pCmd->options.bits.secLevel,
                        pCmd->endpoint, pCmd->gpdSecFrameCnt,
                        pCmd->payloadLen + 1, gpdAsdu,
                        0, pCmd->options.bits.rxAfterTx,
                        pCmd->mic, gpdKey);

        if (sta == SUCCESS) {
            //update cmd id and cmd payload.
            pCmd->gpdCmdID = gpdAsdu[0];
            if (pCmd->payloadLen) {
                memcpy(pCmd->pGpdCmdPayload, &gpdAsdu[1], pCmd->payloadLen);
            }
        }

        if (gpdAsdu) {
            ev_buf_free(gpdAsdu);
        }

        return sta;
    }

    return FAILURE;
}

void gpCommissioningNotificationCmdProcess(zcl_gp_commissioningNotificationCmd_t *pCmd)
{
    //if the GPDF is encrypted.
    if (pCmd->options.bits.secPrcFailed) {
        if (gpSinkCommNotiDecrypt(pCmd) != SUCCESS) {
            return;
        }
        pCmd->options.bits.secPrcFailed = 0;
    }

    if (gpDataTunneledDuplicateCheck(pCmd->options.bits.appId, pCmd->gpdId, pCmd->gpdSecFrameCnt)) {
        return;
    }

    //printf("gpCommNotiRcv: gpdCmdID = %x\n", pCmd->gpdCmdID);

    switch (pCmd->gpdCmdID) {
    case GPDF_CMD_ID_CHANNEL_REQ:
        gpChannelReqCmdHandler(pCmd);
        break;
    case GPDF_CMD_ID_COMMISSIONING:
        gpCommissioningCmdHandler(pCmd);
        break;
    case GPDF_CMD_ID_SUCCESS:
        gpSuccessCmdHandler(pCmd);
        break;
    case GPDF_CMD_ID_DECOMMISSIONING:
        gpDecommissioningCmdHandler(pCmd);
        break;
    case GPDF_CMD_ID_APP_DESCRIPTION:
        gpApplicationDescCmdHandler(pCmd);
        break;
    default:
        break;
    }
}

static void gpSinkPairingSend(zcl_gp_pairingConfigurationCmd_t *pCmd, gpSinkTabEntry_t *pSinkEntry)
{
    if (pCmd->actions.bits.action <= REPLACE_SINK_TABLE_ENTRY) {
        if (pSinkEntry->options.bits.communicationMode != GPS_COMM_MODE_LIGHTWEIGHT_UNICAST) {
            u16 addrAlias = gpAliasSrcAddrDerived(pCmd->options.bits.appId, pCmd->gpdId);
            gpDevAnnceAliasSend(addrAlias);
        }
    }

    zcl_gp_pairingCmd_t *pPairingCmd = (zcl_gp_pairingCmd_t *)ev_buf_allocate(sizeof(zcl_gp_pairingCmd_t));
    if (!pPairingCmd) {
        return;
    }

    memset((u8 *)pPairingCmd, 0, sizeof(zcl_gp_pairingCmd_t));

    pPairingCmd->options.bits.appId = pCmd->options.bits.appId;
    if (pPairingCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        pPairingCmd->gpdId.srcId = pCmd->gpdId.srcId;
    } else if (pPairingCmd->options.bits.appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(pPairingCmd->gpdId.gpdIeeeAddr, pCmd->gpdId.gpdIeeeAddr);
        pPairingCmd->endpoint = pCmd->endpoint;
    }

    if (pCmd->actions.bits.action <= REPLACE_SINK_TABLE_ENTRY) {
        pPairingCmd->options.bits.addSink = 1;
        pPairingCmd->options.bits.removeGPD = 0;
        pPairingCmd->options.bits.commMode = pSinkEntry->options.bits.communicationMode;
        pPairingCmd->options.bits.gpdFixed = pSinkEntry->options.bits.fixedLocation;
        pPairingCmd->options.bits.gpdMacSeqNumCap = pSinkEntry->options.bits.seqNumCapabilities;
        pPairingCmd->options.bits.secLevel = pSinkEntry->secOptions.bits.secLevel;
        pPairingCmd->options.bits.secKeyType = pSinkEntry->secOptions.bits.secKeyType;
        pPairingCmd->options.bits.gpdSecFrameCntPresent = (pSinkEntry->gpdSecFrameCnt == 0xffffffff) ? 0 : 1;
        pPairingCmd->options.bits.gpdSecKeyPresent = (pSinkEntry->secOptions.bits.secLevel > GP_SEC_LEVEL_RESERVED) ? 1 : 0;
        pPairingCmd->options.bits.assignedAliasPresent = pSinkEntry->options.bits.assignedAlias;
        pPairingCmd->options.bits.groupcastRadiusPresent = (pSinkEntry->groupcastRadius == 0xff) ? 0 : 1;

        pPairingCmd->deviceID = pSinkEntry->deviceId;
        pPairingCmd->gpdSecFrameCnt = pSinkEntry->gpdSecFrameCnt;
        pPairingCmd->groupcastRadius = pSinkEntry->groupcastRadius;
        memcpy(pPairingCmd->gpdKey, pSinkEntry->gpdKey, SEC_KEY_LEN);
        pPairingCmd->assignedAlias = pSinkEntry->gpdAssignedAlias;
    } else {//remove
        pPairingCmd->options.bits.removeGPD = (pCmd->actions.bits.action == REMOVE_GPD) ? 1 : 0;
        pPairingCmd->options.bits.commMode = pCmd->options.bits.commMode;
    }

    if (pPairingCmd->options.bits.removeGPD) {
        gpPairingCmdSend(pPairingCmd);
    } else {
        if (pPairingCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_PRE_COMMISSIONED_GROUPID) {
            u8 groupListCnt = 0;

            if (pPairingCmd->options.bits.addSink) {
                groupListCnt = pSinkEntry->sinkGroupListCnt;

                for (u8 i = 0; i < groupListCnt; i++) {
                    pPairingCmd->sinkGroupID = pSinkEntry->groupList[i].groupId;
                    pPairingCmd->assignedAlias = pSinkEntry->groupList[i].alias;
                    pPairingCmd->options.bits.assignedAliasPresent = (pPairingCmd->assignedAlias == 0xffff) ? 0 : 1;

                    gpPairingCmdSend(pPairingCmd);
                }
            } else {
                groupListCnt = (pCmd->pGroupList[0] > SINK_GROUP_LIST_NUM) ? SINK_GROUP_LIST_NUM : pCmd->pGroupList[0];
                u8 *ptr = &pCmd->pGroupList[1];

                for (u8 i = 0; i < groupListCnt; i++) {
                    pPairingCmd->sinkGroupID = BUILD_U16(ptr[0], ptr[1]);
                    pPairingCmd->assignedAlias = BUILD_U16(ptr[2], ptr[3]);
                    ptr += 4;

                    gpPairingCmdSend(pPairingCmd);
                }
            }
        } else {
            if ((pPairingCmd->options.bits.commMode == GPS_COMM_MODE_FULL_UNICAST) ||
                (pPairingCmd->options.bits.commMode == GPS_COMM_MODE_LIGHTWEIGHT_UNICAST)) {
                ZB_64BIT_ADDR_COPY(pPairingCmd->sinkIeeeAddr, NIB_IEEE_ADDRESS());
                pPairingCmd->sinkNwkAddr = NIB_NETWORK_ADDRESS();
            } else if (pPairingCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_DGROUPID) {
                pPairingCmd->sinkGroupID = gpAliasSrcAddrDerived(pCmd->options.bits.appId, pCmd->gpdId);
            }

            gpPairingCmdSend(pPairingCmd);
        }
    }

    ev_buf_free((u8 *)pPairingCmd);
}

static status_t gpSinkPairingCfgActionHandler(zcl_gp_pairingConfigurationCmd_t *pCmd)
{
    gpSinkTabEntry_t *pSinkEntry = gp_getSinkTabByGpdId(pCmd->options.bits.appId, pCmd->gpdId);
    if (pSinkEntry) {
        if ((pCmd->endpoint != pSinkEntry->endpoint) &&
            (pCmd->endpoint != 0) && (pCmd->endpoint != 0xFF)) {
            pSinkEntry = NULL;
        }
    }

    if (!pSinkEntry || !pCmd->actions.bits.sendGpPairing) {
        return ZCL_STA_SUCCESS;
    }

    gpSinkPairingSend(pCmd, pSinkEntry);

    return ZCL_STA_SUCCESS;
}

static status_t gpSinkTabEntryCfgHandler(zcl_gp_pairingConfigurationCmd_t *pCmd)
{
    if ((pCmd->secOptions.bits.secLevel < (zclGpAttr_gpsSecLevel & 0x03))) {
        return ZCL_STA_FAILURE;
    }

    if (pCmd->actions.bits.action == REPLACE_SINK_TABLE_ENTRY) {
        //remove all sink table entry for this GPD.
        gp_transTabEntryRemove(pCmd->options.bits.appId, pCmd->gpdId, pCmd->endpoint);
        gp_sinkRemoveEntries(pCmd->options.bits.appId, pCmd->gpdId, pCmd->endpoint);
    }

    gpSinkTabEntry_t *pSinkEntry = gp_getSinkTabByGpdId(pCmd->options.bits.appId, pCmd->gpdId);
    if (pSinkEntry) {
        if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
            if ((pCmd->endpoint != pSinkEntry->endpoint) &&
                (pCmd->endpoint != 0) && (pCmd->endpoint != 0xFF)) {
                pSinkEntry = NULL;
            }
        }

        if (pCmd->actions.bits.action == EXTEND_SINK_TABLE_ENTRY) {
            if (pSinkEntry && (pSinkEntry->options.bits.communicationMode != pCmd->options.bits.commMode)) {
                pSinkEntry = NULL;
            }
        }
    }
    if (!pSinkEntry) {
        pSinkEntry = gp_sinkTabEntryFreeGet();
        if (!pSinkEntry) {
            return ZCL_STA_FAILURE;
        }
    }

    //new entry.
    if (!pSinkEntry->used) {
        pSinkEntry->used = 1;
        g_gpSinkTab.gpSinkTabNum++;
    }

    /*
     * TODO: Fill sink table according the 'Number of paired endpoints' field?
     */

    //fill entry parameters.
    pSinkEntry->options.bits.appId = pCmd->options.bits.appId;
    pSinkEntry->options.bits.communicationMode = pCmd->options.bits.commMode;
    pSinkEntry->options.bits.seqNumCapabilities = pCmd->options.bits.seqNumCap;
    pSinkEntry->options.bits.rxOnCapability = pCmd->options.bits.rxOnCap;
    pSinkEntry->options.bits.fixedLocation = pCmd->options.bits.fixedLocation;
    pSinkEntry->options.bits.assignedAlias = pCmd->options.bits.assignedAlias;
    pSinkEntry->options.bits.securityUse = pCmd->options.bits.secUse;

    if (pCmd->options.bits.appId == GP_APP_ID_SRC_ID) {
        pSinkEntry->gpdId.srcId = pCmd->gpdId.srcId;
    } else if (pCmd->options.bits.appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(pSinkEntry->gpdId.gpdIeeeAddr, pCmd->gpdId.gpdIeeeAddr);
        pSinkEntry->endpoint = pCmd->endpoint;
    }

    pSinkEntry->deviceId = pCmd->deviceId;
    pSinkEntry->groupcastRadius = pCmd->groupcastRadius;

    if (pCmd->options.bits.assignedAlias) {
        pSinkEntry->gpdAssignedAlias = pCmd->gpdAssignedAlias;
    }

    if (pCmd->options.bits.secUse) {
        pSinkEntry->secOptions.opts = pCmd->secOptions.opts;
        pSinkEntry->gpdSecFrameCnt = pCmd->gpdSecFrameCnt;
        memcpy(pSinkEntry->gpdKey, pCmd->gpdSecKey, SEC_KEY_LEN);
    } else if (pCmd->options.bits.seqNumCap) {
        pSinkEntry->gpdSecFrameCnt = pCmd->gpdSecFrameCnt;
    }

    aps_add_group_req_t addGroupReq;
    TL_SETSTRUCTCONTENT(addGroupReq, 0);

    if (pCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_PRE_COMMISSIONED_GROUPID) {
        pSinkEntry->sinkGroupListCnt = (pCmd->pGroupList[0] > SINK_GROUP_LIST_NUM) ? SINK_GROUP_LIST_NUM
                                                                                   : pCmd->pGroupList[0];
        if (pSinkEntry->sinkGroupListCnt) {
            u8 *ptr = &pCmd->pGroupList[1];
            for (u8 i = 0; i < pSinkEntry->sinkGroupListCnt; i++) {
                pSinkEntry->groupList[i].groupId = BUILD_U16(ptr[0], ptr[1]);
                pSinkEntry->groupList[i].alias = BUILD_U16(ptr[2], ptr[3]);
                ptr += 4;

                addGroupReq.ep = GREEN_POWER_ENDPOINT;
                addGroupReq.group_addr = pSinkEntry->groupList[i].groupId;
                aps_me_group_add_req(&addGroupReq);
            }
        }
    } else if (pCmd->options.bits.commMode == GPS_COMM_MODE_GROUP_DGROUPID) {
        addGroupReq.ep = GREEN_POWER_ENDPOINT;
        addGroupReq.group_addr = gpAliasSrcAddrDerived(pCmd->options.bits.appId, pCmd->gpdId);
        aps_me_group_add_req(&addGroupReq);
    }

    //TODO: numOfPairedEndpoints?
    gp_transTabUpdateWithCfg(pCmd->options.bits.appId, pCmd->gpdId, pCmd->endpoint,
                             pCmd->deviceId, pCmd->numOfGpdCmds,
                             pCmd->pGpdCmdList, pCmd->pClusterList);

    //check if there's application information
    if (pCmd->options.bits.appInfoPresent) {
        if (pCmd->appInfo.bits.switchInfoPresent &&
           (pCmd->deviceId == GPD_DEV_ID_GENERIC_8_CONTACT_SWITCH)) {
            gp_transTabGenSwitchUpdate(pCmd->options.bits.appId, pCmd->gpdId, pCmd->endpoint, &pCmd->switchInfo);
        }

        if (pCmd->appInfo.bits.gpdAppDescCmdFollows) {
            gpAppDescFollowHandler(pSinkEntry);

            return ZCL_STA_SUCCESS;
        }
    }

    pSinkEntry->complete = 1;
    //store sink table.
    gpSinkTabUpdate();
    gp_transTabNvUpdate();

    if (pCmd->actions.bits.sendGpPairing) {
        gpSinkPairingSend(pCmd, pSinkEntry);
    }

    return ZCL_STA_SUCCESS;
}

static void gpSinkPairingRemoveHandler(zcl_gp_pairingConfigurationCmd_t *pCmd)
{
    gpSinkTabEntry_t *pSinkEntry = gp_getSinkTabByGpdId(pCmd->options.bits.appId, pCmd->gpdId);
    if (pSinkEntry) {
        if (pCmd->actions.bits.action == REMOVE_A_PAIRING) {
            if (pCmd->options.bits.commMode != pSinkEntry->options.bits.communicationMode) {
                return;
            }
        }

        u8 rmEndPoint = (pCmd->actions.bits.action == REMOVE_A_PAIRING) ? pSinkEntry->endpoint : 0xFF;

        gp_transTabEntryRemove(pCmd->options.bits.appId, pCmd->gpdId, rmEndPoint);
        gp_sinkRemoveEntries(pCmd->options.bits.appId, pCmd->gpdId, rmEndPoint);

        if (pCmd->actions.bits.sendGpPairing) {
            gpSinkPairingSend(pCmd, NULL);
        }
    }
}

static void gpAppDescCfgHandler(zcl_gp_pairingConfigurationCmd_t *pCmd)
{
    gpAppDescHandler(pCmd->options.bits.appId, pCmd->gpdId, pCmd->endpoint,
                     pCmd->options.bits.rxOnCap, NIB_NETWORK_ADDRESS(),
                     pCmd->reportDescLen, pCmd->pReportDescriptor, FALSE);
}

static status_t gpPairingCfgCmdProcess(zcl_gp_pairingConfigurationCmd_t *pCmd)
{
    status_t status = ZCL_STA_SUCCESS;

    //printf("gpPairingCfgPrc: action = %x, opts = %x, appInfo = %x\n",
    //       pCmd->actions.bits.action, pCmd->options.opts, pCmd->appInfo.info);

    if (((pCmd->options.bits.appId == GP_APP_ID_SRC_ID) && (pCmd->gpdId.srcId == 0)) ||
        ((pCmd->options.bits.appId == GP_APP_ID_GPD) && ZB_IS_64BIT_ADDR_ZERO(pCmd->gpdId.gpdIeeeAddr))) {
        //silently dropped, A.3.5.2.4.1.
        return ZCL_STA_SUCCESS;
    }

    if ((pCmd->actions.bits.action <= REPLACE_SINK_TABLE_ENTRY) &&
        (pCmd->options.bits.secUse && (pCmd->secOptions.bits.secLevel == GP_SEC_LEVEL_RESERVED))) {
        //A.3.5.2.4.1.
        return ZCL_STA_FAILURE;
    }

    switch (pCmd->actions.bits.action) {
    case NO_ACTION:
        status = gpSinkPairingCfgActionHandler(pCmd);
        break;
    case EXTEND_SINK_TABLE_ENTRY:
    case REPLACE_SINK_TABLE_ENTRY:
        status = gpSinkTabEntryCfgHandler(pCmd);
        break;
    case REMOVE_A_PAIRING:
    case REMOVE_GPD:
        gpSinkPairingRemoveHandler(pCmd);
        break;
    case APPLICATION_DESCRIPTION:
        //TODO: gpCommissioningFinalization ?
        gpAppDescCfgHandler(pCmd);
        break;
    default:
        break;
    }

    return status;
}

static void gpSinkTabReqCmdProcess(zclIncomingAddrInfo_t *pAddrInfo, zcl_gp_sinkTabReqCmd_t *pCmd)
{
    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
    dstEpInfo.dstAddr.shortAddr = pAddrInfo->srcAddr;
    dstEpInfo.dstEp = pAddrInfo->srcEp;
    dstEpInfo.profileId = pAddrInfo->profileId;

    zcl_gp_sinkTabRspCmd_t sinkTabRspCmd;
    TL_SETSTRUCTCONTENT(sinkTabRspCmd, 0);

    u8 *pBuf = NULL;

    sinkTabRspCmd.totalTabEntries = gp_getSinkTabEntryTotalNum();

    if (sinkTabRspCmd.totalTabEntries == 0) {
        sinkTabRspCmd.status = ZCL_STA_NOT_FOUND;
        sinkTabRspCmd.startIdx = (pCmd->options.bits.reqType == REQUEST_TABLE_ENTRIES_BY_GPD_ID) ? 0xFF : pCmd->index;
        sinkTabRspCmd.entriesCnt = 0;
        sinkTabRspCmd.entriesLen = 0;

        zcl_gp_sinkTableRspCmdSend(GREEN_POWER_ENDPOINT, &dstEpInfo, TRUE, pAddrInfo->seqNum, &sinkTabRspCmd);
        return;
    }

    if (pCmd->options.bits.reqType == REQUEST_TABLE_ENTRIES_BY_GPD_ID) {
        gpSinkTabEntry_t *pEntry = gp_getSinkTabByGpdId(pCmd->options.bits.appId, pCmd->gpdId);
        if (pEntry) {
            u8 len = gp_getSinkTabEntryLen(pEntry);
            pBuf = (u8 *)ev_buf_allocate(len);
            if (!pBuf) {
                return;
            }

            memset(pBuf, 0, len);

            gp_buildSinkTabEntryFormat(pEntry, pBuf);

            sinkTabRspCmd.status = ZCL_STA_SUCCESS;
            sinkTabRspCmd.startIdx = 0xFF;
            sinkTabRspCmd.entriesCnt = 1;
            sinkTabRspCmd.entriesLen = len;
            sinkTabRspCmd.sinkTabEntry = pBuf;
        } else {
            sinkTabRspCmd.status = ZCL_STA_NOT_FOUND;
            sinkTabRspCmd.startIdx = 0xFF;
            sinkTabRspCmd.entriesCnt = 0;
            sinkTabRspCmd.entriesLen = 0;
        }
    } else if (pCmd->options.bits.reqType == REQUEST_TABLE_ENTRIES_BY_INDEX) {
        sinkTabRspCmd.status = ZCL_STA_NOT_FOUND;
        sinkTabRspCmd.startIdx = pCmd->index;

        u8 entryLen = 0;
        u8 maxEntryLen = 0;
        u8 activeEntryCnt = 0;

        for (u8 i = 0; i < zclGpAttr_gpsMaxSinkTabEntries; i++) {
            maxEntryLen = entryLen;

            if (g_gpSinkTab.gpSinkTab[i].used) {
                activeEntryCnt++;

                if (pCmd->index <= activeEntryCnt - 1) {
                    entryLen += gp_getSinkTabEntryLen(&g_gpSinkTab.gpSinkTab[i]);

                    if (entryLen >= ZCL_GP_MAX_SINK_TABLE_ATTR_LEN) {
                        entryLen = maxEntryLen;
                        break;
                    } else {
                        sinkTabRspCmd.entriesCnt++;
                    }
                }
            }
        }

        activeEntryCnt = 0;
        sinkTabRspCmd.entriesLen = entryLen;

        u8 entriesCnt = sinkTabRspCmd.entriesCnt;

        if (entryLen) {
            sinkTabRspCmd.status = ZCL_STA_SUCCESS;

            pBuf = (u8 *)ev_buf_allocate(entryLen);
            if (!pBuf) {
                return;
            }

            memset(pBuf, 0, entryLen);

            u8 *ptr = pBuf;

            for (u8 i = 0; i < zclGpAttr_gpsMaxSinkTabEntries; i++) {
                if (g_gpSinkTab.gpSinkTab[i].used) {
                    activeEntryCnt++;

                    if (pCmd->index <= activeEntryCnt - 1) {
                        if (entriesCnt) {
                            ptr += gp_buildSinkTabEntryFormat(&g_gpSinkTab.gpSinkTab[i], ptr);
                            entriesCnt--;
                        } else {
                            break;
                        }
                    }
                }
            }

            sinkTabRspCmd.sinkTabEntry = pBuf;
        }
    }

    zcl_gp_sinkTableRspCmdSend(GREEN_POWER_ENDPOINT, &dstEpInfo, TRUE, pAddrInfo->seqNum, &sinkTabRspCmd);

    if (pBuf) {
        ev_buf_free(pBuf);
    }
}

static void gpTransTabReqCmdProcess(zclIncomingAddrInfo_t *pAddrInfo, zcl_gp_transTabReqCmd_t *pCmd)
{
    status_t status = ZCL_STA_NOT_FOUND;

    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
    dstEpInfo.dstAddr.shortAddr = pAddrInfo->srcAddr;
    dstEpInfo.dstEp = pAddrInfo->srcEp;
    dstEpInfo.profileId = pAddrInfo->profileId;

    zcl_gp_transTabRspCmd_t transTabRspCmd;
    TL_SETSTRUCTCONTENT(transTabRspCmd, 0);

    u8 *pBuf = NULL;

    u8 entryLen = 0;
    u8 totalLen = 0;
    u8 activeEntryCnt = 0;
    u8 appId = 0xff;

    transTabRspCmd.startIdx = pCmd->startIdx;
    transTabRspCmd.totalNumOfEntries = gp_transTabNumGet();

    if (transTabRspCmd.totalNumOfEntries) {
        for (u8 i = 0; i < GPS_MAX_TRANS_TABLE_ENTRIES; i++) {
            if (g_gpTransTab.transTab[i].used) {
                activeEntryCnt++;

                if (pCmd->startIdx <= activeEntryCnt - 1) {
                    entryLen = gp_transTabEntryLenGet(&g_gpTransTab.transTab[i]);

                    if (totalLen + entryLen > ZCL_GP_MAX_TRANS_TABLE_ATTR_LEN) {
                        break;
                    }

                    if ((appId != 0xff) && (appId != g_gpTransTab.transTab[i].options.bits.appId)) {
                        break;
                    }

                    appId = g_gpTransTab.transTab[i].options.bits.appId;

                    totalLen += entryLen;

                    transTabRspCmd.entriesCnt++;
                }
            }
        }

        transTabRspCmd.bufLen = totalLen;

        activeEntryCnt = 0;

        u8 entriesCnt = transTabRspCmd.entriesCnt;

        //printf("totalLen = %d\n", totalLen);

        if (totalLen) {
            pBuf = (u8 *)ev_buf_allocate(totalLen);
            if (!pBuf) {
                return;
            }

            memset(pBuf, 0, totalLen);

            u8 *ptr = pBuf;

            for (u8 i = 0; i < GPS_MAX_TRANS_TABLE_ENTRIES; i++) {
                if (g_gpTransTab.transTab[i].used) {
                    activeEntryCnt++;

                    if (pCmd->startIdx <= activeEntryCnt - 1) {
                        if (entriesCnt) {
                            transTabRspCmd.options.opts = g_gpTransTab.transTab[i].options.opts;

                            ptr += gp_buildTransTabEntryFormat(&g_gpTransTab.transTab[i], ptr);
                            entriesCnt--;
                        } else {
                            break;
                        }
                    }
                }
            }

            transTabRspCmd.pBuf = pBuf;

            //printf("buf:\n");
            //DEBUG_ARRAY(1, pBuf, totalLen);

            status = ZCL_STA_SUCCESS;
        }
    }

    transTabRspCmd.status = status;

    zcl_gp_transTableRspCmdSend(GREEN_POWER_ENDPOINT, &dstEpInfo, TRUE, pAddrInfo->seqNum, &transTabRspCmd);

    if (pBuf) {
        ev_buf_free(pBuf);
    }
}

static void gpTransTabUpdateHandler(u8 appId, gpdId_t gpdId, u8 endpoint,
                                    u8 action, gpTranslation_t *pTranslation)
{
    //printf("transTabUpdate: action = %x, gpdCmd = %x, zbEp = %x, cluster = %x, profile = %x\n",
    //       action, pTranslation->gpdCmdId, pTranslation->endpoint, pTranslation->cluster, pTranslation->profile);

    transTabEntry_t *pTransEntry = NULL;

    if (action == ACTION_REMOVE_TRANS_TABLE_ENTRY) {
        bool nvUpdate = 0;
        do {
            pTransEntry = gp_transTabEntryFind(appId, gpdId, endpoint,
                                               pTranslation->gpdCmdId, pTranslation->endpoint,
                                               pTranslation->cluster, pTranslation->profile,
                                               0xFF, 0xFFFF);
            if (pTransEntry) {
                nvUpdate = 1;
                gp_transTabEntryClear(pTransEntry);
            }
        } while (pTransEntry);

        if (nvUpdate) {
            gp_transTabNvUpdate();
        }

        return;
    } else if (action == ACTION_ADD_TRANS_TABLE_ENTRY) {
        pTransEntry = NULL;
    } else if (action == ACTION_REPLACE_TRANS_TABLE_ENTRY) {
        pTransEntry = gp_transTabEntryFind(appId, gpdId, endpoint,
                                           pTranslation->gpdCmdId, pTranslation->endpoint,
                                           pTranslation->cluster, pTranslation->profile,
                                           0xFF, 0xFFFF);
    }

    if (gpTransTabUpdate(appId, gpdId, endpoint, pTransEntry, pTranslation)) {
        gp_transTabNvUpdate();
    }
}

static status_t gpTransTabUpdateCmdProcess(zcl_gp_transTabUpdateCmd_t *pCmd)
{
    u8 *pData = pCmd->pBuf;
    u8 *ptr = NULL;
    s32 remainLen = pCmd->bufLen;
    u8 transCnt = pCmd->options.bits.numOfTrans;

    //printf("gpTransUpdateCmd: action = %x, num = %d\n", pCmd->options.bits.action, pCmd->options.bits.numOfTrans);

    if ((pCmd->bufLen == 0) || (pCmd->options.bits.numOfTrans == 0)) {
        return ZCL_STA_FAILURE;
    }

    do {
        ptr = pData;

        gpTranslation_t translation;
        TL_SETSTRUCTCONTENT(translation, 0);

        if (transCnt--) {
            translation.index = *pData++;
            translation.gpdCmdId = *pData++;
            translation.endpoint = *pData++;
            translation.profile = BUILD_U16(pData[0], pData[1]);
            pData += 2;
            translation.cluster = BUILD_U16(pData[0], pData[1]);
            pData += 2;
            translation.zbCmdId = *pData++;
            translation.zbCmdPayloadLen = *pData++;
            if ((translation.zbCmdPayloadLen != 0) &&
                (translation.zbCmdPayloadLen != 0xff) &&
                (translation.zbCmdPayloadLen != 0xfe)) {
                translation.pZbCmdPayload = pData;
                pData += translation.zbCmdPayloadLen;
            }
            if (pCmd->options.bits.addInfoPresent) {
                translation.addInfoLen = *pData++;
                if (translation.addInfoLen) {
                    translation.pAdditionalInfo = pData;
                    pData += translation.addInfoLen;
                }
            }

            gpTransTabUpdateHandler(pCmd->options.bits.appId, pCmd->gpdId, pCmd->endpoint,
                                    pCmd->options.bits.action, &translation);
        }

        remainLen = remainLen - (pData - ptr);
    } while ((remainLen > 0) && (transCnt > 0));

    return ZCL_STA_SUCCESS;
}

status_t zcl_gpClientCmdHandler(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
    status_t status = ZCL_STA_SUCCESS;

    switch (cmdId) {
    case ZCL_CMD_GP_NOTIFICATION:
        gpNotificationCmdProcess((zcl_gp_notificationCmd_t *)cmdPayload);
        break;
    case ZCL_CMD_GP_COMMISSIONING_NOTIFICATION:
        gpCommissioningNotificationCmdProcess((zcl_gp_commissioningNotificationCmd_t *)cmdPayload);
        break;
    case ZCL_CMD_GP_PAIRING_CONFIGURATION:
        status = gpPairingCfgCmdProcess((zcl_gp_pairingConfigurationCmd_t *)cmdPayload);
        break;
    case ZCL_CMD_GP_SINK_TABLE_REQUEST:
        gpSinkTabReqCmdProcess(pAddrInfo, (zcl_gp_sinkTabReqCmd_t *)cmdPayload);
        break;
    case ZCL_CMD_GP_SINK_COMMISSIONING_MODE:
        status = gpSinkCommModeCmdProcess((zcl_gp_sinkCommissioningModeCmd_t *)cmdPayload);
        break;
    case ZCL_CMD_GP_TRANSLATION_TABLE_REQUEST:
        gpTransTabReqCmdProcess(pAddrInfo, (zcl_gp_transTabReqCmd_t *)cmdPayload);
        break;
    case ZCL_CMD_GP_TRANSLATION_TABLE_UPDATE_COMMAND:
        status = gpTransTabUpdateCmdProcess((zcl_gp_transTabUpdateCmd_t *)cmdPayload);
        break;
    default:
        status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
        break;
    }

    return status;
}

static void gpsGpdfChannelReqCmdHandler(gp_data_ind_t *pGpDataInd)
{
    if ((g_gpsCtx.gpsInCommMode == FALSE) ||
        (pGpDataInd->frameType == GP_NWK_FRAME_TYPE_DATA)) {
        return;
    }

    gpd_channelRequestCmd_payload_t chnReq;
    TL_SETSTRUCTCONTENT(chnReq, 0);
    chnReq.chTogglingBehavior = pGpDataInd->gpdAsdu[1];

    gpd_channelConfigurationCmd_payload_t chnCfg;
    TL_SETSTRUCTCONTENT(chnCfg, 0);
    chnCfg.bits.operationChannel = g_zbMacPib.phyChannelCur - TL_ZB_MAC_CHANNEL_START;
    chnCfg.bits.basic = 1;

    gpdId_t gpdId;
    if (pGpDataInd->appId == GP_APP_ID_SRC_ID) {
        gpdId.srcId = pGpDataInd->srcId;
    } else if (pGpDataInd->appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(gpdId.gpdIeeeAddr, pGpDataInd->srcAddr.extAddr);
    }

    gpChannelRequestHandler(pGpDataInd->appId, gpdId, pGpDataInd->endpoint,
                            NIB_NETWORK_ADDRESS(), chnReq.bits.rxChNext, &chnCfg);
}

static void gpsGpdfCommissioningCmdHandler(gp_data_ind_t *pGpDataInd)
{
    if (pGpDataInd->autoCommissioning) {
        return;
    }

    gpdId_t gpdId;
    if (pGpDataInd->appId == GP_APP_ID_SRC_ID) {
        gpdId.srcId = pGpDataInd->srcId;
    } else if (pGpDataInd->appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(gpdId.gpdIeeeAddr, pGpDataInd->srcAddr.extAddr);
    }

    gpd_commissioning_payload_t commissioning;
    TL_SETSTRUCTCONTENT(commissioning, 0);

    commissioning.payloadLen = gpCommissioningParse(&commissioning, &pGpDataInd->gpdAsdu[1]);

    gpCommissioningHandler(pGpDataInd->appId, gpdId, pGpDataInd->endpoint,
                           pGpDataInd->gpdfKeyType, pGpDataInd->gpdfSecurityLevel,
                           pGpDataInd->gpdSecFrameCnt, pGpDataInd->rxAfterTx,
                           NIB_NETWORK_ADDRESS(), &commissioning);
}

static void gpsGpdfSuccessCmdHandler(gp_data_ind_t *pGpDataInd)
{
    if (!g_gpsCtx.gpsInCommMode) {
        return;
    }

    if (pGpDataInd->autoCommissioning) {
        return;
    }

    if (((pGpDataInd->appId == GP_APP_ID_SRC_ID) && (pGpDataInd->srcId == 0)) ||
        ((pGpDataInd->appId == GP_APP_ID_GPD) && ZB_IS_64BIT_ADDR_ZERO(pGpDataInd->srcAddr.extAddr))) {
        return;
    }

    gpdId_t gpdId;
    if (pGpDataInd->appId == GP_APP_ID_SRC_ID) {
        gpdId.srcId = pGpDataInd->srcId;
    } else if (pGpDataInd->appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(gpdId.gpdIeeeAddr, pGpDataInd->srcAddr.extAddr);
    }

    gpSuccessHandler(pGpDataInd->appId, gpdId, pGpDataInd->endpoint,
                     pGpDataInd->gpdfSecurityLevel, pGpDataInd->gpdfKeyType,
                     pGpDataInd->gpdSecFrameCnt);
}

static void gpsGpdfDecommissioningCmdHandler(gp_data_ind_t *pGpDataInd)
{
    gpdId_t gpdId;
    if (pGpDataInd->appId == GP_APP_ID_SRC_ID) {
        gpdId.srcId = pGpDataInd->srcId;
    } else if (pGpDataInd->appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(gpdId.gpdIeeeAddr, pGpDataInd->srcAddr.extAddr);
    }

    gpDecommissioningHandler(pGpDataInd->appId, gpdId, pGpDataInd->endpoint);
}

static void gpsGpdfDataHandler(gp_data_ind_t *pGpDataInd)
{
    if (g_gpsCtx.gpsInCommMode) {
        return;
    }

    if ((pGpDataInd->status != GP_DATA_IND_STATUS_SEC_SUCCESS) &&
        (pGpDataInd->status != GP_DATA_IND_STATUS_NO_SECURITY)) {
        return;
    }

    if (((pGpDataInd->appId == GP_APP_ID_SRC_ID) && (pGpDataInd->srcId == 0)) ||
        ((pGpDataInd->appId == GP_APP_ID_GPD) && ZB_IS_64BIT_ADDR_ZERO(pGpDataInd->srcAddr.extAddr))) {
        return;
    }

    gpdId_t gpdId;
    if (pGpDataInd->appId == GP_APP_ID_SRC_ID) {
        gpdId.srcId = pGpDataInd->srcId;
    } else if (pGpDataInd->appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(gpdId.gpdIeeeAddr, pGpDataInd->srcAddr.extAddr);
    }

    u8 dataLen = pGpDataInd->gpdAsduLen - 1;//first byte is cmdId.
    u8 *pData = NULL;
    if (dataLen) {
        pData = &pGpDataInd->gpdAsdu[1];
    }

    gpdDataHandler(pGpDataInd->appId, gpdId, pGpDataInd->endpoint,
                   pGpDataInd->gpdfSecurityLevel, pGpDataInd->gpdfKeyType,
                   pGpDataInd->gpdSecFrameCnt, pGpDataInd->gpdCmdId,
                   dataLen, pData);
}

static void gpsGpdfAppDescCmdHandler(gp_data_ind_t *pGpDataInd)
{
    gpdId_t gpdId;
    if (pGpDataInd->appId == GP_APP_ID_SRC_ID) {
        gpdId.srcId = pGpDataInd->srcId;
    } else if (pGpDataInd->appId == GP_APP_ID_GPD) {
        ZB_64BIT_ADDR_COPY(gpdId.gpdIeeeAddr, pGpDataInd->srcAddr.extAddr);
    }

    u8 dataLen = pGpDataInd->gpdAsduLen - 1;//first byte is cmdId.
    u8 *pData = NULL;
    if (dataLen) {
        pData = &pGpDataInd->gpdAsdu[1];
    }

    gpAppDescHandler(pGpDataInd->appId, gpdId, pGpDataInd->endpoint,
                     pGpDataInd->rxAfterTx, NIB_NETWORK_ADDRESS(),
                     dataLen, pData, TRUE);
}

void gpsGpDataIndProcess(gp_data_ind_t *pGpDataInd)
{
    if (!pGpDataInd) {
        return;
    }

    if (pGpDataInd->status > GP_DATA_IND_STATUS_NO_SECURITY) {
        return;
    }

    //printf("GPS_dataInd: cmdId = %x, fc = %x, sta = %x, isCommMode = %x\n",
    //       pGpDataInd->gpdCmdId, pGpDataInd->gpdSecFrameCnt, pGpDataInd->status, g_gpsCtx.gpsInCommMode);

    switch (pGpDataInd->gpdCmdId) {
    case GPDF_CMD_ID_CHANNEL_REQ:
        gpsGpdfChannelReqCmdHandler(pGpDataInd);
        break;
    case GPDF_CMD_ID_COMMISSIONING:
        gpsGpdfCommissioningCmdHandler(pGpDataInd);
        break;
    case GPDF_CMD_ID_SUCCESS:
        gpsGpdfSuccessCmdHandler(pGpDataInd);
        break;
    case GPDF_CMD_ID_DECOMMISSIONING:
        gpsGpdfDecommissioningCmdHandler(pGpDataInd);
        break;
    case GPDF_CMD_ID_APP_DESCRIPTION:
        gpsGpdfAppDescCmdHandler(pGpDataInd);
        break;
    default:
        gpsGpdfDataHandler(pGpDataInd);
        break;
    }
}

#endif	/* GP_SUPPORT_ENABLE */
