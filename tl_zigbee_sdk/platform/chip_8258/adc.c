/********************************************************************************************************
 * @file     adc.c
 *
 * @brief    This is the ADC driver file for TLSR8258
 *
 * @author   junyuan.zhang@telink-semi.com;junwei.lu@telink-semi.com
 * @date     May 8, 2018
 *
 * @par      Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 * @par      History:
 * 			 1.initial release(DEC. 26 2018)
 *
 * @version  A001
 *
 *******************************************************************************************************/

#include "adc.h"
#include "dfifo.h"
#include "analog.h"
#include "clock.h"

volatile unsigned short adc_cal_value = 0xffff;


const GPIO_PinTypeDef ADC_GPIO_tab[10] = {
		GPIO_PB0,GPIO_PB1,
		GPIO_PB2,GPIO_PB3,
		GPIO_PB4,GPIO_PB5,
		GPIO_PB6,GPIO_PB7,
		GPIO_PC4,GPIO_PC5
};

/**
 * @brief This function serves to ADC init.
 * @param[in]   none
 * @return none
 */
void adc_init(void ){

	/****** sar adc Reset ********/
	//reset whole digital adc module
	adc_reset_adc_module();

	/******enable signal of 24M clock to sar adc********/
	adc_enable_clk_24m_to_sar_adc(1);

	/******set adc clk as 4MHz******/
	adc_set_sample_clk(5);

	/******set adc L R channel Gain Stage bias current trimming******/
	pga_left_chn_power_on(0);
	pga_right_chn_power_on(0);
	adc_set_left_gain_bias(GAIN_STAGE_BIAS_PER100);
	adc_set_right_gain_bias(GAIN_STAGE_BIAS_PER100);

	dfifo_disable_dfifo2();
}

/**
 * @brief This function is used for IO port configuration of ADC IO port voltage sampling.
 * @param[in]   GPIO_PinTypeDef pin
 * @return none
 */
void adc_gpio_set(GPIO_PinTypeDef pin)
{
	//ADC GPIO Init
	gpio_set_func(pin, AS_GPIO);
	gpio_set_input_en(pin,0);
	gpio_set_output_en(pin,0);
	gpio_write(pin,0);
}

/**
 * @brief This function is used for IO port configuration of ADC supply voltage sampling.
 * @param[in]   GPIO_PinTypeDef pin
 * @return none
 */
void adc_vbat_set(GPIO_PinTypeDef pin)
{
	gpio_set_func(pin, AS_GPIO);
	gpio_set_input_en(pin,0);
	gpio_set_output_en(pin,1);
	gpio_write(pin,1);
}

/**
 * @brief This function is used for ADC configuration of ADC IO voltage sampling.
 * @param[in]   GPIO_PinTypeDef pin
 * @return none
 */
void adc_base_init(GPIO_PinTypeDef pin)
{
	unsigned char i;
	unsigned char gpio_no=0;

	//set R_max_mc,R_max_c,R_max_s
	adc_set_length_capture_state_for_chn_misc_rns(0xf0);						//max_mc
//	adc_set_length_capture_state_for_chn_left_right(AMIC_ADC_SampleLength[0]);	//max_c	96K
	adc_set_length_set_state(0x0a);												//max_s

	//set total length for sampling state machine and channel
	adc_set_chn_enable(ADC_MISC_CHN);
	adc_set_max_state_cnt(0x02);

	//set channel Vref
//	adc_set_ref_voltage(ADC_LEFT_CHN, ADC_VREF_0P6V);
//	adc_set_ref_voltage(ADC_RIGHT_CHN, ADC_VREF_0P6V);
	adc_set_ref_voltage(ADC_MISC_CHN, ADC_VREF_1P2V);

	//set Vbat divider select,
	adc_set_vref_vbat_divider(ADC_VBAT_DIVIDER_OFF);

	//set channel mode and channel
	adc_gpio_set(pin);		//ADC GPIO Init
	for(i=0;i<10;i++)
	{
		if(pin == ADC_GPIO_tab[i])
		{
			gpio_no = i+1;
			break;
		}
	}
#if ADC_SINGLE_END
	adc_set_input_mode(ADC_MISC_CHN, SINGLE_ENDED_MODE);
	adc_set_ain_channel_single_ended_input_mode(ADC_MISC_CHN, gpio_no);
#else
	adc_set_input_mode(ADC_MISC_CHN, DIFFERENTIAL_MODE);
	adc_set_ain_channel_differential_mode(ADC_MISC_CHN, gpio_no, GND);
#endif

	//set resolution for RNG
	adc_set_resolution(ADC_MISC_CHN, RES14);

	//Number of ADC clock cycles in sampling phase
	adc_set_tsample_cycle(ADC_MISC_CHN, SAMPLING_CYCLES_6);
	//set Analog input pre-scaling and
	adc_set_ain_pre_scaler(ADC_PRESCALER_1F8);

	//set RNG mode
	adc_set_mode(NORMAL_MODE);

}

/**
 * @brief This function is used for ADC configuration of ADC supply voltage sampling.
 * @param[in]   GPIO_PinTypeDef pin
 * @return none
 */
void adc_vbat_init(GPIO_PinTypeDef pin)
{
	unsigned char i;
	unsigned char gpio_no=0;

	//set R_max_mc,R_max_c,R_max_s
	adc_set_length_capture_state_for_chn_misc_rns(0xf0);		 //max_mc
	adc_set_length_set_state(0x0a);									//max_s

	//set total length for sampling state machine and channel
	adc_set_chn_enable(ADC_MISC_CHN);
	adc_set_max_state_cnt(0x02);

	//set channel Vref
	adc_set_ref_voltage(ADC_MISC_CHN, ADC_VREF_1P2V);

	//set Vbat divider select,
	adc_set_vref_vbat_divider(ADC_VBAT_DIVIDER_OFF);

	//set channel mode and channel
	adc_vbat_set(pin);
	for(i=0;i<10;i++)
	{
		if(pin == ADC_GPIO_tab[i])
		{
			gpio_no = i+1;
			break;
		}
	}
#if ADC_SINGLE_END
	adc_set_input_mode(ADC_MISC_CHN, SINGLE_ENDED_MODE);
	adc_set_ain_channel_single_ended_input_mode(ADC_MISC_CHN, gpio_no);
#else
	adc_set_input_mode(ADC_MISC_CHN, DIFFERENTIAL_MODE);
	adc_set_ain_channel_differential_mode(ADC_MISC_CHN, gpio_no, GND);
#endif

	//set resolution for RNG
	adc_set_resolution(ADC_MISC_CHN, RES14);

	//Number of ADC clock cycles in sampling phase
	adc_set_tsample_cycle(ADC_MISC_CHN, SAMPLING_CYCLES_6);

	//set Analog input pre-scaling and
	adc_set_ain_pre_scaler(ADC_PRESCALER_1F8);

	//set RNG mode
	adc_set_mode(NORMAL_MODE);

}


/**
 * @brief This function serves to set the channel reference voltage.
 * @param[in]   enum variable of ADC input channel.
 * @param[in]   enum variable of ADC reference voltage.
 * @return none
 */
void adc_set_ref_voltage(ADC_ChTypeDef ch_n, ADC_RefVolTypeDef v_ref)
{
	if(ch_n & ADC_LEFT_CHN)
	{
		adc_set_vref_chn_left(v_ref);
	}
	if(ch_n & ADC_RIGHT_CHN)
	{
		adc_set_vref_chn_right(v_ref);
	}
	if(ch_n & ADC_MISC_CHN)
	{
		adc_set_vref_chn_misc(v_ref);
	}


	if(v_ref == ADC_VREF_1P2V)
	{
		//Vref buffer bias current trimming: 		150%
		//Vref buffer bias current trimming: 		150%
		//Comparator preamp bias current trimming:  100%
		analog_write(anareg_ain_scale, (analog_read(anareg_ain_scale)&(0xC0)) | 0x3d );
	}
	else
	{
		//Vref buffer bias current trimming: 		100%
		//Vref buffer bias current trimming: 		100%
		//Comparator preamp bias current trimming:  100%
		analog_write(anareg_ain_scale, (analog_read(anareg_ain_scale)&(0xC0)) | 0x15 );
	}

}

/**
 * @brief This function serves to set resolution.
 * @param[in]   enum variable of ADC input channel.
 * @param[in]   enum variable of ADC resolution.
 * @return none
 */
void adc_set_resolution(ADC_ChTypeDef ch_n, ADC_ResTypeDef v_res)
{
	if(ch_n & ADC_LEFT_CHN)
	{
		adc_set_resolution_chn_left(v_res);
	}
	if(ch_n & ADC_RIGHT_CHN)
	{
		adc_set_resolution_chn_right(v_res);
	}
	if(ch_n & ADC_MISC_CHN)
	{
		adc_set_resolution_chn_misc(v_res);
	}
}

/**
 * @brief This function serves to set sample_cycle.
 * @param[in]   enum variable of ADC input channel.
 * @param[in]   enum variable of ADC Sampling cycles.
 * @return none
 */
void adc_set_tsample_cycle(ADC_ChTypeDef ch_n, ADC_SampCycTypeDef adcST)
{
	if(ch_n & ADC_LEFT_CHN)
	{
		adc_set_tsample_cycle_chn_left(adcST);
	}
	if(ch_n & ADC_RIGHT_CHN)
	{
		adc_set_tsample_cycle_chn_right(adcST);
	}
	if(ch_n & ADC_MISC_CHN)
	{
		adc_set_tsample_cycle_chn_misc(adcST);
	}
}

/**
 * @brief This function serves to set input_mode.
 * @param[in]   enum variable of ADC input channel.
 * @param[in]   enum variable of ADC channel input mode.
 * @return none
 */
void adc_set_input_mode(ADC_ChTypeDef ch_n,  ADC_InputModeTypeDef m_input)
{
	if(ch_n & ADC_LEFT_CHN)
	{
		adc_set_input_mode_chn_left(m_input);
	}
	if(ch_n & ADC_RIGHT_CHN)
	{
		adc_set_input_mode_chn_right(m_input);
	}
	if(ch_n & ADC_MISC_CHN)
	{
		adc_set_input_mode_chn_misc(m_input);
	}
}

/**
 * @brief This function serves to set input channel in single_ended_input_mode.
 * @param[in]   enum variable of ADC input channel.
 * @param[in]   enum variable of ADC analog positive input channel.
 * @return none
 */
void adc_set_ain_channel_single_ended_input_mode(ADC_ChTypeDef ch_n, ADC_InputPchTypeDef InPCH)
{

	if(ch_n & ADC_LEFT_CHN)
	{
		adc_set_ain_negative_chn_left(GND);
		adc_set_ain_positive_chn_left(InPCH);
		adc_set_input_mode_chn_left(SINGLE_ENDED_MODE);
	}
	if(ch_n & ADC_RIGHT_CHN)
	{
		adc_set_ain_negative_chn_right(GND);
		adc_set_ain_positive_chn_right(InPCH);
		adc_set_input_mode_chn_right(SINGLE_ENDED_MODE);
	}
	if(ch_n & ADC_MISC_CHN)
	{
		adc_set_ain_negative_chn_misc(GND);
		adc_set_ain_positive_chn_misc(InPCH);
		adc_set_input_mode_chn_misc(SINGLE_ENDED_MODE);
	}
}

/**
 * @brief This function serves to set input channel in differential_mode.
 * @param[in]   enum variable of ADC input channel.
 * @param[in]   enum variable of ADC analog positive input channel.
 * @param[in]   enum variable of ADC analog negative input channel.
 * @return none
 */
void adc_set_ain_channel_differential_mode(ADC_ChTypeDef ch_n, ADC_InputPchTypeDef InPCH,ADC_InputNchTypeDef InNCH)
{

	if(ch_n & ADC_LEFT_CHN)
	{
		adc_set_ain_negative_chn_left(InNCH);
		adc_set_ain_positive_chn_left(InPCH);
		adc_set_input_mode_chn_left(DIFFERENTIAL_MODE);
	}
	if(ch_n & ADC_RIGHT_CHN)
	{
		adc_set_ain_negative_chn_right(InNCH);
		adc_set_ain_positive_chn_right(InPCH);
		adc_set_input_mode_chn_right(DIFFERENTIAL_MODE);
	}
	if(ch_n & ADC_MISC_CHN)
	{
		adc_set_ain_negative_chn_misc(InNCH);
		adc_set_ain_positive_chn_misc(InPCH);
		adc_set_input_mode_chn_misc(DIFFERENTIAL_MODE);
	}
}

/**
 * @brief This function serves to set state length.
 * @param[in]   Value of length of "capture" state for RNS & MISC channel.
 * @param[in]   Value of length of "capture" state for L & R channel.
 * @param[in]   Value of length of "capture" state for L & R & MISC channel.
 * @return none
 */
void adc_set_state_length(unsigned short R_max_mc, unsigned short R_max_c,unsigned char R_max_s)
{
	unsigned char data[3]={0};
	if(R_max_mc&0x3ff)    //r_max_mc[9:0]serves to set length of state for RNS and Misc channel.
	{
		data[0] = (unsigned char)R_max_mc;
		data[2] = (unsigned char)(R_max_mc>>2)&0xc0;
	}
	if(R_max_c&0x3ff)     //r_max_c*9:0+ serves to set length of  state for left and right channel.
	{
		data[1] = (unsigned char)R_max_c;
		data[2] |= (unsigned char)(R_max_c>>4)&0x30;
	}
	if(R_max_s)     //r_max_s serves to set length of  state for left, right and Misc channel.
	{
		data[2] |= (unsigned char)(R_max_s&0x0f);
	}

	WriteAnalogReg(anareg_r_max_mc, data[0]);			//
	WriteAnalogReg(anareg_r_max_c, 	data[1]);			//
	WriteAnalogReg(anareg_r_max_s,  data[2]);			//
}


/**
 * @brief This function serves to set pre_scaling.
 * @param[in]   enum variable of ADC pre_scaling factor.
 * @return none
 */
void adc_set_ain_pre_scaler(ADC_PreScalingTypeDef v_scl)
{


	analog_write(anareg_ain_scale, (analog_read(anareg_ain_scale)&(~FLD_SEL_AIN_SCALE)) | (v_scl<<6) );

	//setting adc_sel_atb ,if stat is 0,clear adc_sel_atb,else set adc_sel_atb[0]if(stat)
	unsigned char tmp;
	if(v_scl)
	{
		//ana_F9<4> must be 1
		tmp = ReadAnalogReg(0xF9);
		tmp = tmp|0x10;                    //open tmp = tmp|0x10;
		WriteAnalogReg (0xF9, tmp);
	}
	else
	{
		//ana_F9 <4> <5> must be 0
		tmp = ReadAnalogReg(0xF9);
		tmp = tmp&0xcf;
		WriteAnalogReg (0xF9, tmp);
	}

}




#define ADC_SAMPLE_NUM		8    //4, 8

/**
 * @brief This function serves to set adc sampling and get results.
 * @param[in]  none.
 * @return the result of sampling.
 */
unsigned int adc_sample_and_get_result(void)
{
	unsigned short temp;
	volatile signed int adc_dat_buf[ADC_SAMPLE_NUM];  //size must 16 byte aligned(16/32/64...)
	unsigned int adc_vol_mv = 0;
	int i,j;

//	adc_power_on_sar_adc(1);
	adc_reset_adc_module();
	unsigned int t0 = clock_time();

	unsigned short adc_sample[ADC_SAMPLE_NUM] = {0};
	unsigned int adc_result;


	for(i=0;i<ADC_SAMPLE_NUM;i++){   	//dfifo data clear
		adc_dat_buf[i] = 0;
	}
	while(!clock_time_exceed(t0, 25));  //wait at least 2 sample cycle(f = 96K, T = 10.4us)

	//dfifo setting will lose in suspend/deep, so we need config it every time
	adc_config_misc_channel_buf((unsigned short *)adc_dat_buf, ADC_SAMPLE_NUM<<2);  //size: ADC_SAMPLE_NUM*4
	dfifo_enable_dfifo2();



//////////////// get adc sample data and sort these data ////////////////
	for(i=0;i<ADC_SAMPLE_NUM;i++){
		while((!adc_dat_buf[i])&&(!clock_time_exceed(t0,16)));  //wait for new adc sample data,
															   //When the data is not zero or more than 1.5 sampling times (when the data is zero),The default data is already ready.
		t0 = clock_time();


		if(adc_dat_buf[i] & BIT(13)){  //14 bit resolution, BIT(13) is sign bit, 1 means negative voltage in differential_mode
			adc_sample[i] = 0;
		}
		else{
			adc_sample[i] = ((unsigned short)adc_dat_buf[i] & 0x1FFF);  //BIT(12..0) is valid adc result
		}


		//insert sort
		if(i){
			if(adc_sample[i] < adc_sample[i-1]){
				temp = adc_sample[i];
				adc_sample[i] = adc_sample[i-1];
				for(j=i-1;j>=0 && adc_sample[j] > temp;j--){
					adc_sample[j+1] = adc_sample[j];
				}
				adc_sample[j+1] = temp;
			}
		}
	}
//////////////////////////////////////////////////////////////////////////////

//	adc_power_on_sar_adc(0);
	dfifo_disable_dfifo2();   //misc channel data dfifo disable




///// get average value from raw data(abandon some small and big data ), then filter with history data //////
#if (ADC_SAMPLE_NUM == 4)  	//use middle 2 data (index: 1,2)
	unsigned int adc_average = (adc_sample[1] + adc_sample[2])/2;
#elif(ADC_SAMPLE_NUM == 8) 	//use middle 4 data (index: 2,3,4,5)
	unsigned int adc_average = (adc_sample[2] + adc_sample[3] + adc_sample[4] + adc_sample[5])/4;
#endif


	adc_result = adc_average;

	//////////////// adc sample data convert to voltage(mv) ////////////////
	//                         (1175mV (Vref), 1/8 scaler)   (BIT<12~0> valid data)
	//			 =  adc_result * adc_cal_value(Vref) >> 10
	//           =  adc_result * 1175(Vref) >> 10
	if((adc_cal_value!=0xffff)&&(adc_cal_value != 0x0000))  //Already calibrated
	{
		adc_vol_mv  = (adc_result * adc_cal_value) >> 10;
	}
	else
	{
		adc_vol_mv = (adc_result * 1175) >> 10;
	}

	return adc_vol_mv;
}


