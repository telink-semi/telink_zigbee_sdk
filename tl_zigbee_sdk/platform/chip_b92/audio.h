/********************************************************************************************************
 * @file    audio.h
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
/** @page AUDIO
 *
 *  Introduction
 *  ===============
 *  audio module consists of 3 parts: audio code, audio in path , and audio out path.
 *
 *  API Reference
 *  ===============
 *  Header File: audio.h
 */
#ifndef audio_H
#define audio_H

#include "i2c.h"
#include "pwm.h"
#include "compiler.h"
#include "reg_include/register.h"

typedef enum
{
    FIFO0 = 0,
    FIFO1,
} audio_fifo_chn_e;

typedef enum
{
    I2S0,
    I2S1,
} audio_i2s_select_e;

typedef enum
{
    AUDIO_RX_FIFO0_IRQ = BIT(0),
    AUDIO_TX_FIFO0_IRQ = BIT(1),
    AUDIO_RX_FIFO1_IRQ = BIT(2),
    AUDIO_TX_FIFO1_IRQ = BIT(3),
} audio_fifo_irq_type_e;

typedef struct
{
    gpio_func_pin_e bclk_pin;
    gpio_func_pin_e adc_lr_clk_pin;
    gpio_func_pin_e adc_dat_pin;
    gpio_func_pin_e dac_lr_clk_pin;
    gpio_func_pin_e dac_dat_pin;
} i2s_pin_config_t;

typedef enum
{
    I2S_RJ_MODE,
    I2S_LJ_MODE,
    I2S_I2S_MODE,
    I2S_DSP_MODE,
} i2s_mode_select_e;

typedef enum
{
    POWER_DOWN,
    POWER_ON,
} power_switch_e;

typedef enum
{
    HP_MODE,  //high performance
    NML_MODE, //normal performance
    LP_MODE,  //low performance
} codec_dac_mode_e;

typedef enum
{
    CHANNEL_LEFT   = BIT(0),
    CHANNEL_RIGHT  = BIT(1),
    CHANNEL_STEREO = BIT(0) | BIT(1),
} audio_chn_sel_e;

typedef enum
{
    LINE_STREAM0_MONO_L = BIT(0),
    LINE_STREAM0_MONO_R = BIT(1),
    LINE_STREAM0_STEREO = BIT(1) | BIT(0),
    AMIC_STREAM0_MONO_L = BIT(2) | BIT(0),
    AMIC_STREAM0_MONO_R = BIT(2) | BIT(1),
    AMIC_STREAM0_STEREO = BIT(2) | BIT(1) | BIT(0),
    DMIC_STREAM0_MONO_L = BIT(3) | BIT(0),
    DMIC_STREAM0_MONO_R = BIT(3) | BIT(1),
    DMIC_STREAM0_STEREO = BIT(3) | BIT(1) | BIT(0),
} codec_stream0_input_src_e;

typedef enum
{
    DMIC_STREAM1_MONO_L         = BIT(0),
    DMIC_STREAM1_MONO_R         = BIT(1),
    DMIC_STREAM1_STEREO         = BIT(1) | BIT(0),
    DMIC_STREAM0_STREAM1_STEREO = BIT(2) | BIT(1) | BIT(0),
} codec_stream1_input_src_e;

typedef enum
{
    CODEC_DAC_MONO_L = BIT(0),
    CODEC_DAC_MONO_R = BIT(1),
    CODEC_DAC_STEREO = BIT(0) | BIT(1),
} audio_dac_output_src_e;

typedef enum
{
    CURR_5P0_UA  = BIT(2) | 3, //5.0uA
    CURR_3P8_UA  = BIT(2) | 0, //3.8uA
    CURR_3P07_UA = 3,          //3.07uA
} codec_lbias_curr_sel_e;

typedef enum
{
    CODEC_BIT_16_MONO,
    CODEC_BIT_20_MONO,
    CODEC_BIT_16_STEREO,
    CODEC_BIT_20_STEREO,
    CODEC_BIT_16_STEREO_STREAM0_STREAM1, //stream0 and stream1 share one fifo,only need to configure stream1
} audio_codec_in_mode_e;

typedef enum
{
    I2S_BIT_16_MONO,
    I2S_BIT_20_OR_24_MONO,
    I2S_BIT_16_STEREO,
    I2S_BIT_20_OR_24_STEREO,
} audio_i2s_in_mode_e;

typedef enum
{
    CODEC_BIT_16_DATA,
    CODEC_BIT_20_DATA,
} audio_codec_wl_mode_e;

typedef enum
{
    BIT_16_MONO_FIFO0,
    BIT_20_OR_24_MONO_FIFO0,
    BIT_16_STEREO_FIFO0,
    BIT_20_OR_24_STEREO_FIFO0,
    BIT_16_MONO_FIFO1,
    BIT_20_OR_24_MONO_FIFO1,   //codec only support 20bit,i2s support 20 bit or 24 bit
    BIT_16_STEREO_FIFO0_AND_FIFO1,
    BIT_20_OR_24_STEREO_FIFO0_AND_FIFO1,
    BIT_20_OR_24_STEREO_FIFO1, //codec only support 20bit,i2s support 20 bit or 24 bit
    BIT_16_STEREO_FIFO1,
} audio_out_mode_e;

typedef enum
{
    I2S_BIT_16_DATA,
    I2S_BIT_20_DATA,
    I2S_BIT_24_DATA,
    I2S_BIT_16_DATA_FIFO0_AND_FIFO1,
    I2S_BIT_20_OR_24_DATA_FIFO0_AND_FIFO1,
} audio_i2s_wl_mode_e;

typedef enum
{
    I2S_MONO,
    I2S_STEREO,
} i2s_channel_select_e;

typedef enum
{
    I2S_AS_SLAVE_EN,
    I2S_AS_MASTER_EN,
} i2s_m_s_mode_e;

typedef enum
{
    DATA_INVERT_DIS,
    DATA_INVERT_EN,
} audio_data_invert_e;

typedef enum
{
    I2S_LR_CLK_INVERT_DIS,
    I2S_LR_CLK_INVERT_EN,
} audio_i2s_lr_clk_invert_e;

typedef struct
{
    unsigned char i2s_lr_clk_invert_select;
    unsigned char i2s_data_invert_select;
} audio_i2s_invert_config_t;

typedef enum
{
    I2S0_IN_FIFO,
    I2S1_IN_FIFO,
    USB_AISO_IN_FIFO,
    SAR_ADC_IN_FIFO,
    CODEC_STREAM0_IN_FIFO,
    CODEC_STREAM1_IN_FIFO,
} audio_mux_ain_e;

typedef enum
{
    I2S0_OUT_FIFO,
    USB_AISO_OUT_FIFO,
    CODEC_INT_OUT_FIFO,
    I2S1_OUT_FIFO,
} audio_mux_aout_e;

/**
 * define audio rate value.
 */
typedef enum
{
    AUDIO_8K,       //8K=12Mhz/1500
    AUDIO_8P0K,     //8.021K=12Mhz/1496
    AUDIO_11P0K,    //11.0259K=12Mhz/1088
    AUDIO_12K,      //12K=12Mhz/1000
    AUDIO_16K,      //16K=12Mhz/750
    AUDIO_22P0588K, //22.0588K=12Mhz/544
    AUDIO_24K,      //24K=12Mhz/500
    AUDIO_32K,      //32K=12Mhz/375
    AUDIO_44P1K,    //44.118K=12Mhz/272
    AUDIO_48K,      //48k=12Mhz/250
    AUDIO_RATE_SIZE
} audio_sample_rate_e;

typedef enum
{
    AMIC_OR_LINE_INPUT = 0,
    DMIC_INPUT,
} audio_stream0_input_mode_e;

typedef enum
{
    CODEC_IN_D_GAIN1_m48_DB = 0,  //-48dB
    CODEC_IN_D_GAIN1_m42_DB = 4,  //-42dB
    CODEC_IN_D_GAIN1_m36_DB = 8,  //-36dB
    CODEC_IN_D_GAIN1_m30_DB = 12, //-30dB
    CODEC_IN_D_GAIN1_m24_DB = 16, //-24dB
    CODEC_IN_D_GAIN1_m18_DB = 20, //-18dB
    CODEC_IN_D_GAIN1_m12_DB = 24, //-12dB
    CODEC_IN_D_GAIN1_m6_DB  = 28, //-6dB
    CODEC_IN_D_GAIN1_0_DB   = 32, // 0dB  default
    CODEC_IN_D_GAIN1_6_DB   = 36, // 6dB
    CODEC_IN_D_GAIN1_12_DB  = 40, // 12dB
    CODEC_IN_D_GAIN1_18_DB  = 44, // 18dB
    CODEC_IN_D_GAIN1_24_DB  = 48, // 24dB
    CODEC_IN_D_GAIN1_30_DB  = 52, // 30dB
    CODEC_IN_D_GAIN1_36_DB  = 56, // 36dB
    CODEC_IN_D_GAIN1_42_DB  = 60, // 42dB
} codec_in_path_digital_gain1_e;

typedef enum
{
    CODEC_IN_D_GAIN0_0_DB,  // 0dB
    CODEC_IN_D_GAIN0_6_DB,  // 6dB
    CODEC_IN_D_GAIN0_12_DB, // 12dB
    CODEC_IN_D_GAIN0_18_DB, // 18dB
} codec_in_path_digital_gain0_e;

typedef enum
{
    CODEC_IN_A_GAIN_45P2_DB, //45.2dB
    CODEC_IN_GAIN_43P5_DB,   //43.5dB
    CODEC_IN_GAIN_42P1_DB,   //42.1dB
    CODEC_IN_GAIN_40P5_DB,   //40.5dB
    CODEC_IN_GAIN_39P1_DB,   //39.1dB
    CODEC_IN_GAIN_37P4_DB,   //37.4dB
    CODEC_IN_GAIN_36P0_DB,   //36.0dB
    CODEC_IN_GAIN_34P6_DB,   //34.6dB default
    CODEC_IN_GAIN_33P0_DB,   //33.0dB
    CODEC_IN_GAIN_30P1_DB,   //30.1dB
    CODEC_IN_GAIN_27P0_DB,   //27.0dB
    CODEC_IN_GAIN_24P0_DB,   //24.0dB
    CODEC_IN_GAIN_21P0_DB,   //21.0dB
    CODEC_IN_GAIN_15P0_DB,   //15.0dB
    CODEC_IN_GAIN_9P0_DB,    //9.0dB
    CODEC_IN_GAIN_0_DB,      //0dB
} codec_in_pga_gain_e;

/**dac pga gain= 20*log10(x/1024)dB,x(11bit), default 0x400(0dB)**/
typedef enum
{
    CODEC_OUT_GAIN_m60P2_DB = 0x1,   //-60.2dB
    CODEC_OUT_GAIN_m48P2_DB = 0x4,   //-48.2dB
    CODEC_OUT_GAIN_m42P1_DB = 0x8,   //-42.1dB
    CODEC_OUT_GAIN_m36P1_DB = 0x10,  //-36.1dB
    CODEC_OUT_GAIN_m32P9_DB = 0x17,  //-32.9dB
    CODEC_OUT_GAIN_m30_DB   = 0x20,  //-30.1dB
    CODEC_OUT_GAIN_m27_DB   = 0x2e,  //-27dB
    CODEC_OUT_GAIN_m24_DB   = 0x41,  //-24dB
    CODEC_OUT_GAIN_m21_DB   = 0x5b,  //-21dB
    CODEC_OUT_GAIN_m18_DB   = 0x81,  //-18dB
    CODEC_OUT_GAIN_m15_DB   = 0xb6,  //-15dB
    CODEC_OUT_GAIN_m12_DB   = 0x101, //-12dB
    CODEC_OUT_GAIN_m9_DB    = 0x16b, //-9dB
    CODEC_OUT_GAIN_m3_DB    = 0x2d5, //-3dB
    CODEC_OUT_GAIN_m6_DB    = 0x201, //-6dB
    CODEC_OUT_GAIN_0_DB     = 0x400, // 0dB default
    CODEC_OUT_GAIN_3_DB     = 0x5a6, // 3dB
    CODEC_OUT_GAIN_6_DB     = 0x7fb, // 6dB
} codec_out_pga_gain_e;

/**digital gain= 20*log10(x/4096)dB,x(12bit),default 0xe42(-1dB)**/
typedef enum
{
    CODEC_OUT_D_GAIN_m72p_2DB = 0x01,  //-72.2dB
    CODEC_OUT_D_GAIN_m66p2_DB = 0x02,  //-66.2dB
    CODEC_OUT_D_GAIN_m60p2_DB = 0x04,  //-60.2dB
    CODEC_OUT_D_GAIN_m54p2_DB = 0x08,  //-54.2dB
    CODEC_OUT_D_GAIN_m51p4_DB = 0x0b,  //-51.4dB
    CODEC_OUT_D_GAIN_m48p2_DB = 0x10,  //-48.2dB
    CODEC_OUT_D_GAIN_m45_DB   = 0x17,  //-45dB
    CODEC_OUT_D_GAIN_m42p1_DB = 0x20,  //-42.1dB
    CODEC_OUT_D_GAIN_m39_DB   = 0x2e,  //-39dB
    CODEC_OUT_D_GAIN_m36_DB   = 0x41,  //-36dB
    CODEC_OUT_D_GAIN_m33_DB   = 0x5c,  //-33dB
    CODEC_OUT_D_GAIN_m30_DB   = 0x81,  //-30dB
    CODEC_OUT_D_GAIN_m27_DB   = 0xb7,  //-27dB
    CODEC_OUT_D_GAIN_m24_DB   = 0x102, //-24dB
    CODEC_OUT_D_GAIN_m21_DB   = 0x16d, //-21dB
    CODEC_OUT_D_GAIN_m18_DB   = 0x204, //-18dB
    CODEC_OUT_D_GAIN_m15_DB   = 0x2d8, //-15dB
    CODEC_OUT_D_GAIN_m12_DB   = 0x405, //-12dB
    CODEC_OUT_D_GAIN_m9_DB    = 0x5ac, //-9dB
    CODEC_OUT_D_GAIN_m6_DB    = 0x804, //-6dB
    CODEC_OUT_D_GAIN_m3_DB    = 0xb53, //-3dB
    CODEC_OUT_D_GAIN_m1_DB    = 0xe42, //-1dB default
    CODEC_OUT_D_GAIN_0_DB     = 0xfff, // 0dB
} codec_out_path_digital_gain_e;

typedef enum
{
    MICBIAS_NORMAL_1V6_MODE,
    MICBIAS_NORMAL_1V8_MODE,
    MICBIAS_AVD_3V3_MODE,
} micbias_work_mode_e;

typedef enum
{
    I2S_ALIGN_BIT_16_DATA    = BIT(0),
    I2S_ALIGN_BIT_20_24_DATA = BIT(1),
} audio_i2s_align_data_width_e;

typedef struct
{
    void                     *data_buf;
    unsigned int              data_buf_size;
    codec_stream0_input_src_e input_src;
    audio_sample_rate_e       sample_rate;
    audio_codec_wl_mode_e     data_width;
    audio_fifo_chn_e          fifo_num;
    dma_chn_e                 dma_num;
} audio_codec_stream0_input_t;

typedef struct
{
    void                     *data_buf;
    unsigned int              data_buf_size;
    codec_stream1_input_src_e input_src;
    audio_sample_rate_e       sample_rate;
    audio_codec_wl_mode_e     data_width;
    audio_fifo_chn_e          fifo_num;
    dma_chn_e                 dma_num;
} audio_codec_stream1_input_t;

typedef struct
{
    void                  *data_buf;
    unsigned int           data_buf_size;
    audio_dac_output_src_e output_src;
    audio_sample_rate_e    sample_rate;
    audio_codec_wl_mode_e  data_width;
    audio_fifo_chn_e       fifo_num;
    dma_chn_e              dma_num;
    codec_dac_mode_e       mode;
} audio_codec_output_t;

typedef struct
{
    void                *data_buf;
    unsigned int         data_buf_size;
    audio_i2s_select_e   i2s_select;
    i2s_channel_select_e i2s_ch_sel;
    audio_i2s_wl_mode_e  data_width;
    audio_fifo_chn_e     fifo_chn;
    dma_chn_e            dma_num;
} audio_i2s_input_output_t;

typedef struct
{
    unsigned short     *sample_rate;
    i2s_pin_config_t   *pin_config;
    audio_i2s_select_e  i2s_select;
    audio_i2s_wl_mode_e data_width;
    i2s_mode_select_e   i2s_mode;
    i2s_m_s_mode_e      master_slave_mode;
} audio_i2s_config_t;

/**
 *  @brief      This function serves to enable bclk and lr_clk.
 *  @param[in]  i2s_select - i2s channel select
 *  @return     none
 */
static inline void audio_clk_i2s_en(audio_i2s_select_e i2s_select)
{
    BM_SET(reg_i2s_en(i2s_select), FLD_AUDIO_I2S_CLK_EN);
}

/**
 *  @brief      This function serves to disable bclk and lr_clk
 *  @param[in]  i2s_select - i2s channel select
 *  @return     none
 */
static inline void audio_clk_i2s_dis(audio_i2s_select_e i2s_select)
{
    BM_CLR(reg_i2s_en(i2s_select), FLD_AUDIO_I2S_CLK_EN);
}

/**
 *  @brief      This function serves to enable source i2s clk.
 *  @param[in]  i2s_select - i2s channel select
 *  @return     none
 */
static inline void audio_set_i2s_src_clk_en(audio_i2s_select_e i2s_select)
{
    reg_i2s_step(i2s_select) |= FLD_I2S_CLK_EN;
}

/**
 *  @brief      This function serves to disable source i2s clk
 *  @param[in]  i2s_select - i2s channel select
 *  @return     none
 */
static inline void audio_set_i2s_src_clk_dis(audio_i2s_select_e i2s_select)
{
    reg_i2s_step(i2s_select) &= ~FLD_I2S_CLK_EN;
}

/**
 *  @brief      This function serves to set the clock of codec
 *  @param[in]  div_numerator   - the dividing factor of div_numerator (15bits valid).
 *  @param[in]  div_denominator - the dividing factor of div_denominator(16bits valid).
 *  @return     none
 *  @attention  The default is from pll 192M(default). If the pll is changed, the clk will be changed accordingly.
 */
static inline void audio_set_codec_clk(unsigned short div_numerator, unsigned short div_denominator)
{
    reg_dmic_step = (div_numerator & FLD_DMIC_STEP) | FLD_DMIC_SEL;
    reg_dmic_mod  = div_denominator;
}

/**
 *  @brief      This function serves to set i2s clock of i2s,i2s_clk =192M*(div_numerator/div_denominator)
 *  @param[in]  i2s_select      - i2s channel select
 *  @param[in]  div_numerator   - the dividing factor of div_numerator (15bits valid).
 *  @param[in]  div_denominator - the dividing factor of div_denominator(16bits valid).
 *  @return     none
 */
static inline void audio_set_i2s_clk(audio_i2s_select_e i2s_select, unsigned short div_numerator, unsigned short div_denominator)
{
    reg_i2s_step(i2s_select) = (div_numerator & FLD_I2S_STEP) | FLD_I2S_CLK_EN;
    reg_i2s_mod(i2s_select)  = div_denominator;
}

/**
 *  @brief      This function serves to set the bclk divider
 *  @param[in]  i2s_select   - i2s channel select
 *  @param[in]  div  - bclk=i2s_clk/(div*2),if div=0,i2s_clk=bclk.
 *  @return     none
 */
static inline void audio_set_i2s_bclk(audio_i2s_select_e i2s_select, unsigned char div)
{
    reg_i2s_pcm_clk_num(i2s_select) = div & 0x0f;
}

/**
 *  @brief      This function serves to set the i2s lrclk divider.
 *  @param[in]  i2s_select - i2s channel select
 *  @param[in]  adc_div - adc_lrclk=bclk/(adc_div)
 *  @param[in]  dac_div - dac_lrclk=bclk/(dac_div)
 *  @return     none
 */
static inline void audio_set_i2s_lrclk(audio_i2s_select_e i2s_select, unsigned short adc_div, unsigned short dac_div)
{
    reg_i2s_int_pcm_num(i2s_select) = (adc_div - 1);
    reg_i2s_dec_pcm_num(i2s_select) = (dac_div - 1);
}

/**
 *  @brief      This function serves to set disable codec clk
 *  @return     none
 */
static inline void audio_codec_clk_en(void)
{
    BM_SET(reg_dmic_step, FLD_DMIC_SEL);
}

/**
 *  @brief      This function serves to set disable codec clk
 *  @return     none
 */
static inline void audio_codec_clk_dis(void)
{
    BM_CLR(reg_dmic_step, FLD_DMIC_SEL);
}

/**
 *  @brief      This function serves to get tx read pointer.
 *  @param[in]  tx_fifo_chn - select fifo channel
 *  @return     the result of tx read pointer.
 */
static inline unsigned short audio_get_tx_rptr(audio_fifo_chn_e tx_fifo_chn)
{
    return reg_tx_rptr(tx_fifo_chn);
}

/**
 *  @brief      This function serves to get rx write pointer.
 *  @param[in]  rx_fifo_chn - select fifo channel
 *  @return     the result of rx write pointer.
 */
static inline unsigned short audio_get_rx_wptr(audio_fifo_chn_e rx_fifo_chn)
{
    return reg_rx_wptr(rx_fifo_chn);
}

/**
 * @brief       This function servers to set irq mask.
 * @param[in]   irq_mask    - the irq mask.
 * @return    - none.
 */
static inline void audio_set_irq_mask(audio_fifo_irq_type_e irq_mask)
{
    reg_fifo_ptr_irq_mask |= irq_mask;
}

/**
 * @brief       This function servers to clear irq mask.
 * @param[in]   irq_mask    - the irq mask.
 * @return    - none.
 */
static inline void audio_clr_irq_mask(audio_fifo_irq_type_e irq_mask)
{
    reg_fifo_ptr_irq_mask &= (~irq_mask);
}

/**
 * @brief       This function servers to get irq status.
 * @param[in]   irq_status  - the irq status.
 * @retval      non-zero        - the interrupt occurred.
 * @retval      zero    - the interrupt did not occur.
 */
static inline unsigned char audio_get_irq_status(audio_fifo_irq_type_e irq_status)
{
    return reg_fifo_ptr_irq_status & irq_status;
}

/**
 * @brief       This function servers to clear irq status.
 * @param[in]   irq_status  - the irq status.
 * @return    - none.
 */
static inline void audio_clr_irq_status(audio_fifo_irq_type_e irq_status)
{
    reg_fifo_ptr_irq_status = irq_status;
}

/**
 *  @brief      This function serves to set tx fifo threshold.
 *  @param[in]  tx_fifo_chn  - select fifo channel
 *  @param[in]  threshold    - threshold value
 *  @return     none.
 */
static inline void audio_set_tx_fifo_threshold(audio_fifo_chn_e tx_fifo_chn, unsigned short threshold)
{
    reg_tx_fifo_th(tx_fifo_chn) = threshold >> 2;
}

/**
 *  @brief      This function serves to set rx fifo threshold.
 *  @param[in]  rx_fifo_chn  - select fifo channel
 *  @param[in]  threshold    - threshold value
 *  @return     none.
 */
static inline void audio_set_rx_fifo_threshold(audio_fifo_chn_e rx_fifo_chn, unsigned short threshold)
{
    reg_rx_fifo_th(rx_fifo_chn) = threshold >> 2;
}

/**
 *  @brief      This function serves to get dma tx buff pointer.
 *  @param[in]  chn - dma channel
 *  @return     the result of tx read pointer.
 */
static inline unsigned int audio_get_tx_dma_rptr(dma_chn_e chn)
{
    return convert_ram_addr_bus2cpu(reg_dma_src_addr(chn));
}

/**
 *  @brief      This function serves to get dma rx buff pointer.
 *  @param[in]  chn - dma channel
 *  @return     the result of rx write pointer.
 */
static inline unsigned int audio_get_rx_dma_wptr(dma_chn_e chn)
{
    return convert_ram_addr_bus2cpu(reg_dma_dst_addr(chn));
}

/**
 *  @brief      This function serves to set  tx buff length
 *  @param[in]  tx_fifo_chn - the fifo channel.
 *  @param[in]  len - the length of tx buff
 *  @return     none
 */
static inline void audio_set_tx_buff_len(audio_fifo_chn_e tx_fifo_chn, unsigned short len)
{
    reg_tx_max(tx_fifo_chn) = ((len) >> 2) - 1;
}

/**
 *  @brief      This function serves to set rx buff length.
 *  @param[in]  rx_fifo_chn - the fifo channel
 *  @param[in]  len - the length of rx buff
 *  @return     none
 */
static inline void audio_set_rx_buff_len(audio_fifo_chn_e rx_fifo_chn, unsigned short len)
{
    reg_rx_max(rx_fifo_chn) = ((len) >> 2) - 1;
}

/**
 *  @brief      This function serves to tx fifo dma trigger number.
 *  @param[in]  tx_fifo_chn - the fifo channel.
 *  @param[in]  number - the number of dma trigger
 *  @return     none
 */
static inline void audio_set_fifo_tx_trig_num(audio_fifo_chn_e tx_fifo_chn, unsigned char number)
{
    reg_fifo_trig(tx_fifo_chn) = (reg_fifo_trig(tx_fifo_chn) & (~FLD_AUDIO_FIFO_AOUT_TRIG_NUM)) | (number & FLD_AUDIO_FIFO_AOUT_TRIG_NUM);
}

/**
 *  @brief      This function serves to rx fifo dma trigger number.
 *  @param[in]  rx_fifo_chn - the fifo channel.
 *  @param[in]  number - the number of dma trigger
 *  @return     none
 */
static inline void audio_set_fifo_rx_trig_num(audio_fifo_chn_e rx_fifo_chn, unsigned char number)
{
    reg_fifo_trig(rx_fifo_chn) = (reg_fifo_trig(rx_fifo_chn) & (~FLD_AUDIO_FIFO_AIN_TRIG_NUM)) | ((number & 0x0f) << 4);
}

/**
 *  @brief      This function serves to stream0 digital gain0.
 *  @param[in]  d_gain - gain value(both ch0_l-left/ch1_r-right)
 *  @return     none
 */
static inline void audio_set_stream0_dig_gain0(codec_in_path_digital_gain0_e d_gain)
{
    reg_codec_alc_ctr5 = (reg_codec_alc_ctr5 & (~FLD_AUDIO_CODEC_DEC_GAIN)) | (d_gain << 4);
}

/**
 *  @brief      This function serves to stream0 digital gain1.
 *  @param[in]  d_gain - gain value(both ch0_l-left/ch1_r-right)
 *  @return     none
 */
static inline void audio_set_stream0_dig_gain1(codec_in_path_digital_gain1_e d_gain)
{
    reg_codec_dec0_vol0 = d_gain;
}

/**
 *  @brief      This function serves to stream1 digital gain.
 *  @param[in]  d_gain- gain value(both ch0_l-left/ch1_r-right)
 *  @return     none
 */
static inline void audio_set_stream1_dig_gain(codec_in_path_digital_gain1_e d_gain)
{
    reg_codec_dec1_vol = d_gain;
}

/**
 *  @brief      This function serves to adc pga(left channel) gain.
 *  @param[in]  pga_gain - gain value
 *  @return     none
 */
static inline void audio_set_adc_pga_l_gain(codec_in_pga_gain_e pga_gain)
{
    reg_codec_ana_reg0 = pga_gain;
}

/**
 *  @brief      This function serves to adc pga(right channel) gain.
 *  @param[in]  pga_gain - gain value
 *  @return     none
 */
static inline void audio_set_adc_pga_r_gain(codec_in_pga_gain_e pga_gain)
{
    reg_codec_ana_reg11 = (reg_codec_ana_reg11 & (~FLD_AUDIO_PGA_VOL_IN_R)) | pga_gain;
}

/**
 *  @brief      This function serves to dac pga(left channel) gain.
 *  @param[in]  pga_gain - gain value
 *  @return     none
 */
static inline void audio_set_dac_pga_l_gain(codec_out_pga_gain_e pga_gain)
{
    reg_codec_pga_r_1 = pga_gain & 0xff;
    reg_codec_pga_r_2 = (reg_codec_pga_r_2 & (~FLD_AUDIO_PGA_GAIN_R_H)) | ((pga_gain >> 8) & 0x07);
}

/**
 *  @brief      This function serves to dac pga(right channel) gain.
 *  @param[in]  pga_gain - gain value
 *  @return     none
 */
static inline void audio_set_dac_pga_r_gain(codec_out_pga_gain_e pga_gain)
{
    reg_codec_pga_l_1 = pga_gain & 0xff;
    reg_codec_pga_l_2 = (reg_codec_pga_l_2 & (~FLD_AUDIO_PGA_GAIN_L_H)) | ((pga_gain >> 8) & 0x07);
}

/**
 *  @brief      This function serves to mute dac (left channel) from pga.
 *  @return     none
 */
static inline void audio_set_dac_pga_l_mute(void)
{
    reg_codec_pga_l_2 |= FLD_AUDIO_PGA_MUTE_L;
}

/**
 *  @brief      This function serves to mute dac (right channel) from pga.
 *  @return     none
 */
static inline void audio_set_dac_pga_r_mute(void)
{
    reg_codec_pga_r_2 |= FLD_AUDIO_PGA_MUTE_R;
}

/**
 *  @brief      This function serves to mute dac (left channel) from pga.
 *  @return     none
 */
static inline void audio_set_dac_pga_l_unmute(void)
{
    reg_codec_pga_l_2 &= (~FLD_AUDIO_PGA_MUTE_L);
}

/**
 *  @brief      This function serves to mute dac (right channel) from pga.
 *  @return     none
 */
static inline void audio_set_dac_pga_r_unmute(void)
{
    reg_codec_pga_r_2 &= (~FLD_AUDIO_PGA_MUTE_R);
}

/**
 *  @brief      This function serves to dac(left channel) volume digital gain.
 *  @param[in]  d_gain - gain value
 *  @return     none
 */
static inline void audio_set_dac_l_gain(codec_out_path_digital_gain_e d_gain)
{
    reg_codec_int_vol_l_2 = d_gain & 0xff;
    reg_codec_int_vol_l_1 = (reg_codec_int_vol_l_1 & (~FLD_AUDIO_CODEC_INT_VOL_L_L)) | ((d_gain >> 8) & 0x0f);
}

/**
 *  @brief      This function serves to dac(right channel) volume digital gain.
 *  @param[in]  d_gain - gain value
 *  @return     none
 */
static inline void audio_set_dac_r_gain(codec_out_path_digital_gain_e d_gain)
{
    reg_codec_int_vol_r_2 = d_gain & 0xff;
    reg_codec_int_vol_r_1 = (reg_codec_int_vol_r_1 & (~FLD_AUDIO_CODEC_INT_VOL_R_L)) | ((d_gain >> 8) & 0x0f);
}

/**
 * @brief This function serves to set the align trigger threshold for IIS.
 * @param[in]  tick - trigger threshold tick
 * @return    none
 */
static inline void audio_set_i2s_align_th_tick(unsigned int tick)
{
    reg_i2s_timer_th = tick;
}

/**
 *  @brief      This function serves to disable i2s is align mask
 *  @return     none
 */
static inline void audio_set_i2s_align_mask_dis(void)
{
    reg_usb_audio_ctrl |= FLD_I2S_ALIGN_MASK;
}

/**
 *  @brief      This function serves to enable i2s is align mask
 *  @return     none
 */
static inline void audio_set_i2s_align_mask_en(void)
{
    reg_usb_audio_ctrl &= ~FLD_I2S_ALIGN_MASK;
}

/**
 * @brief      This function reset AUDIO module.
 * @return     none
 */
static inline void audio_reset(void)
{
    reg_rst2 &= (~FLD_RST2_AUD);
    reg_rst2 |= FLD_RST2_AUD;
}

/**
 * @brief     This function serves to reset fifo.
 * @return    none
 */
static inline void audio_fifo_reset(audio_fifo_chn_e fifo_chn)
{
    reg_i2s0_cfg2 |= ((fifo_chn == FIFO0) ? FLD_AUDIO_FIFO0_RST : FLD_AUDIO_FIFO1_RST);
    reg_i2s0_cfg2 &= (~((fifo_chn == FIFO0) ? FLD_AUDIO_FIFO0_RST : FLD_AUDIO_FIFO1_RST));
}

/**
 * @brief     This function configures stream0 dmic pin.
 * @param[in] dmic0_data -the data of  dmic pin
 * @param[in] dmic0_clk1 - the clk1 of dmic pin
 * @param[in] dmic0_clk2 - the clk2 of dmic pin,if need not set clk2,please set GPIO_NONE_PIN
 * @return    none
 */
void audio_set_stream0_dmic_pin(gpio_func_pin_e dmic0_data, gpio_func_pin_e dmic0_clk1, gpio_func_pin_e dmic0_clk2);

/**
 * @brief     This function configures stream1 dmic pin.
 * @param[in] dmic1_data -the data of dmic  pin
 * @param[in] dmic1_clk1 - the clk1 of dmic pin
 * @param[in] dmic1_clk2 - the clk2 of dmic pin,if need not set clk2,please set GPIO_NONE_PIN
 * @return    none
 */
void audio_set_stream1_dmic_pin(gpio_func_pin_e dmic1_data, gpio_func_pin_e dmic1_clk1, gpio_func_pin_e dmic1_clk2);

/**
 * @brief     This function serves to enable rx_dma channel.
 * @param[in] chn   - dma channel
 * @return    none
 */
void audio_rx_dma_en(dma_chn_e chn);

/**
* @brief     This function serves to disable rx_dma channel.
* @param[in] chn   - dma channel
* @return    none
*/
void audio_rx_dma_dis(dma_chn_e chn);

/**
* @brief     This function serves to enable tx_dma channel.
* @param[in] chn   - dma channel
* @return    none
*/
void audio_tx_dma_en(dma_chn_e chn);

/**
 * @brief     This function serves to disable tx_dma channel.
 * @param[in] chn   - dma channel
 * @return    none
 */
void audio_tx_dma_dis(dma_chn_e chn);

/**
 * @brief     This function serves to config  rx_dma channel.
 * @param[in] chn          - dma channel
 * @param[in] dst_addr     - Pointer to data buffer, it must be 4-bytes aligned address.
 *                           and the actual buffer size defined by the user needs to be not smaller than the data_len, otherwise there may be an out-of-bounds problem.
 * @param[in] data_len     - Length of DMA in bytes, it must be set to a multiple of 4. The maximum value that can be set is 0x10000.
 * @param[in] head_of_list - the head address of dma llp.
 * @return    none
 */
void audio_rx_dma_config(dma_chn_e chn, unsigned short *dst_addr, unsigned int data_len, dma_chain_config_t *head_of_list);

/**
 * @brief     This function serves to set rx dma chain transfer
 * @param[in] config_addr - the head of list of llp_pointer.
 * @param[in] llpointer   - the next element of llp_pointer.
 * @param[in] dst_addr    - Pointer to data buffer, it must be 4-bytes aligned address and the actual buffer size defined by the user needs to
 *                          be not smaller than the data_len, otherwise there may be an out-of-bounds problem.
 * @param[in] data_len    - Length of DMA in bytes, it must be set to a multiple of 4. The maximum value that can be set is 0x10000.
 * @return    none
 */
void audio_rx_dma_add_list_element(dma_chain_config_t *rx_config, dma_chain_config_t *llpointer, unsigned short *dst_addr, unsigned int data_len);

/**
 * @brief     This function serves to set audio rx dma chain transfer.
 * @param[in] rx_fifo_chn - rx fifo select.
 * @param[in] chn         - dma channel
 * @param[in] in_buff     - Pointer to data buffer, it must be 4-bytes aligned address and the actual buffer size defined by the user needs to
 *                          be not smaller than the data_len, otherwise there may be an out-of-bounds problem.
 * @param[in] buff_size   - Length of DMA in bytes, it must be set to a multiple of 4. The maximum value that can be set is 0x10000.
 * @return    none
 */
void audio_rx_dma_chain_init(audio_fifo_chn_e rx_fifo_chn, dma_chn_e chn, unsigned short *in_buff, unsigned int buff_size);

/**
 * @brief     This function serves to config  tx_dma channel.
 * @param[in] chn          - dma channel
 * @param[in] src_addr     - Pointer to data buffer, it must be 4-bytes aligned address.
 * @param[in] data_len     - Length of DMA in bytes, range from 1 to 0x10000.
 * @param[in] head_of_list - the head address of dma llp.
 * @return    none
 */
void audio_tx_dma_config(dma_chn_e chn, unsigned short *src_addr, unsigned int data_len, dma_chain_config_t *head_of_list);

/**
 * @brief     This function serves to set tx dma chain transfer
 * @param[in] config_addr - the head of list of llp_pointer.
 * @param[in] llpointer   - the next element of llp_pointer.
 * @param[in] src_addr    - Pointer to data buffer, it must be 4-bytes aligned address.
 * @param[in] data_len    - Length of DMA in bytes, range from 1 to 0x10000.
 * @return    none
 */
void audio_tx_dma_add_list_element(dma_chain_config_t *config_addr, dma_chain_config_t *llpointer, unsigned short *src_addr, unsigned int data_len);

/**
 * @brief     This function serves to initialize audio tx dma chain transfer.
 * @param[in] tx_fifo_chn - tx fifo select.
 * @param[in] chn         - dma channel
 * @param[in] out_buff    - Pointer to data buffer, it must be 4-bytes aligned address.
 * @param[in] buff_size   - Length of DMA in bytes, range from 1 to 0x10000.
 * @return    none
 */
void audio_tx_dma_chain_init(audio_fifo_chn_e tx_fifo_chn, dma_chn_e chn, unsigned short *out_buff, unsigned int buff_size);

/**
 *  @brief      This function serves to set stream0 sample_rate.
 *  @param[in]  rate - sample_rate
 *  @return     none
 */
void audio_set_codec_stream0_sample_rate(audio_sample_rate_e rate);

/**
 *  @brief      This function serves to set stream1 sample_rate.
 *  @param[in]  rate - sample_rate
 *  @return     none
 */
void audio_set_codec_stream1_sample_rate(audio_sample_rate_e rate);

/**
 *  @brief      This function serves to set dac sample_rate.
 *  @param[in]  rate - sample_rate
 *  @return     none
 */
void audio_set_codec_dac_sample_rate(audio_sample_rate_e rate);

/**
 *  @brief      This function serves to set stream0 input mode.
 *  @param[in]  input_mode - 0:line_in or amic 1:dmic
 *  @return     none
 */
void audio_set_codec_stream0_input_mode(audio_stream0_input_mode_e input_mode);

/**
 * @brief This function serves to configure audio stream0 fifo input.
 * @param[in]  fifo_chn        - select channel fifo
 * @param[in]  source          - codec input source
 * @param[in]  data_width      - fifo data bit width.
 * @return    none
 */
void audio_codec_stream0_fifo_input_config(audio_fifo_chn_e fifo_chn, codec_stream0_input_src_e source, audio_codec_wl_mode_e data_width);

/**
 * @brief This function serves to configure audio stream0 input.
 * @param[in]  source          - codec input source select.
 * @param[in]  rate            - select channel sample rate
 * @return    none
 */
void audio_codec_stream0_input_config(codec_stream0_input_src_e source, audio_sample_rate_e rate);

/**
 *  @brief      This function serves to  swap stream0 data .
 *  @param[in]  en - DATA_INVERT_DIS/DATA_INVERT_EN
 *  @return     none
 */
void audio_swap_stream0_data(audio_data_invert_e en);

/**
  * @brief This function serves to configure audio stream0 channel select swap.
  * @param[in]  fifo_chn        - select channel fifo
  * @param[in]  source          - audio stream0 input source select.
  * @param[in]  rate            - select channel sample rate
  * @return    none
  * @note
  * Condition 1 (default)
  * (1) audio_swap_stream0_data(DATA_INVERT_DIS)
  * (2)mono:fifo0->ch0_l on
  *         fifo1->ch1_r on
  * Condition 2
  * (1) audio_swap_stream0_data(DATA_INVERT_EN)
  * (2) mono:fifo0->ch1_r on
  *          fifo1->ch0_l on
  *
  *
  */
void audio_codec_swap_stream0_data(audio_fifo_chn_e fifo_chn, codec_stream0_input_src_e source, audio_sample_rate_e rate);

/**
  *     @brief      This function serves to set stream0 input path.
  *     @param[in]  che_en  - channel selection
  *     @param[in]  rate    - select channel sample rate
  *     @return     none
  */
void audio_set_codec_stream0_path(codec_stream0_input_src_e che_en, audio_sample_rate_e rate);

/**
  * @brief This function serves to configure audio stream1 fifo input.
  * @param[in]  fifo_chn        - select channel fifo
  * @param[in]  source          - codec input source
  * @param[in]  data_width      - fifo data bit width.
  * @return    none
  */
void audio_codec_stream1_fifo_input_config(audio_fifo_chn_e fifo_chn, codec_stream1_input_src_e source, audio_codec_wl_mode_e data_width);

/**
  * @brief This function serves to configure audio stream1 input.
  * @param[in]  source          - audio input source select.
  * @param[in]  rate            - select channel sample rate
  * @return    none
  */
void audio_codec_stream1_input_config(codec_stream1_input_src_e source, audio_sample_rate_e rate);

/**
 *  @brief      This function serves to  swap stream1 data.
 *  @param[in]  en - DATA_INVERT_DIS/DATA_INVERT_EN
 *  @return     none
 */
void audio_swap_stream1_data(audio_data_invert_e en);

/**
  * @brief This function serves to configure audio stream1 channel select swap.
  * @param[in]  fifo_chn        - select channel fifo
  * @param[in]  source          - audio stream1 input source select.
  * @param[in]  rate            - select channel sample rate
  * @return    none
  * @note
  * Condition 1 (default)
  * (1) audio_swap_stream1_data(DATA_INVERT_DIS)
  * (2)mono:fifo0->ch0_l on
  *         fifo1->ch1_r on
  * Condition 2
  * (1) audio_swap_stream1_data(DATA_INVERT_EN)
  * (2) mono:fifo0->ch1_r on
  *          fifo1->ch0_l on
  *
  *
  */
void audio_codec_swap_stream1_data(audio_fifo_chn_e fifo_chn, codec_stream1_input_src_e source, audio_sample_rate_e rate);

/**
   *    @brief      This function serves to set stream1 input path.
   *    @param[in]  che_en  - channel selection
   *    @param[in]  rate    - select channel sample rate
   *    @return     none
   */
void audio_set_codec_stream1_path(codec_stream1_input_src_e che_en, audio_sample_rate_e rate);

/**
  *     @brief      This function serves to  swap data .
  *     @param[in]  en - DATA_INVERT_DIS/DATA_INVERT_EN
  *     @return     none
  */
void audio_swap_dac_data(audio_data_invert_e en);

/**
 * @brief     This function serves to set codec stream0 input data bit width mode for fifo0 or fifo1 .
 * @param[in]  fifo_chn - fifo0 or fifo1
 * @param[in]  ain_mode - fifo input data bit width selection
 * @return    none
 */
void audio_set_stream0_fifo_input_mode(audio_fifo_chn_e fifo_chn, audio_codec_in_mode_e ain_mode);

/**
 * @brief     This function serves to set codec stream1 input data bit width mode for fifo0 or fifo1 .
 * @param[in]  fifo_chn - fifo0 or fifo1
 * @param[in]  ain_mode - fifo input data bit width selection
 * @return    none
 */
void audio_set_stream1_fifo_input_mode(audio_fifo_chn_e fifo_chn, audio_codec_in_mode_e ain_mode);

/**
 * @brief      This function serves to set codec stream0 input data bit width for fifo0 or fifo1 .
 * @param[in]  fifo_chn   - fifo0 or fifo1
 * @param[in]  source     - audio input source select.
 * @param[in]  data_width - input data bit width selection
 * @return    none
 */
void audio_codec_set_stream0_fifo_input_mode(audio_fifo_chn_e fifo_chn, codec_stream0_input_src_e source, audio_codec_wl_mode_e data_width);

/**
 * @brief      This function serves to set codec stream1 input data bit width mode for fifo0 or fifo1 .
 * @param[in]  fifo_chn   - fifo0 or fifo1
 * @param[in]  source     - audio input source select.
 * @param[in]  data_width - fifo input data bit width selection
 * @return    none
 */
void audio_codec_set_stream1_fifo_input_mode(audio_fifo_chn_e fifo_chn, codec_stream1_input_src_e source, audio_codec_wl_mode_e data_width);

/**
 * @brief     This function serves to set codec dac data output data path.
 * @param[in] int_aout_mode - fifo output source select
 * @return    none
 */
void audio_set_dac_fifo_output_mode(audio_out_mode_e int_aout_mode);

/**
 * @brief      This function serves to set codec dac data bit width
 * @param[in]  fifo_chn     - fifo select
 * @param[in]  source       - audio input source select.
 * @param[in]  wdata_width  - output data bit width selection
 * @return    none
 */
void audio_set_dac_fifo_output_mode_config(audio_fifo_chn_e fifo_chn, audio_dac_output_src_e source, audio_codec_wl_mode_e data_width);

/**
 * @brief     This function serves to set i2s data input data bit width mode.
 * @param[in] fifo_chn      - fifo select
 * @param[in] i2s_select    - i2s channel select
 * @param[in] i2s_ain_mode  - fifo i2s input mode select
 * @return    none
 */
void audio_set_i2s_fifo_input_mode(audio_fifo_chn_e fifo_chn, audio_i2s_select_e i2s_select, audio_i2s_in_mode_e i2s_ain_mode);

/**
 * @brief     This function serves to set i2s data output data bit width mode.
 * @param[in] i2s_select      - i2s channel select
 * @param[in] i2s_aout_mode   - fifo i2s output mode select
 * @return    none
 */
void audio_set_i2s_fifo_output_mode(audio_i2s_select_e i2s_select, audio_out_mode_e i2s_aout_mode);

/**
 * @brief     This function serves to set fifo input data path.
 * @param[in] fifo_chn - fifo select
 * @param[in] ain_sel  - fifo input source select
 * @return    none
 */
void audio_data_fifo_input_path_sel(audio_fifo_chn_e fifo_chn, audio_mux_ain_e ain_sel);

/**
 * @brief     This function serves to set fifo output data path .
 * @param[in] fifo_chn - fifo select
 * @param[in] aout_sel - fifo output source select
 * @return    none
 */
void audio_data_fifo_output_path_sel(audio_fifo_chn_e fifo_chn, audio_mux_aout_e aout_sel);

/**
 * @brief This function serves to configure audio stream output.
 * @param[in]  chn             - audio output source select.
 * @param[in]  rate            - select channel sample rate
 * @param[in]  dac_mode        - select channel dac mode
 * @return    none
 */
void audio_codec_stream_output_config(audio_dac_output_src_e chn, audio_sample_rate_e rate, codec_dac_mode_e dac_mode);

/**
 * @brief     This function configures i2s mclk pin for external codec,mclk=192M*(div_numerator/div_denominator):192M*(1/16)=12M.
 * @param[in] mclk_pin -mclk output pin.
 * @param[in] div_numerator
 * @param[in] div_denominator
 * @return    none
 * @attention If need to use internal codec at the same time, mclk must be set to 12M.
 */
void audio_i2s_set_mclk(gpio_func_pin_e mclk_pin, unsigned short div_numerator, unsigned short div_denominator);

/**
 * @brief     This function configures i2s pin.
 * @param[in] i2s_select - i2s channel select
 * @param[in] config     - i2s config pin struct
 * @return    none
 */
void i2s_set_pin(audio_i2s_select_e i2s_select, i2s_pin_config_t *config);

/**
 * @brief     This function serves to config i2s0 interface, word length, and m/s.
 * @param[in] i2s_sel      - i2s channel select
 * @param[in] i2s_format   - interface protocol
 * @param[in] wl           - audio data word length
 * @param[in] m_s          - select i2s as master or slave
 * @param[in] i2s_config_t - the ptr of i2s_config_t that configure i2s lr_clk phase and lr_clk swap.
 *  i2s_config_t->i2s_lr_clk_invert_select-lr_clk phase control(in RJ,LJ or i2s modes),in i2s mode(opposite phasing in  RJ,LJ mode), 0=right channel data when lr_clk high ,1=right channel data when lr_clk low.
 *                                                                                     in DSP mode(in DSP mode only), DSP mode A/B select,0=DSP mode A ,1=DSP mode B.
 *            i2s_config_t->i2s_data_invert_select - 0=left channel data left,1=right channel data left.
 * but data output channel will be inverted,you can also set i2s_config_t->i2s_data_invert_select=1 to recovery it.
 * @return    none
 */
void audio_i2s_config(audio_i2s_select_e i2s_sel, i2s_mode_select_e i2s_format, audio_i2s_wl_mode_e wl, i2s_m_s_mode_e m_s, audio_i2s_invert_config_t *i2s_config_t);

/**
 * @brief     This function serves to set sampling rate when i2s as master.
 * @param[in] i2s_select                    - i2s channel select
 * @param[in] i2s_clk_config                 i2s_clk_config[2]   i2s_clk_config[3]-->lrclk_adc(sampling rate)
 *                                                  ||                  ||
 *  pll(192M default)------->div---->i2s_clk--->2 * div(div=0,bypass)-->blck----->div
 *                           ||                                         ||
 *           i2s_clk_config[0]/i2s_clk_config[1]                 i2s_clk_config[4]-->lrclk_dac (sampling rate)
 *
 For example:sampling rate=16K,i2s_clk_config[5]={ 8,125,6,64,64}, sampling rate=192M*(8/125)/(2*6)/64=16K

 * @return    none
 * @attention The default is from pll 192M(default). If the pll is changed, the clk will be changed accordingly.
 */
_attribute_ram_code_sec_noinline_ void audio_set_i2s_clock(audio_i2s_select_e i2s_select, unsigned short *i2s_clk_config);

/**
 * @brief     This function set i2s input .
 * @param[in] fifo_chn         - fifo channel selection
 * @param[in] i2s_selecti2s    - channel select
 * @param[in] data_mode        - i2s data bit with
 * @param[in] ch               - i2s channel selection
 * @return    none
 */
void audio_set_i2s_input_chn_wl(audio_fifo_chn_e fifo_chn, audio_i2s_select_e i2s_select, audio_i2s_wl_mode_e data_mode, i2s_channel_select_e ch);

/**
 * @brief     This function set i2s output .
 * @param[in] fifo_chn         - fifo channel selection
 * @param[in] i2s_selecti2s    - channel select
 * @param[in] data_mode        - i2s data bit with
 * @param[in] ch               - i2s channel selection
 * @return    none
 */
void audio_set_i2s_output_chn_wl(audio_fifo_chn_e fifo_chn, audio_i2s_select_e i2s_select, audio_i2s_wl_mode_e data_mode, i2s_channel_select_e ch);

/**
 * @brief      This function serves to set i2c master for codec i2c slave .
 * @param[in]  sda_pin - the pin port selected as I2C sda pin port.
 * @param[in]  scl_pin - the pin port selected as I2C scl pin port.
 * @param[in]  clock   - the division factor of I2C clock,
 *             I2C frequency = System_clock / (4*DivClock).
 * @return     none.
 */
void audio_i2c_init(gpio_func_pin_e sda_pin, gpio_func_pin_e scl_pin, unsigned char clock);

/**
  * @brief     This function serves to power on audio.
  * @return    none
  */
void audio_power_on(void);

/**
  * @brief      This function serves to power down all audio power.
  * @return     none
  * @attention  1.audio digital registers are lost and need to be reinitialized
  *             2.If you only need to turn off adc or dac power, please configure audio_codec_adc_power_down()/audio_codec_dac_power_down().
  */
_attribute_ram_code_sec_optimize_o2_ void audio_power_down(void);

/**
  * @brief     This function serves to initial audio.
  * @return    none
  */
void audio_codec_init(void);

/**
  * @brief     This function serves to set codec_adc clock.
  * @return    none
  */
void audio_codec_set_adc_clock(void);

/**
  * @brief      This function serves to power on codec_adc.
  * @param[in]  ch_en                - dac chn enable
  * @param[in]  micbias_power_en     - micbias power enable
  * @return    none
  */
void audio_codec_adc_power_on(audio_chn_sel_e ch_en, power_switch_e micbias_power_en);

/**
  * @brief     This function serves to power down codec_adc.
  * @return    none
  */
_attribute_ram_code_sec_optimize_o2_ void audio_codec_adc_power_down(void);

/**
  * @brief     This function serves to  select dac/drv lbias current.
  * @param[in] lbias_curr-  current  select
  * @return    none
  */
void audio_codec_set_output_lbias_curr(codec_lbias_curr_sel_e lbias_curr);

/**
  * @brief      This function serves to dac initialize.
  * @param[in]  work_mode - dac work mode
  * @return     none
  */
void audio_codec_dac_init(codec_dac_mode_e work_mode);

/**
  * @brief     This function serves to power on codec_dac.
  * @param[in]  ch_en - dac chn enable
  * @return    none
  */
void audio_codec_dac_power_on(audio_chn_sel_e ch_en);

/**
  * @brief     This function serves to power down codec_dac.
  * @return    none
  */
_attribute_ram_code_sec_optimize_o2_ void audio_codec_dac_power_down(void);

/**
 * @brief     This function serves to power on codec_adc.
 * @param[in]  en           - POWER_DOWN or POWER_ON
 * @param[in]  micbias_mode - select micbias mode
 * @return    none
 */
_attribute_ram_code_sec_optimize_o2_ void audio_codec_set_micbias(power_switch_e en, micbias_work_mode_e micbias_mode);

/**
 * @brief This function serves to initialize audio stream0 fifo.
 * @param[in]  audio_codec_input     - input configuration data structure pointer.
 * @return    none
 */
void audio_codec_stream0_input_init(audio_codec_stream0_input_t *audio_codec_input);
/**
 * @brief This function serves to initialize audio stream1 fifo.
 * @param[in]  audio_codec_input     - input configuration data structure pointer.
 * @return    none
 */
void audio_codec_stream1_input_init(audio_codec_stream1_input_t *audio_codec_input);

/**
 * @brief This function serves to initialize audio stream output.
 * @param[in]  audio_codec_output   - output configuration data structure pointer.
 * @return    none
 * @note   When mono channel, The hardware will output the data of mono channel on the dac L and R at the same time.
 *         If only need one channel output, in order to save power, you can turn off any channel
 * For example:audio_codec_output->output_src= CODEC_DAC_MONO_L or CODEC_DAC_MONO_R select mono channel
 */
void audio_codec_stream_output_init(audio_codec_output_t *audio_codec_output);

/**
 * @brief This function serves to initialize  configuration i2s.
 * @param[in]  i2s_config - the relevant configuration struct pointer
 * @return    none
 */
void audio_i2s_config_init(audio_i2s_config_t *i2s_config);

/**
 * @brief This function serves to initialize  input i2s.
 * @param[in] audio_i2s_input - the relevant input struct pointer
 * @return    none
 */
void audio_i2s_input_init(audio_i2s_input_output_t *audio_i2s_input);

/**
 * @brief This function serves to initialize  output i2s.
 * @param[in]  audio_i2s_output - the relevant output struct pointer
 * @return    none
 */
void audio_i2s_output_init(audio_i2s_input_output_t *audio_i2s_output);

/**
 * @brief This function serves to enable the i2s align function.
 * @return    none
 */
void audio_i2s_align_en(void);

/**
 * @brief This function serves to disable the i2s align function.
 * @return    none
 */
void audio_i2s_align_dis(void);

/**
 * @brief This function serves to sets the align data bit width .(i2s sync mode 16bit:{i2s1_r[15:0], i2s1_l[15:0]}
 *                                                                                   {i2s0_r[15:0], i2s0_l[15:0]}
 *                                                              (i2s sync mode 20bit:{12{i2s1_l[19]},i2s1_l[19:0]}
 *                                                                                   {12{i2s1_r[19]},i2s1_r[19:0]}
 *                                                                                   {12{i2s0_l[19]},i2s0_l[19:0]}
 *                                                                                   {12{i2s0_r[19]},i2s0_r[19:0]}
 *                                                              (i2s sync mode 24bit:{8{i2s1_l[23]}, i2s1_l[23:0]}
 *                                                                                   {8{i2s1_r[23]}, i2s1_r[23:0]}
 *                                                                                   {8{i2s0_l[23]}, i2s0_l[23:0]}
 *                                                                                   {8{i2s0_r[23]}, i2s0_r[23:0]}
 * @param[in]  align_data_width - align data bit widths
 * @return    none
 */
void audio_set_i2s_align_data_width(audio_i2s_align_data_width_e align_data_width);
#endif
