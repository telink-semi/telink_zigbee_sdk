/********************************************************************************************************
 * @file    adc.c
 *
 * @brief   This is the source file for B85
 *
 * @author  Driver & Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
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

#include "adc.h"
#include "dfifo.h"
#include "timer.h"
#include "compiler.h"

#define ADC_PATCH_SAMPLE_NUM		8 //8 /4
extern _attribute_data_retention_ adc_vref_ctr_t adc_vref_cfg;
extern volatile unsigned short	adc_code;
extern unsigned char  adc_pre_scale;

_attribute_ram_code_ unsigned int adc_get_result_with_fluct(unsigned int *v){
	unsigned short temp;
	volatile signed int adc_data_buf[ADC_PATCH_SAMPLE_NUM];  //size must 16 byte aligned(16/32/64...)

	int i,j;
	unsigned int  adc_vol_mv = 0;
	unsigned short adc_sample[ADC_PATCH_SAMPLE_NUM] = {0};
	unsigned short  adc_result=0;

	adc_reset_adc_module();
	

	for(i=0;i<ADC_PATCH_SAMPLE_NUM;i++){   	//dfifo data clear
		adc_data_buf[i] = 0;
	}
	//dfifo setting will lose in suspend/deep, so we need config it every time
	adc_config_misc_channel_buf((unsigned short *)adc_data_buf, ADC_PATCH_SAMPLE_NUM<<2);  //size: ADC_PATCH_SAMPLE_NUM*4
	dfifo_enable_dfifo2();
	unsigned int t0 = clock_time();
	if(ADC_SAMPLE_RATE_SELECT==ADC_SAMPLE_RATE_23K)
	{
		while(!clock_time_exceed(t0, 90));  //wait at least 2 sample cycle(f = 23K, T = 43.4us)
	}
	else if(ADC_SAMPLE_RATE_SELECT==ADC_SAMPLE_RATE_96K)
	{
		while(!clock_time_exceed(t0, 25));  //wait at least 2 sample cycle(f = 96K, T = 10.4us)
	}
//////////////// get adc sample data and sort these data ////////////////
	for(i=0;i<ADC_PATCH_SAMPLE_NUM;i++){
		/*wait for new adc sample data, When the data is not zero and more than 1.5 sampling times (when the data is zero),
	 The default data is already ready.*/


		if(ADC_SAMPLE_RATE_SELECT==ADC_SAMPLE_RATE_23K)
		{
			while(!clock_time_exceed(t0, 90));  //wait at least 2 sample cycle(f = 23K, T = 43.4us)
		}
		else if(ADC_SAMPLE_RATE_SELECT==ADC_SAMPLE_RATE_96K)
		{
			while(!clock_time_exceed(t0, 25));  //wait at least 2 sample cycle(f = 96K, T = 10.4us)
		}

		t0 = clock_time();
		if(adc_data_buf[i] & BIT(13)){  //14 bit resolution, BIT(13) is sign bit, 1 means negative voltage in differential_mode
			adc_sample[i] = 0;
		}
		else{
			adc_sample[i] = ((unsigned short)adc_data_buf[i] & 0x1FFF);  //BIT(12..0) is valid adc result
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
	dfifo_disable_dfifo2();   //misc channel data dfifo disable
///// get average value from raw data(abandon some small and big data ), then filter with history data //////
#if (ADC_PATCH_SAMPLE_NUM == 4)  	//use middle 2 data (index: 1,2)
	unsigned int adc_average = (adc_sample[1] + adc_sample[2])/2;
#elif(ADC_PATCH_SAMPLE_NUM == 8) 	//use middle 4 data (index: 2,3,4,5)
	unsigned int adc_average = (adc_sample[2] + adc_sample[3] + adc_sample[4] + adc_sample[5])/4;
#endif
	adc_code=adc_result = adc_average;


	 //////////////// adc sample data convert to voltage(mv) ////////////////
	//                          (Vref, adc_pre_scale)   (BIT<12~0> valid data)
	//			 =  adc_result * Vref * adc_pre_scale / 0x2000
	//           =  adc_result * adc_pre_scale*Vref >>13

	adc_vol_mv  = (adc_result * adc_pre_scale*adc_vref_cfg.adc_vref)>>13;

	*v = (((adc_sample[ADC_PATCH_SAMPLE_NUM-1] - adc_sample[0]) * adc_pre_scale*adc_vref_cfg.adc_vref)>>13);

	return adc_vol_mv;
}
