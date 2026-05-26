/********************************************************************************************************
 * @file    mcu_boot.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
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
#ifndef DRIVERS_TL321X_EXT_DRIVER_MCU_BOOT_H_
#define DRIVERS_TL321X_EXT_DRIVER_MCU_BOOT_H_


/**
 * @brief   Multiple boot address enumeration
 */
typedef enum{
    MULTI_BOOT_ADDR_0x20000     = 0x20000,  //128 K
    MULTI_BOOT_ADDR_0x40000     = 0x40000,  //256 K
    MULTI_BOOT_ADDR_0x80000     = 0x80000,  //512 K
    MULTI_BOOT_ADDR_0x100000    = 0x100000, //1M
    MULTI_BOOT_ADDR_0x200000    = 0x200000, //2M
    MULTI_BOOT_ADDR_0x400000    = 0x400000, //4M
    MULTI_BOOT_ADDR_0x800000    = 0x800000, //8M
}multi_boot_addr_e;




/* MCU boot relative definition below are used by ota_server.c, sometimes customers may need obtained the source code through authorization
 * so do not hidden these code */
#define     BOOT_MARK_VALUE_1_BYTE                              0x4B
#define     BOOT_MARK_VALUE_4_BYTE                              0x544C4E4B
#define     BOOT_MARK_ADDR                                      0x00020

#define     FW_SIZE_ADDR                                        0x00018

/* used to check flash protection function consistency between the old and new  firmware when OTA upgrading */
#define     FIRMWARE_FLASH_PROTECTION_FLAG_ADDR                 0x00015

/* used to check firmware encryption function consistency between the old and new  firmware when OTA upgrading */
#define     FIRMWARE_ENCRYPTION_FLAG_ADDR                       0x00016

/* used to check secure boot function consistency between the old and new  firmware when OTA upgrading */
#define     SECURE_BOOT_FLAG_ADDR                               0x00017

#endif /* DRIVERS_TL321X_EXT_DRIVER_MCU_BOOT_H_ */
