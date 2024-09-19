/********************************************************************************************************
 * @file    adc.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/**	@page ADC
 *
 *	Introduction
 *	===============
 *	B92 supports hardware ADC function.
 *
 *  When the voltage to be sampled exceeds the ADC sampling range, an external voltage divider circuit must be used
 *  to divide the original voltage into the sampling range and then input to the sampling point and sample through the GPIO.
 *  The recommended configuration of the external divider circuit is as follows:
 *  Hardware Configuration: The 500k resistor and 1M resistor are combined to form 1/3 of the external partial voltage circuit (M level).
 *  Software Configuration: Recommended initial settings are  Vref = 1.2V, pre_scale = 1/4, sample_freq =23K.
 *  The difference in sampling steps between normal mode and sleep mode is as follows:
 *  -# Normal mode:
 *     a. Power on the ADC for immediate sampling and then immediately power off the ADC;
 *     b. Delay time greater than 50ms;(Less than this time, the sampling bias will be larger.)
 *     c. Proceed to the next sampling, following the same steps as a and b.
 *  -# Deep mode or deep retention mode:
 *     After adc sampling, it will enter deep mode or deep retention mode and no additional operation is required.
 *  -# Suspend mode:
 *     After adc sampling, it will enter suspend mode and you need to configure the suspend time to be greater than 200ms.
 *
 *	API Reference
 *	===============
 *	Header File: adc.h
 */
#pragma once


#include "dma.h"
#include "audio.h"
#include "compiler.h"
#include "gpio.h"
#include "reg_include/register.h"

#ifndef INTERNAL_TEST_FUNC_EN
#define INTERNAL_TEST_FUNC_EN            0//only for internal test
#endif
typedef enum{
	ADC_VREF_0P9V = 0x01,//Only for internal testing,not recommended.
	ADC_VREF_1P2V = 0x02,
}adc_ref_vol_e;
typedef enum{
	ADC_VBAT_DIV_OFF = 0,
	ADC_VBAT_DIV_1F4 = 0x01,
}adc_vbat_div_e;

typedef enum {
	NOINPUTN = 0,
	ADC_GPIO_PB1N = 0x02,
	ADC_GPIO_PB2N = 0x03,
	ADC_GPIO_PB3N = 0x04,
	ADC_GPIO_PB4N = 0x05,
	ADC_GPIO_PB5N = 0x06,
	ADC_GPIO_PB6N = 0x07,
	ADC_GPIO_PB7N = 0x08,
	ADC_GPIO_PD0N = 0x09,
	ADC_GPIO_PD1N = 0x0a,
	ADC_TEMPSENSORN_EE = 0x0e,
	GND = 0x0f,
}adc_input_nch_e;
typedef enum {
	NOINPUTP = 0,
	ADC_GPIO_PB1P = 0x02,
	ADC_GPIO_PB2P = 0x03,
	ADC_GPIO_PB3P = 0x04,
	ADC_GPIO_PB4P = 0x05,
	ADC_GPIO_PB5P = 0x06,
	ADC_GPIO_PB6P = 0x07,
	ADC_GPIO_PB7P = 0x08,
	ADC_GPIO_PD0P = 0x09,
	ADC_GPIO_PD1P = 0x0a,
	ADC_TEMPSENSORP_EE = 0x0e,
	ADC_VBAT = 0x0f,
}adc_input_pch_e;
/**
 * @brief adc input pin type
 * |           |              |
 * | :-------- | :----------- |
 * |   <15:12> |    <11:0>    |
 * |adc channel|    gpio pin  |
 */
typedef enum{
	ADC_GPIO_PB1 = GPIO_PB1 | (0x2<<12),
	ADC_GPIO_PB2 = GPIO_PB2 | (0x3<<12),
	ADC_GPIO_PB3 = GPIO_PB3 | (0x4<<12),
	ADC_GPIO_PB4 = GPIO_PB4 | (0x5<<12),
	ADC_GPIO_PB5 = GPIO_PB5 | (0x6<<12),
	ADC_GPIO_PB6 = GPIO_PB6 | (0x7<<12),
	ADC_GPIO_PB7 = GPIO_PB7 | (0x8<<12),
	ADC_GPIO_PD0 = GPIO_PD0 | (0x9<<12),
	ADC_GPIO_PD1 = GPIO_PD1 | (0xa<<12),
}adc_input_pin_def_e;
typedef enum{
	ADC_GPIO_MODE,
	ADC_VBAT_MODE,
}adc_input_pin_mode_e;

typedef enum{
	ADC_RES8 = 0,
	ADC_RES10 = 0x01,
	ADC_RES12 = 0x02,
	ADC_RES14 = 0x03,
}adc_res_e;

typedef enum{
	ADC_SAMPLE_CYC_3,
	ADC_SAMPLE_CYC_6,
	ADC_SAMPLE_CYC_9,
	ADC_SAMPLE_CYC_12,
	ADC_SAMPLE_CYC_15,
	ADC_SAMPLE_CYC_18,
	ADC_SAMPLE_CYC_21,
	ADC_SAMPLE_CYC_24,
	ADC_SAMPLE_CYC_27,
	ADC_SAMPLE_CYC_30,
	ADC_SAMPLE_CYC_33,
	ADC_SAMPLE_CYC_36,
	ADC_SAMPLE_CYC_39,
	ADC_SAMPLE_CYC_42,
	ADC_SAMPLE_CYC_45,
	ADC_SAMPLE_CYC_48,
}adc_sample_cycle_e;

typedef enum{
	ADC_SAMPLE_FREQ_23K,
	ADC_SAMPLE_FREQ_48K,
	ADC_SAMPLE_FREQ_96K,
	ADC_SAMPLE_FREQ_192K,
}adc_sample_freq_e;

typedef enum{
	ADC_MISC_CHN	= BIT(2),
}adc_chn_e;

typedef enum{
	ADC_PRESCALE_1   = 0x00,
//	ADC_PRESCALE_1F2 = 0x01,//Only for internal testing
	ADC_PRESCALE_1F4 = 0x02,
//	ADC_PRESCALE_1F8 = 0x03,//Only for internal testing
}adc_pre_scale_e;
enum{
	ADC_MAX_STATE_NUM  = 0x02,
};

/**
 * @brief      This function open sar_adc power.
 * @return   none.
 */
static inline void adc_power_on(void)
{
	analog_write_reg8(areg_adc_pga_ctrl, (analog_read_reg8(areg_adc_pga_ctrl)&(~FLD_SAR_ADC_POWER_DOWN)));
}
/**
 * @brief      This function close sar_adc power.
 * @return     none
 */
static inline void adc_power_off(void)
{
	analog_write_reg8(areg_adc_pga_ctrl, (analog_read_reg8(areg_adc_pga_ctrl)|FLD_SAR_ADC_POWER_DOWN));
}
/**
 * @brief      This function reset adc module
 * @return     none
 */
static inline void adc_reset(void)
{
	reg_rst3 &= (~FLD_RST3_SARADC );
	reg_rst3 |=FLD_RST3_SARADC;
}
/**
 * @brief      This function enable adc source clock: Pad_24M
 * @return     none
 */
static inline void adc_clk_en(void)
{
	analog_write_reg8(areg_adc_clk_setting	, analog_read_reg8(areg_adc_clk_setting	) | FLD_CLK_24M_TO_SAR_EN);
}
/**
 * @brief      This function disable adc source clock: Pad_24M
 * @return     none
 */
static inline void adc_clk_dis(void)
{
	analog_write_reg8(areg_adc_clk_setting	, analog_read_reg8(areg_adc_clk_setting	) & (~FLD_CLK_24M_TO_SAR_EN));
}
/**
 * @brief      This function sets adc sample clk. adc sample clk = 24M/(1+div)  div=5.
 * @param[in]  div - the divider of adc sample clock.
 * @return     none
 * @attention: The ADC clock is fixed at 4MHz and should not be modified.
 */
static inline void adc_set_clk(unsigned char div)
{
	analog_write_reg8(areg_adc_sample_clk_div, div);
}
/**
 * @brief      This function sets ADC input channel as misc channel.
 * @return     none
 */
static inline void adc_set_m_chn_en(void)
{
	analog_write_reg8(areg_adc_chn_en, FLD_ADC_CHN_EN_M | (ADC_MAX_STATE_NUM<<4) );
}
/**
 * @brief This function serves to set resolution.
 * @param[in]  res - enum variable of ADC resolution.
 * @return none
 */
static inline void adc_set_resolution(adc_res_e res)
{
	analog_write_reg8(areg_adc_res_m, (analog_read_reg8(areg_adc_res_m)&(~FLD_ADC_RES_M)) | res);
}

/**
 * @brief      This function serves to set ADC sample time(the number of adc clocks for sample cycles) for the misc channel.
 * @param[in]  sample_cycle - enum variable of adc sample cycles.
 * @return     none
 */
static inline void adc_set_tsample_cycle(adc_sample_cycle_e sample_cycle)
{
	//ana_ee<7:4> is reserved, so no need care its value
	analog_write_reg8(areg_adc_tsmaple_m, sample_cycle);  //optimize, <7:4> not cared
}
/**
 * @brief This function serves to set input channel in differential_mode.
 * @param[in]  p_ain - enum variable of ADC analog positive input channel.
 * @param[in]  n_ain - enum variable of ADC analog negative input channel.
 * @return none
 *
 */
static inline void adc_set_diff_input(adc_input_pch_e p_ain, adc_input_nch_e n_ain)
{
	analog_write_reg8(areg_adc_res_m, analog_read_reg8(areg_adc_res_m) | FLD_ADC_EN_DIFF_CHN_M);
	analog_write_reg8(areg_adc_ain_chn_misc, n_ain | p_ain<<4 );
}
/**
 * @brief This function serves to set state and capture_state length.
 * @param[in]   r_max_mc - Value of length of "capture" state for MISC channel.
 * @param[in]   r_max_s - Value of length of "set" state for MISC channel.
 * @return none
 */
static inline void adc_set_state_length(unsigned short r_max_mc,unsigned char r_max_s)
{
	analog_write_reg8(areg_r_max_mc, r_max_mc);
	analog_write_reg8(areg_r_max_s, ((r_max_mc>>8)<<6)| (r_max_s & FLD_R_MAX_S));
}
/**
 * @brief      This function serves to config adc_dma_chn channel.
 * @param[in]  chn - the DMA channel
 * @param[in]  fifo_chn - audio fifo channel number
 * @attention  sar_adc can use audio's fifo0 or fifo1, but the same fifo cannot be used on sar_adc and audio at the same time.
 * @return     none
 */
void adc_set_dma_config(dma_chn_e chn,audio_fifo_chn_e fifo_chn);
/**
 * @brief     The adc starts sampling in DMA mode.
 * @param[in] adc_data_buf 	- Pointer to data buffer, it must be 4-bytes aligned address
 * 							  and the actual buffer size defined by the user needs to be not smaller than the data_byte_len,
 * 							  otherwise there may be an out-of-bounds problem.
 * @param[in] data_byte_len - Amount of data to be sampled in bytes,it must be multiple of 4. The maximum value can be up to 0xFFFFFC.
 * @return    none
 */
void adc_start_sample_dma(unsigned short *adc_data_buf,unsigned int data_byte_len);
/**
 * @brief This function is used to set IO port for ADC supply or ADC IO port voltage sampling.
 * @param[in]  mode - ADC gpio pin sample mode
 * @param[in]  pin - adc_input_pin_def_e ADC input gpio pin
 * @return none
 */
void adc_pin_config(adc_input_pin_mode_e mode ,adc_input_pin_def_e pin);
/**
 * @brief This function is used to set two IO port configuration and set it as input channel of ADC difference IO port voltage sampling.
 * @param[in]  p_pin - enum variable of ADC analog positive input IO.
 * @param[in]  n_pin - enum variable of ADC analog negative input IO.
 * @return none
 */
void adc_set_diff_pin(adc_input_pin_def_e p_pin, adc_input_pin_def_e n_pin);

/**
 * @brief     This function is serves to set the reference voltage for calibration of GPIO sampling.
 *            ADC calibration environment: GPIO sampling, the vref is 1.2V, the pre_scale is 1/4, and the sampling frequency is 96K.
 * 		      Therefore, the voltage value measured using the calibration interface in this environment is the most accurate.
 * @param[in] data - the reference voltage for calibration of GPIO sampling.
 * @return none
 */
void adc_set_gpio_calib_vref(unsigned short data);

/**
 * @brief  This function is used to set the offset of the two-point calibration of GPIO sampling.
 * @param[in] offset - the offset for two-point calibration of GPIO sampling.
 * @return none
 */
void adc_set_gpio_two_point_calib_offset(signed char offset);


/**
 * @brief     This function is serves to set the reference voltage for calibration of Vbat sampling.
 *            ADC calibration environment: Vbat sampling,the vref is 1.2V, the divider is 1/4, the pre_scale is 1, and the sampling frequency is 96K.
 * 		      Therefore, the voltage value measured using the calibration interface in this environment is the most accurate.
 * @param[in] data - the reference voltage for calibration of Vbat sampling.
 * @return none
 */
void adc_set_vbat_calib_vref(unsigned short data);


/**
 * @brief This function is used to set the offset of the two-point calibration of Vbat sampling.
 * @param[in] offset - the offset for two-point calibration of Vbat sampling.
 * @return none
 */
void adc_set_vbat_two_point_calib_offset(signed char offset);

/**
 * @brief This function serves to set the reference voltage of the channel.
 * @param[in]  v_ref - enum variable of ADC reference voltage.
 * @return none
 */
void adc_set_ref_voltage(adc_ref_vol_e v_ref);
/**
 * @brief This function serves to set the sample frequency.
 * @param[in]  sample_freq - enum variable of ADC sample frequency.
 * @return none
 */
void adc_set_sample_rate(adc_sample_freq_e sample_freq);
/**
 * @brief This function serves to set pre_scaling factor.
 * @param[in]  pre_scale - enum variable of ADC pre_scaling factor.
 * @return none
 */
void adc_set_scale_factor(adc_pre_scale_e pre_scale);
/**
 * @brief This function is used to initialize the ADC for gpio sampling.
 * @param[in]  pin - adc_input_pin_def_e ADC input gpio pin
 * @param[in]  v_ref - enum variable of ADC reference voltage.
 * @param[in]  pre_scale - enum variable of ADC pre_scaling factor.
 * @param[in]  sample_freq - enum variable of ADC sample frequency.
 * @return none
 * @attention
 * If the parameter gpio_v of the sys_init() function is selected as GPIO_VOLTAGE_3V3,
 * gpio voltage sample suggested initial setting are Vref = 1.2V, pre_scale = 1/4, sample_freq =96K,
 * because the chip is factory calibrated for the adc according to this group configuration.
 * 0.9V Vref pre_scale must be 1.
 * The sampling range are as follows:
 * 			Vref        pre_scale        sampling range
 * 			1.2V			1				0 ~ 1.2V
 * 			0.9V            1				0 ~ 0.9V
 * 			1.2V			1/4				0 ~ VOH (the output voltage of GPIO) (suggest)
 * VOH: The factors that affect VOH can be found in the explanation of the vbat_v parameter of the sys_init() function.
 *
 * If the parameter gpio_v of the sys_init() function is selected as GPIO_VOLTAGE_1V8,
 * gpio voltage sample suggested initial setting are Vref = 1.2V, pre_scale = 1/4, sample_freq =96K,
 * because the chip is factory calibrated for the adc according to this group configuration.
 * The sampling range are as follows:
 * 			Vref        pre_scale        sampling range
 * 			1.2V			1/4				0 ~ 1.8V (suggest)
 *
 * @note  In order to switch the pin of the ADC, it can be done by calling the interface 'adc_pin_config' and 'adc_set_diff_input'.
 */
void adc_gpio_sample_init(adc_input_pin_def_e pin,adc_ref_vol_e v_ref,adc_pre_scale_e pre_scale,adc_sample_freq_e sample_freq);

/**
 * @brief  This function is used to initialize the ADC for battery voltage sampling.
 * @return none
 * @attention battery voltage sample suggested initial setting are Vref = 1.2V, pre_scale = 1, vbat_div = 1/4, sample_freq =96K,
 * 		      because the chip is factory calibrated for the adc according to this group configuration.	
 * 		      Which has higher accuracy, user don't need to change it.
 * 			  The battery voltage sample range is 1.9~4.3V.
 *
 * 			  When the GPIO voltage in sys_init() is configured to GPIO_VOLTAGE_1V8, battery voltage sampling can not be used.
 * 			  Users can use external voltage divider instead, the details refer to the Driver SDK Developer Handbook for this chip.
 */
void adc_battery_voltage_sample_init(void);
/**
 * @brief      This function serves to select Vbat voltage division factor.
 * @param[in]  vbat_div - enum variable of Vbat division factor.
 * @return     none
 */
void adc_set_vbat_divider(adc_vbat_div_e vbat_div);
/**
 * @brief This function is used to initialize the ADC.
 * @param[in]  v_ref - enum variable of ADC reference voltage.
 * @param[in]  pre_scale - enum variable of ADC pre_scaling factor.
 * @param[in]  sample_freq - enum variable of ADC sample frequency.
 * @return     none
 * @attention  Many features are configured in adc_init function. But some features
 * 		such as adc_clk, resolution, tsample_cycle, we think better to set as default value,
 * 		and user don't need to change them in most use cases.
 */
void adc_init(adc_ref_vol_e v_ref,adc_pre_scale_e pre_scale,adc_sample_freq_e sample_freq);
/**
 * @brief This function serves to start adc sample and get raw adc sample code.
 * @param[in]   sample_buffer - This parameter is the first address of the received data buffer, which must be 4 bytes aligned, otherwise the program will enter an exception.
 *                              and the actual buffer size defined by the user needs to be not smaller than the sample_num, otherwise there may be an out-of-bounds problem.
 * @param[in]   sample_num 	  - This parameter is used to set the size of the received dma and must be set to a multiple of 4. The maximum value that can be set is 0xFFFFFC.
 * @return 		none
 */
void adc_get_code_dma(unsigned short *sample_buffer, unsigned short sample_num);
/**
 * @brief This function serves to directly get an adc sample code from analog registers.
 * @return 		adc_code 	- the adc sample code.
 */
unsigned short adc_get_code(void);
/**
 * @brief This function serves to calculate voltage from adc sample code.
 * @param[in]   adc_code	- the adc sample code.
 * @return 		adc_vol_mv 	- the average value of adc voltage value.
 */
unsigned short adc_calculate_voltage(unsigned short adc_code);
#if INTERNAL_TEST_FUNC_EN
/**
 * @brief      This function open temperature sensor power.
 * @return     none
 */
static inline void adc_temp_sensor_power_on(void)
{
	analog_write_reg8(areg_temp_sensor_ctrl, (analog_read_reg8(areg_temp_sensor_ctrl)&(~FLD_TEMP_SENSOR_POWER_DOWN)));
}
/**
 * @brief      This function close temperature sensor power.
 * @return     none
 */
static inline void adc_temp_sensor_power_off(void)
{
	analog_write_reg8(areg_temp_sensor_ctrl, (analog_read_reg8(areg_temp_sensor_ctrl)|FLD_TEMP_SENSOR_POWER_DOWN));
}
/**
 * @brief This function servers to initialize ADC temperature sensor.
 * @return     none.
 * @attention Temperature sensor suggested initial setting are Vref = 1.2V, pre_scale = 1, sample_freq =96K.
 * 			  The user don't need to change it.
 */
void adc_temperature_sample_init(void);
/**
 * @brief This function serves to calculate temperature from temperature sensor adc sample code.
 * @param[in]   adc_code	 		- the temperature sensor adc sample code.
 * @return 		adc_temp_value	 	- the temperature value.
 * attention   Temperature and adc_code are linearly related. We test four chips between -40~130 (Celsius) and got an average relationship:
 * 			Temp =  564 - ((adc_code * 819)>>13),when Vref = 1.2V, pre_scale = 1.
 */
unsigned short adc_calculate_temperature(unsigned short adc_code);
#endif
