/********************************************************************************************************
 * @file    pke.h
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
#ifndef PKE_H
#define PKE_H

#ifdef __cplusplus
extern "C" {
#endif


#include "reg_include/pke_reg.h"
//#include "pke_common.h"
#include "lib/include/crypto_common/eccp_curve.h"


//#define SUPPORT_SM2
//#define SUPPORT_C25519




/***************** PKE register *******************/
#define PKE_CTRL           (*((volatile unsigned int *)(PKE_BASE_ADDR)))
#define PKE_CFG            (*((volatile unsigned int *)(PKE_BASE_ADDR+0x04)))
#define PKE_MC_PTR         (*((volatile unsigned int *)(PKE_BASE_ADDR+0x08)))
#define PKE_RISR           (*((volatile unsigned int *)(PKE_BASE_ADDR+0x0C)))
#define PKE_IMCR           (*((volatile unsigned int *)(PKE_BASE_ADDR+0x10)))
#define PKE_MISR           (*((volatile unsigned int *)(PKE_BASE_ADDR+0x14)))
#define PKE_RT_CODE        (*((volatile unsigned int *)(PKE_BASE_ADDR+0x24)))
#define PKE_EXE_CONF       (*((volatile unsigned int *)(PKE_BASE_ADDR+0x50)))   //
#define PKE_VERSION        (*((volatile unsigned int *)(PKE_BASE_ADDR+0xFC)))
#define PKE_A(a, step)     ((PKE_BASE_ADDR+0x0400+(a)*(step)))
#define PKE_B(a, step)     ((PKE_BASE_ADDR+0x1000+(a)*(step)))



/*********** PKE register action offset ************/
#define PKE_INT_ENABLE_OFFSET                 (8)
#define PKE_START_CALC                        (1)

#define PKE_EXE_OUTPUT_AFFINE                 (0x10)
#define PKE_EXE_R1_MONT_R0_AFFINE             (0x09)
#define PKE_EXE_R1_MONT_R0_MONT               (0x0A)
#define PKE_EXE_R1_AFFINE_R0_AFFINE           (0x05)
#define PKE_EXE_R1_AFFINE_R0_MONT             (0x06)
#define PKE_EXE_ECCP_POINT_MUL                (PKE_EXE_OUTPUT_AFFINE + PKE_EXE_R1_AFFINE_R0_MONT)
#define PKE_EXE_ECCP_POINT_ADD                (PKE_EXE_OUTPUT_AFFINE + PKE_EXE_R1_AFFINE_R0_AFFINE)
#define PKE_EXE_ECCP_POINT_DBL                (PKE_EXE_OUTPUT_AFFINE + PKE_EXE_R1_MONT_R0_AFFINE)
#define PKE_EXE_ECCP_POINT_VER                (PKE_EXE_OUTPUT_AFFINE + PKE_EXE_R1_AFFINE_R0_MONT)

#define PKE_EXE_CFG_ALL_MONT                  (0x002A)
#define PKE_EXE_CFG_ALL_NON_MONT              (0x0000)
#define PKE_EXE_CFG_MOD_EXP                   (0x0316)


/***************** PKE microcode ******************/
#define MICROCODE_PDBL                        (0x04)
#define MICROCODE_PADD                        (0x08)
#define MICROCODE_PVER                        (0x0C)
#define MICROCODE_PMUL                        (0x10)
#define MICROCODE_MODEXP                      (0x14)
#define MICROCODE_MODMUL                      (0x18)
#define MICROCODE_MODINV                      (0x1C)
#define MICROCODE_MODADD                      (0x20)
#define MICROCODE_MODSUB                      (0x24)
#define MICROCODE_MGMR_PRE                    (0x28)
#define MICROCODE_INTMUL                      (0x2C)
#define MICROCODE_Ed25519_PMUL                (0x30)
#define MICROCODE_Ed25519_PADD                (0x34)
#define MICROCODE_C25519_PMUL                 (0x38)


/*********** some PKE algorithm operand length ************/
#define OPERAND_MAX_BIT_LEN                   (4096)
#define OPERAND_MAX_WORD_LEN                  (GET_WORD_LEN(OPERAND_MAX_BIT_LEN))

#define ECCP_MAX_BIT_LEN                      (521)  //ECC521
#define ECCP_MAX_BYTE_LEN                     (GET_BYTE_LEN(ECCP_MAX_BIT_LEN))
#define ECCP_MAX_WORD_LEN                     (GET_WORD_LEN(ECCP_MAX_BIT_LEN))

#define C25519_BYTE_LEN                       (256/8)
#define C25519_WORD_LEN                       (256/32)

#define Ed25519_BYTE_LEN                      C25519_BYTE_LEN
#define Ed25519_WORD_LEN                      C25519_WORD_LEN

#define MAX_RSA_WORD_LEN                      OPERAND_MAX_WORD_LEN
#define MAX_RSA_BIT_LEN                       (MAX_RSA_WORD_LEN<<5)
#define MIN_RSA_BIT_LEN                       (512)


/******************* PKE return code ********************/
enum PKE_RET_CODE
{
    PKE_SUCCESS = 0,
    PKE_STOP,
    PKE_NO_MODINV,
    PKE_NOT_ON_CURVE,
    PKE_INVALID_MC,
    PKE_ZERO_ALL,                   //for ECCP input check
    PKE_INTEGER_TOO_BIG,            //for ECCP input check
    PKE_INVALID_INPUT,
};


//define KDF
typedef void *(*KDF_FUNC)(const void *input, unsigned int byteLen, unsigned char *key, unsigned int keyByteLen);









//APIs
/**
 * @brief       ainv = a^(-1) mod modulus
 * @param[in]   modulus              - modulus.
 * @param[in]   a                    - integer a.
 * @param[out]  ainv                 - ainv = a^(-1) mod modulus.
 * @param[in]   modWordLen           - word length of modulus and ainv.
 * @param[in]   aWordLen             - word length of a.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.please make sure aWordLen <= modWordLen <= OPERAND_MAX_WORD_LEN and a < modulus.
  @endverbatim
 */
unsigned int pke_modinv(const unsigned int *modulus, const unsigned int *a, unsigned int *ainv, unsigned int modWordLen,
        unsigned int aWordLen);

/**
 * @brief       out = (a+b) mod modulus or out = (a-b) mod modulus
 * @param[in]   modulus              - modulus.
 * @param[in]   a                    - integer a.
 * @param[in]   b                    - integer b.
 * @param[out]  out                  - out = a+b mod modulus or out = (a-b) mod modulus.
 * @param[in]   wordLen              - word length of modulus, a, b.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.a,b must be less than modulus.
      -# 2.wordLen must not be bigger than OPERAND_MAX_WORD_LEN.
  @endverbatim
 */
unsigned int pke_modadd(const unsigned int *modulus, const unsigned int *a, const unsigned int *b,
        unsigned int *out, unsigned int wordLen);

/**
 * @brief       out = (a-b) mod modulus
 * @param[in]   modulus              - modulus.
 * @param[in]   a                    - integer a.
 * @param[in]   b                    - integer b.
 * @param[out]  out                  - out = a+b mod modulus or out = (a-b) mod modulus.
 * @param[in]   wordLen              - word length of modulus, a, b.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.a,b must be less than modulus.
      -# 2.wordLen must not be bigger than OPERAND_MAX_WORD_LEN.
  @endverbatim
 */
unsigned int pke_modsub(const unsigned int *modulus, const unsigned int *a, const unsigned int *b,
        unsigned int *out, unsigned int wordLen);

/**
 * @brief       out = a+b
 * @param[in]   a              - modulus.
 * @param[in]   b              - integer a.
 * @param[in]   out            - integer b.
 * @param[out]  wordLen        - out = a+b mod modulus or out = (a-b) mod modulus.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.a+b may overflow.
      -# 2.wordLen must not be bigger than OPERAND_MAX_WORD_LEN.
  @endverbatim
 */
unsigned int pke_add(const unsigned int *a, const unsigned int *b, unsigned int *out, unsigned int wordLen);

/**
 * @brief       out = a-b
 * @param[in]   a              - integer a.
 * @param[in]   b              - integer b.
 * @param[out]  out            - out = a-b.
 * @param[in]   wordLen        - word length of a, b, out.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.please make sure a > b.
      -# 2.wordLen must not be bigger than OPERAND_MAX_WORD_LEN.
  @endverbatim
 */
unsigned int pke_sub(const unsigned int *a, const unsigned int *b, unsigned int *out, unsigned int wordLen);

/**
 * @brief       out = a*b
 * @param[in]   a              - integer a.
 * @param[in]   b              - integer b.
 * @param[out]  out            - out = a*b.
 * @param[in]   ab_wordLen     - word length of out.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.please make sure out buffer word length is bigger than (2*max_bit_len(a,b)+0x1F)>>5.
      -# 2.please make sure ab_wordLen is not bigger than OPERAND_MAX_WORD_LEN/2.
  @endverbatim
 */
unsigned int pke_mul(const unsigned int *a, const unsigned int *b, unsigned int *out, unsigned int ab_wordLen);

/**
 * @brief       calc H(R^2 mod modulus) and n0'( - modulus ^(-1) mod 2^w ) for modMul,modExp, and pointMul. etc
 *              here w is bit width of word, i,e. 32.
 * @param[in]   modulus              - modulus.
 * @param[in]   bitLen               - bit length of modulus.
 * @param[out]  H                    - R^2 mod modulus.
 * @param[out]  n1                   - modulus ^(-1) mod 2^w, here w is 32 actually.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.modulus must be odd.
      -# 2.please make sure word length of buffer H is equal to wordLen(word length of modulus),
 *        and n0 only need one word.
      -# 3.bitLen must not be bigger than OPERAND_MAX_BIT_LEN
  @endverbatim
 */
unsigned int pke_pre_calc_mont(const unsigned int *modulus, unsigned int bitLen, unsigned int *H, unsigned int *n1);

/**
 * @brief       like function pke_pre_calc_mont(), but this one is without output here.
 * @param[in]   modulus              - modulus.
 * @param[in]   wordLen                  - word length of modulus.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.modulus must be odd.
      -# 2.wordLen must not be bigger than OPERAND_MAX_WORD_LEN.
  @endverbatim
 */
unsigned int pke_pre_calc_mont_no_output(const unsigned int *modulus, unsigned int wordLen);

/**
 * @brief       load modulus and pre-calculated mont parameters H(R^2 mod modulus) and n0'(- modulus ^(-1) mod 2^w) for hardware operation.
 * @param[in]   H              - modulus.
 * @param[in]   n1           - R^2 mod modulus.
 * @param[in]   wordLen              - modulus ^(-1) mod 2^w, here w is 32 actually.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.H must be odd.
      -# 2.wordLen must not be bigger than OPERAND_MAX_BIT_LEN.
  @endverbatim
 */
void pke_load_pre_calc_mont(const unsigned int *H, const unsigned int *n1, unsigned int wordLen);

/**
 * @brief       set modulus and pre-calculated mont parameters H(R^2 mod modulus) and n0'(- modulus ^(-1) mod 2^w) for hardware operation.
 * @param[in]   modulus        - modulus.
 * @param[in]   a              - integer a.
 * @param[in]   b              - integer b.
 * @param[out]  out            - out = a*b mod modulus.
 * @param[in]   wordLen        - word length of modulus, a, b.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.modulus must be odd.
      -# 2.a, b must less than modulus.
      -# 3.wordLen must not be bigger than OPERAND_MAX_WORD_LEN.
      -# 4.before calling this function, please make sure the modulus and the pre-calculated mont arguments
       of modulus are located in the right address.
  @endverbatim
 */
unsigned int pke_modmul_internal(const unsigned int *modulus, const unsigned int *a, const unsigned int *b, unsigned int *out,
        unsigned int wordLen);

/**
 * @brief       out = a*b mod modulus.
 * @param[in]   modulus        - modulus.
 * @param[in]   a              - integer a.
 * @param[in]   b              - integer b.
 * @param[out]  out            - out = a*b mod modulus.
 * @param[in]   wordLen        - word length of modulus, a, b.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.modulus must be odd.
      -# 2.a, b must less than modulus.
      -# 3.wordLen must not be bigger than OPERAND_MAX_WORD_LEN.
  @endverbatim
 */
unsigned int pke_modmul(const unsigned int *modulus, const unsigned int *a, const unsigned int *b, unsigned int *out, unsigned int wordLen);

/**
 * @brief       mod exponent, this could be used for rsa encrypting,decrypting,signing,verifying.
 * @param[in]   modulus        - modulus.
 * @param[in]   exponent       - exponent.
 * @param[in]   base           - base number.
 * @param[out]  out            - out = base^(exponent) mod modulus.
 * @param[in]   mod_wordLen    - word length of modulus and base number.
 * @param[in]   exp_wordLen    - word length of exponent.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# before calling this function, please make sure the pre-calculated mont arguments of modulus are
       located in the right address
      -# 2.modulus must be odd.
      -# 3.please make sure exp_wordLen <= mod_wordLen <= OPERAND_MAX_WORD_LEN.
  @endverbatim
 */
unsigned int pke_modexp(const unsigned int *modulus, const unsigned int *exponent, const unsigned int *base,
        unsigned int *out, unsigned int mod_wordLen, unsigned int exp_wordLen);

/**
 * @brief       c = a mod b.
 * @param[in]   a           - nteger a.
 * @param[in]   aWordLen    - word length of integer.
 * @param[in]   b           - integer b, modulus.
 * @param[in]   b_h         - out = base^(exponent) mod modulus.
 * @param[in]   b_n1        - modulus ^(-1) mod 2^w, here w is 32 actually.
 * @param[in]   bWordLen    - word length of integer b and b_h.
 * @param[out]  c           - c = a mod b.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# b must be odd, and please make sure bWordLen is real word length of b.
      -# 2.real bit length of a can not be bigger than 2*(real bit length of b), so aWordLen can
        not be bigger than 2*bWordLen.
      -# 3.please make sure aWordLen <= 2*OPERAND_MAX_WORD_LEN, bWordLen <= OPERAND_MAX_WORD_LEN.
  @endverbatim
 */
unsigned int pke_mod(unsigned int *a, unsigned int aWordLen, const unsigned int *b, const unsigned int *b_h, const unsigned int *b_n1, unsigned int bWordLen,
        unsigned int *c);

/**
 * @brief       ECCP curve point mul(random point), Q=[k]P.
 * @param[in]   curve   - eccp_curve_t curve struct pointer.
 * @param[in]   k       - scalar.
 * @param[in]   Px      - x coordinate of point P.
 * @param[in]   Py      - y coordinate of point P.
 * @param[out]  Qx      - x coordinate of point Q.
 * @param[out]  Qy      - y coordinate of point Q.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.please make sure k in [1,n-1], n is order of ECCP curve.
      -# 2.please make sure input point P is on the curve.
      -# 3.please make sure bit length of the curve is not bigger than ECCP_MAX_BIT_LEN.
      -# 4.even if the input point P is valid, the output may be infinite point, in this case
       it will return error.
  @endverbatim
 */
unsigned int eccp_pointMul(eccp_curve_t *curve, const unsigned int *k, const unsigned int *Px, const unsigned int *Py,
                      unsigned int *Qx, unsigned int *Qy);

/**
 * @brief       ECCP curve point add, Q=P1+P2.
 * @param[in]   curve       - eccp_curve_t curve struct pointer.
 * @param[in]   P1x         - x coordinate of point P1.
 * @param[in]   P1y         - x coordinate of point P1.
 * @param[in]   P2x         - x coordinate of point P2.
 * @param[in]   P2y         - y coordinate of point P2.
 * @param[out]  Qx          - x coordinate of point Q=P1+P2.
 * @param[out]  Qy          - y coordinate of point Q=P1+P2.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.please make sure input point P1 and P2 are both on the curve.
      -# 2.please make sure input point P is on the curve.
      -# 3.even if the input point P1 and P2 are valid, the output may be infinite point,
       in this case it will return error.
  @endverbatim
 */
unsigned int eccp_pointAdd(eccp_curve_t *curve, unsigned int *P1x, unsigned int *P1y, unsigned int *P2x, unsigned int *P2y,
                      unsigned int *Qx, unsigned int *Qy);

//#define ECCP_POINT_DOUBLE   //recommended not to define
#ifdef ECCP_POINT_DOUBLE
unsigned int eccp_pointDouble(eccp_curve_t *curve, unsigned int *Px, unsigned int *Py, unsigned int *Qx, unsigned int *Qy);
#endif

/**
 * @brief       check whether the input point P is on ECCP curve or not.
 * @param[in]   curve   - eccp_curve_t curve struct pointer.
 * @param[in]   Px      - x coordinate of point P.
 * @param[in]   Py      - y coordinate of point P.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.please make sure bit length of the curve is not bigger than ECCP_MAX_BIT_LEN.
      -# 2.after calculation, A1 and A2 will be changed!.
  @endverbatim
 */
unsigned int eccp_pointVerify(eccp_curve_t *curve, unsigned int *Px, unsigned int *Py);

/**
 * @brief       get ECCP public key from private key(the key pair could be used in SM2/ECDSA/ECDH, etc.).
 * @param[in]   curve        - eccp_curve_t curve struct pointer.
 * @param[in]   priKey       - private key, big-endian.
 * @param[out]  pubKey       - public key, big-endian.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.please make sure bit length of the curve is not bigger than ECCP_MAX_BIT_LEN.
  @endverbatim
 */
unsigned int eccp_get_pubkey_from_prikey(eccp_curve_t *curve, unsigned char *priKey, unsigned char *pubKey);

/**
 * @brief       get ECCP key pair(the key pair could be used in SM2/ECDSA/ECDH).
 * @param[in]   curve        - eccp_curve_t curve struct pointer.
 * @param[out]  priKey       - private key, big-endian.
 * @param[out]  pubKey       - public key, big-endian.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.please make sure bit length of the curve is not bigger than ECCP_MAX_BIT_LEN.
  @endverbatim
 */
unsigned int eccp_getkey(eccp_curve_t *curve, unsigned char *priKey, unsigned char *pubKey);




#ifdef __cplusplus
}
#endif

#endif
