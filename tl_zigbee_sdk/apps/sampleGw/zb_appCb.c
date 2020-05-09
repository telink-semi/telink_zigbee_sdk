/********************************************************************************************************
 * @file     zb_appCb.c
 *
 * @brief    call back function for zigbee
 *
 * @author
 * @date     Dec. 1, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
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

/**********************************************************************
 * FUNCTIONS
 */

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

		}else{
#if	(!ZBHCI_EN)
			bdb_networkFormationStart();
#endif
		}
	}else{

	}
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
 * @param   pInd - parameter of device announce indication
 *
 * @return  None
 */
void sampleGW_devAnnHandler(void *arg)
{
#if ZBHCI_EN
	zdo_device_annce_req_t *pDevAnnceReq = (zdo_device_annce_req_t *)arg;

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
 * @param   pRsp - parameter of leave confirm
 *
 * @return  None
 */
void sampleGW_leaveCnfHandler(void *arg)
{
//	nlmeLeaveConf_t *pCnf = (nlmeLeaveConf_t *)arg;
//	printf("sampleGW_leaveCnfHandler, status = %x\n", pCnf->status);
}

/*********************************************************************
 * @fn      sampleGW_leaveIndHandler
 *
 * @brief   Handler for ZDO leave indication message.
 *
 * @param   pInd - parameter of leave indication
 *
 * @return  None
 */
void sampleGW_leaveIndHandler(void* arg)
{
#if ZBHCI_EN
//	nlmeLeaveInd_t *p = (nlmeLeaveInd_t*)arg;
	//zbhciLeaveIndMsgPush(p);
#if 0
	static u16 leaveNodeCnt = 0;
	zbhci_nodeLeaveInd_t ind;
	ind.totalCnt = leaveNodeCnt++;
	memcpy(ind.macAddr, arg, 8);
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


u8 sampleGW_nwkUpdateIndicateHandler(void *arg){
	return FAILURE;
}

#endif  /* __PROJECT_TL_GW__ */
