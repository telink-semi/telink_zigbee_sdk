/********************************************************************************************************
 * @file    sha256.h
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
#ifndef SHA256_H
#define SHA256_H


#include "hash.h"


#ifdef __cplusplus
extern "C"
{
#endif


#ifdef SUPPORT_HASH_SHA256


    typedef hash_ctx_t sha256_ctx_t;
    typedef hash_ctx_t SHA256_CTX;

#ifdef HASH_DMA_FUNCTION
    typedef hash_dma_ctx_t sha256_dma_ctx_t;
    typedef hash_dma_ctx_t SHA256_DMA_CTX;
#endif


    //APIs

    /**
 * @brief           init sha256
 * @param[in]       ctx                  - sha256_ctx_t context pointer
 * @return          HASH_SUCCESS(success), other(error)
 */
    unsigned int sha256_init(sha256_ctx_t *ctx);

    /**
 * @brief           sha256 update message
 * @param[in]       ctx                  - sha256_ctx_t context pointer
 * @param[in]       msg                  - message
 * @param[in]       msg_len            - byte length of the input message
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the three parameters are valid, and ctx is initialize
 */
    unsigned int sha256_update(sha256_ctx_t *ctx, const unsigned char *msg, unsigned int msg_len);

    /**
 * @brief           message update done, get the sha256 digest
 * @param[out]      digest               - sha256 digest, 32 bytes
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the digest buffer is sufficient
 */
    unsigned int sha256_final(sha256_ctx_t *ctx, unsigned char *digest);

    /**
 * @brief           input whole message and get its sha256 digest
 * @param[in]       msg                  - message
 * @param[in]       msg_len            - byte length of the input message, it could be 0
 * @param[out]      digest               - sha256 digest, 32 bytes
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the digest buffer is sufficient
 */
    unsigned int sha256(const unsigned char *msg, unsigned int msg_len, unsigned char *digest);

#ifdef SUPPORT_HASH_NODE
    /**
 * @brief           input whole message and get its sha256 digest(node style)
 * @param[in]       node                 - message node pointer
 * @param[in]       node_num             - number of hash nodes, i.e. number of message segments.
 * @param[out]      digest               - sha256 digest, 32 bytes
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the digest buffer is sufficient
 *        2. if the whole message consists of some segments, every segment is a node, a node includes
 *           address and byte length
 */
    unsigned int sha256_node_steps(const hash_node_t *node, unsigned int node_num, unsigned char *digest);
#endif


#ifdef HASH_DMA_FUNCTION
    /**
 * @brief           init dma sha256
 * @param[in]       ctx                  - sha256_dma_ctx_t context pointer
 * @param[in]       callback             - callback function pointer
 * @return          HASH_SUCCESS(success), other(error)
 */
    unsigned int sha256_dma_init(sha256_dma_ctx_t *ctx, hash_callback callback);

    /**
 * @brief           dma sha256 update some message blocks
 * @param[in]       ctx                  - sha256_dma_ctx_t context pointer
 * @param[in]       msg                  - message blocks
 * @param[in]       msg_len            - byte length of the input message, must be a multiple of sha256
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the four parameters are valid, and ctx is initialize
 */
    unsigned int sha256_dma_update_blocks(sha256_dma_ctx_t *ctx, unsigned int *msg, unsigned int msg_len);

    /**
 * @brief           dma sha256 final(input the remainder message and get the digest)
 * @param[in]       ctx                  - sha256_dma_ctx_t context pointer
 * @param[in]       remainder_msg        - remainder message
 * @param[in]       remainder_bytes      - byte length of the remainder message
 * @param[out]      digest               - sha256 digest, 32 bytes
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the four parameters are valid, and ctx is initialize
 */
    unsigned int sha256_dma_final(sha256_dma_ctx_t *ctx, unsigned int *remainder_msg, unsigned int remainder_bytes, unsigned int *digest);

    /**
 * @brief           dma sha256 digest calculate
 * @param[in]       msg                  - message
 * @param[in]       msg_len            - byte length of the message, it could be 0
 * @param[out]      digest               - sha256 digest, 32 bytes
 * @param[in]       callback             - callback function pointer
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the four parameters are valid
 */
    unsigned int sha256_dma(unsigned int *msg, unsigned int msg_len, unsigned int *digest, hash_callback callback);

#ifdef SUPPORT_HASH_DMA_NODE
    /**
 * @brief           input whole message and get its sha256 digest(dma node style)
 * @param[in]       node                 - message node pointer
 * @param[in]       node_num             - number of hash nodes, i.e. number of message segments.
 * @param[out]      digest               - sha256 digest, 32 bytes
 * @param[in]       callback             - callback function pointer
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the digest buffer is sufficient
 *        2. if the whole message consists of some segments, every segment is a node, a node includes
 *           address and byte length.
 *        3. for every node or segment except the last, its message length must be a multiple of block length
 */
    unsigned int sha256_dma_node_steps(const hash_dma_node_t *node, unsigned int node_num, unsigned int *digest, hash_callback callback);
#endif
#endif


#endif


#ifdef __cplusplus
}
#endif


#endif
