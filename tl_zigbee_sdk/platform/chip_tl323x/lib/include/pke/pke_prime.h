/********************************************************************************************************
 * @file    pke_prime.h
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
#ifndef PKE_PRIME_H
#define PKE_PRIME_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @brief           1:use hardware;  2:use software
 */
#define BIGINT_DIV_CHOICE (2U)

#if (BIGINT_DIV_CHOICE == 1U)
    typedef struct
    {
        unsigned int low;
        unsigned int high;
    } double_uint32_t;
#elif (BIGINT_DIV_CHOICE == 2U)
typedef unsigned int double_uint32_t;
// #define BIGINT_DIV_UINT32
#endif

    /**
 * @brief           1:use Fermat primality test;  2:use Miller-Rabin primality test
 */
#define PRIMALITY_TEST_CHOICE (1U)

#if (PRIMALITY_TEST_CHOICE == 1U)
#define FERMAT_ROUND (3U)
#elif (PRIMALITY_TEST_CHOICE == 2U)
#define MILLER_RABIN_ROUND (3U)
#endif

/**
 * @brief           prime table level(total number of small prime numbers)
  */
#define PTL_MAX  (400U) /**< the max PTL value */
#define PTL_512  (400U) /**< the best PTL value for prime bit length 512 (RSA1024) */
#define PTL_1024 (400U) /**< the best PTL value for prime bit length 1024 \ */
                        /**< (RSA2048) */

#define NOT_PRIME   (0xFFFFFFFFU)
#define MAYBE_PRIME (0U)

    /**
 * @brief           Get a prime number of specified bit length
 * @param[out]      p                    - Pointer to unsigned int big prime number
 * @param[in]       p_bitlen             - Bit length of the prime number
 * @return          
 *        0 (success), p is a prime number with high probability other (error)
 * @note            
 *           p_bitlen must be bigger than 32, but less than 2048.
 */
    unsigned int get_prime(unsigned int p[], unsigned int p_bitlen);

#ifdef __cplusplus
}
#endif

#endif
