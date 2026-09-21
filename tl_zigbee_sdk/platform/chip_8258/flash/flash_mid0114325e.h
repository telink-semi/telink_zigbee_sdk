/********************************************************************************************************
 * @file    flash_mid0114325e.h
 *
 * @brief   This is the header file for B85
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
#ifndef __MID0114325E_H__
#define __MID0114325E_H__

/*
 * @brief     MID = 0x14325e, to distinguish it from ZB25WD80B, 0x0114325e is used here. Flash include ZB25WD80C.
 */


/**
 * @brief   define the section of the protected memory area which is read-only and unalterable.
 * @note    when data protection, only enumeration items in mid0114325e_lock_block_e can be selected,
 *          to ensure that the values returned by the interface flash_get_lock_block_mid0114325e are in the mid0114325e_lock_block_e.
 */
typedef enum
{
    //----------------CMP bit = 0-----------------//
    FLASH_LOCK_NONE_MID0114325E                 = 0x0000, // None
    FLASH_LOCK_UP_64K_MID0114325E               = 0x0004, // 0F0000h-0FFFFFh
    FLASH_LOCK_UP_128K_MID0114325E              = 0x0008, // 0E0000h-0FFFFFh
    FLASH_LOCK_UP_256K_MID0114325E              = 0x000c, // 0C0000h-0FFFFFh
    FLASH_LOCK_UP_512K_MID0114325E              = 0x0010, // 080000h-0FFFFFh
    FLASH_LOCK_ALL_1M_MID0114325E               = 0x001c, // 000000h-0FFFFFh  // also 0x003c, 0x005c, 0x007c, ...
    FLASH_LOCK_LOW_64K_MID0114325E              = 0x0024, // 000000h-00FFFFh
    FLASH_LOCK_LOW_128K_MID0114325E             = 0x0028, // 000000h-01FFFFh
    FLASH_LOCK_LOW_256K_MID0114325E             = 0x002c, // 000000h-03FFFFh
    FLASH_LOCK_LOW_512K_MID0114325E             = 0x0030, // 000000h-07FFFFh
    FLASH_LOCK_UP_4K_MID0114325E                = 0x0044, // 0FF000h-0FFFFFh
    FLASH_LOCK_UP_8K_MID0114325E                = 0x0048, // 0FE000h-0FFFFFh
    FLASH_LOCK_UP_16K_MID0114325E               = 0x004c, // 0FC000h-0FFFFFh
    FLASH_LOCK_UP_32K_MID0114325E               = 0x0050, // 0F8000h-0FFFFFh  // also 0x0054
    FLASH_LOCK_LOW_4K_MID0114325E               = 0x0064, // 000000h-000FFFh
    FLASH_LOCK_LOW_8K_MID0114325E               = 0x0068, // 000000h-001FFFh
    FLASH_LOCK_LOW_16K_MID0114325E              = 0x006c, // 000000h-003FFFh
    FLASH_LOCK_LOW_32K_MID0114325E              = 0x0070, // 000000h-007FFFh  // also 0x0074

    //------------------CMP bit = 1---------------//
    FLASH_LOCK_LOW_960K_CMP1_MID0114325E        = 0x4004, // 000000h-0EFFFFh
    FLASH_LOCK_LOW_896K_CMP1_MID0114325E        = 0x4008, // 000000h-0DFFFFh
    FLASH_LOCK_LOW_768K_CMP1_MID0114325E        = 0x400c, // 000000h-0BFFFFh
    FLASH_LOCK_UP_960K_CMP1_MID0114325E         = 0x4024, // 010000h-0FFFFFh
    FLASH_LOCK_UP_896K_CMP1_MID0114325E         = 0x4028, // 020000h-0FFFFFh
    FLASH_LOCK_UP_768K_CMP1_MID0114325E         = 0x402c, // 040000h-0FFFFFh
    FLASH_LOCK_LOW_1020K_CMP1_MID0114325E       = 0x4044, // 000000h-0FEFFFh
    FLASH_LOCK_LOW_1016K_CMP1_MID0114325E       = 0x4048, // 000000h-0FDFFFh
    FLASH_LOCK_LOW_1008K_CMP1_MID0114325E       = 0x404c, // 000000h-0FBFFFh
    FLASH_LOCK_LOW_992K_CMP1_MID0114325E        = 0x4050, // 000000h-0F7FFFh  // also 0x4054
    FLASH_LOCK_UP_1020K_CMP1_MID0114325E        = 0x4064, // 001000h-0FFFFFh
    FLASH_LOCK_UP_1016K_CMP1_MID0114325E        = 0x4068, // 002000h-0FFFFFh
    FLASH_LOCK_UP_1008K_CMP1_MID0114325E        = 0x406c, // 004000h-0FFFFFh
    FLASH_LOCK_UP_992K_CMP1_MID0114325E         = 0x4070, // 008000h-0FFFFFh  // also 0x4074
} mid0114325e_lock_block_e;

/**
 * @brief     the range of bits to be modified when writing status.
 */
typedef enum{
	FLASH_WRITE_STATUS_BP_MID0114325E	=	0x407c,
}mid0114325e_write_status_bit_e;


/**
 * @brief 		This function reads the status of flash.
 * @return 		the value of status.
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
unsigned short flash_read_status_mid0114325e(void);

/**
 * @brief 		This function write the status of flash.
 * @param[in]  	data	- the value of status.
 * @param[in]  	mask    - mid0114325e_write_status_bit_e
 * @return 		1: success, 0: error, 2: parameter error.
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
unsigned char flash_write_status_mid0114325e(unsigned int data, unsigned int mask);

/**
 * @brief 		This function serves to set the protection area of the flash.
 * @param[in]   data	- refer to the mid0114325e_lock_block_e protection area definition in the .h file.
 * @return 		1: success, 0: error, 2: parameter error.
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
unsigned char flash_lock_mid0114325e(unsigned int data);

/**
 * @brief 		This function serves to flash release protection.
 * @return 		1: success, 0: error, 2: parameter error.
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
unsigned char flash_unlock_mid0114325e(void);

/**
 * @brief 		This function serves to get the protection area of the flash.
 * @return 		mid0114325e_lock_block_e
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
unsigned int flash_get_lock_block_mid0114325e(void);
#endif

