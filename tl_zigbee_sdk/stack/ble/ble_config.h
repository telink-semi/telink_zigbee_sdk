/********************************************************************************************************
 * @file     ble_config.h
 *
 * @brief    This is the header file for ble_config
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

#ifndef BLE_CONFIG_H__
#define BLE_CONFIG_H__


#if defined (MCU_CORE_8258)
#include "ble_8258/ble_config.h"
#elif defined (MCU_CORE_B91)
#include "ble_b91/ble_config.h"
#elif defined (MCU_CORE_TL321X) || defined (MCU_CORE_TL721X) || defined (MCU_CORE_TL323X)
#endif


#endif /* BLE_H_H_ */
