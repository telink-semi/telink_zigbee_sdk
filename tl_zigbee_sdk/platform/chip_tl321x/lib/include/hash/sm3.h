/********************************************************************************************************
 * @file    sm3.h
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
#ifndef SM3_H
#define SM3_H


#include "hash.h"


#ifdef __cplusplus
extern "C"
{
#endif


#ifdef SUPPORT_HASH_SM3


    typedef HASH_CTX SM3_CTX;

    #ifdef HASH_DMA_FUNCTION
    typedef HASH_DMA_CTX SM3_DMA_CTX;
    #endif


    //APIs
    /**
 * @brief       init sm3
 * @param[in]   ctx         - SHA512_CTX context pointer.
 * @return      0:success     other:error
 */
    unsigned int sm3_init(SM3_CTX *ctx);

    /**
 * @brief       sm3 update message
 * @param[in]   ctx            - SM3_CTX context pointer.
 * @param[in]   msg            - message.
 * @param[in]   msg_bytes      - byte length of the input message.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# please make sure the three parameters are valid, and ctx is initialized.
  @endverbatim
 */
    unsigned int sm3_update(SM3_CTX *ctx, unsigned char *msg, unsigned int msg_bytes);

    /**
 * @brief       message update done, get the sm3 digest
 * @param[out]  digest            - sha512_256 digest, 32 bytes.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the digest buffer is sufficient.
  @endverbatim
 */
    unsigned int sm3_final(SM3_CTX *ctx, unsigned char *digest);

    /**
 * @brief       input whole message and get its sm3 digest
 * @param[in]   msg            - message.
 * @param[in]   msg_bytes      - byte length of the input message, it could be 0.
 * @param[out]  digest         - sm3 digest, 32 bytes.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the digest buffer is sufficient.
  @endverbatim
 */
    unsigned int sm3(unsigned char *msg, unsigned int msg_bytes, unsigned char *digest);
    /**
 * @brief        input whole message and get its sm3 digest(node style)
 * @param[in]    node        - input, message node pointer
 * @param[in]    node_num    - input, number of hash nodes, i.e. number of message segments.
 * @param[out]   digest      - output, sm3 digest, 32 bytes
 * @return       0: HASH_SUCCESS(success), other(error)
 * @note
  @verbatim
 *    -# 1. please make sure the digest buffer is sufficient
 *    -# 2. if the whole message consists of some segments, every segment is a node, a node includes
 *        address and byte length.
  @endverbatim
 */
    #ifdef SUPPORT_HASH_NODE
    unsigned int sm3_node_steps(HASH_NODE *node, unsigned int node_num, unsigned char *digest);
    #endif


    #ifdef HASH_DMA_FUNCTION
    /**
 * @brief       init dma sm3
 * @param[in]   ctx           - SM3_DMA_CTX context pointer.
 * @param[in]   callback      - callback function pointer.
 * @return      0:success     other:error
 */
    unsigned int sm3_dma_init(SM3_DMA_CTX *ctx, HASH_CALLBACK callback);

    /**
 * @brief       dma sm3 update some message blocks
 * @param[in]   ctx         - SM3_DMA_CTX context pointer.
 * @param[in]   msg         - message blocks.
 * @param[in]   msg_bytes   - word length of the input message, must be a multiple of sha512
 *                            block word length(32).
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the four parameters are valid, and ctx is initialized.
  @endverbatim
 */
    unsigned int sm3_dma_update_blocks(SM3_DMA_CTX *ctx, unsigned int *msg, unsigned int msg_bytes);

    /**
 * @brief       dma sm3 final(input the remainder message and get the digest)
 * @param[in]   ctx               - SM3_DMA_CTX context pointer.
 * @param[in]   remainder_msg     - remainder message.
 * @param[in]   remainder_bytes   - byte length of the remainder message, must be in [0, BLOCK_BYTE_LEN-1],
 *                                  here BLOCK_BYTE_LEN is block byte length of sm3, it is 64.
 *@param[out]   digest            - sm3 digest, 32 bytes.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the four parameters are valid, and ctx is initialized.
  @endverbatim
  */
    unsigned int sm3_dma_final(SM3_DMA_CTX *ctx, unsigned int *remainder_msg, unsigned int remainder_bytes, unsigned int *digest);

    /**
 * @brief       dma sm3 digest calculate
 * @param[in]   msg           - message.
 * @param[in]   msg_bytes     - byte length of the message, it could be 0.
 * @param[out]  digest        - sm3 digest, 32 bytes.
 * @param[in]   callback      - callback function pointer.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1. please make sure the four parameters are valid.
  @endverbatim
 */
    unsigned int sm3_dma(unsigned int *msg, unsigned int msg_bytes, unsigned int *digest, HASH_CALLBACK callback);

        #ifdef SUPPORT_HASH_DMA_NODE
    /**
 * @brief        input whole message and get its sm3 digest(dma node style)
 * @param[in]    node        - input, message node pointer
 * @param[in]    node_num    - input, number of hash nodes, i.e. number of message segments.
 * @param[out]   digest      - output, sm3 digest, 32 bytes
 * @param[in]    callback    - callback function pointer
 * @return       0: HASH_SUCCESS(success), other(error)
 * @note
  @verbatim
 *     -# 1. please make sure the digest buffer is sufficient
 *     -# 2. if the whole message consists of some segments, every segment is a node, a node includes
 *        address and byte length.
 *     -# 3. for every node or segment except the last, its message length must be a multiple of block length.
  @endverbatim
 */

    unsigned int sm3_dma_node_steps(HASH_DMA_NODE *node, unsigned int node_num, unsigned int *digest, HASH_CALLBACK callback);
        #endif

    #endif

#endif


#ifdef __cplusplus
}
#endif


#endif
