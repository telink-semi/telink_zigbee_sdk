/********************************************************************************************************
 * @file    zcl_door_lock.h
 *
 * @brief   This is the header file for zcl_door_lock
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
 *
 *******************************************************************************************************/

#ifndef ZCL_DOOR_LOCK_H
#define ZCL_DOOR_LOCK_H



/*********************************************************************
 * CONSTANTS
 */
/**
 *  @brief	Door lock cluster Attribute IDs
 */
//Basic Information Attribute Set
#define ZCL_ATTRID_LOCK_STATE                                0x0000
#define ZCL_ATTRID_LOCK_TYPE                                 0x0001
#define ZCL_ATTRID_ACTUATOR_ENABLED                          0x0002
//Security Settings Attribute Set
#define ZCL_ATTRID_SEND_PIN_OTA								 0x0032
#define ZCL_ATTRID_REQUIRE_PIN_FOR_RF_OPERATION				 0x0033

/**
 *  @brief	Door lock cluster Command IDs
 */
//Server Commands Received
#define ZCL_CMD_LOCK_DOOR                                    	0x00
#define ZCL_CMD_UNLOCK_DOOR                                  	0x01
#define ZCL_CMD_DOOR_LOCK_TOGGLE							 	0x02
//Server Commands Generated
#define ZCL_CMD_LOCK_DOOR_RESPONSE                           	0x00
#define ZCL_CMD_UNLOCK_DOOR_RESPONSE                         	0x01
#define ZCL_CMD_DOOR_LOCK_TOGGLE_RESPONSE					 	0x02


/**
 *  @brief	LockState Attribute Values
 */
#define LOCK_STATE_NOT_FULLY_LOCKED								0x00
#define LOCK_STATE_LOCKED										0x01
#define LOCK_STATE_UNLOCKED										0x02
#define LOCK_STATE_UNDEFINED									0xFF

/**
 *  @brief	LockType Attribute Values
 */
#define LOCK_TYPE_DEAD_BOLT										0x00
#define LOCK_TYPE_MAGNETIC										0x01
#define LOCK_TYPE_OTHER											0x02
#define LOCK_TYPE_MORTISE										0x03
#define LOCK_TYPE_RIM											0x04
#define LOCK_TYPE_LATCH_BOLT									0x05
#define LOCK_TYPE_CYLINDRICAL_LOCK								0x06
#define LOCK_TYPE_TUBULAR_LOCK									0x07
#define LOCK_TYPE_INTERCONNECTED_LOCK							0x08
#define LOCK_TYPE_DEAD_LATCH									0x09
#define LOCK_TYPE_DOOR_FURNITURE								0x0A


typedef struct{
	void *pData;
	u8 	dataLen;
}zcl_doorlockReq_t;

typedef struct{
	u8 status;
}zcl_doorlockRsp_t;

#if 0
/* Command callBack Function Type */
typedef status_t (*zcl_doorLock_doorLockReqCb_t)(apsdeDataInd_t *pInd, u8 cmdId, u8 dataLen, u8 *pData);
typedef void (*zcl_doorLock_doorLockRspCb_t)(apsdeDataInd_t *pInd, u8 cmdId, u8 rspStatus);

/**
 *  @brief  Structure definition for commands callBack functions in door lock cluster
 */
typedef struct {
	zcl_doorLock_doorLockReqCb_t	doorLockReqCbFunc;
    zcl_doorLock_doorLockRspCb_t  	doorLockRspCbFunc;
}zcl_doorLock_AppCallbacks_t;

typedef struct {
	u8 zcl_attr_LockState;
	u8 zcl_attr_LockType;
	u8 zcl_attr_ActuatorEnabled;
	u8 zcl_attr_sendPinOta;
	u8 zcl_attr_requirePinForRfOperation;
}zcl_attr_doorLock_t;


extern const zclAttrInfo_t doorLock_attrTbl[];
extern const u8 zcl_doorLock_attrNum;

extern zcl_attr_doorLock_t g_zclDoorLockAttr;

#define zcl_doorLockAttrGet()	&g_zclDoorLockAttr
#endif

status_t zcl_doorLock_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

status_t zcl_doorLock_doorLockReq(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 cmdId, u8 *pCode);
#define zcl_doorLock_lockDoorCmdSend(a,b,c,d)		(zcl_doorLock_doorLockReq((a), (b), (c), ZCL_SEQ_NUM, ZCL_CMD_LOCK_DOOR, (d)))
#define zcl_doorLock_unlockDoorCmdSend(a,b,c,d)		(zcl_doorLock_doorLockReq((a), (b), (c), ZCL_SEQ_NUM, ZCL_CMD_UNLOCK_DOOR, (d)))
#define zcl_doorLock_toggleDoorCmdSend(a,b,c,d)		(zcl_doorLock_doorLockReq((a), (b), (c), ZCL_SEQ_NUM, ZCL_CMD_DOOR_LOCK_TOGGLE, (d)))

status_t zcl_doorLock_doorLockRsp(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 cmdId, status_t rspStatus);
#define zcl_doorLock_doorLockRspCmdSend(a,b,c,d,e,f)	(zcl_doorLock_doorLockRsp((a), (b), (c), (d), (e), (f)))
#define zcl_doorLock_lockDoorRspCmdSend(a,b,c,d,e)		(zcl_doorLock_doorLockRsp((a), (b), (c), (d), ZCL_CMD_LOCK_DOOR_RESPONSE, (e)))
#define zcl_doorLock_unlockDoorRspCmdSend(a,b,c,d,e)	(zcl_doorLock_doorLockRsp((a), (b), (c), (d), ZCL_CMD_UNLOCK_DOOR_RESPONSE, (e)))
#define zcl_doorLock_toggleDoorRspCmdSend(a,b,c,d,e)	(zcl_doorLock_doorLockRsp((a), (b), (c), (d), ZCL_CMD_DOOR_LOCK_TOGGLE_RESPONSE, (e)))

#endif	/* ZCL_DOOR_LOCK_H */
