/********************************************************************************************************
 * @file    rsa_u8.h
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

#ifndef RSA_U8_H
#define RSA_U8_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "rsa.h"

    /**
 * @brief           APIs
 */


    /**
 * @brief           calculates the modular exponentiation of a given number using big-endian 8-bit unsigned integers.
 *                  this function computes the result of (a^e) mod n, where a, e, and n are
 *                  big-endian 8-bit unsigned integers. it supports two different
 *                  implementations: one using 32-bit unsigned integers and byte array reversal,
 *                  and another using the pke_modexp_u8 function.
 * @param[in]       a                    - unsigned char big integer a, base number, make sure a < n.
 * @param[in]       e                    - unsigned char big integer e, exponent, make sure e < n.
 * @param[in]       n                    - unsigned char big integer n, modulus, make sure n is odd.
 * @param[out]      out                  - the result of (a^e) mod n.
 * @param[in]       eBitLen              - real bit length of unsigned char big integer e.
 * @param[in]       nBitLen              - real bit length of unsigned char big integer n.
 * @return          rsa_success if the operation is successful, other values indicate an error.
 * @note            
 *        1. this is for high-level application or protocol to use rsa mod exponent directly. all operands of this api are unsigned char big-endian big numbers.
 *        2. the modulus must be odd.
 *        3. please make sure exp_bitLen <= mod_bitLen <= operand_max_bit_len.
 *        4. a, n, and out have the same word length: ((nBitLen + 31) >> 5); and e word length is (eBitLen + 31) >> 5.
 */
    unsigned int RSA_ModExp_U8(const unsigned char *a, const unsigned char *e, const unsigned char *n, unsigned char *out, unsigned int eBitLen, unsigned int nBitLen);

    /**
 * @brief           calculates the modular exponentiation of a given number using the
 *                  chinese remainder theorem (crt) for rsa.
 *                  this function computes the result of (a^d) mod n, where d is represented by
 *                  the rsa crt private key components (p, q, dp, dq, u). all input and output
 *                  operands are big-endian 8-bit unsigned integers.
 * @param[in]       a                    - unsigned char big integer a, base number, make sure a < n=pq.
 * @param[in]       p                    - unsigned char big integer p, prime number, one part of private key (p, q, dp, dq, u).
 * @param[in]       q                    - unsigned char big integer q, prime number, one part of private key (p, q, dp, dq, u).
 * @param[in]       dp                   - unsigned char big integer dp = e^(-1) mod (p-1), one part of private key (p, q, dp, dq, u).
 * @param[in]       dq                   - unsigned char big integer dq = e^(-1) mod (q-1), one part of private key (p, q, dp, dq, u).
 * @param[in]       u                    - unsigned char big integer u = q^(-1) mod p, one part of private key (p, q, dp, dq, u).
 * @param[out]      out                  - the result of (a^d) mod n.
 * @param[in]       nBitLen              - real bit length of unsigned char big integer n=pq.
 * @return          rsa_success if the operation is successful, other values indicate an error.
 * @note            
 *        1. this is for high-level application or protocol to use rsa mod exponent directly. all operands of this api are unsigned char big-endian big numbers.
 *        2. the modulus must be odd.
 *        3. please make sure exp_bitLen <= mod_bitLen <= operand_max_bit_len.
 *        4. a, n, and out have the same word length: ((nBitLen + 31) >> 5); and p, q, dp, dq, u have the same word length: ((nBitLen / 2 + 31) >> 5).
 */
    unsigned int RSA_CRTModExp_U8(const unsigned char *a, const unsigned char *p, const unsigned char *q, const unsigned char *dp, const unsigned char *dq, const unsigned char *u,
                                  unsigned char *out, unsigned int nBitLen);

    /**
 * @brief           generate rsa key (e, d, n) for high-level use, where operands are all
 *                  unsigned char big-endian big numbers.
 *                  this function generates rsa public key e, private key d, and public modulus
 *                  n. it first calls the internal rsa_getkey function to generate the keys in
 *                  32-bit unsigned integer format. then, it converts the generated keys from
 *                  little-endian 32-bit integers to big-endian 8-bit integers.
 * @param[out]      e                    - unsigned char big integer, rsa public key e.
 * @param[out]      d                    - unsigned char big integer, rsa private key d.
 * @param[out]      n                    - unsigned char big integer, rsa public modulus n.
 * @param[in]       eBitLen              - real bit length of e.
 * @param[in]       nBitLen              - real bit length of n.
 * @return          rsa_success if the operation is successful, other values indicate an error.
 * @note            
 *        1. nBitLen can not be odd.
 *        2. eBitLen must be greater than 1, and less than or equal to nBitLen.
 *        3. if eBitLen is 2, 5, 17, here makes e as 3, 17, 65537 respectively, otherwise e is random.
 */
    unsigned int RSA_GetKey_U8(unsigned char *e, unsigned char *d, unsigned char *n, unsigned int eBitLen, unsigned int nBitLen);

    /**
 * @brief           generate rsa-crt key (e, p, q, dp, dq, u, n) for high-level use, where
 *                  operands are all unsigned char big-endian big numbers.
 *                  this function generates rsa-crt keys in 32-bit unsigned integer format by
 *                  calling the internal rsa_getcrtkey function. then, it converts the generated
 *                  keys from little-endian 32-bit integers to big-endian 8-bit integers.
 * @param[out]      e                    - unsigned char big integer, rsa public key e.
 * @param[out]      p                    - unsigned char big integer, rsa private key p.
 * @param[out]      q                    - unsigned char big integer, rsa private key q.
 * @param[out]      dp                   - unsigned char big integer, rsa private key dp.
 * @param[out]      dq                   - unsigned char big integer, rsa private key dq.
 * @param[out]      u                    - unsigned char big integer, rsa private key u = q^(-1) mod p.
 * @param[out]      n                    - unsigned char big integer, rsa public modulus n.
 * @param[in]       eBitLen              - real bit length of e.
 * @param[in]       nBitLen              - real bit length of n.
 * @return          rsa_success if the operation is successful, other values indicate an error.
 * @note            
 *        1. nBitLen can not be odd.
 *        2. eBitLen must be greater than 1, and less than or equal to nBitLen.
 *        3. if eBitLen is 2, 5, 17, here makes e as 3, 17, 65537 respectively, otherwise e is random.
 */
    unsigned int RSA_GetCRTKey_U8(unsigned char *e, unsigned char *p, unsigned char *q, unsigned char *dp, unsigned char *dq, unsigned char *u, unsigned char *n,
                                  unsigned int eBitLen, unsigned int nBitLen);

#ifdef __cplusplus
}
#endif

#endif
