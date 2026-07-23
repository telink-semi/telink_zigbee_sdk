/********************************************************************************************************
 * @file    adv.h
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
#ifndef ADV_H_
#define ADV_H_

/**
 *  @brief  Definition for ADV maximum random delay time
 */
typedef enum
{
    MAX_DELAY_10MS  = 0x10,
    MAX_DELAY_5MS   = 0X08,
    MAX_DELAY_2P5MS = 0X04,
    MAX_DELAY_0MS   = 0,
} adv_max_delay_t;

/**
 * @brief   Set Advertising Interval maximum random delay time. If not set, default value is 10mS according to BLE SPEC.
 *          This API is mainly used for debug, to control adv_interval, this value will recover to 10mS if stack timing is limited
 *          attention: If this API is used, must be called before API "blc ll_setAdvParam" when initialization !!!
 * @param[in]   max_delay - select one from enumeration variable "adv_max_delay_t", other value invalid
 *
 * @return  None
 */
void blc_ll_setMaxAdvDelay_for_AdvEvent(adv_max_delay_t max_delay);


/**
* @brief       This function is used to set whether to check the adv_interval.
*              ADV interval is checked for undirected ADV by default according to BLE Core Specification.
*              User can use this API to bypass the rule if they want some none standard ADV interval being used for undirected ADV.
* @param[in]   enable -  1: check ADV interval; 0: not check ADV interval
* @return      none
*/
void        blc_ll_setAdvIntervalCheckEnable(u8 enable);

#endif /* ADV_H_ */
