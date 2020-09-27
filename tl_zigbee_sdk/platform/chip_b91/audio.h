/********************************************************************************************************
 * @file	audio.h
 *
 * @brief	This is the header file for B91
 *
 * @author	D.M.H
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
/**	@page AUDIO
 *
 *	Introduction
 *	===============
 *	audio module consists of 3 parts: audio code, audio in path , and audio out path.
 *
 *	API Reference
 *	===============
 *	Header File: audio.h
 */
#ifndef audio_H
#define audio_H

#include "reg_include/register_b91.h"
#include "i2c.h"
#include "pwm.h"
#include "compiler.h"

typedef enum{
	I2S_BCK_PC3      = GPIO_PC3,
	I2S_ADC_LR_PC4   = GPIO_PC4,
	I2S_ADC_DAT_PC5  = GPIO_PC5,
	I2S_DAC_LR_PC6   = GPIO_PC6,
	I2S_DAC_DAT_PC7  = GPIO_PC7,
}i2s_pin_e;


typedef enum{
	DMIC_GROUPB_B2_DAT_B3_B4_CLK,
	DMIC_GROUPC_C1_DAT_C2_C3_CLK,
	DMIC_GROUPD_D4_DAT_D5_D6_CLK,
}dmic_pin_group_e;


typedef enum{
	MIC_MODE,
	SPK_MODE,
	LOOP_MODE,
}audio_loop_mode_e;


typedef enum{
	CODEC_I2S ,
	IO_CODEC ,
	IO_I2S,
}audio_flow_e;


typedef enum{
	I2S_RJ_MODE ,
	I2S_LJ_MODE ,
	I2S_I2S_MODE,
	I2S_DSP_MODE,
}i2s_mode_select_e;

typedef enum{
	CODEC_PARALLEL_MODE ,
	CODEC_LJ_MODE ,
	CODEC_DSP_MODE,
	CODEC_I2S_MODE,
}codec_mode_select_e;


typedef enum{
	CODEC_ITF_AC,
	CODEC_ITF_PD,
}codec_interface_select_e;


typedef enum{
	AMIC_IN,
	DMIC_IN,
	LINE_IN,
}audio_input_mode_e;


typedef enum{
   	BIT_16_MONO,
	BIT_20_OR_24_MONO ,
	BIT_16_STEREO,
	BIT_20_OR_24_STEREO ,
}audio_in_mode_e;

typedef enum{
   	BIT_16_MONO_FIFO0 ,
	BIT_20_OR_24_MONO_FIFO0 ,
	BIT_16_STEREO_FIFO0 ,
	BIT_20_OR_24_STEREO_FIFO0 ,
	BIT_16_MONO_FIFO1 ,
	BIT_20_OR_24_MONO_FIFO1 ,
	BIT_16_STEREO_FIFO0_AND_FIFO1 ,
	BIT_20_OR_24STEREO_FIFO0_AND_FIFO1 ,

}audio_out_mode_e;

typedef enum{
	I2S_BIT_16_DATA,
	I2S_BIT_20_DATA ,
	I2S_BIT_24_DATA,
}i2s_data_select_e;

typedef enum{
	CODEC_BIT_16_DATA,
	CODEC_BIT_20_DATA=2,
	CODEC_BIT_24_DATA,
}codec_data_select_e;


typedef enum{
    MONO_BIT_16,
	MONO_BIT_20,
	MONO_BIT_24,
	STEREO_BIT_16,
	STEREO_BIT_20,
	STEREO_BIT_24,
}audio_channel_wl_mode_e;


typedef enum{
	AUDIO_BIT_16_DATA,
	AUDIO_BIT_20_DATA,
	AUDIO_BIT_24_DATA,
}audio_wl_select_e;

typedef enum{
	AUDIO_MONO,
	AUDIO_STEREO,
}audio_channel_select_e;


typedef enum{
	I2S_S_CODEC_M,
	I2S_M_CODEC_S,
}i2s_codec_m_s_mode_e;

typedef enum{
	MCU_WREG,
	I2C_WREG ,
}codec_wreg_mode_e;

typedef enum{
	I2S_DATA_INVERT_DIS,
	I2S_DATA_INVERT_EN ,
}audio_data_invert_e;

typedef struct {
	u8  audio_in_mode;
	u8  audio_out_mode;
	u8 	i2s_data_select;
	u8  codec_data_select;
	u8  i2s_codec_m_s_mode;
	u8  i2s_data_invert_select;
	u8  in_digital_gain;
	u8  in_analog_gain;
	u8  out_digital_gain;
	u8  out_analog_gain;
}aduio_i2s_codec_config_t;


typedef enum{
	I2S_DATA_IN_FIFO ,
	USB_DATA_IN_FIFO ,
	SAR_ADC_DATA_IN_FIFO,
}audio_mux_ain_e;

typedef enum{
	I2S_OUT ,
	USB_IOS_OUT ,
	NO_USE=3,
}audio_mux_aout_e;

/**
 * define audio rate value.
 */
typedef enum{
	AUDIO_8K,
	AUDIO_16K=3,
	AUDIO_32K=6,
	AUDIO_44EP1K,
	AUDIO_48K,
}audio_sample_rate_e;


typedef enum{
	AMIC_IN_ONLY,
	DMIC_IN_ONLY,
	LINE_IN_ONLY,
	AMIC_IN_OUT,
	DMIC_IN_OUT,
	LINE_IN_OUT,
	OUT_ONLY,
}audio_flow_mode_e;



typedef enum{
	AUDIO_RATE_EQUAL,
	AUDIO_RATE_GT_L0,
	AUDIO_RATE_GT_L1,
	AUDIO_RATE_LT_L0,
	AUDIO_RATE_LT_L1,
}audio_rate_match_e;


/*[0,+43], 1 dB steps*/
typedef enum{
	CODEC_IN_D_GAIN_0_DB=0,
	CODEC_IN_D_GAIN_4_DB=4,
	CODEC_IN_D_GAIN_8_DB=8,
	CODEC_IN_D_GAIN_12_DB=12,
	CODEC_IN_D_GAIN_16_DB=16,
	CODEC_IN_D_GAIN_20_DB=20,
	CODEC_IN_D_GAIN_43_DB=43,
}codec_in_path_digital_gain_e;


typedef enum{
	CODEC_IN_A_GAIN_0_DB,
	CODEC_IN_A_GAIN_4_DB,
	CODEC_IN_A_GAIN_8_DB,
	CODEC_IN_A_GAIN_12_DB,
	CODEC_IN_A_GAIN_16_DB,
	CODEC_IN_A_GAIN_20_DB,
}codec_in_path_analog_gain_e;



typedef enum{
	CODEC_OUT_D_GAIN_0_DB=0,
	CODEC_OUT_D_GAIN_m1_DB,
	CODEC_OUT_D_GAIN_m2_DB,
	CODEC_OUT_D_GAIN_m3_DB,
	CODEC_OUT_D_GAIN_m4_DB,
	CODEC_OUT_D_GAIN_m5_DB,
	CODEC_OUT_D_GAIN_m6_DB,
	CODEC_OUT_D_GAIN_m7_DB,
	CODEC_OUT_D_GAIN_m8_DB,
	CODEC_OUT_D_GAIN_m9_DB,
	CODEC_OUT_D_GAIN_m10_DB,
	CODEC_OUT_D_GAIN_m11_DB,
	CODEC_OUT_D_GAIN_m12_DB,
	CODEC_OUT_D_GAIN_m13_DB,
	CODEC_OUT_D_GAIN_m14_DB,
	CODEC_OUT_D_GAIN_m15_DB,
	CODEC_OUT_D_GAIN_m16_DB,
	CODEC_OUT_D_GAIN_m17_DB,
	CODEC_OUT_D_GAIN_m18_DB,
	CODEC_OUT_D_GAIN_m19_DB,
	CODEC_OUT_D_GAIN_m20_DB,
	CODEC_OUT_D_GAIN_m21_DB,
	CODEC_OUT_D_GAIN_m22_DB,
	CODEC_OUT_D_GAIN_m23_DB,
	CODEC_OUT_D_GAIN_m24_DB,
	CODEC_OUT_D_GAIN_m25_DB,
	CODEC_OUT_D_GAIN_m26_DB,
	CODEC_OUT_D_GAIN_m27_DB,
	CODEC_OUT_D_GAIN_m28_DB,
	CODEC_OUT_D_GAIN_m29_DB,
	CODEC_OUT_D_GAIN_m30_DB,
	CODEC_OUT_D_GAIN_m31_DB=31,

	CODEC_OUT_D_GAIN_32_DB,
	CODEC_OUT_D_GAIN_31_DB,
	CODEC_OUT_D_GAIN_30_DB,
	CODEC_OUT_D_GAIN_29_DB,
	CODEC_OUT_D_GAIN_28_DB,
	CODEC_OUT_D_GAIN_27_DB,
	CODEC_OUT_D_GAIN_26_DB,
	CODEC_OUT_D_GAIN_25_DB,
	CODEC_OUT_D_GAIN_24_DB,
	CODEC_OUT_D_GAIN_23_DB,
	CODEC_OUT_D_GAIN_22_DB,
	CODEC_OUT_D_GAIN_21_DB,
	CODEC_OUT_D_GAIN_20_DB,
	CODEC_OUT_D_GAIN_19_DB,
	CODEC_OUT_D_GAIN_18_DB,
	CODEC_OUT_D_GAIN_17_DB,
	CODEC_OUT_D_GAIN_16_DB,
	CODEC_OUT_D_GAIN_15_DB,
	CODEC_OUT_D_GAIN_14_DB,
	CODEC_OUT_D_GAIN_13_DB,
	CODEC_OUT_D_GAIN_12_DB,
	CODEC_OUT_D_GAIN_11_DB,
	CODEC_OUT_D_GAIN_10_DB,
	CODEC_OUT_D_GAIN_9_DB,
	CODEC_OUT_D_GAIN_8_DB,
	CODEC_OUT_D_GAIN_7_DB,
	CODEC_OUT_D_GAIN_6_DB,
	CODEC_OUT_D_GAIN_5_DB,
	CODEC_OUT_D_GAIN_4_DB,
	CODEC_OUT_D_GAIN_3_DB,
	CODEC_OUT_D_GAIN_2_DB,
	CODEC_OUT_D_GAIN_1_DB,
}codec_out_path_digital_gain_e;



typedef enum{

	CODEC_OUT_A_GAIN_12_DB,
	CODEC_OUT_A_GAIN_11_DB,
	CODEC_OUT_A_GAIN_10_DB,
	CODEC_OUT_A_GAIN_9_DB,
	CODEC_OUT_A_GAIN_8_DB,
	CODEC_OUT_A_GAIN_7_DB,
	CODEC_OUT_A_GAIN_6_DB,
	CODEC_OUT_A_GAIN_5_DB,
	CODEC_OUT_A_GAIN_4_DB,
	CODEC_OUT_A_GAIN_3_DB,
	CODEC_OUT_A_GAIN_2_DB,
	CODEC_OUT_A_GAIN_1_DB,
	CODEC_OUT_A_GAIN_0_DB,
	CODEC_OUT_A_GAIN_m1_DB,
	CODEC_OUT_A_GAIN_m2_DB,
	CODEC_OUT_A_GAIN_m3_DB,
	CODEC_OUT_A_GAIN_m4_DB,
	CODEC_OUT_A_GAIN_m5_DB,
	CODEC_OUT_A_GAIN_m6_DB,
	CODEC_OUT_A_GAIN_m7_DB,
	CODEC_OUT_A_GAIN_m8_DB,
	CODEC_OUT_A_GAIN_m9_DB,
	CODEC_OUT_A_GAIN_m10_DB,
	CODEC_OUT_A_GAIN_m11_DB,
	CODEC_OUT_A_GAIN_m12_DB,
	CODEC_OUT_A_GAIN_m13_DB,
	CODEC_OUT_A_GAIN_m14_DB,
	CODEC_OUT_A_GAIN_m15_DB,
	CODEC_OUT_A_GAIN_m16_DB,
	CODEC_OUT_A_GAIN_m17_DB,
	CODEC_OUT_A_GAIN_m18_DB,
	CODEC_OUT_A_GAIN_m19_DB,
}codec_out_path_analog_gain_e;


typedef enum
{
	INNER_CODEC,
	EXT_CODEC,//wm8731
}codec_type_e;

/**
 * 	@brief      This function serves to set codec supply voltage
 * 	@return     none
 */
static inline void audio_set_codec_supply (void)
{
	analog_write_reg8(0x02, 0xc4);//0xc4 LDO 2.8v,0x44 1.8V
									//The configuration of Retention ldo voltage in the A1 version will be changed.
}


/**
 * 	@brief      This function serves to set the clock of i2s
 * 	@param[in]  step - the dividing factor of step.
 * 	@param[in]  mod - the dividing factor of mod.
 * 	@return     none
 */
static inline void audio_set_i2s_clk(u8 step,u8  mod)
{
	reg_i2s_step = (step&FLD_I2S_STEP)|FLD_I2S_CLK_EN;
	reg_i2s_mod = mod;
}

/**
 * 	@brief      This function serves to set the clock of codec
 * 	@param[in]  step - the dividing factor of step.
 * 	@param[in]  mod - the dividing factor of mod.
 * 	@return     none
 * 	@attention  codec clock  divider from pll,so pll must be 192M
 */
static inline void audio_set_codec_clk(u8 step,u8  mod)
{
	BM_CLR(reg_dmic_clk_set,BIT(0));//set dmic_div
	reg_dmic_step = (step&FLD_DMIC_STEP)|FLD_DMIC_SEL;
	reg_dmic_mod = mod;
}

/**
 * 	@brief      This function serves to set enable i2s clk
 * 	@return     none
 */
static inline void audio_i2s_clk_en(void)
{
	BM_SET(reg_i2s_step,FLD_I2S_CLK_EN);
}

/**
 * 	@brief      This function serves to set disable i2s clk.
 * 	@return     none
 */
static inline void audio_i2s_clk_dis(void)
{
	BM_CLR(reg_i2s_step,FLD_I2S_CLK_EN);
}


/**
 * 	@brief      This function serves to set disable codec clk
 * 	@return     none
 */
static inline void audio_codec_clk_en(void)
{
	BM_SET(reg_dmic_step,FLD_DMIC_SEL);
}


/**
 * 	@brief      This function serves to set disable codec clk
 * 	@return     none
 */
static inline void audio_codec_clk_dis(void)
{
	BM_CLR(reg_dmic_step,FLD_DMIC_SEL);
}


/**
 * 	@brief      This function serves to set the bclk
 * 	@param[in]  div  - bclk=i2s_clk/(div*2),if div=0,i2s_clk=bclk.
 * 	@return     none
 */
static inline void audio_set_i2s_bclk(u8 div)
{
	reg_pcm_clk_num=div&0x0f;
}

/**
 * 	@brief      This function serves to set the lrclk
 * 	@param[in]  div - lrclk=bclk/(div)
 * 	@return     none
 */
static inline void audio_set_lrclk(u16 div)
{
	reg_int_pcm_num=(div-1);
	reg_dec_pcm_num=(div-1);
}

/**
 * 	@brief      This function serves to set  tx buff length,
 * 	@param[in]  len - the length of tx buff
 * 	@return     none
 */
static inline void audio_set_tx_buff_len(u16 len)
{
	reg_tx_max=((len)>>2)-1;
}

/**
 * 	@brief      This function serves to set rx buff length,
 * 	@param[in]  len - the length of rx buff
 * 	@return     none
 */
static inline void audio_set_rx_buff_len(u16 len)
{
   reg_rx_max=((len)>>2)-1;
}


/**
 * 	@brief      This function serves to write codec register by mc.
 * 	@param[in]  addr - the address of codec register
 * 	@param[in]  data - Data to be written
 * 	@return     none
 */
static inline void audio_write_codec_reg(u8 addr,u8  data)
{
	write_reg8(REG_AUDIO_AHB_BASE+((0x80+addr)<<2), data);

}

/**
 * 	@brief      This function serves to read codec register by mc.
 * 	@param[in]  addr - the address of codec register
 * 	@return     data
 */
static inline u8 audio_read_codec_reg(u8 addr)
{
	return read_reg8(REG_AUDIO_AHB_BASE+((0x80+addr)<<2));

}

/**
 * 	@brief      This function serves to tx fifo trigger number.
 * 	@param[in]  number - the number of dma trigger
 * 	@return     none
 */
static inline void audio_set_fifo_tx_trig_num(u8 number)
{
	reg_fifo_trig0 &=~FLD_AUDIO_FIFO_AOUT0_TRIG_NUM;
	reg_fifo_trig0 |=(number&FLD_AUDIO_FIFO_AOUT0_TRIG_NUM);
}

/**
 * 	@brief      This function serves to rx fifo trigger number.
 * 	@param[in]  number - the number of dma trigger
 * 	@return     none
 */
static inline void  audio_set_fifo_rx_trig_num(u8 number)
{
	 reg_fifo_trig0 &= ~FLD_AUDIO_FIFO_AIN0_TRIG_NUM;
	 reg_fifo_trig0 |=((number&0x0f)<<4);
}




/**
 * 	@brief      This function serves to enable or disable i2s clk and codec mc clk .
 * 	@param[in]  i2s_clk_en - 1 enable  : 0 disable
 * 	@param[in]  mc_clk_en  - 1 enable  : 0 disable
 * 	@return     none
 */
static inline void  audio_clk_en(u8 i2s_clk_en,u8 mc_clk_en)
{
	reg_audio_en=MASK_VAL( FLD_AUDIO_I2S_CLK_EN, i2s_clk_en,\
			      FLD_AUDIO_MC_CLK_EN, mc_clk_en);
}


/**
 * 	@brief      This function serves to get dma rx buff pointer.
 * 	@param[in]  chn - dma channel
 * 	@return     none
 */
static inline u32 audio_get_rx_dma_wptr (dma_chn_e chn)
{
	return   convert_ram_addr_bus2cpu(reg_dma_dst_addr(chn));
}

/**
 * 	@brief      This function serves to get dma tx buff pointer.
 * 	@param[in]  chn - dma channel
 * 	@return     none
 */
static inline u32 audio_get_tx_dma_rptr (dma_chn_e chn)
{
	return convert_ram_addr_bus2cpu(reg_dma_src_addr(chn));
}


/**
 * @brief      This function serves to invert data between R channel and L channel.
 * @return     none
 */
static inline void audio_invert_data_en(void)
{
	BM_SET(reg_i2s_cfg,FLD_AUDIO_I2S_LRSWAP);
}

/**
 * @brief      This function serves to invert data between R channel and L channel.
 * @return     none
 */
static inline void audio_invert_data_dis(void)
{
	BM_CLR(reg_i2s_cfg,FLD_AUDIO_I2S_LRSWAP);
}

/**
 * @brief     This function configures dmic pin.
 * @param[in] pin_gp - the group of dmic pin
 * @return    none
 */
void audio_set_dmic_pin(dmic_pin_group_e pin_gp);

/**
 * @brief     This function serves to set data path.
 * @param[in] audio_flow    - audio flow  select
 * @param[in] ain0_mode     - fifo0 input mode select
 * @param[in] ain1_mode     - fifo1 input mode select
 * @param[in] i2s_aout_mode - fifo output source select
 * @return    none
 */
void audio_mux_config(audio_flow_e audio_flow, audio_in_mode_e ain0_mode , audio_in_mode_e ain1_mode,audio_out_mode_e i2s_aout_mode);


/**
 * @brief     This function serves to config codec for dac.
 * @param[in] mode        - select i2s as master or slave
 * @param[in] rate		  - audio sampling rate
 * @param[in] data_select - codec dac word length
 * @param[in] wreg_mode   - mcu or i2c config codec
 * @return    none
 */
void audio_codec_dac_config(i2s_codec_m_s_mode_e mode,audio_sample_rate_e rate,codec_data_select_e data_select,codec_wreg_mode_e  wreg_mode);

/**
 * @brief     This function serves to config codec for adc.
 * @param[in] mode        - select i2s as master or slave
 * @param[in] in_mode     - line_in/amic/dmic input mode select
 * @param[in] rate        - audio sampling rate
 * @param[in] data_select - codec adc word length
 * @param[in] wreg_mode   - mcu or i2c config codec
 * @return    none
 */
void audio_codec_adc_config(i2s_codec_m_s_mode_e mode,audio_input_mode_e in_mode,audio_sample_rate_e rate,codec_data_select_e data_select,codec_wreg_mode_e  wreg_mode);



/**
 * @brief     This function serves to set data path
 * @param[in] ain0_sel  - fifo0 input source select
 * @param[in] aout0_sel - fifo0 output source select
 * @param[in] ain1_sel  - fifo1 input source select
 * @param[in] aout1_sel - fifo1 output source select
 * @return    none
 */
void audio_data_path_sel (audio_mux_ain_e ain0_sel, audio_mux_aout_e aout0_sel, audio_mux_ain_e ain1_sel, audio_mux_aout_e aout1_sel);

/**
 * @brief     This function serves to config interface, word length, and m/s .
 * @param[in] i2s_format - interface protocol
 * @param[in] wl   		 - audio data word length
 * @param[in] m_s        - select i2s as master or slave
 * @param[in] en         - 1 enable audio data invert , 0 disable audio data invert .for example in mono mode switch R and L channel data to fifo0
 * @return    none
 */
void audio_i2s_config(i2s_mode_select_e i2s_format,i2s_data_select_e wl,  i2s_codec_m_s_mode_e m_s , audio_data_invert_e en );

/**
 * @brief     This function serves to set i2s clock and audio sampling rate when i2s as master.
 * @param[in] audio_rate - audio sampling rate
 * @param[in] match		 - the match of audio rate.
 * @param[in] match_en   - initial must 0, then change rate must 1
 * @return    none
 * @attention i2s clock  divider from pll,sampling rate calculation is based on pll=192M,so pll must be 192M
 */
_attribute_ram_code_sec_noinline_ void  audio_set_i2s_clock (audio_sample_rate_e audio_rate,audio_rate_match_e match, u8 match_en);

/**
 * @brief     This function serves to config  rx_dma channel.
 * @param[in] chn          - dma channel
 * @param[in] dst_addr     - the dma address of destination
 * @param[in] data_len     - the length of dma rx size by byte
 * @param[in] head_of_list - the head address of dma llp.
 * @return    none
 */
void audio_rx_dma_config(dma_chn_e chn,u16 * dst_addr,u32 data_len,dma_chain_config_t *head_of_list);

/**
 * @brief     This function serves to set rx dma chain transfer
 * @param[in] rx_config - the head of list of llp_pointer.
 * @param[in] llpointer - the next element of llp_pointer.
 * @param[in] dst_addr  -the dma address of destination.
 * @param[in] data_len  -the length of dma size by byte.
 * @return    none
 */
void audio_rx_dma_add_list_element(dma_chain_config_t * rx_config,dma_chain_config_t *llpointer ,u16 * dst_addr,u32 data_len);

/**
 * @brief     This function serves to config  tx_dma channel.
 * @param[in] chn          - dma channel
 * @param[in] src_addr     - the address of source
 * @param[in] data_len     - the length of dma rx size by byte
 * @param[in] head_of_list - the head address of dma llp.
 * @return    none
 */
void audio_tx_dma_config(dma_chn_e chn,u16 * src_addr, u32 data_len,dma_chain_config_t * head_of_list);

/**
 * @brief     This function serves to set tx dma chain transfer
 * @param[in] config_addr - the head of list of llp_pointer.
 * @param[in] llpointer   - the next element of llp_pointer.
 * @param[in] src_addr    - the address of source
 * @param[in] data_len    - the length of dma size by byte.
 * @return    none
 */
void audio_tx_dma_add_list_element(dma_chain_config_t *config_addr,dma_chain_config_t *llpointer ,u16 * src_addr,u32 data_len);

/**
 * @brief     This function serves to enable rx_dma channel.
 * @return    none
 */
void audio_rx_dma_en(void);

/**
 * @brief     This function serves to disable rx_dma channel.
 * @return    none
 */
 void audio_rx_dma_dis(void);

/**
 * @brief     This function serves to enable tx_dma channel.
 * @return    none
 */
void audio_tx_dma_en(void);


/**
 * @brief     This function serves to disable tx_dma channel.
 * @return    none
 */
void audio_tx_dma_dis(void);


/**
 * @brief     This function serves to  initialize audio by mc
 * @param[in] flow_mode  - select input out flow mode
 * @param[in] rate       - audio sampling rate.
 * @param[in] channel_wl - word length and channel number.
 * @return    none
 */
void audio_init(audio_flow_mode_e flow_mode,audio_sample_rate_e rate,audio_channel_wl_mode_e channel_wl);


/**
 * @brief     This function serves to  initialize audio by i2c
 * @param[in] flow_mode  - select input out flow mode
 * @param[in] rate       - audio sampling rate.
 * @param[in] channel_wl - word length and channel number.
 * @return    none
 */
void audio_init_i2c(audio_flow_mode_e flow_mode,audio_sample_rate_e rate,audio_channel_wl_mode_e channel_wl);


/**
 * @brief This function serves to initialize audio(external codec WM8731) by i2c.
 * @param[in]  pwm0_pin - the pin of pwm0
 * @param[in]  sda_pin  - the pin port selected as I2C sda pin port.
 * @param[in]  scl_pin  - the pin port selected as I2C scl pin port.
 * @return    none
 */
void audio_i2s_init(pwm_pin_e pwm0_pin, i2c_sda_pin_e sda_pin,i2c_scl_pin_e scl_pin);

/**
 * @brief  This function serves to set audio rx dma chain transfer.
 * @param[in] chn       -  dma channel
 * @param[in] in_buff   - the pointer of rx_buff.
 * @param[in] buff_size - the size of rx_buff.
 * @return    none
 */
void audio_rx_dma_chain_init (dma_chn_e chn,u16 * in_buff,u32 buff_size );

/**
 * @brief  This function serves to initialize audio tx dma chain transfer.
 * @param[in] chn       - dma channel
 * @param[in] out_buff  - the pointer of tx_buff.
 * @param[in] buff_size - the size of tx_buff.
 * @return    none
 */
void audio_tx_dma_chain_init (dma_chn_e chn,u16 * out_buff,u32 buff_size);


/**
 * 	@brief      This function serves to set in path digital and analog gain  .
 * 	@param[in]  d_gain - digital gain value
 * 	@param[in]  a_gain - analog  gain value
 * 	@return     none
 */
void audio_set_codec_in_path_a_d_gain (codec_in_path_digital_gain_e d_gain,codec_in_path_analog_gain_e a_gain );

/**
 * 	@brief      This function serves to set out path digital and analog gain  .
 * 	@param[in]  d_gain - digital gain value
 * 	@param[in]  a_gain - analog  gain value
 * 	@return     none
 */
 void audio_set_codec_out_path_a_d_gain (codec_out_path_digital_gain_e d_gain,codec_out_path_analog_gain_e a_gain);

 /**
  * @brief      This function serves to choose which is master to provide clock.
  * @param[in]  m_s - I2S_S_CODEC_M: i2s as slave ,codec as master; I2S_M_CODEC_S: i2s as  master, codec  as slave.
  * @return     none
  */
 void audio_set_i2s_codec_m_s (i2s_codec_m_s_mode_e m_s);



 /**
  * @brief     This function serves to change sample rate for dac.
  * @param[in] rate     -  the sample rate of dac
  * @param[in] buff     - the pointer of audio buff
  * @param[in] buff_len - the length of audio buff
  * @return    none
  */
 _attribute_ram_code_sec_ void audio_change_sample_rate (audio_sample_rate_e  rate,void * buff,u32 buff_len);


 /**
  * @brief     This function serves to power down codec_dac.
  * @return    none
  */
void audio_codec_dac_power_down(void);

 /**
  * @brief     This function serves to power on codec_dac.
  * @return    none
  */
void audio_codec_dac_power_on(void);

/**
 * @brief     This function serves to power down codec_adc.
 * @return    none
 */
void audio_codec_adc_power_down(void);

/**
 * @brief     This function serves to power on codec_adc.
 * @return    none
 */
void audio_codec_adc_power_on(void);
#endif
