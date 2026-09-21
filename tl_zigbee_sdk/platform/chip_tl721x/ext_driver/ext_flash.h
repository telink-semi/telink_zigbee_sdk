/********************************************************************************************************
 * @file    ext_flash.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2024
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

#ifndef DRIVERS_TL721X_EXT_DRIVER_EXT_FLASH_H_
#define DRIVERS_TL721X_EXT_DRIVER_EXT_FLASH_H_

#include "../driver.h"

/**
* @brief Select the number of the slave device.(mspi_slave_device_num_e)
*/
#define BLE_EXT_DRIVER_SLAVE_NUM                SLAVE0


/**
* @brief This function serves to read flash mid and uid,and check the correctness of mid and uid.
*/
#define flash_read_mid_uid_with_check(mid, uid)         flash_read_mid_uid_with_check_with_device_num(BLE_EXT_DRIVER_SLAVE_NUM,mid,uid)


/**
* @brief This function serves to set the protection area of the flash./
*/
unsigned char flash_lock_mid156085(mid156085_lock_block_e data);
unsigned char flash_lock_mid146085(mid146085_lock_block_e data);
unsigned char flash_lock_mid1460c8(mid1460c8_lock_block_e data);
unsigned char flash_lock_mid1560c8(mid1560c8_lock_block_e data);

/**
* @brief This function serves to flash release protection.
*/
unsigned char flash_unlock_mid156085(void);
unsigned char flash_unlock_mid146085(void);
unsigned char flash_unlock_mid1460c8(void);
unsigned char flash_unlock_mid1560c8(void);

/**
* @brief This function serves to get the protection area of the flash.
*/
mid156085_lock_block_e flash_get_lock_block_mid156085(void);
mid146085_lock_block_e flash_get_lock_block_mid146085(void);
mid1460c8_lock_block_e flash_get_lock_block_mid1460c8(void);
mid1560c8_lock_block_e flash_get_lock_block_mid1560c8(void);

/**
 * @brief       This function serves to read MID of flash(MAC id).
 * @return      MID of the flash(4 bytes).
 */
__attribute__((always_inline)) inline unsigned int ble_flash_read_mid(void)
{
    return flash_read_mid_with_device_num(BLE_EXT_DRIVER_SLAVE_NUM);
}


/**
 * @brief       This function is used to set the use of quad IO mode when reading and writing flash.
 * @param[in]   flash_mid   - the mid of flash.
 * @return      1: success, 0: error, 2: parameter error, 3: mid is not supported.
 */
unsigned char ble_flash_4line_enable(void);


#endif /* DRIVERS_TL721X_EXT_DRIVER_EXT_FLASH_H_ */
