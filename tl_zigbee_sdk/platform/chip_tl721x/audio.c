/********************************************************************************************************
 * @file    audio.c
 *
 * @brief   This is the source file for TL721X
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
#include "audio.h"
#include "lib/include/clock.h"
#include "pwm.h"
#include "lib/include/stimer.h"
#include "lib/include/pm/pm.h"

static unsigned char audio_codec_rate[AUDIO_RATE_SIZE + 0x03] = {
    0x06,
    /*8k*/              // 12Mhz/1500=8K
    0x19,
    /*11.0259k*/        // 12Mhz/1088=11.0259K
    0x08,
    /*12k*/             // 12Mhz/1000=12K
    0x0a,
    /*16k*/             // 12Mhz/750=16K
    0x1b,
    /*22.0588k*/        // 12Mhz/544=22.0588K
    0x1c,
    /*24k*/             // 12Mhz/500=24K
    0x0c,
    /*32k*/             // 12Mhz/375=32K
    0x11,
    /*44.118k_clk6mhz*/ // 12Mhz/272=44.118K
    0x00,               // 48k_clk6mhz
    0x0d,               // 32k_clk2mhz
    0x1f,               // 44.118k_clk3mhz
    0x1e};              // 48k_clk3mhz

static unsigned int audio_stream_output_stereo_step[AUDIO_RATE_SIZE] = {
    0x00831001,         /*8k*/
    0x00b4a001,         /*11.0259k*/
    0x00c4a001,         /*12k*/
    0x01062001,         /*16k*/
    0x01696001,         /*22.0588k*/
    0x01893001,         /*24k*/
    0x020c5001,         /*32k*/
    0x02d28001,         /*44.1k*/
    0x03127001,         /*48k*/
};

static unsigned int audio_stream_output_mono_step[AUDIO_RATE_SIZE] = {
    0x01062001, /*8k*/
    0x01694001, /*11.0259k*/
    0x01894001, /*12k*/
    0x020c5001, /*16k*/
    0x02d2c001, /*22.0588k*/
    0x03126001, /*24k*/
    0x0418a801, /*32k*/
    0x05a50001, /*44.1k*/
    0x0624e801, /*48k*/
};

unsigned int audio_sample_rate_value[AUDIO_ASCL_RATE_SIZE] = {
    8000,   /*8k*/
    11025,  /*11.0259k*/
    12000,  /*12k*/
    16000,  /*16k*/
    22058,  /*22.0588k*/
    24000,  /*24k*/
    32000,  /*32k*/
    44100,  /*44.1k*/
    48000,  /*48k*/
    96000,  /*96k*/
    192000, /*192k*/
};

unsigned char      audio_rx_dma_chn;
unsigned char      audio_tx_dma_chn;
unsigned char      audio_rx_fifo_chn;
unsigned char      audio_tx_fifo_chn;
dma_chain_config_t g_audio_tx_dma_list_cfg[0x03];
dma_chain_config_t g_audio_rx_dma_list_cfg[0x03];

audio_i2s_invert_config_t audio_i2s_invert_config[0x03] =
    {
        {
         .i2s_lr_clk_invert_select = I2S_LR_CLK_INVERT_DIS,
         .i2s_data_invert_select   = I2S_DATA_INVERT_DIS,
         },
        {
         .i2s_lr_clk_invert_select = I2S_LR_CLK_INVERT_DIS,
         .i2s_data_invert_select   = I2S_DATA_INVERT_DIS,
         },
        {
         .i2s_lr_clk_invert_select = I2S_LR_CLK_INVERT_DIS,
         .i2s_data_invert_select   = I2S_DATA_INVERT_DIS,
         },
};

dma_config_t audio_dma_rx_config[3] =
    {
        {
         .dst_req_sel    = 0,
         .src_req_sel    = DMA_REQ_AUDIO0_RX, // l_rx req.
            .dst_addr_ctrl  = DMA_ADDR_INCREMENT,
         .src_addr_ctrl  = DMA_ADDR_FIX,
         .dstmode        = DMA_NORMAL_MODE,
         .srcmode        = DMA_HANDSHAKE_MODE,
         .dstwidth       = DMA_CTR_WORD_WIDTH, // must word.
            .srcwidth       = DMA_CTR_WORD_WIDTH, // must word.
            .src_burst_size = 0, // must 0.
            .read_num_en    = 0,
         .priority       = 0,
         .write_num_en   = 0,
         .auto_en        = 0, // must 0.
        },
        {
         .dst_req_sel    = 0,
         .src_req_sel    = DMA_REQ_AUDIO1_RX,                 // r_rx req.
            .dst_addr_ctrl  = DMA_ADDR_INCREMENT,
         .src_addr_ctrl  = DMA_ADDR_FIX,
         .dstmode        = DMA_NORMAL_MODE,
         .srcmode        = DMA_HANDSHAKE_MODE,
         .dstwidth       = DMA_CTR_WORD_WIDTH, // must word.
            .srcwidth       = DMA_CTR_WORD_WIDTH,                                                  // must word.
            .src_burst_size = 0,                                 // must 0.
            .read_num_en    = 0,
         .priority       = 0,
         .write_num_en   = 0,
         .auto_en        = 0, // must 0
        },
        {
         .dst_req_sel    = 0,
         .src_req_sel    = DMA_REQ_AUDIO2_RX,                                    // r_rx req.
            .dst_addr_ctrl  = DMA_ADDR_INCREMENT,
         .src_addr_ctrl  = DMA_ADDR_FIX,
         .dstmode        = DMA_NORMAL_MODE,
         .srcmode        = DMA_HANDSHAKE_MODE,
         .dstwidth       = DMA_CTR_WORD_WIDTH,    // must word.
            .srcwidth       = DMA_CTR_WORD_WIDTH,                                                  // must word.
            .src_burst_size = 0,                                                                                  // must 0.
            .read_num_en    = 0,
         .priority       = 0,
         .write_num_en   = 0,
         .auto_en        = 0, // must 0.
        }
};

dma_config_t audio_dma_tx_config[3] =
    {
        {
         .dst_req_sel    = DMA_REQ_AUDIO0_TX, // tx req.
            .src_req_sel    = 0,
         .dst_addr_ctrl  = DMA_ADDR_FIX,
         .src_addr_ctrl  = DMA_ADDR_INCREMENT,                                // increment.
            .dstmode        = DMA_HANDSHAKE_MODE, // handshake.
            .srcmode        = DMA_NORMAL_MODE,
         .dstwidth       = DMA_CTR_WORD_WIDTH,                                                                                                // must word.
            .srcwidth       = DMA_CTR_WORD_WIDTH,                                               // must word.
            .src_burst_size = 0, // must 0.
            .read_num_en    = 0,
         .priority       = 0,
         .write_num_en   = 0,
         .auto_en        = 0, // must 0.
        },
        {
         .dst_req_sel    = DMA_REQ_AUDIO1_TX,                                    // tx req.
            .src_req_sel    = 0,
         .dst_addr_ctrl  = DMA_ADDR_FIX,
         .src_addr_ctrl  = DMA_ADDR_INCREMENT,                                      // increment.
            .dstmode        = DMA_HANDSHAKE_MODE,                                                  // handshake.
            .srcmode        = DMA_NORMAL_MODE,
         .dstwidth       = DMA_CTR_WORD_WIDTH, // must word.
            .srcwidth       = DMA_CTR_WORD_WIDTH,                                                                                                // must word.
            .src_burst_size = 0,                                 // must 0.
            .read_num_en    = 0,
         .priority       = 0,
         .write_num_en   = 0,
         .auto_en        = 0, // must 0.
        },
        {
         .dst_req_sel    = DMA_REQ_AUDIO2_TX,        // tx req.
            .src_req_sel    = 0,
         .dst_addr_ctrl  = DMA_ADDR_FIX,
         .src_addr_ctrl  = DMA_ADDR_INCREMENT, // increment.
            .dstmode        = DMA_HANDSHAKE_MODE,                                                        // handshake.
            .srcmode        = DMA_NORMAL_MODE,
         .dstwidth       = DMA_CTR_WORD_WIDTH,                                               // must word.
            .srcwidth       = DMA_CTR_WORD_WIDTH, // must word.
            .src_burst_size = 0,                                                                                  // must 0.
            .read_num_en    = 0,
         .priority       = 0,
         .write_num_en   = 0,
         .auto_en        = 0, // must 0.
        }
};

/**
 * @brief     This function configures amic bias pin.
 * @param[in] amic_bias - the amic bias pin.
 * @return    none.
 */
void audio_set_amic_bias_pin(gpio_pin_e amic_bias)
{
    gpio_function_en(amic_bias);
    gpio_output_en(amic_bias);
    gpio_input_dis(amic_bias);
    gpio_set_high_level(amic_bias);
}

/**
 * @brief     This function configures stream0 dmic pin.
 * @param[in] dmic0_data - the data of  dmic pin
 * @param[in] dmic0_clk1 - the clk1 of dmic pin
 * @param[in] dmic0_clk2 - the clk2 of dmic pin,if need not set clk2,please set GPIO_NONE_PIN
 * @return    none
 */
void audio_set_stream0_dmic_pin(gpio_func_pin_e dmic0_data, gpio_func_pin_e dmic0_clk1, gpio_func_pin_e dmic0_clk2)
{
    gpio_input_en((gpio_pin_e)dmic0_data);
    gpio_set_mux_function(dmic0_data, DMIC0_DAT_I);
    gpio_function_dis((gpio_pin_e)dmic0_data);
    gpio_set_mux_function(dmic0_clk1, DMIC0_CLK);
    gpio_function_dis((gpio_pin_e)dmic0_clk1);
    if (dmic0_clk2 != GPIO_NONE_PIN) {
        gpio_set_mux_function(dmic0_clk2, DMIC0_CLK);
        gpio_function_dis((gpio_pin_e)dmic0_clk2);
    }
}

/**
 * @brief     This function configures sdm pin.
 * @param[in] config -sdm config pin struct.
 * @return    none.
 */
void audio_set_sdm_pin(sdm_pin_config_t *config)
{
    if (config->sdm0_p_pin != GPIO_NONE_PIN) {
        gpio_set_mux_function(config->sdm0_p_pin, SDM0_P);
        gpio_function_dis((gpio_pin_e)config->sdm0_p_pin);
    }

    if (config->sdm0_n_pin != GPIO_NONE_PIN) {
        gpio_set_mux_function(config->sdm0_n_pin, SDM0_N);
        gpio_function_dis((gpio_pin_e)config->sdm0_n_pin);
    }

    if (config->sdm1_p_pin != GPIO_NONE_PIN) {
        gpio_set_mux_function(config->sdm1_p_pin, SDM1_P);
        gpio_function_dis((gpio_pin_e)config->sdm1_p_pin);
    }

    if (config->sdm1_n_pin != GPIO_NONE_PIN) {
        gpio_set_mux_function(config->sdm1_n_pin, SDM1_N);
        gpio_function_dis((gpio_pin_e)config->sdm1_n_pin);
    }
}

/**
 * @brief     This function configures sdm pin.
 * @param[in] config -sdm config pin struct.
 * @return    none.
 */
void audio_unset_sdm_pin(sdm_pin_config_t *config)
{
    if (config->sdm0_p_pin != GPIO_NONE_PIN) {
        gpio_function_en((gpio_pin_e)config->sdm0_p_pin);
    }

    if (config->sdm0_n_pin != GPIO_NONE_PIN) {
        gpio_function_en((gpio_pin_e)config->sdm0_n_pin);
    }

    if (config->sdm1_p_pin != GPIO_NONE_PIN) {
        gpio_function_en((gpio_pin_e)config->sdm1_p_pin);
    }

    if (config->sdm1_n_pin != GPIO_NONE_PIN) {
        gpio_function_en((gpio_pin_e)config->sdm1_n_pin);
    }
}

/**
 * @brief     This function configures i2s pin.
 * @param[in] i2s_select       - channel select
 * @param[in] config           - i2s config pin struct.
 * @return    none.
 */
void i2s_set_pin(audio_i2s_select_e i2s_select, i2s_pin_config_t *config)
{
    if (i2s_select <= I2S1) {
        gpio_input_en((gpio_pin_e)config->bclk_pin);
        gpio_set_mux_function(config->bclk_pin, I2S0_BCK_IO + (i2s_select) * 6);
        gpio_function_dis((gpio_pin_e)config->bclk_pin);

        if (config->adc_lr_clk_pin != GPIO_NONE_PIN) {
            gpio_input_en((gpio_pin_e)config->adc_lr_clk_pin);
            gpio_set_mux_function(config->adc_lr_clk_pin, I2S0_LR0_IO + (i2s_select) * 6);
            gpio_function_dis((gpio_pin_e)config->adc_lr_clk_pin);
        }

        if (config->dac_lr_clk_pin != GPIO_NONE_PIN) {
            gpio_input_en((gpio_pin_e)config->dac_lr_clk_pin);
            gpio_set_mux_function(config->dac_lr_clk_pin, I2S0_LR1_IO + (i2s_select) * 6);
            gpio_function_dis((gpio_pin_e)config->dac_lr_clk_pin);
        }

        if (config->adc_dat_pin != GPIO_NONE_PIN) {
            gpio_input_en((gpio_pin_e)config->adc_dat_pin);
            gpio_set_mux_function(config->adc_dat_pin, I2S0_DAT0_IO + (i2s_select) * 6);
            gpio_function_dis((gpio_pin_e)config->adc_dat_pin);
        }

        if (config->dac_dat_pin != GPIO_NONE_PIN) {
            gpio_input_en((gpio_pin_e)config->dac_dat_pin);
            gpio_set_mux_function(config->dac_dat_pin, I2S0_DAT1_IO + (i2s_select) * 6);
            gpio_function_dis((gpio_pin_e)config->dac_dat_pin);
        }
    } else {
        gpio_input_en((gpio_pin_e)config->bclk_pin);
        gpio_set_mux_function((gpio_func_pin_e)config->bclk_pin, I2S2_BCK_IO);
        gpio_function_dis((gpio_pin_e)config->bclk_pin);

        if (config->adc_lr_clk_pin != GPIO_NONE_PIN) {
            gpio_input_en((gpio_pin_e)config->adc_lr_clk_pin);
            gpio_set_mux_function((gpio_func_pin_e)config->adc_lr_clk_pin, I2S2_LR0_IO);
            gpio_function_dis((gpio_pin_e)config->adc_lr_clk_pin);
        }

        if (config->dac_lr_clk_pin != GPIO_NONE_PIN) {
            gpio_input_en((gpio_pin_e)config->dac_lr_clk_pin);
            gpio_set_mux_function((gpio_func_pin_e)config->dac_lr_clk_pin, I2S2_LR1_IO);
            gpio_function_dis((gpio_pin_e)config->dac_lr_clk_pin);
        }

        if (config->adc_dat_pin != GPIO_NONE_PIN) {
            gpio_input_en((gpio_pin_e)config->adc_dat_pin);
            gpio_set_mux_function((gpio_func_pin_e)config->adc_dat_pin, I2S2_DAT0_IO);
            gpio_function_dis((gpio_pin_e)config->adc_dat_pin);
        }

        if (config->dac_dat_pin != GPIO_NONE_PIN) {
            gpio_input_en((gpio_pin_e)config->dac_dat_pin);
            gpio_set_mux_function((gpio_func_pin_e)config->dac_dat_pin, I2S2_DAT1_IO);
            gpio_function_dis((gpio_pin_e)config->dac_dat_pin);
        }
    }
}

/**
 * @brief     This function serves to config  rx_dma channel.
 * @param[in] chn          - dma channel
 * @param[in] dst_addr     - Pointer to data buffer, it must be 4-bytes aligned address.
 *                           and the actual buffer size defined by the user needs to be not smaller than the data_len, otherwise there may be an out-of-bounds problem.
 * @param[in] data_len     - Length of DMA in bytes, it must be set to a multiple of 4. The maximum value that can be set is 0x10000. 
 * @param[in] head_of_list - the head address of dma llp.
 * @return    none
 */
void audio_rx_dma_config(dma_chn_e chn, unsigned short *dst_addr, unsigned int data_len, dma_chain_config_t *head_of_list)
{
    audio_rx_dma_chn = chn;
    audio_set_rx_buff_len(data_len, audio_rx_fifo_chn);
    dma_config(chn, &audio_dma_rx_config[audio_rx_fifo_chn]);
    dma_set_address(chn, REG_AUDIO_FIFO_ADDR(audio_rx_fifo_chn), (unsigned int)(dst_addr));
    dma_set_size(chn, data_len, DMA_WORD_WIDTH);
    reg_dma_llp(chn) = (unsigned int)head_of_list;
}

/**
 * @brief     This function serves to set rx dma chain transfer
 * @param[in] config_addr - the head of list of llp_pointer.
 * @param[in] llpointer   - the next element of llp_pointer.
 * @param[in] dst_addr    - Pointer to data buffer, it must be 4-bytes aligned address and the actual buffer size defined by the user needs to
 *                          be not smaller than the data_len, otherwise there may be an out-of-bounds problem.
 * @param[in] data_len    - Length of DMA in bytes, it must be set to a multiple of 4. The maximum value that can be set is 0x10000.
 * @return    none
 */
void audio_rx_dma_add_list_element(dma_chain_config_t *config_addr, dma_chain_config_t *llpointer, unsigned short *dst_addr, unsigned int data_len)
{
    config_addr->dma_chain_ctl      = reg_dma_ctrl(audio_rx_dma_chn) | BIT(0);
    config_addr->dma_chain_src_addr = REG_AUDIO_FIFO_ADDR(audio_rx_fifo_chn);
    config_addr->dma_chain_dst_addr = (unsigned int)dst_addr;
    config_addr->dma_chain_data_len = dma_cal_size(data_len, DMA_WORD_WIDTH);
    config_addr->dma_chain_llp_ptr  = (unsigned int)llpointer;
}

/**
 * @brief  This function serves to set audio rx dma chain transfer.
 * @param[in] chn         - dma channel.
 * @param[in] in_buff     - Pointer to data buffer, it must be 4-bytes aligned address and the actual buffer size defined by the user needs to
 *                          be not smaller than the data_len, otherwise there may be an out-of-bounds problem.
 * @param[in] buff_size   - Length of DMA in bytes, it must be set to a multiple of 4. The maximum value that can be set is 0x10000.
 * @param[in] rx_fifo_chn - rx fifo select.
 * @return    none.
 */
void audio_rx_dma_chain_init(audio_fifo_chn_e rx_fifo_chn, dma_chn_e chn, unsigned short *in_buff, unsigned int buff_size)
{
    audio_rx_fifo_chn = rx_fifo_chn;
    audio_rx_dma_config(chn, (unsigned short *)in_buff, buff_size, &g_audio_rx_dma_list_cfg[rx_fifo_chn]);
    audio_rx_dma_add_list_element(&g_audio_rx_dma_list_cfg[rx_fifo_chn], &g_audio_rx_dma_list_cfg[rx_fifo_chn], (unsigned short *)in_buff, buff_size);
}

/**
 * @brief     This function serves to config  tx_dma channel.
 * @param[in] chn          - dma channel
 * @param[in] src_addr     - Pointer to data buffer, it must be 4-bytes aligned address.
 * @param[in] data_len     - Length of DMA in bytes, range from 1 to 0x10000.
 * @param[in] head_of_list - the head address of dma llp.
 * @return    none
 */
void audio_tx_dma_config(dma_chn_e chn, unsigned short *src_addr, unsigned int data_len, dma_chain_config_t *head_of_list)
{
    audio_tx_dma_chn = chn;
    audio_set_tx_buff_len(data_len, audio_tx_fifo_chn);
    dma_config(chn, &audio_dma_tx_config[audio_tx_fifo_chn]);
    dma_set_address(chn, (unsigned int)(src_addr), REG_AUDIO_FIFO_ADDR(audio_tx_fifo_chn));
    dma_set_size(chn, data_len, DMA_WORD_WIDTH);
    reg_dma_llp(chn) = (unsigned int)head_of_list;
}

/**
 * @brief     This function serves to set tx dma chain transfer
 * @param[in] config_addr - the head of list of llp_pointer.
 * @param[in] llpointer   - the next element of llp_pointer.
 * @param[in] src_addr    - Pointer to data buffer, it must be 4-bytes aligned address.
 * @param[in] data_len    - Length of DMA in bytes, range from 1 to 0x10000.
 * @return    none
 */
void audio_tx_dma_add_list_element(dma_chain_config_t *config_addr, dma_chain_config_t *llpointer, unsigned short *src_addr, unsigned int data_len)
{
    config_addr->dma_chain_ctl      = reg_dma_ctrl(audio_tx_dma_chn) | BIT(0);
    config_addr->dma_chain_src_addr = (unsigned int)src_addr;
    config_addr->dma_chain_dst_addr = REG_AUDIO_FIFO_ADDR(audio_tx_fifo_chn);
    config_addr->dma_chain_data_len = dma_cal_size(data_len, DMA_WORD_WIDTH);
    config_addr->dma_chain_llp_ptr  = (unsigned int)llpointer;
}

/**
 * @brief  This function serves to initialize audio tx dma chain transfer.
 * @param[in] chn         - dma channel.
 * @param[in] out_buff    - Pointer to data buffer, it must be 4-bytes aligned address.
 * @param[in] buff_size   - Length of DMA in bytes, range from 1 to 0x10000.
 * @param[in] tx_fifo_chn - tx fifo select.
 * @return    none.
 */
void audio_tx_dma_chain_init(audio_fifo_chn_e tx_fifo_chn, dma_chn_e chn, unsigned short *out_buff, unsigned int buff_size)
{
    audio_tx_fifo_chn = tx_fifo_chn;
    audio_tx_dma_config(chn, (unsigned short *)out_buff, buff_size, &g_audio_tx_dma_list_cfg[tx_fifo_chn]);
    audio_tx_dma_add_list_element(&g_audio_tx_dma_list_cfg[tx_fifo_chn], &g_audio_tx_dma_list_cfg[tx_fifo_chn], (unsigned short *)out_buff, buff_size);
}

/**
 * @brief       This function serves to set audio tx dma burst size and trigger number.
 * @param[in]   fifo_chn     - the spi module.
 * @param[in]   dma_chn      - dma channel.
 * @param[in]   burst_size   - dma burst size.
 * @return      none.
 */
void audio_set_dma_tx_burst(audio_fifo_chn_e fifo_chn, dma_chn_e dma_chn, dma_burst_size_e burst_size)
{
    dma_set_burst_size(dma_chn, burst_size);
    audio_set_fifo_tx_trig_num(fifo_chn, burst_size);
}

/**
 * @brief       This function serves to set audio rx dma burst size and trigger number.
 * @param[in]   fifo_chn     - the spi module.
 * @param[in]   dma_chn      - dma channel.
 * @param[in]   burst_size   - dma burst size.
 * @return      none.
 */
void audio_set_dma_rx_burst(audio_fifo_chn_e fifo_chn, dma_chn_e dma_chn, dma_burst_size_e burst_size)
{
    dma_set_burst_size(dma_chn, burst_size);
    audio_set_fifo_rx_trig_num(fifo_chn, burst_size);
}

/**
 * @brief     This function serves to power on audio.
 * @return    none.
 */
void audio_power_on(void)
{
    pm_set_dig_module_power_switch(FLD_PD_AUDIO_EN, PM_POWER_UP);
    BM_SET(reg_rst2, FLD_RST2_AUD);
    BM_SET(reg_clk_en2, FLD_CLK2_AUD_EN);
}

/**
 * @brief      This function serves to power down all audio power.
 * @return     none.
 */
void audio_power_down(void)
{
    BM_CLR(reg_rst2, FLD_RST2_AUD);
    BM_CLR(reg_clk_en2, FLD_CLK2_AUD_EN);
    pm_set_dig_module_power_switch(FLD_PD_AUDIO_EN, PM_POWER_DOWN);
}

/**
 *  @brief      This function serves to set the clock of audio
 *  @param[in]  div_numerator   - the dividing factor of div_numerator (15bits valid).
 *  @param[in]  div_denominator - the dividing factor of div_denominator(16bits valid).
 *  @return     none
 *  @attention  The default is from pll 240M(default). If the pll is changed, the clk will be changed accordingly.
 */
void audio_set_audio_clk(unsigned short div_numerator, unsigned short div_denominator)
{
    clock_bbpll_config(PLL_CLK);
    reg_dmic_step = (div_numerator & FLD_DMIC_STEP) | FLD_DMIC_SEL;
    reg_dmic_mod  = div_denominator;
}

/**
 * @brief     This function serves to initial audio.
 * @return    none.
 */
void audio_init(void)
{
    audio_power_on();
    audio_set_audio_clk(1, sys_clk.pll_clk / 24); // audio clk=240M*(div_numerator/div_denominator):240M*(1/pll_clk / 24)=24M
}

/**
 * @brief     This function serves to initial audio codec.
 * @return    none
 */
void audio_codec_init(void)
{
    reg_codec_clkcfg |= MASK_VAL(FLD_CLK_USB, 0x01, FLD_CLK_DIV2, 0x01);
}

/**
 * @brief     This function serves to reinit audio codec clock.
 * @param[in]  div_numerator   - the dividing factor of div_numerator (15bits valid).
 * @param[in]  div_denominator - the dividing factor of div_denominator(16bits valid).
 * @param[in]  clk_usb_mode - CLK_USB_MODE_OFF 128 downsampling; CLK_USB_MODE_ON 125 downsampling.
 * @param[in]  rate - audio sample rate.
 * @return    none
 * @note      Just for test, should be called after audio_codec_stream0_input_init
 *            CLK_USB_MODE_OFF: codec input sample rate using 128 downsampling (higher THD+N Ratio)
 *            CLK_USB_MODE_ON:  codec input sample rate using 125 downsampling
 *            Therefore, changing this configuration requires a simultaneous modification of the codec's clock
 *            in order to get the corresponding sample rate.
 *            Need to be placed after audio_codec_stream0_input_init.
 */
void audio_reset_audio_clk(unsigned short div_numerator, unsigned short div_denominator, codec_clk_usb_mode_e clk_usb_mode, audio_sample_rate_e rate)
{
    if (clk_usb_mode == CLK_USB_MODE_OFF) {
        reg_codec_clkcfg &= ~FLD_CLK_USB;
    } else {
        reg_codec_clkcfg |= FLD_CLK_USB;
    }
    audio_set_audio_clk(div_numerator, div_denominator); // audio clk=240M*(div_numerator/div_denominator)
    reg_codec_clkcfg = (reg_codec_clkcfg & (~FLD_CLK_SR)) | (audio_codec_rate[rate] << 0x01);
}

/**
 * @brief     This function serves to set codec_adc clock.
 * @return    none.
 */
void audio_codec_set_adc_clock(void)
{
    reg_codec_cfg1 |= FLD_R_CK_SEL; // b1'0: adc data clk to adc core clk ratio 6:1, b1'1: adc data clk to adc core clk ratio 1:1;
}

/**
 * @brief      This function serves to power on codec_adc.
 * @return     none.
 */
void audio_codec_adc_power_on(void)
{
    audio_codec_set_adc_vmid(POWER_ON);
    analog_write_reg8(areg_0x8d, (analog_read_reg8(areg_0x8d) & (~FLD_AUDIO_INMUTE_PGA)));
    analog_write_reg8(areg_0x8e, (analog_read_reg8(areg_0x8e) & (~(FLD_AUDIO_PD_PGABUF | FLD_AUDIO_PD_PGABOOST | FLD_AUDIO_PD_INPPGA | FLD_AUDIO_PD_ASDM | FLD_AUDIO_PD_BIAS))));
}

/**
 * @brief     This function serves to power down codec_adc.
 * @return    none
 */
void audio_codec_adc_power_down(void)
{
    audio_codec_set_adc_vmid(POWER_DOWN);
    analog_write_reg8(areg_0x8d, (analog_read_reg8(areg_0x8d) | FLD_AUDIO_INMUTE_PGA));
    analog_write_reg8(areg_0x8e, (analog_read_reg8(areg_0x8e) | FLD_AUDIO_PD_PGABUF | FLD_AUDIO_PD_PGABOOST | FLD_AUDIO_PD_INPPGA | FLD_AUDIO_PD_ASDM | FLD_AUDIO_PD_BIAS));
}

/**
 * @brief      This function serves to set adc vmid.
 * @param[in]  en               - POWER_DOWN or POWER_ON.
 * @return     none.
 */
void audio_codec_set_adc_vmid(power_switch_e en)
{
    if (en) {
        if (g_chip_version == CHIP_VERSION_A1) {
            /***enable vmid voltage***/
            analog_write_reg8(areg_0x8f, (analog_read_reg8(areg_0x8f) & (~FLD_AUDIO_VMID_PD)));
        } else {
            /***enable vmid voltage***/
            analog_write_reg8(areg_0x8f, (analog_read_reg8(areg_0x8f) | FLD_AUDIO_VMID_PD));
        }
        /***keep 0.9v vmid***/
        analog_write_reg8(areg_0x8e, (analog_read_reg8(areg_0x8e) | FLD_AUDIO_PD_ENABLE_0R6));
    } else {
        if (g_chip_version == CHIP_VERSION_A1) {
            /***disable vmid voltage***/
            analog_write_reg8(areg_0x8f, (analog_read_reg8(areg_0x8f) | FLD_AUDIO_VMID_PD));
        } else {
            /***disable vmid voltage***/
            analog_write_reg8(areg_0x8f, (analog_read_reg8(areg_0x8f) & (~FLD_AUDIO_VMID_PD)));
        }
    }
}

/**
 *  @brief      This function serves to set stream input mode.
 *  @param[in]  input_mode - 0:line_in or amic 1:dmic.
 *  @return     none.
 */
void audio_set_codec_stream0_input_mode(audio_codec_input_mode_e input_mode)
{
    reg_codec_vol = (reg_codec_vol & (~FLD_MIC_SEL)) | MASK_VAL(FLD_MIC_SEL, input_mode);
}

/**
 *  @brief      This function serves to set stream input path.
 *  @param[in]  che_en  - channel selection.
 *  @return     none.
 */
void audio_set_codec_stream0_path(audio_chn_sel_e che_en)
{
    reg_codec_cfg = (reg_codec_cfg & (~(FLD_L_CH_EN | FLD_R_CH_EN))) | MASK_VAL(FLD_L_CH_EN, (che_en & CHANNEL_LEFT) ? (0x01) : (0x00), FLD_R_CH_EN, (che_en & CHANNEL_RIGHT) ? (0x01) : (0x00));
}

/**
 *  @brief      This function serves to set stream sample_rate.
 *  @param[in]  source - audio input source select.
 *  @param[in]  rate   - sample_rate.
 *  @return     none.
 */
void audio_set_codec_stream0_sample_rate(codec_stream0_input_src_e source, audio_sample_rate_e rate)
{
    if (source & BIT(3)) {
        /* When the sampling rate is greater than or equal to 32K there is a data channel reversal problem,
        * so reg_codec_cfg bit[5] need to be configured to adjust the direction.
        * Moreover, this bit is also recommended to be configured for scenarios with less than 32k sampling rate,
        * and in this case, the direction will not be flipped.
        */
        reg_codec_cfg |= FLD_R_NEG;
        if (rate >= AUDIO_32K) {
            rate += 3;
        }
    }
    reg_codec_clkcfg = (reg_codec_clkcfg & (~FLD_CLK_SR)) | (audio_codec_rate[rate] << 0x01);
}

/**
 *  @brief      This function serves to set codec configuration.
 *  @param[in]  source - audio input source select.
 *  @param[in]  rate   - sample_rate.
 *  @return     none.
 */
void audio_codec_stream0_input_config(codec_stream0_input_src_e source, audio_sample_rate_e rate)
{
    if (!(source & BIT(3))) {
        audio_codec_set_adc_clock();
        audio_codec_adc_power_on();
    }
    audio_set_codec_stream0_input_mode(source >> 3);
    audio_set_codec_stream0_path(source & 3);
    audio_set_codec_stream0_sample_rate(source, rate);
    /*Initialization sets all input stream pga gain to 0db and dig gain -24db
     * 1.for performance test
     */
    audio_set_adc_pga_gain(CODEC_IN_GAIN_0_DB);
    audio_set_stream0_dig_gain(CODEC_IN_D_GAIN_m24_DB);
}

/**
 *  @brief      This function serves to set codec mono mode channel.
 *  @param[in]  fifo_chn - fifo channel.
 *  @param[in]  chn      - mono mode channel.
 *  @return     none.
 */
void audio_set_codec_fifo_mono_channel(audio_fifo_chn_e fifo_chn, audio_chn_sel_e chn)
{
    if (chn & CHANNEL_LEFT) {
        reg_mono_rxfifo_sel(0x00) = (reg_mono_rxfifo_sel(0x00) & (~FLD_DECL_RXFIFO_SEL)) | MASK_VAL(FLD_DECL_RXFIFO_SEL, fifo_chn);
    } else if (chn & CHANNEL_RIGHT) {
        reg_mono_rxfifo_sel(0x00) = (reg_mono_rxfifo_sel(0x00) & (~FLD_DECR_RXFIFO_SEL)) | MASK_VAL(FLD_DECR_RXFIFO_SEL, fifo_chn);
    }
}

/**
 *  @brief      This function serves to set stream codec input fifo data format.
 *  @param[in]  fifo_chn   - fifo channel.
 *  @param[in]  ain_mode   - number of channels.
 *  @param[in]  data_width - data bit width.
 *  @return     none.
 */
void audio_set_stream0_fifo_input_mode(audio_fifo_chn_e fifo_chn, audio_channel_select_e ain_mode, audio_codec_wl_mode_e data_width)
{
    if (ain_mode == AUDIO_STEREO) {
        ain_mode += 1;
    }

    switch (fifo_chn) {
    case FIFO0:
        reg_dec_ain_mode = (reg_dec_ain_mode & (~FLD_DEC_AIN0_MODE)) | MASK_VAL(FLD_DEC_AIN0_MODE, ain_mode + data_width);
        break;
    case FIFO1:
        reg_dec_ain_mode = (reg_dec_ain_mode & (~FLD_DEC_AIN1_MODE)) | MASK_VAL(FLD_DEC_AIN1_MODE, ain_mode + data_width);
        break;
    case FIFO2:
        reg_dec_ain_mode = (reg_dec_ain_mode & (~FLD_DEC_AIN2_MODE)) | MASK_VAL(FLD_DEC_AIN2_MODE, ain_mode + data_width);
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to set codec stream0 input data bit width for fifo0/fifo1/fifo2 .
 * @param[in]  fifo_chn   - fifo0/fifo1/fifo2
 * @param[in]  source     - audio input source select.
 * @param[in]  data_width - input data bit width selection
 * @return     none.
 */
void audio_codec_set_stream0_fifo_input_mode(audio_fifo_chn_e fifo_chn, codec_stream0_input_src_e source, audio_codec_wl_mode_e data_width)
{
    unsigned char ain_mode = 0;
    if ((source & CHANNEL_STEREO) == CHANNEL_STEREO) {
        ain_mode = AUDIO_STEREO;
    } else {
        ain_mode = AUDIO_MONO;
        audio_set_codec_fifo_mono_channel(fifo_chn, source & 3);
    }
    audio_set_stream0_fifo_input_mode(fifo_chn, ain_mode, data_width);
}

/**
 * @brief     This function serves to set fifo input data path.
 * @param[in] fifo_chn - fifo select
 * @param[in] ain_sel  - fifo input source select
 * @return    none
 */
void audio_data_fifo_input_path_sel(audio_fifo_chn_e fifo_chn, audio_mux_ain_e ain_sel)
{
    if (FIFO0 == fifo_chn) {
        reg_fifoin0_sel = (reg_fifoin0_sel & (~FLD_AIN0_SEL)) | ain_sel;
    } else if (FIFO1 == fifo_chn) {
        reg_fifoin12_sel = (reg_fifoin12_sel & (~FLD_AIN1_SEL)) | ain_sel;
    } else {
        reg_fifoin12_sel = (reg_fifoin12_sel & (~FLD_AIN2_SEL)) | (ain_sel << 4);
    }
}

/**
 *  @brief      This function serves to set sdm sample rate.
 *  @param[in]  output_src - sdm channel.
 *  @param[in]  rate       - sdm sample rate.
 *  @return     none.
 */
void audio_set_stream_output_sample_rate(audio_stream_output_src_e output_src, audio_sample_rate_e rate)
{
    unsigned int *step_index[2] = {audio_stream_output_mono_step, audio_stream_output_stereo_step};

    audio_set_ascl_channel(ASCL0, (audio_channel_select_e)output_src);
    audio_set_ascl_tune_step(ASCL0, step_index[output_src][rate]);
}

/**
 *  @brief      This function serves to set sdm path.
 *  @param[in]  chn  - channel selection.
 *  @return     none.
 */
void audio_set_sdm_path(audio_stream_output_src_e chn)
{
    unsigned char value = 0x00;
    if (chn == SDM_MONO) {
        value = 0x01;
    }
    reg_aud_en = (reg_aud_en & (~FLD_MONO)) | MASK_VAL(FLD_MONO, value);
}

/**
 * @brief This function serves to enable audio codec fifo input.
 * @param[in]  fifo_chn        - select channel fifo
 * @return     none.
 */
void audio_codec_input_path_en(audio_fifo_chn_e fifo_chn)
{
    audio_data_fifo_input_path_sel(fifo_chn, CODEC_IN_FIFO);
}

/**
 * @brief This function serves to configure audio stream0 fifo input.
 * @param[in]  fifo_chn        - select channel fifo
 * @param[in]  source          - codec input source
 * @param[in]  data_width      - fifo data bit width.
 * @return     none.
 */
void audio_codec_stream0_fifo_input_config(audio_fifo_chn_e fifo_chn, codec_stream0_input_src_e source, audio_codec_wl_mode_e data_width)
{
    audio_rxfifo_en(fifo_chn);
    audio_rxfifo_wptr_en(fifo_chn);
    audio_codec_set_stream0_fifo_input_mode(fifo_chn, source, data_width);
}

/**
 *  @brief      This function serves to set codec initialization.
 *  @param[in]  audio_codec - audio_codec_stream0_input_t pointer.
 *  @return     none.
 */
void audio_codec_stream0_input_init(audio_codec_stream0_input_t *audio_codec)
{
    audio_codec_init();
    audio_codec_stream0_input_config(audio_codec->input_src, audio_codec->sample_rate);
    audio_codec_stream0_fifo_input_config(audio_codec->fifo_chn, audio_codec->input_src, audio_codec->data_width);
}

/**
 * @brief      This function serves to configure audio stream output.
 * @param[in]  chn        - audio output source select.
 * @param[in]  rate       - select channel sample rate.
 * @return     none.
 */
void audio_stream_output_config(audio_stream_output_src_e chn, audio_sample_rate_e rate)
{
    /***sdm and i2s0 share a common clock***/
    audio_set_i2s0_clk(1, sys_clk.pll_clk / 2); // sdm clk is currently configured for 2m.
    audio_ascl_lerp_en(ASCL0);
    audio_set_stream_output_sample_rate(chn, rate);
    audio_set_ascl_gain(ASCL0, ASCL_OUT_D_GAIN_0_DB);
}

/**
 * @brief This function serves to configure audio stream fifo output.
 * @param[in]  fifo_chn         - select channel fifo
 * @param[in]  chn              - audio output source select.
 * @param[in]  data_width       - fifo data bit width.
 * @return    none
 */
void audio_codec_stream_fifo_output_config(audio_fifo_chn_e fifo_chn, audio_stream_output_src_e chn, audio_codec_wl_mode_e data_width)
{
    /***sdm can only use fifo0***/
    audio_txfifo_en(fifo_chn);
    audio_txfifo_wptr_en(fifo_chn);
    audio_set_sdm_path(chn);
    audio_set_i2s_fifo_output_mode(fifo_chn, (audio_channel_select_e)chn, (audio_i2s_fifo_wl_e)data_width);
}

/**
 * @brief      This function serves to set sdm initialization.
 * @param[in]  audio_stream_output   - audio_codec_output_t pointer.
 * @return     none.
 */
void audio_codec_stream_output_init(audio_codec_output_t *audio_stream_output)
{
    audio_stream_output_config(audio_stream_output->output_src, audio_stream_output->sample_rate);
    audio_codec_stream_fifo_output_config(FIFO0, audio_stream_output->output_src, audio_stream_output->data_width);
}

/**
 *  @brief      This function serves to set ascl step.
 *  @param[in]  ascl_select - ascl channel.
 *  @param[in]  value       - ascl step value.
 *  @return     none.
 */
void audio_set_ascl_tune_step(audio_ascl_select_e ascl_select, unsigned int value)
{
    if (ascl_select != ASCL0) {
        reg_step_tune_12(ascl_select) = value;
    } else {
        reg_step_tune_0 = value;
    }
}

/**
 *  @brief      This function serves to set ascl channel.
 *  @param[in]  ascl_select - ascl channel.
 *  @param[in]  chn         - audio channel select.
 *  @return     none.
 */
void audio_set_ascl_channel(audio_ascl_select_e ascl_select, audio_channel_select_e chn)
{
    if (ascl_select != ASCL0) {
        if (chn == AUDIO_MONO) {
            BM_SET(reg_ascl_config(ascl_select), FLD_MONO_12);
        } else {
            BM_CLR(reg_ascl_config(ascl_select), FLD_MONO_12);
        }
    } else {
        if (chn == AUDIO_MONO) {
            BM_SET(reg_aud_en, FLD_MONO);
        } else {
            BM_CLR(reg_aud_en, FLD_MONO);
        }
    }
}

/**
 *  @brief      This function serves to set ascl conversion sample rate change.
 *  @param[in]  ascl_select          - ascl channel.
 *  @param[in]  sample_rate_in       - input sample rate.
 *  @param[in]  sample_rate_out      - output sample rate.
 *  @param[in]  chn                  - audio channel select.
 *  @return     none.
 */
void audio_ascl_set_conversion_sample_rate(audio_ascl_select_e ascl_select, audio_ascl_sample_rate_e sample_rate_in, audio_ascl_sample_rate_e sample_rate_out, audio_channel_select_e chn)
{
    unsigned int step = 0;

    step = 0x80000 / audio_sample_rate_value[sample_rate_out] * audio_sample_rate_value[sample_rate_in];
    if (chn == AUDIO_MONO) {
        step <<= 1;
    }
    step = (step << 12) | 0x01;
    audio_set_ascl_channel(ascl_select, chn);
    audio_set_ascl_tune_step(ascl_select, step);
}

/**
 * @brief      This function serves to set sdm  left channel dither control.
 * @param[in]  dither_control_src   - dither control source.
 * @param[in]  config_value         - When the input source is DC_CONSTANT, config_value is the constant value of the input.
 *                                    When it is PN_SEQUENCE or PN_SEQUENCE_SHAPPING, config_value is the number of bits
 *                                    (in the range of 0-16) at the PN1/PN2 generator.
 * @return     none.
 */
void audio_sdm_dither_control_left(audio_stream_output_dither_control_e dither_control_src, unsigned short config_value)
{
    if (dither_control_src == DC_CONSTANT) {
        audio_sdm_dc_constant_left_en();
        audio_sdm_set_dc_constant_value_left(config_value);
    } else {
        if (dither_control_src == PN_SEQUENCE) {
            audio_sdm_pn_sequence_left_en();
        } else if (dither_control_src == PN_SEQUENCE_SHAPPING) {
            audio_sdm_pn_sequence_with_shapping_left_en();
        }
        audio_sdm_set_pn_sequence_bit_num_left(config_value);
    }
}

/**
 * @brief      This function serves to set sdm  right channel dither control.
 * @param[in]  dither_control_src   - dither control source.
 * @param[in]  config_value         - When the input source is DC_CONSTANT, config_value is the constant value of the input.
 *                                    When it is PN_SEQUENCE or PN_SEQUENCE_SHAPPING, config_value is the number of bits
 *                                    (in the range of 0-16) at the PN1/PN2 generator.
 * @return     none.
 */
void audio_sdm_dither_control_right(audio_stream_output_dither_control_e dither_control_src, unsigned short config_value)
{
    if (dither_control_src == DC_CONSTANT) {
        audio_sdm_dc_constant_right_en();
        audio_sdm_set_dc_constant_value_right(config_value);
    } else {
        if (dither_control_src == PN_SEQUENCE) {
            audio_sdm_pn_sequence_right_en();
        } else if (dither_control_src == PN_SEQUENCE_SHAPPING) {
            audio_sdm_pn_sequence_with_shapping_right_en();
        }
        audio_sdm_set_pn_sequence_bit_num_right(config_value);
    }
}

/**
 * @brief      This function serves to set ascl data format.
 * @param[in]  ascl_select   - ascl channel.
 * @param[in]  ascl_mode     - ascl data format.
 * @return     none.
 */
void audio_set_ascl_format(audio_ascl_select_e ascl_select, ascl_mode_select_e ascl_mode)
{
    if (ascl_select != ASCL0) {
        reg_ascl_config(ascl_select) = ((reg_ascl_config(ascl_select) & (~FLD_ASCL_FORMAT_SEL_12)) | MASK_VAL(FLD_ASCL_FORMAT_SEL_12, ascl_mode));
    } else {
        reg_aud_en = ((reg_aud_en & (~FLD_ASCL_FORMAT_SEL)) | MASK_VAL(FLD_ASCL_FORMAT_SEL, ascl_mode));
    }
}

/**
 * @brief     This function configures the internal codec clk to the external codec mclk via the debug pin,mclk=240M*(div_numerator/div_denominator):240M*(1/20)=12M.
 * @param[in] mclk_pin -mclk output pin.(only set PC6).
 * @return    none.
 * @attention If need to use internal codec at the same time, mclk must be set to 12M.
 */
void audio_set_codec_clk_as_mclk(gpio_func_pin_e mclk_pin)
{
    reg_codec_clkcfg = (reg_codec_clkcfg & (~FLD_CLK_DIV2)) | MASK_VAL(FLD_CLK_DIV2, 1, FLD_CLK_EN, 1);
    audio_set_debug_clk_as_mclk(CODEC_CLK_DBG);
    gpio_set_mux_function(mclk_pin, DBG_AUDIO_DAC);
    gpio_function_dis((gpio_pin_e)mclk_pin);
}

/**
 * @brief     This function configures i2s clk pin for extern codec clk,mclk=240M*(div_numerator/div_denominator):240M*(1/20)=12M.
 * @param[in] i2s_select i2s channel.
 * @param[in] mclk_pin -mclk output pin.
 * @param[in] div_numerator   - the dividing factor of div_numerator (15bits valid).
 * @param[in] div_denominator - the dividing factor of div_denominator(16bits valid).
 * @return    none.
 */
void audio_set_i2s_clk_as_mclk(audio_i2s_select_e i2s_select, gpio_func_pin_e mclk_pin, unsigned short div_numerator, unsigned short div_denominator)
{
    audio_set_i2s_clk(i2s_select, div_numerator, div_denominator);
    if (i2s_select <= I2S1) {
        gpio_set_mux_function(mclk_pin, I2S0_CLK + (i2s_select) * 6);
    } else {
        gpio_set_mux_function(mclk_pin, I2S2_CLK);
    }
    gpio_function_dis((gpio_pin_e)mclk_pin);
}

/**
 * @brief     This function configures audio module debug clk as codec mclk
 * @param[in] clk debug clk.
 * @return    none.
 */
void audio_set_debug_clk_as_mclk(audio_clk_dbg_e clk)
{
    BM_SET(reg_aclk_dbg, clk);
}

/**
 * @brief     This function serves to set sampling rate when i2s as master.
 * @param[in] i2s_select                    - i2s channel select.
 * @param[in] i2s_clk_config                 i2s_clk_config[2]   i2s_clk_config[3]-->lrclk_adc(sampling rate)
                                                  ||                  ||
 *  pll(240M default)------->div---->i2s_clk--->2*div(div=0,bypass)--->blck----->div
 *                           ||                                       ||
 *           i2s_clk_config[0]/i2s_clk_config[1]                 i2s_clk_config[4]-->lrclk_dac (sampling rate)
 *
 For example:sampling rate=16K,i2s_clk_config[5]={ 4,1875,0,32,32}, sampling rate=240M*(4/1875)/32=16K

 * @return    none
 * @attention The default is from pll 240M(default). If the pll is changed, the clk will be changed accordingly.
 */
_attribute_ram_code_sec_noinline_ void audio_set_i2s_clock(audio_i2s_select_e i2s_select, unsigned short *i2s_clk_config)
{
    audio_set_i2s_clk(i2s_select, i2s_clk_config[0], i2s_clk_config[1]);
    audio_set_i2s_bclk(i2s_select, i2s_clk_config[2]);
    audio_set_i2s_lrclk(i2s_select, i2s_clk_config[3], i2s_clk_config[4]);
}

/**
 * @brief     This function set i2s input data bit with.
 * @param[in] i2s_select       - channel select.
 * @param[in] data_mode        - i2s data bit with.
 * @return    none.
 */
void audio_set_i2s_data_wl(audio_i2s_select_e i2s_select, audio_i2s_wl_mode_e data_mode)
{
    reg_i2s_cfg2(i2s_select) = (reg_i2s_cfg2(i2s_select) & (~FLD_I2S_WL)) | MASK_VAL(FLD_I2S_WL, data_mode);
}

/**
 * @brief     This function set i2s format mode.
 * @param[in] i2s_select       - channel select.
 * @param[in] i2s_mode         - i2s format mode.
 * @return    none.
 */
void audio_set_i2s_format(audio_i2s_select_e i2s_select, i2s_mode_select_e i2s_mode)
{
    reg_i2s_cfg2(i2s_select) = (reg_i2s_cfg2(i2s_select) & (~I2S_FORMAT)) | MASK_VAL(I2S_FORMAT, i2s_mode);
}

/**
 *  @brief      This function serves to set i2s clk enable.
 *  @param[in]  i2s_select -i2s channel.
 *  @return     none.
 */
void audio_i2s_clk_en(audio_i2s_select_e i2s_select)
{
    BM_SET(reg_i2s_cfg1(i2s_select), FLD_I2S_CLK_EN);
}

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
void audio_i2s_config(audio_i2s_select_e i2s_select, i2s_mode_select_e i2s_format, audio_i2s_wl_mode_e wl, i2s_m_s_mode_e m_s, audio_i2s_invert_config_t *i2s_config_t)
{
    reg_i2s_cfg1(i2s_select) = (reg_i2s_cfg1(i2s_select) & (~(FLD_I2S_ADC_DCI_MS | FLD_I2S_DAC_DCI_MS | FLD_I2S_ADC_FRM_LOOP))) | MASK_VAL(FLD_I2S_ADC_DCI_MS, m_s, FLD_I2S_DAC_DCI_MS, m_s);

    reg_i2s_cfg2(i2s_select) = (reg_i2s_cfg2(i2s_select) & (~(FLD_I2S_WL | I2S_FORMAT | FLD_I2S_ADC_MBCLK_LOOP | FLD_I2S_DAC_MBCLK_LOOP))) | MASK_VAL(FLD_I2S_WL, wl, I2S_FORMAT, i2s_format);

    reg_i2s_cfg3(i2s_select) = (reg_i2s_cfg3(i2s_select) & (~(FLD_I2S_LRP | FLD_I2S_LRSWAP | FLD_I2S_DAC_FRM_LOOP))) | MASK_VAL(FLD_I2S_LRP, i2s_config_t->i2s_lr_clk_invert_select, FLD_I2S_LRSWAP, i2s_config_t->i2s_data_invert_select);
}

/**
 * @brief This function serves to initialize  configuration i2s.
 * @param[in]  i2s_config - the relevant configuration struct pointer
 * @return    none
 */
void audio_i2s_config_init(audio_i2s_config_t *i2s_config)
{
    i2s_set_pin(i2s_config->i2s_select, i2s_config->pin_config);
    if (i2s_config->master_slave_mode == I2S_AS_MASTER_EN) {
        audio_set_i2s_clock(i2s_config->i2s_select, i2s_config->sample_rate);
    } else {
        /* * When using i2s slave mode, just i2s src clk should be enabled. * add by jiawei.shi, confirmed by ting.fang 20250310 */
        audio_i2s_src_clk_en(i2s_config->i2s_select);
    }
    audio_i2s_config(i2s_config->i2s_select, i2s_config->i2s_mode, i2s_config->data_width, i2s_config->master_slave_mode, &audio_i2s_invert_config[i2s_config->i2s_select]);
}

/**
 *  @brief      This function serves to set i2s mono mode channel.
 *  @param[in]  i2s_select - i2s channel.
 *  @param[in]  fifo_chn   - fifo channel.
 *  @param[in]  chn        - mono mode channel.
 *  @return     none.
 */
void audio_set_i2s_fifo_mono_channel(audio_i2s_select_e i2s_select, audio_fifo_chn_e fifo_chn, audio_chn_sel_e chn)
{
    if ((i2s_select == I2S0) || (i2s_select == I2S1)) {
        switch (chn) {
        case CHANNEL_LEFT:
            reg_mono_rxfifo_sel(i2s_select) = (reg_mono_rxfifo_sel(i2s_select) & (~FLD_I2S01_MONOL_RXFIFO_SEL)) | MASK_VAL(FLD_I2S01_MONOL_RXFIFO_SEL, fifo_chn);
            break;
        case CHANNEL_RIGHT:
            reg_mono_rxfifo_sel(i2s_select) = (reg_mono_rxfifo_sel(i2s_select) & (~FLD_I2S01_MONOR_RXFIFO_SEL)) | MASK_VAL(FLD_I2S01_MONOR_RXFIFO_SEL, fifo_chn);
            break;
        default:
            break;
        }
    } else {
        switch (chn) {
        case CHANNEL_LEFT:
            reg_mono_rxfifo_sel(i2s_select) = (reg_mono_rxfifo_sel(i2s_select) & (~FLD_I2S2_MONOL_RXFIFO_SEL)) | MASK_VAL(FLD_I2S2_MONOL_RXFIFO_SEL, fifo_chn);
            break;
        case CHANNEL_RIGHT:
            reg_mono_rxfifo_sel(i2s_select) = (reg_mono_rxfifo_sel(i2s_select) & (~FLD_I2S2_MONOR_RXFIFO_SEL)) | MASK_VAL(FLD_I2S2_MONOR_RXFIFO_SEL, fifo_chn);
            break;
        default:
            break;
        }
    }
}

/**
 *  @brief      This function serves to set i2s input fifo data format.
 *  @param[in]  i2s_select - i2s channel.
 *  @param[in]  fifo_chn   - fifo channel.
 *  @param[in]  ain_mode   - number of channels.
 *  @param[in]  data_width - data bit width.
 *  @return     none.
 */
void audio_set_i2s_fifo_input_mode(audio_i2s_select_e i2s_select, audio_fifo_chn_e fifo_chn, audio_channel_select_e ain_mode, audio_i2s_fifo_wl_e data_width)
{
    if (ain_mode == AUDIO_STEREO) {
        ain_mode += 1;
    }

    switch (fifo_chn) {
    case FIFO0:
        reg_i2s_ain_mode(i2s_select) = (reg_i2s_ain_mode(i2s_select) & (~FLD_I2S_AIN0_MODE)) | MASK_VAL(FLD_I2S_AIN0_MODE, ain_mode + data_width);
        break;
    case FIFO1:
        reg_i2s_ain_mode(i2s_select) = (reg_i2s_ain_mode(i2s_select) & (~FLD_I2S_AIN1_MODE)) | MASK_VAL(FLD_I2S_AIN1_MODE, ain_mode + data_width);
        break;
    case FIFO2:
        reg_i2s_ain_mode(i2s_select) = (reg_i2s_ain_mode(i2s_select) & (~FLD_I2S_AIN2_MODE)) | MASK_VAL(FLD_I2S_AIN2_MODE, ain_mode + data_width);
        break;
    default:
        break;
    }
}

/**
 * @brief     This function set i2s input .
 * @param[in] fifo_chn         - fifo channel selection
 * @param[in] i2s_select       - channel select
 * @param[in] data_mode        - i2s data bit with
 * @param[in] ch               - i2s channel selection
 * @return    none
 */
void audio_set_i2s_input_chn_wl(audio_fifo_chn_e fifo_chn, audio_i2s_select_e i2s_select, audio_i2s_wl_mode_e data_mode, i2s_chn_select_e ch)
{
    audio_channel_select_e chn_mode;
    audio_i2s_wl_mode_e    ain_mode;

    if (ch != I2S_CHANNEL_STEREO) {
        chn_mode = AUDIO_MONO;
    } else {
        chn_mode = AUDIO_STEREO;
    }
    if (data_mode != I2S_BIT_16_DATA) {
        ain_mode = (audio_i2s_wl_mode_e)BIT_20_24_DATA;
    } else {
        ain_mode = (audio_i2s_wl_mode_e)BIT_16_DATA;
    }

    audio_set_i2s_fifo_input_mode(i2s_select, fifo_chn, chn_mode, (audio_i2s_fifo_wl_e)ain_mode);
}

/**
 * @brief This function serves to initialize  input i2s.
 * @param[in] audio_i2s_input - the relevant input struct pointer
 * @return    none
 */
void audio_i2s_input_init(audio_i2s_input_output_t *audio_i2s_input)
{
    if (audio_i2s_input->i2s_ch_sel != I2S_CHANNEL_STEREO) {
        audio_set_i2s_fifo_mono_channel(audio_i2s_input->i2s_select, audio_i2s_input->fifo_chn, (audio_chn_sel_e)audio_i2s_input->i2s_ch_sel);
    }
    audio_rxfifo_en(audio_i2s_input->fifo_chn);
    audio_rxfifo_wptr_en(audio_i2s_input->fifo_chn);
    audio_set_i2s_input_chn_wl(audio_i2s_input->fifo_chn, audio_i2s_input->i2s_select, audio_i2s_input->data_width, audio_i2s_input->i2s_ch_sel);
    audio_data_fifo_input_path_sel(audio_i2s_input->fifo_chn, (audio_mux_ain_e)audio_i2s_input->i2s_select);
}

/**
 *  @brief      This function serves to set  output fifo data format.
 *  @param[in]  fifo_chn   - fifo channel.
 *  @param[in]  ain_mode   - number of channels.
 *  @param[in]  data_width - data bit width.
 *  @return     none.
 *  @attention  sdm and i2s0 share tx fifo0.
 */
void audio_set_i2s_fifo_output_mode(audio_fifo_chn_e fifo_chn, audio_channel_select_e ain_mode, audio_i2s_fifo_wl_e data_width)
{
    if (ain_mode == AUDIO_STEREO) {
        ain_mode += 1;
    }

    switch (fifo_chn) {
    case FIFO0:
        reg_i2s_aout_mode = (reg_i2s_aout_mode & (~FLD_I2S0_AOUT_MODE)) | MASK_VAL(FLD_I2S0_AOUT_MODE, ain_mode + data_width);
        break;
    case FIFO1:
        reg_i2s_aout_mode = (reg_i2s_aout_mode & (~FLD_I2S1_AOUT_MODE)) | MASK_VAL(FLD_I2S1_AOUT_MODE, ain_mode + data_width);
        break;
    case FIFO2:
        reg_i2s_aout_mode = (reg_i2s_aout_mode & (~FLD_I2S2_AOUT_MODE)) | MASK_VAL(FLD_I2S2_AOUT_MODE, ain_mode + data_width);
        break;
    default:
        break;
    }
}

/**
 * @brief     This function set i2s input .
 * @param[in] fifo_chn         - fifo channel selection
 * @param[in] data_mode        - i2s data bit with
 * @param[in] ch               - i2s channel selection
 * @return    none
 */
void audio_set_i2s_output_chn_wl(audio_fifo_chn_e fifo_chn, audio_i2s_wl_mode_e data_mode, i2s_chn_select_e ch)
{
    audio_channel_select_e chn_mode;
    audio_i2s_wl_mode_e    ain_mode;

    if (ch != I2S_CHANNEL_STEREO) {
        chn_mode = AUDIO_MONO;
    } else {
        chn_mode = AUDIO_STEREO;
    }
    if (data_mode != I2S_BIT_16_DATA) {
        ain_mode = (audio_i2s_wl_mode_e)BIT_20_24_DATA;
    } else {
        ain_mode = (audio_i2s_wl_mode_e)BIT_16_DATA;
    }

    audio_set_i2s_fifo_output_mode(fifo_chn, chn_mode, (audio_i2s_fifo_wl_e)ain_mode);
}

/**
 * @brief  This function serves to initialize  output i2s.
 * @param[in] audio_i2s_output - audio_i2s_input_output_t pointer.
 * @return    none.
 */
void audio_i2s_output_init(audio_i2s_input_output_t *audio_i2s_output)
{
    audio_txfifo_en((audio_fifo_chn_e)audio_i2s_output->i2s_select);
    audio_txfifo_wptr_en((audio_fifo_chn_e)audio_i2s_output->i2s_select);
    audio_set_i2s_output_chn_wl(audio_i2s_output->fifo_chn, audio_i2s_output->data_width, audio_i2s_output->i2s_ch_sel);
}

/**
 * @brief  This function serves to configure tdm mode and slot.
 * @param[in] tdm_config - audio_i2s2_tdm_config_t pointer.
 * @return    none.
 */
void audio_i2s2_tdm_config_init(audio_i2s2_tdm_config_t *tdm_config)
{
    reg_i2s2_tdm_cfg = (reg_i2s2_tdm_cfg & (~(FLD_I2S2_TDM_MODE | FLD_I2S_TDM_SLOT))) | MASK_VAL(FLD_I2S2_TDM_MODE, tdm_config->tdm_mode, FLD_I2S_TDM_SLOT, tdm_config->slot_num);
}

/**
 *  @brief      This function serves to set  input fifo data format.
 *  @param[in]  data_width - tdm mode fifo data format.
 *  @return     none.
 */
void audio_set_i2s2_tdm_input_fifo_width(audio_i2s_wl_mode_e data_width)
{
    audio_i2s_fifo_wl_e tdm_data_width;

    if (data_width != I2S_BIT_16_DATA) {
        tdm_data_width = BIT_20_24_DATA;
    } else {
        tdm_data_width = BIT_16_DATA;
    }
    reg_i2s2_tdm_mode_sel = (reg_i2s2_tdm_mode_sel & (~FLD_I2S2_TDM_AIN_MODE)) | MASK_VAL(FLD_I2S2_TDM_AIN_MODE, BIT(tdm_data_width));
}

/**
 * @brief  This function serves to config tdm input interface tdm channel.
 * @param[in] ch   - tdm channel.
 * @return    none.
 */
void audio_i2s2_tdm_input_config(tdm_slot_number_select_e ch)
{
    reg_i2s2_tdm_cfg = (reg_i2s2_tdm_cfg & (~FLD_I2S2_TDM_RX_CH_NUM)) | MASK_VAL(FLD_I2S2_TDM_RX_CH_NUM, ch);
}

/**
 * @brief  This function serves to tdm input initialization.
 * @param[in] input - i2s2 tdm mode input path struct pointer.
 * @return    none.
 */
void audio_i2s2_tdm_input_init(audio_i2s2_tdm_input_output_t *input)
{
    audio_i2s2_tdm_input_config(input->tdm_chn);
    audio_set_i2s2_tdm_input_fifo_width(input->data_width);
    audio_set_dma_rx_burst(input->fifo_chn, input->dma_num, input->burst_size);
}

/**
 *  @brief      This function serves to set  output fifo data format.
 *  @param[in]  data_width - tdm mode fifo data format.
 *  @return     none.
 */
void audio_set_i2s2_tdm_output_fifo_width(audio_i2s_wl_mode_e data_width)
{
    audio_i2s_fifo_wl_e tdm_data_width;

    if (data_width != I2S_BIT_16_DATA) {
        tdm_data_width = BIT_20_24_DATA;
    } else {
        tdm_data_width = BIT_16_DATA;
    }
    reg_i2s2_tdm_mode_sel = (reg_i2s2_tdm_mode_sel & (~FLD_I2S2_TDM_AOUT_MODE)) | MASK_VAL(FLD_I2S2_TDM_AOUT_MODE, BIT(tdm_data_width));
}

/**
 * @brief  This function serves to config tdm output interface tdm channel.
 * @param[in] ch   - tdm channel.
 * @return    none.
 */
void audio_i2s2_tdm_output_config(tdm_slot_number_select_e ch)
{
    reg_i2s2_tdm_cfg = (reg_i2s2_tdm_cfg & (~FLD_I2S2_TDM_TX_CH_NUM)) | MASK_VAL(FLD_I2S2_TDM_TX_CH_NUM, ch);
}

/**
 * @brief  This function serves to tdm output initialization.
 * @param[in] output - i2s2 tdm mode output path struct pointer.
 * @return    none.
 */
void audio_i2s2_tdm_output_init(audio_i2s2_tdm_input_output_t *output)
{
    audio_i2s2_tdm_output_config(output->tdm_chn);
    audio_set_i2s2_tdm_output_fifo_width(output->data_width);
    audio_set_dma_tx_burst(output->fifo_chn, output->dma_num, output->burst_size);
}

/**
 *  @brief      This function serves to set i2s ascl enable.
 *  @param[in]  i2s_select - i2s channel.
 *  @return     none.
 */
void audio_set_i2s_ascl_en(audio_i2s_select_e i2s_select)
{
    audio_ascl_select_e ascl_select;

    if (i2s_select == I2S0) {
        ascl_select = ASCL0;
    } else if (i2s_select == I2S1) {
        ascl_select = ASCL1;
    } else {
        ascl_select = ASCL2;
    }

    audio_set_ascl_format(ascl_select, HIGH_BIT_16);
    audio_ascl_lerp_en(ascl_select);
    BM_CLR(reg_i2s_route(i2s_select), FLD_ASCL_BYPASS);
}

/**
 * @brief     his function serves to sets the align data bit width.
 * @param[in]  i2s_align_mode_select - i2s align mode.
 * @param[in]  audio_channel_select  - number of channels.
 * @param[in]  audio_i2s_wl_mode     - i2s fifo bit width.
 * @return    none.
 */
void audio_set_i2s_align_data_width(i2s_align_mode_select_e i2s_align_mode_select, audio_channel_select_e audio_channel_select, audio_i2s_wl_mode_e audio_i2s_wl_mode)
{
    audio_i2s_fifo_wl_e fifo_wl_mode = BIT_16_DATA;

    if (audio_channel_select == AUDIO_STEREO) {
        audio_channel_select += 1;
    }

    if (audio_i2s_wl_mode != I2S_BIT_16_DATA) {
        fifo_wl_mode = BIT_20_24_DATA;
    }

    i2s_align_mode_select = 4 * (i2s_align_mode_select + 1) + audio_channel_select + fifo_wl_mode;
    reg_i2s_sync_mode_sel = (reg_i2s_sync_mode_sel & (~FLD_I2S_SYNC_MODE)) | MASK_VAL(FLD_I2S_SYNC_MODE, i2s_align_mode_select);
}

/**
 *  @brief      This function serves to i2s align mode.
 *  @param[in]  mode       - i2s align mode.
 *  @return     none.
 */
void audio_set_i2s_align_mode(i2s_align_mode_select_e mode)
{
    unsigned char i2s_align_mode;

    if (mode == I2S0_I2S1_ALIGN) {
        i2s_align_mode = BIT(0) | BIT(1);
    } else if (mode == I2S1_I2S2_ALIGN) {
        i2s_align_mode = BIT(1) | BIT(2);
    } else {
        i2s_align_mode = BIT(0) | BIT(1) | BIT(2);
    }

    reg_i2s_align_cfg = (reg_i2s_align_cfg & (~FLD_I2S_ALIGN_CTRL)) | MASK_VAL(FLD_I2S_ALIGN_CTRL, i2s_align_mode);
}

/**
 *  @brief      This function serves to clear codec's input pop.
 *  @param[in]  t_ms        - cut input codec's data.
 *  @return     none.
 */
void audio_codec_clr_input_pop(unsigned char t_ms)
{
    delay_ms(t_ms);
    audio_mic_mute_dis();
}
