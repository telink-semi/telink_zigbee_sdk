/********************************************************************************************************
 * @file    audio.h
 *
 * @brief   This is the header file for TL721X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "reg_include/audio_reg.h"
#include "lib/include/stimer.h"

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/

/**
 *  @brief  Define fifo type
 */
typedef enum
{
    FIFO0 = 0,
    FIFO1,
    FIFO2,
} audio_fifo_chn_e;

/**
 *  @brief  Define the i2s peripheral selection.
 */
typedef enum
{
    I2S0 = 0,
    I2S1,
    I2S2,
} audio_i2s_select_e;

/**
 *  @brief  Define the ascl peripheral selection.
 */
typedef enum
{
    ASCL0 = 0x02,
    ASCL1 = 0x00,
    ASCL2 = 0x01,
} audio_ascl_select_e;

/**
 *  @brief  Define the codec input mode selection.
 */
typedef enum
{
    AMIC_OR_LINE_INPUT = 0,
    DMIC_INPUT,
} audio_codec_input_mode_e;

/**
 *  @brief  Define the audio fifo Interrupt type.
 */
typedef enum
{
    AUDIO_RX_FIFO0_IRQ = BIT(0),
    AUDIO_RX_FIFO1_IRQ = BIT(1),
    AUDIO_RX_FIFO2_IRQ = BIT(2),
    AUDIO_TX_FIFO0_IRQ = BIT(3),
    AUDIO_TX_FIFO1_IRQ = BIT(4),
    AUDIO_TX_FIFO2_IRQ = BIT(5),
} audio_fifo_irq_type_e;

/**
 *  @brief  Define the i2s mode selection.
 */
typedef enum
{
    I2S_RJ_MODE,
    I2S_LJ_MODE,
    I2S_I2S_MODE,
    I2S_DSP_MODE,
    I2S2_TDM_MODE,
} i2s_mode_select_e;

/**
 *  @brief  Define the ascl mode selection.
 */
typedef enum
{
    LOW_BIT_16,
    HIGH_BIT_16,
} ascl_mode_select_e;

/**
 *  @brief  Define the i2s data bit width selection.
 */
typedef enum
{
    I2S_BIT_16_DATA,
    I2S_BIT_20_DATA,
    I2S_BIT_24_DATA,
} audio_i2s_wl_mode_e;

/**
 *  @brief  Define the audio i2s data bit width.
 */
typedef enum
{
    BIT_16_DATA,
    BIT_20_24_DATA,
} audio_i2s_fifo_wl_e;

/**
 *  @brief  Define the audio code data bit width.
 */
typedef enum
{
    CODEC_BIT_16_DATA, // sdm currently only supports 16bit data bit widths.
    CODEC_BIT_20_DATA,
} audio_codec_wl_mode_e;

/**
 *  @brief  Define the audio channel selection.
 */
typedef enum
{
    AUDIO_MONO,
    AUDIO_STEREO,
} audio_channel_select_e;

/**
 *  @brief  Define the audio output source selection.
 */
typedef enum
{
    SDM_MONO,
    SDM_STEREO,
} audio_stream_output_src_e;

/**
 *  @brief  Define the audio channel selection.
 */
typedef enum
{
    CHANNEL_LEFT = BIT(0),
    CHANNEL_RIGHT = BIT(1),
    CHANNEL_STEREO = BIT(0) | BIT(1),
} audio_chn_sel_e;

/**
 *  @brief  Define the audio i2s master slave mode.
 */
typedef enum
{
    I2S_AS_SLAVE_EN,
    I2S_AS_MASTER_EN,
} i2s_m_s_mode_e;

/**
 *  @brief  Define audio  data invert type
 */
typedef enum
{
    I2S_DATA_INVERT_DIS,
    I2S_DATA_INVERT_EN,
} audio_data_invert_e;

/**
 *  @brief  Define i2s clock invert enable/disable
 */
typedef enum
{
    I2S_LR_CLK_INVERT_DIS,
    I2S_LR_CLK_INVERT_EN,
} audio_i2s_lr_clk_invert_e;

/**
 *  @brief  Define the audio i2s line mode.
 */
typedef enum
{
    I2S_5LINE_MODE,
    I2S_4LINE_DAC_MODE,
    I2S_4LINE_ADC_MODE,
} i2s_line_mode_e;

/**
 *  @brief  Define the audio i2s align selection.
 */
typedef enum
{
    I2S1_I2S2_ALIGN,
    I2S0_I2S1_ALIGN,
    I2S0_I2S1_I2S2_ALIGN,
} i2s_align_mode_select_e;

/**
 *  @brief  Define the audio i2s align clock.
 */
typedef enum
{
    I2S_CLK,
    I2S_ALIGN_CLK,
} i2s_align_clk_e;

/**
 *  @brief  Define the audio i2s channel selection.
 */
typedef enum
{
    I2S_CHANNEL_LEFT = BIT(0),
    I2S_CHANNEL_RIGHT = BIT(1),
    I2S_CHANNEL_STEREO = BIT(0) | BIT(1),
} i2s_chn_select_e;

/**
 *  @brief  Define the audio i2s tdm slot number selection.
 */
typedef enum
{
    TDM_2CHANNEL,
    TDM_4CHANNEL,
    TDM_6CHANNEL,
    TDM_8CHANNEL,
} tdm_slot_number_select_e;

/**
 *  @brief  Define the audio i2s tdm mode.
 */
typedef enum
{
    TDM_MODE_A,
    TDM_MODE_B,
    TDM_MODE_C,
} tdm_mode_select_e;

/**
 *  @brief  Define the audio i2s tdm slot width selection.
 */
typedef enum
{
    BIT16_SLOT_WIDTH,
    BIT24_SLOT_WIDTH,
    BIT32_SLOT_WIDTH,
} tdm_slot_width_select_e;

/**
 *  @brief  Define the audio debug clock source.
 */
typedef enum
{
    ACLK_DBG_OPEN = BIT(0),
    I2S0_CLK_DBG = BIT(1),
    I2S1_CLK_DBG = BIT(2),
    I2S2_CLK_DBG = BIT(3),
    ADC_CLK6M_DBG = BIT(4),
} aclk_dbg_e;

/**
 *  @brief  Define the audio i2s invert configuration.
 */
typedef struct
{
    unsigned char i2s_lr_clk_invert_select;
    unsigned char i2s_data_invert_select;
} audio_i2s_invert_config_t;

/**
 *  @brief  Define the audio input pathway multiplexing options.
 */
typedef enum
{
    I2S0_IN_FIFO,
    I2S1_IN_FIFO,
    I2S2_IN_FIFO,
    CODEC_IN_FIFO,
    USB_AISO_IN_FIFO,
} audio_mux_ain_e;

/**
 *  @brief  Define the audio codec/sdm sample rate.
 */
typedef enum
{
    AUDIO_8K,       /**< 8K=12Mhz/1500 */
    AUDIO_8P0K,     /**< 8.021K=12Mhz/1496 */
    AUDIO_11P0K,    /**< 11.0259K=12Mhz/1088 */
    AUDIO_12K,      /**< 12K=12Mhz/1000 */
    AUDIO_16K,      /**< 16K=12Mhz/750 */
    AUDIO_22P0588K, /**< 22.0588K=12Mhz/544 */
    AUDIO_24K,      /**< 24K=12Mhz/500 */
    AUDIO_32K,      /**< 32K=12Mhz/375 */
    AUDIO_44P1K,    /**< 44.118K=12Mhz/272 */
    AUDIO_48K,      /**< 48k=12Mhz/250 */
    AUDIO_RATE_SIZE,
} audio_sample_rate_e;

/**
 *  @brief  Define the audio ascl sample rate.
 */
typedef enum
{
    AUDIO_ASCL_8K,
    AUDIO_ASCL_8P0K,
    AUDIO_ASCL_11P0K,
    AUDIO_ASCL_12K,
    AUDIO_ASCL_16K,
    AUDIO_ASCL_22P0588K,
    AUDIO_ASCL_24K,
    AUDIO_ASCL_32K,
    AUDIO_ASCL_44P1K,
    AUDIO_ASCL_48K,
    AUDIO_ASCL_96K,
    AUDIO_ASCL_192K,
    AUDIO_ASCL_RATE_SIZE,
} audio_ascl_sample_rate_e;

/**
 *  @brief  Define the audio stream output dither control source.
 */
typedef enum
{
    DC_CONSTANT,
    PN_SEQUENCE,
    PN_SEQUENCE_SHAPPING,
} audio_stream_output_dither_control_e;

/**
 *  @brief  Define codec input pga gain type
 */
typedef enum
{
    CODEC_IN_A_GAIN_45P2_DB, /**< 45.2dB */
    CODEC_IN_GAIN_43P5_DB,   /**< 43.5dB */
    CODEC_IN_GAIN_42P1_DB,   /**< 42.1dB */
    CODEC_IN_GAIN_40P5_DB,   /**< 40.5dB */
    CODEC_IN_GAIN_39P1_DB,   /**< 39.1dB */
    CODEC_IN_GAIN_37P4_DB,   /**< 37.4dB */
    CODEC_IN_GAIN_36P0_DB,   /**< 36.0dB */
    CODEC_IN_GAIN_34P6_DB,   /**< 34.6dB default */
    CODEC_IN_GAIN_33P0_DB,   /**< 33.0dB */
    CODEC_IN_GAIN_30P1_DB,   /**< 30.1dB */
    CODEC_IN_GAIN_27P0_DB,   /**< 27.0dB */
    CODEC_IN_GAIN_24P0_DB,   /**< 24.0dB */
    CODEC_IN_GAIN_21P0_DB,   /**< 21.0dB */
    CODEC_IN_GAIN_15P0_DB,   /**< 15.0dB */
    CODEC_IN_GAIN_9P0_DB,    /**< 9.0dB */
    CODEC_IN_GAIN_0_DB,      /**< 0dB */
} codec_in_pga_gain_e;

/**
 *  @brief  Define codec input path digital gain type
 */
typedef enum
{
    CODEC_IN_D_GAIN_m48_DB = 0,  /**< -48dB */
    CODEC_IN_D_GAIN_m42_DB = 4,  /**< -42dB */
    CODEC_IN_D_GAIN_m36_DB = 8,  /**< -36dB */
    CODEC_IN_D_GAIN_m30_DB = 12, /**< -30dB */
    CODEC_IN_D_GAIN_m24_DB = 16, /**< -24dB */
    CODEC_IN_D_GAIN_m18_DB = 20, /**< -18dB */
    CODEC_IN_D_GAIN_m12_DB = 24, /**< -12dB */
    CODEC_IN_D_GAIN_m6_DB = 28,  /**< -6dB */
    CODEC_IN_D_GAIN_0_DB = 32,   /**<  0dB  default */
    CODEC_IN_D_GAIN_6_DB = 36,   /**<  6dB */
    CODEC_IN_D_GAIN_12_DB = 40,  /**<  12dB */
    CODEC_IN_D_GAIN_18_DB = 44,  /**<  18dB */
    CODEC_IN_D_GAIN_24_DB = 48,  /**<  24dB */
    CODEC_IN_D_GAIN_30_DB = 52,  /**<  30dB */
    CODEC_IN_D_GAIN_36_DB = 56,  /**<  36dB */
    CODEC_IN_D_GAIN_42_DB = 60,  /**<  42dB */
} codec_in_path_digital_gain_e;

/**
 *  @brief  Define codec output path digital gain type
 */
typedef enum
{
    ASCL_OUT_D_GAIN_m48_DB = 0x20, /**<-48dB */
    ASCL_OUT_D_GAIN_m42_DB = 0x24, /**<-42dB */
    ASCL_OUT_D_GAIN_m36_DB = 0x28, /**<-36dB */
    ASCL_OUT_D_GAIN_m30_DB = 0x2C, /**<-30dB */
    ASCL_OUT_D_GAIN_m24_DB = 0x30, /**<-24dB */
    ASCL_OUT_D_GAIN_m18_DB = 0x34, /**<-18dB */
    ASCL_OUT_D_GAIN_m12_DB = 0x38, /**<-12dB */
    ASCL_OUT_D_GAIN_m6_DB = 0x3C,  /**<-6dB */
    ASCL_OUT_D_GAIN_0_DB = 0x40,   /**< 0dB  default  */
    ASCL_OUT_D_GAIN_6_DB = 0x44,   /**< 6dB */
    ASCL_OUT_D_GAIN_12_DB = 0x48,  /**< 12dB */
    ASCL_OUT_D_GAIN_18_DB = 0x4C,  /**< 18dB */
    ASCL_OUT_D_GAIN_24_DB = 0x50,  /**< 24dB */
    ASCL_OUT_D_GAIN_30_DB = 0x54,  /**< 30dB */
    ASCL_OUT_D_GAIN_36_DB = 0x58,  /**< 36dB */
    ASCL_OUT_D_GAIN_42_DB = 0x5C,  /**< 42dB */
} ascl_out_path_digital_gain_e;

/**
 *  @brief  Define audio codec stream0 input struct
 */
typedef enum
{
    LINE_STREAM0_NONO_L = BIT(0),
    AMIC_STREAM0_NONO_L = BIT(2) | BIT(0),
    DMIC_STREAM0_MONO_L = BIT(3) | BIT(0),
    DMIC_STREAM0_MONO_R = BIT(3) | BIT(1),
    DMIC_STREAM0_STEREO = BIT(3) | BIT(1) | BIT(0),
} codec_stream0_input_src_e;

/**
 *  @brief  Define power switch state
 */
typedef enum
{
    POWER_DOWN,
    POWER_ON,
} power_switch_e;

/**
 * @brief  Define the sdm pin definition structure.
 */
typedef struct
{
    gpio_func_pin_e sdm0_p_pin;
    gpio_func_pin_e sdm0_n_pin;
    gpio_func_pin_e sdm1_p_pin;
    gpio_func_pin_e sdm1_n_pin;
} sdm_pin_config_t;

/**
 * @brief  Define the i2s pin definition structure.
 */
typedef struct
{
    gpio_func_pin_e bclk_pin;
    gpio_func_pin_e adc_lr_clk_pin;
    gpio_func_pin_e dac_lr_clk_pin;
    gpio_func_pin_e adc_dat_pin;
    gpio_func_pin_e dac_dat_pin;
} i2s_pin_config_t;

/**
 *  @brief  Define audio codec stream0 input struct
 */
typedef struct
{
    void *data_buf;
    unsigned int data_buf_size;
    codec_stream0_input_src_e input_src;
    unsigned char sample_rate;
    unsigned char data_width;
    audio_fifo_chn_e fifo_chn;
    dma_chn_e dma_num;
} audio_codec_stream0_input_t;

/**
 *  @brief  Define audio codec output struct
 */
typedef struct
{
    void *data_buf;
    unsigned int data_buf_size;
    audio_stream_output_src_e output_src;
    unsigned char sample_rate;
    unsigned char data_width;
    dma_chn_e dma_num;
} audio_codec_output_t;

/**
 *  @brief  Define audio i2s general configuration struct
 */
typedef struct
{
    unsigned short *sample_rate;
    i2s_pin_config_t *pin_config;
    audio_i2s_select_e i2s_select;
    audio_i2s_wl_mode_e data_width;
    i2s_mode_select_e i2s_mode;
    i2s_m_s_mode_e master_slave_mode;
} audio_i2s_config_t;

/**
 *  @brief  Define audio i2s input output struct
 */
typedef struct
{
    void *data_buf;
    unsigned int data_buf_size;
    audio_i2s_select_e i2s_select;
    i2s_chn_select_e i2s_ch_sel;
    audio_i2s_wl_mode_e data_width;
    audio_fifo_chn_e fifo_chn;
    dma_chn_e dma_num;
} audio_i2s_input_output_t;

/**
 *  @brief  Define audio i2s tdm mode configuration struct
 */
typedef struct
{
    tdm_mode_select_e tdm_mode;
    tdm_slot_width_select_e slot_num;
} audio_i2s2_tdm_config_t;

/**
 *  @brief  Define audio i2s tdm mode input output struct
 */
typedef struct
{
    tdm_slot_number_select_e tdm_chn;
    audio_i2s_wl_mode_e data_width;
    dma_burst_size_e burst_size;
    audio_fifo_chn_e fifo_chn;
    dma_chn_e dma_num;
} audio_i2s2_tdm_input_output_t;

/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/
/*!
 * @name   External interface
 * @{
 */

/**
 *  @brief      This function serves to rxfifo enable.
 *  @param[in]  fifo_chn - fifo channel.
 *  @return     none.
 */
static inline void audio_rxfifo_en(audio_fifo_chn_e fifo_chn)
{
    reg_rxfifo_en |= BIT(fifo_chn);
}

/**
 *  @brief      This function serves to txfifo enable.
 *  @param[in]  fifo_chn - fifo channel.
 *  @return     none.
 */
static inline void audio_txfifo_en(audio_fifo_chn_e fifo_chn)
{
    reg_fifo_outen |= BIT(fifo_chn);
}

/**
 *  @brief      This function serves to rxfifo disable.
 *  @param[in]  fifo_chn - fifo channel.
 *  @return     none.
 */
static inline void audio_rxfifo_dis(audio_fifo_chn_e fifo_chn)
{
    reg_rxfifo_en &= ~BIT(fifo_chn);
}

/**
 *  @brief      This function serves to txfifo disable.
 *  @param[in]  fifo_chn - fifo channel.
 *  @return     none.
 */
static inline void audio_txfifo_dis(audio_fifo_chn_e fifo_chn)
{
    reg_fifo_outen &= ~BIT(fifo_chn);
}

/**
 *  @brief      This function serves to txfifo wptr pointer enable.
 *  @param[in]  fifo_chn - fifo channel.
 *  @return     none.
 */
static inline void audio_txfifo_wptr_en(audio_fifo_chn_e fifo_chn)
{
    reg_rx_wptr_en |= BIT(fifo_chn + 4); // offset value is 4.
}
/**
 *  @brief      This function serves to rxfifo wptr pointer enable.
 *  @param[in]  fifo_chn - fifo channel.
 *  @return     none.
 */
static inline void audio_rxfifo_wptr_en(audio_fifo_chn_e fifo_chn)
{
    reg_rx_wptr_en |= BIT(fifo_chn);
}

/**
 *  @brief      This function serves to rxfifo threshold enable.
 *  @param[in]  fifo_chn - fifo channel.
 *  @return     none.
 */
static inline void audio_rxfifo_th_irq_en(audio_fifo_chn_e fifo_chn)
{
    reg_fifo_th_en |= BIT(fifo_chn);
}

/**
 *  @brief      This function serves to txfifo threshold enable.
 *  @param[in]  fifo_chn - fifo channel.
 *  @return     none.
 */
static inline void audio_txfifo_th_irq_en(audio_fifo_chn_e fifo_chn)
{
    reg_fifo_th_en |= BIT(fifo_chn + 3); // offset value is 3.
}

/**
 *  @brief      This function serves to txfifo wptr pointer disable.
 *  @param[in]  fifo_chn - fifo channel.
 *  @return     none.
 */
static inline void audio_txfifo_wptr_dis(audio_fifo_chn_e fifo_chn)
{
    reg_rx_wptr_en &= ~BIT(fifo_chn + 4); // offset value is 4.
}
/**
 *  @brief      This function serves to rxfifo wptr pointer disable.
 *  @param[in]  fifo_chn - fifo channel.
 *  @return     none.
 */
static inline void audio_rxfifo_wptr_dis(audio_fifo_chn_e fifo_chn)
{
    reg_rx_wptr_en &= ~BIT(fifo_chn);
}

/**
 *  @brief      This function serves to txfifo threshold disable.
 *  @param[in]  fifo_chn - fifo channel.
 *  @return     none.
 */
static inline void audio_txfifo_th_irq_dis(audio_fifo_chn_e fifo_chn)
{
    reg_fifo_th_en &= ~BIT(fifo_chn);
}

/**
 *  @brief      This function serves to rxfifo threshold disable.
 *  @param[in]  fifo_chn - fifo channel.
 *  @return     none.
 */
static inline void audio_rxfifo_th_irq_dis(audio_fifo_chn_e fifo_chn)
{
    reg_fifo_th_en &= ~BIT(fifo_chn);
}

/**
 *  @brief      This function serves to get tx read pointer.
 *  @param[in]  tx_fifo_chn   - tx fifo channel.
 *  @return     the result of - tx read pointer.
 */
static inline unsigned short audio_get_tx_rptr(audio_fifo_chn_e tx_fifo_chn)
{
    return reg_txfifo_rptr(tx_fifo_chn);
}

/**
 *  @brief      This function serves to get rx write pointer.
 *  @param[in]  rx_fifo_chn   - rx fifo channel.
 *  @return     the result of - rx write pointer.
 */
static inline unsigned short audio_get_rx_wptr(audio_fifo_chn_e rx_fifo_chn)
{
    return reg_rxfifo_wptr(rx_fifo_chn);
}

/**
 *  @brief      This function serves to clear txfifo data.
 *  @param[in]  tx_fifo_chn - tx fifo channel.
 *  @return     none.
 */
static inline void audio_txfifo_clear(audio_fifo_chn_e tx_fifo_chn)
{
    reg_fifo_clr |= BIT(tx_fifo_chn + 4); // offset value is 4.
}

/**
 *  @brief      This function serves to clear rxfifo data.
 *  @param[in]  rx_fifo_chn - rx fifo channel.
 *  @return     none.
 */
static inline void audio_rxfifo_clear(audio_fifo_chn_e rx_fifo_chn)
{
    reg_fifo_clr |= BIT(rx_fifo_chn);
}

/**
 * @brief      This function reset AUDIO module.
 * @return     none.
 */
static inline void audio_reset(void)
{
    reg_rst2 &= (~FLD_RST2_AUD);
    reg_rst2 |= FLD_RST2_AUD;
}

/**
 *  @brief      This function serves to enable rxfifo irq.
 *  @param[in]  rx_fifo_chn - rx fifo channel.
 *  @return     none.
 */
static inline void audio_rxfifo_irq_en(audio_fifo_chn_e rx_fifo_chn)
{
    reg_rxfifo_en |= BIT(rx_fifo_chn + 4); // offset value is 4.
}

/**
 *  @brief      This function serves to enable txfifo irq.
 *  @param[in]  tx_fifo_chn - tx fifo channel.
 *  @return     none.
 */
static inline void audio_txfifo_irq_en(audio_fifo_chn_e tx_fifo_chn)
{
    reg_fifo_outen |= BIT(tx_fifo_chn + 4); // offset value is 4.
}

/**
 * @brief       This function servers to get irq status.
 * @param[in]   irq_status  - the irq status.
 * @return    - the value of status is be set.
 */
static inline unsigned char audio_get_irq_status(audio_fifo_irq_type_e irq_status)
{
    return reg_fifo_irq & irq_status;
}

/**
 * @brief       This function servers to clear irq status.
 * @param[in]   irq_status  - the irq status.
 * @return    - none.
 */
static inline void audio_clr_irq_status(audio_fifo_irq_type_e irq_status)
{
    reg_fifo_irq = irq_status;
}

/**
 *  @brief      This function serves to set tx fifo threshold.
 *  @param[in]  tx_fifo_chn - tx fifo channel.
 *  @param[in]  threshold   - fifo threshold.
 *  @return     none.
 */
static inline void audio_set_tx_fifo_threshold(audio_fifo_chn_e tx_fifo_chn, unsigned short threshold)
{
    reg_txfifo(tx_fifo_chn) = threshold >> 2;
}

/**
 *  @brief      This function serves to set rx fifo threshold.
 *  @param[in]  rx_fifo_chn - rx fifo channel.
 *  @param[in]  threshold   - fifo threshold.
 *  @return     none.
 */
static inline void audio_set_rx_fifo_threshold(audio_fifo_chn_e rx_fifo_chn, unsigned short threshold)
{
    reg_rxfifo(rx_fifo_chn) = threshold >> 2;
}

/**
 *  @brief      This function serves to get dma tx buff pointer.
 *  @param[in]  chn - dma channel.
 *  @return  the result of tx read pointer.
 */
static inline unsigned int audio_get_tx_dma_rptr(dma_chn_e chn)
{
    return reg_dma_src_addr(chn);
}

/**
 *  @brief      This function serves to get dma rx buff pointer.
 *  @param[in]  chn - dma channel.
 *  @return     the result of rx write pointer.
 */
static inline unsigned int audio_get_rx_dma_wptr(dma_chn_e chn)
{
    return reg_dma_dst_addr(chn);
}

/**
 *  @brief      This function serves to set  tx buff length.
 *  @param[in]  len - the length of tx buff.
 *  @param[in]  tx_fifo_chn - tx fifo channel.
 *  @return     none.
 */
static inline void audio_set_tx_buff_len(unsigned short len, audio_fifo_chn_e tx_fifo_chn)
{
    reg_txfifo_max(tx_fifo_chn) = ((len) >> 2) - 1;
}

/**
 *  @brief      This function serves to set rx buff length.
 *  @param[in]  rx_fifo_chn rx fifo channel.
 *  @param[in]  len - the length of rx buff.
 *  @return     none.
 */
static inline void audio_set_rx_buff_len(unsigned short len, audio_fifo_chn_e rx_fifo_chn)
{
    reg_rxfifo_max(rx_fifo_chn) = ((len) >> 2) - 1;
}

/**
 *  @brief      This function serves to tx fifo trigger number.
 *  @param[in]  tx_fifo_chn - tx fifo channel.
 *  @param[in]  number - the number of dma trigger.
 *  @return     none.
 */
static inline void audio_set_fifo_tx_trig_num(audio_fifo_chn_e tx_fifo_chn, unsigned short number)
{
    reg_txfifo_trig_num(tx_fifo_chn) = (reg_txfifo_trig_num(tx_fifo_chn) & (~FLD_TXFIFO_TRIG_NUM)) | MASK_VAL(FLD_TXFIFO_TRIG_NUM, number);
}

/**
 *  @brief      This function serves to rx fifo trigger number.
 *  @param[in]  rx_fifo_chn - rx fifo channel.
 *  @param[in]  number - the number of dma trigger.
 *  @return     none.
 */
static inline void audio_set_fifo_rx_trig_num(audio_fifo_chn_e rx_fifo_chn, unsigned short number)
{
    reg_rxfifo_trig_num(rx_fifo_chn) = (reg_rxfifo_trig_num(rx_fifo_chn) & (~FLD_AUDIO_RXFIFO_TRIG_NUM)) | MASK_VAL(FLD_AUDIO_RXFIFO_TRIG_NUM, number);
}

/**
 *  @brief      This function serves to mic mute enable.
 *  @return     none.
 */
static inline void audio_mic_mute_en(void)
{
    BM_SET(reg_codec_cfg1, FLD_DATO_SOFT_MUTE);
}

/**
 *  @brief      This function serves to mic mute disable.
 *  @return     none.
 */
static inline void audio_mic_mute_dis(void)
{
    BM_CLR(reg_codec_cfg1, FLD_DATO_SOFT_MUTE);
}

/**
 * @brief     This function serves to enable rx_dma channel.
 * @param[in] chn   - dma channel.
 * @return    none.
 */
static inline void audio_rx_dma_en(dma_chn_e chn)
{
    dma_chn_en(chn);
}

/**
 * @brief     This function serves to disable rx_dma channel.
 * @param[in] chn   - dma channel.
 * @return    none.
 */
static inline void audio_rx_dma_dis(dma_chn_e chn)
{
    dma_chn_dis(chn);
}

/**
 * @brief     This function serves to enable tx_dma channel.
 * @param[in] chn   - dma channel.
 * @return    none.
 */
static inline void audio_tx_dma_en(dma_chn_e chn)
{
    dma_chn_en(chn);
}

/**
 * @brief     This function serves to disable dis_dma channel.
 * @param[in] chn   - dma channel.
 * @return    none.
 */
static inline void audio_tx_dma_dis(dma_chn_e chn)
{
    dma_chn_dis(chn);
}

/**
 *  @brief      This function serves to set adc pga gain.
 *  @param[in]  pga_gain - gain value.
 *  @return     none.
 */
static inline void audio_set_adc_pga_gain(codec_in_pga_gain_e pga_gain)
{
    analog_write_reg8(areg_0x8d, ((analog_read_reg8(areg_0x8d) & (~FLD_AUDIO_PGAVOL_IN)) | MASK_VAL(FLD_AUDIO_PGAVOL_IN, pga_gain)));
}

/**
 *  @brief      This function serves to get adc pga gain.
 *  @return     gain value.
 */
static inline codec_in_pga_gain_e audio_get_adc_pga_gain(void)
{
    codec_in_pga_gain_e val;
    val = (analog_read_reg8(areg_0x8d) & FLD_AUDIO_PGAVOL_IN) >> 4;
    return val;
}

/**
 *  @brief      This function serves to stream0 digital gain.
 *  @param[in]  d_gain - gain value(Both left and right channels are in effect).
 *  @return     none.
 */
static inline void audio_set_stream0_dig_gain(codec_in_path_digital_gain_e d_gain)
{
    reg_codec_vol = ((reg_codec_vol & (~FLD_DEC_VOL)) | d_gain);
}

/**
 *  @brief      This function serves to get stream0 digital gain.
 *  @return     gain value.
 */
static inline codec_in_path_digital_gain_e audio_get_stream0_dig_gain(void)
{
    codec_in_path_digital_gain_e val;
    val = reg_codec_vol & FLD_DEC_VOL;
    return val;
}

/**
 * @brief This function serves to set the trigger threshold for codec.
 * @param[in]  target_value - trigger threshold value.
 * @return     none.
 */
static inline void audio_set_codec_target_value(unsigned int target_value)
{
    reg_codec_stimer_target = target_value;
}

/**
 *  @brief      This function serves to set disable codec clk.
 *  @return     none.
 */
static inline void audio_codec_clk_en(void)
{
    BM_SET(reg_dmic_step, FLD_DMIC_SEL);
}

/**
 *  @brief      This function serves to set disable codec clk.
 *  @return     none.
 */
static inline void audio_codec_clk_dis(void)
{
    BM_CLR(reg_dmic_step, FLD_DMIC_SEL);
}

/**
 *  @brief      This function serves to codec enable.
 *  @return     none.
 */
static inline void audio_codec_en(void)
{
    BM_SET(reg_codec_rst, FLD_EN_DEC);
    BM_SET(reg_codec_clkcfg, FLD_CODEC_CLK_EN);
}

/**
 *  @brief      This function serves to codec disable.
 *  @return     none.
 */
static inline void audio_codec_dis(void)
{
    BM_CLR(reg_codec_rst, FLD_EN_DEC);
    BM_CLR(reg_codec_clkcfg, FLD_CODEC_CLK_EN);
}

/**
 *  @brief      This function serves to codec schedule enable.
 *  @return     none.
 */
static inline void audio_codec_schedule_en(void)
{
    BM_SET(reg_codec_mingain, FLD_SCHDL_EN);
}

/**
 *  @brief      This function serves to codec schedule disable.
 *  @return     none.
 */
static inline void audio_codec_schedule_dis(void)
{
    BM_CLR(reg_codec_mingain, FLD_SCHDL_EN);
}

/**
 *  @brief      This function serves to stream0 input enable.
 *  @param[in] chn   - dma channel.
 *  @return     none.
 */
static inline void audio_codec_stream0_input_en(dma_chn_e chn)
{
    audio_codec_en();
    audio_rx_dma_en(chn);
}

/**
 *  @brief      This function serves to stream0 input disable.
 *  @param[in] chn   - dma channel.
 *  @return     none.
 */
static inline void audio_codec_stream0_input_dis(dma_chn_e chn)
{
    audio_codec_dis();
    audio_rx_dma_dis(chn);
}

/**
 *  @brief      This function serves to enable sdm dc constant left.
 *  @return     none.
 */
static inline void audio_sdm_dc_constant_left_en(void)
{
    BM_SET(reg_const_set, FLD_CONST_SEL_L);
}

/**
 *  @brief      This function serves to enable sdm dc constant right.
 *  @return     none.
 */
static inline void audio_sdm_dc_constant_right_en(void)
{
    BM_SET(reg_const_set, FLD_CONST_SEL_R);
}

/**
 *  @brief      This function serves to enable sdm pn sequence left.
 *  @return     none.
 */
static inline void audio_sdm_pn_sequence_left_en(void)
{
    BM_CLR(reg_const_set, FLD_CONST_SEL_L);
    BM_CLR(reg_aud_ctrl, FLD_SHAP_L);
    BM_SET(reg_aud_pn1l, FLD_PN_SEL_L);
}

/**
 *  @brief      This function serves to enable sdm pn sequence right.
 *  @return     none.
 */
static inline void audio_sdm_pn_sequence_right_en(void)
{
    BM_CLR(reg_const_set, FLD_CONST_SEL_R);
    BM_CLR(reg_aud_ctrl, FLD_SHAP_R);
    BM_SET(reg_aud_pn2, FLD_PN_SEL_R);
}

/**
 *  @brief      This function serves to enable sdm pn sequence with shaping left.
 *  @return     none.
 */
static inline void audio_sdm_pn_sequence_with_shapping_left_en(void)
{
    BM_CLR(reg_const_set, FLD_CONST_SEL_L);
    BM_SET(reg_aud_ctrl, FLD_SHAP_L);
}

/**
 *  @brief      This function serves to enable sdm pn sequence with shaping right.
 *  @return     none.
 */
static inline void audio_sdm_pn_sequence_with_shapping_right_en(void)
{
    BM_CLR(reg_const_set, FLD_CONST_SEL_R);
    BM_SET(reg_aud_ctrl, FLD_SHAP_R);
}

/**
 *  @brief      This function serves to disable sdm dither left.
 *  @return     none.
 */
static inline void audio_sdm_dither_control_left_dis(void)
{
    BM_CLR(reg_aud_ctrl, FLD_SHAP_L);
    BM_CLR(reg_aud_pn1l, FLD_PN_SEL_L);
    BM_CLR(reg_const_set, FLD_CONST_SEL_L);
}

/**
 *  @brief      This function serves to disable sdm dither right.
 *  @return     none.
 */
static inline void audio_sdm_dither_control_right_dis(void)
{
    BM_CLR(reg_aud_ctrl, FLD_SHAP_R);
    BM_CLR(reg_aud_pn2, FLD_PN_SEL_R);
    BM_CLR(reg_const_set, FLD_CONST_SEL_R);
}

/**
 *  @brief      This function serves to set sdm dc constant left value.
 *  @return     none.
 */
static inline void audio_sdm_set_dc_constant_value_left(unsigned short value)
{
    reg_const_l = value;
}

/**
 *  @brief      This function serves to set sdm dc constant right value.
 *  @return     none.
 */
static inline void audio_sdm_set_dc_constant_value_right(unsigned short value)
{
    reg_const_r = value;
}

/**
 *  @brief      This function serves to set sdm pn sequence left value.
 *  @return     none.
 */
static inline void audio_sdm_set_pn_sequence_bit_num_left(unsigned short value)
{
    reg_aud_pn1l = (reg_aud_pn1l & (~FDL_PN1_BITS_L)) | MASK_VAL(FDL_PN1_BITS_L, value & 0x1f);
    reg_aud_pn2 = (reg_aud_pn2 & (~FLD_PN2_BITS_L)) | MASK_VAL(FLD_PN2_BITS_L, value & 0x1f);
}

/**
 *  @brief      This function serves to set sdm pn sequence right value.
 *  @return     none.
 */
static inline void audio_sdm_set_pn_sequence_bit_num_right(unsigned short value)
{
    reg_aud_pn1r = (reg_aud_pn1r & (~FLD_PN1_R)) | MASK_VAL(FLD_PN1_R, value & 0x1f);
    reg_const_set = (reg_const_set & (~FLD_PN2_BITS_R)) | MASK_VAL(FLD_PN2_BITS_R, value & 0x1f);
}

/**
 *  @brief      This function serves to sdm mute enable.
 *  @return     none.
 */
static inline void audio_sdm_mute_en(void)
{
    BM_SET(reg_aud_vol, FLD_MUTE_0);
}

/**
 *  @brief      This function serves to sdm mute disable.
 *  @return     none.
 */
static inline void audio_sdm_mute_dis(void)
{
    BM_CLR(reg_aud_vol, FLD_MUTE_0);
}

/**
 *  @brief      This function serves to ascl gain.
 *  @param[in]  ascl_select - ascl channel.
 *  @param[in]  gain        - ascl gain value.
 *  @return     none.
 */
static inline void audio_set_ascl_gain(audio_ascl_select_e ascl_select, ascl_out_path_digital_gain_e gain)
{
    if (ascl_select != ASCL0)
    {
        reg_ascl_vol(ascl_select) = ((reg_ascl_vol(ascl_select) & (~FLD_VOL_12)) | MASK_VAL(FLD_VOL_12, gain));
    }
    else
    {
        reg_aud_vol = ((reg_aud_vol & (~FLD_VOL_0)) | MASK_VAL(FLD_VOL_0, gain));
    }
}

/**
 *  @brief      This function serves to sdm enable.
 *  @return     none.
 */
static inline void audio_sdm_en(void)
{
    BM_SET(reg_aud_en, FLD_SDMPEN);
}

/**
 *  @brief      This function serves to sdm disable.
 *  @return     none.
 */
static inline void audio_sdm_dis(void)
{
    BM_CLR(reg_aud_en, FLD_SDMPEN);
}

/**
 *  @brief      This function serves to enable linear interpolation.
 *  @param[in]  ascl_select - ascl channel.
 *  @return     none.
 */
static inline void audio_ascl_lerp_en(audio_ascl_select_e ascl_select)
{
    if (ascl_select != ASCL0)
    {
        BM_SET(reg_line(ascl_select), FLD_LINE_12);
    }
    else
    {
        BM_SET(reg_aud_ctrl, FLD_LINE_0);
    }
}

/**
 *  @brief      This function serves to disable linear interpolation.
 *  @param[in]  ascl_select - ascl channel.
 *  @return     none.
 */
static inline void audio_ascl_lerp_dis(audio_ascl_select_e ascl_select)
{
    if (ascl_select != ASCL0)
    {
        BM_CLR(reg_line(ascl_select), FLD_LINE_12);
    }
    else
    {
        BM_CLR(reg_aud_ctrl, FLD_LINE_0);
    }
}

/**
 *  @brief      This function serves to stream output enable.
 *  @param[in] chn   - dma channel.
 *  @return     none.
 */
static inline void audio_codec_stream_output_en(dma_chn_e chn)
{
    audio_sdm_en();
    audio_tx_dma_en(chn);
}

/**
 *  @brief      This function serves to stream output disable.
 *  @param[in] chn   - dma channel.
 *  @return     none.
 */
static inline void audio_codec_stream_output_dis(dma_chn_e chn)
{
    audio_sdm_dis();
    audio_tx_dma_dis(chn);
}

/**
 *  @brief      This function serves to enable bclk and lr_clk.
 *  @param[in]  i2s_select i2s channel.
 *  @return     none.
 */
static inline void audio_clk_i2s_en(audio_i2s_select_e i2s_select)
{
    BM_SET(reg_i2s_cfg1(i2s_select), FLD_I2S_CLK_EN);
}

/**
 *  @brief      This function serves to disable bclk and lr_clk
 *  @param[in]  i2s_select - i2s channel.
 *  @return     none.
 */
static inline void audio_clk_i2s_dis(audio_i2s_select_e i2s_select)
{
    BM_CLR(reg_i2s_cfg1(i2s_select), FLD_I2S_CLK_EN);
}

/**
 *  @brief      This function serves to enable source i2s0 clk.
 *  @return     none.
 */
static inline void audio_i2s0_src_clk_en(void)
{
    reg_i2s0_step |= FLD_I2S0_CLK_EN_0;
}

/**
 *  @brief      This function serves to enable source i2s1 clk.
 *  @return     none.
 */
static inline void audio_i2s1_src_clk_en(void)
{
    reg_i2s1_step |= FLD_I2S1_CLK_EN_0;
}

/**
 *  @brief      This function serves to enable source i2s2 clk.
 *  @return     none.
 */
static inline void audio_i2s2_src_clk_en(void)
{
    reg_i2s2_step |= FLD_I2S1_CLK_EN_0;
}

/**
 *  @brief      This function serves to disable all clk related i2s0.
 *  @return     none.
 */
static inline void audio_i2s0_src_clk_dis(void)
{
    reg_i2s0_step &= ~FLD_I2S0_CLK_EN_0;
}

/**
 *  @brief      This function serves to disable all clk related i2s1.
 *  @return     none.
 */
static inline void audio_i2s1_src_clk_dis(void)
{
    reg_i2s1_step &= ~FLD_I2S1_CLK_EN_0;
}

/**
 *  @brief      This function serves to disable all clk related i2s2.
 *  @return     none.
 */
static inline void audio_i2s2_src_clk_dis(void)
{
    reg_i2s2_step &= ~FLD_I2S1_CLK_EN_0;
}

/**
 *  @brief      This function serves to set i2s0 clock of i2s0
 *  @param[in]  div_numerator   - the dividing factor of div_numerator (15bits valid).
 *  @param[in]  div_denominator - the dividing factor of div_denominator(16bits valid).
 *  @return     none.
 */
static inline void audio_set_i2s0_clk(unsigned short div_numerator, unsigned short div_denominator)
{
    reg_i2s0_step = (div_numerator & FLD_I2S0_STEP) | FLD_I2S0_CLK_EN_0;
    reg_i2s0_mod = div_denominator;
}

/**
 *  @brief      This function serves to set i2s1 clock of i2s1
 *  @param[in]  div_numerator   - the dividing factor of div_numerator (15bits valid).
 *  @param[in]  div_denominator - the dividing factor of div_denominator(16bits valid).
 *  @return     none.
 */
static inline void audio_set_i2s1_clk(unsigned short div_numerator, unsigned short div_denominator)
{
    reg_i2s1_step = (div_numerator & FLD_I2S0_STEP) | FLD_I2S1_CLK_EN_0;
    reg_i2s1_mod = div_denominator;
}

/**
 *  @brief      This function serves to set i2s2 clock of i2s2
 *  @param[in]  div_numerator   - the dividing factor of div_numerator (15bits valid).
 *  @param[in]  div_denominator - the dividing factor of div_denominator(16bits valid).
 *  @return     none.
 */
static inline void audio_set_i2s2_clk(unsigned short div_numerator, unsigned short div_denominator)
{
    reg_i2s2_step = (div_numerator & FLD_I2S0_STEP) | FLD_I2S2_CLK_EN_0;
    reg_i2s2_mod = div_denominator;
}

/**
 *  @brief      This function serves to set i2s clock of i2s.
 *  @param[in]  i2s_select i2s channel.
 *  @param[in]  div_numerator   - the dividing factor of div_numerator (15bits valid).
 *  @param[in]  div_denominator - the dividing factor of div_denominator(16bits valid).
 *  @return     none.
 */
static inline void audio_set_i2s_clk(audio_i2s_select_e i2s_select, unsigned short div_numerator, unsigned short div_denominator)
{
    (i2s_select == I2S0) ? (audio_set_i2s0_clk(div_numerator, div_denominator)) : ((i2s_select == I2S1) ? (audio_set_i2s1_clk(div_numerator, div_denominator)) : (audio_set_i2s2_clk(div_numerator, div_denominator)));
}

/**
 *  @brief      This function serves to set the bclk.
 *  @param[in]  i2s_select - i2s channel.
 *  @param[in]  div  - bclk=i2s_clk/(div*2),if div=0,i2s_clk=bclk.
 *  @return     none.
 */
static inline void audio_set_i2s_bclk(audio_i2s_select_e i2s_select, unsigned char div)
{
    reg_i2s_pcm_clk_num(i2s_select) = div;
}

/**
 *  @brief      This function serves to set the i2s lrclk divider.
 *  @param[in]  i2s_select - i2s channel.
 *  @param[in]  adc_div - adc_lrclk=bclk/(adc_div).
 *  @param[in]  dac_div - dac_lrclk=bclk/(dac_div).
 *  @return     none.
 */
static inline void audio_set_i2s_lrclk(audio_i2s_select_e i2s_select, unsigned short adc_div, unsigned short dac_div)
{
    reg_i2s_int_pcm_num(i2s_select) = (adc_div - 1);
    reg_i2s_dec_pcm_num(i2s_select) = (dac_div - 1);
}

/**
 *  @brief      This function serves to set i2s 2lane rx enable.
 *  @param[in]  i2s_select - i2s channel.
 *  @return     none.
 */
static inline void audio_i2s_2lane_rx_en(audio_i2s_select_e i2s_select)
{
    BM_SET(reg_i2s_cfg3(i2s_select), FLD_RX_2LANE_ENABLE);
}

/**
 *  @brief      This function serves to set i2s 2lane tx enable.
 *  @param[in]  i2s_select - i2s channel.
 *  @return     none.
 */
static inline void audio_i2s_2lane_tx_en(audio_i2s_select_e i2s_select)
{
    BM_SET(reg_i2s_cfg3(i2s_select), FLD_TX_2LANE_ENABLE);
}

/**
 *  @brief      This function serves to set i2s 2lane rx disable.
 *  @param[in]  i2s_select - i2s channel.
 *  @return     none.
 */
static inline void audio_i2s_2lane_rx_dis(audio_i2s_select_e i2s_select)
{
    BM_CLR(reg_i2s_cfg3(i2s_select), FLD_RX_2LANE_ENABLE);
}

/**
 *  @brief      This function serves to set i2s 2lane tx disable.
 *  @param[in]  i2s_select - i2s channel.
 *  @return     none.
 */
static inline void audio_i2s_2lane_tx_dis(audio_i2s_select_e i2s_select)
{
    BM_CLR(reg_i2s_route(i2s_select), FLD_TX_2LANE_ENABLE);
}

/**
 *  @brief      This function serves to set i2s ascl disable.
 *  @param[in]  i2s_select - i2s channel.
 *  @return     none.
 */
static inline void audio_set_i2s_ascl_dis(audio_i2s_select_e i2s_select)
{
    BM_SET(reg_i2s_route(i2s_select), FLD_ASCL_BYPASS);
}

/**
 * @brief This function serves to enable the i2s 2fifo mode.
 * @param[in]  i2s_select - i2s channel.
 * @return    none.
 */
static inline void audio_i2s_2fifo_en(audio_i2s_select_e i2s_select)
{
    BM_SET(reg_i2s_2fifo_mode, BIT(i2s_select));
}

/**
 * @brief This function serves to disable the i2s 2fifo mode.
 * @param[in]  i2s_select - i2s channel.
 * @return    none.
 */
static inline void audio_i2s_2fifo_dis(audio_i2s_select_e i2s_select)
{
    BM_CLR(reg_i2s_2fifo_mode, BIT(i2s_select));
}

/**
 * @brief This function serves to enable the i2s schedule.
 * @param[in]  i2s_select - i2s channel.
 * @return    none.
 */
static inline void audio_i2s_schedule_en(audio_i2s_select_e i2s_select)
{
    BM_SET(reg_i2s_route(i2s_select), FLD_I2S_SCHEDULE_EN);
}

/**
 * @brief This function serves to disable the i2s schedule.
 * @param[in]  i2s_select - i2s channel.
 * @return    none.
 */
static inline void audio_i2s_schedule_dis(audio_i2s_select_e i2s_select)
{
    BM_CLR(reg_i2s_route(i2s_select), FLD_I2S_SCHEDULE_EN);
}

/**
 * @brief This function serves to enable the i2s align function.
 * @return    none.
 */
static inline void audio_i2s_align_en(void)
{
    reg_i2s_align_cfg |= FLD_I2S_ALIGN_EN | FLD_I2S_CLK_SEL;
}

/**
 * @brief This function serves to disable the i2s align function.
 * @return    none.
 */
static inline void audio_i2s_align_dis(void)
{
    reg_i2s_align_cfg &= ~(FLD_I2S_ALIGN_EN | FLD_I2S_CLK_SEL);
}

/**
 *  @brief      This function serves to disable i2s is align mask.
 *  @return     none.
 */
static inline void audio_i2s_align_mask_dis(void)
{
    BM_SET(reg_i2s_align_cfg, FLD_I2S_ALIGN_MASK);
}
/**
 *  @brief      This function serves to enable i2s is align mask.
 *  @return     none.
 */
static inline void audio_i2s_align_mask_en(void)
{
    BM_CLR(reg_i2s_align_cfg, FLD_I2S_ALIGN_MASK);
}

/**
 * @brief This function serves to setting the i2s line mode.
 * @param[in]  i2s_select - i2s channel.
 * @param[in]  line_mode - i2s line_mode.
 * @return    none.
 */
static inline void audio_set_i2s_line_mode(audio_i2s_select_e i2s_select, i2s_line_mode_e line_mode)
{
    reg_i2s_route(i2s_select) = (reg_i2s_route(i2s_select) & (~FLD_I2S_MODE)) | MASK_VAL(FLD_I2S_MODE, line_mode);
}

/**
 * @brief This function serves to set the align trigger threshold for i2s.
 * @param[in]  tick - trigger threshold tick.
 * @return     none.
 */
static inline void audio_set_i2s_align_th_tick(unsigned int tick)
{
    reg_i2s_timer_th = tick;
}

/**
 * @brief This function serves to set the trigger threshold for i2s.
 * @param[in]  i2s_select - i2s channel.
 * @param[in]  target_value - trigger threshold value.
 * @return     none.
 */
static inline void audio_set_i2s_target_value(audio_i2s_select_e i2s_select, unsigned int target_value)
{
    reg_i2s_stimer_target(i2s_select) = target_value;
}

/**
 * @brief     This function configures amic bias pin.
 * @param[in] amic_bias - the amic bias pin.
 * @return    none.
 */
void audio_set_amic_bias_pin(gpio_pin_e amic_bias);

/**
 * @brief     This function configures stream0 dmic pin.
 * @param[in] dmic0_data - the data of  dmic pin
 * @param[in] dmic0_clk1 - the clk1 of dmic pin
 * @param[in] dmic0_clk2 - the clk2 of dmic pin,if need not set clk2,please set GPIO_NONE_PIN
 * @return    none
 */
void audio_set_stream0_dmic_pin(gpio_func_pin_e dmic0_data, gpio_func_pin_e dmic0_clk1, gpio_func_pin_e dmic0_clk2);

/**
 * @brief     This function configures sdm pin.
 * @param[in] config -sdm config pin struct.
 * @return    none.
 */
void audio_set_sdm_pin(sdm_pin_config_t *config);

/**
 * @brief     This function configures i2s pin.
 * @param[in] i2s_select       - channel select
 * @param[in] config           - i2s config pin struct.
 * @return    none.
 */
void i2s_set_pin(audio_i2s_select_e i2s_select, i2s_pin_config_t *config);

/**
 * @brief     This function serves to config  rx_dma channel.
 * @param[in] chn          - dma channel.
 * @param[in] dst_addr     - the dma address of destination.
 * @param[in] data_len     - the length of dma rx size by byte.
 * @param[in] head_of_list - the head address of dma llp.
 * @return    none.
 */
void audio_rx_dma_config(dma_chn_e chn, unsigned short *dst_addr, unsigned int data_len, dma_chain_config_t *head_of_list);

/**
 * @brief     This function serves to set rx dma chain transfer
 * @param[in] config_addr - the head of list of llp_pointer.
 * @param[in] llpointer   - the next element of llp_pointer.
 * @param[in] dst_addr    - the dma address of destination.
 * @param[in] data_len    - the length of dma size by byte.
 * @return    none.
 */
void audio_rx_dma_add_list_element(dma_chain_config_t *rx_config, dma_chain_config_t *llpointer, unsigned short *dst_addr, unsigned int data_len);

/**
 * @brief  This function serves to set audio rx dma chain transfer.
 * @param[in] chn         - dma channel.
 * @param[in] in_buff     - Pointer to data buffer, it must be 4-bytes aligned address and the actual buffer size defined by the user needs to
 *                          be not smaller than the data_len, otherwise there may be an out-of-bounds problem.
 * @param[in] buff_size   - Length of DMA in bytes, it must be set to a multiple of 4. The maximum value that can be set is 0x10000.
 * @param[in] rx_fifo_chn - rx fifo select.
 * @return    none.
 */
void audio_rx_dma_chain_init(dma_chn_e chn, unsigned short *in_buff, unsigned int buff_size, audio_fifo_chn_e rx_fifo_chn);

/**
 * @brief     This function serves to config  tx_dma channel.
 * @param[in] chn          - dma channel.
 * @param[in] src_addr     - the address of source.
 * @param[in] data_len     - the length of dma rx size by byte.
 * @param[in] head_of_list - the head address of dma llp.
 * @return    none
 */
void audio_tx_dma_config(dma_chn_e chn, unsigned short *src_addr, unsigned int data_len, dma_chain_config_t *head_of_list);

/**
 * @brief     This function serves to set tx dma chain transfer.
 * @param[in] config_addr - the head of list of llp_pointer.
 * @param[in] llpointer   - the next element of llp_pointer.
 * @param[in] src_addr    - the address of source.
 * @param[in] data_len    - the length of dma size by byte.
 * @return    none.
 */
void audio_tx_dma_add_list_element(dma_chain_config_t *config_addr, dma_chain_config_t *llpointer, unsigned short *src_addr, unsigned int data_len);

/**
 * @brief  This function serves to initialize audio tx dma chain transfer.
 * @param[in] chn         - dma channel.
 * @param[in] out_buff    - Pointer to data buffer, it must be 4-bytes aligned address.
 * @param[in] buff_size   - Length of DMA in bytes, range from 1 to 0x10000.
 * @param[in] tx_fifo_chn - tx fifo select.
 * @return    none.
 */
void audio_tx_dma_chain_init(dma_chn_e chn, unsigned short *out_buff, unsigned int buff_size, audio_fifo_chn_e tx_fifo_chn);

/**
 * @brief       This function serves to set audio tx dma burst size and trigger number.
 * @param[in]   fifo_chn     - the spi module.
 * @param[in]   dma_chn      - dma channel.
 * @param[in]   burst_size   - dma burst size.
 * @return      none.
 */
void audio_set_dma_tx_burst(audio_fifo_chn_e fifo_chn, dma_chn_e dma_chn, dma_burst_size_e burst_size);

/**
 * @brief       This function serves to set audio rx dma burst size and trigger number.
 * @param[in]   fifo_chn     - the spi module.
 * @param[in]   dma_chn      - dma channel.
 * @param[in]   burst_size   - dma burst size.
 * @return      none.
 */
void audio_set_dma_rx_burst(audio_fifo_chn_e fifo_chn, dma_chn_e dma_chn, dma_burst_size_e burst_size);

/**
 * @brief     This function serves to power on audio.
 * @return    none.
 */
void audio_power_on(void);

/**
 * @brief      This function serves to power down all audio power.
 * @return     none.
 */
void audio_power_down(void);

/**
 *  @brief      This function serves to set the clock of audio
 *  @param[in]  div_numerator   - the dividing factor of div_numerator (15bits valid).
 *  @param[in]  div_denominator - the dividing factor of div_denominator(16bits valid).
 *  @return     none
 *  @attention  The default is from pll 240M(default). If the pll is changed, the clk will be changed accordingly.
 */
void audio_set_audio_clk(unsigned short div_numerator, unsigned short div_denominator);

/**
 * @brief     This function serves to initial audio.
 * @return    none.
 */
void audio_init(void);

/**
 * @brief     This function serves to initial audio codec.
 * @return    none
 */
void audio_codec_init(void);

/**
 * @brief     This function serves to set codec_adc clock.
 * @return    none.
 */
void audio_codec_set_adc_clock(void);

/**
 * @brief      This function serves to power on codec_adc.
 * @return     none.
 */
void audio_codec_adc_power_on(void);

/**
 * @brief     This function serves to power down codec_adc.
 * @return    none
 */
void audio_codec_adc_power_down(void);

/**
 * @brief      This function serves to set adc vmid.
 * @param[in]  en               - POWER_DOWN or POWER_ON.
 * @return     none.
 */
void audio_codec_set_adc_vmid(power_switch_e en);

/**
 *  @brief      This function serves to set stream input mode.
 *  @param[in]  input_mode - 0:line_in or amic 1:dmic.
 *  @return     none.
 */
void audio_set_codec_stream0_input_mode(audio_codec_input_mode_e input_mode);

/**
 *  @brief      This function serves to set stream input path.
 *  @param[in]  che_en  - channel selection.
 *  @return     none.
 */
void audio_set_codec_stream0_path(audio_chn_sel_e che_en);

/**
 *  @brief      This function serves to set stream sample_rate.
 *  @param[in]  source - audio input source select.
 *  @param[in]  rate   - sample_rate.
 *  @return     none.
 */
void audio_set_codec_stream0_sample_rate(codec_stream0_input_src_e source, audio_sample_rate_e rate);

/**
 *  @brief      This function serves to set codec configuration.
 *  @param[in]  source - audio input source select.
 *  @param[in]  rate   - sample_rate.
 *  @return     none.
 */
void audio_codec_stream0_input_config(codec_stream0_input_src_e source, audio_sample_rate_e rate);

/**
 *  @brief      This function serves to set codec mono mode channel.
 *  @param[in]  fifo_chn - fifo channel.
 *  @param[in]  chn      - mono mode channel.
 *  @return     none.
 */
void audio_set_codec_fifo_mono_channel(audio_fifo_chn_e fifo_chn, audio_chn_sel_e chn);

/**
 *  @brief      This function serves to set stream codec input fifo data format.
 *  @param[in]  fifo_chn   - fifo channel.
 *  @param[in]  ain_mode   - number of channels.
 *  @param[in]  data_width - data bit width.
 *  @return     none.
 */
void audio_set_stream0_fifo_input_mode(audio_fifo_chn_e fifo_chn, audio_channel_select_e ain_mode, audio_codec_wl_mode_e data_width);

/**
 * @brief      This function serves to set codec stream0 input data bit width for fifo0/fifo1/fifo2 .
 * @param[in]  fifo_chn   - fifo0/fifo1/fifo2
 * @param[in]  source     - audio input source select.
 * @param[in]  data_width - input data bit width selection
 * @return     none.
 */
void audio_codec_set_stream0_fifo_input_mode(audio_fifo_chn_e fifo_chn, codec_stream0_input_src_e source, audio_codec_wl_mode_e data_width);

/**
 * @brief     This function serves to set fifo input data path.
 * @param[in] fifo_chn - fifo select
 * @param[in] ain_sel  - fifo input source select
 * @return    none
 */
void audio_data_fifo_input_path_sel(audio_fifo_chn_e fifo_chn, audio_mux_ain_e ain_sel);

/**
 *  @brief      This function serves to set sdm sample rate.
 *  @param[in]  output_src - sdm channel.
 *  @param[in]  rate       - sdm sample rate.
 *  @return     none.
 */
void audio_set_stream_output_sample_rate(audio_stream_output_src_e output_src, audio_sample_rate_e rate);

/**
 *  @brief      This function serves to set sdm path.
 *  @param[in]  chn  - channel selection.
 *  @return     none.
 */
void audio_set_sdm_path(audio_stream_output_src_e chn);

/**
 * @brief This function serves to enable audio codec fifo input.
 * @param[in]  fifo_chn        - select channel fifo
 * @return     none.
 */
void audio_codec_input_path_en(audio_fifo_chn_e fifo_chn);

/**
 * @brief This function serves to configure audio stream0 fifo input.
 * @param[in]  fifo_chn        - select channel fifo
 * @param[in]  source          - codec input source
 * @param[in]  data_width      - fifo data bit width.
 * @return     none.
 */
void audio_codec_stream0_fifo_input_config(audio_fifo_chn_e fifo_chn, codec_stream0_input_src_e source, audio_codec_wl_mode_e data_width);

/**
 *  @brief      This function serves to set codec initialization.
 *  @param[in]  audio_codec - audio_codec_stream0_input_t pointer.
 *  @return     none.
 */
void audio_codec_stream0_input_init(audio_codec_stream0_input_t *audio_codec);

/**
 * @brief      This function serves to configure audio stream output.
 * @param[in]  chn        - audio output source select.
 * @param[in]  rate       - select channel sample rate.
 * @return     none.
 */
void audio_stream_output_config(audio_stream_output_src_e chn, audio_sample_rate_e rate);

/**
 * @brief This function serves to configure audio stream fifo output.
 * @param[in]  fifo_chn         - select channel fifo
 * @param[in]  chn              - audio output source select.
 * @param[in]  data_width       - fifo data bit width.
 * @return    none
 */
void audio_codec_stream_fifo_output_config(audio_fifo_chn_e fifo_chn, audio_stream_output_src_e chn, audio_codec_wl_mode_e data_width);

/**
 * @brief      This function serves to set sdm initialization.
 * @param[in]  audio_stream_output   - audio_codec_output_t pointer.
 * @return     none.
 */
void audio_codec_stream_output_init(audio_codec_output_t *audio_stream_output);

/**
 *  @brief      This function serves to set ascl step.
 *  @param[in]  ascl_select - ascl channel.
 *  @param[in]  value       - ascl step value.
 *  @return     none.
 */
void audio_set_ascl_tune_step(audio_ascl_select_e ascl_select, unsigned int value);

/**
 *  @brief      This function serves to set ascl channel.
 *  @param[in]  ascl_select - ascl channel.
 *  @param[in]  chn         - audio channel select.
 *  @return     none.
 */
void audio_set_ascl_channel(audio_ascl_select_e ascl_select, audio_channel_select_e chn);

/**
 *  @brief      This function serves to set ascl conversion sample rate change.
 *  @param[in]  ascl_select          - ascl channel.
 *  @param[in]  sample_rate_in       - input sample rate.
 *  @param[in]  sample_rate_out      - output sample rate.
 *  @param[in]  chn                  - audio channel select.
 *  @return     none.
 */
void audio_ascl_set_conversion_sample_rate(audio_ascl_select_e ascl_select, audio_ascl_sample_rate_e sample_rate_in, audio_ascl_sample_rate_e sample_rate_out, audio_channel_select_e chn);

/**
 * @brief      This function serves to set sdm  left channel dither control.
 * @param[in]  dither_control_src   - dither control source.
 * @param[in]  config_value         - When the input source is DC_CONSTANT, config_value is the constant value of the input.
 *                                    When it is PN_SEQUENCE or PN_SEQUENCE_SHAPPING, config_value is the number of bits
 *                                    (in the range of 0-16) at the PN1/PN2 generator.
 * @return     none.
 */
void audio_sdm_dither_control_left(audio_stream_output_dither_control_e dither_control_src, unsigned short config_value);

/**
 * @brief      This function serves to set sdm  right channel dither control.
 * @param[in]  dither_control_src   - dither control source.
 * @param[in]  config_value         - When the input source is DC_CONSTANT, config_value is the constant value of the input.
 *                                    When it is PN_SEQUENCE or PN_SEQUENCE_SHAPPING, config_value is the number of bits
 *                                    (in the range of 0-16) at the PN1/PN2 generator.
 * @return     none.
 */
void audio_sdm_dither_control_right(audio_stream_output_dither_control_e dither_control_src, unsigned short config_value);

/**
 * @brief      This function serves to set ascl data format.
 * @param[in]  ascl_select   - ascl channel.
 * @param[in]  ascl_mode     - ascl data format.
 * @return     none.
 */
void audio_set_ascl_format(audio_ascl_select_e ascl_select, ascl_mode_select_e ascl_mode);

/**
 * @brief     This function configures i2s mclk pin for extern codec,mclk=240M*(div_numerator/div_denominator):240M*(1/20)=12M.
 * @param[in] mclk_pin -mclk output pin.
 * @return    none.
 * @attention If need to use internal codec at the same time, mclk must be set to 12M.
 */
void audio_i2s_set_mclk(gpio_func_pin_e mclk_pin);

/**
 * @brief     This function configures i2s mclk pin for extern iis clk,mclk=240M*(div_numerator/div_denominator):240M*(1/20)=12M.
 * @param[in] i2s_select i2s channel.
 * @param[in] mclk_pin -mclk output pin.
 * @param[in] div_numerator   - the dividing factor of div_numerator (15bits valid).
 * @param[in] div_denominator - the dividing factor of div_denominator(16bits valid).
 * @return    none.
 */
void audio_set_i2s_clk_as_mclk(audio_i2s_select_e i2s_select, gpio_func_pin_e mclk_pin, unsigned short div_numerator, unsigned short div_denominator);

/**
 * @brief     This function configures debug clk as iis mclk,mclk=240M*(div_numerator/div_denominator):240M*(1/20)=12M.
 * @param[in] clk debug clk.
 * @return    none.
 */
void audio_aclk_debug_set_mclk(aclk_dbg_e clk);

/**
 * @brief     This function serves to set sampling rate when i2s as master.
 * @param[in] i2s_select                    - i2s channel select.
 * @param[in] i2s_clk_config                 i2s_clk_config[2]   i2s_clk_config[3]-->lrclk_adc(sampling rate)
                                                  ||                  ||
 *  pll(192M default)------->div---->i2s_clk--->2*div(div=0,bypass)--->blck----->div
 *                           ||                                       ||
 *           i2s_clk_config[0]/i2s_clk_config[1]                 i2s_clk_config[4]-->lrclk_dac (sampling rate)
 *
 For example:sampling rate=16K,i2s_clk_config[5]={ 4,1875,0,32,32}, sampling rate=240M*(4/1875)/32=16K

 * @return    none
 * @attention The default is from pll 240M(default). If the pll is changed, the clk will be changed accordingly.
 */
_attribute_ram_code_sec_noinline_ void audio_set_i2s_clock(audio_i2s_select_e i2s_select, unsigned short *i2s_clk_config);

/**
 * @brief     This function set i2s input data bit with .
 * @param[in] i2s_select       - channel select.
 * @param[in] data_mode        - i2s data bit with.
 * @return    none.
 */
void audio_set_i2s_data_wl(audio_i2s_select_e i2s_select, audio_i2s_wl_mode_e data_mode);

/**
 * @brief     This function set i2s format mode.
 * @param[in] i2s_select       - channel select.
 * @param[in] i2s_mode         - i2s format mode.
 * @return    none.
 */
void audio_set_i2s_format(audio_i2s_select_e i2s_select, i2s_mode_select_e i2s_mode);

/**
 *  @brief      This function serves to set i2s clk enable.
 *  @param[in]  i2s_select -i2s channel.
 *  @return     none.
 */
void audio_i2s_clk_en(audio_i2s_select_e i2s_sel);

/**
 * @brief     This function serves to config i2s interface, word length, and m/s.
 * @param[in] i2s_select - i2s channel.
 * @param[in] i2s_format - interface protocol.
 * @param[in] wl         - audio data word length.
 * @param[in] m_s        - select i2s as master or slave.
 * @param[in] i2s_config_t - the ptr of i2s_config_t that configure i2s lr_clk phase and lr_clk swap.
 *  i2s_config_t->i2s_lr_clk_invert_select-lr_clk phase control(in RJ,LJ or i2s modes),in i2s mode(opposite phasing in  RJ,LJ mode), 0=right channel data when lr_clk high ,1=right channel data when lr_clk low.
 *                                                                                     in DSP mode(in DSP mode only), DSP mode A/B select,0=DSP mode A ,1=DSP mode B.
 *            i2s_config_t->i2s_data_invert_select - 0=left channel data left,1=right channel data left.
 * but data output channel will be inverted,you can also set i2s_config_t->i2s_data_invert_select=1 to recovery it.
 * @return    none.
 */
void audio_i2s_config(audio_i2s_select_e i2s_select, i2s_mode_select_e i2s_format, audio_i2s_wl_mode_e wl, i2s_m_s_mode_e m_s, audio_i2s_invert_config_t *i2s_config_t);

/**
 * @brief This function serves to initialize  configuration i2s.
 * @param[in]  i2s_config - the relevant configuration struct pointer
 * @return    none
 */
void audio_i2s_config_init(audio_i2s_config_t *i2s_config);

/**
 *  @brief      This function serves to set i2s mono mode channel.
 *  @param[in]  i2s_select - i2s channel.
 *  @param[in]  fifo_chn   - fifo channel.
 *  @param[in]  chn        - mono mode channel.
 *  @return     none.
 */
void audio_set_i2s_fifo_mono_channel(audio_i2s_select_e i2s_select, audio_fifo_chn_e fifo_chn, audio_chn_sel_e chn);

/**
 *  @brief      This function serves to set i2s input fifo data format.
 *  @param[in]  i2s_select - i2s channel.
 *  @param[in]  fifo_chn   - fifo channel.
 *  @param[in]  ain_mode   - number of channels.
 *  @param[in]  data_width - data bit width.
 *  @return     none.
 */
void audio_set_i2s_fifo_input_mode(audio_i2s_select_e i2s_select, audio_fifo_chn_e fifo_chn, audio_channel_select_e ain_mode, audio_i2s_fifo_wl_e data_width);

/**
 * @brief     This function set i2s input .
 * @param[in] fifo_chn         - fifo channel selection
 * @param[in] i2s_select       - channel select
 * @param[in] data_mode        - i2s data bit with
 * @param[in] ch               - i2s channel selection
 * @return    none
 */
void audio_set_i2s_input_chn_wl(audio_fifo_chn_e fifo_chn, audio_i2s_select_e i2s_select, audio_i2s_wl_mode_e data_mode, i2s_chn_select_e ch);

/**
 * @brief This function serves to initialize  input i2s.
 * @param[in] audio_i2s_input - the relevant input struct pointer
 * @return    none
 */
void audio_i2s_input_init(audio_i2s_input_output_t *audio_i2s_input);

/**
 *  @brief      This function serves to set  output fifo data format.
 *  @param[in]  fifo_chn   - fifo channel.
 *  @param[in]  ain_mode   - number of channels.
 *  @param[in]  data_width - data bit width.
 *  @return     none.
 *  @attention  sdm and i2s0 share tx fifo0.
 */
void audio_set_i2s_fifo_output_mode(audio_fifo_chn_e fifo_chn, audio_channel_select_e ain_mode, audio_i2s_fifo_wl_e data_width);

/**
 * @brief     This function set i2s input .
 * @param[in] fifo_chn         - fifo channel selection
 * @param[in] data_mode        - i2s data bit with
 * @param[in] ch               - i2s channel selection
 * @return    none
 */
void audio_set_i2s_output_chn_wl(audio_fifo_chn_e fifo_chn, audio_i2s_wl_mode_e data_mode, i2s_chn_select_e ch);

/**
 * @brief  This function serves to initialize  output i2s.
 * @param[in] audio_i2s_output - audio_i2s_input_output_t pointer.
 * @return    none.
 */
void audio_i2s_output_init(audio_i2s_input_output_t *audio_i2s_output);

/**
 * @brief  This function serves to configure tdm mode and slot.
 * @param[in] tdm_config - audio_i2s2_tdm_config_t pointer.
 * @return    none.
 */
void audio_i2s2_tdm_config_init(audio_i2s2_tdm_config_t *tdm_config);

/**
 *  @brief      This function serves to set  input fifo data format.
 *  @param[in]  data_width - tdm mode fifo data format.
 *  @return     none.
 */
void audio_set_i2s2_tdm_input_fifo_width(audio_i2s_wl_mode_e data_width);

/**
 * @brief  This function serves to config tdm input interface tdm channel.
 * @param[in] ch   - tdm channel.
 * @return    none.
 */
void audio_i2s2_tdm_input_config(tdm_slot_number_select_e ch);

/**
 * @brief  This function serves to tdm input initialization.
 * @param[in] input - i2s2 tdm mode input path struct pointer.
 * @return    none.
 */
void audio_i2s2_tdm_input_init(audio_i2s2_tdm_input_output_t *input);

/**
 *  @brief      This function serves to set  output fifo data format.
 *  @param[in]  data_width - tdm mode fifo data format.
 *  @return     none.
 */
void audio_set_i2s2_tdm_output_fifo_width(audio_i2s_wl_mode_e data_width);

/**
 * @brief  This function serves to config tdm output interface tdm channel.
 * @param[in] ch   - tdm channel.
 * @return    none.
 */
void audio_i2s2_tdm_output_config(tdm_slot_number_select_e ch);

/**
 * @brief  This function serves to tdm output initialization.
 * @param[in] output - i2s2 tdm mode output path struct pointer.
 * @return    none.
 */
void audio_i2s2_tdm_output_init(audio_i2s2_tdm_input_output_t *output);

/**
 *  @brief      This function serves to set i2s ascl enable.
 *  @param[in]  i2s_select - i2s channel.
 *  @return     none.
 */
void audio_set_i2s_ascl_en(audio_i2s_select_e i2s_select);

/**
 * @brief     his function serves to sets the align data bit width.
 * @param[in]  i2s_align_mode_select - i2s align mode.
 * @param[in]  audio_channel_select  - number of channels.
 * @param[in]  audio_i2s_wl_mode     - i2s fifo bit width.
 * @return    none.
 */
void audio_set_i2s_align_data_width(i2s_align_mode_select_e i2s_align_mode_select, audio_channel_select_e audio_channel_select, audio_i2s_wl_mode_e audio_i2s_wl_mode);

/**
 *  @brief      This function serves to i2s align mode.
 *  @param[in]  mode       - i2s align mode.
 *  @return     none.
 */
void audio_set_i2s_align_mode(i2s_align_mode_select_e mode);

/**
 *  @brief      This function serves to clear codec's input pop.
 *  @param[in]  t_ms        - cut input codec's data.
 *  @return     none.
 */
void audio_codec_clr_input_pop(unsigned char t_ms);
/** @} */
#endif
