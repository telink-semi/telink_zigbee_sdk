/********************************************************************************************************
 * @file     gap.h
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



#define GAP_ADTYPE_FLAGS                        0x01 //!< Discovery Mode: @ref GAP_ADTYPE_FLAGS_MODES
#define GAP_ADTYPE_16BIT_INCOMPLETE             0x02 //!< Incomplete List of 16-bit Service Class UUIDs
#define GAP_ADTYPE_16BIT_COMPLETE               0x03 //!< Complete List of 16-bit Service Class UUIDs
#define GAP_ADTYPE_32BIT_INCOMPLETE             0x04 //!< Service: More 32-bit UUIDs available
#define GAP_ADTYPE_32BIT_COMPLETE               0x05 //!< Service: Complete list of 32-bit UUIDs
#define GAP_ADTYPE_128BIT_INCOMPLETE            0x06 //!< Service: More 128-bit UUIDs available
#define GAP_ADTYPE_128BIT_COMPLETE              0x07 //!< Service: Complete list of 128-bit UUIDs
#define GAP_ADTYPE_LOCAL_NAME_SHORT             0x08 //!< Shortened local name
#define GAP_ADTYPE_LOCAL_NAME_COMPLETE          0x09 //!< Complete local name
#define GAP_ADTYPE_TX_POWER_LEVEL               0x0A //!< TX Power Level: 0xXX: -127 to +127 dBm
#define GAP_ADTYPE_OOB_CLASS_OF_DEVICE          0x0D //!< Simple Pairing OOB Tag: Class of device (3 octets)
#define GAP_ADTYPE_OOB_SIMPLE_PAIRING_HASHC     0x0E //!< Simple Pairing OOB Tag: Simple Pairing Hash C (16 octets)
#define GAP_ADTYPE_OOB_SIMPLE_PAIRING_RANDR     0x0F //!< Simple Pairing OOB Tag: Simple Pairing Randomizer R (16 octets)
#define GAP_ADTYPE_DEVICE_ID                    0x10 //!< Device ID Profile v1.3 or later
#define GAP_ADTYPE_SM_TK                        0x10 //!< Security Manager TK Value
#define GAP_ADTYPE_SM_OOB_FLAG                  0x11 //!< Secutiry Manager OOB Flags
#define GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE    0x12 //!< Min and Max values of the connection interval (2 octets Min, 2 octets Max) (0xFFFF indicates no conn interval min or max)
#define GAP_ADTYPE_SERVICES_LIST_16BIT          0x14 //!< Service Solicitation: list of 16-bit Service UUIDs
#define GAP_ADTYPE_SERVICES_LIST_32BIT          0x1F //!< Service Solicitation: list of 32-bit Service UUIDs
#define GAP_ADTYPE_SERVICES_LIST_128BIT         0x15 //!< Service Solicitation: list of 128-bit Service UUIDs
#define GAP_ADTYPE_SERVICE_DATA                 0x16 //!< Service Data
#define GAP_ADTYPE_SERVICE_DATA_UUID_16BIT      0x16 //!< Service Data - 16-bit UUID
#define GAP_ADTYPE_SERVICE_DATA_UUID_32BIT      0x20 //!< Service Data - 32-bit UUID
#define GAP_ADTYPE_SERVICE_DATA_UUID_128BIT     0x21 //!< Service Data - 128-bit UUID
#define GAP_ADTYPE_TARGET_ADDR_PUBLIC           0x17 //!< Public Target Address
#define GAP_ADTYPE_TARGET_ADDR_RANDOM           0x18 //!< Random Target Address
#define GAP_ADTYPE_APPEARANCE                   0x19 //!< Appearance
#define GAP_ADTYPE_ADVERTISING_INTERVAL         0x1A //!< Advertising Interval
#define GAP_ADTYPE_LE_BLUETOOTH_DEVICE_ADDR     0x1B //!< ​LE Bluetooth Device Address
#define GAP_ADTYPE_LE_ROLE                      0x1C //!< LE Role
#define GAP_ADTYPE_SIMPLE_PAIRING_HASHC_256     0x1D //!< Simple Pairing Hash C-256
#define GAP_ADTYPE_SIMPLE_PAIRING_RAND_R256     0x1E //!< Simple Pairing Randomizer R-256
#define GAP_ADTYPE_3D_INFORMATION_DATA          0x3D //!< 3D Synchronization Profile, v1.0 or later
#define GAP_ADTYPE_MANUFACTURER_SPECIFIC        0xFF //!< Manufacturer Specific Data: first 2 octets contain the Company Identifier Code followed by the additional manufacturer specific data



#define GAP_ADTYPE_LE_LIMITED_DISCOVERABLE_MODE_BIT       0x01
#define GAP_ADTYPE_LE_GENERAL_DISCOVERABLE_MODE_BIT       0x02
#define GAP_ADTYPE_LMP_BIT37_BIT                          0x04



#define GAP_APPEARE_UNKNOWN                     0x0000 //!< Unknown





/**
 * @brief      this  function is used to initialize GAP Peripheral
 * @param	   none
 * @return     none
 */
void 		blc_gap_peripheral_init(void);












