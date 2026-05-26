/********************************************************************************************************
 * @file    ble_rf_pa_ctrl.c
 *
 * @brief   This is the source file for BLE external rf PA
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

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "ble_common.h"

_attribute_data_retention_	rf_pa_callback_t  blc_rf_pa_cb = 0;

static u32 ble_rf_pa_tx_pin = 0;
static u32 ble_rf_pa_rx_pin = 0;
static _attribute_ram_code_ void app_ble_rf_pa_handler(int type){
	if(type == PA_TYPE_TX_ON){
		drv_gpio_write(ble_rf_pa_tx_pin, 1);
		drv_gpio_write(ble_rf_pa_rx_pin, 0);
	}else if(type == PA_TYPE_RX_ON){
		drv_gpio_write(ble_rf_pa_tx_pin, 0);
		drv_gpio_write(ble_rf_pa_rx_pin, 1);
	}else{
		drv_gpio_write(ble_rf_pa_tx_pin, 0);
		drv_gpio_write(ble_rf_pa_rx_pin, 0);
	}
}


void ble_rf_pa_init(bool need_setio, u32 TXEN_pin, u32 RXEN_pin)
{
	ble_rf_pa_tx_pin = TXEN_pin;
	ble_rf_pa_rx_pin = RXEN_pin;

    if(need_setio){
		drv_gpio_func_set(ble_rf_pa_tx_pin);
		drv_gpio_output_en(ble_rf_pa_tx_pin, 1);
		drv_gpio_write(ble_rf_pa_tx_pin, 0);

		drv_gpio_func_set(ble_rf_pa_rx_pin);
		drv_gpio_output_en(ble_rf_pa_rx_pin, 1);
		drv_gpio_write(ble_rf_pa_rx_pin, 1);
    }

    blc_rf_pa_cb = app_ble_rf_pa_handler;
}

