/********************************************************************************************************
 * @file    flash_mid156085.h
 *
 * @brief   This is the header file for B91
 *
 * @author  Driver Group
 * @date    2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef __MID156085_H__
#define __MID156085_H__

/*
 * @brief     MID = 0x156085 Flash include P25Q16SU.
 */


/**
 * @brief   define the section of the protected memory area which is read-only and unalterable.
 * @note    when data protection, only enumeration items in mid156085_lock_block_e can be selected,
 *          to ensure that the values returned by the interface flash_get_lock_block_mid156085 are in the mid156085_lock_block_e.
 */
typedef enum
{
    //---------WPS=0, CMP bit = 0-----------------//
    FLASH_LOCK_NONE_MID156085     = 0x0000, //000000h-000000h   //0x0020 0x407c...
    FLASH_LOCK_UP_64K_MID156085   = 0x0004, //1F0000h-1FFFFFh
    FLASH_LOCK_UP_128K_MID156085  = 0x0008, //1E0000h-1FFFFFh
    FLASH_LOCK_UP_256K_MID156085  = 0x000c, //1C0000h-1FFFFFh
    FLASH_LOCK_UP_512K_MID156085  = 0x0010, //180000h-1FFFFFh
    FLASH_LOCK_UP_1M_MID156085    = 0x0014, //100000h-1FFFFFh  //0x4034
    FLASH_LOCK_LOW_64K_MID156085  = 0x0024, //000000h-00FFFFh
    FLASH_LOCK_LOW_128K_MID156085 = 0x0028, //000000h-01FFFFh
    FLASH_LOCK_LOW_256K_MID156085 = 0x002c, //000000h-03FFFFh
    FLASH_LOCK_LOW_512K_MID156085 = 0x0030, //000000h-07FFFFh
    FLASH_LOCK_LOW_1M_MID156085   = 0x0034, //000000h-0FFFFFh  //0x4014
    FLASH_LOCK_UP_4K_MID156085    = 0x0044, //1FF000h-1FFFFFh
    FLASH_LOCK_UP_8K_MID156085    = 0x0048, //1FE000h-1FFFFFh
    FLASH_LOCK_UP_16K_MID156085   = 0x004c, //1FC000h-1FFFFFh
    FLASH_LOCK_UP_32K_MID156085   = 0x0050, //1F8000h-1FFFFFh   //0x0054
    FLASH_LOCK_LOW_4K_MID156085   = 0x0064, //000000h-000FFFh
    FLASH_LOCK_LOW_8K_MID156085   = 0x0068, //000000h-001FFFh
    FLASH_LOCK_LOW_16K_MID156085  = 0x006c, //000000h-003FFFh
    FLASH_LOCK_LOW_32K_MID156085  = 0x0070, //000000h-007FFFh   //0x0074
    //----------------WPS=0, CMP bit = 1---------------//
    FLASH_LOCK_LOW_1984K_MID156085 = 0x4004, //000000h-1EFFFFh
    FLASH_LOCK_LOW_1920K_MID156085 = 0x4008, //000000h-1DFFFFh
    FLASH_LOCK_LOW_1792K_MID156085 = 0x400c, //000000h-1BFFFFh
    FLASH_LOCK_LOW_1536K_MID156085 = 0x4010, //000000h-17FFFFh
    FLASH_LOCK_UP_1984K_MID156085  = 0x4024, //010000h-1FFFFFh
    FLASH_LOCK_UP_1920K_MID156085  = 0x4028, //020000h-1FFFFFh
    FLASH_LOCK_UP_1792K_MID156085  = 0x402c, //040000h-1FFFFFh
    FLASH_LOCK_UP_1536K_MID156085  = 0x4030, //080000h-1FFFFFh
    FLASH_LOCK_LOW_2044K_MID156085 = 0x4044, //000000h-1FEFFFh
    FLASH_LOCK_LOW_2040K_MID156085 = 0x4048, //000000h-1FDFFFh
    FLASH_LOCK_LOW_2032K_MID156085 = 0x404c, //000000h-1FBFFFh
    FLASH_LOCK_LOW_2016K_MID156085 = 0x4050, //000000h-1F7FFFh   //0x4054
    FLASH_LOCK_UP_2044K_MID156085  = 0x4064, //001000h-1FFFFFh
    FLASH_LOCK_UP_2040K_MID156085  = 0x4068, //002000h-1FFFFFh
    FLASH_LOCK_UP_2032K_MID156085  = 0x406c, //004000h-1FFFFFh
    FLASH_LOCK_UP_2016K_MID156085  = 0x4070, //008000h-1FFFFFh   //0x4074
    FLASH_LOCK_ALL_2M_MID156085    = 0x007c, //000000h-1FFFFFh   //0x4000 0x4040 0x4020 0x4060...
} mid156085_lock_block_e;

/**
 * @brief   the options of qe 
 * 
 */
typedef enum
{
    FLASH_QE_DISABLE_MID156085 = 0x0000,
    FLASH_QE_ENABLE_MID156085  = 0x0200,
} mid156085_qe_e;

/**
 * @brief     The starting address of the Security Registers.
 */
typedef enum
{
    FLASH_OTP_0x001000_1024B_MID156085 = 0x001000, //001000h-0013FFh
    FLASH_OTP_0x002000_1024B_MID156085 = 0x002000, //002000h-0023FFh
    FLASH_OTP_0x003000_1024B_MID156085 = 0x003000, //003000h-0033FFh
} mid156085_otp_block_e;

/**
 * @brief     the lock area of the Security Registers.
 */
typedef enum
{
    FLASH_LOCK_OTP_0x001000_1024B_MID156085 = 0x0800, //001000h-0013FFh
    FLASH_LOCK_OTP_0x002000_1024B_MID156085 = 0x1000, //002000h-0023FFh
    FLASH_LOCK_OTP_0x003000_1024B_MID156085 = 0x2000, //003000h-0033FFh
} mid156085_lock_otp_e;

/**
 * @brief     the range of masks to be modified when writing status.
 */
typedef enum
{
    FLASH_WRITE_STATUS_BP_MID156085  = 0x407c, //the values that can be set can refer to mid156085_lock_block_e
    FLASH_WRITE_STATUS_QE_MID156085  = 0x0200, //the values that can be set can refer to mid156085_qe_e
    FLASH_WRITE_STATUS_OTP_MID156085 = 0x3800, //the values that can be set can refer to mid156085_lock_otp_e

    /*Related to flash hardware protection.When using this function, you need to pay attention. 
     *If the #WP pin of the flash is grounded and the hardware protection is set at this time, 
     *the status register of the flash will be locked and irreversible.*/
    FLASH_WRITE_STATUS_SRP_MID156085 = 0x0180,
} mid156085_write_status_mask_e;

/**
 * @brief     the range of bits to be modified when writing configure.
 */
typedef enum
{
    FLASH_WRITE_CONFIGURE_WPS_MID156085 = 0x04,
} mid156085_write_config_bit_e;

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
unsigned short flash_read_status_mid156085(void);
/**
 * @brief       This function write the status of flash.
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
unsigned char flash_write_status_mid156085(unsigned short data, unsigned int mask);

/**
 * @brief       This function serves to set the protection area of the flash.
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
unsigned char flash_lock_mid156085(unsigned int data);

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
unsigned char flash_unlock_mid156085(void);

/**
 * @brief       This function serves to get the protection area of the flash.
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
unsigned int flash_get_lock_block_mid156085(void);

/**
 * @brief       This function serves to read data from the Security Registers of the flash.
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
void flash_read_otp_mid156085(unsigned long addr, unsigned long len, unsigned char *buf);

/**
 * @brief       This function serves to write data to the Security Registers of the flash you choose.
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
void flash_write_otp_mid156085(unsigned long addr, unsigned long len, unsigned char *buf);

/**
 * @brief       This function serves to erase the data of the Security Registers that you choose.
 *              You can erase 1024-byte one time.
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
void flash_erase_otp_mid156085(mid156085_otp_block_e addr);

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
void flash_lock_otp_mid156085(mid156085_lock_otp_e data);

/**
 * @brief       This function write the configure of flash.
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
void flash_write_config_mid156085(unsigned char data, mid156085_write_config_bit_e bit);
#endif
