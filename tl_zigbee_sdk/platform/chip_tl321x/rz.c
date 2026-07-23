/********************************************************************************************************
 * @file    rz.c
 *
 * @brief   This is the source file for TL321X
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
#include "rz.h"

/**
 * @brief      This setting serve to set the configuration of rz dma.
 */
dma_config_t rz_dma_config={
  .dst_req_sel    = DMA_REQ_RZ_TX,        /*DstReqSel   :7:2   */
  .src_req_sel    = 0,                    /*SrcReqSel   :13:8 */
  .dst_addr_ctrl  = DMA_ADDR_FIX,         /*DstAddrCtrl :15:14  0:increment address 1: decrement address 2: fixed address */
  .src_addr_ctrl  = DMA_ADDR_INCREMENT,   /*SrcAddrCtrl :17:16  0:increment address 1: decrement address 2: fixed address */
  .dstmode        = DMA_HANDSHAKE_MODE,   /*DstMode:18   0 normal mode  1 handshake*/
  .srcmode        = DMA_NORMAL_MODE,      /*SrcMode :19   0 normal mode  1 handshake*/
  .dstwidth       = DMA_CTR_WORD_WIDTH,   /*DstWidth :21:20 00:byte 01:hword 02:word*/
  .srcwidth       = DMA_CTR_WORD_WIDTH,   /*SrcWidth :23:22  00:byte 01:hword 02:word*/
  .src_burst_size = DMA_BURST_1_WORD,     /*SrcBurstSize: 26:24*/
  .vacant_bit     = 0,
  .read_num_en    = 0,
  .priority       = 0,
  .write_num_en   = 0,                    /*wnum_en : 30*/
  .auto_en        = 0,                    /*auto_en : 31*/
};


/**
 * @brief     This function servers to configure DMA channel and some configures.
 * @param[in] chn - to select the DMA channel.
 * @return    none
 */
void rz_set_dma_config(dma_chn_e chn)
{
  dma_config(chn,&rz_dma_config);
}

/**
 * @brief     This  function serves to set rz dma.
 * @param[in] rz_dma_chn - dma channel.
 * @param[in] src_addr   - the address of source.
 * @param[in] size_byte  - the length of data.
 * @return    none
 * @note      src_addr : must be aligned by word (4 bytes), otherwise the program will enter an exception
 */
void rz_set_dma(dma_chn_e rz_dma_chn, unsigned int *src_addr,unsigned int size_byte )
{
  dma_set_size(rz_dma_chn, size_byte, DMA_WORD_WIDTH);
  dma_set_address(rz_dma_chn, (unsigned int) src_addr, reg_rz_tx_data_addr);
  dma_chn_en(rz_dma_chn);
}

/**
 * @brief     This function servers to enable/disable jitter on t0L/t1L.
 * @param[in] jitter_ctrl_tl - jitter mode.
 * @return    none.
 */
void rz_t0l_t1l_jitter_config(rz_jitter_ctrl_tl_e jitter_ctrl_tl) {
    if (ENABLE_JITTER_ON_T0L_T1L == jitter_ctrl_tl) {
        reg_rz_ctrl1 |= FLD_RZ_JITTER_L_EN;
    } else if (DISABLE_JITTER_ON_T0L_T1L == jitter_ctrl_tl) {
        reg_rz_ctrl1 &= ~FLD_RZ_JITTER_L_EN;
    }
}

/**
 * @brief     This function servers to enable/disable jitter on t0H/t1H.
 * @param[in] jitter_ctrl_th - jitter mode.
 * @return    none.
 */
void rz_t0h_t1h_jitter_config(rz_jitter_ctrl_th_e jitter_ctrl_th) {
    if (ENABLE_JITTER_ON_T0H_T1H == jitter_ctrl_th) {
        reg_rz_ctrl1 |= FLD_RZ_JITTER_H_EN;
    } else if (DISABLE_JITTER_ON_T0H_T1H == jitter_ctrl_th) {
        reg_rz_ctrl1 &= ~FLD_RZ_JITTER_H_EN;
    }
}

/**
 * @brief      This function configures rz control register.
 * @param[in]  rz_tx_config - the prt of rz config that configured control register
 * @return     none
 */
void rz_set_tx_config(rz_tx_config_t *rz_tx_config) {

    BM_SET(reg_rst6, FLD_RST6_RZ);
    BM_SET(reg_clk_en6, FLD_CLK6_RZ_EN);

    rz_set_polarity(rz_tx_config->pola);
    rz_set_msb_lsb_mode(rz_tx_config->bit_msb);
    rz_set_big_little_endian_mode(rz_tx_config->big_endian_mode);
    rz_set_global_data_mode(rz_tx_config->global_data_mode);
    rz_set_data_align_mode(rz_tx_config->align_32bits_mode);
    rz_set_tx_data_mode(rz_tx_config->addr_mode);
    rz_set_fifo_lvl(3);

    rz_t0l_t1l_jitter_config(rz_tx_config->jitter_l_en);
    rz_t0h_t1h_jitter_config(rz_tx_config->jitter_h_en);
    rz_jitter_range_config(rz_tx_config->jitter_range);

    rz_clr_txfifo_auto();
}
