/********************************************************************************************************
 * @file	zcl_ota_attr.c
 *
 * @brief	This is the source file for zcl_ota_attr
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
/**********************************************************************
 * INCLUDES
 */
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
