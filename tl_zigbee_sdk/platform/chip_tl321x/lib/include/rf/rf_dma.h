/********************************************************************************************************
 * @file    rf_dma.h
 *
 * @brief   This is the header file for TL321X
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
#ifndef _RF_DMA_H_
#define _RF_DMA_H_

#include "reg_include/rf_reg.h"
#include "dma.h"

/*********************************************************************************************************************
 *                                         RF DMA global constants                                                   *
 *********************************************************************************************************************/

/**
 * @brief Supports up to 2 DMA channels.
 */
typedef enum
{
    RF_TX_DMA = 0,
    RF_RX_DMA,
} rf_dma_chn_e;

/**
 * @brief Define the peripherals of RF DMA transfer width.
 * @note <p> DMA supports the following three types of transfer bytes, For Peripheral-related DMA transfer, transfer width is only supported for DMA_WORD_WIDTH.
 *       <p> If the destination address and source address of DMA are both SRAM, either one can be chosen.
 */
typedef enum
{
    RF_DMA_BYTE_WIDTH  = 1, /**< data is transmitted in bytes. */
    RF_DMA_HWORD_WIDTH = 2, /**< data is transmitted in half word. */
    RF_DMA_WORD_WIDTH  = 4, /**< data is transmitted in word. */
} rf_dma_transfer_width_e;

/**
 * @brief Define RF DMA control struct.
 * -# dst_req_sel: if dstmode is DMA_HANDSHAKE_MODE, dst_req_sel is set to the value defined in dma_req_sel_e(peripherals tx),otherwise dstmode is DMA_NORMAL_MODE and does not need to be set,just write 0.
 * -# src_req_sel: if srcmode is DMA_HANDSHAKE_MODE, src_req_sel is set to the value defined in dma_req_sel_e(peripherals rx),otherwise srcmode is DMA_NORMAL_MODE and does not need to be set,just write 0.
 * -# dst_addr_ctrl: see dma_addr_ctrl_e.
 * -# src_addr_ctrl: see dma_addr_ctrl_e.
 * -# dstmode: see dma_mode_e.
 * -# srcmode: see dma_mode_e.
 * -# dstwidth:see dma_ctr_width_e.
 * -# srcwidth:see dma_ctr_width_e.
 * -# src_burst_size:see dma_burst_size_e.
 * -# read_num_en:0 - need to manually configure reg_dma_size. 1 - The hardware automatically uses the value of the first four bytes of the source address as the transfer length instead of reg_dma_size(Not usually used).
 * -# priority:Generally it is set to 0, because setting to 1 will block the transmission of the other channels set to 0.
 * -# write_num_en:1- When reg_dma_size is set to the maximum value, the hardware automatically writes the length of the received data to the SRAM(4 bytes) before the destination address. 0-Hardware will not write data length to SRAM
 * -# auto_en:Only supports RF_TX_DMA. After auto_enable_en is enabled, RF_TX_DMA is automatically enabled based on the baseband generation requests.
 */
typedef struct
{
    unsigned int                : 4;
    unsigned int dst_req_sel    : 5; /*DstReqSel   :8:4   */
    unsigned int src_req_sel    : 5; /*SrcReqSel   :13:9 */
    unsigned int dst_addr_ctrl  : 2; /*DstAddrCtrl :15:14  */
    unsigned int src_addr_ctrl  : 2; /*SrcAddrCtrl :17:16   */
    unsigned int dstmode        : 1; /*DstMode:18   */
    unsigned int srcmode        : 1; /*SrcMode :19   */
    unsigned int dstwidth       : 2; /*DstWidth :21:20 */
    unsigned int srcwidth       : 2; /*SrcWidth :23:22 */
    unsigned int src_burst_size : 3; /*SrcBurstSize: 26:24*/
    unsigned int vacant_bit     : 1; /*vacant:27*/
    unsigned int read_num_en    : 1; /*Rnum_en :28*/
    unsigned int priority       : 1; /*Pri :29*/
    unsigned int write_num_en   : 1; /*wnum_en : 30*/
    unsigned int auto_en        : 1; /*auto_en : 31*/
} rf_dma_config_t;

/*********************************************************************************************************************
 *                                         RF DMA declaration                                                        *
 *********************************************************************************************************************/

/**
* @brief     This function configures DMA control register.
* @param[in] chn    - dma channel
* @param[in] config - the prt of dma_config that configured control register
* @return    none
* @note      When a certain dma channel has not finished the transmission (bit 0 of reg_dma_ctr0(chn) is 1),need to disable dma before writing to the dma register.
*/
static inline void rf_dma_config(rf_dma_chn_e chn, rf_dma_config_t *config)
{
    reg_bb_dma_ctrl(chn) = (reg_bb_dma_ctrl(chn) & (~BIT_RNG(4, 31))) | (*(unsigned int *)config);
}

/**
* @brief   this function set source address for DMA,
* @param[in]  chn - DMA channel
* @param[in]  src_addr - the address of source.
* @note      When a certain dma channel has not finished the transmission (bit 0 of reg_dma_ctr0(chn) is 1),need to disable dma before writing to the dma register.
**/
static inline void rf_dma_set_src_address(rf_dma_chn_e chn, unsigned int src_addr)
{
    reg_bb_dma_src_addr(chn) = (unsigned int)src_addr;
}

/**
* @brief   this function set destination address for DMA,
* @param[in]  chn - DMA channel
* @param[in]  dst_addr - the address of destination.
* @note       When a certain dma channel has not finished the transmission (bit 0 of reg_dma_ctr0(chn) is 1),need to disable dma before writing to the dma register.
**/
static inline void rf_dma_set_dst_address(rf_dma_chn_e chn, unsigned int dst_addr)
{
    reg_bb_dma_dst_addr(chn) = (unsigned int)dst_addr;
}

/**
* @brief   this  function set  the DMA to tx/rx size byte.
* @param[in] chn - DMA channel
* @param[in] size_byte  - the address of dma tx/rx size .The maximum transmission length of DMA is 0xFFFFFC bytes  and cannot exceed this length.
* @param[in] byte_width -  dma   tx/rx  width
* @return    none
* @note      When a certain dma channel has not finished the transmission (bit 0 of reg_dma_ctr0(chn) is 1),need to disable dma before writing to the dma register.
*/
static inline void rf_dma_set_size(rf_dma_chn_e chn, unsigned int size_byte, rf_dma_transfer_width_e byte_width)
{
    reg_bb_dma_size(chn) = ((size_byte + byte_width - 1) / byte_width) | ((size_byte % byte_width) << 22);
}

/**
 * @brief   This function serves to reset RF DMA.
 * @return  none
 * @note    After calling this interface, it is necessary to reconfigure RF DMA.
 */
static _always_inline void rf_dma_reset(void)
{
    reg_n22_rst0 &= ~(FLD_RST0_DMA_BB); // reset DMA_BB
    reg_n22_rst0 |= FLD_RST0_DMA_BB;    // clear DMA_BB reset
}

/**
 * @brief      This function servers to enable RF DMA that selected channel.
 * @param[in] chn - rf dma channel.
 * @return    none
 */
static inline void rf_dma_chn_en(rf_dma_chn_e chn)
{
    BM_SET(reg_bb_dma_ctr0(chn), FLD_BB_DMA_CHANNEL_ENABLE);
    BM_SET(reg_bb_dma_ctr3(chn), FLD_BB_DMA_AUTO_ENABLE_EN);
}

/**
 * @brief      This function servers to disable RF DMA that selected channel.
 * @param[in] chn - rf dma channel.
 * @return    none
 */
static inline void rf_dma_chn_dis(rf_dma_chn_e chn)
{
    BM_CLR(reg_bb_dma_ctr0(chn), FLD_BB_DMA_CHANNEL_ENABLE);
    BM_CLR(reg_bb_dma_ctr3(chn), FLD_BB_DMA_AUTO_ENABLE_EN);
}

#endif /*_RF_DMA_H_ */
