/********************************************************************************************************
 * @file	sampleGateway.c
 *
 * @brief	This is the source file for sampleGateway
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
#include "ota.h"
#include "gp.h"
#include "sampleGateway.h"
#include "app_ui.h"
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
 * GLOBAL VARIABLES
 */
app_ctx_t g_appGwCtx;


#if ZBHCI_EN
extern mac_appIndCb_t macAppIndCbList;
#endif

#ifdef ZCL_OTA
//running code firmware information
ota_preamble_t sampleGW_otaInfo = {
	.fileVer 			= FILE_VERSION,
	.imageType 			= IMAGE_TYPE,
	.manufacturerCode 	= MANUFACTURER_CODE_TELINK,
};
#endif


//Must declare the application call back function which used by ZDO layer
const zdo_appIndCb_t appCbLst = {
	bdb_zdoStartDevCnf,					//start device cnf cb
	NULL,								//reset cnf cb
	sampleGW_devAnnHandler,				//device announce indication cb
	sampleGW_leaveIndHandler,			//leave ind cb
	sampleGW_leaveCnfHandler,			//leave cnf cb
	sampleGW_nwkUpdateIndicateHandler,	//nwk update ind cb
	NULL,								//permit join ind cb
	NULL,								//nlme sync cnf cb
	sampleGW_tcJoinIndHandler,			//tc join ind cb
};


/**
 *  @brief Definition for bdb commissioning setting
 */
bdb_commissionSetting_t g_bdbCommissionSetting = {
	.usedInstallCode = 0,

	.linkKey.tcLinkKey.keyType = SS_GLOBAL_LINK_KEY,
	.linkKey.tcLinkKey.key = (u8 *)tcLinkKeyCentralDefault,       		//can use unique link key stored in NV

	.linkKey.distributeLinkKey.keyType = MASTER_KEY,
	.linkKey.distributeLinkKey.key = (u8 *)linkKeyDistributedMaster,  	//use linkKeyDistributedCertification before testing

	.linkKey.touchLinkKey.keyType = MASTER_KEY,
	.linkKey.touchLinkKey.key = (u8 *)touchLinkKeyMaster,   			//use touchLinkKeyCertification before testing

	.touchlinkEnable = 0,			 									//disable touch link for coordinator
};

/**********************************************************************
 * LOCAL VARIABLES
 */


/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      stack_init
 *
 * @brief   This function initialize the ZigBee stack and related profile. If HA/ZLL profile is
 *          enabled in this application, related cluster should be registered here.
 *
 * @param   None
 *
 * @return  None
 */
void stack_init(void)
{
	zb_init();

#if ZBHCI_EN
	zb_macCbRegister((mac_appIndCb_t *)&macAppIndCbList);
#endif
	zb_zdoCbRegister((zdo_appIndCb_t *)&appCbLst);
}

/*********************************************************************
 * @fn      user_app_init
 *
 * @brief   This function initialize the application(Endpoint) information for this node.
 *
 * @param   None
 *
 * @return  None
 */
void user_app_init(void)
{
	af_nodeDescManuCodeUpdate(MANUFACTURER_CODE_TELINK);

    /* Initialize ZCL layer */
	/* Register Incoming ZCL Foundation command/response messages */
	zcl_init(sampleGW_zclProcessIncomingMsg);

	/* Register endPoint */
	af_endpointRegister(SAMPLE_GW_ENDPOINT, (af_simple_descriptor_t *)&sampleGW_simpleDesc, zcl_rx_handler, sampleGW_dataSendConfirm);
#if AF_TEST_ENABLE
	/* A sample of AF data handler. */
	af_endpointRegister(SAMPLE_TEST_ENDPOINT, (af_simple_descriptor_t *)&sampleTestDesc, afTest_rx_handler, afTest_dataSendConfirm);
#endif

	/* Register ZCL specific cluster information */
	zcl_register(SAMPLE_GW_ENDPOINT, SAMPLE_GW_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)g_sampleGwClusterList);

#ifdef ZCL_GREEN_POWER
	gp_init();
#endif

#ifdef ZCL_OTA
    ota_init(OTA_TYPE_SERVER, (af_simple_descriptor_t *)&sampleGW_simpleDesc, &sampleGW_otaInfo, NULL);
#endif
}


void led_init(void)
{
	led_off(LED_PERMIT);
	light_init();
}

void app_task(void)
{
	static bool assocPermit = 0;
	if(assocPermit != zb_getMacAssocPermit()){
		assocPermit = zb_getMacAssocPermit();
		if(assocPermit){
			led_on(LED_PERMIT);
		}else{
			led_off(LED_PERMIT);
		}
	}

	if(BDB_STATE_GET() == BDB_STATE_IDLE){
		app_key_handler();
	}
}

static void sampleGwSysException(void)
{
	//SYSTEM_RESET();
	light_off();

	while(1){
		gpio_toggle(LED_POWER);
		WaitMs(100);
	}
}



/*********************************************************************
 * @fn      user_init
 *
 * @brief   User level initialization code.
 *
 * @param   isRetention - if it is waking up with ram retention.
 *
 * @return  None
 */
void user_init(bool isRetention)
{
	(void)isRetention;

#if defined(MCU_CORE_8258) || defined(MCU_CORE_8278) || defined(MCU_CORE_B91)
	extern u8 firmwareCheckWithUID(void);
	if(firmwareCheckWithUID()){
		while(1);
	}
#endif

	/* Initialize LEDs*/
	led_init();

#if PA_ENABLE
	/* external RF PA used */
	rf_paInit(PA_TX, PA_RX);
#endif

	/* Initialize Stack */
	stack_init();

	sys_exceptHandlerRegister(sampleGwSysException);

	/* Initialize user application */
	user_app_init();

	/* User's Task */
#if ZBHCI_EN
    /*
     * define ZBHCI_USB_PRINT, ZBHCI_USB_CDC or ZBHCI_UART as 1 in app_cfg.h
     * if needing to enable ZBHCI_EN
     *
     * */
    zbhciInit();
	ev_on_poll(EV_POLL_HCI, zbhciTask);
#endif
	ev_on_poll(EV_POLL_IDLE, app_task);

	//af_nodeDescStackRevisionSet(20);

	/*
	 * bdb initialization start,
	 * once initialization is done, the g_zbDemoBdbCb.bdbInitCb() will be called
	 *
	 * */
    bdb_init((af_simple_descriptor_t *)&sampleGW_simpleDesc, &g_bdbCommissionSetting, &g_zbDemoBdbCb, 1);
}

#endif  /* __PROJECT_TL_GW__ */

