/********************************************************************************************************
 * @file     gap_stack.h
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

#ifndef STACK_BLE_GAP_STACK_H_
#define STACK_BLE_GAP_STACK_H_





typedef int (*host_ota_mian_loop_callback_t)(void);
typedef int (*host_ota_terminate_callback_t)(u16);
extern host_ota_mian_loop_callback_t    		host_ota_main_loop_cb;
extern host_ota_terminate_callback_t    		host_ota_terminate_cb;







/******************************* gap event start ************************************************************************/

extern u32		gap_eventMask;


int blc_gap_send_event (u32 h, u8 *para, int n);

/******************************* gap event  end ************************************************************************/



/******************************* gap start ************************************************************************/



/**
 * @brief      this  function is used to initialize GAP Central
 * @param	   none
 * @return     none
 */
void 		blc_gap_central_init(void);



/******************************* gap end ************************************************************************/


#endif /* GAP_STACK_H_ */
