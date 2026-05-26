/********************************************************************************************************
 * @file    leg_scan.h
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
#ifndef LLMS_SCAN_H_
#define LLMS_SCAN_H_


#include "stack/ble/ble_multi/hci/hci_cmd.h"


/**
 * @brief      for user to initialize legacy scanning module
 *             notice that only one module can be selected between legacy scanning module and extended scanning module
 * @param      none
 * @return     none
 */
void blc_ll_initLegacyScanning_module(void);


/**
 * @brief      This function is used to set the scan parameters
 * @param[in]  scan_type - passive Scanning or active scanning.
 * @param[in]  scan_interval - time interval from when the Controller started its last LE scan until it begins the subsequent LE scan
 * @param[in]  scan_window - The duration of the LE scan.
 * @param[in]  ownAddrType - Own_Address_Type
 * @param[in]  scan_fp - Scanning_Filter_Policy
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setScanParameter(scan_type_t scan_type, scan_inter_t scan_interval, scan_wind_t scan_window, own_addr_type_t ownAddrType, scan_fp_type_t scan_fp);


/**
 * @brief      enable or disable legacy scanning.
 * @param[in]  scan_enable
 * @param[in]  filter_duplicate - controls whether the Link Layer should filter out
 *                                duplicate advertising reports (Filtering_Enabled) to the Host,
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setScanEnable(scan_en_t scan_enable, dupFilter_en_t filter_duplicate);


#endif /* LLMS_SCAN_H_ */
