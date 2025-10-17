/********************************************************************************************************
 * @file    sampleGateway.c
 *
 * @brief   This is the source file for sampleGateway
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
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
    .fileVer          = FILE_VERSION,
    .imageType        = IMAGE_TYPE,
    .manufacturerCode = MANUFACTURER_CODE_TELINK,
};
#endif

//Must declare the application call back function which used by ZDO layer
const zdo_appIndCb_t appCbLst = {
    bdb_zdoStartDevCnf,                 //start device cnf cb
    NULL,                               //reset cnf cb
    sampleGW_devAnnHandler,             //device announce indication cb
    sampleGW_leaveIndHandler,           //leave ind cb
    sampleGW_leaveCnfHandler,           //leave cnf cb
    sampleGW_nwkUpdateIndicateHandler,  //nwk update ind cb
    sampleGW_permitJoinIndHandler,      //permit join ind cb
    NULL,                               //nlme sync cnf cb
    sampleGW_tcJoinIndHandler,          //tc join ind cb
    sampleGW_tcFrameCntReachedHandler,  //tc detects that the frame counter is near limit
};

/**
 *  @brief Definition for bdb commissioning setting
 */
bdb_commissionSetting_t g_bdbCommissionSetting = {
    .linkKey.tcLinkKey.keyType = SS_GLOBAL_LINK_KEY,
    .linkKey.tcLinkKey.key     = (u8 *)tcLinkKeyCentralDefault,         //can use unique link key stored in NV

    .linkKey.distributeLinkKey.keyType = MASTER_KEY,
    .linkKey.distributeLinkKey.key     = (u8 *)linkKeyDistributedMaster,//use linkKeyDistributedCertification before testing

    .linkKey.touchLinkKey.keyType = MASTER_KEY,
    .linkKey.touchLinkKey.key     = (u8 *)touchLinkKeyMaster,           //use touchLinkKeyCertification before testing

    .touchlinkEnable = 0,                                               //disable touch link for coordinator
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

#if ZCL_GP_SUPPORT
    gp_init(SAMPLE_GW_ENDPOINT);
#endif

#if ZCL_OTA_SUPPORT
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
    app_key_handler();
    localPermitJoinState();
}

static void sampleGwSysException(void)
{
#if 1
    SYSTEM_RESET();
#else
    gpio_toggle(LED_POWER);
    while(1);
#endif
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
    if (firmwareCheckWithUID()) {
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
