/********************************************************************************************************
 * @file    insert_task.h
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
#ifndef TLKSTK_SIMULATE_INSERT_TASK_H_
#define TLKSTK_SIMULATE_INSERT_TASK_H_



////////////////////////////////////// Insert Task 1 //////////////////////////////////////
/**
 * @brief    This function is used to initialize simulate insert task 1 module
 * @param    none
 * @return    none
 */
void tlksdk_simu_initInsertTask1_module(void);

/**
 * @brief       This function is used to set simulate insert task 1 interval
 * @param[in]  scan_interval - 625uS unit
 * @param[in]  scan_window - 625uS unit
 * @return       none
 */
void tlksdk_simu_setInsertTask1Interval(scan_inter_t scan_interval, scan_wind_t scan_window);


/**
 * @brief       This function is used to set simulate insert task 1 enable
 * @param[in]  enable - enable or disable
 * @return       none
 */
void tlksdk_simu_enableInsertTask1(simu_task_en_e enable);







////////////////////////////////////// Insert Task 2 //////////////////////////////////////
/**
 * @brief    This function is used to initialize simulate insert task 2 module
 * @param    none
 * @return    none
 */
void tlksdk_simu_initInsertTask2_module(void);


/**
 * @brief       This function is used to set simulate insert task 2 interval
 * @param[in]  scan_interval - 625uS unit
 * @param[in]  scan_window - 625uS unit
 * @return       none
 */
void tlksdk_simu_setInsertTask2Interval(scan_inter_t scan_interval, scan_wind_t scan_window);


/**
 * @brief       This function is used to set simulate insert task 2 enable
 * @param[in]  enable - enable or disable
 * @return       none
 */
void tlksdk_simu_setInsertTask2Enable(simu_task_en_e enable);









////////////////////////////////////// Insert Task 3 //////////////////////////////////////
/**
 * @brief    This function is used to initialize simulate insert task 3 module
 * @param    none
 * @return    none
 */
void tlksdk_simu_initInsertTask3_module(void);


/**
 * @brief       This function is used to set simulate insert task 3 interval
 * @param[in]  scan_interval - 625uS unit
 * @param[in]  scan_window - 625uS unit
 * @return       none
 */
void tlksdk_simu_setInsertTask3Interval(scan_inter_t scan_interval, scan_wind_t scan_window);


/**
 * @brief       This function is used to set simulate insert task 3 enable
 * @param[in]  enable - enable or disable
 * @return       none
 */
void tlksdk_simu_setInsertTask3Enable(simu_task_en_e enable);


#endif /* TLKSTK_SIMULATE_INSERT_TASK_H_ */
