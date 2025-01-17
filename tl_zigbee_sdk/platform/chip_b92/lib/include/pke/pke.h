/********************************************************************************************************
 * @file    pke.h
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
#ifndef PKE_H
#define PKE_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "reg_include/pke_reg.h"
#include "lib/include/trng.h"
#include "eccp_curve.h"
#include "pke_utility.h"


#define pke_clr_irq_status    pke_clear_interrupt
#define pke_get_irq_status    pke_wait_till_done
#define pke_opr_start         pke_start
#define pke_mod_add           pke_modadd
#define pke_mod_sub           pke_modsub
#define pke_mod_mul           pke_modmul
#define pke_mod_inv           pke_modinv
#define div2n_u32             Big_Div2n
#define sub_u32               pke_sub
#define pke_eccp_point_mul    eccp_pointMul
#define pke_eccp_point_add    eccp_pointAdd
#define pke_eccp_point_verify eccp_pointVerify
#define pke_x25519_point_mul  x25519_pointMul
#define pke_ed25519_point_mul ed25519_pointMul
#define pke_ed25519_point_add ed25519_pointAdd


//#define SUPPORT_SM2
#define SUPPORT_C25519

#ifndef NULL
    #define NULL 0
#endif

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

/*********** PKE register action offset ************/
#define PKE_INT_ENABLE_OFFSET       (8)
#define PKE_START_CALC              (1)

#define PKE_EXE_OUTPUT_AFFINE       (0x10)
#define PKE_EXE_R1_MONT_R0_AFFINE   (0x09)
#define PKE_EXE_R1_MONT_R0_MONT     (0x0A)
#define PKE_EXE_R1_AFFINE_R0_AFFINE (0x05)
#define PKE_EXE_R1_AFFINE_R0_MONT   (0x06)
#define PKE_EXE_ECCP_POINT_MUL      (PKE_EXE_OUTPUT_AFFINE + PKE_EXE_R1_AFFINE_R0_MONT)
#define PKE_EXE_ECCP_POINT_ADD      (PKE_EXE_OUTPUT_AFFINE + PKE_EXE_R1_AFFINE_R0_AFFINE)
#define PKE_EXE_ECCP_POINT_DBL      (PKE_EXE_OUTPUT_AFFINE + PKE_EXE_R1_MONT_R0_AFFINE)
#define PKE_EXE_ECCP_POINT_VER      (PKE_EXE_OUTPUT_AFFINE + PKE_EXE_R1_AFFINE_R0_MONT)

#define PKE_EXE_CFG_ALL_MONT        (0x002A)
#define PKE_EXE_CFG_ALL_NON_MONT    (0x0000)
#define PKE_EXE_CFG_MOD_EXP         (0x0316)


/***************** PKE microcode ******************/
#define MICROCODE_PDBL         (0x04)
#define MICROCODE_PADD         (0x08)
#define MICROCODE_PVER         (0x0C)
#define MICROCODE_PMUL         (0x10)
#define MICROCODE_MODEXP       (0x14)
#define MICROCODE_MODMUL       (0x18)
#define MICROCODE_MODINV       (0x1C)
#define MICROCODE_MODADD       (0x20)
#define MICROCODE_MODSUB       (0x24)
#define MICROCODE_MGMR_PRE     (0x28)
#define MICROCODE_INTMUL       (0x2C)
#define MICROCODE_Ed25519_PMUL (0x30)
#define MICROCODE_Ed25519_PADD (0x34)
#define MICROCODE_C25519_PMUL  (0x38)


/*********** some PKE algorithm operand length ************/
#define OPERAND_MAX_BIT_LEN  (2048)
#define OPERAND_MAX_WORD_LEN (GET_WORD_LEN(OPERAND_MAX_BIT_LEN))

#define ECCP_MAX_BIT_LEN     (256) //ECC256
#define ECCP_MAX_BYTE_LEN    (GET_BYTE_LEN(ECCP_MAX_BIT_LEN))
#define ECCP_MAX_WORD_LEN    (GET_WORD_LEN(ECCP_MAX_BIT_LEN))

#define C25519_BYTE_LEN      (256 / 8)
#define C25519_WORD_LEN      (256 / 32)

#define Ed25519_BYTE_LEN     C25519_BYTE_LEN
#define Ed25519_WORD_LEN     C25519_WORD_LEN

#define MAX_RSA_WORD_LEN     OPERAND_MAX_WORD_LEN
#define MAX_RSA_BIT_LEN      (MAX_RSA_WORD_LEN << 5)
#define MIN_RSA_BIT_LEN      (512)

    /******************* PKE return code ********************/
    enum PKE_RET_CODE
    {
        PKE_SUCCESS = 0,
        PKE_STOP,
        PKE_NO_MODINV,
        PKE_NOT_ON_CURVE,
        PKE_INVALID_MC,
        PKE_ZERO_ALL,        //for ECCP input check
        PKE_INTEGER_TOO_BIG, //for ECCP input check
        PKE_INVALID_INPUT,
    };

    //define KDF
    typedef void *(*KDF_FUNC)(const void *input, unsigned int byteLen, unsigned char *key, unsigned int keyByteLen);


    //APIs

    /**
 * @brief       get random data.
 * @param[in]   rand_pke  - random data array.
 * @param[in]   bytes     - length of the random data.
 * @return      TRNG_SUCCESS(success), other(error).
 */
    unsigned char rand_get(unsigned char *rand, unsigned int byteLen);

    /**
 * @brief       ainv = a^(-1) mod modulus.
 * @param[in]   modulus    - modulus.
 * @param[in]   a          - integer a.
 * @param[in]   modWordLen - word length of modulus and ainv.
 * @param[in]   aWordLen   - word length of a.
 * @param[out]  ainv       - ainv = a^(-1) mod modulus.
 * @return      PKE_SUCCESS(success), other(inverse not exists or error).
 * @caution     please make sure aWordLen <= modWordLen <= OPERAND_MAX_WORD_LEN and a < modulus.
 */
    unsigned int pke_modinv(const unsigned int *modulus, volatile const unsigned int *a, unsigned int *ainv, unsigned int modWordLen, unsigned int aWordLen);

    /**
 * @brief       out = (a+b) mod modulus.
 * @param[in]   modulus  - modulus.
 * @param[in]   a        - integer a.
 * @param[in]   b        - integer b.
 * @param[in]   wordLen  - word length of modulus, a, b.
 * @param[out]  out      - out = a+b mod modulus.
 * @return      PKE_SUCCESS(success), other(error).
 * @caution     1. a,b must be less than modulus.
 *              2. wordLen must not be bigger than OPERAND_MAX_WORD_LEN.
 */
    unsigned int pke_modadd(const unsigned int *modulus, volatile const unsigned int *a, volatile const unsigned int *b, unsigned int *out, unsigned int wordLen);

    /**
 * @brief       out = (a-b) mod modulus.
 * @param[in]   modulus - modulus.
 * @param[in]   a       - integer a.
 * @param[in]   b       - integer b.
 * @param[in]   wordLen - word length of modulus, a, b.
 * @param[out]  out     - out = a-b mod modulus.
 * @return      PKE_SUCCESS(success), other(error).
 * @caution     1. a,b must be less than modulus.
 *              2. wordLen must not be bigger than OPERAND_MAX_WORD_LEN.
 */
    unsigned int pke_modsub(const unsigned int *modulus, const unsigned int *a, const unsigned int *b, unsigned int *out, unsigned int wordLen);

    /**
 * @brief       out = a+b.
 * @param[in]   a         - integer a.
 * @param[in]   b         - integer b.
 * @param[in]   wordLen   - word length of a, b, out.
 * @param[out]  out       - out = a+b.
 * @return      PKE_SUCCESS(success), other(error).
 * @caution     1. a+b may overflow.
 *              2. wordLen must not be bigger than OPERAND_MAX_WORD_LEN.
 */
    unsigned int pke_add(volatile const unsigned int *a, volatile const unsigned int *b, unsigned int *out, unsigned int wordLen);

    /**
 * @brief       out = a-b.
 * @param[in]   a         - integer a.
 * @param[in]   b         - integer b.
 * @param[in]   wordLen   - word length of a, b, out.
 * @param[out]  out       - out = a-b.
 * @return      PKE_SUCCESS(success), other(error).
 * @caution     1. please make sure a > b.
 *              2. wordLen must not be bigger than OPERAND_MAX_WORD_LEN.
 */
    unsigned int pke_sub(volatile const unsigned int *a, const unsigned int *b, volatile unsigned int *out, unsigned int wordLen);

    /**
 * @brief       out = a*b.
 * @param[in]   a       - integer a.
 * @param[in]   b       - integer b.
 * @param[in]   wordLen - word length of a, b.
 * @param[out]  out     - out = a*b.
 * @return      PKE_SUCCESS(success), other(error).
 * @caution     1. please make sure out buffer word length is bigger than (2*max_bit_len(a,b)+0x1F)>>5.
 *              2. please make sure ab_wordLen is not bigger than OPERAND_MAX_WORD_LEN/2.
 */
    unsigned int pke_mul(const unsigned int *a, const unsigned int *b, volatile unsigned int *out, unsigned int ab_wordLen);

    /**
 * @brief       load the pre-calculated mont parameters H(R^2 mod modulus).
 * @param[in]   H       - R^2 mod modulus.
 * @param[in]   n1      - -modulus ^(-1) mod 2^w, here w is 32 actually.
 * @param[in]   wordLen - word length of modulus or H.
 * @return      none.
 * @caution     1. please make sure the 2 input parameters are both valid.
 *              2. wordLen must not be bigger than OPERAND_MAX_WORD_LEN.
 */
    void pke_load_pre_calc_mont(volatile const unsigned int *H, volatile const unsigned int *n1, unsigned int wordLen);

    /**
 * @brief       out = a*b mod modulus.
 * @param[in]   modulus  - modulus.
 * @param[in]   a        - integer a.
 * @param[in]   b        - integer b.
 * @param[in]   wordLen  - word length of modulus, a, b.
 * @param[out]  out      - out = a*b mod modulus.
 * @return      PKE_SUCCESS(success), other(error).
 * @caution     1. modulus must be odd.
 *              2. a, b must less than modulus.
 *              3. wordLen must not be bigger than OPERAND_MAX_WORD_LEN.
 *              4. before calling this function, please make sure the pre-calculated mont arguments of modulus are located in the right address.
 */
    unsigned int pke_modmul_internal(volatile const unsigned int *modulus, volatile const unsigned int *a, volatile const unsigned int *b, volatile unsigned int *out, unsigned int wordLen);

    /**
 * @brief       out = a*b mod modulus.
 * @param[in]   modulus  - modulus.
 * @param[in]   a        - integer a.
 * @param[in]   b        - integer b.
 * @param[in]   wordLen  - word length of modulus, a, b.
 * @param[out]  out      - out = a*b mod modulus.
 * @return      PKE_SUCCESS(success), other(error).
 * @caution     1. modulus must be odd.
 *              2. a, b must less than modulus.
 *              3. wordLen must not be bigger than OPERAND_MAX_WORD_LEN.
 */
    unsigned int pke_modmul(const unsigned int *modulus, const unsigned int *a, const unsigned int *b, unsigned int *out, unsigned int wordLen);

    /**
 * @brief       mod exponent, this could be used for rsa encrypting,decrypting,signing,verifying.
 * @param[in]   modulus     - modulus.
 * @param[in]   exponent    - exponent.
 * @param[in]   base        - base number.
 * @param[in]   mod_wordLen - word length of modulus and base number.
 * @param[in]   exp_wordLen - word length of exponent.
 * @param[out]  out         - out = a^(b) mod modulus.
 * @return      PKE_SUCCESS(success), other(error)
 * @caution     1. before calling this function, please make sure R^2 mod modulus, the pre-calculated mont arguments of
 *              modulus is located in the right address.
 *              2. modulus must be odd.
 *              3. please make sure exp_wordLen <= mod_wordLen <= OPERAND_MAX_WORD_LEN.
 */
    unsigned int pke_modexp(volatile const unsigned int *modulus, volatile const unsigned int *exponent, volatile const unsigned int *base, volatile unsigned int *out, unsigned int mod_wordLen, unsigned int exp_wordLen);

    /**
 * @brief       c = a mod b.
 * @param[in]   a          - real bit length of e.
 * @param[in]   aWordLen   - real bit length of n.
 * @param[in]   b          - real bit length of n.
 * @param[in]   b_h        - real bit length of n.
 * @param[in]   b_n1       - real bit length of n.
 * @param[in]   bWordLen   - real bit length of n.
 * @param[out]  c          - unsigned int big integer, RSA public key e.
 * @return      PKE_SUCCESS(success), other(error).
 * @caution     1. b must be odd, and please make sure bWordLen is real word length of b.
 *              2. please make sure aWordLen <= 2*OPERAND_MAX_WORD_LEN, bWordLen <= OPERAND_MAX_WORD_LEN.
 *              3. real bit length of a can not be bigger than 2*(real bit length of b).
 */
    unsigned int pke_mod(unsigned int *a, unsigned int aWordLen, const unsigned int *b, volatile const unsigned int *b_h, volatile const unsigned int *b_n1, unsigned int bWordLen, unsigned int *c);

    /**
 * @brief       ECCP curve point mul(random point), Q=[k]P.
 * @param[in]   curve - eccp_curve_t curve struct pointer.
 * @param[in]   k     - scalar.
 * @param[in]   Px    - x coordinate of point P.
 * @param[in]   Py    - y coordinate of point P.
 * @param[out]  Qx    - x coordinate of point Q.
 * @param[out]  Qy    - y coordinate of point Q.
 * @return      PKE_SUCCESS(success), other(error).
 * @caution     1. please make sure k in [1,n-1], n is order of ECCP curve.
 *              2. please make sure input point P is on the curve.
 *              3. please make sure bit length of the curve is not bigger than ECCP_MAX_BIT_LEN.
 */
    unsigned int eccp_pointMul(eccp_curve_t *curve, unsigned int *k, const unsigned int *Px, const unsigned int *Py, volatile unsigned int *Qx, volatile unsigned int *Qy);

    /**
 * @brief       ECCP curve point add, Q=P1+P2.
 * @param[in]   curve - eccp_curve_t curve struct pointer.
 * @param[in]   P1x   - x coordinate of point P1.
 * @param[in]   P1y   - y coordinate of point P1.
 * @param[in]   P2x   - x coordinate of point P2.
 * @param[in]   P2y   - y coordinate of point P2.
 * @param[out]  Qx    - x coordinate of point Q=P1+P2.
 * @param[out]  Qy    - y coordinate of point Q=P1+P2.
 * @return      PKE_SUCCESS(success), other(error).
 * @caution     1. please make sure input point P1 and P2 are both on the curve.
 *              2. please make sure bit length of the curve is not bigger than ECCP_MAX_BIT_LEN.
 */
    unsigned int eccp_pointAdd(eccp_curve_t *curve, unsigned int *P1x, unsigned int *P1y, unsigned int *P2x, unsigned int *P2y, unsigned int *Qx, unsigned int *Qy);

//#define ECCP_POINT_DOUBLE   //recommended not to define
#ifdef ECCP_POINT_DOUBLE
    /**
 * @brief       ECCP curve point double, Q=[2]P.
 * @param[in]   curve - eccp_curve_t curve struct pointer.
 * @param[in]   Px    - x coordinate of point P.
 * @param[in]   Py    - y coordinate of point P.
 * @param[out]  Qx    - x coordinate of point Q=[2]P.
 * @param[out]  Qy    - y coordinate of point Q=[2]P.
 * @return      PKE_SUCCESS(success), other(error).
 * @caution     1. please make sure input point P is on the curve.
 *              2. please make sure bit length of the curve is not bigger than ECCP_MAX_BIT_LEN.
 */
    unsigned int eccp_pointDouble(eccp_curve_t *curve, unsigned int *Px, unsigned int *Py, unsigned int *Qx, unsigned int *Qy);
#endif

    /**
 * @brief       check whether the input point P is on ECCP curve or not.
 * @param[in]   curve - eccp_curve_t curve struct pointer.
 * @param[in]   Px    - x coordinate of point P.
 * @param[in]   Py    - y coordinate of point P.
 * @return      PKE_SUCCESS(success, on the curve), other(error or not on the curve).
 * @caution     1. please make sure bit length of the curve is not bigger than ECCP_MAX_BIT_LEN.
 *              2. after calculation, A1 and A2 will be changed!
 */
    unsigned int eccp_pointVerify(eccp_curve_t *curve, unsigned int *Px, unsigned int *Py);

    /**
 * @brief       get ECCP public key from private key(the key pair could be used in SM2/ECDSA/ECDH).
 * @param[in]   curve   - eccp_curve_t curve struct pointer.
 * @param[in]   priKey  - private key, big-endian.
 * @param[out]  pubKey  - public key, big-endian.
 * @return      PKE_SUCCESS(success), other(error).
 * @caution     please make sure bit length of the curve is not bigger than ECCP_MAX_BIT_LEN.
 */
    unsigned int eccp_get_pubkey_from_prikey(eccp_curve_t *curve, unsigned char *priKey, unsigned char *pubKey);

    /**
 * @brief       get ECCP key pair(the key pair could be used in SM2/ECDSA/ECDH).
 * @param[in]   curve  - eccp_curve_t curve struct pointer.
 * @param[out]  priKey - private key, big-endian.
 * @param[out]  pubKey - public key, big-endian.
 * @return      PKE_SUCCESS(success), other(error).
 * @caution     please make sure bit length of the curve is not bigger than ECCP_MAX_BIT_LEN.
 */
    unsigned int eccp_getkey(eccp_curve_t *curve, unsigned char *priKey, unsigned char *pubKey);


#ifdef SUPPORT_C25519
    /**
 * @brief       c25519 point mul(random point), Q=[k]P.
 * @param[in]   curve   - c25519 curve struct pointer.
 * @param[in]   k       - scalar.
 * @param[in]   Pu      - u coordinate of point P.
 * @param[out]  Qu      - u coordinate of point Q.
 * @return      PKE_SUCCESS(success), other(error).
 * @caution     1. please make sure input point P is on the curve.
 *              2. even if the input point P is valid, the output may be infinite point, in this case return error.
 *              3. please make sure the curve is c25519.
 */
    unsigned int x25519_pointMul(mont_curve_t *curve, unsigned int *k, const unsigned int *Pu, unsigned int *Qu);

    /**
 * @brief       edwards25519 curve point mul(random point), Q=[k]P.
 * @param[in]   curve   - edwards25519 curve struct pointer.
 * @param[in]   k       - scalar.
 * @param[in]   Px      - x coordinate of point P.
 * @param[in]   Py      - y coordinate of point P.
 * @param[out]  Qx      - x coordinate of point Q.
 * @param[out]  Qy      - y coordinate of point Q.
 * @return      PKE_SUCCESS(success), other(error)
 * @caution     1. please make sure input point P is on the curve.
 *              2. even if the input point P is valid, the output may be neutral point (0, 1), it is valid.
 *              3. please make sure the curve is edwards25519.
 *              4. k could not be zero now.
 */
    unsigned int ed25519_pointMul(edward_curve_t *curve, unsigned int *k, unsigned int *Px, unsigned int *Py, unsigned int *Qx, unsigned int *Qy);

    /**
 * @brief       edwards25519 point add, Q=P1+P2.
 * @param[in]   curve   - edwards25519 curve struct pointer.
 * @param[in]   P1x     - x coordinate of point P1.
 * @param[in]   P1y     - y coordinate of point P1.
 * @param[in]   P2x     - x coordinate of point P2.
 * @param[in]   P2y     - y coordinate of point P2.
 * @param[out]  Qx      - x coordinate of point Q=P1+P2.
 * @param[out]  Qy      - y coordinate of point Q=P1+P2.
 * @return      PKE_SUCCESS(success), other(error).
 * @caution     1. please make sure input point P1 and P2 are both on the curve.
 *              2. the output point may be neutral point (0, 1), it is valid.
 *              3. please make sure the curve is edwards25519.
 */
    unsigned int ed25519_pointAdd(edward_curve_t *curve, unsigned int *P1x, unsigned int *P1y, unsigned int *P2x, unsigned int *P2y, unsigned int *Qx, unsigned int *Qy);

#endif


#ifdef __cplusplus
}
#endif

#endif
