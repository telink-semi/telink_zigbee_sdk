/********************************************************************************************************
 * @file    trng_basic.h
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
#ifndef TRNG_BASIC_H
#define TRNG_BASIC_H


#include "lib/include/trng/trng_config.h"


#ifdef __cplusplus
extern "C" {
#endif




//TRNG register address
#define rTRNG_CR             (*((volatile unsigned int *)(TRNG_BASE_ADDR + 0x0000U)))
#define rTRNG_MSEL           (*((volatile unsigned int *)(TRNG_BASE_ADDR + 0x0004U)))
#define rTRNG_SR             (*((volatile unsigned int *)(TRNG_BASE_ADDR + 0x0008U)))
#define rTRNG_DR             (*((volatile unsigned int *)(TRNG_BASE_ADDR + 0x000CU)))
#define rTRNG_RESEED         (*((volatile unsigned int *)(TRNG_BASE_ADDR + 0x0010U)))
#define rRO_CLK_EN           (*((volatile unsigned int *)(TRNG_BASE_ADDR + 0x0014U)))
#define rRO_SRC_EN1          (*((volatile unsigned int *)(TRNG_BASE_ADDR + 0x0018U)))
#define rRO_SRC_EN2          (*((volatile unsigned int *)(TRNG_BASE_ADDR + 0x001CU)))
#define rTRNG_HT_CR          (*((volatile unsigned int *)(TRNG_BASE_ADDR + 0x0020U)))
#define rTRNG_HT_SR          (*((volatile unsigned int *)(TRNG_BASE_ADDR + 0x0024U)))
#define rTRNG_VERSION        (*((volatile unsigned int *)(TRNG_BASE_ADDR + 0x0030U)))


//TRNG freq config
#define TRNG_RO_FREQ_4                  (0U)
#define TRNG_RO_FREQ_8                  (1U)
#define TRNG_RO_FREQ_16                 (2U)
#define TRNG_RO_FREQ_32                 (3U)     //default


//TRNG action offset
#define TRNG_GLOBAL_INT_OFFSET          (24U)
#define TRNG_READ_EMPTY_INT_OFFSET      (17U)
#define TRNG_DATA_INT_OFFSET            (16U)
#define TRNG_FREQ_OFFSET                (16U)
#define TRNG_SELF_TEST_READY_OFFSET     (3U)



//TRNG return code
#define TRNG_SUCCESS                    (0U)
#define TRNG_BUFFER_NULL                (1U)
#define TRNG_INVALID_INPUT              (2U)
#define TRNG_INVALID_CONFIG             (3U)
#define TRNG_HT_ERROR                   (4U)
#define TRNG_TIMEOUT_ERROR              (5U)
#define TRNG_ERROR                      (6U)



typedef unsigned int (*GET_RAND_WORDS)(unsigned int *a, unsigned int words);


//API
/**
 * @brief   This function serves to get trng IP version.
 * @return  trng IP version(hardware version)
 */
unsigned int trng_get_version(void);

/**
 * @brief   This function serves to get trng driver version.
 * @return  trng driver version(software version)
 */
unsigned int trng_get_driver_version(void);

/**
 * @brief   TRNG global interruption enable.
 * @return  none
 */
void trng_global_int_enable(void);

/**
 * @brief   TRNG global interruption disable
 * @return  none
 */
void trng_global_int_disable(void);

/**
 * @brief       TRNG empty-read interruption enable.
 * @return      none
 * @note
  @verbatim
      -# 1. works when global interruption is enabled.
  @endverbatim
 */
void trng_empty_read_int_enable(void);

/**
 * @brief       TRNG global interruption disable
 * @return      none
 */
void trng_empty_read_int_disable(void);

/**
 * @brief       TRNG data interruption enable.
 * @return      none
 * @note
  @verbatim
      -# 1. works when global interruption is enabled.
  @endverbatim
 */
void trng_data_int_enable(void);

/**
 * @brief       TRNG data interruption disable
 * @return      none
 */
void trng_data_int_disable(void);

/**
 * @brief       TRNG enable
 * @return      none
 */
void trng_enable(void);

/**
 * @brief       TRNG enable
 * @return      none
 */
void trng_disable(void);



#ifdef TRNG_RO_ENTROPY

/**
 * @brief: check if TRNG self test ready.
 * parameters:
 * return: 1(ready), 0(not ready)
 * caution:
 *     1. this works while i_skip_startup is 0. if i_skip_startup is 1, no
 *        need to check this.
 */
unsigned int trng_if_self_test_ready(void);

/**
 * @brief       set RO entropy config
 * @param[in]   cfg       - RO entropy config, only the low 4 bits are valid, every bit,indicates one RO entropy, the MSB is RO 4, and LSB is RO 1.
 * @return      TRNG_SUCCESS(success), other(error)
 * @note
  @verbatim
      -# 1. only the low 4 bits of cfg are valid.
      -# 2. if the low 4 bits of cig is 0, that means to disable all RO entropy.
      -# 3. Turning on 4 ro entropy sources will have better randomization performance,
            but as the number of entropy sources turned on increases, the power consumption
            of the trng module will also increase..
  @endverbatim
 */
unsigned int trng_ro_entropy_config(unsigned char cfg);

/**
 * @brief       set sub RO entropy config
 * @param[in]   sn        - RO entropy source series number, must be in [1,4].
 * @param[in]   cfg       - the config value of RO sn.
 * @return      TRNG_SUCCESS(success), other(error)
 */
unsigned int trng_ro_sub_entropy_config(unsigned char sn, unsigned short cfg);

/**
 * @brief       set TRNG mode
 * @param[in]   with_post_processing       - 0:no,  other:yes
 * @return      none
 * @note
  @verbatim
      -# 1. True random mode when set to 0, pseudo-random mode when set to a non-zero value.
  @endverbatim
 */
void trng_set_mode(unsigned char with_post_processing);

/**
 * @brief       reseed TRNG(works when DRBG is enabled)
 * @return      none
 * @note
  @verbatim
      -# 1. used for DRBG
  @endverbatim
 */
void trng_reseed(void);

/**
 * @brief       TRNG set frequency
 * @param[in]   freq       frequency config, must be in [0,3], and
 *                                  0: 1/4 of input frequency, the lower the sample frequency, the better the randomization performance,
 *                                  1: 1/8 ...,
 *                                  2: 1/16 ...,
 *                                  3: 1/32 ...,
 * @return      TRNG_SUCCESS(success), other(error)
 */
unsigned int trng_set_freq(unsigned char freq);

/**
 * @brief       get some rand words
 * @param[in]   a       random words
 * @param[in]   words   word number of output, must be in [1, 8]
 * @return      TRNG_SUCCESS(success), other(error)
 * @note
  @verbatim
      -# 1. used for DRBG
  @endverbatim
 */
unsigned int get_rand_uint32(unsigned int *a, unsigned int words);

/**
 * @brief: get some rand words(with post-processing, but without reseed)
 * parameters:
 *     a -------------------------- output, random words
 *     words ---------------------- input, word number of output, must be in [1, 8]
 * return: TRNG_SUCCESS(success), other(error)
 * caution:
 *     1. please make sure the two parameters are valid
 */
unsigned int get_rand_uint32_without_reseed(unsigned int *a, unsigned int words);

/**
 * @brief       get some rand words(with reseed)
 * @param[in]   a       random words
 * @param[in]   words   word number of output, must be in [1, 8]
 * @return      TRNG_SUCCESS(success), other(error)
* @note
  @verbatim
      -# 1. please make sure the two parameters are valid
  @endverbatim
 */
unsigned int get_rand_uint32_with_reseed(unsigned int *a, unsigned int words);

/**
 * @brief       get rand buffer(internal basis interface)
 * @param[in]   rand                byte buffer rand
 * @param[in]   bytes               byte length of rand
 * @param[in]   get_rand_words      function pointer to get some random words(at most 8 words)
 * @return      TRNG_SUCCESS(success), other(error)
 */
unsigned int get_rand_buffer(unsigned char *rand, unsigned int bytes, GET_RAND_WORDS get_rand_words);

#endif




#ifdef __cplusplus
}
#endif


#endif

