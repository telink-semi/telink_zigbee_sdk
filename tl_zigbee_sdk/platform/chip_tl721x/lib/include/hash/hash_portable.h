/********************************************************************************************************
 * @file    hash_portable.h
 *
 * @brief   This is the header file for TL721X
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
#ifndef HASH_PORTABLE_H
#define HASH_PORTABLE_H
#include "driver.h"


#ifdef __cplusplus
extern "C"
{
#endif

    /**
 * @brief       get hash tx dma channel.
 * @return      hash tx dma channel
 */
    dma_chn_e hash_get_tx_dma_channel(void);


    /**
 * @brief       get hash rx dma channel.
 * @return      hash rx dma channel
 */
    dma_chn_e hash_get_rx_dma_channel(void);


    /**
 * @brief       set hash tx dma channel.
 * @param[in]   chn  - hash tx dma channel.
 * @return      none
 */
    void hash_set_tx_dma_channel(dma_chn_e chn);

    /**
 * @brief       set hash rx dma channel.
 * @param[in]   chn  - hash rx dma channel.
 * @return      none
 */
    void hash_set_rx_dma_channel(dma_chn_e chn);

    /**
 * @brief       configure the destination address and data length for hash tx dma.
 * @param[in]   chn              - hash tx dma channel.
 * @param[in]   buf_addr         - destination address.
 * @param[in]   len              - data length.
 * @return      none
 * @note
 */
    void hash_tx_dma(dma_chn_e chn, unsigned int buf_addr, unsigned int len);

    /**
 * @brief       configure the receiving  address and data length for hash rx dma.
 * @param[in]   chn              - hash rx dma channel.
 * @param[in]   buf_addr         - receiving address.
 * @param[in]   len              - data length.
 * @return      none
 * @note
 */
    void hash_rx_dma(dma_chn_e chn, unsigned int buf_addr, unsigned int len);

    /**
 * @brief Sets tx dma channel for hash DMA transfers.
 * @param[in]  tx_chn - The DMA channel to be used for transmit.
 * @return      none
 */
    void hash_set_tx_dma_config(dma_chn_e tx_chn);

    /**
 * @brief Sets rx dma channel for hash DMA transfers.
 * @param[in]  rx_chn     - The DMA channel to be used for transmit.
 * @param[in]  burst_size - The burst size to be set.
 * @return     none
 */
    void hash_set_rx_dma_config(dma_chn_e rx_chn, dma_burst_size_e burst_size);

    /**
  * @brief Initialize HASH-related generic configurations.
  * @note       Only after calling this function can other HASH related functions be called.
  *             Otherwise, other HASH function settings will not take effect.
  * @return None.
  *
  */
    void hash_dig_en(void);

    /**
 * @brief     Resets HASH module,before using HASH, it is needed to call hash_reset() to avoid affecting the use of HASH.
 * @return    none
 */
    static inline void hash_reset(void)
    {
        reg_rst4 &= ~FLD_RST4_HASH;
        reg_rst4 |= FLD_RST4_HASH;
    }

    /**
 * @brief     Enable the clock of HASH module.
 * @return    none
 */
    static inline void hash_clk_en(void)
    {
        reg_clk_en4 |= FLD_CLK4_HASH_EN;
    }

#ifdef __cplusplus
}
#endif

#endif
