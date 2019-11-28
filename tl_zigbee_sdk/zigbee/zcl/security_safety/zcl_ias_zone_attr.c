/********************************************************************************************************
 * @file     zcl_ias_zone_attr.c
 *
 * @brief	 attribute configuration for IAS zone cluster
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


/**********************************************************************
 * LOCAL TYPES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */
#ifdef ZCL_IAS_ZONE

/* Attribute default */
u8  zcl_attr_zoneState = ZONE_STATE_NOT_ENROLLED;
u16 zcl_attr_zoneType = 0;
u16 zcl_attr_zoneStatus = 0;
u8  zcl_attr_iasCieAddr[8] = {0};
u8  zcl_attr_zoneID = 0xFF;

/* Attribute record list */
const zclAttrInfo_t iasZone_attrTbl[] =
{
	{ ZCL_ATTRID_ZONE_STATE,   ZCL_DATA_TYPE_ENUM8,     ACCESS_CONTROL_READ,  (u8*)&zcl_attr_zoneState },
	{ ZCL_ATTRID_ZONE_TYPE,    ZCL_DATA_TYPE_ENUM16,    ACCESS_CONTROL_READ,  (u8*)&zcl_attr_zoneType },
	{ ZCL_ATTRID_ZONE_STATUS,  ZCL_DATA_TYPE_BITMAP16,  ACCESS_CONTROL_READ,  (u8*)&zcl_attr_zoneStatus },
	{ ZCL_ATTRID_IAS_CIE_ADDR, ZCL_DATA_TYPE_IEEE_ADDR, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  zcl_attr_iasCieAddr },
	{ ZCL_ATTRID_ZONE_ID,	   ZCL_DATA_TYPE_UINT8,     ACCESS_CONTROL_READ,  (u8*)&zcl_attr_zoneID},
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_iasZone_attrNum = ( sizeof(iasZone_attrTbl) / sizeof(zclAttrInfo_t) );

#endif    /* ZCL_IAS_ZONE */
#endif
