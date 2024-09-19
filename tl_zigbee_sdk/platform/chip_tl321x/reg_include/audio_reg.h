/********************************************************************************************************
 * @file    audio_reg.h
 *
 * @brief   This is the header file for TL321X
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
#ifndef AUDIO_REG_H
#define AUDIO_REG_H
#include "soc.h"

#define     REG_AUDIO_AHB_BASE          0x120000
#define     REG_AUDIO_FIFO_ADDR(i)      (0x120000+0x40*(i))

#define     REG_AUDIO_APB_BASE          0x140500
#define     REG_AUDIO_CODEC_BASE        0x140500+0x80

#define   reg_i2s_en(i)                 REG_ADDR8(REG_AUDIO_APB_BASE+((i)*0x30))
enum
{
    FLD_AUDIO_I2S_CLK_EN              =  BIT(0),// reg_i2s_en(0) and reg_i2s_en(1)
    FLD_AUDIO_I2S_CLK_DIV2            =  BIT(1),// reg_i2s_en(0) and reg_i2s_en(1)
    FLD_AUDIO_I2S0_MODE               =  BIT_RNG(2,3),//only reg_i2s_en(0)
    FLD_AUDIO_I2S1_MODE               =  BIT_RNG(4,5),//only reg_i2s_en(0)
};


enum
{
    FLD_AUDIO_I2S1_CLK_EN              =  BIT(0),
    FLD_AUDIO_I2S1_CLK_DIV2            =  BIT(1),
};

#define   reg_i2s_cfg1(i)            REG_ADDR8(REG_AUDIO_APB_BASE+0x01+((i)*0x30))

#define   reg_i2s0_cfg1          REG_ADDR8(REG_AUDIO_APB_BASE+0x01)
#define   reg_i2s1_cfg1          REG_ADDR8(REG_AUDIO_APB_BASE+0x31)

enum
{
    FLD_AUDIO_I2S_FORMAT                  =  BIT_RNG(0,1),
    FLD_AUDIO_I2S_WL                      =  BIT_RNG(2,3),
    FLD_AUDIO_I2S_LRP                     =  BIT(4),
    FLD_AUDIO_I2S_LRSWAP                  =  BIT(5),
    FLD_AUDIO_I2S_ADC_DCI_MS              =  BIT(6),
    FLD_AUDIO_I2S_DAC_DCI_MS              =  BIT(7),
};

#define   reg_i2s0_cfg2              REG_ADDR8(REG_AUDIO_APB_BASE+0x02)
enum
{
    FLD_AUDIO_I2S0_BCM_BITS                 =  BIT_RNG(0,1),
    FLD_AUDIO_I2S0_BCK_INV                  =  BIT(2),
    FLD_AUDIO_FIFO0_RST                     =  BIT(3),
    FLD_AUDIO_FIFO1_RST                     =  BIT(4),
};

#define   reg_i2s1_cfg2              REG_ADDR8(REG_AUDIO_APB_BASE+0x32)
enum
{
    FLD_AUDIO_I2S1_BCM_BITS                 =  BIT_RNG(0,1),
    FLD_AUDIO_I2S1_BCK_INV                  =  BIT(2),
};

#define   reg_audio_is0_ctrl                    REG_ADDR8(REG_AUDIO_APB_BASE+0x03)
enum
{
    FLD_AUDIO_CODEC_I2S0_SEL              =  BIT(2),
    FLD_AUDIO_I2S0_OUT_BIT_SEL            =  BIT(3),//for debug
};

#define   reg_audio_is1_ctrl                   REG_ADDR8(REG_AUDIO_APB_BASE+0x33)
enum
{
    FLD_AUDIO_I2S_SYNC_MODE                =  BIT_RNG(0,1),
    FLD_AUDIO_I2S1_OUT_BIT_SEL             =  BIT(3),//for debug
};

#define   reg_audio_i2s_tune(i)                 REG_ADDR8(REG_AUDIO_APB_BASE+0x04+((i)*0x30))

enum
{
    FLD_AUDIO_I2S_AIN0_COME            =  BIT_RNG(0,1),
    FLD_AUDIO_I2S_AIN1_COME            =  BIT_RNG(2,3),
    FLD_AUDIO_I2S_AOUT_COME            =  BIT_RNG(4,7),
};


#define   reg_audio_dec0_sel            REG_ADDR8(REG_AUDIO_APB_BASE+0x60)
enum
{
        FLD_AUDIO_DEC0_AIN0_COME            =  BIT_RNG(0,1),
        FLD_AUDIO_DEC0_AIN1_COME            =  BIT_RNG(2,3),
        FLD_AUDIO_INT_AOUT_COME            =  BIT_RNG(4,7),
};

#define   reg_audio_dec1_sel            REG_ADDR8(REG_AUDIO_APB_BASE+0x56)
enum
{
        FLD_AUDIO_DEC1_AIN0_COME            =  BIT_RNG(0,2),
        FLD_AUDIO_DEC1_AIN1_COME            =  BIT_RNG(4,6),
};

#define   reg_audio_sel1                REG_ADDR8(REG_AUDIO_APB_BASE+0x05)
enum
{
    FLD_AUDIO_AIN0_SEL             =  BIT_RNG(0,2),
    FLD_AUDIO_AOUT0_SEL            =  BIT_RNG(3,4),
};

#define   reg_audio_sel2                 REG_ADDR8(REG_AUDIO_APB_BASE+0x06)
enum
{
    FLD_AUDIO_AIN1_SEL             =  BIT_RNG(0,2),
    FLD_AUDIO_AOUT1_SEL            =  BIT_RNG(3,4),
};

#define   reg_audio_sel(i)               REG_ADDR8(REG_AUDIO_APB_BASE+0x05+(i))
enum
{
    FLD_AUDIO_AIN_SEL             =  BIT_RNG(0,2),
    FLD_AUDIO_AOUT_SEL            =  BIT_RNG(3,4),
};


#define   reg_fifo_trig0                    REG_ADDR8(REG_AUDIO_APB_BASE+0x0a)
#define   reg_fifo_trig1                    REG_ADDR8(REG_AUDIO_APB_BASE+0x0b)
#define   reg_fifo_trig(i)                  REG_ADDR8(REG_AUDIO_APB_BASE+0x0a+(i))

enum
{
    FLD_AUDIO_FIFO_AOUT_TRIG_NUM        =  BIT_RNG(0,3),
    FLD_AUDIO_FIFO_AIN_TRIG_NUM         =  BIT_RNG(4,7),
};



#define   reg_audio_ptr_set                 REG_ADDR8(REG_AUDIO_APB_BASE+0x10)
enum
{
    FLD_AUDIO_TX_PTR_SEL                 =  BIT(0),
    FLD_AUDIO_RX_PTR_SEL                 =  BIT(4),
};

#define   reg_audio_ptr_en              REG_ADDR8(REG_AUDIO_APB_BASE+0x11)
enum
{
    FLD_AUDIO_TX_WPTR_PTR_EN                 =  BIT(0),
    FLD_AUDIO_TX_RPTR_PTR_EN                 =  BIT(1),
    FLD_AUDIO_RX_WPTR_PTR_EN                 =  BIT(2),
    FLD_AUDIO_RX_RPTR_PTR_EN                 =  BIT(3),

};

#define   reg_tx_rptr(i)            REG_ADDR16(REG_AUDIO_APB_BASE+0x22+((i)*0x20))
#define   reg_rx_wptr(i)            REG_ADDR16(REG_AUDIO_APB_BASE+0x28+((i)*0x20))


#define   reg_tx_max(i)             REG_ADDR16(REG_AUDIO_APB_BASE+(0x26)+((i)*0x20))
#define   reg_rx_max(i)             REG_ADDR16(REG_AUDIO_APB_BASE+(0x2e)+((i)*0x20))

#define   reg_tx_fifo_th(i)         REG_ADDR16(REG_AUDIO_APB_BASE+0x24+((i)*0x20))
#define   reg_rx_fifo_th(i)         REG_ADDR16(REG_AUDIO_APB_BASE+0x2A+((i)*0x20))

#define   reg_fifo_ptr_irq_mask        REG_ADDR8(REG_AUDIO_APB_BASE+0x62)
#define   reg_fifo_ptr_irq_status      REG_ADDR8(REG_AUDIO_APB_BASE+0x63)
 enum
{
    FLD_AUDIO_RX_FIFO0                   =  BIT(0),
    FLD_AUDIO_TX_FIFO0                   =  BIT(1),
    FLD_AUDIO_RX_FIFO1                   =  BIT(2),
    FLD_AUDIO_TX_FIFO1                   =  BIT(3),
};

#define   reg_i2s_int_pcm_num(i)      REG_ADDR16(REG_AUDIO_APB_BASE+0x50-((i)*0x1A))
#define   reg_i2s_dec_pcm_num(i)      REG_ADDR16(REG_AUDIO_APB_BASE+0x52-((i)*0x1A))

#define   reg_i2s_pcm_clk_num(i)      REG_ADDR8(REG_AUDIO_APB_BASE+0x54-((i)*0x1A))

#define   reg_codec_dec0_vol0         REG_ADDR8(REG_AUDIO_APB_BASE+0x61)

#define   reg_codec_dec0_vol1         REG_ADDR8(REG_AUDIO_APB_BASE+0x25)

#define   reg_codec_dec1_vol          REG_ADDR8(REG_AUDIO_APB_BASE+0x55)

#define   reg_usb_audio_ctrl              REG_ADDR8(REG_AUDIO_APB_BASE+0x40)
enum
{
    FLD_USB_RD_MODE                =  BIT(0),
    FLD_CODEC_ALIGN_MASK           =  BIT(1),
    FLD_I2S_ALIGN_MASK             =  BIT(2),
};

#define   reg_align_ctrl              REG_ADDR8(REG_AUDIO_APB_BASE+0x57)
enum
{
    FLD_CODEC_ALIGN_EN           =  BIT(0),
    FLD_I2S_ALIGN_EN             =  BIT(1),
    FLD_I2S_ALIGN_CLK_SEL        =  BIT(2),
    FLD_I2S_CLK_SEL              =  BIT(3),
    FLD_I2S_ALIGN_CTRL           =  BIT_RNG(4,5),
    FLD_CODEC_ALIGN_CTRL         =  BIT_RNG(6,7),
};


#define   reg_i2s_timer_th            REG_ADDR32(REG_AUDIO_APB_BASE+0x5C)


/****************************************************codec register*****************************************************************/
#define   reg_codec_dsm_ctr1          REG_ADDR8(REG_AUDIO_CODEC_BASE)
enum
{
    FLD_AUDIO_CODEC_DEC0_SWAP         =  BIT(4),
    FLD_AUDIO_CODEC_INT_SWAP          =  BIT(5),
};

/**
 * BIT[0] DAC left volume soft mute
 * BIT[1] DAC right volume soft mute
 * BIT[2:4] gain control of left channel sidetone
 * BIT[5:7] gain control of left channel sidetone
 */
#define   reg_codec_sidetone          REG_ADDR8(REG_AUDIO_CODEC_BASE+0x04)
enum
{
    FLD_AUDIO_CODEC_INT_MUTE_L        = BIT(0),
    FLD_AUDIO_CODEC_INT_MUTE_R        = BIT(1),
    FLD_AUDIO_CODEC_SDTG_L            = BIT_RNG(2,4),
    FLD_AUDIO_CODEC_SDTG_R            = BIT_RNG(5,7),
};

/**
 * BIT[0:7] DAC dac pga softmute step
 */
#define   reg_codec_mute_step_sel          REG_ADDR8(REG_AUDIO_CODEC_BASE+0x2a)

/**
 * BIT[0:3] DAC right channel volume high byte
 * BIT[4:7] k1 factor for ALC
 */
#define   reg_codec_int_vol_r_1       REG_ADDR8(REG_AUDIO_CODEC_BASE+0x05)
enum
{
    FLD_AUDIO_CODEC_INT_VOL_R_L        = BIT_RNG(0,3),
    FLD_AUDIO_CODEC_ALC_K1            = BIT_RNG(4,7),
};
/**
 * BIT[0:7] DAC right channel volume low byte
 */
#define   reg_codec_int_vol_r_2       REG_ADDR8(REG_AUDIO_CODEC_BASE+0x06)
enum
{
    FLD_AUDIO_CODEC_INT_VOL_R_H         =  BIT_RNG(0,7),
};

/**
 * BIT[0:3] DAC left channel volume high byte
 * BIT[4:7] k1 factor for ALC
 */
#define   reg_codec_int_vol_l_1       REG_ADDR8(REG_AUDIO_CODEC_BASE+0x07)
enum
{
    FLD_AUDIO_CODEC_INT_VOL_L_L         = BIT_RNG(0,3),
    FLD_AUDIO_CODEC_ALC_K2            = BIT_RNG(4,7),
};
/**
 * BIT[0:7] DAC left channel volume low byte
 */
#define   reg_codec_int_vol_l_2       REG_ADDR8(REG_AUDIO_CODEC_BASE+0x08)
enum
{
    FLD_AUDIO_CODEC_INT_VOL_L_H         =  BIT_RNG(0,7),
};


#define   reg_codec_clk_ctr1          REG_ADDR8(REG_AUDIO_CODEC_BASE+0x0A)
enum
{
    FLD_AUDIO_CODEC_CLK_USB           =  BIT(0),
    FLD_AUDIO_CODEC_DEC0_CLK_SR       =  BIT_RNG(1,5),
    FLD_AUDIO_CODEC_CLK_DIV2          =  BIT(6),
    FLD_AUDIO_CODEC_CLK0_EN           =  BIT(7),
};

#define   reg_codec_clk_ctr2          REG_ADDR8(REG_AUDIO_CODEC_BASE+0x0B)
enum
{
    FLD_AUDIO_CODEC_DEC0_EN           =  BIT(0),
    FLD_AUDIO_CODEC_INT_EN            =  BIT(1),
    FLD_AUDIO_CODEC_INT_CLK_SR        =  BIT_RNG(3,7),
};

/**
 * BIT[0:7] DAC right channel pga gain low byte
 */
#define   reg_codec_pga_r_1          REG_ADDR8(REG_AUDIO_CODEC_BASE+0x0c)
enum
{
    FLD_AUDIO_PGA_GAIN_R_L         =  BIT_RNG(0,7),
};
/**
 * BIT[0:2] DAC right channel pga gain high byte
 * BIT[3]   DAC right channel soft mute   1'b0: disable,1'b1: enable
 */
#define   reg_codec_pga_r_2          REG_ADDR8(REG_AUDIO_CODEC_BASE+0x0d)
enum
{
    FLD_AUDIO_PGA_GAIN_R_H         =  BIT_RNG(0,2),
    FLD_AUDIO_PGA_MUTE_R             =  BIT(3),
};
/**
 * BIT[0:7] DAC left channel pga gain low byte
 */
#define   reg_codec_pga_l_1          REG_ADDR8(REG_AUDIO_CODEC_BASE+0x0e)
enum
{
    FLD_AUDIO_PGA_GAIN_L_L         =  BIT_RNG(0,7),
};
/**
 * BIT[0:2] DAC left channel pga gain high byte
 * BIT[3]   DAC left channel soft mote  default 0, 1'b0: disable, 1'b1: enable
 *
 * BIT[6]   cic filter compensition enable 1'b0: disable,  1'b1: enable
 */
#define   reg_codec_pga_l_2          REG_ADDR8(REG_AUDIO_CODEC_BASE+0x0f)
enum
{
    FLD_AUDIO_PGA_GAIN_L_H         =  BIT_RNG(0,2),
    FLD_AUDIO_PGA_MUTE_L           =  BIT(3),
    FLD_AUDIO_DSM_GCOEF_SEL        =  BIT(4),
    FLD_AUDIO_DSM_OPT_SEL          =  BIT(5),
    FLD_AUDIO_CIC_COMP_EN          =  BIT(6),
};

#define   reg_codec_alc_ctr5          REG_ADDR8(REG_AUDIO_CODEC_BASE+0x25)
enum
{
    FLD_AUDIO_CODEC_DAC_L_INV        =  BIT(0),
    FLD_AUDIO_CODEC_DAC_R_INV        =  BIT(1),
    FLD_AUDIO_CODEC_INT_GAIN         =  BIT(2),       // dac 1:+6dB
    FLD_AUDIO_CODEC_DEC_GAIN         =  BIT_RNG(4,5), // 1:+6dB,2:+12dB,3:+18dB only dec0
    FLD_AUDIO_CODEC_DIGMIX           =  BIT(7),

};


#define   reg_codec_ana_reg0         REG_ADDR8(REG_AUDIO_CODEC_BASE+0x10)
enum
{
    FLD_AUDIO_PGA_VOL_IN_L        =  BIT_RNG(0,3),
};

/**
 * BIT[0] mute the input gain stage of PGA_L
 * BIT[1] PGA_L is mute
 * BIT[2] the left PGA S2D buffer is power down
 * BIT[3] the left PGA boost stage is power down
 * BIT[4] the left INPPGA is power down
 */
#define   reg_codec_ana_reg1         REG_ADDR8(REG_AUDIO_CODEC_BASE+0x11)

/**
 * BIT[0] mute the input gain stage of PGA_R
 * BIT[1] PGA_R is mute
 * BIT[2] the right PGA S2D buffer is power down
 * BIT[3] the right PGA boost stage is power down
 * BIT[4] the right INPPGA is power down
 */
#define   reg_codec_ana_reg12         REG_ADDR8(REG_AUDIO_CODEC_BASE+0x1c)
enum
{
    FLD_AUDIO_PD_PGABUF            =  BIT(2),
    FLD_AUDIO_PD_PGABOOST          =  BIT(3),
    FLD_AUDIO_PD_INPPGA            =  BIT(4),
};

/**
 * BIT[6] power down ASDM_L
 */
#define   reg_codec_ana_reg2         REG_ADDR8(REG_AUDIO_CODEC_BASE+0x12)
/**
 * BIT[6] power down ASDM_R
 */
#define   reg_codec_ana_reg13        REG_ADDR8(REG_AUDIO_CODEC_BASE+0x1d)
enum
{
    FLD_AUDIO_PD_ASDM            =  BIT(6),
};

/**
 * BIT[0]   power down left  channel  PA
 * BIT[1]   power down right channel  PA
 * BIT[2]   power down left  channel  DAC
 * BIT[3]   power down right channel  DAC
 * BIT[4:5] DAC bias current ctrl for channels:2'b00:default 2'b01:0.5x 2'b10:0.75x 2'b11:1.25x
 */
#define   reg_codec_ana_reg3         REG_ADDR8(REG_AUDIO_CODEC_BASE+0x13)
enum
{
    FLD_AUDIO_PD_PA_L              =  BIT(0),
    FLD_AUDIO_PD_PA_R              =  BIT(1),
    FLD_AUDIO_PD_DAC_L             =  BIT(2),
    FLD_AUDIO_PD_DAC_R             =  BIT(3),
    FLD_AUDIO_PD_SCF_CTL           =  BIT_RNG(4,5),
};

/**
 * BIT[0:1] VMDSEL:Vmid impedance select 2'b0:472k/2;2'b1:236k/2;2'd2:30k/2;2'd3:6k/2
 * BIT[2:3] PD_VMID:power down VMID
 * BIT[5:6] IBSEL: codec current select. 2'd0:x4/3;2'd1:default;2'd2:x6/3;2'd3:x2/3(default)
 */
#define   reg_codec_ana_reg4         REG_ADDR8(REG_AUDIO_CODEC_BASE+0x14)

/**
 *  BIT[0]  LVEN : DAC/DRV/Vrefbuf lbias current select
 *          LVEN   IBSEL(reg_codec_ana_reg4 )       lb3u
             0       11                             3.07uA(default)
             1       11                             5.0uA
             1       01                             3.0uA
             1       00                             3.8uA
   BIT[2]  PDBIAS
 */
#define   reg_codec_ana_reg5         REG_ADDR8(REG_AUDIO_CODEC_BASE+0x15)


/**
 * BIT[0] L_HP_ctrl_vref  1'b1:Enable high-performance Vref_buf. default 1
   BIT[1] L_INT_vref 1    1'b1:Bypass the Vref_buf LPF filter.
   BIT[2] L_LP_ctrl_vref  1'b1:enable low-performance Vref_buf.
   BIT[3] L_LP_ctrl_dacL  1'b1:enable low-performance left DAC.
   BIT[4] L_LP_ctrl_dacR  1'b1:enable low-performance right DAC.
   BIT[5] L_LP_ctrl_drvL  1'b1:enable low-performance left driver.
   BIT[6] L_LP_ctrl_drvR  1'b1:enable low-performance right driver.
 */

#define   reg_codec_ana_reg6         REG_ADDR8(REG_AUDIO_CODEC_BASE+0x16)
enum
{
    FLD_AUDIO_HP_VREF_BUF          =  BIT(0),
    FLD_AUDIO_INT_VREF             =  BIT(1),
    FLD_AUDIO_LP_VREF_BUF          =  BIT(2),
    FLD_AUDIO_LP_L_DAC             =  BIT(3),
    FLD_AUDIO_LP_R_DAC             =  BIT(4),
    FLD_AUDIO_LP_L_DRV             =  BIT(5),
    FLD_AUDIO_LP_R_DRV             =  BIT(6),
};
/**
 * BIT[0] L_FRC_ON_DRVL   Force turn on left  DRV control signal. 1'b1, force on DRV enable; 1'b0, disable force on function.
 * BIT[1] L_FRC_ON_DRVR   Force turn on right DRV control signal. 1'b1, force on DRV enable; 1'b0, disable force on function.  "
 * BIT[2] L_HP_ctrl_drvL  1'b1:enable high-performance left  driver.
 * BIT[3] L_HP_ctrl_drvR  1'b1:enable high-performance right driver.
 */
#define   reg_codec_ana_reg7         REG_ADDR8(REG_AUDIO_CODEC_BASE+0x17)
enum
{
    FLD_AUDIO_FRC_ON_DRVL          =  BIT(0),
    FLD_AUDIO_FRC_ON_DRVR          =  BIT(1),
    FLD_AUDIO_HP_CTRL_DRV_L        =  BIT(2),
    FLD_AUDIO_HP_CTRL_DRV_R        =  BIT(3),
};

/**
 * BIT[0:2]  ldo_adc_trim  change Vmid
 * BIT[3]    ldo_adc_en
 */
#define   reg_codec_ana_reg8         REG_ADDR8(REG_AUDIO_CODEC_BASE+0x18)

/**
 * BIT[0:2]  dac_adc_trim
 * BIT[3]    ldo_dac_en
 */
#define   reg_codec_ana_reg9         REG_ADDR8(REG_AUDIO_CODEC_BASE+0x19)

/**
 * BIT[0:2]  dac_pa_trim
 * BIT[3]    ldo_pa_en
 */
#define   reg_codec_ana_reg10        REG_ADDR8(REG_AUDIO_CODEC_BASE+0x1a)
/**
 * BIT[0:3]  PGAVOL_INR
 * BIT[4]    LVEN_ADC    ADC/Micbias  lbias current select
 * BIT[5:6]  IBSEL_ADC:  ADC/Micbias  lbias current select
 *           LVEN_ADC   IBSEL_ADC     lb3u
             0            11          3.07uA(default)
             0            10          1.7uA
             0            01          2.18uA
             0            00          2.54uA
             1            11          5.0uA
             1            10          2.18uA
             1            01          3.0uA
             1            00          3.8uA
   BIT[7]    PDBIAS Power-down BIAS for ADC
 */
#define   reg_codec_ana_reg11        REG_ADDR8(REG_AUDIO_CODEC_BASE+0x1b)
enum
{
    FLD_AUDIO_PGA_VOL_IN_R        =  BIT_RNG(0,3),
};
 /**
  * BIT[2]   Enabling FLD_AUDIO_CODEC_R_NEG bit is valid for both stream0 and stream1.
  * If this bit is 0 and the DMIC sampling rate is 44.1k and 48k,the left and right channel data will be exchanged.
 **/

#define   reg_codec_mic_ctr          REG_ADDR8(REG_AUDIO_CODEC_BASE+0x28)
enum
{
    FLD_AUDIO_CODEC_DEC0_CH0_EN        =  BIT(0),
    FLD_AUDIO_CODEC_DEC0_CH1_EN        =  BIT(1),
    FLD_AUDIO_CODEC_R_NEG              =  BIT(2),
    FLD_AUDIO_CODEC_MIC_SEL            =  BIT(3),
};

#define   reg_codec_adc_1m_clk_ctr          REG_ADDR8(REG_AUDIO_CODEC_BASE+0x29)
enum
{
    FLD_AUDIO_CODEC_CLK_1M_EN         =  BIT(0),
    FLD_AUDIO_CODEC_CLK_1M_SEL        =  BIT(1),// 1:6M  0:1M
    FLD_AUDIO_CODEC_DEC_SFT_CTRL      =  BIT_RNG(2,3),
    FLD_AUDIO_CODEC_SOFTMUTE_MODE_SEL =  BIT(4),
    FLD_AUDIO_CODEC_ANA_PA_CLK_6M_EN  =  BIT(5),
    FLD_AUDIO_CODEC_DMIC_CLK_MODE     =  BIT(6),
};

#define   reg_codec_dec1_ctr0          REG_ADDR8(REG_AUDIO_CODEC_BASE+0x2B)
enum
{
    FLD_AUDIO_CODEC_DEC1_EN            =  BIT(0),
    FLD_AUDIO_CODEC_CLK1_EN            =  BIT(1),
    FLD_AUDIO_CODEC_DEC1_CH0_EN        =  BIT(2),
    FLD_AUDIO_CODEC_DEC1_CH1_EN        =  BIT(3),
    FLD_AUDIO_CODEC_DEC1_SWAP          =  BIT(4),
};


#define   reg_codec_dec1_ctr1          REG_ADDR8(REG_AUDIO_CODEC_BASE+0x2C)
enum
{
    FLD_AUDIO_CODEC_DEC1_CLK_SR        =  BIT_RNG(0,4),
};

#define   i2s_timer_th0          REG_ADDR8(REG_AUDIO_CODEC_BASE+0x5C)
#define   i2s_timer_th1          REG_ADDR8(REG_AUDIO_CODEC_BASE+0x5D)
#define   i2s_timer_th2          REG_ADDR8(REG_AUDIO_CODEC_BASE+0x5E)
#define   i2s_timer_th3          REG_ADDR8(REG_AUDIO_CODEC_BASE+0x5F)

#endif
