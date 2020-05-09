/********************************************************************************************************
 * @file     sampleLight.c
 *
 * @brief    HA Router
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

#if (__PROJECT_DUAL_MODE_LIGHT__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "bdb.h"
#include "ota.h"
#include "gp.h"
#include "factory_reset.h"
#include "sampleLight.h"
#include "sampleLightCtrl.h"
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
app_ctx_t gLightCtx;


#ifdef ZCL_OTA
extern ota_callBack_t sampleLight_otaCb;

//running code firmware information
ota_preamble_t sampleLight_otaInfo = {
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
		sampleLight_leaveIndHandler,//leave ind cb
		sampleLight_leaveCnfHandler,//leave cnf cb
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
	.touchlinkLqiThreshold = 0xe0,			   							/* threshold for touch-link scan req/resp command */
};

/**********************************************************************
 * LOCAL VARIABLES
 */
ev_time_event_t *sampleLightAttrsStoreTimerEvt = NULL;


/**********************************************************************
 * FUNCTIONS
 */
#if DUAL_MODE
void dualModeDisable(void)
{
	u8 dualModeEnable = 0;
	flash_read(CFG_TELINK_DUAL_MODE_ENABLE, 1, (u8 *)&dualModeEnable);
	if(dualModeEnable == 0x5a){
		dualModeEnable = 0;
		flash_write(CFG_TELINK_DUAL_MODE_ENABLE, 1, (u8 *)&dualModeEnable);
	}
}

bool isDualModeEnable(void)
{
	u8 dualModeEnable = 0;
	flash_read(CFG_TELINK_DUAL_MODE_ENABLE, 1, (u8 *)&dualModeEnable);

	return (dualModeEnable == 0x5a) ? TRUE : FALSE;
}

void dualModeInit(void)
{
    u32 sdkType = 0;
    u32 zigbeeSdkType = TYPE_TLK_ZIGBEE;

    flash_read(CFG_TELINK_SDK_TYPE, 4, (u8 *)&sdkType);
    if(sdkType != TYPE_TLK_ZIGBEE){
    	//Store SDK type.
		if(sdkType == 0xFFFFFFFF){
			flash_write(CFG_TELINK_SDK_TYPE, 4, (u8 *)&zigbeeSdkType);
		}else{
			nv_resetAll();
			flash_erase(CFG_TELINK_SDK_TYPE);
			flash_write(CFG_TELINK_SDK_TYPE, 4, (u8 *)&zigbeeSdkType);
		}

		if(!isDualModeEnable()){
			return;
		}

		u8 flashInfo = 0;
		flash_read(0 + 8, 1, &flashInfo);
		if((flashInfo != 0x4b) && (flashInfo != 0xff)){
			//Store SIG Mesh Code.
			flash_erase(CFG_TELINK_SIG_MESH_CODE_4K);

			u32 crcValue = 0xffffffff;
			u32 addressOffset = 0;
			u8 buf[FLASH_PAGE_SIZE] = {0};
			for(u8 i = 0; i < (FLASH_SECTOR_SIZE /FLASH_PAGE_SIZE ); i++){
				memset((u8 *)buf, 0, FLASH_PAGE_SIZE);
				flash_read(addressOffset, FLASH_PAGE_SIZE, buf);
				crcValue = xcrc32(buf, FLASH_PAGE_SIZE, crcValue);
				flash_write(CFG_TELINK_SIG_MESH_CODE_4K + addressOffset, FLASH_PAGE_SIZE, buf);
				addressOffset += FLASH_PAGE_SIZE;
			}

			flash_write(CFG_TELINK_SIG_MESH_CRC, 4, (u8 *)&crcValue);
		}
    }else{
    	//do nothing.
    }
}

void dualModeRecovery(void)
{
	u32 value = 0xffffffff;
	flash_read(CFG_TELINK_SIG_MESH_CRC, 4, (u8 *)&value);

	if(isDualModeEnable() && (value != 0xffffffff)){
		u32 crcValue = 0xffffffff;
		u32 addressOffset = 0;

		flash_erase(0);

		for(u8 i = 0; i < (FLASH_SECTOR_SIZE /FLASH_PAGE_SIZE ); i++){
			u8 buf[FLASH_PAGE_SIZE] = {0};
			flash_read(CFG_TELINK_SIG_MESH_CODE_4K + addressOffset, FLASH_PAGE_SIZE, buf);
			crcValue = xcrc32(buf, FLASH_PAGE_SIZE, crcValue);
			if(addressOffset == 0){
				buf[8] = 0xff;
			}
			flash_write(addressOffset, FLASH_PAGE_SIZE, buf);
			addressOffset += FLASH_PAGE_SIZE;
		}

		if(crcValue == value){
			u8 flashInfo = 0x4b;
			flash_write((0 + 8), 1, &flashInfo);//enable boot-up flag

			u32 sdkType = TYPE_DUAL_MODE_RECOVER;
			flash_erase(CFG_TELINK_SDK_TYPE);
			flash_write(CFG_TELINK_SDK_TYPE, 4, (u8 *)&sdkType);

			SYSTEM_RESET();
		}
	}
}
#endif

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
#if DUAL_MODE
	dualModeInit();
#endif
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
	af_nodeDescManuCodeUpdate(TELINK_MANUFACTURER_CODE);

    /* Initialize ZCL layer */
	/* Register Incoming ZCL Foundation command/response messages */
    zcl_init(sampleLight_zclProcessIncomingMsg);

	/* Register endPoint */
	af_endpointRegister(SAMPLE_LIGHT_ENDPOINT, (af_simple_descriptor_t *)&sampleLight_simpleDesc, zcl_rx_handler, NULL);

	/* Initialize or restore attributes, this must before 'zcl_register()' */
	zcl_sampleLightAttrsInit();
	zcl_reportingTabInit();

	/* Register ZCL specific cluster information */
	zcl_register(SAMPLE_LIGHT_ENDPOINT, SAMPLELIGHT_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)g_sampleLightClusterList);

#ifdef ZCL_GREEN_POWER
	/* Initialize GP */
	gp_init();
#endif

#ifdef ZCL_OTA
	/* Initialize OTA */
    ota_init(OTA_TYPE_CLIENT, (af_simple_descriptor_t *)&sampleLight_simpleDesc, &sampleLight_otaInfo, &sampleLight_otaCb);
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
		ev_on_timer(sampleLightAttrsStoreTimerEvt, 200 * 1000);
	}else{
		sampleLightAttrsStoreTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleLightAttrsStoreTimerCb, NULL, 200 * 1000);
	}
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
	if(BDB_STATE_GET() == BDB_STATE_IDLE){
		localPermitJoinState();
		report_handler();

#if 0/* NOTE: If set to '1', the latest status of lighting will be stored. */
		sampleLightAttrsChk();
#endif
	}
}

void factoryRst_init(void)
{
	gLightCtx.powerCntFacRst = 0;
	if(factory_reset_handle()){
		gLightCtx.powerCntFacRst = 1;
	}
}

void factoryRst_task(void)
{
	if(BDB_STATE_GET() == BDB_STATE_IDLE){
		if(gLightCtx.powerCntFacRst){
			gLightCtx.powerCntFacRst = 0;
			gLightCtx.powerCntFacRst2SigMesh = 1;

			factory_reset();
		}else{
			factory_reset_cnt_check();
		}
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
	hwLight_init();

	/* Power up counter check */
	factoryRst_init();

	/* Initialize Stack */
	stack_init();

	/* Initialize user application */
	user_app_init();

	/* Adjust light state to default attributes*/
	light_adjust();

	/* User's Task */
#if ZBHCI_EN
	zbhciInit();
	ev_on_poll(EV_POLL_HCI, zbhciTask);
#endif
	ev_on_poll(EV_POLL_FACTORY_RST, factoryRst_task);
	ev_on_poll(EV_POLL_IDLE, app_task);

    /* Read the pre-insatll code from NV */
    zb_pre_install_code_load(&gLightCtx.linkKey);

    /* Set default reporting configuration */
    u8 reportableChange = 0x00;
    bdb_defaultReportingCfg(SAMPLE_LIGHT_ENDPOINT, HA_PROFILE_ID, ZCL_CLUSTER_GEN_ON_OFF, ZCL_ATTRID_ONOFF,
    						0x0000, 0x003c, (u8 *)&reportableChange);

    /* Initialize BDB */
	bdb_init((af_simple_descriptor_t *)&sampleLight_simpleDesc, &g_bdbCommissionSetting, &g_zbDemoBdbCb, 1);
}

#endif  /* __PROJECT_DUAL_MODE_LIGHT__ */

