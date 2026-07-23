/********************************************************************************************************
 * @file    flash_mid1271cd.h
 *
 * @brief   This is the header file for TL321X
 *
 * @author  Driver Group
 * @date    2026
 *
 * @par     Copyright (c) 2026, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef __MID1271CD_H__
#define __MID1271CD_H__

/*
 * @brief     MID = 0x1271cd Flash include TH25D20UB.
 */

/**
 * @brief   TH25D20UB does not support Quad SPI.
 */
#define FLASH_NO_QE_SUPPORT_MID1271CD 0

/**
 * @brief   define the section of the protected memory area which is read-only and unalterable.
 * @note    when data protection, only enumeration items in mid1271cd_lock_block_e can be selected,
 *          to ensure that the values returned by the interface flash_get_lock_block_mid1271cd are in the mid1271cd_lock_block_e.
 */
typedef enum
{
    //---------------- CMP bit = 0---------------//
    FLASH_LOCK_NONE_MID1271CD     = 0x0000, //000000h-000000h
    FLASH_LOCK_UP_64K_MID1271CD   = 0x0004, //030000h-03FFFFh
    FLASH_LOCK_UP_128K_MID1271CD  = 0x0008, //020000h-03FFFFh
    FLASH_LOCK_LOW_64K_MID1271CD  = 0x0024, //000000h-00FFFFh
    FLASH_LOCK_LOW_128K_MID1271CD = 0x0028, //000000h-01FFFFh
    FLASH_LOCK_ALL_256K_MID1271CD = 0x000c, //000000h-03FFFFh
    FLASH_LOCK_UP_4K_MID1271CD    = 0x0044, //03F000h-03FFFFh
    FLASH_LOCK_UP_8K_MID1271CD    = 0x0048, //03E000h-03FFFFh
    FLASH_LOCK_UP_16K_MID1271CD   = 0x004c, //03C000h-03FFFFh
    FLASH_LOCK_UP_32K_MID1271CD   = 0x0050, //038000h-03FFFFh
    FLASH_LOCK_LOW_4K_MID1271CD   = 0x0064, //000000h-000FFFh
    FLASH_LOCK_LOW_8K_MID1271CD   = 0x0068, //000000h-001FFFh
    FLASH_LOCK_LOW_16K_MID1271CD  = 0x006c, //000000h-003FFFh
    FLASH_LOCK_LOW_32K_MID1271CD  = 0x0070, //000000h-007FFFh
    //---------------- CMP bit = 1---------------//
    FLASH_LOCK_LOW_192K_MID1271CD  = 0x4004, //000000h-02FFFFh
    FLASH_LOCK_LOW_128K_MID1271CD2 = 0x4008, //000000h-01FFFFh
    FLASH_LOCK_UP_192K_MID1271CD   = 0x4024, //010000h-03FFFFh
    FLASH_LOCK_UP_128K_MID1271CD2  = 0x4028, //020000h-03FFFFh
    FLASH_LOCK_LOW_252K_MID1271CD  = 0x4044, //000000h-03EFFFh
    FLASH_LOCK_LOW_248K_MID1271CD  = 0x4048, //000000h-03DFFFh
    FLASH_LOCK_LOW_240K_MID1271CD  = 0x404c, //000000h-03BFFFh
    FLASH_LOCK_LOW_224K_MID1271CD  = 0x4050, //000000h-037FFFh
    FLASH_LOCK_UP_252K_MID1271CD   = 0x4064, //001000h-03FFFFh
    FLASH_LOCK_UP_248K_MID1271CD   = 0x4068, //002000h-03FFFFh
    FLASH_LOCK_UP_240K_MID1271CD   = 0x406c, //004000h-03FFFFh
    FLASH_LOCK_UP_224K_MID1271CD   = 0x4070, //008000h-03FFFFh
} mid1271cd_lock_block_e;

/**
 * @brief     The starting address of the Security Registers.
 */
typedef enum
{
    FLASH_OTP_0x001000_512B_MID1271CD = 0x001000, //001000h-0011FFh
    FLASH_OTP_0x002000_512B_MID1271CD = 0x002000, //002000h-0021FFh
    FLASH_OTP_0x003000_512B_MID1271CD = 0x003000, //003000h-0031FFh
} mid1271cd_otp_block_e;

/**
 * @brief     the lock area of the Security Registers.
 */
typedef enum
{
    FLASH_LOCK_OTP_0x001000_512B_MID1271CD = 0x0800, //001000h-0011FFh
    FLASH_LOCK_OTP_0x002000_512B_MID1271CD = 0x1000, //002000h-0021FFh
    FLASH_LOCK_OTP_0x003000_512B_MID1271CD = 0x2000, //003000h-0031FFh
} mid1271cd_lock_otp_e;

/**
 * @brief     the range of masks to be modified when writing status.
 * @note      TH25D20UB does not support Quad SPI, so there is no QE related mask.
 */
typedef enum
{
    FLASH_WRITE_STATUS_BP_MID1271CD  = 0x407c, //the values that can be set can refer to mid1271cd_lock_block_e
    FLASH_WRITE_STATUS_OTP_MID1271CD = 0x3800, //the values that can be set can refer to mid1271cd_lock_otp_e

    /*Related to flash hardware protection.When using this function, you need to pay attention.
     *If the #WP pin of the flash is grounded and the hardware protection is set at this time,
     *the status register of the flash will be locked and irreversible.*/
    FLASH_WRITE_STATUS_SRP_MID1271CD = 0x0180,
} mid1271cd_write_status_mask_e;

/**
 * @brief       This function reads the status of flash.
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
unsigned short flash_read_status_mid1271cd(void);

/**
 * @brief       This function write the status of flash.
 * @param[in]   data    - the status value of the flash after the mask.
 * @param[in]   mask    - mid1271cd_write_status_mask_e.
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
unsigned char flash_write_status_mid1271cd(unsigned short data, unsigned int mask);

/**
 * @brief       This function serves to set the protection area of the flash.
 * @param[in]   data    - mid1271cd_lock_block_e.
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
unsigned char flash_lock_mid1271cd(unsigned int data);

/**
 * @brief       This function serves to flash release protection.
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
unsigned char flash_unlock_mid1271cd(void);

/**
 * @brief       This function serves to get the protection area of the flash.
 * @return      mid1271cd_lock_block_e.
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
unsigned int flash_get_lock_block_mid1271cd(void);

/**
 * @brief       This function serves to read data from the Security Registers of the flash.
 * @param[in]   addr    - the start address of the Security Registers.
 *                      the address of the  Security Registers #1 0x001000-0x0011ff
 *                      the address of the  Security Registers #2 0x002000-0x0021ff
 *                      the address of the  Security Registers #3 0x003000-0x0031ff
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
void flash_read_otp_mid1271cd(unsigned long addr, unsigned long len, unsigned char *buf);

/**
 * @brief       This function serves to write data to the Security Registers of the flash you choose.
 * @param[in]   addr    - the start address of the Security Registers.
 *                      the address of the  Security Registers #1 0x001000-0x0011ff
 *                      the address of the  Security Registers #2 0x002000-0x0021ff
 *                      the address of the  Security Registers #3 0x003000-0x0031ff
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
void flash_write_otp_mid1271cd(unsigned long addr, unsigned long len, unsigned char *buf);

/**
 * @brief       This function serves to erase the data of the Security Registers that you choose.
 *              You can erase 512-byte one time.
 * @param[in]   addr    - the address that you want to erase.
 *                      the address of the  Security Registers #1 0x001000-0x0011ff
 *                      the address of the  Security Registers #2 0x002000-0x0021ff
 *                      the address of the  Security Registers #3 0x003000-0x0031ff
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
void flash_erase_otp_mid1271cd(mid1271cd_otp_block_e addr);

/**
 * @brief       This function serves to provide the write protect control to the Security Registers.
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
void flash_lock_otp_mid1271cd(mid1271cd_lock_otp_e data);
#endif
