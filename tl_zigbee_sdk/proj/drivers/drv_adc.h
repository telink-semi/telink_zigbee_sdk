/********************************************************************************************************
 * @file     drv_adc.h
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
#pragma once

#include "platform_includes.h"
#include "../common/types.h"
#include "../common/compiler.h"


/****
* brief: ADC initiate function, set the ADC clock details (4MHz) and start the ADC clock.
* param[in] null
*
* @return	  1: set success ;
*             0: set error
*/
unsigned char drv_adc_init();

/****
* brief: get the sample data
* param[in] null
* @return,the result
*/
unsigned short drv_get_adc_data();


#if defined (MCU_CORE_8258)

typedef enum{
	Drv_ADC_BASE_MODE,
	Drv_ADC_VBAT_MODE,
}Drv_ADC_Mode;

/****
* brief: set ADC test mode and pin
* param[in]
* @return
*/
void drv_adc_mode_pin_set(Drv_ADC_Mode mode, GPIO_PinTypeDef pin);

/****
* brief: set ADC calibrated value
* param[in]
* @return
*/
void drv_adc_set_calValue(unsigned short value);

/****
* brief: start/stop ADC
* param[in] TRUE/FALSE
* @return
*/
void drv_adc_enable(bool enable);

#else

typedef enum{
	Drv_ADC_LEFT_CHN 	= BIT(0),
	Drv_ADC_RIGHT_CHN	= BIT(1),
	Drv_ADC_MISC_CHN	= BIT(2),
	Drv_ADC_RNS_CHN		= BIT(3),
}Drv_ADC_ChTypeDef;

typedef enum{
	Drv_SINGLE_ENDED_MODE = 0,  //single-ended mode
	Drv_DIFFERENTIAL_MODE = 1,  //differential mode
}DRV_ADC_InputModeTypeDef;

/****
* brief: set the ADC setting parameter
* param[in] adc_chan, enum the channel
* param[in] mode,single or differential mode
* param[in] pcha_p,the positive input pin
* param[in] pcha_n,the negative input,if the mode is the single mode,it is GND
* param[in] sample_time,the sample time cycle,reference to the API.
* param[in] ref_vol,the reference voltage,should be the enum mode,reference to the API.
* param[in] res,the sample resolution,should be the enum mode,reference to the API.
* @return
*/
void drv_ADC_ParamSetting(Drv_ADC_ChTypeDef ad_ch,DRV_ADC_InputModeTypeDef mode,u8 pcha_p, u8 pcha_n,u8 sample_time,u8 ref_vol,u8 res);
#endif
