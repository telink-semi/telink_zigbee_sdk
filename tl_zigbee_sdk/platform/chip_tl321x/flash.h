/********************************************************************************************************
 * @file    flash.h
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
/** @page FLASH
 *
 *  Introduction
 *  ===============
 *  supports basic flash functions
 *
 *  API Reference
 *  ===============
 *  Header File: flash.h
 *
 *  Attention
 *  ==============
 *  -# the description of parameters addr contained on the interface:
 *      - It is not necessary to add the base address 0x20000000, which ranges from 0 to the size of the flash storage space.
 *  -# by default, the flash functions that call sub-functions (all declared in flash_base.h) cannot be interrupted by interrupts,
 *     if the function needs to be interrupted by a high priority interrupt during execution refer to flash_plic_preempt_config().
 * 
 */
#pragma once

#include "lib/include/mspi.h"
#include "compiler.h"

#define PAGE_SIZE       256
#define PAGE_SIZE_OTP   256

/**
 * @brief     flash command definition.
 * |   --        |   --        |     --        |    --    |    --    |
 * | <31:24>     |    <18>     | <15:12>       |  <11:8>  |   <7:0>  |
 * |   command   |  token_en   | transfer mode |   dummy  |   format |
 */
typedef enum{
    //The command called by the flash_mspi_read_ram() function.
/*
 * FLASH_DREAD_CMD/FLASH_X4READ_CMD/FLASH_X8READ_CMD,bit[22] Set to 1:
 * the configuration of flash xip in the S file refers to the read cmd here, bit[22] is invalid in the interface configuration;
 * In xip configuration, bit[22] is timeout enable (the function of this bit: when xip reaches a certain time without taking instructions,
 * cs will not be pulled down, saving power consumption).(added by shuaixing.zhai 20240428)
 * attention: The maximum frequency of some flash single line reads may be smaller than the mspi frequency configured by the chip, it is not recommended to use the function of single line reads,
 * if you have to use it, please refer to the flash datasheet to ensure that the maximum frequency of flash single line reads is larger than the mspi frequency configured by the chip.(added by xiaobin.huang 20240717)
 */
    FLASH_DREAD_CMD                            = 0x3b4097a9,
    FLASH_X4READ_CMD                           = 0xeb4493ba,
    FLASH_READ_SECURITY_REGISTERS_CMD          = 0x480097a8,
    FLASH_READ_UID_CMD_GD_PUYA_ZB_TH           = 0x4b0097a8,
    FLASH_GET_JEDEC_ID                         = 0x9f002080,
    FLASH_READ_STATUS_CMD_LOWBYTE              = 0x05002080,
    FLASH_READ_STATUS_CMD_HIGHBYTE             = 0x35002080,
    FLASH_READ_CONFIGURE_CMD                   = 0x15002080,
    //The command called by the flash_mspi_write_ram() function.
    FLASH_WRITE_CMD                            = 0x020010a8,
    FLASH_QUAD_PAGE_PROGRAM_CMD                = 0x320010aa,
    FLASH_SECT_ERASE_CMD                       = 0x200070a8,
    FLASH_WRITE_SECURITY_REGISTERS_CMD         = 0x420010a8,
    FLASH_ERASE_SECURITY_REGISTERS_CMD         = 0x440070a8,
    FLASH_WRITE_STATUS_CMD_LOWBYTE             = 0x01001080,
    FLASH_WRITE_STATUS_CMD_HIGHBYTE            = 0x31001080,    // Flash Type = P25Q16SU/P25Q32SU for write status register-1;
    FLASH_WRITE_CONFIGURE_CMD_1                = 0x31001080,    // Flash Type = P25Q80U  for write configure register;
    FLASH_WRITE_CONFIGURE_CMD_2                = 0x11001080,    // Flash Type = P25Q16SU/P25Q32SU  for write configure register;
    // other command called by flash_send_cmd() function
    FLASH_WRITE_DISABLE_CMD                     = 0x04007080,
    FLASH_WRITE_ENABLE_CMD                      = 0x06007080,
    FLASH_WRITE_DEEP_CMD                        = 0xb9007080,
    FLASH_WRITE_RELEASE_CMD                     = 0xab007080,
}flash_command_e;

/**
 * @brief     flash status type definition
 */
typedef enum{
    FLASH_TYPE_8BIT_STATUS              = 0,
    FLASH_TYPE_16BIT_STATUS_ONE_CMD     = 1,
    FLASH_TYPE_16BIT_STATUS_TWO_CMD     = 2,
}flash_status_typedef_e;

/**
 * @brief     flash vendor and technology definition
 */
typedef enum{
    FLASH_ETOX_ZB       = 0x0100325E,   // 325E     bit[24]:ETOX: Byte Program Time != Page Programming Time
    FLASH_ETOX_GD       = 0x010060C8,   // 60C8/4051
    FLASH_SONOS_PUYA    = 0x02006085,   // 6085     bit[25]:SONOS:Byte Program Time == Page Programming Time
    FLASH_SONOS_TH      = 0x020060EB,   // 60EB
    FLASH_SST_TH        = 0x040060CD,   // 60CD     bit[26]:SST:  Byte Program Time != Page Programming Time
}flash_vendor_e;

/**
 * @brief   flash capacity definition
 *          Call flash_read_mid function to get the size of flash capacity.
 *          Example is as follows:
 *          unsigned int mid = flash_read_mid();
 *          The value of (mid&0x00ff0000)>>16 reflects flash capacity.
 * @note    If there is a new flash with different capacity, you need to add the corresponding calibration interface to user_read_flash_value_calib().
 */
typedef enum {
    FLASH_SIZE_64K     = 0x10,
    FLASH_SIZE_128K    = 0x11,
    FLASH_SIZE_256K    = 0x12,
    FLASH_SIZE_512K    = 0x13,
    FLASH_SIZE_1M      = 0x14,
    FLASH_SIZE_2M      = 0x15,
    FLASH_SIZE_4M      = 0x16,
    FLASH_SIZE_8M      = 0x17,
    FLASH_SIZE_16M     = 0x18,
}flash_capacity_e;

/**
 * @brief     flash mid definition
 */
typedef enum{
    MID146085   =   0x146085,//P25Q80SU
    MID136085   =   0x136085,//P25Q40SU
    MID156085   =   0x156085,//P25Q16SU
    MID166085   =   0x166085,//P25Q32SU
}flash_mid_e;


typedef struct{
    unsigned int  flash_xip_rd_dual:1;          /**< xip mspi dual io mode */
    unsigned int  flash_xip_rd_quad:1;          /**< xip mspi quad io mode */
    unsigned int  flash_xip_rd_addr_len:2;      /**< xip read addr len;  2'b00 1byte  2'b01 2byte  2'b10 3byte  2'b11 4byte*/
    unsigned int  flash_xip_rd_addr_fmt:1;
    unsigned int  flash_xip_rd_addr_en:1;
    unsigned int  flash_xip_rd_cmd_fmt:1;
    unsigned int  flash_xip_rd_cmd_en:1;

    unsigned int  flash_xip_rd_dummy_cnt:4; /**< dummy cycle = flash_xip_rd_dummy_cnt + 1 */
    unsigned int  flash_xip_rd_transmode:4;

    unsigned int  flash_xip_cmd1_en:1;
    unsigned int  flash_xip_token_val_sel:1;
    unsigned int  flash_xip_token_en:1;
    unsigned int  flash_xip_rd_dummy_cnt_add:1;
    unsigned int  flash_reserve:1;
    unsigned int  flash_xip_page_mode_en:1;
    unsigned int  flash_xip_timeout_mode_en:1;
    unsigned int  flash_xip_tcem_mode_en:1;
    unsigned int  flash_xip_read_cmd:8;         /**< xip read command */

}flash_rd_xip_config_t;

typedef struct{
    unsigned int  flash_xip_wr_dual:1;          /**< xip mspi dual io mode */
    unsigned int  flash_xip_wr_quad:1;          /**< xip mspi quad io mode */
    unsigned int  flash_xip_wr_addr_len:2;      /**< xip read addr len;  2'b00 1byte  2'b01 2byte  2'b10 3byte  2'b11 4byte*/
    unsigned int  flash_xip_wr_addr_fmt:1;
    unsigned int  flash_xip_wr_addr_en:1;
    unsigned int  flash_xip_wr_cmd_fmt:1;
    unsigned int  flash_xip_wr_cmd_en:1;

    unsigned int  flash_xip_wr_dummy_cnt:4; /**< dummy cycle = flash_xip_rd_dummy_cnt + 1 */
    unsigned int  flash_xip_wr_transmode:4;

    unsigned int  flash_xip_cmd1_en:8;
    unsigned int  flash_xip_write_cmd:8;            /**< xip read command */

}flash_wr_xip_config_t;



typedef void (*flash_handler_t)(unsigned long, unsigned long, unsigned char*);
typedef unsigned char (*flash_read_check_handler_t)(unsigned long, unsigned long, unsigned char*);
extern _attribute_data_retention_sec_ flash_handler_t flash_read_page;
extern _attribute_data_retention_sec_ flash_handler_t flash_write_page;
extern _attribute_data_retention_sec_ flash_handler_t flash_write_page_encrypt;
extern _attribute_data_retention_sec_ flash_read_check_handler_t flash_read_page_decrypt_check;


/*******************************************************************************************************************
 *                                              Primary interface
 ******************************************************************************************************************/

/**
 * @brief       This function serve to change the read function and write function.
 * @param[in]   read    - the read function.
 * @param[in]   write   - the write function.
 * @none
 */
static _always_inline void flash_change_rw_func(flash_handler_t read, flash_handler_t write)
{
    flash_read_page = read;
    flash_write_page = write;
}

/**
 * @brief       This function serves to erase a sector.
 * @param[in]   addr    - the access address of flash(must be 0 or a multiple of 0x1000).
 * @return      none.
 * @note        Attention: The block erase takes a long time, please pay attention to feeding the dog in advance.
 *              The maximum block erase time is listed at the beginning of this document and is available for viewing.
 *
 *              Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
_attribute_text_sec_ void flash_erase_sector(unsigned long addr);

/**
 * @brief       This function reads the content from a page to the buf with dual read mode.
 * @param[in]   addr    - the access address of flash.
 * @param[in]   len     - the length(in byte, must be above 0) of content needs to read out from the page.
 * @param[out]  buf     - the start address of the buffer(ram address).
 * @return      none.
 * @note        cmd:1x, addr:1x, data:2x, dummy:8
 *              Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
_attribute_text_sec_ void flash_dread(unsigned long addr, unsigned long len, unsigned char *buf);

/**
 * @brief       This function reads the content from a page to the buf with 4*IO read mode.
 * @param[in]   addr    - the access address of flash.
 * @param[in]   len     - the length(in byte, must be above 0) of content needs to read out from the page.
 * @param[out]  buf     - the start address of the buffer(ram address).
 * @return      none.
 * @note        cmd:1x, addr:4x, data:4x, dummy:6
 *              Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
_attribute_text_sec_ void flash_4read(unsigned long addr, unsigned long len, unsigned char *buf);

/**
 * @brief       This function serves to decrypt the read data from the flash at the specified address and compare it with the plain text in dual read mode.
 * @param[in]   addr        - the access address of flash.
 * @param[in]   plain_len   - the length(in byte, must be above 0) of content needs to read out from the page.
 * @param[out]  plain_buf   - the start address of the plain buffer(ram address).
 * @return      0: check pass; 1: check fail.
 * @note        cmd:1x, addr:1x, data:2x, dummy:8
 *              Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
_attribute_text_sec_ unsigned char flash_dread_decrypt_check(unsigned long addr,unsigned long plain_len,unsigned char* plain_buf);

/**
 * @brief       This function serves to decrypt the read data from the flash at the specified address and compare it with the plain text in 4*IO read mode.
 * @param[in]   addr        - the access address of flash.
 * @param[in]   plain_len   - the length(in byte, must be above 0) of content needs to read out from the page.
 * @param[out]  plain_buf   - the start address of the plain buffer(ram address).
 * @return      0: check pass; 1: check fail.
 * @note        cmd:1x, addr:4x, data:4x, dummy:6
 *              Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
_attribute_text_sec_ unsigned char flash_4read_decrypt_check(unsigned long addr,unsigned long plain_len,unsigned char* plain_buf);

/**
 * @brief       This function writes the buffer's content to the flash with single mode.It is necessary to confirm whether this area
 *              has been erased before writing. If not, you need to erase and then write, otherwise the write may be unsuccessful
 *              and the data will become the wrong value. Note that when erasing, the minimum is erased by sector (4k bytes).
 *              Do not erase the useful information in other locations of the sector during erasing.
 * @param[in]   addr    - the access address of flash.
 * @param[in]   len     - the length(in byte, must be above 0) of content needs to write into the flash.
 * @param[in]   buf     - the start address of the content needs to write into(ram address).
 * @return      none.
 * @note        cmd:1x, addr:1x, data:1x
 *              the function support cross-page writing,which means the len of buf can bigger than 256.
 *
 *              Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
_attribute_text_sec_ void flash_page_program(unsigned long addr, unsigned long len, unsigned char *buf);

/**
 * @brief       This function writes the buffer's content to the flash with quad page program mode.It is necessary to confirm whether this area
 *              has been erased before writing. If not, you need to erase and then write, otherwise the write may be unsuccessful
 *              and the data will become the wrong value. Note that when erasing, the minimum is erased by sector (4k bytes).
 *              Do not erase the useful information in other locations of the sector during erasing.
 * @param[in]   addr    - the access address of flash.
 * @param[in]   len     - the length(in byte, must be above 0) of content needs to write into the flash.
 * @param[in]   buf     - the start address of the content needs to write into(ram address).
 * @return      none.
 * @note        cmd:1x, addr:1x, data:4x
 *              the function support cross-page writing,which means the len of buf can bigger than 256.
 *
 *              Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
_attribute_text_sec_ void flash_quad_page_program(unsigned long addr, unsigned long len, unsigned char *buf);

/**
 * @brief       This function writes the buffer's content to the flash with single mode in encrypt mode.It is necessary to confirm whether this area
 *              has been erased before writing. If not, you need to erase and then write, otherwise the write may be unsuccessful
 *              and the data will become the wrong value. Note that when erasing, the minimum is erased by sector (4k bytes).
 *              Do not erase the useful information in other locations of the sector during erasing.
 * @param[in]   addr    - the access address of flash.
 * @param[in]   len     - the length(in byte, must be above 0) of content needs to write into the flash.
 * @param[in]   buf     - the start address of the content needs to write into(ram address).
 * @return      none.
 * @note        cmd:1x, addr:1x, data:1x
 *              the function support cross-page writing,which means the len of buf can bigger than 256.
 *
 *              Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
_attribute_text_sec_ void flash_page_program_encrypt(unsigned long addr, unsigned long len, unsigned char *buf);

/**
 * @brief       This function writes the buffer's content to the flash with quad page program mode in encrypt mode.It is necessary to confirm whether this area
 *              has been erased before writing. If not, you need to erase and then write, otherwise the write may be unsuccessful
 *              and the data will become the wrong value. Note that when erasing, the minimum is erased by sector (4k bytes).
 *              Do not erase the useful information in other locations of the sector during erasing.
 * @param[in]   addr    - the access address of flash.
 * @param[in]   len     - the length(in byte, must be above 0) of content needs to write into the flash.
 * @param[in]   buf     - the start address of the content needs to write into(ram address).
 * @return      none.
 * @note        cmd:1x, addr:1x, data:4x
 *              the function support cross-page writing,which means the len of buf can bigger than 256.
 *
 *              Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
_attribute_text_sec_ void flash_quad_page_program_encrypt(unsigned long addr, unsigned long len, unsigned char *buf);

/**
 * @brief       This function serves to read MID of flash(MAC id). Before reading UID of flash,
 *              you must read MID of flash. and then you can look up the related table to select
 *              the idcmd and read UID of flash.
 * @return      MID of the flash.
 * @note        Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
_attribute_text_sec_ unsigned int flash_read_mid(void);

/**
 * @brief       This function serves to read UID of flash.Before reading UID of flash, you must read MID of flash.
 *              and then you can look up the related table to select the idcmd and read UID of flash.
 * @param[in]   idcmd   - different flash vendor have different read-uid command. E.g: GD/PUYA:0x4B; XTX: 0x5A.
 * @param[in]   buf     - store UID of flash.
 * @return      none.
 * @note        Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
_attribute_text_sec_ void flash_read_uid(unsigned char idcmd, unsigned char *buf);

/**
 * @brief       This function serves to set priority threshold. When the interrupt priority is greater than
 *              the maximum of the current interrupt threshold and the given interrupt threshold, flash process will disturb by interrupt.
 * @param[in]   preempt_en  - 1 can disturb by interrupt, 0 can disturb by interrupt.
 * @param[in]   threshold   - priority Threshold, .
 * @return      none.
 * @note
 *              -# The correlation between flash_plic_preempt_config() and the flash functions that call sub-functions (all declared in flash_base.h) is as follows:
 *                  - When preempt_en = 1 and interrupt nesting is enabled (plic_preempt_feature_en):
 *                      - During the flash functions execution, the threshold of the PLIC is set to the maximum of the threshold before calling the interface and the given threshold value. \n
 *                        This means that when the external interrupt priority is greater than this maximum value, the execution of the flash function is disturbed by this interrupt.
 *                      - machine timer and software interrupt will definitely interrupt the flash functions execution, they are not controlled by the plic interrupt threshold
 *                  - In other cases(preempt_en = 0 or plic_preempt_feature_en = 0), global interrupts (including machine timer and software interrupt) will be turned off during the execution of the flash functions and will be restored when the flash functions exits.
 *              -# If the flash operation may be interrupted by an interrupt, it is necessary to ensure that the interrupt handling function and the function it calls must be in the RAM code. 
 */
_attribute_text_sec_ void flash_plic_preempt_config(unsigned char preempt_en, unsigned char threshold);

/**
 * @brief       This function is used to update the read configuration parameters of xip(eXecute In Place),
 *              this configuration will affect the speed of MCU fetching,
 *              this parameter needs to be consistent with the corresponding parameters in the flash datasheet.
 * @param[in]   config  - xip configuration,reference structure flash_rd_xip_config_t
 *              note:the config parameter can be flash_common_cmd/flash_rd_xip_config_t to know the definition of each bit of config.
 * @return none
 */
_attribute_text_sec_ void flash_set_rd_xip_config(unsigned int config);

/**
 * @brief       This function is used to update the write configuration parameters of xip(eXecute In Place),
 *              this parameter needs to be consistent with the corresponding parameters in the flash datasheet.
 * @param[in]   config  - xip configuration,reference structure flash_wr_xip_config_t
 * @return none
 */
_attribute_text_sec_ void flash_set_wr_xip_config(flash_wr_xip_config_t config);

/*******************************************************************************************************************
 *                                              Secondary interface
 ******************************************************************************************************************/

/**
 * @brief       This function serves to get flash vendor.
 * @param[in]   flash_mid - MID of the flash(4 bytes).
 * @return      0 - err, other - flash vendor.
 */
unsigned int flash_get_vendor(unsigned int flash_mid);

/**
 * @brief       This function serves to get flash capacity.
 * @param[in]   flash_mid - MID of the flash(4 bytes).
 * @return      flash capacity.
 */
flash_capacity_e  flash_get_capacity(unsigned int flash_mid);
