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

#ifdef ZCL_OTA
ota_callBack_t sampleLight_otaCb =
{
	sampleLight_otaProcessMsgHandler,
};
#endif


/**********************************************************************
 * LOCAL VARIABLES
 */

/**********************************************************************
 * FUNCTIONS
 */
s32 sampleLight_bdbNetworkSteerStart(void *arg){
	bdb_networkSteerStart();

	return -1;
}

#if FIND_AND_BIND_SUPPORT
s32 sampleLight_bdbFindAndBindStart(void *arg){
	bdb_findAndBindStart(BDB_COMMISSIONING_ROLE_TARGET);

	return -1;
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
		 * start bdb commissioning
		 * */
		if(joinedNetwork){
#ifdef ZCL_OTA
			ota_queryStart(30);
#endif
		}else{
#if	(!ZBHCI_EN)
			u16 jitter=0;
			do{
				jitter = zb_random();
				jitter &= 0xfff;
			}while(jitter==0);
			TL_ZB_TIMER_SCHEDULE(sampleLight_bdbNetworkSteerStart, NULL, jitter * 1000);
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
#if FIND_AND_BIND_SUPPORT
	    if(!gLightCtx.bdbFindBindFlg){
	    	gLightCtx.bdbFindBindFlg = TRUE;
#endif

	    	light_blink_start(2, 200, 200);

#ifdef ZCL_OTA
	    	ota_queryStart(30);
#endif

#if FIND_AND_BIND_SUPPORT
	    	//start Finding & Binding
        	TL_ZB_TIMER_SCHEDULE(sampleLight_bdbFindAndBindStart, NULL, 1 * 1000 * 1000);
        }
#endif
	}else if(status == BDB_COMMISSION_STA_IN_PROGRESS){

	}else if(status == BDB_COMMISSION_STA_NOT_AA_CAPABLE){

	}else if((status == BDB_COMMISSION_STA_NO_NETWORK)||(status == BDB_COMMISSION_STA_TCLK_EX_FAILURE)){
		if(gLightCtx.installCodeAvailable){
			/* Switch the two kinds of link keys (default TCLK or install code derived key) to attempt join network. */
			if(gLightCtx.useInstallCodeFlg){
				g_bdbCommissionSetting.linkKey.tcLinkKey.keyType = gLightCtx.linkKey.tcLinkKey.keyType;
				g_bdbCommissionSetting.linkKey.tcLinkKey.key = gLightCtx.linkKey.tcLinkKey.key;
			}else{
				g_bdbCommissionSetting.linkKey.tcLinkKey.keyType = SS_GLOBAL_LINK_KEY;
				g_bdbCommissionSetting.linkKey.tcLinkKey.key = (u8 *)tcLinkKeyCentralDefault;
			}

			bdb_linkKeyCfg(&g_bdbCommissionSetting, TRUE);
			gLightCtx.useInstallCodeFlg = !gLightCtx.useInstallCodeFlg;
		}

		u16 jitter = 0;
		do{
			jitter = zb_random();
			jitter &= 0xfff;
		}while(jitter==0);
		TL_ZB_TIMER_SCHEDULE(sampleLight_bdbNetworkSteerStart, NULL, jitter * 1000);
	}else if(status == BDB_COMMISSION_STA_TARGET_FAILURE){

	}else if(status == BDB_COMMISSION_STA_FORMATION_FAILURE){

	}else if(status == BDB_COMMISSION_STA_NO_IDENTIFY_QUERY_RESPONSE){

	}else if(status == BDB_COMMISSION_STA_BINDING_TABLE_FULL){

	}else if(status == BDB_COMMISSION_STA_NO_SCAN_RESPONSE){

	}else if(status == BDB_COMMISSION_STA_NOT_PERMITTED){

	}else if(status == BDB_COMMISSION_STA_FORMATION_DONE){
#if ZBHCI_EN

#else
		tl_zbMacChannelSet(DEFAULT_CHANNEL);  //set default channel
#endif
	}
}


extern void sampleLight_zclIdentifyCmdHandler(u8 endpoint, u16 srcAddr, u16 identifyTime);
void zbdemo_bdbIdentifyCb(u8 endpoint, u16 srcAddr, u16 identifyTime){
#if FIND_AND_BIND_SUPPORT
	sampleLight_zclIdentifyCmdHandler(endpoint, srcAddr, identifyTime);
#endif
}



#ifdef ZCL_OTA
void sampleLight_otaProcessMsgHandler(u8 evt, u8 status)
{
	if(evt == OTA_EVT_START){
		if(status == ZCL_STA_SUCCESS){

		}else{

		}
	}else if(evt == OTA_EVT_COMPLETE){
		if(status == ZCL_STA_SUCCESS){
			ota_mcuReboot();
		}else{
			ota_queryStart(30);
		}
	}
}
#endif

s32 sampleLight_softReset(void *arg){
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
void sampleLight_leaveCnfHandler(void *p)
{
	nlme_leave_cnf_t *pCnf = (nlme_leave_cnf_t *)p;

    if(pCnf->status == SUCCESS){
    	light_blink_start(3, 200, 200);

    	//waiting blink over
    	TL_ZB_TIMER_SCHEDULE(sampleLight_softReset, NULL, 2 * 1000 * 1000);
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
void sampleLight_leaveIndHandler(void *p)
{
//	nlmeLeaveInd_t *pInd = (nlmeLeaveInd_t *)p;

}

u8 sampleLight_nwkUpdateIndicateHandler(void *arg){
	return FAILURE;
}

#endif  /* __PROJECT_TL_DIMMABLE_LIGHT__ */
