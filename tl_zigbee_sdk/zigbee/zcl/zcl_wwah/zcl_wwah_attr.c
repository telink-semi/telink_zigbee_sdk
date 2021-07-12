/********************************************************************************************************
 * @file	zcl_wwah_attr.c
 *
 * @brief	This is the source file for zcl_wwah_attr
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
