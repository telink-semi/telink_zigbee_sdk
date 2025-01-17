/********************************************************************************************************
 * @file    drv_adc.c
 *
 * @brief   This is the source file for drv_adc
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
#include "../tl_common.h"

#if defined(MCU_CORE_826x)
    #define ADC_VALUE_GET_WITH_BASE_MODE(v)     (3300 * (v - 128)/(16384 - 256))//Vref(mV) * (v - 128)/(2^14 - 2^8)
    #define ADC_VALUE_GET_WITH_VBAT_MODE(v)     (3*(1428*(v - 128)/(16384 - 256)))
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
    #define ADC_SAMPLE_NUM                      8
    #define ADC_SAMPLE_FREQ                     ADC_SAMPLE_FREQ_96K
    #define ADC_SAMPLE_NDMA_DELAY_TIME          ((1000 / ( 6 * (2 << (ADC_SAMPLE_FREQ)))) + 1)//delay 2 sample
#if defined(MCU_CORE_TL721X)
    #define ADC_PRESCALE						ADC_PRESCALE_1F8
#else//b91/b92/tl321x
    #define ADC_PRESCALE						ADC_PRESCALE_1F4
#endif
#endif

#if defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
/**
 * @brief This function serves to get adc sample code by manual and convert to voltage value.
 * @return 		adc_vol_mv_average 	- the average value of adc voltage value.
 */
static u16 adc_get_voltage(void)
{
    u16 adc_sample_buffer[ADC_SAMPLE_NUM] = {0};
    u16 adc_code_average = 0;

    /* get adc sample data */
#if defined(MCU_CORE_B91) || defined(MCU_CORE_B92)
    for (u8 cnt = 0; cnt < ADC_SAMPLE_NUM; cnt++) {
        //wait at least 2 sample cycle(f = 96K, T = 10.4us)
        delay_us(ADC_SAMPLE_NDMA_DELAY_TIME);
        adc_sample_buffer[cnt] = adc_get_code();
    }
#elif defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
    //start
    adc_start_sample_nodma();

    u8 cnt = 0;
    while (cnt < ADC_SAMPLE_NUM) {
        u8 fifoCnt = adc_get_rxfifo_cnt();
        if (fifoCnt) {
            adc_sample_buffer[cnt] = adc_get_code();
            if (adc_sample_buffer[cnt] & BIT(11)) {
                //12 bit resolution, BIT(11) is sign bit, 1 means negative voltage in differential_mode
                adc_sample_buffer[cnt] = 0;
            } else {
                //BIT(10..0) is valid adc code
                adc_sample_buffer[cnt] &= 0x7FF;
            }
            cnt++;
        }
    }
#endif

    /* insert sort and get average value */
    u16 temp = 0;
    s8 i, j;
    for (i = 1; i < ADC_SAMPLE_NUM; i++) {
        if (adc_sample_buffer[i] < adc_sample_buffer[i - 1]) {
            temp = adc_sample_buffer[i];
            adc_sample_buffer[i] = adc_sample_buffer[i - 1];
            for (j = i - 1; j >= 0 && adc_sample_buffer[j] > temp; j--) {
                adc_sample_buffer[j + 1] = adc_sample_buffer[j];
            }
            adc_sample_buffer[j + 1] = temp;
        }
    }

    /* get average value from raw data(abandon 1/4 small and 1/4 big data) */
    for (i = ADC_SAMPLE_NUM >> 2; i < (ADC_SAMPLE_NUM - (ADC_SAMPLE_NUM >> 2)); i++) {
        adc_code_average += adc_sample_buffer[i] / (ADC_SAMPLE_NUM >> 1);
    }

#if defined(MCU_CORE_B91) || defined(MCU_CORE_B92)
    return adc_calculate_voltage(adc_code_average);
#elif defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
    return adc_calculate_voltage(ADC_M_CHANNEL, adc_code_average);
#endif
}
#endif

/****
* brief: ADC initiate function
* param[in] null
*
* @return	  1: set success ;
*             0: set error
*/
bool drv_adc_init(void)
{
#if defined(MCU_CORE_826x)
    ADC_Init();
    AUDIO2ADC();
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
    adc_init();
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92)
    //do nothing
#elif defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
    adc_init(NDMA_M_CHN);
#endif
    return TRUE;
}

/****
* brief: get the sample data
* param[in] null
* @return,the result
*/
u16 drv_get_adc_data(void)
{
#if defined(MCU_CORE_826x)
    u32 tmpSum = 0;
    for (u8 i = 0; i < 8; i++) {
        tmpSum += ADC_SampleValueGet();
    }
    tmpSum /= 8;
    if (tmpSum < 128) {
        tmpSum = 128;
    }
    return (u16)ADC_VALUE_GET_WITH_VBAT_MODE(tmpSum);
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
    return (u16)adc_sample_and_get_result();
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
    return adc_get_voltage();
#else
    return 0;
#endif
}

/****
* brief: Set ADC mode and pin
* param[in] mode, base or vbat mode
* param[in] pin, the pin number
* @return
*/
#if defined(MCU_CORE_826x)
void drv_adc_mode_pin_set(drv_adc_mode_e mode, ADC_InputPTypeDef pin)
{
    if (mode == DRV_ADC_BASE_MODE) {
        ADC_ParamSetting(pin, SINGLEEND, RV_AVDD, RES14, S_3);
    } else if (mode == DRV_ADC_VBAT_MODE) {
        ADC_BatteryCheckInit(Battery_Chn_VCC);
    }
}
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
void drv_adc_mode_pin_set(drv_adc_mode_e mode, GPIO_PinTypeDef pin)
{
    if (mode == DRV_ADC_BASE_MODE) {
        adc_base_init(pin);
    } else if (mode == DRV_ADC_VBAT_MODE) {
        adc_vbat_init(pin);
    }
}
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92)
void drv_adc_mode_pin_set(drv_adc_mode_e mode, adc_input_pin_def_e pin)
{
    if (mode == DRV_ADC_BASE_MODE) {
		adc_gpio_sample_init(pin, ADC_VREF_1P2V, ADC_PRESCALE, ADC_SAMPLE_FREQ);
    } else if (mode == DRV_ADC_VBAT_MODE) {
        /* The battery voltage sample range is 1.8~3.5V,
         * and must set sys_init() function with the mode for battery voltage less than 3.6V.
         * For battery voltage > 3.6V, should take some external voltage divider.
         */
        (void)pin;
        adc_battery_voltage_sample_init();
    }
}
#elif defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
void drv_adc_mode_pin_set(drv_adc_mode_e mode, adc_input_pin_e pin)
{
    if (mode == DRV_ADC_BASE_MODE) {
        adc_gpio_cfg_t adc_gpio_cfg_m;
        adc_gpio_cfg_m.v_ref = ADC_VREF_1P2V;
        adc_gpio_cfg_m.pre_scale = ADC_PRESCALE;
        adc_gpio_cfg_m.sample_freq = ADC_SAMPLE_FREQ;
        adc_gpio_cfg_m.pin = pin;

        adc_gpio_sample_init(ADC_M_CHANNEL, adc_gpio_cfg_m);
    } else if (mode == DRV_ADC_VBAT_MODE) {
        (void)pin;
        adc_vbat_sample_init(ADC_M_CHANNEL);
    }
}
#endif

/**
 * @brief      This function sets sar_adc power.
 * @param[in]  enable enable=1 : power on. enable=0: power off.
 * @return     none
 */
void drv_adc_enable(bool enable)
{
#if defined(MCU_CORE_826x)
    if (enable) {
        EN_ADCCLK;
    } else {
        DIS_ADCCLK;
    }
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
    adc_power_on_sar_adc((unsigned char)enable);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
    if (enable) {
        adc_power_on();
#if defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
		delay_us(30);//Wait >30us after adc_power_on() for ADC to be stable.
#endif
    } else {
        adc_power_off();
    }
#endif
}
