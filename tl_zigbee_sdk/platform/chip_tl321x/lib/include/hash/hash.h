/********************************************************************************************************
 * @file    hash.h
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
#ifndef HASH_H
#define HASH_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "hash_basic.h"

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
    } __attribute__((packed, aligned(4))) HASH_CTX;


#ifdef HASH_DMA_FUNCTION
    //HASH DMA context
    typedef struct
    {
    #ifdef CONFIG_HASH_SUPPORT_MUL_THREAD
        unsigned int iterator[HASH_ITERATOR_MAX_WORD_LEN]; //keep current hash iterator value for multiple thread
    #endif

        unsigned int  total[HASH_TOTAL_LEN_MAX_WORD_LEN];  //total byte length of the whole message
        HASH_CALLBACK callback;
        HASH_ALG      hash_alg;                            //current hash algorithm
        unsigned char block_word_len;

    #ifdef CONFIG_HASH_SUPPORT_MUL_THREAD
        unsigned char iterator_word_len;
        unsigned char first_update_flag; //whether first time to update message(1:yes, 0:no)
    #endif
        unsigned char digest_byte_len;   //just for hmac
    } HASH_DMA_CTX;
#endif


#ifdef SUPPORT_HASH_NODE
    typedef struct
    {
        unsigned char *msg_addr;
        unsigned int   msg_bytes;
    } HASH_NODE;
#endif


#ifdef SUPPORT_HASH_DMA_NODE
    typedef struct
    {
    #ifdef CONFIG_HASH_SUPPORT_ADDRESS_HIGH_LOW
        unsigned int msg_addr_h;
        unsigned int msg_addr_l;
    #else
        unsigned int *msg_addr;
    #endif
        unsigned int msg_bytes;
    } HASH_DMA_NODE;
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
    unsigned char hash_get_block_word_len(HASH_ALG hash_alg);

    /**
 * @brief       get hash digest word length
 * @param[in]   hash_alg                    - specific hash algorithm.
 * @return      hash block word length
 * @note
  @verbatim
      -# 1. please make sure hash_alg is valid.
  @endverbatim
 */
    unsigned char hash_get_digest_word_len(HASH_ALG hash_alg);

    /**
 * @brief       get hash iterator word length
 * @param[in]   hash_alg                    - specific hash algorithm.
 * @return      hash block word length
 * @note
  @verbatim
      -# 1. please make sure hash_alg is valid.
  @endverbatim
 */
    unsigned char hash_get_iterator_word_len(HASH_ALG hash_alg);

    /**
 * @brief       get hash IV pointer
 * @param[in]   hash_alg                    - specific hash algorithm.
 * @return      IV address
 */
    unsigned int *hash_get_IV(HASH_ALG hash_alg);

    /**
 * @brief       input hash IV
 * @param[in]   hash_alg                    - specific hash algorithm.
 * @param[in]   hash_iterator_words         - iterator word length.
 * @return      none
 */
    void hash_set_IV(HASH_ALG hash_alg, unsigned int hash_iterator_words);

    /**
 * @brief       hash message total byte length a = a+b
 * @param[in]   a                - big number a, total byte length of hash message.
 * @param[in]   a_words          - word length of buffer a.
 * @param[in]   b                - integer to be added to a.
 * @return      0:success     other(error, hash total length overflow)
 */
    unsigned int hash_total_byte_len_add_uint32(unsigned int *a, unsigned int a_words, unsigned int b);

    /**
 * @brief       start HASH iteration calc
 * @param[in]   ctx              - HASH_CTX context pointer
 * @return      none
 */
    void hash_start_calculate(HASH_CTX *ctx);

    /**
 * @brief       hash iterate calc with some blocks
 * @param[in]   ctx                     - HASH_CTX context pointer.
 * @param[in]   msg                    - message of some blocks.
 * @param[in]   block_count             - count of blocks.
 * @return      none
 * @note
  @verbatim
      -# 1.  please make sure the three parameters is valid.
  @endverbatim
 */
    void hash_calc_blocks(HASH_CTX *ctx, const unsigned char *msg, unsigned int block_count);

    /**
 * @brief       hash iterate calc with padding
 * @param[in]   ctx                - HASH_CTX context pointer.
 * @param[in]   msg               - message that contains the last block(maybe not full).
 * @param[in]   msg_bytes          - byte length of msg.
 * @return      none
 * @note
  @verbatim
      -# 1.  msg contains the last byte of the total message while the total message length is not a
        multiple of hash block length, otherwise byte length of msg is zero.
      -# 2.  at present this function does not support the case that byte length of msg is a multiple
        of hash block length. actually msg_bytes here must be less than the hash block byte length,
        namely, this function is just for the remainder message, and will do padding, finally get
        digest.
      -# 3.  before calling this function, some blocks(could be 0 block) must be calculated.
  @endverbatim
 */
    void hash_calc_rand_len_msg(HASH_CTX *ctx, unsigned char *msg, unsigned int msg_bytes);

    /**
 * @brief       init HASH with iv and updated message length
 * @param[in]   ctx                - HASH_CTX context pointer.
 * @param[in]   hash_alg           - specific hash algorithm.
 * @param[in]   iv                 - iv or iterator after updating some blocks.
 * @param[in]   byte_length_h        - high 32 bit of updated message byte length.
 * @param[in]   byte_length_l      - ow 32 bit of updated message byte length,
 *                                   this must be a multiple of block byte length
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.  please make sure the four parameters are valid.
      -# 2.  updated message byte length must be a multiple of block byte length
  @endverbatim
 */
    unsigned int hash_init_with_iv_and_updated_length(HASH_CTX *ctx, HASH_ALG hash_alg, unsigned int *iv, unsigned int byte_length_h, unsigned int byte_length_l);

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

#ifdef SUPPORT_HASH_NODE
    /**
 * @brief       input whole message and get its digest(node style)
 * @param[in]   hash_alg            - specific hash algorithm.
 * @param[in]   node                - message node pointer.
 * @param[in]   node_num            - number of hash nodes, i.e. number of message segments.
 * @param[in]   digest              - hash digest
 * @return      0:success     other:error
   @verbatim
      -# 1.  please make sure the digest buffer is sufficient.
      -# 2.  if the whole message consists of some segments, every segment is a node, a node includes address and byte length.
   @endverbatim
 */
    unsigned int hash_node_steps(HASH_ALG hash_alg, HASH_NODE *node, unsigned int node_num, unsigned char *digest);
#endif


#ifdef HASH_DMA_FUNCTION
    /**
 * @brief       dma hash digest calculate
 * @param[in]   ctx                 - HASH_DMA_CTX context pointer.
 * @param[in]   hash_alg            - specific hash algorithm.
 * @param[in]   iv                    - iv or iterator after updating some blocks.
 * @param[in]   byte_length_h       - high 32 bit of updated message byte length.
 * @param[in]   byte_length_l       - low 32 bit of updated message byte length,
 *                                    this must be a multiple of block byte length.
 * @param[in]   callback            - callback function pointer.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.  please make sure the four parameters are valid.
      -# 2. updated message byte length must be a multiple of block byte length
  @endverbatim
 */
    unsigned int hash_dma_init_with_iv_and_updated_length(HASH_DMA_CTX *ctx, HASH_ALG hash_alg, unsigned int *iv, unsigned int byte_length_h, unsigned int byte_length_l, HASH_CALLBACK callback);

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
 * @param[in]   msg               - message blocks.
 * @param[in]   msg_bytes           - word length of the input message, must be a multiple of hash block word length.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.  please make sure the four parameters are valid, and ctx is initialized.
  @endverbatim
 */
    unsigned int hash_dma_update_blocks(HASH_DMA_CTX *ctx, unsigned int *msg, unsigned int msg_bytes);
    /**
 * @brief       dma hash final(input the remainder message and get the digest)
 * @param[in]   ctx                - HASH_DMA_CTX context pointer.
 * @param[in]   remainder_msg      - remainder message.
 * @param[in]   remainder_bytes    - byte length of the remainder message.
 * @param[out]  digest             - hash digest
 * @return      HASH_SUCCESS(success), other(error)
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
 * @param[in]   digest           - hash digest.
 * @param[in]   callback       - callback function pointer.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.  please make sure the four parameters are valid.
  @endverbatim
 */
    unsigned int hash_dma(HASH_ALG hash_alg, unsigned int *msg, unsigned int msg_bytes, unsigned int *digest, HASH_CALLBACK callback);
    #ifdef CONFIG_HASH_SUPPORT_ADDRESS_HIGH_LOW
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
    unsigned int hash_dma_update_blocks(HASH_DMA_CTX *ctx, unsigned int *msg, unsigned int msg_bytes);

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


        #ifdef SUPPORT_HASH_DMA_NODE
    unsigned int hash_dma_node_steps(HASH_ALG hash_alg, HASH_DMA_NODE *node, unsigned int node_num, unsigned int digest_h, unsigned int digest_l, HASH_CALLBACK callback);
        #endif
    #else

        #ifdef SUPPORT_HASH_DMA_NODE
    /**
 * @brief       input whole message and get its digest(dma node style)
 * @param[in]   hash_alg       - specific hash algorithm.
 * @param[in]   node           - message node pointer.
 * @param[in]   node_num       - number of hash nodes, i.e. number of message segments.
 * @param[out]   digest           - hash digest.
 * @param[in]   callback       - callback function pointer.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.  please make sure the four parameters are valid.
      -# 2.  if the whole message consists of some segments, every segment is a node, a node includes
             address and byte length.
      -# 3.  for every node or segment except the last, its message length must be a multiple of block length.
  @endverbatim
 */
    unsigned int hash_dma_node_steps(HASH_ALG hash_alg, HASH_DMA_NODE *node, unsigned int node_num, unsigned int *digest, HASH_CALLBACK callback);
        #endif
    #endif
#endif


#ifdef __cplusplus
}
#endif

#endif
