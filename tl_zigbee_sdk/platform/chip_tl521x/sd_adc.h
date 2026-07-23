/********************************************************************************************************
 * @file    sd_adc.h
 *
 * @brief   This is the header file for tl521x
 *
 * @author  Driver Group
 * @date    2026
 *
 * @par     Copyright (c) 2026, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#pragma once

#include "dma.h"
#include "compiler.h"
#include "gpio.h"
#include "reg_include/register.h"
#include "lib/include/pm/pm.h"

extern volatile unsigned char g_sd_adc_divider;
extern volatile unsigned short g_sd_adc_downsample_rate;
/**
 * Since the voltage at the input P and N terminals, after voltage divider, must be <= 1.2V,
 * and the input voltage of the GPIO does not exceed 3.2V.
 * the detection ranges of the different modes are as follows:
    +--------------+---------+-----------------+
    | Sample Mode  | Divider | Detection Range |
    +--------------+---------+-----------------+
    |   GPIO_MODE  |   off   |    -1.2-1.2V    |
    |              |   1/2   |    -2.4-2.4V    |
    |              |   1/4   |    -3.2-3.2V    |
    +--------------+---------+-----------------+
    |   VBAT_MODE  |   1/2   |     1.8-4.5V    |
    |              |   1/4   |     1.8-4.5V    |
    +--------------+---------+-----------------+
    |   TEMP_MODE  |    \    |  -40-85Celsius  |
    +--------------+---------+-----------------+
 */

#ifndef SD_ADC_INTERNAL_TEST_FUNC_EN
    #define SD_ADC_INTERNAL_TEST_FUNC_EN  0 //only for internal test
#endif
/**
 *  @brief  Define SD ADC voltage reference
 */
typedef enum
{
    SD_ADC_VBG,
    SD_ADC_VMID,
} sd_adc_vref_e;

typedef enum{
    SD_ADC_SAMPLE_MODE,
    SD_ADC_AUDIO_MODE,
}sd_adc_mode_e;

/**
 * @brief dc op mode enum
 */
typedef enum{
    SD_ADC_SINGLE_DC_MODE            = 1,
    SD_ADC_2DC_MODE                  = 3, //A0 version not support 2DC mode
    SD_ADC_AUDIO_ONLY_MODE           = 4,
}sd_dc_op_mode_e;

/**
 * @brief sd_adc positive input pin type
 * |               |              |
 * | :------------ | :----------- |
 * |    <15:12>    |    <11:0>    |
 * | sd_adc channel|    gpio pin  |
 */
typedef enum{
    SD_ADC_GPIO_PC0P = GPIO_PC0 | (0x0<<12),
    SD_ADC_GPIO_PC1P = GPIO_PC1 | (0x1<<12),
    SD_ADC_GPIO_PC2P = GPIO_PC2 | (0x2<<12),
    SD_ADC_GPIO_PC3P = GPIO_PC3 | (0x3<<12),
    SD_ADC_GPIO_PC4P = GPIO_PC4 | (0x4<<12),
    SD_ADC_GPIO_PC5P = GPIO_PC5 | (0x5<<12),
    SD_ADC_GPIO_PB4P = GPIO_PB4 | (0x6<<12),
    SD_ADC_GPIO_PB5P = GPIO_PB5 | (0x7<<12),
    SD_ADC_GPIO_PB6P = GPIO_PB6 | (0x8<<12),
    SD_ADC_GPIO_PB7P = GPIO_PB7 | (0x9<<12),
    SD_ADC_TEMP_SENSOR_P = 0 | (0xb<<12),
}sd_adc_p_input_pin_def_e;

/**
 * @brief sd_adc negative input pin type
 * |               |              |
 * | :------------ | :----------- |
 * |     <15:12>   |    <11:0>    |
 * | sd_adc channel|    gpio pin  |
 */
typedef enum{
    SD_ADC_GPIO_PC0N = GPIO_PC0 | (0x0<<12),
    SD_ADC_GPIO_PC1N = GPIO_PC1 | (0x1<<12),
    SD_ADC_GPIO_PC2N = GPIO_PC2 | (0x2<<12),
    SD_ADC_GPIO_PC3N = GPIO_PC3 | (0x3<<12),
    SD_ADC_GPIO_PC4N = GPIO_PC4 | (0x4<<12),
    SD_ADC_GPIO_PC5N = GPIO_PC5 | (0x5<<12),
    SD_ADC_GPIO_PB4N = GPIO_PB4 | (0x6<<12),
    SD_ADC_GPIO_PB5N = GPIO_PB5 | (0x7<<12),
    SD_ADC_GPIO_PB6N = GPIO_PB6 | (0x8<<12),
    SD_ADC_GPIO_PB7N = GPIO_PB7 | (0x9<<12),
    SD_ADC_GNDN = 0 | (0xb<<12),
}sd_adc_n_input_pin_def_e;

/**
 * @brief sd_adc downsample rate type.
 */
typedef enum{
    SD_ADC_DOWNSAMPLE_RATE_64  = 64,
    SD_ADC_DOWNSAMPLE_RATE_128 = 128,
    SD_ADC_DOWNSAMPLE_RATE_256 = 256,
}sd_adc_downsample_rate_e;

/**
 * @brief sd_adc vbat channel divider
 */
typedef enum{
    SD_ADC_VBAT_DIV_1F4 = 0x01,
#if SD_ADC_INTERNAL_TEST_FUNC_EN
    SD_ADC_VBAT_DIV_1F8 = 0,
    SD_ADC_VBAT_DIV_1F2 = 0x02,
#endif
}sd_adc_vbat_div_e;

/**
 * @brief sd_adc positive input channel divider
 */
typedef enum{
    SD_ADC_GPIO_CHN_DIV_1F4 = 0x01,
    SD_ADC_GPIO_CHN_DIV_1F2 = 0x02,
    SD_ADC_GPIO_CHN_DIV_OFF = 0x03,
#if SD_ADC_INTERNAL_TEST_FUNC_EN
    SD_ADC_GPIO_CHN_DIV_1F8 = 0,

#endif
}sd_adc_gpio_chn_div_e;

/**
 * @brief sd_adc DC channel type.
 */
typedef enum
{
    SD_ADC_DC0, //user don't need care and config on A0 version
    SD_ADC_DC1, //user don't need care and config on A0 version
} sd_adc_dc_chn_e ;

/**
 * @brief sd_adc downsample rate type.
 */
typedef enum{
    SD_ADC_SAPMPLE_CLK_1M  = 1,
    SD_ADC_SAPMPLE_CLK_2M  = 2,
}sd_adc_sample_clk_freq_e;

/**
 * @brief sd_adc gpio sample configuration.
 */
typedef struct {
    sd_adc_sample_clk_freq_e clk_freq;
    sd_adc_downsample_rate_e downsample_rate;
    sd_adc_gpio_chn_div_e gpio_div;

    sd_adc_p_input_pin_def_e input_p;
    sd_adc_n_input_pin_def_e input_n;

    sd_adc_p_input_pin_def_e input_chn1_p;
    sd_adc_n_input_pin_def_e input_chn1_n;

} sd_adc_gpio_cfg_t;

/**
 * @brief sd_adc gpio sample pin configuration.
 */
typedef struct {
    sd_adc_p_input_pin_def_e input_p;
    sd_adc_n_input_pin_def_e input_n;
} sd_adc_gpio_pin_cfg_t;

/**
 * @brief sd_adc sample result type.
 */
typedef enum{
    SD_ADC_VOLTAGE_10X_MV,
    SD_ADC_VOLTAGE_MV,
    TEMP_VALUE,
}sd_adc_result_type_e;

/**
 * @brief sd_adc sample mode.
 */
typedef enum{
    SD_ADC_GPIO_MODE,
    SD_ADC_VBAT_MODE,
    SD_ADC_TEMP_MODE,
}sd_adc_mux_mode_e;

/**********************************************************************************************************************
 *                                                  SD_ADC only interface                                                 *
 **********************************************************************************************************************/
/**
 * @brief This function is used to calibrate sd adc sample voltage for single GPIO.
 * @param[in] vref - GPIO sampling calibration value.
 * @param[in] offset - GPIO sampling two-point calibration value offset.
 * @return none
 */
void sd_adc_set_single_gpio_calib_vref(unsigned short vref, signed short offset);

/**
 * @brief This function is used to calibrate sd adc sample voltage for VABT.
 * @param[in] vref - GPIO sampling calibration value.
 * @param[in] offset - GPIO sampling two-point calibration value offset.
 * @return none
 */
void sd_adc_set_vbat_calib_vref(unsigned short vref, signed short offset);

/**
 * @brief This function is used to calibrate sd adc sample voltage for GPIO differential.
 * @param[in] vref - GPIO sampling calibration value.
 * @param[in] offset - GPIO sampling two-point calibration value offset.
 * @return none
 */
void sd_adc_set_diff_gpio_calib_vref(unsigned short vref, signed short offset);

/**
 * @brief      This function servers to clear the rx fifo.
 * @param[in]  none
 * @return     none
 */
static inline void sd_adc_clr_fifo_wptr(void)
{
    reg_rxfifo2_clr |= FLD_RXFIFO2_CLR_PCLK;
}

/**
 * @brief      This function serves to set the positive and negative input gpio.
 * @param[in]  chn - sd_adc_dc_chn_e
 * @param[in]  p_ain - sd_adc_p_input_pin_def_e
 * @param[in]  n_ain - sd_adc_n_input_pin_def_e
 * @return none
 */
static inline void sd_adc_set_diff_input(sd_adc_dc_chn_e  chn, sd_adc_p_input_pin_def_e p_ain, sd_adc_n_input_pin_def_e n_ain)
{
    if(chn == SD_ADC_DC1)
    {
        analog_write_reg8(areg_dc1_sel_ana_input, ((p_ain>>12) << 4) | (n_ain>>12));
    }else
    {
        analog_write_reg8(areg_sel_ana_input, ((p_ain>>12) << 4) | (n_ain>>12));
    }
}

/**
 * @brief      This function serves to set the sample clock frequency of the SD_ADC (SD_ADC source clock: pclk).
 * @param[in]  sd_adc_sample_clk_freq_e  - sample clock frequency: SD_ADC_SAPMPLE_CLK_1M / SD_ADC_SAPMPLE_CLK_2M.
 * @return     none
 */
static inline void sd_adc_set_clk_freq(sd_adc_sample_clk_freq_e clk_freq)
{
    reg_dfifo_dc_clk_div = (sys_clk.pclk/2/clk_freq)-1;//sd_adc sample clock frequency = pclk/2/(div+1)
}


/**
 * @brief      This function serves to set the downsample rate of the SD_ADC.
 * @param[in]  downsample_rate -sd_adc_downsample_rate_e
 * @return     none
 */
static inline void sd_adc_set_downsample_rate(sd_adc_downsample_rate_e downsample_rate)
{
    reg_dfifo_aidx2 = (reg_dfifo_aidx2 & (~FLD_R_DEC)) | ((downsample_rate>>7)<<4);
    g_sd_adc_downsample_rate = downsample_rate;
}


/**
 * @brief      This function serves to set the downsample rate of the SD_ADC.
 * @param[in]  trig_num -trigger number
 * @return     none
 * @note       -# The depth of rx fifo is 16 words, so trig num can only be configured up to 15. (interrupt triggered condition: rx fifo num > trig num)
 *             -# In NDMA_INTERRUPT_MODE, trigger number = SD_ADC_SAMPLE_CNT - 1, trigger number must be <16.
 *
 */
static inline void sd_adc_set_rx_fifo_trig_cnt(unsigned char trig_num)
{
    reg_rxfifo2_trig_num = ((reg_rxfifo2_trig_num & (~FLD_RXFIFO2_TRIG_NUM)) | trig_num);
}

/**
 * @brief     This function servers to stop SD_ADC sampling.
 * @param[in] none
 * @return    none
 */
static inline void sd_adc_sample_stop(void)
{
    BM_CLR(reg_clk_en4, FLD_CLK4_DC_EN);//dc clk signal disable
}

/**
 * @brief     This function servers to start SD_ADC sampling.
 * @param[in] none
 * @return    none
 * @note      the first 4 sampling data anomaly after each calling sd_adc_sample_start().
 */
static inline void sd_adc_sample_start(void)
{
    sd_adc_clr_fifo_wptr();//Make sure that the write pointer is cleared before starting sampling.
    BM_SET(reg_clk_en4, FLD_CLK4_DC_EN);//dc clk signal enable
}

#if SD_ADC_INTERNAL_TEST_FUNC_EN
/**
 * @brief     This function servers to set SD_ADC irq mask for NDMA mode.
 * @param[in] none
 * @return    none
 */
static inline void sd_adc_set_irq_mask(void)
{
    reg_dfifo_dc_mode |= FLD_R_MASK;
}

/**
 * @brief     This function servers to clear SD_ADC irq mask for NDMA mode.
 * @param[in] none
 * @return    none
 */
static inline void sd_adc_clr_irq_mask(void)
{
    reg_dfifo_dc_mode &= ~FLD_R_MASK;
}

/**
 * @brief      This function servers to get sd_adc irq status.
 * @param[in]  none
 * @return     irq status
 * @note       Whether irq mask are set or not, irq status is set to 1 when fifo data count > trigger num,
 *             and irq status is automatically set to 0 when fifo data count <= trigger num.
 */
static inline unsigned char sd_adc_get_irq_status(void)
{
    return reg_irq_fifo & FLD_FIFO2_MID;
}

/**
 * @brief      This function servers to clear sd_adc irq status.
 * @param[in]  none
 * @return     none
 */
static inline void sd_adc_clr_irq_status(void)
{
    /**
     * The wptr must be cleared to 0 immediately after entering the interrupt,
     * otherwise the interrupt will be triggered all the time because of the wptr > trigger threshold value.
     */
    sd_adc_clr_fifo_wptr();
    sd_adc_sample_stop();
    reg_irq_fifo |= FLD_R_IRQ;
}
#endif

/**
 * @brief     This function servers to power on temperature sensor.
 * @param[in] none
 * @return    none
 */
static inline void sd_adc_temp_sensor_power_on(void)
{
#if PM_POWER_OPTIMIZATION
    g_areg_aon_06 &= ~FLD_PD_TEMP_SENSOR_3V;
    analog_write_reg8(areg_aon_0x06,g_areg_aon_06);
#else
    analog_write_reg8(areg_aon_0x06,analog_read_reg8(areg_aon_0x06) & (~FLD_PD_TEMP_SENSOR_3V));
#endif
}

/**
 * @brief      This function servers to power off temperature sensor.
 * @param[in]  none
 * @return     none
 */
static inline void sd_adc_temp_sensor_power_off(void)
{
#if PM_POWER_OPTIMIZATION
    g_areg_aon_06 |= FLD_PD_TEMP_SENSOR_3V;
    analog_write_reg8(areg_aon_0x06,g_areg_aon_06);
#else
    analog_write_reg8(areg_aon_0x06,analog_read_reg8(areg_aon_0x06) | FLD_PD_TEMP_SENSOR_3V);
#endif
}

/**
 * @brief      This function serves to configure sd_adc_dma_chn channel.
 * @param[in]  chn - the DMA channel
 * @return     none
 */
void sd_adc_set_dma_config(dma_chn_e chn);

/**
 * @brief     The adc starts sampling in DMA mode.
 * @param[in] sd_adc_data_buf - Pointer to data buffer, it must be 4-bytes aligned address
 *                            and the actual buffer size defined by the user needs to be not smaller than the data_byte_len,
 *                            otherwise there may be an out-of-bounds problem.
 * @param[in] data_byte_len - Amount of data to be sampled in bytes, the maximum value can be up to 0xFFFFFC.
 * @return    none
 */
void sd_adc_start_sample_dma(signed int *sd_adc_data_buf, unsigned int data_byte_len);

/**
 * @brief     This function serves to get adc DMA sample status.
 * @return      0: the sample is in progress.
 *              !0: the sample is finished.
 */
unsigned char sd_adc_get_sample_status_dma(void);
/**
 * @brief     This function serves to clear adc DMA irq status.
 * @return    none
 * @note      The code is placed in the ram code section, in order to shorten the time.
 */
_attribute_ram_code_sec_noinline_ void sd_adc_clr_irq_status_dma(void);

static inline signed int sd_adc_get_raw_code(void)
{
    return reg_rxfifo2_r_dat0;
}

/**
 * @brief     This function serves to get the rxfifo cnt,when data enters rxfifo, the rxfifo cnt increases; when reading data from rx_fifo, rxfifo cnt decays.
 * @return    none
 */
static inline unsigned char sd_adc_get_rxfifo_cnt(void)
{
    return reg_rxfifo2_num & FLD_RXFIFO2_NUM;
}

/**
 * @brief      This function is used to initialize the positive and negative channels for gpio and temp sensor sampling.
 * @param[in]  chn   - sd_adc_dc_chn_e
 * @param[in]  p_pin - sd_adc_p_input_pin_def_e
 * @param[in]  n_pin - sd_adc_n_input_pin_def_e
 * @return     none
 */
void sd_adc_gpio_pin_init(sd_adc_dc_chn_e  chn, sd_adc_p_input_pin_def_e p_pin,sd_adc_n_input_pin_def_e n_pin);

/**
 * @brief      This function serves to select Vbat voltage division.
 * @param[in]  div - sd_adc_vbat_div_e.
 * @return     none
*/
void sd_adc_set_vbat_divider(sd_adc_vbat_div_e div);

/**
 * @brief      This function serves to select the positive and negative voltage divider of the gpio.
 * @param[in]  gpio_div - sd_adc_gpio_chn_div_e.
 * @return     none
 * @note       the positive and negative voltage divider must be configured to be the same,otherwise the output voltage value or temperature values will be incorrect.
 */
void sd_adc_set_gpio_divider(sd_adc_gpio_chn_div_e gpio_div);

/**
 * @brief      This function is used to initialize the SD_ADC.
 * @param[in]  mode - sd_dc_op_mode_e
 * @return     none
 */
void sd_adc_init(sd_dc_op_mode_e mode);

/**
 * @brief      This function is used to initialize the SD_ADC for gpio sampling.
 * @param[in]  cfg -Pointer to configure the GPIO channel structure.
 * @return     none
 * @note       If you switch to gpio mode from other modes, the first 4 codes of the sample are abnormal
 *             due to the internal filter reset, so you need to discard the first 4 codes.
 */
void sd_adc_gpio_sample_init(sd_adc_gpio_cfg_t *cfg);

/**
 * @brief      This function is used to drop the data before entering the filter in 2dc mode and audio+dc mode, preventing the data of two channels from interfering with each other.
 * @param[in]  drop_num -drop the data before entering the filter(drop_num <= 15).
 * @return     none
 * @note       -# drop means to drop the data before entering the filter to prevent the data of two channels from interfering with each other.
 *             -# When using 2dc mode, it will drop both dc0 and dc1 data at the same time.
 *             -# When using audio+dc mode, it will only drop dc data, not audio data.
 *             -# Recommended drop 4 codes, you can set according to the actual situation,  drop num must be <= 15.
 */
void sd_adc_set_drop_num(unsigned char drop_num);


/**
 * @brief      This function is used to set sample number of each dc channel in 2dc mode
 * @param[in]  each_chn_sample_num -the sample number of each dc in 2dc mode
 * @return     none
 */
void sd_adc_set_each_dc_chn_sample_num(unsigned short each_chn_sample_num);

/**
 * @brief      This function is used to initialize the SD_ADC for vbat sampling.
 * @param[in]  clk_freq  - sample clock frequency: SD_ADC_SAPMPLE_CLK_1M / SD_ADC_SAPMPLE_CLK_2M.
 * @param[in]  div             - sd_adc_vbat_div_e.
 * @param[in]  downsample_rate - sd_adc_downsample_rate_e
 * @return     none
 * @note       If you switch to vbat mode from other modes, the first 4 codes of the sample are abnormal
 *             due to the internal filter reset, so you need to discard the first 4 codes.
 */
void sd_adc_vbat_sample_init(unsigned char clk_freq, sd_adc_vbat_div_e div, sd_adc_downsample_rate_e downsample_rate);
#if SD_ADC_INTERNAL_TEST_FUNC_EN
/**
 * @brief      This function is used to initialize the SD_ADC for Temperature Sensor sampling.
 * @param[in]  clk_freq  - sample clock frequency: SD_ADC_SAPMPLE_CLK_1M / SD_ADC_SAPMPLE_CLK_2M.
 * @param[in]  downsample_rate - sd_adc_downsample_rate_e
 * @return     none
 * @note       If you switch to temp mode from other modes, the first 4 codes of the sample are abnormal
 *             due to the internal filter reset, so you need to discard the first 4 codes.
 */
void sd_adc_temp_init(unsigned char clk_freq, sd_adc_downsample_rate_e downsample_rate);

/**
 * @brief       This function is used to convert SD_ADC sample code to celsius value.
 * @param[in]   sd_adc_code    - the adc sample code.
 * @return      Celsius value.
 */
signed short sd_adc_calculate_temperature(signed int sd_adc_code);
#endif
/**
 * @brief       This function is used to convert SD_ADC sample code to a voltage.
 * @param[in]   sd_adc_code    - the sd_adc sample code.
 * @param[in]   type       - sd_adc_result_type_e.
 * @return      sd_adc_vol_mv  - the average value of adc voltage value.
 */
signed int sd_adc_calculate_voltage(signed int sd_adc_code,sd_adc_result_type_e type);

/**********************************************************************************************************************
 *                                         Audio and SD_ADC common interface                                              *
 **********************************************************************************************************************/
/**
 * @brief     This function servers to power on SD_ADC.
 * @param[in] mode -sd_adc_mode_e
 * @return    none
 * @note      -# After sd_adc_power_on(SD_ADC_SAMPLE_MODE), must wait >160us(when the C10 capacitor on the development board is 10nF) for VMID power to stabilize, otherwise there will be hundreds of sample data will be abnormal.
 *            -# After each call to sd_adc_power_on(SD_ADC_SAMPLE_MODE) and wait >160us(when the C10 capacitor on the development board is 10nF), the first 4 codes sampled by sd_adc may be abnormal and need to be discarded.
 *
 */
void sd_adc_power_on(sd_adc_mode_e mode);

/**
 * @brief     This function servers to power off SD_ADC.
 * @param[in] mode -sd_adc_mode_e
 * @return    none
 */
void sd_adc_power_off(sd_adc_mode_e mode);

/**
 * @brief This function serves to set SD_ADC op mode.
 * @param[in]  mode - sd_dc_op_mode_e
 * @return none
 */
static inline void sd_adc_set_op_mode(sd_dc_op_mode_e mode)
{
    reg_dc_mode_config = (reg_dc_mode_config & (~FLD_OP_MODE)) | mode;
}

/**
 * @brief       This function is used to enable the data weighted average algorithm function to improve SD ADC performance.
 * @param[in]   none
 * @return      none
 */
static inline void sd_adc_data_weighted_average_en(void)
{
    analog_write_reg8(areg_0x10c, analog_read_reg8(areg_0x10c) | FLD_L_DWA_EN);
}


