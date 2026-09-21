/********************************************************************************************************
 * @file    ext_flash.c
 *
 * @brief   This is the source file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2024
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
#include "ext_flash.h"

/**
 * @brief       This function is used to set the use of quad IO mode when reading and writing flash.
 * @param       None
 * @return      1: success, 0: error, 2: parameter error, 3: mid is not supported.
 */
unsigned char ble_flash_4line_enable(void)
{
    unsigned int mid = ble_flash_read_mid();
    unsigned char status = flash_4line_en_with_device_num(BLE_EXT_DRIVER_SLAVE_NUM, mid);
    if(status == 1)
    {
        flash_read_page = flash_4read;
        flash_set_rd_xip_config_with_device_num(BLE_EXT_DRIVER_SLAVE_NUM, FLASH_X4READ_CMD);
        flash_write_page = flash_quad_page_program;
    }
    return status;
}


unsigned char flash_lock_mid156085(mid156085_lock_block_e data)
{
    return flash_lock_mid156085_with_device_num(BLE_EXT_DRIVER_SLAVE_NUM,data);
}

unsigned char flash_unlock_mid156085(void)
{
    return flash_unlock_mid156085_with_device_num(BLE_EXT_DRIVER_SLAVE_NUM);
}


mid156085_lock_block_e flash_get_lock_block_mid156085(void)
{
    return flash_get_lock_block_mid156085_with_device_num(BLE_EXT_DRIVER_SLAVE_NUM);

}

unsigned char flash_lock_mid146085(mid146085_lock_block_e data)
{
    return flash_lock_mid146085_with_device_num(BLE_EXT_DRIVER_SLAVE_NUM,data);
}

unsigned char flash_unlock_mid146085(void)
{
    return flash_unlock_mid146085_with_device_num(BLE_EXT_DRIVER_SLAVE_NUM);
}

mid146085_lock_block_e flash_get_lock_block_mid146085(void)
{
    return flash_get_lock_block_mid146085_with_device_num(BLE_EXT_DRIVER_SLAVE_NUM);
}

unsigned char flash_lock_mid1460c8(mid1460c8_lock_block_e data)
{
    return flash_lock_mid1460c8_with_device_num(BLE_EXT_DRIVER_SLAVE_NUM,data);
}

unsigned char flash_unlock_mid1460c8(void)
{
    return flash_unlock_mid1460c8_with_device_num(BLE_EXT_DRIVER_SLAVE_NUM);
}

mid1460c8_lock_block_e flash_get_lock_block_mid1460c8(void)
{
    return flash_get_lock_block_mid1460c8_with_device_num(BLE_EXT_DRIVER_SLAVE_NUM);
}

unsigned char flash_lock_mid1560c8(mid1560c8_lock_block_e data)
{
    return flash_lock_mid1560c8_with_device_num(BLE_EXT_DRIVER_SLAVE_NUM,data);
}

unsigned char flash_unlock_mid1560c8(void)
{
    return flash_unlock_mid1560c8_with_device_num(BLE_EXT_DRIVER_SLAVE_NUM);
}

mid1560c8_lock_block_e flash_get_lock_block_mid1560c8(void)
{
    return flash_get_lock_block_mid1560c8_with_device_num(BLE_EXT_DRIVER_SLAVE_NUM);
}
