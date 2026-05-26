/********************************************************************************************************
 * @file     ble_controller.h
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

#ifndef BLE_CONTROLLER_H_
#define BLE_CONTROLLER_H_


#include "stack/ble/ble_b91/ble_common.h"
#include "stack/ble/ble_b91/ble_format.h"


#include "stack/ble/ble_b91/hci/hci.h"
#include "stack/ble/ble_b91/hci/hci_const.h"
#include "stack/ble/ble_b91/hci/hci_cmd.h"
#include "stack/ble/ble_b91/hci/hci_event.h"


#include "stack/ble/ble_b91/controller/ll/ll.h"
#include "stack/ble/ble_b91/controller/ll/ll_adv.h"
#include "stack/ble/ble_b91/controller/ll/ll_pm.h"
#include "stack/ble/ble_b91/controller/ll/ll_scan.h"
#include "stack/ble/ble_b91/controller/ll/ll_whitelist.h"
#include "stack/ble/ble_b91/controller/ll/ll_resolvlist.h"
#include "stack/ble/ble_b91/controller/ll/ll_conn/ll_conn.h"
#include "stack/ble/ble_b91/controller/ll/ll_conn/ll_slave.h"
#include "stack/ble/ble_b91/controller/ll/ll_conn/ll_conn_csa.h"

#if(MCU_CORE_TYPE == MCU_CORE_825x || MCU_CORE_TYPE == MCU_CORE_827x)
//#include "stack/ble/ble_b91/controller/ll/ll_init.h"
//#include "stack/ble/ble_b91/controller/ll/ll_conn/ll_master.h"
#endif

#include "stack/ble/ble_b91/controller/ll/ll_ext_adv.h"


#include "phy/phy.h"
#include "phy/phy_stack.h"
#include "phy/phy_test.h"


#include "stack/ble/ble_b91/algorithm/ecc/ecc_ll.h"
#include "stack/ble/ble_b91/algorithm/aes_ccm/ble_aes_ccm.h"


/*********************************************************/
//Remove when file merge to SDK //
#include "stack/ble/ble_b91/ble_stack.h"
#include "stack/ble/ble_b91/ble_config.h"
#include "stack/ble/ble_b91/trace.h"

#include "stack/ble/ble_b91/controller/ll/ll_stack.h"
#include "stack/ble/ble_b91/controller/ll/ll_conn/conn_stack.h"
/*********************************************************/

#endif /* BLE_H_ */
