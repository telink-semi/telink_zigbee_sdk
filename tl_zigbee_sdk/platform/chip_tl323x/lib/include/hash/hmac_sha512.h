/********************************************************************************************************
 * @file    hmac_sha512.h
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
#ifndef HMAC_SHA512_H
#define HMAC_SHA512_H


#include "hmac.h"


#ifdef __cplusplus
extern "C"
{
#endif


#ifdef SUPPORT_HASH_SHA512


    typedef hmac_ctx_t hmac_sha512_ctx_t;
    typedef hmac_ctx_t HMAC_SHA512_CTX;

#ifdef HASH_DMA_FUNCTION
    typedef hmac_dma_ctx_t hmac_sha512_dma_ctx_t;
    typedef hmac_dma_ctx_t HMAC_SHA512_DMA_CTX;
#endif


    //APIs

    /**
 * @brief           init hmac-sha512
 * @param[in]       ctx                  - hmac_sha512_ctx_t context pointer
 * @param[in]       key                  - key
 * @param[in]       sp_key_idx           - index of secure port key
 * @param[in]       key_bytes            - byte length of key, it could be 0
 * @return          HASH_SUCCESS(success), other(error)
 */
    unsigned int hmac_sha512_init(hmac_sha512_ctx_t *ctx, const unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes);

    /**
 * @brief           hmac-sha512 update message
 * @param[in]       ctx                  - hmac_sha512_ctx_t context pointer
 * @param[in]       msg                  - message
 * @param[in]       msg_len            - byte length of the input message
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the three parameters are valid, and ctx is initialize
 */
    unsigned int hmac_sha512_update(hmac_sha512_ctx_t *ctx, const unsigned char *msg, unsigned int msg_len);

    /**
 * @brief           message update done, get the hmac
 * @param[in]       ctx                  - hmac_ctx_t context pointer
 * @param[out]      mac                  - hmac
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the ctx is valid and initialized
 *        2. please make sure the mac buffer is sufficient
 */
    unsigned int hmac_sha512_final(hmac_sha512_ctx_t *ctx, unsigned char *mac);

    /**
 * @brief           input key and whole message, get the hmac
 * @param[in]       key                  - key
 * @param[in]       sp_key_idx           - index of secure port key
 * @param[in]       key_bytes            - byte length of the key
 * @param[in]       msg                  - message
 * @param[in]       msg_len            - byte length of the input message
 * @param[out]      mac                  - hmac
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the mac buffer is sufficient
 */
    unsigned int hmac_sha512(const unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, const unsigned char *msg, unsigned int msg_len, unsigned char *mac);

#ifdef SUPPORT_HASH_NODE
    /**
 * @brief           input key and whole message, get the hmac(node style)
 * @param[in]       key                  - key
 * @param[in]       sp_key_idx           - index of secure port key
 * @param[in]       key_bytes            - byte length of the key
 * @param[in]       node                 - message node pointer
 * @param[in]       node_num             - number of hash nodes, i.e. number of message segments.
 * @param[out]      mac                  - hmac
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the mac buffer is sufficient
 *        2. if the whole message consists of some segments, every segment is a node, a node includes
 *           address and byte length
 */
    unsigned int hmac_sha512_node_steps(const unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, const hash_node_t *node, unsigned int node_num,
                                        unsigned char *mac);
#endif


#ifdef HASH_DMA_FUNCTION
    /**
 * @brief           init dma hmac-sha512
 * @param[in]       ctx                  - hmac_sha512_dma_ctx_t context pointer
 * @param[in]       key                  - key
 * @param[in]       sp_key_idx           - index of secure port key
 * @param[in]       key_bytes            - key byte length
 * @param[in]       callback             - callback function pointer
 * @return          HASH_SUCCESS(success), other(error)
 */
    unsigned int hmac_sha512_dma_init(hmac_sha512_dma_ctx_t *ctx, const unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, hash_callback callback);

    /**
 * @brief           dma hmac-sha512 update message
 * @param[in]       ctx                  - hmac_sha512_dma_ctx_t context pointer
 * @param[in]       msg                  - message
 * @param[in]       msg_len            - byte length of the input message, must be a multiple of block byte length
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the four parameters are valid, and ctx is initialize
 */
    unsigned int hmac_sha512_dma_update_blocks(hmac_sha512_dma_ctx_t *ctx, unsigned int *msg, unsigned int msg_len);

    /**
 * @brief           dma hmac-sha512 message update done, get the hmac
 * @param[in]       ctx                  - hmac_sha512_dma_ctx_t context pointer
 * @param[in]       remainder_msg        - message
 * @param[in]       remainder_bytes      - byte length of the remainder message
 * @param[out]      mac                  - hmac
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the three parameters are valid, and ctx is initialize
 */
    unsigned int hmac_sha512_dma_final(hmac_sha512_dma_ctx_t *ctx, unsigned int *remainder_msg, unsigned int remainder_bytes, unsigned int *mac);

    /**
 * @brief           dma hmac-sha512 input key and message, get the hmac
 * @param[in]       key                  - key
 * @param[in]       sp_key_idx           - index of secure port key
 * @param[in]       key_bytes            - key byte length
 * @param[in]       msg                  - message
 * @param[in]       msg_len            - byte length of the input message
 * @param[out]      mac                  - hmac
 * @param[in]       callback             - callback function pointer
 * @return          HASH_SUCCESS(success), other(error)
 */
    unsigned int hmac_sha512_dma(const unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, unsigned int *msg, unsigned int msg_len, unsigned int *mac,
                                 hash_callback callback);

#ifdef SUPPORT_HASH_DMA_NODE
    /**
 * @brief           dma hmac input key and message, get the hmac(node style)
 * @param[in]       key                  - key
 * @param[in]       sp_key_idx           - index of secure port key
 * @param[in]       key_bytes            - key byte length
 * @param[in]       node                 - message node pointer
 * @param[in]       node_num             - number of hash nodes, i.e. number of message segments.
 * @param[out]      mac                  - hmac
 * @param[in]       callback             - callback function pointer
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the digest buffer is sufficient
 *        2. if the whole message consists of some segments, every segment is a node, a node includes
 *           address and byte length.
 *        3. for every node or segment except the last, its message length must be a multiple of block length
 */
    unsigned int hmac_sha512_dma_node_steps(const unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, const hash_dma_node_t *node, unsigned int node_num,
                                            unsigned int *mac, hash_callback callback);
#endif
#endif


#endif


#ifdef __cplusplus
}
#endif


#endif
