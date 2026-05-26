/********************************************************************************************************
 * @file    pwm.c
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
#include "pwm.h"


dma_config_t pwm_tx_dma_config = {
    .dst_req_sel    = DMA_REQ_PWM_TX,     //tx req
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
    .auto_en        = 0,
};

/**
 * @brief     This function servers to set pin as pwm.
 * @param[in] pin - selected pin
 * @return    none.
 */
void pwm_set_pin(pwm_pin_e pin)
{
    unsigned char val       = 0;
    unsigned char start_bit = (BIT_LOW_BIT(pin & 0xff) % 4) << 1;
    unsigned char mask      = (unsigned char)~BIT_RNG(start_bit, start_bit + 1);

    if (pin == PWM_PWM2_PB7) {                                                                                         // Pad Function Mux:0
        val = 0;
        BM_CLR(reg_gpio_pad_mul_sel, BIT(3));
    } else if ((pin == PWM_PWM0_PB4) || (pin == PWM_PWM4_PD7) || (pin == PWM_PWM2_N_PE6) || (pin == PWM_PWM3_N_PE7)) { // Pad Function Mux:1
        val = 1 << (start_bit);
    } else {                                                                                                           // Pad Function Mux:2
        val = 2 << (start_bit);
        reg_gpio_pad_mul_sel |= BIT(0);
    }

    reg_gpio_func_mux(pin) = (reg_gpio_func_mux(pin) & mask) | val;
    gpio_function_dis(pin);
}

/**
 * @brief     This function servers to configure DMA channel and some configures.
 * @param[in] chn - to select the DMA channel.
 * @return    none
 * @note      In the case that the DMA transfer is not completed(bit 0 of reg_dma_ctr0(chn): 1-the transmission has not been completed,0-the transmission is completed), re-calling the DMA-related functions may cause problems.
 *            If you must do this, you must perform the following sequence:
 *            1. dma_chn_dis(chn) 2.pwm_reset()3.pwm_set_dma_buf()/pwm_ir_dma_mode_start()
 */
void pwm_set_dma_config(dma_chn_e chn)
{
    dma_config(chn, &pwm_tx_dma_config);
}

/**
 * @brief     This function servers to configure DMA channel address and length.
 * @param[in] chn - to select the DMA channel.
 * @param[in] buf_addr - the address where DMA need to get data from SRAM.
 * @param[in] len - the length of data in SRAM.
 * @return    none
 * @note      buf_addr: must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
void pwm_set_dma_buf(dma_chn_e chn, unsigned int buf_addr, unsigned int len)
{
    dma_set_address(chn, buf_addr, reg_pwm_data_buf_adr);
    dma_set_size(chn, len, DMA_WORD_WIDTH);
}

/**
 * @brief     This function servers to enable DMA channel.
 * @param[in] chn - to select the DMA channel.
 * @return    none
 */
void pwm_ir_dma_mode_start(dma_chn_e chn)
{
    dma_chn_en(chn);
}

/**
 * @brief     This function servers to configure DMA head node.
 * @param[in] chn - to select the DMA channel.
 * @param[in] src_addr - to configure DMA source address.
 * @param[in] data_len - to configure DMA length.
 * @param[in] head_of_list - to configure the address of the next node configure.
 * @return    none
 * @note      src_addr: must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
void pwm_set_dma_chain_llp(dma_chn_e chn, unsigned short *src_addr, unsigned int data_len, dma_chain_config_t *head_of_list)
{
    /*1.DMA is not finished yet, need to disable dma before writing to the dma register.
     *2.In order not to affect the interaction between pwm and dma, reset pwm is required after dma is disabled.
     * confirmed by jianzhi,modify by minghai.duan(20211025)*/
    dma_chn_dis(chn);
    pwm_reset();
    dma_config(chn, &pwm_tx_dma_config);
    dma_set_address(chn, (unsigned int)src_addr, reg_pwm_data_buf_adr);
    dma_set_size(chn, data_len, DMA_WORD_WIDTH);
    reg_dma_llp(chn) = (unsigned int)convert_ram_addr_cpu2bus(head_of_list);
}

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
void pwm_set_tx_dma_add_list_element(dma_chn_e chn, dma_chain_config_t *config_addr, dma_chain_config_t *llpoint, unsigned short *src_addr, unsigned int data_len)
{
    config_addr->dma_chain_ctl      = reg_dma_ctrl(chn) | BIT(0);
    config_addr->dma_chain_src_addr = (unsigned int)convert_ram_addr_cpu2bus(src_addr);
    config_addr->dma_chain_dst_addr = reg_pwm_data_buf_adr;
    config_addr->dma_chain_data_len = dma_cal_size(data_len, DMA_WORD_WIDTH);
    config_addr->dma_chain_llp_ptr  = (unsigned int)convert_ram_addr_cpu2bus(llpoint);
}
