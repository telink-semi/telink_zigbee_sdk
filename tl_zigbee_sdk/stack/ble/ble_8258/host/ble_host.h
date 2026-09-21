/********************************************************************************************************
 * @file    ble_host.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    12,2021
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
 *
 *******************************************************************************************************/
#ifndef STACK_BLE_HOST_BLE_HOST_H_
#define STACK_BLE_HOST_BLE_HOST_H_

#include "stack/ble/ble_8258/ble_common.h"
#include "stack/ble/ble_8258/ble_format.h"

#include "stack/ble/ble_8258/host/l2cap/l2cap.h"
#include "stack/ble/ble_8258/host/signaling/signaling.h"


#include "stack/ble/ble_8258/host/attr/att.h"
#include "stack/ble/ble_8258/host/attr/gatt.h"


#include "stack/ble/ble_8258/host/smp/smp.h"
#include "stack/ble/ble_8258/host/smp/smp_peripheral.h"
#include "stack/ble/ble_8258/host/smp/smp_storage.h"


#include "stack/ble/ble_8258/host/gap/gap.h"
#include "stack/ble/ble_8258/host/gap/gap_event.h"


#include "stack/ble/ble_8258/device/multi_device.h"
/*********************************************************/




/*********************************************************/

#include "stack/ble/ble_8258/host/smp/smp_central.h"

#endif /* STACK_BLE_HOST_BLE_HOST_H_ */
