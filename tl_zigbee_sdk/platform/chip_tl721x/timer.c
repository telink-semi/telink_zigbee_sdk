/********************************************************************************************************
 * @file    timer.c
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
#include "timer.h"
#include "driver.h"

/**********************************************************************************************************************
 *                                              global variable                                                       *
 *********************************************************************************************************************/
dma_config_t timer_rx_dma_config[2] = {
    {
     .dst_req_sel    = 0, //tx req
        .src_req_sel    = DMA_REQ_TIMER0_RX,
     .dst_addr_ctrl  = DMA_ADDR_INCREMENT,
     .src_addr_ctrl  = DMA_ADDR_FIX,
     .dstmode        = DMA_NORMAL_MODE,
     .srcmode        = DMA_HANDSHAKE_MODE,
     .dstwidth       = DMA_CTR_WORD_WIDTH, //must be word
        .srcwidth       = DMA_CTR_WORD_WIDTH, //must be word
        .src_burst_size = 0,
     .read_num_en    = 0,
     .priority       = 0,
     .write_num_en   = 0,
     .auto_en        = 0, //must be 0
    },
    {
     .dst_req_sel    = 0,                    //tx req
        .src_req_sel    = DMA_REQ_TIMER1_RX,
     .dst_addr_ctrl  = DMA_ADDR_INCREMENT,
     .src_addr_ctrl  = DMA_ADDR_FIX,
     .dstmode        = DMA_NORMAL_MODE,
     .srcmode        = DMA_HANDSHAKE_MODE,
     .dstwidth       = DMA_CTR_WORD_WIDTH, //must be word
        .srcwidth       = DMA_CTR_WORD_WIDTH,                                              //must be word
        .src_burst_size = 0,
     .read_num_en    = 0,
     .priority       = 0,
     .write_num_en   = 0,
     .auto_en        = 0, //must be 0
    }
};

/**********************************************************************************************************************
 *                                              local variable                                                     *
 *********************************************************************************************************************/
static unsigned char timer_dma_rx_chn[2];

/**********************************************************************************************************************
 *                                         global function implementation                                             *
 *********************************************************************************************************************/

/**
 * @brief     the specified timer start working.
 * @param[in] type - select the timer to start.
 * @return    none
 */
void timer_start(timer_type_e type)
{
    switch (type) {
    case TIMER0:
        reg_tmr_ctrl0 |= FLD_TMR0_EN;
        break;
    case TIMER1:
        reg_tmr_ctrl0 |= FLD_TMR1_EN;
        break;
    default:
        break;
    }
}

/**
 * @brief     the specified timer stop working.
 * @param[in] type - select the timer to stop.
 * @return    none
 */
void timer_stop(timer_type_e type)
{
    switch (type) {
    case TIMER0:
        reg_tmr_ctrl0 &= (~FLD_TMR0_EN);
        break;
    case TIMER1:
        reg_tmr_ctrl0 &= (~FLD_TMR1_EN);
        break;
    default:
        break;
    }
}

/**
 * @brief     set mode of timer.
 * @param[in] type - select the timer to start.
 * @param[in] mode - select mode for timer.
 * @return    none
 */
void timer_set_mode(timer_type_e type, timer_mode_e mode)
{
    switch (type) {
    case TIMER0:
        reg_tmr_sta1 = FLD_TMR0_MODE_IRQ; //clear irq status
        reg_tmr_ctrl0 &= (~FLD_TMR0_MODE);
        reg_tmr_ctrl0 |= mode;
        break;
    case TIMER1:
        reg_tmr_sta1 = FLD_TMR1_MODE_IRQ; //clear irq status
        reg_tmr_ctrl0 &= (~FLD_TMR1_MODE);
        reg_tmr_ctrl0 |= (mode << 4);
        break;
    default:
        break;
    }
}

/**
 * @brief     initiate GPIO for gpio trigger and gpio width mode of timer.
 * @param[in] type - select the timer to start.
 * @param[in] pin - select pin for timer.
 * @param[in] pol - select polarity for gpio trigger and gpio width
 * @return    none
 */
void timer_gpio_init(timer_type_e type, gpio_pin_e pin, gpio_pol_e pol)
{
    gpio_function_en(pin);
    gpio_output_dis(pin); //disable output
    gpio_input_en(pin);   //enable input
    switch (type) {
    case TIMER0:
        if (pol == POL_FALLING) {
            gpio_set_up_down_res(pin, GPIO_PIN_PULLUP_10K);
            gpio_set_gpio2risc0_irq(pin, INTR_LOW_LEVEL);
        } else if (pol == POL_RISING) {
            gpio_set_up_down_res(pin, GPIO_PIN_PULLDOWN_100K);
            gpio_set_gpio2risc0_irq(pin, INTR_HIGH_LEVEL);
        }
        break;

    case TIMER1:
        if (pol == POL_FALLING) {
            gpio_set_up_down_res(pin, GPIO_PIN_PULLUP_10K);
            gpio_set_gpio2risc1_irq(pin, INTR_LOW_LEVEL);
        } else if (pol == POL_RISING) {
            gpio_set_up_down_res(pin, GPIO_PIN_PULLDOWN_100K);
            gpio_set_gpio2risc1_irq(pin, INTR_HIGH_LEVEL);
        }
        break;

    default:
        break;
    }
}

/**
 * @brief     This function serves to set timer rx_dam channel and config dma rx default.
 * @param[in] type - TIMER0 or TIMER1.
 * @param[in] chn  - dma channel.
 * @return    none
 */
void timer_set_rx_dma_config(timer_type_e type, dma_chn_e chn)
{
    timer_dma_rx_chn[type] = chn;
    dma_config(chn, &timer_rx_dma_config[type]);
}

/**
 * @brief       This function serves to receive data function by DMA, this function tell the DMA to get data from the timer data fifo.
 *              1. if the receiving length information of DMA is set to 0xFFFFFC byte(max_value), and write_num is turned on,
 *                 then The length of the data received by dma will be written back to the first four bytes of addr.
 *              2. if the receiving length information of DMA is set to less than 0xFFFFFC byte, and write_num is turned on,
 *                 then the length of data received by DMA will not be written to the first four bytes of addr.
 * @param[in]   type     - TIMER0 or TIMER1.
 * @param[in]   addr     - pointer to the buffer receive data.
 * @param[in]   rev_size - the receive length of DMA,The maximum transmission length of DMA is 0xFFFFFC bytes, so dont'n over this length.
 * @return      none
 */
void timer_receive_dma(timer_type_e type, unsigned char *addr, unsigned int rev_size)
{
    dma_set_address(timer_dma_rx_chn[type], REG_TMR_CCAPT(type), (unsigned int)addr);
    dma_set_size(timer_dma_rx_chn[type], rev_size, DMA_WORD_WIDTH);
    dma_chn_en(timer_dma_rx_chn[type]);
}

/**
 * @brief     This function servers to configure DMA head node,the chain function only applies to data_len = 0xFFFFFC.
 * @param[in] type - TIMER0/TIMER1.
 * @param[in] chn          - to select the DMA channel.
 * @param[in] dst_addr     - the dma address of destination
 * @param[in] data_len     - to configure DMA length.
 * @param[in] head_of_list - the head address of dma llp.
 * @return    none
 */
void timer_set_dma_chain_llp(timer_type_e type, dma_chn_e chn, unsigned char *dst_addr, unsigned int data_len, dma_chain_config_t *head_of_list)
{
    timer_set_rx_dma_config(type, chn);
    dma_set_address(chn, REG_TMR_CCAPT(type), (unsigned int)dst_addr);
    dma_set_size(chn, data_len, DMA_WORD_WIDTH);
    reg_dma_llp(chn) = (unsigned int)(head_of_list);
}

/**
 * @brief     This function servers to configure DMA cycle chain node.
 * @param[in] type - TIMER0/TIMER1.
 * @param[in] chn - to select the DMA channel.
 * @param[in] config_addr  - to servers to configure the address of the current node.
 * @param[in] llpoint - to configure the address of the next node configure.
 * @param[in] src_addr - to configure DMA source address.
 * @param[in] data_len - to configure DMA length.
 * @return    none
 */
void timer_set_rx_dma_add_list_element(timer_type_e type, dma_chn_e chn, dma_chain_config_t *config_addr, dma_chain_config_t *llpoint, unsigned short *dst_addr, unsigned int data_len)
{
    config_addr->dma_chain_ctl      = reg_dma_ctrl(chn) | BIT(0);
    config_addr->dma_chain_src_addr = REG_TMR_CCAPT(type);
    config_addr->dma_chain_dst_addr = (unsigned int)(dst_addr);
    config_addr->dma_chain_data_len = dma_cal_size(data_len, DMA_WORD_WIDTH);
    config_addr->dma_chain_llp_ptr  = (unsigned int)(llpoint);
}

/*
 * @brief     This function set timer input capture mode.
 * @param[in] capt_mode  - timer input capture mode.
 * @return    none.
 */
void timer_set_input_capture_mode(timer_type_e type, timer_capt_mode_e capt_mode, gpio_pin_e pin)
{
    gpio_function_en(pin);
    gpio_output_dis(pin); //disable output
    gpio_input_en(pin);   //enable input
    switch(type)
        {
            case TIMER0:
                reg_tmr_sta1 = FLD_TMR0_CAPT_IRQ;//clear irq status
                reg_tmr_ctrl0 &= (~FLD_TMR0_MODE);
                reg_tmr_ctrl1 &= (~FLD_TMR0_CAPT_MODE);
                reg_tmr_ctrl1 |= capt_mode;

                gpio_gpio2risc0_irq_en(pin);
                BM_SET(reg_gpio_irq_ctrl, FLD_GPIO_IRQ_LVL_GPIO2RISC0);
                reg_gpio_irq_clr = FLD_GPIO_IRQ_GPIO2RISC0_CLR;//must clear cause to unexpected interrupt.
                gpio_set_irq_mask(GPIO_IRQ_MASK_GPIO2RISC0);//the mask of gpio2risc0 needs to be turned on for the signal to go from gpio to timer0

                break;
            case TIMER1:
                reg_tmr_sta1 = FLD_TMR1_CAPT_IRQ;//clear irq status
                reg_tmr_ctrl0 &= (~FLD_TMR1_MODE);
                reg_tmr_ctrl1 &= (~FLD_TMR1_CAPT_MODE);
                reg_tmr_ctrl1 |= capt_mode<<4;

                gpio_gpio2risc1_irq_en(pin);
                BM_SET(reg_gpio_irq_ctrl, FLD_GPIO_IRQ_LVL_GPIO2RISC1);
                reg_gpio_irq_clr =FLD_GPIO_IRQ_GPIO2RISC1_CLR;//must clear cause to unexpected interrupt.
                gpio_set_irq_mask(GPIO_IRQ_MASK_GPIO2RISC1);//the mask of gpio2risc1 needs to be turned on for the signal to go from gpio to timer1

                break;
            default:
                break;
        }
    timer_input_capture_en(type);
}

/*
 * @brief     This function is used to reset the tick value to 0 when it is equal to the preset capture tick value or when a capture is triggered.
 * @param[in] type - TIMER0 or TIMER1.
 * @param[in] reset - 1: reset the tick  0: do not reset the tick
 * @return    none.
 */
void timer_reset_tick(timer_type_e type, unsigned char reset)
{
  unsigned char mask = 0;

  switch(type) {
    case TIMER0:
      mask = FLD_TMR0_NOWRAP;
      break;
    case TIMER1:
      mask = FLD_TMR1_NOWRAP;
      break;
    default:
      break;
  }

  if(mask != 0)
  {
      if(reset != 0) {
        BM_CLR(reg_tmr_ctrl0, mask);
      } else {
        BM_SET(reg_tmr_ctrl0, mask);
      }
  }

}

