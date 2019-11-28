/********************************************************************************************************
 * @file     zcl_door_lock_attr.c
 *
 * @brief	 attribute configuration for door lock cluster
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
