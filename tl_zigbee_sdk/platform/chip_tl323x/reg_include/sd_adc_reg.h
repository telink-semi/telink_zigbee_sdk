/********************************************************************************************************
 * @file    sd_adc_reg.h
 *
 * @brief   This is the header file for tl323x
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
#ifndef SD_ADC_REG_H
#define SD_ADC_REG_H
#include "soc.h"

/**********************************************************************************************************************
 *                                         Analog  registers are as follows                                           *
 **********************************************************************************************************************/





#define areg_0x10f       0x10f
enum
{
    FLD_PD_BUFFER_3P3V                 = BIT_RNG(0,4),
    FLD_AUDIO_PD_BIAS                   = BIT(6),
    FLD_AUDIO_PD_ADC                    = BIT(5),
    FLD_AUDIO_PD_VMID                   = BIT(7),
};


#define areg_0x10c         0x10c
enum{
    FLD_LDO_FORCE_ANA                  = BIT(0),
    FLD_XO_EN_CLK_ANA                  = BIT(1), //xtal_24M clock to analog   0:turn off  1:turn on   default :0
    FLD_ADC_TEST                       = BIT_RNG(2,4),
    FLD_PD_BPLL_OUT48M_1P05V           = BIT(5),
    FLD_EN_VBAT                        = BIT(6),
    FLD_L_DEM_EN                       = BIT(7),
};


#define areg_0x10d                      0x10d
enum{
    FLD_L_FS6M_INV_EN                    = BIT(0),
     FLD_L_LV_EN                         = BIT(1), //Adjust output current in CODEC_BIAS
     FLD_L_PWR_OTA1                       = BIT_RNG(2,3),
     FLD_L_PWR_OTA2                       = BIT_RNG(4,5),
     FLD_L_PWR_OTA3                       = BIT_RNG(6,7),
};

#define areg_0x10e       0x10e
enum{
    FLD_L_PWR_RVB                        = BIT_RNG(0,1),
    FLD_L_PWR_ADC_BUFFER                 = BIT_RNG(2,3),
    FLD_L_LOWPWR_BUFFER_3P3V             = BIT(4),
    FLD_L_ADC_CORE_HALFL                 = BIT(5),
     FLD_L_PD_BUFFER                     = BIT_RNG(6,7),
};


#define areg_sel_ana_input     0x111
enum{
    FLD_SEL_ANA_INPUT_P                   =  BIT_RNG(0,3),
    FLD_SEL_ANA_INPUT_N                   =  BIT_RNG(4,7),
};

#define areg_sel_ana_input_div     0x112
enum{
    FLD_SEL_ANA_INPUT_P_DIV                   =  BIT_RNG(0,1),
    FLD_SEL_ANA_INPUT_N_DIV                   =  BIT_RNG(2,3),
    FLD_SEL_VBAT_DIV                          =  BIT_RNG(4,5),
    FLD_L_SEL_DIVOUT_P                        = BIT_RNG(6,7),
};

enum {
    FLD_DIVOUT_P_VBAT  = 0x02,
    FLD_DIVOUT_P_GPIO  = 0x01,
    FLD_DIVOUT_P_TEMP  = 0x00,
};

#define areg_dc1_sel_ana_input     0x110
enum{
    FLD_DC1_SEL_ANA_INPUT_P                   =  BIT_RNG(0,3),
    FLD_DC1_SEL_ANA_INPUT_N                   =  BIT_RNG(4,7),
};

/**********************************************************************************************************************
 *                                         Digital registers are as follow                                            *
 **********************************************************************************************************************/
#define DC_BASE   0x80140680

#define reg_dfifo_aidx2            REG_ADDR8(DC_BASE + 0x03)
enum{
    FLD_R_DEC   = BIT_RNG(4,6),
};


#define reg_dfifo_dc_clk_div       REG_ADDR8(DC_BASE + 0x04)
enum{
    FLD_DFIFO_DC_CLK_DIV    = BIT_RNG(0,6),
};

#define reg_dc_mode_config      REG_ADDR8(DC_BASE + 0x05)
enum{
    FLD_OP_MODE = BIT_RNG(0,2),
    FLD_DC_DROP_NUM     = BIT_RNG(4,7),
};

#define reg_rx2_wptr_en  REG_ADDR8(DC_BASE + 0x06)
enum{
    RX2_WPTR_EN = BIT(0),
};

#define reg_dfifo_dc_mode            REG_ADDR8(DC_BASE + 0x8)
enum{
    FLD_R_MASK  = BIT(7),
};

#define reg_rxfifo2_th_l           REG_ADDR8(DC_BASE + 0x9)
enum{
    FLD_RXFIFO2_TH_L    = BIT_RNG(0,7),
};

#define reg_rxfifo2_th_h           REG_ADDR8(DC_BASE + 0xa)
enum{
    FLD_RXFIFO2_TH_H    = BIT_RNG(0,7),
};


#define reg_irq_fifo             REG_ADDR8(DC_BASE + 0xb)
enum{
    FLD_R_IRQ   = BIT(3),
    FLD_FIFO2_MID   = BIT(7),
};

#define reg_rxfifo2_trig_num          REG_ADDR8(DC_BASE + 0xc)
enum{
    FLD_RXFIFO2_TRIG_NUM = BIT_RNG(0,4),
};

#define reg_rxfifo2_clr         REG_ADDR8(DC_BASE + 0xd)
enum{
    FLD_RXFIFO2_CLR_PCLK =  BIT(0),
};

#define reg_rxfifo2_st         REG_ADDR8(DC_BASE + 0xe)
enum{
    FLD_RXFIFO2_CLR_BAK = BIT(0),
    FLD_RCFIFO2_OVERRUN  = BIT(4),
};


#define reg_rxfifo2_num         REG_ADDR8(DC_BASE + 0xf)
enum{
    FLD_RXFIFO2_NUM = BIT_RNG(0,4),
};

#define reg_rxfifo2_max         REG_ADDR16(DC_BASE + 0x12)
enum{
    FLD_RXFIFO2_MAX_L = BIT_RNG(0,7),
    FLD_RXFIFO2_MAX_H = BIT_RNG(8,15),
};


#define SD_ADC_FIFO          (DC_BASE + 0x18)

#define reg_rxfifo2_r_dat0       REG_ADDR32(DC_BASE + 0x18)
enum{
    FLD_RXFIFO2_R_DAT0 = BIT_RNG(0,7),
    FLD_RXFIFO2_R_DAT1 = BIT_RNG(8,15),
    FLD_RXFIFO2_R_DAT2 = BIT_RNG(16,23),
    FLD_RXFIFO2_R_DAT3 = BIT_RNG(24,31),
};

#define reg_dfifo_n2         REG_ADDR16(DC_BASE + 0x20)
enum{
    FLD_N2L = BIT_RNG(0,7),
    FLD_N2H = BIT_RNG(8,15),
};

#define reg_cnt_num_all         REG_ADDR8(DC_BASE + 0x22)
enum{
    FLD_CNT_NUM_ALL = BIT_RNG(0,7),
};

#define reg_cnt_num_switch        REG_ADDR8(DC_BASE + 0x23)
enum{
    FLD_CNT_NUM_SWITCH = BIT_RNG(0,7),
};


#endif
