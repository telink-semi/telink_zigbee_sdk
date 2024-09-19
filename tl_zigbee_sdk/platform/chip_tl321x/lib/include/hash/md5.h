/********************************************************************************************************
 * @file    md5.h
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
#ifndef MD5_H
#define MD5_H



#include "hash.h"



#ifdef __cplusplus
extern "C" {
#endif


#ifdef SUPPORT_HASH_MD5


typedef HASH_CTX MD5_CTX;

#ifdef HASH_DMA_FUNCTION
typedef HASH_DMA_CTX MD5_DMA_CTX;
#endif


//APIs
/**
 * @brief       init md5
 * @param[in]   ctx            - HMAC_CTX context pointer.
 * @return      0:success     other:error
 */
unsigned int md5_init(MD5_CTX *ctx);

/**
 * @brief       md5 update message
 * @param[in]   ctx            - HMAC_CTX context pointer.
 * @param[in]   msg            - message.
 * @param[in]   msg_bytes      - byte length of the input message.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the three parameters are valid, and ctx is initialized.
  @endverbatim
 */
unsigned int md5_update(MD5_CTX *ctx, const unsigned char *msg, unsigned int msg_bytes);

/**
 * @brief       message update done, get the md5 digest
 * @param[out]  digest            - md5 digest, 16 bytes.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the digest buffer is sufficient.
  @endverbatim
 */
unsigned int md5_final(MD5_CTX *ctx, unsigned char *digest);

/**
 * @brief       input whole message and get its md5 digest
 * @param[in]   msg            - message.
 * @param[in]   msg_bytes      - byte length of the input message, it could be 0.
 * @param[out]  digest         - md5 digest, 16 bytes.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the digest buffer is sufficient.
  @endverbatim
 */
unsigned int md5(unsigned char *msg, unsigned int msg_bytes, unsigned char *digest);


#ifdef HASH_DMA_FUNCTION
/**
 * @brief       init dma md5
 * @param[in]   ctx            - MD5_DMA_CTX context pointer.
 * @param[in]   callback       - callback function pointer.
 * @return      0:success     other:error
 */
unsigned int md5_dma_init(MD5_DMA_CTX *ctx, HASH_CALLBACK callback);

/**
 * @brief       dma md5 update some message blocks
 * @param[in]   ctx            - MD5_DMA_CTX context pointer.
 * @param[in]   msg            - message blocks.
 * @param[in]   msg_words      - word length of the input message, must be a multiple of md5
 *                              block word length(16).
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the four parameters are valid, and ctx is initialized.
  @endverbatim
 */
unsigned int md5_dma_update_blocks(MD5_DMA_CTX *ctx, unsigned int *msg, unsigned int msg_words);

/**
 * @brief       dma md5 final(input the remainder message and get the digest)
 * @param[in]   ctx                      - MD5_DMA_CTX context pointer.
 * @param[in]   remainder_msg            - remainder message.
 * @param[in]   remainder_bytes          - byte length of the remainder message, must be in [0, BLOCK_BYTE_LEN-1],
 *                                         here BLOCK_BYTE_LEN is block byte length of md5, it is 64.
 * @param[out]  digest                   - md5 digest, 16 bytes.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the four parameters are valid, and ctx is initialized.
  @endverbatim
 */
unsigned int md5_dma_final(MD5_DMA_CTX *ctx, unsigned int *remainder_msg, unsigned int remainder_bytes, unsigned int *digest);

/**
 * @brief       dma md5 digest calculate
 * @param[in]   msg                      - message.
 * @param[in]   msg_bytes                - byte length of the message, it could be 0.
 * @param[out]  digest                   - md5 digest, 16 bytes.
 * @param[in]   callback                 - callback function pointer.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the four parameters are valid.
  @endverbatim
 */
unsigned int md5_dma(unsigned int *msg, unsigned int msg_bytes, unsigned int *digest, HASH_CALLBACK callback);
#endif


#endif


#ifdef __cplusplus
}
#endif


#endif
