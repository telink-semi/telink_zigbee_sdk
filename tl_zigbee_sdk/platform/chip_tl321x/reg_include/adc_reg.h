/********************************************************************************************************
 * @file    adc_reg.h
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
#ifndef ADC_REG_H
#define ADC_REG_H
#include "soc.h"

/**********************************************************************************************************************
 *                                         Analog  registers are as follows                                           *
 **********************************************************************************************************************/

#define areg_adc_clk_setting  0x87
enum
{
    FLD_CLK_24M_TO_SAR_EN = BIT(0),
};

#define areg_adc_res_m                  0xec
enum{
    FLD_ADC_RES_M         =             BIT_RNG(0,1),
    FLD_ADC_EN_DIFF_CHN_M =             BIT(6),
};

#define areg_adc_data_transfer_control  0xf2
enum{
    FLD_AUTO_NOT_EN         = BIT(3),
};

#define areg_adc_data_sample_control    0xf3
enum{
    FLD_NOT_SAMPLE_ADC_DATA         = BIT(0),//hold mode
    FLD_DWA_EN_O                    = BIT(1),
    FLD_ANA_RD_EN                   = BIT(2),
};
#define areg_adc_sample_clk_div     0xf4
enum{
    FLD_ADC_SAMPLE_CLK_DIV  =          BIT_RNG(0,3),
};

#define areg_m_chn_data_valid_status                0xf6
enum{
    FLD_M_CHN_DATA_VALID_STATUS            =BIT(0),
};

/** The areg_adc_misc register stores only the data for the M channel
 *  and will not store the data for the L and R channels.
 */
#define areg_adc_misc_l                 0xf7
#define areg_adc_misc_h                 0xf8


#define areg_ain_scale                  0xfa
enum{
    FLD_ADC_ITRIM_PREAMP    =   BIT_RNG(0,1),
    FLD_ADC_ITRIM_VREFBUF   =   BIT_RNG(2,3),
    FLD_ADC_ITRIM_VCMBUF    =   BIT_RNG(4,5),
};

#define areg_adc_pga_ctrl               0xfc
enum{
    FLD_SAR_ADC_POWER_DOWN      = BIT(5),
};

#define areg_temp_sensor_ctrl  0x06
enum{
    FLD_TEMP_SENSOR_POWER_DOWN = BIT(2),
};

/**********************************************************************************************************************
 *                                         Digital registers are as follow                                            *
 **********************************************************************************************************************/

#define ADC_BASE_ADDR 0x1401C0

#define reg_adc_r_mux(i)        REG_ADDR8(ADC_BASE_ADDR + (i))
enum{
    FLD_P_INPUT_PIN = BIT_RNG(0,3),
    FLD_N_INPUT_PIN = BIT_RNG(4,7),
};

#define reg_adc_tsamp   REG_ADDR8(ADC_BASE_ADDR + 0x03)
enum{
    FLD_M_TSAMP = BIT_RNG(0,3),
    FLD_L_TSAMP = BIT_RNG(4,7),
};

#define reg_adc_r_tsamp  REG_ADDR8(ADC_BASE_ADDR + 0x04)
enum{
    FLD_R_TSAMP = BIT_RNG(0,3),
};

#define reg_adc_vabt_div    REG_ADDR8(ADC_BASE_ADDR + 0x05)
enum{
    FLD_M_VBAT_DIV = BIT_RNG(0,1),
    FLD_L_VBAT_DIV = BIT_RNG(2,3),
    FLD_R_VBAT_DIV = BIT_RNG(4,5),
};

#define reg_adc_channel_set_state(i)        REG_ADDR8(ADC_BASE_ADDR + 0x06 + (i))
enum{
    FLD_R_MAX_S = BIT_RNG(0,3),
    FLD_SEL_AI_SCALE = BIT_RNG(4,5),
    FLD_SEL_VREF = BIT_RNG(6,7),
};

#define reg_adc_rng_set_state        REG_ADDR8(ADC_BASE_ADDR + 0x09)

#define reg_adc_capture_state(i)    REG_ADDR16(ADC_BASE_ADDR + 0x0a + (i)*0x02)
enum{
    FLD_R_MAX_C_L = BIT_RNG(0,7),
    FLD_R_MAX_C_H = BIT_RNG(8,9),
};

#define reg_adc_rng_capture_state   REG_ADDR16(ADC_BASE_ADDR + 0x10)

#define reg_adc_config0     REG_ADDR8(ADC_BASE_ADDR + 0x28)
enum{
    FLD_SCANT_MAX = BIT_RNG(4,6),
};

#define reg_adc_config1     REG_ADDR8(ADC_BASE_ADDR + 0x29)
enum{
    FLD_SAR_ADC_CLK_DIV = BIT_RNG(0,3),
    FLD_ADC_MODE = BIT(5),
};

#define reg_adc_config2     REG_ADDR8(ADC_BASE_ADDR + 0x2a)
enum{
    FLD_M_CHANNEL_EN = BIT(0),
    FLD_L_CHANNEL_EN = BIT(1),
    FLD_R_CHANNEL_EN = BIT(2),
    FLD_RX_DMA_ENABLE = BIT(4),
    FLD_CLK_EN = BIT(5),
    FLD_SAR_RX_INTERRUPT_ENABLE = BIT(6),
    FLD_TRIG_MODE = BIT(7),
};

#define reg_adc_rxfifo_trig_num     REG_ADDR8(ADC_BASE_ADDR + 0x2b)
enum{
    FLD_RXFIFO_TRIG_NUM = BIT_RNG(0,2),
    FLD_BUF_CNT = BIT_RNG(4,7),
};

#define reg_adc_rxfifo_dat(i)      REG_ADDR16(ADC_BASE_ADDR + 0x2c + 2*(i))
#define SAR_ADC_FIFO            (ADC_BASE_ADDR+0x2c)

#define reg_soft_control        REG_ADDR8(ADC_BASE_ADDR + 0x30)
enum{
    FLD_SAR_IRQ_RX_STATUS = BIT(0),
    FLD_FIFO_CLR = BIT(1),
    FLD_TRIG_START = BIT(2),
    FLD_SOFT_START = BIT(7),
};
#define reg_sample_times      REG_ADDR8(ADC_BASE_ADDR + 0x31)
enum{
    FLD_PEM_TASK_ENABLE    = BIT(0),
    FLD_PEM_EVENT0_ENABLE  = BIT(1),
    FLD_PEM_EVENT1_ENABLE  = BIT(2),
};

#endif
