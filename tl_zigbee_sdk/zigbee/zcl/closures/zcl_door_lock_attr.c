/********************************************************************************************************
 * @file    zcl_door_lock_attr.c
 *
 * @brief   This is the source file for zcl_door_lock_attr
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
#ifdef ZCL_DOOR_LOCK

/* Attribute default */
zcl_attr_doorLock_t g_zclDoorLockAttr = {
	.zcl_attr_LockState 		= 0,
	.zcl_attr_LockType 			= 0,
	.zcl_attr_ActuatorEnabled 	= 0,
	.zcl_attr_sendPinOta 		= 0,
	.zcl_attr_requirePinForRfOperation = 0,
};


/* Attribute record list */
const zclAttrInfo_t doorLock_attrTbl[] =
{
    { ZCL_ATTRID_LOCK_STATE,	   				ZCL_DATA_TYPE_ENUM8,     ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE,  	(u8*)&g_zclDoorLockAttr.zcl_attr_LockState},
    { ZCL_ATTRID_LOCK_TYPE,	   					ZCL_DATA_TYPE_ENUM8,     ACCESS_CONTROL_READ,  								(u8*)&g_zclDoorLockAttr.zcl_attr_LockType},
	{ ZCL_ATTRID_ACTUATOR_ENABLED,				ZCL_DATA_TYPE_BOOLEAN,   ACCESS_CONTROL_READ,  								(u8*)&g_zclDoorLockAttr.zcl_attr_ActuatorEnabled},
	{ ZCL_ATTRID_SEND_PIN_OTA,					ZCL_DATA_TYPE_BOOLEAN,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,		(u8*)&g_zclDoorLockAttr.zcl_attr_sendPinOta},
	{ ZCL_ATTRID_REQUIRE_PIN_FOR_RF_OPERATION,	ZCL_DATA_TYPE_BOOLEAN,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,		(u8*)&g_zclDoorLockAttr.zcl_attr_requirePinForRfOperation},
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 		ZCL_DATA_TYPE_UINT16,  	 ACCESS_CONTROL_READ,  								(u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_doorLock_attrNum = ( sizeof(doorLock_attrTbl) / sizeof(zclAttrInfo_t) );

#endif    /* ZCL_DOOR_LOCK */

#endif
