/********************************************************************************************************
 * @file    adc.h
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
/** @page ADC
 *
 *  Introduction
 *  ===============
 *  TL321X supports hardware ADC function.
 *
 *  API Reference
 *  ===============
 *  Header File: adc.h
 */
#pragma once

#include "dma.h"
#include "audio.h"
#include "compiler.h"
#include "gpio.h"
#include "reg_include/register.h"

#ifndef INTERNAL_TEST_FUNC_EN
    #define INTERNAL_TEST_FUNC_EN 0 //only for internal test
#endif

extern unsigned char g_adc_rx_fifo_index;

typedef enum
{
    ADC_VREF_0P9V = 0x01, //Only for internal testing,not recommended.
    ADC_VREF_1P2V = 0x02,
} adc_ref_vol_e;

typedef enum
{
    ADC_VBAT_DIV_OFF = 0,
    ADC_VBAT_DIV_1F4 = 0x01,
} adc_vbat_div_e;

typedef enum
{
    NOINPUTN           = 0,
    ADC_GPIO_PB0N      = 0x01,
    ADC_GPIO_PB1N      = 0x02,
    ADC_GPIO_PB2N      = 0x03,
    ADC_GPIO_PB3N      = 0x04,
    ADC_GPIO_PB4N      = 0x05,
    ADC_GPIO_PB5N      = 0x06,
    ADC_GPIO_PB6N      = 0x07,
    ADC_GPIO_PB7N      = 0x08,
    ADC_GPIO_PD0N      = 0x09,
    ADC_GPIO_PD1N      = 0x0a,
    ADC_TEMPSENSORN_EE = 0x0e,
    GND                = 0x0f,
} adc_input_nch_e;

typedef enum
{
    NOINPUTP           = 0,
    ADC_GPIO_PB0P      = 0x01,
    ADC_GPIO_PB1P      = 0x02,
    ADC_GPIO_PB2P      = 0x03,
    ADC_GPIO_PB3P      = 0x04,
    ADC_GPIO_PB4P      = 0x05,
    ADC_GPIO_PB5P      = 0x06,
    ADC_GPIO_PB6P      = 0x07,
    ADC_GPIO_PB7P      = 0x08,
    ADC_GPIO_PD0P      = 0x09,
    ADC_GPIO_PD1P      = 0x0a,
    ADC_TEMPSENSORP_EE = 0x0e,
    ADC_VBAT           = 0x0f,
} adc_input_pch_e;

/**
 * @brief adc input pin type
 * |           |              |
 * | :-------- | :----------- |
 * |   <15:12> |    <11:0>    |
 * |adc channel|    gpio pin  |
 */
typedef enum
{
    ADC_GPIO_PB0 = GPIO_PB0 | (0x1 << 12),
    ADC_GPIO_PB1 = GPIO_PB1 | (0x2 << 12),
    ADC_GPIO_PB2 = GPIO_PB2 | (0x3 << 12),
    ADC_GPIO_PB3 = GPIO_PB3 | (0x4 << 12),
    ADC_GPIO_PB4 = GPIO_PB4 | (0x5 << 12),
    ADC_GPIO_PB5 = GPIO_PB5 | (0x6 << 12),
    ADC_GPIO_PB6 = GPIO_PB6 | (0x7 << 12),
    ADC_GPIO_PB7 = GPIO_PB7 | (0x8 << 12),
    ADC_GPIO_PD0 = GPIO_PD0 | (0x9 << 12),
    ADC_GPIO_PD1 = GPIO_PD1 | (0xa << 12),
} adc_input_pin_def_e;

typedef enum
{
    ADC_GPIO_MODE,
    ADC_VBAT_MODE,
} adc_input_pin_mode_e;

/**
 * @brief adc sample channel type.
 * @attention
 *  #1 Only M channel can be used in NDMA mode, multiple channels can be used in DMA mode.
 *  #2 When using one channels in DMA mode, only M channel can be selected.
 *  #3 When using two channels in DMA mode, only M and L channels can be selected.
 */
typedef enum
{
    ADC_M_CHANNEL,
    ADC_L_CHANNEL,
    ADC_R_CHANNEL,
} adc_sample_chn_e;

/**
 * @brief adc channel cnt type.
*|             |             |
*| :--------   | :-----------|
*|   <7:4>     |    <3:0>    |
*|transfer_mode| channel_cnt |
**/
typedef enum
{
    NDMA_M_CHN    = 1 | (0 << 4),
    DMA_M_CHN     = 1 | (1 << 4),
#if INTERNAL_TEST_FUNC_EN
    DMA_M_L_CHN   = 2 | (1 << 4),
    DMA_M_L_R_CHN = 3 | (1 << 4),
#endif
} adc_chn_cnt_e;

/**
 * @brief enumeration of adc resolutions.
 * @note  #1 The adc performs best when set to ADC_RES12 by default, and it is not recommended to change this configuration.
 *        #2 The sign and valid bits corresponding to adc code for setting different resolutions are as follows:
 *           If ADC_RES8 is set, the sign bit of adc code is bit11 and bit0-6 are valid bits.
 *           If ADC_RES10 is set, the sign bit of adc code is bit11 and bit0-8 are valid bits.
 *           If ADC_RES12 is set, the sign bit of adc code is bit11 and bit0-10 are valid bits (default)
 *        #3 Regardless of the set resolution, the final output code value is always 16bit.
 */
typedef enum
{
    ADC_RES8  = 0,
    ADC_RES10 = 0x01,
    ADC_RES12 = 0x02,
} adc_res_e;

typedef enum
{
    ADC_SAMPLE_CYC_3,
    ADC_SAMPLE_CYC_6,
    ADC_SAMPLE_CYC_9,
    ADC_SAMPLE_CYC_12,
    ADC_SAMPLE_CYC_15,
    ADC_SAMPLE_CYC_18,
    ADC_SAMPLE_CYC_21,
    ADC_SAMPLE_CYC_24,
    ADC_SAMPLE_CYC_27,
    ADC_SAMPLE_CYC_30,
    ADC_SAMPLE_CYC_33,
    ADC_SAMPLE_CYC_36,
    ADC_SAMPLE_CYC_39,
    ADC_SAMPLE_CYC_42,
    ADC_SAMPLE_CYC_45,
    ADC_SAMPLE_CYC_48,
} adc_sample_cycle_e;

/**
 * @brief adc sample frequency type.
 * |         |           |             |             |
 * | :-------|  :--------| :--------   | :-----------|
 * | <25:24> |  <23:8>   |   <7:4>     |    <3:0>    |
 * |  flag   |  capture  | sample_cycle|     set     |
 *
 */
typedef enum
{
    /**
     * In NDMA mode, configure the sample cycle corresponding to each sampling frequency to the maximum value and delay >30us after adc power on to wait for adc to stabilize,
     * to solve the problem of the first code abnormality after adc power on.(jira:BUT-8) (Confirmed by haitao.gu, added by xiaobin.huang at 20240903)
     */
    ADC_SAMPLE_FREQ_23K  = (0 << 24) | (1023 << 8) | (ADC_SAMPLE_CYC_48 << 4) | 15,
    ADC_SAMPLE_FREQ_48K  = (1 << 24) | (490 << 8) | (ADC_SAMPLE_CYC_48 << 4) | 10,
    ADC_SAMPLE_FREQ_96K  = (2 << 24) | (240 << 8) | (ADC_SAMPLE_CYC_27 << 4) | 10,
    ADC_SAMPLE_FREQ_192K = (3 << 24) | (115 << 8) | (ADC_SAMPLE_CYC_6 << 4) | 10,//When you select ADC_SAMPLE_FREQ_192K, the first code of the vbat sample mode is an exception and needs to be discarded.
} adc_sample_freq_e;

typedef enum
{
    ADC_PRESCALE_1 = 0x00,
    //  ADC_PRESCALE_1F2 = 0x01,//Only for internal testing
    ADC_PRESCALE_1F4 = 0x02,
    //  ADC_PRESCALE_1F8 = 0x03,//Only for internal testing
} adc_pre_scale_e;

typedef struct
{
    adc_vbat_div_e    divider;
    adc_ref_vol_e     v_ref;
    adc_pre_scale_e   pre_scale;
    adc_sample_freq_e sample_freq;
    adc_input_pch_e   input_p;
    adc_input_nch_e   input_n;
} adc_chn_cfg_t;

typedef struct
{
    adc_ref_vol_e       v_ref;
    adc_pre_scale_e     pre_scale;
    adc_sample_freq_e   sample_freq;
    adc_input_pin_def_e pin;
} adc_gpio_cfg_t;

typedef enum
{
    DMA,
    NDMA,
} adc_transfer_mode_e;

/**********************************************************************************************************************
 *                                         DMA and NDMA common interface                                              *
 **********************************************************************************************************************/
/**
 * @brief    This function is used to power on sar_adc.
 * @return   none.
 * @note     -# User need to wait >100us after adc_power_on() for ADC to be stable.
 *           -# If you calling adc_power_off(), because all analog circuits of ADC are turned off after adc_power_off(),
 *            it is necessary to wait >30us after re-adc_power_on() for ADC to be stable.
 */
void adc_power_on(void);
/**
 * @brief      This function is used to power off sar_adc.
 * @return     none
 */
void adc_power_off(void);

/**
 * @brief This function serves to set input channel in differential_mode.
 * @param[in]  chn - enum variable of ADC sample channel.
 * @param[in]  p_ain - enum variable of ADC analog positive input pin.
 * @param[in]  n_ain - enum variable of ADC analog negative input pin.
 * @return none
 */
static inline void adc_set_diff_input(adc_sample_chn_e chn, adc_input_pch_e p_ain, adc_input_nch_e n_ain)
{
    analog_write_reg8(areg_adc_res_m, analog_read_reg8(areg_adc_res_m) | FLD_ADC_EN_DIFF_CHN_M);
    reg_adc_r_mux(chn) = (p_ain | (n_ain << 4));
}

/**
 * @brief This function is used to set IO port for ADC supply or ADC IO port voltage sampling.
 * @param[in]  mode - ADC gpio pin sample mode
 * @param[in]  pin - adc_input_pin_def_e ADC input gpio pin
 * @return none
 */
void adc_pin_config(adc_input_pin_mode_e mode, adc_input_pin_def_e pin);
/**
 * @brief This function is used to set two IO port configuration and set it as input channel of ADC difference IO port voltage sampling.
 * @param[in]  chn - enum variable of ADC sample channel.
 * @param[in]  p_pin - enum variable of ADC analog positive input IO.
 * @param[in]  n_pin - enum variable of ADC analog negative input IO.
 * @return none
 */
void adc_set_diff_pin(adc_sample_chn_e chn, adc_input_pin_def_e p_pin, adc_input_pin_def_e n_pin);

/**
 * @brief      This function serves to select Vbat voltage division factor.
 * @param[in]  chn - enum variable of ADC sample channel
 * @param[in]  vbat_div - enum variable of Vbat division factor.
 * @return     none
 */
void adc_set_vbat_divider(adc_sample_chn_e chn, adc_vbat_div_e vbat_div);

/**
 * @brief This function is used to initialize the ADC.
 * @param[in]  channel_cnt - transfer_mode and the number of channels used.
 * @return none
 * @attention Many features are configured in adc_init function. But some features
 *      such as adc digital clk, adc analog clk, resolution, we think better to set as default value,
 *      and user don't need to change them in most use cases.
 */
void adc_init(adc_chn_cnt_e channel_cnt);
/**
 * @brief This function is used to initialize the ADC for gpio sampling.
 * @param[in]  chn -the channel to be configured.
 * @param[in]  cfg -structure for configuring ADC channel.
 * @return none
 */
void adc_gpio_sample_init(adc_sample_chn_e chn, adc_gpio_cfg_t cfg);
/**
 * @brief This function is used to initialize the ADC for vbat sampling.
 * @param[in]  chn -structure for configuring ADC channel.
 * @return none
 */
void adc_vbat_sample_init(adc_sample_chn_e chn);

/**
 * @brief  This function is used to initialize the ADC for gpio sampling to indirectly sample the vbat voltage.
 * @param[in]  chn -the channel to be configured.
 * @param[in]  cfg -structure for configuring ADC channel.
 * @return none
 * @attention Only for 1.9V ~ 3.6V vbat power supply, if the vbat power supply > 3.6V, ADC_VBAT_SAMPLE must be selected.
 */
void adc_gpio_sample_vbat_init(adc_sample_chn_e chn, adc_gpio_cfg_t cfg);
#if INTERNAL_TEST_FUNC_EN
/**
 * @brief This function is used to initialize the ADC for Temperature Sensor sampling.
 * @param[in]  chn -structure for configuring ADC channel.
 * @return none
 */
void adc_temp_init(adc_sample_chn_e chn);
/**
 * @brief This function serves to calculate temperature from temperature sensor adc sample code.
 * @param[in]   adc_code            - the temperature sensor adc sample code.
 * @return      adc_temp_value      - the temperature value.
 * @attention   Temperature and adc_code are linearly related. We test four chips between -40~130 (Celsius) and got an average relationship:
 *          Temp =  564 - ((adc_code * 819)>>11),when Vref = 1.2V, pre_scale = 1.
 */
unsigned short adc_calculate_temperature(unsigned short adc_code);
#endif

/**
 * @brief This function serves to calculate voltage from adc sample code.
 * @param[in]   chn - enum variable of ADC sample channel.
 * @param[in]   adc_code    - the adc sample code(should be positive value.)
 * @return      adc_vol_mv  - the average value of adc voltage value(adc voltage value >= 0).
 */
unsigned short adc_calculate_voltage(adc_sample_chn_e chn, unsigned short adc_code);

/**
 * @brief       This function sets the threshold that triggers the DMA IRQ or SAR ADC RX IRQ.
 * @param[in]   trig_num -trigger threshold.
 * @return      none
 * @note        -# The DMA IRQ or SAR ADC RX IRQ is triggered when fifo_cnt> trig_cnt.
 *              -# unit:half word, range:0-7(the actual fifo depth is 8 words, but the register is only 3 bits, so it is limited to 0~7).
 *              -# The data of rx fifo is stored according to HALF word.
 *              -# In DMA mode, DMA is configured to transmit by word, so the trigger cnt must be set to an odd number, otherwise there will be incorrect data to the buffer.
 *              -# In DMA mode, it has been configured by default to 1 (greater than 1 half word, i.e., 1 word triggers a DMA request to the DMA once ).
 *              -# In NDMA mode, the threshold can be configured according to the usage situation.
 *              -# The threshold is recalculated after the dma has finished moving.
 */
static inline void adc_set_rx_fifo_trig_cnt(unsigned char trig_num)
{
    reg_adc_rxfifo_trig_num = ((reg_adc_rxfifo_trig_num & (~FLD_RXFIFO_TRIG_NUM)) | trig_num);
}

/**
 * @brief This function is used to calib ADC 1.2V vref for GPIO.
 * @param[in] vref - GPIO sampling calibration value.
 * @param[in] offset - GPIO sampling two-point calibration value offset.
 * @return none
 */
void adc_set_gpio_calib_vref(unsigned short vref, signed char offset);

/**
 * @brief This function is used to calib ADC 1.2V vref for Vbat.
 * @param[in] vref - Vbat channel sampling calibration value.
 * @param[in] offset - Vbat channel sampling two-point calibration value offset.
 * @return none
 */
void adc_set_vbat_calib_vref(unsigned short vref, signed char offset);

/**********************************************************************************************************************
 *                                                DMA only interface                                                  *
 **********************************************************************************************************************/
/**
 * @brief      This function serves to configure adc_dma_chn channel.
 * @param[in]  chn - the DMA channel
 * @return     none
 */
void adc_set_dma_config(dma_chn_e chn);

/**
 * @brief     The adc starts sampling in DMA mode.
 * @param[in] adc_data_buf  - Pointer to data buffer, it must be 4-bytes aligned address
 *                            and the actual buffer size defined by the user needs to be not smaller than the data_byte_len,
 *                            otherwise there may be an out-of-bounds problem.
 * @param[in] data_byte_len - Amount of data to be sampled in bytes, the maximum value can be up to 0xFFFFFC.
 * @note      The depth of SAR_ADC_FIFO is 8 words, and its characteristic is store first, fetch first.
 *            When all three channels are working at the same time, the order of storing adc channel data into SAR_ADC_FIFO is M: L: R,
 *            so the arrangement after dma handling is also M: L: R.
 * @return    none
 */
void adc_start_sample_dma(unsigned short *adc_data_buf, unsigned int data_byte_len);

/**
 * @brief     This function serves to get adc DMA irq status.
 * @return    0: the sample is in progress.
 *            !0: the sample is finished.
 * @note      The code is placed in the ram code section, in order to shorten the time.
 */
_attribute_ram_code_sec_noinline_ unsigned char adc_get_irq_status_dma(void);

/**
 * @brief     This function serves to clear adc DMA irq status.
 * @return    none
 * @note      The code is placed in the ram code section, in order to shorten the time.
 */
_attribute_ram_code_sec_noinline_ void adc_clr_irq_status_dma(void);


/**********************************************************************************************************************
 *                                                NDMA only interface                                                 *
 **********************************************************************************************************************/
/**
 * @brief This function serves to directly get an adc sample code from fifo.
 * @return  adc_code    - the adc sample code.
 *                      - Bit[11:15] of the adc code read from reg_adc_rxfifo_dat are sign bits,if the adc code is positive, bits [11:15] are all 1's,
 *                        if the adc code is negative, bits [11:15] are all 0's and valid data bits are Bit[0:10],the valid range is 0~0x7FF.
 */
unsigned short adc_get_raw_code(void);

/**
 * @brief     Get the irq status of ADC.
 * @retval    non-zero      -  the interrupt occurred.
 * @retval    zero  -  the interrupt did not occur.
 */
static inline unsigned char adc_get_irq_status(void)
{
    return reg_soft_control & FLD_SAR_IRQ_RX_STATUS;
}

/**
 * @brief     Clear the irq status of ADC.
 * @return    none
 * @note      The irq status of ADC will not be cleared automatically after reading out the data from the rxfifo, you need to clear manually.
 */
static inline void adc_clr_irq_status(void)
{
    reg_soft_control |= FLD_SAR_IRQ_RX_STATUS;
}

/**
 * @brief   This function is used to enable the transmission of data from the adc's M channel, L channel, and R channel to the sar adc rxfifo.
 * @return  none
 */
void adc_start_sample_nodma(void);

/**
 * @brief   This function is used to disable the transmission of data from the adc's M channel, L channel, and R channel to the sar adc rxfifo and clear rx fifo cnt and g_adc_rx_fifo_index.
 * @return  none
 */
void adc_stop_sample_nodma(void);

/**
 * @brief     Get the rxfifo cnt,when data enters rxfifo, the rxfifo cnt increases; when reading data from rx_fifo, rxfifo cnt decays.
 * @return    none
 * @note      After waking up from suspend mode,rxfifo will not be cleared.(confirmed by qiangkai.xu, added by xiaobin.huang at 20240903)
 */
static inline unsigned char adc_get_rxfifo_cnt(void)
{
    return ((reg_adc_rxfifo_trig_num & FLD_BUF_CNT) >> 4);
}

/**
 * @brief     Set the 'g_adc_rx_fifo_index' to 0,'g_adc_rx_fifo_index' is used to synchronize the rxfifo hardware pointer in NDMA mode.
 * @return    none.
 */
static inline void adc_clr_rx_index(void)
{
    g_adc_rx_fifo_index = 0;
}

/**
 * @brief     Clear the ADC rx fifo.
 * @return    none
 */
static inline void adc_clr_rx_fifo_cnt(void)
{
    reg_soft_control |= FLD_FIFO_CLR;
    adc_clr_rx_index();
}
