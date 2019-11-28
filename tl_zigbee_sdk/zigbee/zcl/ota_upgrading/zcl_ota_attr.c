/********************************************************************************************************
 * @file     zcl_ota_attr.c
 *
 * @brief	 attribute configuration for ota cluster
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


/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "../zcl_include.h"

#if 0
/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * LOCAL TYPES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */
#ifdef ZCL_OTA

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
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_ota_attrNum = ( sizeof(ota_attrTbl) / sizeof(zclAttrInfo_t) );

#endif    /* ZCL_OTA */
#endif
