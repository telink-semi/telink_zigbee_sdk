/********************************************************************************************************
 * @file    gap.h
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

#include "tl_common.h"

typedef enum
{
    Gap_Role_Broadcaster = 0,
    Gap_Role_Observer    = 1,
    Gap_Role_Peripheral  = 2,
    Gap_Role_Central     = 3,
} gap_role_t;

#define GAP_ADTYPE_LE_LIMITED_DISCOVERABLE_MODE_BIT 0x01
#define GAP_ADTYPE_LE_GENERAL_DISCOVERABLE_MODE_BIT 0x02
#define GAP_ADTYPE_LMP_BIT37_BIT                    0x04

#define GAP_APPEARANCE_UNKNOWN                      0x0000 //!< Unknown
#define GAP_APPEARANCE_EARBUD                       0x0941 // Earbud
#define GAP_APPEARANCE_HEADSET                      0x0942 // Headset
#define GAP_APPEARANCE_HEADPHONES                   0x0943 // Headphones

#define GAP_APPEARANCE_GENERIC_HID                  0x0300
#define GAP_APPEARANCE_HID_KEYBOARD                 0x03C1
#define GAP_APPEARANCE_HID_MOUSE                    0x03C2


/*
 * @brief     this function is used to set data pending time for a single connection.
 *            i.e  in num_10ms*10ms after service discovery, notify/indication on this connection will fail.
 * @param     connHandle. connection handle.
 *            num_10ms.unit is *10ms. for example if num_10ms = 30, that indicate 30*10 = 300ms.
 */
u8 blc_gap_setSingleServerDataPendingTime_upon_ClientCmd(u16 connHandle, u16 num_10ms);

/*
 * @brief     this function is used to set data pending time.
 *            i.e  in num_10ms*10ms after service discovery, notify/indication will fail.
 * @param     num_10ms.unit is *10ms. for example if num_10ms = 30, that indicate 30*10 = 300ms.
 */
void blc_att_setServerDataPendingTime_upon_ClientCmd(u16 num_10ms);


/**
 * @brief      This function is used to disable MTU exchange automatically.
 *             User can call this API in connection callback(HCI_LE_EVT_MASK_CONNECTION_COMPLETE/HCI_SUB_EVT_LE_ENHANCED_CONNECTION_COMPLETE).
 * @param[in]  connHandle - ACL connection handle.
 * @return     none.
 */
void blc_gap_mtuAutoExgDisable(u16 connHandle);


/**
 * @brief      this  function is used to initialize GAP
 * @param      none
 * @return     none
 */
void blc_gap_init(void);


/**
* @brief      this function is used check if any host initialization incorrect.
*             attention: this function must be called after all host Initialization finished.
* @param      none
* @return     status - 0x00:  succeed, no error
*                      other: error code
*/
init_err_t blc_host_checkHostInitialization(void);
