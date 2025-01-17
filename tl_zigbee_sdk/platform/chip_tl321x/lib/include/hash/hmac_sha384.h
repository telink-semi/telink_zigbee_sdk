/********************************************************************************************************
 * @file    hmac_sha384.h
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
#ifndef HMAC_SHA384_H
#define HMAC_SHA384_H


#include "hmac.h"


#ifdef __cplusplus
extern "C"
{
#endif


#ifdef SUPPORT_HASH_SHA384


    typedef HMAC_CTX HMAC_SHA384_CTX;

    #ifdef HASH_DMA_FUNCTION
    typedef HMAC_DMA_CTX HMAC_SHA384_DMA_CTX;
    #endif


    //APIs

    unsigned int hmac_sha384_init(HMAC_SHA384_CTX *ctx, unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes);

    unsigned int hmac_sha384_update(HMAC_SHA384_CTX *ctx, unsigned char *msg, unsigned int msg_bytes);

    unsigned int hmac_sha384_final(HMAC_SHA384_CTX *ctx, unsigned char *mac);

    unsigned int hmac_sha384(unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, unsigned char *msg, unsigned int msg_bytes, unsigned char *mac);

    /**
 * @brief        input key and whole message, get the hmac(node style)
 * @param[in]    key           - input, key
 * @param[in]    sp_key_idx    - input, index of secure port key
 * @param[in]    key_bytes     - input, byte length of the key
 * @param[in]    node          - input, message node pointer
 * @param[in]    node_num      - input, number of hash nodes, i.e. number of message segments.
 * @param[out]   mac           - output, hmac
 * @return       0:HASH_SUCCESS(success), other(error)
 * @note
  @verbatim
 *     -# 1. please make sure the mac buffer is sufficient
 *     -# 2. if the whole message consists of some segments, every segment is a node, a node includes
 *        address and byte length.
  @endverbatim
 */
    #ifdef SUPPORT_HASH_NODE
    unsigned int hmac_sha384_node_steps(unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, HASH_NODE *node, unsigned int node_num, unsigned char *mac);
    #endif


    #ifdef HASH_DMA_FUNCTION
    unsigned int hmac_sha384_dma_init(HMAC_SHA384_DMA_CTX *ctx, unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, HASH_CALLBACK callback);

        #ifdef CONFIG_HASH_SUPPORT_ADDRESS_HIGH_LOW
    unsigned int hmac_sha384_dma_update_blocks(HMAC_SHA384_DMA_CTX *ctx, unsigned int msg_h, unsigned int msg_l, unsigned int msg_bytes);

    unsigned int hmac_sha384_dma_final(HMAC_SHA384_DMA_CTX *ctx, unsigned int remainder_msg_h, unsigned int remainder_msg_l, unsigned int remainder_bytes, unsigned int mac_h, unsigned int mac_l);

    unsigned int hmac_sha384_dma(unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, unsigned int msg_h, unsigned int msg_l, unsigned int msg_bytes, unsigned int mac_h, unsigned int mac_l, HASH_CALLBACK callback);

            #ifdef SUPPORT_HASH_DMA_NODE
    unsigned int hmac_sha384_dma_node_steps(unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, HASH_DMA_NODE *node, unsigned int node_num, unsigned int mac_h, unsigned int mac_l, HASH_CALLBACK callback);
            #endif
        #else
    unsigned int hmac_sha384_dma_update_blocks(HMAC_SHA384_DMA_CTX *ctx, unsigned int *msg, unsigned int msg_bytes);

    unsigned int hmac_sha384_dma_final(HMAC_SHA384_DMA_CTX *ctx, unsigned int *remainder_msg, unsigned int remainder_bytes, unsigned int *mac);

    unsigned int hmac_sha384_dma(unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, unsigned int *msg, unsigned int msg_bytes, unsigned int *mac, HASH_CALLBACK callback);

            #ifdef SUPPORT_HASH_DMA_NODE
    unsigned int hmac_sha384_dma_node_steps(unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, HASH_DMA_NODE *node, unsigned int node_num, unsigned int *mac, HASH_CALLBACK callback);
            #endif
        #endif
    #endif


#endif


#ifdef __cplusplus
}
#endif


#endif
