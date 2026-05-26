/********************************************************************************************************
 * @file     attr_stack.h
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

#ifndef STACK_BLE_ATTR_ATTR_STACK_H_
#define STACK_BLE_ATTR_ATTR_STACK_H_







#define ATT_MTU_SIZE                        23  //!< Minimum ATT MTU size




//TELINK MTU no longer than 256, so 1 byte is enough
typedef struct{
	u16 init_MTU;
	u16 effective_MTU;

	u8 *pPendingPkt;

	u8 Data_pending_time;    //10ms unit
	u8 mtu_exchange_pending;
}att_para_t;
extern att_para_t bltAtt;






extern u16	blt_indicate_handle;









extern u32 att_service_discover_tick;

u8 * l2cap_att_handler(u16 connHandle, u8 * p);


u8 			blc_gatt_requestServiceAccess(u16 connHandle, int gatt_perm);




#if (MCU_CORE_TYPE == MCU_CORE_B91)
int			blt_att_sendMtuRequest (u16 connHandle);
#endif







#endif /* STACK_BLE_ATTR_ATTR_STACK_H_ */
