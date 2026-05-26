/********************************************************************************************************
 * @file    debug.h
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
#ifndef STACK_BLE_DEBUG_H_
#define STACK_BLE_DEBUG_H_

/**
 *  @brief stack log
 */
typedef enum
{
    STK_LOG_NONE = 0,

    STK_LOG_LL_CMD  = BIT(0),
    STK_LOG_HCI_CMD = BIT(1),
    STK_LOG_HCI_EVT = BIT(2),

    STK_LOG_ATT_RX = BIT(5),
    STK_LOG_ATT_TX = BIT(6),

    STK_LOG_SMP_LTK = BIT(10),

    STK_LOG_OTA_FLOW = BIT(15),
    STK_LOG_OTA_DATA = BIT(16),

    STK_LOG_HCI_CS  = BIT(20),
    STK_LOG_EBQ_CS  = BIT(21),
    STK_LOG_LL_CS   = BIT(22),
    STK_LOG_ALGO_CS = BIT(23),

    STK_LOG_LL_RX = BIT(26),
    STK_LOG_LL_TX = BIT(27),
    STK_LOG_HCI_HDT = BIT(28),
    STK_LOG_LL_HDT  = BIT(29),
    STK_LOG_SNIF_ACL = BIT(30),
    STK_LOG_SNIF_CS  = BIT(31),

    STK_LOG_ALL = 0xFFFFFFFF,
} stk_log_msk_t;

/**
 * @brief      for user to configure which type of stack print information they want
 * @param[in]  mask - debug information combination
 * @return     none
 */
void blc_debug_enableStackLog(stk_log_msk_t mask);


/**
 * @brief      for user to add some type of stack print information they want
 * @param[in]  mask - debug information combination
 * @return     none
 */
void blc_debug_addStackLog(stk_log_msk_t mask);


/**
 * @brief      for user to remove some type of stack print information they want
 * @param[in]  mask - debug information combination
 * @return     none
 */
void blc_debug_removeStackLog(stk_log_msk_t mask);


/*********************************************************/
//#include "vcd.h"





/*********************************************************/


#endif /* STACK_BLE_DEBUG_H_ */
