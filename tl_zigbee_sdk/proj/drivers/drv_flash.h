/********************************************************************************************************
 * @file    drv_flash.h
 *
 * @brief   This is the header file for drv_flash
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278) || defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL321X)
typedef u8 (*drv_flash_lock)(u32 blockSize);
typedef u8 (*drv_flash_unlock)(void);
#elif defined(MCU_CORE_TL721X)
typedef u8 (*drv_flash_lock)(mspi_slave_device_num_e num, u32 blockSize);
typedef u8 (*drv_flash_unlock)(mspi_slave_device_num_e num);
#endif

typedef struct{
	u32 mid;
	drv_flash_unlock unlock;
	drv_flash_lock lock;
	u32 blockSize;
}drv_flash_opt_t;


void flash_write(u32 addr, u32 len, u8 *buf);
bool flash_writeWithCheck(u32 addr, u32 len, u8 *buf);
void flash_read(u32 addr, u32 len, u8 *buf);
void flash_erase(u32 addr);
void flash_loadOpt(void);
void flash_lock(void);
void flash_unlock(void);
