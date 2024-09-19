/********************************************************************************************************
 * @file    flash_base.h
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
#include "compiler.h"
#include "flash.h"


/**
 * @brief       This function serves to the interrupt protection of flash interface.
 * @return      none.
 * @note        _always_inline:the function is inlined to ensure security.
 */
_attribute_ram_code_sec_ void flash_reg_access_protect(void);

/**
 * @brief       This function serves to the interrupt restore of flash interface.
 * @return      none.
 * @note        _always_inline:the function is inlined to ensure security.
 */
_attribute_ram_code_sec_ void flash_reg_access_restore(void);

/**
 * @brief       This function reads or write the content from a page to the buf.
 * @param[in]   cmd         - the data fmt and cmd.
 * @param[in]   addr        - the access address of flash.
 * @param[out]  data        - the start address of the buffer.
 * @param[in]   data_len    - the length(in byte) of content needs to read out from the page.
 * @param[in]   is_is_encrypt   - whether to encrypt or decrypt.
 * @param[in]   mspi_wr     - Reading and writing to choose.
 * @param[in]   w_en_cmd    - the flash write enable cmd.
 * @param[in]   busy_cmd    - the flash read status cmd.
 * @return      none.
 * @note       _always_inline : make it harder to crack encrypted data, so this interface is not allowed to be modified.
 */
_attribute_ram_code_sec_ void flash_mspi_wr_ram(unsigned int cmd,unsigned long addr, unsigned char *data,unsigned long data_len,unsigned char is_encrypt,mspi_func_e mspi_wr,unsigned int w_en_cmd,unsigned int busy_cmd);

/**
 * @brief       This function reads the content from a page to the buf.
 * @param[in]   cmd     - the data fmt and cmd.
 * @param[in]   addr    - the access address of flash.
 * @param[out]  data    - the start address of the buffer.
 * @param[in]   data_len- the length(in byte) of content needs to read out from the page.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void flash_mspi_read_ram(unsigned int cmd,unsigned long addr, unsigned char *data,unsigned long data_len);

/**
 * @brief       This function serves to decrypt the read data from the flash at the specified address and compare it with the plain text in dual read mode.
 * @param[in]   cmd     - the data fmt and cmd.
 * @param[in]   addr    - the access address of flash.
 * @param[out]  data    - the start address of the plain buffer.
 * @param[in]   data_len- the length(in byte) of content needs to read out from the page.
 * @return      0: check pass; 1: check fail.
 * @note        the purpose the interface is all in ramcode : make it harder to crack encrypted data, so this interface is not allowed to be modified.
 */
_attribute_ram_code_sec_noinline_ unsigned char flash_mspi_read_decrypt_check_ram(unsigned int cmd,unsigned long addr, unsigned char *data,unsigned long data_len);

/**
 * @brief       This function serves to write write data to flash(include erase,write status).
 * @param[in]   cmd     - the flash cmd and mspi control.
 * @param[in]   addr    - the access address of flash.
 * @param[in]   data    - the buffer which stored the data you want to write to flash.
 * @param[in]   data_len- the length(in byte, must be above 0) you want to write.
 * @param[in]   w_en_cmd- the flash write enable cmd.
 * @param[in]   busy_cmd- the flash read status cmd.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void flash_mspi_write_ram(unsigned int cmd,unsigned long addr, unsigned char *data, unsigned long data_len,unsigned int w_en_cmd,unsigned int busy_cmd);

/**
 * @brief       This function serves to write write data to flash(include erase,write status) in encrypt mode.
 * @param[in]   cmd     - the flash cmd and mspi control.
 * @param[in]   addr    - the access address of flash.
 * @param[in]   data    - the buffer which stored the data you want to write to flash.
 * @param[in]   data_len- the length(in byte, must be above 0) you want to write.
 * @param[in]   w_en_cmd- the flash write enable cmd.
 * @param[in]   busy_cmd- the flash read status cmd.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void flash_mspi_write_encrypt_ram(unsigned int cmd,unsigned long addr, unsigned char *data, unsigned long data_len,unsigned int w_en_cmd,unsigned int busy_cmd);

/**
 * @brief       This function serves to set flash write command.This function interface is only used internally by flash,
 *              and is currently included in the H file for compatibility with other SDKs. When using this interface,
 *              please ensure that you understand the precautions of flash before using it.
 * @param[in]   cmd - set command.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void flash_send_cmd(unsigned int cmd);

/**
 * @brief       This function is used to update the read configuration parameters of xip(eXecute In Place),
 *              this configuration will affect the speed of MCU fetching,
 *              this parameter needs to be consistent with the corresponding parameters in the flash datasheet.
 * @param[in]   config  - xip configuration,reference structure flash_rd_xip_config_t
 * @return none
 */
_attribute_ram_code_sec_noinline_ void flash_set_rd_xip_config_sram(unsigned int config);

/**
 * @brief       This function is used to update the write configuration parameters of xip(eXecute In Place),
 *              this parameter needs to be consistent with the corresponding parameters in the flash datasheet.
 * @param[in]   config  - xip configuration,reference structure flash_wr_xip_config_t
 * @return none
 */
_attribute_ram_code_sec_noinline_ void flash_set_wr_xip_config_sram(flash_wr_xip_config_t config);

