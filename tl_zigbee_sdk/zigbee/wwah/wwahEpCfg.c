/********************************************************************************************************
 * @file    wwahEpCfg.c
 *
 * @brief   This is the source file for wwahEpCfg
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
zcl_wwahAttr_t g_zcl_wwahAttrs = {
    .disableOTADowngrades               = FALSE,
    .mgmtLeaveWithoutRejoinEnabled      = TRUE,
    .nwkRetryCount                      = 3,
    .macRetryCount                      = 3,
    .routerCheckInEnabled               = FALSE,
    .touchlinkInterpanEnabled           = TRUE,
    .parentClassificationEnabled        = FALSE,
    .configurationModeEnabled           = TRUE,
    .currentDebugReportID               = 0,
    .tcSecurityOnNwkKeyRotationEnabled  = FALSE,
    .pendingNwkUpdateChannel            = 0xFF,
    .pendingNwkUpdatePanID              = 0xFFFF,
    .otaMaxOfflineDuration              = 0,
};

/* Attribute record list */
const zclAttrInfo_t wwah_attrTbl[] = {
    { ZCL_ATTRID_WWAH_DISABLE_OTA_DOWNGRADES,                  ZCL_DATA_TYPE_BOOLEAN, ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.disableOTADowngrades },
    { ZCL_ATTRID_WWAH_MGMT_LEAVE_WITHOUT_REJOIN_ENABLED,       ZCL_DATA_TYPE_BOOLEAN, ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.mgmtLeaveWithoutRejoinEnabled },
    { ZCL_ATTRID_WWAH_NWK_RETRY_COUNT,                         ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.nwkRetryCount },
    { ZCL_ATTRID_WWAH_MAC_RETRY_COUNT,                         ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.macRetryCount },
    { ZCL_ATTRID_WWAH_ROUTER_CHECK_IN_ENABLED,                 ZCL_DATA_TYPE_BOOLEAN, ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.routerCheckInEnabled },
    { ZCL_ATTRID_WWAH_TOUCHLINK_INTERPAN_ENABLED,              ZCL_DATA_TYPE_BOOLEAN, ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.touchlinkInterpanEnabled },
    { ZCL_ATTRID_WWAH_PARENT_CLASSIFICATION_ENABLED,           ZCL_DATA_TYPE_BOOLEAN, ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.parentClassificationEnabled },
    { ZCL_ATTRID_WWAH_CONFIGURATION_MODE_ENABLED,              ZCL_DATA_TYPE_BOOLEAN, ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.configurationModeEnabled },
    { ZCL_ATTRID_WWAH_CURRENT_DEBUG_REPORT_ID,                 ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.currentDebugReportID },
    { ZCL_ATTRID_WWAH_TC_SECURITY_ON_NWK_KEY_ROTATION_ENABLED, ZCL_DATA_TYPE_BOOLEAN, ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.tcSecurityOnNwkKeyRotationEnabled },
    { ZCL_ATTRID_WWAH_PENDING_NWK_UPDATE_CHANNEL,              ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.pendingNwkUpdateChannel },
    { ZCL_ATTRID_WWAH_PENDING_NWK_UPDATE_PANID,                ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.pendingNwkUpdatePanID },
    { ZCL_ATTRID_WWAH_OTA_MAX_OFFLINE_DURATION,                ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ, (u8*)&g_zcl_wwahAttrs.otaMaxOfflineDuration },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,                      ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ, (u8*)&zcl_attr_global_clusterRevision },
};

#define ZCL_WWAH_ATTR_NUM       sizeof(wwah_attrTbl) / sizeof(zclAttrInfo_t)


/**
 *  @brief Definition for WWAH ZCL specific cluster
 */
const zcl_specClusterInfo_t g_wwahClusterList[] = {
    {ZCL_CLUSTER_WWAH, MANUFACTURER_CODE_AMAZON, ZCL_WWAH_ATTR_NUM, wwah_attrTbl, zcl_wwah_register, zcl_wwahCb},
};

u8 WWAH_CB_CLUSTER_NUM = (sizeof(g_wwahClusterList) / sizeof(g_wwahClusterList[0]));

#endif /* ZCL_WWAH */
