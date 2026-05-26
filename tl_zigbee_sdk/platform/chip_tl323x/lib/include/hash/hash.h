/********************************************************************************************************
 * @file    hash.h
 *
 * @brief   This is the header file for tl323x
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#define CAST2UINT32(a) (unsigned int)(a)

#include "hash_basic.h"

    /**
 * @brief           to calculate hash or hmac
 */
    typedef enum
    {
        HASH_MODE,
        HMAC_MODE
    } hfe_mode_e;

    /**
 * @brief           HASH status
 */
    typedef struct
    {
        unsigned int busy : 1; // calculate busy flag
    } hash_status_t;

    /**
 * @brief           HASH context
 */
    typedef struct
    {
#ifdef CONFIG_HASH_SUPPORT_MUL_THREAD
        unsigned int iterator[HASH_ITERATOR_MAX_WORD_LEN]; //keep current hash iterator value for multiple thread
#endif

        unsigned char hash_buffer[HASH_BLOCK_MAX_BYTE_LEN]; //block buffer
        unsigned int  total[HASH_TOTAL_LEN_MAX_WORD_LEN];   //total byte length of the whole message
        hash_status_t status;                               //hash update status, .busy=1 means doing, .busy=0 means idle
        hash_alg_e    alg;                                  //current hash algorithm
#ifndef CONFIG_SUPPORT_STRUCTURE_OPTIMIZATION
        hfe_mode_e hfe_mode; //actually no use
#endif
        unsigned char block_byte_len;
        unsigned char iterator_word_len;
        unsigned char digest_byte_len;
        unsigned char first_update_flag; //whether first time to update message(1:yes, 0:no)
        unsigned char finish_flag;       //whether the whole message has been inputted(1:yes, 0:no)
    } hash_ctx_t;


#ifdef HASH_DMA_FUNCTION
    /**
 * @brief           HASH DMA context
 */
    typedef struct
    {
#ifdef CONFIG_HASH_SUPPORT_MUL_THREAD
        unsigned int iterator[HASH_ITERATOR_MAX_WORD_LEN]; //keep current hash iterator value for multiple thread
#endif

        unsigned int  total[HASH_TOTAL_LEN_MAX_WORD_LEN]; //total byte length of the whole message
        hash_callback callback;
        hash_alg_e    alg; //current hash algorithm
#ifndef CONFIG_SUPPORT_STRUCTURE_OPTIMIZATION
        hfe_mode_e hfe_mode; //actually no use
#endif
        unsigned char block_word_len;

#ifdef CONFIG_HASH_SUPPORT_MUL_THREAD
        unsigned char iterator_word_len;
        unsigned char first_update_flag; //whether first time to update message(1:yes, 0:no)
#endif
        unsigned char digest_byte_len; //just for hmac
    } hash_dma_ctx_t;
#endif


#ifdef SUPPORT_HASH_NODE
    typedef struct
    {
        const unsigned char *msg_addr;
        unsigned int         msg_len;
    } hash_node_t;
#endif


#ifdef SUPPORT_HASH_DMA_NODE
    typedef struct
    {
        const unsigned int *msg_addr;
        unsigned int        msg_len;
    } hash_dma_node_t;
#endif


    //fix to old project
    typedef hash_status_t   hash_status_t;
    typedef hfe_mode_e      HFE_MODE;
    typedef hash_alg_e      HASH_ALG;
    typedef hash_ctx_t      HASH_CTX;
    typedef hash_dma_ctx_t  HASH_DMA_CTX;
    typedef hash_node_t     HASH_NODE;
    typedef hash_dma_node_t HASH_DMA_NODE;


    //APIs
    /**
 * @brief           check whether the hash algorithm is valid or not
 * @param[in]       alg                  - specific hash algorithm
 * @return          HASH_SUCCESS(valid), other(invalid)
 */
    unsigned int check_hash_alg(hash_alg_e alg);

    /**
 * @brief           get hash block word length
 * @param[in]       alg                  - specific hash algorithm
 * @return          hash block word length
 * @note            
 *        1. please make sure alg is valid
 */
    unsigned char hash_get_block_word_len(hash_alg_e alg);

    /**
 * @brief           get hash iterator word length
 * @param[in]       alg                  - specific hash algorithm
 * @return          hash iterator word length
 * @note            
 *        1. please make sure alg is valid
 */
    unsigned char hash_get_iterator_word_len(hash_alg_e alg);

    /**
 * @brief           get hash digest word length
 * @param[in]       alg                  - specific hash algorithm
 * @return          hash digest word length
 * @note            
 *        1. please make sure alg is valid
 */
    unsigned char hash_get_digest_word_len(hash_alg_e alg);

    /**
 * @brief           get hash IV pointer
 * @param[in]       alg                  - specific hash algorithm
 * @return          IV address
 */
    const unsigned int *hash_get_iv(hash_alg_e alg);

    /**
 * @brief           input hash IV
 * @param[in]       alg                  - specific hash algorithm
 * @param[in]       hash_iterator_words  - iterator word length
 * @return          none
 */
    void hash_set_iv(hash_alg_e alg, unsigned int hash_iterator_words);

    /**
 * @brief           hash message total byte length a = a+b
 * @param[in,out]   a                    - big number a, total byte length of hash message
 * @param[in]       a_words              - word length of buffer a
 * @param[in]       b                    - integer to be added to a
 * @return          0(success), other(error, hash total length overflow)
 */
    unsigned int hash_total_byte_len_add_uint32(unsigned int *a, unsigned int a_words, unsigned int b);


    /**
 * @brief           start HASH iteration calc
 * @param[in]       ctx                  - hash_ctx_t context pointer
 * @return          none
 */
    void hash_start_calculate(hash_ctx_t *ctx);

    /**
 * @brief           hash iterate calc with some blocks
 * @param[in]       ctx                  - hash_ctx_t context pointer
 * @param[in]       msg                  - message of some blocks
 * @param[in]       block_count          - count of blocks
 * @return          none
 * @note            
 *        1. please make sure the three parameters is valid
 */
    void hash_calc_blocks(hash_ctx_t *ctx, const unsigned char *msg, unsigned int block_count);

    /**
 * @brief           hash iterate calc with padding
 * @param[in]       ctx                  - hash_ctx_t context pointer
 * @param[in]       msg                  - message that contains the last block(maybe not full)
 * @param[in]       msg_len              - byte length of msg
 * @return          none
 * @note            
 *        1. msg contains the last byte of the total message while the total message length is not a
 *           multiple of hash block length, otherwise byte length of msg is zero.
 *        2. at present this function does not support the case that byte length of msg is a multiple
 *           of hash block length. actually msg_len here must be less than the hash block byte length,
 *           namely, this function is just for the remainder message, and will do padding, finally get
 *           digest.
 *        3. before calling this function, some blocks(could be 0 block) must be calculated
 */
    void hash_calc_rand_len_msg(hash_ctx_t *ctx, const unsigned char *msg, unsigned int msg_len);


    /**
 * @brief           init HASH with iv and updated message length
 * @param[in]       ctx                  - hash_ctx_t context pointer
 * @param[in]       alg                  - specific hash algorithm
 * @param[in]       iv                   - iv or iterator after updating some blocks
 * @param[in]       byte_length_h        - high 32 bit of updated message byte length
 * @param[in]       byte_length_l        - low 32 bit of updated message byte length,
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure alg is valid
 *        2. updated message byte length must be a multiple of block byte length
 */
    unsigned int hash_init_with_iv_and_updated_length(hash_ctx_t *ctx, hash_alg_e alg, const unsigned int *iv, unsigned int byte_length_h, unsigned int byte_length_l);

    /**
 * @brief           init HASH
 * @param[in]       ctx                  - hash_ctx_t context pointer
 * @param[in]       alg                  - specific hash algorithm
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure alg is valid
 */
    unsigned int hash_init(hash_ctx_t *ctx, hash_alg_e alg);

    /**
 * @brief           hash update message
 * @param[in]       ctx                  - hash_ctx_t context pointer
 * @param[in]       msg                  - message
 * @param[in]       msg_len            - byte length of the input message
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the three parameters are valid, and ctx is initialize
 */
    unsigned int hash_update(hash_ctx_t *ctx, const unsigned char *msg, unsigned int msg_len);

    /**
 * @brief           message update done, get the digest
 * @param[in]       ctx                  - hash_ctx_t context pointer
 * @param[out]      digest               - hash digest
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the ctx is valid and initialized
 *        2. please make sure the digest buffer is sufficient
 */
    unsigned int hash_final(hash_ctx_t *ctx, unsigned char *digest);

    /**
 * @brief           input whole message and get its digest
 * @param[in]       alg                  - specific hash algorithm
 * @param[in]       msg                  - message
 * @param[in]       msg_len            - byte length of the input message, it could be 0
 * @param[out]      digest               - hash digest
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the digest buffer is sufficient
 */
    unsigned int hash(hash_alg_e alg, const unsigned char *msg, unsigned int msg_len, unsigned char *digest);

#ifdef SUPPORT_HASH_NODE
    /**
 * @brief           input whole message and get its digest(node style)
 * @param[in]       alg                  - specific hash algorithm
 * @param[in]       node                 - message node pointer
 * @param[in]       node_num             - number of hash nodes, i.e. number of message segments.
 * @param[out]      digest               - hash digest
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the digest buffer is sufficient
 *        2. if the whole message consists of some segments, every segment is a node, a node includes
 *           address and byte length
 */
    unsigned int hash_node_steps(hash_alg_e alg, const hash_node_t *node, unsigned int node_num, unsigned char *digest);
#endif


#ifdef HASH_DMA_FUNCTION
    /**
 * @brief           init dma hash with iv and updated message length
 * @param[in]       ctx                  - hash_dma_ctx_t context pointer
 * @param[in]       alg                  - specific hash algorithm
 * @param[in]       iv                   - iv or iterator after updating some blocks
 * @param[in]       byte_length_h        - high 32 bit of updated message byte length
 * @param[in]       byte_length_l        - low 32 bit of updated message byte length,
 * @param[in]       callback             - callback function pointer
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure alg is valid
 *        2. updated message byte length must be a multiple of block byte length
 */
    unsigned int hash_dma_init_with_iv_and_updated_length(hash_dma_ctx_t *ctx, hash_alg_e alg, const unsigned int *iv, unsigned int byte_length_h, unsigned int byte_length_l,
                                                          hash_callback callback);

    /**
 * @brief           init dma hash
 * @param[in]       ctx                  - hash_dma_ctx_t context pointer
 * @param[in]       alg                  - specific hash algorithm
 * @param[in]       callback             - callback function pointer
 * @return          HASH_SUCCESS(success), other(error)
 */
    unsigned int hash_dma_init(hash_dma_ctx_t *ctx, hash_alg_e alg, hash_callback callback);

    /**
 * @brief           dma hash update some message blocks
 * @param[in]       ctx                  - hash_dma_ctx_t context pointer
 * @param[in]       msg                  - message blocks
 * @param[in]       msg_len            - byte length of the input message, must be a multiple of hash block byte length
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the four parameters are valid, and ctx is initialize
 */
    unsigned int hash_dma_update_blocks(hash_dma_ctx_t *ctx, const unsigned int *msg, unsigned int msg_len);

    /**
 * @brief           dma hash final(input the remainder message and get the digest)
 * @param[in]       ctx                  - hash_dma_ctx_t context pointer
 * @param[in]       msg                  - remainder message
 * @param[in]       msg_len              - byte length of the remainder message
 * @param[out]      digest               - hash digest
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the four parameters are valid, and ctx is initialized
 *        2. if remainder_msg is NULL, or remainder_bytes is zero, in this case input valid,
 *           means the message is NULL
 */
    unsigned int hash_dma_final(hash_dma_ctx_t *ctx, const unsigned int *msg, unsigned int msg_len, unsigned int *digest);

    /**
 * @brief           dma hash digest calculate
 * @param[in]       alg                  - specific hash algorithm
 * @param[in]       msg                  - message
 * @param[in]       msg_len            - byte length of the message, it could be 0
 * @param[out]      digest               - hash digest
 * @param[in]       callback             - callback function pointer
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the four parameters are valid
 */
    unsigned int hash_dma(hash_alg_e alg, unsigned int *msg, unsigned int msg_len, unsigned int *digest, hash_callback callback);

#ifdef SUPPORT_HASH_DMA_NODE
    /**
 * @brief           input whole message and get its digest(dma node style)
 * @param[in]       alg                  - specific hash algorithm
 * @param[in]       node                 - message node pointer
 * @param[in]       node_num             - number of hash nodes, i.e. number of message segments.
 * @param[out]      digest               - hash digest
 * @param[in]       callback             - callback function pointer
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the digest buffer is sufficient
 *        2. if the whole message consists of some segments, every segment is a node, a node includes
 *           address and byte length.
 *        3. for every node or segment except the last, its message length must be a multiple of block length
 */
    unsigned int hash_dma_node_steps(hash_alg_e alg, const hash_dma_node_t *node, unsigned int node_num, unsigned int *digest, hash_callback callback);
#endif
#endif


#ifdef __cplusplus
}
#endif

#endif
