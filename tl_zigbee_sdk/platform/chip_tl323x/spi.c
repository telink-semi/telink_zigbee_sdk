/********************************************************************************************************
 * @file    spi.c
 *
 * @brief   This is the source file for tl323x
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "spi.h"
#include "lib/include/clock.h"

static unsigned char s_gspi_tx_dma_chn;
static unsigned char s_gspi_master_rx_dma_chn;
static unsigned char s_gspi_slave_rx_dma_chn;
dma_chain_config_t   g_spi_rx_dma_list_cfg;
dma_chain_config_t   g_spi_tx_dma_list_cfg;

spi_timeout_error_t g_spi_timeout_error = {
    .g_spi_error_timeout_us   = 0xffffffff,
    .spi_timeout_handler      = gspi_timeout_handler,
    .g_spi_error_timeout_code = SPI_API_ERROR_TIMEOUT_NONE,
};
dma_config_t gspi_tx_dma_config = {
    .dst_req_sel    = DMA_REQ_GSPI_TX,    //tx req.
    .src_req_sel    = 0,
    .dst_addr_ctrl  = DMA_ADDR_FIX,
    .src_addr_ctrl  = DMA_ADDR_INCREMENT, //increment.
    .dstmode        = DMA_HANDSHAKE_MODE, //handshake.
    .srcmode        = DMA_NORMAL_MODE,
    .dstwidth       = DMA_CTR_WORD_WIDTH, //must word.
    .srcwidth       = DMA_CTR_WORD_WIDTH, //must word.
    .src_burst_size = 0,                  //master tx dma support burst1(1 word).
    .read_num_en    = 0,
    .priority       = 0,
    .write_num_en   = 0,
    .auto_en        = 0, //must 0.
};
dma_config_t gspi_master_rx_dma_config = {
    .dst_req_sel    = 0,
    .src_req_sel    = DMA_REQ_GSPI_RX, //rx req.
    .dst_addr_ctrl  = DMA_ADDR_INCREMENT,
    .src_addr_ctrl  = DMA_ADDR_FIX,
    .dstmode        = DMA_NORMAL_MODE,
    .srcmode        = DMA_HANDSHAKE_MODE,
    .dstwidth       = DMA_CTR_WORD_WIDTH, //must word.
    .srcwidth       = DMA_CTR_WORD_WIDTH, //must word.
    .src_burst_size = 0,                  //master rx dma support burst1(1 word).
    .read_num_en    = 0,
    .priority       = 0,
    .write_num_en   = 0,
    .auto_en        = 0, //must 0.
};
dma_config_t gspi_slave_rx_dma_config = {
    .dst_req_sel    = 0,
    .src_req_sel    = DMA_REQ_GSPI_RX, //rx req.
    .dst_addr_ctrl  = DMA_ADDR_INCREMENT,
    .src_addr_ctrl  = DMA_ADDR_FIX,
    .dstmode        = DMA_NORMAL_MODE,
    .srcmode        = DMA_HANDSHAKE_MODE,
    .dstwidth       = DMA_CTR_WORD_WIDTH, //must word.
    .srcwidth       = DMA_CTR_WORD_WIDTH, //must word.
    .src_burst_size = 0,                  //slave not support burst.
    .read_num_en    = 0,
    .priority       = 0,
    .write_num_en   = 1,                  //When write_num_en is set to 1, dma will write back the length information at the address 4 bytes before the destination address.
    .auto_en        = 0,                  //must 0.
};

/**
 * @brief     This function serves to record the api status.
 * @param[in] spi_error_timeout_code - spi_api_error_timeout_code_e.
 * @return    none.
 * @note      This function can be rewritten according to the application scenario.
 */
__attribute__((weak)) void gspi_timeout_handler(unsigned int spi_error_timeout_code)
{
    g_spi_timeout_error.g_spi_error_timeout_code = spi_error_timeout_code;
}

/**
  * @brief     This function serves to set the spi timeout(us).
  * @param[in] timeout_us - the timeout(us).
  * @return    none.
  * @note      The default timeout (g_spi_error_timeout_us) is the larger value.If the timeout exceeds the feed dog time and triggers a watchdog restart,
  *            g_spi_error_timeout_us can be changed to a smaller value via this interface, depending on the application.
  *            The minimum time for g_spi_error_timeout_us is related to the following factors:
  *            1.spi clock rate;
  *            2.cclk clock rate;
  *            3.spi's data line(SPI_SINGLE_MODE, SPI_DUAL_MODE, SPI_QUAD_MODE);
  *            4.maximum interrupt processing time;
  *            5.spi write/read data length (especially SPI_WAIT_BUSY);
  *            Solution in case of timeout exit:
  *            when timeout exits, solution:
  *            reset SPI(as master or slave) module,corresponding api:spi_hw_fsm_reset;
  */
void spi_set_error_timeout(unsigned int timeout_us)
{
    g_spi_timeout_error.g_spi_error_timeout_us = timeout_us;
}

/**
   * @brief     This function serves to return the spi api error timeout code.
   * @return    none.
   */
spi_api_error_timeout_code_e spi_get_error_timeout_code(void)
{
    return g_spi_timeout_error.g_spi_error_timeout_code;
}

/**
 * @brief       This function servers to determine whether the txfifo is full.
 * @param[in]   spi_sel     - the spi module.
 * @return      1:Indicates that the txfifo is full. 0:Indicates that the txfifo is not full.
 */
bool spi_txfifo_is_full(spi_sel_e spi_sel)
{
    return reg_spi_txfifo_status(spi_sel) & FLD_SPI_TXF_FULL;
}

/**
 * @brief       This function servers to determine whether the rxfifo is empty.
 * @param[in]   spi_sel     - the spi module.
 * @return      1:Indicates that the rxfifo is empty. 0:Indicates that the rxfifo is not empty.
 */
bool spi_rxfifo_is_empty(spi_sel_e spi_sel)
{
    return reg_spi_rxfifo_status(spi_sel) & FLD_SPI_RXF_EMPTY;
}

/**
 * @brief       This function determines whether tx_fifo can continue to write a word.
 * @param[in]   spi_sel     - the spi module.
 * @return      1: cannot continue to write. 0: can continue to write.
 */
bool spi_txfifo_num_is_word(spi_sel_e spi_sel)
{
    unsigned char tx_fifo_depth = 20;

    return (tx_fifo_depth - (reg_spi_txfifo_status(spi_sel) & FLD_SPI_TXF_ENTRIES) < 4);
}

/**
 * @brief       This function determines whether rx_fifo can continue to read a word.
 * @param[in]   spi_sel     - the spi module.
 * @return      1:cannot continue to read. 0:can continue to read.
 */
bool spi_rxfifo_num_is_word(spi_sel_e spi_sel)
{
    return ((reg_spi_rxfifo_status(spi_sel) & FLD_SPI_RXF_ENTRIES) < 4);
}

/**
 * @brief      This function selects  pin  for gspi master or slave mode.
 * @param[in]  pin       - the selected pin.
 * @param[in]  function  - pin-multiplexing function.
 * @return     none.
 */
void gspi_set_pin_mux(gpio_func_pin_e pin, gpio_func_e function)
{
    if (pin != GPIO_NONE_PIN) {
        /**
         * When configuring the mux pin of SPI, pull up the CSN configuration.
         * The chip SPI defaults to the master, and when the slave configures the pin as CSN, since the CSN is floating and in the input state,
         * an end interrupt may be generated by external influences.
         * Added by pengxiang
         */
        if (GSPI_CN0_IO == function) {
            gpio_set_up_down_res((gpio_pin_e)pin, GPIO_PIN_PULLUP_10K);
        }
        /**
         * Move the operation of gpio_input_en() to the first.
         * Added by pengxiang
         */
        gpio_input_en((gpio_pin_e)pin);
        reg_gpio_func_mux(pin) = function;
        gpio_function_dis((gpio_pin_e)pin);
    }
}

/**
 * @brief       This function enable gspi csn pin.
 * @param[in]   pin - the csn pin.
 * @return      none.
 */
void gspi_cs_pin_en(gpio_func_pin_e pin)
{
    gspi_set_pin_mux((gpio_func_pin_e)pin, GSPI_CN0_IO);
}

/**
 * @brief       This function disable gspi csn pin.
 * @param[in]   pin - the csn pin.
 * @return      none.
 */
void gspi_cs_pin_dis(gpio_func_pin_e pin)
{
    gpio_output_en((gpio_pin_e)pin);
    gpio_input_dis((gpio_pin_e)pin);
    gpio_set_high_level((gpio_pin_e)pin);
    gpio_function_en((gpio_pin_e)pin); //gpio_function_en must be set at the end
}

/**
 * @brief       This function change gspi csn pin.
 * @param[in]   current_csn_pin - the current csn pin.
 * @param[in]   next_csn_pin - the next csn pin.
 * @return      none.
 */
void gspi_change_csn_pin(gpio_func_pin_e current_csn_pin, gpio_func_pin_e next_csn_pin)
{
    gspi_cs_pin_dis(current_csn_pin);
    gspi_cs_pin_en(next_csn_pin);
}

/**
 * @brief       This function servers to set gspi pin.
 * @param[in]   gspi_pin_config - the pointer of pin config struct.
 * @return      none.
 */
void gspi_set_pin(gspi_pin_config_t *spi_pin_config)
{
    gspi_set_pin_mux(spi_pin_config->spi_clk_pin, GSPI_CK_IO);
    gspi_set_pin_mux(spi_pin_config->spi_csn_pin, GSPI_CN0_IO);
    gspi_set_pin_mux(spi_pin_config->spi_mosi_io0_pin, GSPI_MOSI_IO);
    gspi_set_pin_mux(spi_pin_config->spi_miso_io1_pin, GSPI_MISO_IO);
    gspi_set_pin_mux(spi_pin_config->spi_io2_pin, GSPI_IO2_IO);
    gspi_set_pin_mux(spi_pin_config->spi_io3_pin, GSPI_IO3_IO);
}

/**
 * @brief      This function selects  pin  for sspi master or slave mode.
 * @param[in]  pin       - the selected pin.
 * @param[in]  function  - pin-multiplexing function.
 * @return     none.
 */
void sspi_set_pin_mux(gpio_func_pin_e pin, gpio_func_e function)
{
    if (pin != GPIO_NONE_PIN) {
        /**
         * When configuring the mux pin of SPI, pull up the CSN configuration.
         * The chip SPI defaults to the master, and when the slave configures the pin as CSN, since the CSN is floating and in the input state,
         * an end interrupt may be generated by external influences.
         * Added by pengxiang
         */
        if (SSPI_CN_I == function) {
            gpio_set_up_down_res((gpio_pin_e)pin, GPIO_PIN_PULLUP_10K);
        }
        /**
         * Move the operation of gpio_input_en() to the first.
         * Added by pengxiang
         */
        gpio_input_en((gpio_pin_e)pin);
        reg_gpio_func_mux(pin) = function;
        gpio_function_dis((gpio_pin_e)pin);
    }
}

/**
 * @brief   This function servers to set sspi pin.
 * @param[in]   spi_pin_config - the pointer of pin config struct.
 * @return  none.
 */
void spi_slave_set_pin(sspi_pin_config_t *spi_pin_config)
{
    /*  initialize sspi*/
    reg_rst1 |= FLD_RST1_SPISLV;
    reg_clk_en1 |= FLD_RST1_SPISLV;
    if ((spi_pin_config->spi_csn_pin != GPIO_FC_PD0)) {
        if ((reg_gpio_func_mux(GPIO_FC_PD0) == SSPI_CN_I) && ((reg_gpio_func(GPIO_FC_PD0) & GPIO_PD0) == 0)) {
            gpio_function_en(GPIO_PD0);
        }
    }
    if ((spi_pin_config->spi_clk_pin != GPIO_FC_PD1)) {
        if ((reg_gpio_func_mux(GPIO_FC_PD1) == SSPI_CK_I) && ((reg_gpio_func(GPIO_FC_PD1) & GPIO_PD1) == 0)) {
            gpio_function_en(GPIO_PD1);
        }
    }
    if ((spi_pin_config->spi_mosi_io0_pin != GPIO_FC_PD2)) {
        if ((reg_gpio_func_mux(GPIO_FC_PD2) == SSPI_SI_IO) && ((reg_gpio_func(GPIO_FC_PD2) & GPIO_PD2) == 0)) {
            gpio_function_en(GPIO_PD2);
        }
    }
    if ((spi_pin_config->spi_miso_io1_pin != GPIO_FC_PD3)) {
        if ((reg_gpio_func_mux(GPIO_FC_PD3) == SSPI_SO_IO) && ((reg_gpio_func(GPIO_FC_PD3) & GPIO_PD3) == 0)) {
            gpio_function_en(GPIO_PD3);
        }
    }

    sspi_set_pin_mux(spi_pin_config->spi_clk_pin, SSPI_CK_I);
    sspi_set_pin_mux(spi_pin_config->spi_csn_pin, SSPI_CN_I);
    sspi_set_pin_mux(spi_pin_config->spi_mosi_io0_pin, SSPI_SI_IO);
    sspi_set_pin_mux(spi_pin_config->spi_miso_io1_pin, SSPI_SO_IO);
}

/**
 * @brief       This function configures the clock and working mode for SPI interface.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   div_clock   - the division factor for SPI module.
 *              spi_clock_out = pll_clk / div_clock
 * @param[in]    mode       - the selected working mode of SPI module.
 *               bit2: CPHA-SPI_CLK Phase,bit3: CPOL-SPI_CLK Polarity
 *              MODE0:  CPOL = 0 , CPHA = 0;
 *              MODE1:  CPOL = 0 , CPHA = 1;
 *              MODE2:  CPOL = 1 , CPHA = 0;
 *              MODE3:  CPOL = 1 , CPHA = 1;
 * @return      none.
 * @note        - When using the spi master module, this interface must be configured first, otherwise the following interfaces will not take effect.
 *              - If the master is not connected or if there are other slave devices that can receive a higher frequency, spi_clk on the io output from the master will be 48M max.
 *              - The master connects to the telink's slave, since the maximum clock of the slave spi module is 48M, according to the formula(spi_clock_in(output from master clk_pin )  <= slave clk/4), the maximum value of spi_clk on the io is 96/4=24M.
 *              - Due to the divider register bit width limitation,The limits between src_clk and spi_clock are as follows.
 *               src_clk                    GSPI(spi_clk  minimum)=src_clk/255
 *               SRC_CLK_XTAL_24M                          94117
 */
void spi_master_init(spi_sel_e spi_sel, unsigned short div_clock, spi_mode_type_e mode)
{
    reg_rst1 |= FLD_RST1_GSPI;
    reg_clk_en1 |= FLD_CLK1_GSPI_EN;
    if (div_clock > 255) { //GSPI clock source select pll_clk when div_clock <= 255,select xtl 24m when div_clock > 255.
        clock_bbpll_config(PLL_CLK);
        reg_gspi_clk_set = ((FLD_GSPI_CLK_MOD & (unsigned char)(div_clock * 24 / (sys_clk.pll_clk))) | (1 << 8));
    } else {
        reg_gspi_clk_set = ((FLD_GSPI_CLK_MOD & (unsigned char)div_clock) | (2 << 8));
    }
    reg_spi_ctrl3(spi_sel) |= (FLD_SPI_MASTER_MODE | FLD_SPI_AUTO_HREADY_EN);                 //master
    reg_spi_ctrl3(spi_sel) = ((reg_spi_ctrl3(spi_sel) & (~FLD_SPI_WORK_MODE)) | (mode << 2)); // select SPI mode, support four modes.
    spi_rx_irq_trig_cnt(spi_sel, 4);                                                          //default gspi burst1(N=1) ,rx_fifo trig_cnt=4*N=4.
    spi_tx_irq_trig_cnt(spi_sel, 4);                                                          //default gspi burst1(N=1) ,tx_fifo trig_cnt=tx fifo depth-4*N=8-4=4.
    spi_xip_dis(spi_sel);
}

/**
 * @brief       This function configures the clock and working mode for SPI interface.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   mode    - the selected working mode of SPI module.
 *               bit2: CPHA-SPI_CLK Phase,bit3: CPOL-SPI_CLK Polarity
 *              MODE0:  CPOL = 0 , CPHA = 0;
 *              MODE1:  CPOL = 0 , CPHA = 1;
 *              MODE2:  CPOL = 1 , CPHA = 0;
 *              MODE3:  CPOL = 1 , CPHA = 1;
 * @return      none.
 * @note       - When using the spi slave module, this interface must be configured first, otherwise the following interfaces will not take effect.
 *             - spi_clock_in(output from master clk_pin )  <= slave clk/4.
 *             - The master connects to the telink's slave, since the maximum clock of the slave spi module is 96M, according to the formula, the maximum value of spi_clk on the io is 24M.
 *                The limits between src_clk and spi_clock  are as follows.
 *              src_clk (sys_clk.pll_clk)                GPSI(spi_clk maximum )=src_clk/4
 *               SRC_CLK_PLL_144M                             (144M/2)/4=18000000
 *               SRC_CLK_PLL_156M                             (156M/2)/4=19500000
 *               SRC_CLK_PLL_180M                             (180M/2)/4=22500000
 *               SRC_CLK_PLL_192M                             (192M/2)/4=24000000
 */
void spi_slave_init(spi_sel_e spi_sel, spi_mode_type_e mode)
{
    reg_rst1 |= FLD_RST1_GSPI;
    reg_clk_en1 |= FLD_CLK1_GSPI_EN;
    reg_gspi_clk_set = ((FLD_GSPI_CLK_MOD & (unsigned char)(sys_clk.pll_clk / 48)) | (2 << 8));
    reg_spi_ctrl3(spi_sel) &= (~FLD_SPI_MASTER_MODE);                                         //slave.
    reg_spi_ctrl3(spi_sel) |= FLD_SPI_AUTO_HREADY_EN;                                         //slave.
    reg_spi_ctrl3(spi_sel) = ((reg_spi_ctrl3(spi_sel) & (~FLD_SPI_WORK_MODE)) | (mode << 2)); // select SPI mode, support four modes.

    spi_rx_irq_trig_cnt(spi_sel, 4);                                                          //default gspi burst1(N=1) ,rx_fifo trig_cnt=4*N=4.
    spi_tx_irq_trig_cnt(spi_sel, 4);                                                          //default gspi burst1(N=1) ,tx_fifo trig_cnt=rx fifo depth-4*N=8-4=4.
    spi_xip_dis(spi_sel);
}

/**
 * @brief       This function servers to set dummy cycle cnt.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   dummy_cnt   - the cnt of dummy clock.
 * @return      none.
 */
void spi_set_dummy_cnt(spi_sel_e spi_sel, unsigned char dummy_cnt)
{
    reg_spi_ctrl2(spi_sel) = ((reg_spi_ctrl2(spi_sel) & (~FLD_SPI_DUMMY_CNT)) | ((dummy_cnt - 1) & FLD_SPI_DUMMY_CNT));
    reg_spi_ctrl4(spi_sel) = ((reg_spi_ctrl4(spi_sel) & (~FLD_SPI_DUMMY_CNT_ADD)) | ((dummy_cnt - 1) & FLD_SPI_DUMMY_CNT_ADD));
}

/**
 * @brief       This function servers to set spi transfer mode.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   mode    - transfer mode.
 * @return      none.
 */
void spi_set_transmode(spi_sel_e spi_sel, spi_tans_mode_e mode)
{
    reg_spi_ctrl2(spi_sel) = ((reg_spi_ctrl2(spi_sel) & (~FLD_SPI_TRANSMODE)) | ((mode & 0xf) << 4));
    reg_spi_slv_trans_mode(spi_sel) = ((reg_spi_slv_trans_mode(spi_sel) & (~FLD_SPI_SLV_TRANS_MODE)) | (mode & 0x0f));
}

/**
 * @brief       This function servers to set spi io  mode.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   mode    - single/dual/quad /3line.
 * @return      none.
  */
void spi_set_io_mode(spi_sel_e spi_sel, spi_io_mode_e mode)
{
    if (mode == SPI_3_LINE_MODE) {
        /*The io mode must set to single mode*/
        spi_3line_mode_en(spi_sel);
        reg_spi_ctrl1(spi_sel) = ((reg_spi_ctrl1(spi_sel) & (~FLD_SPI_DATA_LANE)));
    } else {
        /*must disable 3line mode*/
        reg_spi_ctrl1(spi_sel) = ((reg_spi_ctrl1(spi_sel) & (~FLD_SPI_DATA_LANE)) | (mode & 0x3));
        spi_3line_mode_dis(spi_sel);
    }
}

/**
 * @brief       This function servers to config normal mode.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   mode    - nomal ,mode 3line.
 * @return      none.
 */
void spi_master_config(spi_sel_e spi_sel, spi_normal_3line_mode_e mode)
{
    spi_cmd_dis(spi_sel);
    spi_addr_dis(spi_sel);
    spi_set_io_mode(spi_sel, (spi_io_mode_e)mode);
}

/**
 * @brief       This function servers to config spi special mode.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   config  - the pointer of pin special config struct.
 * @return      none.
 */
void spi_master_config_plus(spi_sel_e spi_sel, spi_wr_rd_config_t *config)
{
    spi_set_io_mode(spi_sel, config->spi_io_mode);
    spi_set_addr_len(spi_sel, config->spi_addr_len);
    spi_set_dummy_cnt(spi_sel, config->spi_dummy_cnt);

    if (1 == config->spi_cmd_en) {
        spi_cmd_en(spi_sel);
    } else if (0 == config->spi_cmd_en) {
        spi_cmd_dis(spi_sel);
    }

    if (1 == config->spi_cmd_fmt_en) {
        spi_cmd_fmt_en(spi_sel);
    } else if (0 == config->spi_cmd_fmt_en) {
        spi_cmd_fmt_dis(spi_sel);
    }

    if (1 == config->spi_addr_en) {
        spi_addr_en(spi_sel);
    } else if (0 == config->spi_addr_en) {
        spi_addr_dis(spi_sel);
    }

    if (1 == config->spi_addr_fmt_en) {
        spi_addr_fmt_en(spi_sel);
    } else if (0 == config->spi_addr_fmt_en) {
        spi_addr_fmt_dis(spi_sel);
    }
    if (1 == config->spi_token_en) {
        spi_token_en(spi_sel);
    } else if (0 == config->spi_token_en) {
        spi_token_dis(spi_sel);
    }
}

/**
 * @brief       This function servers to send command by spi.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   cmd - command.
 * @return      DRV_API_SUCCESS:command sent successfully. others:fail to send command.
 *              DRV_API_TIMEOUT:timeout exit(solution refer to the note for spi_set_error_timeout).
 */
drv_api_status_e spi_master_send_cmd(spi_sel_e spi_sel, unsigned char cmd)
{
    spi_tx_fifo_clr(spi_sel);
    spi_set_transmode(spi_sel, SPI_MODE_NONE_DATA); //nodata.
    spi_set_cmd(spi_sel, cmd);
    if (SPI_WAIT(spi_is_busy, spi_sel, g_spi_timeout_error.g_spi_error_timeout_us, g_spi_timeout_error.spi_timeout_handler, SPI_API_ERROR_TIMEOUT_BUS_BUSY)) {
        return DRV_API_TIMEOUT;
    }
    return DRV_API_SUCCESS;
}

/**
 * @brief       This function servers to write spi fifo.
 * tx_fifo_depth are fixed sizes.
 * gspi with txfifo deepth = 8 bytes.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   data    - the pointer to the data for write.
 * @param[in]   len     - write length.
 * @return      DRV_API_SUCCESS:data write to txfifo successfully. others:data write to txfifo failed.
 *              DRV_API_TIMEOUT:timeout exit(solution refer to the note for spi_set_error_timeout).
 *              Master timeout will only be affected by their own factors, slave timeout will not only be affected by their own factors,
                will also be affected by the master side of the influence.
 */
drv_api_status_e spi_write(spi_sel_e spi_sel, unsigned char *data, unsigned int len)
{
    unsigned int  word_len   = len >> 2;
    unsigned char single_len = len & 3;

    //The tx _fifo depth of the gspi is 8 bytes. When the remaining size in the tx_fifo is not less than 4 bytes, the MCU will move the data according to the word length.
    for (unsigned int i = 0; i < word_len; i++) {
        if (SPI_WAIT(spi_txfifo_num_is_word, spi_sel, g_spi_timeout_error.g_spi_error_timeout_us, g_spi_timeout_error.spi_timeout_handler, SPI_API_ERROR_TIMEOUT_TXFIFO_NUM_WORD)) {
            return DRV_API_TIMEOUT;
        }
        reg_spi_wr_rd_data_word(spi_sel) = ((unsigned int *)data)[i];
    }
    //When the remaining size in tx_fifo is less than 4 bytes, the MCU moves the data according to the word length.
    for (unsigned int i = 0; i < single_len; i++) {
        if (SPI_WAIT(spi_txfifo_is_full, spi_sel, g_spi_timeout_error.g_spi_error_timeout_us, g_spi_timeout_error.spi_timeout_handler, SPI_API_ERROR_TIMEOUT_TXFIFO_FULL)) {
            return DRV_API_TIMEOUT;
        }
        reg_spi_wr_rd_data(spi_sel, i % 4) = data[(word_len * 4) + i];
    }
    return DRV_API_SUCCESS;
}

/**
 * @brief       This function servers to read spi fifo.
 * rx_fifo_depth are fixed sizes.
 * gspi with rxfifo deepth = 8 bytes.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   data    - the pointer to the data for read.
 * @param[in]   len     - write length.
 * @return      DRV_API_SUCCESS:data fetched from rxfifo successfully. others:data fetched from rxfifo failed.
 *              DRV_API_TIMEOUT:timeout exit(solution refer to the note for spi_set_error_timeout).
 *              Master timeout will only be affected by their own factors, slave timeout will not only be affected by their own factors,
                will also be affected by the master side of the influence.
 */
drv_api_status_e spi_read(spi_sel_e spi_sel, unsigned char *data, unsigned int len)
{
    unsigned int  word_len   = len >> 2;
    unsigned char single_len = len & 3;
    //When the data size in rx_fifo is not less than 4 bytes, the MCU moves the data according to the word length.
    for (unsigned int i = 0; i < word_len; i++) {
        if (SPI_WAIT(spi_rxfifo_num_is_word, spi_sel, g_spi_timeout_error.g_spi_error_timeout_us, g_spi_timeout_error.spi_timeout_handler, SPI_API_ERROR_TIMEOUT_RXFIFO_NUM_WORD)) {
            return DRV_API_TIMEOUT;
        }
        ((unsigned int *)data)[i] = reg_spi_wr_rd_data_word(spi_sel);
    }
    //When the data size in rx_fifo is less than 4 bytes, the MCU moves the data according to the word length.
    for (unsigned char i = 0; i < single_len; i++) {
        if (SPI_WAIT(spi_rxfifo_is_empty, spi_sel, g_spi_timeout_error.g_spi_error_timeout_us, g_spi_timeout_error.spi_timeout_handler, SPI_API_ERROR_TIMEOUT_RXFIFO_EMPTY)) {
            return DRV_API_TIMEOUT;
        }
        data[(word_len * 4) + i] = reg_spi_wr_rd_data((spi_sel), i % 4);
    }
    return DRV_API_SUCCESS;
}

/**
 * @brief       This function serves to normal write data in normal.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   data    - the pointer to the data for write.
 * @param[in]   len     - write length.
 * @return      DRV_API_SUCCESS:master write data successfully. others:master write data failed.
 *              DRV_API_TIMEOUT:timeout exit(solution refer to the note for spi_set_error_timeout).
 */
drv_api_status_e spi_master_write(spi_sel_e spi_sel, unsigned char *data, unsigned int len)
{
    spi_tx_dma_dis(spi_sel);
    spi_tx_fifo_clr(spi_sel);
    spi_tx_cnt(spi_sel, len);
    spi_set_transmode(spi_sel, SPI_MODE_WRITE_ONLY);
    spi_set_cmd(spi_sel, 0x00); //when  cmd  disable that  will not sent cmd,just trigger spi send.
    spi_write(spi_sel, (unsigned char *)data, len);
    if (SPI_WAIT(spi_is_busy, spi_sel, g_spi_timeout_error.g_spi_error_timeout_us, g_spi_timeout_error.spi_timeout_handler, SPI_API_ERROR_TIMEOUT_BUS_BUSY)) {
        return DRV_API_TIMEOUT;
    }
    return DRV_API_SUCCESS;
}

/**
 * @brief       This function serves to normal write and read data.
 * This interface cannot be used for full duplex.
 * rd_len shouldn't set to 0. Must write first, then read.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   wr_data - the pointer to the data for write.
 * @param[in]   wr_len  - write length.
 * @param[in]   rd_data - the pointer to the data for read.
 * @param[in]   rd_len  - read length.
 * @return      DRV_API_SUCCESS:master write read data successfully. others:master write read data failed.
 *              DRV_API_TIMEOUT:timeout exit(solution refer to the note for spi_set_error_timeout).
 */
drv_api_status_e spi_master_write_read(spi_sel_e spi_sel, unsigned char *wr_data, unsigned int wr_len, unsigned char *rd_data, unsigned int rd_len)
{
    spi_tx_dma_dis(spi_sel);
    spi_rx_dma_dis(spi_sel);
    spi_tx_fifo_clr(spi_sel);
    spi_rx_fifo_clr(spi_sel);
    spi_tx_cnt(spi_sel, wr_len);
    spi_rx_cnt(spi_sel, rd_len);
    spi_set_transmode(spi_sel, SPI_MODE_WRITE_READ);
    spi_set_cmd(spi_sel, 0x00); //when  cmd  disable that  will not sent cmd,just trigger spi send.
    spi_write(spi_sel, (unsigned char *)wr_data, wr_len);
    spi_read(spi_sel, (unsigned char *)rd_data, rd_len);
    if (SPI_WAIT(spi_is_busy, spi_sel, g_spi_timeout_error.g_spi_error_timeout_us, g_spi_timeout_error.spi_timeout_handler, SPI_API_ERROR_TIMEOUT_BUS_BUSY)) {
        return DRV_API_TIMEOUT;
    }
    return DRV_API_SUCCESS;
}

/**
 * @brief       This function serves to single/dual/quad write to the SPI slave.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   cmd         - cmd one byte will first write.
 * @param[in]   addr        - the address of slave.
 * @param[in]   data        -  pointer to the data need to write.
 * @param[in]   data_len    - length in byte of the data need to write.
 * @param[in]   wr_mode     - write mode.dummy or not dummy.
 * @return      DRV_API_SUCCESS:master write data successfully. others:master write data failed.
 *              DRV_API_TIMEOUT:timeout exit(solution refer to the note for spi_set_error_timeout).
 */
drv_api_status_e spi_master_write_plus(spi_sel_e spi_sel, unsigned char cmd, unsigned int addr, unsigned char *data, unsigned int data_len, spi_wr_tans_mode_e wr_mode)
{
    spi_tx_dma_dis(spi_sel);
    spi_tx_fifo_clr(spi_sel);
    spi_set_address(spi_sel, addr);

    if (data_len == 0) {
        spi_set_transmode(spi_sel, SPI_MODE_NONE_DATA);
    } else {
        spi_set_transmode(spi_sel, (spi_tans_mode_e)wr_mode);
        spi_tx_cnt(spi_sel, data_len);
    }
    spi_set_cmd(spi_sel, cmd);
    if (data_len != 0) {
        spi_write(spi_sel, (unsigned char *)data, data_len);
    }
    if (SPI_WAIT(spi_is_busy, spi_sel, g_spi_timeout_error.g_spi_error_timeout_us, g_spi_timeout_error.spi_timeout_handler, SPI_API_ERROR_TIMEOUT_BUS_BUSY)) {
        return DRV_API_TIMEOUT;
    }
    return DRV_API_SUCCESS;
}

/**
 * @brief       This function serves to normal write data repeatedly.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   data        - the pointer to the data for write.
 * @param[in]   len         - write length.
 * @param[in]   repeat_time - number of times to write data repeatedly.
 * @return     DRV_API_SUCCESS:master write data successfully. others:master write data failed.
 *             DRV_API_TIMEOUT:timeout exit(solution refer to the note for spi_set_error_timeout).
 */
drv_api_status_e spi_master_write_repeat(spi_sel_e spi_sel, unsigned char *data, unsigned int len, unsigned int repeat_time)
{
    unsigned int i = 0, j = 0, k = 0;
    spi_tx_dma_dis(spi_sel);
    spi_tx_fifo_clr(spi_sel);
    spi_tx_cnt(spi_sel, len * repeat_time);
    spi_set_transmode(spi_sel, SPI_MODE_WRITE_ONLY);
    spi_set_cmd(spi_sel, 0x00); //when  cmd  disable that  will not sent cmd,just trigger spi send.
    for (i = 0; i < repeat_time; i++) {
        for (j = 0; j < len; j++, k++) {
            if (SPI_WAIT(spi_txfifo_is_full, spi_sel, g_spi_timeout_error.g_spi_error_timeout_us, g_spi_timeout_error.spi_timeout_handler, SPI_API_ERROR_TIMEOUT_TXFIFO_FULL)) {
                return DRV_API_TIMEOUT;
            }
            reg_spi_wr_rd_data(spi_sel, k % 4) = data[j];
        }
    }
    if (SPI_WAIT(spi_is_busy, spi_sel, g_spi_timeout_error.g_spi_error_timeout_us, g_spi_timeout_error.spi_timeout_handler, SPI_API_ERROR_TIMEOUT_BUS_BUSY)) {
        return DRV_API_TIMEOUT;
    }
    return DRV_API_SUCCESS;
}

/**
 * @brief       This function serves to single/dual/quad write data to the SPI slave repeatedly.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   cmd         - cmd one byte will first write.
 * @param[in]   addr        - the address of slave.
 * @param[in]   data        - pointer to the data need to write.
 * @param[in]   data_len    - length in byte of the data need to write.
 * @param[in]   wr_mode     - write mode.dummy or not dummy.
 * @param[in]   repeat_time - number of times to write data repeatedly.
 * @return      DRV_API_SUCCESS:master write data successfully. others:master write data failed.
 *              DRV_API_TIMEOUT:timeout exit(solution refer to the note for spi_set_error_timeout).
 * @attention   Only data would be written repeatedly. the typical sending order is cmd + address + data * repeat_time,
 *              cmd and address would not be repeated.
 */
drv_api_status_e spi_master_write_repeat_plus(spi_sel_e spi_sel, unsigned char cmd, unsigned int addr, unsigned char *data, unsigned int data_len, unsigned int repeat_time, spi_wr_tans_mode_e wr_mode)
{
    unsigned int i = 0, j = 0, k = 0;
    spi_tx_dma_dis(spi_sel);
    spi_tx_fifo_clr(spi_sel);
    spi_set_address(spi_sel, addr);
    spi_tx_cnt(spi_sel, data_len * repeat_time);
    spi_set_transmode(spi_sel, (spi_tans_mode_e)wr_mode);
    spi_set_cmd(spi_sel, cmd);
    for (i = 0; i < repeat_time; i++) {
        for (j = 0; j < data_len; j++, k++) {
            if (SPI_WAIT(spi_txfifo_is_full, spi_sel, g_spi_timeout_error.g_spi_error_timeout_us, g_spi_timeout_error.spi_timeout_handler, SPI_API_ERROR_TIMEOUT_TXFIFO_FULL)) {
                return DRV_API_TIMEOUT;
            }
            reg_spi_wr_rd_data(spi_sel, k % 4) = data[j];
        }
    }
    if (SPI_WAIT(spi_is_busy, spi_sel, g_spi_timeout_error.g_spi_error_timeout_us, g_spi_timeout_error.spi_timeout_handler, SPI_API_ERROR_TIMEOUT_BUS_BUSY)) {
        return DRV_API_TIMEOUT;
    }
    return DRV_API_SUCCESS;
}

/**
 * @brief       This function serves to single/dual/quad  read from the SPI slave.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   cmd         - cmd one byte will first write.
 * @param[in]   addr        - the address of slave.
 * @param[in]   data        - pointer to the data need to read.
 * @param[in]   data_len    - the length of data.
 * @param[in]   rd_mode     - read mode.dummy or not dummy.
 * @return      DRV_API_SUCCESS:master read data successfully. others:master read data failed.
 *              DRV_API_TIMEOUT:timeout exit(solution refer to the note for spi_set_error_timeout).
 */
drv_api_status_e spi_master_read_plus(spi_sel_e spi_sel, unsigned char cmd, unsigned int addr, unsigned char *data, unsigned int data_len, spi_rd_tans_mode_e rd_mode)
{
    spi_rx_dma_dis(spi_sel);
    spi_rx_fifo_clr(spi_sel);
    spi_set_address(spi_sel, addr);
    spi_set_transmode(spi_sel, (spi_tans_mode_e)rd_mode);
    spi_rx_cnt(spi_sel, data_len);
    spi_set_cmd(spi_sel, cmd);
    spi_read(spi_sel, (unsigned char *)data, data_len);
    if (SPI_WAIT(spi_is_busy, spi_sel, g_spi_timeout_error.g_spi_error_timeout_us, g_spi_timeout_error.spi_timeout_handler, SPI_API_ERROR_TIMEOUT_BUS_BUSY)) {
        return DRV_API_TIMEOUT;
    }
    return DRV_API_SUCCESS;
}

/**
 * @brief       This function serves to write address, then read data from the SPI slave.
 * This interface cannot be used for full duplex.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   cmd         - cmd one byte will first write.
 * @param[in]   addrs       - pointer to the address of slave.
 * @param[in]   addr_len    - the length of address.
 * @param[in]   data        - the pointer to the data for read.
 * @param[in]   data_len    - read length.
 * @param[in]   wr_mode     - write mode.dummy or not dummy.
 * @return      DRV_API_SUCCESS:master write read data successfully, others:master write read data failed.
 *              DRV_API_TIMEOUT:timeout exit(solution refer to the note for spi_set_error_timeout).
 */
drv_api_status_e spi_master_write_read_plus(spi_sel_e spi_sel, unsigned char cmd, unsigned char *addrs, unsigned int addr_len, unsigned char *data, unsigned int data_len, spi_rd_tans_mode_e wr_mode)
{
    spi_tx_dma_dis(spi_sel);
    spi_rx_dma_dis(spi_sel);
    spi_tx_fifo_clr(spi_sel);
    spi_rx_fifo_clr(spi_sel);
    spi_tx_cnt(spi_sel, addr_len);
    spi_rx_cnt(spi_sel, data_len);
    spi_set_transmode(spi_sel, (spi_tans_mode_e)wr_mode);
    spi_set_cmd(spi_sel, cmd);
    spi_write(spi_sel, (unsigned char *)addrs, addr_len);
    spi_read(spi_sel, (unsigned char *)data, data_len);
    if (SPI_WAIT(spi_is_busy, spi_sel, g_spi_timeout_error.g_spi_error_timeout_us, g_spi_timeout_error.spi_timeout_handler, SPI_API_ERROR_TIMEOUT_BUS_BUSY)) {
        return DRV_API_TIMEOUT;
    }
    return DRV_API_SUCCESS;
}

/**
 * @brief       This function serves to set rx_dma channel and config dma rx default for spi master.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   chn     - dma channel.
 * @return      none.
 */
void spi_set_master_rx_dma_config(spi_sel_e spi_sel, dma_chn_e chn)
{
    (void)spi_sel; /* Note:The TL323X only supports GSPI, and the spi_sel parameter is retained for compatibility with different chip interfaces.*/
    s_gspi_master_rx_dma_chn = chn;
    dma_config(chn, &gspi_master_rx_dma_config);
}

/**
 * @brief       This function serves to set rx_dma channel and config dma rx default for spi slave.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   chn     - dma channel.
 * @return      none.
 */
void spi_set_slave_rx_dma_config(spi_sel_e spi_sel, dma_chn_e chn)
{
    (void)spi_sel; /* Note:The TL323X only supports GSPI, and the spi_sel parameter is retained for compatibility with different chip interfaces.*/
    s_gspi_slave_rx_dma_chn = chn;
    dma_config(chn, &gspi_slave_rx_dma_config);
}

/**
 * @brief       This function serves to set tx_dma channel and config dma tx default.
 * @param[in]   spi_sel - the spi module.
 * @param[in]   chn     - dma channel.
 * @return      none.
 */
void spi_set_tx_dma_config(spi_sel_e spi_sel, dma_chn_e chn)
{
    (void)spi_sel; /* Note:The TL323X only supports GSPI, and the spi_sel parameter is retained for compatibility with different chip interfaces.*/
    s_gspi_tx_dma_chn = chn;
    dma_config(chn, &gspi_tx_dma_config);
}

/**
 * @brief       this  function set spi dma channel.
 * @param[in]   spi_dma_chn - dma channel.
 * @param[in]   src_addr    - the address of source.
 * @param[in]   dst_addr    - the address of destination.
 * @param[in]   len         - the length of data.
 * @return      none.
 */
void spi_set_dma(dma_chn_e spi_dma_chn, unsigned int src_addr, unsigned int dst_addr, unsigned int len)
{
    dma_set_address(spi_dma_chn, src_addr, dst_addr);
    dma_set_size(spi_dma_chn, len, DMA_WORD_WIDTH);
    dma_chn_en(spi_dma_chn);
}

/**
 * @brief       this  function set spi tx dma channel.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   src_addr    - the address of source.
 * @param[in]   len         - the length of data.
 * @return      none.
 * @note        src_addr : must be aligned by word (4 bytes), otherwise the program will enter an exception
 *
 */
void spi_set_tx_dma(spi_sel_e spi_sel, unsigned char *src_addr, unsigned int len)
{
    spi_tx_dma_en(spi_sel);
    dma_set_address(s_gspi_tx_dma_chn, (unsigned int)src_addr, reg_spi_data_buf_adr(spi_sel));
    dma_set_size(s_gspi_tx_dma_chn, len, DMA_WORD_WIDTH);
    dma_chn_en(s_gspi_tx_dma_chn);
}

/**
 * @brief       this  function set spi rx dma channel.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   dst_addr    - the address of destination.
 * @param[in]   len         - the length of data.
 * @return      none.
 * @note
               -# If write_num is enabled, the length of data received by DMA will be written into the first four bytes of the address.
               -# A length greater than XX_len itself and a multiple of 4 is denoted as CEILING(XX_len,4). For example: XX_len=3 ,CEILING(XX_len,4)=4; XX_len=21 ,CEILING(Tx_length, 4)=24.
                  The actual length sent by master  is denoted as Tx_len, The length (param[in]-len) of the interface configuration is denoted as Rx_len.
                  when CEILING(Tx_len,4) > CEILING(Rx_len,4), When the length of the DMA carry reaches Rx_len, the DMA will not stop working and the excess data will not be carried into the buff.
                  for example:Tx_len=21,Rx_len=20,When the DMA stops working the buff is written with a length of 21 and only 20 bytes of data are stored.It is recommended to configure the appropriate Rx_len to avoid this situation.
               -# After DMA transfer completion, the interface needs to be invoked again to read the next batch of data.
 */
void spi_set_rx_dma(spi_sel_e spi_sel, unsigned char *dst_addr, unsigned int len)
{
    spi_rx_dma_en(spi_sel);
    dma_set_address(s_gspi_slave_rx_dma_chn, reg_spi_data_buf_adr(spi_sel), (unsigned int)(dst_addr));
    dma_set_size(s_gspi_slave_rx_dma_chn, len, DMA_WORD_WIDTH);
    dma_chn_en(s_gspi_slave_rx_dma_chn);
}

/**
 * @brief       This function serves to normal write data by dma.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   src_addr    - the pointer to the data for write.
 * @param[in]   len         - write length.
 * @return      none.
 * @note        src_addr : must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
void spi_master_write_dma(spi_sel_e spi_sel, unsigned char *src_addr, unsigned int len)
{
    spi_tx_fifo_clr(spi_sel);
    spi_tx_dma_en(spi_sel);
    spi_tx_cnt(spi_sel, len);
    spi_set_transmode(spi_sel, SPI_MODE_WRITE_ONLY);
    spi_set_dma(s_gspi_tx_dma_chn, (unsigned int)src_addr, reg_spi_data_buf_adr(spi_sel), len);
    spi_set_cmd(spi_sel, 0x00);
}

/**
 * @brief       This function serves to normal write cmd and address, then read data by dma.
 * This interface could be used for full duplex.
 * When this interface is used for full-duplex communication, it can only be used on the master side,
 * and the spi_master_config() interface must be called first to disable hardware cmd and hardware address, addr_len is equal to data_len.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   addr        - the address of slave.
 * @param[in]   addr_len    - write length.
 * @param[in]   data        - the pointer to the data for read.
 * @param[in]   data_len    - read length.
 * @return      none.
 * @note        addr/data : must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
void spi_master_write_read_dma(spi_sel_e spi_sel, unsigned char *addr, unsigned int addr_len, unsigned char *data, unsigned int data_len)
{
    spi_tx_fifo_clr(spi_sel);
    spi_rx_fifo_clr(spi_sel);
    spi_tx_dma_en(spi_sel);
    spi_rx_dma_en(spi_sel);
    spi_tx_cnt(spi_sel, addr_len);
    spi_rx_cnt(spi_sel, data_len);
    spi_set_transmode(spi_sel, SPI_MODE_WRITE_READ);

    spi_set_dma(s_gspi_tx_dma_chn, (unsigned int)(addr), reg_spi_data_buf_adr(spi_sel), addr_len);
    spi_set_dma(s_gspi_master_rx_dma_chn, reg_spi_data_buf_adr(spi_sel), (unsigned int)(data), data_len);
    spi_set_cmd(spi_sel, 0x00); //when  cmd  disable that  will not sent cmd,just trigger spi send.
}

/**
 * @brief       This function serves to single/dual/quad  write to the SPI slave by dma.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   cmd         - cmd one byte will first write.
 * @param[in]   addr        - the address of slave.
 * @param[in]   data        - pointer to the data need to write.
 * @param[in]   data_len    - length in byte of the data need to write.
 * @param[in]   wr_mode     - write mode.dummy or not dummy.
 * @return      none.
 * @note        data : must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
void spi_master_write_dma_plus(spi_sel_e spi_sel, unsigned char cmd, unsigned int addr, unsigned char *data, unsigned int data_len, spi_wr_tans_mode_e wr_mode)
{
    spi_tx_fifo_clr(spi_sel);
    spi_tx_dma_en(spi_sel);
    spi_tx_cnt(spi_sel, data_len);
    spi_set_transmode(spi_sel, (spi_tans_mode_e)wr_mode);
    spi_set_address(spi_sel, addr);
    spi_set_dma(s_gspi_tx_dma_chn, (unsigned int)data, reg_spi_data_buf_adr(spi_sel), data_len);
    spi_set_cmd(spi_sel, cmd);
}

/**
 * @brief       This function serves to single/dual/quad  read from the SPI slave by dma.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   cmd         - cmd one byte will first write.
 * @param[in]   addr        - the address of slave.
 * @param[in]   dst_addr    - pointer to the buffer that will cache the reading out data.
 * @param[in]   data_len    - length in byte of the data need to read.
 * @param[in]   rd_mode     - read mode.dummy or not dummy.
 * @return      none.
 * @note        dst_addr : must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
void spi_master_read_dma_plus(spi_sel_e spi_sel, unsigned char cmd, unsigned int addr, unsigned char *dst_addr, unsigned int data_len, spi_rd_tans_mode_e rd_mode)
{
    spi_rx_fifo_clr(spi_sel);
    spi_rx_dma_en(spi_sel);
    spi_set_transmode(spi_sel, (spi_tans_mode_e)rd_mode);
    spi_rx_cnt(spi_sel, data_len);
    spi_set_address(spi_sel, addr);
    spi_set_dma(s_gspi_master_rx_dma_chn, reg_spi_data_buf_adr(spi_sel), (unsigned int)(dst_addr), data_len);
    spi_set_cmd(spi_sel, cmd);
}

/**
 * @brief       This function serves to single/dual/quad write address and read from the SPI slave by dma.
 * This interface could be used for full duplex.
 * When this interface is used for full-duplex communication, it can only be used on the master side.
 * 1.the spi_master_config() interface must be called first to disable hardware cmd and hardware address
 * 2.must cmd is 0,addr_len is equal to rd_len,rd_mode is SPI_MODE_WRITE_AND_READ.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   cmd         - cmd one byte will first write.
 * @param[in]   addr        - the address of slave.
 * @param[in]   addr_len    - the length of address.
 * @param[in]   rd_data     - pointer to the buffer that will cache the reading out data.
 * @param[in]   rd_len      - length in byte of the data need to read.
 * @param[in]   rd_mode     - read mode.dummy or not dummy.
 * @return      none.
 * @note        addr/rd_data : must be aligned by word (4 bytes), otherwise the program will enter an exception.
 */
void spi_master_write_read_dma_plus(spi_sel_e spi_sel, unsigned char cmd, unsigned char *addr, unsigned int addr_len, unsigned char *rd_data, unsigned int rd_len, spi_rd_tans_mode_e rd_mode)
{
    spi_tx_fifo_clr(spi_sel);
    spi_rx_fifo_clr(spi_sel);
    spi_tx_dma_en(spi_sel);
    spi_rx_dma_en(spi_sel);
    spi_tx_cnt(spi_sel, addr_len);
    spi_rx_cnt(spi_sel, rd_len);
    spi_set_transmode(spi_sel, (spi_tans_mode_e)rd_mode);
    spi_set_dma(s_gspi_tx_dma_chn, (unsigned int)(addr), reg_spi_data_buf_adr(spi_sel), addr_len);
    spi_set_dma(s_gspi_master_rx_dma_chn, reg_spi_data_buf_adr(spi_sel), (unsigned int)(rd_data), rd_len);
    spi_set_cmd(spi_sel, cmd); //when  cmd  disable that  will not sent cmd,just trigger spi send .
}

/**
 * @brief       This function serves to write and read data simultaneously.
 * This interface can only be used by the master.
 * When initializing the master, call the spi_master_config() interface to disable the hardware cmd and hardware address,
 * and then start sending and receiving data.
 * The default chunk size sent and received by this interface is 8byte.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   write_data  - write data pointer.
 * @param[in]   read_data   - read data pointer.
 * @param[in]   len         - write/read length.
 * @return      none.
 */
void spi_master_write_read_full_duplex(spi_sel_e spi_sel, unsigned char *write_data, unsigned char *read_data, unsigned int len)
{
    unsigned int spi_wr_rd_size = 8;
    unsigned int chunk_size     = spi_wr_rd_size;
    spi_tx_dma_dis(spi_sel);
    spi_rx_dma_dis(spi_sel);
    spi_set_transmode(spi_sel, SPI_MODE_WRITE_AND_READ);
    spi_tx_fifo_clr(spi_sel);
    spi_rx_fifo_clr(spi_sel);
    spi_tx_cnt(spi_sel, len);
    spi_rx_cnt(spi_sel, len);
    spi_set_cmd(spi_sel, 0x00); //when  cmd  disable that  will not sent cmd,just trigger spi send .


    for (unsigned int i = 0; i < len; i = i + chunk_size) {
        if (chunk_size > (len - i)) {
            chunk_size = len - i;
        }
        spi_write(spi_sel, write_data + i, chunk_size);
        if (len < spi_wr_rd_size) {
            spi_read(spi_sel, read_data, chunk_size);
        } else if (i == 0) {
            spi_read(spi_sel, read_data, chunk_size - 1);
        } else if ((len - i) > spi_wr_rd_size) {
            spi_read(spi_sel, read_data + i - 1, chunk_size);
        } else {
            spi_read(spi_sel, read_data + i - 1, chunk_size + 1);
        }
    }
}

/**
 * @brief       This function serves to read data in normal.
 * @param[in]   spi_sel     - the spi module.
 * @param[in]   data        - read data pointer.
 * @param[in]   len         - read length.
 * @return      none.
 */
void spi_master_read(spi_sel_e spi_sel, unsigned char *data, unsigned int len)
{
    spi_rx_dma_dis(spi_sel);
    spi_rx_fifo_clr(spi_sel);
    spi_rx_cnt(spi_sel, len);
    spi_set_transmode(spi_sel, SPI_MODE_READ_ONLY);
    spi_set_cmd(spi_sel, 0x00); //when  cmd  disable that  will not sent cmd,just trigger spi send.
    spi_read(spi_sel, (unsigned char *)data, len);
}

/**
 * @brief     This function serves to config slave rx_dma channel llp.
 * @param[in] spi_sel   - the spi module.
 * @param[in] chn          - dma channel.
 * @param[in] dst_addr     - the dma address of destination.
 * @param[in] head_of_list - the head address of dma llp.
 * @return    none.
 */
void spi_set_slave_rx_dma_chain_llp(spi_sel_e spi_sel, dma_chn_e chn, unsigned char *dst_addr, unsigned int data_len, dma_chain_config_t *head_of_list)
{
    spi_set_slave_rx_dma_config(spi_sel, chn);
    spi_rx_fifo_clr(spi_sel);
    /*
     * Dma sof function needs to be disabled for rx.
     */
    dma_set_llp_sof_mode(chn, 0);
    spi_rx_dma_en(spi_sel);
    dma_set_address(chn, reg_spi_data_buf_adr(spi_sel), (unsigned int)(dst_addr));
    dma_set_size(chn, data_len, DMA_WORD_WIDTH);
    reg_dma_llp(chn) = (unsigned int)(head_of_list);
}

/**
 * @brief     This function serves to set rx dma chain transfer
 * @param[in] spi_sel   - the spi module.
 * @param[in] chn          - dma channel.
 * @param[in] config_addr  - the head of list of llp_pointer.
 * @param[in] llpointer    - the next element of llp_pointer.
 * @param[in] dst_addr     -the dma address of destination.
 * @param[in] data_len     -the length of dma size by byte.
 * @return    none.
 */
void spi_rx_dma_add_list_element(spi_sel_e spi_sel, dma_chn_e chn, dma_chain_config_t *config_addr, dma_chain_config_t *llpointer, unsigned char *dst_addr, unsigned int data_len)
{
    config_addr->dma_chain_ctl      = reg_dma_ctrl(chn) | BIT(0);
    config_addr->dma_chain_src_addr = reg_spi_data_buf_adr(spi_sel);
    config_addr->dma_chain_dst_addr = (unsigned int)(dst_addr);
    config_addr->dma_chain_data_len = dma_cal_size(data_len, 4);
    config_addr->dma_chain_llp_ptr  = (unsigned int)(llpointer);
}

/**
 * @brief      This function serves to set spi slave rx dma chain transfer.
 * @param[in] spi_sel   - the spi module.
 * @param[in] chn       - dma channel.
 * @param[in] dst_addr  - the dma address of destination.
 * @param[in] data_len  - the length of dma size by byte.
 * @return     none.
 */
void spi_slave_rx_dma_chain_init(spi_sel_e spi_sel, dma_chn_e chn, unsigned char *dst_addr, unsigned int data_len)
{
    spi_set_slave_rx_dma_chain_llp(spi_sel, chn, dst_addr, data_len, &g_spi_rx_dma_list_cfg);
    spi_rx_dma_add_list_element(spi_sel, chn, &g_spi_rx_dma_list_cfg, &g_spi_rx_dma_list_cfg, (unsigned char *)(dst_addr), data_len);
    dma_chn_en(chn);
}

/**
 * @brief     This function serves to config slave rx_dma channel llp.
 * @param[in] spi_sel       - the spi module.
 * @param[in] chn          - dma channel.
 * @param[in] src_addr     - the dma address of source.
 * @param[in] data_len     - the length of dma rx size by byte.
 * @param[in] head_of_list - the head address of dma llp.
 * @return    none.
 */
void spi_set_slave_tx_dma_chain_llp(spi_sel_e spi_sel, dma_chn_e chn, unsigned char *src_addr, unsigned int data_len, dma_chain_config_t *head_of_list)
{
    dma_config(chn, &gspi_tx_dma_config);
    /* The sof function of dma needs to be started at tx otherwise data anomalies will occur.
     */
    dma_set_llp_sof_mode(chn, 1);
    spi_tx_dma_en(spi_sel);
    dma_set_address(chn, (unsigned int)(src_addr), reg_spi_data_buf_adr(spi_sel));
    dma_set_size(chn, data_len, DMA_WORD_WIDTH);
    reg_dma_llp(chn) = (unsigned int)(head_of_list);
}

/**
 * @brief     This function serves to set tx dma chain transfer
 * @param[in] spi_sel       - the spi module.
 * @param[in] chn            - dma channel.
 * @param[in] config_addr    - the head of list of llp_pointer.
 * @param[in] llpointer      - the next element of llp_pointer.
 * @param[in] src_addr       - the dma address of source.
 * @param[in] data_len       - the length of dma size by byte.
 * @return    none.
 */
void spi_tx_dma_add_list_element(spi_sel_e spi_sel, dma_chn_e chn, dma_chain_config_t *config_addr, dma_chain_config_t *llpointer, unsigned char *src_addr, unsigned int data_len)
{
    config_addr->dma_chain_ctl      = reg_dma_ctrl(chn) | BIT(0);
    config_addr->dma_chain_src_addr = (unsigned int)(src_addr);
    config_addr->dma_chain_dst_addr = reg_spi_data_buf_adr(spi_sel);
    config_addr->dma_chain_data_len = dma_cal_size(data_len, 4);
    config_addr->dma_chain_llp_ptr  = (unsigned int)(llpointer);
}

/**
 * @brief      This function serves to set spi slave tx dma chain transfer.
 * @param[in] spi_sel   - the spi module.
 * @param[in] chn       - dma channel.
 * @param[in] dst_addr  - the dma address of destination.
 * @param[in] data_len  - the length of dma size by byte.
 * @return     none.
 */
void spi_slave_tx_dma_chain_init(spi_sel_e spi_sel, dma_chn_e chn, unsigned char *dst_addr, unsigned int data_len)
{
    spi_set_slave_tx_dma_chain_llp(spi_sel, chn, dst_addr, data_len, &g_spi_tx_dma_list_cfg);
    spi_tx_dma_add_list_element(spi_sel, chn, &g_spi_tx_dma_list_cfg, &g_spi_tx_dma_list_cfg, (unsigned char *)(dst_addr), data_len);
    dma_chn_en(chn);
}
