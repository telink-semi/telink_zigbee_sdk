/********************************************************************************************************
 * @file    ble_host.h
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
#ifndef STACK_BLE_HOST_BLE_HOST_H_
#define STACK_BLE_HOST_BLE_HOST_H_


#include "ble_common.h"
#include "ble_format.h"


/* ATT/GATT old version */
#include "host/att/att_v0/att_v0.h"
#include "host/att/att_v0/gatt_v0.h"

/* ATT/GATT new version */
#include "host/att/att.h"
#include "host/att/att.h"
#include "host/att/atts.h"
#include "host/att/attc.h"
#include "host/att/att_uuid.h"
#include "host/att/att_cmd.h"
#include "host/gatt/gatt.h"
#include "host/gatt/gatts.h"
#include "host/gatt/gattc.h"


#include "host/smp/smp.h"
#include "host/smp/smp_storage.h"

#include "host/gap/gap.h"
#include "host/gap/gapc.h"
#include "host/gap/gap_event.h"

#include "host/host_comp.h"

#include "host/l2cap/l2cap.h"
#include "host/signaling/signaling.h"

#include "debug/debug.h"

#ifdef TLK_ONLY_BLE_HOST
#include "host/hci/hci_simu_ll/hci_simu_ll_api.h"
#include "host/hci/hci_simu_ll/hci_simu_ll_acl_conn.h"
#include "host/hci/hci_simu_ll/hci_simu_ll_leg_adv.h"
#endif
/*********************************************************/


/* ATT/GATT old version */
/* ATT/GATT new version */



/*********************************************************/


#endif /* STACK_BLE_HOST_BLE_HOST_H_ */
