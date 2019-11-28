/********************************************************************************************************
 * @file     drv_adc.c
 *
 * @brief	 adc driver interface file
 *
 * @author
 * @date     Oct. 8, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
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
 *
 *******************************************************************************************************/
#include "drv_adc.h"



/****
* brief: ADC initiate function, set the ADC clock details (4MHz) and start the ADC clock.
* param[in] null
*
* @return	  1: set success ;
*             0: set error
*/
unsigned char drv_adc_init()
{
#if	defined (MCU_CORE_826x)
	//set the ADC clock details (4MHz) and start the ADC clock.
	return ADC_Init();
#elif defined(MCU_CORE_HAWK)

#else
	adc_init();
	return 1;
#endif
}

/****
* brief: get the sample data
* param[in] null
* @return,the result
*/
unsigned short drv_get_adc_data()
{
#if defined (MCU_CORE_8258)
	return (unsigned short)adc_sample_and_get_result();
#else
	return ADC_SampleValueGet();
#endif
}

#if defined (MCU_CORE_8258)
void drv_adc_mode_pin_set(Drv_ADC_Mode mode, GPIO_PinTypeDef pin)
{
	if(mode == Drv_ADC_BASE_MODE){
		adc_base_init(pin);
	}else if(mode == Drv_ADC_VBAT_MODE){
		adc_vbat_init(pin);
	}
}


void drv_adc_enable(bool enable)
{
	adc_power_on_sar_adc((unsigned char)enable);
}

#else
/****
* brief: Set ADC channel,8269 only support the left and misc channel
* param[in] ad_ch, enum the channel
*
* @return
*/
void drv_adc_channel_set(Drv_ADC_ChTypeDef ad_ch)
{
	if(ad_ch == Drv_ADC_LEFT_CHN)
	{
		ADC2AUDIO();
	}
	else if(ad_ch == Drv_ADC_MISC_CHN)
	{
		AUDIO2ADC();
	}
	else
	{
		//unsupported!
		return;
	}
}

/****
* brief: set the hardware channel of analog input
* param[in] adc_chan, enum the channel,the same as the drv_adc_channel_set
* param[in] mode,single or differential mode
* param[in] pcha_p,the positive input
* param[in] pcha_n,the negative input,if the single mode,it is GND
* @return
*/
void drv_set_mode_Pcha(Drv_ADC_ChTypeDef adc_chan, DRV_ADC_InputModeTypeDef mode, u8 pcha_p, u8 pcha_n)
{
	ADC_AnaModeSet(mode);
	ADC_AnaChSet(pcha_p);
}


/****
* brief: set the sample time cycle
* param[in] adc_chan, enum the channel,the same as the drv_adc_channel_set
* param[in] sample_time,the sample time cycle,reference to the API.
* @return
*/
void drv_set_sample_time(Drv_ADC_ChTypeDef adc_chan,u8 sample_time)
{
	ADC_SampleTimeSet(sample_time);
}


/****
* brief: set the sample reference voltage
* param[in] adc_chan, enum the channel,the same as the drv_adc_channel_set
* param[in] ref_vol,the reference voltage,should be the enum mode,reference to the API.
* @return
*/
void drv_set_ref_vol(Drv_ADC_ChTypeDef adc_chan,u8 ref_vol)
{
	ADC_RefVoltageSet(ref_vol);
}

/****
* brief: set the sample resolution
* param[in] adc_chan, enum the channel,the same as the drv_adc_channel_set
* param[in] res,the reference voltage,should be the enum mode,reference to the API.
* @return
*/
void drv_set_resolution(Drv_ADC_ChTypeDef adc_chan,u8 res)
{
	ADC_ResSet(res);
}

/****
* brief: set the ADC setting parameter
* param[in] adc_chan, enum the channel
* param[in] mode,single or differential mode
* param[in] pcha_p,the positive input
* param[in] pcha_n,the negative input,if the single mode,it is GND
* param[in] sample_time,the sample time cycle,reference to the API.
* param[in] ref_vol,the reference voltage,should be the enum mode,reference to the API.
* param[in] res,the reference voltage,should be the enum mode,reference to the API.
* @return
*/
void drv_ADC_ParamSetting(Drv_ADC_ChTypeDef ad_ch,DRV_ADC_InputModeTypeDef mode,u8 pcha_p, u8 pcha_n,u8 sample_time,u8 ref_vol,u8 res)
{
	drv_adc_channel_set(ad_ch);
	drv_set_mode_Pcha(ad_ch, mode, pcha_p, pcha_n);
	drv_set_sample_time(ad_ch, sample_time);
	drv_set_ref_vol(ad_ch, ref_vol);
	drv_set_resolution(ad_ch, res);
}
#endif
