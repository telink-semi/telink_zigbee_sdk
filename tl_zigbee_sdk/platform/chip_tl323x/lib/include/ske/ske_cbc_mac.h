/********************************************************************************************************
 * @file    ske_cbc_mac.h
 *
 * @brief   This is the header file for tl323x
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
#ifndef SKE_CBC_MAC_H
#define SKE_CBC_MAC_H



#include "ske.h"


#ifdef __cplusplus
extern "C" {
#endif


    typedef struct
    {
        ske_ctx_t     ske_cbc_mac_ctx[1];
        unsigned char mac_bytes;
        unsigned char is_updated;
        unsigned char left_bytes;
        unsigned char block_buf[16];
        ske_padding_e padding;
        ske_mac_e     mac_action;
    } ske_cbc_mac_ctx_t;

    typedef ske_cbc_mac_ctx_t SKE_CBC_MAC_CTX;

    typedef struct
    {
        ske_ctx_t     ske_cbc_mac_ctx[1];
        unsigned char mac_bytes;
    } ske_cbc_mac_dma_ctx_t;

    typedef ske_cbc_mac_dma_ctx_t SKE_CBC_MAC_DMA_CTX;

    /**
 * @brief           APIs
 */

    /**
 * @brief           ske_lp cbc mac init(CPU style)
 * @param[in]       ctx                  - ske_cbc_mac_ctx_t context pointer
 * @param[in]       alg                  - ske_lp algorithm
 * @param[in]       mac_action           - must be SKE_GENERATE_MAC or SKE_VERIFY_MAC
 * @param[in]       padding              - ske_lp cbc mac padding scheme, like SKE_NO_PADDING,SKE_ZERO_PADDING.
 * @param[in]       key                  - key in bytes
 * @param[in]       sp_key_idx           - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX],
 * @param[in]       mac_bytes            - mac byte length, must be bigger than 1, and not bigger than block length
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
 *           otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX
 */
    unsigned int ske_lp_cbc_mac_init(ske_cbc_mac_ctx_t *ctx, ske_alg_e alg, ske_mac_e mac_action, ske_padding_e padding, const unsigned char *key, unsigned short sp_key_idx,
                                     unsigned char mac_bytes);

    /**
 * @brief           ske_lp cbc_mac update message(CPU style)
 * @param[in]       ctx                  - ske_cbc_mac_ctx_t context pointer
 * @param[in]       msg                  - message
 * @param[in]       msg_len            - byte length of message.
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. msg_len could be any value
 */
    unsigned int ske_lp_cbc_mac_update(ske_cbc_mac_ctx_t *ctx, const unsigned char *msg, unsigned int msg_len);

    /**
 * @brief           ske_lp cbc_mac finish, and get the mac(CPU style)
 * @param[in]       ctx                  - ske_cbc_mac_ctx_t context pointer
 * @param[in]       mac                  - input(for generating mac), output(for verifying mac)
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. if ctx->mac_action is SKE_GENERATE_MAC, mac is output. and if ctx->mac_action is SKE_VERIFY_MAC,
 *           mac is input, return value SKE_SUCCESS means the mac is valid, otherwise mac is invalid.
 *        2. for the case that padding is SKE_NO_PADDING, if the total length of message is not a multiple of
 *           block length, it will return error
 */
    unsigned int ske_lp_cbc_mac_final(ske_cbc_mac_ctx_t *ctx, unsigned char *mac);

    /**
 * @brief           ske_lp cbc mac(CPU style, one-off style)
 * @param[in]       alg                  - ske_lp algorithm
 * @param[in]       mac_action           - must be SKE_GENERATE_MAC or SKE_VERIFY_MAC
 * @param[in]       padding              - ske_lp cbc mac padding scheme, like SKE_NO_PADDING,SKE_ZERO_PADDING.
 * @param[in]       key                  - key in bytes
 * @param[in]       sp_key_idx           - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX],
 * @param[in]       msg                  - message
 * @param[in]       msg_len            - byte length of message.
 * @param[in]       mac                  - input(for generating mac), output(for verifying mac)
 * @param[in]       mac_bytes            - mac byte length, must be bigger than 1, and not bigger than block length
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
 *           otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX]
 *        2. msg_len can not be 0
 *        3. if mac_action is SKE_GENERATE_MAC, mac is output. and if mac_action is SKE_VERIFY_MAC,
 *           mac is input, return value SKE_SUCCESS means the mac is valid, otherwise mac is invalid.
 *        4. for the case that padding is SKE_NO_PADDING, if the total length of message is not a multiple of
 *           block length, it will return error
 */
    unsigned int ske_lp_cbc_mac(ske_alg_e alg, ske_mac_e mac_action, ske_padding_e padding, const unsigned char *key, unsigned short sp_key_idx, const unsigned char *msg,
                                unsigned int msg_len, unsigned char *mac, unsigned char mac_bytes);


#ifdef SKE_LP_DMA_FUNCTION
    /**
 * @brief           ske_lp cbc mac dma style init
 * @param[in]       ctx                  - ske_cbc_mac_dma_ctx_t context pointer
 * @param[in]       alg                  - ske_lp algorithm
 * @param[in]       key                  - key in bytes
 * @param[in]       sp_key_idx           - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX],
 * @param[in]       mac_bytes            - mac byte length, must be bigger than 1, and not bigger than block length
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
 *           otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX
 */
    unsigned int ske_lp_dma_cbc_mac_init(ske_cbc_mac_dma_ctx_t *ctx, ske_alg_e alg, const unsigned char *key, unsigned short sp_key_idx, unsigned char mac_bytes);

    /**
 * @brief           ske_lp cbc mac dma style update message blocks(excluding the last block, or the message tail)
 * @param[in]       ctx                  - ske_cbc_mac_dma_ctx_t context pointer
 * @param[in]       msg                  - message of some blocks, excluding last block(or message tail)
 * @param[in]       msg_words            - word length of msg, must be a multiple of block word length
 * @param[out]      tmp_out              - temporary output ciphertext, ske_lp need to output, it occupies the same blocks as the
 * @param[in]       callback             - callback function pointer, this could be NULL, means doing nothing
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. the input msg must be some blocks, and excludes the last block(or message tail
 */
    unsigned int ske_lp_dma_cbc_mac_update_blocks_excluding_last_block(ske_cbc_mac_dma_ctx_t *ctx, unsigned int *msg, unsigned int msg_words, unsigned int *tmp_out,
                                                                       SKE_CALLBACK callback);

    /**
 * @brief           ske_lp cbc mac dma style update message including the last block(or message tail), and get the mac
 * @param[in]       ctx                  - ske_cbc_mac_dma_ctx_t context pointer
 * @param[in]       msg                  - message including the last block(or message tail)
 * @param[in]       msg_len            - byte length of msg, can not be 0
 * @param[out]      tmp_out              - temporary output ciphertext, ske_lp need to output, it occupies the same blocks as the
 * @param[out]      mac                  - cbc_mac, occupies a block
 * @param[in]       callback             - callback function pointer, this could be NULL, means doing nothing
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. if the actual message length msg_len is not a multiple of block length, please make sure the
 *           last block(or message tail) is padded with 0 already
 */
    unsigned int ske_lp_dma_cbc_mac_update_including_last_block(ske_cbc_mac_dma_ctx_t *ctx, unsigned int *msg, unsigned int msg_len, unsigned int *tmp_out, unsigned int *mac,
                                                                SKE_CALLBACK callback);
    /**
 * @brief           ske_lp cbc_mac(DMA style, one-off style)
 * @param[in]       alg                  - ske_lp algorithm
 * @param[in]       key                  - key in byte buffer style
 * @param[in]       sp_key_idx           - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX],
 * @param[in]       msg                  - message
 * @param[in]       msg_len            - byte length of message, can not be 0
 * @param[out]      tmp_out              - temporary output ciphertext, ske_lp need to output, it occupies the same blocks as the
 * @param[out]      mac                  - cbc mac
 * @param[in]       mac_bytes            - mac byte length, must be bigger than 1, and not bigger than block length
 * @param[in]       callback             - callback function pointer, this could be NULL, means doing nothing
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
 *           otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX]
 *        2. msg_len can not be 0
 *        3. if the actual message length msg_len is not a multiple of block length, please make sure the last block
 *           is padded with 0 already
 */
    unsigned int ske_lp_dma_cbc_mac(ske_alg_e alg, const unsigned char *key, unsigned short sp_key_idx, unsigned int *msg, unsigned int msg_len, unsigned int *tmp_out,
                                    unsigned int *mac, unsigned int mac_bytes, SKE_CALLBACK callback);
#endif


#ifdef __cplusplus
}
#endif

#endif
