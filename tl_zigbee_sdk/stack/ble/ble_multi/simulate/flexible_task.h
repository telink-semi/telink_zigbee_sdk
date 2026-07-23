/********************************************************************************************************
 * @file    flexible_task.h
 *
 * @brief   This is the header file for Bluetooth SDK
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
#ifndef TLKSTK_SIMULATE_FLEXIBLE_TASK_H_
#define TLKSTK_SIMULATE_FLEXIBLE_TASK_H_


/**
 * @brief    This function is used to initialize simulate flexible task module
 * @param    none
 * @return    none
 */
void tlksdk_simu_initFlexibleTask_module(void);


/**
 * @brief       This function is used to set simulate flexible task interval
 * @param[in]  interval_us - simulate flexible task interval; unit: uS; must be in the range of 20000(10mS) ~ 4000000(4S)
 * @return       none
 */
void tlksdk_simu_setFlexibleTaskInterval(u32 interval_us);


/**
 * @brief       This function is used to set simulate flexible task enable
 * @param[in]  enable - enable or disable
 * @return       none
 */
void tlksdk_simu_enableFlexibleTask(simu_task_en_e enable);


#endif /* TLKSTK_SIMULATE_FLEXIBLE_TASK_H_ */
