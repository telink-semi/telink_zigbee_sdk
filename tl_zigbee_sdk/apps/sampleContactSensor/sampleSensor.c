/********************************************************************************************************
 * @file	sampleSensor.c
 *
 * @brief	This is the source file for sampleSensor
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
app_ctx_t g_sensorAppCtx;


#ifdef ZCL_OTA
extern ota_callBack_t sampleSensor_otaCb;

//running code firmware information
ota_preamble_t sampleSensor_otaInfo = {
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
	sampleSensor_leaveIndHandler,//leave ind cb
	sampleSensor_leaveCnfHandler,//leave cnf cb
	NULL,//nwk update ind cb
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

#if PM_ENABLE
/**
 *  @brief Definition for wakeup source and level for PM
 */
drv_pm_pinCfg_t g_sensorPmCfg[] = {
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
	zcl_register(SAMPLE_SENSOR_ENDPOINT, SAMPLE_SENSOR_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)g_sampleSensorClusterList);

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
			drv_pm_lowPowerEnter();
		}
#endif

		report_handler();
	}
}

static void sampleSensorSysException(void)
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
 * @param   isRetention - if it is waking up with ram retention.
 *
 * @return  None
 */
void user_init(bool isRetention)
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
	drv_pm_wakeupPinConfig(g_sensorPmCfg, sizeof(g_sensorPmCfg)/sizeof(drv_pm_pinCfg_t));
#endif

	if(!isRetention){
		/* Initialize Stack */
		stack_init();

		/* Initialize user application */
		user_app_init();

		/* Register except handler for test */
		sys_exceptHandlerRegister(sampleSensorSysException);

		/* User's Task */
#if ZBHCI_EN
		ev_on_poll(EV_POLL_HCI, zbhciTask);
#endif
		ev_on_poll(EV_POLL_IDLE, app_task);

		/* Load the pre-install code from flash */
		if(bdb_preInstallCodeLoad(&g_sensorAppCtx.tcLinkKey.keyType, g_sensorAppCtx.tcLinkKey.key) == RET_OK){
			g_bdbCommissionSetting.linkKey.tcLinkKey.keyType = g_sensorAppCtx.tcLinkKey.keyType;
			g_bdbCommissionSetting.linkKey.tcLinkKey.key = g_sensorAppCtx.tcLinkKey.key;
		}

		/* Initialize BDB */
		u8 repower = drv_pm_deepSleep_flag_get() ? 0 : 1;
		bdb_init((af_simple_descriptor_t *)&sampleSensor_simpleDesc, &g_bdbCommissionSetting, &g_zbDemoBdbCb, repower);
	}else{
		/* Re-config phy when system recovery from deep sleep with retention */
		mac_phyReconfig();
	}
}

#endif  /* __PROJECT_TL_CONTACT_SENSOR__ */
