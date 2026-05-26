/********************************************************************************************************
 * @file    sampleSwitchEpCfg.c
 *
 * @brief   This is the source file for sampleSwitchEpCfg
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
#if (__PROJECT_TL_SWITCH__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zcl_include.h"
#include "sampleSwitch.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */
#ifndef ZCL_BASIC_MFG_NAME
#define ZCL_BASIC_MFG_NAME    {6,'T','E','L','I','N','K'}
#endif
#ifndef ZCL_BASIC_MODEL_ID
#define ZCL_BASIC_MODEL_ID    {8,'T','L','S','R','x','x','x','x'}
#endif


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * GLOBAL VARIABLES
 */
/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const u16 sampleSwitch_inClusterList[] = {
    ZCL_CLUSTER_GEN_BASIC,
#ifdef ZCL_POWER_CFG
    ZCL_CLUSTER_GEN_POWER_CFG,
#endif
    ZCL_CLUSTER_GEN_IDENTIFY,
#ifdef ZCL_POLL_CTRL
    ZCL_CLUSTER_GEN_POLL_CONTROL,
#endif
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const u16 sampleSwitch_outClusterList[] = {
#ifdef ZCL_GROUP
    ZCL_CLUSTER_GEN_GROUPS,
#endif
#ifdef ZCL_SCENE
    ZCL_CLUSTER_GEN_SCENES,
#endif
#ifdef ZCL_ON_OFF
    ZCL_CLUSTER_GEN_ON_OFF,
#endif
#ifdef ZCL_LEVEL_CTRL
    ZCL_CLUSTER_GEN_LEVEL_CONTROL,
#endif
#ifdef ZCL_OTA
    ZCL_CLUSTER_OTA,
#endif
#ifdef ZCL_ZLL_COMMISSIONING
    ZCL_CLUSTER_TOUCHLINK_COMMISSIONING,
#endif
#ifdef ZCL_ZBD_CONFIG
    ZCL_CLUSTER_GEN_ZBD_CONFIG
#endif
};

/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
#define SAMPLESWITCH_IN_CLUSTER_NUM     (sizeof(sampleSwitch_inClusterList) / sizeof(sampleSwitch_inClusterList[0]))
#define SAMPLESWITCH_OUT_CLUSTER_NUM    (sizeof(sampleSwitch_outClusterList) / sizeof(sampleSwitch_outClusterList[0]))

/**
 *  @brief Definition for simple description for HA profile
 */
const af_simple_descriptor_t sampleSwitch_simpleDesc = {
    HA_PROFILE_ID,                      /* Application profile identifier */
    HA_DEV_LEVEL_CTRL_SWITCH,           /* Application device identifier */
    SAMPLE_SWITCH_ENDPOINT,             /* Endpoint */
    1,                                  /* Application device version */
    0,                                  /* Reserved */
    SAMPLESWITCH_IN_CLUSTER_NUM,        /* Application input cluster count */
    SAMPLESWITCH_OUT_CLUSTER_NUM,       /* Application output cluster count */
    (u16 *)sampleSwitch_inClusterList,  /* Application input cluster list */
    (u16 *)sampleSwitch_outClusterList, /* Application output cluster list */
};

/* Basic */
zcl_basicAttr_t g_zcl_basicAttrs = {
    .zclVersion   = 0x03,
    .appVersion   = 0x00,
    .stackVersion = 0x02,
    .hwVersion    = 0x00,
    .manuName     = ZCL_BASIC_MFG_NAME,
    .modelId      = ZCL_BASIC_MODEL_ID,
    .powerSource  = POWER_SOURCE_MAINS_1_PHASE,
    .deviceEnable = TRUE,
};

const zclAttrInfo_t basic_attrTbl[] = {
    { ZCL_ATTRID_BASIC_ZCL_VER,           ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_basicAttrs.zclVersion},
    { ZCL_ATTRID_BASIC_APP_VER,           ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_basicAttrs.appVersion},
    { ZCL_ATTRID_BASIC_STACK_VER,         ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_basicAttrs.stackVersion},
    { ZCL_ATTRID_BASIC_HW_VER,            ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_basicAttrs.hwVersion},
    { ZCL_ATTRID_BASIC_MFR_NAME,          ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,                        (u8*)g_zcl_basicAttrs.manuName},
    { ZCL_ATTRID_BASIC_MODEL_ID,          ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,                        (u8*)g_zcl_basicAttrs.modelId},
    { ZCL_ATTRID_BASIC_POWER_SOURCE,      ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_basicAttrs.powerSource},
//  { ZCL_ATTRID_BASIC_DEV_ENABLED,       ZCL_DATA_TYPE_BOOLEAN,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_zcl_basicAttrs.deviceEnable},

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_global_clusterRevision},
};

#define ZCL_BASIC_ATTR_NUM      sizeof(basic_attrTbl) / sizeof(zclAttrInfo_t)

#ifdef ZCL_POWER_CFG
/* Power configuration */
zcl_powerCfgAttr_t g_zcl_powerCfgAttrs = {
    .batteryVoltage             = 0xFF,
    .batteryPercentRemaining    = 0xFF,
};

const zclAttrInfo_t powerCfg_attrTbl[] = {
    { ZCL_ATTRID_BATTERY_VOLTAGE,              ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ,                             (u8*)&g_zcl_powerCfgAttrs.batteryVoltage },
    { ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING, ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&g_zcl_powerCfgAttrs.batteryPercentRemaining },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,      ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ,                            (u8*)&zcl_attr_global_clusterRevision },
};

#define ZCL_POWERCFG_ATTR_NUM   sizeof(powerCfg_attrTbl) / sizeof(zclAttrInfo_t)
#endif

/* Identify */
zcl_identifyAttr_t g_zcl_identifyAttrs = {
    .identifyTime = 0x0000,
};

const zclAttrInfo_t identify_attrTbl[] = {
    { ZCL_ATTRID_IDENTIFY_TIME,           ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_zcl_identifyAttrs.identifyTime },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_global_clusterRevision},
};

#define ZCL_IDENTIFY_ATTR_NUM   sizeof(identify_attrTbl) / sizeof(zclAttrInfo_t)

#ifdef ZCL_POLL_CTRL
/* Poll Control */
zcl_pollCtrlAttr_t g_zcl_pollCtrlAttrs = {
    .chkInInterval       = 0x3840,
    .longPollInterval    = 0x14,
    .shortPollInterval   = 0x02,
    .fastPollTimeout     = 0x28,
    .chkInIntervalMin    = 0x00,
    .longPollIntervalMin = 0x00,
    .fastPollTimeoutMax  = 0x00,
};

const zclAttrInfo_t pollCtrl_attrTbl[] = {
    { ZCL_ATTRID_CHK_IN_INTERVAL,         ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_zcl_pollCtrlAttrs.chkInInterval },
    { ZCL_ATTRID_LONG_POLL_INTERVAL,      ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ,                        (u8*)&g_zcl_pollCtrlAttrs.longPollInterval },
    { ZCL_ATTRID_SHORT_POLL_INTERVAL,     ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ,                        (u8*)&g_zcl_pollCtrlAttrs.shortPollInterval },
    { ZCL_ATTRID_FAST_POLL_TIMEOUT,       ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_zcl_pollCtrlAttrs.fastPollTimeout },
//  { ZCL_ATTRID_CHK_IN_INTERVAL_MIN,     ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ,                        (u8*)&g_zcl_pollCtrlAttrs.chkInIntervalMin},
//  { ZCL_ATTRID_LONG_POLL_INTERVAL_MIN,  ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ,                        (u8*)&g_zcl_pollCtrlAttrs.longPollIntervalMin },
//  { ZCL_ATTRID_FAST_POLL_TIMEOUT_MAX,   ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ,                        (u8*)&g_zcl_pollCtrlAttrs.fastPollTimeoutMax},

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_global_clusterRevision},
};

#define ZCL_POLLCTRL_ATTR_NUM   sizeof(pollCtrl_attrTbl) / sizeof(zclAttrInfo_t)
#endif

#ifdef ZCL_ZBD_CONFIG
zcl_zbdAttr_t g_zcl_zbdAttrs = {
    .interfaceState = 0x01, //0x00 to 0xFF
    .anJoinTimeout = 3600,  //0x000000 to 0x100000
};
const zclAttrInfo_t zbd_attrTbl[] = {
    { ZCL_ATTRID_ZBD_INTERFACE_STATE,        ZCL_DATA_TYPE_BITMAP8, ACCESS_CONTROL_READ, (u8*)&g_zcl_zbdAttrs.interfaceState},
    { ZCL_ATTRID_ZBD_ANONYMOUS_JOIN_TIMEOUT, ZCL_DATA_TYPE_UINT24,  ACCESS_CONTROL_READ, (u8*)&g_zcl_zbdAttrs.anJoinTimeout},
    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,    ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ, (u8*)&zcl_attr_global_clusterRevision},
};

#define ZCL_ZDD_ATTR_NUM      sizeof(zbd_attrTbl) / sizeof(zclAttrInfo_t)
#endif
/**
 *  @brief Definition for simple switch ZCL specific cluster
 */
const zcl_specClusterInfo_t g_sampleSwitchClusterList[] = {
    {ZCL_CLUSTER_GEN_BASIC,        MANUFACTURER_CODE_NONE, ZCL_BASIC_ATTR_NUM,    basic_attrTbl,    zcl_basic_register,    sampleSwitch_basicCb},
#ifdef ZCL_POWER_CFG
    {ZCL_CLUSTER_GEN_POWER_CFG,    MANUFACTURER_CODE_NONE, ZCL_POWERCFG_ATTR_NUM, powerCfg_attrTbl, zcl_powerCfg_register, NULL},
#endif
    {ZCL_CLUSTER_GEN_IDENTIFY,     MANUFACTURER_CODE_NONE, ZCL_IDENTIFY_ATTR_NUM, identify_attrTbl, zcl_identify_register, sampleSwitch_identifyCb},
#ifdef ZCL_GROUP
    {ZCL_CLUSTER_GEN_GROUPS,       MANUFACTURER_CODE_NONE, 0,                     NULL,             zcl_group_register,    sampleSwitch_groupCb},
#endif
#ifdef ZCL_SCENE
    {ZCL_CLUSTER_GEN_SCENES,       MANUFACTURER_CODE_NONE, 0,                     NULL,             zcl_scene_register,    sampleSwitch_sceneCb},
#endif
#ifdef ZCL_POLL_CTRL
    {ZCL_CLUSTER_GEN_POLL_CONTROL, MANUFACTURER_CODE_NONE, ZCL_POLLCTRL_ATTR_NUM, pollCtrl_attrTbl, zcl_pollCtrl_register, sampleSwitch_pollCtrlCb},
#endif
#ifdef ZCL_ZBD_CONFIG
    {ZCL_CLUSTER_GEN_ZBD_CONFIG,   MANUFACTURER_CODE_NONE, ZCL_ZDD_ATTR_NUM,      zbd_attrTbl,      zcl_zbd_register,      sampleSwitch_zbdCb},
#endif
};

u8 SAMPLE_SWITCH_CB_CLUSTER_NUM = (sizeof(g_sampleSwitchClusterList) / sizeof(g_sampleSwitchClusterList[0]));

/**********************************************************************
 * FUNCTIONS
 */

#ifdef ZCL_ZBD_CONFIG
nv_sts_t zcl_zbdInterfaceAttr_save(void)
{
    nv_sts_t st = NV_SUCC;

#if NV_ENABLE
    u8 readInterState = 0;

    st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_ZBD_INF_STATE_INFO, sizeof(readInterState), (u8*)&readInterState);

    if(st == NV_SUCC){
        if(readInterState != g_zcl_zbdAttrs.interfaceState){
            readInterState = g_zcl_zbdAttrs.interfaceState;
            st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_ZBD_INF_STATE_INFO, sizeof(readInterState), (u8*)&readInterState);
        }
    }else if(st == NV_ITEM_NOT_FOUND){
        readInterState = g_zcl_zbdAttrs.interfaceState;
        st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_ZBD_INF_STATE_INFO, sizeof(readInterState), (u8*)&readInterState);
    }
#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}

nv_sts_t zcl_zbdInterfaceAttr_restore(void)
{
    nv_sts_t st = NV_SUCC;

#if NV_ENABLE
    u8 zcl_nv_zbdState = 0;;

    st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_ZBD_INF_STATE_INFO, sizeof(zcl_nv_zbdState), (u8*)&zcl_nv_zbdState);

    if(st == NV_SUCC){
        g_zcl_zbdAttrs.interfaceState = zcl_nv_zbdState;
    }

#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}

nv_sts_t zcl_zbdAnjoinTimeoutAttr_save(void)
{
    nv_sts_t st = NV_SUCC;

#if NV_ENABLE
    u32 anJoinTimeout = 0;

    st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_ZBD_ANJOIN_TIMEOUT_INFO, sizeof(anJoinTimeout), (u8*)&anJoinTimeout);

    if(st == NV_SUCC){
        if(anJoinTimeout != g_zcl_zbdAttrs.anJoinTimeout){
            anJoinTimeout = g_zcl_zbdAttrs.anJoinTimeout;
            st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_ZBD_ANJOIN_TIMEOUT_INFO, sizeof(anJoinTimeout), (u8*)&anJoinTimeout);
        }
    }else if(st == NV_ITEM_NOT_FOUND){
        anJoinTimeout = g_zcl_zbdAttrs.anJoinTimeout;
        st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_ZBD_ANJOIN_TIMEOUT_INFO, sizeof(anJoinTimeout), (u8*)&anJoinTimeout);
    }

#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}

nv_sts_t zcl_zbdAnjoinTimeoutAttr_restore(void)
{
    nv_sts_t st = NV_SUCC;

#if NV_ENABLE
    u32 anJoinTimeout = 0;

    st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_ZBD_ANJOIN_TIMEOUT_INFO, sizeof(anJoinTimeout), (u8*)&anJoinTimeout);

    if(st == NV_SUCC){
        g_zcl_zbdAttrs.anJoinTimeout = anJoinTimeout;
    }

#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}
#endif

/*********************************************************************
 * @fn      zcl_sampleSwitchAttrsInit
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
void zcl_sampleSwitchAttrsInit(void)
{
#ifdef ZCL_ZBD_CONFIG
    zcl_zbdInterfaceAttr_restore();
    zcl_zbdAnjoinTimeoutAttr_restore();
#endif
}

#endif  /* __PROJECT_TL_SWITCH__ */
