/********************************************************************************************************
 * @file     zcl_ias_wd_attr.c
 *
 * @brief	 attribute configuration for Warning Device equipment of the IAS system.
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
#ifdef ZCL_IAS_WD

/* Attribute default */
u16 zcl_attr_MaxDuration = 240;


/* Attribute record list */
const zclAttrInfo_t iasWD_attrTbl[] =
{
	{ ZCL_ATTRID_WD_MAX_DURATION,	   ZCL_DATA_TYPE_UINT16,     ACCESS_CONTROL_READ| ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_MaxDuration},
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_iasWD_attrNum = ( sizeof(iasWD_attrTbl) / sizeof(zclAttrInfo_t) );

#endif    /* ZCL_IAS_ZONE */
#endif
