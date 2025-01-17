/********************************************************************************************************
 * @file    flash_base.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#pragma once
#include "compiler.h"
#include "flash.h"
/**
 * @brief       This function reads the content from a page to the buf.
 * @param[in]   cmd       - the data fmt and cmd.
 * @param[in]   addr      - the start address of the page.
 * @param[out]  data      - the start address of the buffer.
 * @param[in]   data_len  - the length(in byte) of content needs to read out from the page.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void flash_mspi_read_ram(flash_command_e cmd, unsigned long addr, unsigned char *data, unsigned long data_len);


/**
 * @brief       This function reads the content from a page to the buf in in decrypt mode.
 * @param[in]   cmd       - the data fmt and cmd.
 * @param[in]   addr      - the start address of the page.
 * @param[out]  data      - the start address of the buffer.
 * @param[in]   data_len  - the length(in byte) of content needs to read out from the page.
 * @return      none.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ unsigned char flash_mspi_read_decrypt_check_ram(flash_command_e cmd, unsigned long addr, unsigned char *data, unsigned long data_len);
/**
 * @brief       This function serves to write write data to flash(include erase,write status).
 * @param[in]   cmd     - the flash cmd and mspi control.
 * @param[in]   addr    - the start address you want to write, use 0 if do not need address.
 * @param[in]   data     - the buffer which stored the data you want to write to flash.
 * @param[in]   data_len     - the length(in byte, must be above 0) you want to write.
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_ void flash_mspi_write_ram(flash_command_e cmd, unsigned long addr, unsigned char *data, unsigned long data_len);

/**
 * @brief       This function serves to write write data to flash(include erase,write status) in encrypt mode.
 * @param[in]   cmd     - the flash cmd and mspi control.
 * @param[in]   addr    - the start address you want to write, use 0 if do not need address.
 * @param[in]   data     - the buffer which stored the data you want to write to flash.
 * @param[in]   data_len     - the length(in byte, must be above 0) you want to write.
 * @return      none.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void flash_mspi_write_encrypt_ram(flash_command_e cmd, unsigned long addr, unsigned char *data, unsigned long data_len);


/**
 * @brief       This function serves to set flash write command.This function interface is only used internally by flash,
 *              and is currently included in the H file for compatibility with other SDKs. When using this interface,
 *              please ensure that you understand the precautions of flash before using it.
 * @param[in]   cmd - set command.
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_ void flash_send_cmd(unsigned char cmd);
