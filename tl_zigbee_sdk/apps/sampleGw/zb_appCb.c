/********************************************************************************************************
 * @file	zb_appCb.c
 *
 * @brief	This is the source file for zb_appCb
 *
 * @author	Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
#define DEBUG_HEART		0

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
bdb_appCb_t g_zbDemoBdbCb = {zbdemo_bdbInitCb, zbdemo_bdbCommissioningCb, zbdemo_bdbIdentifyCb, NULL};

#if ZBHCI_EN
bool sampleGw_macAssocReqIndHandler(void *arg);
mac_appIndCb_t macAppIndCbList = {NULL, NULL, sampleGw_macAssocReqIndHandler};
#endif


/**********************************************************************
 * LOCAL VARIABLES
 */
u32 heartInterval = 0;

#if DEBUG_HEART
ev_time_event_t *heartTimerEvt = NULL;
#endif

/**********************************************************************
 * FUNCTIONS
 */
#if DEBUG_HEART
static s32 heartTimerCb(void *arg){
	if(heartInterval == 0){
		heartTimerEvt = NULL;
		return -1;
	}

	gpio_toggle(LED_POWER);

	return heartInterval * 1000;
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
void zbdemo_bdbInitCb(u8 status, u8 joinedNetwork){
	if(status == BDB_INIT_STATUS_SUCCESS){
		/*
		 * for a non-factory-new device:
		 * 		steer a network
		 *
		 * for a factory-new device:
		 * 	 	do nothing until receiving the command from host if HCI is enabled,
		 * 		forms a central network if HCI is disabled.
		 *
		 */
		if(joinedNetwork){
			heartInterval = 1000;
		}else{
#if	(!ZBHCI_EN)
			bdb_networkFormationStart();
#endif
			heartInterval = 500;
		}
	}else{
		heartInterval = 200;
	}

#if DEBUG_HEART
	if(heartTimerEvt){
		TL_ZB_TIMER_CANCEL(&heartTimerEvt);
	}
	heartTimerEvt = TL_ZB_TIMER_SCHEDULE(heartTimerCb, NULL, heartInterval * 1000);
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
void zbdemo_bdbCommissioningCb(u8 status, void *arg){
	if(status == BDB_COMMISSION_STA_SUCCESS){
		heartInterval = 1000;
	}else if(status == BDB_COMMISSION_STA_IN_PROGRESS){

	}else if(status == BDB_COMMISSION_STA_NOT_AA_CAPABLE){

	}else if(status == BDB_COMMISSION_STA_NO_NETWORK){

	}else if(status == BDB_COMMISSION_STA_TARGET_FAILURE){

	}else if(status == BDB_COMMISSION_STA_FORMATION_FAILURE){

	}else if(status == BDB_COMMISSION_STA_NO_IDENTIFY_QUERY_RESPONSE){

	}else if(status == BDB_COMMISSION_STA_BINDING_TABLE_FULL){

	}else if(status == BDB_COMMISSION_STA_NO_SCAN_RESPONSE){

	}else if(status == BDB_COMMISSION_STA_NOT_PERMITTED){

	}else if(status == BDB_COMMISSION_STA_TCLK_EX_FAILURE){

	}else if(status == BDB_COMMISSION_STA_FORMATION_DONE){
#if ZBHCI_EN

#else
		/* If you comment out the channel setting,
		 * this demo will automatically select a channel,
		 * which is the result of the energy scan.
		 */
	    tl_zbMacChannelSet(DEFAULT_CHANNEL);  //set default channel
#endif
	    heartInterval = 1000;
	}
}


extern void sampleGW_zclIdentifyCmdHandler(u8 endpoint, u16 srcAddr, u16 identifyTime);
void zbdemo_bdbIdentifyCb(u8 endpoint, u16 srcAddr, u16 identifyTime){
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
//	printf("sampleGW_leaveCnfHandler, status = %x\n", pLeaveCnf->status);
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
	//zbhciLeaveIndMsgPush(pLeaveInd);
#if 0
	static u16 leaveNodeCnt = 0;
	zbhci_nodeLeaveInd_t ind;
	ind.totalCnt = leaveNodeCnt++;
	memcpy(ind.macAddr, pLeaveInd->device_address, 8);
	zbhciAppNodeLeaveIndPush((void *)&ind);
#endif
#endif
}

void sampleGW_dataSendConfirm(void *arg){
#if ZBHCI_EN
	apsdeDataConf_t *pApsDataCnf = (apsdeDataConf_t *)arg;

	zbhciAppDataSendConfirmPush((void *)pApsDataCnf);
#endif
}

#if ZBHCI_EN
bool sampleGw_macAssocReqIndHandler(void *arg){
	zb_mlme_associate_ind_t *ind = (zb_mlme_associate_ind_t *)arg;

	return zbhciMacAddrGetPush(ind->devAddress);
}
#endif


bool sampleGW_nwkUpdateIndicateHandler(nwkCmd_nwkUpdate_t *pNwkUpdateCmd){
	return FAILURE;
}

bool sampleGW_tcJoinIndHandler(zdo_tc_join_ind_t *pTcJoinInd){
	return TRUE;
}

#endif  /* __PROJECT_TL_GW__ */
