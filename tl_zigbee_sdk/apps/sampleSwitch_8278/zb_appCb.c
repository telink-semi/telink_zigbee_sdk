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
#if (__PROJECT_TL_SWITCH_8278__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "bdb.h"
#include "ota.h"
#include "sampleSwitch.h"
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
void zbdemo_bdbInitCb(u8 status, u8 joinedNetwork);
void zbdemo_bdbCommissioningCb(u8 status, void *arg);
void zbdemo_bdbIdentifyCb(u8 endpoint, u16 srcAddr, u16 identifyTime);
void zbdemo_bdbFindBindSuccessCb(findBindDst_t *pDstInfo);


/**********************************************************************
 * LOCAL VARIABLES
 */
bdb_appCb_t g_zbDemoBdbCb =
{
	zbdemo_bdbInitCb,
	zbdemo_bdbCommissioningCb,
	zbdemo_bdbIdentifyCb,
	zbdemo_bdbFindBindSuccessCb
};

#ifdef ZCL_OTA
ota_callBack_t sampleSwitch_otaCb =
{
	sampleSwitch_otaProcessMsgHandler,
};
#endif


/**********************************************************************
 * FUNCTIONS
 */
s32 sampleSwitch_bdbNetworkSteerStart(void *arg){
	bdb_networkSteerStart();

	return -1;
}

#if FIND_AND_BIND_SUPPORT
s32 sampleSwitch_bdbFindAndBindStart(void *arg){
	BDB_ATTR_GROUP_ID_SET(0x1234);//only for initiator
	bdb_findAndBindStart(BDB_COMMISSIONING_ROLE_INITIATOR);

	g_switchAppCtx.bdbFBTimerEvt = NULL;
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
		 * for non-factory-new device:
		 * 		load zcl data from NV, start poll rate, start ota query, bdb_networkSteerStart
		 *
		 * for factory-new device:
		 * 		steer a network
		 *
		 */
		if(joinedNetwork){
			zb_setPollRate(POLL_RATE * 3);

#ifdef ZCL_OTA
			ota_queryStart(30);
#endif

#ifdef ZCL_POLL_CTRL
			sampleSwitch_zclCheckInStart();
#endif
		}else{
			u16 jitter = zb_random();
			jitter &= 0xfff;
			TL_ZB_TIMER_SCHEDULE(sampleSwitch_bdbNetworkSteerStart, NULL, jitter * 1000);
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
u8 sleepCnt = 0;
void zbdemo_bdbCommissioningCb(u8 status, void *arg){
	if(status == BDB_COMMISSION_STA_SUCCESS){
		zb_setPollRate(POLL_RATE * 3);

#ifdef ZCL_POLL_CTRL
		sampleSwitch_zclCheckInStart();
#endif

		light_blink_start(2, 200, 200);

#ifdef ZCL_OTA
        ota_queryStart(30);
#endif

#if FIND_AND_BIND_SUPPORT
        //start Finding & Binding
		if(!g_switchAppCtx.bdbFBTimerEvt){
			g_switchAppCtx.bdbFBTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleSwitch_bdbFindAndBindStart, NULL, 50 * 1000);
		}
#endif
	}else if(status == BDB_COMMISSION_STA_IN_PROGRESS){

	}else if(status == BDB_COMMISSION_STA_NOT_AA_CAPABLE){

	}else if(status == BDB_COMMISSION_STA_NO_NETWORK){
		/*if(sleepCnt++ >= 5){
			sleepCnt = 0;
			zb_setPollRate(0);
		}else*/
		{
			u16 jitter = zb_random();
			jitter &= 0xfff;
			TL_ZB_TIMER_SCHEDULE(sampleSwitch_bdbNetworkSteerStart, NULL, jitter * 1000);
		}
	}else if(status == BDB_COMMISSION_STA_TARGET_FAILURE){

	}else if(status == BDB_COMMISSION_STA_FORMATION_FAILURE){

	}else if(status == BDB_COMMISSION_STA_NO_IDENTIFY_QUERY_RESPONSE){

	}else if(status == BDB_COMMISSION_STA_BINDING_TABLE_FULL){

	}else if(status == BDB_COMMISSION_STA_NO_SCAN_RESPONSE){

	}else if(status == BDB_COMMISSION_STA_NOT_PERMITTED){

	}else if(status == BDB_COMMISSION_STA_TCLK_EX_FAILURE){

	}else if(status == BDB_COMMISSION_STA_PARENT_LOST){
		/*
		 * Becoming an orphan node now.
		 * Attempt to join network by invoking rejoin request,
		 * internal will start an rejoin backoff timer
		 * based on 'config_rejoin_backoff_time' once rejoin failed.
		 */
		//zb_rejoin_mode_set(REJOIN_INSECURITY);
		zb_rejoinReq(NLME_REJOIN_METHOD_REJOIN, zb_apsChannelMaskGet());
	}else if(status == BDB_COMMISSION_STA_REJOIN_FAILURE){

	}
}


extern void sampleSwitch_zclIdentifyCmdHandler(u8 endpoint, u16 srcAddr, u16 identifyTime);
void zbdemo_bdbIdentifyCb(u8 endpoint, u16 srcAddr, u16 identifyTime){
#if FIND_AND_BIND_SUPPORT
	sampleSwitch_zclIdentifyCmdHandler(endpoint, srcAddr, identifyTime);
#endif
}

/*********************************************************************
 * @fn      zbdemo_bdbFindBindSuccessCb
 *
 * @brief   application callback for finding & binding
 *
 * @param   pDstInfo
 *
 * @return  None
 */
void zbdemo_bdbFindBindSuccessCb(findBindDst_t *pDstInfo){
#if FIND_AND_BIND_SUPPORT
	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEpInfo.dstAddr.shortAddr = pDstInfo->addr;
	dstEpInfo.dstEp = pDstInfo->endpoint;
	dstEpInfo.profileId = HA_PROFILE_ID;

	zcl_identify_identifyCmd(SAMPLE_SWITCH_ENDPOINT, &dstEpInfo, FALSE, 0, 0);
#endif
}



#ifdef ZCL_OTA
void sampleSwitch_otaProcessMsgHandler(u8 evt, u8 status)
{
	//printf("sampleSwitch_otaProcessMsgHandler: status = %x\n", status);
	if(evt == OTA_EVT_START){
		if(status == ZCL_STA_SUCCESS){
			zb_setPollRate(QUEUE_POLL_RATE);
		}else{

		}
	}else if(evt == OTA_EVT_COMPLETE){
		zb_setPollRate(POLL_RATE * 3);

		if(status == ZCL_STA_SUCCESS){
			ota_mcuReboot();
		}else{
			ota_queryStart(30);
		}
	}
}
#endif

/*********************************************************************
 * @fn      sampleSwitch_leaveCnfHandler
 *
 * @brief   Handler for ZDO Leave Confirm message.
 *
 * @param   pRsp - parameter of leave confirm
 *
 * @return  None
 */
void sampleSwitch_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf)
{
    if(pLeaveCnf->status == SUCCESS){
    	//SYSTEM_RESET();
    }
}

/*********************************************************************
 * @fn      sampleSwitch_leaveIndHandler
 *
 * @brief   Handler for ZDO leave indication message.
 *
 * @param   pInd - parameter of leave indication
 *
 * @return  None
 */
void sampleSwitch_leaveIndHandler(nlme_leave_ind_t *pLeaveInd)
{
    //printf("sampleSwitch_leaveIndHandler, rejoin = %d\n", pLeaveInd->rejoin);
    //printfArray(pLeaveInd->device_address, 8);
}


#endif  /* __PROJECT_TL_SWITCH_8278__ */
