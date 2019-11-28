/********************************************************************************************************
 * @file     zcl_scene_attr.c
 *
 * @brief	 attribute configuration for scene cluster
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
#ifdef ZCL_SCENE

zcl_sceneAttr_t zcl_attr_scene = { 0x00, 0x00, 0x0000, 0x00, 0x00};

/* Attribute record list */
const zclAttrInfo_t scene_attrTbl[] =
{
    { ZCL_ATTRID_SCENE_SCENE_COUNT,       ZCL_DATA_TYPE_UINT8,    	ACCESS_CONTROL_READ,  (u8*)&zcl_attr_scene.sceneCnt       },
    { ZCL_ATTRID_SCENE_CURRENT_SCENE,     ZCL_DATA_TYPE_UINT8,    	ACCESS_CONTROL_READ,  (u8*)&zcl_attr_scene.currentScene   },
    { ZCL_ATTRID_SCENE_CURRENT_GROUP,     ZCL_DATA_TYPE_UINT16,   	ACCESS_CONTROL_READ,  (u8*)&zcl_attr_scene.currentGroup   },
    { ZCL_ATTRID_SCENE_SCENE_VALID,       ZCL_DATA_TYPE_BOOLEAN,  	ACCESS_CONTROL_READ,  (u8*)&zcl_attr_scene.sceneValid     },
    { ZCL_ATTRID_SCENE_NAME_SUPPORT,      ZCL_DATA_TYPE_BITMAP8,  	ACCESS_CONTROL_READ,  (u8*)&zcl_attr_scene.nameSupport    },
#ifdef ZCL_ATTR_LAST_CONFIG_BY_ENABLE
    { ZCL_ATTRID_SCENE_LAST_CONFIG_BY,    ZCL_DATA_TYPE_IEEE_ADDR,  ACCESS_CONTROL_READ,  (u8*)&zcl_attr_scene.lastConfigBy   },
#endif
    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_scene_attrNum = ( sizeof(scene_attrTbl) / sizeof(zclAttrInfo_t) );

nv_sts_t zcl_scene_saveAttr(void)
{
	return zcl_attr_sceneParams_save();
}

nv_sts_t zcl_scene_loadAttr(void)
{
	return zcl_attr_sceneParams_restore();
}

#endif    /* ZCL_SCENE */

#endif
