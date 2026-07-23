/********************************************************************************************************
 * @file    fixed_task.h
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
#ifndef TLKSTK_SIMULATE_FIXED_TASK_H_
#define TLKSTK_SIMULATE_FIXED_TASK_H_



/**
 * @brief Enumeration defining values for simulate fixed task ID
 */
typedef enum{
    SIMU_FIXED_TASK_ID_0        = 0,
    SIMU_FIXED_TASK_ID_1        = 1,
    SIMU_FIXED_TASK_ID_2        = 2,
    SIMU_FIXED_TASK_ID_3        = 3,
}simu_fixed_task_id_e;


/**
 * @brief Enumeration defining values for simulate task enable or disable
 */
typedef enum {
    SIMU_TASK_DISABLE = 0x00,
    SIMU_TASK_ENABLE  = 0x01,
}simu_task_en_e;



/**
 * @brief    This function is used to initialize simulate fixed task module
 * @param    none
 * @return    none
 */
void tlksdk_simu_initfixedTask_module(void);


/**
 * @brief       This function is used to set simulate fixed task's parameters: interval, duration and anchor point
 * @param[in]  task_id -  simulate fixed task ID
 * @param[in]  interval_us - simulate fixed task interval; unit: uS; must be in the range of 10000(10mS) ~ 4000000(4S)
 * @param[in]  duration_us - simulate fixed task duration; unit: uS; must be smaller than interval_us
 * @param[in]  anchor_point_tick - simulate fixed task anchor point; unit: system tick
 * @return       none
 */
void tlksdk_simu_setfixedTask(simu_fixed_task_id_e task_id, u32 interval_us, u32 duration_us, u32 anchor_point_tick);


/**
 * @brief       This function is used to set simulate fixed task enable
 * @param[in]  task_id -  simulate fixed task ID
 * @param[in]  enable - enable or disable
 * @return       none
 */
void tlksdk_simu_enablefixedTask(simu_fixed_task_id_e task_id, simu_task_en_e enable);




#endif /* TLKSTK_SIMULATE_FIXED_TASK_H_ */
