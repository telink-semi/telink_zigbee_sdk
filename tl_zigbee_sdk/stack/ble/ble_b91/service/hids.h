/********************************************************************************************************
 * @file     hids.h
 *
 * @brief    This is the header file for b91 BLE SDK
 *
 * @author	 BLE GROUP
 * @date         12,2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
 *******************************************************************************************************/

#pragma once


/**
 * @brief	hids_uuid Hids Characteristic UUID
 */
#define CHARACTERISTIC_UUID_HID_BOOT_KEY_INPUT          0x2A22    //!< HID Boot Keyboard Input Report
#define CHARACTERISTIC_UUID_HID_BOOT_KEY_OUTPUT         0x2A32    //!< HID Boot Keyboard Output Report
#define CHARACTERISTIC_UUID_HID_BOOT_MOUSE_INPUT        0x2A33    //!< HID Boot Mouse Input Report
#define CHARACTERISTIC_UUID_HID_INFORMATION             0x2A4A    //!< HID Information
#define CHARACTERISTIC_UUID_HID_REPORT_MAP              0x2A4B    //!< HID Report Map
#define CHARACTERISTIC_UUID_HID_CONTROL_POINT           0x2A4C    //!< HID Control Point
#define CHARACTERISTIC_UUID_HID_REPORT                  0x2A4D    //!< HID Report
#define CHARACTERISTIC_UUID_HID_PROTOCOL_MODE           0x2A4E    //!< HID Protocol Mode


/**
 *  @brief  HID Report ID
 */
#define HID_REPORT_ID_KEYBOARD_INPUT                    1   //!< Keyboard input report ID
#define HID_REPORT_ID_CONSUME_CONTROL_INPUT             2   //!< Consumer Control input report ID
#define HID_REPORT_ID_MOUSE_INPUT                       3   //!< Mouse input report ID
#define HID_REPORT_ID_GAMEPAD_INPUT						4   //!< Gamepad  input report ID
#define HID_REPORT_ID_LED_OUT                           0   //!< LED output report ID
#define HID_REPORT_ID_FEATURE                           0   //!< Feature report ID

#define HID_REPORT_ID_CTRL_VOICE                        9

#define HID_REPORT_ID_AUDIO_FIRST_INPUT                 10//250
#define HID_REPORT_ID_AUDIO_SECND_INPUT                 11//251
#define HID_REPORT_ID_AUDIO_THIRD_INPUT                 12//247



/**
 *  @brief  HID Report type
 */

#define HID_REPORT_TYPE_INPUT                           1
#define HID_REPORT_TYPE_OUTPUT                          2
#define HID_REPORT_TYPE_FEATURE                         3



/**
 *  @brief  Definition for HID protocol mode
 */
#define HID_PROTOCOL_MODE_BOOT                          0
#define HID_PROTOCOL_MODE_REPORT                        1
#define DFLT_HID_PROTOCOL_MODE                          HID_PROTOCOL_MODE_REPORT



/**
 *  @brief  Definition for HID information flags
 */
#define HID_FLAGS_REMOTE_WAKE                           0x01      // RemoteWake
#define HID_FLAGS_NORMALLY_CONNECTABLE                  0x02      // NormallyConnectable
