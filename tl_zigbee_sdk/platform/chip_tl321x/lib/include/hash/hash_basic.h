/********************************************************************************************************
 * @file    hash_basic.h
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
    #define HMAC_MAX_KEY_IDX     (8)  //if key is from secure port, the max key index(or the number of keys)
    #define HMAC_MAX_SP_KEY_SIZE (64) //for secure port key, max bytes of one key
#endif


//some register offset
#define HASH_LAST_OFFSET         (24)
#define HASH_DMA_OFFSET          (17)
#define HASH_INTERRUPTION_OFFSET (16)


#if 0
//HASH register struct
typedef struct {
    unsigned int HASH_CTRL;                /* Offset: 0x000 (W1S)  Control register */
    unsigned int HASH_CFG;                 /* Offset: 0x004 (R/W)  Config register */
    unsigned int HASH_SR1;                 /* Offset: 0x008 (R)    Status register 1 */
    unsigned int HASH_SR2;                 /* Offset: 0x00C (W1C)  Status register 2 */
    unsigned int REV_1[4];
    unsigned int HASH_PCR_LEN[4];          /* Offset: 0x020 (R/W)  message length register */
    unsigned int HASH_OUT[16];             /* Offset: 0x030 (R)    Output register */
    unsigned int HASH_IN[16];              /* Offset: 0x070 (W)    Hash iterator Input register */
    unsigned int HASH_VERSION;             /* Offset: 0x0B0 (R)    Version register */
    unsigned int REV_2[19];
    unsigned int HASH_M_DIN[32];           /* Offset: 0x100 (R/W)  Hash message Input register */
    unsigned int DMA_SA;                   /* Offset: 0x180 (R/W)  DMA Source Address register */
    unsigned int DMA_DA;                   /* Offset: 0x184 (R/W)  DMA Destination Address register */
    unsigned int DMA_RLEN;                 /* Offset: 0x188 (R/W)  DMA Input Length register */
    unsigned int DMA_WLEN;                 /* Offset: 0x18C (R/W)  DMA Output Length register */
} HASH_REG;
#else

    #define rHASH_CTRL       (*((volatile unsigned int *)(HASH_BASE_ADDR)))                      /* Offset: 0x000 (W1S)  Control register */
    #define rHASH_CFG        (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x004)))              /* Offset: 0x004 (R/W)  Config register */
    #define rHASH_SR1        (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x008)))              /* Offset: 0x008 (R)    Status register 1 */
    #define rHASH_SR2        (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x00C)))              /* Offset: 0x00C (W1C)  Status register 2 */
    #define rHASH_PCR_LEN(i) (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x020 + ((i) << 2)))) /* Offset: 0x020 (R/W)  message length register, 4 words */
    #define rHASH_OUT(i)     (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x030 + ((i) << 2)))) /* Offset: 0x030 (R)    Output register, 16 words */
    #define rHASH_IN(i)      (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x070 + ((i) << 2)))) /* Offset: 0x070 (W)    Hash iterator Input register, 16 words */
    #define rHASH_VERSION    (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x0B0)))              /* Offset: 0x0B0 (R)    Version register */
    #define rHASH_M_DIN(i)   (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x100 + ((i) << 2)))) /* Offset: 0x100 (R/W)  Hash message Input register, 32 words */
    #define rHASH_DMA_SA     (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x180)))              /* Offset: 0x180 (R/W)  DMA Source Address register */
    #define rHASH_DMA_DA     (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x184)))              /* Offset: 0x184 (R/W)  DMA Destination Address register */
    #define rHASH_DMA_RLEN   (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x188)))              /* Offset: 0x188 (R/W)  DMA Input Length register */
    #define rHASH_DMA_WLEN   (*((volatile unsigned int *)(HASH_BASE_ADDR + 0x18C)))              /* Offset: 0x18C (R/W)  DMA Output Length register */

#endif


//HASH max length
#if (defined(SUPPORT_HASH_SHA384) || defined(SUPPORT_HASH_SHA512) || defined(SUPPORT_HASH_SHA512_224) || defined(SUPPORT_HASH_SHA512_256))
    #define HASH_DIGEST_MAX_WORD_LEN    (16)
    #define HASH_BLOCK_MAX_WORD_LEN     (32)
    #define HASH_TOTAL_LEN_MAX_WORD_LEN (4)
#else
    #define HASH_DIGEST_MAX_WORD_LEN    (8)
    #define HASH_BLOCK_MAX_WORD_LEN     (16)
    #define HASH_TOTAL_LEN_MAX_WORD_LEN (2)
#endif

#define HASH_ITERATOR_MAX_WORD_LEN HASH_DIGEST_MAX_WORD_LEN
#define HASH_BLOCK_MAX_BYTE_LEN    (HASH_BLOCK_MAX_WORD_LEN << 2)

    //HASH algorithm definition
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
    } HASH_ALG;


    //HASH return code
    enum HASH_RET_CODE
    {
        HASH_SUCCESS = 0,
        HASH_BUFFER_NULL,
        HASH_CONFIG_INVALID,
        HASH_INPUT_INVALID,
        HASH_LEN_OVERFLOW,
        HASH_OUTPUT_ZERO_ALL,
        HASH_ERROR,
    };

    //hash callback function type
    typedef void (*HASH_CALLBACK)(void);


#ifndef NULL
    #define NULL 0
#endif

    //APIs
    /**
 * @brief       get HFE IP version.
 * @return      HFE IP version
 */
    unsigned int hash_get_version(void);

    /**
 * @brief       set hash to be CPU mode.
 * @return      none
 */
    void hash_set_cpu_mode(void);

    /**
 * @brief       set hash to be DMA mode.
 * @return      none
 */
    void hash_set_dma_mode(void);

    /**
 * @brief       set the specific hash algorithm.
 * @param[in]   hash_alg   - specific hash algorithm.
 * @return      none
 * @note
  @verbatim
      -# 1. please make sure hash_alg is valid.
  @endverbatim
 */
    void hash_set_alg(HASH_ALG hash_alg);

    /**
 * @brief       enable hash interruption in CPU mode or DMA mode.
 * @return      none
 */
    void hash_enable_interruption(void);

    /**
 * @brief       disable hash interruption in CPU mode or DMA mode.
 * @return      none
 */
    void hash_disable_interruption(void);

    /**
 * @brief       set the tag whether current block is the last message block or not.
 * @param[in]   tag  - 0(no), other(yes).
 * @return      none
 * @note
  @verbatim
      -# 1. if it is the last block, please config hash_reg->HASH_MSG_LEN,
 *        then the hardware will do the padding and post-processing.
  @endverbatim
 */
    void hash_set_last_block(unsigned int tag);

    /**
 * @brief       get current HASH iterator value.
 * @param[in]   iterator  - current hash iterator.
 * @param[in]   iterator word length.
 * @return      none
 */
    void hash_get_iterator(unsigned char *iterator, unsigned int hash_iterator_words);

    /**
 * @brief       input current iterator value.
 * @param[in]   iterator             - hash iterator value.
 * @param[in]   hash_iterator_words  - iterator word length.
 * @return      none
 * @note
  @verbatim
      -# 1. iterator must be word aligned.
  @endverbatim
 */
    void hash_set_iterator(const unsigned int *iterator, unsigned int hash_iterator_words);

    /**
 * @brief       clear rHASH_PCR_LEN.
 * @return      none
 */
    void hash_clear_msg_len(void);

    /**
 * @brief       set the total byte length of the whole message.
 * @param[in]   msg_total_bytes             - total byte length of the whole message.
 * @param[in]   words                       - word length of array msg_total_bytes.
 * @return      none
 */
    void hash_set_msg_total_byte_len(unsigned int *msg_total_bytes, unsigned int words);

    /**
 * @brief       start HASH iteration calc.
 * @return      none
 */
    void hash_start(void);

    /**
 * @brief       wait till done.
 * @return      none
 */
    void hash_wait_till_done(void);

    /**
 * @brief       DMA wait till done.
 * @param[in]   callback             - callback function pointer.
 * @return      none
 */
    void hash_dma_wait_till_done(HASH_CALLBACK callback);
    /**
 * @brief       set dma output bytes length.
 * @param[in]   bytes             - byte length of the written data for hash hardware.
 * @return      none
 */
    void hash_set_dma_output_len(unsigned int bytes);

    /**
 * @brief       input message.
 * @param[in]   msg             - message.
 * @param[in]   msg_bytes       - word length of msg.
 * @return      none
 * @note
  @verbatim
      -# 1. if msg does not contain the last block, please make sure msg_words is a multiple of the
        hash block word length.
  @endverbatim
 */
    void hash_input_msg_u8(const unsigned char *msg, unsigned int msg_bytes);

#ifdef HASH_DMA_FUNCTION
    #ifdef CONFIG_HASH_SUPPORT_ADDRESS_HIGH_LOW

    void hash_dma_operate(unsigned int in_h, unsigned int in_l, unsigned int out_h, unsigned int out_l, unsigned int inByteLen, HASH_CALLBACK callback);
    #else

    /**
 * @brief       input message.
 * @param[in]   in             - message of some blocks, or message including the last byte(last block).
 * @param[out]  out            - hash digest or hmac.
 * @param[in]   inByteLen      - actual byte length of input msg.
 * @param[in]   callback       - callback function pointer.
 * @return      none
 * @note
  @verbatim
      -# 1. for DMA operation, the unit of input and output is 4 words, so, please make sure the buffer
        out is sufficient.
      -# 2. if just to input message, not to get digest or hmac, please set para out to be NULL and WLEN to be 0.
        if to get the digest or hmac, para out can not be NULL, and please set WLEN to be digest length.
  @endverbatim
 */
    void hash_dma_operate(unsigned int *in, unsigned int *out, unsigned int inByteLen, HASH_CALLBACK callback);
    #endif
#endif


#ifdef __cplusplus
}
#endif

#endif
