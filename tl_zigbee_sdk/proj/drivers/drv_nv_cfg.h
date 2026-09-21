/********************************************************************************************************
 * @file    drv_nv_cfg.h
 *
 * @brief   This is the header file for drv_nv_cfg
 *
 * @author  Zigbee Group
 * @date    2026
 *
 * @par     Copyright (c) 2026, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
#pragma once

/********************************************************************
 * @brief       Flash map usage.
 *
 * #if !defined(BOOT_LOADER_MODE) || (BOOT_LOADER_MODE == 0)
 *
 *                1M                             2M                            4M
 * 0x100000  ------------         0x200000  ------------         0x400000  ------------
 *          |  MAC_Addr  |                 |  MAC_Addr  |                 |  MAC_Addr  |
 *  0xFF000 |------------|        0x1FF000 |------------|        0x3FF000 |------------|
 *          | F_CFG_Info |                 | F_CFG_Info |                 | F_CFG_Info |
 *  0xFE000 |------------|        0x1FE000 |------------|        0x3FE000 |------------|
 *          | U_Cfg_Info |                 | U_Cfg_Info |                 | U_Cfg_Info |
 *  0xFC000 |------------|        0x1FC000 |------------|        0x3FC000 |------------|
 *          |     NV     |                 |     NV     |                 |     NV     |
 *  0xE6000 |------------|        0x1E6000 |------------|        0x3E6000 |------------|
 *          |   NV_BLE   |                 |   NV_BLE   |                 |   NV_BLE   |
 *  0xE4000 |------------|        0x1E4000 |------------|        0x3E4000 |------------|
 *          |            |                 |  Reserved  |                 |  Reserved  |
 *          |  OTA_Image |        0x100000 |------------|        0x100000 |------------|
 *          |   (400k)   |                 |            |                 |            |
 *          |            |                 |  OTA_Image |                 |  OTA_Image |
 *  0x80000 |------------|                 |   (512k)   |                 |   (512k)   |
 *          |  Reserved  |                 |            |                 |            |
 *  0x64000 |------------|         0x80000 |------------|         0x80000 |------------|
 *          |            |                 |            |                 |            |
 *          |  Firmware  |                 |  Firmware  |                 |  Firmware  |
 *          |   (400k)   |                 |   (512k)   |                 |   (512k)   |
 *          |            |                 |            |                 |            |
 *  0x00000  ------------          0x00000  ------------          0x00000  ------------
 *
 * #else
 *
 *               1M                              2M                            4M
 * 0x100000  ------------         0x200000  ------------         0x400000  ------------
 *          |  MAC_Addr  |                 |  MAC_Addr  |                 |  MAC_Addr  |
 *  0xFF000 |------------|        0x1FF000 |------------|        0x3FF000 |------------|
 *          | F_CFG_Info |                 | F_CFG_Info |                 | F_CFG_Info |
 *  0xFE000 |------------|        0x1FE000 |------------|        0x3FE000 |------------|
 *          | U_Cfg_Info |                 | U_Cfg_Info |                 | U_Cfg_Info |
 *  0xFC000 |------------|        0x1FC000 |------------|        0x3FC000 |------------|
 *          |     NV     |                 |     NV     |                 |     NV     |
 *  0xE6000 |------------|        0x1E6000 |------------|        0x3E6000 |------------|
 *          |   NV_BLE   |                 |   NV_BLE   |                 |   NV_BLE   |
 *  0xE4000 |------------|        0x1E4000 |------------|        0x3E4000 |------------|
 *          |            |                 |  Reserved  |                 |  Reserved  |
 *          |            |        0x100000 |------------|        0x100000 |------------|
 *          |  OTA_Image |                 |            |                 |            |
 *          |   (440k)   |                 |  OTA_Image |                 |  OTA_Image |
 *          |            |                 |   (496k)   |                 |   (496k)   |
 *  0x76000 |------------|                 |            |                 |            |
 *          |            |         0x84000 |------------|         0x84000 |------------|
 *          |            |                 |            |                 |            |
 *          |  Firmware  |                 |  Firmware  |                 |  Firmware  |
 *          |   (440k)   |                 |   (496k)   |                 |   (496k)   |
 *          |            |                 |            |                 |            |
 *  0x08000 |------------|         0x08000 |------------|         0x08000 |------------|
 *          | BootLoader |                 | BootLoader |                 | BootLoader |
 *  0x00000  ------------          0x00000  ------------          0x00000  ------------
 *
 * #endif
 */

#if defined(MCU_CORE_8258)
    #if (CHIP_TYPE == TLSR_8258_512K)
        #error "B85(512k) not support"
    #elif (CHIP_TYPE == TLSR_8258_1M)
        #if !BOOT_LOADER_MODE
        #error "B85(1M) only support bootloader mode"
        #endif
    #endif
#endif

/* Offset */
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
#define FLASH_TLNK_FLAG_OFFSET          8
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_TL721X) || \
      defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL323X) || \
      defined(MCU_CORE_TL521X)
#define FLASH_TLNK_FLAG_OFFSET          32
#endif

/************************************************************************
 * Flash address of APP firmware.
 */
#define FLASH_ADDR_OF_APP_FW            APP_IMAGE_ADDR

/* Flash Base Address define */
/************************************************************************
 * Flash address of factory setting parameters,
 * where are stored the MAC ADDRESS and CALIBRATION information.
 */
//1M flash
#define FLASH_ADDR_OF_MAC_ADDR_1M       0xFF000
#define FLASH_ADDR_OF_F_CFG_INFO_1M     0xFE000
//2M flash
#define FLASH_ADDR_OF_MAC_ADDR_2M       0x1FF000
#define FLASH_ADDR_OF_F_CFG_INFO_2M     0x1FE000
//4M flash
#define FLASH_ADDR_OF_MAC_ADDR_4M       0x3FF000
#define FLASH_ADDR_OF_F_CFG_INFO_4M     0x3FE000

/************************************************************************/
extern u32 g_u32MacFlashAddr;
extern u32 g_u32CfgFlashAddr;

#define MAC_BASE_ADD                    (g_u32MacFlashAddr)
#define FACTORY_CFG_BASE_ADD            (g_u32CfgFlashAddr)
/************************************************************************/

/**************************************************************************************
 * The following is the detailed factory configure information (F_CFG_Info).
 */
/* 8 bytes for MAC address. */
#define CFG_MAC_ADDRESS                 (MAC_BASE_ADD)

/* 2 bytes for USB ID. */
#if ZB_TEST_ENABLE
#if !defined(BOOT_LOADER_MODE) || (BOOT_LOADER_MODE == 0)
#define FLASH_USB_PID_ZBTEST            (0xFB000)
#define CFG_TELINK_USB_ID               (FLASH_USB_PID_ZBTEST)
#endif
#else
#define CFG_TELINK_USB_ID               (MAC_BASE_ADD + 0x40)
#endif

/* 1 byte for frequency offset calibration. */
#define CFG_FREQUENCY_OFFSET            (FACTORY_CFG_BASE_ADD)

/* 2 bytes for TP GAIN.
 * 0x77040(or 0xFE040) for BLE TP_GAIN_0, 0x77041(or 0xFE041) for BLE TP_GAIN_1,
 * 0x77042(or 0xFE042) for zigbee TP_GAIN_0, 0x77043(or 0xFE043) for zigbee TP_GAIN_1.
 */
#define CFG_TP_GAIN                     (FACTORY_CFG_BASE_ADD + 0x42)

/* Not supported for current SDK. */
#define CFG_32K_COUNTER_CALIBRATION     (FACTORY_CFG_BASE_ADD + 0x80)

/* ADC calibration. */
#define CFG_SAR_ADC_CALIBRATION         (FACTORY_CFG_BASE_ADD + 0xC0)
#define CFG_SD_ADC_CALIBRATION          (FACTORY_CFG_BASE_ADD + 0xE0)

/* Not supported for current SDK. */
#define CFG_24M_CRYSTAL_CALIBRATION     (FACTORY_CFG_BASE_ADD + 0x100)

/* Not supported for current SDK. */
#define CFG_T_SENSOR_CALIBRATION        (FACTORY_CFG_BASE_ADD + 0x140)

/* UID-based Firmware Encryption data(16 bytes), 0x77180(or 0xFE180) ~ 0x7718F(or 0xFE18F). */
#define CFG_FIRMWARE_ENCRYPTION         (FACTORY_CFG_BASE_ADD + 0x180)

/* 2 bytes for VDD_F calibration.
 * 0x771C0(or 0xFF1C0) for DCDC trim,
 * 0x771C1(or 0xFF1C1) for LDO trim.
 */
#define CFG_VDD_F_CALIBRATION           (FACTORY_CFG_BASE_ADD + 0x1C0)

/* 2 bytes for RX DCOC */
#define CFG_RX_DCOC_CALIBRATION         (FACTORY_CFG_BASE_ADD + 0x280)

/**************************************************************************************
 * The following is the detailed user configure information (U_CFG_Info).
 */
/* 16 bytes for pre-install code. */
//1M Flash: 0xFE000  - 0x1000 = 0xFD000
//2M Flash: 0x1FE000 - 0x1000 = 0x1FD000
//4M Flash: 0x3FE000 - 0x1000 = 0x3FD000
#define CFG_PRE_INSTALL_CODE            (FACTORY_CFG_BASE_ADD - 0x1000)

/* 1 byte for factory reset.
 * If not 0xFF, means the device is doing factory reset(erase NV).
 * The device will check this byte when powered on, if it is not 0xFF,
 * it will erase NV first.
 */
//1M Flash: 0xFE000  - 0x2000 = 0xFC000
//2M Flash: 0x1FE000 - 0x2000 = 0x1FC000
//4M Flash: 0x3FE000 - 0x2000 = 0x3FC000
#define CFG_FACTORY_RST_CNT             (FACTORY_CFG_BASE_ADD - 0x2000)

/************************************************************************
 * Flash address of NV module.
 */
//1M Flash: 0xFE000  - 0x18000 = 0xE6000
//2M Flash: 0x1FE000 - 0x18000 = 0x1E6000
//4M Flash: 0x3FE000 - 0x18000 = 0x3E6000
#define NV_BASE_ADDRESS_ZB              (FACTORY_CFG_BASE_ADD - 0x18000)
//1M Flash: 0xFE000  - 0x1A000 = 0xE4000
//2M Flash: 0x1FE000 - 0x1A000 = 0x1E4000
//4M Flash: 0x3FE000 - 0x1A000 = 0x3E4000
#define NV_BASE_ADDRESS_BLE             (FACTORY_CFG_BASE_ADD - 0x1A000)

#define FLASH_SMP_PAIRING_ADDR          (NV_BASE_ADDRESS_BLE)
#define FLASH_SMP_PAIRING_MAX_SIZE      (4096)

/************************************************************************
 * Flash address of OTA image.
 */
//1M Flash: 0xE4000
//2M Flash: 0x100000
//4M Flash: 0x100000
#define FLASH_ADDR_OTA_IMAGE_END        ((MAC_BASE_ADD > 0x100000) ? 0x100000 : NV_BASE_ADDRESS_BLE)

#if !defined(BOOT_LOADER_MODE) || (BOOT_LOADER_MODE == 0)
//unchangeable address
#define FLASH_ADDR_OF_OTA_IMAGE         (0x80000)
//1M Flash: max size = 0xE4000  - 0x80000 = 400K
//2M Flash: max size = 0x100000 - 0x80000 = 512K
//4M Flash: max size = 0x100000 - 0x80000 = 512K
#define FLASH_OTA_IMAGE_MAX_SIZE        (FLASH_ADDR_OTA_IMAGE_END - FLASH_ADDR_OF_OTA_IMAGE)
#else
//1M Flash: max size = (0xE4000  - 0x8000) / 2 = 440k
//2M Flash: max size = (0x100000 - 0x8000) / 2 = 496k
//4M Flash: max size = (0x100000 - 0x8000) / 2 = 496k
#define FLASH_OTA_IMAGE_MAX_SIZE        ((FLASH_ADDR_OTA_IMAGE_END - FLASH_ADDR_OF_APP_FW) / 2)
//1M Flash: ota addr = 0x8000 + 440k = 0x76000
//2M Flash: ota addr = 0x8000 + 496k = 0x84000
//4M Flash: ota addr = 0x8000 + 496k = 0x84000
#define FLASH_ADDR_OF_OTA_IMAGE         (FLASH_ADDR_OF_APP_FW + FLASH_OTA_IMAGE_MAX_SIZE)
#endif
