/********************************************************************************************************
 * @file     zcl_basic_attr.c
 *
 * @brief	 attribute configuration for basic cluster
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
#ifndef ZCL_BASIC_MFG_NAME
#define ZCL_BASIC_MFG_NAME     {6,'T','E','L','I','N','K'}
#endif
#ifndef ZCL_BASIC_MODEL_ID
#define ZCL_BASIC_MODEL_ID	   {8,'T','L','S','R','8','2','6','x'}
#endif
#ifndef ZCL_BASIC_DATE_CODE
#define ZCL_BASIC_DATE_CODE    {8,'2','0','1','7','0','8','1','4'}
#endif
#ifndef ZCL_BASIC_LOC_DESC
#define ZCL_BASIC_LOC_DESC     {7,'U','N','K','N','O','W','N'}
#endif
#ifndef ZCL_BASIC_BUILD_ID
#define ZCL_BASIC_BUILD_ID     {10,'0','1','2','2','0','5','2','0','1','7'}
#endif

/**********************************************************************
 * LOCAL TYPES
 */

/**********************************************************************
 * LOCAL VARIABLES
 */
#ifdef ZCL_BASIC
const u8 zcl_attr_zclVer = 3;
const u8 zcl_attr_appVer = 0;
const u8 zcl_attr_stackVer = 2;
const u8 zcl_attr_hwVer = 0;
const u8 zcl_attr_mfName[ZCL_BASIC_MAX_LENGTH] = ZCL_BASIC_MFG_NAME;
const u8 zcl_attr_dateCode[ZCL_BASIC_MAX_LENGTH] = ZCL_BASIC_DATE_CODE;
const u8 zcl_attr_swBuildId[ZCL_BASIC_MAX_LENGTH] = ZCL_BASIC_BUILD_ID;
const u8 zcl_attr_powerSrc = POWER_SOURCE_MAINS_1_PHASE;
u8 zcl_attr_modelID[ZCL_BASIC_MAX_LENGTH] = ZCL_BASIC_MODEL_ID;
u8 zcl_attr_locDesc[ZCL_BASIC_MAX_LENGTH] = ZCL_BASIC_LOC_DESC;
u8 zcl_attr_phyEnv = 0;
u8 zcl_attr_devEnabled = 1;
u8 zcl_attr_alarmMask = 0;
u8 zcl_attr_localCfg = 0;


/* Attribute record list */
const zclAttrInfo_t basic_attrTbl[] =
{
#ifdef ZCL_ATTR_VERSION_ENABLE
	{ ZCL_ATTRID_BASIC_ZCL_VER,      ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,  (u8*)&zcl_attr_zclVer},
#endif
#ifdef ZCL_ATTR_APPLICATION_VERSION_ENABLE
	{ ZCL_ATTRID_BASIC_APP_VER,      ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,  (u8*)&zcl_attr_appVer},
#endif
#ifdef ZCL_ATTR_STACK_VERSION_ENABLE
	{ ZCL_ATTRID_BASIC_STACK_VER,    ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,  (u8*)&zcl_attr_stackVer},
#endif
#ifdef ZCL_ATTR_HW_VERSION_ENABLE
	{ ZCL_ATTRID_BASIC_HW_VER,       ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,  (u8*)&zcl_attr_hwVer},
#endif
#ifdef ZCL_ATTR_MANUFACTURER_NAME_ENABLE
	{ ZCL_ATTRID_BASIC_MFR_NAME,     ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,  (u8*)&zcl_attr_mfName},
#endif
#ifdef ZCL_ATTR_MODEL_IDENTIFIER_ENABLE
	{ ZCL_ATTRID_BASIC_MODEL_ID,     ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,  (u8*)&zcl_attr_modelID},
#endif
#ifdef ZCL_ATTR_DATE_CODE_ENABLE
	{ ZCL_ATTRID_BASIC_DATE_CODE,    ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,  (u8*)&zcl_attr_dateCode},
#endif
#ifdef ZCL_ATTR_POWER_SOURCE_ENABLE
	{ ZCL_ATTRID_BASIC_POWER_SOURCE, ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ,  (u8*)&zcl_attr_powerSrc},
#endif
#ifdef ZCL_ATTR_LOC_DESC_ENABLE
	{ ZCL_ATTRID_BASIC_LOC_DESC,     ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_locDesc},
#endif
#ifdef ZCL_ATTR_PHY_ENV_ENABLE
	{ ZCL_ATTRID_BASIC_PHY_ENV,      ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_phyEnv},
#endif
#ifdef ZCL_ATTR_DEV_ENABLED_ENABLE
	{ ZCL_ATTRID_BASIC_DEV_ENABLED,  ZCL_DATA_TYPE_BOOLEAN,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_devEnabled},
#endif
#ifdef ZCL_ATTR_ALARM_MASK_ENABLE
	{ ZCL_ATTRID_BASIC_ALARM_MASK,   ZCL_DATA_TYPE_BITMAP8,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_alarmMask},
#endif
#ifdef ZCL_ATTR_DISABLE_LOCAL_CFG_ENABLE
	{ ZCL_ATTRID_BASIC_DISABLE_LOCAL_CFG, ZCL_DATA_TYPE_BITMAP8, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_localCfg},
#endif
#ifdef ZCL_ATTR_SW_BUILD_ID
	{ ZCL_ATTRID_BASIC_SW_BUILD_ID,  ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,  (u8*)&zcl_attr_swBuildId},
#endif
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_basic_attrNum = ( sizeof(basic_attrTbl) / sizeof(zclAttrInfo_t) );

#endif    /* ZCL_BASIC */

#endif
