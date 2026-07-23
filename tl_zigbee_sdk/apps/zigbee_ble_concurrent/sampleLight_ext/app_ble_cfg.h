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

#else
#define APP_SECURITY_ENABLE                         1
#define APP_DIRECT_ADV_ENABLE                       1

/*
 * ble role elect: master or slave (master is just for 8258, not supported by b91 till now)
 * */
#define BLE_SLAVE_ROLE_ENABLE                       1
#define BLE_MASTER_ROLE_ENABLE                      0

#if BLE_MASTER_ROLE_ENABLE
    #define BLE_HOST_SMP_ENABLE                     0  //1 for standard security management,  0 for telink referenced paring&bonding(no security)
    #define BLE_HOST_SIMPLE_SDP_ENABLE              1  //simple service discovery
#else
    #define SCAN_IN_ADV_STATE                       0  //only for slave, add time slot for scan after ADV
#endif
#endif

//ble_multi_conn
#elif defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL323X) || defined(MCU_CORE_TL521X)

#define ACL_CENTRAL_MAX_NUM                         0 // ACL central maximum number
#define ACL_PERIPHR_MAX_NUM                         1 // ACL peripheral maximum number

///////////////////////// Feature Configuration////////////////////////////////////////////////
#if BLE_SUPPORT_ADV_ONLY
#define ACL_PERIPHR_SMP_ENABLE                      0  //1 for smp,  0 no security
#define ACL_CENTRAL_SMP_ENABLE                      0  //1 for smp,  0 no security
#define BLE_OTA_SERVER_ENABLE                       0
#else
#if ZIGBEE_DIRECT_ENABLE
#define ZCL_ZBD_SUPPORT                             1
#define ZIGBEE_DIRECT_TUNNEL_ENABLE                 1

#define ACL_PERIPHR_SMP_ENABLE                      0  //1 for smp,  0 no security
#define ACL_CENTRAL_SMP_ENABLE                      0  //1 for smp,  0 no security
#else
#define ACL_PERIPHR_SMP_ENABLE                      1  //1 for smp,  0 no security
#define ACL_CENTRAL_SMP_ENABLE                      0  //1 for smp,  0 no security
#define BLE_OTA_SERVER_ENABLE                       0

/*Enable BLE_LATENCY_WORK_MODE macro, BLE connection latency will work in power supply mode.*/
#define BLE_LATENCY_WORK_MODE                       0
#endif
#endif

///////////////////////// OS settings /////////////////////////////////////////////////////////
#define FREERTOS_ENABLE                             0

///////////////////////// UI Configuration ////////////////////////////////////////////////////

///////////////////////// DEBUG  Configuration ////////////////////////////////////////////////
#define APP_LOG_EN                                  1
#endif

extern unsigned int  tlk_flash_mid;
extern unsigned int  tlk_flash_vendor;
extern unsigned char tlk_flash_capacity;

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
