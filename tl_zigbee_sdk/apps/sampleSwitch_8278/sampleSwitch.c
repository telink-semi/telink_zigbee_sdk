/********************************************************************************************************
 * @file     sampleSwitch.c
 *
 * @brief    HA end device
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
#if PM_ENABLE
#include "pm_interface.h"
#endif
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
app_ctx_t g_switchAppCtx;


#ifdef ZCL_OTA
extern ota_callBack_t sampleSwitch_otaCb;

//running code firmware information
ota_preamble_t sampleSwitch_otaInfo = {
		.fileVer = CURRENT_FILE_VERSION,
		.imageType = IMAGE_TYPE,
		.manufacturerCode = TELINK_MANUFACTURER_CODE,
};
#endif

extern void bdb_zdoStartDevCnf(void* arg);

//Must declare the application call back function which used by ZDO layer
const zdo_appIndCb_t appCbLst = {
		bdb_zdoStartDevCnf,//start device cnf cb
		NULL,//reset cnf cb
		NULL,//device announce indication cb
		sampleSwitch_leaveIndHandler,//leave ind cb
		sampleSwitch_leaveCnfHandler,//leave cnf cb
		NULL,//nwk update ind cb
		NULL,//permit join ind cb
		NULL,//nlme sync cnf cb
};


/**
 *  @brief Definition for BDB finding and binding cluster
 */
u16 bdb_findBindClusterList[] =
{
	ZCL_CLUSTER_GEN_ON_OFF,
};

/**
 *  @brief Definition for BDB finding and binding cluster number
 */
#define FIND_AND_BIND_CLUSTER_NUM		(sizeof(bdb_findBindClusterList)/sizeof(bdb_findBindClusterList[0]))

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
	.touchlinkLqiThreshold = 0x3f,			   							/* threshold for touch-link scan req/resp command */
};

#if PM_ENABLE
/**
 *  @brief Definition for wakeup source and level for PM
 */
pm_pinCfg_t g_switchPmCfg[] = {
	{
		BUTTON1,
		PM_WAKEUP_LEVEL
	},
	{
		BUTTON2,
		PM_WAKEUP_LEVEL
	}
};
#endif
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
#ifdef ZCL_POLL_CTRL
	af_powerDescPowerModeUpdate(POWER_MODE_RECEIVER_COMES_PERIODICALLY);
#else
	af_powerDescPowerModeUpdate(POWER_MODE_RECEIVER_COMES_WHEN_STIMULATED);
#endif

    /* Initialize ZCL layer */
	/* Register Incoming ZCL Foundation command/response messages */
	zcl_init(sampleSwitch_zclProcessIncomingMsg);

	/* register endPoint */
	af_endpointRegister(SAMPLE_SWITCH_ENDPOINT, (af_simple_descriptor_t *)&sampleSwitch_simpleDesc, zcl_rx_handler, NULL);

	/* Register ZCL specific cluster information */
	zcl_register(SAMPLE_SWITCH_ENDPOINT, SAMPLE_SWITCH_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)g_sampleSwitchClusterList);

#ifdef ZCL_OTA
    ota_init(OTA_TYPE_CLIENT, (af_simple_descriptor_t *)&sampleSwitch_simpleDesc, &sampleSwitch_otaInfo, &sampleSwitch_otaCb);
#endif
}



void led_init(void)
{
	light_init();
}

void app_task(void)
{
	app_key_handler();

	if(bdb_isIdle()){
#if PM_ENABLE
#if INTERLEAVE_SLEEP_MODE
		if(!tl_stackBusy() && zb_isTaskDone()){
			if(zb_getPollRate()){
				zb_setPollRate(0);
			}

			if(!zb_getPollRate()){
				keepaliveMsgSendStop();
			}
		}
#endif

		/* if PAD wake up, wait until de-bounce finished*/
		if(g_switchAppCtx.restartLoopTimes++ >= KEY_PRESSED_CHECK_TIMES && !g_switchAppCtx.keyPressed){
#if INTERLEAVE_SLEEP_MODE
			u32 sleepMs = 2 * 60 * 1000;
			pm_interleaveSleepEnter(PLATFORM_WAKEUP_TIMER | PLATFORM_WAKEUP_PAD, sleepMs);
#else
			pm_deepSleepEnter(PLATFORM_WAKEUP_TIMER | PLATFORM_WAKEUP_PAD, zb_getPollRate());
#endif
		}
#endif
	}
}

static void sampleSwitchSysException(void)
{
	SYSTEM_RESET();
	//light_on();
	//while(1);
}

/*********************************************************************
 * @fn      user_init
 *
 * @brief   User level initialization code.
 *
 * @param   None
 *
 * @return  None
 */
void user_init(void)
{
	/* Initialize LEDs*/
	led_init();

#if PA_ENABLE
	rf_paInit(PA_TX, PA_RX);
#endif

#if ZBHCI_EN
	zbhciInit();
#endif

#if PM_ENABLE
	pm_wakeupPinConfig(g_switchPmCfg, sizeof(g_switchPmCfg)/sizeof(pm_pinCfg_t));
#endif

	if(pmParam.is_pad_wakeup){
		g_switchAppCtx.restartLoopTimes = 0;
	}else{
		g_switchAppCtx.restartLoopTimes = KEY_PRESSED_CHECK_TIMES;
	}

	if(pmParam.back_mode == BACK_FROM_REPOWER || pmParam.back_mode == BACK_FROM_DEEP){
		/* Initialize Stack */
		stack_init();

		/* Initialize user application */
		user_app_init();

		/* User's Task */
#if ZBHCI_EN
		ev_on_poll(EV_POLL_HCI, zbhciTask);
#endif
		ev_on_poll(EV_POLL_IDLE, app_task);

		/* read the pre-insatll code in NV, */
		zb_pre_install_code_load(&g_switchAppCtx.linkKey);

		bdb_findBindMatchClusterSet(FIND_AND_BIND_CLUSTER_NUM, bdb_findBindClusterList);

		/* Initialize BDB */
		u8 repower = deep_sleep_flag_get() ? 0 : 1;

		bdb_init((af_simple_descriptor_t *)&sampleSwitch_simpleDesc, &g_bdbCommissionSetting, &g_zbDemoBdbCb, repower);
	}else{
		mac_phyReconfig();
	}

	/* Register except handler for test */
	sys_exceptHandlerRegister(sampleSwitchSysException);

#if INTERLEAVE_SLEEP_MODE
	if(!zb_getPollRate() && zb_isDeviceJoinedNwk()){
		zb_endDeviceSyncReq();
	}
#endif
}

#endif  /* __PROJECT_TL_SWITCH_8278__ */

