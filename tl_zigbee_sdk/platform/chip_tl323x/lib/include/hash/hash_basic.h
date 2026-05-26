/********************************************************************************************************
 * @file    hash_basic.h
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
#ifndef HASH_BASIC_H
#define HASH_BASIC_H

#include "reg_include/hash_reg.h"
#include "dma.h"
#include "lib/include/hash/hash_config.h"


#ifdef __cplusplus
extern "C"
{
#endif


#ifdef HMAC_SECURE_PORT_FUNCTION
#define HMAC_MAX_K_IDX       (8)  //if key is from secure port, the max key index(or the number of keys)
#define HMAC_MAX_SP_KEY_SIZE (64) //for secure port key, max bytes of one key
#endif


//some register offset
#define HASH_LAST_OFFSET         (24)
#define HASH_DMA_OFFSET          (17)
#define HASH_INTERRUPTION_OFFSET (16)


#define rHASH_CTRL               (*((volatile unsigned int *)(HASH_BASE_ADDR)))                       /* Offset: 0x000 (W1S)  Control register */
#define rHASH_CFG                (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x004U)))              /* Offset: 0x004 (R/W)  Config register */
#define rHASH_SR1                (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x008U)))              /* Offset: 0x008 (R)    Status register 1 */
#define rHASH_SR2                (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x00CU)))              /* Offset: 0x00C (W1C)  Status register 2 */
#define rHASH_PCR_LEN(i)         (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x020U + ((i) << 2)))) /* Offset: 0x020 (R/W)  message length register, 4 words */
#define rHASH_OUT(i)             (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x030U + ((i) << 2)))) /* Offset: 0x030 (R)    Output register, 16 words */
#define rHASH_IN(i)              (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x070U + ((i) << 2)))) /* Offset: 0x070 (W)    Hash iterator Input register, 16 words */
#define rHASH_VERSION            (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x0B0U)))              /* Offset: 0x0B0 (R)    Version register */
#define rHASH_M_DIN(i)           (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x100U + ((i) << 2)))) /* Offset: 0x100 (R/W)  Hash message Input register, 32 words */
#define rHASH_DMA_SA_L           (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x180U)))              /* Offset: 0x180 (R/W)  DMA Source Address register, Low 32bit*/
#define rHASH_DMA_DA_L           (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x184U)))              /* Offset: 0x184 (R/W)  DMA Destination Address register, Low 32 bit*/
#define rHASH_DMA_SA_H           (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x190U)))              /* Offset: 0x180 (R/W)  DMA Source Address register, High 32 bit*/
#define rHASH_DMA_DA_H           (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x194U)))              /* Offset: 0x184 (R/W)  DMA Destination Address register, High 32 bit*/
#define rHASH_DMA_RLEN           (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x188U)))              /* Offset: 0x188 (R/W)  DMA Input Length register */
#define rHASH_DMA_WLEN           (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x18CU)))              /* Offset: 0x18C (R/W)  DMA Output Length register */


//HASH max length
#if (defined(SUPPORT_HASH_SHA384) || defined(SUPPORT_HASH_SHA512) || defined(SUPPORT_HASH_SHA512_224) || defined(SUPPORT_HASH_SHA512_256))
#define HASH_DIGEST_MAX_WORD_LEN    (16U)
#define HASH_BLOCK_MAX_WORD_LEN     (32U)
#define HASH_TOTAL_LEN_MAX_WORD_LEN (4U)
#else
#define HASH_DIGEST_MAX_WORD_LEN    (8U)
#define HASH_BLOCK_MAX_WORD_LEN     (16U)
#define HASH_TOTAL_LEN_MAX_WORD_LEN (2U)
#endif

#define HASH_ITERATOR_MAX_WORD_LEN HASH_DIGEST_MAX_WORD_LEN
#define HASH_BLOCK_MAX_BYTE_LEN    (HASH_BLOCK_MAX_WORD_LEN << 2)

    /**
 * @brief           HASH algorithm definition
 */
    typedef enum
    {
#ifdef SUPPORT_HASH_SM3
        HASH_SM3 = 0,
#endif

#ifdef SUPPORT_HASH_MD5
        HASH_MD5 = 1,
#endif

#ifdef SUPPORT_HASH_SHA256
        HASH_SHA256 = 2,
#endif

#ifdef SUPPORT_HASH_SHA384
        HASH_SHA384 = 3,
#endif

#ifdef SUPPORT_HASH_SHA512
        HASH_SHA512 = 4,
#endif

#ifdef SUPPORT_HASH_SHA1
        HASH_SHA1 = 5,
#endif

#ifdef SUPPORT_HASH_SHA224
        HASH_SHA224 = 6,
#endif

#ifdef SUPPORT_HASH_SHA512_224
        HASH_SHA512_224 = 7,
#endif

#ifdef SUPPORT_HASH_SHA512_256
        HASH_SHA512_256 = 8,
#endif

#ifdef SUPPORT_HASH_SHA3_224
        HASH_SHA3_224 = 9,
#endif

#ifdef SUPPORT_HASH_SHA3_256
        HASH_SHA3_256 = 10,
#endif

#ifdef SUPPORT_HASH_SHA3_384
        HASH_SHA3_384 = 11,
#endif

#ifdef SUPPORT_HASH_SHA3_512
        HASH_SHA3_512 = 12,
#endif
    } hash_alg_e;


//HASH return code
#define HASH_RT_OFFSET       (0x400U)
#define HASH_SUCCESS         (0U)
#define HASH_BUFFER_NULL     (HASH_RT_OFFSET + 1U)
#define HASH_CONFIG_INVALID  (HASH_RT_OFFSET + 2U)
#define HASH_INPUT_INVALID   (HASH_RT_OFFSET + 3U)
#define HASH_LEN_OVERFLOW    (HASH_RT_OFFSET + 4U)
#define HASH_OUTPUT_ZERO_ALL (HASH_RT_OFFSET + 5U)
#define HASH_ERROR           (HASH_RT_OFFSET + 10U)


    //hash callback function type
    typedef void (*hash_callback)(void);


    /**
 * @brief           get HFE IP version
 * @return          HFE IP version
 */
    unsigned int hash_get_version(void);

    /**
 * @brief           get hash driver version
 * @return          hash driver version(software version)
 */
    unsigned int hash_get_driver_version(void);

    /**
 * @brief           set hash to be CPU mode
 * @return          none
 */
    void hash_set_cpu_mode(void);

    /**
 * @brief           set hash to be DMA mode
 * @return          none
 */
    void hash_set_dma_mode(void);

    /**
 * @brief           set the specific hash algorithm
 * @param[in]       alg                  - specific hash algorithm
 * @return          none
 * @note            
 *        1. please make sure alg is valid
 */
    void hash_set_alg(hash_alg_e alg);

    /**
 * @brief           enable hash interruption in CPU mode or DMA mode
 * @return          none
 * @note            
 */
    void hash_enable_interruption(void);

    /**
 * @brief           disable hash interruption in CPU mode or DMA mode
 * @return          none
 * @note            
 */
    void hash_disable_interruption(void);

    /**
 * @brief           set the tag whether current block is the last message block or not
 * @param[in]       tag                  - 0(no), other(yes)
 * @return          none
 * @note            
 *        1. if it is the last block, please config rHASH_MSG_LEN,
 *           then the hardware will do the padding and post-processing
 */
    void hash_set_last_block(unsigned int tag);

    /**
 * @brief           get current HASH iterator value
 * @param[out]      iterator             - current hash iterator
 * @param[in]       hash_iterator_words  - iterator word length
 * @return          none
 */
    void hash_get_iterator(unsigned char *iterator, unsigned int hash_iterator_words);

    /**
 * @brief           input current iterator value
 * @param[in]       iterator             - hash iterator value
 * @param[in]       hash_iterator_words  - iterator word length
 * @return          none
 * @note            
 *        1. iterator must be word aligned
 */
    void hash_set_iterator(const unsigned int *iterator, unsigned int hash_iterator_words);

    /**
 * @brief           clear rHASH_PCR_LEN
 * @return          none
 * @note            
 */
    void hash_clear_msg_len(void);

    /**
 * @brief           set the total byte length of the whole message
 * @param[in]       msg_total_bytes      - total byte length of the whole message
 * @param[in]       words                - word length of array msg_total_bytes
 * @return          none
 */
    void hash_set_msg_total_byte_len(unsigned int *msg_total_bytes, unsigned int words);

    /**
 * @brief           set dma output bytes length
 * @param[in]       bytes                - byte length of the written data for hash hardware
 * @return          none
 */
    void hash_set_dma_output_len(unsigned int bytes);

    /**
 * @brief           start HASH iteration calc
 * @return          none
 */
    void hash_start(void);

    /**
 * @brief           wait till done
 * @return          none
 */
    void hash_wait_till_done(void);

    /**
 * @brief           DMA wait till done
 * @param[in]       callback             - callback function pointer
 * @return          none
 */
    void hash_dma_wait_till_done(hash_callback callback);

    /**
 * @brief           input message(at most a block)
 * @param[in]       msg                  - message
 * @param[in]       msg_len            - byte length of msg, can not be greater than block bytes
 * @return          none
 * @note            
 *        1. msg_len can not be greater than block byte
 */
    void hash_input_msg_u8(const unsigned char *msg, unsigned int msg_len);

#ifdef HASH_DMA_FUNCTION
    /**
 * @brief           basic HASH DMA operation
 * @param[in]       in                   - message of some blocks, or message including the last byte(last block)
 * @param[out]      out                  - hash digest or hmac.
 * @param[in]       inByteLen            - actual byte length of input msg
 * @param[in]       callback             - callback function pointer
 * @return          none
 * @note            
 *        1. for DMA operation, the unit of input and output is 4 words, so, please make sure the buffer
 *           out is sufficient.
 *        2. if just to input message, not to get digest or hmac, please set para out to be NULL and WLEN to be 0.
 *           if to get the digest or hmac, para out can not be NULL, and please set WLEN to be digest length
 */
    void hash_dma_operate(const unsigned int *in, const unsigned int *out, unsigned int inByteLen, hash_callback callback);
#endif


#ifdef __cplusplus
}
#endif

#endif
