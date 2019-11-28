/********************************************************************************************************
 * @file     zcl_occupancy_sensing_attr.c
 *
 * @brief	 attribute configuration for occupancy sensing cluster
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
#ifdef ZCL_OCCUPANCY_SENSING
u8  zcl_attr_occup = 0;
u8  zcl_attr_occupSensorType = 0;
u16 zcl_attr_pirOccu2UncoouD = 0;
u16 zcl_attr_pirUnoccu2OccuD = 0;
u8  zcl_attr_pirUnoccu2OccuT = 1;
u16 zcl_attr_ultraOccu2UncoouD = 0;
u16 zcl_attr_ultraUnoccu2OccuD = 0;
u8  zcl_attr_ultraUnoccu2OccuT = 1;

/* Attribute record list */
const zclAttrInfo_t occupancy_attrTbl[] =
{
#ifdef ZCL_ATTR_OCCUPANCY_ENABLE
	{ ZCL_ATTRID_OCCUPANCY,  ZCL_DATA_TYPE_BITMAP8,   ACCESS_CONTROL_READ,  (u8*)&zcl_attr_occup},
#endif
#ifdef ZCL_ATTR_OCCUPANCY_SENSOR_TYPE_ENABLE
	{ ZCL_ATTRID_OCCUPANCY_SENSOR_TYPE,  ZCL_DATA_TYPE_ENUM8,   ACCESS_CONTROL_READ,  (u8*)&zcl_attr_occupSensorType},
#endif

#ifdef ZCL_ATTR_PIR_OCCU2UNOCCU_DELAY_ENABLE
	{ ZCL_ATTRID_PIR_OCCU2UNOCCU_DELAY,  ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_pirOccu2UncoouD},
#endif
#ifdef ZCL_ATTR_PIR_UNOCCU2OCCU_DELAY_ENABLE
	{ ZCL_ATTRID_PIR_UNOCCU2OCCU_DELAY,  ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_pirUnoccu2OccuD},
#endif
#ifdef ZCL_ATTR_PIR_UNOCCU2OCCU_THRES_ENABLE
	{ ZCL_ATTRID_PIR_UNOCCU2OCCU_THRES,  ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_pirUnoccu2OccuT},
#endif

#ifdef ZCL_ATTR_ULTRA_OCCU2UNOCCU_DELAY_ENABLE
	{ ZCL_ATTRID_ULTRA_OCCU2UNOCCU_DELAY,  ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_ultraOccu2UncoouD},
#endif
#ifdef ZCL_ATTR_ULTRA_UNOCCU2OCCU_DELAY_ENABLE
	{ ZCL_ATTRID_ULTRA_UNOCCU2OCCU_DELAY,  ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_ultraUnoccu2OccuD},
#endif
#ifdef ZCL_ATTR_ULTRA_UNOCCU2OCCU_THRES_ENABLE
	{ ZCL_ATTRID_ULTRA_UNOCCU2OCCU_THRES,  ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_ultraUnoccu2OccuT},
#endif
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_occupancy_attrNum = ( sizeof(occupancy_attrTbl) / sizeof(zclAttrInfo_t) );

#endif    /* ZCL_OCCUPANCY_SENSING */

#endif
