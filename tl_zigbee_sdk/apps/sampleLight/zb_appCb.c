/********************************************************************************************************
 * @file    zb_appCb.c
 *
 * @brief   This is the source file for zb_appCb
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
#include "bdb.h"
#include "ota.h"
#include "sampleLight.h"
#include "sampleLightCtrl.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */
#define DEBUG_HEART     0

/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */
void zbdemo_bdbInitCb(u8 status, u8 joinedNetwork);
void zbdemo_bdbCommissioningCb(u8 status, void *arg);
void zbdemo_bdbIdentifyCb(u8 endpoint, u16 srcAddr, u16 identifyTime);

/**********************************************************************
 * GLOBAL VARIABLES
 */
bdb_appCb_t g_zbDemoBdbCb = {
    zbdemo_bdbInitCb,
    zbdemo_bdbCommissioningCb,
    zbdemo_bdbIdentifyCb,
    NULL
};

#ifdef ZCL_OTA
ota_callBack_t sampleLight_otaCb = {
    sampleLight_otaProcessMsgHandler,
};
#endif

/**********************************************************************
 * LOCAL VARIABLES
 */
static u32 heartInterval = 0;

#if DEBUG_HEART
static ev_timer_event_t *heartTimerEvt = NULL;
#endif
static ev_timer_event_t *steerTimerEvt = NULL;
static ev_timer_event_t *rejoinBackoffTimerEvt = NULL;

/**********************************************************************
 * FUNCTIONS
 */
#if DEBUG_HEART
static s32 heartTimerCb(void *arg)
{
    if (heartInterval == 0) {
        heartTimerEvt = NULL;
        return -1;
    }

    gpio_toggle(LED_POWER);

    return heartInterval;
}
#endif

static s32 sampleLight_bdbNetworkSteerStart(void *arg)
{
    bdb_networkSteerStart();

    steerTimerEvt = NULL;
    return -1;
}

#if FIND_AND_BIND_SUPPORT
static s32 sampleLight_bdbFindAndBindStart(void *arg)
{
    bdb_findAndBindStart(BDB_COMMISSIONING_ROLE_TARGET);

    return -1;
}
#endif

static s32 sampleLight_rejoinBackoff(void *arg)
{
    static bool rejoinMode = REJOIN_SECURITY;

    if (zb_isDeviceFactoryNew()) {
        rejoinBackoffTimerEvt = NULL;
        return -1;
    }

    //printf("rejoin mode = %d\n", rejoinMode);

    zb_rejoinSecModeSet(rejoinMode);
    zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);

    rejoinMode = !rejoinMode;

    return 0;
}

/*********************************************************************
 * @fn      zbdemo_bdbInitCb
 *
 * @brief   application callback for bdb initiation
 *
 * @param   status - the status of bdb init BDB_INIT_STATUS_SUCCESS or BDB_INIT_STATUS_FAILURE
 *
 * @param   joinedNetwork  - 1: node is on a network, 0: node isn't on a network
 *
 * @return  None
 */
void zbdemo_bdbInitCb(u8 status, u8 joinedNetwork)
{
    //printf("bdbInitCb: sta = %x, joined = %x\n", status, joinedNetwork);

    if (status == BDB_INIT_STATUS_SUCCESS) {
        /*
         * start bdb commissioning
         * */
        if (joinedNetwork) {
            heartInterval = 1000;

#ifdef ZCL_OTA
            ota_queryStart(OTA_PERIODIC_QUERY_INTERVAL);
#endif
        } else {
            heartInterval = 500;

#if (!ZBHCI_EN)
            u16 jitter = 0;
            do {
                jitter = zb_random() % 0x0fff;
            } while (jitter == 0);

            if (steerTimerEvt) {
                TL_ZB_TIMER_CANCEL(&steerTimerEvt);
            }
            steerTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleLight_bdbNetworkSteerStart, NULL, jitter);
#endif
        }
    } else {
        heartInterval = 200;
    }

#if DEBUG_HEART
    if (heartTimerEvt) {
        TL_ZB_TIMER_CANCEL(&heartTimerEvt);
    }
    heartTimerEvt = TL_ZB_TIMER_SCHEDULE(heartTimerCb, NULL, heartInterval);
#endif
}

/*********************************************************************
 * @fn      zbdemo_bdbCommissioningCb
 *
 * @brief   application callback for bdb commissioning
 *
 * @param   status - the status of bdb commissioning
 *
 * @param   arg
 *
 * @return  None
 */
void zbdemo_bdbCommissioningCb(u8 status, void *arg)
{
    //printf("bdbCommCb: sta = %x\n", status);

    switch (status) {
    case BDB_COMMISSION_STA_SUCCESS:
        heartInterval = 1000;

        light_blink_start(2, 200, 200);

        if (steerTimerEvt) {
            TL_ZB_TIMER_CANCEL(&steerTimerEvt);
        }

        if (rejoinBackoffTimerEvt) {
            TL_ZB_TIMER_CANCEL(&rejoinBackoffTimerEvt);
        }

#ifdef ZCL_OTA
        ota_queryStart(OTA_PERIODIC_QUERY_INTERVAL);
#endif

#if FIND_AND_BIND_SUPPORT
        if (!gLightCtx.bdbFindBindFlg) {
            gLightCtx.bdbFindBindFlg = TRUE;
            TL_ZB_TIMER_SCHEDULE(sampleLight_bdbFindAndBindStart, NULL, 1000);
        }
#endif
        break;
    case BDB_COMMISSION_STA_IN_PROGRESS:
        break;
    case BDB_COMMISSION_STA_NOT_AA_CAPABLE:
        break;
    case BDB_COMMISSION_STA_NO_NETWORK:
    case BDB_COMMISSION_STA_TCLK_EX_FAILURE:
    case BDB_COMMISSION_STA_TARGET_FAILURE:
        {
            u16 jitter = 0;
            do {
                jitter = zb_random() % 0x2710;
            } while (jitter < 5000);

            if (steerTimerEvt) {
                TL_ZB_TIMER_CANCEL(&steerTimerEvt);
            }
            steerTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleLight_bdbNetworkSteerStart, NULL, jitter);
        }
        break;
    case BDB_COMMISSION_STA_FORMATION_FAILURE:
        break;
    case BDB_COMMISSION_STA_NO_IDENTIFY_QUERY_RESPONSE:
        break;
    case BDB_COMMISSION_STA_BINDING_TABLE_FULL:
        break;
    case BDB_COMMISSION_STA_NO_SCAN_RESPONSE:
        break;
    case BDB_COMMISSION_STA_NOT_PERMITTED:
        break;
    case BDB_COMMISSION_STA_REJOIN_FAILURE:
        if (!rejoinBackoffTimerEvt) {
            rejoinBackoffTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleLight_rejoinBackoff, NULL, 60 * 1000);
        }
        break;
    case BDB_COMMISSION_STA_FORMATION_DONE:
#ifndef ZBHCI_EN
        tl_zbMacChannelSet(DEFAULT_CHANNEL);  //set default channel
#endif
        break;
    default:
        break;
    }
}

void zbdemo_bdbIdentifyCb(u8 endpoint, u16 srcAddr, u16 identifyTime)
{
#if FIND_AND_BIND_SUPPORT
    extern void sampleLight_zclIdentifyCmdHandler(u8 endpoint, u16 srcAddr, u16 identifyTime);
    sampleLight_zclIdentifyCmdHandler(endpoint, srcAddr, identifyTime);
#endif
}

#ifdef ZCL_OTA
void sampleLight_otaProcessMsgHandler(u8 evt, u8 status)
{
    if (evt == OTA_EVT_START) {
        if (status == ZCL_STA_SUCCESS) {

        } else {

        }
    } else if (evt == OTA_EVT_COMPLETE) {
        if (status == ZCL_STA_SUCCESS) {
            ota_mcuReboot();
        } else {
            ota_queryStart(OTA_PERIODIC_QUERY_INTERVAL);
        }
    }
}
#endif

s32 sampleLight_softReset(void *arg)
{
    SYSTEM_RESET();

    return -1;
}

/*********************************************************************
 * @fn      sampleLight_leaveCnfHandler
 *
 * @brief   Handler for ZDO Leave Confirm message.
 *
 * @param   pRsp - parameter of leave confirm
 *
 * @return  None
 */
void sampleLight_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf)
{
    if(pLeaveCnf->status == SUCCESS){
    	light_blink_start(3, 200, 200);

    	//waiting blink over
    	TL_ZB_TIMER_SCHEDULE(sampleLight_softReset, NULL, 2 * 1000);
    }
}

/*********************************************************************
 * @fn      sampleLight_leaveIndHandler
 *
 * @brief   Handler for ZDO leave indication message.
 *
 * @param   pInd - parameter of leave indication
 *
 * @return  None
 */
void sampleLight_leaveIndHandler(nlme_leave_ind_t *pLeaveInd)
{

}

/*********************************************************************
 *
 * @brief   Receive notification of PAN ID conflict.
 *
 * @param   pNwkUpdateCmd - Conflicting PAN ID information
 *
 * @return  TRUE  - Allow PAN ID conflict handling
 *          FALSE - Truncate the execution of PAN ID conflict handling
 */
bool sampleLight_nwkUpdateIndicateHandler(nwkCmd_nwkUpdate_t *pNwkUpdate)
{
    return FALSE;
}

/*********************************************************************
 * @fn      sampleLight_nwkStatusIndHandler
 *
 * @brief   Handler for NWK status indication message.
 *
 * @param   pInd - parameter of NWK status indication
 *
 * @return  None
 */
void sampleLight_nwkStatusIndHandler(zdo_nwk_status_ind_t *pNwkStatusInd)
{
    //printf("nwkStatusIndHandler: addr = %x, status = %x\n", pNwkStatusInd->shortAddr, pNwkStatusInd->status);

    if (pNwkStatusInd->status == NWK_COMMAND_STATUS_BAD_FRAME_COUNTER) {
        tl_zb_normal_neighbor_entry_t *nbe = nwk_neTblGetByShortAddr(pNwkStatusInd->shortAddr);
        if (nbe) {
            //printf("curFC = %d, rcvFC = %d, failCnt = %d\n", nbe->incomingFrameCnt, nbe->receivedFrameCnt, nbe->frameCounterFailCnt);

            /*
             * When the network does not support the network key update feature,
             * this is a barbaric method to solve the decryption failure problem
             * caused by Frame Counter overflow, but it may also brings
             * the hidden danger of being attacked.
             */
#if 1
            if ((nbe->frameCounterFailCnt >= 10) && (nbe->receivedFrameCnt < nbe->incomingFrameCnt)) {
                nbe->incomingFrameCnt = 0;
            }
#endif
        }
    } else if (pNwkStatusInd->status == NWK_COMMAND_STATUS_BAD_KEY_SEQUENCE_NUMBER) {
        zb_rejoinSecModeSet(REJOIN_INSECURITY);
        zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
    }
}

#endif  /* __PROJECT_TL_DIMMABLE_LIGHT__ */
