/********************************************************************************************************
 * @file     l2cap_stack.h
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

#ifndef STACK_BLE_L2CAP_L2CAP_STACK_H_
#define STACK_BLE_L2CAP_L2CAP_STACK_H_



typedef struct{
	u16 	connParaUpReq_minInterval;
	u16 	connParaUpReq_maxInterval;
	u16 	connParaUpReq_latency;
	u16 	connParaUpReq_timeout;

	u8		connParaUpReq_pending;
	u8      rsvd[3];
}para_up_req_t;

extern _attribute_aligned_(4) para_up_req_t	para_upReq;


typedef struct
{
	u8 *rx_p;
	u8 *tx_p;

	u16 max_rx_size;
	u16 max_tx_size;
}l2cap_buff_t;

extern _attribute_aligned_(4) l2cap_buff_t l2cap_buff;


ble_sts_t 	blt_update_parameter_request (void);
ble_sts_t   blc_l2cap_pushData_2_controller (u16 connHandle, u16 cid, u8 *format, int format_len, u8 *pDate, int data_len);




#endif /* STACK_BLE_L2CAP_L2CAP_STACK_H_ */
