/********************************************************************************************************
 * @file    analog.c
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
#include "lib/include/plic.h"
#include "analog.h"
#include "compiler.h"
#include "stimer.h"
/**********************************************************************************************************************
 *                                            local constants                                                       *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                              local macro                                                        *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                             local data type                                                     *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                              global variable                                                       *
 *********************************************************************************************************************/
#if (ANALOG_DMA == 1)
dma_config_t analog_tx_dma_config = {
    .dst_req_sel    = DMA_REQ_ALGM_TX,    /* < tx req  */
    .src_req_sel    = 0,
    .dst_addr_ctrl  = DMA_ADDR_FIX,
    .src_addr_ctrl  = DMA_ADDR_INCREMENT, /* < increment */
    .dstmode        = DMA_HANDSHAKE_MODE, /* < handshake */
    .srcmode        = DMA_NORMAL_MODE,
    .dstwidth       = DMA_CTR_WORD_WIDTH, /* < must word */
    .srcwidth       = DMA_CTR_WORD_WIDTH, /* < must word */
    .src_burst_size = 0,                  /* < must 0 */
    .read_num_en    = 0,
    .priority       = 0,
    .write_num_en   = 0,
    .auto_en        = 0, /* < must 0 */
};
dma_config_t analog_rx_dma_config = {
    .dst_req_sel    = 0, //tx req
    .src_req_sel    = DMA_REQ_ALGM_RX,
    .dst_addr_ctrl  = DMA_ADDR_INCREMENT,
    .src_addr_ctrl  = DMA_ADDR_FIX,
    .dstmode        = DMA_NORMAL_MODE,
    .srcmode        = DMA_HANDSHAKE_MODE,
    .dstwidth       = DMA_CTR_WORD_WIDTH, /* < must word */
    .srcwidth       = DMA_CTR_WORD_WIDTH, /* < must word */
    .src_burst_size = 0,
    .read_num_en    = 0,
    .priority       = 0,
    .write_num_en   = 0,
    .auto_en        = 0, //must 0
};
#endif
/**********************************************************************************************************************
 *                                              local variable                                                     *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *                                          local function prototype                                               *
 *********************************************************************************************************************/


/**
 * @brief      This function serves to judge whether analog write/read is busy .
 * @return     none.
 */
static _always_inline void analog_wait(void)
{
    while (reg_ana_ctrl & FLD_ANA_BUSY) {
    }
}

/**
 * @brief      This function serves to judge whether analog Tx buffer is empty.
 * @return     none.
 */
static _always_inline void analog_wait_txbuf_no_empty(void)
{
    /**
        Because the logic of the current chip hardware to write the first data is:
        regardless of whether there is data in the FIFO, the write operation will be performed immediately.
        In order to prevent incorrect data from being emitted due to the slow filling of the FIFO,
        we need to determine whether the FIFO is non-empty before triggering the write action.
    */
    while (!(reg_ana_buf_cnt & FLD_ANA_TX_BUFCNT))
        ;
}

/**********************************************************************************************************************
 *                                         global function implementation                                             *
 *********************************************************************************************************************/


/**
 * @brief      This function serves to analog register read by byte.
 * @param[in]  addr - address need to be read.
 * @return     the result of read.
 */
_attribute_ram_code_sec_noinline_ unsigned char analog_read_reg8(unsigned char addr)
{
    unsigned int r = core_interrupt_disable();
    reg_ana_addr   = addr;
    reg_ana_len    = 0x1;
    reg_ana_ctrl   = FLD_ANA_CYC;
    analog_wait();
    unsigned char data = reg_ana_data(0);
    core_restore_interrupt(r);
    return data;
}

/**
 * @brief      This function serves to analog register write by byte.
 * @param[in]  addr - address need to be write.
 * @param[in]  data - the value need to be write.
 * @return     none.
 */
_attribute_ram_code_sec_noinline_ void analog_write_reg8(unsigned char addr, unsigned char data)
{
    unsigned int r  = core_interrupt_disable();
    reg_ana_addr    = addr;
    reg_ana_data(0) = data;
    analog_wait_txbuf_no_empty();
    reg_ana_ctrl = (FLD_ANA_CYC | FLD_ANA_RW);
    analog_wait();
    reg_ana_ctrl = 0x00;
    core_restore_interrupt(r);
}

/**
 * @brief      This function serves to analog register write by halfword.
 * @param[in]  addr - address need to be write.
 * @param[in]  data - the value need to be write.
 * @return     none.
 */
_attribute_ram_code_sec_noinline_ void analog_write_reg16(unsigned char addr, unsigned short data)
{
    unsigned int r      = core_interrupt_disable();
    reg_ana_addr        = addr;
    reg_ana_addr_data16 = data;
    analog_wait_txbuf_no_empty();
    reg_ana_ctrl = (FLD_ANA_CYC | FLD_ANA_RW);
    analog_wait();
    reg_ana_ctrl = 0x00;
    core_restore_interrupt(r);
}

/**
 * @brief      This function serves to analog register read by halfword.
 * @param[in]  addr - address need to be read.
 * @return     the result of read.
 */
_attribute_ram_code_sec_noinline_ unsigned short analog_read_reg16(unsigned char addr)
{
    unsigned int r = core_interrupt_disable();
    reg_ana_len    = 2;
    reg_ana_addr   = addr;
    reg_ana_ctrl   = FLD_ANA_CYC;
    analog_wait();
    unsigned short data = reg_ana_addr_data16;
    core_restore_interrupt(r);
    return data;
}

/**
 * @brief      This function serves to analog register read by word.
 * @param[in]  addr - address need to be read.
 * @return     the result of read.
 */
_attribute_ram_code_sec_noinline_ unsigned int analog_read_reg32(unsigned char addr)
{
    unsigned int r = core_interrupt_disable();
    reg_ana_len    = 4;
    reg_ana_addr   = addr;
    reg_ana_ctrl   = FLD_ANA_CYC;
    analog_wait();
    unsigned int data = reg_ana_addr_data32;
    core_restore_interrupt(r);
    return data;
}

/**
 * @brief      This function serves to analog register write by word.
 * @param[in]  addr - address need to be write.
 * @param[in]  data - the value need to be write.
 * @return     none.
 */
_attribute_ram_code_sec_noinline_ void analog_write_reg32(unsigned char addr, unsigned int data)
{
    unsigned int r      = core_interrupt_disable();
    reg_ana_addr        = addr;
    reg_ana_addr_data32 = data;
    analog_wait_txbuf_no_empty();
    reg_ana_ctrl = (FLD_ANA_CYC | FLD_ANA_RW);
    analog_wait();
    reg_ana_ctrl = 0x00;
    core_restore_interrupt(r);
}

#if (ANALOG_DMA == 1)
/**
 * @brief      This function serves to analog register write by word using dma.
 * @param[in]  chn - the dma channel.
 * @param[in]  addr - address need to be read.
 * @param[in]  pdat - the value need to be read.
 * @return     none.
 */
void analog_read_reg32_dma(dma_chn_e chn, unsigned char addr, void *pdat)
{
    unsigned int r = core_interrupt_disable();
    reg_ana_len    = 0x04;
    reg_ana_addr   = addr;
    reg_ana_ctrl   = FLD_ANA_CYC;
    dma_set_address(chn, 0x80140184, (unsigned int)pdat);
    dma_set_size(chn, 4, DMA_WORD_WIDTH);
    dma_config(chn, &analog_rx_dma_config);
    dma_chn_en(chn);
    analog_wait();
    core_restore_interrupt(r);
}

/**
 * @brief      This function serves to analog register write by word using dma.
 * @param[in]  chn - the dma channel.
 * @param[in]  addr - address need to be write.
 * @param[in]  data - the value need to be write.
 * @return     none.
 */
void analog_write_reg32_dma(dma_chn_e chn, unsigned char addr, void *pdat)
{
    unsigned int r = core_interrupt_disable();
    reg_ana_addr   = addr;
    dma_set_address(chn, (unsigned int)pdat, 0x80140184);
    dma_set_size(chn, 4, DMA_WORD_WIDTH);
    dma_config(chn, &analog_tx_dma_config);
    dma_chn_en(chn);
    analog_wait_txbuf_no_empty();
    reg_ana_ctrl = FLD_ANA_CYC | FLD_ANA_RW;
    analog_wait();
    reg_ana_ctrl = 0x00;
    core_restore_interrupt(r);
}
#endif

/**
 * @brief      This function write buffer to analog register.
 * @param[in]  addr - address need to be write.
 * @param[in]  *buff - the buffer need to be write.
 * @param[in]  len - the length of buffer.(The data length cannot be greater than 8)
 * @return     none.
 */
_attribute_ram_code_sec_noinline_ void analog_write_buff(unsigned char addr, unsigned char *buff, unsigned char len)
{
    if (len > 8) {
        return;
    }

    unsigned int r = core_interrupt_disable();

    //The length of the written data cannot exceed 8 bytes, because a maximum of 8 bytes of data can be stored at a time.
    //If the data exceeds 8 bytes, you need to wait for the previous data to be sent. Otherwise, data overwrite may occur.
    //If you determine whether 8 bytes are full each time you write data, the process of writing data may be interrupted.
    //Because the process of writing to the analog register cannot be interrupted,if it is interrupted,
    //the write operation will stop and subsequent data will not be written.So you also need to close the interrupt,
    //and you also need to put it in the ram code.(add by weihua.zhang, confirmed by xuqiang.zhang 20240111)
    reg_ana_addr    = addr;
    reg_ana_data(0) = *(buff++);
    analog_wait_txbuf_no_empty();
    reg_ana_ctrl = (FLD_ANA_CYC | FLD_ANA_RW);
    for (unsigned int i = 1; i < len; i++) {
        reg_ana_data(i % 4) = *(buff++);
    }

    analog_wait();
    reg_ana_ctrl = 0x00;
    core_restore_interrupt(r);
}

/**
 * @brief      This function read data from analog register to buffer.
 * @param[in]  addr - address need to be read from.
 * @param[in]  *buff - the buffer need to be put data.
 * @param[in]  len - the length of read data.
 * @return     none.
 */
_attribute_ram_code_sec_noinline_ void analog_read_buff(unsigned char addr, unsigned char *buff, unsigned char len)
{
    unsigned int r = core_interrupt_disable();

    reg_ana_len  = len;
    reg_ana_addr = addr;
    reg_ana_ctrl = FLD_ANA_CYC;
    for (unsigned int i = 0; i < len; i++) {
        while (!(reg_ana_buf_cnt & FLD_ANA_RX_BUFCNT))
            ;
        *(buff++) = reg_ana_data(i % 4);
    }

    analog_wait();
    reg_ana_ctrl = 0x00;
    core_restore_interrupt(r);
}

#if (ANALOG_DMA == 1)
/**
 * @brief      This function write buffer to analog register by dma channel.
 * @param[in]  chn - the dma channel.
 * @param[in]  addr - address need to be write.
 * @param[in]  *pdat - the buffer need to be write.
 * @param[in]  len - the length of buffer.
 * @return     none.
 */
void analog_write_buff_dma(dma_chn_e chn, unsigned char addr, unsigned char *pdat, unsigned int len)
{
    unsigned int r = core_interrupt_disable();
    reg_ana_addr   = addr;
    dma_set_address(chn, (unsigned int)pdat, 0x80140184);
    dma_set_size(chn, len, DMA_WORD_WIDTH);
    dma_config(chn, &analog_tx_dma_config);
    dma_chn_en(chn);
    analog_wait_txbuf_no_empty();
    reg_ana_ctrl = 0x60;
    analog_wait();
    reg_ana_ctrl = 0x00;
    core_restore_interrupt(r);
}

/**
 * @brief      This function write buffer to analog register by dma channel.
 * @param[in]  chn - the dma channel.
 * @param[in]  addr - address need to be read from.
 * @param[in]  *pdat - the buffer need to be put data.
 *             note: The size of array pdat must be a multiple of 4.
 *                  For example, if you just need read 5 byte by dma, you should
 *                  define the size of array pdat to be greater than 8 other than 5.
 *                  Because the dma would return 4 byte data every time, 5 byte is
 *                  not enough to store them.
 * @param[in]  len - the length of read data.
 * @return     none.
 */
void analog_read_buff_dma(dma_chn_e chn, unsigned char addr, unsigned char *pdat, unsigned int len)
{
    unsigned int r = core_interrupt_disable();
    reg_ana_len    = len;
    reg_ana_addr   = addr;
    dma_set_address(chn, 0x80140184, (unsigned int)pdat);
    dma_set_size(chn, len, DMA_WORD_WIDTH);
    dma_config(chn, &analog_rx_dma_config);
    dma_chn_en(chn);
    reg_ana_ctrl = FLD_ANA_CYC;
    analog_wait();
    core_restore_interrupt(r);
}

/**
 * @brief      This function write buffer to analog register by dma channel.
 * @param[in]  chn  - the dma channel.
 * @param[in]  pdat - the buffer(addr & data) ptr need to be write,
 *             note: The array pdat should look like this,
 *             |  pdat     |            |        |
 *             |  :------  | :----------|  :---- |
 *             |  pdat[0]  |   address  |  0x3a  |
 *             |  pdat[1]  |    data    |  0x11  |
 *             |  pdat[2]  |   address  |  0x3b  |
 *             |  pdat[3]  |    data    |  0x22  |
 *             |  ......   |            |        |
 *              It means write data 0x11 to address 0x3a,
 *                       write data 0x22 to address 0x3b,
 *                       ......
 * @param[in]  len - the length of read data.
 * @return     none.
 */
void analog_write_addr_data_dma(dma_chn_e chn, void *pdat, int len)
{
    unsigned int r = core_interrupt_disable();
    dma_set_address(chn, (unsigned int)pdat, 0x80140184);
    dma_set_size(chn, len, DMA_WORD_WIDTH);
    dma_config(chn, &analog_tx_dma_config);
    dma_chn_en(chn);
    analog_wait_txbuf_no_empty();
    reg_ana_ctrl    = FLD_ANA_RW;
    reg_ana_dma_ctl = FLD_ANA_CYC1 | FLD_ANA_DMA_EN;
    /**
        After data is written to the analog register, the TX_DONE flag will become 0 after several cycles.
        During these cycles, if you read the TX_DONE state,
        it will be mistakenly judged that the write is complete, so a delay is required.
    */
    delay_us(1);
    while (!(reg_ana_sta & BIT(3)))
        ;
    reg_ana_ctrl    = 0x00;
    reg_ana_dma_ctl = 0;
    core_restore_interrupt(r);
}
#endif
