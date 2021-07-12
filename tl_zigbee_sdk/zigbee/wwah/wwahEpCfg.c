/********************************************************************************************************
 * @file	wwahEpCfg.c
 *
 * @brief	This is the source file for wwahEpCfg
 *
 * @author	Zigbee Group
 * @date	2021
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
/**********************************************************************
 * INCLUDES
 */
#include "../zcl/zcl_include.h"
#include "wwah.h"

#ifdef ZCL_WWAH

/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * GLOBAL VARIABLES
 */
/* Attribute default */
zcl_wwahAttr_t g_zcl_wwahAttrs =
{
	.disableOTADowngrades 				= FALSE,
	.mgmtLeaveWithoutRejoinEnabled 		= TRUE,
	.nwkRetryCount						= 3,
	.macRetryCount						= 3,
	.routerCheckInEnabled				= FALSE,
	.touchlinkInterpanEnabled			= TRUE,
	.parentClassificationEnabled		= FALSE,
	.configurationModeEnabled			= TRUE,
	.currentDebugReportID				= 0,
	.tcSecurityOnNwkKeyRotationEnabled  = FALSE,
	.pendingNwkUpdateChannel			= 0xFF,
	.pendingNwkUpdatePanID				= 0xFFFF,
	.otaMaxOfflineDuration				= 0,
};

/* Attribute record list */
const zclAttrInfo_t wwah_attrTbl[] =
{
	{ ZCL_ATTRID_WWAH_DISABLE_OTA_DOWNGRADES, 			ZCL_DATA_TYPE_BOOLEAN, 	ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.disableOTADowngrades },
	{ ZCL_ATTRID_WWAH_MGMT_LEAVE_WITHOUT_REJOIN_ENABLED,ZCL_DATA_TYPE_BOOLEAN, 	ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.mgmtLeaveWithoutRejoinEnabled },
	{ ZCL_ATTRID_WWAH_NWK_RETRY_COUNT, 					ZCL_DATA_TYPE_UINT8, 	ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.nwkRetryCount },
	{ ZCL_ATTRID_WWAH_MAC_RETRY_COUNT, 					ZCL_DATA_TYPE_UINT8 , 	ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.macRetryCount },
	{ ZCL_ATTRID_WWAH_ROUTER_CHECK_IN_ENABLED, 			ZCL_DATA_TYPE_BOOLEAN, 	ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.routerCheckInEnabled },
	{ ZCL_ATTRID_WWAH_TOUCHLINK_INTERPAN_ENABLED, 		ZCL_DATA_TYPE_BOOLEAN, 	ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.touchlinkInterpanEnabled },
	{ ZCL_ATTRID_WWAH_PARENT_CLASSIFICATION_ENABLED, 	ZCL_DATA_TYPE_BOOLEAN, 	ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.parentClassificationEnabled },
	{ ZCL_ATTRID_WWAH_CONFIGURATION_MODE_ENABLED, 		ZCL_DATA_TYPE_BOOLEAN,  ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.configurationModeEnabled },
	{ ZCL_ATTRID_WWAH_CURRENT_DEBUG_REPORT_ID, 			ZCL_DATA_TYPE_UINT8,  	ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.currentDebugReportID },
	{ ZCL_ATTRID_WWAH_TC_SECURITY_ON_NWK_KEY_ROTATION_ENABLED, ZCL_DATA_TYPE_BOOLEAN,  	ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.tcSecurityOnNwkKeyRotationEnabled },
	{ ZCL_ATTRID_WWAH_PENDING_NWK_UPDATE_CHANNEL, 		ZCL_DATA_TYPE_UINT8,  	ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.pendingNwkUpdateChannel },
	{ ZCL_ATTRID_WWAH_PENDING_NWK_UPDATE_PANID, 		ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.pendingNwkUpdatePanID },
	{ ZCL_ATTRID_WWAH_OTA_MAX_OFFLINE_DURATION, 		ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.otaMaxOfflineDuration },

	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 				ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ, (u8*)&zcl_attr_global_clusterRevision },
};

#define ZCL_WWAH_ATTR_NUM	 sizeof(wwah_attrTbl) / sizeof(zclAttrInfo_t)


/**
 *  @brief Definition for WWAH ZCL specific cluster
 */
const zcl_specClusterInfo_t g_wwahClusterList[] =
{
	{ZCL_CLUSTER_WWAH, MANUFACTURER_CODE_AMAZON, ZCL_WWAH_ATTR_NUM,	wwah_attrTbl, zcl_wwah_register, zcl_wwahCb},
};

u8 WWAH_CB_CLUSTER_NUM = (sizeof(g_wwahClusterList)/sizeof(g_wwahClusterList[0]));

#endif /* ZCL_WWAH */
