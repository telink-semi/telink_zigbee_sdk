/********************************************************************************************************
 * @file    ext_scan.h
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
#ifndef LL_SCAN_EXT_H_
#define LL_SCAN_EXT_H_

#include "stack/ble/ble_multi/hci/hci_cmd.h"


/**
 * @brief      for user to initialize extended scanning module
 *             notice that only one module can be selected between legacy scanning module and extended scanning module
 * @param      none
 * @return     none
 */
void blc_ll_initExtendedScanning_module(void);


/**
 * @brief      This function is used to set the extended scan parameters to be used on the advertising physical channels
 *             attention: when scan_phys is SCAN_PHY_1M,    scanType_1 & scanInter_1 & scanWindow_1 are invalid parameters,
 *                        when scan_phys is SCAN_PHY_CODED, scanType_0 & scanInter_0 & scanWindow_0 are invalid parameters,
 * @param[in]  ownAddrType - Own_Address_Type
 * @param[in]  scan_fp - Scanning_Filter_Policy
 * @param[in]  scan_phys - Scanning_PHYs, "SCAN_PHY_1M" or "SCAN_PHY_CODED"
 *
 *             Attention:
 *             scanType_0/scanInter_0/scanWindow_0 are only for 1M    PHY.  If    1M PHY is not supported, these parameters are ignored.
 *             scanType_1/scanInter_1/scanWindow_1 are only for Coded PHY.  If Coded PHY is not supported, these parameters are ignored.
 *
 * @param[in]  scanType_0 - Scan_Type for 1M PHY, Passive Scanning or Active Scanning.
 * @param[in]  scanInter_0 - Scan_Interval for 1M PHY, Time interval from when the Controller started its last scan until it
                               begins the subsequent scan on the primary advertising physical channel.
 * @param[in]  scanWindow_0 - Duration of the scan on the primary advertising physical channel for 1M PHY
 *
 * @param[in]  scanType_1 - Scan_Type for Coded PHY, Passive Scanning or Active Scanning.
 * @param[in]  scanInter_1 - Scan_Interval for Coded PHY, Time interval from when the Controller started its last scan until it
                               begins the subsequent scan on the primary advertising physical channel.
 * @param[in]  scanWindow_1 - Duration of the scan on the primary advertising physical channel  for Coded PHY
 *
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setExtScanParam(own_addr_type_t ownAddrType, scan_fp_type_t scan_fp, scan_phy_t scan_phys, scan_type_t scanType_0, scan_inter_t scanInter_0, scan_wind_t scanWindow_0, scan_type_t scanType_1, scan_inter_t scanInter_1, scan_wind_t scanWindow_1);


/**
 * @brief
 * @param num_test[in]   the number of individual tests in the decision instructions.
 * @param pDec_test[in]  the pointer to the test parameter buffer.
 * @return
 */
ble_sts_t blc_ll_setDecisionInstructCmd(u8 num_test, dec_ins_t* pDec_test);


/**
 * @brief      This function is used to enable or disable scanning.
 * @param[in]  extScan_en - 0x00: Scanning disabled; 0x01: Scanning enabled
 * @param[in]  filter_duplicate - Filter_Duplicates
 * @param[in]  duration - Scan duration
 * @param[in]  period - Time interval from when the Controller started its last Scan_Duration until it begins the
 *                      subsequent Scan_Duration.
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setExtScanEnable(scan_en_t extScan_en, dupe_fltr_en_t filter_duplicate, scan_durn_t duration, scan_period_t period);


/* special use API */
void blc_ll_setExtendedScanSecondaryChannelRxDataProcessEnable(u8 enable);


#endif /* LL_SCAN_EXT_H_ */
