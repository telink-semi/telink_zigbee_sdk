/********************************************************************************************************
 * @file    flash_type.h
 *
 * @brief   This is the header file for TL721X
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
#ifndef __FLASH_TYPE_H__
#define __FLASH_TYPE_H__

#include "flash.h"
#include "flash_mid156085.h"    // P25Q16SU


/**
 * @brief       This function reads the status of flash.
 * @param[in]   device_num  - the number of slave device.
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
unsigned char flash_read_status(mspi_slave_device_num_e device_num, flash_command_e cmd);

/**
 * @brief       This function write the status of flash.
 * @param[in]   device_num  - the number of slave device.
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
void flash_write_status(mspi_slave_device_num_e device_num, flash_status_typedef_e type, unsigned short data);

/**
 * @brief       This function serves to read data from the Security Registers of the flash.
 * @param[in]   device_num  - the number of slave device.
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
void flash_read_otp(mspi_slave_device_num_e device_num, unsigned long addr, unsigned long len, unsigned char* buf);

/**
 * @brief       This function serves to write data to the Security Registers of the flash you choose.
 * @param[in]   device_num  - the number of slave device.
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
void flash_write_otp(mspi_slave_device_num_e device_num, unsigned long addr, unsigned long len, unsigned char *buf);

/**
 * @brief       This function serves to erase the data of the Security Registers that you choose.
 * @param[in]   device_num  - the number of slave device.
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
void flash_erase_otp(mspi_slave_device_num_e device_num, unsigned long addr);

/**
 * @brief       This function is used to write the configure of the flash.
 * @param[in]   device_num  - the number of slave device.
 * @param[in]   addr    - the flash configure address.
 * @param[in]   cmd     - the write command.
 * @param[in]   data    - the start address of the data buffer.
 * @param[in]   w_en_cmd- the flash write enable cmd.
 * @param[in]   busy_cmd- the flash read status cmd.
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
_attribute_text_sec_ void flash_write_config(mspi_slave_device_num_e device_num, unsigned long addr, unsigned int cmd, unsigned char data);

/**
 * @brief       This function is used to read the configure of the flash.
 * @param[in]   device_num  - the number of slave device.
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
_attribute_text_sec_ unsigned char flash_read_config(mspi_slave_device_num_e device_num, unsigned long addr, unsigned int cmd);

#endif

