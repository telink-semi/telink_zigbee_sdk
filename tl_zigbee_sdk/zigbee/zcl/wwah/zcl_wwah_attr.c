/********************************************************************************************************
 * @file     zcl_wwah_attr.c
 *
 * @brief	 attribute configuration for WWAH cluster
 *
 * @author
 * @date     June. 10, 2017
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
#if 0
#include "tl_common.h"

/**********************************************************************
 * INCLUDES
 */
#include "../zcl_include.h"


/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * LOCAL TYPES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */
#ifdef ZCL_WWAH

/* Attribute default */
u8 zcl_attr_wwah_version = 1;
addrExt_t zcl_attr_wwah_hubAddress = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
bool zcl_attr_wwah_otaFromHubOnly = FALSE;
bool zcl_attr_wwah_disableOTADowngrades = FALSE;
bool zcl_attr_wwah_disableTCRejoins = FALSE;
u8 zcl_attr_wwah_nwkRetryCount = 0;
u8 zcl_attr_wwah_macRetryCount = 0;
bool zcl_attr_wwah_routerCheckInEnabled = FALSE;
bool zcl_attr_wwah_routingCapability = TRUE;
bool zcl_attr_wwah_parentingCapability = TRUE;
bool zcl_attr_wwah_appEvtRetryEnabled = FALSE;
u8 zcl_attr_wwah_appEvtRetryQueueSize = 0;
bool zcl_attr_wwah_rejoinEnabled = FALSE;
u8 zcl_attr_wwah_macPollCCAWaitTime = 3;


/* Attribute record list */
const zclAttrInfo_t wwah_attrTbl[] =
{
	{ ZCL_ATTRID_WWAH_VERSION,  				ZCL_DATA_TYPE_UINT8,		ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_version },
	{ ZCL_ATTRID_WWAH_HUB_ADDRESS, 				ZCL_DATA_TYPE_IEEE_ADDR, 	ACCESS_CONTROL_READ, (u8*)zcl_attr_wwah_hubAddress },
	{ ZCL_ATTRID_WWAH_FROM_HUB_ONLY, 			ZCL_DATA_TYPE_BOOLEAN, 		ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_otaFromHubOnly },
	{ ZCL_ATTRID_WWAH_DISABLE_OTA_DOWNGRADES, 	ZCL_DATA_TYPE_BOOLEAN, 		ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_disableOTADowngrades },
	{ ZCL_ATTRID_WWAH_DISABLE_TC_REJOINS, 		ZCL_DATA_TYPE_BOOLEAN, 		ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_disableTCRejoins },
	{ ZCL_ATTRID_WWAH_NWK_RETRY_COUNT, 			ZCL_DATA_TYPE_UINT8, 		ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_nwkRetryCount },
	{ ZCL_ATTRID_WWAH_MAC_RETRY_COUNT, 			ZCL_DATA_TYPE_UINT8 , 		ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_macRetryCount },
	{ ZCL_ATTRID_WWAH_ROUTER_CHECK_IN_ENABLED, 	ZCL_DATA_TYPE_BOOLEAN, 		ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_routerCheckInEnabled },
	{ ZCL_ATTRID_WWAH_ROUTING_CAPABILITY, 		ZCL_DATA_TYPE_BOOLEAN, 		ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_wwah_routingCapability },
	{ ZCL_ATTRID_WWAH_PARENTING_CAPABILITY, 	ZCL_DATA_TYPE_BOOLEAN, 		ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zcl_attr_wwah_parentingCapability },
	{ ZCL_ATTRID_WWAH_APP_EVT_RETRY_ENABLED, 	ZCL_DATA_TYPE_BOOLEAN, 		ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_appEvtRetryEnabled },
	{ ZCL_ATTRID_WWAH_APP_EVT_RETRY_QUEUE_SIZE, ZCL_DATA_TYPE_UINT8,  		ACCESS_CONTROL_READ,  (u8*)&zcl_attr_wwah_appEvtRetryQueueSize},
	{ ZCL_ATTRID_WWAH_REJOIN_ENABLED, 			ZCL_DATA_TYPE_BOOLEAN,  	ACCESS_CONTROL_READ,  (u8*)&zcl_attr_wwah_rejoinEnabled},
	{ ZCL_ATTRID_WWAH_MAC_POLL_CCA_WAIT_TIME, 	ZCL_DATA_TYPE_UINT8,  		ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_wwah_macPollCCAWaitTime},
};

const u8 zcl_wwah_attrNum = ( sizeof(wwah_attrTbl) / sizeof(zclAttrInfo_t) );

#endif    /* ZCL_WWAH */
#endif
