/********************************************************************************************************
 * @file	zcl_door_lock.h
 *
 * @brief	This is the header file for zcl_door_lock
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
#define zcl_doorLock_doorLockRspCmdSend(a,b,c,d,e)		(zcl_doorLock_doorLockRsp((a), (b), (c), ZCL_SEQ_NUM, (d), (e)))
#define zcl_doorLock_lockDoorRspCmdSend(a,b,c,d)		(zcl_doorLock_doorLockRsp((a), (b), (c), ZCL_SEQ_NUM, ZCL_CMD_LOCK_DOOR_RESPONSE, (d)))
#define zcl_doorLock_unlockDoorRspCmdSend(a,b,c,d)		(zcl_doorLock_doorLockRsp((a), (b), (c), ZCL_SEQ_NUM, ZCL_CMD_UNLOCK_DOOR_RESPONSE, (d)))
#define zcl_doorLock_toggleDoorRspCmdSend(a,b,c,d)		(zcl_doorLock_doorLockRsp((a), (b), (c), ZCL_SEQ_NUM, ZCL_CMD_DOOR_LOCK_TOGGLE_RESPONSE, (d)))

#endif	/* ZCL_DOOR_LOCK_H */
