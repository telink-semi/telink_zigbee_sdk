/********************************************************************************************************
 * @file    sha512.h
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
#ifndef SHA512_H
#define SHA512_H



#include "hash.h"



#ifdef __cplusplus
extern "C" {
#endif


#ifdef SUPPORT_HASH_SHA512


typedef HASH_CTX SHA512_CTX;

#ifdef HASH_DMA_FUNCTION
typedef HASH_DMA_CTX SHA512_DMA_CTX;
#endif


//APIs
/**
 * @brief       init sha512
 * @param[in]   ctx         - SHA512_CTX context pointer.
 * @return      0:success     other:error
 */
unsigned int sha512_init(SHA512_CTX *ctx);

/**
 * @brief       sha512 update message
 * @param[in]   ctx            - SHA512_CTX context pointer.
 * @param[in]   msg            - message.
 * @param[in]   msg_bytes      - byte length of the input message.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# please make sure the three parameters are valid, and ctx is initialized.
  @endverbatim
 */
unsigned int sha512_update(SHA512_CTX *ctx, const unsigned char *msg, unsigned int msg_bytes);

/**
 * @brief       message update done, get the sha512 digest
 * @param[out]  digest            - sha512_256 digest, 32 bytes.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the digest buffer is sufficient.
  @endverbatim
 */
unsigned int sha512_final(SHA512_CTX *ctx, unsigned char *digest);

/**
 * @brief       input whole message and get its sha512 digest
 * @param[in]   msg            - message.
 * @param[in]   msg_bytes      - byte length of the input message, it could be 0.
 * @param[out]  digest         - sha512 digest, 64 bytes.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the digest buffer is sufficient.
  @endverbatim
 */
unsigned int sha512(unsigned char *msg, unsigned int msg_bytes, unsigned char *digest);


#ifdef HASH_DMA_FUNCTION
/**
 * @brief       init dma sha512
 * @param[in]   ctx           - SHA512_DMA_CTX context pointer.
 * @param[in]   callback      - callback function pointer.
 * @return      0:success     other:error
 */
unsigned int sha512_dma_init(SHA512_DMA_CTX *ctx, HASH_CALLBACK callback);

/**
 * @brief       dma sha512 update some message blocks
 * @param[in]   ctx         - SHA512_DMA_CTX context pointer.
 * @param[in]   msg         - message blocks.
 * @param[in]   msg_words   - word length of the input message, must be a multiple of sha512
 *                            block word length(32).
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the four parameters are valid, and ctx is initialized.
  @endverbatim
 */
unsigned int sha512_dma_update_blocks(SHA512_DMA_CTX *ctx, unsigned int *msg, unsigned int msg_words);

/**
 * @brief       dma sha512 final(input the remainder message and get the digest)
 * @param[in]   ctx               - SHA512_DMA_CTX context pointer.
 * @param[in]   remainder_msg     - remainder message.
 * @param[in]   remainder_bytes   - byte length of the remainder message, must be in [0, BLOCK_BYTE_LEN-1],
 *                                  here BLOCK_BYTE_LEN is block byte length of sha512, it is 128.
 *@param[out]   digest            - sha512_256 digest, 32 bytes.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the four parameters are valid, and ctx is initialized.
  @endverbatim
  */
unsigned int sha512_dma_final(SHA512_DMA_CTX *ctx, unsigned int *remainder_msg, unsigned int remainder_bytes, unsigned int *digest);

/**
 * @brief       dma sha512 digest calculate
 * @param[in]   msg           - message.
 * @param[in]   msg_bytes     - byte length of the message, it could be 0.
 * @param[out]  digest        - sha512 digest, 64 bytes.
 * @param[in]   callback      - callback function pointer.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the four parameters are valid.
  @endverbatim
 */
unsigned int sha512_dma(unsigned int *msg, unsigned int msg_bytes, unsigned int *digest, HASH_CALLBACK callback);
#endif


#endif


#ifdef __cplusplus
}
#endif


#endif
