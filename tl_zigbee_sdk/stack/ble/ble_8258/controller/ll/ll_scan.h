/********************************************************************************************************
 * @file    ll_scan.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef LL_SCAN_H_
#define LL_SCAN_H_





/**
 * @brief      This function is used to initialize scanning module
 * @param[in]  public_adr - public address pointer
 * @return     none
 */
void 		blc_ll_initScanning_module(u8 *public_adr);


/**
 * @brief      This function is used to set the scan parameters
 * 			   Please refer to BLE Core Specification: Vol 4, Part E, 7.8.10 for more information to understand the meaning of each parameters and
 * 			   the return values.
 * @param[in]  scan_type - passive Scanning or active scanning.
 * @param[in]  scan_interval - time interval from when the Controller started its last LE scan until it begins the subsequent LE scan
 * @param[in]  scan_window - The duration of the LE scan.
 * @param[in]  ownAddrType - Own_Address_Type
 * @param[in]  scan_fp - Scanning_Filter_Policy
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t	blc_ll_setScanParameter (scan_type_t scan_type, u16 scan_interval, u16 scan_window, own_addr_type_t  ownAddrType, scan_fp_type_t scan_fp);


/**
 * @brief	   This function is used to enable or disable legacy scanning.
 * 			   Please refer to BLE Core Specification: Vol 4, Part E, 7.8.11 for more information to understand the meaning of each parameters and
 * 			   the return values.
 * @param[in]  scan_enable
 * @param[in]  filter_duplicate - controls whether the Link Layer should filter out
 * 								  duplicate advertising reports (Filtering_Enabled) to the Host,
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t	blc_ll_setScanEnable (scan_en_t scan_enable, dupFilter_en_t filter_duplicate);


/**
 * @brief      This function is used to add scanning state in advertising state of slave role.
 * @param[in]  none.
 * @return     Status - 0x00: BLE success; 0x01-0xFF: fail
 */
ble_sts_t	blc_ll_addScanningInAdvState(void);


/**
 * @brief      This function is used to remove scanning state in advertising state of slave role.
 * @param[in]  none.
 * @return     Status - 0x00: BLE success; 0x01-0xFF: fail
 */
ble_sts_t	blc_ll_removeScanningFromAdvState(void);


/**
 * @brief      This function is used to add scanning state in connection state of slave role.
 * @param[in]  none.
 * @return     Status - 0x00: BLE success; 0x01-0xFF: fail
 */
ble_sts_t   blc_ll_addScanningInConnSlaveRole(void);


/**
 * @brief      This function is used to remove scanning state in connection state of slave role.
 * @param[in]  none.
 * @return     Status - 0x00: BLE success; 0x01-0xFF: fail
 */
ble_sts_t	blc_ll_removeScanningFromConnSLaveRole(void);


/**
 * @brief      This function is used to determine whether scan request is sent for all advertising.
 * @param[in]  scan_req_filter_enable - Scan request filter enable.
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t	blc_ll_scanReq_filter_en(u8 scan_req_filter_enable);

/**
 * @brief      This function is used to set Scan channel.
 * @param[in]  scan_channelMap - channel map
 * @return     Status - 0x00:  success;
 * 						other: fail
 */
ble_sts_t 	bls_ll_setScanChannelMap(adv_chn_map_t scan_channelMap);





/**
 * @brief      This function is used to enable or disable RX packet header Stimer tick in ADV report event
 * 			   attention: it's not standard BLE function, just for some customer's special requirement
 * @param[in]  randomAddr -  Random Device Address
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
void		blc_ll_advReport_setRxPacketTickEnable(int en);

/**
 *  @brief  Event Parameters for TLK defined special ADV report event
			if user enable RX packet header Stimer tick, should this data structure to analyze ADV report event
 */
typedef struct{
	u8		subcode;
	u8		nreport;
	u8		event_type;
	u8		adr_type;
	u8		mac[6];
	u32 	rxPktHeader_tick;	//RX packet header Stimer tick
	u8		len;
	u8		data[1];
}hci_tlk_advReportWithRxTickEvt_t;



#endif /* LL_SCAN_H_ */
