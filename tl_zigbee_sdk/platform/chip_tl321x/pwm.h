/********************************************************************************************************
 * @file    pwm.h
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
#ifndef PWM_H_
#define PWM_H_
#include "dma.h"
#include "gpio.h"
#include "reg_include/register.h"

/**
 * @brief  enum variable, the number of PWM channels supported
 */
typedef enum
{
    PWM0_ID = 0,
    PWM1_ID,
    PWM2_ID,
    PWM3_ID,
    PWM4_ID,
    PWM5_ID,
} pwm_id_e;

/**
 * @brief  enum variable, represents the PWM mode.
 */
typedef enum
{
    PWM_NORMAL_MODE      = 0x00,
    PWM_COUNT_MODE       = 0x01,
    PWM_IR_MODE          = 0x03,
    PWM_IR_FIFO_MODE     = 0x07,
    PWM_IR_DMA_FIFO_MODE = 0x0F,
} pwm_mode_e;

/**
 * @brief  Select the 32K clock source of pwm.
 */
typedef enum
{
    PWM_CLOCK_32K_CHN_PWM0 = 0x01,
    PWM_CLOCK_32K_CHN_PWM1 = 0x02,
    PWM_CLOCK_32K_CHN_PWM2 = 0x04,
    PWM_CLOCK_32K_CHN_PWM3 = 0x08,
    PWM_CLOCK_32K_CHN_PWM4 = 0x10,
    PWM_CLOCK_32K_CHN_PWM5 = 0x20
} pwm_clk_32k_en_chn_e;

/**
 * @brief     This function servers to set pwm clock frequency, when pwm clock source is pclk.
 * @param[in] pwm_clk_div - variable of the pwm clock.
 *            PWM frequency = System_clock / (pwm_clk_div+1).
 * @return    none.
 */
static inline void pwm_set_clk(unsigned char pwm_clk_div)
{
    reg_pwm_clkdiv = pwm_clk_div;
}

/**
 * @brief     This function servers to set pwm clock source is 32K.
 *            If you use 32K as the PWM clock source, you can work in suspend mode.
 *            but the 32K clock source cannot be divided and can only work in continuous mode and counting mode
 * @param[in] pwm_32K_en_chn - This variable is used to select the specific PWM channel.
 * @return    none.
 */

static inline void pwm_32k_chn_en(pwm_clk_32k_en_chn_e pwm_32K_en_chn)
{
    reg_pwm_mode32k |= pwm_32K_en_chn;
}

/**
 * @brief      This function serves to set the pwm function.
 * @param[in]  pin      - the pin needs to set.
 * @param[in]  function - the function need to set.
 * @return     none.
 */
void pwm_set_pin(gpio_func_pin_e pin, gpio_func_e func);

/**
 * @brief     This function servers to set pwm count status(CMP) time.
 * @param[in] id   - variable of enum to select the pwm number,Value range 0~5.
 * @param[in] tcmp - variable of the CMP.
 * @return    none.
 * @note      When pwm is in the process of sending and the pwm_set_tcmp() interface is called,
 *            the new value of PWM_TCMP will be valid at the beginning of the next cycle.
 */
static inline void pwm_set_tcmp(pwm_id_e id, unsigned short tcmp)
{
    reg_pwm_cmp(id) = tcmp;
}

/**
 * @brief     This function servers to set pwm cycle time.
 * @param[in] id   - variable of enum to select the pwm number,Value range 0~5.
 * @param[in] tmax - variable of the cycle time.
 * @return    none.
 * @note      When pwm is in the process of sending and the pwm_set_tmax() interface is called,
 *            the new value of PWM_TMAX will be valid at the beginning of the next cycle.
 */
static inline void pwm_set_tmax(pwm_id_e id, unsigned short tmax)
{
    reg_pwm_max(id) = tmax;
}

/**
 * @brief     This function servers to start the pwm,can have more than one PWM open at the same time.
 * @param[in] en - variable of enum to select the pwm.
 * @return    none.
 */
static inline void pwm_start(pwm_en_e en)
{
    reg_pwm_enable |= en;
}

/**
 * @brief     This function servers to stop the pwm,can have more than one PWM stop at the same time.
 * @param[in] en - variable of enum to select the pwm.
 * @return    none.
 * @note      When pwm_stop() is called, the PWMx immediately changes the transmit signal to low.
 */
static inline void pwm_stop(pwm_en_e en)
{
    reg_pwm_enable &= ~en;
}

/**
 * @brief     This function servers to revert the PWMx.
 * @param[in] id - variable of enum to select the pwm number,Value range 0~5.
 * @return    none.
 */
static inline void pwm_invert_en(pwm_id_e id)
{
    reg_pwm_invert |= BIT(id);
}

/**
 * @brief     This function servers to disable the PWM revert function.
 * @param[in] id - variable of enum to select the pwm number,Value range 0~5.
 * @return    none.
 */
static inline void pwm_invert_dis(pwm_id_e id)
{
    BM_CLR(reg_pwm_invert, BIT(id));
}

/**
 * @brief     This function servers to revert the PWMx_N.
 * @param[in] id - variable of enum to select the pwm number,Value range 0~5.
 * @return    none.
 */
static inline void pwm_n_invert_en(pwm_id_e id)
{
    reg_pwm_n_invert |= BIT(id);
}

/**
 * @brief     This function servers to disable the PWM revert function.
 * @param[in] id - variable of enum to select the pwm number,Value range 0~5.
 * @return    none.
 */
static inline void pwm_n_invert_dis(pwm_id_e id)
{
    BM_CLR(reg_pwm_n_invert, BIT(id));
}

/**
 * @brief     This function servers to enable the pwm polarity.
 * @param[in] id - variable of enum to select the pwm number,Value range 0~5.
 * @return    none.
 */
static inline void pwm_set_polarity_en(pwm_id_e id)
{
    BM_SET(reg_pwm_pol, BIT(id));
}

/**
 * @brief     This function servers to disable the pwm polarity.
 * @param[in] id - variable of enum to select the pwm number,Value range 0~5.
 * @return    none.
 */
static inline void pwm_set_polarity_dis(pwm_id_e id)
{
    BM_CLR(reg_pwm_pol, BIT(id));
}

/**
 * @brief     This function servers to enable the pwm interrupt.
 * @param[in] mask - variable of enum to select the pwm interrupt source.
 * @return    none.
 */
static inline void pwm_set_irq_mask(pwm_irq_e mask)
{
    reg_pwm_irq_mask |= mask;
}

/**
 * @brief     This function servers to disable the pwm interrupt function.
 * @param[in] mask - variable of enum to select the pwm interrupt source.
 * @return    none.
 */
static inline void pwm_clr_irq_mask(pwm_irq_e mask)
{
    reg_pwm_irq_mask &= ~mask;
}

/**
 * @brief     This function servers to get the pwm interrupt status.
 * @param[in] status - variable of enum to select the pwm interrupt source.
 * @retval      non-zero      -  the interrupt occurred.
 * @retval      zero  -  the interrupt did not occur.
 */
static inline unsigned short pwm_get_irq_status(pwm_irq_e status)
{
    return (reg_pwm_irq_sta & status);
}

/**
 * @brief     This function servers to clear the pwm interrupt.When a PWM interrupt occurs, the corresponding interrupt flag bit needs to be cleared manually.
 * @param[in] status  - variable of enum to select the pwm interrupt source.
 * @return    none.
 */
static inline void pwm_clr_irq_status(pwm_irq_e status)
{
    reg_pwm_irq_sta = status;
}

/**
 * @brief     This function servers to set pwm0 mode.
 * @param[in] mode - variable of enum to indicates the pwm mode.
 * @return    none.
 */
static inline void pwm_set_pwm0_mode(pwm_mode_e mode)
{
    reg_pwm0_mode = mode; //only PWM0 has count/IR/fifo IR mode
}

/**
 * @brief     This function servers to set pwm0 output to analog.
 * @return    none.
 */
static inline void pwm_set_pwm0_output_to_ana_ir_en(void)
{
    BM_SET(reg_pwm0_mode, BIT(4));
}

/**
 * @brief     This function servers to disable pwm0 output to analog.
 * @return    none.
 */
static inline void pwm_set_pwm0_output_to_ana_ir_dis(void)
{
    BM_CLR(reg_pwm0_mode, BIT(4));
}

/**
 * @brief     This function servers to set pwm cycle time & count status.
 * @param[in] max_tick - variable of the cycle time.
 * @param[in] cmp_tick - variable of the CMP.
 * @return    none.
 */
static inline void pwm_set_pwm0_tcmp_and_tmax_shadow(unsigned short max_tick, unsigned short cmp_tick)
{
    reg_pwm_tcmp0_shadow = cmp_tick;
    reg_pwm_tmax0_shadow = max_tick;
}

/**
 * @brief     This function servers to set the pwm0 pulse number.
 * @param[in] pulse_num - variable of the pwm pulse number.The maximum bits is 14bits.
 * @return    none.
 */
static inline void pwm_set_pwm0_pulse_num(unsigned short pulse_num)
{
    reg_pwm0_pulse_num = pulse_num;
}

/**
 * @brief     This function serves to set trigger level of interrupt for IR FIFO mode
 * @param[in] trig_level - FIFO  num int trigger level.When fifo numbers is less than this value.It's will take effect.
 * @return    none
 */
static inline void pwm_set_pwm0_ir_fifo_irq_trig_level(unsigned char trig_level)
{
    reg_pwm_ir_fifo_irq_trig_level = trig_level;
}

/**
 * @brief     This function serves to clear data in fifo. Only when pwm is in not active mode,
 *            it is possible to clear data in fifo.
 * @return    none
 */
static inline void pwm_clr_pwm0_ir_fifo(void)
{
    reg_pwm_ir_clr_fifo_data |= FLD_PWM0_IR_FIFO_CLR_DATA;
}

/**
 * @brief     This function serves to get the number of data in fifo.
 * @return    the number of data in fifo
 */
static inline unsigned char pwm_get_pwm0_ir_fifo_data_num(void) //????TODO
{
    return (reg_pwm_ir_fifo_data_status & FLD_PWM0_IR_FIFO_DATA_NUM);
}

/**
 * @brief     This function serves to determine whether data in fifo is empty.
 * @return    yes: 1 ,no: 0;
 */
static inline unsigned char pwm_get_pwm0_ir_fifo_is_empty(void)
{
    return (reg_pwm_ir_fifo_data_status & FLD_PWM0_IR_FIFO_EMPTY);
}

/**
 * @brief     This function serves to determine whether data in fifo is full.
 * @return    yes: 1 ,no: 0;
 */
static inline unsigned char pwm_get_pwm0_ir_fifo_is_full(void)
{
    return (reg_pwm_ir_fifo_data_status & FLD_PWM0_IR_FIFO_FULL);
}

/**
 * @brief     This function serves to config the pwm's dma wave form.
 * @param[in] pulse_num - the number of pulse.
 * @param[in] shadow_en - whether enable shadow mode.
 * @param[in] carrier_en - must 1 or 0.
 * @return    none.
 */
static inline unsigned short pwm_cal_pwm0_ir_fifo_cfg_data(unsigned short pulse_num, unsigned char shadow_en, unsigned char carrier_en)
{
    return (carrier_en << 15 | (shadow_en << 14) | (pulse_num & 0x3fff));
}

/**
 * @brief     This function serves to write data into fifo
 * @param[in] pulse_num  - the number of pulse
 * @param[in] use_shadow - determine whether the configuration of shadow cmp and shadow max is used
 *                         1: use shadow, 0: not use
 * @param[in] carrier_en - enable sending carrier, 1: enable, 0: disable
 * @return    none
 */
static inline void pwm_set_pwm0_ir_fifo_cfg_data(unsigned short pulse_num, unsigned char use_shadow, unsigned char carrier_en)
{
    static unsigned char index    = 0;
    unsigned short       cfg_data = pwm_cal_pwm0_ir_fifo_cfg_data(pulse_num, use_shadow, carrier_en);
    while (pwm_get_pwm0_ir_fifo_is_full())
        ;
    reg_pwm_ir_fifo_dat(index) = cfg_data;
    index++;
    index &= 0x01;
}

/**
 * @brief     This function servers to configure DMA channel and some configures.
 * @param[in] chn - to select the DMA channel.
 * @return    none
 */
void pwm_set_dma_config(dma_chn_e chn);


/**
 * @brief     This function servers to configure DMA channel address and length.
 * @param[in] chn - to select the DMA channel.
 * @param[in] buf_addr - the address where DMA need to get data from SRAM.
 * @param[in] len - the length of data in SRAM.
 * @return    none
 * @note      buf_addr: must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
void pwm_set_dma_buf(dma_chn_e chn, unsigned int buf_addr, unsigned int len);


/**
 * @brief     This function servers to enable DMA channel.
 * @param[in] chn - to select the DMA channel.
 * @return    none
 */
void pwm_ir_dma_mode_start(dma_chn_e chn);


/**
 * @brief     This function servers to configure DMA head node.
 * @param[in] chn - to select the DMA channel.
 * @param[in] src_addr - to configure DMA source address.
 * @param[in] data_len - to configure DMA length.
 * @param[in] head_of_list - to configure the address of the next node configure.
 * @return    none
 * @note      src_addr: must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
void pwm_set_dma_chain_llp(dma_chn_e chn, unsigned short *src_addr, unsigned int data_len, dma_chain_config_t *head_of_list);


/**
 * @brief     This function servers to configure DMA cycle chain node.
 * @param[in] chn - to select the DMA channel.
 * @param[in] config_addr  - to servers to configure the address of the current node.
 * @param[in] llpoint - to configure the address of the next node configure.
 * @param[in] src_addr - to configure DMA source address.
 * @param[in] data_len - to configure DMA length.
 * @return    none
 * @note      src_addr: must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
void pwm_set_tx_dma_add_list_element(dma_chn_e chn, dma_chain_config_t *config_addr, dma_chain_config_t *llpoint, unsigned short *src_addr, unsigned int data_len);

/**
 * @brief       This function servers to set pwm shift time.
 * @param[in]   id   - variable of enum to select the pwm number,Value range 0~5.
 * @param[in]   shift_clk_num - pwm_pclk_speed(M)*pwm_shift_time(US),pwm_shift_time=tmax/2.
 * @return      none.
 * @attention   1: must: shift time must be set before setting the duty and cycle.
 *              2: If you want to reconfigure shift time, you must reconfiguretcmp and tmax (Whether the tcmp value and tmax value need to be modified or not) again
 *               after reconfiguring shift time.
 *              3: If you only want to reconfigure tcmp and tmax, you only need to configure tcmp and tmax without first configuring shift time.
 */
static inline void pwm_set_shift_time(pwm_id_e id, unsigned short shift_clk_num)
{
    reg_pwm_phase(id) = shift_clk_num;
}

/**
 * @brief     This function servers to enable the pwm center align,the pwm defaults to edge alignment.
 *            Edge alignment:In this mode, the pulse counter is a cyclic increment count,the duty cycle count is determined at the beginning,count initial value is 0.
 *            Center alignment: In this mode, the pulse counter is a two-way count,the duty cycle count is determined at the center,count initial value is 0.
 * @param[in] id -  select the pwm id,Value range 0~5.
 * @return    none.
 */
static inline void pwm_set_align_en(pwm_id_e id)
{
    reg_pwm_align |= BIT(id);
}

/**
 * @brief     This function servers to disable the pwm align.
 * @param[in] id -  select the pwm id,Value range 0~5.
 * @return    none.
 */
static inline void pwm_set_align_dis(pwm_id_e id)
{
    reg_pwm_align &= ~BIT(id);
}

/**
 * @brief     This function servers to disable pwm clock source 32K.
 *            If you use 32K as the PWM clock source, you can work in suspend mode.
 *            but the 32K clock source cannot be divided and can only work in continuous mode and counting mode
 * @param[in] pwm_32K_en_chn - This variable is used to select the specific PWM channel.
 * @return    none.
 */

static inline void pwm_32k_chn_dis(pwm_clk_32k_en_chn_e pwm_32K_en_chn)
{
    BM_CLR(reg_pwm_mode32k, pwm_32K_en_chn);
}
#endif
