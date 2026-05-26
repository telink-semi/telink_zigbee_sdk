/********************************************************************************************************
 * @file    app_cfg.h
 *
 * @brief   This is the header file for app_cfg
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
#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif


/**********************************************************************
 * BLE configuration
 */
//ble_single_conn
#if defined(MCU_CORE_8258) || defined(MCU_CORE_B91)
#if ZIGBEE_DIRECT_ENABLE    //only support for b91
#define ZCL_ZBD_SUPPORT                             1
#define ZIGBEE_DIRECT_TUNNEL_ENABLE                 1

#define APP_SECURITY_ENABLE                         0
#define APP_DIRECT_ADV_ENABLE                       0

#define ZBHCI_BLE                                   0
#else
#define APP_SECURITY_ENABLE                         1
#define APP_DIRECT_ADV_ENABLE                       1
#define ZBHCI_BLE                                   1
#endif

//ble_multi_conn
#elif defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL323X)
#define ACL_CENTRAL_MAX_NUM                         0 // ACL central maximum number
#define ACL_PERIPHR_MAX_NUM                         1 // ACL peripheral maximum number

/*Enable BLE_LATENCY_WORK_MODE macro, BLE connection latency will work in power supply mode.*/
#define BLE_LATENCY_WORK_MODE                       0
///////////////////////// Feature Configuration////////////////////////////////////////////////
#if BLE_SUPPORT_CONTROLLER_ONLY
#define ZBHCI_BLE                                   0
#define ZBHCI_BLE_CONTROLLER                        1

#define APP_LE_EXTENDED_ADV_EN                      0

#define HCI_TR_RX_BUF_SIZE                          (300)
#define HCI_TR_TX_BUF_SIZE                          (300)

#else
#define ZBHCI_BLE_CONTROLLER                        0

#if ZIGBEE_DIRECT_ENABLE
#define ZCL_ZBD_SUPPORT                             1
#define ZIGBEE_DIRECT_TUNNEL_ENABLE                 1

#define ACL_PERIPHR_SMP_ENABLE                      0  //1 for smp,  0 no security
#define ACL_CENTRAL_SMP_ENABLE                      0  //1 for smp,  0 no security

#define ZBHCI_BLE                                   0
#else

#define ZBHCI_BLE                                   1


#define ACL_PERIPHR_SMP_ENABLE                      1  //1 for smp,  0 no security
#define ACL_CENTRAL_SMP_ENABLE                      0  //1 for smp,  0 no security
#define BLE_OTA_SERVER_ENABLE                       0
#endif

#endif
///////////////////////// OS settings /////////////////////////////////////////////////////////
#define FREERTOS_ENABLE                             0

///////////////////////// DEBUG  Configuration ////////////////////////////////////////////////
#define APP_LOG_EN                                  1
#endif

#if (ZBHCI_BLE || ZBHCI_BLE_CONTROLLER)
#if !defined(ZBHCI_EN) || (ZBHCI_EN == 0)
#define ZBHCI_EN                                    1
#endif
#endif

extern unsigned int  tlk_flash_mid;
extern unsigned int  tlk_flash_vendor;
extern unsigned char tlk_flash_capacity;

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
