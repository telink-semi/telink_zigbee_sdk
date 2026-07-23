/********************************************************************************************************
 * @file    keyscan.h
 *
 * @brief   This is the source file for tl521x
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
/**    @page KEYSCAN
 *
 *    Introduction
 *    ===============
 *    1. Keyscan can only save 9 keys at the same time in the same row, and can save 30 keys at the same time in different rows.
 *    2. In each debounce cycle, the hardware will finally store an end flag in the fifo.
 *    3. The duration of a single key must be more than 16ms.
 *
 *    API Reference
 *    ===============
 *    Header File: keyscan.h
 */
#ifndef KEYSCAN_H_
#define KEYSCAN_H_

#include "lib/include/analog.h"
#include "reg_include/keyscan_reg.h"
#include "gpio.h"
#include "dma.h"

/**
 * @brief        Set the end flag of the hardware upload key value after each keyscan interrupt is generated.
 *                 Since the configured end_flag is an 8bit value and cannot conflict with the keyboard key value,
 *                 and only number "27" of the column number "0-31" can not be configured as a keyscan column,
 *                 so select "0xfb" to configure the end flag of the hardware upload key value .
 */
#define KESYCAN_END_FLAG 0xff

/**
 *  @brief      KEYSCAN pin value
 *  @note       Any one can be chosen between the two. It should be noted that if the IO of the second group is used, the IO of the first group can only be in GPIO function.
 */
typedef enum
{
    KS_PD3 = 0,       KS_PH3 =BIT(5)|0,
    KS_PD4 = 1,       KS_PH4 =BIT(5)|1,
    KS_PD5 = 2,       KS_PH5 =BIT(5)|2,
    KS_PD6 = 3,       KS_PH6 =BIT(5)|3,
    KS_PD7 = 4,       KS_PH7 =BIT(5)|4,
    KS_PE0 = 5,
    KS_PE1 = 6,
    KS_PE2 = 7,
    KS_PC3 = 8,        KS_PG3 =BIT(5)|8,
    KS_PC4 = 9,        KS_PG4 =BIT(5)|9,
    KS_PC5 = 10,       KS_PG5 =BIT(5)|10,
    KS_PC6 = 11,       KS_PG6 =BIT(5)|11,
    KS_PC7 = 12,       KS_PG7 =BIT(5)|12,
    KS_PD0 = 13,       KS_PH0 =BIT(5)|13,
    KS_PD1 = 14,       KS_PH1 =BIT(5)|14,
    KS_PD2 = 15,       KS_PH2 =BIT(5)|15,
    KS_PB3 = 16,       KS_PF3 =BIT(5)|16,
   /*KS_PB4 = 17,*/    KS_PF4 =BIT(5)|17,
   /*KS_PB5 = 18,*/    KS_PF5 =BIT(5)|18,
   /*KS_PB6 = 19,*/    KS_PF6 =BIT(5)|19,
    KS_PB7 = 20,       KS_PF7 =BIT(5)|20,
    KS_PC0 = 21,       KS_PG0 =BIT(5)|21,
    KS_PC1 = 22,       KS_PG1 =BIT(5)|22,
    KS_PC2 = 23,       KS_PG2 =BIT(5)|23,
    KS_PA0 = 24,       KS_PE3 =BIT(5)|24,
    KS_PA1 = 25,       KS_PE4 =BIT(5)|25,
    KS_PA2 = 26,       KS_PE5 =BIT(5)|26,
    KS_PA3 = 27,       KS_PE6 =BIT(5)|27,
    KS_PA4 = 28,       KS_PE7 =BIT(5)|28,
    KS_PB0 = 29,       KS_PF0 =BIT(5)|29,
    KS_PB1 = 30,       KS_PF1 =BIT(5)|30,
   /*KS_PB2 = 31,       KS_PF2 =BIT(5)|31,*/
} ks_value_e;

/**
 *  @brief      KEYSCAN scan time
 *              If the debounce period is 8ms,the triple scan times will check the key values at the three time points of 0/4/8,and the double scan times will check the key values at the two time points of 0/8.
 *              Write the same key value to fifo.
 */
typedef enum
{
    DOUBLE_SCAN_TIMES = 0, /**< Scan double times in one debounce cycle */
    TRIPLE_SCAN_TIMES = 1, /**< Scan triple times in one debounce cycle */
} ks_scan_times_e;

/**
 *  @brief      KEYSCAN Debounce Period
 *              The debounce period is an integer multiple of a scan period, and the scan period is given by hardware and is fixed at 4ms.
 */
typedef enum
{
    DEBOUNCE_PERIOD_8MS  = 0x2,
    DEBOUNCE_PERIOD_12MS = 0x3,
    DEBOUNCE_PERIOD_16MS = 0x4,
    DEBOUNCE_PERIOD_20MS = 0x5,
    DEBOUNCE_PERIOD_24MS = 0x6,
    DEBOUNCE_PERIOD_28MS = 0x7,
} ks_debounce_period_e;

/**
 *  @brief      Define pull up or down types
 *              The column GPIO must remain unfloating.
 */
typedef enum
{
    KS_EXT_PIN_PULLUP   = (0x01 << 4) | GPIO_PIN_UP_DOWN_FLOAT, /**< External pull-up.When the column of the keyscan is configured as pull-up, when the column is detected as low level, it means that the button is pressed */
    KS_EXT_PIN_PULLDOWN = (0x02 << 4) | GPIO_PIN_UP_DOWN_FLOAT, /**< External pull-down.When the column of the keyscan is configured as pull-down, when the column is detected as high level, it means that the button is pressed */
    KS_INT_PIN_PULLUP   = (0x01 << 4) | GPIO_PIN_PULLUP_20K,    /**< Internal pull-up.When the column of the keyscan is configured as pull-up, when the column is detected as low level, it means that the button is pressed */
    KS_INT_PIN_PULLDOWN = (0x02 << 4) | GPIO_PIN_PULLDOWN_100K, /**< Internal pull-down.When the column of the keyscan is configured as pull-down, when the column is detected as high level, it means that the button is pressed */
} ks_col_pull_type_e;


typedef enum{
    KEYSCAN_8K = 8,
} ks_baud_rate_e;

typedef enum{
    KS_24MRC =1,
    KS_24MXTAL=2,
} ks_clk_src_e;


/**
 * @brief         This function servers to clear irq status.
 *                 After the hardware writes the key value into the fifo in each debounce period, an interrupt is triggered until it enters the idle state.
 * @return      none.
 */
static inline void keyscan_set_irq_mask(void)
{
    reg_ks_mask |= FLD_KS_FRM_END;
}

/**
 * @brief         This function servers to clear irq status.
 *                 After the hardware writes the key value into the fifo in each debounce period, an interrupt is triggered until it enters the idle state.
 * @return      none.
 */
static inline void keyscan_clr_irq_status(void)
{
    reg_ks_irq |= FLD_KS_FRM_END;
}

static inline void keyscan_clr_rxdone_irq_status(void)
{
     reg_ks_gated = FLD_KS_RXDONE_IRQ;
}
/**
 * @brief         This function servers to get write pointer.
 * @return      the value of the write pointer.
 *              Every time the hardware writes a key value into fifo, the write pointer increases by one.
 */
static inline unsigned char keyscan_get_irq_status(void)
{
    return reg_ks_irq & FLD_KS_FRM_END;
}

static inline unsigned char keyscan_get_rxdone_irq_status(void)
{
    return reg_ks_gated &FLD_KS_RXDONE_IRQ;
}
/**
 * @brief       This function servers to get read pointer.
 * @return      the value of the read pointer.
 *              Every time the software reads a key value from fifo, the read pointer increases by one.
 */
static inline unsigned char keyscan_get_rptr(void)
{
    return reg_ks_irq & FLD_KS_RPTR;
}

/**
 * @brief       This function servers to get write pointer.
 * @return      the value of the write pointer.
 *              Every time the hardware writes a key value into fifo, the write pointer increases by one.
 */
static inline unsigned char keyscan_get_wptr(void)
{
    return reg_ks_rptr & FLD_KS_R_WPTR;
}

/**
 * @brief         This function serves to enable keyscan module.
 * @return         none
 */
static inline void keyscan_enable(void)
{
    BM_SET(reg_ks_en, FLD_KS_EN);
}

/**
 * @brief         This function serves to disable keyscan module.
 * @return         none
 */
static inline void keyscan_disable(void)
{
    BM_CLR(reg_ks_en, FLD_KS_EN);
}

/**
 * @brief         This function serves to enable keyscan dma.
 * @return         none
 */
static inline void keyscan_dma_enable(void)
{
    BM_SET(reg_ks_a_en0, FLD_KS_RXDMA_EN); //keyscan dma enable
}

/**
 * @brief      This function serves to configure keyscan DMA.
 * @param[in]  chn       - DMA channel
 * @param[in]  data_buffer  - the DMA data_buffer address
 * @param[in]  size_byte - DMA buffer size
 * @return     none
 */
void keyscan_dma_config(dma_chn_e chn, unsigned int *data_buffer, unsigned char size_byte);

/**
 * @brief     This function serves to configure adc_dma_chn channel llp.
 * @param[in] chn          - the DMA channel
 * @param[in] data_buffer  - the DMA data_buffer address
 * @param[in] config       - the DMA config
 * @param[in] head_of_list - the DMA linked list head
 * @param[in] size_byte    - the DMA buffer size
 * @return    none
 */
void keyscan_dma_config_llp_list(dma_chn_e chn, unsigned int *data_buffer, unsigned int size_byte, dma_chain_config_t *head_of_list);

/**
 * @brief      This function serves to configure adc_dma_chn channel list element.
 * @param[in]  chn          - the DMA channel
 * @param[in]  data_buffer  - the DMA data_buffer address
 * @param[in]  node         - the list node
 * @param[in]  llpointer    - the linked list pointer
 * @param[in]  size_byte    - the DMA buffer size
 * @return     none
 */
void keyscan_dma_add_list_element(dma_chn_e chn, unsigned int *data_buffer, dma_chain_config_t *node, dma_chain_config_t *llpointer, unsigned int size_byte);

/**
 * @brief      This function serves to configure keyscan DMA LLP.
 * @param[in]  chn       - DMA channel
 * @param[in]  data_buffer  - the DMA data_buffer address
 * @param[in]  size_byte - DMA buffer size
 * @return     none
 */
void keyscan_dma_config_llp(dma_chn_e chn, unsigned int *data_buffer, unsigned char size_byte);

/**
 * @brief      This function serves to set the rows and columns of the keyscan .
 * @param[in]  ks_row       - The array element must be a member of the enumeration type ks_value_e.
 * @param[in]  row_cnt      - Count of rows. Range is 1-8.
 * @param[in]  ks_col       - The array element must be a member of the enumeration type ks_value_e.
 * @param[in]  col_cnt      - Count of columns. Range is 1-31.
 * @param[in]  ks_col_pull  - Configure the type of pull-up/pull-down resistors for column GPIO.
 * @return     none
 */
void keyscan_set_martix(unsigned char *ks_row, unsigned char row_cnt, unsigned char *ks_col, unsigned char col_cnt, ks_col_pull_type_e ks_col_pull);

/**
 * @brief       This function serves to get the key value.
 *              The key value "0xfb"(KESYCAN_END_FLAG) is the end sign, which indicates the end of a debounce period. It is written into fifo by the hardware after uploading the last valid key value.
 *              Each time the software reads the key value, the read pointer will increase by 1.
 *              Each time the hardware writes the key value, the write pointer will increase by 1.
 * @return      key value - bit[7:5] represents the row number, the range is from 0 to 7.
 *                          bit[4:0] represents the column number, the range is from 0 to 30.
 */
unsigned char keyscan_get_ks_value(void);

/**
 * @brief      This function serves to set keyscan configuration.
 * @param[in]  debounce_period          - Set the time of a debounce cycle.
 * @param[in]  enter_idle_period_num    - Set how many debounce cycles to enter the idle state without detecting a button.
 *                                        The value setting range is 1-32.
 *                                        The idle state will stop scanning and reduce power consumption.
 *                                        Press again to exit the idle state.
 * @param[in]  scan_times               - 1 is to set triple scan times,0 is to set double scan times.
 * @return     none
 */
void keyscan_init(ks_debounce_period_e debounce_period, unsigned char enter_idle_period_num, ks_scan_times_e scan_times);

/**
 * @brief      This function serves to set keyscan configuration.
 * @param[in]  baud_rate                - ks_baud_rate_e.
 * @param[in]  clk_src                  - ks_clk_src_e.
 * @param[in]  enter_idle_period_num    - Set how many debounce cycles to enter the idle state without detecting a button.
 *                                        The value setting range is 1-32.
 *                                        The idle state will stop scanning and reduce power consumption.
 *                                        Press again to exit the idle state.
 * @return     none
 */
void keyscan_init_clk_24m(ks_baud_rate_e baud_rate, ks_clk_src_e clk_src, unsigned char enter_idle_period_num);

#endif /* KEYSCAN_H_ */
