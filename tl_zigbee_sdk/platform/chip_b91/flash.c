/********************************************************************************************************
 * @file    flash.c
 *
 * @brief   This is the source file for B91
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "lib/include/plic.h"
#include "lib/include/sys.h"
#include "flash.h"
#include "mspi.h"
#include "timer.h"
#include "core.h"
#include "stimer.h"
#include "string.h"

_attribute_data_retention_sec_ flash_handler_t flash_read_page  = flash_dread;
_attribute_data_retention_sec_ flash_handler_t flash_write_page = flash_page_program;

_attribute_data_retention_sec_ static preempt_config_t s_flash_preempt_config =
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
 *              -# The correlation between flash_plic_preempt_config() and the flash functions that call sub-functions(flash_mspi_read_ram/flash_mspi_write_ram) is as follows:
 *                  - When preempt_en = 1 and interrupt nesting is enabled (plic_preempt_feature_en):
 *                      - During the flash functions execution, the threshold of the PLIC is set to the maximum of the threshold before calling the interface and the given threshold value. \n
 *                        This means that when the external interrupt priority is greater than this maximum value, the execution of the flash function is disturbed by this interrupt.
 *                      - machine timer and software interrupt will definitely interrupt the flash functions execution, they are not controlled by the plic interrupt threshold
 *                  - In other cases(preempt_en = 0 or plic_preempt_feature_en = 0), global interrupts (including machine timer and software interrupt) will be turned off during the execution of the flash functions and will be restored when the flash functions exits.
 *              -# If the flash operation may be interrupted by an interrupt, it is necessary to ensure that the interrupt handling function and the function it calls must be in the RAM code. 
 * 
 */
void flash_plic_preempt_config(unsigned char preempt_en, unsigned char threshold)
{
    s_flash_preempt_config.preempt_en = preempt_en;
    s_flash_preempt_config.threshold  = threshold;
}

/*******************************************************************************************************************
 *                              Functions for internal use in flash,
 *      There is no need to add an evasion solution to solve the problem of access flash conflicts.
 ******************************************************************************************************************/
/**
 * @brief       This function to determine whether the flash is busy..
 * @return      1:Indicates that the flash is busy. 0:Indicates that the flash is free
 */
_attribute_ram_code_sec_ static inline int flash_is_busy(void)
{
    return mspi_read() & 0x01; //the busy bit, pls check flash spec
}

/**
 * @brief       This function serves to set flash write command.This function interface is only used internally by flash,
 *              and is currently included in the H file for compatibility with other SDKs. When using this interface,
 *              please ensure that you understand the precautions of flash before using it.
 * @param[in]   cmd - set command.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void flash_send_cmd(flash_command_e cmd)
{
    mspi_high();
    CLOCK_DLY_10_CYC;
    mspi_low();
    mspi_write(cmd);
    mspi_wait();
}

/**
 * @brief       This function serves to send flash address.
 * @param[in]   addr    - the flash address.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ static void flash_send_addr(unsigned int addr)
{
    mspi_write((unsigned char)(addr >> 16));
    mspi_wait();
    mspi_write((unsigned char)(addr >> 8));
    mspi_wait();
    mspi_write((unsigned char)(addr));
    mspi_wait();
}

/**
 * @brief     This function serves to wait flash done.(make this a asynchronous version).
 * @return    none.
 */
_attribute_ram_code_sec_noinline_ static void flash_wait_done(void)
{
    flash_send_cmd(FLASH_READ_STATUS_CMD_LOWBYTE);

    int i;
    for (i = 0; i < 10000000; ++i) {
        if (!flash_is_busy()) {
            break;
        }
    }
    mspi_high();
}

/*******************************************************************************************************************
 *      It is necessary to add an evasion plan to solve the problem of access flash conflict.
 ******************************************************************************************************************/

/**
 * @brief       This function is used to read data from flash or read the status of flash.
 * @param[in]   cmd         - the read command.
 * @param[in]   addr        - starting address.
 * @param[in]   addr_en     - whether need to send an address.
 * @param[in]   dummy_cnt   - the length(in byte) of dummy.
 * @param[out]  data        - the start address of the data buffer(ram address).
 * @param[in]   data_len    - the length(in byte, must be above 0) of content needs to read out.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void flash_mspi_read_ram(flash_command_e cmd, unsigned long addr, unsigned char addr_en, unsigned char dummy_cnt, unsigned char *data, unsigned long data_len)
{
    unsigned int r = plic_enter_critical_sec(s_flash_preempt_config.preempt_en, s_flash_preempt_config.threshold);
    mspi_stop_xip();
    /*Flash single line mode does not distinguish between mspi read and mspi write.This is just to be compatible with single line, dual line and quad line.*/
    mspi_fm_write_en(); /* write mode */
    mspi_fm_data_line(MSPI_SINGLE_LINE);
    flash_send_cmd(cmd);
    if (cmd == FLASH_X4READ_CMD) {
        mspi_fm_data_line(MSPI_QUAD_LINE);
    }
    if (addr_en) {
        flash_send_addr(addr);
    }
    for (unsigned int i = 0; i < dummy_cnt; ++i) {
        mspi_write(0x00); /* dummy */
        mspi_wait();
    }
    if (cmd == FLASH_DREAD_CMD) {
        mspi_fm_data_line(MSPI_DUAL_LINE);
    }
    mspi_fm_read_en();    /* read mode */
    mspi_write(0x00);     /* to issue clock */
    mspi_wait();
    mspi_fm_rd_trig_en(); /* auto mode, mspi_get() automatically triggers mspi_write(0x00) once. */
    mspi_wait();
    for (unsigned int i = 0; i < data_len; ++i) {
        *data++ = mspi_get();
        mspi_wait();
    }
    mspi_fm_rd_trig_dis(); /* off read auto mode */
    if ((cmd == FLASH_X4READ_CMD) || (cmd == FLASH_DREAD_CMD)) {
        mspi_fm_data_line(MSPI_SINGLE_LINE);
    }
    mspi_high();

    CLOCK_DLY_5_CYC;
    plic_exit_critical_sec(s_flash_preempt_config.preempt_en, r);
}

/**
 * @brief       This function is used to write data or status to flash.
 * @param[in]   cmd         - the write command.
 * @param[in]   addr        - starting address.
 * @param[in]   addr_en     - whether need to send an address.
 * @param[out]  data        - the start address of the data buffer(ram address).
 * @param[in]   data_len    - the length(in byte, must be above 0) of content needs to be written.
 * @return      none.
 * @note        important:  "data" must not reside at flash, such as constant string.If that case, pls copy to memory first before write.
 */
_attribute_ram_code_sec_noinline_ void flash_mspi_write_ram(flash_command_e cmd, unsigned long addr, unsigned char addr_en, unsigned char *data, unsigned long data_len)
{
    unsigned int r = plic_enter_critical_sec(s_flash_preempt_config.preempt_en, s_flash_preempt_config.threshold);
    mspi_stop_xip();
    /*Flash single line mode does not distinguish between mspi read and mspi write.This is just to be compatible with single line, dual line and quad line.*/
    mspi_fm_write_en(); /* write mode */
    mspi_fm_data_line(MSPI_SINGLE_LINE);
    flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
    flash_send_cmd(cmd);
    if (addr_en) {
        flash_send_addr(addr);
    }
    if (cmd == FLASH_QUAD_PAGE_PROGRAM_CMD) {
        mspi_fm_data_line(MSPI_QUAD_LINE);
    }
    for (unsigned int i = 0; i < data_len; ++i) {
        mspi_write(data[i]);
        mspi_wait();
    }
    mspi_high();
    if (cmd == FLASH_QUAD_PAGE_PROGRAM_CMD) {
        mspi_fm_data_line(MSPI_SINGLE_LINE);
    }
    flash_wait_done();

    CLOCK_DLY_5_CYC;
    plic_exit_critical_sec(s_flash_preempt_config.preempt_en, r);
}

/**
 * @brief       This function serves to erase a sector.
 * @param[in]   addr    - must be 0 or a multiple of 0x1000.
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
    flash_mspi_write_ram(FLASH_SECT_ERASE_CMD, addr, 1, NULL, 0);
    ENABLE_BTB;
}

/**
 * @brief         This function serves to erase a block of 32k.
 * @param[in]   addr    - must be 0 or a multiple of 0x8000.
 * @return         none.
 * @note        Attention: The block erase takes a long time, please pay attention to feeding the dog in advance.
 *                 The maximum block erase time is listed at the beginning of this document and is available for viewing.
 *
 *                 Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
_attribute_text_sec_ void flash_erase_block_32k(unsigned long addr)
{
    DISABLE_BTB;
    flash_mspi_write_ram(FLASH_32K_BLOCK_ERASE_CMD, addr, 1, NULL, 0);
    ENABLE_BTB;
}

/**
 * @brief       This function serves to erase a block of 64k.
 * @param[in]   addr    - must be 0 or a multiple of 0x10000.
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
_attribute_text_sec_ void flash_erase_block_64k(unsigned long addr)
{
    DISABLE_BTB;
    flash_mspi_write_ram(FLASH_64K_BLOCK_ERASE_CMD, addr, 1, NULL, 0);
    ENABLE_BTB;
}

/**
 * @brief       This function reads the content from a page to the buf with dual read mode.
 * @param[in]   addr    - the start address of the page.
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
    flash_mspi_read_ram(FLASH_DREAD_CMD, addr, 1, 1, buf, len);
    ENABLE_BTB;
}

/**
 * @brief       This function reads the content from a page to the buf with 4*IO read mode.
 * @param[in]   addr    - the start address of the page.
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
    flash_mspi_read_ram(FLASH_X4READ_CMD, addr, 1, 3, buf, len);
    ENABLE_BTB;
}

/**
 * @brief       This function writes the buffer's content to the flash.
 * @param[in]   addr    - the start address of the area.
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
            flash_mspi_write_ram(cmd, addr, 1, const_buf, nw);
            ENABLE_BTB;
        }
        else
        {
            DISABLE_BTB;
            flash_mspi_write_ram(cmd, addr, 1, buf, nw);
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
 * @param[in]   addr    - the start address of the area.
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
 * @param[in]   addr    - the start address of the area.
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
 * @brief       This function reads the status of flash.
 * @param[in]   cmd - the cmd of read status. FLASH_READ_STATUS_CMD_LOWBYTE or FLASH_READ_STATUS_CMD_HIGHBYTE.
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
    flash_mspi_read_ram(cmd, 0, 0, 0, &status, 1);
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
        flash_mspi_write_ram(FLASH_WRITE_STATUS_CMD_LOWBYTE, 0, 0, buf, 1);
    } else if (type == FLASH_TYPE_16BIT_STATUS_ONE_CMD) {
        flash_mspi_write_ram(FLASH_WRITE_STATUS_CMD_LOWBYTE, 0, 0, buf, 2);
    } else if (type == FLASH_TYPE_16BIT_STATUS_TWO_CMD) {
        flash_mspi_write_ram(FLASH_WRITE_STATUS_CMD_LOWBYTE, 0, 0, (unsigned char *)&buf[0], 1);
        flash_mspi_write_ram(FLASH_WRITE_STATUS_CMD_HIGHBYTE, 0, 0, (unsigned char *)&buf[1], 1);
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
    flash_mspi_read_ram(FLASH_READ_SECURITY_REGISTERS_CMD, addr, 1, 1, buf, len);
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
        flash_mspi_write_ram(FLASH_WRITE_SECURITY_REGISTERS_CMD, addr, 1, buf, nw);
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
    flash_mspi_write_ram(FLASH_ERASE_SECURITY_REGISTERS_CMD, addr, 1, NULL, 0);
    ENABLE_BTB;
}

/**
 * @brief       This function serves to read MID of flash(MAC id). Before reading UID of flash,
 *              you must read MID of flash. and then you can look up the related table to select
 *              the idcmd and read UID of flash.
 * @return      MID of the flash(4 bytes).
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
    flash_mspi_read_ram(FLASH_GET_JEDEC_ID, 0, 0, 0, (unsigned char *)(&flash_mid), 3);
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
    if (idcmd == FLASH_READ_UID_CMD_GD_PUYA_ZB_TH) //< GD/PUYA/ZB/TH
    {
        flash_mspi_read_ram(idcmd, 0, 0, 4, buf, 16);
    }
    ENABLE_BTB;
}

/**
 * @brief       This function is used to update the configuration parameters of xip(eXecute In Place),
 *              this configuration will affect the speed of MCU fetching,
 *              this parameter needs to be consistent with the corresponding parameters in the flash datasheet.
 * @param[in]   config  - xip configuration,reference structure flash_xip_config_t
 * @return none
 */
_attribute_ram_code_sec_noinline_ void flash_set_xip_config_sram(flash_xip_config_e config)
{
    unsigned int r = plic_enter_critical_sec(s_flash_preempt_config.preempt_en, s_flash_preempt_config.threshold);

    mspi_stop_xip();
    reg_mspi_xip_config = config;
    CLOCK_DLY_5_CYC;

    plic_exit_critical_sec(s_flash_preempt_config.preempt_en, r);
}

_attribute_text_sec_ void flash_set_xip_config(flash_xip_config_e config)
{
    DISABLE_BTB;
    flash_set_xip_config_sram(config);
    ENABLE_BTB;
}

/**
 * @brief       This function is used to write the configure of the flash,P25Q16SU/P25Q32SU/PY25Q128H uses this function.
 * @param[in]   cmd         - the write command.
 * @param[out]  data        - the start address of the data buffer.
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
_attribute_text_sec_ void flash_write_config(flash_command_e cmd, unsigned char data)
{
    DISABLE_BTB;
    flash_mspi_write_ram(cmd, 0, 0, &data, 1);
    ENABLE_BTB;
}

/**
 * @brief       This function is used to read the configure of the flash,P25Q16SU/P25Q32SU/PY25Q128H uses this function.
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
_attribute_text_sec_ unsigned char flash_read_config(void)
{
    unsigned char config = 0;
    DISABLE_BTB;
    flash_mspi_read_ram(FLASH_READ_CONFIGURE_CMD, 0, 0, 0, &config, 1);
    ENABLE_BTB;
    return config;
}

/*******************************************************************************************************************
 *                                  secondary calling function,
 *  there is no need to add an circumvention solution to solve the problem of access flash conflicts.
 ******************************************************************************************************************/

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
    case 0x0000345E:
        return FLASH_ETOX_ZB;
    case 0x000060C8:
        return FLASH_ETOX_GD;
    case 0x00004051:
        return FLASH_ETOX_GD;
    case 0x00006085:
        return FLASH_SONOS_PUYA;
    case 0x00002085:
        return FLASH_ETOX_PUYA;
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
