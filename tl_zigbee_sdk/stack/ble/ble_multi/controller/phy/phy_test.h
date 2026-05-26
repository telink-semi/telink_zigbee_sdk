/********************************************************************************************************
 * @file    phy_test.h
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
#ifndef PHY_TEST_H_
#define PHY_TEST_H_

#include "tl_common.h"

#define BLC_PHYTEST_DISABLE 0
#define BLC_PHYTEST_ENABLE  1

/**
 * @brief      for user to initialize PHY test module
 * @param      none
 * @return     none
 */
void blc_phy_initPhyTest_module(void);


/**
 * @brief      for user to set PHY test enable or disable
 * @param[in]  en - 1: enable; 0:disable
 * @return     status: 0x00 command OK, no other rvalue
 */
ble_sts_t blc_phy_setPhyTestEnable(u8 en);


/**
 * @brief      for user to get PHY test status: enable or disable
 * @param      none
 * @return     1: PHY test is enable; 0: PHY test is disable
 */
bool blc_phy_isPhyTestEnable(void);

/**
 * @brief      This function is used to manipulate "phytest"
 * @param[in]  *p - the phy test Command packet
 * @param[in]  n  - Reserved for future use
 * @return     0
 */
int blc_phytest_cmd_handler(u8 *p, int n);

#endif /* PHY_TEST_H_ */
