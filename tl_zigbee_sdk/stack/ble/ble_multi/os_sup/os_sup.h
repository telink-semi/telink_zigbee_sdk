/********************************************************************************************************
 * @file    os_sup.h
 *
 * @brief   This is the source file for BLE SDK
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
#ifndef OS_SUP_H_
#define OS_SUP_H_

#include "tl_common.h"

/**
 *  @brief  Define the prototypes that the os_give_sem_t must conform to.
 */
typedef void (*os_give_sem_t)(void);

/**
 *  @brief  Define the prototypes that the take_mutex_sem&give_mutex_sem must conform to.
 */
typedef void (*os_mutex_sem_t)(void);

/**
 * @brief       This feature is used to enable support for OS mode
 * @param[in]   true - Enable, false - disable
 * @return      none
 */
void blc_setOsSupEnable(bool en);

/**
 * @brief       This function is to check if OS is supported
 * @param[in]   none
 * @return      true - Enable, false - disable
 */
bool blc_isOsSupEnable(void);

/**
 * @brief       This feature is used to check if the task schedule is busy
 * @param[in]   none
 * @return      true - busy,   false - no busy
 */
bool blc_isBleSchedulerBusy(void);

/**
 * @brief      Register an input for the Bluetooth OS semaphore
 * @param[in]  give_sem_from_isr -Interrupt  use
 * @param[in]  give_sem - Non-interruptible  use
 * @return
 */
void blc_ll_registerGiveSemCb(os_give_sem_t give_sem_from_isr, os_give_sem_t give_sem);

/**
 * @brief      Register a Bluetooth send data mutex.
 * Handling multitasking while sending packets results in an exception.
 * @param[in]  take_mutex_sem    -  lock
 * @param[in]  give_mutex_sem    -  unlock
 * @return
 */
void blc_ll_registerMutexSemCb(os_mutex_sem_t take_mutex_sem, os_mutex_sem_t give_mutex_sem);

#endif /* OS_SUP_H_ */
