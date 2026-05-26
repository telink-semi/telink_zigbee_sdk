/********************************************************************************************************
 * @file    acl_peripheral.h
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
#ifndef ACL_PERIPHERAL_H_
#define ACL_PERIPHERAL_H_


/**
 * @brief      for user to initialize ACL connection peripheral role.
 * @param      none
 * @return     none
 */
void blc_ll_initAclPeriphrRole_module(void);


/**
 * @brief      for user to initialize LinkLayer ACL connection peripheral role TX FIFO.
 * @param[in]  pRxbuf - TX FIFO buffer address.
 * @param[in]  fifo_size - TX FIFO size
 * @param[in]  fifo_number - TX FIFO number, can only be 4, 8, 16 or 32
 * @param[in]  conn_number - Number of supported ACL peripheral connections
 * @return     status, 0x00:  succeed
 *                     other: failed
 */
ble_sts_t blc_ll_initAclPeriphrTxFifo(u8 *pTxbuf, int fifo_size, int fifo_number, int conn_number);


#endif /* ACL_PERIPHERAL_H_ */
