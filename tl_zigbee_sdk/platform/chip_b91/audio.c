/********************************************************************************************************
 * @file    audio.c
 *
 * @brief   This is the source file for B91
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "clock.h"
#include "pwm.h"
#include "stimer.h"


unsigned char audio_rx_dma_chn;
unsigned char audio_tx_dma_chn;

dma_chain_config_t g_audio_tx_dma_list_cfg;
dma_chain_config_t g_audio_rx_dma_list_cfg;

audio_i2s_codec_config_t audio_i2s_codec_config =
    {
        .audio_in_mode         = BIT_16_MONO,
        .audio_out_mode        = BIT_16_MONO_FIFO0,
        .i2s_data_select       = I2S_BIT_16_DATA,
        .codec_data_select     = CODEC_BIT_16_DATA,
        .i2s_codec_m_s_mode    = I2S_M_CODEC_S,
        .in_digital_gain       = CODEC_IN_D_GAIN_0_DB,
        .in_analog_gain        = CODEC_IN_A_GAIN_0_DB,
        .out_digital_gain      = CODEC_OUT_D_GAIN_0_DB,
        .out_analog_gain       = CODEC_OUT_A_GAIN_0_DB,
        .mic_input_mode_select = DIFF_ENDED_INPUT,   //0 single-ended input, 1 differential input
        .dac_output_chn_select = DAC_OUTPUT_L_R_CHN, //0 right and left channel both active ,1 only left channel active
};


audio_i2s_invert_config_t audio_i2s_invert_config = {
    .i2s_lr_clk_invert_select = I2S_LR_CLK_INVERT_DIS,
    .i2s_data_invert_select   = I2S_DATA_INVERT_DIS,
};


dma_config_t audio_dma_rx_config =
    {
        .dst_req_sel    = 0,
        .src_req_sel    = DMA_REQ_AUDIO0_RX, //rx req
        .dst_addr_ctrl  = DMA_ADDR_INCREMENT,
        .src_addr_ctrl  = DMA_ADDR_FIX,
        .dstmode        = DMA_NORMAL_MODE,
        .srcmode        = DMA_HANDSHAKE_MODE,
        .dstwidth       = DMA_CTR_WORD_WIDTH, //must word
        .srcwidth       = DMA_CTR_WORD_WIDTH, //must word
        .src_burst_size = 0,                  //must 0
        .read_num_en    = 0,
        .priority       = 0,
        .write_num_en   = 0,
        .auto_en        = 0, //must 0
};

dma_config_t audio_dma_tx_config =
    {
        .dst_req_sel    = DMA_REQ_AUDIO0_TX,  //tx req
        .src_req_sel    = 0,
        .dst_addr_ctrl  = DMA_ADDR_FIX,
        .src_addr_ctrl  = DMA_ADDR_INCREMENT, //increment
        .dstmode        = DMA_HANDSHAKE_MODE, //handshake
        .srcmode        = DMA_NORMAL_MODE,
        .dstwidth       = DMA_CTR_WORD_WIDTH, //must word
        .srcwidth       = DMA_CTR_WORD_WIDTH, //must word
        .src_burst_size = 0,                  //must 0
        .read_num_en    = 0,
        .priority       = 0,
        .write_num_en   = 0,
        .auto_en        = 0, //must 0
};

/**
 * @brief      This function serves to invert data between R channel and L channel.
 * @param[in]  en - I2S_DATA_INVERT_DIS: L channel ( left channel data left);  I2S_DATA_INVERT_EN(right channel data left)
 * @attention must be set before audio_init().
 * @return     none
 */
void audio_set_mono_chn(audio_data_invert_e en)
{
    audio_i2s_invert_config.i2s_data_invert_select = en;
}

/**
 * @brief      This function serves to invert LR-clk.
 * @param[in]  en -lr_clk phase control(in RJ,LJ or i2s modes),in i2s mode(opposite phasing in  RJ,LJ mode), 0=right channel data when lr_clk high ,1=right channel data when lr_clk low.
 *                                                             in DSP mode(in DSP mode only), DSP mode A/B select,0=DSP mode A ,1=DSP mode B
 * @attention If the left and right channels are both active,there will be a phase difference(about 1 sample) between the left and right channels,invert lr_clk can eliminate the phase difference,but data output channel will invert.
 * @attention must be set before audio_init().
 * @return     none
 */
void audio_invert_i2s_lr_clk(audio_i2s_lr_clk_invert_e en)
{
    audio_i2s_invert_config.i2s_lr_clk_invert_select = en;
}

/**
 * @brief      This function serves to set mic input mode.
 * @param[in]  input_mode - 0 single-ended input, 1 differential input.
 * @attention must be set before audio_init().
 * @return     none
 */
void audio_set_codec_mic_input_mode(audio_input_mode_select_e input_mode)
{
    audio_i2s_codec_config.mic_input_mode_select = input_mode;
}

/**
 *  @brief      This function serves to set in path digital and analog gain  .
 *  @param[in]  d_gain - digital gain value
 *  @param[in]  a_gain - analog  gain value
 *  @return     none
 */
void audio_set_codec_in_path_a_d_gain(codec_in_path_digital_gain_e d_gain, codec_in_path_analog_gain_e a_gain)
{
    audio_set_codec_adc_d_gain(d_gain);
    audio_set_codec_adc_a_gain(a_gain);
}

/**
 *  @brief      This function serves to set out path digital and analog gain  .
 *  @param[in]  d_gain - digital gain value
 *  @param[in]  a_gain - analog  gain value
 *  @return     none
 */
void audio_set_codec_out_path_a_d_gain(codec_out_path_digital_gain_e d_gain, codec_out_path_analog_gain_e a_gain)
{
    audio_set_codec_dac_d_gain(d_gain);
    audio_set_codec_dac_a_gain(a_gain);
}

/**
  * @brief      This function serves to choose which is master to provide clock.
  * @param[in]  m_s - I2S_S_CODEC_M: i2s as slave ,codec as master; I2S_M_CODEC_S: i2s as  master, codec  as slave.
  * @attention must be set before audio_init().
  * @return     none
  */
void audio_set_i2s_codec_m_s(i2s_codec_m_s_mode_e m_s)
{
    audio_i2s_codec_config.i2s_codec_m_s_mode = m_s;
}

/**
 * @brief  This function serves to set dac output channel.
 * @param[in] chn-DAC_OUTPUT_L_R_CHN - right and left channel both active ; DAC_OUTPUT_L_CHN, only left channel active.
 * @return    none
 * @attention must be set before audio_init().
 */
void audio_set_output_chn(audio_output_chn_e chn)
{
    audio_i2s_codec_config.dac_output_chn_select = chn;
}

/**
 *  @brief      This function serves to choose which is master to provide clock.
 *  @param[in]  chn_wl: select word  length and audio channel number
 *  @return     none
 */
void audio_set_chn_wl(audio_channel_wl_mode_e chn_wl)
{
    switch (chn_wl) {
    case MONO_BIT_16:
        audio_i2s_codec_config.audio_in_mode     = BIT_16_MONO;
        audio_i2s_codec_config.audio_out_mode    = BIT_16_MONO_FIFO0;
        audio_i2s_codec_config.i2s_data_select   = I2S_BIT_16_DATA;
        audio_i2s_codec_config.codec_data_select = CODEC_BIT_16_DATA;
        break;

    case MONO_BIT_20:
        audio_i2s_codec_config.audio_in_mode     = BIT_20_OR_24_MONO;
        audio_i2s_codec_config.audio_out_mode    = BIT_20_OR_24_MONO_FIFO0;
        audio_i2s_codec_config.i2s_data_select   = I2S_BIT_20_DATA;
        audio_i2s_codec_config.codec_data_select = CODEC_BIT_20_DATA;
        break;

    case MONO_BIT_24:
        audio_i2s_codec_config.audio_in_mode     = BIT_20_OR_24_MONO;
        audio_i2s_codec_config.audio_out_mode    = BIT_20_OR_24_MONO_FIFO0;
        audio_i2s_codec_config.i2s_data_select   = I2S_BIT_24_DATA;
        audio_i2s_codec_config.codec_data_select = CODEC_BIT_24_DATA;
        break;

    case STEREO_BIT_16:
        audio_i2s_codec_config.audio_in_mode     = BIT_16_STEREO;
        audio_i2s_codec_config.audio_out_mode    = BIT_16_STEREO_FIFO0;
        audio_i2s_codec_config.i2s_data_select   = I2S_BIT_16_DATA;
        audio_i2s_codec_config.codec_data_select = CODEC_BIT_16_DATA;
        break;

    case STEREO_BIT_20:
        audio_i2s_codec_config.audio_in_mode     = BIT_20_OR_24_STEREO;
        audio_i2s_codec_config.audio_out_mode    = BIT_20_OR_24_STEREO_FIFO0;
        audio_i2s_codec_config.i2s_data_select   = I2S_BIT_20_DATA;
        audio_i2s_codec_config.codec_data_select = CODEC_BIT_20_DATA;
        break;

    case STEREO_BIT_24:
        audio_i2s_codec_config.audio_in_mode     = BIT_20_OR_24_STEREO;
        audio_i2s_codec_config.audio_out_mode    = BIT_20_OR_24_STEREO_FIFO0;
        audio_i2s_codec_config.i2s_data_select   = I2S_BIT_24_DATA;
        audio_i2s_codec_config.codec_data_select = CODEC_BIT_24_DATA;
        break;
    }
}

/**
 * @brief     This function selects  pin  for i2s.
 * @param[in]  pin - the selected pin.
 * @return    none
 */
void audio_i2s_set_pin_mux(i2s_pin_e pin)
{
    unsigned char val       = 0;
    unsigned char start_bit = (BIT_LOW_BIT(pin & 0xff) % 4) << 1;
    unsigned char mask      = (unsigned char)~BIT_RNG(start_bit, start_bit + 1);
    if (pin == I2S_BCK_PC3) {
        val = 0;                //function 0
    } else if ((pin == I2S_ADC_LR_PC6) || (pin == I2S_ADC_DAT_PC7) || (pin == I2S_DAC_LR_PC4) || (pin == I2S_DAC_DAT_PC5)) {
        val = 1 << (start_bit); //function 1
    }
    gpio_input_en((gpio_pin_e)pin);
    reg_gpio_func_mux(pin) = (reg_gpio_func_mux(pin) & mask) | val;
    gpio_function_dis(pin);
}

/**
 * @brief     This function configures i2s pin.
 * @param[in] none
 * @return    none
 * @note      GPIO_PC3 -- I2S_BCK_IO
 *            GPIO_PC6 -- I2S_LR_IN_IO
 *            GPIO_PC7 -- I2S_DAT_IN_IO
 *            GPIO_PC4 -- I2S_LR_OUT_IO
 *            GPIO_PC5 -- I2S_DAT_OUT_IO
 */
void audio_i2s_set_pin(void)
{
    audio_i2s_set_pin_mux(I2S_BCK_PC3);
    audio_i2s_set_pin_mux(I2S_ADC_LR_PC6);
    audio_i2s_set_pin_mux(I2S_ADC_DAT_PC7);
    audio_i2s_set_pin_mux(I2S_DAC_LR_PC4);
    audio_i2s_set_pin_mux(I2S_DAC_DAT_PC5);
}

/**
 * @brief      This function serves to set codec active, the bias voltage can only be set after setting the codec active.
 * @return     none
 */
void audio_codec_active(void)
{
    audio_set_codec_clk(1, 16);                                    /* from ppl 192/16=12M */
    audio_clk_en(0, 1);
    reg_audio_codec_vic_ctr = FLD_AUDIO_CODEC_SLEEP_ANALOG;        /* active analog sleep mode */
    while (!(reg_audio_codec_stat_ctr & FLD_AUDIO_CODEC_PON_ACK))
        ;                                                          /* wait codec can be configured */
    addr_audio_codec_cr_ck &= ~FLD_AUDIO_CODEC_MCLK_FREQ;          /* set codec MCLK 12M, if not set, the default value is also 12M */
    BM_CLR(reg_audio_codec_vic_ctr, FLD_AUDIO_CODEC_SLEEP_ANALOG); /* disable sleep mode */
}

/**
 * @brief      This function serves to set amic micbias.
 * @param[in]  en               - POWER_DOWN or POWER_ON.
 * @param[in]  micbias_mode     - micbias output mode.
 * @return     none
 * @note       The interface audio_codec_active() must be called before the bias voltage can be set.
 */
void audio_codec_set_micbias(power_switch_e en, micbias_work_mode_e micbias_mode)
{
    if (en) {
        reg_audio_codec_mic1_ctr = (reg_audio_codec_mic1_ctr & (~(FLD_AUDIO_CODEC_MICBIAS1_SB | FLD_AUDIO_CODEC_MICBIAS1_V))) | MASK_VAL(FLD_AUDIO_CODEC_MICBIAS1_SB, 0, FLD_AUDIO_CODEC_MICBIAS1_V, micbias_mode);
    } else {
        reg_audio_codec_mic1_ctr |= FLD_AUDIO_CODEC_MICBIAS1_SB;
    }
    /* 0x17 <7>:mic bias voltage select(default 0)   0:micbias voltage = 2.08V, 1:micbias voltage = 1.66V
                <5>:pd_micbias(default 1)                0:micbias active,          1:micbias power down
        */
}

/**
 *  @brief      This function serves to set codec supply voltage
 *  @param[in]  volt - the voltage of codec supply.A1 2.8V default,A0 1.8V default.
 *  @return     none
 *
 */
void audio_set_codec_supply(codec_volt_supply_e volt)
{
    if (0xff == g_chip_version) //A0 1.8v default ( BIT(7) - 1: 2.8v 0: 1.8v )
    {
        if (CODEC_2P8V == volt) {
            analog_write_reg8(0x02, analog_read_reg8(0x02) | BIT(7));
        }

        else if (CODEC_1P8V == volt) {
            analog_write_reg8(0x02, analog_read_reg8(0x02) & (~BIT(7)));
        }

    }

    else //A1 2.8v default ( BIT(7) - 1: 1.8v 0: 2.8v )
    {
        if (CODEC_1P8V == volt) {
            analog_write_reg8(0x02, analog_read_reg8(0x02) | BIT(7));
        }

        else if (CODEC_2P8V == volt) {
            analog_write_reg8(0x02, analog_read_reg8(0x02) & (~BIT(7)));
        }
    }
}

/**
 * @brief     This function configures dmic pin.
 * @param[in] pin_gp - the group of dmic pin
 * @return    none
 */
void audio_set_dmic_pin(dmic_pin_group_e pin_gp)
{
    if (pin_gp == DMIC_GROUPB_B2_DAT_B3_B4_CLK) {
        reg_gpio_pad_mul_sel = BIT(2);
        reg_gpio_pb_fuc_h    = reg_gpio_pb_fuc_h & (~BIT_RNG(0, 1));
        reg_gpio_pb_fuc_l    = (reg_gpio_pb_fuc_l & (~BIT_RNG(4, 7)));
        gpio_function_dis(GPIO_PB2 | GPIO_PB3 | GPIO_PB4);
        gpio_input_en(GPIO_PB2);
    } else if (pin_gp == DMIC_GROUPC_C1_DAT_C2_C3_CLK) {
        reg_gpio_pad_mul_sel = BIT(0);
        reg_gpio_pc_fuc_l    = (reg_gpio_pc_fuc_l & (~BIT_RNG(2, 7))) | ((2 << 2) | (2 << 4) | (2 << 6));
        gpio_function_dis(GPIO_PC1 | GPIO_PC2 | GPIO_PC3);
        gpio_input_en(GPIO_PC1);

    } else if (pin_gp == DMIC_GROUPD_D4_DAT_D5_D6_CLK) //can not use in A0
    {
        reg_gpio_pd_fuc_h = (reg_gpio_pd_fuc_h & (~BIT_RNG(0, 5))) | ((1 << 0) | (1 << 2) | (1 << 4));
        gpio_function_dis(GPIO_PD4 | GPIO_PD5 | GPIO_PD6);
        gpio_input_en(GPIO_PD4);
    } else if (pin_gp == DMIC_B2_DAT_B3_CLK) {
        reg_gpio_pad_mul_sel = BIT(2);
        reg_gpio_pb_fuc_l    = (reg_gpio_pb_fuc_l & (~BIT_RNG(4, 7)));
        gpio_function_dis(GPIO_PB2 | GPIO_PB3);
        gpio_input_en(GPIO_PB2);
    } else if (pin_gp == DMIC_C1_DAT_C2_CLK) {
        reg_gpio_pad_mul_sel = BIT(0);
        reg_gpio_pc_fuc_l    = (reg_gpio_pc_fuc_l & (~BIT_RNG(2, 5))) | ((2 << 2) | (2 << 4));
        gpio_function_dis(GPIO_PC1 | GPIO_PC2);
        gpio_input_en(GPIO_PC1);
    } else if (pin_gp == DMIC_D4_DAT_D5_CLK) //can not use in A0
    {
        reg_gpio_pd_fuc_h = (reg_gpio_pd_fuc_h & (~BIT_RNG(0, 3))) | ((1 << 0) | (1 << 2));
        gpio_function_dis(GPIO_PD4 | GPIO_PD5);
        gpio_input_en(GPIO_PD4);
    }
}

/**
 * @brief     This function serves to enable rx_dma channel.
 * @return    none
 */
void audio_rx_dma_en(void)
{
    dma_chn_en(audio_rx_dma_chn);
}

/**
  * @brief     This function serves to disable rx_dma channel.
  * @return    none
  */
void audio_rx_dma_dis(void)
{
    dma_chn_dis(audio_rx_dma_chn);
}

/**
  * @brief     This function serves to enable tx_dma channel.
  * @return    none
  */
void audio_tx_dma_en(void)
{
    dma_chn_en(audio_tx_dma_chn);
}

/**
  * @brief     This function serves to disable dis_dma channel.
  * @return    none
  */
void audio_tx_dma_dis(void)
{
    dma_chn_dis(audio_tx_dma_chn);
}

/**
  * @brief     This function serves to config  rx_dma channel.
  * @param[in] chn          - dma channel
  * @param[in] dst_addr     - This parameter is the first address of the received data buffer, which must be 4 bytes aligned, otherwise the program will enter an exception.
  *                           and the actual buffer size defined by the user needs to be not smaller than the data_len, otherwise there may be an out-of-bounds problem.
  * @param[in] data_len     - This parameter is used to set the size of the received dma and must be set to a multiple of 4. The maximum value that can be set is 0xFFFFFC.
  * @param[in] head_of_list - the head address of dma llp.
  * @return    none
  */
void audio_rx_dma_config(dma_chn_e chn, unsigned short *dst_addr, unsigned int data_len, dma_chain_config_t *head_of_list)
{
    /*DMA is not finished yet, need to disable dma before writing to the dma register.
     * confirmed by qiangkai,modify by minghai.duan(20211025)*/
    audio_rx_dma_chn = chn;
    audio_rx_dma_dis();
    audio_set_rx_buff_len(data_len);
    dma_config(audio_rx_dma_chn, &audio_dma_rx_config);
    dma_set_address(chn, REG_AUDIO_AHB_BASE, (unsigned int)dst_addr);
    dma_set_size(chn, data_len, DMA_WORD_WIDTH);
    reg_dma_llp(chn) = (unsigned int)convert_ram_addr_cpu2bus(head_of_list);
}

/**
 * @brief     This function serves to set rx dma chain transfer
 * @param[in] config_addr - the head of list of llp_pointer.
 * @param[in] llpointer   - the next element of llp_pointer.
 * @param[in] dst_addr    - This parameter is the first address of the received data buffer, which must be 4 bytes aligned, otherwise the program will enter an exception.
 *                          and the actual buffer size defined by the user needs to be not smaller than the data_len, otherwise there may be an out-of-bounds problem.
 * @param[in] data_len    - This parameter is used to set the size of the received dma and must be set to a multiple of 4. The maximum value that can be set is 0xFFFFFC.
 * @return    none
 */
void audio_rx_dma_add_list_element(dma_chain_config_t *config_addr, dma_chain_config_t *llpointer, unsigned short *dst_addr, unsigned int data_len)
{
    config_addr->dma_chain_ctl      = reg_dma_ctrl(audio_rx_dma_chn) | BIT(0);
    config_addr->dma_chain_src_addr = REG_AUDIO_AHB_BASE;
    config_addr->dma_chain_dst_addr = (unsigned int)convert_ram_addr_cpu2bus(dst_addr);
    config_addr->dma_chain_data_len = dma_cal_size(data_len, 4);
    config_addr->dma_chain_llp_ptr  = (unsigned int)convert_ram_addr_cpu2bus(llpointer);
}

/**
 * @brief     This function serves to config  tx_dma channel.
 * @param[in] chn          - dma channel
 * @param[in] src_addr     - the address of source
 * @param[in] data_len     - the length of dma rx size by byte
 * @param[in] head_of_list - the head address of dma llp.
 * @return    none
 * @note      src_addr : must be aligned by word (4 bytes), otherwise the program will enter an exception
 */
void audio_tx_dma_config(dma_chn_e chn, unsigned short *src_addr, unsigned int data_len, dma_chain_config_t *head_of_list)
{
    /*DMA is not finished yet, need to disable dma before writing to the dma register.
    * confirmed by qiangkai,modify by minghai.duan(20211025)*/
    audio_tx_dma_chn = chn;
    audio_tx_dma_dis();
    audio_set_tx_buff_len(data_len);
    dma_config(audio_tx_dma_chn, &audio_dma_tx_config);
    dma_set_address(chn, (unsigned int)src_addr, REG_AUDIO_AHB_BASE);
    dma_set_size(chn, data_len, DMA_WORD_WIDTH);
    reg_dma_llp(chn) = (unsigned int)convert_ram_addr_cpu2bus(head_of_list);
}

/**
 * @brief     This function serves to set tx dma chain transfer
 * @param[in] config_addr - the head of list of llp_pointer.
 * @param[in] llpointer   - the next element of llp_pointer.
 * @param[in] src_addr    - the address of source
 * @param[in] data_len    - the length of dma size by byte.
 * @return    none
 * @note      src_addr : must be aligned by word (4 bytes), otherwise the program will enter an exception
 */
void audio_tx_dma_add_list_element(dma_chain_config_t *config_addr, dma_chain_config_t *llpointer, unsigned short *src_addr, unsigned int data_len)
{
    config_addr->dma_chain_ctl      = reg_dma_ctrl(audio_tx_dma_chn) | BIT(0);
    config_addr->dma_chain_src_addr = (unsigned int)convert_ram_addr_cpu2bus(src_addr);
    config_addr->dma_chain_dst_addr = REG_AUDIO_AHB_BASE;
    config_addr->dma_chain_data_len = dma_cal_size(data_len, 4);
    config_addr->dma_chain_llp_ptr  = (unsigned int)convert_ram_addr_cpu2bus(llpointer);
}

/**
 * @brief     This function serves to  initialize audio by mc
 * @param[in] flow_mode  - select input out flow mode
 * @param[in] rate       - audio sampling rate.
 * @param[in] channel_wl - word length and channel number.
 * @return    none
 */
void audio_init(audio_flow_mode_e flow_mode, audio_sample_rate_e rate, audio_channel_wl_mode_e channel_wl)
{
    audio_set_chn_wl(channel_wl);
    audio_set_codec_clk(1, 16); //from ppl 192/16=12M
    audio_mux_config(CODEC_I2S, audio_i2s_codec_config.audio_in_mode, audio_i2s_codec_config.audio_in_mode, audio_i2s_codec_config.audio_out_mode);
    audio_i2s_config(I2S_I2S_MODE, audio_i2s_codec_config.i2s_data_select, audio_i2s_codec_config.i2s_codec_m_s_mode, &audio_i2s_invert_config);
    audio_set_i2s_clock(rate, AUDIO_RATE_EQUAL, 0);
    audio_clk_en(1, 1);
    reg_audio_codec_vic_ctr = FLD_AUDIO_CODEC_SLEEP_ANALOG; //active analog sleep mode
    while (!(reg_audio_codec_stat_ctr & FLD_AUDIO_CODEC_PON_ACK))
        ;                                                   //wait codec can be configured
    addr_audio_codec_cr_ck &= ~FLD_AUDIO_CODEC_MCLK_FREQ;   //set codec MCLK 12M, if not set, the default value is also 12M
    if (flow_mode < BUF_TO_LINE_OUT) {
        audio_codec_adc_config(audio_i2s_codec_config.i2s_codec_m_s_mode, (flow_mode % 3), rate, audio_i2s_codec_config.codec_data_select, MCU_WREG);
    }

    if (flow_mode > LINE_IN_TO_BUF) {
        audio_codec_dac_config(audio_i2s_codec_config.i2s_codec_m_s_mode, rate, audio_i2s_codec_config.codec_data_select, MCU_WREG);
    }
    audio_data_fifo0_path_sel(I2S_DATA_IN_FIFO, I2S_OUT);
}

/**
 * @brief     This function serves to read data from codec register.
 * @param[in] addr: the address of codec register
 * @attention This function only supports internal codec.
 * @return    none
 */
unsigned char audio_i2c_codec_read(unsigned char addr)
{
    unsigned char codec_addr_rdat[2] = {0x00, 0x00};
    codec_addr_rdat[0]               = addr << 1;
    i2c_master_write_read(0x34 << 1, &codec_addr_rdat[0], 1, &codec_addr_rdat[1], 1);
    return codec_addr_rdat[1];
}

/**
 * @brief     This function serves to write data to  codec register.
 * @param[in] addr: the address of codec register
 * @attention This function only supports internal codec.
 * @return    none
 */
void audio_i2c_codec_write(unsigned char addr, unsigned char wdat)
{
    unsigned char codec_addr_wdat[2] = {0x00, 0x00};
    codec_addr_wdat[0]               = addr << 1;
    codec_addr_wdat[1]               = wdat;
    i2c_master_write(0x34 << 1, codec_addr_wdat, 2);
}

/**
 * @brief      This function serves to enable i2c master for codec i2c slave .
 * @param[in]  none.
 * @return     none.
 */
void audio_i2c_init(codec_type_e codec_type, i2c_sda_pin_e sda_pin, i2c_scl_pin_e scl_pin)
{
    i2c_master_init();
    i2c_set_master_clk((unsigned char)(sys_clk.pclk * 1000 * 1000 / (4 * 200000))); //set i2c frequency 200K.
    if (codec_type == INNER_CODEC) {
        reg_audio_i2c_mode = 0x05;                                                  //codec config by i2c
        reg_audio_i2c_addr = 0x34;
    } else if (codec_type == EXT_CODEC) {
        i2c_set_pin(sda_pin, scl_pin);
    }
}

/**
 * @brief     This function serves to  initialize audio by i2c
 * @param[in] flow_mode  - select input out flow mode
 * @param[in] rate       - audio sampling rate.
 * @param[in] channel_wl - word length and channel number.
 * @return    none
 */
void audio_init_i2c(audio_flow_mode_e flow_mode, audio_sample_rate_e rate, audio_channel_wl_mode_e channel_wl)
{
    audio_set_chn_wl(channel_wl);
    audio_set_codec_clk(1, 16); ////from ppl 192/16=12M
    audio_mux_config(CODEC_I2S, audio_i2s_codec_config.audio_in_mode, audio_i2s_codec_config.audio_in_mode, audio_i2s_codec_config.audio_out_mode);
    audio_i2s_config(I2S_I2S_MODE, audio_i2s_codec_config.i2s_data_select, audio_i2s_codec_config.i2s_codec_m_s_mode, &audio_i2s_invert_config);
    audio_set_i2s_clock(rate, AUDIO_RATE_EQUAL, 0);
    audio_clk_en(1, 1);
    audio_i2c_init(INNER_CODEC, 0, 0);
    audio_i2c_codec_write(addr_audio_codec_vic_ctr, FLD_AUDIO_CODEC_SLEEP_ANALOG); //active analog sleep mode
    while (!(audio_i2c_codec_read(addr_audio_codec_stat_ctr) & FLD_AUDIO_CODEC_PON_ACK))
        ;                                                                          //wait codec can be configured
    if (flow_mode < BUF_TO_LINE_OUT) {
        audio_codec_adc_config(audio_i2s_codec_config.i2s_codec_m_s_mode, (flow_mode % 3), rate, audio_i2s_codec_config.codec_data_select, I2C_WREG);
    }
    if (flow_mode > LINE_IN_TO_BUF) {
        audio_codec_dac_config(audio_i2s_codec_config.i2s_codec_m_s_mode, rate, audio_i2s_codec_config.codec_data_select, I2C_WREG);
    }
    audio_data_fifo0_path_sel(I2S_DATA_IN_FIFO, I2S_OUT);
}

/**
 * @brief     This function serves to config codec for dac.
 * @param[in] mode        - select i2s as master or slave
 * @param[in] rate        - audio sampling rate
 * @param[in] data_select - codec dac word length
 * @param[in] wreg_mode   - mcu or i2c config codec
 * @return    none
 */
void audio_codec_dac_config(i2s_codec_m_s_mode_e mode, audio_sample_rate_e rate, codec_data_select_e data_select, codec_wreg_mode_e wreg_mode)
{
    if (wreg_mode == MCU_WREG) {
        audio_set_codec_dac_mute();                                                                  //dac mute
        BM_CLR(reg_audio_codec_vic_ctr, FLD_AUDIO_CODEC_SB | FLD_AUDIO_CODEC_SB_ANALOG);             //disable sb_analog,disable global standby
        delay_ms(50);                                                                                //for eliminate pop
        BM_CLR(reg_audio_codec_vic_ctr, FLD_AUDIO_CODEC_SLEEP_ANALOG);                               //disable sleep mode

        if (audio_i2s_codec_config.dac_output_chn_select) {
            BM_CLR(reg_audio_codec_dac_ctr, FLD_AUDIO_CODEC_DAC_SB);                                 //active DAC power
            BM_SET(reg_audio_codec_dac_ctr, FLD_AUDIO_CODEC_DAC_LEFT_ONLY);                          //active left channel only
        } else {
            BM_CLR(reg_audio_codec_dac_ctr, FLD_AUDIO_CODEC_DAC_SB | FLD_AUDIO_CODEC_DAC_LEFT_ONLY); //active DAC power,active left and right channel
        }


        /* data word length ,interface master/slave selection, audio interface protocol selection ,active dac audio interface*/
        reg_audio_codec_dac_itf_ctr = MASK_VAL(FLD_AUDIO_CODEC_FORMAT, CODEC_I2S_MODE, FLD_AUDIO_CODEC_DAC_ITF_SB, CODEC_ITF_AC, FLD_AUDIO_CODEC_SLAVE, mode, FLD_AUDIO_CODEC_WL, data_select);

        /*disable DAC digital gain coupling, Left channel DAC digital gain*/
        audio_set_codec_dac_d_gain(audio_i2s_codec_config.out_digital_gain);
        audio_set_codec_dac_a_gain(audio_i2s_codec_config.out_analog_gain);
        reg_audio_codec_dac_freq_ctr = (FLD_AUDIO_CODEC_DAC_FREQ & (rate == AUDIO_ADC_16K_DAC_48K ? AUDIO_48K : rate));

        if (mode) {
            while (!(reg_audio_codec_stat_ctr & FLD_AUDIO_CODEC_DAC_LOCKED))
                ;                     //wait codec DAC can be lockedjust for codec as slave
        }

        audio_set_codec_dac_unmute(); /*dac unmute*/

    }

    else if (wreg_mode == I2C_WREG) {
        /*active DAC power,active left and right channel,dac mute*/
        audio_i2c_codec_write(addr_audio_codec_dac_ctr, MASK_VAL(FLD_AUDIO_CODEC_DAC_SB, 0, FLD_AUDIO_CODEC_DAC_LEFT_ONLY, 0, FLD_AUDIO_CODEC_DAC_SOFT_MUTE, 1));


        /*disable sleep mode ,disable sb_analog,disable global standby*/
        audio_i2c_codec_write(addr_audio_codec_vic_ctr, MASK_VAL(FLD_AUDIO_CODEC_SB, 0, FLD_AUDIO_CODEC_SB_ANALOG, 0, FLD_AUDIO_CODEC_SLEEP_ANALOG, 0));

        /*data word length ,interface master/slave selection, audio interface protocol selection ,active dac audio interface */
        audio_i2c_codec_write(addr_audio_codec_dac_itf_ctr, MASK_VAL(FLD_AUDIO_CODEC_FORMAT, CODEC_I2S_MODE, FLD_AUDIO_CODEC_DAC_ITF_SB, CODEC_ITF_AC, FLD_AUDIO_CODEC_SLAVE, mode, FLD_AUDIO_CODEC_WL, data_select));

        /* disable DAC digital gain coupling, Left channel DAC digital gain */
        audio_i2c_codec_write(addr_audio_codec_dacl_gain, MASK_VAL(FLD_AUDIO_CODEC_DAC_LRGOD, 0, FLD_AUDIO_CODEC_DAC_GODL, audio_i2s_codec_config.out_digital_gain));


        audio_i2c_codec_write(addr_audio_codec_dacr_gain, MASK_VAL(FLD_AUDIO_CODEC_DAC_GODR, audio_i2s_codec_config.out_digital_gain)); /* Right channel DAC digital gain */


        /* disable Headphone gain coupling, set Left channel HP amplifier gain */
        audio_i2c_codec_write(addr_audio_codec_hpl_gain, MASK_VAL(FLD_AUDIO_CODEC_HPL_LRGO, 0, FLD_AUDIO_CODEC_HPL_GOL, audio_i2s_codec_config.out_analog_gain));

        audio_i2c_codec_write(addr_audio_codec_hpr_gain, MASK_VAL(FLD_AUDIO_CODEC_HPR_GOR, audio_i2s_codec_config.out_analog_gain)); /*  Right channel HP amplifier gain programming*/

        audio_i2c_codec_write(addr_audio_codec_dac_freq_ctr, (FLD_AUDIO_CODEC_DAC_FREQ & rate));
        while (!(audio_i2c_codec_read(addr_audio_codec_stat_ctr) & FLD_AUDIO_CODEC_DAC_LOCKED))
            ; //wait codec adc locked
        /*dac mute*/
        audio_i2c_codec_write(addr_audio_codec_dac_ctr, MASK_VAL(FLD_AUDIO_CODEC_DAC_SB, 0, FLD_AUDIO_CODEC_DAC_LEFT_ONLY, 0, FLD_AUDIO_CODEC_DAC_SOFT_MUTE, 0));
    }
}

/**
 * @brief     This function serves to config codec for adc.
 * @param[in] mode        - select i2s as master or slave
 * @param[in] in_mode     - line_in/amic/dmic input mode select
 * @param[in] rate        - audio sampling rate
 * @param[in] data_select - codec adc word length
 * @param[in] wreg_mode   - mcu or i2c config codec
 * @return    none
 */
void audio_codec_adc_config(i2s_codec_m_s_mode_e mode, audio_input_mode_e in_mode, audio_sample_rate_e rate, codec_data_select_e data_select, codec_wreg_mode_e wreg_mode)
{
    if (wreg_mode == MCU_WREG) {
        audio_set_codec_adc_mute();                                                                                             /*adc mute*/
        if ((audio_i2s_codec_config.audio_in_mode == BIT_16_MONO) || ((audio_i2s_codec_config.audio_in_mode == BIT_20_OR_24_MONO))) {
            if (!audio_i2s_invert_config.i2s_data_invert_select) {
                reg_audio_codec_adc12_ctr = (reg_audio_codec_adc12_ctr & ~(FLD_AUDIO_CODEC_ADC1_SB)) | FLD_AUDIO_CODEC_ADC2_SB; /*active adc1 channel,mono.*/

            } else {
                reg_audio_codec_adc12_ctr = (reg_audio_codec_adc12_ctr & ~(FLD_AUDIO_CODEC_ADC2_SB)) | FLD_AUDIO_CODEC_ADC1_SB; /*active adc2 channel,mono.*/
            }
        } else {
            BM_CLR(reg_audio_codec_adc12_ctr, FLD_AUDIO_CODEC_ADC1_SB | FLD_AUDIO_CODEC_ADC2_SB);                               /*active adc1 and adc2  channel*/
        }
        BM_CLR(reg_audio_codec_vic_ctr, FLD_AUDIO_CODEC_SB | FLD_AUDIO_CODEC_SB_ANALOG | FLD_AUDIO_CODEC_SLEEP_ANALOG);         /*disable sleep mode ,disable sb_analog,disable global standby*/

        if (in_mode == AMIC_INPUT) {
            reg_audio_dmic_12 &= ~(FLD_AUDIO_CODEC_ADC_DMIC_SEL2 | FLD_AUDIO_CODEC_ADC_DMIC_SEL1);
            /*Microphone 1 input selection ,Microphone biasing active,Single-ended input,MICBIAS1 output=2.08V,*/
            reg_audio_codec_mic1_ctr = MASK_VAL(FLD_AUDIO_CODEC_MIC1_SEL, 0, FLD_AUDIO_CODEC_MICBIAS1_SB, 0, FLD_AUDIO_CODEC_MIC_DIFF1, audio_i2s_codec_config.mic_input_mode_select, FLD_AUDIO_CODEC_MICBIAS1_V, 0);
            /*Microphone 2 input selection,Single-ended input*/
            reg_audio_codec_mic2_ctr = MASK_VAL(FLD_AUDIO_CODEC_MIC2_SEL, 0, FLD_AUDIO_CODEC_MIC_DIFF2, audio_i2s_codec_config.mic_input_mode_select);


            audio_set_codec_adc_a_gain(audio_i2s_codec_config.in_analog_gain);
        } else if (in_mode == DMIC_INPUT) {
            reg_audio_dmic_12 = MASK_VAL(FLD_AUDIO_CODEC_ADC_DMIC_SEL2, 1, FLD_AUDIO_CODEC_ADC_DMIC_SEL1, 1, FLD_AUDIO_CODEC_DMIC2_SB, CODEC_ITF_AC, FLD_AUDIO_CODEC_DMIC1_SB, CODEC_ITF_AC);
        }

        else if (in_mode == LINE_INPUT) {
            reg_audio_dmic_12 &= ~(FLD_AUDIO_CODEC_ADC_DMIC_SEL2 | FLD_AUDIO_CODEC_ADC_DMIC_SEL1);
            reg_audio_codec_mic1_ctr = MASK_VAL(FLD_AUDIO_CODEC_MIC1_SEL, 0, FLD_AUDIO_CODEC_MICBIAS1_SB, 0, FLD_AUDIO_CODEC_MIC_DIFF1, audio_i2s_codec_config.mic_input_mode_select, FLD_AUDIO_CODEC_MICBIAS1_V, 0);

            reg_audio_codec_mic2_ctr = MASK_VAL(FLD_AUDIO_CODEC_MIC2_SEL, 0, FLD_AUDIO_CODEC_MIC_DIFF2, audio_i2s_codec_config.mic_input_mode_select);

            audio_set_codec_adc_a_gain(audio_i2s_codec_config.in_analog_gain);
        }
        audio_set_codec_adc_d_gain(audio_i2s_codec_config.in_digital_gain);
        /*data word length ,interface master/slave selection, audio interface protocol selection  */
        reg_audio_codec_adc_itf_ctr = MASK_VAL(FLD_AUDIO_CODEC_FORMAT, CODEC_I2S_MODE, FLD_AUDIO_CODEC_SLAVE, mode, FLD_AUDIO_CODEC_WL, data_select);
        /*audio adc interface active*/
        BM_CLR(reg_audio_codec_adc2_ctr, FLD_AUDIO_CODEC_ADC12_SB);

        /*  adc high pass filter active, set adc sample rate   */
        reg_audio_codec_adc_freq_ctr = MASK_VAL(FLD_AUDIO_CODEC_ADC12_HPF_EN, 1, FLD_AUDIO_CODEC_ADC_FREQ, rate == AUDIO_ADC_16K_DAC_48K ? AUDIO_16K : rate);
        if (mode) {
            while (!(reg_audio_codec_stat_ctr & FLD_AUDIO_CODEC_ADC12_LOCKED))
                ;                     //wait codec adc can be lockedjust for codec as slave
        }

        audio_set_codec_adc_unmute(); /*adc unmute*/
    }

    else if (wreg_mode == I2C_WREG) {
        /*active adc0 and adc1  channel, if mono only active adc1,adc mute*/
        audio_i2c_codec_write(addr_audio_codec_adc12_ctr, MASK_VAL(FLD_AUDIO_CODEC_ADC1_SB, 0, FLD_AUDIO_CODEC_ADC2_SB, 0, FLD_AUDIO_CODEC_ADC12_SOFT_MUTE, 1));

        /*disable sleep mode ,disable sb_analog,disable global standby*/
        audio_i2c_codec_write(addr_audio_codec_vic_ctr, MASK_VAL(FLD_AUDIO_CODEC_SB, 0, FLD_AUDIO_CODEC_SB_ANALOG, 0, FLD_AUDIO_CODEC_SLEEP_ANALOG, 0));


        if (in_mode == AMIC_INPUT) {
            /*Microphone 1 input selection ,Microphone biasing active,Single-ended input,MICBIAS1 output=2.08V,*/
            audio_i2c_codec_write(addr_audio_codec_mic1_ctr, MASK_VAL(FLD_AUDIO_CODEC_MIC1_SEL, 0, FLD_AUDIO_CODEC_MICBIAS1_SB, 0, FLD_AUDIO_CODEC_MIC_DIFF1, 0, FLD_AUDIO_CODEC_MICBIAS1_V, 0));

            /*Microphone 2 input selection,Single-ended input*/
            audio_i2c_codec_write(addr_audio_codec_mic2_ctr, MASK_VAL(FLD_AUDIO_CODEC_MIC2_SEL, 0, FLD_AUDIO_CODEC_MIC_DIFF2, 0));

            /*analog 0/4/8/12/16/20 dB boost gain*/
            audio_i2c_codec_write(addr_audio_codec_mic_l_R_gain, MASK_VAL(FLD_AUDIO_CODEC_AMIC_L_GAIN, audio_i2s_codec_config.in_analog_gain, FLD_AUDIO_CODEC_AMIC_R_GAIN, audio_i2s_codec_config.in_analog_gain));

        } else if (in_mode == DMIC_INPUT) {
            audio_i2c_codec_write(addr_audio_dmic_12, MASK_VAL(FLD_AUDIO_CODEC_ADC_DMIC_SEL2, 1, FLD_AUDIO_CODEC_ADC_DMIC_SEL1, 1, FLD_AUDIO_CODEC_DMIC2_SB, CODEC_ITF_AC, FLD_AUDIO_CODEC_DMIC1_SB, CODEC_ITF_AC));
        }

        else if (in_mode == LINE_INPUT) {
            /*analog 0/4/8/12/16/20 dB boost gain*/
            audio_i2c_codec_write(addr_audio_codec_mic_l_R_gain, MASK_VAL(FLD_AUDIO_CODEC_AMIC_L_GAIN, audio_i2s_codec_config.in_analog_gain, FLD_AUDIO_CODEC_AMIC_R_GAIN, audio_i2s_codec_config.in_analog_gain));
        }

        /*0db~43db  1db step ,digital programmable gain*/
        audio_i2c_codec_write(addr_audio_adc1_gain, MASK_VAL(FLD_AUDIO_CODEC_ADC_LRGID, 1, FLD_AUDIO_CODEC_ADC_GID1, audio_i2s_codec_config.in_digital_gain));


        audio_i2c_codec_write(addr_audio_codec_adc_itf_ctr, MASK_VAL(FLD_AUDIO_CODEC_FORMAT, CODEC_I2S_MODE, FLD_AUDIO_CODEC_SLAVE, mode, FLD_AUDIO_CODEC_WL, data_select));

        audio_i2c_codec_write(addr_audio_codec_adc2_ctr, ~FLD_AUDIO_CODEC_ADC12_SB); /*audio adc interface active*/


        /*  adc high pass filter active, set adc sample rate   */
        audio_i2c_codec_write(addr_audio_codec_adc_freq_ctr, MASK_VAL(FLD_AUDIO_CODEC_ADC12_HPF_EN, 1, FLD_AUDIO_CODEC_ADC_FREQ, rate));

        while (!(audio_i2c_codec_read(addr_audio_codec_stat_ctr) & FLD_AUDIO_CODEC_ADC12_LOCKED))
            ; //wait codec adc locked
        /*dac mute*/
        audio_i2c_codec_write(addr_audio_codec_adc12_ctr, MASK_VAL(FLD_AUDIO_CODEC_ADC1_SB, 0, FLD_AUDIO_CODEC_ADC2_SB, 0, FLD_AUDIO_CODEC_ADC12_SOFT_MUTE, 0));
    }
}

/**
 * @brief     This function serves to set data path.
 * @param[in] audio_flow    - audio flow  select
 * @param[in] ain0_mode     - fifo0 input mode select
 * @param[in] ain1_mode     - fifo1 input mode select
 * @param[in] i2s_aout_mode - fifo output source select
 * @return    none
 */
void audio_mux_config(audio_flow_e audio_flow, audio_in_mode_e ain0_mode, audio_in_mode_e ain1_mode, audio_out_mode_e i2s_aout_mode)
{
    reg_audio_ctrl |= audio_flow;
    reg_audio_tune = MASK_VAL(FLD_AUDIO_I2S_I2S_AIN0_COME, ain0_mode, FLD_AUDIO_I2S_I2S_AIN1_COME, ain1_mode, FLD_AUDIO_I2S_I2S_AOUT_COME, i2s_aout_mode);
}

/**
 * @brief     This function serves to config interface, word length, and m/s .
 * @param[in] i2s_format - interface protocol
 * @param[in] wl         - audio data word length
 * @param[in] m_s        - select i2s as master or slave
 * @param[in] i2s_config_t - the prt of i2s_config_t that configure i2s lr_clk phase and lr_clk swap.
 *  i2s_config_t->i2s_lr_clk_invert_select-lr_clk phase control(in RJ,LJ or i2s modes),in i2s mode(opposite phasing in  RJ,LJ mode), 0=right channel data when lr_clk high ,1=right channel data when lr_clk low.
 *                                                                                     in DSP mode(in DSP mode only), DSP mode A/B select,0=DSP mode A ,1=DSP mode B.
 *            i2s_config_t->i2s_data_invert_select - 0=left channel data left,1=right channel data left.
 * @attention:If the left and right channels are both active in i2s mode,there will be a phase difference(about 1 sample) between the left and right channels,you can set i2s_lr_clk_invert_select=1 to eliminate the phase difference,
 * but data output channel will be inverted,you can also set i2s_config_t->i2s_data_invert_select=1 to recovery it.
 * @return    none
 */
void audio_i2s_config(i2s_mode_select_e i2s_format, i2s_data_select_e wl, i2s_codec_m_s_mode_e m_s, audio_i2s_invert_config_t *i2s_config_t)
{
    reg_i2s_cfg = MASK_VAL(FLD_AUDIO_I2S_FORMAT, i2s_format, FLD_AUDIO_I2S_WL, wl, FLD_AUDIO_I2S_LRP, i2s_config_t->i2s_lr_clk_invert_select, FLD_AUDIO_I2S_LRSWAP, i2s_config_t->i2s_data_invert_select, FLD_AUDIO_I2S_ADC_DCI_MS, m_s, FLD_AUDIO_I2S_DAC_DCI_MS, m_s);
}

audio_i2s_clk_config_t audio_i2s_8k_config =
    {
        .i2s_clk_step      = 8,   //set i2s clk step
        .i2s_clk_mode      = 125, //set i2s clk mode,set i2s clk=192M*(8/125)= 12.288M
        .i2s_bclk_div      = 12,  //12.288M/(2*12) = 512K bclk
        .i2s_lrclk_adc_div = 64,  //adc sample rate =512K/64 = 8k
        .i2s_lrclk_dac_div = 64,  //dac sample rate =512K/64 = 8k
};

audio_i2s_clk_config_t audio_i2s_16k_config =
    {
        .i2s_clk_step      = 8,   //set i2s clk step
        .i2s_clk_mode      = 125, //set i2s clk mode,set i2s clk=192M*(8/125)= 12.288M
        .i2s_bclk_div      = 6,   //12.288M/(2*6) = 1.024M bclk
        .i2s_lrclk_adc_div = 64,  //adc sample rate =1.024M/64 = 16k
        .i2s_lrclk_dac_div = 64,  //dac sample rate =1.024M/64 = 16k
};

audio_i2s_clk_config_t audio_i2s_24k_config =
    {
        .i2s_clk_step      = 8,   //set i2s clk step
        .i2s_clk_mode      = 125, //set i2s clk mode,set i2s clk=192M*(8/125)= 12.288M
        .i2s_bclk_div      = 4,   //12.288M/(2*4) = 1.536M bclk
        .i2s_lrclk_adc_div = 64,  //adc sample rate =1.536M/64 = 24k
        .i2s_lrclk_dac_div = 64,  //dac sample rate =1.536M/64 = 24k
};

audio_i2s_clk_config_t audio_i2s_32k_config =
    {
        .i2s_clk_step      = 8,   //set i2s clk step
        .i2s_clk_mode      = 125, //set i2s clk mode,set i2s clk=192M*(8/125)= 12.288M
        .i2s_bclk_div      = 3,   //12.288M/(2*3) = 2.048M bclk
        .i2s_lrclk_adc_div = 64,  //adc sample rate =2.048M/64 = 32k
        .i2s_lrclk_dac_div = 64,  //dac sample rate =2.048M/64 = 32k
};

audio_i2s_clk_config_t audio_i2s_192k_config =
    {
        .i2s_clk_step      = 8,   //set i2s clk step
        .i2s_clk_mode      = 125, //set i2s clk mode,i2s clk=192M*(8/125)= 12.288M
        .i2s_bclk_div      = 1,   //12.288M/(1*2) = 6.144M bclk
        .i2s_lrclk_adc_div = 32,  //adc sample rate =6.144M/32 =192k
        .i2s_lrclk_dac_div = 32,  //dac sample rate =6.144M/32 =192k
};
audio_i2s_clk_config_t audio_i2s_adc_16k_dac_48k_config =
    {
        .i2s_clk_step      = 2,   //set i2s clk step
        .i2s_clk_mode      = 125, //set i2s clk mode,i2s clk=192M*(2/125)= 3.072 M
        .i2s_bclk_div      = 0,   //3.072M/(1) = 3.072M bclk
        .i2s_lrclk_adc_div = 192, //adc sample rate =3.072M/192 = 16k
        .i2s_lrclk_dac_div = 64,  //dac sample rate =3.072M/64  = 48k
};

audio_i2s_clk_config_t audio_i2s_48k_config[AUDIO_MATCH_SIZE] =
    {
        {
         //48000
            .i2s_clk_step      = 2,
         .i2s_clk_mode      = 125,
         .i2s_bclk_div      = 0,
         .i2s_lrclk_adc_div = 64,
         .i2s_lrclk_dac_div = 64,
         },

        {
         //48004
            .i2s_clk_step      = 3,
         .i2s_clk_mode      = 169,
         .i2s_bclk_div      = 0,
         .i2s_lrclk_adc_div = 71,
         .i2s_lrclk_dac_div = 71,
         },

        {
         //48012.0
            .i2s_clk_step      = 4,
         .i2s_clk_mode      = 129,
         .i2s_bclk_div      = 0,
         .i2s_lrclk_adc_div = 124,
         .i2s_lrclk_dac_div = 124,
         },

        {
         //47994.0
            .i2s_clk_step      = 2,
         .i2s_clk_mode      = 63,
         .i2s_bclk_div      = 0,
         .i2s_lrclk_adc_div = 127,
         .i2s_lrclk_dac_div = 127,
         },

        {
         //47985.0
            .i2s_clk_step      = 4,
         .i2s_clk_mode      = 165,
         .i2s_bclk_div      = 0,
         .i2s_lrclk_adc_div = 97,
         .i2s_lrclk_dac_div = 97,
         },
};

audio_i2s_clk_config_t audio_i2s_44k1_config[AUDIO_MATCH_SIZE] =
    {
        {
         //44099.9
            .i2s_clk_step      = 8,
         .i2s_clk_mode      = 215,
         .i2s_bclk_div      = 0,
         .i2s_lrclk_adc_div = 162,
         .i2s_lrclk_dac_div = 162,
         },

        {
         //44110.2
            .i2s_clk_step      = 11,
         .i2s_clk_mode      = 228,
         .i2s_bclk_div      = 0,
         .i2s_lrclk_adc_div = 210,
         .i2s_lrclk_dac_div = 210,
         },

        {
         //44117.6
            .i2s_clk_step      = 5,
         .i2s_clk_mode      = 170,
         .i2s_bclk_div      = 0,
         .i2s_lrclk_adc_div = 128,
         .i2s_lrclk_dac_div = 128,
         },

        {
         //44094.4
            .i2s_clk_step      = 7,
         .i2s_clk_mode      = 254,
         .i2s_bclk_div      = 0,
         .i2s_lrclk_adc_div = 120,
         .i2s_lrclk_dac_div = 120,
         },

        {
         //44081.6
            .i2s_clk_step      = 9,
         .i2s_clk_mode      = 245,
         .i2s_bclk_div      = 0,
         .i2s_lrclk_adc_div = 160,
         .i2s_lrclk_dac_div = 160,
         },
};

/**
 * @brief     This function serves to set i2s clock and audio sampling rate when i2s as master.
 * @param[in] audio_rate - audio sampling rate
 * @param[in] match      - the match of audio rate.
 * @param[in] match_en   - initial must 0, then change rate must 1
 * @return    none
 * @attention i2s clock  divider from pll,sampling rate calculation is based on pll=192M,so pll must be 192M
 */
_attribute_ram_code_sec_noinline_ void audio_set_i2s_clock(audio_sample_rate_e audio_rate, audio_rate_match_e match, unsigned char match_en)
{
    reg_tx_wptr = 0xffff; //enable tx_rptr
    unsigned short          tx_rptr_old;
    audio_i2s_clk_config_t *clk_config_ptr = 0;
    switch (audio_rate) {
    case AUDIO_8K:
        clk_config_ptr = &audio_i2s_8k_config;
        break;

    case AUDIO_16K:
        clk_config_ptr = &audio_i2s_16k_config;
        break;

    case AUDIO_24K:
        clk_config_ptr = &audio_i2s_24k_config;
        break;

    case AUDIO_32K:
        clk_config_ptr = &audio_i2s_32k_config;
        break;

    case AUDIO_192K:
        clk_config_ptr = &audio_i2s_192k_config;
        break;

    case AUDIO_ADC_16K_DAC_48K:
        clk_config_ptr = &audio_i2s_adc_16k_dac_48k_config;
        break;

    case AUDIO_48K:
        if (match_en) {
            tx_rptr_old = reg_tx_rptr;
            while (tx_rptr_old == reg_tx_rptr)
                ;
        }
        clk_config_ptr = &audio_i2s_48k_config[match];

        break;

    case AUDIO_44EP1K:
        if (match_en) {
            tx_rptr_old = reg_tx_rptr;
            while (tx_rptr_old == reg_tx_rptr)
                ;
        }
        clk_config_ptr = &audio_i2s_44k1_config[match];
        break;

    default:
        break;
    }

    audio_set_i2s_clk(clk_config_ptr->i2s_clk_step, clk_config_ptr->i2s_clk_mode);
    audio_set_i2s_bclk(clk_config_ptr->i2s_bclk_div);
    audio_set_lrclk(clk_config_ptr->i2s_lrclk_adc_div, clk_config_ptr->i2s_lrclk_dac_div);
}

/**
 * @brief     This function serves to set audio rx dma chain transfer.
 * @param[in] chn       -  dma channel
 * @param[in] in_buff     - This parameter is the first address of the received data buffer, which must be 4 bytes aligned, otherwise the program will enter an exception.
 *                          and the actual buffer size defined by the user needs to be not smaller than the buff_size, otherwise there may be an out-of-bounds problem.
 * @param[in] buff_size   - This parameter is used to set the size of the received dma and must be set to a multiple of 4. The maximum value that can be set is 0xFFFFFC.
 * @return    none
 */
void audio_rx_dma_chain_init(dma_chn_e chn, unsigned short *in_buff, unsigned int buff_size)
{
    audio_rx_dma_config(chn, (unsigned short *)in_buff, buff_size, &g_audio_rx_dma_list_cfg);
    audio_rx_dma_add_list_element(&g_audio_rx_dma_list_cfg, &g_audio_rx_dma_list_cfg, (unsigned short *)in_buff, buff_size);
    audio_rx_dma_en();
}

/**
  * @brief  This function serves to initialize audio tx dma chain transfer.
  * @param[in] chn       - dma channel
  * @param[in] out_buff  - the pointer of tx_buff.
  * @param[in] buff_size - the size of tx_buff.
  * @return    none
  * @note      out_buff : must be aligned by word (4 bytes), otherwise the program will enter an exception
  */
void audio_tx_dma_chain_init(dma_chn_e chn, unsigned short *out_buff, unsigned int buff_size)
{
    audio_tx_dma_config(chn, (unsigned short *)out_buff, buff_size, &g_audio_tx_dma_list_cfg);
    audio_tx_dma_add_list_element(&g_audio_tx_dma_list_cfg, &g_audio_tx_dma_list_cfg, (unsigned short *)out_buff, buff_size);
    audio_tx_dma_en();
}

#define WM8731_ANA_AUDIO_PATH_CTRL        0x08 //Analogue Audio Path Control
#define WM8731_DIG_AUDIO_PATH_CTRL        0x0a //Digital Audio Path Control
#define WM8731_POWER_DOWN_CTRL            0x0c //Power Down Control
#define WM8731_ST_LINE_VOL                0x00 //Set linmute volume
#define WM8731_ST_RINE_VOL                0x02 //Set rinmute volume
#define WM8731_DIG_AUDIO_INTERFACE_FORMAT 0x0e //Digital Audio Interface Format
#define WM8731_SAMPLING_CTRL              0x10 //Sampling Control
#define WM8731_ACTIVE_CTRL                0x12 //Active Control
#define WM8731_RESET_CTRL                 0x1e //Reset Register

unsigned char LineIn_To_I2S_CMD_TAB[9][2] = {
    {WM8731_RESET_CTRL,                 0x00},
    {WM8731_ST_LINE_VOL,                0x10},
    {WM8731_ST_RINE_VOL,                0x10},
    {WM8731_ANA_AUDIO_PATH_CTRL,        0x13},
    {WM8731_DIG_AUDIO_PATH_CTRL,        0x00},
    {WM8731_POWER_DOWN_CTRL,            0x22},
    {WM8731_DIG_AUDIO_INTERFACE_FORMAT, 0x02},
    {WM8731_SAMPLING_CTRL,              0x19},
    {WM8731_ACTIVE_CTRL,                0x01},
};

/**
 * @brief     This function serves to  set external  codec by i2c
 * @return    none
 */

void audio_set_ext_codec(void)
{
    for (unsigned char i = 0; i < 9; i++) {
        i2c_master_write(0x34, &LineIn_To_I2S_CMD_TAB[i][0], 2);
    }
}

/**
 * @brief     This function serves to  set pwm0 as external codec mclk
 * @param[in] pin - the pin of pwm0
 * @return    none
 */
void pwm_set(pwm_pin_e pin)
{
    pwm_set_pin(pin);
    pwm_set_clk((unsigned char)(sys_clk.pclk * 1000 * 1000 / 24000000 - 1)); //set pwm clk equal pclk 24M
    pwm_set_pwm0_mode(PWM_NORMAL_MODE);
    pwm_set_tcmp(PWM0_ID, 1);
    pwm_set_tmax(PWM0_ID, 2);                                                //24M/2=12M pwm  mclk to  ext codec clk
    pwm_start(FLD_PWM0_EN);
}

/**
 * @brief This function serves to initialize audio(external codec WM8731) by i2c.
 * @param[in]  pwm0_pin - the pin of pwm0
 * @param[in]  sda_pin  - the pin port selected as I2C sda pin port.
 * @param[in]  scl_pin  - the pin port selected as I2C scl pin port.
 * @return    none
 */
void audio_i2s_init(pwm_pin_e pwm0_pin, i2c_sda_pin_e sda_pin, i2c_scl_pin_e scl_pin)
{
    pwm_set(pwm0_pin);
    audio_i2s_set_pin();
    audio_i2c_init(EXT_CODEC, sda_pin, scl_pin);
    audio_set_chn_wl(MONO_BIT_16);
    audio_mux_config(IO_I2S, BIT_16_MONO, BIT_16_MONO, BIT_16_MONO_FIFO0);
    audio_i2s_config(I2S_I2S_MODE, I2S_BIT_16_DATA, I2S_M_CODEC_S, &audio_i2s_invert_config);
    audio_set_i2s_clock(AUDIO_32K, AUDIO_RATE_EQUAL, 0);
    audio_clk_en(1, 1);
    audio_set_ext_codec();
    audio_data_fifo0_path_sel(I2S_DATA_IN_FIFO, I2S_OUT);
}

/**
 * @brief This function serves to initialize  configuration i2s.
 * @param[in]  i2s_config - the relevant configuration struct pointer
 * @return    none
 */
void audio_i2s_config_init(audio_i2s_config_t *i2s_config)
{
    audio_i2s_set_pin();
    if (i2s_config->master_slave_mode == I2S_M_CODEC_S) {
        audio_set_i2s_clock(i2s_config->sample_rate, AUDIO_RATE_EQUAL, 0);
    }
    audio_i2s_config(i2s_config->i2s_mode, i2s_config->data_width, i2s_config->master_slave_mode, &audio_i2s_invert_config);
    audio_set_i2s_cmode(i2s_config->audio_io_mode);
    audio_clk_en(1, 1);
}

/**
 * @brief This function serves to initialize  input i2s.
 * @param[in] audio_i2s_input - the relevant input struct pointer
 * @return    none
 */
void audio_i2s_input_init(audio_i2s_input_output_t *audio_i2s_input)
{
    unsigned char ain_mode = 0;
    if (audio_i2s_input->audio_ch_sel == AUDIO_MONO) {
        ain_mode = audio_i2s_input->data_width == I2S_BIT_16_DATA ? 0 : 1;
    } else {
        ain_mode = audio_i2s_input->data_width == I2S_BIT_16_DATA ? 2 : 3;
    }
    if (audio_i2s_input->fifo_chn == FIFO0) {
        reg_audio_sel  = (reg_audio_sel & (~FLD_AUDIO_AIN0_SEL)) | MASK_VAL(FLD_AUDIO_AIN0_SEL, I2S_DATA_IN_FIFO);
        reg_audio_tune = (reg_audio_tune & (~FLD_AUDIO_I2S_I2S_AIN0_COME)) | MASK_VAL(FLD_AUDIO_I2S_I2S_AIN0_COME, ain_mode);
    } else {
        reg_audio_sel  = (reg_audio_sel & (~FLD_AUDIO_AIN1_SEL)) | MASK_VAL(FLD_AUDIO_AIN1_SEL, I2S_DATA_IN_FIFO);
        reg_audio_tune = (reg_audio_tune & (~FLD_AUDIO_I2S_I2S_AIN1_COME)) | MASK_VAL(FLD_AUDIO_I2S_I2S_AIN1_COME, ain_mode);
    }
}

/**
 * @brief  This function serves to initialize output i2s.
 * @param[in] audio_i2s_output - audio_i2s_input_output_t pointer.
 * @return    none.
 */
void audio_i2s_output_init(audio_i2s_input_output_t *audio_i2s_output)
{
    unsigned char aout_mode = 0;
    if (audio_i2s_output->audio_ch_sel == AUDIO_MONO) {
        aout_mode = audio_i2s_output->data_width == I2S_BIT_16_DATA ? 0 : 1;
    } else {
        aout_mode = audio_i2s_output->data_width == I2S_BIT_16_DATA ? 2 : 3;
    }
    if (audio_i2s_output->fifo_chn == FIFO0) {
        reg_audio_sel  = (reg_audio_sel & (~FLD_AUDIO_AOUT0_SEL)) | MASK_VAL(FLD_AUDIO_AOUT0_SEL, I2S_OUT);
        reg_audio_tune = (reg_audio_tune & (~FLD_AUDIO_I2S_I2S_AOUT_COME)) | MASK_VAL(FLD_AUDIO_I2S_I2S_AOUT_COME, aout_mode);
    } else {
        reg_audio_sel  = (reg_audio_sel & (~FLD_AUDIO_AOUT1_SEL)) | MASK_VAL(FLD_AUDIO_AOUT1_SEL, I2S_OUT);
        reg_audio_tune = (reg_audio_tune & (~FLD_AUDIO_I2S_I2S_AOUT_COME)) | MASK_VAL(FLD_AUDIO_I2S_I2S_AOUT_COME, aout_mode + 4);
    }
}

/**
 * @brief    This function serves to active soft mute dac and disable dma .
 * @return    none
 */
void audio_pause_out_path(void)
{
    audio_set_codec_dac_mute(); //dac mute
    audio_tx_dma_dis();
}

/**
  * @brief    This function serves to inactive soft mute dac and enable dma after change_sample_rate.
  * @return    none
  */
void audio_resume_out_path(void)
{
    audio_set_codec_dac_unmute(); //dac unmute
    audio_tx_dma_en();
}

/**
 * @brief     This function serves to change sample rate for dac.
 * @param[in] rate     -  the sample rate of dac
 * @return    none
 */
_attribute_ram_code_sec_ void audio_change_sample_rate(audio_sample_rate_e rate)
{
    audio_set_i2s_clock(rate, AUDIO_RATE_EQUAL, 1);
    reg_audio_codec_dac_freq_ctr = (FLD_AUDIO_CODEC_DAC_FREQ & rate);
}

/**
 * @brief     This function serves to power down codec_dac.
 * @return    none
 */
void audio_codec_dac_power_down(void)
{
    audio_set_codec_dac_mute();
    delay_ms(10);
    BM_SET(reg_audio_codec_dac_itf_ctr, FLD_AUDIO_CODEC_DAC_ITF_SB);
    BM_SET(reg_audio_codec_dac_ctr, FLD_AUDIO_CODEC_DAC_SB);
    delay_ms(1);
    BM_SET(reg_audio_codec_vic_ctr, FLD_AUDIO_CODEC_SLEEP_ANALOG);
    reg_audio_codec_vic_ctr = MASK_VAL(FLD_AUDIO_CODEC_SB, CODEC_ITF_PD, FLD_AUDIO_CODEC_SB_ANALOG, CODEC_ITF_PD, FLD_AUDIO_CODEC_SLEEP_ANALOG, CODEC_ITF_PD);
    audio_tx_dma_dis();
    audio_clk_en(0, 0);
}

/**
 * @brief     This function serves to power on codec_dac.
 * @return    none
 */
void audio_codec_dac_power_on(void)
{
    audio_clk_en(1, 1);
    BM_SET(reg_audio_codec_vic_ctr, FLD_AUDIO_CODEC_SLEEP_ANALOG);
    delay_ms(1);
    BM_CLR(reg_audio_codec_dac_ctr, FLD_AUDIO_CODEC_DAC_SB);
    BM_CLR(reg_audio_codec_dac_itf_ctr, FLD_AUDIO_CODEC_DAC_ITF_SB);
    reg_audio_codec_vic_ctr = MASK_VAL(FLD_AUDIO_CODEC_SB, CODEC_ITF_AC, FLD_AUDIO_CODEC_SB_ANALOG, CODEC_ITF_AC, FLD_AUDIO_CODEC_SLEEP_ANALOG, CODEC_ITF_AC);
    audio_set_codec_dac_unmute(); //dac unmute
    audio_tx_dma_en();
}

/**
 * @brief     This function serves to power down codec_adc.
 * @return    none
 */
void audio_codec_adc_power_down(void)
{
    audio_set_codec_adc_mute();
    delay_ms(10);
    BM_SET(reg_audio_codec_adc2_ctr, FLD_AUDIO_CODEC_ADC12_SB);
    BM_SET(reg_audio_codec_adc12_ctr, FLD_AUDIO_CODEC_ADC1_SB | FLD_AUDIO_CODEC_ADC2_SB);

    BM_SET(reg_audio_codec_vic_ctr, FLD_AUDIO_CODEC_SLEEP_ANALOG);
    reg_audio_codec_vic_ctr = MASK_VAL(FLD_AUDIO_CODEC_SB, CODEC_ITF_PD, FLD_AUDIO_CODEC_SB_ANALOG, CODEC_ITF_PD, FLD_AUDIO_CODEC_SLEEP_ANALOG, CODEC_ITF_PD);
    audio_rx_dma_dis();
    audio_clk_en(0, 0);
}

/**
 * @brief     This function serves to power on codec_adc.
 * @return    none
 */
void audio_codec_adc_power_on(void)
{
    audio_clk_en(1, 1);
    BM_SET(reg_audio_codec_vic_ctr, FLD_AUDIO_CODEC_SLEEP_ANALOG);
    delay_ms(1);
    BM_CLR(reg_audio_codec_adc12_ctr, FLD_AUDIO_CODEC_ADC1_SB | FLD_AUDIO_CODEC_ADC2_SB);
    BM_CLR(reg_audio_codec_adc2_ctr, FLD_AUDIO_CODEC_ADC12_SB);
    reg_audio_codec_vic_ctr = MASK_VAL(FLD_AUDIO_CODEC_SB, CODEC_ITF_AC, FLD_AUDIO_CODEC_SB_ANALOG, CODEC_ITF_AC, FLD_AUDIO_CODEC_SLEEP_ANALOG, CODEC_ITF_AC);
    audio_set_codec_adc_unmute();

    audio_rx_dma_en();
}
