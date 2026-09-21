/********************************************************************************************************
 * @file    pke_prime.h
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
#ifndef PKE_PRIME_H
#define PKE_PRIME_H

#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif


//1:use hardware;  2:use software
#define BIGINT_DIV_CHOICE (2)

#if (BIGINT_DIV_CHOICE == 1)
    typedef struct
    {
        unsigned int low;
        unsigned int high;
    } double_unsigned_int;
#elif (BIGINT_DIV_CHOICE == 2)
typedef unsigned int double_unsigned_int;
#endif


//1:use Fermat primality test;  2:use Miller_Rabin primality test
#define PRIMALITY_TEST_CHOICE (1)

#if (PRIMALITY_TEST_CHOICE == 1)
    #define FERMAT_ROUND (3)
#elif (PRIMALITY_TEST_CHOICE == 2)
    #define MILLER_RABIN_ROUND (3)
#endif


//prime table level(total number of small prime numbers)
#define PTL_MAX     (400) //the max PTL value
#define PTL_512     (400) //the best PTL value for prime bit length 512 (RSA1024)
#define PTL_1024    (400) //the best PTL value for prime bit length 1024 (RSA2048)


#define NOT_PRIME   (0xFFFFFFFF)
#define MAYBE_PRIME (0)


    /**
 * @brief       get prime number of pBitLen
 * @param[in]   p              - pointer to unsigned int big prime number.
 * @param[in]   pBitLen        - bit length of p.
 * @return      none
 * @note
 * @verbatim
      -# 1.pBitLen must be bigger than 32, but less than 2048.
  @endverbatim
 */
    unsigned int get_prime(unsigned int p[], unsigned int pBitLen);


#ifdef __cplusplus
}
#endif

#endif
