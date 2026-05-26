/********************************************************************************************************
 * @file    adc.c
 *
 * @brief   This is the source file for TL321X
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
#include "adc.h"
#include "audio.h"
#include "compiler.h"
#include "lib/include/stimer.h"
#define ADC_CHN_CNT                        3
#define ADC_GPIO_VREF_DEFAULT_VALUE        1230
#define ADC_GPIO_VREF_OFFSET_DEFAULT_VALUE 0

#define ADC_VBAT_VREF_DEFAULT_VALUE        1230
#define ADC_VBAT_VREF_OFFSET_DEFAULT_VALUE 0

_attribute_data_retention_sec_ unsigned short g_adc_vref[ADC_CHN_CNT] = {ADC_GPIO_VREF_DEFAULT_VALUE, ADC_VBAT_VREF_DEFAULT_VALUE, ADC_GPIO_VREF_DEFAULT_VALUE}; //default ADC ref voltage (unit:mV)
_attribute_data_retention_sec_ signed char    g_adc_vref_offset[ADC_CHN_CNT];                                                                                    //ADC calibration value voltage offset (unit:mV).
_attribute_data_retention_sec_ unsigned short g_adc_gpio_calib_vref        = ADC_GPIO_VREF_DEFAULT_VALUE;                                                        //ADC gpio calibration value voltage (unit:mV)(used for gpio voltage sample).
_attribute_data_retention_sec_ signed char    g_adc_gpio_calib_vref_offset = ADC_GPIO_VREF_OFFSET_DEFAULT_VALUE;                                                 //ADC gpio calibration value voltage offset (unit:mV)(used for gpio voltage sample).
_attribute_data_retention_sec_ unsigned short g_adc_vbat_calib_vref        = ADC_VBAT_VREF_DEFAULT_VALUE;                                                        //ADC vbat calibration value voltage (unit:mV)(used for vbat voltage sample).
_attribute_data_retention_sec_ signed char    g_adc_vbat_calib_vref_offset = ADC_VBAT_VREF_OFFSET_DEFAULT_VALUE;                                                 //ADC vbat calibration value voltage offset (unit:mV)(used for vbat voltage sample).

volatile unsigned char g_adc_pre_scale[ADC_CHN_CNT];
volatile unsigned char g_adc_vbat_divider[ADC_CHN_CNT];
unsigned char          g_adc_rx_fifo_index = 0;
unsigned char          g_channel_cnt       = 0;

dma_chn_e    adc_dma_chn;
dma_config_t adc_rx_dma_config =
    {
        .dst_req_sel    = 0,
        .src_req_sel    = DMA_REQ_SAR_ADC_RX,
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

/**********************************************************************************************************************
 *                                         DMA and NDMA common interface                                              *
 **********************************************************************************************************************/
/**
 * @brief      This function is used to reset sar_adc module.
 * @return     none
 */
static inline void adc_reset(void)
{
    reg_rst3 &= (~FLD_RST3_SARADC);
    reg_rst3 |= FLD_RST3_SARADC;
    adc_clr_rx_index();
}

/**
 * @brief      This function enable adc source clock: xtal_24M
 * @return     none
 */
static inline void adc_clk_en(void)
{
    reg_clk_en3 |= FLD_CLK3_SARADC_EN;
}

/**
 * @brief      This function disable ADC analog clock.
 * @return     none
 */
static inline void adc_clk_dis(void)
{
    reg_clk_en3 = reg_clk_en3 & (~FLD_CLK3_SARADC_EN);
}

/**
 * @brief      This function sets adc digital clock and analog clock.
 * @return     none
 * @attention  ADC digital clock from crystal(24MHz), analog clock from digital clock.
 *             The ADC digital clock and analog clock are fixed at 24 MHz and 4 MHz, respectively, and should not be modified.
 */
static inline void adc_set_clk(void)
{
    reg_adc_config1 = ((reg_adc_config1 & FLD_SAR_ADC_CLK_DIV) | 1); //div=1, adc digital clk = 24MHz/div.(crystal = 24MHz)
    analog_write_reg8(areg_adc_sample_clk_div, 5);                   //div=5, adc analog clk = 24MHz/(1+div) = 4M.
}

/**
 * @brief      This function sets ADC input channel as M channel,L channel or R channel.
 * @param[in]  chn - enum variable of ADC sample channel.
 * @return     none
 */
static inline void adc_set_chn_en(adc_sample_chn_e chn)
{
    reg_adc_config2 |= BIT(chn);
}

/**
 * @brief This function serves to set resolution.
 * @param[in]  res - enum variable of ADC resolution.
 * @return none
 */
static inline void adc_set_resolution(adc_res_e res)
{
    analog_write_reg8(areg_adc_res_m, (analog_read_reg8(areg_adc_res_m) & (~FLD_ADC_RES_M)) | res);
}

/**
 * @brief      This function serves to set ADC sample time(the number of adc clocks for sample cycles) for M channel,L channel or R channel.
 * @param[in]  chn - enum variable of ADC sample channel.
 * @param[in]  sample_cycle - enum variable of adc sample cycles.
 * @return     none
 */
static void adc_set_tsample_cycle(adc_sample_chn_e chn, adc_sample_cycle_e sample_cycle)
{
    switch (chn) {
    case ADC_M_CHANNEL:
        reg_adc_tsamp = ((reg_adc_tsamp & (~FLD_M_TSAMP)) | sample_cycle);
        break;
#if(COMPATIBLE_WITH_TL321X_AND_TL323X == 0)
    case ADC_L_CHANNEL:
        reg_adc_tsamp = ((reg_adc_tsamp & (~FLD_L_TSAMP)) | (sample_cycle << 4));
        break;
    case ADC_R_CHANNEL:
        reg_adc_r_tsamp = ((reg_adc_r_tsamp & (~FLD_R_TSAMP)) | sample_cycle);
        break;
#endif
    default:
        break;
    }
}

/**
 * @brief This function serves to set state and capture_state length.
 * @param[in]   chn - enum variable of ADC sample channel.
 * @param[in]   r_max_mc - Value of length of "capture" state for channel.
 * @param[in]   r_max_s - Value of length of "set" state for channel.
 * @return none
 */
static inline void adc_set_state_length(adc_sample_chn_e chn, unsigned short r_max_mc, unsigned char r_max_s)
{
    reg_adc_capture_state(chn)     = r_max_mc;
    reg_adc_channel_set_state(chn) = ((reg_adc_channel_set_state(chn) & (~FLD_R_MAX_S)) | r_max_s);
}

/**
 * @brief   This function is used to enable the transmission of data from the adc's M channel, L channel, and R channel to the sar adc rxfifo.
 * @return  none
 * @note    -# In DMA mode, must enable this function.
 *          -# In NDMA mode, if this function is not enabled, the adc code can only be read from the registers, but it will lead to the problem of repeatedly getting the same adc code when calling adc_get_raw_code() several times,
 *             if this function is enabled, the adc code can be read from the rx fifo, and this problem can be avoided, so it is also must enable it.
 */
static inline void adc_all_chn_data_to_fifo_en(void)
{
    analog_write_reg8(areg_adc_data_transfer_control, analog_read_reg8(areg_adc_data_transfer_control) & (~FLD_AUTO_NOT_EN));
}

/**
 * @brief   This function is used to disable the transmission of data from the adc's M channel, L channel, and R channel to the sar adc rxfifo.
 * @return  none
 */
static inline void adc_all_chn_data_to_fifo_dis(void)
{
    analog_write_reg8(areg_adc_data_transfer_control, FLD_AUTO_NOT_EN);
}

/**
 * @brief       This function is used to set the scan channel cnt.
 * @param[in]   chn_cnt - range:1-3
 * @return      none
 */
static inline void adc_set_scan_chn_cnt(unsigned char chn_cnt)
{
    reg_adc_config0 = ((reg_adc_config0 & (~FLD_SCANT_MAX)) | ((chn_cnt * 2) << 4)); //scan_cnt = chn_cnt*2
}
#if INTERNAL_TEST_FUNC_EN
/**
 * @brief       This function is used to enable the data weighted average algorithm function to improve ADC performance.
 * @return      none
 */
static inline void adc_data_weighted_average_en(void)
{
    analog_write_reg8(areg_adc_data_sample_control, (analog_read_reg8(areg_adc_data_sample_control) | FLD_DWA_EN_O));
}
#endif
/**
 * @brief      This function disable adc digital clock.
 * @return     none
 * @note       -# After disabling the digital clock, rxfifo will not be updated.
 *             -# When you don't need to keep the adc sampling, you can disable the digital clock for power saving purposes.
 */
static inline void adc_dig_clk_dis(void)
{
    reg_adc_config2 &= (~FLD_CLK_EN);
}

/**
 * @brief      This function enable adc digital clock.
 * @return     none
 */
static inline void adc_dig_clk_en(void)
{
    reg_adc_config2 |= FLD_CLK_EN;
}

/**
 * @brief      This function serves to stop the state machine at the beginning of the M channel and not start sampling.
 * @return     none
 */
static inline void adc_set_scan_chn_dis(void)
{
    /**
     * FLD_SCANT_MAX cannot be configured to 0, because 0 is the RNG channel, if RNG is not enabled (default is not enabled), the state machine will not stop at the RNG channel after setting 0, it will not stop.
     */
    reg_adc_config0 = ((reg_adc_config0 & (~FLD_SCANT_MAX)) | (1 << 4));
}

/**
 * @brief    This function is used to power on sar_adc.
 * @return   none.
 * @note     -# User need to wait >100us after adc_power_on() for ADC to be stable.
 *           -# If you calling adc_power_off(), because all analog circuits of ADC are turned off after adc_power_off(),
 *            it is necessary to wait >30us after re-adc_power_on() for ADC to be stable.
 */
void adc_power_on(void)
{
    /**
     * adc_set_scan_chn_dis() must be called to stop the state machine at the beginning of the M channel and not start sampling.
     */
    adc_set_scan_chn_dis();
    analog_write_reg8(areg_adc_pga_ctrl, (analog_read_reg8(areg_adc_pga_ctrl) & (~FLD_SAR_ADC_POWER_DOWN)));
    adc_dig_clk_en();
}

/**
 * @brief      This function is used to power off sar_adc.
 * @return     none
 */
void adc_power_off(void)
{
    analog_write_reg8(areg_adc_pga_ctrl, (analog_read_reg8(areg_adc_pga_ctrl) | FLD_SAR_ADC_POWER_DOWN));
}

/**
 * @brief This function is used to set IO port for ADC supply or ADC IO port voltage sampling.
 * @param[in]  mode - ADC gpio pin sample mode
 * @param[in]  pin - adc_input_pin_def_e ADC input gpio pin
 * @return none
 */
void adc_pin_config(adc_input_pin_mode_e mode, adc_input_pin_def_e pin)
{
    unsigned short adc_input_pin = pin & 0xfff;
    switch (mode) {
    case ADC_GPIO_MODE:
        gpio_function_en(adc_input_pin);
        gpio_input_dis(adc_input_pin);
        gpio_output_dis(adc_input_pin);
        gpio_set_low_level(adc_input_pin);
        break;
    case ADC_VBAT_MODE:
        gpio_function_en(adc_input_pin);
        gpio_input_dis(adc_input_pin);
        gpio_output_en(adc_input_pin);
        gpio_set_high_level(adc_input_pin);
        break;
    }
}

/**
 * @brief This function is used to set two IO port configuration and set it as input channel of ADC difference IO port voltage sampling.
 * @param[in]  chn - enum variable of ADC sample channel.
 * @param[in]  p_pin - enum variable of ADC analog positive input IO.
 * @param[in]  n_pin - enum variable of ADC analog negative input IO.
 * @return none
 */
void adc_set_diff_pin(adc_sample_chn_e chn, adc_input_pin_def_e p_pin, adc_input_pin_def_e n_pin)
{
    adc_pin_config(ADC_GPIO_MODE, p_pin);
    adc_pin_config(ADC_GPIO_MODE, n_pin);
    adc_set_diff_input(chn, p_pin >> 12, n_pin >> 12);
}

/**
 * @brief This function serves to set the reference voltage of the channel.
 * @param[in]  chn - enum variable of ADC sample channel.
 * @param[in]  v_ref - enum variable of ADC reference voltage.
 * @return none
 * @note       1. adc_set_ref_voltage does not take effect immediately after configuration, it needs to be delayed 100us after calling adc_dig_clk_en().
 *             2. If you call adc_set_ref_voltage() alone, please change the value of g_adc_vref of the corresponding channel, otherwise the voltage conversion will be wrong.
 *
 */
static void adc_set_ref_voltage(adc_sample_chn_e chn, adc_ref_vol_e v_ref)
{
    reg_adc_channel_set_state(chn) = (reg_adc_channel_set_state(chn) & (~FLD_SEL_VREF)) | (v_ref << 6);
    if (v_ref == ADC_VREF_1P2V) {
        //Vref buffer bias current trimming:        150%
        //Comparator preamp bias current trimming:  100%
        analog_write_reg8(areg_ain_scale, (analog_read_reg8(areg_ain_scale) & (0xC0)) | 0x3d);
    }
#if(COMPATIBLE_WITH_TL321X_AND_TL323X == 0)
    else if (v_ref == ADC_VREF_0P9V) {
        //Vref buffer bias current trimming:        100%
        //Comparator preamp bias current trimming:  100%
        analog_write_reg8(areg_ain_scale, (analog_read_reg8(areg_ain_scale) & (0xC0)) | 0x15);
    }
#endif
}

/**
 * @brief This function serves to set the sample frequency.
 * @param[in]  chn - enum variable of ADC sample channel.
 * @param[in]  sample_freq - enum variable of ADC sample frequency.
 * @return none
 */
static void adc_set_sample_rate(adc_sample_chn_e chn, adc_sample_freq_e sample_freq)
{
    unsigned char  adc_set          = sample_freq & 0xf;
    unsigned char  adc_sample_cycle = (sample_freq >> 4) & 0xf;
    unsigned short adc_capture      = (sample_freq >> 8) & 0xffff;
    adc_set_state_length(chn, adc_capture, adc_set);
    adc_set_tsample_cycle(chn, adc_sample_cycle);
}

/**
 * @brief This function serves to set pre_scaling factor.
 * @param[in]  chn - enum variable of ADC sample channel.
 * @param[in]  pre_scale - enum variable of ADC pre_scaling factor.
 * @return none
 */
static inline void adc_set_scale_factor(adc_sample_chn_e chn, adc_pre_scale_e pre_scale)
{
    reg_adc_channel_set_state(chn) = ((reg_adc_channel_set_state(chn) & (~FLD_SEL_AI_SCALE)) | (pre_scale << 4));
    g_adc_pre_scale[chn]           = 1 << pre_scale;
}

/**
 * @brief      This function serves to select Vbat voltage division factor.
 * @param[in]  chn - enum variable of ADC sample channel
 * @param[in]  vbat_div - enum variable of Vbat division factor.
 * @return     none
 * @note       adc_set_vbat_divider() does not take effect immediately after configuration, it needs to be delayed 100us after calling adc_dig_clk_en().
 */
void adc_set_vbat_divider(adc_sample_chn_e chn, adc_vbat_div_e vbat_div)
{
    unsigned char offset = (chn == 0) ? 0 : (1 << chn);

    reg_adc_vabt_div        = (reg_adc_vabt_div & (~BIT_RNG(offset, offset + 1))) | ((vbat_div) << offset);
    g_adc_vbat_divider[chn] = vbat_div ? (5 - vbat_div) : 1;
}

/**
 * @brief This function is used to initialize the ADC.
 * @param[in]  channel_cnt - transfer_mode and the number of channels used.
 * @return none
 * @attention Many features are configured in adc_init function. But some features
 *      such as adc digital clk, adc analog clk, resolution, we think better to set as default value,
 *      and user don't need to change them in most use cases.
 */
void adc_init(adc_chn_cnt_e channel_cnt)
{
    adc_power_off();                                    //power off sar adc
    adc_reset();                                        //reset whole digital adc module
    adc_clk_en();                                       //enable signal of 24M clock to sar adc
    adc_set_clk();                                      //set adc digital clk to 24MHz and adc analog clk to 4MHz
    adc_set_resolution(ADC_RES12);                      //default adc_resolution set as 12bit ,BIT(11) is sign bit
    //Enabling dwa affects adc performance.(confirmed by liupeng)
    //adc_data_weighted_average_en();
    if (NDMA_M_CHN == channel_cnt) {
        adc_all_chn_data_to_fifo_dis();
        reg_adc_config2 &= ~FLD_RX_DMA_ENABLE;          //In NDMA mode,RX DMA needs to be disabled.
        reg_adc_config2 |= FLD_SAR_RX_INTERRUPT_ENABLE; //SAR_RX_INTERRUPT must be enabled to call adc_get_irq_status() to get adc irq status.
    }
    /**
     * The set and capture of RNG channel are configured to 0 by default, and the actual state machine scanning time of RNG channel is the maximum time(about 25us),
     * and by configuring both of them to 1 (the minimum scanning time), the state machine scanning time of RNG channel is only (1+1)/24M=83ns,
     * which enables the state machine to enter into the set state of the M channel faster, and prevents the first code of M channel abnormality.
     */
    reg_adc_rng_set_state     = 0x01;
    reg_adc_rng_capture_state = 0x01;
    g_channel_cnt             = channel_cnt;
}

/**
 * @brief This function is used to configure the channel of the ADC.
 * @param[in]  chn -the channel to be configured.
 * @param[in]  adc_cfg -structure for configuring ADC channel.
 * @return none
 */
void adc_chn_config(adc_sample_chn_e chn, adc_chn_cfg_t adc_cfg)
{
    adc_set_diff_input(chn, adc_cfg.input_p, adc_cfg.input_n);
    adc_set_vbat_divider(chn, adc_cfg.divider);
    adc_set_ref_voltage(chn, adc_cfg.v_ref);
    adc_set_scale_factor(chn, adc_cfg.pre_scale);
    adc_set_sample_rate(chn, adc_cfg.sample_freq);
    adc_set_chn_en(chn);
}

/**
 * @brief This function is used to initialize the ADC for gpio sampling.
 * @param[in]  chn -the channel to be configured.
 * @param[in]  cfg -structure for configuring ADC channel.
 * @return none
 */
void adc_gpio_sample_init(adc_sample_chn_e chn, adc_gpio_cfg_t cfg)
{
    adc_pin_config(ADC_GPIO_MODE, cfg.pin);
    adc_chn_cfg_t chn_cfg =
        {
            .divider     = ADC_VBAT_DIV_OFF,
            .v_ref       = cfg.v_ref,
            .pre_scale   = cfg.pre_scale,
            .sample_freq = cfg.sample_freq,
            .input_p     = cfg.pin >> 12,
            .input_n     = GND,
        };
    adc_chn_config(chn, chn_cfg);
    /**
     * At present, the reference voltage is 1.2V by default, and the calibration is also based on 1.2V. If other chips have different gears in the future,
     * it is necessary to add a judgment here: only when the corresponding gears of the corresponding calibration conditions are selected,
     * the following calibration code can be invoked
     */
    g_adc_vref[chn]        = g_adc_gpio_calib_vref;        //set gpio sample calib vref
    g_adc_vref_offset[chn] = g_adc_gpio_calib_vref_offset; //set adc_vref_offset as adc_gpio_calib_vref_offset
}
#if(COMPATIBLE_WITH_TL321X_AND_TL323X == 0)
/**
 * @brief This function is used to initialize the ADC for vbat sampling.
 * @param[in]  chn -structure for configuring ADC channel.
 * @return none
 * @note   When you select ADC_SAMPLE_FREQ_192K, the first code of the vbat sample mode is an exception and needs to be discarded.
 *
 */
void adc_vbat_sample_init(adc_sample_chn_e chn)
{
    adc_chn_cfg_t chn_cfg =
        {
            .divider     = ADC_VBAT_DIV_1F4,
            .v_ref       = ADC_VREF_1P2V,
            .pre_scale   = ADC_PRESCALE_1,
            .sample_freq = ADC_SAMPLE_FREQ_96K,
            .input_p     = ADC_VBAT,
            .input_n     = GND,

        };
    adc_chn_config(chn, chn_cfg);
    /**
     * At present, the reference voltage is 1.2V by default, and the calibration is also based on 1.2V. If other chips have different gears in the future,
     * it is necessary to add a judgment here: only when the corresponding gears of the corresponding calibration conditions are selected,
     * the following calibration code can be invoked
     */
    g_adc_vref[chn]        = g_adc_vbat_calib_vref;        //set vbat sample calib vref
    g_adc_vref_offset[chn] = g_adc_vbat_calib_vref_offset; //set g_adc_vref_offset as g_adc_vbat_calib_vref_offset
}
#endif
/**
 * @brief  This function is used to initialize the ADC for gpio sampling to indirectly sample the vbat voltage.
 * @param[in]  chn -the channel to be configured.
 * @param[in]  cfg -structure for configuring ADC channel.
 * @return none
 * @attention Only for 1.9V ~ 3.6V vbat power supply, if the vbat power supply > 3.6V, ADC_VBAT_SAMPLE must be selected.
 */
void adc_gpio_sample_vbat_init(adc_sample_chn_e chn, adc_gpio_cfg_t cfg)
{
    adc_pin_config(ADC_VBAT_MODE, cfg.pin);
    adc_chn_cfg_t chn_cfg =
        {
            .divider     = ADC_VBAT_DIV_OFF,
            .v_ref       = cfg.v_ref,
            .pre_scale   = cfg.pre_scale,
            .sample_freq = cfg.sample_freq,
            .input_p     = cfg.pin >> 12,
            .input_n     = GND,
        };
    /**
     * At present, the reference voltage is 1.2V by default, and the calibration is also based on 1.2V. If other chips have different gears in the future,
     * it is necessary to add a judgment here: only when the corresponding gears of the corresponding calibration conditions are selected,
     * the following calibration code can be invoked
     */
    adc_chn_config(chn, chn_cfg);
    g_adc_vref[chn]        = g_adc_gpio_calib_vref;        //set gpio sample calib vref
    g_adc_vref_offset[chn] = g_adc_gpio_calib_vref_offset; //set adc_vref_offset as adc_gpio_calib_vref_offset
}

#if INTERNAL_TEST_FUNC_EN
/**
 * @brief       This function is used to enable the status of the valid adc code for the m channel.
 * @return      none
 * @attention   The adc_ana_read_en() API must be called before the adc_get_m_chn_valid_status()API.
 * @note        This function is used in NDMA mode where adc_get_m_chn_valid_status() needs to be called.
 */
static inline void adc_ana_read_en(void)
{
    analog_write_reg8(areg_adc_data_sample_control, analog_read_reg8(areg_adc_data_sample_control) | FLD_ANA_RD_EN);
}

/**
 * @brief      This function open temperature sensor power.
 * @return     none
 */
static inline void adc_temp_sensor_power_on(void)
{
    analog_write_reg8(areg_temp_sensor_ctrl, (analog_read_reg8(areg_temp_sensor_ctrl) & (~FLD_TEMP_SENSOR_POWER_DOWN)));
}

/**
 * @brief      This function close temperature sensor power.
 * @return     none
 */
static inline void adc_temp_sensor_power_off(void)
{
    analog_write_reg8(areg_temp_sensor_ctrl, (analog_read_reg8(areg_temp_sensor_ctrl) | FLD_TEMP_SENSOR_POWER_DOWN));
}

/**
 * @brief This function is used to initialize the ADC for Temperature Sensor sampling.
 * @param[in]  chn -structure for configuring ADC channel.
 * @return none
 */
void adc_temp_init(adc_sample_chn_e chn)
{
    adc_chn_cfg_t chn_cfg =
        {
            .divider     = ADC_VBAT_DIV_OFF,
            .v_ref       = ADC_VREF_1P2V,
            .pre_scale   = ADC_PRESCALE_1,
            .sample_freq = ADC_SAMPLE_FREQ_96K,
            .input_p     = ADC_TEMPSENSORP_EE,
            .input_n     = ADC_TEMPSENSORN_EE,

        };
    adc_chn_config(chn, chn_cfg);
    adc_temp_sensor_power_on();
}

/**
 * @brief This function serves to calculate temperature from temperature sensor adc sample code.
 * @param[in]   adc_code            - the temperature sensor adc sample code.
 * @return      adc_temp_value      - the temperature value.
 * @attention   Temperature and adc_code are linearly related. We test four chips between -40~130 (Celsius) and got an average relationship:
 *          Temp =  564 - ((adc_code * 819)>>11),when Vref = 1.2V, pre_scale = 1.
 */
unsigned short adc_calculate_temperature(unsigned short adc_code)
{
    //////////////// adc sample data convert to temperature(Celsius) ////////////////
    //adc_temp_value = 564 - ((adc_code * 819)>>11)
    return 564 - ((adc_code * 819) >> 11);
}
#endif
/**
 * @brief This function serves to calculate voltage from adc sample code.
 * @param[in]   chn - enum variable of ADC sample channel.
 * @param[in]   adc_code    - the adc sample code(should be positive value.)
 * @return      adc_vol_mv  - the average value of adc voltage value(adc voltage value >= 0).
 */
unsigned short adc_calculate_voltage(adc_sample_chn_e chn, unsigned short adc_code)
{
    /**
     *  adc sample code convert to voltage(mv):
     *  (adc code BIT<10~0> is valid data)
     *  adc_voltage  =  (adc_code * Vref * adc_pre_scale / 0x800) + offset
     *               =  (adc_code * Vref * adc_pre_scale >>11) + offset
     */
    unsigned short adc_voltage = (((adc_code * g_adc_vbat_divider[chn] * g_adc_pre_scale[chn] * g_adc_vref[chn]) >> 11) + g_adc_vref_offset[chn]);

    if(adc_voltage & BIT(15))
    {
        return 0;//When the adc_voltage < 0, the returned voltage value should be 0.
    }else
    {
        return adc_voltage;
    }
}

/**
 * @brief This function is used to calib ADC 1.2V vref for GPIO.
 * @param[in] vref - GPIO sampling calibration value.
 * @param[in] offset - GPIO sampling two-point calibration value offset.
 * @return none
 */
void adc_set_gpio_calib_vref(unsigned short vref, signed char offset)
{
    g_adc_gpio_calib_vref        = vref;
    g_adc_gpio_calib_vref_offset = offset;
}

/**
 * @brief This function is used to calib ADC 1.2V vref for Vbat.
 * @param[in] vref - Vbat channel sampling calibration value.
 * @param[in] offset - Vbat channel sampling two-point calibration value offset.
 * @return none
 */
void adc_set_vbat_calib_vref(unsigned short vref, signed char offset)
{
    g_adc_vbat_calib_vref        = vref;
    g_adc_vbat_calib_vref_offset = offset;
}

/**********************************************************************************************************************
 *                                                DMA only interface                                                  *
 **********************************************************************************************************************/
/**
 * @brief      This function serves to configure adc_dma_chn channel.
 * @param[in]  chn - the DMA channel
 * @return     none
 */
void adc_set_dma_config(dma_chn_e chn)
{
    adc_dma_chn     = chn;
    reg_adc_config2 = FLD_RX_DMA_ENABLE;
    dma_config(chn, &adc_rx_dma_config);
    reg_dma_llp(adc_dma_chn) = 0;
    /*
     * Configuration differs from TL7518 for the following reasons:
     * The TL7518's RX FIFO is stored in WORD units.
     * The TL721X's RX FIFO is stored in HALF WORD units.
     */
    adc_set_rx_fifo_trig_cnt(1); //Default value is 1, users should not change.
    adc_all_chn_data_to_fifo_en();
}

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
void adc_start_sample_dma(unsigned short *adc_data_buf, unsigned int data_byte_len)
{
    dma_set_address(adc_dma_chn, SAR_ADC_FIFO, (unsigned int)adc_data_buf);
    dma_set_size(adc_dma_chn, data_byte_len, DMA_WORD_WIDTH);
    /*
     * dma_chn_en() must be in front of adc_set_scan_chn_cnt() to prevent mis-ordering of multi-channel sampling data when using multiple channels.
     */
    dma_chn_en(adc_dma_chn);
    adc_set_scan_chn_cnt(g_channel_cnt & 0x0f);
}

/**
 * @brief     This function serves to get adc DMA irq status.
 * @return    0: the sample is in progress.
 *            !0: the sample is finished.
 * @note      The code is placed in the ram code section, in order to shorten the time.
 */
_attribute_ram_code_sec_noinline_ unsigned char adc_get_irq_status_dma(void)
{
    return (dma_get_tc_irq_status(1 << adc_dma_chn));
}

/**
 * @brief     This function serves to clear adc DMA irq status.
 * @return    none
 * @note      The code is placed in the ram code section, in order to shorten the time.
 */
_attribute_ram_code_sec_noinline_ void adc_clr_irq_status_dma(void)
{
    /*
     * adc_set_scan_chn_dis() must be called when DMA is finished to stop the state machine at the beginning of the M channel to prevent mis-ordering of multi-channel sampling data when using multiple channels.
     */
    adc_set_scan_chn_dis();
    dma_chn_dis(adc_dma_chn);
    dma_clr_tc_irq_status(1 << adc_dma_chn);
}

/**********************************************************************************************************************
 *                                                NDMA only interface                                                 *
 **********************************************************************************************************************/
/**
 * @brief       This function is used to get the status of the valid adc code for the m channel.
 * @return      0:Before sampling one adc_code at each time,m_chn_valid_status is automatically set to 0.
 *              !0:After sampling one adc_code at each time,m_chn_valid_status is automatically set to 1.
 * @attention   Must call adc_ana_read_en()API first to get the status of the valid adc code for the m channel.
 */
static inline unsigned char adc_get_m_chn_valid_status(void)
{
    return (analog_read_reg8(areg_m_chn_data_valid_status) & FLD_M_CHN_DATA_VALID_STATUS);
}

/**
 * @brief This function serves to directly get an adc sample code from fifo.
 * @return  adc_code    - the adc sample code.
 *                      - Bit[11:15] of the adc code read from reg_adc_rxfifo_dat are sign bits,if the adc code is positive, bits [11:15] are all 1's,
 *                        if the adc code is negative, bits [11:15] are all 0's and valid data bits are Bit[0:10],the valid range is 0~0x7FF.
 */
unsigned short adc_get_raw_code(void)
{
    unsigned short adc_code = 0;
    /**
     * Change the way to get adc code from read areg_adc_misc to read rx fifo, to solve the problem that the valid status is 1 for a long time
     * due to the slow change of valid status in adc_get_m_chn_valid_status(), which leads to fetching the same code repeatedly.(Confirmed by qiangkai.xu, added by xiaobin.huang at 20240903)
     */
    if (!g_adc_rx_fifo_index) {
        adc_code            = reg_adc_rxfifo_dat(g_adc_rx_fifo_index);
        g_adc_rx_fifo_index = 1;
    } else {
        adc_code            = reg_adc_rxfifo_dat(g_adc_rx_fifo_index);
        g_adc_rx_fifo_index = 0;
    }
    return adc_code;
}

/**
 * @brief   This function is used to enable the transmission of data from the adc's M channel, L channel, and R channel to the sar adc rxfifo.
 * @return  none
 */
void adc_start_sample_nodma(void)
{
    adc_clr_rx_fifo_cnt(); //If the fifo is not cleared, there may be residual values in the fifo that affect the sampling results.
    adc_all_chn_data_to_fifo_en();
    adc_set_scan_chn_cnt(1);
}

/**
 * @brief   This function is used to disable the transmission of data from the adc's M channel, L channel, and R channel to the sar adc rxfifo and clear rx fifo cnt and g_adc_rx_fifo_index.
 * @return  none
 */
void adc_stop_sample_nodma(void)
{
    adc_all_chn_data_to_fifo_dis();
    adc_clr_rx_fifo_cnt();
}

/**
 * @brief     This function serves to get adc DMA sample status.
 * @return      0: the sample is in progress.
 *              !0: the sample is finished.
 */
unsigned char adc_get_sample_status_dma(void)
{
    return (dma_get_tc_irq_status(1 << adc_dma_chn));
}
