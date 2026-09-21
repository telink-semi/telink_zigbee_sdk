/********************************************************************************************************
 * @file    ll_conn.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef LL_CONN_H_
#define LL_CONN_H_


/**
 * @brief	LE connection handle for slave & master role
 * 			Telink single connection SDK support only one ACL Slave or only one Master connection,
 * 			We use constant connection handle value for them in controller to simplify design and development,
 * 			so user can use these two connection handle directly.
 * 			attention: If using Telink multiple connection SDK, connection handle must be managed dynamically !!!
 */
#define			BLM_CONN_HANDLE									0x0080	//ACL Master connection handle
#define			BLS_CONN_HANDLE									0x0040	//ACL Slave connection handle



#define 		BLE_INVALID_CONNECTION_HANDLE    				0xffff
#define 		IS_CONNECTION_HANDLE_VALID(handle)  			( handle != BLE_INVALID_CONNECTION_HANDLE )

/**
 * @brief      this function is used to initialize ACL connection module.
 * 			   notice that: user must sue this API for both connection slave and master role.
 * @param	   none
 * @return     none
 */
void		blc_ll_initConnection_module(void);



/**
 * @brief      this function is used to exchange data length
 * @param[in]  opcode - LL Control PDU Opcode
 * @param[in]  maxTxOct -  maximum Tx octets
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t 	blc_ll_exchangeDataLength (u8 opcode, u16 maxTxOct);


/**
 * @brief	This function is used to obtain the effective maximum TX data length
 * @param	none
 * @return	EffectiveMaxTxOctets
 */
u16 blc_ll_get_connEffectiveMaxTxOctets(void);

/**
 * @brief	This function is used to obtain the effective maximum RX data length
 * @param	none
 * @return	EffectiveMaxRxOctets
 */
u16 blc_ll_get_connEffectiveMaxRxOctets(void);

/**
 * @brief      this function is used to set maximum MD(more data) number.
 * @param[in]  num - max number
 * @return     none
 */
void 		blc_ll_init_max_md_nums(u8 num);


#endif /* LL_CONN_H_ */
