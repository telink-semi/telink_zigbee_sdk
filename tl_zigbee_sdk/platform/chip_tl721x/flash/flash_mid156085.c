/********************************************************************************************************
 * @file    flash_mid156085.c
 *
 * @brief   This is the source file for TL721X
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
#include "flash_type.h"


/**
 * @brief       This function reads the status of flash.
 * @param[in]   device_num  - the number of slave device.
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
unsigned short flash_read_status_mid156085_with_device_num(mspi_slave_device_num_e device_num)
{
    unsigned char status_low = flash_read_status(device_num, FLASH_READ_STATUS_CMD_LOWBYTE);
    unsigned char status_high = flash_read_status(device_num, FLASH_READ_STATUS_CMD_HIGHBYTE);
    return (status_low | (status_high << 8));
}

/**
 * @brief       This function write the status of flash.
 * @param[in]   device_num  - the number of slave device.
 * @param[in]   data    - the status value of the flash after the mask.
 * @param[in]   mask        - mid156085_write_status_mask_e.
 * @return      1: success, 0: error, 2: parameter error.
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
unsigned char flash_write_status_mid156085_with_device_num(mspi_slave_device_num_e device_num, unsigned short data, unsigned int mask)
{
    if (0 != (data & ~mask))
    {
        return 2;
    }

    unsigned short status = flash_read_status_mid156085_with_device_num(device_num);
    if(data != (status & mask)) //To reduce the operation of the status register.
    {
        status = data | (status & ~(mask));
        flash_write_status(device_num, FLASH_TYPE_16BIT_STATUS_ONE_CMD, status);
        status = flash_read_status_mid156085_with_device_num(device_num);
    }
    if(data == (status & mask))
    {
        return 1;
    }
    return 0;
}

/**
 * @brief       This function serves to set the protection area of the flash.
 * @param[in]   device_num  - the number of slave device.
 * @param[in]   data    - mid156085_lock_block_e.
 * @return      1: success, 0: error, 2: parameter error.
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
unsigned char flash_lock_mid156085_with_device_num(mspi_slave_device_num_e device_num, unsigned int data)
{
    flash_write_config_mid156085_with_device_num(device_num, 0x00, FLASH_WRITE_CONFIGURE_WPS_MID156085);
    return flash_write_status_mid156085_with_device_num(device_num, data, FLASH_WRITE_STATUS_BP_MID156085);
}

/**
 * @brief       This function serves to flash release protection.
 * @param[in]   device_num  - the number of slave device.
 * @return      1: success, 0: error, 2: parameter error.
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
unsigned char flash_unlock_mid156085_with_device_num(mspi_slave_device_num_e device_num)
{
    flash_write_config_mid156085_with_device_num(device_num, 0x00, FLASH_WRITE_CONFIGURE_WPS_MID156085);
    return flash_write_status_mid156085_with_device_num(device_num, FLASH_LOCK_NONE_MID156085, FLASH_WRITE_STATUS_BP_MID156085);
}

/**
 * @brief       This function serves to get the protection area of the flash.
 * @param[in]   device_num  - the number of slave device.
 * @return      mid156085_lock_block_e.
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
unsigned int flash_get_lock_block_mid156085_with_device_num(mspi_slave_device_num_e device_num)
{
    return flash_read_status_mid156085_with_device_num(device_num) & FLASH_WRITE_STATUS_BP_MID156085;
}

/**
 * @brief       This function serves to read data from the Security Registers of the flash.
 * @param[in]   device_num  - the number of slave device.
 * @param[in]   addr    - the start address of the Security Registers.
 *                      the address of the  Security Registers #1 0x001000-0x0013ff
 *                      the address of the  Security Registers #2 0x002000-0x0023ff
 *                      the address of the  Security Registers #3 0x003000-0x0033ff
 * @param[in]   len     - the length(in byte, must be above 0) of the content to be read.
 * @param[out]  buf     - the starting address of the content to be read.
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
void flash_read_otp_mid156085_with_device_num(mspi_slave_device_num_e device_num, unsigned long addr, unsigned long len, unsigned char* buf)
{
    flash_read_otp(device_num, addr, len, buf);
}

/**
 * @brief       This function serves to write data to the Security Registers of the flash you choose.
 * @param[in]   device_num  - the number of slave device.
 * @param[in]   addr    - the start address of the Security Registers.
 *                      the address of the  Security Registers #1 0x001000-0x0013ff
 *                      the address of the  Security Registers #2 0x002000-0x0023ff
 *                      the address of the  Security Registers #3 0x003000-0x0033ff
 * @param[in]   len     - the length(in byte, must be above 0) of content to be written.
 * @param[in]   buf     - the starting address of the content to be written.
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
void flash_write_otp_mid156085_with_device_num(mspi_slave_device_num_e device_num, unsigned long addr, unsigned long len, unsigned char *buf)
{
    flash_write_otp(device_num, addr, len, buf);
}

/**
 * @brief       This function serves to erase the data of the Security Registers that you choose.
 *              You can erase 1024-byte one time.
 * @param[in]   device_num  - the number of slave device.
 * @param[in]   addr    - the address that you want to erase.
 *                      the address of the  Security Registers #1 0x001000-0x0013ff
 *                      the address of the  Security Registers #2 0x002000-0x0023ff
 *                      the address of the  Security Registers #3 0x003000-0x0033ff
 * @return      none.
 * @note        Even you choose the middle area of the Security Registers,it will erase the whole area.
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
void flash_erase_otp_mid156085_with_device_num(mspi_slave_device_num_e device_num, mid156085_otp_block_e addr)
{
    flash_erase_otp(device_num, addr);
}

/**
 * @brief       This function serves to provide the write protect control to the Security Registers.
 * @param[in]   device_num  - the number of slave device.
 * @param[in]   data    - the lock area of the Security Registers.
 * @return      none.
 * @note        once they are set to 1, the Security Registers will become read-only permanently,
 *              you can't write or erase the area anymore.
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
void flash_lock_otp_mid156085_with_device_num(mspi_slave_device_num_e device_num, mid156085_lock_otp_e data)
{
    flash_write_status_mid156085_with_device_num(device_num, data, FLASH_WRITE_STATUS_OTP_MID156085);
}

/**
 * @brief       This function write the configure of flash.
 * @param[in]   device_num  - the number of slave device.
 * @param[in]   data    - the value of configure.
 * @param[in]   bit     - the range of bits to be modified when writing configure.
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
void flash_write_config_mid156085_with_device_num(mspi_slave_device_num_e device_num, unsigned char data, mid156085_write_config_bit_e bit)
{
    unsigned char config = 0;
    config = flash_read_config(device_num, 0, FLASH_READ_CONFIGURE_CMD);
    data |= (config & ~(bit));
    flash_write_config(device_num, 0, FLASH_WRITE_CONFIGURE_CMD_2, data);
}

