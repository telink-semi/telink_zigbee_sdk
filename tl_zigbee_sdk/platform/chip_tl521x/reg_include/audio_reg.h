/********************************************************************************************************
 * @file    audio_reg.h
 *
 * @brief   This is the header file for tl322x
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef AUDIO_REG_H
#define AUDIO_REG_H
#include "soc.h"

#define REG_AUDIO_AHB_BASE               0x120000
#define REG_AUDIO_FIFO_ADDR(i)          (0x120000+0x40*(i))

#define AUDIO_DFIFO_REG_BASE             0x141000
#define AUDIO_CIP_BASE                   0x141080

#define I2S_TDM_BASE                     0x1410f0
#define ASCL0_SDM_BASE                   0x141110
#define ASCL1_BASE                       0x141128

///****************************************************fifo register*****************************************************************/

/**
 * BIT[0] enable sdm softmute left channel.
 * BIT[1] enable sdm softmute right channel.
 * BIT[2] sdm softmute left channel mode select, 1'b1:config step.1'b0:step is 1.
 * BIT[3] sdm softmute right channel mode select, 1'b1:config step.1'b0:step is 1.
 * BIT[4] sdm bypass softmute.
 */
#define reg_sdm_softmute                    REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x00)
enum
{
    FLD_SDM_SOFTMUTE_L                      = BIT(0),
    FLD_SDM_SOFTMUTE_R                      = BIT(1),
    FLD_SDM_SOFTMUTE_L_MODE_SEL             = BIT(2),
    FLD_SDM_SOFTMUTE_R_MODE_SEL             = BIT(3),
    FLD_SDM_BYPASS_SOFTMUTE                 = BIT(4),
};

/**
 * BIT[0:15]   reg_rxfifo_max[15:0] rxfifo wptr max byte.
 */
#define reg_rxfifo_max(i)                   REG_ADDR16((AUDIO_DFIFO_REG_BASE+0x02)+(i*0x04))

/**
 * BIT[0:15]   reg_sdm_mute_step[15:0] sdm mute step.
 */
#define reg_sdm_mute_step_l                 REG_ADDR16(AUDIO_DFIFO_REG_BASE+0x04)
#define reg_sdm_mute_step_r                 REG_ADDR16(AUDIO_DFIFO_REG_BASE+0x0e)

/**
 * BIT[0:15]   reg_sdm_pga_gain[15:0] sdm pga gain.
 */
#define reg_sdm_pga_gain_l                  REG_ADDR16(AUDIO_DFIFO_REG_BASE+0x08)
#define reg_sdm_pga_gain_r                  REG_ADDR16(AUDIO_DFIFO_REG_BASE+0x0a)

/**
 * BIT[0] enable audio input of rxfifo0.
 * BIT[1] enable audio input of rxfifo1.
 * BIT[4] enable interrupts of rxfifo0.
 * BIT[5] enable interrupts of rxfifo1.
 */
#define reg_rxfifo_en                       REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x10)
enum
{
    FLD_RXFIFO0_AINEN                       = BIT(0),
    FLD_RXFIFO1_AINEN                       = BIT(1),
    FLD_RXFIFO0_IRQ_EN                      = BIT(4),
    FLD_RXFIFO1_IRQ_EN                      = BIT(5)
};

/**
 * BIT[0:1] change sampling point, set 2'b01.
 * BIT[5] change samping  clk edge.
 * BIT[6] enable left mic channel.
 * BIT[7] enable right mic channel.
 */
#define reg_codec_cfg                       REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x11)
enum{
    FLD_R_IF                                = BIT_RNG(0,1),
    FLD_R_NEG                               = BIT(5),
    FLD_L_CH_EN                             = BIT(6),
    FLD_R_CH_EN                             = BIT(7),
};

/**
 * BIT[0:5] mic vol control: 6'h00:-48db,6'h04:-42db,6'h08:-36db,6'h0c:-30db,6'h10:-24db,
 *                           6'h14:-18db,6'h18:-12db,6'h1c:-6db,6'h20:0db,6'h24:6db,6'h28:12db,
 *                           6'h2c:18db,6'h30:24db,6'h34:30db,6'h38:36db,6'h3c:42db.
 * BIT[7] mic selection:0:amic 1:dmic.
 */
#define reg_codec_vol                       REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x12)
enum{
    FLD_DEC_VOL                             = BIT_RNG(0,5),
    FLD_MIC_SEL                             = BIT(7),
};

/**
 * BIT[0] rst_sdm enable
 * BIT[1] rst_i2s enable
 * BIT[2] rst_codec enable
 * BIT[3] rst ascl0 enable
 * BIT[4] rst_ascl1_line0 enable
 * BIT[5] rst_ascl1_line1 enable
 * BIT[6] rst_i2s_aclk enable
 */
#define reg_rstn_en                         REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x13)
enum{
    FLD_RST_SDM                             = BIT(0),
    FLD_RST_I2S                             = BIT(1),
    FLD_RST_CODEC                           = BIT(2),
    FLD_RST_ASCL0                           = BIT(3),
    FLD_RST_ASCL1_LINE0                     = BIT(4),
    FLD_RST_ASCL1_LINE1                     = BIT(5),
    FLD_RST_I2S_ACLK                        = BIT(6)
};

/**
 * BIT[0:15]   reg_txfifo_rptr[15:0] txfifo read ptr byte.
 */
#define reg_txfifo_rptr(i)                  REG_ADDR16((AUDIO_DFIFO_REG_BASE+0x14)+(i*0x04))

/**
 * BIT[0:15]   reg_rxfifo_wptr[15:0] rxfifo write ptr byte.
 */
#define reg_rxfifo_wptr(i)                  REG_ADDR16((AUDIO_DFIFO_REG_BASE+0x16)+(i*0x04))

/**
 * BIT[0:3]   reg_rxfifo_num[3:0] get rxfifo data number.
 */
#define reg_rxfifo_num(i)                   REG_ADDR8((AUDIO_DFIFO_REG_BASE+0x20)+(i*0x04))

/**
 * BIT[0:2] rxfifo0 input selsect: 2'd0:i2s;2'd1:codec;2'd2:usb.
 */
#define reg_fifoin_sel(i)                   REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x33 + i)
enum{
    FLD_AIN0_SEL                            = BIT_RNG(0,1)
};

/**
 * BIT[0] and BIT[1] must be set to different values, otherwise BIT[0] has a higher priority than BIT[1].
 * BIT[0] : codec mono left fifo selsect: 0:fifo0; 1:fifo1.
 * BIT[1] : codec mono right fifo selsect: 0:fifo0; 1:fifo1.
 */
#define reg_dec_mono_rxfifo_sel             REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x35)
#define reg_i2s_mono_rxfifo_sel             REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x36)
enum{
    FLD_MONOL_RXFIFO_SEL                    = BIT(0),
    FLD_MONOR_RXFIFO_SEL                    = BIT(1)
};

/**
 * BIT[0:1] i2s input fifo0 selsect: 2'd0:16bit mono;2'd1:20bit/24bit/32bit mono;2'd2:16bit stereo;2'd3:20bit/24bit/32bit stereo.
 * BIT[2:3] i2s input fifo1 selsect: 2'd0:16bit mono;2'd1:20bit/24bit/32bit mono;2'd2:16bit stereo;2'd3:20bit/24bit/32bit stereo.
 */
#define reg_i2s_ain_mode                    REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x3a)
enum{
    FLD_I2S_AIN0_MODE                       = BIT_RNG(0,1),
    FLD_I2S_AIN1_MODE                       = BIT_RNG(2,3)
};

/**
 * BIT[0:1] codec input fifo0 selsect: 16bit mono;2'd1:20bit mono;2'd2:16bit stereo;2'd3:20bit stereo.
 * BIT[2:3] codec input fifo1 selsect: 16bit mono;2'd1:20bit mono;2'd2:16bit stereo;2'd3:20bit stereo.
 * BIT[4]   1'b1: dec using high 16bit when width length is 16bit
 */
#define reg_dec_ain_mode                    REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x3b)
enum{
    FLD_DEC_AIN0_MODE                       = BIT_RNG(0,1),
    FLD_DEC_AIN1_MODE                       = BIT_RNG(2,3),
    FLD_DEC_16bit_H_O                       = BIT(4),
};

/**
 * BIT[0:3] set/get rxfifo trig number.
 */
#define reg_rxfifo_trig_num(i)              REG_ADDR8((AUDIO_DFIFO_REG_BASE+0x3c)+i)
enum{
    FLD_AUDIO_RXFIFO_TRIG_NUM               = BIT_RNG(0,3),
};

/**
 * BIT[0] rxfifo0 wptr enable.
 * BIT[1] rxfifo1 wptr enable.
 * BIT[4] txfifo0 wptr enable.
 * BIT[5] txfifo1 rptr enable.
 */
#define reg_rx_wptr_en                      REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x3f)
enum{
    FLD_RX0_WPTR_EN                         = BIT(0),
    FLD_RX1_WPTR_EN                         = BIT(1),
    FLD_TX0_RPTR_EN                         = BIT(4),
    FLD_TX1_RPTR_EN                         = BIT(5)
};

/**
 * BIT[0] write a rxfifo0 clear pulse in pclk domain, read rxfifo0 clear in hclk domain.
 * BIT[1] write a rxfifo1 clear pulse in pclk domain, read rxfifo1 clear in hclk domain.
 * BIT[3] write a txfifo0 clear pulse in pclk domain, read txfifo0 clear in hclk domain.
 * BIT[4] write a txfifo1 clear pulse in pclk domain, read txfifo1 clear in hclk domain.
 */
#define reg_fifo_clr                        REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x40)
enum{
    FLD_RXFIFO0_CLR                         = BIT(0),
    FLD_RXFIFO1_CLR                         = BIT(1),
    FLD_TXFIFO0_CLR                         = BIT(4),
    FLD_TXFIFO1_CLR                         = BIT(5)
};

/**
 * BIT[0] enable audio output of rxfifo0.
 * BIT[1] enable audio output of rxfifo1.
 * BIT[4] enable interrupt of rxfifo0.
 * BIT[5] enable interrupt of rxfifo1.
 */
#define reg_fifo_outen                      REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x41)
enum{
    FLD_TXFIFO0_AOUTEN                      = BIT(0),
    FLD_TXFIFO1_AOUTEN                      = BIT(1),
    FLD_TXFIFO0_IRQ_EN                      = BIT(4),
    FLD_TXFIFO1_IRQ_EN                      = BIT(5)
};

/**
 * BIT[0:1] sdm out mode select:2'd0:16bit mono;2'd1:20bit/24bit/32bit mono;2'd2:16bit stereo;2'd3:20bit/24bit/32bit stereo.
 * BIT[4:5] i2s out mode select:2'd0:16bit mono;2'd1:20bit/24bit/32bit mono;2'd2:16bit stereo;2'd3:20bit/24bit/32bit stereo.
 */
#define reg_i2s_aout_mode                   REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x42)
enum{
    FLD_SDM_AOUT_MODE                       = BIT_RNG(0,1),
    FLD_I2S_AOUT_MODE                       = BIT_RNG(4,5)
};

/**
 * BIT[0:3] txfifo trig number.
 */
#define reg_txfifo_trig_num(i)              REG_ADDR8((AUDIO_DFIFO_REG_BASE+0x43)+i)
enum{
    FLD_TXFIFO_TRIG_NUM                     = BIT_RNG(0,3),
};

/**
 * BIT[0] write when rxfifo0 full.
 * BIT[1] write when rxfifo1 full.
 * BIT[4] read when txfifo0 empty.
 * BIT[5] read when txfifo1 empty.
 */
#define reg_fifo_st                         REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x46)
enum{
    FLD_RXFIFO0_OVERRUN                     = BIT(0),
    FLD_RXFIFO1_OVERRUN                     = BIT(1),
    FLD_TXFIFO0_UNDERRUN                    = BIT(4),
    FLD_TXFIFO1_UNDERRUN                    = BIT(5)
};

/**
 * BIT[0:1] i2s tdm mode input select:0:16bit TDM enable;1:20/24/32bit TDM enable.
 * BIT[2:3] i2s tdm mode output select:0:16bit TDM enable;1:20/24/32bit TDM enable.
 */
#define reg_i2s_tdm_mode_sel                REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x47)
enum{
    FLD_I2S_TDM_AIN_MODE                    = BIT_RNG(0,1),
    FLD_I2S_TDM_AOUT_MODE                   = BIT_RNG(2,3),
};

/**
 * BIT[0] sdm_2fifo_mode enable.
 * BIT[1] i2s_2fifo_mode enable.
 */
#define reg_i2s_2fifo_mode                  REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x4b)
enum{
    FLD_SDM_2FIFO_MODE                      = BIT(0),
    FLD_I2S_2FIFO_MODE                      = BIT(1)
};

/**
 * BIT[0:3] txfifo min num for ascl empty.
 * BIT[4:7] txfifo max num for ascl empty.
 */
#define reg_ascl_aful_aemp(i)               REG_ADDR8((AUDIO_DFIFO_REG_BASE+0x4c)+(i*0x01))
enum{
    FLD_TXFIFO_NUML                         = BIT_RNG(0,3),
    FLD_TXFIFO_NUMH                         = BIT_RNG(4,7),
};

/**
 * BIT[0:3] txfifo data number.
 */
#define reg_txfifo_num(i)                   REG_ADDR8((AUDIO_DFIFO_REG_BASE+0x50)+(i*0x01))
enum{
    FLD_TXFIFO_MUN                          = BIT_RNG(0,3),
};

/**
 * BIT[0:15] reg_txfifo[15:0] txfifo when interrupt.
 */
#define reg_txfifo(i)                       REG_ADDR16((AUDIO_DFIFO_REG_BASE+0x56)+(i*0x02))

/**
 * BIT[0:15] reg_rxfifo[15:0] rxfifo when interrupt.
 */
#define reg_rxfifo(i)                       REG_ADDR16((AUDIO_DFIFO_REG_BASE+0x5c)+(i*0x02))

/**
 * BIT[0:15] reg_rxfifo[15:0] txfifo max num byte for rptr.
 */
#define reg_txfifo_max(i)                   REG_ADDR16((AUDIO_DFIFO_REG_BASE+0x62)+(i*0x02))

/**
 * BIT[0] enter_mute_l_pclk.
 * BIT[1] enter_mute_r_pclk.
 * BIT[2] leave_mute_l_pclk.
 * BIT[3] leave_mute_r_pclk.
 * BIT[4] sdm_enter_mute_l_pclk.
 * BIT[5] sdm_enter_mute_r_pclk.
 * BIT[6] sdm_leave_mute_l_pclk.
 * BIT[7] sdm_leave_mute_r_pclk.
 */
#define softmute_irq_pclk                   REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x66)
enum{
    FLD_ENTER_MUTE_L_PCLK                   = BIT(0),
    FLD_ENTER_MUTE_R_PCLK                   = BIT(1),
    FLD_LEAVE_MUTE_L_PCLK                   = BIT(2),
    FLD_LEAVE_MUTE_R_PCLK                   = BIT(3),
    FLD_SDM_ENTER_MUTE_L_PCLK               = BIT(4),
    FLD_SDM_ENTER_MUTE_R_PCLK               = BIT(5),
    FLD_SDM_LEAVE_MUTE_L_PCLK               = BIT(6),
    FLD_SDM_LEAVE_MUTE_R_PCLK               = BIT(7),
};

/**
 * BIT[0] rxfifo0_irq
 * BIT[1] rxfifo1_irq.
 * BIT[2] txfifo0_irq.
 * BIT[3] txfifo1_irq.
 * BIT[4] sdm enter_mute_l_irq.
 * BIT[5] sdm enter_mute_r_irq.
 * BIT[6] sdm leave_mute_l_irq.
 * BIT[7] sdm leave_mute_r_irq.
 */
#define reg_fifo_irq                        REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x68)
enum{
    FLD_RXFIFO0_IRQ                         = BIT(0),
    FLD_RXFIFO1_IRQ                         = BIT(1),
    FLD_TXFIFO0_IRQ                         = BIT(2),
    FLD_TXFIFO1_IRQ                         = BIT(3),
    FLD_SDM_ENTER_MUTE_L_IRQ                = BIT(4),
    FLD_SDM_ENTER_MUTE_R_IRQ                = BIT(5),
    FLD_SDM_LEAVE_MUTE_L_IRQ                = BIT(6),
    FLD_SDM_LEAVE_MUTE_R_IRQ                = BIT(7),
};

/**
 * BIT[0] clearing the rxfifo0 threshold interrupt and determining the status of the threshold interrupt.
 * BIT[1] clearing the rxfifo1 threshold interrupt and determining the status of the threshold interrupt.
 * BIT[2] txfifo0_th_irq.
 * BIT[3] txfifo1_th_irq.
 * BIT[4] enter_mute_l_irq.
 * BIT[5] enter_mute_r_irq.
 * BIT[6] leave_mute_l_irq.
 * BIT[7] leave_mute_r_irq.
 */
#define reg_fifo_th_irq                     REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x69)
enum{
    FLD_RXFIFO0_TH_IRQ                      = BIT(0),
    FLD_RXFIFO1_TH_IRQ                      = BIT(1),
    FLD_TXFIFO0_TH_IRQ                      = BIT(2),
    FLD_TXFIFO1_TH_IRQ                      = BIT(3),
    FLD_ENTER_MUTE_L_IRQ                    = BIT(4),
    FLD_ENTER_MUTE_R_IRQ                    = BIT(5),
    FLD_LEAVE_MUTE_L_IRQ                    = BIT(6),
    FLD_LEAVE_MUTE_R_IRQ                    = BIT(7),

};

/**
 * BIT[0] enable rxfifo0 threshold interrupt.
 * BIT[1] enable rxfifo1 threshold interrupt.
 * BIT[2] enable txfifo0 threshold interrupt.
 * BIT[3] enable txfifo1 threshold interrupt.
 * BIT[4] enable enter mute L interrupt.
 * BIT[5] enable enter mute R interrupt.
 * BIT[6] enable leave mute L interrupt.
 * BIT[7] enable leave mute R interrupt.
 */
#define reg_fifo_th_en                      REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x6a)
enum{
    FLD_RXFIFO0_TH_IRQ_EN                   = BIT(0),
    FLD_RXFIFO1_TH_IRQ_EN                   = BIT(1),
    FLD_TXFIFO0_TH_IRQ_EN                   = BIT(2),
    FLD_TXFIFO1_TH_IRQ_EN                   = BIT(3),
    FLD_ENTER_MUTE_L_IRQ_EN                 = BIT(4),
    FLD_ENTER_MUTE_R_IRQ_EN                 = BIT(5),
    FLD_LEAVE_MUTE_L_IRQ_EN                 = BIT(6),
    FLD_LEAVE_MUTE_R_IRQ_EN                 = BIT(7),
};

/**
 * BIT[0] debug out audio clock.
 * BIT[1] debug out sdm clock.
 * BIT[3] debug out i2s clock.
 * BIT[4] debug out audio adc 6m clock.
 * BIT[5] debug out audio adc 1m clock.
 * BIT[6] debug out audio codec clock.
 */
#define reg_aclk_dbg                        REG_ADDR8(AUDIO_DFIFO_REG_BASE+0x6b)
enum{
    FLD_ACLK_DBG_OPEN                       = BIT(0),
    FLD_SDM_CLK_DBG                         = BIT(1),
    FLD_I2S_CLK_DBG                         = BIT(3),
    FLD_ADC_CLK6M_DBG                       = BIT(4),
    FLD_ADC_CLK1M_DBG                       = BIT(5),
    FLD_CODEC_CLK_DBG                       = BIT(6),
};


///****************************************************codec register*****************************************************************/

/**
 * BIT[0] high-pass filter enable.
 * BIT[1] 1'b1: dmic clk; b1'0: 1M.
 * BIT[3] for sram enable,1'b1:sram always active.
 * BIT[4] mic input softmute enable.
 * BIT[5] alc output data softmute enable.
 * BIT[6] 1'b1:2M,1'b0:1M.
 */
#define reg_codec_cfg1                      REG_ADDR8(AUDIO_CIP_BASE+0x00)
enum{
    FLD_HPF_EN                              = BIT(0),
    FLD_R_CK_SEL                            = BIT(1),
    FLD_SRAM_CE_MANUAL                      = BIT(3),
    FLD_DATI_SOFT_MUTE                      = BIT(4),
    FLD_DATO_SOFT_MUTE                      = BIT(5),
    FLD_R_SEL_2M                            = BIT(6),
};

/**
 * BIT[0:1] 6M/(adc_clk_div*2).
 * BIT[4:7] coefficient for ALC.
 */
#define reg_codec_k1                        REG_ADDR8(AUDIO_CIP_BASE+0x05)
enum{
    FLD_CODEC_ADC_CLK_DIV                   = BIT_RNG(0,1),
    FLD_K1                                  = BIT_RNG(4,7),
};

/**
 * BIT[0] select clk usb mode.
 * BIT[1:5] sample rate:5'b00110:8k;5'b11001:11.0259k;5'b01000:12k;
                        5'b01010:16k; 5'b11011:22.0588k; 5'b11100:24k; 5'b01100:32k;
                        5'b01101:32k@2Mhz;5'b10001:44.118k;5'b00000:48k;5'b11110:48k@3MHz;
                        5'b11111:44.118k@3MHz.
 * BIT[6] codec clk div2 enable.
 * BIT[7] codec clk enable.
 */
#define reg_codec_clkcfg                    REG_ADDR8(AUDIO_CIP_BASE+0x0a)
enum{
    FLD_CLK_USB                             = BIT(0),
    FLD_CLK_SR                              = BIT_RNG(1,5),
    FLD_CLK_DIV2                            = BIT(6),
    FLD_CODEC_CLK_EN                        = BIT(7),
};

/**
 * BIT[0] codec rst disable.
 * BIT[4:6] cic shift val.default val 1:no shift;0:right shift 1;2 left shift 1;3:left shift 2;4:left shift 3.
 */
#define reg_codec_rst                       REG_ADDR8(AUDIO_CIP_BASE+0x0b)
enum{
    FLD_EN_DEC                              = BIT(0),
    FLD_SFT_ZC                              = BIT_RNG(4,6),
};

/**
 * BIT[0:3] decide the ref:1.5db/step.4'b0000:28dBFS...4'b1011:-12dBFS(default)...4'b1111:-6dBFS.
 * BIT[4:6] maxgain for ALC:6db/step;3'b000:-72dB...3'b111:-30db.
 */
#define reg_codec_alc                       REG_ADDR8(AUDIO_CIP_BASE+0x20)
enum{
    FLD_ALCL                                = BIT_RNG(0,3),
    FLD_MAXGAIN                             = BIT_RNG(4,6),
};

/**
 * BIT[0:3] hold time x2 step:4'b0000:0ms;4'b0001:2.67ms;4'b0010:5.33ms...4'b1111:43.691s.
 * BIT[5:6] alc select.2'b00:no alc;2'b01:right only;2'b10:left only;2'b11:stereo.
 */
#define reg_codec_alcsel                    REG_ADDR8(AUDIO_CIP_BASE+0x21)
enum{
    FLD_HLD                                 = BIT_RNG(0,3),
    FLD_ALCSEL                              = BIT_RNG(5,6),
};

/**
 * BIT[0:3] Change the rate of gain increase x2 step ,4'b0000:6ms...4'b1111:6.144s.
 * BIT[4:7] Change the gain reduction rate x2 step,4'b0000:24ms...4'b1111:24.576s.
 */
#define reg_codec_atk                       REG_ADDR8(AUDIO_CIP_BASE+0x22)
enum{
    FLD_ATK                                 = BIT_RNG(0,3),
    FLD_DCY                                 = BIT_RNG(4,7),
};

/**
 * BIT[0] noise gate enable.
 * BIT[1:2] noise gate type.2'b00:hold PGA gain constant;2'b01:mute output;
                            2'b10:softmute/unmute output;2'b11:reserve.
 * BIT[3:7] noise gate threshold value,1.5db step;5'b00000:-76.5dbFS...5'b11110:-31.5dbFS;5'b11111:-30dbFS
 */
#define reg_codec_noise                     REG_ADDR8(AUDIO_CIP_BASE+0x23)
enum{
    FLD_NGAT                                = BIT(0),
    FLD_NGG                                 = BIT_RNG(1,2),
    FLD_NGTH                                = BIT_RNG(3,7),
};

/**
 * BIT[0:2] mingain,6db step:3'b000:-72db...3'b111:-30db.
 * BIT[4] dmic clk sel.
 * BIT[5] 1:trig codec via pem.
 * BIT[6] 1:disable codec via pem.
 * BIT[7] codec schedule enable.
 */
#define reg_codec_mingain                   REG_ADDR8(AUDIO_CIP_BASE+0x24)
enum{
    FLD_MINGAIN                             = BIT_RNG(0,2),
    FLD_DMIC_CLK_SEL                        = BIT(4),
    FLD_CODEC_TRIG_EN                       = BIT(5),
    FLD_CODEC_DIS_EN                        = BIT(6),
    FLD_SCHDL_EN                            = BIT(7),
};

/**
 * BIT[0] softmute of left channel.
 * BIT[1] softmute of right channel.
 * BIT[2] 1'b1:config step.1'b0:step is 1.
 * BIT[4] bypass softmute
 */
#define reg_softmute_en                     REG_ADDR8(AUDIO_CIP_BASE+0x25)
enum{
    FLD_SOFTMUTE_L                          = BIT(0),
    FLD_SOFTMUTE_R                          = BIT(1),
    FLD_SOFTMUTE_MODE_SEL                   = BIT(2),
    FLD_BYPASS_SOFTMUTE                     = BIT(4),
};

/**
 * BIT[0:7] when softmute sel is 1,softmute's step.
 */
#define reg_softmute_step_sel               REG_ADDR8(AUDIO_CIP_BASE+0x26)
enum{
    FLD_SOFTMUTE_STEP                       = BIT_RNG(0,7),
};


/**
 * BIT[0:3] Magnification of the input positive number.
 * BIT[4:7] Magnification of the input negative number.
 */
#define reg_dmic_dat                        REG_ADDR8(AUDIO_CIP_BASE+0x27)
enum{
    FLD_DMIC_DAT_POS                        = BIT_RNG(0,3),
    FLD_DMIC_DAT_NEG                        = BIT_RNG(4,7),
};

/**
 * BIT[0:31] reg_codec_stimer_target[31:0] codec stimer for schedule.
 */
#define reg_codec_stimer_target             REG_ADDR32(AUDIO_CIP_BASE+0x28)

/**
 * BIT[0:10] softmmute pga gain L
 */
#define reg_softmmute_pga_gain_l            REG_ADDR16(AUDIO_CIP_BASE+0x2c)

/**
 * BIT[0:10] softmmute pga gain R
 */
#define reg_softmmute_pga_gain_r            REG_ADDR16(AUDIO_CIP_BASE+0x2e)

///****************************************************iis register*****************************************************************/

/**
 * BIT[0:1] bclk frequency:2'd0:bcm function disabled;2'd1:mclk/4;2'd2:mclk/8;2'd3:mclk/16.
 * BIT[2] i2s clk enable.
 * BIT[3] i2s clk divide2 enable:1'b1: enable,1'b0: disable.
 * BIT[4] bclk invert.
 * BIT[5] i2s adc as master.
 * BIT[6] i2s dac as master.
 * BIT[7] adc frm loop from pad.
 */
#define reg_i2s_cfg1(i)                     REG_ADDR8((I2S_TDM_BASE+0x00)+(i*0x00))
enum{
    FLD_I2S_BCM_BITS                        = BIT_RNG(0,1),
    FLD_I2S_CLK_EN                          = BIT(2),
    FLD_I2S_CLK_DIV2                        = BIT(3),
    FLD_I2S_BLCKINV_O                       = BIT(4),
    FLD_I2S_ADC_DCI_MS                      = BIT(5),
    FLD_I2S_DAC_DCI_MS                      = BIT(6),
    FLD_I2S_ADC_FRM_LOOP                    = BIT(7),
};

/**
 * BIT[0] set i2s adc lrclk loop:1'b0 adc bclk from i2s clk 1'b1 adc bclk loop gpio.
 * BIT[1] set i2s dac lrclk loop:1'b0 dac bclk from i2s clk 1'b1 dac bclk loop gpio.
 * BIT[2] set i2s lrclk invert:1'b0: frm ; 1'b1 frm_inv.
 * BIT[3:4] i2s word length:2'b00: 16;2'b01: 20;2'b10: 24;3'b11: 32.
 * BIT[5:7] i2s format:3'b000: RJ;3'b001: LJ;3'b010: I2S;3'b011: DSP 3'b100 TDM.
 */
#define reg_i2s_cfg2(i)                     REG_ADDR8((I2S_TDM_BASE+0x01)+(i*0x00))
enum{
    FLD_I2S_ADC_MBCLK_LOOP                  = BIT(0),
    FLD_I2S_DAC_MBCLK_LOOP                  = BIT(1),
    FLD_FRM_INV                             = BIT(2),
    FLD_I2S_WL                              = BIT_RNG(3,4),
    I2S_FORMAT                              = BIT_RNG(5,7),
};

/**
 * BIT[0] enable I2S l channel and r channel data swap..
 * BIT[1] enable i2s rx_2lane.
 * BIT[2] enable i2s tx_2lane.
 * BIT[3] enable i2s dac frm:0: dac_frm_clk disable 1:dac_frm_clk enable.
 * BIT[4] enable i2s adc frm:0: adc_frm_clk disable 1:adc_frm_clk enable.
 * BIT[5] i2s tx dat select: 0:i2s tx data low bits   1:sel i2s tx dat high bits.
 * BIT[6] i2s dsp mode select:DSP mode select when i2s DSP format or LRCLK invert operation :1'b1: dsp mode A;1'b0: dsp mode B;
 *                                                                                           1'b1: LRCLK invert;1'b0: not invert.
 * BIT[7] dac frm loop from pad.
 */
#define reg_i2s_cfg3(i)                     REG_ADDR8((I2S_TDM_BASE+0x02)+(i*0x00))
enum{
    FLD_I2S_LRSWAP                          = BIT(0),
    FLD_RX_2LANE_ENABLE                     = BIT(1),
    FLD_TX_2LANE_ENABLE                     = BIT(2),
    FLD_I2S_DAC_FRM_ENABLE                  = BIT(3),
    FLD_I2S_ADC_FRM_ENABLE                  = BIT(4),
    FLD_I2S_TX_DAT_SEL                      = BIT(5),
    FLD_I2S_LRP                             = BIT(6),
    FLD_I2S_DAC_FRM_LOOP                    = BIT(7),
};

/**
 * BIT[0:1] i2s work mode.2'b00: I2S 5line mode;2'b01: I2S 4line dac mode;2'b10: I2S 4line adc mode
 * BIT[2] i2s pad bclk select:0:adc bclk ;1 dac bclk.
 * BIT[3] i2s rec bit select:0: low byte ; 1 high byte.
 * BIT[4] schedule enable.
 * BIT[5] i2s ascl bypass:0: enable ascl   1:ascl bypass.
 * BIT[6] enable i2s pem trig:1: trig i2s via pem.
 * BIT[7] disable i2s pem: disable i2s via pem.
 */
#define reg_i2s_route(i)                    REG_ADDR8((I2S_TDM_BASE+0x03)+(i*0x00))
enum{
    FLD_I2S_MODE                            = BIT_RNG(0,1),
    FLD_I2S_PAD_BCLK_SEL                    = BIT(2),
    FLD_I2S_REC_BIT_SEL                     = BIT(3),
    FLD_I2S_SCHEDULE_EN                     = BIT(4),
    FLD_ASCL_BYPASS                         = BIT(5),
    FLD_PEM_TRIG_EN_0                       = BIT(6),
    FLD_PEM_DIS_EN_0                        = BIT(7),
};

/**
 * BIT[0:12] dac I2S LRCLK counter.
 */
#define reg_i2s_int_pcm_num(i)              REG_ADDR16((I2S_TDM_BASE+0x04)+(i*0x00))

/**
 * BIT[0:12] adc i2s LRCLK counter.
 */
#define reg_i2s_dec_pcm_num(i)              REG_ADDR16((I2S_TDM_BASE+0x06)+(i*0x00))

/**
 * BIT[0:7] reg_i2s_pcm_clk_num[0:7] bclk division factor, i2s0_clk/(n*2).
 */
#define reg_i2s_pcm_clk_num(i)              REG_ADDR8((I2S_TDM_BASE+0x08)+(i*0x00))

#define reg_i2s_dactune                     REG_ADDR8(I2S_TDM_BASE+0x09)
enum{
    FLD_I2S_DACTUNE_L1                      = BIT_RNG(0,3),
    FLD_I2S_DACTUNE_L2                      = BIT_RNG(4,7),
};

#define reg_i2s_adctune                     REG_ADDR8(I2S_TDM_BASE+0x0a)
enum{
    FLD_I2S_ADCTUNE_L1                      = BIT_RNG(0,3),
    FLD_I2S_ADCTUNE_L2                      = BIT_RNG(4,7),
};

#define reg_i2s_fifo_config                 REG_ADDR8(I2S_TDM_BASE+0x0b)
enum{
    FLD_TXFIFO_LESSL1                       = BIT(0),
    FLD_TXFIFO_LESSL2                       = BIT(1),
    FLD_TXFIFO_MOREL1                       = BIT(2),
    FLD_TXFIFO_MOREL2                       = BIT(3),
    FLD_RXFIFO_LESSL1                       = BIT(4),
    FLD_RXFIFO_LESSL2                       = BIT(5),
    FLD_RXFIFO_MOREL1                       = BIT(6),
    FLD_RXFIFO_MOREL2                       = BIT(7),
};

/**
 * BIT[0:31] reg_i2s_stimer_target[0:31] i2s stimer for schedule.
 */
#define reg_i2s_stimer_target(i)            REG_ADDR32((I2S_TDM_BASE+0x0c)+(i*0x00))

/**
 * BIT[6] fix pad2reg timing in i2s_slave mode.
 */
#define reg_i2s_align_cfg(i)                REG_ADDR8(I2S_TDM_BASE+0x10)
enum{
    FLD_RX_DSP_START_SEL                    = BIT(6),
};

/**
 * BIT[0:1] configure the number of i2s tdm rx channel:2'b00 2channel; 2'b01 4channel; 2'b10 6channel; 2'b11 8channel.
 * BIT[2:3] configure the number of i2s tdm tx channel:2'b00 2channel; 2'b01 4channel; 2'b10 6channel; 2'b11 8channel.
 * BIT[4:5] i2s TDM mode:2'b00:TDM_mode_a; 2'b01:TDM_mode_b; 2'b10:TDM_mode_c.
 * BIT[6:7] i2s TDM slot:2'b00:16 BCLK wide; 2'b01:24 BCLK wide; 2'b10:32 BCLK wide.
 */
#define reg_i2s_tdm_cfg                     REG_ADDR8(I2S_TDM_BASE+0x11)
enum{
    FLD_I2S_TDM_RX_CH_NUM                   = BIT_RNG(0,1),
    FLD_I2S_TDM_TX_CH_NUM                   = BIT_RNG(2,3),
    FLD_I2S_TDM_MODE                        = BIT_RNG(4,5),
    FLD_I2S_TDM_SLOT                        = BIT_RNG(6,7),
};

///****************************************************ascl register*****************************************************************/

/**
 * BIT[0] enable mono.
 * BIT[2] enable sdm.
 * BIT[3] enable ISO.
 * BIT[4] enable pem.
 * BIT[5] disable pem.
 * BIT[6] enable grp.
 * BIT[7] enable HPF.
 */
#define reg_aud_en                          REG_ADDR8(ASCL0_SDM_BASE+0x00)
enum{
    FLD_MONO                                = BIT(0),
    FLD_SDMPEN                              = BIT(2),
    FLD_ISOPEN                              = BIT(3),
    FLD_PEM_TRIG_EN_1                       = BIT(4),
    FLD_PEM_DIS_EN_1                        = BIT(5),
    FLD_SDMGRP                              = BIT(6),
    FLD_HPF                                 = BIT(7),
};

/**
 * BIT[0:7] reg_grpmid[7:0] middle of GRP.
 */
#define reg_grpmid                          REG_ADDR8(ASCL0_SDM_BASE+0x01)

/**
 * BIT[0:6] audio sound volume.[0]:add a quarter;[1]add a half;[6:2]:shift left.
 * BIT[7] mute enable.
 */
#define reg_aud_vol                         REG_ADDR8(ASCL0_SDM_BASE+0x02)
enum{
    FLD_VOL_0                               = BIT_RNG(0,6),
    FLD_MUTE_0                              = BIT(7),
};

/**
 * BIT[0] 1'b1:not multiply 2 when PWM;1'b0:multiply 2
 * BIT[1] PWM enable.
 * BIT[2] select either linear interpolation or delay interpolation for the rate matching block.1'b1: linear interpolation.
 * BIT[3] left 1'b1:4 bit output; 1'b0:1 bit output
 * BIT[4] right 1'b1:4 bit output; 1'b0:1 bit output
 * BIT[5] left shapping enable.
 * BIT[6] right shapping enable.
 */
#define reg_aud_ctrl                        REG_ADDR8(ASCL0_SDM_BASE+0x03)
enum{
    FLD_NOX2                                = BIT(0),
    FLD_PWM                                 = BIT(1),
    FLD_LINE_0                              = BIT(2),
    FLD_YSEL_L                              = BIT(3),
    FLD_YSEL_R                              = BIT(4),
    FLD_SHAP_L                              = BIT(5),
    FLD_SHAP_R                              = BIT(6),
};

#define reg_step_tune_0                     REG_ADDR32(ASCL0_SDM_BASE+0x04)

/**
 * BIT[0:7] reg_tune_0[7:0] tune step for rate matching block,to adjust step value when fifo almost full or empty.
 */
#define reg_tune_0                          REG_ADDR8(ASCL0_SDM_BASE+0x04)

/**
 * BIT[0] sdm afifo clr.
 * BIT[1] sdm afifo overrun.
 * BIT[2] sdm fifo empty.
 * BIT[3] 1: sdm_data from afifo
 * BIT[4:7] step[0:3]
 */
#define reg_step_l_0                        REG_ADDR8(ASCL0_SDM_BASE+0x05)
enum{
    FLD_AFIFO_CLR                           = BIT(0),
    FLD_SDM_AFIFO_OVERRUN                   = BIT(1),
    FLD_SDM_FIFO_EMPTY                      = BIT(2),
    FLD_SDM_DATA_SYNC_SEL                   = BIT(3),
    FLD_STEP_L                              = BIT_RNG(4,7),
};

/**
 * BIT[0:7] step[4:11]
 */
#define reg_step_m_0                        REG_ADDR8(ASCL0_SDM_BASE+0x06)

/**
 * BIT[0:7] step[12:19]
 */
#define reg_step_h_0                        REG_ADDR8(ASCL0_SDM_BASE+0x07)

/**
 * BIT[0:4] bits used in pn1 of left channel,range from 0 to 16.
 * BIT[5:6] [5]:pn2 of left enable; [6]:pn1 of left enable.
 */
#define reg_aud_pn1l                        REG_ADDR8(ASCL0_SDM_BASE+0x08)
enum{
    FDL_PN1_BITS_L                          = BIT_RNG(0,4),
    FLD_PN_SEL_L                            = BIT_RNG(5,6),
};

/**
 * BIT[0] bits used in pn2 of left channel,range from 0 to 16.
 * BIT[5:6] [5]:pn2 of left enable; [6]:pn1 of left enable.
 */
#define reg_aud_pn2                         REG_ADDR8(ASCL0_SDM_BASE+0x09)
enum{
    FLD_PN2_BITS_L                          = BIT_RNG(0,4),
    FLD_PN_SEL_R                            = BIT_RNG(5,6),
};

/**
 * BIT[0:4] bits used in pn1 of right channel,range from 0 to 16.
 * BIT[5] 1'b1:exchange data in between SDMs.
 * BIT[6:7] 0: sel [15:0]; 1:[19:4],2: sel [23:8]
 */
#define reg_aud_pn1r                        REG_ADDR8(ASCL0_SDM_BASE+0x0a)
enum{
    FLD_PN1_R                               = BIT_RNG(0,4),
    FLD_CHANNEL_CHG                         = BIT(5),
    FLD_ASCL0_FORMAT_SEL                    = BIT_RNG(6,7),
};

/**
 * BIT[0:4] bits used in pn2 of right channel,range from 0 to 16.
 * BIT[5] 1'b1:left channel use const value.
 * BIT[6] 1'b1:right channel use const value.
 */
#define reg_const_set                       REG_ADDR8(ASCL0_SDM_BASE+0x0b)
enum{
    FLD_PN2_BITS_R                          = BIT_RNG(0,4),
    FLD_CONST_SEL_L                         = BIT(5),
    FLD_CONST_SEL_R                         = BIT(6),
};

/**
 * BIT[0:15] reg_const_l[15:0] Configure the Dither const value of left channel.
 */
#define reg_const_l                         REG_ADDR16(ASCL0_SDM_BASE+0x0c)

/**
 * BIT[0:15] reg_const_r[15:0] Configure the Dither const valueof right channel.
 */
#define reg_const_r                         REG_ADDR16(ASCL0_SDM_BASE+0x0e)

/**
 * BIT[0] enable mono mode.
 * BIT[1] ascl format select:0: sel [15:0] from [23:0]; 1: sel [23:8] from [23:0].
 * BIT[7] enable HPF.
 */
#define reg_ascl_config(i)                  REG_ADDR8((ASCL1_BASE+0x00)+(i*0x00))
enum{
    FLD_ASCL_MON                            = BIT(0),
    FLD_ASCL_FORMAT_SEL                     = BIT(1),
    FLD_ASCL_HPF                            = BIT(7),
};

/**
 * BIT[0:6] audio sound volume.[0]:add a quarter;[1]add a half;[6:2]:shift left.
 * BIT[7] mute enable.
 */
#define reg_ascl_vol(i)                     REG_ADDR8((ASCL1_BASE+0x02)+(i*0x00))
enum{
    FLD_VOL_1                               = BIT_RNG(0,6),
    FLD_MUTE_1                              = BIT(7),
};

/**
 * BIT[2] enable line.
 */
#define reg_line(i)                         REG_ADDR8((ASCL1_BASE+0x03)+(i*0x00))
enum{
    FLD_LINE_1                              = BIT(2),
};

/**
 * BIT[0:7] tune step for rate matching block,to adjust step value when fifo almost full or empty.
 */

#define reg_step_tune_1(i)                  REG_ADDR32((ASCL1_BASE+0x04)+(i*0x00))

/**
 * BIT[0:7] reg_tune_1[7:0] tune step for rate matching block,to adjust step value when fifo almost full or empty.
 */
#define reg_tune_1(i)                       REG_ADDR8((ASCL1_BASE+0x04)+(i*0x00))

/**
 * BIT[4:7] step_i[0:3], change sample rate.
 */
#define reg_step_l_1(i)                     REG_ADDR8((ASCL1_BASE+0x05)+(i*0x00))
enum{
    FLD_STEP_L_1                            = BIT_RNG(4,7),
};

/**
 * BIT[0:7] step_i[4:11], change sample rate.
 */
#define reg_step_m_1(i)                     REG_ADDR8((ASCL1_BASE+0x06)+(i*0x00))

/**
 * BIT[0:7] step_i[12:19], change sample rate..
 */
#define reg_step_h_1(i)                     REG_ADDR8((ASCL1_BASE+0x07)+(i*0x00))

#endif
