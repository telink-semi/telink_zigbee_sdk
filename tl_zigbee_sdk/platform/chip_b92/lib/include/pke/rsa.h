/********************************************************************************************************
 * @file    rsa.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/********* pke version:1.1 *********/
#ifndef RSA_H
#define RSA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pke.h"







//RSA return code
enum RSA_RET_CODE
{
    RSA_SUCCESS = PKE_SUCCESS,
    RSA_BUFFER_NULL = PKE_SUCCESS+0x30,
    RSA_INPUT_TOO_LONG,
    RSA_INPUT_INVALID,
};


//APIs

/**
 * @brief		out = a^e mod n.
 * @param[in]	a		- unsigned int big integer a, base number, make sure a < n.
 * @param[in]	e		- unsigned int big integer e, exponent, make sure e < n.
 * @param[in]	n		- unsigned int big integer n, modulus, make sure n is odd.
 * @param[out]	out		- out = a^e mod n.
 * @param[in]	eBitLen	- real bit length of unsigned int big integer e.
 * @param[in]	nBitLen	- real bit length of unsigned int big integer n.
 * @return		RSA_SUCCESS(success), other(error).
 * @caution		a, n, and out have the same word length:((nBitLen+31)>>5); and e word length is (eBitLen+31)>>5
 */
unsigned int RSA_ModExp(unsigned int *a, unsigned int *e, unsigned int *n, unsigned int *out, unsigned int eBitLen, unsigned int nBitLen);

/**
 * @brief       out = a^d mod n, here d represents RSA CRT private key (p,q,dp,dq,u).
 * @param[in]   a       - unsigned int big integer a, base number, make sure a < n=pq.
 * @param[in]   p       - unsigned int big integer p, prime number, one part of private key (p,q,dp,dq,u).
 * @param[in]   q       - unsigned int big integer q, prime number, one part of private key (p,q,dp,dq,u).
 * @param[in]   dp      - unsigned int big integer dp = e^(-1) mod (p-1), one part of private key (p,q,dp,dq,u).
 * @param[in]   dq      - unsigned int big integer dq = e^(-1) mod (q-1), one part of private key (p,q,dp,dq,u).
 * @param[in]   u       - unsigned int big integer u = q^(-1) mod p, one part of private key (p,q,dp,dq,u).
 * @param[in]   nBitLen - real bit length of unsigned int big integer n=pq.
 * @param[out]	out     - out = a^d mod n.
 * @return   	RSA_SUCCESS(success), other(error).
 * @caution     a and out have the same word length:((nBitLen+31)>>5); and p,p_h,q,q_h,dp,dq,u have the same word length:((nBitLen/2+31)>>5).
 */
unsigned int RSA_CRTModExp(unsigned int *a, unsigned int *p, unsigned int *q, unsigned int *dp, unsigned int*dq,
		unsigned int *u, unsigned int *out,  unsigned int nBitLen);

/**
 * @brief       generate RSA key (e,d,n).
 * @param[in]   eBitLen - real bit length of e.
 * @param[in]   nBitLen - real bit length of n.
 * @param[out]	e       - unsigned int big integer, RSA public key e.
 * @param[out]	d       - unsigned int big integer, RSA private key d.
 * @param[out]	n       - unsigned int big integer, RSA public module n.
 * @return   	RSA_SUCCESS(success), other(error).
 * @caution     1. nBitLen can not be even.
 *              2. eBitLen must be larger than 1, and less than or equal to nBitLen.
 */
unsigned int RSA_GetKey(unsigned int *e, unsigned int *d, unsigned int *n, unsigned int eBitLen, unsigned int nBitLen);

/**
 * @brief       generate RSA-CRT key (e,p,q,dp,dq,u,n).
 * @param[in]   eBitLen - real bit length of e.
 * @param[in]   nBitLen - real bit length of n.
 * @param[out]	e       - unsigned int big integer, RSA public key e.
 * @param[out]	p       - unsigned int big integer, RSA private key p.
 * @param[out]	q       - unsigned int big integer, RSA private key q.
 * @param[out]	dp      - unsigned int big integer, RSA private key dp.
 * @param[out]	dq      - unsigned int big integer, RSA private key dq.
 * @param[out]	u       - unsigned int big integer, RSA private key u = q^(-1) mod p.
 * @param[out]	n       - unsigned int big integer, RSA public module n.
 * @return   	RSA_SUCCESS(success), other(error).
 * @Caution     1. nBitLen can not be even.
 *              2. eBitLen must be larger than 1, and less than or equal to nBitLen.
 */
unsigned int RSA_GetCRTKey(unsigned int *e, unsigned int *p, unsigned int *q, unsigned int *dp, unsigned int *dq, unsigned int *u,
		unsigned int *n, unsigned int eBitLen, unsigned int nBitLen);


#ifdef __cplusplus
}
#endif

#endif
