/********************************************************************************************************
 * @file     att.h
 *
 * @brief    This is the header file for b91 BLE SDK
 *
 * @author	 BLE GROUP
 * @date         12,2021
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

#pragma once

#include "tl_common.h"


/** @defgroup ATT_PERMISSIONS_BITMAPS GAP ATT Attribute Access Permissions Bit Fields
 * @{
 * (See the Core_v5.0(Vol 3/Part C/10.3.1/Table 10.2) for more information)
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
#define ATT_PERMISSIONS_AUTHOR_WRITE         (ATT_PERMISSIONS_WRITE | ATT_PERMISSIONS_AUTHEN) 		//!< Write requires Authorization
#define ATT_PERMISSIONS_AUTHOR_RDWR          (ATT_PERMISSIONS_RDWR | ATT_PERMISSIONS_AUTHOR) 		//!< Read & Write requires Authorization


/** @} End GAP_ATT_PERMISSIONS_BITMAPS */


/** @ add to group GATT_Characteristic_Property GATT characteristic properties
 * @{
 */
#define CHAR_PROP_BROADCAST              0x01 //!< permit broadcasts of the Characteristic Value
#define CHAR_PROP_READ                   0x02 //!< permit reads of the Characteristic Value
#define CHAR_PROP_WRITE_WITHOUT_RSP      0x04 //!< Permit writes of the Characteristic Value without response
#define CHAR_PROP_WRITE                  0x08 //!< Permit writes of the Characteristic Value with response
#define CHAR_PROP_NOTIFY                 0x10 //!< Permit notifications of a Characteristic Value without acknowledgement
#define CHAR_PROP_INDICATE               0x20 //!< Permit indications of a Characteristic Value with acknowledgement
#define CHAR_PROP_AUTHEN                 0x40 //!< permit signed writes to the Characteristic Value
#define CHAR_PROP_EXTENDED               0x80 //!< additional characteristic properties are defined
/** @} end of group GATT_Characteristic_Property */




#if (MCU_CORE_TYPE == MCU_CORE_B91)
	typedef int (*att_readwrite_callback_t)(u16 connHandle, void* p);
#elif (MCU_CORE_TYPE == MCU_CORE_825x || MCU_CORE_TYPE == MCU_CORE_827x)
	typedef int (*att_readwrite_callback_t)(void* p);
#endif

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




/**
 * @brief	This function is used to define ATT MTU size exchange callback
 */
typedef int (*att_mtuSizeExchange_callback_t)(u16, u16);

/**
 * @brief	This function is used to define ATT Handle value confirm callback
 */
typedef int (*att_handleValueConfirm_callback_t)(void);




/**
 * @brief	This function is used to set ATT table
 * @param	*p - the pointer of attribute table
 * @return	none.
 */
void		bls_att_setAttributeTable (u8 *p);


//mtu size
/**
 * @brief	This function is used to set RX MTU size
 * @param	mtu_size - ATT MTU size
 * @return	0: success
 * 			other: fail
 */
ble_sts_t	blc_att_setRxMtuSize(u16 mtu_size);



/**
 * @brief	This function is used to set prepare write buffer
 * @param	*p - the pointer of buffer
 * @param	len - the length of buffer
 * @return	none.
 */
void  		blc_att_setPrepareWriteBuffer(u8 *p, u16 len);

/**
 * @brief	This function is used to request MTU size exchange
 * @param	connHandle - connect handle
 * @param	mtu_size - ATT MTU size
 * @return	0: success
 * 			other: fail
 */
//Attention: this API hide in stack, user no need use !!!
ble_sts_t	 blc_att_requestMtuSizeExchange (u16 connHandle, u16 mtu_size);

/**
 * @brief	This function is used to set effective ATT MTU size
 * @param	connHandle - connect handle
 * @param	effective_mtu - bltAtt.effective_MTU
 * @return	none.
 */
void  		blt_att_setEffectiveMtuSize(u16 connHandle, u8 effective_mtu);

/**
 * @brief	This function is used to reset effective ATT MTU size
 * @param	connHandle - connect handle
 * @return	none.
 */
void  		blt_att_resetEffectiveMtuSize(u16 connHandle);

/**
 * @brief	This function is used to reset RX MTU size
 * @param	mtu_size - ATT MTU size
 * @return	0: success
 * 			other: fail
 */
void  		blt_att_resetRxMtuSize(u16 connHandle);

/**
 * @brief   This function is used to get effective MTU size.
 * @param	connHandle - connect handle
 * @return  effective MTU value.
 */
u16  blc_att_getEffectiveMtuSize(u16 connHandle);

#if(MCU_CORE_TYPE == MCU_CORE_825x || MCU_CORE_TYPE == MCU_CORE_827x)
/**
 * @brief      set device name
 * @param[in]  p - the point of name
 * @param[in]  len - the length of name
 * @return     BLE_SUCCESS
 */
ble_sts_t 	bls_att_setDeviceName(u8* pName,u8 len);  //only module/mesh/hci use

ble_sts_t	blc_att_responseMtuSizeExchange (u16 connHandle, u16 mtu_size);
ble_sts_t	bls_att_pushNotifyData (u16 attHandle, u8 *p, int len);
ble_sts_t	bls_att_pushIndicateData (u16 attHandle, u8 *p, int len);

		// 0x04: ATT_OP_FIND_INFO_REQ
void 	att_req_find_info(u8 *dat, u16 start_attHandle, u16 end_attHandle);
		// 0x06: ATT_OP_FIND_BY_TYPE_VALUE_REQ
void 	att_req_find_by_type (u8 *dat, u16 start_attHandle, u16 end_attHandle, u8 *uuid, u8* attr_value, int len);
		// 0x08: ATT_OP_READ_BY_TYPE_REQ
void 	att_req_read_by_type (u8 *dat, u16 start_attHandle, u16 end_attHandle, u8 *uuid, int uuid_len);
		// 0x0a: ATT_OP_READ_REQ
void 	att_req_read (u8 *dat, u16 attHandle);
		// 0x0c: ATT_OP_READ_BLOB_REQ
void 	att_req_read_blob (u8 *dat, u16 attHandle, u16 offset);
		// 0x10: ATT_OP_READ_BY_GROUP_TYPE_REQ
void 	att_req_read_by_group_type (u8 *dat, u16 start_attHandle, u16 end_attHandle, u8 *uuid, int uuid_len);
		// 0x12: ATT_OP_WRITE_REQ
void 	att_req_write (u8 *dat, u16 attHandle, u8 *buf, int len);
		// 0x52: ATT_OP_WRITE_CMD
void 	att_req_write_cmd (u8 *dat, u16 attHandle, u8 *buf, int len);

#endif




