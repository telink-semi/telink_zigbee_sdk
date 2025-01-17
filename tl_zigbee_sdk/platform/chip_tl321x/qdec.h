/********************************************************************************************************
 * @file    qdec.h
 *
 * @brief   This is the header file for TL321X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#pragma once

#include "gpio.h"
#include "dma.h"
#include "reg_include/register.h"
#include "lib/include/clock.h"

/**
 * qedc input channel
 */
typedef enum
{
    /**
      PA2 is not recommended
      Because its use is limited, it is necessary to ensure that the PA2 input function is disabled before the sys_init /pm_sleep_wakeup/pm_set_dig_module_power_switch functions,
      otherwise RF may not work properly.For details, refer to the comments of the functions mentioned above(BUT-53)
    */
    QDEC_CHN_PA2,//This pin is not recommended. Because when using PA2, baseband cannot be turned off during suspend sleep.
    QDEC_CHN_PA3,
    QDEC_CHN_PB6,
    QDEC_CHN_PB7,
    QDEC_CHN_PC2,
    QDEC_CHN_PC3,
    QDEC_CHN_PD6,
    QDEC_CHN_PD7,
} qdec_chn_e;

/**
 * qedc mode
 */
typedef enum
{
    COMMON_MODE,
    DOUBLE_ACCURACY_MODE,
} qdec_mode_e;

/**
 *the hardware debouncing threshold
 */
typedef enum
{
    QDEC_THRSH_P187p5us = 0,
    QDEC_THRSH_P375p0us,
    QDEC_THRSH_P750p0us,
    QDEC_THRSH_P1500p0us,
    QDEC_THRSH_P3000p0us,
    QDEC_THRSH_P6000p0us,
    QDEC_THRSH_P12000p0us,
    QDEC_THRSH_P24000p0us,
} qdec_thrsh_e;

/**
 * @brief      This function servers to set input port.
 * @param[in]  chn_a - input types of a channel.
 * @param[in]  chn_b - input types of b channel.
 * @return     none.
 */
void qdec_set_pin(qdec_chn_e chn_a, qdec_chn_e chn_b);

/**
 * @brief      This function servers to set qdec mode,qdec mode:common mode and double accuracy mode.
 *             common mode:the qdec counter value is increased/decreased by 1 only when the same rising/falling edges are detected from the two phase signals.
 *             double accuracy mode:the qdec counter value is increased/decreased by 1 on each rising/falling edge of the two phase signals;
 *             the counter value will be increased/decreased by 2 for one wheel rolling.
 * @param[in]  mode - mode type to select.
 * @return     none.
 */
void qdec_set_mode(qdec_mode_e mode);

/**
 * @brief      This function servers to initial qedc source clock.
 * @param[in]  none.
 * @return     none.
 */
void qdec_clk_en(void);

/**
 * @brief      This function servers to read hardware counting value,
 *             After reading the value, if there is no input to channels A and B, then reading the value is 0 through this function,
 *             If the count is not read, the qdec read real time counting value increases or decreases with the direction of the wheel according to the qdec mode.
 * @param[in]  none.
 * @return     hardware counting value.
 */
signed char qdec_get_count_value(void);

/**
 * @brief      This function servers to reset qdec and the qdec counter value is cleared zero.
 * @param[in]  none.
 * @return     none.
 */
void qdec_reset(void);

/**
 * @brief      This function servers to set hardware debouncing.
 * @param[in]  thrsh - any signal with width lower than the threshold will be regarded as jitter,
 *             effective signals input from Channel A and B should contain high/low level with width more than the threshold.
 * @return     none.
 */
void qdec_set_debouncing(qdec_thrsh_e thrsh);
