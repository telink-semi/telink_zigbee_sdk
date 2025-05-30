/********************************************************************************************************
 * @file    zcl_sampleLightCb.c
 *
 * @brief   This is the source file for zcl_sampleLightCb
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
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
#if (__PROJECT_TL_DIMMABLE_LIGHT__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "ota.h"
#include "sampleLight.h"
#include "sampleLightCtrl.h"

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
static void sampleLight_zclReadRspCmd(zclReadRspCmd_t *pReadRspCmd);
#endif
#ifdef ZCL_WRITE
static void sampleLight_zclWriteReqCmd(u16 clusterId, zclWriteCmd_t *pWriteReqCmd);
static void sampleLight_zclWriteRspCmd(zclWriteRspCmd_t *pWriteRspCmd);
#endif
#ifdef ZCL_REPORT
static void sampleLight_zclCfgReportCmd(zclCfgReportCmd_t *pCfgReportCmd);
static void sampleLight_zclCfgReportRspCmd(zclCfgReportRspCmd_t *pCfgReportRspCmd);
static void sampleLight_zclReportCmd(zclReportCmd_t *pReportCmd);
#endif
static void sampleLight_zclDfltRspCmd(zclDefaultRspCmd_t *pDftRspCmd);


/**********************************************************************
 * GLOBAL VARIABLES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */
#ifdef ZCL_IDENTIFY
static ev_timer_event_t *identifyTimerEvt = NULL;
#endif


/**********************************************************************
 * FUNCTIONS
 */
void sampleLight_zclIdentifyCmdHandler(u8 endpoint, u16 srcAddr, u16 identifyTime);

/*********************************************************************
 * @fn      sampleLight_zclProcessIncomingMsg
 *
 * @brief   Process ZCL Foundation incoming message.
 *
 * @param   pInMsg - pointer to the received message
 *
 * @return  None
 */
void sampleLight_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg)
{
    //printf("sampleLight_zclProcessIncomingMsg\n");

    switch (pInHdlrMsg->hdr.cmd) {
#ifdef ZCL_READ
    case ZCL_CMD_READ_RSP:
        sampleLight_zclReadRspCmd(pInHdlrMsg->attrCmd);
        break;
#endif
#ifdef ZCL_WRITE
    case ZCL_CMD_WRITE:
    case ZCL_CMD_WRITE_NO_RSP:
        sampleLight_zclWriteReqCmd(pInHdlrMsg->msg->indInfo.cluster_id, pInHdlrMsg->attrCmd);
        break;
    case ZCL_CMD_WRITE_RSP:
        sampleLight_zclWriteRspCmd(pInHdlrMsg->attrCmd);
        break;
#endif
#ifdef ZCL_REPORT
    case ZCL_CMD_CONFIG_REPORT:
        sampleLight_zclCfgReportCmd(pInHdlrMsg->attrCmd);
        break;
    case ZCL_CMD_CONFIG_REPORT_RSP:
        sampleLight_zclCfgReportRspCmd(pInHdlrMsg->attrCmd);
        break;
    case ZCL_CMD_REPORT:
        sampleLight_zclReportCmd(pInHdlrMsg->attrCmd);
        break;
#endif
    case ZCL_CMD_DEFAULT_RSP:
        sampleLight_zclDfltRspCmd(pInHdlrMsg->attrCmd);
        break;
    default:
        break;
    }
}

#ifdef ZCL_READ
/*********************************************************************
 * @fn      sampleLight_zclReadRspCmd
 *
 * @brief   Handler for ZCL Read Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleLight_zclReadRspCmd(zclReadRspCmd_t *pReadRspCmd)
{
    //printf("sampleLight_zclReadRspCmd\n");
}
#endif

#ifdef ZCL_WRITE
/*********************************************************************
 * @fn      sampleLight_zclWriteReqCmd
 *
 * @brief   Handler for ZCL Write Request command.
 *
 * @param
 *
 * @return  None
 */
static void sampleLight_zclWriteReqCmd(u16 clusterId, zclWriteCmd_t *pWriteReqCmd)
{
    u8 numAttr = pWriteReqCmd->numAttr;
    zclWriteRec_t *attr = pWriteReqCmd->attrList;

    if (clusterId == ZCL_CLUSTER_GEN_ON_OFF) {
        for (u8 i = 0; i < numAttr; i++) {
            if (attr[i].attrID == ZCL_ATTRID_START_UP_ONOFF) {
                zcl_onOffAttr_save();
            }
        }
    } else if (clusterId == ZCL_CLUSTER_GEN_LEVEL_CONTROL) {
        for (u8 i = 0; i < numAttr; i++) {
            if (attr[i].attrID == ZCL_ATTRID_LEVEL_START_UP_CURRENT_LEVEL) {
                zcl_levelAttr_save();
            }
        }
    } else if (clusterId == ZCL_CLUSTER_LIGHTING_COLOR_CONTROL) {
        for (u8 i = 0; i < numAttr; i++) {
            if (attr[i].attrID == ZCL_ATTRID_START_UP_COLOR_TEMPERATURE_MIREDS) {
                zcl_colorCtrlAttr_save();
            }
        }
    } else if (clusterId == ZCL_CLUSTER_GEN_IDENTIFY) {
        for (u8 i = 0; i < numAttr; i++) {
            if (attr[i].attrID == ZCL_ATTRID_IDENTIFY_TIME) {
                sampleLight_zclIdentifyCmdHandler(SAMPLE_LIGHT_ENDPOINT, 0xFFFE, g_zcl_identifyAttrs.identifyTime);
            }
        }
    }
}

/*********************************************************************
 * @fn      sampleLight_zclWriteRspCmd
 *
 * @brief   Handler for ZCL Write Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleLight_zclWriteRspCmd(zclWriteRspCmd_t *pWriteRspCmd)
{
    //printf("sampleLight_zclWriteRspCmd\n");
}
#endif


/*********************************************************************
 * @fn      sampleLight_zclDfltRspCmd
 *
 * @brief   Handler for ZCL Default Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleLight_zclDfltRspCmd(zclDefaultRspCmd_t *pDftRspCmd)
{
    //printf("sampleLight_zclDfltRspCmd\n");
}

#ifdef ZCL_REPORT
/*********************************************************************
 * @fn      sampleLight_zclCfgReportCmd
 *
 * @brief   Handler for ZCL Configure Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleLight_zclCfgReportCmd(zclCfgReportCmd_t *pCfgReportCmd)
{
    //printf("sampleLight_zclCfgReportCmd\n");
}

/*********************************************************************
 * @fn      sampleLight_zclCfgReportRspCmd
 *
 * @brief   Handler for ZCL Configure Report Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleLight_zclCfgReportRspCmd(zclCfgReportRspCmd_t *pCfgReportRspCmd)
{
    //printf("sampleLight_zclCfgReportRspCmd\n");
}

/*********************************************************************
 * @fn      sampleLight_zclReportCmd
 *
 * @brief   Handler for ZCL Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void sampleLight_zclReportCmd(zclReportCmd_t *pReportCmd)
{
    //printf("sampleLight_zclReportCmd\n");
}
#endif

#ifdef ZCL_BASIC
/*********************************************************************
 * @fn      sampleLight_basicCb
 *
 * @brief   Handler for ZCL Basic Reset command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t sampleLight_basicCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
    if (cmdId == ZCL_CMD_BASIC_RESET_FAC_DEFAULT) {
        //Reset all the attributes of all its clusters to factory defaults
        //zcl_nv_attr_reset();
    }

    //return ZCL_STA_SUCCESS;
    return ZCL_STA_UNSUP_CLUSTER_COMMAND;
}
#endif

#ifdef ZCL_IDENTIFY
s32 sampleLight_zclIdentifyTimerCb(void *arg)
{
    if (g_zcl_identifyAttrs.identifyTime <= 0) {
        light_blink_stop();

        identifyTimerEvt = NULL;
        return -1;
    }
    g_zcl_identifyAttrs.identifyTime--;
    return 0;
}

void sampleLight_zclIdentifyTimerStop(void)
{
    if (identifyTimerEvt) {
        TL_ZB_TIMER_CANCEL(&identifyTimerEvt);
    }
}

/*********************************************************************
 * @fn      sampleLight_zclIdentifyCmdHandler
 *
 * @brief   Handler for ZCL Identify command. This function will set blink LED.
 *
 * @param   endpoint
 * @param   srcAddr
 * @param   identifyTime - identify time
 *
 * @return  None
 */
void sampleLight_zclIdentifyCmdHandler(u8 endpoint, u16 srcAddr, u16 identifyTime)
{
    g_zcl_identifyAttrs.identifyTime = identifyTime;

    if (identifyTime == 0) {
        sampleLight_zclIdentifyTimerStop();
        light_blink_stop();
    } else {
        if (!identifyTimerEvt) {
            light_blink_start(identifyTime, 500, 500);
            identifyTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleLight_zclIdentifyTimerCb, NULL, 1000);
        }
    }
}

/*********************************************************************
 * @fn      sampleLight_zcltriggerCmdHandler
 *
 * @brief   Handler for ZCL trigger command.
 *
 * @param   pTriggerEffect
 *
 * @return  None
 */
static void sampleLight_zcltriggerCmdHandler(zcl_triggerEffect_t *pTriggerEffect)
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
 * @fn      sampleLight_identifyCb
 *
 * @brief   Handler for ZCL Identify command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t sampleLight_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
    if (pAddrInfo->dstEp == SAMPLE_LIGHT_ENDPOINT) {
        if (pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR) {
            switch (cmdId) {
            case ZCL_CMD_IDENTIFY:
                sampleLight_zclIdentifyCmdHandler(pAddrInfo->dstEp, pAddrInfo->srcAddr, ((zcl_identifyCmd_t *)cmdPayload)->identifyTime);
                break;
            case ZCL_CMD_TRIGGER_EFFECT:
                sampleLight_zcltriggerCmdHandler((zcl_triggerEffect_t *)cmdPayload);
                break;
            default:
                break;
            }
        }
    }

    return ZCL_STA_SUCCESS;
}
#endif

#endif  /* __PROJECT_TL_DIMMABLE_LIGHT__ */
