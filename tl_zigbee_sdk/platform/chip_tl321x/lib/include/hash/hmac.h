/********************************************************************************************************
 * @file    hmac.h
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
#ifndef HMAC_H
#define HMAC_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "hash.h"


#define HMAC_IPAD          (0x36363636)
#define HMAC_OPAD          (0x5c5c5c5c)
#define HMAC_IPAD_XOR_OPAD (HMAC_IPAD ^ HMAC_OPAD)

    //HMAC context
    typedef struct
    {
        HASH_CTX     hash_ctx[1];
        unsigned int K0[HASH_BLOCK_MAX_WORD_LEN];

    } __attribute__((packed, aligned(4))) HMAC_CTX;

//HMAC DMA context
#ifdef HASH_DMA_FUNCTION
    typedef struct
    {
        unsigned int K0[HASH_BLOCK_MAX_WORD_LEN];
        HASH_DMA_CTX hash_dma_ctx[1];
    } __attribute__((packed, aligned(4))) HMAC_DMA_CTX;
#endif


    //APIs
    /**
 * @brief       init HMAC
 * @param[in]   ctx            - HMAC_CTX context pointer.
 * @param[in]   hash_alg       - specific hash algorithm.
 * @param[in]   key            - key.
 * @param[in]   sp_key_idx     - index of secure port key.
 * @param[in]   key_bytes      - byte length of key, it could be 0.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure hash_alg is valid.
  @endverbatim
 */
    unsigned int hmac_init(HMAC_CTX *ctx, HASH_ALG hash_alg, unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes);

    /**
 * @brief       hmac update message
 * @param[in]   ctx            - HMAC_CTX context pointer.
 * @param[in]   msg            - message.
 * @param[in]   msg_bytes      - byte length of the input message.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the three parameters are valid, and ctx is initialized.
  @endverbatim
 */
    unsigned int hmac_update(HMAC_CTX *ctx, unsigned char *msg, unsigned int msg_bytes);

    /**
 * @brief       message update done, get the hmac
 * @param[in]   ctx            - HMAC_CTX context pointer.
 * @param[out]  mac            - message.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the ctx is valid and initialized.
      -# 2. please make sure the mac buffer is sufficient.
  @endverbatim
 */
    unsigned int hmac_final(HMAC_CTX *ctx, unsigned char *mac);

    /**
 * @brief       input key and whole message, get the hmac
 * @param[in]   hash_alg       - specific hash algorithm.
 * @param[in]   key            - key.
 * @param[in]   sp_key_idx     - index of secure port key.
 * @param[in]   key_bytes      - byte length of the key.
 * @param[in]   msg            - message.
 * @param[in]   msg_bytes      - byte length of the input message.
 * @param[out]  mac            - hmac.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the mac buffer is sufficient.
  @endverbatim
 */
    unsigned int hmac(HASH_ALG hash_alg, unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, unsigned char *msg, unsigned int msg_bytes, unsigned char *mac);

#ifdef SUPPORT_HASH_NODE
    /**
 * @brief        input key and whole message, get the hmac(node style)
 * @param[in]    hash_alg The specific hash algorithm to use.
 * @param[in]    key The key used for the HMAC calculation.
 * @param[in]    sp_key_idx The index of the secure port key.
 * @param[in]    key_bytes The byte length of the key.
 * @param[in]    node A pointer to the message node.
 * @param[in]    node_num The number of hash nodes, i.e., the number of message segments.
 * @param[out]   mac The output HMAC value.
 * @return       HASH_SUCCESS on success, or other values on error.
 * @note
 *     -# 1. please make sure the mac buffer is sufficient
 *     -# 2. here hmac is not for SHA3.
 *     -# 3. if the whole message consists of some segments, every segment is a node, a node includes
 *           address and byte length.
 */
    unsigned int hmac_node_steps(HASH_ALG hash_alg, unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, HASH_NODE *node, unsigned int node_num, unsigned char *mac);
#endif

#ifdef HASH_DMA_FUNCTION

    /**
 * @brief       input key and whole message, get the hmac
 * @param[in]   ctx            - HMAC_DMA_CTX context pointer.
 * @param[in]   hash_alg       - specific hash algorithm.
 * @param[in]   key            - key.
 * @param[in]   sp_key_idx     - index of secure port key.
 * @param[in]   key_bytes      - key byte length.
 * @param[in]   callback       - callback function pointer.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure hash_alg is valid.
      -# 1. here hmac is not for SHA3.
  @endverbatim
 */
    unsigned int hmac_dma_init(HMAC_DMA_CTX *ctx, HASH_ALG hash_alg, unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, HASH_CALLBACK callback);

    /**
 * @brief       dma hmac update message
 * @param[in]   ctx            - HMAC_DMA_CTX context pointer.
 * @param[in]   msg            - message.
 * @param[in]   msg_bytes      - word length of the input message, must be a multiple of block word length of HASH.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the four parameters are valid, and ctx is initialized.
  @endverbatim
 */
    unsigned int hmac_dma_update_blocks(HMAC_DMA_CTX *ctx, unsigned int *msg, unsigned int msg_bytes);

    /**
 * @brief       dma hmac message update done, get the hmac
 * @param[in]   ctx                - HMAC_DMA_CTX context pointer.
 * @param[in]   remainder_msg      - message.
 * @param[in]   remainder_bytes    - byte length of the last message, must be in [0, BLOCK_BYTE_LEN-1],
 *                                   here BLOCK_BYTE_LEN is block byte length of HASH.
 * @param[out]  mac                - hmac
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the three parameters are valid, and ctx is initialized.
  @endverbatim
 */
    unsigned int hmac_dma_final(HMAC_DMA_CTX *ctx, unsigned int *remainder_msg, unsigned int remainder_bytes, unsigned int *mac);

    /**
 * @brief       dma hmac input key and message, get the hmac
 * @param[in]   hash_alg                - specific hash algorithm.
 * @param[in]   key                     - key.
 * @param[in]   sp_key_idx              - index of secure port key.
 * @param[in]   key_bytes               - key byte length.
 * @param[in]   msg                     - message.
 * @param[in]   msg_bytes               - byte length of the input message.
 * @param[out]  mac                     - hmac.
 * @param[in]   callback                - callback function pointer.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure hash_alg is valid.
  @endverbatim
 */
    unsigned int hmac_dma(HASH_ALG hash_alg, unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, unsigned int *msg, unsigned int msg_bytes, unsigned int *mac, HASH_CALLBACK callback);

    #ifdef SUPPORT_HASH_DMA_NODE
    /**
 * @brief       dma hmac input key and message, get the hmac(node style).
 * @param[in]   hash_alg          - specific hash algorithm
 * @param[in]   key               - key.
 * @param[in]   sp_key_idx        - index of secure port key.
 * @param[in]   key_bytes         - key byte length.
 * @param[in]   node              - message node pointer
 * @param[in]   node_num          - number of hash nodes, i.e. number of message segments.
 * @param[out]   mac               - hmac.
 * @param[in]   callback          - callback function pointer
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the mac buffer is sufficient.
      -# 2. if the whole message consists of some segments, every segment is a node, a node includes
            address and byte length.
      -# 3. here hmac is not for SHA3.
      -# 4. if the whole message consists of some segments, every segment is a node, a node includes
            address and byte length.
      -# 5. for every node or segment except the last, its message length must be a multiple of block length.
  @endverbatim
 */
    unsigned int hmac_dma_node_steps(HASH_ALG hash_alg, unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, HASH_DMA_NODE *node, unsigned int node_num, unsigned int *mac, HASH_CALLBACK callback);

    #endif
    /**
 * @brief       HMAC enable secure port
 * @param[in]   sp_key_idx     - index of secure port key.
 * @return      none
 */
    void hash_hmac_enable_secure_port(unsigned short sp_key_idx);

#endif


#ifdef __cplusplus
}
#endif

#endif
