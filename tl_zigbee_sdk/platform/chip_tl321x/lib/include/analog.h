/********************************************************************************************************
 * @file    analog.h
 *
 * @brief   This is the header file for TL321X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/*******************************      analog control registers: 0xb8      ******************************/
/** @page ANALOG
 *
 *  Introduction
 *  ===============
 *  analog support dma and normal mode, in each mode, support byte/halfword/word/buffer write and read.
 *  When reading and writing analog registers in DMA mode, exit interface after configuration.
 *  But the actual operation of the analog register is not finished, and the DMA is still moving the data.
 *  An interrupt may be opened at this time, and if there is an operation on the analog register,
 *  it will interrupt the previous DMA reading and writing the analog register, creating an unknown risk.
 *  Therefore, it is not recommended to use DMA to read and write analog registers.
 *
 *  API Reference
 *  ===============
 *  Header File: analog.h
 */
#pragma once


#include "reg_include/register.h"
#include "compiler.h"
#include "lib/include/core.h"
#include "error_handler/error_handler.h"


/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/
#define ANALOG_TIMEOUT 0

/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/


/**
 * @brief      This function serves to analog register read by byte.
 * @param[in]  addr - address need to be read.
 * @return     the result of read.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ unsigned char analog_read_reg8(unsigned char addr);

/**
 * @brief      This function serves to analog register write by byte.
 * @param[in]  addr - address need to be write.
 * @param[in]  data - the value need to be write.
 * @return     none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void analog_write_reg8(unsigned char addr, unsigned char data);

/**
 * @brief      This function serves to analog register read by halfword.
 * @param[in]  addr - address need to be read.
 * @return     the result of read.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ unsigned short analog_read_reg16(unsigned char addr);

/**
 * @brief      This function serves to analog register write by halfword.
 * @param[in]  addr - address need to be write.
 * @param[in]  data - the value need to be write.
 * @return     none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void analog_write_reg16(unsigned char addr, unsigned short data);

/**
 * @brief      This function serves to analog register read by word.
 * @param[in]  addr - address need to be read.
 * @return     the result of read.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ unsigned int analog_read_reg32(unsigned char addr);

/**
 * @brief      This function serves to analog register write by word.
 * @param[in]  addr - address need to be write.
 * @param[in]  data - the value need to be write.
 * @return     none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void analog_write_reg32(unsigned char addr, unsigned int data);
/**
 * @brief      This function serves to analog register read.
 * @param[in]  addr  - address need to be read.
 * @param[out] buff  - the ptr of buffer to store the read data.
 * @param[in]  len   - the length of read value.
 * @return     none.
 */
_attribute_ram_code_sec_noinline_ void analog_read_buff(unsigned char addr, unsigned char *buff, unsigned char len);

/**
 * @brief      This function serves to analog register write.
 * @param[in]  addr  - address need to be write.
 * @param[in]  buff  - the ptr of value need to be write.
 * @param[in]  len   - the length of write value.(The data length cannot be greater than 8)
 * @return     none.
 */
_attribute_ram_code_sec_noinline_ void analog_write_buff(unsigned char addr, unsigned char *buff, unsigned char len);

/********************************************************************************************************
 *                                          internal
 *******************************************************************************************************/

/********************************************************************************************************
 *              this is only internal interface, customers do not need to care.
 *******************************************************************************************************/
#if ANALOG_TIMEOUT
/**
 * @brief      This function serves to judge whether analog Tx buffer is empty.
 * @return     0:not empty      1: empty
 */
_attribute_ram_code_sec_optimize_o2_noinline_  bool analog_txbuf_no_empty(void);


/**
 * @brief      This function serves to judge whether analog is busy.
 * @return     0: not busy  1:busy
 */
_attribute_ram_code_sec_optimize_o2_noinline_  bool analog_busy(void);


    /**
 * @brief      This function serves to judge whether analog write/read is busy .
 * @return     none.
 */
    #define analog_wait() wait_condition_fails_or_timeout(analog_busy, g_drv_api_error_timeout_us, drv_timeout_handler, (unsigned int)DRV_API_ERROR_TIMEOUT_ANALOG_WAIT)

    /**
 * @brief      This function serves to judge whether analog Tx buffer is empty.
 * @return     none.
 */
    #define analog_wait_txbuf_no_empty() wait_condition_fails_or_timeout(analog_txbuf_no_empty, g_drv_api_error_timeout_us, drv_timeout_handler, (unsigned int)DRV_API_ERROR_TIMEOUT_ANA_TX_BUFCNT)
#endif
