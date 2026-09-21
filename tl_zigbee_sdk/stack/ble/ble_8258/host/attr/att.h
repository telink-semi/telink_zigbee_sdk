/********************************************************************************************************
 * @file    att.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    12,2021
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
 *
 *******************************************************************************************************/
#pragma once

#include "tl_common.h"


/**
 * @brief		ATT_PERMISSIONS_BITMAPS GAP ATT Attribute Access Permissions Bit Fields
 * refer to BLE SPEC: Vol 3,Part C,"10.3.1 Responding to a service request" for more information
 */
#define ATT_PERMISSIONS_AUTHOR				 0x10 //Attribute access(Read & Write) requires Authorization
#define ATT_PERMISSIONS_ENCRYPT				 0x20 //Attribute access(Read & Write) requires Encryption
#define ATT_PERMISSIONS_AUTHEN				 0x40 //Attribute access(Read & Write) requires Authentication(MITM protection)
#define ATT_PERMISSIONS_SECURE_CONN			 0x80 //Attribute access(Read & Write) requires Secure_Connection
#define ATT_PERMISSIONS_SECURITY			 (ATT_PERMISSIONS_AUTHOR | ATT_PERMISSIONS_ENCRYPT | ATT_PERMISSIONS_AUTHEN | ATT_PERMISSIONS_SECURE_CONN)

//user can choose permission below
#define ATT_PERMISSIONS_READ                 0x01 //!< Attribute is Readable
#define ATT_PERMISSIONS_WRITE                0x02 //!< Attribute is Writable
#define ATT_PERMISSIONS_RDWR	             (ATT_PERMISSIONS_READ | ATT_PERMISSIONS_WRITE)   //!< Attribute is Readable & Writable


#define ATT_PERMISSIONS_ENCRYPT_READ         (ATT_PERMISSIONS_READ  | ATT_PERMISSIONS_ENCRYPT) 		//!< Read requires Encryption
#define ATT_PERMISSIONS_ENCRYPT_WRITE        (ATT_PERMISSIONS_WRITE | ATT_PERMISSIONS_ENCRYPT) 		//!< Write requires Encryption
#define ATT_PERMISSIONS_ENCRYPT_RDWR         (ATT_PERMISSIONS_RDWR  | ATT_PERMISSIONS_ENCRYPT) 		//!< Read & Write requires Encryption


#define ATT_PERMISSIONS_AUTHEN_READ          (ATT_PERMISSIONS_READ  | ATT_PERMISSIONS_ENCRYPT | ATT_PERMISSIONS_AUTHEN) 		//!< Read requires Authentication
#define ATT_PERMISSIONS_AUTHEN_WRITE         (ATT_PERMISSIONS_WRITE | ATT_PERMISSIONS_ENCRYPT | ATT_PERMISSIONS_AUTHEN) 		//!< Write requires Authentication
#define ATT_PERMISSIONS_AUTHEN_RDWR          (ATT_PERMISSIONS_RDWR  | ATT_PERMISSIONS_ENCRYPT | ATT_PERMISSIONS_AUTHEN) 		//!< Read & Write requires Authentication


#define ATT_PERMISSIONS_SECURE_CONN_READ	 (ATT_PERMISSIONS_READ  | ATT_PERMISSIONS_SECURE_CONN | ATT_PERMISSIONS_ENCRYPT | ATT_PERMISSIONS_AUTHEN)   //!< Read requires Secure_Connection
#define ATT_PERMISSIONS_SECURE_CONN_WRITE    (ATT_PERMISSIONS_WRITE | ATT_PERMISSIONS_SECURE_CONN | ATT_PERMISSIONS_ENCRYPT | ATT_PERMISSIONS_AUTHEN)  //!< Write requires Secure_Connection
#define ATT_PERMISSIONS_SECURE_CONN_RDWR	 (ATT_PERMISSIONS_RDWR  | ATT_PERMISSIONS_SECURE_CONN | ATT_PERMISSIONS_ENCRYPT | ATT_PERMISSIONS_AUTHEN)   //!< Read & Write requires Secure_Connection


#define ATT_PERMISSIONS_AUTHOR_READ          (ATT_PERMISSIONS_READ | ATT_PERMISSIONS_AUTHOR) 		//!< Read requires Authorization
#define ATT_PERMISSIONS_AUTHOR_WRITE         (ATT_PERMISSIONS_WRITE | ATT_PERMISSIONS_AUTHOR) 		//!< Write requires Authorization
#define ATT_PERMISSIONS_AUTHOR_RDWR          (ATT_PERMISSIONS_RDWR | ATT_PERMISSIONS_AUTHOR) 		//!< Read & Write requires Authorization

/** @End GAP_ATT_PERMISSIONS_BITMAPS */




/**
 * @brief		GATT_Characteristic_Property GATT characteristic properties
 */
#define CHAR_PROP_BROADCAST              0x01 //!< permit broadcasts of the Characteristic Value
#define CHAR_PROP_READ                   0x02 //!< permit reads of the Characteristic Value
#define CHAR_PROP_WRITE_WITHOUT_RSP      0x04 //!< Permit writes of the Characteristic Value without response
#define CHAR_PROP_WRITE                  0x08 //!< Permit writes of the Characteristic Value with response
#define CHAR_PROP_NOTIFY                 0x10 //!< Permit notifications of a Characteristic Value without acknowledgement
#define CHAR_PROP_INDICATE               0x20 //!< Permit indications of a Characteristic Value with acknowledgement
#define CHAR_PROP_AUTHEN                 0x40 //!< permit signed writes to the Characteristic Value
#define CHAR_PROP_EXTENDED               0x80 //!< additional characteristic properties are defined
/** end of group GATT_Characteristic_Property */



typedef int (*att_readwrite_callback_t)(void* p);


typedef struct attribute
{
  u16  attNum;
  u8   perm;
  u8   uuidLen;
  u32  attrLen;    //4 bytes aligned
  u8* uuid;
  u8* pAttrValue;
  att_readwrite_callback_t w;
  att_readwrite_callback_t r;
} attribute_t;

typedef struct {
	unsigned char opcode;
	unsigned char data[0];
} attr_pkt_t;

/**
 * @brief	ATT multiple handle value notify structure
 */
typedef struct {
	u16 handle;
	u16 length;
	u8* value;
} atts_mulHandleNtf_t;

/**
 * @brief	This function is used to define ATT MTU size exchange callback
 */
typedef int (*att_mtuSizeExchange_callback_t)(u16, u16);

/**
 * @brief	This function is used to define ATT Handle value confirm callback
 */
typedef int (*att_handleValueConfirm_callback_t)(void);


/**
 * @brief		application custom ATT handle table element structure
 * @attention	All att handles, including attHl_sdk and attHl_cus must be sorted in ascending order.
 * @attention	The minimum attHl_cus must larger than att table size.
 */
typedef struct att_convert_t{
  u16  attHl_sdk; //attribute handle value in attribute table
  u16  attHl_cus; //attribute handle value for custom need
} attHl_convert_t;


/**
 * @brief	This function is used to set ATT table
 * @param	p - the pointer of attribute table
 * @return	none.
 */
void		bls_att_setAttributeTable(u8 *p);





/**
 * @brief	This function is used to set RX MTU size
 * 			if not call this API, default MTU size is 23; if user want to use greater MTU, this API must be called.
 * @param	mtu_size - ATT MTU size, in range of 23 ~ 512
 * @return	0: success
 * 			other: fail
 */
ble_sts_t	blc_att_setRxMtuSize(u16 mtu_size);


/**
 * @brief	This function is used to request MTU size exchange
 * @param	connHandle - connect handle
 * @param	mtu_size - ATT MTU size
 * @return	0: success
 * 			other: fail
 */
ble_sts_t	 blc_att_requestMtuSizeExchange (u16 connHandle, u16 mtu_size);




/**
 * @brief	This function is used to set effective ATT MTU size
 * 			attention: only ACL Master use this API !!!
 * 					   ACL Slave no need to use this API(SDK already do it in stack inside)
 * @param	connHandle - connect handle
 * @param	effective_mtu - bltAtt.effective_MTU
 * @return	none.
 */
void  		blc_att_setEffectiveMtuSize(u16 connHandle, u16 effective_mtu);

/**
 * @brief	   This function is used to reset effective ATT MTU size
 * 			   attention: only ACL Master use this API !!!
 * 					   ACL Slave no need to use this API(SDK already do it in stack inside)
 * @param[in]  connHandle - connect handle
 * @return	   none.
 */
void  		blc_att_resetEffectiveMtuSize(u16 connHandle);

/**
 * @brief   This function is used to get effective MTU size.
 * 			attention: only ACL Master use this API !!!
 * @param	connHandle - connect handle
 * @return  effective MTU value.
 */
u16  		blc_att_getEffectiveMtuSize(u16 connHandle);

/**
 * @brief      This function is used to response to MTU size exchange.
 * 			   attention: only ACL Master use this API !!!
 * 					   ACL Slave no need to use this API(SDK already do it in stack inside)
 * @param[in]  connHandle - connect handle
 * @param[in]  mtu_size - MTU size
 * @return     success or fail
 */
ble_sts_t	blc_att_responseMtuSizeExchange (u16 connHandle, u16 mtu_size);

/**
 * @brief   This function is used to set the server data pending time when some client command triggered.
 *          "ServerDataPendingTime" default value is 300mS, use can change this value with this API.
 *
 *          In big amount of Central device test, we find that a handle value notify or handle value indication command during
 *          Central device service discovery process sometimes make the service discovery fail.
 *          To solve this issue, we design a mechanism to optimize: to block these server command for a while when detected that
 *          there maybe a service discovery is ongoing.
 *          When server receives any of the 4 client command below:
 *          (1) ATT_OP_READ_BY_GROUP_TYPE_REQ
 *          (2) ATT_OP_FIND_BY_TYPE_VALUE_REQ
 *          (3) ATT_OP_READ_BY_TYPE_REQ
 *          (4) ATT_OP_FIND_INFO_REQ
 *			stack internal will start a timer, the timeout value of this timer is "ServerDataPendingTime".
 *			If this timer did not reach the timeout value, error "GATT_ERR_DATA_PENDING_DUE_TO_SERVICE_DISCOVERY_BUSY" will return
 *			for these 3 GATT API below, server data not allowed to send.
 *          (1) blc gatt_pushHandleValueNotify
 *          (2) blc gatt_pushMultiHandleValueNotify
 *          (3) blc gatt_pushHandleValueIndicate
 *
 *         Note that this design may bring some unexpected effect. For example, blc gatt_pushHandleValueNotify sometimes may fail due to
 *         a ATT_OP_READ_BY_TYPE_REQ command received, but not in SDP process. User can set "ServerDataPendingTime" to 0 by this API to
 *         avoid this kind of situation.
 * @param	num_10ms - the pending time, step is 10ms.
 * @return	none.
 */
void  blc_att_setServerDataPendingTime_upon_ClientCmd(u8 num_10ms);

/**
 * @brief	This function is used to set prepare write buffer
 * @param	p - the pointer of buffer
 * @param	len - the length of buffer
 * @return	none.
 */
void  		blc_att_setPrepareWriteBuffer(u8 *p, u16 len);


/**
 * @brief      This function is used to set reject of write request. If enable, return of ATT write callback will take effect.  Error codes refer to Core Spec.
 * @param[in]  WriteReqReject_en - 0: Disable;
 *                           1: Enable.
 * @return     none.
 */
void 		blc_att_enableWriteReqReject (u8 WriteReqReject_en);

/**
 * @brief      This function is used to set reject of read request. If enable, return of ATT read callback will take effect.  Error codes refer to Core Spec.
 * @param[in]  ReadReqReject_en - 0: Disable;
 *                           1: Enable.
 * @return     none.
 */
void 		blc_att_enableReadReqReject (u8 ReadReqReject_en);


/**
 * @brief      This function is used to set device name
 * @param[in]  pName - the point of name
 * @param[in]  len - the length of name
 * @return     BLE_SUCCESS
 */
ble_sts_t 	bls_att_setDeviceName(u8* pName,u8 len);  //only module/mesh/hci use


/**
 * @brief      This function is used to set whether to hold the ATT Response PDU during the pairing phase 3.
 * @param[in]  hold_enable - 1: enable, holding ATT Response PDU during in pairing phase
 *                      0: disable, allowing ATT Response PDU during in pairing phase
 * @return     none.
 */
void         blc_att_holdAttributeResponsePayloadDuringPairingPhase(u8 hold_enable);
