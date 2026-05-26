/********************************************************************************************************
 * @file    eccp_curve.h
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
#ifndef ECCP_CURVE_H
#define ECCP_CURVE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lib/include/pke/pke_common.h"


/**
 * @brief           sample ecc GF(p) curve
 */
#define SUPPORT_BRAINPOOLP160R1
// #define SUPPORT_BRAINPOOLP192R1
// #define SUPPORT_BRAINPOOLP224R1
#define SUPPORT_BRAINPOOLP256R1
#if (ECCP_MAX_BIT_LEN >= 320)
// #define SUPPORT_BRAINPOOLP320R1
#endif
#if (ECCP_MAX_BIT_LEN >= 384)
// #define SUPPORT_BRAINPOOLP384R1
#endif
#if (ECCP_MAX_BIT_LEN >= 512)
// #define SUPPORT_BRAINPOOLP512R1
#endif
// #define SUPPORT_SECP160R1
// #define SUPPORT_SECP160R2
#define SUPPORT_SECP192R1
#define SUPPORT_SECP224R1
#define SUPPORT_SECP256R1
#if (ECCP_MAX_BIT_LEN >= 384)
// #define SUPPORT_SECP384R1
#endif
#if (ECCP_MAX_BIT_LEN >= 521)
// #define SUPPORT_SECP521R1
#endif
// #define SUPPORT_SECP160K1
// #define SUPPORT_SECP192K1
// #define SUPPORT_SECP224K1
// #define SUPPORT_SECP256K1
// #define SUPPORT_BN256
#if (ECCP_MAX_BIT_LEN >= 638)
#define SUPPORT_BN638
#endif
#if (ECCP_MAX_BIT_LEN >= 1024)
#define SUPPORT_ANDERS_1024_1
#endif

    /**
 * @brief           eccp curve struct
 */
    typedef struct
    {
        unsigned int        eccp_p_bitLen; // bit length of prime p
        unsigned int        eccp_n_bitLen; // bit length of order n
        const unsigned int *eccp_p;        // prime p
        const unsigned int *eccp_p_h;
#if (defined(PKE_LP) || defined(PKE_SECURE) || defined(PKE_UHP_ECC))
        const unsigned int *eccp_p_n0;
#endif
        const unsigned int *eccp_a;
        const unsigned int *eccp_b;
        const unsigned int *eccp_Gx;
        const unsigned int *eccp_Gy;
        const unsigned int *eccp_n; // order of curve or point(Gx,Gy)
        const unsigned int *eccp_n_h;
#if (defined(PKE_LP) || defined(PKE_SECURE) || defined(PKE_UHP_ECC))
        const unsigned int *eccp_n_n0;
#endif
#if (defined(PKE_HP) || defined(PKE_UHP) || defined(PKE_UHP_ECC))
        const unsigned int *eccp_half_Gx;
        const unsigned int *eccp_half_Gy;
#endif
    } eccp_curve_t;

#ifdef SUPPORT_BRAINPOOLP160R1
    extern const eccp_curve_t brainpoolp160r1[1];
#endif

#ifdef SUPPORT_BRAINPOOLP192R1
    extern const eccp_curve_t brainpoolp192r1[1];
#endif

#ifdef SUPPORT_BRAINPOOLP224R1
    extern const eccp_curve_t brainpoolp224r1[1];
#endif

#ifdef SUPPORT_BRAINPOOLP256R1
    extern const eccp_curve_t brainpoolp256r1[1];
#endif

#ifdef SUPPORT_BRAINPOOLP320R1
    extern const eccp_curve_t brainpoolp320r1[1];
#endif

#ifdef SUPPORT_BRAINPOOLP384R1
    extern const eccp_curve_t brainpoolp384r1[1];
#endif

#ifdef SUPPORT_BRAINPOOLP512R1
    extern const eccp_curve_t brainpoolp512r1[1];
#endif

#ifdef SUPPORT_SECP160R1
    extern const eccp_curve_t secp160r1[1];
#endif

#ifdef SUPPORT_SECP160R2
    extern const eccp_curve_t secp160r2[1];
#endif

#ifdef SUPPORT_SECP192R1
    extern const eccp_curve_t secp192r1[1];
#endif

#ifdef SUPPORT_SECP224R1
    extern const eccp_curve_t secp224r1[1];
#endif

#ifdef SUPPORT_SECP256R1
    extern const eccp_curve_t secp256r1[1];
#endif

#ifdef SUPPORT_SECP384R1
    extern const eccp_curve_t secp384r1[1];
#endif

#ifdef SUPPORT_SECP521R1
    extern const eccp_curve_t secp521r1[1];
#endif

#ifdef SUPPORT_SECP160K1
    extern const eccp_curve_t secp160k1[1];
#endif

#ifdef SUPPORT_SECP192K1
    extern const eccp_curve_t secp192k1[1];
#endif

#ifdef SUPPORT_SECP224K1
    extern const eccp_curve_t secp224k1[1];
#endif

#ifdef SUPPORT_SECP256K1
    extern const eccp_curve_t secp256k1[1];
#endif

#ifdef SUPPORT_BN256
    extern const eccp_curve_t bn256[1];
#endif

#ifdef SUPPORT_BN638
    extern const eccp_curve_t bn638[1];
#endif

#ifdef SUPPORT_ANDERS_1024_1
    extern const eccp_curve_t anders_1024_1[1];
#endif

#ifdef SUPPORT_SM2
    extern const eccp_curve_t sm2_curve[1];
    extern const unsigned int g_sm2p256v1_n_minus_1[8];
#endif

#ifdef SUPPORT_SM9
    extern const eccp_curve_t sm9_curve[1];
#endif

    /********* Curve25519 struct *********/
    typedef struct
    {
        unsigned int        p_bitLen; // bit length of prime p
        unsigned int        n_bitLen; // bit length of order n
        const unsigned int *p;
        const unsigned int *p_h;
#if (defined(PKE_LP) || defined(PKE_SECURE))
        const unsigned int *p_n0;
#endif
        const unsigned int *a24; // (A-2)/4
        const unsigned int *u;
        const unsigned int *v;
        const unsigned int *n; // order of curve or point(Gx,Gy)
        const unsigned int *n_h;
#if (defined(PKE_LP) || defined(PKE_SECURE))
        const unsigned int *n_n0;
#endif
        const unsigned int *cofactor;
    } mont_curve_t;

    /********* Edward Curve 25519 struct *********/
    typedef struct
    {
        unsigned int        p_bitLen; // bit length of prime p
        unsigned int        n_bitLen; // bit length of order n
        const unsigned int *p;
        const unsigned int *p_h;
#if (defined(PKE_LP) || defined(PKE_SECURE))
        const unsigned int *p_n0;
#endif
        const unsigned int *d;
        const unsigned int *Gx;
        const unsigned int *Gy;
        const unsigned int *n; // order of curve or point(Gx,Gy)
        const unsigned int *n_h;
#if (defined(PKE_LP) || defined(PKE_SECURE))
        const unsigned int *n_n0;
#endif
        const unsigned int *cofactor;
    } edward_curve_t;

#ifdef SUPPORT_C25519
    extern const mont_curve_t   c25519[1];
    extern const edward_curve_t ed25519[1];
#endif

#ifdef __cplusplus
}
#endif

#endif
