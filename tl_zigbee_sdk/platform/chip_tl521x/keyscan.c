/********************************************************************************************************
 * @file    keyscan.c
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
#include "keyscan.h"

#define KEYSCAN_BUFFER_DMA_ADDR 0x8014071c

dma_chain_config_t ks_rx_dma_list_config;

dma_config_t keyscan_rx_dma_config =
    {
        .dst_req_sel    = 0,
        .src_req_sel    = DMA_REQ_KS_RX,
        .dst_addr_ctrl  = DMA_ADDR_INCREMENT,
        .src_addr_ctrl  = DMA_ADDR_FIX,
        .dstmode        = DMA_NORMAL_MODE,
        .srcmode        = DMA_HANDSHAKE_MODE,
        .dstwidth       = DMA_CTR_WORD_WIDTH, //must word
        .srcwidth       = DMA_CTR_WORD_WIDTH, //must word
        .src_burst_size = 0,                  //must 0
        .read_num_en    = 0,
        .priority       = 0,
        .write_num_en   = 1,
        .auto_en        = 0, //must 0
};

volatile unsigned char g_keyscan_col_cnt = 0;
volatile unsigned char g_keyscan_col[31] = {0};

unsigned int g_col_mask = 0x00;

/**
 * @brief      This function serves to configure keyscan DMA.
 * @param[in]  chn       - DMA channel
 * @param[in]  data_buffer  - the DMA data_buffer address
 * @param[in]  size_byte - DMA buffer size
 * @return     none
 */
void keyscan_dma_config(dma_chn_e chn, unsigned int *data_buffer, unsigned char size_byte)
{
    dma_set_address(chn, KEYSCAN_BUFFER_DMA_ADDR, (unsigned int)data_buffer);
    dma_set_size(chn, size_byte, DMA_WORD_WIDTH);
    dma_config(chn, &keyscan_rx_dma_config);
    dma_chn_en(chn);
}

/**
 * @brief     This function serves to configure adc_dma_chn channel llp.
 * @param[in] chn          - the DMA channel
 * @param[in] data_buffer  - the DMA data_buffer address
 * @param[in] config       - the DMA config
 * @param[in] head_of_list - the DMA linked list head
 * @param[in] size_byte    - the DMA buffer size
 * @return    none
 */
void keyscan_dma_config_llp_list(dma_chn_e chn, unsigned int *data_buffer, unsigned int size_byte, dma_chain_config_t *head_of_list)
{
    dma_set_address(chn, KEYSCAN_BUFFER_DMA_ADDR, (unsigned int)data_buffer);
    dma_set_size(chn, size_byte, DMA_WORD_WIDTH);
    dma_config(chn, &keyscan_rx_dma_config);
    reg_dma_llp(chn) = (unsigned int)(head_of_list);
}

/**
 * @brief      This function serves to configure adc_dma_chn channel list element.
 * @param[in]  chn          - the DMA channel
 * @param[in]  data_buffer  - the DMA data_buffer address
 * @param[in]  node         - the list node
 * @param[in]  llpointer    - the linked list pointer
 * @param[in]  size_byte    - the DMA buffer size
 * @return     none
 */
void keyscan_dma_add_list_element(dma_chn_e chn, unsigned int *data_buffer, dma_chain_config_t *node, dma_chain_config_t *llpointer, unsigned int size_byte)
{
    node->dma_chain_ctl      = reg_dma_ctrl(chn) | BIT(0);
    node->dma_chain_src_addr = KEYSCAN_BUFFER_DMA_ADDR;
    node->dma_chain_dst_addr = (unsigned int)(data_buffer);
    node->dma_chain_data_len = dma_cal_size(size_byte, 4);
    node->dma_chain_llp_ptr  = (unsigned int)(llpointer);
}

/**
 * @brief      This function serves to configure keyscan DMA LLP.
 * @param[in]  chn       - DMA channel
 * @param[in]  data_buffer  - the DMA data_buffer address
 * @param[in]  size_byte - DMA buffer size
 * @return     none
 */
void keyscan_dma_config_llp(dma_chn_e chn, unsigned int *data_buffer, unsigned char size_byte)
{
    keyscan_dma_config_llp_list(chn, data_buffer, size_byte, &ks_rx_dma_list_config);
    keyscan_dma_add_list_element(chn, data_buffer, &ks_rx_dma_list_config, &ks_rx_dma_list_config, size_byte);
    dma_chn_en(chn);
}

/**
 * @brief      This function serves to map keyscan pin into corresponding gpio pin.
 * @param[in]  pin          - keyscan pin
 * @return     unsigned int - gpio pin
 */
static unsigned int keyscan_pin_map(unsigned char pin)
{
    unsigned int gpio_pin;
    if(pin & BIT(5)){
         pin = pin&(~BIT(5));
         if (pin > 23) {     //PE3-PE7 PF0-PF2
            if (pin > 28) { //PF0-PF2
                gpio_pin = ((0x500) | BIT(pin % 29));
            } else {        //PE3-PE7
                gpio_pin = ((0x400) | BIT((pin % 24)+3));
            }
        } else {
            if (pin % 8 > 4) {
                gpio_pin = ((8 - pin / 8) << 8) | BIT((pin % 8) - 5);
            } else {
                gpio_pin = ((7 - pin / 8) << 8) | BIT((pin % 8) + 3);
            }
        }
    }else{
        if (pin > 23) {     //PA0-PA4 PB0-PB2
            if (pin > 28) { //PB0-PB2
                gpio_pin = ((0x100) | BIT(pin % 29));
            } else {        //PA0-PA4
                gpio_pin = ((0x000) | BIT(pin % 24));
            }
        } else {
            if (pin % 8 > 4) {
                gpio_pin = ((4 - pin / 8) << 8) | BIT((pin % 8) - 5);
            } else {
                gpio_pin = ((3 - pin / 8) << 8) | BIT((pin % 8) + 3);
            }
        }
    }

    return gpio_pin;
}

/**
 * @brief      This function serves to set the debounce period and how long to enter the idle state.
 * @param[in]  debounce_period          - Set the time of a debounce cycle.
 * @param[in]  enter_idle_period_num    - Set how many debounce cycles to enter the idle state without detecting a button.
 *                                        The value setting range is 1-32.
 *                                        The idle state will stop scanning and reduce power consumption.
 *                                        Press again to exit the idle state.
 * @return     none
 */
void keyscan_set_time(ks_debounce_period_e debounce_period, unsigned char enter_idle_period_num)
{
    reg_ks_frm_num = ((debounce_period << 5) & FLD_KS_DEBOUNCE_PERIOD) | ((enter_idle_period_num - 1) & FLD_KS_FRM_NUM);
}

/**
 * @brief      This function serves to set keyscan rows.
 * @param[in]  ks_row   - The array element must be a member of the enumeration type ks_value_e.
 * @param[in]  row_cnt  - Count of rows. Range is 1-8.
 * @return     none
 */
void keyscan_set_row(unsigned char *ks_row, unsigned char row_cnt, ks_col_pull_type_e ks_col_pull)
{
    unsigned char i;
    unsigned int  martix_row_sel   = 0;
    unsigned char martix_row_7_sel = 0;
    if ((KS_EXT_PIN_PULLUP == ks_col_pull) || (KS_INT_PIN_PULLUP == ks_col_pull)) {
       BM_SET(reg_ks_en, FLD_KS_OUT_INV );
    } else if ((KS_EXT_PIN_PULLDOWN == ks_col_pull) || (KS_INT_PIN_PULLDOWN == ks_col_pull)) {
       BM_CLR(reg_ks_en, FLD_KS_OUT_INV );
    }
    ks_col_pull = ks_col_pull & 0x0f;
    for (i = 0; i < row_cnt; i++) {
        gpio_function_dis(keyscan_pin_map(ks_row[i]));
        //gpio_set_up_down_res(keyscan_pin_map(ks_row[i]),(gpio_pull_type_e)ks_col_pull);
        gpio_set_mux_function(keyscan_pin_map(ks_row[i]), KEYS_IO);

        if (i < 6) {
            martix_row_sel |= ((ks_row[i]&(~BIT(5))) << (5 * i));
        }
        if (i == 6) {
            martix_row_sel |= ((ks_row[i]&(~BIT(5))) & 0x3) << 30;
            martix_row_7_sel |= (ks_row[i]&(~BIT(5))) >> 2;
        }
        if (i == 7) {
            martix_row_7_sel |= (ks_row[i]&(~BIT(5))) << 3;
        }
    }

    for (i = row_cnt; i < 8; i++) {
        if (i < 6) {
            martix_row_sel |= (0x1f << (5 * i));
        }
        if (i == 6) {
            martix_row_sel |= (0x1f & 0x3) << 30;
            martix_row_7_sel |= 0x1f >> 2;
        }
        if (i == 7) {
            martix_row_7_sel |= 0x1f << 3;
        }
    }
    reg_ks_row_sel0 = martix_row_sel;
    reg_ks_row_sel4 = martix_row_7_sel;
}

/**
 * @brief      This function serves to set keyscan columns.
 * @param[in]  ks_col       - The array element must be a member of the enumeration type ks_value_e.
 * @param[in]  col_cnt      - Count of columns. Range is 1-31.
 * @param[in]  ks_col_pull  - Configure the type of pull-up/pull-down resistors for column GPIO.
 * @return     none
 */
void keyscan_set_col(unsigned char *ks_col, unsigned char col_cnt, ks_col_pull_type_e ks_col_pull)
{
    unsigned char i              = 0;
    unsigned int  martix_col_sel = 0;

    if ((KS_EXT_PIN_PULLUP == ks_col_pull) || (KS_INT_PIN_PULLUP == ks_col_pull)) {
        BM_SET(reg_ks_en, FLD_KS_IN_INV );
    } else if ((KS_EXT_PIN_PULLDOWN == ks_col_pull) || (KS_INT_PIN_PULLDOWN == ks_col_pull)) {
        BM_CLR(reg_ks_en, FLD_KS_IN_INV );
    }

    ks_col_pull = ks_col_pull & 0x0f;

    for (i = 0; i < col_cnt; i++) {
        gpio_function_dis(keyscan_pin_map(ks_col[i]));
        gpio_input_en(keyscan_pin_map(ks_col[i]));
        gpio_output_dis(keyscan_pin_map(ks_col[i]));
        gpio_set_up_down_res(keyscan_pin_map(ks_col[i]), (gpio_pull_type_e)ks_col_pull);
        gpio_set_mux_function(keyscan_pin_map(ks_col[i]), KEYS_IO);

        martix_col_sel |= (unsigned int)(1 << (ks_col[i]&(~BIT(5))));
    }
    reg_ks_col_mask0 = martix_col_sel;

    g_col_mask = martix_col_sel;
}

/**
 * @brief      This function serves to set the rows and columns of the keyscan .
 * @param[in]  ks_row       - The array element must be a member of the enumeration type ks_value_e.
 * @param[in]  row_cnt      - Count of rows. Range is 1-8.
 * @param[in]  ks_col       - The array element must be a member of the enumeration type ks_value_e.
 * @param[in]  col_cnt      - Count of columns. Range is 1-31.
 * @param[in]  ks_col_pull  - Configure the type of pull-up/pull-down resistors for column GPIO.
 * @return     none
 */
void keyscan_set_martix(unsigned char *ks_row, unsigned char row_cnt, unsigned char *ks_col, unsigned char col_cnt, ks_col_pull_type_e ks_col_pull)
{

    unsigned char i;
    /* g_keyscan_col_cnt saves the total number of columns during user configuration,
     * and g_keyscan_col saves the number of columns during user configuration.*/
    g_keyscan_col_cnt = col_cnt;
    for (i = 0; i < col_cnt; i++) {
        g_keyscan_col[i] = (ks_col[i]&(~BIT(5)));
    }
    keyscan_set_row((unsigned char *)ks_row, (unsigned char)row_cnt,ks_col_pull);
    keyscan_set_col((unsigned char *)ks_col, (unsigned char)col_cnt, ks_col_pull);
}

/**
 * @brief       This function serves to get the key value.
 *              The key value "0xfb"(KESYCAN_END_FLAG) is the end sign, which indicates the end of a debounce period. It is written into fifo by the hardware after uploading the last valid key value.
 *              Each time the software reads the key value, the read pointer will increase by 1.
 *              Each time the hardware writes the key value, the write pointer will increase by 1.
 * @return      key value - bit[7:5] represents the row number, the range is from 0 to 7.
 *                          bit[4:0] represents the column number, the range is from 0 to 31.
 */
unsigned char keyscan_get_ks_value(void)
{
    unsigned char ks_key;
    ks_key                = reg_ks_key;
    unsigned char pos_row = ks_key & 0xe0;
    unsigned char pos_col = ks_key & 0x1f;
    /* After the software reads the key value in the FIFO,
     * it converts the column number of the key value to the column number when configured by the user according to g_keyscan_col.*/
    for (unsigned char i = 0; i < g_keyscan_col_cnt; i++) {
        if (pos_col == g_keyscan_col[i]) {
            pos_col = i;
            break;
        }
    }
    return (pos_row | pos_col);
}

/**
 * @brief         This function serves to set keyscan scan times.
 * @param[in]     scan_times - 1 is to set triple scan times,0 is to set double scan times.
 * @return         none
 */
void keyscan_set_scan_times(ks_scan_times_e scan_times)
{
    if (scan_times) {
        BM_CLR(reg_ks_en, FLD_KS_TRI_CHK_DIS);
    } else {
        BM_SET(reg_ks_en, FLD_KS_TRI_CHK_DIS);
    }
}

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
void keyscan_init(ks_debounce_period_e debounce_period, unsigned char enter_idle_period_num, ks_scan_times_e scan_times)
{
    /* There are two 32K clocks working in Keyscan, and the FLD_CLK1_KS_32K_EN bit can be turned on at the same time.
     * One is the scan clock, the other is the clear interrupt clock, and their clock sources are the same.
     * When Keyscan enters the idle state, the scan clock will be gated, reducing power consumption.*/
    reg_rst5 |= FLD_RST5_KEY_SCAN;
    reg_clk_en5 |= FLD_RST5_KEY_SCAN;

    reg_ks_end_flag = KESYCAN_END_FLAG;

    keyscan_set_time(debounce_period, enter_idle_period_num);
    keyscan_set_scan_times(scan_times);

    BM_CLR(reg_ks_a_en0, FLD_KS_CAPTURE_IO); //When idle, row power on

    //enable keyscan clk
    BM_SET(reg_ks_en, FLD_KS_CLK_EN /*| FLD_KS_IE */| FLD_KS_RESET);
}

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
void keyscan_init_clk_24m(ks_baud_rate_e baud_rate, ks_clk_src_e clk_src, unsigned char enter_idle_period_num){
    unsigned int clk_div = (unsigned int)(24000000/(unsigned int)(2*2*baud_rate * 32000));
    reg_clkks_mode =  ( clk_div& 0xff) | (clk_src<<5);
    reg_rst5 |= FLD_RST5_KEY_SCAN;
    reg_clk_en5 |= FLD_CLK5_KEYSCAN_EN;

    reg_ks_end_flag = KESYCAN_END_FLAG;

    keyscan_set_time(1, enter_idle_period_num);
    keyscan_set_scan_times(1);

    BM_CLR(reg_ks_a_en0, FLD_KS_CAPTURE_IO); //When idle, row power on
    reg_ks_a_en0 |= (reg_ks_a_en0 &(~(FLD_KS_RXDMA_CACHE)));
    BM_CLR(reg_ks_en,FLD_KS_IE);
    reg_ks_a_en0 |= FLD_KS_RXDONE_IE;
    //enable keyscan clk and set irq mask.If the mask bit is not enabled, the FLD_IRQ_KS status bit will not be set.
    BM_SET(reg_ks_en, FLD_KS_CLK_EN | FLD_KS_RESET);


}

