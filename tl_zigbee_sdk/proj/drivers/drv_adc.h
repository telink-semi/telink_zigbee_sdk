/********************************************************************************************************
 * @file	drv_adc.h
 *
 * @brief	This is the header file for drv_adc
 *
 * @author	Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#pragma once



typedef enum{
	DRV_ADC_BASE_MODE,
	DRV_ADC_VBAT_MODE,
}drv_adc_mode_t;

/****
* brief: ADC initiate function
* param[in] null
*
* @return	  1: set success ;
*             0: set error
*/
bool drv_adc_init(void);

/****
* brief: get the sample data
* param[in] null
* @return,the result
*/
u16 drv_get_adc_data(void);

/****
* brief: set ADC test mode and pin
* param[in]
* @return
*/
#if defined (MCU_CORE_826x)
void drv_adc_mode_pin_set(drv_adc_mode_t mode, ADC_InputPTypeDef pin);
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
void drv_adc_mode_pin_set(drv_adc_mode_t mode, GPIO_PinTypeDef pin);
#elif defined(MCU_CORE_B91)
void drv_adc_mode_pin_set(drv_adc_mode_t mode, adc_input_pin_def_e pin);
#endif

/****
* brief: start/stop ADC
* param[in] TRUE/FALSE
* @return
*/
void drv_adc_enable(bool enable);


