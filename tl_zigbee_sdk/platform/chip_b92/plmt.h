/********************************************************************************************************
 * @file    plmt.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2023
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/** @page PLMT
 *
 *  Introduction
 *  ===============
 *   Platform-Level Machine Time interrupt (PLMT)
 *
 *  API Reference
 *  ===============
 *  Header File: plmt.h
 *
 *  PLMT features
 *  ===============
 *    - mtime and mtimecmp register have a 64-bit precision.
 *    - Support timer interrupt generation when mtime >= mtimecmp
 *
 *  How to use this driver
 *  ===============
 *    -# Init mtime clk using the function mtime_clk_init().
 *    -# Enable global interrupt using the function core_interrupt_enable().
 *    -# Enable MTI(machine timer interrupt) using the function core_mie_enable();
 *    -# Set mtime interval using the function mtime_set_interval_ms().
 *    -# Re-implement the mtime_irq_handler() function and set next interval using the function mtime_set_interval_ms().
 *
 */

#ifndef __PLMT_H_
#define __PLMT_H_
#include "reg_include/register.h"
#include "core.h"
#include "clock.h"

/**
 * @brief     This function servers to get mtimecmp value.
 * @return    The value of mtimecmp
 */
static inline unsigned long long mtime_get_cmp_value(void)
{
    return reg_mtimecmp;
}

/**
 * @brief     This function servers to config mtime clk source.
 * 
 * @param[in] src
 *              - CLK_32K_RC select 32k rc as the mtime clock source.
 *              - CLK_32K_XTAL select 32k xtal as the mtime clock source.
 * @return    none
 * @note      The frequency of mtime clk is 32k rc or xtal, rc clock source needs to be dynamically calibrated by calling lock_cal_32k_rc().
 */
void mtime_clk_init(clk_32k_type_e src);

/**
 * @brief     This function servers to set mtime value.
 * @param[in] value - The value of mtime.
 * @return    none
 */
_attribute_ram_code_sec_ void mtime_set_value(unsigned long long value);

/**
 * @brief     This function servers to get mtime value.
 * @return    The value of mtime
 */
_attribute_ram_code_sec_ unsigned long long mtime_get_value(void);

/**
 * @brief     This function servers to set mtimecmp value.
 * @param[in] value - The value of mtimecmp.
 * @return    none
 */
_attribute_ram_code_sec_ void mtime_set_cmp_value(unsigned long long value);

/**
 * @brief     This function servers to set the mtime trigger time.
 * @param[in] period_ms - The mtime trigger time, unit is millisecond.
 * @return    none
 */
_attribute_ram_code_sec_ void mtime_set_interval_ms(unsigned long long period);

#endif
