/********************************************************************************************************
 * @file    hmac.h
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
#ifndef HMAC_H
#define HMAC_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "hash.h"


#define HMAC_IPAD          (0x36363636U)
#define HMAC_OPAD          (0x5c5c5c5cU)
#define HMAC_IPAD_XOR_OPAD (HMAC_IPAD ^ HMAC_OPAD)

    /**
 * @brief           HMAC context
 */
    typedef struct
    {
        hash_ctx_t     hash_ctx[1];
        unsigned int   K0[HASH_BLOCK_MAX_WORD_LEN];
        unsigned int   key_len_flag;
        unsigned int   is_sp_key;
        unsigned int   sp_key_bytes;
        unsigned short sp_key_idx;
    } hmac_ctx_t;

    /**
 * @brief           HMAC DMA context
 */
#ifdef HASH_DMA_FUNCTION
    typedef struct
    {
        unsigned int   K0[HASH_BLOCK_MAX_WORD_LEN];
        unsigned int   key_len_flag;
        hash_dma_ctx_t hash_dma_ctx[1];
        unsigned int   is_sp_key;
        unsigned int   sp_key_bytes;
        unsigned short sp_key_idx;
    } hmac_dma_ctx_t;
#endif


    //fix to old project
    typedef hmac_ctx_t     HMAC_CTX;
    typedef hmac_dma_ctx_t HMAC_DMA_CTX;


    //APIs

    /**
 * @brief           init HMAC
 * @param[in]       ctx                  - context pointer
 * @param[in]       alg                  - specific hash algorithm
 * @param[in]       key                  - key
 * @param[in]       sp_key_idx           - index of secure port key
 * @param[in]       key_bytes            - byte length of key, it could be 0
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure alg is valid
 */
    unsigned int hmac_init(hmac_ctx_t *ctx, hash_alg_e alg, const unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes);

    /**
 * @brief           hmac update message
 * @param[in]       ctx                  - hmac_ctx_t context pointer
 * @param[in]       msg                  - message
 * @param[in]       msg_len            - byte length of the input message
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the three parameters are valid, and ctx is initialize
 */
    unsigned int hmac_update(hmac_ctx_t *ctx, const unsigned char *msg, unsigned int msg_len);

    /**
 * @brief           message update done, get the hmac
 * @param[in]       ctx                  - hmac_ctx_t context pointer
 * @param[out]      mac                  - hmac
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the ctx is valid and initialized
 *        2. please make sure the mac buffer is sufficient
 */
    unsigned int hmac_final(hmac_ctx_t *ctx, unsigned char *mac);

    /**
 * @brief           input key and whole message, get the hmac
 * @param[in]       alg                  - specific hash algorithm
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
    unsigned int hmac(hash_alg_e alg, const unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, const unsigned char *msg, unsigned int msg_len,
                      unsigned char *mac);

#ifdef SUPPORT_HASH_NODE
    /**
 * @brief           input key and whole message, get the hmac(node style)
 * @param[in]       alg                  - specific hash algorithm
 * @param[in]       key                  - key
 * @param[in]       sp_key_idx           - index of secure port key
 * @param[in]       key_bytes            - byte length of the key
 * @param[in]       node                 - message node pointer
 * @param[in]       node_num             - number of hash nodes, i.e. number of message segments.
 * @param[out]      mac                  - hmac
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the mac buffer is sufficient
 *        2. here hmac is not for SHA3.
 *        3. if the whole message consists of some segments, every segment is a node, a node includes
 *           address and byte length
 */
    unsigned int hmac_node_steps(hash_alg_e alg, const unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, const hash_node_t *node, unsigned int node_num,
                                 unsigned char *mac);
#endif


#ifdef HASH_DMA_FUNCTION
    /**
 * @brief           init dma hmac
 * @param[in]       ctx                  - hmac_dma_ctx_t context pointer
 * @param[in]       alg                  - specific hash algorithm
 * @param[in]       key                  - key
 * @param[in]       sp_key_idx           - index of secure port key
 * @param[in]       key_bytes            - key byte length
 * @param[in]       callback             - callback function pointer
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure alg is valid
 *        2. here hmac is not for SHA3
 */
    unsigned int hmac_dma_init(hmac_dma_ctx_t *ctx, hash_alg_e alg, const unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, hash_callback callback);

    /**
 * @brief           dma hmac update message
 * @param[in]       ctx                  - hmac_dma_ctx_t context pointer
 * @param[in]       msg                  - message
 * @param[in]       msg_len            - byte length of the input message, must be a multiple of block byte length of HASH
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the four parameters are valid, and ctx is initialize
 */
    unsigned int hmac_dma_update_blocks(hmac_dma_ctx_t *ctx, const unsigned int *msg, unsigned int msg_len);

    /**
 * @brief           dma hmac message update done, get the hmac
 * @param[in]       ctx                  - hmac_dma_ctx_t context pointer
 * @param[in]       remainder_msg        - message
 * @param[in]       remainder_bytes      - byte length of the last message
 * @param[out]      mac                  - hmac
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the three parameters are valid, and ctx is initialize
 */
    unsigned int hmac_dma_final(hmac_dma_ctx_t *ctx, const unsigned int *remainder_msg, unsigned int remainder_bytes, unsigned int *mac);

    /**
 * @brief           dma hmac input key and message, get the hmac
 * @param[in]       alg                  - specific hash algorithm
 * @param[in]       key                  - key
 * @param[in]       sp_key_idx           - index of secure port key
 * @param[in]       key_bytes            - key byte length
 * @param[in]       msg                  - message
 * @param[in]       msg_len            - byte length of the input message
 * @param[out]      mac                  - hmac
 * @param[in]       callback             - callback function pointer
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure alg is valid
 *        2. here hmac is not for SHA3
 */
    unsigned int hmac_dma(hash_alg_e alg, const unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, unsigned int *msg, unsigned int msg_len, unsigned int *mac,
                          hash_callback callback);

#ifdef SUPPORT_HASH_DMA_NODE
    /**
 * @brief           dma hmac input key and message, get the hmac(node style)
 * @param[in]       alg                  - specific hash algorithm
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
 *        2. please make sure alg is valid
 *        3. here hmac is not for SHA3.
 *        4. if the whole message consists of some segments, every segment is a node, a node includes
 *           address and byte length.
 *        5. for every node or segment except the last, its message length must be a multiple of block length
 */
    unsigned int hmac_dma_node_steps(hash_alg_e alg, const unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, const hash_dma_node_t *node,
                                     unsigned int node_num, unsigned int *mac, hash_callback callback);
#endif
#endif


#ifdef __cplusplus
}
#endif

#endif
