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

#ifndef DRIVERS_TL321X_EXT_DRIVER_EXT_FLASH_H_
#define DRIVERS_TL321X_EXT_DRIVER_EXT_FLASH_H_

#include "../driver.h"


/**
 * @brief       This function serves to read MID of flash(MAC id).
 * @return      MID of the flash(4 bytes).
 */
__attribute__((always_inline)) inline unsigned int ble_flash_read_mid(void)
{
    return flash_read_mid();
}


/**
 * @brief       This function is used to set the use of quad IO mode when reading and writing flash.
 * @param[in]   flash_mid   - the mid of flash.
 * @return      1: success, 0: error, 2: parameter error, 3: mid is not supported.
 */
unsigned char ble_flash_4line_enable(void);


#endif /* DRIVERS_TL321X_EXT_DRIVER_EXT_FLASH_H_ */
