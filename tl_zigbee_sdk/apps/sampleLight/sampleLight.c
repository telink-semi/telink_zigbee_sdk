/********************************************************************************************************
 * @file    sampleLight.c
 *
 * @brief   This is the source file for sampleLight
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "gp.h"
#include "sampleLight.h"
#include "sampleLightCtrl.h"
#include "app_ui.h"
#include "factory_reset.h"
#if ZBHCI_EN
#include "zbhci.h"
#endif
#if ZCL_WWAH_SUPPORT
#include "wwah.h"
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
app_ctx_t gLightCtx;


#ifdef ZCL_OTA
extern ota_callBack_t sampleLight_otaCb;

//running code firmware information
ota_preamble_t sampleLight_otaInfo = {
	.fileVer 			= FILE_VERSION,
	.imageType 			= IMAGE_TYPE,
	.manufacturerCode 	= MANUFACTURER_CODE_TELINK,
};
#endif


//Must declare the application call back function which used by ZDO layer
const zdo_appIndCb_t appCbLst = {
	bdb_zdoStartDevCnf,//start device cnf cb
	NULL,//reset cnf cb
	NULL,//device announce indication cb
	sampleLight_leaveIndHandler,//leave ind cb
	sampleLight_leaveCnfHandler,//leave cnf cb
	sampleLight_nwkUpdateIndicateHandler,//nwk update ind cb
	NULL,//permit join ind cb
	NULL,//nlme sync cnf cb
	NULL,//tc join ind cb
	NULL,//tc detects that the frame counter is near limit
};


/**
 *  @brief Definition for bdb commissioning setting
 */
bdb_commissionSetting_t g_bdbCommissionSetting = {
	.linkKey.tcLinkKey.keyType = SS_GLOBAL_LINK_KEY,
	.linkKey.tcLinkKey.key = (u8 *)tcLinkKeyCentralDefault,       		//can use unique link key stored in NV

	.linkKey.distributeLinkKey.keyType = MASTER_KEY,
	.linkKey.distributeLinkKey.key = (u8 *)linkKeyDistributedMaster,  	//use linkKeyDistributedCertification before testing

	.linkKey.touchLinkKey.keyType = MASTER_KEY,
	.linkKey.touchLinkKey.key = (u8 *)touchLinkKeyMaster,   			//use touchLinkKeyCertification before testing

#if TOUCHLINK_SUPPORT
	.touchlinkEnable = 1,												/* enable touch-link */
#else
	.touchlinkEnable = 0,												/* disable touch-link */
#endif
	.touchlinkChannel = DEFAULT_CHANNEL, 								/* touch-link default operation channel for target */
	.touchlinkLqiThreshold = 0xA0,			   							/* threshold for touch-link scan req/resp command */
};

/**********************************************************************
 * LOCAL VARIABLES
 */
ev_timer_event_t *sampleLightAttrsStoreTimerEvt = NULL;


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
	/* Initialize ZB stack */
	zb_init();

	/* Register stack CB */
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
    zcl_init(sampleLight_zclProcessIncomingMsg);

	/* Register endPoint */
	af_endpointRegister(SAMPLE_LIGHT_ENDPOINT, (af_simple_descriptor_t *)&sampleLight_simpleDesc, zcl_rx_handler, NULL);
#if AF_TEST_ENABLE
	/* A sample of AF data handler. */
	af_endpointRegister(SAMPLE_TEST_ENDPOINT, (af_simple_descriptor_t *)&sampleTestDesc, afTest_rx_handler, afTest_dataSendConfirm);
#endif

	/* Initialize or restore attributes, this must before 'zcl_register()' */
	zcl_sampleLightAttrsInit();
	zcl_reportingTabInit();

	/* Register ZCL specific cluster information */
	zcl_register(SAMPLE_LIGHT_ENDPOINT, SAMPLELIGHT_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)g_sampleLightClusterList);

#if ZCL_GP_SUPPORT
	/* Initialize GP */
	gp_init();
#endif

#if ZCL_OTA_SUPPORT
	/* Initialize OTA */
    ota_init(OTA_TYPE_CLIENT, (af_simple_descriptor_t *)&sampleLight_simpleDesc, &sampleLight_otaInfo, &sampleLight_otaCb);
#endif

#if ZCL_WWAH_SUPPORT
    /* Initialize WWAH server */
    wwah_init(WWAH_TYPE_SERVER, (af_simple_descriptor_t *)&sampleLight_simpleDesc);
#endif
}



s32 sampleLightAttrsStoreTimerCb(void *arg)
{
	zcl_onOffAttr_save();
	zcl_levelAttr_save();
	zcl_colorCtrlAttr_save();

	sampleLightAttrsStoreTimerEvt = NULL;
	return -1;
}

void sampleLightAttrsStoreTimerStart(void)
{
	if(sampleLightAttrsStoreTimerEvt){
		TL_ZB_TIMER_CANCEL(&sampleLightAttrsStoreTimerEvt);
	}
	sampleLightAttrsStoreTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleLightAttrsStoreTimerCb, NULL, 200);
}

void sampleLightAttrsChk(void)
{
	if(gLightCtx.lightAttrsChanged){
		gLightCtx.lightAttrsChanged = FALSE;
		if(zb_isDeviceJoinedNwk()){
			sampleLightAttrsStoreTimerStart();
		}
	}
}

void report_handler(void)
{
	if(zb_isDeviceJoinedNwk()){
		if(zcl_reportingEntryActiveNumGet()){
			u16 second = 1;//TODO: fix me

			reportNoMinLimit();

			//start report timer
			reportAttrTimerStart(second);
		}else{
			//stop report timer
			reportAttrTimerStop();
		}
	}
}

void app_task(void)
{
	app_key_handler();
	localPermitJoinState();
	if(BDB_STATE_GET() == BDB_STATE_IDLE){
		//factroyRst_handler();

		report_handler();

#if 0/* NOTE: If set to '1', the latest status of lighting will be stored. */
		sampleLightAttrsChk();
#endif
	}
}

static void sampleLightSysException(void)
{
	zcl_onOffAttr_save();
	zcl_levelAttr_save();
	zcl_colorCtrlAttr_save();

	SYSTEM_RESET();
	//led_on(LED_POWER);
	//while(1);
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

	/* Initialize LEDs*/
	led_init();
	hwLight_init();

	//factroyRst_init();

	/* Initialize Stack */
	stack_init();

	/* Initialize user application */
	user_app_init();

	/* Register except handler for test */
	sys_exceptHandlerRegister(sampleLightSysException);

	/* Adjust light state to default attributes*/
	light_adjust();

	/* User's Task */
#if ZBHCI_EN
	zbhciInit();
	ev_on_poll(EV_POLL_HCI, zbhciTask);
#endif
	ev_on_poll(EV_POLL_IDLE, app_task);

    /* Read the pre-install code from NV */
	if(bdb_preInstallCodeLoad(&gLightCtx.tcLinkKey.keyType, gLightCtx.tcLinkKey.key) == RET_OK){
		g_bdbCommissionSetting.linkKey.tcLinkKey.keyType = gLightCtx.tcLinkKey.keyType;
		g_bdbCommissionSetting.linkKey.tcLinkKey.key = gLightCtx.tcLinkKey.key;
	}

    /* Set default reporting configuration */
    u8 reportableChange = 0x00;
    bdb_defaultReportingCfg(SAMPLE_LIGHT_ENDPOINT, HA_PROFILE_ID, ZCL_CLUSTER_GEN_ON_OFF, ZCL_ATTRID_ONOFF,
    						0x0000, 0x003c, (u8 *)&reportableChange);

    /* Initialize BDB */
	bdb_init((af_simple_descriptor_t *)&sampleLight_simpleDesc, &g_bdbCommissionSetting, &g_zbDemoBdbCb, 1);
}

#endif  /* __PROJECT_TL_DIMMABLE_LIGHT__ */

