/********************************************************************************************************
 * @file    zcl_sampleSensorCb.c
 *
 * @brief   This is the source file for zcl_sampleSensorCb
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
#if (__PROJECT_TL_CONTACT_SENSOR__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleSensor.h"
#include "app_ui.h"


/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */
#ifdef ZCL_READ
static void sampleSensor_zclReadRspCmd(zclIncoming_t *pInMsg);
#endif
#ifdef ZCL_WRITE
static void sampleSensor_zclWriteRspCmd(zclIncoming_t *pInMsg);
static void sampleSensor_zclWriteReqCmd(zclIncoming_t *pInMsg);
#endif
#ifdef ZCL_REPORT
static void sampleSensor_zclCfgReportCmd(zclIncoming_t *pInMsg);
static void sampleSensor_zclCfgReportRspCmd(zclIncoming_t *pInMsg);
static void sampleSensor_zclReportCmd(zclIncoming_t *pInMsg);
#endif
static void sampleSensor_zclDfltRspCmd(zclIncoming_t *pInMsg);


/**********************************************************************
 * GLOBAL VARIABLES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */
#ifdef ZCL_IDENTIFY
static ev_timer_event_t *identifyTimerEvt = NULL;
#endif
#ifdef ZCL_IAS_ZONE
static ev_timer_event_t *zclEnrollTimerEvt = NULL;
#endif
#ifdef ZCL_POLL_CTRL
static ev_timer_event_t *zclFastPollTimeoutTimerEvt = NULL;
static ev_timer_event_t *zclCheckInTimerEvt = NULL;
#endif

/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      sampleSensor_zclProcessIncomingMsg
 *
 * @brief   Process ZCL Foundation incoming message.
 *
 * @param   pInMsg - pointer to the received message
 *
 * @return  None
 */
void sampleSensor_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg)
{
    //printf("sampleSensor_zclProcessIncomingMsg\n");

    switch (pInHdlrMsg->hdr.cmd) {
#ifdef ZCL_READ
    case ZCL_CMD_READ_RSP:
        sampleSensor_zclReadRspCmd(pInHdlrMsg);
        break;
#endif
#ifdef ZCL_WRITE
    case ZCL_CMD_WRITE_RSP:
        sampleSensor_zclWriteRspCmd(pInHdlrMsg);
        break;
    case ZCL_CMD_WRITE:
    case ZCL_CMD_WRITE_NO_RSP:
        sampleSensor_zclWriteReqCmd(pInHdlrMsg);
        break;
#endif
#ifdef ZCL_REPORT
    case ZCL_CMD_CONFIG_REPORT:
        sampleSensor_zclCfgReportCmd(pInHdlrMsg);
        break;
    case ZCL_CMD_CONFIG_REPORT_RSP:
        sampleSensor_zclCfgReportRspCmd(pInHdlrMsg);
        break;
    case ZCL_CMD_REPORT:
        sampleSensor_zclReportCmd(pInHdlrMsg);
        break;
#endif
    case ZCL_CMD_DEFAULT_RSP:
        sampleSensor_zclDfltRspCmd(pInHdlrMsg);
        break;
    default:
        break;
    }
}

#ifdef ZCL_READ
/*********************************************************************
 * @fn      sampleSensor_zclReadRspCmd
 *
 * @brief   Handler for ZCL Read Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleSensor_zclReadRspCmd(zclIncoming_t *pInMsg)
{
    zclReadRspCmd_t *pReadRspCmd = (zclReadRspCmd_t *)pInMsg->attrCmd;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t *)pInMsg->msg;
    u16 clusterId = pApsdeInd->indInfo.cluster_id;

    (void)pReadRspCmd;
    (void)clusterId;

    //printf("sampleSensor_zclReadRspCmd\n");
}
#endif	/* ZCL_READ */

#ifdef ZCL_WRITE
/*********************************************************************
 * @fn      sampleSensor_zclWriteRspCmd
 *
 * @brief   Handler for ZCL Write Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleSensor_zclWriteRspCmd(zclIncoming_t *pInMsg)
{
    zclWriteRspCmd_t *pWriteRspCmd = (zclWriteRspCmd_t *)pInMsg->attrCmd;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t *)pInMsg->msg;
    u16 clusterId = pApsdeInd->indInfo.cluster_id;

    (void)pWriteRspCmd;
    (void)clusterId;

    //printf("sampleSensor_zclWriteRspCmd\n");
}

/*********************************************************************
 * @fn      sampleSensor_zclWriteReqCmd
 *
 * @brief   Handler for ZCL Write Request command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleSensor_zclWriteReqCmd(zclIncoming_t *pInMsg)
{
    zclWriteCmd_t *pWriteReqCmd = (zclWriteCmd_t *)pInMsg->attrCmd;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t *)pInMsg->msg;
    u16 clusterId = pApsdeInd->indInfo.cluster_id;
    u8 numAttr = pWriteReqCmd->numAttr;
    zclWriteRec_t *attr = pWriteReqCmd->attrList;

#ifdef ZCL_IDENTIFY
    if (clusterId == ZCL_CLUSTER_GEN_IDENTIFY) {
        for (s32 i = 0; i < numAttr; i++) {
            if (attr[i].attrID == ZCL_ATTRID_IDENTIFY_TIME) {
                sampleSensor_zclIdentifyCmdHandler(SAMPLE_SENSOR_ENDPOINT, 0xFFFE, g_zcl_identifyAttrs.identifyTime);
            }
        }
    } else
#endif
#ifdef ZCL_IAS_ZONE
    if (clusterId == ZCL_CLUSTER_SS_IAS_ZONE) {
        for (s32 i = 0; i < numAttr; i++) {
            if (attr[i].attrID == ZCL_ATTRID_IAS_CIE_ADDR) {
                sampleSensor_iasZoneEnrollStart();
            }
        }
    }
#endif
#ifdef ZCL_POLL_CTRL
    if (clusterId == ZCL_CLUSTER_GEN_POLL_CONTROL) {
        for (s32 i = 0; i < numAttr; i++) {
            if (attr[i].attrID == ZCL_ATTRID_CHK_IN_INTERVAL) {
                sampleSensor_zclCheckInStart();
                return;
            }
        }
    } else
#endif
    {

    }
}
#endif	/* ZCL_WRITE */

/*********************************************************************
 * @fn      sampleSensor_zclDfltRspCmd
 *
 * @brief   Handler for ZCL Default Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleSensor_zclDfltRspCmd(zclIncoming_t *pInMsg)
{
    zclDefaultRspCmd_t *pDftRspCmd = (zclDefaultRspCmd_t *)pInMsg->attrCmd;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t *)pInMsg->msg;
    u16 clusterId = pApsdeInd->indInfo.cluster_id;

    //printf("sampleSensor_zclDfltRspCmd\n");

    if (pDftRspCmd->statusCode == ZCL_STA_SUCCESS) {
        if (clusterId == ZCL_CLUSTER_GEN_IDENTIFY) {
            if (pDftRspCmd->commandID == ZCL_CMD_TRIGGER_EFFECT) {
                epInfo_t dstEpInfo;
                TL_SETSTRUCTCONTENT(dstEpInfo, 0);

                dstEpInfo.profileId = pApsdeInd->indInfo.profile_id;
                dstEpInfo.dstEp = pApsdeInd->indInfo.src_ep;
                dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
                dstEpInfo.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;

                zcl_group_removeAllCmd(SAMPLE_SENSOR_ENDPOINT, &dstEpInfo, FALSE);
            }
        } else if (clusterId == ZCL_CLUSTER_GEN_GROUPS) {
            if (pDftRspCmd->commandID == ZCL_CMD_GROUP_REMOVE_ALL_GROUP) {
                epInfo_t dstEpInfo;
                TL_SETSTRUCTCONTENT(dstEpInfo, 0);

                dstEpInfo.profileId = pApsdeInd->indInfo.profile_id;
                dstEpInfo.dstEp = pApsdeInd->indInfo.src_ep;
                dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
                dstEpInfo.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;

                u16 groupID = 0x1234;
                u8 groupName[8] = {0};
                zcl_group_addCmd(SAMPLE_SENSOR_ENDPOINT, &dstEpInfo, TRUE, groupID, groupName);
            }
        }
    }
}

#ifdef ZCL_REPORT
/*********************************************************************
 * @fn      sampleSensor_zclCfgReportCmd
 *
 * @brief   Handler for ZCL Configure Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleSensor_zclCfgReportCmd(zclIncoming_t *pInMsg)
{
    zclCfgReportCmd_t *pCfgReportCmd = (zclCfgReportCmd_t *)pInMsg->attrCmd;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t *)pInMsg->msg;
    u16 clusterId = pApsdeInd->indInfo.cluster_id;

    (void)pCfgReportCmd;
    (void)clusterId;

    //printf("sampleSensor_zclCfgReportCmd\n");
}

/*********************************************************************
 * @fn      sampleSensor_zclCfgReportRspCmd
 *
 * @brief   Handler for ZCL Configure Report Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleSensor_zclCfgReportRspCmd(zclIncoming_t *pInMsg)
{
    zclCfgReportRspCmd_t *pCfgReportRspCmd = (zclCfgReportRspCmd_t *)pInMsg->attrCmd;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t *)pInMsg->msg;
    u16 clusterId = pApsdeInd->indInfo.cluster_id;

    (void)pCfgReportRspCmd;
    (void)clusterId;

    //printf("sampleSensor_zclCfgReportRspCmd\n");
}

/*********************************************************************
 * @fn      sampleSensor_zclReportCmd
 *
 * @brief   Handler for ZCL Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleSensor_zclReportCmd(zclIncoming_t *pInMsg)
{
    zclReportCmd_t *pReportCmd = (zclReportCmd_t *)pInMsg->attrCmd;
    apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t *)pInMsg->msg;
    u16 clusterId = pApsdeInd->indInfo.cluster_id;

    (void)pReportCmd;
    (void)clusterId;

    //printf("sampleSensor_zclReportCmd\n");
}
#endif	/* ZCL_REPORT */

#ifdef ZCL_BASIC
/*********************************************************************
 * @fn      sampleSensor_basicCb
 *
 * @brief   Handler for ZCL Basic Reset command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t sampleSensor_basicCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
    if (cmdId == ZCL_CMD_BASIC_RESET_FAC_DEFAULT) {
        //Reset all the attributes of all its clusters to factory defaults
        //zcl_nv_attr_reset();
    }

    return ZCL_STA_UNSUP_CLUSTER_COMMAND;
}
#endif	/* ZCL_BASIC */

#ifdef ZCL_IDENTIFY
static s32 sampleSensor_zclIdentifyTimerCb(void *arg)
{
    if (g_zcl_identifyAttrs.identifyTime <= 0) {
        light_blink_stop();

        identifyTimerEvt = NULL;
        return -1;
    }
    g_zcl_identifyAttrs.identifyTime--;
    return 0;
}

static void sampleSensor_zclIdentifyTimerStop(void)
{
    if (identifyTimerEvt) {
        TL_ZB_TIMER_CANCEL(&identifyTimerEvt);
    }
}

/*********************************************************************
 * @fn      sampleSensor_zclIdentifyCmdHandler
 *
 * @brief   Handler for ZCL Identify command. This function will set blink LED.
 *
 * @param   endpoint
 * @param   srcAddr
 * @param   identifyTime - identify time
 *
 * @return  None
 */
void sampleSensor_zclIdentifyCmdHandler(u8 endpoint, u16 srcAddr, u16 identifyTime)
{
    g_zcl_identifyAttrs.identifyTime = identifyTime;

    if (identifyTime == 0) {
        sampleSensor_zclIdentifyTimerStop();
        light_blink_stop();
    } else {
        if (!identifyTimerEvt) {
            light_blink_start(identifyTime, 500, 500);
            identifyTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleSensor_zclIdentifyTimerCb, NULL, 1000);
        }
    }
}

/*********************************************************************
 * @fn      sampleSensor_zcltriggerCmdHandler
 *
 * @brief   Handler for ZCL trigger command.
 *
 * @param   pTriggerEffect
 *
 * @return  None
 */
static void sampleSensor_zcltriggerCmdHandler(zcl_triggerEffect_t *pTriggerEffect)
{
    u8 effectId = pTriggerEffect->effectId;
    //u8 effectVariant = pTriggerEffect->effectVariant;

    switch (effectId) {
    case IDENTIFY_EFFECT_BLINK:
        light_blink_start(1, 500, 500);
        break;
    case IDENTIFY_EFFECT_BREATHE:
        light_blink_start(15, 300, 700);
        break;
    case IDENTIFY_EFFECT_OKAY:
        light_blink_start(2, 250, 250);
        break;
    case IDENTIFY_EFFECT_CHANNEL_CHANGE:
        light_blink_start(1, 500, 7500);
        break;
    case IDENTIFY_EFFECT_FINISH_EFFECT:
        light_blink_start(1, 300, 700);
        break;
    case IDENTIFY_EFFECT_STOP_EFFECT:
        light_blink_stop();
        break;
    default:
        break;
    }
}

/*********************************************************************
 * @fn      sampleSensor_zclIdentifyQueryRspCmdHandler
 *
 * @brief   Handler for ZCL Identify Query response command.
 *
 * @param   endpoint
 * @param   srcAddr
 * @param   identifyRsp
 *
 * @return  None
 */
static void sampleSensor_zclIdentifyQueryRspCmdHandler(u8 endpoint, u16 srcAddr, zcl_identifyRspCmd_t *identifyRsp)
{
    if (identifyRsp->timeout) {
        findBindDst_t dstInfo;
        dstInfo.addr = srcAddr;
        dstInfo.endpoint = endpoint;

        bdb_addIdentifyActiveEpForFB(dstInfo);
    }
}

/*********************************************************************
 * @fn      sampleSensor_identifyCb
 *
 * @brief   Handler for ZCL Identify command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t sampleSensor_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
    if (pAddrInfo->dstEp == SAMPLE_SENSOR_ENDPOINT) {
        if (pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR) {
            switch (cmdId) {
            case ZCL_CMD_IDENTIFY:
                sampleSensor_zclIdentifyCmdHandler(pAddrInfo->dstEp, pAddrInfo->srcAddr, ((zcl_identifyCmd_t *)cmdPayload)->identifyTime);
                break;
            case ZCL_CMD_TRIGGER_EFFECT:
                sampleSensor_zcltriggerCmdHandler((zcl_triggerEffect_t *)cmdPayload);
                break;
            default:
                break;
            }
        } else {
            if (cmdId == ZCL_CMD_IDENTIFY_QUERY_RSP) {
                sampleSensor_zclIdentifyQueryRspCmdHandler(pAddrInfo->dstEp, pAddrInfo->srcAddr, (zcl_identifyRspCmd_t *)cmdPayload);
            }
        }
    }

    return ZCL_STA_SUCCESS;
}
#endif	/* ZCL_IDENTIFY */

#ifdef ZCL_GROUP
/*********************************************************************
 * @fn      sampleSensor_zclAddGroupRspCmdHandler
 *
 * @brief   Handler for ZCL add group response command.
 *
 * @param   pAddGroupRsp
 *
 * @return  None
 */
static void sampleSensor_zclAddGroupRspCmdHandler(zcl_addGroupRsp_t *pAddGroupRsp)
{

}

/*********************************************************************
 * @fn      sampleSensor_zclViewGroupRspCmdHandler
 *
 * @brief   Handler for ZCL view group response command.
 *
 * @param   pViewGroupRsp
 *
 * @return  None
 */
static void sampleSensor_zclViewGroupRspCmdHandler(zcl_viewGroupRsp_t *pViewGroupRsp)
{

}

/*********************************************************************
 * @fn      sampleSensor_zclRemoveGroupRspCmdHandler
 *
 * @brief   Handler for ZCL remove group response command.
 *
 * @param   pRemoveGroupRsp
 *
 * @return  None
 */
static void sampleSensor_zclRemoveGroupRspCmdHandler(zcl_removeGroupRsp_t *pRemoveGroupRsp)
{

}

/*********************************************************************
 * @fn      sampleSensor_zclGetGroupMembershipRspCmdHandler
 *
 * @brief   Handler for ZCL get group membership response command.
 *
 * @param   pGetGroupMembershipRsp
 *
 * @return  None
 */
static void sampleSensor_zclGetGroupMembershipRspCmdHandler(zcl_getGroupMembershipRsp_t *pGetGroupMembershipRsp)
{

}

/*********************************************************************
 * @fn      sampleSensor_groupCb
 *
 * @brief   Handler for ZCL Group command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t sampleSensor_groupCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
    if (pAddrInfo->dstEp == SAMPLE_SENSOR_ENDPOINT) {
        if (pAddrInfo->dirCluster == ZCL_FRAME_SERVER_CLIENT_DIR) {
            switch (cmdId) {
            case ZCL_CMD_GROUP_ADD_GROUP_RSP:
                sampleSensor_zclAddGroupRspCmdHandler((zcl_addGroupRsp_t *)cmdPayload);
                break;
            case ZCL_CMD_GROUP_VIEW_GROUP_RSP:
                sampleSensor_zclViewGroupRspCmdHandler((zcl_viewGroupRsp_t *)cmdPayload);
                break;
            case ZCL_CMD_GROUP_REMOVE_GROUP_RSP:
                sampleSensor_zclRemoveGroupRspCmdHandler((zcl_removeGroupRsp_t *)cmdPayload);
                break;
            case ZCL_CMD_GROUP_GET_MEMBERSHIP_RSP:
                sampleSensor_zclGetGroupMembershipRspCmdHandler((zcl_getGroupMembershipRsp_t *)cmdPayload);
                break;
            default:
                break;
            }
        }
    }

    return ZCL_STA_SUCCESS;
}
#endif	/* ZCL_GROUP */

#ifdef ZCL_IAS_ZONE
static void sampleSensor_iasZoneEnrollReqCmdSend(u16 dstAddr)
{
    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.profileId = HA_PROFILE_ID;
    dstEpInfo.dstEp = 0xFF;
    dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
    dstEpInfo.dstAddr.shortAddr = dstAddr;

    zoneEnrollReq_t zoneEnrollReq;
    zoneEnrollReq.zoneType = g_zcl_iasZoneAttrs.zoneType;
    zoneEnrollReq.manufacturerCode = MANUFACTURER_CODE_TELINK;

    printf("zcl send: Zone EnrollReq\n");
    zcl_iasZone_enrollReqCmd(SAMPLE_SENSOR_ENDPOINT, &dstEpInfo, FALSE, &zoneEnrollReq);
}

static void sampleSensor_iasZoneNwkAddrReqCb(void *arg)
{
    zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)arg;
    zdo_nwk_addr_resp_t *rsp = (zdo_nwk_addr_resp_t *)p->zpdu;

    if (rsp->status == ZDO_SUCCESS) {
        sampleSensor_iasZoneEnrollReqCmdSend(rsp->nwk_addr_remote);
    }
}

static s32 sampleSensor_iasZoneEnrollTimerCb(void *arg)
{
    u16 dstAddr = NWK_BROADCAST_RESERVED;

    if (g_zcl_iasZoneAttrs.zoneState == ZONE_STATE_ENROLLED) {
        zclEnrollTimerEvt = NULL;
        return -1;
    }

    if (zb_getNwkAddrByExtAddr(g_zcl_iasZoneAttrs.iasCieAddr, &dstAddr) == SUCCESS) {
        sampleSensor_iasZoneEnrollReqCmdSend(dstAddr);
    } else {
        u8 sn = 0;
        zdo_nwk_addr_req_t req;
        ZB_IEEE_ADDR_COPY(req.ieee_addr_interest, g_zcl_iasZoneAttrs.iasCieAddr);
        req.req_type = ZDO_ADDR_REQ_SINGLE_RESP;
        req.start_index = 0;
        zb_zdoNwkAddrReq(NWK_BROADCAST_ROUTER_COORDINATOR, &req, &sn, sampleSensor_iasZoneNwkAddrReqCb);
    }

    return 3000;
}

void sampleSensor_iasZoneEnrollStart(void)
{
    if (ZB_IEEE_ADDR_IS_INVALID(g_zcl_iasZoneAttrs.iasCieAddr) ||
       (ZB_IEEE_ADDR_IS_ZERO(g_zcl_iasZoneAttrs.iasCieAddr))) {
        return;
    }

    if (zclEnrollTimerEvt) {
        TL_ZB_TIMER_CANCEL(&zclEnrollTimerEvt);
    }
    zclEnrollTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleSensor_iasZoneEnrollTimerCb, NULL, 1000);
}

/*********************************************************************
 * @fn      sampleSensor_zclIasZoneEnrollRspCmdHandler
 *
 * @brief   Handler for ZCL IAS ZONE Enroll response command.
 *
 * @param   pZoneEnrollRsp
 *
 * @return  None
 */
static void sampleSensor_zclIasZoneEnrollRspCmdHandler(zclIncomingAddrInfo_t *pAddrInfo, zoneEnrollRsp_t *pZoneEnrollRsp)
{
    if (pZoneEnrollRsp->code == ZONE_ENROLL_SUCC) {
        addrExt_t extAddr;
        if (zb_getExtAddrByNwkAddr(pAddrInfo->srcAddr, extAddr) == SUCCESS) {
            if (ZB_IEEE_ADDR_CMP(extAddr, g_zcl_iasZoneAttrs.iasCieAddr) &&
               (g_zcl_iasZoneAttrs.zoneState == ZONE_STATE_NOT_ENROLLED)) {
                g_zcl_iasZoneAttrs.zoneState = ZONE_STATE_ENROLLED;
                g_zcl_iasZoneAttrs.zoneId = pZoneEnrollRsp->zoneId;

                u8 sn = 0;
                zdo_bind_req_t req;
                memset(&req, 0, sizeof(zdo_bind_req_t));

                ZB_IEEE_ADDR_COPY(req.src_addr, NIB_IEEE_ADDRESS());
                req.src_endpoint = pAddrInfo->dstEp;
                req.dst_addr_mode = LONG_EXADDR_DSTENDPOINT;
                ZB_IEEE_ADDR_COPY(req.dst_ext_addr, g_zcl_iasZoneAttrs.iasCieAddr);
                req.dst_endpoint = pAddrInfo->srcEp;
                req.cid16_l = LO_UINT16(ZCL_CLUSTER_SS_IAS_ZONE);
                req.cid16_h = HI_UINT16(ZCL_CLUSTER_SS_IAS_ZONE);

                zb_zdoBindUnbindReq(TRUE, &req, &sn, NULL);

                zcl_iasZoneAttr_save();
            }
        }
    }
}

/*********************************************************************
 * @fn      sampleSensor_zclIasZoneInitNormalOperationModeCmdHandler
 *
 * @brief   Handler for ZCL IAS ZONE normal operation mode command.
 *
 * @param
 *
 * @return  status
 */
static status_t sampleSensor_zclIasZoneInitNormalOperationModeCmdHandler(void)
{
    u8 status = ZCL_STA_UNSUP_CLUSTER_COMMAND;

    return status;
}

/*********************************************************************
 * @fn      sampleSensor_zclIasZoneInitTestModeCmdHandler
 *
 * @brief   Handler for ZCL IAS ZONE test mode command.
 *
 * @param   pZoneInitTestMode
 *
 * @return  status
 */
static status_t sampleSensor_zclIasZoneInitTestModeCmdHandler(zoneInitTestMode_t *pZoneInitTestMode)
{
    u8 status = ZCL_STA_UNSUP_CLUSTER_COMMAND;

    return status;
}

/*********************************************************************
 * @fn      sampleSensor_iasZoneCb
 *
 * @brief   Handler for ZCL IAS Zone command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t sampleSensor_iasZoneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
    status_t status = ZCL_STA_SUCCESS;

    if (pAddrInfo->dstEp == SAMPLE_SENSOR_ENDPOINT) {
        if (pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR) {
            switch (cmdId) {
            case ZCL_CMD_ZONE_ENROLL_RSP:
                sampleSensor_zclIasZoneEnrollRspCmdHandler(pAddrInfo, (zoneEnrollRsp_t *)cmdPayload);
                break;
            case ZCL_CMD_INIT_NORMAL_OPERATION_MODE:
                status = sampleSensor_zclIasZoneInitNormalOperationModeCmdHandler();
                break;
            case ZCL_CMD_INIT_TEST_MODE:
                status = sampleSensor_zclIasZoneInitTestModeCmdHandler((zoneInitTestMode_t *)cmdPayload);
                break;
            default:
                break;
            }
        }
    }

    return status;
}
#endif  /* ZCL_IAS_ZONE */

#ifdef ZCL_POLL_CTRL
static void sampleSensor_zclCheckInCmdSend(void)
{
    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
    dstEpInfo.dstEp = SAMPLE_SENSOR_ENDPOINT;
    dstEpInfo.profileId = HA_PROFILE_ID;

    zcl_pollCtrl_checkInCmd(SAMPLE_SENSOR_ENDPOINT, &dstEpInfo, TRUE);
}

static s32 sampleSensor_zclCheckInTimerCb(void *arg)
{
    zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

    if (!pPollCtrlAttr->chkInInterval) {
        zclCheckInTimerEvt = NULL;
        return -1;
    }

    sampleSensor_zclCheckInCmdSend();

    return 0;
}

void sampleSensor_zclCheckInStart(void)
{
    zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

    if (pPollCtrlAttr->chkInInterval) {
        if (zb_bindingTblSearched(ZCL_CLUSTER_GEN_POLL_CONTROL, SAMPLE_SENSOR_ENDPOINT)) {
            sampleSensor_zclCheckInCmdSend();

            if (zclCheckInTimerEvt) {
                TL_ZB_TIMER_CANCEL(&zclCheckInTimerEvt);
            }
            zclCheckInTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleSensor_zclCheckInTimerCb, NULL,
                                                      pPollCtrlAttr->chkInInterval * POLL_RATE_QUARTERSECONDS);
        }
    }
}

static void sampleSensor_zclSetFastPollMode(bool fastPollMode)
{
    zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

    u32 pollRate = fastPollMode ? pPollCtrlAttr->shortPollInterval : pPollCtrlAttr->longPollInterval;

    zb_setPollRate(pollRate * POLL_RATE_QUARTERSECONDS);
}

static s32 sampleSensor_zclFastPollTimeoutCb(void *arg)
{
    sampleSensor_zclSetFastPollMode(FALSE);

    zclFastPollTimeoutTimerEvt = NULL;
    return -1;
}

static status_t sampleSensor_zclPollCtrlChkInRspCmdHandler(zcl_chkInRsp_t *pCmd)
{
    zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

    if (!zb_bindingTblSearched(ZCL_CLUSTER_GEN_POLL_CONTROL, SAMPLE_SENSOR_ENDPOINT)) {
        return ZCL_STA_ACTION_DENIED;
    }

    if (pCmd->startFastPolling) {
        u16 fastPollTimeoutCnt = 0;

        if (pCmd->fastPollTimeout) {
            if (pCmd->fastPollTimeout > pPollCtrlAttr->fastPollTimeoutMax) {
                return ZCL_STA_INVALID_FIELD;
            }

            fastPollTimeoutCnt = pCmd->fastPollTimeout;
        } else {
            fastPollTimeoutCnt = pPollCtrlAttr->fastPollTimeout;
        }

        if (fastPollTimeoutCnt) {
            sampleSensor_zclSetFastPollMode(TRUE);

            if (zclFastPollTimeoutTimerEvt) {
                TL_ZB_TIMER_CANCEL(&zclFastPollTimeoutTimerEvt);
            }
            zclFastPollTimeoutTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleSensor_zclFastPollTimeoutCb, NULL,
                                                              fastPollTimeoutCnt * POLL_RATE_QUARTERSECONDS);
        }
    } else {
        //continue in normal operation and not required to go into fast poll mode.
    }

    return ZCL_STA_SUCCESS;
}

static status_t sampleSensor_zclPollCtrlFastPollStopCmdHandler(void)
{
    if (!zb_bindingTblSearched(ZCL_CLUSTER_GEN_POLL_CONTROL, SAMPLE_SENSOR_ENDPOINT)) {
        return ZCL_STA_ACTION_DENIED;
    }

    if (!zclFastPollTimeoutTimerEvt) {
        return ZCL_STA_ACTION_DENIED;
    } else {
        if (zclFastPollTimeoutTimerEvt) {
            TL_ZB_TIMER_CANCEL(&zclFastPollTimeoutTimerEvt);
        }
        sampleSensor_zclSetFastPollMode(FALSE);
    }

    return ZCL_STA_SUCCESS;
}

static status_t sampleSensor_zclPollCtrlSetLongPollIntervalCmdHandler(zcl_setLongPollInterval_t *pCmd)
{
    zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

    if ((pCmd->newLongPollInterval >= 0x04) &&
        (pCmd->newLongPollInterval <= 0x6E0000) &&
        (pCmd->newLongPollInterval <= pPollCtrlAttr->chkInInterval) &&
        (pCmd->newLongPollInterval >= pPollCtrlAttr->shortPollInterval)) {
        pPollCtrlAttr->longPollInterval = pCmd->newLongPollInterval;
    } else {
        return ZCL_STA_INVALID_VALUE;
    }

    return ZCL_STA_SUCCESS;
}

static status_t sampleSensor_zclPollCtrlSetShortPollIntervalCmdHandler(zcl_setShortPollInterval_t *pCmd)
{
    zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

    if ((pCmd->newShortPollInterval >= 0x01) &&
        (pCmd->newShortPollInterval <= 0xffff) &&
        (pCmd->newShortPollInterval <= pPollCtrlAttr->longPollInterval)) {
        pPollCtrlAttr->shortPollInterval = pCmd->newShortPollInterval;
    } else {
        return ZCL_STA_INVALID_VALUE;
    }

    return ZCL_STA_SUCCESS;
}

/*********************************************************************
 * @fn      sampleSensor_pollCtrlCb
 *
 * @brief   Handler for ZCL Poll Control command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t sampleSensor_pollCtrlCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
    status_t status = ZCL_STA_SUCCESS;

    if (pAddrInfo->dstEp == SAMPLE_SENSOR_ENDPOINT) {
        if (pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR) {
            switch (cmdId) {
            case ZCL_CMD_CHK_IN_RSP:
                status = sampleSensor_zclPollCtrlChkInRspCmdHandler((zcl_chkInRsp_t *)cmdPayload);
                break;
            case ZCL_CMD_FAST_POLL_STOP:
                status = sampleSensor_zclPollCtrlFastPollStopCmdHandler();
                break;
            case ZCL_CMD_SET_LONG_POLL_INTERVAL:
                status = sampleSensor_zclPollCtrlSetLongPollIntervalCmdHandler((zcl_setLongPollInterval_t *)cmdPayload);
                break;
            case ZCL_CMD_SET_SHORT_POLL_INTERVAL:
                status = sampleSensor_zclPollCtrlSetShortPollIntervalCmdHandler((zcl_setShortPollInterval_t *)cmdPayload);
                break;
            default:
                break;
            }
        }
    }

    return status;
}
#endif	/* ZCL_POLL_CTRL */

#endif  /* __PROJECT_TL_CONTACT_SENSOR__ */
