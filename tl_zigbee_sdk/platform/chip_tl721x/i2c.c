/********************************************************************************************************
 * @file    i2c.c
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
#include "i2c.h"

static unsigned char i2c_dma_tx_chn;
static unsigned char i2c_dma_rx_chn;

dma_config_t i2c_tx_dma_config = {
    .dst_req_sel    = DMA_REQ_I2C_TX,     //tx req
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
    .auto_en        = 0, //must 0
};

dma_config_t i2c_rx_dma_config = {
    .dst_req_sel    = 0,
    .src_req_sel    = DMA_REQ_I2C_RX,
    .dst_addr_ctrl  = DMA_ADDR_INCREMENT,
    .src_addr_ctrl  = DMA_ADDR_FIX,
    .dstmode        = DMA_NORMAL_MODE,
    .srcmode        = DMA_HANDSHAKE_MODE,
    .dstwidth       = DMA_CTR_WORD_WIDTH, //must word
    .srcwidth       = DMA_CTR_WORD_WIDTH, ////must word
    .src_burst_size = 0,
    .read_num_en    = 0,
    .priority       = 0,
    .write_num_en   = 1,
    .auto_en        = 0, //must 0
};

/**
 * record i2c error code, can obtain the value through the i2c_get_error_timeout_code interface.
 */
volatile i2c_api_error_timeout_code_e g_i2c_error_timeout_code = I2C_API_ERROR_TIMEOUT_NONE;

/**
 * This parameter is 0x20 by default, that is, each write or read API opens the stop command.
 * if g_i2c_stop_en=0x00,it means every write or read API will disable stop command.
 */
unsigned char g_i2c_stop_en = 0x20;

/**
 * i2c error timeout(us),a large value is set by default,can set it by i2c_set_error_timeout().
 */
unsigned int g_i2c_error_timeout_us = 0xffffffff;

/**
 * @brief     This function serves to set the i2c timeout(us).
 * @param[in] timeout_us - the timeout(us).
 * @return    none.
 * @note      the default timeout (g_i2c_error_timeout_us) is the larger value.If the timeout exceeds the feed dog time and triggers a watchdog restart,
 *            g_i2c_error_timeout_us can be changed to a smaller value via this interface, depending on the application.
 *            g_i2c_error_timeout_us the minimum time must meet the following conditions:
 *            1. eight i2c data;
 *            2. maximum interrupt processing time;
 *            3. maximum normal stretch time of the slave;(the stretch description: if the slave end cannot process in time, the clk will be stretch,the master will hold,
 *               when the slave is processed, the clk will be released and the master will continue working.)
 *            when timeout exits, solution:
 *            1.reset master,reset slave(i2c_hw_fsm_reset);
 *            2.ensure that the clk/data is high(gpio_get_level);
 */
void i2c_set_error_timeout(unsigned int timeout_us)
{
    g_i2c_error_timeout_us = timeout_us;
}

/**
 * @brief     This function serves to return the i2c api error code.
 * @return    none.
 */
i2c_api_error_timeout_code_e i2c_get_error_timeout_code(void)
{
    return g_i2c_error_timeout_code;
}

/**
 * @brief      The function of this interface is equivalent to that after the user finishes calling the write or read interface, the stop signal is not sent,
 *             and then the write or read command is executed again. The driver defaults that every write or read API will send a stop command at the end
 * @param[in]  en - Input parameters.Decide whether to disable the stop function after each write or read interface
 * @return     none
 */
void i2c_master_send_stop(unsigned char en)
{
    //this function uses the en parameter to determine whether i2c master is configured with stop,
    //if it is configured with stop, then master needs to send stop again after the stop signal is sent, needs to manually configure i2c stop again.
    if (en == 1) {
        g_i2c_stop_en = 0x20;
    } else {
        g_i2c_stop_en = 0x00;
    }
}

/**
 * @brief      This function selects a pin port for I2C interface.
 * @param[in]  sda_pin - the pin port selected as I2C sda pin port.
 * @param[in]  scl_pin - the pin port selected as I2C scl pin port.
 * @return     none
 */
void i2c_set_pin(gpio_func_pin_e sda_pin, gpio_func_pin_e scl_pin)
{
    //When the pad is configured with mux input and a pull-up resistor is required, gpio_input_en needs to be placed before gpio_function_dis,
    //otherwise first set gpio_input_disable and then call the mux function interface,the mux pad will misread the short low-level timing.confirmed by minghai.20210709.
    gpio_input_en((gpio_pin_e)sda_pin); //enable sda input
    gpio_input_en((gpio_pin_e)scl_pin); //enable scl input
    gpio_set_up_down_res((gpio_pin_e)sda_pin, GPIO_PIN_PULLUP_10K);
    gpio_set_up_down_res((gpio_pin_e)scl_pin, GPIO_PIN_PULLUP_10K);

    gpio_set_mux_function(scl_pin, I2C_SCL_IO);
    gpio_set_mux_function(sda_pin, I2C_SDA_IO);
    //disable sda_pin and scl_pin gpio function.
    gpio_function_dis((gpio_pin_e)scl_pin);
    gpio_function_dis((gpio_pin_e)sda_pin);
}

/**
 * @brief      This function serves to enable i2c master function.
 * @return     none.
 */
void i2c_master_init(void)
{
    BM_SET(reg_rst0, FLD_RST0_I2C);
    BM_SET(reg_clk_en0, FLD_CLK0_I2C_EN);
    reg_i2c_ctrl2 |= FLD_I2C_MASTER; //i2c master enable.
    i2c_master_detect_nack_en();
    i2c_master_stretch_en();
}

/**
 * @brief      This function serves to set the i2c clock frequency.The i2c clock is consistent with the pclk.
 * @param[in]  clock - the division factor of I2C clock,the i2c frequency can meet 1M, and the maximum limit is not confirmed,
 *             I2C frequency = pclk/ (4*DivClock).
 * @return     none
 */
void i2c_set_master_clk(unsigned char clock)
{
    //i2c frequency = pclk/(4*clock)
    reg_i2c_sp = clock;

    //set enable flag.
    reg_clk_en0 |= FLD_CLK0_I2C_EN;
}

/**
 * @brief      This function serves to enable slave mode.
 * @param[in]  id - the id of slave device.it contains write or read bit,the laster bit is write or read bit.
 *                       ID|0x01 indicate read. ID&0xfe indicate write.
 * @return     none
 */
void i2c_slave_init(unsigned char id)
{
    BM_SET(reg_rst0, FLD_RST0_I2C);
    BM_SET(reg_clk_en0, FLD_CLK0_I2C_EN);
    reg_i2c_ctrl2 &= (~FLD_I2C_MASTER); //enable slave mode.
    reg_i2c_id = id;
}

/**
 * @brief     This function serves to record the api status.
 * @param[in] i2c_error_timeout_code - i2c_api_error_code_e.
 * @return    none.
 * @note      This function can be rewritten according to the application scenario,can by g_i2c_error_timeout_code to obtain details about the timeout reason,
 *            for the solution, refer to the i2c_set_error_timeout note.
 */
__attribute__((weak)) void i2c_timeout_handler(unsigned int i2c_error_timeout_code)
{
    g_i2c_error_timeout_code = i2c_error_timeout_code;
}

//I2c timeout wait
#define I2C_WAIT(i2c_api_error_code) wait_condition_fails_or_timeout(i2c_master_busy, g_i2c_error_timeout_us, i2c_timeout_handler, (unsigned int)i2c_api_error_code)

/**
 *@brief   in no_dma master,check whether the master ID phase receives nack that slave returns,if nack is received, launch stop cycle.
 *@return  0:no detect nack, 1: detect nack in id phase.
 */
static inline unsigned char i2c_master_id_nack_detect(void)
{
    //After enabling master nack, I2C_MASTER_NAK_IRQ interrupt will be activated if master receives nack returned by slave.
    if (i2c_get_irq_status(I2C_MASTER_NAK_STATUS)) {
        i2c_clr_irq_status(I2C_MASTER_NAK_STATUS);
        reg_i2c_sct1 = (FLD_I2C_LS_STOP);
        //wait for the STOP signal to finish sending.
        if (I2C_WAIT(I2C_API_ERROR_TIMEOUT_STOP)) {
            return DRV_API_TIMEOUT;
        }
        return 1;
    }
    return 0;
}

/**
 *@brief   in no_dma master,check whether the master data phase receives nack that slave return,if nack is received,clear fifo.
 *@return  0:no detect nack;
 *         1: detect nack in data phase;
 *         DRV_API_TIMEOUT:timeout exit;
 */
static unsigned char i2c_master_data_nack_detect(void)
{
    if (i2c_get_irq_status(I2C_MASTER_NAK_STATUS)) {
        i2c_clr_irq_status(I2C_MASTER_NAK_STATUS);
        reg_i2c_sct1 = (FLD_I2C_LS_STOP);
        //If configure stop once more, will no longer be busy as long as the current stop is sent.
        if (I2C_WAIT(I2C_API_ERROR_TIMEOUT_STOP)) {
            return DRV_API_TIMEOUT;
        }
        i2c_clr_irq_status(I2C_TX_BUF_STATUS);
        return 1;
    }
    return 0;
}

//nack detect status
#define I2C_NACK_DETECT(detect_func)         \
    do {                                     \
        int err = detect_func();             \
        if (err == 1) {                      \
            return 0;                        \
        } else if (err == DRV_API_TIMEOUT) { \
            return DRV_API_TIMEOUT;          \
        }                                    \
    } while (0)

/**
 * @brief      The function of this API is to ensure that the data can be successfully sent out.
 *             can choose whether to send stop,if i2c stop is not configured, the next time a start signal is sent, it will be a restart signal,
 *             but if a nack exception is received in id or data phase, during exception handling, a stop signal will be sent.
 *             1.in the id phase,detect nack,stop sending.
 *             2.in the data phase,detect nack,stop sending.
 * @param[in]  id   - to set the slave ID.
 * @param[in]  data - The data to be sent.
 * @param[in]  len  - This length is the total length, including both the length of the slave RAM address and the length of the data to be sent.
 * @return     0: received nack in id or data phase,and then stop
 *               - check whether the id is correct;
 *               - if sometimes ack and sometimes nack, check the io driver capability, and use the oscilloscope to check compliance with the i2c spec;
 *               - check whether the slave is abnormal;
 *             1: write successfully;
 *             DRV_API_TIMEOUT: timeout return(solution refer to the note for i2c_set_error_timeout);
 */
unsigned char i2c_master_write(unsigned char id, unsigned char *data, unsigned int len)
{
    i2c_clr_irq_status(I2C_TX_BUF_STATUS);
    reg_i2c_id   = id & (~FLD_I2C_WRITE_READ_BIT); //BIT(0):R:High  W:Low
    reg_i2c_sct1 = (FLD_I2C_LS_ID | FLD_I2C_LS_START);
    if (I2C_WAIT(I2C_API_ERROR_TIMEOUT_ID)) {
        return DRV_API_TIMEOUT;
    }

    I2C_NACK_DETECT(i2c_master_id_nack_detect);
    i2c_master_set_len(len);
    unsigned int       cnt         = 0;
    unsigned long long start_cycle = rdmcycle();
    while (cnt < len) {
        I2C_NACK_DETECT(i2c_master_data_nack_detect);
        if (i2c_get_tx_buf_cnt() < 8) {
            start_cycle               = rdmcycle();
            reg_i2c_data_buf(cnt % 4) = data[cnt]; //write data
            cnt++;
            if (cnt == 1) {
                //If stop is configured in the data phase, when nack is received in the data phase, stop will also be configured in the exception handling,
                //which may result in one more stop configured. therefore, the next time you read or write, stop needs to be cleared to avoid sending stop by mistake.
                reg_i2c_sct1 &= ~FLD_I2C_LS_STOP;
                reg_i2c_sct1 = (FLD_I2C_LS_DATAW | g_i2c_stop_en); //launch stop cycle
            }
        }
        if (core_cclk_time_exceed(start_cycle, g_i2c_error_timeout_us)) {
            i2c_timeout_handler(I2C_API_ERROR_TIMEOUT_WRITE_DATA);
            return DRV_API_TIMEOUT;
        }
    }
    while (i2c_get_tx_buf_cnt()) {
        if (core_cclk_time_exceed(start_cycle, g_i2c_error_timeout_us)) {
            i2c_timeout_handler(I2C_API_ERROR_TIMEOUT_WRITE_DATA);
            return DRV_API_TIMEOUT;
        }
        I2C_NACK_DETECT(i2c_master_data_nack_detect);
    }
    if (I2C_WAIT(I2C_API_ERROR_TIMEOUT_STOP)) {
        return DRV_API_TIMEOUT;
    }

    return 1;
}

/**
 * @brief      This function serves to read a packet of data from the specified address of slave device.
 *             can choose whether to send stop,if i2c stop is not configured, the next time a start signal is sent, it will be a restart signal,
 *             but if a nack exception is received in id phase, during exception handling, a stop signal will be sent.
 *             only in id phase, detect nack,stop receiving.
 * @param[in]  id   - to set the slave ID.
 * @param[in]  data - Store the read data
 * @param[in]  len  - The total length of the data read back.
 * @return     0: the master receive NACK after sending out the id and then send stop;
 *               - check whether the id is correct;
 *               - if sometimes ack and sometimes nack, check the io driver capability, and use the oscilloscope to check compliance with the i2c spec;
 *               - check whether the slave is abnormal;
 *             1: the master receive the data successfully;
 *             DRV_API_TIMEOUT: timeout return(solution refer to the note for i2c_set_error_timeout);
 */
unsigned char i2c_master_read(unsigned char id, unsigned char *data, unsigned int len)
{
    i2c_clr_irq_status(I2C_RX_BUF_STATUS);
    reg_i2c_id = id | FLD_I2C_WRITE_READ_BIT; //BIT(0):R:High  W:Low
    reg_i2c_ctrl3 |= FLD_I2C_RNCK_EN;         //i2c rnck enable.
    reg_i2c_sct1 = (FLD_I2C_LS_ID | FLD_I2C_LS_START);
    if (I2C_WAIT(I2C_API_ERROR_TIMEOUT_ID)) {
        return DRV_API_TIMEOUT;
    }
    I2C_NACK_DETECT(i2c_master_id_nack_detect);
    i2c_master_set_len(len);
    reg_i2c_sct1 &= ~FLD_I2C_LS_STOP;
    reg_i2c_sct1 = (FLD_I2C_LS_ID_R | FLD_I2C_LS_DATAR | g_i2c_stop_en);

    unsigned int       cnt         = 0;
    unsigned long long start_cycle = rdmcycle();
    while (cnt < len) {
        if (i2c_get_rx_buf_cnt() > 0) {
            data[cnt] = reg_i2c_data_buf(cnt % 4);
            cnt++;
            start_cycle = rdmcycle();
        }
        if (core_cclk_time_exceed(start_cycle, g_i2c_error_timeout_us)) {
            i2c_timeout_handler(I2C_API_ERROR_TIMEOUT_READ_DATA);
            return DRV_API_TIMEOUT;
        }
    }
    if (I2C_WAIT(I2C_API_ERROR_TIMEOUT_STOP)) {
        return DRV_API_TIMEOUT;
    }
    return 1;
}

/**
 * @brief      This function serves to write data and read data,
 *             can choose whether to send stop or not,If i2c stop is not configured,the master between writing and reading, it will be a restart signal,
 *             and after reading and writing, a stop signal is sent,but the stop configure,stays in its original state,
 *             when a nack exception signal is received, during exception handling, a stop signal will be sent.
 * @param[in]  id      - to set the slave ID.
 * @param[in]  wr_data - The data to be sent.
 * @param[in]  wr_len  - This length is the total length, including both the length of the slave RAM address and the length of the data to be sent.
 * @param[in]  rd_data - Store the read data
 * @param[in]  rd_len  - The total length of the data read back.
 * @return     0: the master detect nack in the id or data when the master write,stop sending,and return.
 *                or the master detect nack in the id when the master read,stop receiving,and return.
 *                - check whether the id is correct;
 *                - if sometimes ack and sometimes nack, check the io driver capability, and use the oscilloscope to check compliance with the i2c spec;
 *                - check whether the slave is abnormal;
 *             1: the master receive the data successfully.
 *             DRV_API_TIMEOUT: timeout return(solution refer to the note for i2c_set_error_timeout);
 */
unsigned char i2c_master_write_read(unsigned char id, unsigned char *wr_data, unsigned int wr_len, unsigned char *rd_data, unsigned int rd_len)
{
    //set i2c master write.
    if (!i2c_master_write(id, wr_data, wr_len)) {
        return 0;
    }
    //i2c_master_write_read: the master between writing and reading,it will be a restart signal,and after reading and writing, a stop signal is sent,
    //in order to after write and read, a stop signal is sent,so need to enable stop during read.
    unsigned char i2c_stop_en = g_i2c_stop_en;
    if (0x00 == i2c_stop_en) {
        i2c_master_send_stop(1);
    }
    //set i2c master read.
    if (!i2c_master_read(id, rd_data, rd_len)) {
        if (0x00 == i2c_stop_en) {
            i2c_master_send_stop(0);
        }
        return 0;
    }
    //since the configuration state of stop is changed in this interface,
    //the previous configuration needs to be restored either after the function reads or when an exception occurs during the read process.
    if (0x00 == i2c_stop_en) {
        i2c_master_send_stop(0);
    }
    return 1;
}

/**
 * @brief      The function of this API is just to write data to the i2c tx_fifo by DMA.
 *             can choose whether to send stop,if i2c stop is not configured, the next time a start signal is sent, it will be a restart signal,
 *             but if a nack exception is received in id or data phase, during exception handling, a stop signal will be sent.
 * @param[in]  id   - to set the slave ID.
 * @param[in]  data - The data to be sent,must be aligned by word (4 bytes).
 * @param[in]  len  - This length is the total length, including both the length of the slave RAM address and the length of the data to be sent,
 *                    the maximum transmission length of DMA is 0xFFFFFC bytes, so dont'n over this length.
 * @return     none.
 * @note       After the DMA transfer is complete, the interface needs to be re-invoked to write the next batch of data.
 */
void i2c_master_write_dma(unsigned char id, unsigned char *data, unsigned int len)
{
    i2c_clr_irq_status(I2C_TX_BUF_STATUS);
    reg_i2c_id = (id & (~FLD_I2C_WRITE_READ_BIT)); //BIT(0):R:High  W:Low

    dma_set_size(i2c_dma_tx_chn, len, DMA_WORD_WIDTH);
    dma_set_address(i2c_dma_tx_chn, (unsigned int)(data), reg_i2c_data_buf0_addr);
    dma_chn_en(i2c_dma_tx_chn);

    i2c_master_set_len(len);
    reg_i2c_sct1 &= ~FLD_I2C_LS_STOP;
    reg_i2c_sct1 = (FLD_I2C_LS_ID | FLD_I2C_LS_START | FLD_I2C_LS_DATAW | g_i2c_stop_en);
}

/**
 * @brief      This function serves to read a packet of data from the specified address of slave device by dma.
 *             can choose whether to send stop,if i2c stop is not configured, the next time a start signal is sent, it will be a restart signal,
 *             but if a nack exception is received in id phase, during exception handling, a stop signal will be sent.
 * @param[in]  id      - to set the slave ID.
 * @param[in]  rx_data - Store the read data,must be aligned by word (4 bytes).
 * @param[in]  len     - The total length of the data read back,the maximum transmission length of DMA is 0xFFFFFC bytes, so dont'n over this length.
 * @return     none
 * @note       After the DMA transfer is complete, the interface needs to be re-invoked to read the next batch of data.
 */
void i2c_master_read_dma(unsigned char id, unsigned char *rx_data, unsigned int len)
{
    i2c_clr_irq_status(I2C_RX_BUF_STATUS);
    reg_i2c_id = id | FLD_I2C_WRITE_READ_BIT; //BIT(0):R:High  W:Low
    reg_i2c_ctrl3 |= FLD_I2C_RNCK_EN;         //i2c rnck enable.

    dma_set_wnum_dis(i2c_dma_rx_chn);         //The data received by the master is the same length as the data sent, so there is no need for a writer_num.
    dma_set_size(i2c_dma_rx_chn, len, DMA_WORD_WIDTH);
    dma_set_address(i2c_dma_rx_chn, reg_i2c_data_buf0_addr, (unsigned int)(rx_data));
    dma_chn_en(i2c_dma_rx_chn);

    i2c_master_set_len(len);
    reg_i2c_sct1 &= ~FLD_I2C_LS_STOP;
    reg_i2c_sct1 = (FLD_I2C_LS_ID | FLD_I2C_LS_DATAR | FLD_I2C_LS_START | FLD_I2C_LS_ID_R | g_i2c_stop_en);
}

/**
 * @brief      Send an amount of data in DMA mode
 * @param[in]  data -  Pointer to data buffer, it must be 4-bytes aligned address
 * @param[in]  len  -  Amount of data to be sent in bytes, range from 1 to 0xFFFFFC
 * @return     none.
 * @note       After the DMA transfer is complete, the interface needs to be re-invoked to write the next batch of data.
 */
void i2c_slave_set_tx_dma(unsigned char *data, unsigned int len)
{
    i2c_clr_irq_status(I2C_TX_BUF_STATUS);
    dma_set_address(i2c_dma_tx_chn, (unsigned int)(data), reg_i2c_data_buf0_addr);
    dma_set_size(i2c_dma_tx_chn, len, DMA_WORD_WIDTH);
    dma_chn_en(i2c_dma_tx_chn);
}

/**
@brief         This function serves to receive a packet of data from master device.
@param[in]     data - the pointer of rx_buff, it must be 4-bytes aligned address
@param[in]     len - length in bytes, must be a multiple of 4, with a maximum value of 0xFFFFFC.
@return        none.
@note
               -# If write_num is enabled, the length of data received by DMA will be written into the first four bytes of the address.
               -# A length greater than XX_len itself and a multiple of 4 is denoted as CEILING(XX_len,4). For example: XX_len=3 ,CEILING(XX_len,4)=4; XX_len=21 ,CEILING(Tx_length, 4)=24.
                  The actual length sent by master  is denoted as Tx_len, The length (param[in]-len) of the interface configuration is denoted as Rx_len.
                  when CEILING(Tx_len,4) > CEILING(Rx_len,4), When the length of the DMA carry reaches Rx_len, the DMA will not stop working and the excess data will not be carried into the buff.
                  for example:Tx_len=21,Rx_len=20,When the DMA stops working the buff is written with a length of 21 and only 20 bytes of data are stored.It is recommended to configure the appropriate Rx_len to avoid this situation.
                -# After DMA transfer completion, the interface needs to be invoked again to read the next batch of data.
*/
void i2c_slave_set_rx_dma(unsigned char *data, unsigned int len)
{
    dma_chn_dis(i2c_dma_rx_chn);
    i2c_clr_irq_status(I2C_RX_BUF_STATUS);
    dma_set_address(i2c_dma_rx_chn, reg_i2c_data_buf0_addr, (unsigned int)(data));
    dma_set_size(i2c_dma_rx_chn, len, DMA_WORD_WIDTH);
    dma_chn_en(i2c_dma_rx_chn);
}

unsigned char i2c_slave_rx_index = 0; //logs the current read position in the fifo.

/**
 * @brief     This function serves to receive data .
 * @param[in]  data - the data need read.
 * @param[in]  len - The total length of the data
 * @return    none
 */
void i2c_slave_read(unsigned char *data, unsigned int len)
{
    unsigned int cnt = 0;
    while (cnt < len) {
        if (i2c_get_rx_buf_cnt() > 0) {
            data[cnt] = reg_i2c_data_buf(i2c_slave_rx_index);
            i2c_slave_rx_index++;
            i2c_slave_rx_index &= 0x03;
            cnt++;
        }
    }
}

/**
 * @brief     This function serves to receive i2c data by byte with not DMA method.
 * @param[in]  data - the data need send.
 * @param[in]  len - The total length of the data.
 * @return    none
 */
void i2c_slave_write(unsigned char *data, unsigned int len)
{
    i2c_clr_irq_status(I2C_TX_BUF_STATUS);
    unsigned int cnt = 0;
    while (cnt < len) {
        if (i2c_get_tx_buf_cnt() < 8) {
            reg_i2c_data_buf(cnt % 4) = data[cnt];
            cnt++;
        }
    }
}

/**
 * @brief     This function serves to set i2c tx_dma channel and config dma tx default.
 * @param[in] chn: dma channel.
 * @return    none
 */
void i2c_set_tx_dma_config(dma_chn_e chn)
{
    i2c_dma_tx_chn = chn;
    dma_config(chn, &i2c_tx_dma_config);
}

/**
 * @brief     This function serves to set i2c rx_dma channel and config dma rx default.
 * @param[in] chn: dma channel.
 * @return    none
 */
void i2c_set_rx_dma_config(dma_chn_e chn)
{
    i2c_dma_rx_chn = chn;
    dma_config(chn, &i2c_rx_dma_config);
}

/**
 * @brief     This function serves to configure the master i2c send and receive byte length,the hardware needs to know what the length is.
 * @param[in] len - the maximum transmission length of i2c is 0xffffff bytes, so dont'n over this length.
 * @return    none
 */
void i2c_master_set_len(unsigned int len)
{
    reg_i2c_len(0) = len & 0xff;
    reg_i2c_len(1) = (len >> 8) & 0xff;
    reg_i2c_len(2) = (len >> 16) & 0xff;
}

/**
 *@brief     in slave ,judge whether the master is sending a read cmd or a write cmd.
 *@return    1:if return 1,it means that the slave is to get master read cmd.
 *           0:if return 0,it means that the slave is to get master write cmd.
 */
i2c_slave_wr_e i2c_slave_get_cmd(void)
{
    return (i2c_slave_status1 & FLD_I2C_SLAVE_READ);
}

/**
 * @brief     in master,judge whether master is to read or write
 * @return    1:if return 1,it means that the state of the master is read status.
 *            0:if return 0,it means that the state of the master is not read status.
 */
i2c_master_wr_e i2c_get_master_wr_status(void)
{
    return (reg_i2c_sct1 & FLD_I2C_LS_ID_R);
}

/********************************************************************************************
 *****|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|*****
 *****|                             i2c1_m.c                                           |*****
 *****|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|*****
 ********************************************************************************************/
/**
 * This parameter is 0x20 by default, that is, each write or read API opens the stop command.
 * if g_i2c_stop_en=0x00,it means every write or read API will disable stop command.
 */
unsigned char g_i2c1_m_stop_en = 0x20;

/**
 * record i2c1_m error code, can obtain the value through the i2c1_m_get_error_timeout_code() interface.
 */
volatile i2c1_m_api_error_timeout_code_e g_i2c1_m_error_timeout_code = I2C1_M_API_ERROR_TIMEOUT_NONE;

/**
 * i2c1_m error timeout(us),a large value is set by default,can set it by i2c1_m_set_error_timeout().
 */
unsigned int g_i2c1_m_error_timeout_us = 0xffffffff;

/**
 * @brief     This function serves to set the i2c1_m timeout(us).
 * @param[in] timeout_us - the timeout(us).
 * @return    none.
 * @note      i2c1_m_master_init add i2c1_m_master_stretch_en(), the master will be stuck if the slave pulls the Master abnormally,
 *            the default timeout (g_i2c1_m_error_timeout_us) is the larger value.If the timeout exceeds the feed dog time and triggers a watchdog restart,
 *            g_i2c1_m_error_timeout_us can be changed to a smaller value via this interface, depending on the application.
 *            g_i2c1_m_error_timeout_us the minimum time must meet the following conditions:
 *            1. two i2c data;
 *            2. maximum interrupt processing time;
 *            3. maximum normal stretch time of the slave;(the stretch description: if the slave end cannot process in time, the clk will be stretch,the master will hold,
 *               when the slave is processed, the clk will be released and the master will continue working.)
 *            when timeout exits, solution:
 *            1.reset master,reset slave(i2c1_m_hw_fsm_reset);
 *            2.ensure that the clk/data is high(gpio_get_level);;
 */
void i2c1_m_set_error_timeout(unsigned int timeout_us)
{
    g_i2c1_m_error_timeout_us = timeout_us;
}

/**
 * @brief     This function serves to return the i2c1_m api error code.
 * @return    none.
 */
i2c1_m_api_error_timeout_code_e i2c1_m_get_error_timeout_code(void)
{
    return g_i2c1_m_error_timeout_code;
}

/**
 * @brief      The function of this interface is equivalent to that after the user finishes calling the write or read interface, the stop signal is not sent,
 *             and then the write or read command is executed again. The driver defaults that every write or read API will send a stop command at the end
 * @param[in]  en - Input parameters.Decide whether to disable the stop function after each write or read interface
 * @return     none
 */
void i2c1_m_master_send_stop(unsigned char en)
{
    //this function uses the en parameter to determine whether i2c master is configured with stop,
    //if it is configured with stop, then master needs to send stop again after the stop signal is sent, needs to manually configure i2c stop again.
    if (en == 1) {
        g_i2c1_m_stop_en = 0x20;
    } else {
        g_i2c1_m_stop_en = 0x00;
    }
}

/**
 * @brief      This function selects a pin port for I2C interface.
 * @param[in]  sda_pin - the pin port selected as I2C sda pin port.
 * @param[in]  scl_pin - the pin port selected as I2C scl pin port.
 * @return     none
 */
void i2c1_m_set_pin(gpio_func_pin_e sda_pin, gpio_func_pin_e scl_pin)
{
    //When the pad is configured with mux input and a pull-up resistor is required, gpio_input_en needs to be placed before gpio_function_dis,
    //otherwise first set gpio_input_disable and then call the mux function interface,the mux pad will misread the short low-level timing.confirmed by minghai.20210709.
    gpio_input_en((gpio_pin_e)sda_pin); //enable sda input
    gpio_input_en((gpio_pin_e)scl_pin); //enable scl input
    gpio_set_up_down_res((gpio_pin_e)sda_pin, GPIO_PIN_PULLUP_10K);
    gpio_set_up_down_res((gpio_pin_e)scl_pin, GPIO_PIN_PULLUP_10K);
    //disable sda_pin and scl_pin gpio function.
    gpio_function_dis((gpio_pin_e)scl_pin);
    gpio_function_dis((gpio_pin_e)sda_pin);
    gpio_set_mux_function(scl_pin, I2C1_SCL_IO);
    gpio_set_mux_function(sda_pin, I2C1_SDA_IO);
}

/**
 * @brief      This function serves to enable i2c1_m master function.
 * @return     none.
 */
void i2c1_m_master_init(void)
{
    BM_SET(reg_rst2, FLD_RST2_I2C1);
    BM_SET(reg_clk_en2, FLD_CLK2_I2C1_EN);
    reg_i2c1_m_sct0 |= FLD_I2C1_M_MASTER; //i2c master enable.
    i2c1_m_master_stretch_en();
}

/**
 * @brief      This function serves to set the i2c1_m clock frequency.The i2c1_m clock is consistent with the pclk.
 * @param[in]  clock - the division factor of i2c1_m clock,
 *             i2c1_m frequency = pclk / (4*clock).
 * @return     none
 */
void i2c1_m_set_master_clk(unsigned char clock)
{
    //i2c1_m frequency = system_clock/(4*clock)
    reg_i2c1_m_sp = clock;

    //set enable flag.
    reg_clk_en0 |= FLD_CLK0_I2C_EN;
}

/**
 * @brief     This function serves to record the api status.
 * @param[in] i2c1_m_error_timeout_code - i2c1_m_api_error_code_e.
 * @return    none.
 * @note      This function can be rewritten according to the application scenario,can by g_i2c1_m_error_timeout_code to obtain details about the timeout reason,
 *            for the solution, refer to the i2c1_m_set_error_timeout note.
 */
__attribute__((weak)) void i2c1_m_timeout_handler(unsigned int i2c1_m_error_timeout_code)
{
    g_i2c1_m_error_timeout_code = i2c1_m_error_timeout_code;
}

#define I2C1_M_WAIT(i2c1_m_api_error_code) wait_condition_fails_or_timeout(i2c1_m_master_busy, g_i2c1_m_error_timeout_us, i2c1_m_timeout_handler, (unsigned int)i2c1_m_api_error_code)

/**
 *@brief   check whether receives nack that slave return.
 *@return  DRV_API_SUCCESS: detect nack;
 *         DRV_API_FAILURE: no detect nack in data phase;
 *         DRV_API_TIMEOUT: timeout exit;
 */
static drv_api_status_e i2c1_m_nack_detect(void)
{
    if (reg_i2c1_m_mst & FLD_I2C1_M_MAT_ACK_IN) {
        reg_i2c1_m_ctrl = FLD_I2C1_M_LS_STOP;
        if (I2C1_M_WAIT(I2C1_M_API_ERROR_TIMEOUT_STOP)) {
            return DRV_API_TIMEOUT;
        }
        return DRV_API_SUCCESS;
    }
    return DRV_API_FAILURE;
}

//nack detect status
#define I2C1_M_NACK_DETECT(detect_func)      \
    do {                                     \
        int err = detect_func();             \
        if (err == DRV_API_SUCCESS) {        \
            return 0;                        \
        } else if (err == DRV_API_TIMEOUT) { \
            return DRV_API_TIMEOUT;          \
        }                                    \
    } while (0)

/**
 *  @brief      This function serves to write a packet of data to the specified address of slave device
 *  @param[in]  id      - to set the slave ID.
 *  @param[in]  data_buf - the first SRAM buffer address to write data to slave in.
 *  @param[in]  data_len - the length of data master write to slave.
 *  @return     0:During sending id or data, the master receives the nack returned by the slave, and stops sending.
 *                - check whether the id is correct;
 *                - if sometimes ack and sometimes nack, check the io driver capability, and use the oscilloscope to check compliance with the i2c spec;
 *                - check whether the slave is abnormal;
 *              1:Master sent data successfully.
 *              DRV_API_TIMEOUT: timeout return(solution refer to the note for i2c1_m_set_error_timeout);
 */
unsigned char i2c1_m_master_write(unsigned char id, unsigned char *data_buf, unsigned int data_len)
{
    //start + id
    reg_i2c1_m_id   = (id & (~FLD_I2C1_M_WRITE_READ_BIT)); //BIT(0):R:High  W:Low
    reg_i2c1_m_ctrl = (FLD_I2C1_M_LS_ID | FLD_I2C1_M_LS_START);
    if (I2C1_M_WAIT(I2C1_M_API_ERROR_TIMEOUT_ID)) {
        return DRV_API_TIMEOUT;
    }
    I2C1_M_NACK_DETECT(i2c1_m_nack_detect);
    //write data
    unsigned int cnt = 0;
    for (cnt = 0; cnt < data_len; cnt++) {
        reg_i2c1_m_dr   = data_buf[cnt];
        reg_i2c1_m_ctrl = FLD_I2C1_M_LS_DATAR; //launch data read cycle
        if (I2C1_M_WAIT(I2C1_M_API_ERROR_TIMEOUT_WRITE_DATA)) {
            return DRV_API_TIMEOUT;
        }
        I2C1_M_NACK_DETECT(i2c1_m_nack_detect);
    }
    //stop
    reg_i2c1_m_ctrl = g_i2c1_m_stop_en; //launch stop cycle
    if (I2C1_M_WAIT(I2C1_M_API_ERROR_TIMEOUT_STOP)) {
        return DRV_API_TIMEOUT;
    }
    return 1;
}

/**
 * @brief      This function serves to read a packet of data.
 * @param[in]  id      - to set the slave ID.
 * @param[in]  data_buf - the first address of SRAM buffer master store data in.
 * @param[in]  data_len - the length of data master read from slave.
 * @return     0:During sending id, the master receives the nack returned by the slave, and stops sending.
 *             - check whether the id is correct;
 *             - if sometimes ack and sometimes nack, check the io driver capability, and use the oscilloscope to check compliance with the i2c spec;
 *             - check whether the slave is abnormal;
 *             1:Master receive data successfully.
 *             DRV_API_TIMEOUT: timeout return(solution refer to the note for i2c1_m_set_error_timeout);
 */
unsigned char i2c1_m_master_read(unsigned char id, unsigned char *data_buf, unsigned int data_len)
{
    //start + id(Read)
    reg_i2c1_m_id   = id | FLD_I2C1_M_WRITE_READ_BIT; //BIT(0):R:High  W:Low
    reg_i2c1_m_ctrl = (FLD_I2C1_M_LS_ID | FLD_I2C1_M_LS_START);
    if (I2C1_M_WAIT(I2C1_M_API_ERROR_TIMEOUT_ID)) {
        return DRV_API_TIMEOUT;
    }
    I2C1_M_NACK_DETECT(i2c1_m_nack_detect);
    //read data
    unsigned int cnt = 0;
    while (--data_len) {
        reg_i2c1_m_ctrl = (FLD_I2C1_M_LS_DATAR | FLD_I2C1_M_LS_ID_R);
        if (I2C1_M_WAIT(I2C1_M_API_ERROR_TIMEOUT_READ_DATA)) {
            return DRV_API_TIMEOUT;
        }
        data_buf[cnt] = reg_i2c1_m_dr;
        cnt++;
    }
    //when the last byte, master will ACK to slave
    reg_i2c1_m_ctrl = (FLD_I2C1_M_LS_DATAR | FLD_I2C1_M_LS_ID_R | FLD_I2C1_M_LS_ACK);
    if (I2C1_M_WAIT(I2C1_M_API_ERROR_TIMEOUT_READ_DATA)) {
        return DRV_API_TIMEOUT;
    }
    data_buf[cnt] = reg_i2c1_m_dr;
    //launch stop cycle
    reg_i2c1_m_ctrl = g_i2c1_m_stop_en;
    if (I2C1_M_WAIT(I2C1_M_API_ERROR_TIMEOUT_STOP)) {
        return DRV_API_TIMEOUT;
    }
    return 1;
}

/**
 * @brief      This function serves to read a packet of data from the specified address of slave device.
 *             the master between writing and reading, it will be a restart signal,and after reading and writing, a stop signal is sent,
 *             but the stop configure,stays in its original state,when a nack exception signal is received, during exception handling, a stop signal will be sent.
 * @param[in]  id      - to set the slave ID.
 * @param[in]  wr_data - specifies the SRAM address of the slave to send data.
 * @param[in]  wr_len - the length of register. enum 0 or 1 or 2 or 3 based on the demand of i2c slave.
 * @param[in]  rd_data - the first address of SRAM buffer master store data in.
 * @param[in]  rd_len - the length of data master read from slave.
 * @return     0:During sending id+address, the master receives the nack returned by the slave, and stops sending.
 *             - check whether the id is correct;
 *             - if sometimes ack and sometimes nack, check the io driver capability, and use the oscilloscope to check compliance with the i2c spec;
 *             - check whether the slave is abnormal;
 *             1:Master receive data successfully.
 *             DRV_API_TIMEOUT: timeout return(solution refer to the note for i2c1_m_set_error_timeout);
 */
unsigned char i2c1_m_master_write_read(unsigned char id, unsigned char *wr_data, unsigned int wr_len, unsigned char *rd_data, unsigned int rd_len)
{
    //set i2c master write.
    if (!i2c1_m_master_write(id, wr_data, wr_len)) {
        return 0;
    }
    //i2c_master_write_read: the master between writing and reading,it will be a restart signal,and after reading and writing, a stop signal is sent,
    //in order to after write and read, a stop signal is sent,so need to enable stop during read.
    unsigned char i2c1_m_stop_en = g_i2c1_m_stop_en;
    if (0x00 == i2c1_m_stop_en) {
        i2c1_m_master_send_stop(1);
    }
    //set i2c master read.
    if (!i2c1_m_master_read(id, rd_data, rd_len)) {
        if (0x00 == i2c1_m_stop_en) {
            i2c1_m_master_send_stop(0);
        }
        return 0;
    }
    //since the configuration state of stop is changed in this interface,
    //the previous configuration needs to be restored either after the function reads or when an exception occurs during the read process.
    if (0x00 == i2c1_m_stop_en) {
        i2c1_m_master_send_stop(0);
    }
    return 1;
}
