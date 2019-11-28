/********************************************************************************************************
 * @file     zcl_window_covering_attr.c
 *
 * @brief	 attribute configuration for window covering cluster
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
#ifdef ZCL_WINDOW_COVERING

/* Attribute default */
u8 zcl_attr_WindowCoveringType = 0;
u8 zcl_attr_ConfigStatus = 0x03;
u8 zcl_attr_Mode = 0x04;


/* Attribute record list */
const zclAttrInfo_t windowCovering_attrTbl[] =
{
    { ZCL_ATTRID_WINDOW_COVERING_TYPE,	   	ZCL_DATA_TYPE_ENUM8,     	ACCESS_CONTROL_READ,  						(u8*)&zcl_attr_WindowCoveringType},
    { ZCL_ATTRID_CONFIG_STATUS,	   			ZCL_DATA_TYPE_BITMAP8,     	ACCESS_CONTROL_READ,  						(u8*)&zcl_attr_ConfigStatus},
	{ ZCL_ATTRID_MODE,	   					ZCL_DATA_TYPE_BITMAP8,     	ACCESS_CONTROL_READ| ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_Mode},
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	ZCL_DATA_TYPE_UINT16,  		ACCESS_CONTROL_READ,  						(u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_windowCovering_attrNum = ( sizeof(windowCovering_attrTbl) / sizeof(zclAttrInfo_t) );

#endif    /* ZCL_IAS_ZONE */
#endif
