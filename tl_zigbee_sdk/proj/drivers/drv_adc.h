/********************************************************************************************************
 * @file    drv_adc.h
 *
 * @brief   This is the header file for drv_adc
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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


typedef enum {
    DRV_ADC_BASE_MODE,
    DRV_ADC_VBAT_MODE,
} drv_adc_mode_e;

/**
 * @brief  ADC initiate function
 *
 * @param  none
 *
 * @return 1 - success; 0 - error
 */
bool drv_adc_init(void);

/**
 * @brief  Get the sample data
 *
 * @param  none
 *
 * @return the result
 */
u16 drv_get_adc_data(void);

/**
 * @brief  Set ADC test mode and pin
 *
 * @param  mode - base or vbat mode
 * @param  pin  - the pin number
 *
 * @return none
 */
#if defined (MCU_CORE_826x)
void drv_adc_mode_pin_set(drv_adc_mode_e mode, ADC_InputPTypeDef pin);
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
void drv_adc_mode_pin_set(drv_adc_mode_e mode, GPIO_PinTypeDef pin);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
void drv_adc_mode_pin_set(drv_adc_mode_e mode, adc_input_pin_def_e pin);
#elif defined(MCU_CORE_TL323X) || defined(MCU_CORE_TL521X)
void drv_adc_mode_pin_set(drv_adc_mode_e mode, sd_adc_p_input_pin_def_e pin);
#endif

/**
 * @brief  Set ADC power.
 *
 * @param  enable - power on or off
 *
 * @return none
 */
void drv_adc_enable(bool enable);

