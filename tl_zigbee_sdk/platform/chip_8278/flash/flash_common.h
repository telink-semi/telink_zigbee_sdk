/********************************************************************************************************
 * @file    flash_common.h
 *
 * @brief   This is the header file for B87
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
#ifndef __FLASH_COMMON_H__
#define __FLASH_COMMON_H__
#include "flash_type.h"
typedef unsigned char (*flash_lock_fp)(unsigned int);
typedef unsigned char (*flash_unlock_fp)(void);
typedef unsigned int(*flash_get_lock_block_fp)(void);

typedef struct {
	unsigned int mid;
    //flash_protect
	flash_get_lock_block_fp  get_lock;
	flash_unlock_fp  unlock;
	flash_lock_fp lock_func;
	unsigned int lock_size;
}flash_hal_handler_t;

typedef struct {
	unsigned int mid;
	unsigned int block_size;
}flash_user_defined_list_t;

typedef struct {
	flash_user_defined_list_t *list;
	unsigned int flash_cnt;
}flash_hal_user_handler_t;

/**
 * @brief		This function serves to read flash mid and uid,and check the correctness of mid and uid.
 * @param[out]	flash_mid	- Flash Manufacturer ID.
 * @param[out]	flash_uid	- Flash Unique ID.
 * @return		0: flash no uid or not a known flash model 	 1:the flash model is known and the uid is read.
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
int flash_read_mid_uid_with_check(unsigned int *flash_mid, unsigned char *flash_uid);

#endif

