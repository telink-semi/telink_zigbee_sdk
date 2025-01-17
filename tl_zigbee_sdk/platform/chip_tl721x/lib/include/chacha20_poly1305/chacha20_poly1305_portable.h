/********************************************************************************************************
 * @file    chacha20_poly1305_portable.h
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
#ifndef CHACHA20_POLY1305_PORTABLE_H
#define CHACHA20_POLY1305_PORTABLE_H
#include "driver.h"


#ifdef __cplusplus
extern "C"
{
#endif

    /**
 * @brief       get chacha20_poly1305 tx dma channel.
 * @return      chacha20_poly1305 tx dma channel
 */
    dma_chn_e chacha20_poly1305_get_tx_dma_channel(void);

    /**
 * @brief       get chacha20_poly1305 rx dma channel.
 * @return      chacha20_poly1305 rx dma channel
 */
    dma_chn_e chacha20_poly1305_get_rx_dma_channel(void);

    /**
 * @brief       set chacha20_poly1305 tx dma channel.
 * @param[in]   chn  - chacha20_poly1305 tx dma channel.
 * @return      none
 */
    void chacha20_poly1305_set_tx_dma_channel(dma_chn_e chn);

    /**
 * @brief       set chacha20_poly1305 rx dma channel.
 * @param[in]   chn  - chacha20_poly1305 rx dma channel.
 * @return      none
 */
    void chacha20_poly1305_set_rx_dma_channel(dma_chn_e chn);

    /**
 * @brief configure the dma channel destination address and data length for chacha20 tx dma.
 * @param[in]   chn              - dma channel
 * @param[in]   buf_addr         - destination address.
 * @param[in]   len              - data length.
 * @return      none
 * @note
 */
    void chacha20_poly1305_tx_dma(dma_chn_e chn, unsigned int buf_addr, unsigned int len);

    /**
 * @brief configure the dma channel destination address and data length for chacha20_poly1305 rx dma.
 * @param[in]   chn              - dma channel
 * @param[in]   buf_addr         - receiving address.
 * @param[in]   len              - data length.
 * @return      none
 * @note
 */
    void chacha20_poly1305_rx_dma(dma_chn_e chn, unsigned int buf_addr, unsigned int len);

    /**
 * @brief Sets tx dma channel for chacha20_poly1305 DMA transfers.
 * @param[in]   tx_chn - The DMA channel to be used for transmit.
 * @return      none
 */
    void chacha20_poly1305_set_tx_dma_config(dma_chn_e tx_chn);

    /**
 * @brief Sets rx dma channel for chacha20_poly1305 DMA transfers.
 * @param[in]  rx_chn     - The DMA channel to be used for transmit.
 * @param[in]  burst_size - The burst size to be set.
 * @return     none
 */
    void chacha20_poly1305_set_rx_dma_config(dma_chn_e rx_chn, dma_burst_size_e burst_size);

    /**
 * @brief Initialize CHACHA20_POLY1305-related generic configurations.
 * @note        Only after calling this function can other CHACHA20_POLY1305 related functions be called.
 *              Otherwise, other CHACHA20_POLY1305 function settings will not take effect.
 * @return None.
 */
    void chacha20_poly1305_dig_en(void);

    /**
 * @brief     Resets CHACHA20_POLY1305 module,before using CHACHA20_POLY1305, it is needed to call chacha20_poly1305_reset() to avoid affecting the use of CHACHA20_POLY1305.
 * @return    none
 */
    static inline void chacha20_poly1305_reset(void)
    {
        reg_rst5 &= ~FLD_RST5_CHACHA20;
        reg_rst5 |= FLD_RST5_CHACHA20;
    }

    /**
 * @brief     Enable the clock of CHACHA20_POLY1305 module.
 * @return    none
 */
    static inline void chacha20_poly1305_clk_en(void)
    {
        reg_clk_en5 |= FLD_CLK5_CHACHA20_EN;
    }

#ifdef __cplusplus
}
#endif

#endif
