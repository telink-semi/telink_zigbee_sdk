/********************************************************************************************************
 * @file    wwah.c
 *
 * @brief   This is the source file for wwah
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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

#include "../common/includes/zb_common.h"
#include "../zcl/zcl_include.h"
#include "wwah.h"
#ifdef ZCL_OTA
#include "ota.h"
#endif

#ifdef ZCL_WWAH

/**********************************************************************
 * TYPEDEFS
 */
typedef struct{
	ev_timer_event_t *periodicRouterCheckInEvt;
	epInfo_t	dstEpInfo;
	u16			manufCode;
	u8			srcEp;
}wwah_perRouterCheckInInfo_t;

/**********************************************************************
 * GLOBAL VARIABLES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */
wwah_perRouterCheckInInfo_t g_zcl_perRouterCheckInInfo;

/**********************************************************************
 * FUNCTIONS
 */

void wwah_init(wwah_type_e type, af_simple_descriptor_t *simpleDesc)
{
	if(type == WWAH_TYPE_CLIENT){
		//TODO:
		return;
	}

	zcl_register(simpleDesc->endpoint, WWAH_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)g_wwahClusterList);
}

static void zcl_wwah_reqNewApsLinkKey(void *arg)
{
	ss_apsmeRequestKeyReq_t requestKey;
	TL_SETSTRUCTCONTENT(requestKey, 0);

	requestKey.keyType = SS_KEYREQ_TYPE_TCLK;
	requestKey.dstAddr.shortAddr = 0x0000;
	requestKey.dstAddrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST;

	zb_apsmeRequestKeyReq(&requestKey);
}

static void zcl_wwah_rejoin(void *arg)
{
	zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
}

static s32 periodicRouterCheckInCb(void *arg)
{
	if(!zcl_wwah_getRouterCheckedFlag()){
		TL_SCHEDULE_TASK(zcl_wwah_rejoin, NULL);

		g_zcl_perRouterCheckInInfo.periodicRouterCheckInEvt = NULL;
		return -1;
	}

	zclReadCmd_t *pReadCmd = (zclReadCmd_t *)ev_buf_allocate(sizeof(zclReadCmd_t) + sizeof(u16));
	if(pReadCmd){
		pReadCmd->numAttr = 1;
		pReadCmd->attrID[0] = ZCL_ATTRID_GLOBAL_CLUSTER_REVISION;

		zcl_sendReadWithMfgCodeCmd( g_zcl_perRouterCheckInInfo.srcEp, &g_zcl_perRouterCheckInInfo.dstEpInfo,
									ZCL_CLUSTER_GEN_BASIC, MANUFACTURER_CODE_AMAZON,
									FALSE, ZCL_FRAME_CLIENT_SERVER_DIR, pReadCmd );

		ev_buf_free((u8 *)pReadCmd);

		zcl_wwah_clearRouterCheckedFlag();
	}

	return 0;
}

static status_t wwah_requestNewApsLinkKeyHandler(void)
{
	TL_SCHEDULE_TASK(zcl_wwah_reqNewApsLinkKey, NULL);

	return ZCL_STA_SUCCESS;
}

static status_t wwah_periodicRouterCheckInsHandler(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, zcl_wwah_enablePeriodicRouterCheckInsCmd_t *pCheckInCmd)
{
	status_t status = ZCL_STA_SUCCESS;

	if(cmdId == ZCL_CMD_WWAH_ENABLE_PERIODIC_ROUTER_CHECK_INS){
		if(pCheckInCmd->checkInInterval){
			g_zcl_wwahAttrs.routerCheckInEnabled = TRUE;

			if(g_zcl_perRouterCheckInInfo.periodicRouterCheckInEvt){
				TL_ZB_TIMER_CANCEL(&g_zcl_perRouterCheckInInfo.periodicRouterCheckInEvt);
			}
			if(pCheckInCmd->checkInInterval){
				g_zcl_perRouterCheckInInfo.srcEp = pAddrInfo->dstEp;
				g_zcl_perRouterCheckInInfo.dstEpInfo.dstEp = pAddrInfo->srcEp;
				g_zcl_perRouterCheckInInfo.dstEpInfo.profileId = pAddrInfo->profileId;
				g_zcl_perRouterCheckInInfo.dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
				g_zcl_perRouterCheckInInfo.dstEpInfo.dstAddr.shortAddr = pAddrInfo->srcAddr;
				g_zcl_perRouterCheckInInfo.dstEpInfo.txOptions |= APS_TX_OPT_ACK_TX;
	    		if(pAddrInfo->apsSec){
	    			g_zcl_perRouterCheckInInfo.dstEpInfo.txOptions |= APS_TX_OPT_SECURITY_ENABLED;
	    		}

				g_zcl_perRouterCheckInInfo.periodicRouterCheckInEvt = TL_ZB_TIMER_SCHEDULE(periodicRouterCheckInCb, NULL, pCheckInCmd->checkInInterval * 1000);
			}
		}else{
			status = ZCL_STA_FAILURE;
		}
	}else{	//ZCL_CMD_WWAH_DISABLE_PERIODIC_ROUTER_CHECK_INS
		g_zcl_wwahAttrs.routerCheckInEnabled = FALSE;

		if(g_zcl_perRouterCheckInInfo.periodicRouterCheckInEvt){
			TL_ZB_TIMER_CANCEL(&g_zcl_perRouterCheckInInfo.periodicRouterCheckInEvt);
		}
	}

	if(status == ZCL_STA_SUCCESS){
		//TODO: attr changed
	}

	return status;
}

static status_t wwah_setPendingNwkUpdateHandler(zclIncomingAddrInfo_t *pAddrInfo, zcl_wwah_setPendingNwkUpdateCmd_t *pSetPendingNwkUpdateCmd)
{
	g_zcl_wwahAttrs.pendingNwkUpdateChannel = pSetPendingNwkUpdateCmd->channel;
	g_zcl_wwahAttrs.pendingNwkUpdatePanID = pSetPendingNwkUpdateCmd->panID;

	zdo_af_set_accept_nwk_update_channel(pSetPendingNwkUpdateCmd->channel);
	zdo_af_set_accept_nwk_update_pan_id(pSetPendingNwkUpdateCmd->panID);

	//TODO: attr changed

	return ZCL_STA_SUCCESS;
}

static status_t wwah_cmdHandler(u8 cmdId)
{
	status_t status = ZCL_STA_SUCCESS;

	switch(cmdId){
		case ZCL_CMD_WWAH_DISABLE_OTA_DOWNGRADES:
			g_zcl_wwahAttrs.disableOTADowngrades = TRUE;
			break;
		case ZCL_CMD_WWAH_DISABLE_MGMT_LEAVE_WITHOUT_REJOIN:
			g_zcl_wwahAttrs.mgmtLeaveWithoutRejoinEnabled = FALSE;
			zdo_af_set_mgmtLeave_use_aps_sec(TRUE);
			break;
		case ZCL_CMD_WWAH_DISABLE_TOUCHLINK_INTERPAN_MSG_SUPPORT:
			g_zcl_wwahAttrs.touchlinkInterpanEnabled = FALSE;
			break;
		case ZCL_CMD_WWAH_ENABLE_PARENT_CLASSIFICATION:
			g_zcl_wwahAttrs.parentClassificationEnabled = TRUE;
			break;
		case ZCL_CMD_WWAH_DISABLE_PARENT_CLASSIFICATION:
			g_zcl_wwahAttrs.parentClassificationEnabled = FALSE;
			break;
		case ZCL_CMD_WWAH_ENABLE_TC_SECURITY_ON_NWK_KEY_ROTATION:
			g_zcl_wwahAttrs.tcSecurityOnNwkKeyRotationEnabled = TRUE;
			zdo_af_set_use_tc_sec_on_nwk_key_rotation(TRUE);
			break;
		case ZCL_CMD_WWAH_ENABLE_CONFIGURATION_MODE:
			g_zcl_wwahAttrs.configurationModeEnabled = TRUE;
			APS_IB().aps_zdo_restricted_mode = FALSE;
			break;
		case ZCL_CMD_WWAH_DISABLE_CONFIGURATION_MODE:
			g_zcl_wwahAttrs.configurationModeEnabled = FALSE;
			APS_IB().aps_zdo_restricted_mode = TRUE;
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
	}

	if(status == ZCL_STA_SUCCESS){
		//TODO: attr changed
	}

	return status;
}


static status_t wwah_useTrustCenterForClusterHandler(zclIncomingAddrInfo_t *pAddrInfo, wwah_useTCForClusterInfo_t *pUseTCForCluster)
{
#ifdef ZCL_OTA
	for(u8 i = 0; i < pUseTCForCluster->num; i++){
		if(pUseTCForCluster->clusterID[i] == ZCL_CLUSTER_OTA){
			ota_wwah_useTrustCenter(pAddrInfo->srcEp);
		}
	}
#endif

	return ZCL_STA_SUCCESS;
}

status_t zcl_wwahCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	status_t status = ZCL_STA_SUCCESS;

	if(pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR){
		switch(cmdId){
			case ZCL_CMD_WWAH_REQUEST_NEW_APS_LINK_KEY:
				status = wwah_requestNewApsLinkKeyHandler();
				break;
			case ZCL_CMD_WWAH_ENABLE_PERIODIC_ROUTER_CHECK_INS:
			case ZCL_CMD_WWAH_DISABLE_PERIODIC_ROUTER_CHECK_INS:
				status = wwah_periodicRouterCheckInsHandler(pAddrInfo, cmdId, (zcl_wwah_enablePeriodicRouterCheckInsCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_WWAH_SET_PENDING_NWK_UPDATE:
				status = wwah_setPendingNwkUpdateHandler(pAddrInfo, (zcl_wwah_setPendingNwkUpdateCmd_t *)cmdPayload);
				break;
			case ZCL_CMD_WWAH_DISABLE_OTA_DOWNGRADES:
			case ZCL_CMD_WWAH_DISABLE_MGMT_LEAVE_WITHOUT_REJOIN:
			case ZCL_CMD_WWAH_DISABLE_TOUCHLINK_INTERPAN_MSG_SUPPORT:
			case ZCL_CMD_WWAH_ENABLE_PARENT_CLASSIFICATION:
			case ZCL_CMD_WWAH_DISABLE_PARENT_CLASSIFICATION:
			case ZCL_CMD_WWAH_ENABLE_TC_SECURITY_ON_NWK_KEY_ROTATION:
			case ZCL_CMD_WWAH_ENABLE_CONFIGURATION_MODE:
			case ZCL_CMD_WWAH_DISABLE_CONFIGURATION_MODE:
				status = wwah_cmdHandler(cmdId);
				break;
			case ZCL_CMD_WWAH_USE_TRUST_CENTER_FOR_CLUSTER:
				status = wwah_useTrustCenterForClusterHandler(pAddrInfo, (wwah_useTCForClusterInfo_t *)cmdPayload);
				break;
			default:
				status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
				break;
		}
	}else{
		status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
	}

	return status;
}

#endif /* ZCL_WWAH */
