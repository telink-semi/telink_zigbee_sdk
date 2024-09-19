/********************************************************************************************************
 * @file    hmac_sm3.h
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
#ifndef HMAC_SM3_H
#define HMAC_SM3_H



#include "hmac.h"



#ifdef __cplusplus
extern "C" {
#endif


#ifdef SUPPORT_HASH_SM3


typedef HMAC_CTX HMAC_SM3_CTX;

#ifdef HASH_DMA_FUNCTION
typedef HMAC_DMA_CTX HMAC_SM3_DMA_CTX;
#endif


//APIs
/**
 * @brief       init hmac-sm3
 * @param[in]   ctx               - HMAC_SM3_CTX context pointer.
 * @param[in]   key               - key.
 * @param[in]   sp_key_idx        - index of secure port key
 * @param[in]   key_bytes         - byte length of key, it could be 0.
 * @return      0:success     other:error
 */
unsigned int hmac_sm3_init(HMAC_SM3_CTX *ctx, unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes);

/**
 * @brief       hmac-sm3 update message
 * @param[in]   ctx               - HMAC_SM3_CTX context pointer.
 * @param[in]   msg               - message.
 * @param[in]   msg_bytes         - byte length of the input message.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the three parameters are valid, and ctx is initialized.
  @endverbatim
 */
unsigned int hmac_sm3_update(HMAC_SM3_CTX *ctx, const unsigned char *msg, unsigned int msg_bytes);

/**
 * @brief       message update done, get the hmac
 * @param[in]   ctx               - HMAC_CTX context pointer.
 * @param[out]  mac               - hmac.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the ctx is valid and initialized.
      -# 2. please make sure the mac buffer is sufficient.
  @endverbatim
 */
unsigned int hmac_sm3_final(HMAC_SM3_CTX *ctx, unsigned char *mac);

/**
 * @brief       input key and whole message, get the hmac
 * @param[in]   key               - key.
 * @param[in]   sp_key_idx        - index of secure port key.
 * @param[in]   key_bytes         - byte length of the key.
 * @param[in]   msg               - message.
 * @param[in]   msg_bytes         - byte length of the input message.
 * @param[out]  mac               - hmac.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the mac buffer is sufficient.
  @endverbatim
 */
unsigned int hmac_sm3(unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, unsigned char *msg, unsigned int msg_bytes, unsigned char *mac);


#ifdef HASH_DMA_FUNCTION
/**
 * @brief       init dma hmac-sm3
 * @param[in]   ctx            - HMAC_SM3_DMA_CTX context pointer.
 * @param[in]   key            - key.
 * @param[in]   sp_key_idx     - index of secure port key.
 * @param[in]   key_bytes      - message.
 * @param[in]   msg_bytes      - key byte length.
 * @param[in]   callback       - callback function pointer.
 * @return      0:success     other:error
 */
unsigned int hmac_sm3_dma_init(HMAC_SM3_DMA_CTX *ctx, const unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes,
        HASH_CALLBACK callback);

/**
 * @brief       input key and whole message, get the hmac
 * @param[in]   ctx               - HMAC_SM3_DMA_CTX context pointer.
 * @param[in]   msg               - message.
 * @param[in]   msg_words         - word length of the input message, must be a multiple of block word length
 *                                  of SM3(16).
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.please make sure the four parameters are valid, and ctx is initialized.
  @endverbatim
 */
unsigned int hmac_sm3_dma_update_blocks(HMAC_SM3_DMA_CTX *ctx, unsigned int *msg, unsigned int msg_words);

/**
 * @brief       input key and whole message, get the hmac
 * @param[in]   ctx               - HMAC_SM3_DMA_CTX context pointer.
 * @param[in]   remainder_msg     - message.
 * @param[in]   remainder_bytes   - byte length of the last message, must be in [0, BLOCK_BYTE_LEN-1],
 *                                  here BLOCK_BYTE_LEN is block byte length of SM3(64).
 * @param[out]  mac               - hmac.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the three parameters are valid, and ctx is initialized.
  @endverbatim
 */
unsigned int hmac_sm3_dma_final(HMAC_SM3_DMA_CTX *ctx, unsigned int *remainder_msg, unsigned int remainder_bytes,
        unsigned int *mac);

/**
 * @brief       dma hmac-sm3 input key and message, get the hmac
 * @param[in]   key            - key.
 * @param[in]   sp_key_idx     - index of secure port key.
 * @param[in]   key_bytes      - message.
 * @param[in]   msg            - key byte length.
 * @param[in]   msg_bytes      - message.
 * @param[out]  mac            - byte length of the input message.
 * @param[in]   callback       - callback function pointer.
 * @return      0:success     other:error
 */
unsigned int hmac_sm3_dma(unsigned char *key, unsigned short sp_key_idx, unsigned int key_bytes, unsigned int *msg, unsigned int msg_bytes,
        unsigned int *mac, HASH_CALLBACK callback);
#endif


#endif


#ifdef __cplusplus
}
#endif


#endif
