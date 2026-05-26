/********************************************************************************************************
 * @file    device_information.h
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
#pragma once


/**
 * @brief   device_char_uuid Device Information Characteristic UUID
 */
#define CHARACTERISTIC_UUID_MANU_NAME_STRING     0x2A29
#define CHARACTERISTIC_UUID_MODEL_NUM_STRING     0x2A24
#define CHARACTERISTIC_UUID_SERIAL_NUM_STRING    0x2A25
#define CHARACTERISTIC_UUID_HW_REVISION_STRING   0x2A27
#define CHARACTERISTIC_UUID_FW_REVISION_STRING   0x2A26
#define CHARACTERISTIC_UUID_SW_REVISION_STRING   0x2A28
#define CHARACTERISTIC_UUID_SYSTEM_ID            0x2A23
#define CHARACTERISTIC_UUID_IEEE_11073_CERT_LIST 0x2A2A
#define CHARACTERISTIC_UUID_PNP_ID               0x2A50
