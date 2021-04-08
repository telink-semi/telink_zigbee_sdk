/********************************************************************************************************
 * @file	audio.h
 *
 * @brief	This is the header file for B87
 *
 * @author	Driver & Zigbee Group
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
#ifndef audio_H
#define audio_H


#include "register.h"
#include "i2c.h"
#include "analog.h"


#define AUDIO_DBL_BUF_ENABLE	0

/**
 * define audio rate value.
 */
typedef enum{
	AUDIO_8K,
	AUDIO_16K,
	AUDIO_32K,
	AUDIO_48K,
	RATE_SIZE
}AudioRate_Typedef;


/**
 * define audio output type.
 */
typedef enum{
	GPIO_PB4_PB5,
	GPIO_PB6_PB7,
}Audio_SDM_OutPut_Pin;


/**
 * define audio input type.
 */
typedef enum{
	AMIC,
	DMIC,
	I2S_IN,
	USB_IN,
	BUF_IN,
}AudioInput_Typedef;

/**
 * define codec mode.
 */
typedef enum{
	CODEC_MODE_MICPHONE_TO_HEADPHONE_LINEOUT_I2S,
	CODEC_MODE_LINE_TO_HEADPHONE_LINEOUT_I2S,
	CODEC_MODE_I2S_TO_HEADPHONE_LINEOUT,
}CodecMode_Typedef;


/**
 * @brief     This function serves to reset audio Module
 * @param[in] none.
 * @return    none
 */
static inline void audio_reset(void)
{
	reg_rst2 = FLD_RST2_AUD;
	reg_rst2 = 0;
}

/**
 * 	@brief      This function serves to set the clock of codec,
 * 	            codec clk is fixd as 24MHz, codec_clk=48M*codec_step[6:0]/codec_mod[7:0]
 * 	@param[in]  step - the dividing factor of step.
 * 	@param[in]  mod - the dividing factor of mod.
 * 	@return     none
 */
static inline void audio_set_codec_clk(unsigned char step,unsigned char mod)
{
	reg_codec_clk_step = step;
	reg_codec_clk_mod = mod;
}

/**
 * 	@brief      This function serves to set the clock of i2s
 * 	@param[in]  step - the dividing factor of step.
 * 	@param[in]  mod - the dividing factor of mod.
 * 	@return     none
 */
static inline void audio_set_i2s_clk(unsigned char step,unsigned char mod)
{
	reg_i2s_step = step|FLD_I2S_CLK_EN;
	reg_i2s_mod = mod;
}
/**
 * @brief      This function performs to read pointer/write pointer location.
 * @param[in]  none.
 * @return     the address of read/write.
 */
static inline unsigned short get_mic_wr_ptr (void)
{
	return reg_audio_wptr >>1;
}
/**
 * 	@brief     audio amic initial function. configure ADC corresponding parameters. set hpf,lpf and decimation ratio.
 * 	@param[in] Audio_Rate - audio rate value
 * 	@return    none
 */
void audio_amic_init(AudioRate_Typedef Audio_Rate);

/**
 * @brief     This function servers to close PGA input and CODEC clock.After calling the Audio module,
 *            you need to call this API to close Audio,to get the current back to the call.
 * @param[in] none.
 * @return    none.
 */
void audio_codec_and_pga_disable(void);

/**
 * @brief     This function servers to receive data from buffer.
 * @param[in] buf - the buffer in which the data need to write
 * @param[in] len - the length of the buffer.
 * @return    none.
 */
void audio_rx_data_from_buff(signed char* buf,unsigned int len);

/**
 * @brief     This function servers to receive data from sample buffer by 16 bits.
 * @param[in] buf - the buffer in which the data need to write
 * @param[in] len - the length of the buffer by short.
 * @return    none.
 */
void audio_rx_data_from_sample_buff(const short *buf, unsigned int len);

/**
 * 	@brief     audio DMIC init function, config the speed of DMIC and downsample audio data to required speed.         actually audio data is dmic_speed/d_samp.
 * 	@param[in] Audio_Rate - set the DMIC speed. such as 1 indicate 1M and 2 indicate 2M.
 * 	@return    none.
 */
void audio_dmic_init(AudioRate_Typedef Audio_Rate);
/**
 * 	@brief     audio USB init function, config the speed of DMIC and downsample audio data to required speed.
 *            actually audio data is dmic_speed/d_samp.
 * 	@param[in] Audio_Rate - set the DMIC speed. such as 1 indicate 1M and 2 indicate 2M.
 * 	@return    none.
 */
void audio_usb_init(AudioRate_Typedef Audio_Rate);

/**
 * @brief     audio buff init function, config the speed of DMIC and downsample audio data to required speed.
 *            actually audio data is dmic_speed/d_samp.
 * @param[in] Audio_Rate  - audio rate.
 * @return    none.
 */
void audio_buff_init(AudioRate_Typedef Audio_Rate);

/**
 * 	@brief     audio I2S init in function, config the speed of i2s and MCLK to required speed.
 * 	@param[in] none.
 * 	@return    none.
 */
void audio_i2s_init(void);


/**
*	@brief	   sdm setting function, enable or disable the sdm output, configure SDM output paramaters.
*	@param[in]  OutPin - SDM Output pin.
*	@param[in]	InType -	  SDM input type, such as AMIC,DMIC,I2S_IN,USB_IN.
*	@param[in]	Audio_Rate - audio sampling rate, such as 16K,32k etc.
*	@param[in]	audio_out_en - audio output enable or disable set, '1' enable audio output; '0' disable output.
*	@return	none
*/
void audio_set_sdm_output(Audio_SDM_OutPut_Pin OutPin,AudioInput_Typedef InType,AudioRate_Typedef Audio_Rate,unsigned char audio_out_en);

/**
 * @brief     This function servers to set USB input/output.
 * @param[in] none.
 * @return    none.
 */
void audio_set_usb_output(void);

/**
 * 	@brief	   	i2s setting function, enable or disable the i2s output, configure i2s output paramaters
 * 	@param[in] 	InType		- select audio input type including amic ,dmic ,i2s and usb
 * 	@param[in] 	Audio_Rate 	- select audio rate, which will decide on which adc sampling rate and relative decimation configuration will be chosen.
 * 	@return	  	none
 */
void audio_set_i2s_output(AudioInput_Typedef InType,AudioRate_Typedef Audio_Rate);

/**
 * 	@brief     This function serves to set I2S input.
 * 	@param[in] i2c_pin_group - select the pin for I2S.
 * 	@param[in] CodecMode - select I2S mode.
 * 	@param[in] sysclk - system clock.
 * 	@return    none.
 */

void audio_set_codec(I2C_GPIO_SdaTypeDef sda_pin,I2C_GPIO_SclTypeDef scl_pin, CodecMode_Typedef CodecMode, unsigned int sysclk);

/**
 * 	@brief     This function serves to set MUTE PGA.
 * 	@param[in] ebable - enable or disable MUTE PGA
 * 	@return    none.
 */
static inline void audio_set_mute_pga(unsigned char enable)
{
	if(!enable)
	{
		analog_write(codec_ana_cfg2,analog_read(codec_ana_cfg2) | BIT(5));
	}
	else
	{
		analog_write(codec_ana_cfg2,analog_read(codec_ana_cfg2) & ~BIT(5));//0a//2a
	}
}

#endif
