/********************************************************************************************************
 * @file    otaEpCfg.c
 *
 * @brief   This is the source file for otaEpCfg
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
 *******************************************************************************************************/

/**********************************************************************
 * INCLUDES
 */
#include "../zcl/zcl_include.h"
#include "ota.h"

#ifdef ZCL_OTA
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
addrExt_t zcl_attr_upgradeServerID = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
u32 zcl_attr_fileOffset = 0xffffffff;
u32 zcl_attr_currFileVer = 0xffffffff;
u16 zcl_attr_currZigbeeStackVer = 0xffff;
u32 zcl_attr_downloadFileVer = 0xffffffff;
u16 zcl_attr_downloadZigbeeStackVer = 0xffff;
u8 zcl_attr_imageUpgradeStatus = 0;
u16 zcl_attr_manufacturerID = 0;
u16 zcl_attr_imageTypeID = 0xffff;
u16 zcl_attr_minBlockPeriod = 0;//in milliseconds
//u32 zcl_attr_imageStamp = 0;

/* Attribute record list */
const zclAttrInfo_t ota_attrTbl[] =
{
	{ ZCL_ATTRID_OTA_UPGRADE_SERVER_ID,  		ZCL_DATA_TYPE_IEEE_ADDR,ACCESS_CONTROL_READ, (u8*)zcl_attr_upgradeServerID },
	{ ZCL_ATTRID_OTA_FILE_OFFSET, 				ZCL_DATA_TYPE_UINT32, 	ACCESS_CONTROL_READ, (u8*)&zcl_attr_fileOffset },
	{ ZCL_ATTRID_OTA_CURRENT_FILE_VER, 			ZCL_DATA_TYPE_UINT32, 	ACCESS_CONTROL_READ, (u8*)&zcl_attr_currFileVer },
	{ ZCL_ATTRID_OTA_CURRENT_ZIGBEE_STACK_VER, 	ZCL_DATA_TYPE_UINT16, 	ACCESS_CONTROL_READ, (u8*)&zcl_attr_currZigbeeStackVer },
	{ ZCL_ATTRID_OTA_DOWNLOAD_FILE_VER, 		ZCL_DATA_TYPE_UINT32, 	ACCESS_CONTROL_READ, (u8*)&zcl_attr_downloadFileVer },
	{ ZCL_ATTRID_OTA_DOWNLOAD_ZIGBEE_STACK_VER, ZCL_DATA_TYPE_UINT16, 	ACCESS_CONTROL_READ, (u8*)&zcl_attr_downloadZigbeeStackVer },
	{ ZCL_ATTRID_OTA_IMAGE_UPGRADE_STAUS, 		ZCL_DATA_TYPE_ENUM8 , 	ACCESS_CONTROL_READ, (u8*)&zcl_attr_imageUpgradeStatus },
	{ ZCL_ATTRID_OTA_MANUFACTURER_ID, 			ZCL_DATA_TYPE_UINT16, 	ACCESS_CONTROL_READ, (u8*)&zcl_attr_manufacturerID },
	{ ZCL_ATTRID_OTA_IMAGE_TYPE_ID, 			ZCL_DATA_TYPE_UINT16, 	ACCESS_CONTROL_READ, (u8*)&zcl_attr_imageTypeID },
	{ ZCL_ATTRID_OTA_MINIMUM_BLOCK_PERIOD, 		ZCL_DATA_TYPE_UINT16, 	ACCESS_CONTROL_READ, (u8*)&zcl_attr_minBlockPeriod },
//	{ ZCL_ATTRID_OTA_IMAGE_STAMP, 				ZCL_DATA_TYPE_UINT32, 	ACCESS_CONTROL_READ, (u8*)&zcl_attr_imageStamp },
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 		ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ, (u8*)&zcl_attr_global_clusterRevision},
};

#define ZCL_OTA_ATTR_NUM	sizeof(ota_attrTbl) / sizeof(zclAttrInfo_t)


/**
 *  @brief Definition for OTA ZCL specific cluster
 */
const zcl_specClusterInfo_t g_otaClusterList[] =
{
	{ZCL_CLUSTER_OTA, MANUFACTURER_CODE_NONE, ZCL_OTA_ATTR_NUM,	ota_attrTbl, zcl_ota_register, zcl_otaCb},
};

u8 OTA_CB_CLUSTER_NUM = (sizeof(g_otaClusterList)/sizeof(g_otaClusterList[0]));

/**********************************************************************
 * FUNCTIONS
 */

#endif	/* ZCL_OTA */
