/********************************************************************************************************
 * @file    dma.c
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
#include "dma.h"
#include "lib/include/core.h"

/**
 * @brief     This interface is used to configure the address contents corresponding to different modules at once through the dma chain table.
 * @param[in] chn               - dma channel.
 * @param[in] dma_chain_config  - dma chain node buff pointer.
 * @param[in] chain_node_cnt    - dma chain node cnt.
 * @return    none
 * @note      1.only support moving from flash to other M2M_DMA_CFG(ram, flash, and registers that do not require handshaking with the dma).
 *            2.If any of them are configured with analog registers, it is required that the first must be an analog register and can send useless data.
 *            3.If there is a write operation to the analog registers, you need to wait until after the dma interrupt and then call
 *              the interface dma_write_reg_is_complete() to ensure that the write operation to the analog registers has been completed.
 */
void dma_write_reg(dma_chn_e chn, const dma_chain_config_t *chain, unsigned char node_cnt)
{
    unsigned int alg_word = 0;
    for (unsigned char i = 0; i < node_cnt; i++) {
        if (chain[i].dma_chain_dst_addr == ((unsigned int)(ANALOG_DATA_REG_ADDR))) {
            alg_word += chain[i].dma_chain_data_len;
        }
    }
    unsigned int dma_cfg = chain[0].dma_chain_ctl;
    reg_ana_len          = alg_word * 4;
    dma_set_address(chn, chain[0].dma_chain_src_addr, chain[0].dma_chain_dst_addr);
    if ((chain[0].dma_chain_ctl & (FLD_DMA_CHANNEL_DST_WIDTH | FLD_DMA_CHANNEL_SRC_WIDTH)) == 0) {
        dma_set_size(chn, chain[0].dma_chain_data_len, DMA_BYTE_WIDTH);
    } else {
        dma_set_size(chn, (chain[0].dma_chain_data_len) * 4, DMA_WORD_WIDTH);
    }
    reg_dma_llp(chn) = chain[0].dma_chain_llp_ptr;
    dma_config(chn, (dma_config_t *)&dma_cfg);
    if (chain[0].dma_chain_dst_addr == ((unsigned int)(ANALOG_DATA_REG_ADDR))) {
        while (!(reg_ana_buf_cnt & FLD_ANA_TX_BUFCNT))
            ;
        reg_ana_ctrl    = FLD_ANA_RW;
        reg_ana_dma_ctl = (reg_ana_dma_ctl&FLD_ANA_DIV_MOD)|FLD_ANA_CYC1 | FLD_ANA_DMA_EN | FLD_ANA_AUTO_RXCLR_EN;
    }
}

void dma_write_reg_is_complete(void)
{
    while (reg_ana_ctrl & FLD_ANA_BUSY) {
    }
    reg_ana_dma_ctl = (reg_ana_dma_ctl&FLD_ANA_DIV_MOD)|FLD_ANA_NDMA_RXDONE_EN | FLD_ANA_AUTO_RXCLR_EN; //ndma_en=0
}

#define DMA_IRQ_STATUS_MAX_CLR_TIMES 3

/**
 * @brief       this function serves to clear all dma irq status.
 * @return      Indicates whether clearing irq status was successful.
 */
drv_api_status_e dma_clr_all_irq_status(void)
{
    unsigned char j = 0;
    for (j = 0; j < DMA_IRQ_STATUS_MAX_CLR_TIMES; j++) {
        reg_dma_tc_isr  = 0xff;
        reg_dma_err_isr = 0xff;
        reg_dma_abt_isr = 0xff;
        if (!(reg_dma_tc_isr || reg_dma_err_isr || reg_dma_abt_isr)) {
            break;
        }
    }
    if (j == DMA_IRQ_STATUS_MAX_CLR_TIMES) {
        return DRV_API_FAILURE;
    } else {
        return DRV_API_SUCCESS;
    }
}

/**
 * @brief       this function serves to wait all dma chn complete.
 * @param       wait_timeout_us - timeout.
 * @return      DRV_API_TIMEOUT : wait timeout;
 *              DRV_API_SUCCESS : all dma complete status is successful;
 */
drv_api_status_e dma_wait_for_all_chn_to_complete(unsigned int timeout_us)
{
    unsigned char      dma_done_flag = 1;
    unsigned long long start         = rdmcycle();
    while (dma_done_flag) {
        if (core_cclk_time_exceed(start, timeout_us)) {
            return DRV_API_TIMEOUT;
        }
        unsigned char i = 0;
        for (i = 0; i < DMA_CNT; i++) {
            if (dma_chn_is_complete(i)) {
                break;
            }
        }
        if (i == DMA_CNT) {
            dma_done_flag = 0;
        }
    }
    return DRV_API_SUCCESS;
}
