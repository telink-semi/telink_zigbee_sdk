/********************************************************************************************************
 * @file    zcl_scene_attr.c
 *
 * @brief   This is the source file for zcl_scene_attr
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
