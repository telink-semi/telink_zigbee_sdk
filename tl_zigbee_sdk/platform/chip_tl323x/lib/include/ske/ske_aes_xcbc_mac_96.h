/********************************************************************************************************
 * @file    ske_aes_xcbc_mac_96.h
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
#ifndef AES_XCBC_MAC_96_H
#define AES_XCBC_MAC_96_H


#include "ske.h"


#ifdef __cplusplus
extern "C"
{
#endif


    typedef struct
    {
        unsigned char k2[16];
        unsigned char k3[16];
        unsigned char block_buf[16];
        ske_ctx_t     ske_xcbc_mac_ctx[1];
        unsigned char left_bytes;
        ske_mac_e     mac_action;
    } ske_aes_xcbc_mac_96_ctx_t;

    typedef ske_aes_xcbc_mac_96_ctx_t SKE_AES_XCBC_MAC_96_CTX;

    typedef struct
    {
        unsigned char k2[16];
        unsigned char k3[16];
        ske_ctx_t     ske_xcbc_mac_ctx[1];
    } ske_aes_xcbc_mac_96_dma_ctx_t;

    typedef ske_aes_xcbc_mac_96_dma_ctx_t SKE_AES_XCBC_MAC_96_DMA_CTX;

    /**
 * @brief           APIs
 */

    /**
 * @brief           ske_lp aes_xcbc_mac_96 init(CPU style)
 * @param[in]       ctx                  - ske_aes_xcbc_mac_96_ctx_t context pointer
 * @param[in]       mac_action           - must be SKE_GENERATE_MAC or SKE_VERIFY_MAC
 * @param[in]       key                  - key in bytes
 * @param[in]       sp_key_idx           - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX],
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
 *           otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX
 */
    unsigned int ske_lp_aes_xcbc_mac_96_init(ske_aes_xcbc_mac_96_ctx_t *ctx, ske_mac_e mac_action, const unsigned char *key, unsigned short sp_key_idx);

    /**
 * @brief           ske_lp aes_xcbc_mac_96 update message(CPU style)
 * @param[in]       ctx                  - ske_aes_xcbc_mac_96_ctx_t context pointer
 * @param[in]       msg                  - message
 * @param[in]       msg_len            - byte length of message.
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. msg_len could be any value
 */
    unsigned int ske_lp_aes_xcbc_mac_96_update(ske_aes_xcbc_mac_96_ctx_t *ctx, unsigned char *msg, unsigned int msg_len);

    /**
 * @brief           ske_lp aes_xcbc_mac_96 finish, and get the mac or verify the mac(CPU style)
 * @param[in]       ctx                  - ske_aes_xcbc_mac_96_ctx_t context pointer
 * @param[in]       mac                  - input(for generating mac), output(for verifying mac)
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. if ctx->mac_action is SKE_GENERATE_MAC, mac is output. and if ctx->mac_action is SKE_VERIFY_MAC,
 *           mac is input, return value SKE_SUCCESS means the mac is valid, otherwise mac is invalid
 */
    unsigned int ske_lp_aes_xcbc_mac_96_final(ske_aes_xcbc_mac_96_ctx_t *ctx, unsigned char mac[12]);

    /**
 * @brief           ske_lp xcbc_mac_96(CPU style, one-off style)
 * @param[in]       mac_action           - must be SKE_GENERATE_MAC or SKE_VERIFY_MAC
 * @param[in]       key                  - key in bytes
 * @param[in]       sp_key_idx           - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX],
 * @param[in]       msg                  - message
 * @param[in]       msg_len            - byte length of message.
 * @param[in]       mac                  - input(for generating mac), output(for verifying mac)
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
 *           otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX]
 *        2. msg_len could be any value.
 *        3. if mac_action is SKE_GENERATE_MAC, mac is output. and if mac_action is SKE_VERIFY_MAC,
 *           mac is input, return value SKE_SUCCESS means the mac is valid, otherwise mac is invalid
 */
    unsigned int ske_lp_aes_xcbc_mac_96(ske_mac_e mac_action, const unsigned char *key, unsigned short sp_key_idx, unsigned char *msg, unsigned int msg_len, unsigned char mac[12]);


#ifdef SKE_LP_DMA_FUNCTION
    /**
 * @brief           ske_lp aes-xcbc-mac-96 dma style init
 * @param[in]       ctx                  - ske_aes_xcbc_mac_96_dma_ctx_t context pointer
 * @param[in]       key                  - key in bytes
 * @param[in]       sp_key_idx           - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX],
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
 *           otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX
 */
    unsigned int ske_lp_dma_aes_xcbc_mac_96_init(ske_aes_xcbc_mac_96_dma_ctx_t *ctx, const unsigned char *key, unsigned short sp_key_idx);

    /**
 * @brief           ske_lp aes-xcbc-mac-96 dma style update update message blocks(excluding the last block, or the message tail)
 * @param[in]       ctx                  - ske_aes_xcbc_mac_96_dma_ctx_t context pointer
 * @param[in]       msg                  - message of some blocks, excluding last block(or message tail)
 * @param[in]       msg_words            - word length of msg, must be a multiple of block word length
 * @param[out]      tmp_out              - temporary output ciphertext, ske_lp need to output, it occupies the same blocks as the
 * @param[in]       callback             - callback function pointer, this could be NULL, means doing nothing
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. the input msg must be some blocks, and excludes the last block(or message tail
 */
    unsigned int ske_lp_dma_aes_xcbc_mac_96_update_blocks_excluding_last_block(ske_aes_xcbc_mac_96_dma_ctx_t *ctx, unsigned int *msg, unsigned int msg_words, unsigned int *tmp_out,
                                                                               SKE_CALLBACK callback);

    /**
 * @brief           ske_lp aes-xcbc-mac-96 dma style update message including the last block(or message tail), and get the mac
 * @param[in]       ctx                  - ske_aes_xcbc_mac_96_dma_ctx_t context pointer
 * @param[in]       msg                  - message including the last block(or message tail)
 * @param[in]       msg_len            - byte length of msg, could be 0
 * @param[out]      tmp_out              - temporary output ciphertext, ske_lp need to output, it occupies the same blocks as the
 * @param[out]      mac                  - aes-xcbc-mac-96, occupies 12 bytes
 * @param[in]       callback             - callback function pointer, this could be NULL, means doing nothing
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. if the whole message length is 0, this case is supported. in this case, msg occupies a block, and
 *           please set msg_len to 0
 */
    unsigned int ske_lp_dma_aes_xcbc_mac_96_update_including_last_block(ske_aes_xcbc_mac_96_dma_ctx_t *ctx, unsigned int *msg, unsigned int msg_len, unsigned int *tmp_out,
                                                                        unsigned int *mac, SKE_CALLBACK callback);
    /**
 * @brief           ske_lp dma_aes_xcbc_mac_96(DMA style, one-off style)
 * @param[in]       key                  - key in byte buffer style
 * @param[in]       sp_key_idx           - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX],
 * @param[in]       msg                  - message
 * @param[in]       msg_len            - byte length of message.
 * @param[out]      tmp_out              - temporary output ciphertext, ske_lp need to output, it occupies the same blocks as the
 * @param[out]      mac                  - mac
 * @param[in]       callback             - callback function pointer, this could be NULL, means doing nothing
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
 *           otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX]
 *        2. msg_len is actual byte length of message, it could be any value(including 0).
 *           (1). if msg_len is not 0, msg must have (msg_len+15)/16 blocks.
 *           (2). if msg_len is 0, msg occupies a block
 */
    unsigned int ske_lp_dma_aes_xcbc_mac_96(const unsigned char *key, unsigned short sp_key_idx, unsigned int *msg, unsigned int msg_len, unsigned int *tmp_out, unsigned int *mac,
                                            SKE_CALLBACK callback);
#endif


#ifdef __cplusplus
}
#endif

#endif
