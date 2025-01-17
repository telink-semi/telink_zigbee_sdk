/********************************************************************************************************
 * @file    sha1.h
 *
 * @brief   This is the header file for TL721X
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
#ifndef SHA1_H
#define SHA1_H


#include "hash.h"


#ifdef __cplusplus
extern "C"
{
#endif


#ifdef SUPPORT_HASH_SHA1


    typedef HASH_CTX SHA1_CTX;

    #ifdef HASH_DMA_FUNCTION
    typedef HASH_DMA_CTX SHA1_DMA_CTX;
    #endif


    //APIs
    /**
 * @brief       init sha1
 * @param[in]   ctx         - SHA1_CTX context pointer.
 * @return      0:success     other:error
 */
    unsigned int sha1_init(SHA1_CTX *ctx);

    /**
 * @brief       sha1 update message
 * @param[in]   ctx            - SHA1_CTX context pointer.
 * @param[in]   msg            - message.
 * @param[in]   msg_bytes      - byte length of the input message.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the three parameters are valid, and ctx is initialized.
  @endverbatim
 */
    unsigned int sha1_update(SHA1_CTX *ctx, const unsigned char *msg, unsigned int msg_bytes);

    /**
 * @brief       message update done, get the sha1 digest
 * @param[in]   ctx            - SHA1_CTX context pointer.
 * @param[out]  digest         - sha1 digest, 20 bytes.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the digest buffer is sufficient.
  @endverbatim
 */
    unsigned int sha1_final(SHA1_CTX *ctx, unsigned char *digest);

    /**
 * @brief       input whole message and get its sha1 digest
 * @param[in]   msg            - message.
 * @param[in]   msg_bytes      - byte length of the input message, it could be 0.
 * @param[out]  digest         - sha1 digest, 20 bytes.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the digest buffer is sufficient.
  @endverbatim
 */
    unsigned int sha1(unsigned char *msg, unsigned int msg_bytes, unsigned char *digest);


    #ifdef HASH_DMA_FUNCTION
    /**
 * @brief       init dma sha1
 * @param[in]   ctx            - SHA1_DMA_CTX context pointer.
 * @param[in]   callback       - callback function pointer.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the digest buffer is sufficient.
  @endverbatim
 */
    unsigned int sha1_dma_init(SHA1_DMA_CTX *ctx, HASH_CALLBACK callback);

    /**
 * @brief       dma sha1 update some message blocks
 * @param[in]   ctx            - SHA1_DMA_CTX context pointer.
 * @param[in]   msg            - message blocks.
 * @param[in]   msg_words      - word length of the input message, must be a multiple of sha1
 *                               block word length(16).
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the four parameters are valid, and ctx is initialized.
  @endverbatim
 */
    unsigned int sha1_dma_update_blocks(SHA1_DMA_CTX *ctx, unsigned int *msg, unsigned int msg_words);

    /**
 * @brief       dma sha1 final(input the remainder message and get the digest)
 * @param[in]   ctx               - SHA1_DMA_CTX context pointer.
 * @param[in]   remainder_msg     - remainder message.
 * @param[in]   remainder_bytes   - byte length of the remainder message, must be in [0, BLOCK_BYTE_LEN-1],
 *                                  here BLOCK_BYTE_LEN is block byte length of sha1, it is 64.
 * @param[out]  digest            - sha1 digest, 20 bytes.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the four parameters are valid, and ctx is initialized.
  @endverbatim
 */
    unsigned int sha1_dma_final(SHA1_DMA_CTX *ctx, unsigned int *remainder_msg, unsigned int remainder_bytes, unsigned int *digest);

    /**
 * @brief       dma sha1 digest calculate
 * @param[in]   msg               - message.
 * @param[in]   msg_bytes         - byte length of the message, it could be 0.
 * @param[out]  digest            - sha1 digest, 20 bytes.
 * @param[in]   callback          - callback function pointer.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the four parameters are valid, and ctx is initialized.
  @endverbatim
 */
    unsigned int sha1_dma(unsigned int *msg, unsigned int msg_bytes, unsigned int *digest, HASH_CALLBACK callback);
    #endif


#endif


#ifdef __cplusplus
}
#endif


#endif
