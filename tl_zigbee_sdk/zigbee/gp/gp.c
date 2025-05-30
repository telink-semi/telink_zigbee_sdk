/********************************************************************************************************
 * @file    gp.c
 *
 * @brief   This is the source file for gp
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
#include "../zcl/zcl_include.h"
#include "gp.h"
#include "gp_base.h"
#include "gp_proxy.h"
#if GP_BASIC_COMBO
#include "gp_sink.h"
#endif

#if GP_SUPPORT_ENABLE
/**********************************************************************
 * GLOBAL VARIABLES
 */
const u8 g_gpSharedKey[SEC_KEY_LEN] = GP_SHARED_KEY;

gp_appCb_t *gpAppCb = NULL;

/**********************************************************************
 * LOCAL VARIABLES
 */
/**
 *  @brief Callback for GP primitive process
 */
const gp_stubCb_t gpStubCbList = {
    gpDataCnfPrc,
    gpDataIndPrc,
    gpSecReqPrc,
};

const gp_appCb_t gpAppCbList = {
    gpChangleChannelReqCb,
    gpCommissioningModeCb,
};

/**
 *  @brief Callback handler for GP received Device Announce Command.
 */
gpDeviceAnnounceCheckCb_t g_gpDeviceAnnounceCheckCb = gpDevAnnceCheckCb;

/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      gp_init
 *
 * @brief   This function initialize the Green Power EndPoint information for this node.
 *
 * @param   None
 *
 * @return  None
 */
void gp_init(u8 endpoint)
{
    /* Initialize GP Stub */
    gpStubCbInit((gp_stubCb_t *)&gpStubCbList);

    /* Register GP EndPoint descriptor */
    af_endpointRegister(GREEN_POWER_ENDPOINT, (af_simple_descriptor_t *)&gp_simpleDesc, zcl_rx_handler, NULL);

    /* Register GP ZCL command callBacks */
    zcl_register(GREEN_POWER_ENDPOINT, GP_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)g_gpClusterList);

    gp_registerAppCb((gp_appCb_t *)&gpAppCbList);

    gp_proxyInit();

#if GP_BASIC_COMBO
    gp_sinkInit(endpoint);
#endif
}

/*********************************************************************
 * @fn      gp_registerAppCb
 *
 * @brief   This function for application layer to register GP callBacks.
 *
 * @param   cb
 *
 * @return  None
 */
void gp_registerAppCb(gp_appCb_t *cb)
{
    gpAppCb = cb;
}

bool gpChangleChannelReqCb(void)
{
    //Allow change channel request.

    return TRUE;
}

void gpCommissioningModeCb(bool isInCommMode)
{
    //printf("gpIsInCommMode: %x\n", isInCommMode);

    if (!isInCommMode) {
        zb_nlmePermitJoiningRequest(0);
    }
}

void gpsCommissionModeInvork(void)
{
#if GP_BASIC_COMBO
    bool action = g_gpsCtx.gpsInCommMode ? FALSE : TRUE;

    gpsCommissionModeSet(action, TRUE);
#endif
}

void gpCommissioningNotificationCmdFromLocalCb(zcl_gp_commissioningNotificationCmd_t *pCmd)
{
#if GP_BASIC_COMBO
    gpCommissioningNotificationCmdProcess(pCmd);
#endif
}

static s32 gpAliasConflictTimeoutCb(void *arg)
{
    gpDevAnnceAliasSend(g_gppCtx.gpAliasConflictAddr);

    g_gppCtx.gpAliasConflictAddr = NWK_BROADCAST_RESERVED;

    g_gppCtx.aliasConflictTimeoutEvt = NULL;
    return -1;
}

static void gpAliansConflictTimeoutEvtStop(void)
{
    if (g_gppCtx.aliasConflictTimeoutEvt) {
        TL_ZB_TIMER_CANCEL(&g_gppCtx.aliasConflictTimeoutEvt);
    }
}

bool gpDevAnnceCheckCb(u16 aliasNwkAddr, addrExt_t aliasIeeeAddr)
{
    //printf("gpDevAnnceChk: aliasNwkAddr = %x\n", aliasNwkAddr);

    if (ZB_IEEE_ADDR_IS_INVALID(aliasIeeeAddr)) {
        if (g_gppCtx.aliasConflictTimeoutEvt) {
            if (g_gppCtx.gpAliasConflictAddr == aliasNwkAddr) {
                gpAliansConflictTimeoutEvtStop();
            }
            return TRUE;
        }
    } else {
        if (g_gppCtx.aliasConflictTimeoutEvt) {
            return TRUE;
        }

        bool sendDevAnnce = FALSE;

#if GP_BASIC_COMBO
        if (!sendDevAnnce && gp_getSinkTabEntryTotalNum()) {
            for (u8 i = 0; i < zclGpAttr_gpsMaxSinkTabEntries; i++) {
                if (g_gpSinkTab.gpSinkTab[i].used) {
                    u16 addr = 0xFFFF;

                    if (!g_gpSinkTab.gpSinkTab[i].options.bits.assignedAlias) {
                        addr = gpAliasSrcAddrDerived(g_gpSinkTab.gpSinkTab[i].options.bits.appId,
                                                     g_gpSinkTab.gpSinkTab[i].gpdId);
                    } else {
                        addr = g_gpSinkTab.gpSinkTab[i].gpdAssignedAlias;
                    }

                    if ((addr != 0xFFFF) && (addr == aliasNwkAddr)) {
                        g_gppCtx.gpAliasConflictAddr = aliasNwkAddr;
                        sendDevAnnce = TRUE;
                        break;
                    }
                }
            }
        }
#endif
        if (!sendDevAnnce && gp_getProxyTabEntryTotalNum()) {
            for (u8 i = 0; i < zclGpAttr_gppMaxProxyTabEntries; i++) {
                if (g_gpProxyTab.gpProxyTab[i].used) {
                    for (u8 j = 0; j < g_gpProxyTab.gpProxyTab[i].lwSinkCnt; j++) {
                        if (ZB_IEEE_ADDR_CMP(g_gpProxyTab.gpProxyTab[i].lightweightSinkAddrList[j].sinkIeeeAddr, aliasIeeeAddr)) {
                            g_gpProxyTab.gpProxyTab[i].lightweightSinkAddrList[j].sinkNwkAddr = aliasNwkAddr;
                        }
                    }

                    if ((aliasNwkAddr == gpAliasSrcAddrDerived(g_gpProxyTab.gpProxyTab[i].options.bits.appId, g_gpProxyTab.gpProxyTab[i].gpdId)) ||
                        (aliasNwkAddr == g_gpProxyTab.gpProxyTab[i].gpdAssignedAlias)) {
                        tl_zbNwkAddrConflictStatusSend(NWK_BROADCAST_RX_ON_WHEN_IDLE, aliasNwkAddr, TRUE);

                        g_gppCtx.gpAliasConflictAddr = aliasNwkAddr;
                        sendDevAnnce = TRUE;
                        break;
                    }
                }
            }
        }

        if (sendDevAnnce) {
            //A.3.6.3.1, between Dmin and Dmax ms
            u16 devAnnceDelay = (zb_random() / 650) + 5;
            g_gppCtx.aliasConflictTimeoutEvt = TL_ZB_TIMER_SCHEDULE(gpAliasConflictTimeoutCb, NULL, devAnnceDelay);
            return TRUE;
        }
    }

    return FALSE;
}

/*********************************************************************
 * @fn      gpDataCnfPrc
 *
 * @brief   On receipt of GP-DATA.confirm primitive.
 *
 * @param   pGpDataCnf
 *
 * @return  None
 */
void gpDataCnfPrc(gp_data_cnf_t *pGpDataCnf)
{
    //printf("gpDataCnf: sta = %x, handle = %x\n", pGpDataCnf->status, pGpDataCnf->gpepHandle);

    switch (pGpDataCnf->status) {
    case GP_DATA_CNF_STATUS_TX_QUEUE_FULL:
        //printf("txQueueFull\n");
        break;
    case GP_DATA_CNF_STATUS_ENTRY_REPLACED:
        //printf("entryReplaced\n");
        break;
    case GP_DATA_CNF_STATUS_ENTRY_ADDED:
        //printf("entryAdded\n");
        break;
    case GP_DATA_CNF_STATUS_ENTRY_EXPIRED:
        //printf("entryExpired\n");
        break;
    case GP_DATA_CNF_STATUS_ENTRY_REMOVED:
        //printf("entryRemoved\n");
        break;
    case GP_DATA_CNF_STATUS_GPDF_SENDING_FINALIZED:
        //printf("GPDF_sendingFinalized\n");
        break;
    default:
        break;
    }
}

/*********************************************************************
 * @fn      gpDataIndPrc
 *
 * @brief   On receipt of GP-DATA.indication primitive.
 *
 * @param   arg
 *
 * @return  None
 */
void gpDataIndPrc(void *arg)
{
    gp_data_ind_t *pGpDataInd = (gp_data_ind_t *)arg;

    //printf("gpDataIndPrc: status = %x\n", pGpDataInd->status);

#if GP_BASIC_COMBO
    gpsGpDataIndProcess(pGpDataInd);
#endif
    gppGpDataIndProcess(pGpDataInd);
}

/*********************************************************************
 * @fn      gpSecReqPrc
 *
 * @brief   On receipt of GP-SEC.request primitive.
 *
 * @param   arg
 *
 * @return  None
 */
void gpSecReqPrc(void *arg)
{
    gp_sec_req_t *pGpSecReq = (gp_sec_req_t *)arg;

    //printf("gpSecReqPrc: type = %x, lvl = %x, num = %x, handle = %x\n",
    //       pGpSecReq->gpdfKeyType, pGpSecReq->gpdfSecurityLevel,
    //       pGpSecReq->gpdSecFrameCnt, pGpSecReq->dgpStubHandle);

    gp_sec_req_t gpSecReq;
    memcpy((u8 *)&gpSecReq, pGpSecReq, sizeof(*pGpSecReq));

    gp_sec_rsp_t *pGpSecRsp = (gp_sec_rsp_t *)arg;
    memset((u8 *)pGpSecRsp, 0, sizeof(gp_sec_rsp_t));

    pGpSecRsp->appId = gpSecReq.appId;
    pGpSecRsp->dgpStubHandle = gpSecReq.dgpStubHandle;
    pGpSecRsp->endpoint = gpSecReq.endpoint;
    memcpy((u8 *)&(pGpSecRsp->gpdId), (u8 *)&gpSecReq.gpdId, sizeof(gpdId_t));
    pGpSecRsp->gpdfSecurityLevel = gpSecReq.gpdfSecurityLevel;
    pGpSecRsp->gpdSecFrameCnt = gpSecReq.gpdSecFrameCnt;

    //A.3.7.3.1.2
#if GP_BASIC_COMBO
    if (g_gpsCtx.gpsInCommMode || gp_getSinkTabByGpdId(gpSecReq.appId, gpSecReq.gpdId)) {
        pGpSecRsp->status = gpSinkSecOperation(gpSecReq.appId, gpSecReq.gpdId,
                                               gpSecReq.endpoint, gpSecReq.gpdSecFrameCnt,
                                               gpSecReq.gpdfSecurityLevel, gpSecReq.gpdfKeyType,
                                               &pGpSecRsp->gpdfKeyType, pGpSecRsp->gpdKey);

        TL_SCHEDULE_TASK(gpSecRsp, arg);

        return;
    }
#endif

    pGpSecRsp->status = gpProxySecOperation(gpSecReq.appId, gpSecReq.gpdId,
                                            gpSecReq.endpoint, gpSecReq.gpdSecFrameCnt,
                                            gpSecReq.gpdfSecurityLevel, gpSecReq.gpdfKeyType,
                                            &pGpSecRsp->gpdfKeyType, pGpSecRsp->gpdKey);

    TL_SCHEDULE_TASK(gpSecRsp, arg);
}

/*********************************************************************
 * @fn      zcl_gpCb
 *
 * @brief	Handler for ZCL Green Power command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t zcl_gpCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
    status_t status = ZCL_STA_SUCCESS;

    if (pAddrInfo->dstEp == GREEN_POWER_ENDPOINT) {
        if (pAddrInfo->dirCluster == ZCL_FRAME_SERVER_CLIENT_DIR) {
            status = zcl_gpServerCmdHandler(pAddrInfo, cmdId, cmdPayload);
        } else {
#if GP_BASIC_COMBO
            status = zcl_gpClientCmdHandler(pAddrInfo, cmdId, cmdPayload);
#else
            status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
#endif
        }
    }

    return status;
}

#endif /* GP_SUPPORT_ENABLE */
