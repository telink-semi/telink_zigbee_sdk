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
#if (__PROJECT_TL_GW__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "bdb.h"
#include "sampleGateway.h"
#if ZBHCI_EN
#include "zbhci.h"
#endif

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

#if ZBHCI_EN
bool sampleGw_macAssocReqIndHandler(void *arg);
mac_appIndCb_t macAppIndCbList = {
    NULL,
    NULL,
    sampleGw_macAssocReqIndHandler
};
#endif


/**********************************************************************
 * LOCAL VARIABLES
 */
u32 heartInterval = 0;

#if DEBUG_HEART
ev_timer_event_t *heartTimerEvt = NULL;
#endif

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
         * for a non-factory-new device:
         * steer a network
         *
         * for a factory-new device:
         * do nothing until receiving the command from host if HCI is enabled,
         * forms a central network if HCI is disabled.
         *
         */
        if (joinedNetwork) {
            heartInterval = 1000;
        } else {
#if (!ZBHCI_EN)
            bdb_networkFormationStart();
#endif
            heartInterval = 500;
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

    if (status == BDB_COMMISSION_STA_SUCCESS) {
        heartInterval = 1000;
    } else if (status == BDB_COMMISSION_STA_IN_PROGRESS) {

    } else if (status == BDB_COMMISSION_STA_NOT_AA_CAPABLE) {

    } else if (status == BDB_COMMISSION_STA_NO_NETWORK) {

    } else if (status == BDB_COMMISSION_STA_TARGET_FAILURE) {

    } else if (status == BDB_COMMISSION_STA_FORMATION_FAILURE) {

    } else if (status == BDB_COMMISSION_STA_NO_IDENTIFY_QUERY_RESPONSE) {

    } else if (status == BDB_COMMISSION_STA_BINDING_TABLE_FULL) {

    } else if (status == BDB_COMMISSION_STA_NO_SCAN_RESPONSE) {

    } else if (status == BDB_COMMISSION_STA_NOT_PERMITTED) {

    } else if (status == BDB_COMMISSION_STA_TCLK_EX_FAILURE) {

    } else if (status == BDB_COMMISSION_STA_FORMATION_DONE) {
#if ZBHCI_EN

#else
        /* If comment out the channel setting,
         * it will automatically select a channel,
         * which is from the result of the energy scan.
         */
        tl_zbMacChannelSet(DEFAULT_CHANNEL);  //set default channel
#endif
        heartInterval = 1000;
    }
}


extern void sampleGW_zclIdentifyCmdHandler(u8 endpoint, u16 srcAddr, u16 identifyTime);
void zbdemo_bdbIdentifyCb(u8 endpoint, u16 srcAddr, u16 identifyTime)
{
    sampleGW_zclIdentifyCmdHandler(endpoint, srcAddr, identifyTime);
}



/*********************************************************************
 * @fn      sampleGW_devAnnHandler
 *
 * @brief   Handler for ZDO Device Announce message. When this function be called means
 *          there is new node join PAN or a node rejoin the PAN.
 *
 * @param   pDevAnnceReq - parameter of device announce indication
 *
 * @return  None
 */
void sampleGW_devAnnHandler(zdo_device_annce_req_t *pDevAnnceReq)
{
#if ZBHCI_EN
    u8 array[64];
    memset(array, 0, 64);

    u8 *pBuf = array;

    *pBuf++ = HI_UINT16(pDevAnnceReq->nwk_addr_local);
    *pBuf++ = LO_UINT16(pDevAnnceReq->nwk_addr_local);
    memcpy(pBuf, pDevAnnceReq->ieee_addr_local, 8);
    ZB_LEBESWAP(pBuf, 8);
    pBuf += 8;
    memcpy(pBuf, (u8 *)&(pDevAnnceReq->mac_capability), 1);
    pBuf++;

    zbhciTx(ZBHCI_CMD_NODES_DEV_ANNCE_IND, pBuf - array, array);
#endif
}

/*********************************************************************
 * @fn      sampleGW_leaveCnfHandler
 *
 * @brief   Handler for ZDO Leave Confirm message.
 *
 * @param   pLeaveCnf - parameter of leave confirm
 *
 * @return  None
 */
void sampleGW_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf)
{
    //printf("sampleGW_leaveCnfHandler, status = %x\n", pLeaveCnf->status);
}

/*********************************************************************
 * @fn      sampleGW_leaveIndHandler
 *
 * @brief   Handler for ZDO leave indication message.
 *
 * @param   pLeaveInd - parameter of leave indication
 *
 * @return  None
 */
void sampleGW_leaveIndHandler(nlme_leave_ind_t *pLeaveInd)
{
#if ZBHCI_EN
    static u16 leaveNodeCnt = 0;
    zbhci_nodeLeaveInd_t ind;
    ind.totalCnt = leaveNodeCnt++;
    memcpy(ind.macAddr, pLeaveInd->deviceAddr, 8);
    zbhciAppNodeLeaveIndPush((void *)&ind);
#endif
}

void sampleGW_dataSendConfirm(void *arg)
{
#if ZBHCI_EN
    apsdeDataConf_t *pApsDataCnf = (apsdeDataConf_t *)arg;

    zbhciAppDataSendConfirmPush((void *)pApsDataCnf);
#endif
}

#if ZBHCI_EN
bool sampleGw_macAssocReqIndHandler(void *arg)
{
    zb_mlme_associate_ind_t *ind = (zb_mlme_associate_ind_t *)arg;

    return zbhciMacAddrGetPush(ind->devAddress);
}
#endif

/*********************************************************************
 *
 * @brief   Receive notification of PAN ID conflict.
 *
 * @param   pNwkUpdateCmd - Conflicting PAN ID information
 *
 * @return  TRUE  - Allow PAN ID conflict handling
 *          FALSE - Truncate the execution of PAN ID conflict handling
 */
bool sampleGW_nwkUpdateIndicateHandler(nwkCmd_nwkUpdate_t *pNwkUpdateCmd)
{
    return FALSE;
}

/*********************************************************************
 *
 * @brief   Notification of trust center join.
 *
 * @param   pTcJoinInd - address information
 *
 * @return  TRUE  - Allow delivery of transport key
 *          FALSE - Truncate delivery transport key
 */
bool sampleGW_tcJoinIndHandler(zdo_tc_join_ind_t *pTcJoinInd)
{
    return TRUE;
}

/*********************************************************************
 *
 * @brief   Notification of frame counter near limit.
 * 			The function will be called when the truster center detects
 * 			the frame counter for any device in its neighbor table is
 * 			grater than 0x80000000, or its outgoing frame counter
 * 			reaches 0x80000000.
 *
 * @param   None
 *
 * @return  None
 */
void sampleGW_tcFrameCntReachedHandler(void)
{
    ss_tcUpdateNwkKey_t updateNwkKey;

    /* Broadcast NWK key updates. */
    ZB_IEEE_ADDR_ZERO(updateNwkKey.dstAddr);
    drv_generateRandomData(updateNwkKey.key, CCM_KEY_SIZE);

    zb_tcUpdateNwkKey(&updateNwkKey);
}

/*********************************************************************
 *
 * @brief   Notification of permit join request.
 *
 * @param   pPermitJoinReq - permit join duration
 *
 * @return  None
 */
void sampleGW_permitJoinIndHandler(nlme_permitJoining_req_t *pPermitJoinReq)
{
    /* When received a permit join request by TC, check the allowRemoteTcPolicyChange and
     * local status of permit join. If the allowRemoteTcPolicyChange is FALSE, and the
     * local status of permit join is FALSE too, the TC may broadcast a Mgmt_permit_join_req
     * with permitDuration = 0 to close the network. */
    if (SS_ALLOW_REMOTE_TC_POLICY_CHANGE == FALSE) {
        if (zb_getMacAssocPermit() == FALSE) {
            u8 sn = 0;
            zb_mgmtPermitJoinReq(0xfffc, 0, 0x01, &sn, NULL);
        }
    }
}

#endif  /* __PROJECT_TL_GW__ */
