/********************************************************************************************************
 * @file    trng.h
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
#ifndef TRNG_H
#define TRNG_H


#include "reg_include/trng_reg.h"
#include "reg_include/soc.h"
#include "driver.h"

#ifdef __cplusplus
extern "C"
{
#endif


//TRNG register address
#define TRNG_CR      (*(volatile unsigned int *)(TRNG_BASE_ADDR + 0x0000))
#define TRNG_MSEL    (*(volatile unsigned int *)(TRNG_BASE_ADDR + 0x0004))
#define TRNG_SR      (*(volatile unsigned int *)(TRNG_BASE_ADDR + 0x0008))
#define TRNG_DR      (*(volatile unsigned int *)(TRNG_BASE_ADDR + 0x000C))
#define TRNG_VERSION (*(volatile unsigned int *)(TRNG_BASE_ADDR + 0x0010))
#define TRNG_RESEED  (*(volatile unsigned int *)(TRNG_BASE_ADDR + 0x0040))
#define TRNG_HT_CR   (*(volatile unsigned int *)(TRNG_BASE_ADDR + 0x0060))
#define TRNG_HT_SR   (*(volatile unsigned int *)(TRNG_BASE_ADDR + 0x0070))
#define RO_SRC_EN1   (*(volatile unsigned int *)(TRNG_BASE_ADDR + 0x0080))
#define RO_SRC_EN2   (*(volatile unsigned int *)(TRNG_BASE_ADDR + 0x0084))
#define SCLK_FREQ    (*(volatile unsigned int *)(TRNG_BASE_ADDR + 0x0088))

#define TERO_CR      (*(volatile unsigned int *)(TRNG_BASE_ADDR + 0x00B0))
#define TERO_THOLD   (*(volatile unsigned int *)(TRNG_BASE_ADDR + 0x00B4))
#define TERO_CNT(i)  (*(volatile unsigned int *)(TRNG_BASE_ADDR + 0x00C0 + 4 * i))
#define TERO_SR      (*(volatile unsigned int *)(TRNG_BASE_ADDR + 0x00D0))
#define TERO_DR      (*(volatile unsigned int *)(TRNG_BASE_ADDR + 0x00D4))
#define TERO_RCR(i)  (*(volatile unsigned int *)(TRNG_BASE_ADDR + 0x00E0 + 4 * i))


//TRNG freq config
#define TRNG_RO_FREQ_4  (0)
#define TRNG_RO_FREQ_8  (1)
#define TRNG_RO_FREQ_16 (2)
#define TRNG_RO_FREQ_32 (3) //default


//TRNG action offset
#define TRNG_GLOBAL_INT_OFFSET     (24)
#define TRNG_READ_EMPTY_INT_OFFSET (17)
#define TRNG_DATA_INT_OFFSET       (16)
#define TRNG_RO_ENTROPY_OFFSET     (4)
#define TRNG_TERO_THRESHOLD_OFFSET (24)
#define TRNG_TERO_ENTROPY_OFFSET   (8)

    //TRNG return code
    enum TRNG_RET_CODE
    {
        TRNG_SUCCESS = 0,
        TRNG_BUFFER_NULL,
        TRNG_INVALID_INPUT,
        TRNG_INVALID_CONFIG,
        TRNG_HT_ERROR,
        TRNG_ERROR
    };

    //API
    /**
 * @brief       TRNG global interruption enable
 * @return      none
 */
    void trng_global_int_enable(void);

    /**
 * @brief       TRNG global interruption disable
 * @return      none
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

    /**
 * @brief       set RO entropy config
 * @param[in]   cfg       - RO entropy config, only the low 4 bits are valid, every bit,indicates one RO entropy, the MSB is RO 0, and LSB is RO 3.
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
 * @param[in]   sn        - RO entropy source series number, must be in [0,3].
 * @param[in]   cfg       - the config value of RO sn.
 * @return      TRNG_SUCCESS(success), other(error)
 */
    unsigned int trng_ro_sub_entropy_config(unsigned char sn, unsigned short cfg);

    /**
 * @brief       set sub RO entropy config
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
 * @brief       get rand(for internal test)
 * @param[in]   rand                byte buffer rand
 * @param[in]   bytes               byte length of rand
 * @return      TRNG_SUCCESS(success), other(error)
 * @note
  @verbatim
      -# 1. Directly from the entropy source to take the random number,
            depending on the current configuration, the power-up default true random number mode,
            if you get the true random number and the current mode is configured as a post-processing, you can use trng_disable, trng_set_mode, trng_enable interface function to set to the true random mode, and then call the function,
            if you get the pseudo-random please use get_rand.
  @endverbatim
 */
    unsigned int get_rand_internal(unsigned char *a, unsigned int bytes);

    /**
 * @brief       get rand(without entropy reducing)
 * @param[in]   rand                byte buffer rand
 * @param[in]   bytes               byte length of rand
 * @return      TRNG_SUCCESS(success), other(error)
 * * @note
  @verbatim
      -# 1. This interface uses pseudo-random mode by default, through post-processing, the randomness will be better and easier to pass trng-related authentication
      -# 2. After the call the mode configuration is changed to post-processing mode.
  @endverbatim
 */
    unsigned int get_rand(unsigned char *rand, unsigned int bytes);


    /**
 * @brief       TERO RNG enable
 * @return      none
 */
    void tero_enable(void);

    /**
 * @brief       TERO RNG disable
 * @return      none
 */
    void tero_disable(void);

    /**
 * @brief       TERO RNG set the system cycle threshold of the TERO counter kept
 * @param[in]   threshold_value                 threshold value
 * @return      none
 */
    unsigned int tero_set_stop_threshold(unsigned char threshold_value);

    /**
 * @brief       set TERO entropy config
 * @param[in]   cfg       random words
 * @return      TRNG_SUCCESS(success), other(error)
 * @note
  @verbatim
      -# 1. please make sure the two parameters are valid
  @endverbatim
 */
    unsigned int tero_entropy_config(unsigned char cfg);

    /**
 * @brief       TERO RNG set output as rng
 * @return      none
 */
    void tero_set_output_rng(void);

    /**
 * @brief       TERO RNG set output as oscillation times
 * @return      none
 */
    void tero_set_output_osc_times(void);

    /**
 * @brief       select TREO 1&2 or TERO 3&4 when output is oscillation times
 * @param[in]   cfg       random words
 * @return      none
 */
    void tero_set_osc_sel(unsigned char cfg);

    /**
 * @brief       set lower limit of oscillation times
 * @param[in]   value       lower limit value
 * @return      none
 */
    void tero_set_osc_times_lower_limit(unsigned short value);

    /**
 * @brief       set upper limit of oscillation times
 * @param[in]   value       lower limit value
 * @return      none
 */
    void tero_set_osc_times_upper_limit(unsigned short value);

    /**
 * @brief       get tero rand
 * @param[in]   a       byte buffer a
 * @param[in]   bytes   byte length of rand
 * @return      none
 */
    unsigned int get_tero_rand(unsigned char *a, unsigned int bytes);


#ifdef __cplusplus
}
#endif


#endif
