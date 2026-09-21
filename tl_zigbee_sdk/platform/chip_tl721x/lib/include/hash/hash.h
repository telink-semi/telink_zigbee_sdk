/********************************************************************************************************
 * @file    hash.h
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
#ifndef HASH_H
#define HASH_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "hash_basic.h"

    //#define CONFIG_HASH_SUPPORT_MUL_THREAD


    //HASH status
    typedef struct
    {
        unsigned int busy : 1; // calculate busy flag
    } hash_status_t;

    //HASH context
    typedef struct
    {
#ifdef CONFIG_HASH_SUPPORT_MUL_THREAD
        unsigned int iterator[HASH_ITERATOR_MAX_WORD_LEN];  //keep current hash iterator value for multiple thread
#endif

        unsigned char hash_buffer[HASH_BLOCK_MAX_BYTE_LEN]; //block buffer
        unsigned int  total[HASH_TOTAL_LEN_MAX_WORD_LEN];   //total byte length of the whole message
        HASH_ALG      hash_alg;                             //current hash algorithm
        unsigned char block_byte_len;
        unsigned char iterator_word_len;
        unsigned char digest_byte_len;
        hash_status_t status;            //hash update status, .busy=1 means doing .busy=0 means idle
        unsigned char first_update_flag; //whether first time to update message(1:yes, 0:no)
        unsigned char finish_flag;       //whether the whole message has been inputted(1:yes, 0:no)
    } HASH_CTX;


#ifdef HASH_DMA_FUNCTION
    //HASH DMA context
    typedef struct
    {
        unsigned int  total[HASH_TOTAL_LEN_MAX_WORD_LEN]; //total byte length of the whole message
        HASH_ALG      hash_alg;                           //current hash algorithm
        unsigned char block_word_len;
        HASH_CALLBACK callback;
    } HASH_DMA_CTX;
#endif


    //APIs
    /**
 * @brief       check whether the hash algorithm is valid or not
 * @param[in]   hash_alg            - specific hash algorithm.
 * @return      0:success     other:error
 */
    unsigned int check_hash_alg(HASH_ALG hash_alg);

    /**
 * @brief       get hash block word length
 * @param[in]   hash_alg                    - specific hash algorithm.
 * @return      hash block word length
 * @note
  @verbatim
      -# 1. please make sure hash_alg is valid.
  @endverbatim
 */
    unsigned int hash_get_block_word_len(HASH_ALG hash_alg);

    /**
 * @brief       get hash digest word length
 * @param[in]   hash_alg                    - specific hash algorithm.
 * @return      hash block word length
 * @note
  @verbatim
      -# 1. please make sure hash_alg is valid.
  @endverbatim
 */
    unsigned int hash_get_digest_word_len(HASH_ALG hash_alg);

    /**
 * @brief       get hash iterator word length
 * @param[in]   hash_alg                    - specific hash algorithm.
 * @return      hash block word length
 * @note
  @verbatim
      -# 1. please make sure hash_alg is valid.
  @endverbatim
 */
    unsigned int hash_get_iterator_word_len(HASH_ALG hash_alg);

    /**
 * @brief       init HASH
 * @param[in]   ctx                     - HASH_CTX context pointer.
 * @param[in]   hash_alg                - specific hash algorithm.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.  please make sure hash_alg is valid.
  @endverbatim
 */
    unsigned int hash_init(HASH_CTX *ctx, HASH_ALG hash_alg);

    /**
 * @brief       hash iterate calc with some blocks
 * @param[in]   ctx                - HASH_CTX context pointer.
 * @param[in]   msg                - message.
 * @param[in]   msg_bytes          - byte length of the input message.
 * @return      none
 * @note
  @verbatim
      -# 1.  please make sure the three parameters are valid, and ctx is initialized.
  @endverbatim
 */
    unsigned int hash_update(HASH_CTX *ctx, const unsigned char *msg, unsigned int msg_bytes);

    /**
 * @brief       message update done, get the digest
 * @param[in]   ctx                - HASH_CTX context pointer.
 * @param[out]  digest             - hash digest.
 * @return      none
 * @note
  @verbatim
      -# 1.  please make sure the ctx is valid and initialized.
      -# 2.  please make sure the digest buffer is sufficient.
  @endverbatim
 */
    unsigned int hash_final(HASH_CTX *ctx, unsigned char *digest);

    /**
 * @brief       message update done, get the digest
 * @param[in]   hash_alg           - specific hash algorithm.
 * @param[in]   msg                - message.
 * @param[in]   msg_bytes          - byte length of the input message, it could be 0.
 * @param[out]  digest             - hash digest.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.  please make sure the digest buffer is sufficient.
  @endverbatim
 */
    unsigned int hash(HASH_ALG hash_alg, unsigned char *msg, unsigned int msg_bytes, unsigned char *digest);


#ifdef HASH_DMA_FUNCTION
    /**
 * @brief       message update done, get the digest
 * @param[in]   ctx           - HASH_DMA_CTX context pointer.
 * @param[in]   hash_alg      - specific hash algorithm.
 * @param[in]   callback      - callback function pointer.
 * @return      0:success     other:error
 */
    unsigned int hash_dma_init(HASH_DMA_CTX *ctx, HASH_ALG hash_alg, HASH_CALLBACK callback);

    /**
 * @brief       dma hash update some message blocks
 * @param[in]   ctx                - HASH_DMA_CTX context pointer.
 * @param[in]   msg                - message blocks.
 * @param[in]   msg_words          - word length of the input message, must be a multiple of hash block word length.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.  please make sure the four parameters are valid, and ctx is initialized.
  @endverbatim
 */
    unsigned int hash_dma_update_blocks(HASH_DMA_CTX *ctx, unsigned int *msg, unsigned int msg_words);

    /**
 * @brief       dma hash update some message blocks
 * @param[in]   ctx                - HASH_DMA_CTX context pointer.
 * @param[in]   remainder_msg      - message.
 * @param[in]   remainder_bytes    - byte length of the last message, must be in [0, BLOCK_BYTE_LEN-1],
 *                                  here BLOCK_BYTE_LEN is block byte length of MD5(64)
 * @param[out]   digest            - hash digest
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.  please make sure the four parameters are valid, and ctx is initialized.
  @endverbatim
 */
    unsigned int hash_dma_final(HASH_DMA_CTX *ctx, unsigned int *remainder_msg, unsigned int remainder_bytes, unsigned int *digest);

    /**
 * @brief       dma hash digest calculate
 * @param[in]   hash_alg       - specific hash algorithm.
 * @param[in]   msg            - message.
 * @param[in]   msg_bytes      - byte length of the message, it could be 0.
 * @param[in]   digest         - hash digest.
 * @param[in]   callback       - callback function pointer.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.  please make sure the four parameters are valid.
  @endverbatim
 */
    unsigned int hash_dma(HASH_ALG hash_alg, unsigned int *msg, unsigned int msg_bytes, unsigned int *digest, HASH_CALLBACK callback);
#endif


#ifdef __cplusplus
}
#endif

#endif
