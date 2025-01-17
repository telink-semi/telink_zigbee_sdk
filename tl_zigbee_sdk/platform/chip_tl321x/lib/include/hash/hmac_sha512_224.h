/********************************************************************************************************
 * @file    hmac_sha512_224.h
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
#ifndef HMAC_SHA512_224_H
#define HMAC_SHA512_224_H


#include "hmac.h"


#ifdef __cplusplus
extern "C"
{
#endif


#ifdef SUPPORT_HASH_SHA512_224


    typedef HMAC_CTX HMAC_SHA512_224_CTX;

    #ifdef HASH_DMA_FUNCTION
    typedef HMAC_DMA_CTX HMAC_SHA512_224_DMA_CTX;
    #endif


    //APIs

    unsigned int hmac_sha512_224_init(HMAC_SHA512_224_CTX *ctx, unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes);

    unsigned int hmac_sha512_224_update(HMAC_SHA512_224_CTX *ctx, unsigned char *msg, unsigned int msg_bytes);

    unsigned int hmac_sha512_224_final(HMAC_SHA512_224_CTX *ctx, unsigned char *mac);

    unsigned int hmac_sha512_224(unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, unsigned char *msg, unsigned int msg_bytes, unsigned char *mac);

    #ifdef SUPPORT_HASH_NODE
    unsigned int hmac_sha512_224_node_steps(unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, HASH_NODE *node, unsigned int node_num, unsigned char *mac);
    #endif


    #ifdef HASH_DMA_FUNCTION
    unsigned int hmac_sha512_224_dma_init(HMAC_SHA512_224_DMA_CTX *ctx, unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, HASH_CALLBACK callback);

        #ifdef CONFIG_HASH_SUPPORT_ADDRESS_HIGH_LOW
    unsigned int hmac_sha512_224_dma_update_blocks(HMAC_SHA512_224_DMA_CTX *ctx, unsigned int msg_h, unsigned int msg_l, unsigned int msg_bytes);

    unsigned int hmac_sha512_224_dma_final(HMAC_SHA512_224_DMA_CTX *ctx, unsigned int remainder_msg_h, unsigned int remainder_msg_l, unsigned int remainder_bytes, unsigned int mac_h, unsigned int mac_l);

    unsigned int hmac_sha512_224_dma(unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, unsigned int msg_h, unsigned int msg_l, unsigned int msg_bytes, unsigned int mac_h, unsigned int mac_l, HASH_CALLBACK callback);

            #ifdef SUPPORT_HASH_DMA_NODE
    unsigned int hmac_sha512_224_dma_node_steps(unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, HASH_DMA_NODE *node, unsigned int node_num, unsigned int mac_h, unsigned int mac_l, HASH_CALLBACK callback);
            #endif
        #else
    unsigned int hmac_sha512_224_dma_update_blocks(HMAC_SHA512_224_DMA_CTX *ctx, unsigned int *msg, unsigned int msg_bytes);

    unsigned int hmac_sha512_224_dma_final(HMAC_SHA512_224_DMA_CTX *ctx, unsigned int *remainder_msg, unsigned int remainder_bytes, unsigned int *mac);

    unsigned int hmac_sha512_224_dma(unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, unsigned int *msg, unsigned int msg_bytes, unsigned int *mac, HASH_CALLBACK callback);

            #ifdef SUPPORT_HASH_DMA_NODE
    unsigned int hmac_sha512_224_dma_node_steps(unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, HASH_DMA_NODE *node, unsigned int node_num, unsigned int *mac, HASH_CALLBACK callback);
            #endif
        #endif
    #endif


#endif


#ifdef __cplusplus
}
#endif


#endif
