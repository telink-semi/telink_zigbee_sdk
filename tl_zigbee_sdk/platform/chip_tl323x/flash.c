/********************************************************************************************************
 * @file    flash.c
 *
 * @brief   This is the source file for tl323x
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "lib/include/sys.h"
#include "lib/include/flash_base.h"
#include "flash.h"

#include "lib/include/mspi.h"
#include "timer.h"
#include "lib/include/core.h"
#include "lib/include/stimer.h"
#include "types.h"

#include <string.h>

_attribute_data_retention_sec_ flash_handler_t flash_read_page  = flash_dread;
_attribute_data_retention_sec_ flash_handler_t flash_write_page = flash_page_program;
/*
 * note:flash_write_page_encrypt and flash_read_page_decrypt_check should be used in combination,and the check read address is corresponding to the encrypted write.
 */
_attribute_data_retention_sec_ flash_handler_t            flash_write_page_encrypt      = flash_page_program_encrypt;
_attribute_data_retention_sec_ flash_read_check_handler_t flash_read_page_decrypt_check = flash_dread_decrypt_check;


_attribute_data_retention_sec_ preempt_config_t s_flash_preempt_config =
    {
        .preempt_en = 0,
        .threshold  = 1,
};

/*******************************************************************************************************************
 *                                              Primary interface
 ******************************************************************************************************************/
/**
 * @brief       This function serves to set priority threshold. When the interrupt priority is greater than the maximum of the current interrupt threshold and the given interrupt threshold, flash process will disturb by interrupt.
 * @param[in]   preempt_en  - 1 can disturb by interrupt, 0 can disturb by interrupt.
 * @param[in]   threshold   - priority Threshold.
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
void flash_plic_preempt_config(unsigned char preempt_en, unsigned char threshold)
{
    s_flash_preempt_config.preempt_en = preempt_en;
    s_flash_preempt_config.threshold  = threshold;
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
_attribute_text_sec_ void flash_erase_sector(unsigned long addr)
{
    DISABLE_BTB;
    flash_mspi_write_ram(FLASH_SECT_ERASE_CMD, addr, 0, 0, FLASH_WRITE_ENABLE_CMD, FLASH_READ_STATUS_CMD_LOWBYTE);
    ENABLE_BTB;
}

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
_attribute_text_sec_ void flash_dread(unsigned long addr, unsigned long len, unsigned char *buf)
{
    DISABLE_BTB;
    flash_mspi_read_ram(FLASH_DREAD_CMD, addr, buf, len);
    ENABLE_BTB;
}

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
_attribute_text_sec_ void flash_4read(unsigned long addr, unsigned long len, unsigned char *buf)
{
    DISABLE_BTB;
    flash_mspi_read_ram(FLASH_X4READ_CMD, addr, buf, len);
    ENABLE_BTB;
}

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
_attribute_text_sec_ unsigned char flash_dread_decrypt_check(unsigned long addr, unsigned long plain_len, unsigned char *plain_buf)
{
    unsigned char check_data = 0;
    DISABLE_BTB;
    check_data = flash_mspi_read_decrypt_check_ram(FLASH_DREAD_CMD, addr, plain_buf, plain_len);
    ENABLE_BTB;
    return check_data;
}

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
_attribute_text_sec_ unsigned char flash_4read_decrypt_check(unsigned long addr, unsigned long plain_len, unsigned char *plain_buf)
{
    unsigned char check_data = 0;
    DISABLE_BTB;
    check_data = flash_mspi_read_decrypt_check_ram(FLASH_X4READ_CMD, addr, plain_buf, plain_len);
    ENABLE_BTB;
    return check_data;
}

/**
 * @brief       This function writes the buffer's content to the flash.
 * @param[in]   addr    - the access address of flash.
 * @param[in]   len     - the length(in byte, must be above 0) of content needs to write into the flash.
 * @param[in]   buf     - the start address of the content needs to write into(ram address).
 * @param[in]   cmd     - the write command. FLASH_WRITE_CMD or FLASH_QUAD_PAGE_PROGRAM_CMD.
 * @note        buf pointer passed in should be a sram addr, otherwise a data access error will occurred in spi transfer stage,
 *              because xip function was disabled during spi flash access.
 *              to avoid this issue, a sram moving operation was added if the buf pointer passed in is flash addr, and the stack cost was defined by STACK_SIZE_FOR_FLASH_DATA.
 *              once the passed buffer len is larger than STACK_SIZE_FOR_FLASH_DATA, the program stalled for user debug.
 * @return      none.
 */
_attribute_text_sec_ static void flash_write(unsigned long addr, unsigned long len, unsigned char *buf, flash_command_e cmd)
{
    unsigned int ns = PAGE_SIZE - (addr & (PAGE_SIZE - 1));
    int          nw = 0;
    unsigned int is_flash_addr = (((unsigned int)buf & FLASH_ADDR_MASK) == FLASH_ADDR_BASE) ? 1 : 0;

    while (len > 0) {
        nw = len > ns ? ns : len;

        if(1 == is_flash_addr)
        {
            if(nw > STACK_SIZE_FOR_FLASH_DATA) while(1);
            unsigned char const_buf[nw];
            
            memcpy(const_buf, buf, nw);
            DISABLE_BTB;
            flash_mspi_write_ram(cmd, addr, const_buf, nw, FLASH_WRITE_ENABLE_CMD, FLASH_READ_STATUS_CMD_LOWBYTE);
            ENABLE_BTB;
        }
        else
        {
            DISABLE_BTB;
            flash_mspi_write_ram(cmd, addr, buf, nw, FLASH_WRITE_ENABLE_CMD, FLASH_READ_STATUS_CMD_LOWBYTE);
            ENABLE_BTB;
        }

        ns = PAGE_SIZE;
        addr += nw;
        buf += nw;
        len -= nw;
    }
}

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
_attribute_text_sec_ void flash_page_program(unsigned long addr, unsigned long len, unsigned char *buf)
{
    flash_write(addr, len, buf, FLASH_WRITE_CMD);
}

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
_attribute_text_sec_ void flash_quad_page_program(unsigned long addr, unsigned long len, unsigned char *buf)
{
    flash_write(addr, len, buf, FLASH_QUAD_PAGE_PROGRAM_CMD);
}

/**
 * @brief       This function writes the buffer's content to the flash in encrypt mode.
 * @param[in]   addr    - the access address of flash.
 * @param[in]   len     - the length(in byte, must be above 0) of content needs to write into the flash.
 * @param[in]   buf     - the start address of the content needs to write into(ram address).
 * @param[in]   cmd     - the write command. FLASH_WRITE_CMD or FLASH_QUAD_PAGE_PROGRAM_CMD.
 * @return      none.
 */
_attribute_text_sec_ static void flash_write_encrypt(unsigned long addr, unsigned long len, unsigned char *buf, flash_command_e cmd)
{
    unsigned int ns = PAGE_SIZE - (addr & (PAGE_SIZE - 1));
    int          nw = 0;

    while (len > 0) {
        nw = len > ns ? ns : len;
        DISABLE_BTB;
        flash_mspi_write_encrypt_ram(cmd, addr, buf, nw, FLASH_WRITE_ENABLE_CMD, FLASH_READ_STATUS_CMD_LOWBYTE);
        ENABLE_BTB;
        ns = PAGE_SIZE;
        addr += nw;
        buf += nw;
        len -= nw;
    }
}

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
_attribute_text_sec_ void flash_page_program_encrypt(unsigned long addr, unsigned long len, unsigned char *buf)
{
    flash_write_encrypt(addr, len, buf, FLASH_WRITE_CMD);
}

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
_attribute_text_sec_ void flash_quad_page_program_encrypt(unsigned long addr, unsigned long len, unsigned char *buf)
{
    flash_write_encrypt(addr, len, buf, FLASH_QUAD_PAGE_PROGRAM_CMD);
}

/**
 * @brief       This function reads the status of flash.
 * @param[in]   cmd  - the cmd of read status. FLASH_READ_STATUS_CMD_LOWBYTE or FLASH_READ_STATUS_CMD_HIGHBYTE.
 * @return      the value of status.
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
_attribute_text_sec_ unsigned char flash_read_status(flash_command_e cmd)
{
    unsigned char status = 0;
    DISABLE_BTB;
    flash_mspi_read_ram(cmd, 0, &status, 1);
    ENABLE_BTB;
    return status;
}

/**
 * @brief       This function write the status of flash.
 * @param[in]   type    - the type of status.8 bit or 16 bit.
 * @param[in]   data    - the value of status.
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
_attribute_text_sec_ void flash_write_status(flash_status_typedef_e type, unsigned short data)
{
    unsigned char buf[2];

    buf[0] = data;
    buf[1] = data >> 8;
    DISABLE_BTB;
    if (type == FLASH_TYPE_8BIT_STATUS) {
        flash_mspi_write_ram(FLASH_WRITE_STATUS_CMD_LOWBYTE, 0, buf, 1, FLASH_WRITE_ENABLE_CMD, FLASH_READ_STATUS_CMD_LOWBYTE);
    } else if (type == FLASH_TYPE_16BIT_STATUS_ONE_CMD) {
        flash_mspi_write_ram(FLASH_WRITE_STATUS_CMD_LOWBYTE, 0, buf, 2, FLASH_WRITE_ENABLE_CMD, FLASH_READ_STATUS_CMD_LOWBYTE);
    } else if (type == FLASH_TYPE_16BIT_STATUS_TWO_CMD) {
        flash_mspi_write_ram(FLASH_WRITE_STATUS_CMD_LOWBYTE, 0, (unsigned char *)&buf[0], 1, FLASH_WRITE_ENABLE_CMD, FLASH_READ_STATUS_CMD_LOWBYTE);
        flash_mspi_write_ram(FLASH_WRITE_STATUS_CMD_HIGHBYTE, 0, (unsigned char *)&buf[1], 1, FLASH_WRITE_ENABLE_CMD, FLASH_READ_STATUS_CMD_LOWBYTE);
    }
    ENABLE_BTB;
}

/**
 * @brief       This function serves to read data from the Security Registers of the flash.
 * @param[in]   addr    - the start address of the Security Registers.
 * @param[in]   len     - the length(in byte, must be above 0) of the content to be read.
 * @param[out]  buf     - the starting address of the content to be read(ram address).
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
_attribute_text_sec_ void flash_read_otp(unsigned long addr, unsigned long len, unsigned char *buf)
{
    DISABLE_BTB;
    flash_mspi_read_ram(FLASH_READ_SECURITY_REGISTERS_CMD, addr, buf, len);
    ENABLE_BTB;
}

/**
 * @brief       This function serves to write data to the Security Registers of the flash you choose.
 * @param[in]   addr    - the start address of the Security Registers.
 * @param[in]   len     - the length(in byte, must be above 0) of content to be written.
 * @param[in]   buf     - the starting address of the content to be written(ram address).
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
_attribute_text_sec_ void flash_write_otp(unsigned long addr, unsigned long len, unsigned char *buf)
{
    unsigned int ns = PAGE_SIZE_OTP - (addr & (PAGE_SIZE_OTP - 1));
    int          nw = 0;

    while (len > 0) {
        nw = len > ns ? ns : len;
        DISABLE_BTB;
        flash_mspi_write_ram(FLASH_WRITE_SECURITY_REGISTERS_CMD, addr, buf, nw, FLASH_WRITE_ENABLE_CMD, FLASH_READ_STATUS_CMD_LOWBYTE);
        ENABLE_BTB;
        ns = PAGE_SIZE_OTP;
        addr += nw;
        buf += nw;
        len -= nw;
    }
}

/**
 * @brief       This function serves to erase the data of the Security Registers that you choose.
 * @param[in]   addr    - the address that you want to erase.
 * @return      none.
 * @Attention   Even you choose the middle area of the Security Registers,it will erase the whole area.
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
_attribute_text_sec_ void flash_erase_otp(unsigned long addr)
{
    DISABLE_BTB;
    flash_mspi_write_ram(FLASH_ERASE_SECURITY_REGISTERS_CMD, addr, 0, 0, FLASH_WRITE_ENABLE_CMD, FLASH_READ_STATUS_CMD_LOWBYTE);
    ENABLE_BTB;
}

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
_attribute_text_sec_ unsigned int flash_read_mid(void)
{
    unsigned int flash_mid = 0;
    DISABLE_BTB;
    flash_mspi_read_ram(FLASH_GET_JEDEC_ID, 0, (unsigned char *)&flash_mid, 3);
    ENABLE_BTB;
    return flash_mid;
}

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
_attribute_text_sec_ void flash_read_uid(unsigned char idcmd, unsigned char *buf)
{
    DISABLE_BTB;
    if (idcmd == ((FLASH_READ_UID_CMD_GD_PUYA_ZB_TH >> 24) & 0xff)) {
        flash_mspi_read_ram(FLASH_READ_UID_CMD_GD_PUYA_ZB_TH, 0, buf, 16);
    }
    ENABLE_BTB;
}

/**
 * @brief       This function is used to update the read configuration parameters of xip(eXecute In Place),
 *              this configuration will affect the speed of MCU fetching,
 *              this parameter needs to be consistent with the corresponding parameters in the flash datasheet.
 * @param[in]   config  - xip configuration,reference structure flash_rd_xip_config_t
 *              note:the config parameter can be flash_common_cmd/flash_rd_xip_config_t to know the definition of each bit of config.
 * @return none
 */
_attribute_text_sec_ void flash_set_rd_xip_config(unsigned int config)
{
    DISABLE_BTB;
    flash_set_rd_xip_config_sram(config);
    ENABLE_BTB;
}

/**
 * @brief       This function is used to update the write configuration parameters of xip(eXecute In Place),
 *              this parameter needs to be consistent with the corresponding parameters in the flash datasheet.
 * @param[in]   config  - xip configuration,reference structure flash_wr_xip_config_t
 * @return none
 */
_attribute_text_sec_ void flash_set_wr_xip_config(flash_wr_xip_config_t config)
{
    DISABLE_BTB;
    flash_set_wr_xip_config_sram(config);
    ENABLE_BTB;
}

/**
 * @brief       This function is used to write the configure of the flash.
 * @param[in]   addr    - the flash configure address.
 * @param[in]   cmd     - the write command.
 * @param[in]   data    - the start address of the data buffer.
 * @return      none.
 * @note        important:  "data" must not reside at flash, such as constant string.If that case, pls copy to memory first before write.
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
_attribute_text_sec_ void flash_write_config(unsigned long addr, unsigned int cmd, unsigned char data)
{
    DISABLE_BTB;
    flash_mspi_write_ram(cmd, addr, &data, 1, FLASH_WRITE_ENABLE_CMD, FLASH_READ_STATUS_CMD_LOWBYTE);
    ENABLE_BTB;
}

/**
 * @brief       This function is used to read the configure of the flash.
 * @param[in]   addr    - the flash configure address.
 * @param[in]   cmd     - the read command.
 * @return      the value of configure.
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
_attribute_text_sec_ unsigned char flash_read_config(unsigned long addr, unsigned int cmd)
{
    unsigned char config = 0;
    DISABLE_BTB;
    flash_mspi_read_ram(cmd, addr, &config, 1);
    ENABLE_BTB;
    return config;
}

/********************************************************************************************************
 *                                  secondary calling function,
 *  there is no need to add an circumvention solution to solve the problem of access flash conflicts.
 *******************************************************************************************************/

/**
 * @brief       This function serves to get flash vendor.
 * @param[in]   flash_mid - MID of the flash(4 bytes).
 * @return      0 - err, other - flash vendor.
 */
unsigned int flash_get_vendor(unsigned int flash_mid)
{
    switch (flash_mid & 0x0000ffff) {
    case 0x0000325E:
        return FLASH_ETOX_ZB;
    case 0x000060C8:
        return FLASH_ETOX_GD;
    case 0x00004051:
        return FLASH_ETOX_GD;
    case 0x00006085:
        return FLASH_SONOS_PUYA;
    case 0x000060EB:
        return FLASH_SONOS_TH;
    case 0x000060CD:
        return FLASH_SST_TH;
    default:
        return 0;
    }
}

/**
 * @brief       This function serves to get flash capacity.
 * @param[in]   flash_mid - MID of the flash(4 bytes).
 * @return      flash capacity.
 */
flash_capacity_e flash_get_capacity(unsigned int flash_mid)
{
    return (flash_mid & 0x00ff0000) >> 16;
}
