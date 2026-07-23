/********************************************************************************************************
 * @file    ble_controller.h
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
#ifndef BLE_CONTROLLER_H_
#define BLE_CONTROLLER_H_


#include "ble_common.h"
#include "ble_format.h"


#include "hci/hci.h"
#include "hci/hci_const.h"
#include "hci/hci_cmd.h"
#include "hci/hci_event.h"

#include "controller/ll/ll.h"

#if !defined(TLK_ONLY_BLE_HOST)
#include "controller/ll/ll_pm.h"
#endif


//#include "controller/ll/fsu/ll_fsu.h"

//#include "controller/ll/feat_page/ll_feat_page.h"

#include "controller/ll/acl_conn/acl_conn.h"
#include "controller/ll/acl_conn/acl_peripheral.h"
#include "controller/ll/acl_conn/acl_central.h"

//#include "controller/ll/past/past.h"

//#include "controller/ll/pcl/pcl.h"


#include "controller/ll/adv/adv.h"
#include "controller/ll/adv/leg_adv.h"
#include "controller/ll/adv/ext_adv.h"

#include "controller/ll/scan/scan.h"
#include "controller/ll/scan/leg_scan.h"
#include "controller/ll/scan/ext_scan.h"
//#include "controller/ll/scan/mon_adv.h"


#include "controller/ll/init/init.h"
#include "controller/ll/init/leg_init.h"
#include "controller/ll/init/ext_init.h"


//#include "controller/ll/prdadv/pda.h"
//#include "controller/ll/prdadv/prd_adv.h"
//#include "controller/ll/prdadv/PAwR_adv.h"
//#include "controller/ll/prdadv/pda_sync.h"
//#include "controller/ll/prdadv/PAwR_sync.h"

//#include "controller/ial/ial.h"
//#include "controller/ll/iso/iso.h"

//#include "controller/ll/iso/bis.h"
//#include "controller/ll/iso/bis_bcst.h"
//#include "controller/ll/iso/bis_sync.h"
//#include "controller/ll/iso/cis.h"
//#include "controller/ll/iso/cis_central.h"
//#include "controller/ll/iso/cis_peripheral.h"
//#include "controller/ll/aoa_aod/aoa_aod.h"
//#include "controller/ll/chn_sound/cs.h"
//#include "controller/ll/chn_sound/cs_test_cmd/cs_test_cmd.h"


#include "controller/whitelist/whitelist.h"
#include "controller/whitelist/resolvelist.h"

#include "controller/csa/csa.h"
#include "controller/phy/phy.h"
#include "controller/phy/phy_test.h"
//#include "controller/phy/hdt_phy_test.h"

//#include "controller/ll/subrate/subrate.h"

#include "controller/contr_comp.h"


#include "debug/debug.h"

//#include "controller/ll/hdt/hdt.h"
/*********************************************************/

//#include "controller/ll/prdadv/pda_stack.h"
//#include "controller/ll/aoa_aod/aoa_stack.h"
//#include "controller/ial/ial_stack.h"
//#include "controller/ll/past/past_stack.h"
//#include "controller/ll/iso/iso_stack.h"
//#include "controller/ll/iso/bis_stack.h"
//#include "controller/ll/iso/cis_stack.h"
//#include "controller/ll/subrate/subrate_stack.h"
//#include "controller/ll/chn_sound/cs_stack.h"
//#include "controller/ll/chn_sound/cs_drbg/cs_drbg_stack.h"
//#include "controller/ll/sniffer/cs_sniffer/cs_sniffer_stack.h"
//#include "controller/ll/sniffer/adv_sniffer/adv_sniffer_stack.h"
//#include "controller/ll/private_pdu/private_pdu.h"


//#include "controller/ll/hdt/hdt_stack.h"
/*********************************************************/

#if(SCHEDULER_USE_STIMER_TICK_REPLACE_BIG_SMALL_SLOT)
    #include "system/system_internal.h"
    #include "simulate/simulate_internal.h"
    #include "simulate/fixed_task.h"
    #include "simulate/flexible_task.h"
    #include "simulate/insert_task.h"
#endif


#endif /* BLE_H_ */
