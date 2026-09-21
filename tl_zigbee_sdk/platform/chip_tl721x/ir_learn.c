/********************************************************************************************************
 * @file    ir_learn.c
 *
 * @brief   This is the source file for TL721X
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
#include "ir_learn.h"

static dma_config_t g_irlearn_dma_rx_config = {
    .dst_req_sel    = 0,
    .src_req_sel    = DMA_REQ_IRLEARN_RX,
    .dst_addr_ctrl  = DMA_ADDR_INCREMENT,
    .src_addr_ctrl  = DMA_ADDR_FIX,
    .dstmode        = DMA_NORMAL_MODE,
    .srcmode        = DMA_HANDSHAKE_MODE,
    .dstwidth       = DMA_CTR_WORD_WIDTH,
    .srcwidth       = DMA_CTR_WORD_WIDTH,
    .src_burst_size = DMA_BURST_1_WORD,
    .vacant_bit     = 0,
    .read_num_en    = 0,
    .priority       = 0,
    .write_num_en   = 0,
    .auto_en        = 0,
};

static unsigned char s_ir_learn_rx_fifo_hword_index = 0;

/**********************************************************************************************************************
 *                                                ir_learn tx interface                                               *
 **********************************************************************************************************************/
/**
 * @brief      This function servers to set ir_learn tx mode.
 * @param[in]  tx_mode - ir_learn tx mode.
 * @return     none
 */
static void ir_learn_tx_mode(ir_learn_tx_e tx_mode)
{
    switch (tx_mode) {
    case DIGITAL_TX_MODE:
        ir_learn_ana_tx_dis();
        pwm_set_pwm0_output_to_ana_ir_dis();
        break;
    case ANALOG_TX_MODE:
        ir_learn_ana_tx_en();
        pwm_set_pwm0_output_to_ana_ir_en();
        break;
    default:
        break;
    }
}

/**
 * @brief      This function servers to initial ir_learn tx.
 * @param[in]  ir_learn_tx - the pointer of ir_learn_tx_t struct.
 * @return     none
 */
void ir_learn_tx_init(ir_learn_tx_t *ir_learn_tx)
{
    ir_learn_tx_mode(ir_learn_tx->tx_mode);
}

/**********************************************************************************************************************
 *                                                ir_learn rx common interface                                        *
 **********************************************************************************************************************/
/**
 * @brief      This function servers to set ir_learn rx mode.
 * @param[in]  rx_mode - ir_learn rx mode.
 * @return     none
 */
static void ir_learn_rx_mode(ir_learn_rx_e rx_mode)
{
    switch (rx_mode) {
    case DIGITAL_RX_MODE:
        ir_learn_ana_rx_dis();
        break;
    case ANALOG_RX_MODE:
        ir_learn_ana_rx_en();
        break;
    default:
        break;
    }
}

/**
 * @brief      This function servers to set digital rx pin of the ir_learn.
 * @param[in]  ir_learn_rx_pin - the rx pin of ir_learn.
 * @param[in]  pull_type       - digital rx pin pull up or pull down.
 * @return     none
 * @note
 *             - The digital rx has to set the receive pin. The analog rx does not have to set the receive pin.
 *             - The pull-up/down resistor is configured to give the digital IO an idle level.
 */
void ir_learn_set_dig_rx_pin(gpio_pin_e ir_learn_rx_pin, gpio_pull_type_e pull_type)
{
    gpio_input_en(ir_learn_rx_pin);
    gpio_set_up_down_res(ir_learn_rx_pin, pull_type);
    gpio_set_mux_function((gpio_func_pin_e)ir_learn_rx_pin, IR_LEARN_I);
    gpio_function_dis(ir_learn_rx_pin);
}

/**
 * @brief      This function servers to initial ir_learn rx.
 * @param[in]  ir_learn_rx - the pointer of ir_learn_rx_t struct.
 * @return     none
 */
void ir_learn_rx_init(ir_learn_rx_t *ir_learn_rx)
{
    BM_SET(reg_rst5, FLD_RST5_IR_LEARN);
    BM_SET(reg_clk_en5, FLD_CLK5_IR_LEARN_EN);
    ir_learn_rx_mode(ir_learn_rx->rx_mode); /* TODO: A2 chip version need software trig timing to fix the first edge loss issue. */
    reg_ir_learn_ctrl1 = MASK_VAL(FLD_IR_LEARN_MODE, ir_learn_rx->cnt_mode, FLD_IR_LEARN_INV, ir_learn_rx->rx_invert_en, FLD_IR_LEARN_HIGH_WR_EN, ir_learn_rx->high_data_en, FLD_IR_LEARN_CYCLE_WR_EN, ir_learn_rx->cycle_data_en, FLD_IR_LEARN_MODE_2BYTE, ir_learn_rx->data_format, FLD_IR_LEARN_NO_TIMEOUT, !ir_learn_rx->timeout_en, FLD_IR_LEARN_INPUT_SEL, ir_learn_rx->rx_mode);
    ir_learn_dma_auto_clr_rx_fifo_en();     /* dma mode automatically clears the FIFO pointer, otherwise software clearing is required. */
}

/**********************************************************************************************************************
 *                                                ir_learn rx NDMA interface                                          *
 **********************************************************************************************************************/
/**
 * @brief      This function servers to read a half word from rx fifo.
 * @return     half word value of rx fifo.
 * @note
 *             - The clock source of ir_learn is fixed to pclk and cannot be divided.
 */
unsigned short ir_learn_get_data_by_hword(void)
{
    unsigned short rx_data = reg_ir_learn_data_hword_buff(s_ir_learn_rx_fifo_hword_index);
    s_ir_learn_rx_fifo_hword_index++;
    s_ir_learn_rx_fifo_hword_index &= 0x01;
    return rx_data;
}

/**
 * @brief      This function servers to clear software half word read index.
 * @return     none
 * @note
 *             - After ir_learn_hw_fsm_reset hardware fifo ptr auto clear, software ptr should also be cleared.
 *             - After suspend wakeup, hardware fifo ptr are not automatically cleared, so there is no need to clear software pointers,.
 */
void ir_learn_clr_rx_fifo_hword_index(void)
{
    s_ir_learn_rx_fifo_hword_index = 0;
}

/**********************************************************************************************************************
 *                                                ir_learn rx DMA interface                                           *
 **********************************************************************************************************************/
/**
 * @brief      This function serves to set ir_learn receive dma.
 * @param[in]  chn      - dma channel.
 * @param[out] dst_addr - the destination address.
 * @param[in]  len      - the length of data.
 * @return     none.
 */
void ir_learn_receive_dma(dma_chn_e chn, unsigned int *dst_addr, unsigned int len)
{
    dma_config(chn, &g_irlearn_dma_rx_config);
    dma_set_address(chn, reg_ir_learn_fifo_addr, (unsigned int)dst_addr);
    dma_set_size(chn, len, DMA_WORD_WIDTH);
    dma_chn_en(chn);
}

/**
 * @brief      This function serves to set tx dma chain transfer
 * @param[in]  chn         - dma channel.
 * @param[in]  config_addr - the head of list of llp_pointer.
 * @param[in]  llpointer   - the next element of llp_pointer.
 * @param[in]  dst_addr    - pointer to data buffer, it must be 4-bytes aligned address.
 * @param[in]  data_len    - length of DMA in byte.
 * @return     none
 */
void ir_learn_rx_dma_add_list_element(dma_chn_e chn, dma_chain_config_t *config_addr, dma_chain_config_t *llpointer, unsigned char *dst_addr, unsigned int data_len)
{
    config_addr->dma_chain_ctl      = reg_dma_ctrl(chn) | BIT(0);
    config_addr->dma_chain_src_addr = reg_ir_learn_fifo_addr;
    config_addr->dma_chain_dst_addr = (unsigned int)dst_addr;
    config_addr->dma_chain_data_len = dma_cal_size(data_len, 4);
    config_addr->dma_chain_llp_ptr  = (unsigned int)llpointer;
}

/**
 * @brief      This function serves to set ir_learn receive dma chain.
 * @param[in]  chn          - dma channel.
 * @param[out] dst_addr     - the destination address.
 * @param[in]  data_len     - to configure DMA length.
 * @param[in]  head_of_list - to configure the address of the next node configure.
 * @return     none.
 */
void ir_learn_set_dma_chain_llp(dma_chn_e chn, unsigned char *dst_addr, unsigned int data_len, dma_chain_config_t *head_of_list)
{
    dma_config(chn, &g_irlearn_dma_rx_config);
    dma_set_address(chn, reg_ir_learn_fifo_addr, (unsigned int)(dst_addr));
    dma_set_size(chn, data_len, DMA_WORD_WIDTH);
    reg_dma_llp(chn) = (unsigned int)head_of_list;
}
