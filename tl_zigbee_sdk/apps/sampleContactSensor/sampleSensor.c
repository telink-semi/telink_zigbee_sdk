/********************************************************************************************************
 * @file     sampleSensor.c
 *
 * @brief    HA endDevice
 *
 * @author
 * @date     Jan. 4, 2018
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

#if (__PROJECT_TL_CONTACT_SENSOR__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "bdb.h"
#include "ota.h"
#include "sampleSensor.h"
#include "app_ui.h"
#if PM_ENABLE
#include "pm_interface.h"
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
app_ctx_t g_sensorAppCtx;


#ifdef ZCL_OTA
extern ota_callBack_t sampleSensor_otaCb;

//running code firmware information
ota_preamble_t sampleSensor_otaInfo = {
		.fileVer = CURRENT_FILE_VERSION,
		.imageType = IMAGE_TYPE,
		.manufaurerCode = TELINK_MANUFACTURER_CODE,
};
#endif

extern void bdb_zdoStartDevCnf(void* arg);

//Must declare the application call back function which used by ZDO layer
const zdo_appIndCb_t appCbLst = {
		bdb_zdoStartDevCnf,//start device cnf cb
		NULL,//reset cnf cb
		NULL,//device announce indication cb
		sampleSensor_leaveIndHandler,//leave ind cb
		sampleSensor_leaveCnfHandler,//leave cnf cb
		NULL,//nwk update ind cb
		NULL,//permit join ind cb
		NULL,//nlme sync cnf cb
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
	.touchlinkLqiThreshold = 0x3f,			   							/* threshold for touch-link scan req/resp command */
};

#if PM_ENABLE
/**
 *  @brief Definition for wakeup source and level for PM
 */
pm_pinCfg_t g_SensorPmCfg[] = {
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
	zb_init();
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
	zcl_init(sampleSensor_zclProcessIncomingMsg);

	/* Register endPoint */
	af_endpointRegister(SAMPLE_SENSOR_ENDPOINT, (af_simple_descriptor_t *)&sampleSensor_simpleDesc, zcl_rx_handler, NULL);

	/* Register ZCL specific cluster information */
	zcl_register(SAMPLE_SENSOR_ENDPOINT, SAMPLE_SENSOR_CB_CLUSTER_NUM, g_sampleSensorClusterList);

#ifdef ZCL_OTA
    ota_init(OTA_TYPE_CLIENT, (af_simple_descriptor_t *)&sampleSensor_simpleDesc, &sampleSensor_otaInfo, &sampleSensor_otaCb);
#endif
}



void led_init(void)
{
	light_init();
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

	if(bdb_isIdle()){
#if PM_ENABLE
		if(!g_sensorAppCtx.keyPressed){
			pm_suspendEnter(PLATFORM_WAKEUP_TIMER | PLATFORM_WAKEUP_PAD, zb_getPollRate());
		}
#endif

		report_handler();
	}
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

	/* Initialize Stack */
	stack_init();

	/* Initialize user application */
	user_app_init();

	/* User's Task */
#if ZBHCI_EN
	zbhciInit();
	ev_on_poll(EV_POLL_HCI, zbhciTask);
#endif

#if PM_ENABLE
	pm_wakeupPinConfig(g_SensorPmCfg, sizeof(g_SensorPmCfg)/sizeof(pm_pinCfg_t));
#endif

	ev_on_poll(EV_POLL_IDLE, app_task);

#if PM_ENABLE
	if(deep_sleep_flag_get()){
		bdb_init((af_simple_descriptor_t *)&sampleSensor_simpleDesc, &g_bdbCommissionSetting, &g_zbDemoBdbCb, 0);
	}else
#endif
	{
		bdb_init((af_simple_descriptor_t *)&sampleSensor_simpleDesc, &g_bdbCommissionSetting, &g_zbDemoBdbCb, 1);
	}
}

#endif  /* __PROJECT_TL_CONTACT_SENSOR__ */
