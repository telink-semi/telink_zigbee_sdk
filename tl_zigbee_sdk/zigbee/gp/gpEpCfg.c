/********************************************************************************************************
 * @file    gpEpCfg.c
 *
 * @brief   This is the source file for gpEpCfg
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
#include "gp.h"
#include "gp_proxy.h"
#if GP_BASIC_COMBO
#include "gp_sink.h"
#endif

#if GP_SUPPORT_ENABLE
/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * GLOBAL VARIABLES
 */
/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const u16 gp_inClusterList[] = {
#if GP_BASIC_COMBO
    ZCL_CLUSTER_GEN_GREEN_POWER
#endif
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const u16 gp_outClusterList[] = {
    ZCL_CLUSTER_GEN_GREEN_POWER
};

/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
#if GP_BASIC_COMBO
    #define GP_IN_CLUSTER_NUM           1
    #define GP_OUT_CLUSTER_NUM          1
#else
    #define GP_IN_CLUSTER_NUM           0
    #define GP_OUT_CLUSTER_NUM          1
#endif

/**
 *  @brief Definition for simple description for HA profile
 */
const af_simple_descriptor_t gp_simpleDesc = {
    GP_PROFILE_ID,              /* Application profile identifier */
#if GP_BASIC_COMBO
    GP_DEVICE_ID_COMBO_BASIC,   /* Application device identifier */
#else
    GP_DEVICE_ID_PROXY_BASIC,   /* Application device identifier */
#endif
    GREEN_POWER_ENDPOINT,       /* Endpoint */
    0,                          /* Application device version */
    0,                          /* Reserved */
    GP_IN_CLUSTER_NUM,          /* Application input cluster count */
    GP_OUT_CLUSTER_NUM,         /* Application output cluster count */
    (u16 *)gp_inClusterList,    /* Application input cluster list */
    (u16 *)gp_outClusterList,   /* Application output cluster list */
};

/* Attribute default */
#if GP_BASIC_COMBO
u8 zclGpAttr_gpsMaxSinkTabEntries = GPS_MAX_SINK_TABLE_ENTRIES;
u8 zclGpAttr_sinkTabEntry[ZCL_GP_MAX_SINK_TABLE_ATTR_LEN + 2] = {0x0, 0x0};
u8 zclGpAttr_gpsCommMode = GPS_COMM_MODE_GROUP_DGROUPID;
u8 zclGpAttr_gpsCommExitMode = GPS_EXIT_ON_FIRST_PAIRING_SUCCESS;
u16 zclGpAttr_gpsCommWindow = GPP_COMMISSIONING_WINDOW_DEFAULT;
#if ZB_COORDINATOR_ROLE
u8 zclGpAttr_gpsSecLevel = GPS_MINIMAL_GPD_SEC_LEVEL_4B_FC_MIC | GPS_PROTECTION_WITH_GPLINKKEY;
#elif ZB_ROUTER_ROLE
u8 zclGpAttr_gpsSecLevel = GPS_MINIMAL_GPD_SEC_LEVEL_4B_FC_MIC | GPS_PROTECTION_WITH_GPLINKKEY | GPS_INVOLVE_TC;
#endif
u8 zclGpAttr_gpsFunc[3] = {0};
u8 zclGpAttr_gpsActiveFunc[3] = {0};
#endif
u8 zclGpAttr_gppMaxProxyTabEntries = GPP_MAX_PROXY_TABLE_ENTRIES;
u16 zclGpAttr_clusterRev = GP_CLUSTER_REVISION;
u8 zclGpAttr_proxyTabEntry[ZCL_GP_MAX_PROXY_TABLE_ATTR_LEN + 2] = {0x0, 0x0,};
u8 zclGpAttr_gppFunc[3] = {0};
u8 zclGpAttr_gppActiveFunc[3] = {0};
u8 zclGpAttr_gpSharedSecKeyType = GP_SEC_KEY_TYPE_NO_KEY;
u8 zclGpAttr_gpSharedSecKey[SEC_KEY_LEN] = {0};
u8 zclGpAttr_gpLinkKey[SEC_KEY_LEN] = GP_LINK_KEY;

/* Attribute record list */
const zclAttrInfo_t gp_attrTbl[] = {
#if GP_BASIC_COMBO
    /* Attributes of the GP server cluster */
    { ZCL_ATTRID_GP_GPS_MAX_SINK_TABLE_ENTRIES,  ZCL_DATA_TYPE_UINT8,           ACCESS_CONTROL_READ,                        (u8*)&zclGpAttr_gpsMaxSinkTabEntries },
    { ZCL_ATTRID_GP_SINK_TABLE,                  ZCL_DATA_TYPE_LONG_OCTET_STR,  ACCESS_CONTROL_READ,                        (u8*)zclGpAttr_sinkTabEntry },
    { ZCL_ATTRID_GP_GPS_COMMUNICATION_MODE,      ZCL_DATA_TYPE_BITMAP8,         ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zclGpAttr_gpsCommMode },
    { ZCL_ATTRID_GP_GPS_COMMISSIONING_EXIT_MODE, ZCL_DATA_TYPE_BITMAP8,         ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zclGpAttr_gpsCommExitMode },
    { ZCL_ATTRID_GP_GPS_COMMISSIONING_WINDOW,    ZCL_DATA_TYPE_UINT16,          ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zclGpAttr_gpsCommWindow },
    { ZCL_ATTRID_GP_GPS_SECURITY_LEVEL,          ZCL_DATA_TYPE_BITMAP8,         ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zclGpAttr_gpsSecLevel },
    { ZCL_ATTRID_GP_GPS_FUNCTIONALITY,           ZCL_DATA_TYPE_BITMAP24,        ACCESS_CONTROL_READ,                        (u8*)zclGpAttr_gpsFunc },
    { ZCL_ATTRID_GP_GPS_ACTIVE_FUNCTIONALITY,    ZCL_DATA_TYPE_BITMAP24,        ACCESS_CONTROL_READ,                        (u8*)zclGpAttr_gpsActiveFunc },
#endif
    /* Attributes of the GP client cluster */
    { ZCL_ATTRID_GP_GPP_MAX_PROXY_TABLE_ENTRIES, ZCL_DATA_TYPE_UINT8,           ACCESS_CONTROL_READ,                        (u8*)&zclGpAttr_gppMaxProxyTabEntries },
    { ZCL_ATTRID_GP_PROXY_TABLE,                 ZCL_DATA_TYPE_LONG_OCTET_STR,  ACCESS_CONTROL_READ,                        (u8*)zclGpAttr_proxyTabEntry },
    { ZCL_ATTRID_GP_GPP_FUNCTIONALITY,           ZCL_DATA_TYPE_BITMAP24,        ACCESS_CONTROL_READ,                        (u8*)zclGpAttr_gppFunc },
    { ZCL_ATTRID_GP_GPP_ACTIVE_FUNCTIONALITY,    ZCL_DATA_TYPE_BITMAP24,        ACCESS_CONTROL_READ,                        (u8*)zclGpAttr_gppActiveFunc },
    /* Attributes shared by client and server of GP cluster */
    { ZCL_ATTRID_GP_SHARED_SECURITY_KEY_TYPE,    ZCL_DATA_TYPE_BITMAP8,         ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zclGpAttr_gpSharedSecKeyType },
    { ZCL_ATTRID_GP_SHARED_SECURITY_KEY,         ZCL_DATA_TYPE_128_BIT_SEC_KEY, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)zclGpAttr_gpSharedSecKey },
    { ZCL_ATTRID_GP_LINK_KEY,                    ZCL_DATA_TYPE_128_BIT_SEC_KEY, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)zclGpAttr_gpLinkKey },
    { ZCL_ATTRID_GP_CLUSTER_REVISION,            ZCL_DATA_TYPE_UINT16,          ACCESS_CONTROL_READ,                        (u8*)&zclGpAttr_clusterRev },
};

#define ZCL_GP_ATTR_NUM         sizeof(gp_attrTbl) / sizeof(zclAttrInfo_t)


/**
 *  @brief Definition for Green Power ZCL specific cluster
 */
const zcl_specClusterInfo_t g_gpClusterList[] = {
    {ZCL_CLUSTER_GEN_GREEN_POWER, MANUFACTURER_CODE_NONE, ZCL_GP_ATTR_NUM, gp_attrTbl, zcl_gp_register, zcl_gpCb},
};

u8 GP_CB_CLUSTER_NUM = (sizeof(g_gpClusterList) / sizeof(g_gpClusterList[0]));

/**********************************************************************
 * FUNCTIONS
 */



#endif	/* #if GP_SUPPORT_ENABLE */
