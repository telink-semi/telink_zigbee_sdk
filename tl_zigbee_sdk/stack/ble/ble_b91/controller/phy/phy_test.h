/********************************************************************************************************
 * @file     phy_test.h
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

#ifndef PHY_TEST_H_
#define PHY_TEST_H_

#include "tl_common.h"



/**
 * @brief	PHY test mode
 */
#ifndef			PHYTEST_MODE_DISABLE
#define			PHYTEST_MODE_DISABLE					0
#endif

#ifndef			PHYTEST_MODE_THROUGH_2_WIRE_UART
#define 		PHYTEST_MODE_THROUGH_2_WIRE_UART		1   //Direct Test Mode through a 2-wire UART interface
#endif

#ifndef			PHYTEST_MODE_OVER_HCI_WITH_USB
#define 		PHYTEST_MODE_OVER_HCI_WITH_USB			2   //Direct Test Mode over HCI(UART hardware interface)
#endif

#ifndef			PHYTEST_MODE_OVER_HCI_WITH_UART
#define 		PHYTEST_MODE_OVER_HCI_WITH_UART			3   //Direct Test Mode over HCI(USB  hardware interface)
#endif




/**
 * @brief	PHY test mode enableor disable
 */
#define 		BLC_PHYTEST_DISABLE						0
#define 		BLC_PHYTEST_ENABLE						1









/**
 * @brief      for user to initialize PHY test module
 * @param      none
 * @return     none
 */
void 	  blc_phy_initPhyTest_module(void);


/**
 * @brief      for user to set PHY test enable or disable
 * @param[in]  en - 1: enable; 0:disable
 * @return     status: 0x00 command OK, no other rvalue
 */
ble_sts_t blc_phy_setPhyTestEnable (u8 en);


/**
 * @brief      for user to get PHY test status: enable or disable
 * @param      none
 * @return     1: PHY test is enable; 0: PHY test is disable
 */
bool 	  blc_phy_isPhyTestEnable(void);


#if (MCU_CORE_TYPE == MCU_CORE_B91)
	/**
	 * @brief      uart RX data process for PHY test 2 wire UART mode
	 * @param      none
	 * @return     always 0
	 */
	int 	 blc_phyTest_2wire_rxUartCb (void);


	/**
	 * @brief      uart TX data process for PHY test 2 wire UART mode
	 * @param      none
	 * @return     always 0
	 */
	int 	 blc_phyTest_2wire_txUartCb (void);


	/**
	 * @brief      uart RX data process for PHY test hci UART mode
	 * @param      none
	 * @return     always 0
	 */
	int blc_phyTest_hci_rxUartCb (void);
#elif (MCU_CORE_TYPE == MCU_CORE_825x || MCU_CORE_TYPE == MCU_CORE_827x)
	/**
	 * @brief      uart RX data process for PHY test 2 wire UART mode
	 * @param      none
	 * @return     always 0
	 */
	int 	 phy_test_2_wire_rx_from_uart (void);


	/**
	 * @brief      uart TX data process for PHY test 2 wire UART mode
	 * @param      none
	 * @return     always 0
	 */
	int 	 phy_test_2_wire_tx_to_uart (void);
#endif




#endif /* PHY_TEST_H_ */
