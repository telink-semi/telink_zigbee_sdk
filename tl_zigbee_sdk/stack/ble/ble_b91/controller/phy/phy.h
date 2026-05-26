/********************************************************************************************************
 * @file     phy.h
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

#ifndef PHY_H_
#define PHY_H_

#include "stack/ble/ble_b91/hci/hci_cmd.h"


/**
 * @brief      this function is used to initialize 2M/Coded PHY  feature
 * @param	   none
 * @return     none
 */
void		blc_ll_init2MPhyCodedPhy_feature(void);


/**
 * @brief       this function is used to set PHY type for connection
 * @param[in]	connHandle -
 * @param[in]	all_phys - preference PHY for TX & RX
 * @param[in]	tx_phys - preference PHY for TX
 * @param[in]	rx_phys - preference PHY for RX
 * @param[in]	phy_options - LE coding indication prefer
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t  	blc_ll_setPhy(	u16 connHandle,					le_phy_prefer_mask_t all_phys,
							le_phy_prefer_type_t tx_phys, 	le_phy_prefer_type_t rx_phys,
							le_ci_prefer_t phy_options);


/**
 * @brief       This function is used to set LE Coded PHY preference, S2 or S8, or no specific preference.
 * @param[in]	prefer_CI - Reference structure: hci_le_readPhyCmd_retParam_t.
 * @return      status, 0x00:  succeed
 * 					    other: failed
 */
ble_sts_t	blc_ll_setDefaultConnCodingIndication(le_ci_prefer_t prefer_CI);


#endif /* PHY_H_ */
