/********************************************************************************************************
 * @file    mcu_config.h
 *
 * @brief   This is the header file for B85
 *
 * @author  Driver Group
 * @date    May 8,2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef DRIVERS_B85_EXT_DRIVER_MCU_CONFIG_H_
#define DRIVERS_B85_EXT_DRIVER_MCU_CONFIG_H_

/*
 * attention: user can not change any code in this file,
 * 			  these are hardware feature !!!
 */


/*
	Flash Type	uid CMD			MID		Company
	ZB25WD40B	0x4b		0x13325E	ZB
	ZB25WD80B	0x4b		0x14325E	ZB
	GD25LD40C	0x4b		0x1360C8	GD
	GD25LD40E	0x4b		0x1360C8	GD
	GD25LD80C	0x4b(AN)	0x1460C8	GD
	GD25LD80E	0x4b(AN)	0x1460C8	GD
 */
#define FLASH_ZB25WD40B_SUPPORT_EN					1
#define FLASH_ZB25WD80B_SUPPORT_EN					1
#define FLASH_GD25LD40C_SUPPORT_EN					1
#define FLASH_GD25LD40E_SUPPORT_EN					1
#define FLASH_GD25LD80C_SUPPORT_EN					1
#define FLASH_GD25LD80E_SUPPORT_EN					1



/**
 * @brief	this MCU do not support multiple priority interrupt, do not change this macro !!!
 */
#define	MCU_SUPPORT_MULTI_PRIORITY_IRQ							0


/**
 * @brief	this MCU do not support Hardware secure boot, do not change this macro !!!
 */
#define HARDWARE_SECURE_BOOT_SUPPORT_EN							0


/**
 * @brief	this MCU do not support Hardware firmware encryption, do not change this macro !!!
 */
#define HARDWARE_FIRMWARE_ENCRYPTION_SUPPORT_EN					0


/**
 * @brief	use some special method to process flash writing in OTA progress for Zbit Flash on MCU which not support IRQ break nesting
 * 			attention: this macro only used in ota_server.c.
 */
#define ZBIT_FLASH_ON_SINGLE_PRIORITY_IRQ_IC_WORKAROUND_EN		((!MCU_SUPPORT_MULTI_PRIORITY_IRQ) && (FLASH_ZB25WD40B_SUPPORT_EN || FLASH_ZB25WD80B_SUPPORT_EN))



#endif
