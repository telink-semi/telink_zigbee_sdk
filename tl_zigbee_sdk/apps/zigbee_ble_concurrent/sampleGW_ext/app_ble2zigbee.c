/********************************************************************************************************
 * @file    app_ble2zigbee.c
 *
 * @brief   This is the source file for app_ble2zigbee
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
#include "tl_common.h"
#include "zbhci.h"

/**
 * @brief      the callback of the ble indication, transfer the command to zigbee operation
 *
 * @param[in]   cmdId  command identifier
 *
 * @param[in]   payload command payload
 *
 *
 * @return      None
 */
int zb_ble_hci_cmd_handler(u16 cmdId, u8 len, u8 *payload)
{
#if(ZBHCI_BLE) || (ZBHCI_BLE_CONTROLLER)
    zbhciCmdHandler(cmdId, (u16)len, payload);
#endif
    return 0;
}

