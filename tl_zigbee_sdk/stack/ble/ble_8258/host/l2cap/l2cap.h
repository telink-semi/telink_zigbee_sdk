/********************************************************************************************************
 * @file    l2cap.h
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
#ifndef _L2CAP_H
#define _L2CAP_H

#include "tl_common.h"
/**
 * @brief MTU maximum value with SDK default buffer
 * 		  SDK use L2CAP TX & RX default buffer inside. Maximum MTU is 250
 * 		  if user need MTU greater than 250, they must register new buffer with API "blc l2cap_initMtuBuffer"
 */
#define	ATT_MTU_MAX_SDK_DFT_BUF				  250


/**
 * @brief	Telink defined L2CAP connect update response callback
 */
typedef int (*l2cap_conn_update_rsp_callback_t) (u8 id, u16 result);



/**
 * @brief	Connection parameter update response
 */
typedef enum{
	CONN_PARAM_UPDATE_ACCEPT = 0x0000,
	CONN_PARAM_UPDATE_REJECT = 0x0001,
}conn_para_up_rsp;



/**
 * @brief	This function is used to set connect request parameter for updating connect parameter
 * @param	min_interval - connect interval minimum
 * @param	max_interval - connect interval maximum
 * @param	latency - connect latency
 * @param	timeout - connect timeout
 * @return	none.
 */
void		bls_l2cap_requestConnParamUpdate (u16 min_interval, u16 max_interval, u16 latency, u16 timeout);


/**
 * @brief	This function is used to set the minimal time for sending connect parameter update request after connect being created
 * @param	time_ms - the unit is millisecond
 * @return	none.
 */
void        bls_l2cap_setMinimalUpdateReqSendingTime_after_connCreate(int time_ms);


/**
 * @brief	This function is used to register the function for handler L2CAP data
 * @param	p - the pointer direct to blc_l2cap_handler
 * @return	none.
 */
void		blc_l2cap_register_handler (void *p);


/**
 * @brief	This function is used to handler L2CAP data
 * @param	connHandle - connection handle
 * @param	p - the pointer of l2cap data
 * @return	0
 */
int 		blc_l2cap_packet_receive (u16 connHandle, u8 * p);


/**
 * @brief	This function is used to register the function to process L2CAP SIG CHANNEL packet
 * @param	p - the pointer of l2cap data
 * @return	none.
 */
void 		blc_l2cap_reg_att_sig_handler(void *p);


/**
 * @brief	This function is used to register the function to process L2CAP ATTRIBUTE PROCTOCOL packet
 * @param	p - the pointer of l2cap data
 * @return	none.
 */
void 		blc_l2cap_reg_att_cli_handler(void *p);

/**
 * @brief	This function is used to send connect parameter update response
 * @param	connHandle - connection handle
 * @param	req_id - Request packet identifier
 * @param	result - connect parameter update result
 * @return	none.
 */
void  		blc_l2cap_SendConnParamUpdateResponse(u16 connHandle, u8 req_id, conn_para_up_rsp result);


/**
 * @brief	This function is used to register the callback function for response connect parameter update
 * @param	cb - the pointer of callback function
 * @return	none.
 */
void 		blc_l2cap_registerConnUpdateRspCb(l2cap_conn_update_rsp_callback_t cb);


/**
 * @brief		This function is used to register l2cap buffer for big MTU
 * 			    attention: default buffer inside stack is enough for MTU size no greater than "ATT_MTU_MAX_SDK_DFT_BUF"
 * 			    		   if user need MTU size greater value, they must use this API to register bigger TX & RX buffer.
 * @param[in]	pMTU_rx_buff - the pointer of rx buffer
 * @param[in]	mtu_rx_size   - the size of of rx buffer
 * @param[in]	pMTU_tx_buff - the pointer of tx buffer
 * @param[in]	mtu_tx_size   - the size of of tx buffer
 * @return		Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t 	blc_l2cap_initMtuBuffer(u8 *pMTU_rx_buff, u16 mtu_rx_size, u8 *pMTU_tx_buff, u16 mtu_tx_size);


#endif
