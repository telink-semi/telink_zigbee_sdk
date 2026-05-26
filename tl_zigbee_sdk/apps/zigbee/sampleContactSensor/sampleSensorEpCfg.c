/********************************************************************************************************
 * @file    sampleSensorEpCfg.c
 *
 * @brief   This is the source file for sampleSensorEpCfg
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
#if (__PROJECT_TL_CONTACT_SENSOR__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zcl_include.h"
#include "sampleSensor.h"

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
const u16 sampleSensor_inClusterList[] = {
    ZCL_CLUSTER_GEN_BASIC,
#ifdef ZCL_POWER_CFG
    ZCL_CLUSTER_GEN_POWER_CFG,
#endif
    ZCL_CLUSTER_GEN_IDENTIFY,
#ifdef ZCL_POLL_CTRL
    ZCL_CLUSTER_GEN_POLL_CONTROL,
#endif
#ifdef ZCL_IAS_ZONE
    ZCL_CLUSTER_SS_IAS_ZONE,
#endif
#ifdef ZCL_DIAGNOSTICS
    ZCL_CLUSTER_GEN_DIAGNOSTICS,
#endif
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const u16 sampleSensor_outClusterList[] = {
#ifdef ZCL_GROUP
    ZCL_CLUSTER_GEN_GROUPS,
#endif
#ifdef ZCL_ON_OFF
    ZCL_CLUSTER_GEN_ON_OFF,
#endif
#ifdef ZCL_OTA
    ZCL_CLUSTER_OTA,
#endif
#ifdef ZCL_ZLL_COMMISSIONING
    ZCL_CLUSTER_TOUCHLINK_COMMISSIONING,
#endif
};

/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
#define SAMPLESENSOR_IN_CLUSTER_NUM     (sizeof(sampleSensor_inClusterList) / sizeof(sampleSensor_inClusterList[0]))
#define SAMPLESENSOR_OUT_CLUSTER_NUM    (sizeof(sampleSensor_outClusterList) / sizeof(sampleSensor_outClusterList[0]))

/**
 *  @brief Definition for simple description for HA profile
 */
const af_simple_descriptor_t sampleSensor_simpleDesc = {
    HA_PROFILE_ID,                      /* Application profile identifier */
    HA_DEV_IAS_ZONE,                    /* Application device identifier */
    SAMPLE_SENSOR_ENDPOINT,             /* Endpoint */
    1,                                  /* Application device version */
    0,                                  /* Reserved */
    SAMPLESENSOR_IN_CLUSTER_NUM,        /* Application input cluster count */
    SAMPLESENSOR_OUT_CLUSTER_NUM,       /* Application output cluster count */
    (u16 *)sampleSensor_inClusterList,  /* Application input cluster list */
    (u16 *)sampleSensor_outClusterList, /* Application output cluster list */
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

#ifdef ZCL_IAS_ZONE
/* IAS Zone */
zcl_iasZoneAttr_t g_zcl_iasZoneAttrs = {
    .zoneState  = ZONE_STATE_NOT_ENROLLED,
    .zoneType   = ZONE_TYPE_CONTACT_SWITCH,
    .zoneStatus = 0x00,
    .iasCieAddr = {0x00},
    .zoneId     = ZCL_ZONE_ID_INVALID,
};

const zclAttrInfo_t iasZone_attrTbl[] = {
    { ZCL_ATTRID_ZONE_STATE,              ZCL_DATA_TYPE_ENUM8,     ACCESS_CONTROL_READ,                        (u8*)&g_zcl_iasZoneAttrs.zoneState },
    { ZCL_ATTRID_ZONE_TYPE,               ZCL_DATA_TYPE_ENUM16,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_iasZoneAttrs.zoneType },
    { ZCL_ATTRID_ZONE_STATUS,             ZCL_DATA_TYPE_BITMAP16,  ACCESS_CONTROL_READ,                        (u8*)&g_zcl_iasZoneAttrs.zoneStatus },
    { ZCL_ATTRID_IAS_CIE_ADDR,            ZCL_DATA_TYPE_IEEE_ADDR, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)g_zcl_iasZoneAttrs.iasCieAddr },
    { ZCL_ATTRID_ZONE_ID,                 ZCL_DATA_TYPE_UINT8,     ACCESS_CONTROL_READ,                        (u8*)&g_zcl_iasZoneAttrs.zoneId},

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,    ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_global_clusterRevision},
};

#define	ZCL_IASZONE_ATTR_NUM    sizeof(iasZone_attrTbl) / sizeof(zclAttrInfo_t)
#endif

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

#define	ZCL_POLLCTRL_ATTR_NUM   sizeof(pollCtrl_attrTbl) / sizeof(zclAttrInfo_t)
#endif

#ifdef ZCL_DIAGNOSTICS
/* Diagnostics */

const zclAttrInfo_t diagnostics_attrTbl[] = {
    { ZCL_DIAGNOSTICS_ATTRID_LAST_MESSAGE_LQI,  ZCL_DATA_TYPE_UINT8,  ACCESS_CONTROL_READ, (u8*)&g_sysDiags.lastMessageLQI },
    { ZCL_DIAGNOSTICS_ATTRID_LAST_MESSAGE_RSSI, ZCL_DATA_TYPE_INT8,   ACCESS_CONTROL_READ, (u8*)&g_sysDiags.lastMessageRSSI },
    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,       ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8*)&zcl_attr_global_clusterRevision},
};

#define ZCL_DIAGNOSTICS_ATTR_NUM   sizeof(diagnostics_attrTbl) / sizeof(zclAttrInfo_t)
#endif

/**
 *  @brief Definition for simple contact sensor ZCL specific cluster
 */
const zcl_specClusterInfo_t g_sampleSensorClusterList[] = {
    {ZCL_CLUSTER_GEN_BASIC,        MANUFACTURER_CODE_NONE, ZCL_BASIC_ATTR_NUM,       basic_attrTbl,       zcl_basic_register,       sampleSensor_basicCb},
#ifdef ZCL_POWER_CFG
    {ZCL_CLUSTER_GEN_POWER_CFG,    MANUFACTURER_CODE_NONE, ZCL_POWERCFG_ATTR_NUM,    powerCfg_attrTbl,    zcl_powerCfg_register,    NULL},
#endif
    {ZCL_CLUSTER_GEN_IDENTIFY,     MANUFACTURER_CODE_NONE, ZCL_IDENTIFY_ATTR_NUM,    identify_attrTbl,    zcl_identify_register,    sampleSensor_identifyCb},
#ifdef ZCL_GROUP
    {ZCL_CLUSTER_GEN_GROUPS,       MANUFACTURER_CODE_NONE, 0,                        NULL,                zcl_group_register,       sampleSensor_groupCb},
#endif
#ifdef ZCL_IAS_ZONE
    {ZCL_CLUSTER_SS_IAS_ZONE,      MANUFACTURER_CODE_NONE, ZCL_IASZONE_ATTR_NUM,     iasZone_attrTbl,     zcl_iasZone_register,     sampleSensor_iasZoneCb},
#endif
#ifdef ZCL_POLL_CTRL
    {ZCL_CLUSTER_GEN_POLL_CONTROL, MANUFACTURER_CODE_NONE, ZCL_POLLCTRL_ATTR_NUM,    pollCtrl_attrTbl,    zcl_pollCtrl_register,    sampleSensor_pollCtrlCb},
#endif
#ifdef ZCL_DIAGNOSTICS
    {ZCL_CLUSTER_GEN_DIAGNOSTICS,  MANUFACTURER_CODE_NONE, ZCL_DIAGNOSTICS_ATTR_NUM, diagnostics_attrTbl, zcl_diagnostics_register, NULL},
#endif
};

u8 SAMPLE_SENSOR_CB_CLUSTER_NUM = (sizeof(g_sampleSensorClusterList) / sizeof(g_sampleSensorClusterList[0]));

/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      zcl_iasZoneAttr_save
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_iasZoneAttr_save(void)
{
    nv_sts_t st = NV_SUCC;

#ifdef ZCL_IAS_ZONE
#if NV_ENABLE
    bool needSave = FALSE;
    zcl_nv_iasZone_t zcl_nv_iasZone;

    st = nv_flashReadNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_IAS_ZONE, sizeof(zcl_nv_iasZone_t), (u8 *)&zcl_nv_iasZone);
    if (st == NV_SUCC) {
        if (!ZB_IEEE_ADDR_CMP(zcl_nv_iasZone.iasCieAddr, g_zcl_iasZoneAttrs.iasCieAddr) ||
           (zcl_nv_iasZone.zoneId != g_zcl_iasZoneAttrs.zoneId) ||
           (zcl_nv_iasZone.zoneState != g_zcl_iasZoneAttrs.zoneState)) {
            needSave = TRUE;
        }
    } else if(st == NV_ITEM_NOT_FOUND) {
        needSave = TRUE;
    }

    if (needSave) {
        ZB_IEEE_ADDR_COPY(zcl_nv_iasZone.iasCieAddr, g_zcl_iasZoneAttrs.iasCieAddr);
        zcl_nv_iasZone.zoneId = g_zcl_iasZoneAttrs.zoneId;
        zcl_nv_iasZone.zoneState = g_zcl_iasZoneAttrs.zoneState;

        st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_IAS_ZONE, sizeof(zcl_nv_iasZone_t), (u8 *)&zcl_nv_iasZone);
    }
#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif
#endif

    return st;
}

/*********************************************************************
 * @fn      zcl_iasZoneAttr_restore
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_iasZoneAttr_restore(void)
{
    nv_sts_t st = NV_SUCC;

#ifdef ZCL_IAS_ZONE
#if NV_ENABLE
    zcl_nv_iasZone_t zcl_nv_iasZone;

    st = nv_flashReadNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_IAS_ZONE, sizeof(zcl_nv_iasZone_t), (u8 *)&zcl_nv_iasZone);
    if (st == NV_SUCC) {
        ZB_IEEE_ADDR_COPY(g_zcl_iasZoneAttrs.iasCieAddr, zcl_nv_iasZone.iasCieAddr);
        g_zcl_iasZoneAttrs.zoneId = zcl_nv_iasZone.zoneId;
        g_zcl_iasZoneAttrs.zoneState = zcl_nv_iasZone.zoneState;
    }
#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif
#endif

    return st;
}

/*********************************************************************
 * @fn      zcl_sampleSensorAttrsInit
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
void zcl_sampleSensorAttrsInit(void)
{
    zcl_iasZoneAttr_restore();
}
#endif	/* __PROJECT_TL_CONTACT_SENSOR__ */
