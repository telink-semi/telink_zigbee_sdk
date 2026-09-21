/********************************************************************************************************
 * @file    chacha20_poly1305.h
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
#ifndef CHACHA20_POLY1305_H
#define CHACHA20_POLY1305_H


#include "chacha20_poly1305_basic.h"


//open the macro supoort the interleaved computation
#define CONFIG_CHACHA20_POLY1305_SUPPORT_MUL_THREAD


#ifdef __cplusplus
extern "C"
{
#endif


    typedef struct
    {
        unsigned char            cur_tag[17];
        unsigned int             cur_cnt;
        unsigned long long       aad_bytes;
        unsigned long long       payload_bytes;
        CHACHA20_POLY1305_CRYPTO crypto;
#ifdef CONFIG_CHACHA20_POLY1305_SUPPORT_MUL_THREAD
        unsigned char key[32];
        unsigned char iv[8];
        unsigned int  constant;
#endif
    } CHACHA20_POLY1305_CTX;

    typedef struct
    {
        unsigned char            cur_tag[17];
        unsigned int             cur_cnt;
        unsigned long long       aad_bytes;
        unsigned long long       payload_bytes;
        CHACHA20_POLY1305_CRYPTO crypto;
#ifdef CONFIG_CHACHA20_POLY1305_SUPPORT_MUL_THREAD
        unsigned char key[32];
        unsigned char iv[8];
        unsigned int  constant;
#endif
    } CHACHA20_POLY1305_DMA_CTX;

    //APIs
    /**
 * @brief       input message.
 * @param[in]   crypto             - encrypting or decrypting.
 * @param[in]   key                - key in bytes.
 * @param[in]   constant           - constant of nonce.
 * @param[in]   iv                 - iv of nonce.
 * @param[in]   stage              - data interleave stage.
 * @param[in]   dma_en             - for DMA mode(not 0) or not(0).
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. this function is common for CPU/DMA.
  @endverbatim
 */
    unsigned int chacha20_poly1305_init_internal(CHACHA20_POLY1305_CRYPTO crypto, unsigned char key[32], unsigned int constant, unsigned char iv[8], CHACHA20_POLY1305_SEC_STAGE stage, unsigned int dma_en);

    /**
 * @brief       hacha20_poly1305 init (CPU style).
 * @param[in]   ctx             - CHACHA20_POLY1305_CTX context pointer.
 * @param[in]   crypto          - encrypting or decrypting.
 * @param[in]   key             - key in bytes.
 * @param[in]   constant        - constant of nonce.
 * @param[in]   iv              - iv of nonce.
 * @param[in]   aad             - all aad.
 * @param[in]   aad_bytes       - byte length of aad.
 * @return      0:success     other:error
 */
    unsigned int chacha20_poly1305_init(CHACHA20_POLY1305_CTX *ctx, CHACHA20_POLY1305_CRYPTO crypto, unsigned char key[32], unsigned int constant, unsigned char iv[8], unsigned char *aad, unsigned long long aad_bytes);

    /**
 * @brief       chacha20_poly1305 update message excluding the last data (CPU mode).
 * @param[in]   ctx                - CHACHA20_POLY1305_CTX context pointer.
 * @param[in]   payload_in         - payload of some blocks, excluding last block(or message tail).
 * @param[out]  payload_out        - ciphertext or plaintext.
 * @param[in]   payload_bytes      - byte length of payload, must be a multiple of 64, could be 0.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. if the whole payload length is 0, this case is supported. in this case, payload_in and
        payload_out could set NULL.
  @endverbatim
 */
    unsigned int chacha20_poly1305_update_excluding_last_data(CHACHA20_POLY1305_CTX *ctx, unsigned char *payload_in, unsigned char *payload_out, unsigned long long payload_bytes);

    /**
 * @brief       chacha20_poly1305 update message including the last data (CPU mode), and get the tag.
 * @param[in]   ctx                - CHACHA20_POLY1305_CTX context pointer.
 * @param[in]   payload_in         - payload, including last block(or message tail).
 * @param[in]   payload_out        - ciphertext or plaintext.
 * @param[in]   payload_bytes      - byte length of payload.
 * @param[out]  tag                - chacha20_poly1305 tag, occupies 16 bytes.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. if the all payload length is 0, this case is supported. in this case, payload_in and
 *        payload_out could set NULL and payload_bytes should be 0.
  @endverbatim
 */
    unsigned int chacha20_poly1305_update_including_last_data(CHACHA20_POLY1305_CTX *ctx, unsigned char *payload_in, unsigned char *payload_out, unsigned long long payload_bytes, unsigned char tag[16]);

    /**
 * @brief       chacha20_poly1305 (CPU style, one-off style).
 * @param[in]   crypto             - encrypting or decrypting.
 * @param[in]   key                - key in bytes.
 * @param[in]   constant           - constant of nonce.
 * @param[in]   iv                 - iv of nonce.
 * @param[in]   aad                - key in bytes.
 * @param[in]   aad_bytes          - byte length of aad.
 * @param[in]   payload_in         - payload of some blocks, excluding last block(or message tail).
 * @param[out]  payload_out        - ciphertext or plaintext.
 * @param[in]   payload_bytes      - byte length of payload, could be 0.
 * @param[out]  tag                - chacha20_poly1305 tag, occupies 16 bytes.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. aad_bytes and payload_bytes could be any value. if length bytes is 0, could set pointer is NULL.
  @endverbatim
 */
    unsigned int chacha20_poly1305(CHACHA20_POLY1305_CRYPTO crypto, unsigned char key[32], unsigned int constant, unsigned char iv[8], unsigned char *aad, unsigned long long aad_bytes, unsigned char *payload_in, unsigned char *payload_out, unsigned long long payload_bytes, unsigned char tag[16]);


#ifdef CHACHA20_POLY1305_DMA_FUNCTION
    /**
 * @brief       chacha20_poly1305 init (DMA style).
 * @param[in]   ctx                - CHACHA20_POLY1305_DMA_CTX context pointer.
 * @param[in]   crypto             - encrypting or decrypting.
 * @param[in]   key                - key in bytes.
 * @param[in]   constant           - constant of nonce.
 * @param[in]   iv                 - iv of nonce.
 * @param[in]   aad                - key in bytes.
 * @param[in]   aad_bytes          - byte length of aad.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. in dma mode, the byte length of aad occupy 32 bit.
  @endverbatim
 */
    unsigned int chacha20_poly1305_dma_init(CHACHA20_POLY1305_DMA_CTX *ctx, CHACHA20_POLY1305_CRYPTO crypto, unsigned char key[32], unsigned int constant, unsigned char iv[8], unsigned int *aad, unsigned int aad_bytes);

    /**
 * @brief       chacha20_poly1305 update message excluding the last data (DMA mode).
 * @param[in]   ctx                - CHACHA20_POLY1305_DMA_CTX context pointer.
 * @param[in]   payload_in         - payload of some blocks, excluding last block(or message tail).
 * @param[out]  payload_out        - ciphertext or plaintext.
 * @param[in]   payload_bytes      - byte words of payload, must be a multiple of 16, could be 0.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. if the whole payload length is 0, this case is supported. in this case, payload_in and
        payload_out could set NULL.
  @endverbatim
 */
    unsigned int chacha20_poly1305_dma_update_excluding_last_data(CHACHA20_POLY1305_DMA_CTX *ctx, unsigned int *payload_in, unsigned int *payload_out, unsigned int payload_bytes);

    /**
 * @brief       chacha20_poly1305 update message including the last data (DMA mode), and get the tag.
 * @param[in]   ctx                - CHACHA20_POLY1305_DMA_CTX context pointer.
 * @param[in]   payload_in         - payload, including last block(or message tail).
 * @param[out]  payload_out        - ciphertext or plaintext.
 * @param[in]   payload_bytes      - byte length of payload.
 * @param[out]  tag                - chacha20_poly1305 tag, occupies 16 bytes.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. if the all payload length is 0, this case is supported. in this case, payload_in and
        payload_out could set NULL and payload_bytes should be 0.
  @endverbatim
 */
    unsigned int chacha20_poly1305_dma_update_including_last_data(CHACHA20_POLY1305_DMA_CTX *ctx, unsigned int *payload_in, unsigned int *payload_out, unsigned int payload_bytes, unsigned char tag[16]);

    /**
 * @brief       chacha20_poly1305 (DMA style, one-off style).
 * @param[in]   crypto                - encrypting or decrypting.
 * @param[in]   key                   - key in bytes.
 * @param[in]   constant              - iv of nonce.
 * @param[in]   iv                    - byte length of payload.
 * @param[in]   aad                   - key in bytes.
 * @param[in]   aad_bytes             - byte length of aad.
 * @param[in]   payload_in            - payload of some blocks, excluding last block(or message tail).
 * @param[out]  payload_out           - ciphertext or plaintext.
 * @param[in]   payload_bytes         - byte length of payload, could be 0.
 * @param[out]  tag                   - chacha20_poly1305 tag, occupies 4 words.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.aad_bytes and payload_bytes could be any value. if length bytes is 0, could set pointer is NULL.
      -# 2.in dma mode, the byte length of aad or payload occupy 32 bit.
  @endverbatim
 */
    unsigned int chacha20_poly1305_dma(CHACHA20_POLY1305_CRYPTO crypto, unsigned char key[32], unsigned int constant, unsigned char iv[8], unsigned int *aad, unsigned int aad_bytes, unsigned int *payload_in, unsigned int *payload_out, unsigned int payload_bytes, unsigned char tag[16]);


    /**
 * @brief Initializes CHACHA20 DMA for data transfers.
 * @param[in] tx_chn       - The DMA channel to be used for transmit.
 * @param[in] rx_chn       - The DMA channel to be used for receive.
 * @param[in] burst_size   - The burst size to be set for DMA transfers.
 * @return None.
 */
    void telink_chacha20_dma_init(dma_chn_e tx_chn, dma_chn_e rx_chn, dma_burst_size_e burst_size);

#endif


#ifdef __cplusplus
}
#endif

#endif
