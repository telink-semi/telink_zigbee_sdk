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

    STK_LOG_LL_CMD              = BIT(1),
    STK_LOG_ACL_DATA            = BIT(2),
    STK_LOG_HCI                 = BIT(3),
    STK_LOG_BSTNOOP             = BIT(4),

    STK_LOG_SMP                 = BIT(5),
    STK_LOG_ATT                 = BIT(6),
    STK_LOG_MAILBOX             = BIT(7),
    STK_LOG_SHAREMEM            = BIT(8),
    STK_LOG_HOST_COMMON         = BIT(9),

    STK_LOG_SMP_LTK             = BIT(10),
    STK_LOG_PM                  = BIT(11),

    STK_LOG_OTA_FLOW            = BIT(15),
    STK_LOG_OTA_DATA            = BIT(16),

    STK_LOG_HCI_CS              = BIT(20),
    STK_LOG_EBQ_CS              = BIT(21),
    STK_LOG_LL_CS               = BIT(22),
    STK_LOG_ALGO_CS             = BIT(23),
    STK_LOG_MULTI_CS            = BIT(24),

    STK_LOG_LL_RX               = BIT(26),
    STK_LOG_LL_TX               = BIT(27),
    STK_LOG_HCI_HDT             = BIT(28),
    STK_LOG_LL_HDT              = BIT(29),
    STK_LOG_SNIF_ACL            = BIT(30),
    STK_LOG_SNIF_CS             = BIT(31),

    STK_LOG_ALL                 = 0xFFFFFFFF,
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

/**
 * @brief      Get stack error log from SRAM error buffer (same as get_error)
 * @param[in]  buf  - pointer to buffer for storing error codes
 * @param[in]  size - max number of error codes to read(default 32)
 * @return     non-zero: has error / zero: no error
 */
u8 blc_ll_get_stack_error(u32 *buf, u8 size);



/**
 * @brief debug gpio index enumeration based on stack logic
 */
typedef enum {
    STK_IO_BLE_RF_RX_IRQ     = 0,
    STK_IO_BLE_RF_TX_IRQ     = 1,
    STK_IO_BLE_RF_PER_S_P_0  = 2, // PERIPHERAL
    STK_IO_BLE_RF_PER_S_P_1  = 3, // PERIPHERAL
    STK_IO_BLE_RF_PER_S_P_2  = 4, // PERIPHERAL
    STK_IO_BLE_RF_PER_S_P_3  = 5, // PERIPHERAL
    STK_IO_BLE_PM_START_POST = 6,
    STK_IO_BLE_MAX_NUM       = 7, // END
} stk_debug_io_t;

/**
 * @brief       bind an external mapping table to the library
 * @param[in]   p_table - pointer to the GPIO pin array defined in application
 * @param[in]   num     - number of elements in the array
 * @return      none
 * @note        Hardware initialization (function_en, output_en, etc.)
 * must be handled by the application layer.
 */
void blc_debug_gpio_init(gpio_pin_e *p_table, uint8_t num);

#endif /* STACK_BLE_DEBUG_H_ */
