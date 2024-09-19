/********************************************************************************************************
 * @file    ir_learn.h
 *
 * @brief   This is the source file for TL321X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
/** @page IR_LEARN
 *
 *  Introduction
 *  ===============
 *  The function of IR_LEARN module is mainly counting, there is an internal 24bit counter which can count the
 *  pulse width duration (high) and cycle duration of the IR signal carrier.
 *
 *  -# The module supports two modes to read the data in rxfifo:
 *    - non-DMA mode (read by rx_buf interrupt).
 *    - DMA mode (read by DMA).
 *
 *  API Reference
 *  ===============
 *  Header File: ir_learn.h
 */
#ifndef __IR_LEARN_H__
#define __IR_LEARN_H__
#include "gpio.h"
#include "reg_include/register.h"
#include "dma.h"
#include "pwm.h"

/**
 * @brief ir_learn capture mode
 */
typedef enum
{
    RISING_EDGE_START_CNT,  /**< After enabling the ir_learning module(ir_learn_en), wait for the first rising edge tick to start counting. */
    FALLING_EDGE_START_CNT, /**< After enabling the ir_learning module(ir_learn_en), wait for the first falling edge tick to start counting. */
} ir_learn_start_cnt_mode_e;

/**
 * @brief ir_learn data width.
 */
typedef enum
{
    IR_LEARN_BIT_24_DATA, /**< Make a word of high zeros. */
    IR_LEARN_BIT_16_DATA, /**< Two 16bits of data are concatenated into a word. */
} ir_learn_data_format_e;

/**
 * @brief ir_learn rx mode.
 */
typedef enum
{
    DIGITAL_RX_MODE,
    ANALOG_RX_MODE, /**< Note that the A1 chip version analog RX loses the first triggered edge. */
} ir_learn_rx_e;

/**
 * @brief ir_learn tx mode.
 */
typedef enum
{
    DIGITAL_TX_MODE,
    ANALOG_TX_MODE,
} ir_learn_tx_e;

/**
 * @brief ir_learn rx struct.
 */
typedef struct
{
    bool high_data_en;                  /**< 1: high write enable, 0: high write disable. */
    bool cycle_data_en;                 /**< 1: cycle write enable, 0: cycle write disable.*/
    bool timeout_en;                    /**< 1: timeout enable, 0: timeout disable. */
    bool rx_invert_en;                  /**< 1: rx invert enable, 0: rx invert disable, analog rx mode need enable. */
    ir_learn_start_cnt_mode_e cnt_mode; /**< start count mode select. */
    ir_learn_data_format_e data_format; /**< data format select. */
    ir_learn_rx_e rx_mode;              /**< ir_learn rx signal select. */
} ir_learn_rx_t;

/**
 * @brief ir_learn tx struct.
 */
typedef struct
{
    ir_learn_tx_e tx_mode; /**< ir_learn tx signal select. */
} ir_learn_tx_t;

/**********************************************************************************************************************
 *                                                ir_learn tx interface                                               *
 **********************************************************************************************************************/
/**
 * @brief      This function servers to initial ir_learn tx.
 * @param[in]  ir_learn_tx - the pointer of ir_learn_tx_t struct.
 * @return     none
 */
void ir_learn_tx_init(ir_learn_tx_t *ir_learn_tx);

/**
 * @brief      This function servers to enable ir_learn analog tx.
 * @return     none
 * @note
 *             - If you disable the analog tx function before going to sleep, you need to enable the analog tx function after waking up from sleep.
 */
static inline void ir_learn_ana_tx_en(void)
{
    analog_write_reg8(0x11, (analog_read_reg8(0x11) | 0x80));
}

/**
 * @brief      This function servers to disable ir_learn analog tx.
 * @return     none
 * @note
 *             - If the analog tx function is enabled, you need to disable it before going to sleep to save power consumption.
 */
static inline void ir_learn_ana_tx_dis(void)
{
    analog_write_reg8(0x11, (analog_read_reg8(0x11) & 0x7f));
}

/**********************************************************************************************************************
 *                                                ir_learn rx common interface                                        *
 **********************************************************************************************************************/
/**
 * @brief      This function is used to enable the ir_learn function.
 * @return     none
 */
static inline void ir_learn_en(void)
{
    BM_SET(reg_ir_learn_ctrl0, FLD_IR_LEARN_EN);
}

/**
 * @brief      This function is used to disable the ir_learn function.
 * @return     none
 * @note
 *             - After the module is disable, the tick count value, high register, cycle register will be reset.
 */
static inline void ir_learn_dis(void)
{
    BM_CLR(reg_ir_learn_ctrl0, FLD_IR_LEARN_EN);
}

/**
 * @brief      This function servers to enable ir_learn analog rx.
 * @return     none
 * @note
 *             - If you disable the analog rx function before going to sleep, you need to enable the analog rx function after waking up from sleep.
 */
static inline void ir_learn_ana_rx_en(void)
{
    analog_write_reg8(0x0f, (analog_read_reg8(0x0f) | 0x08));
}

/**
 * @brief      This function servers to disable ir_learn analog rx.
 * @return     none
 * @note
 *             - If the analog rx function is enabled, you need to disable it before going to sleep to save power consumption.
 */
static inline void ir_learn_ana_rx_dis(void)
{
    analog_write_reg8(0x0f, (analog_read_reg8(0x0f) & 0xf7));
}

/**
 * @brief      This function servers to set the tick count mode.
 * @param[in]  cnt_mode - select tick count mode.
 * @return     none.
 */
static inline void ir_learn_set_start_cnt_mode(ir_learn_start_cnt_mode_e cnt_mode)
{
    reg_ir_learn_ctrl1 = (reg_ir_learn_ctrl1 & (~FLD_IR_LEARN_MODE)) | MASK_VAL(FLD_IR_LEARN_MODE, cnt_mode);
}

/**
 * @brief      This function servers to enable ir_learn input invert.
 * @return     none.
 */
static inline void ir_learn_rx_invert_en(void)
{
    BM_SET(reg_ir_learn_ctrl1, FLD_IR_LEARN_INV);
}

/**
 * @brief      This function servers to disable ir_learn input invert.
 * @return     none.
 */
static inline void ir_learn_rx_invert_dis(void)
{
    BM_CLR(reg_ir_learn_ctrl1, FLD_IR_LEARN_INV);
}

/**
 * @brief      This function servers to enable the ir_learn high value write into fifo.
 * @return     none
 */
static inline void ir_learn_high_level_data_en(void)
{
    BM_SET(reg_ir_learn_ctrl1, FLD_IR_LEARN_HIGH_WR_EN);
}

/**
 * @brief      This function servers to disable the ir_learn high value write into fifo.
 * @return     none
 */
static inline void ir_learn_high_level_data_dis(void)
{
    BM_CLR(reg_ir_learn_ctrl1, FLD_IR_LEARN_HIGH_WR_EN);
}

/**
 * @brief      This function servers to enable the ir_learn cycle write into fifo.
 * @return     none
 */
static inline void ir_learn_cycle_data_en(void)
{
    BM_SET(reg_ir_learn_ctrl1, FLD_IR_LEARN_CYCLE_WR_EN);
}

/**
 * @brief      This function servers to disable the ir_learn cycle write into fifo.
 * @return     none
 */
static inline void ir_learn_cycle_data_dis(void)
{
    BM_CLR(reg_ir_learn_ctrl1, FLD_IR_LEARN_CYCLE_WR_EN);
}

/**
 * @brief      This function servers to set data width.
 * @param[in]  data_format - data format select.
 * @return     none
 */
static inline void ir_learn_set_data_format(ir_learn_data_format_e data_format)
{
    reg_ir_learn_ctrl1 = (reg_ir_learn_ctrl1 & ~FLD_IR_LEARN_MODE_2BYTE) | MASK_VAL(FLD_IR_LEARN_MODE_2BYTE, data_format);
}

/**
 * @brief      This function servers to clear software half word read index.
 * @return     none
 * @note
 *             - After ir_learn_hw_fsm_reset hardware fifo ptr auto clear, software ptr should also be cleared.
 *             - After suspend wakeup, hardware fifo ptr are not automatically cleared, so there is no need to clear software pointers,.
 */
void ir_learn_clr_rx_fifo_hword_index(void);

/**
 * @brief     This function serves to ir_learn finite state machine reset(the configuration register is still there and does not need to be reconfigured).
 * @return    none.
 */
static inline void ir_learn_hw_fsm_reset(void)
{
    BM_CLR(reg_rst5, FLD_RST5_IR_LEARN);
    BM_SET(reg_rst5, FLD_RST5_IR_LEARN);
    ir_learn_clr_rx_fifo_hword_index(); /* hardware ptr auto clear, software ptr need clear also. */
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
void ir_learn_set_dig_rx_pin(gpio_pin_e ir_learn_rx_pin, gpio_pull_type_e pull_type);

/**
 * @brief      This function servers to initial ir_learn rx.
 * @param[in]  ir_learn_rx - the pointer of ir_learn_rx_t struct.
 * @return     none
 */
void ir_learn_rx_init(ir_learn_rx_t *ir_learn_rx);

/**********************************************************************************************************************
 *                                                ir_learn rx NDMA interface                                           *
 **********************************************************************************************************************/
/**
 * @brief      This function servers to enable the ir_learn timeout.
 * @return     none
 */
static inline void ir_learn_rx_timeout_en(void)
{
    BM_CLR(reg_ir_learn_ctrl1, FLD_IR_LEARN_NO_TIMEOUT);
}

/**
 * @brief      This function servers to disable the ir_learn timeout.
 * @return     none
 */
static inline void ir_learn_rx_timeout_dis(void)
{
    BM_SET(reg_ir_learn_ctrl1, FLD_IR_LEARN_NO_TIMEOUT);
}

/**
 * @brief      This function servers to set trigger value of the timeout.
 * @param[in]  timeout_value - the timeout value.
 * @return     none
 * @note
 *              - The timeout_val should be greater than the low level duration of the PWM wave.
 */
static inline void ir_learn_set_rx_timeout(unsigned int timeout_value)
{
    reg_ir_learn_timeout_byte(0) = timeout_value & 0xff;
    reg_ir_learn_timeout_byte(1) = (timeout_value >> 8) & 0xff;
    reg_ir_learn_timeout_byte(2) = (timeout_value >> 16) & 0xff;
}

/**
 * @brief      This function serves to set the ir_learn irq mask.
 * @param[in]  mask - variable of enum to select the mask source.
 * @return     none
 */
static inline void ir_learn_set_irq_mask(ir_learn_irq_e mask)
{
    BM_SET(reg_ir_learn_irq_mask, mask);
}

/**
 * @brief      This function serves to clear the ir_learn irq mask.
 * @param[in]  mask - variable of enum to select the mask source.
 * @return     none
 */
static inline void ir_learn_clr_irq_mask(ir_learn_irq_e mask)
{
    BM_CLR(reg_ir_learn_irq_mask, mask);
}

/**
 * @brief      This function serves to get the mask of ir_learn irq.
 * @return     ir_learn irq mask.
 */
static inline unsigned char ir_learn_get_irq_mask(void)
{
    return reg_ir_learn_irq_mask & 0xf;
}

/**
 * @brief        This function is used to clear rx fifo count.
 * @return       none
 * @note
 *               - After the hardware FIFO pointer is cleared, the software pointer needs to be cleared as well(call ir_learn_clr_rx_fifo_hword_index()).
 */
static inline void ir_learn_clr_rx_fifo_cnt(void)
{
    BM_SET(reg_ir_learn_ctrl0, FLD_IR_LEARN_RXFIFO_CLR);
    ir_learn_clr_rx_fifo_hword_index();
}

/**
 * @brief      This function servers to get the ir_learn interrupt status.
 * @param[in]  status - variable of enum to select the ir_learn interrupt source.
 * @return     ir_learn irq status.
 */
static inline unsigned char ir_learn_get_irq_status(ir_learn_irq_e status)
{
    return reg_ir_learn_irq_status & status;
}

/**
 * @brief      This function servers to clear the ir_learn interrupt. \n
 *             When a ir_learn interrupt occurs, the corresponding interrupt flag bit needs to be cleared manually.
 * @param[in]  status  - variable of enum to select the ir_learn interrupt source.
 * @return     none.
 * @note
 *             - Make sure the data in the RX fifo has been read before clearing the interrupt flag bit.
 */
static inline void ir_learn_clr_irq_status(ir_learn_irq_e status)
{
    if (status & FLD_IR_LEARN_TIMEOUT_IRQ)
    {
        ir_learn_clr_rx_fifo_cnt();
    }
    reg_ir_learn_irq_status = status;
}

/**
 * @brief      This function servers to set the number of bytes to triggered the receive interrupt.
 * @param[in]  rx_level - the interrupt trigger level, bit[0-2] valid, range 0~7.
 * @return     none
 * @note
 *             - The rx buff interrupt is triggered when the number of bytes in the rx FIFO is greater than the set trigger level.
 */
static inline void ir_learn_rx_irq_trig_cnt(unsigned char rx_level)
{
    reg_ir_learn_fifo_ctrl = (reg_ir_learn_fifo_ctrl & (~FLD_IR_LEARN_FIFO_THRES)) | (rx_level & 0x07);
}

/**
 * @brief      This function servers to get rx fifo status.
 * @return     rx fifo status.
 */
static inline unsigned char ir_learn_get_rx_fifo_status(ir_learn_fifo_sta_e status)
{
    return reg_ir_learn_fifo_state & status;
}

/**
 * @brief      This function servers to read a word from rx fifo.
 * @return     word value of rx fifo.
 * @note
 *             - The clock source of ir_learn is fixed to pclk and cannot be divided.
 */
static inline unsigned int ir_learn_get_data_by_word(void)
{
    return reg_ir_learn_data_word_buff;
}

/**
 * @brief      This function servers to read a half word from rx fifo.
 * @return     half word value of rx fifo.
 * @note
 *             - The clock source of ir_learn is fixed to pclk and cannot be divided.
 */
unsigned short ir_learn_get_data_by_hword(void);

/**********************************************************************************************************************
 *                                                ir_learn rx DMA interface                                           *
 **********************************************************************************************************************/
/**
 * @brief      This function servers to enable the ir_learn dma mode auto clear buf_cnt.
 * @return     none
 * @note
 *             - If automatic clearing is not enabled in DMA mode, software is required to clear it manually. 
 *             - Auto clear rx fifo This feature is only available in DMA mode.
 */
static inline void ir_learn_dma_auto_clr_rx_fifo_en(void)
{
    BM_SET(reg_ir_learn_fifo_ctrl, FLD_IR_LEARN_FIFO_AUTO_CLR);
}

/**
 * @brief      This function servers to disable the ir_learn dma mode auto clear buf_cnt.
 * @return     none
 * @note
 */
static inline void ir_learn_dma_auto_clr_rx_fifo_dis(void)
{
    BM_CLR(reg_ir_learn_fifo_ctrl, FLD_IR_LEARN_FIFO_AUTO_CLR);
}

/**
 * @brief      This function serves to set ir_learn receive dma.
 * @param[in]  chn      - dma channel.
 * @param[out] dst_addr - the destination address.
 * @param[in]  len      - the length of data.
 * @return     none.
 */
void ir_learn_receive_dma(dma_chn_e chn, unsigned int *dst_addr, unsigned int len);

/**
 * @brief      This function serves to set tx dma chain transfer
 * @param[in]  chn         - dma channel.
 * @param[in]  config_addr - the head of list of llp_pointer.
 * @param[in]  llpointer   - the next element of llp_pointer.
 * @param[in]  dst_addr    - pointer to data buffer, it must be 4-bytes aligned address.
 * @param[in]  data_len    - length of DMA in byte.
 * @return     none
 */
void ir_learn_rx_dma_add_list_element(dma_chn_e chn, dma_chain_config_t *config_addr, dma_chain_config_t *llpointer, unsigned char *dst_addr,
                                      unsigned int data_len);

/**
 * @brief      This function serves to set ir_learn receive dma chain.
 * @param[in]  chn          - dma channel.
 * @param[out] dst_addr     - the destination address.
 * @param[in]  data_len     - to configure DMA length.
 * @param[in]  head_of_list - to configure the address of the next node configure.
 * @return     none.
 */
void ir_learn_set_dma_chain_llp(dma_chn_e chn, unsigned char *dst_addr, unsigned int data_len, dma_chain_config_t *head_of_list);

#endif /* __IR_LEARN_H__ */
