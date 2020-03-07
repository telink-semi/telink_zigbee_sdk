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
bool zcl_attr_wwah_disableOTADowngrades = FALSE;
bool zcl_attr_wwah_mgmtLeaveWithoutRejoinEnabled = TRUE;
u8 zcl_attr_wwah_nwkRetryCount = 3;
u8 zcl_attr_wwah_macRetryCount = 3;
bool zcl_attr_wwah_routerCheckInEnabled = FALSE;
bool zcl_attr_wwah_touchlinkInterpanEnabled = TRUE;
bool zcl_attr_wwah_parentClassificationEnabled = FALSE;
bool zcl_attr_wwah_appEvtRetryEnabled = TRUE;
u8 zcl_attr_wwah_appEvtRetryQueueSize = 10;
bool zcl_attr_wwah_rejoinEnabled = FALSE;
u8 zcl_attr_wwah_macPollFailureWaitTime = 3;
bool zcl_attr_wwah_configurationModeEnabled = TRUE;
u8 zcl_attr_wwah_currentDebugReportID = 0;
bool zcl_attr_wwah_tcSecurityOnNwkKeyRotationEnabled = FALSE;
bool zcl_attr_wwah_badParentRecoveryEnabled = FALSE;
u8 zcl_attr_wwah_pendingNwkUpdateChannel = 0xFF;
u16 zcl_attr_wwah_pendingNwkUpdatePanID = 0xFFFF;
u16 zcl_attr_wwah_otaMaxOfflineDuration = 0;

/* Attribute record list */
const zclAttrInfo_t wwah_attrTbl[] =
{
	{ ZCL_ATTRID_WWAH_DISABLE_OTA_DOWNGRADES, 			ZCL_DATA_TYPE_BOOLEAN, 	ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_disableOTADowngrades },
	{ ZCL_ATTRID_WWAH_MGMT_LEAVE_WITHOUT_REJOIN_ENABLED,ZCL_DATA_TYPE_BOOLEAN, 	ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_mgmtLeaveWithoutRejoinEnabled },
	{ ZCL_ATTRID_WWAH_NWK_RETRY_COUNT, 					ZCL_DATA_TYPE_UINT8, 	ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_nwkRetryCount },
	{ ZCL_ATTRID_WWAH_MAC_RETRY_COUNT, 					ZCL_DATA_TYPE_UINT8 , 	ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_macRetryCount },
	{ ZCL_ATTRID_WWAH_ROUTER_CHECK_IN_ENABLED, 			ZCL_DATA_TYPE_BOOLEAN, 	ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_routerCheckInEnabled },
	{ ZCL_ATTRID_WWAH_TOUCHLINK_INTERPAN_ENABLED, 		ZCL_DATA_TYPE_BOOLEAN, 	ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_touchlinkInterpanEnabled },
	{ ZCL_ATTRID_WWAH_PARENT_CLASSIFICATION_ENABLED, 	ZCL_DATA_TYPE_BOOLEAN, 	ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_parentClassificationEnabled },
	{ ZCL_ATTRID_WWAH_APP_EVT_RETRY_ENABLED, 			ZCL_DATA_TYPE_BOOLEAN, 	ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_appEvtRetryEnabled },
	{ ZCL_ATTRID_WWAH_APP_EVT_RETRY_QUEUE_SIZE, 		ZCL_DATA_TYPE_UINT8,  	ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_appEvtRetryQueueSize },
	{ ZCL_ATTRID_WWAH_REJOIN_ENABLED, 					ZCL_DATA_TYPE_BOOLEAN,  ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_rejoinEnabled },
	{ ZCL_ATTRID_WWAH_MAC_POLL_FAILURE_WAIT_TIME, 		ZCL_DATA_TYPE_UINT8,  	ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_macPollFailureWaitTime },
	{ ZCL_ATTRID_WWAH_CONFIGURATION_MODE_ENABLED, 		ZCL_DATA_TYPE_BOOLEAN,  ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_configurationModeEnabled },
	{ ZCL_ATTRID_WWAH_CURRENT_DEBUG_REPORT_ID, 			ZCL_DATA_TYPE_UINT8,  	ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_currentDebugReportID },
	{ ZCL_ATTRID_WWAH_TC_SECURITY_ON_NWK_KEY_ROTATION_ENABLED, ZCL_DATA_TYPE_BOOLEAN,  	ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_tcSecurityOnNwkKeyRotationEnabled },
	{ ZCL_ATTRID_WWAH_BAD_PARENT_RECOVERY_ENABLED, 		ZCL_DATA_TYPE_BOOLEAN,  ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_badParentRecoveryEnabled },
	{ ZCL_ATTRID_WWAH_PENDING_NWK_UPDATE_CHANNEL, 		ZCL_DATA_TYPE_UINT8,  	ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_pendingNwkUpdateChannel },
	{ ZCL_ATTRID_WWAH_PENDING_NWK_UPDATE_PANID, 		ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_pendingNwkUpdatePanID },
	{ ZCL_ATTRID_WWAH_OTA_MAX_OFFLINE_DURATION, 		ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ, (u8*)&zcl_attr_wwah_otaMaxOfflineDuration },
};

const u8 zcl_wwah_attrNum = ( sizeof(wwah_attrTbl) / sizeof(zclAttrInfo_t) );

#endif    /* ZCL_WWAH */
#endif
