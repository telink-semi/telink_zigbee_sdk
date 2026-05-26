/********************************************************************************************************
 * @file    mcu_config.h
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
#ifndef DRIVERS_TL321X_EXT_DRIVER_MCU_CONFIG_H_
#define DRIVERS_TL321X_EXT_DRIVER_MCU_CONFIG_H_



/*
 * attention: user can not change any code in this file,
 *            these are hardware feature !!!
 */


/**
 * @brief   Public Device Address Company ID for this chip
 */
#define PDA_COMPANY_ID                                          0xC0A3C7



/*
    Flash Type  uid CMD     MID         Company
    P25Q80U     0x4b        0x146085    PUYA
    P25Q16SU    0x4b        0x156085    PUYA
    P25Q32SU    0x4b        0x166085    PUYA
    P25Q128L    0x4b        0x186085    PUYA
    GD25LQ16E   0x4b        0x1560c8    GD
 */
#define FLASH_P25Q80U_SUPPORT_EN                                1
#define FLASH_P25Q16SU_SUPPORT_EN                               1
#define FLASH_P25Q32SU_SUPPORT_EN                               0
#define FLASH_P25Q128L_SUPPORT_EN                               0
#define FLASH_P25Q128H_SUPPORT_EN                               0
#define FLASH_GD25LQ16E_SUPPORT_EN                              0

#define FLASH_P25Q80SU_SUPPORT_EN                               0
#define FLASH_GD25LE80E_SUPPORT_EN                              1
#define FLASH_GD25LE16E_SUPPORT_EN                              1


/**
 * @brief   RF THREE CHANNEL CALIBRATION
 */
#define RF_THREE_CHANNEL_CALIBRATION                            0



/**
 * @brief   MAC address burning on EFUSE/OTP
 */
#define BUILT_IN_MAC_ON_DEVICE                                  1




/**
 * @brief   Hardware secure boot
 *          doing firmware signature verification when MCU boot
 */
#define HARDWARE_SECURE_BOOT_SUPPORT_EN                         0

/**
 * @brief   Hardware firmware encryption
 */
#define HARDWARE_FIRMWARE_ENCRYPTION_SUPPORT_EN                 0

/**
 * @brief   Hardware secure debug
 */
#define HARDWARE_SECURE_DEBUG_SUPPORT_EN                        0



/**
 * @brief   support multiple priority interrupt
 */
#define MCU_SUPPORT_MULTI_PRIORITY_IRQ                          1


/**
 * @brief   MCU support multiple priority interrupt & support at least one type of GD Flash
 *
 * for multiple priority interrupt IC, such as B91m, support break nesting, generally use Puya flash, add GD flash vendor
 * GD flash: write 16B takes 100~200 uS, write 240byte flash may take 2~3 mS
 * if disabling IRQ for 3mS, system IRQ OK, but customer's low priority IRQ maybe delayed too much time
 * So when write firmware in OTA, will split big PDU to 16, for example, big PDU 240 will PDU 16 *15
 */
#define GD_FLASH_SUPPORTED_ON_MULTI_PRIORITY_IRQ_IC             1


#endif /* DRIVERS_TL321X_EXT_DRIVER_MCU_CONFIG_H_ */
