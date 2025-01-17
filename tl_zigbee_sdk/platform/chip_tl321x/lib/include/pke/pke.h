/********************************************************************************************************
 * @file    pke.h
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
#ifndef PKE_H
#define PKE_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "pke_common.h"
#include "lib/include/crypto_common/eccp_curve.h"


#ifdef SUPPORT_SM2
    extern eccp_curve_t sm2_curve[1];
#endif

#ifdef SUPPORT_SM9
    extern eccp_curve_t sm9_curve[1];
#endif

#ifdef SUPPORT_C25519
    extern edward_curve_t ed25519[1];
#endif

#ifdef SUPPORT_C25519
    extern mont_curve_t c25519[1];
#endif
/***************** PKE register *******************/
#define rPKE_CTRL       (*((volatile unsigned int *)(PKE_BASE_ADDR)))
#define rPKE_CFG        (*((volatile unsigned int *)(PKE_BASE_ADDR + 0x04U)))
#define rPKE_MC_PTR     (*((volatile unsigned int *)(PKE_BASE_ADDR + 0x08U)))
#define rPKE_RISR       (*((volatile unsigned int *)(PKE_BASE_ADDR + 0x0CU)))
#define rPKE_IMCR       (*((volatile unsigned int *)(PKE_BASE_ADDR + 0x10U)))
#define rPKE_MISR       (*((volatile unsigned int *)(PKE_BASE_ADDR + 0x14U)))
#define rPKE_RT_CODE    (*((volatile unsigned int *)(PKE_BASE_ADDR + 0x24U)))
#define rPKE_EXE_CONF   (*((volatile unsigned int *)(PKE_BASE_ADDR + 0x50U)))
#define rPKE_VERSION    (*((volatile unsigned int *)(PKE_BASE_ADDR + 0xFCU)))
#define rPKE_A(a, step) ((unsigned int *)(PKE_BASE_ADDR + 0x0400U + (a) * (step)))
#define rPKE_B(a, step) ((unsigned int *)(PKE_BASE_ADDR + 0x1000U + (a) * (step)))


/*********** PKE register action offset ************/
#define PKE_START_CALC              (1U)

#define PKE_EXE_OUTPUT_AFFINE       (0x10U)
#define PKE_EXE_R1_MONT_R0_AFFINE   (0x09U)
#define PKE_EXE_R1_MONT_R0_MONT     (0x0AU)
#define PKE_EXE_R1_AFFINE_R0_AFFINE (0x05U)
#define PKE_EXE_R1_AFFINE_R0_MONT   (0x06U)
#define PKE_EXE_ECCP_POINT_MUL      (PKE_EXE_OUTPUT_AFFINE + PKE_EXE_R1_AFFINE_R0_MONT)
#define PKE_EXE_ECCP_POINT_ADD      (PKE_EXE_OUTPUT_AFFINE + PKE_EXE_R1_AFFINE_R0_AFFINE)
#define PKE_EXE_ECCP_POINT_DBL      (PKE_EXE_OUTPUT_AFFINE + PKE_EXE_R1_MONT_R0_AFFINE)
#define PKE_EXE_ECCP_POINT_VER      (PKE_EXE_OUTPUT_AFFINE + PKE_EXE_R1_AFFINE_R0_MONT)

#define PKE_EXE_CFG_ALL_MONT        (0x002AU)
#define PKE_EXE_CFG_ALL_NON_MONT    (0x0000U)
#define PKE_EXE_CFG_MODEXP          (0x0316U)


/***************** PKE microcode ******************/
#define MICROCODE_PDBL         (0x04U)
#define MICROCODE_PADD         (0x08U)
#define MICROCODE_PVER         (0x0CU)
#define MICROCODE_PMUL         (0x10U)
#define MICROCODE_MODEXP       (0x14U)
#define MICROCODE_MODMUL       (0x18U)
#define MICROCODE_MODINV       (0x1CU)
#define MICROCODE_MODADD       (0x20U)
#define MICROCODE_MODSUB       (0x24U)
#define MICROCODE_MGMR_PRE     (0x28U)
#define MICROCODE_INTMUL       (0x2CU)
#define MICROCODE_Ed25519_PMUL (0x30U)
#define MICROCODE_Ed25519_PADD (0x34U)
#define MICROCODE_C25519_PMUL  (0x38U)


/*********** some PKE algorithm operand length ************/
#define OPERAND_MAX_WORD_LEN (GET_WORD_LEN(OPERAND_MAX_BIT_LEN))

#define ECCP_MAX_BYTE_LEN    (GET_BYTE_LEN(ECCP_MAX_BIT_LEN))
#define ECCP_MAX_WORD_LEN    (GET_WORD_LEN(ECCP_MAX_BIT_LEN))

#define C25519_BYTE_LEN      (256U / 8U)
#define C25519_WORD_LEN      (256U / 32U)

#define Ed25519_BYTE_LEN     C25519_BYTE_LEN
#define Ed25519_WORD_LEN     C25519_WORD_LEN

#define RSA_MAX_WORD_LEN     (GET_WORD_LEN(RSA_MAX_BIT_LEN))
#define RSA_MAX_BYTE_LEN     (GET_BYTE_LEN(RSA_MAX_BIT_LEN))
#define RSA_MIN_BIT_LEN      (512U)

#define DH_MAX_WORD_LEN      (GET_WORD_LEN(DH_MAX_BIT_LEN))
#define DH_MAX_BYTE_LEN      (GET_BYTE_LEN(DH_MAX_BIT_LEN))
#define DH_MIN_BIT_LEN       (512U)

#define SM2_BIT_LEN          (256U)
#define SM2_BYTE_LEN         (32U)
#define SM2_STEPS            (SM2_BYTE_LEN + 4U)
#define SM2_WORD_LEN         (8U)

#define SM9_BASE_BIT_LEN     (256U)
#define SM9_BASE_BYTE_LEN    (SM9_BASE_BIT_LEN / 8U)
#define SM9_STEPS            (SM9_BASE_BYTE_LEN + 4U)
#define SM9_BASE_WORD_LEN    (SM9_BASE_BIT_LEN / 32U)


/******************* PKE return code ********************/
#define PKE_SUCCESS         (0U)
#define PKE_STOP            (1U)
#define PKE_NO_MODINV       (2U)
#define PKE_NOT_ON_CURVE    (3U)
#define PKE_INVALID_MC      (4U)
#define PKE_ZERO_ALL        (5U) //for ECCP input check
#define PKE_INTEGER_TOO_BIG (6U) //for ECCP input check
#define PKE_INVALID_INPUT   (7U)
#define PKE_FINISHED        (8U)
#define PKE_ERROR           (9U)


    /**
 * @brief       Ed25519 decode point
 * @param[in]   in_y              - encoded Ed25519 point
 * @param[in]   out_x            - x coordinate of input point
 * @param[in]   out_y             - y coordinate of input point
 * @return      PKE_SUCCESS(success)     other:error
 */
    unsigned int ed25519_decode_point(unsigned char in_y[32], unsigned char out_x[32], unsigned char out_y[32]);
    //APIs

    /**
 * @brief       a = a*(2^n) or a = a<<n
 * @param[in]   a                    - input/output, big integer
 * @param[out]  aWordLen             - word length of a.
 * @param[in]   n                    - exponent n.
 * @return      result word length
 */
    unsigned int Big_Mul2n(unsigned int a[], int32_t aWordLen, unsigned char n);

    /**
 * @brief       get J0
 * @param[in]   n0                    - a U32 odd integer
 * @return      J0, (- n0 ^(-1) mod 2^w), here w is 32 actually
 */
    unsigned int get_J0(unsigned int n0);

    /**
 * @brief       get pke IP version
 * @return      pke IP version
 */
    unsigned int pke_get_version(void);

    /**
 * @brief       get pke driver version
 * @return      pke driver version(software version)
 */
    unsigned int pke_get_driver_version(void);

    /**
 * @brief       clear finished and interrupt tag
 * @return      none
 */
    void pke_clear_interrupt(void);

    /**
 * @brief       enable pke interrupt
 * @return      none
 */
    void pke_enable_interrupt(void);

    /**
 * @brief       disable pke interrupt
 * @return      none
 */
    void pke_disable_interrupt(void);

    /**
 * @brief       set operand width
 * @param[in]   bitLen         - bit length of operand.
 * @return      uint bytes of hardware operand.
 * @note
  @verbatim
      -# 1.please make sure aWordLen <= modWordLen <= OPERAND_MAX_WORD_LEN and a < modulus.
  @endverbatim
 */
    unsigned int pke_set_operand_width(unsigned int bitLen);

    /**
 * @brief       set operation micro code
 * @return      current operand byte length
 */
    unsigned int pke_get_operand_bytes(void);

    /**
 * @brief       get exe config
 * @return      current exe config value.
 */
    unsigned int pke_get_exe_cfg(void);

    /**
 * @brief       set exe config
 * @param[in]   cfg      - specific config value.
 * @return      none
 */
    void pke_set_exe_cfg(unsigned int cfg);

    /**
 * @brief       ainv = a^(-1) mod modulus
 * @param[in]   modulus              - modulus.
 * @param[in]   a                    - integer a.
 * @param[out]  ainv                 - ainv = a^(-1) mod modulus.
 * @param[in]   modWordLen           - word length of modulus and ainv.
 * @param[in]   aWordLen             - word length of a.
 * @return      PKE_SUCCESS(success), other(inverse not exists or error)
 * @note
  @verbatim
      -# 1.please make sure aWordLen <= modWordLen <= OPERAND_MAX_WORD_LEN and a < modulus.
  @endverbatim
 */
    unsigned int pke_modinv(unsigned int *modulus, unsigned int *a, unsigned int *ainv, unsigned int modWordLen, unsigned int aWordLen);

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
    unsigned int pke_modadd(unsigned int *modulus, unsigned int *a, unsigned int *b, unsigned int *out, unsigned int wordLen);

    /**
 * @brief       out = (a-b) mod modulus
 * @param[in]   modulus              - modulus.
 * @param[in]   a                    - integer a.
 * @param[in]   b                    - integer b.
 * @param[out]  out                  - out = a+b mod modulus or out = (a-b) mod modulus.
 * @param[in]   wordLen              - word length of modulus, a, b.
 * @return      PKE_SUCCESS(success), other(error)
 * @note
  @verbatim
      -# 1.a,b must be less than modulus.
      -# 2.wordLen must not be bigger than OPERAND_MAX_WORD_LEN.
  @endverbatim
 */
    unsigned int pke_modsub(unsigned int *modulus, unsigned int *a, unsigned int *b, unsigned int *out, unsigned int wordLen);

    /**
 * @brief       out = a+b
 * @param[in]   a              - modulus.
 * @param[in]   b              - integer a.
 * @param[in]   out            - integer b.
 * @param[out]  wordLen        - word length of a, b, out
 * @return      PKE_SUCCESS(success), other(error)
 * @note
  @verbatim
      -# 1.a+b may overflow.
      -# 2.wordLen must not be bigger than OPERAND_MAX_WORD_LEN.
  @endverbatim
 */
    unsigned int pke_add(unsigned int *a, unsigned int *b, unsigned int *out, unsigned int wordLen);

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
    unsigned int pke_sub(unsigned int *a, unsigned int *b, unsigned int *out, unsigned int wordLen);

    /**
 * @brief       out = a*b
 * @param[in]   a              - integer a.
 * @param[in]   a_wordLen      - word length of a.
 * @param[in]   b              - integer b.
 * @param[in]   b_wordLen      - word length of b.
 * @param[out]  out            - out = a*b.
 * @param[in]   out_wordLen    - word length of out.
 * @return      0:PKE_SUCCESS     other:error
 * @note
  @verbatim
      -# 1.please make sure out buffer word length is bigger than (2*max_bit_len(a,b)+0x1F)>>5.
      -# 2.please make sure ab_wordLen is not bigger than OPERAND_MAX_WORD_LEN/2.
  @endverbatim
 */
    unsigned int pke_mul_internal(unsigned int *a, unsigned int *b, unsigned int *out, unsigned int a_wordLen, unsigned int b_wordLen, unsigned int out_wordLen);

    /**
 * @brief       out = a*b
 * @param[in]   a              - integer a.
 * @param[in]   b              - integer b.
 * @param[out]  out            - out = a*b.
 * @param[in]   ab_wordLen        - word length of a, b, out.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.please make sure out buffer word length is bigger than (2*max_bit_len(a,b)+0x1F)>>5
      -# 2.please make sure ab_wordLen is not bigger than OPERAND_MAX_WORD_LEN/2
  @endverbatim
 */
    unsigned int pke_mul(unsigned int *a, unsigned int *b, unsigned int *out, unsigned int ab_wordLen);

#ifndef PKE_CONFIG_ALL_MODEXP_PRE_CALC_WITH_MGMR_MICROCODE
    /**
 * @brief       calc H(R^2 mod modulus) and n0'( - modulus ^(-1) mod 2^w ) for modMul,modExp, and pointMul. etc
 *              here w is bit width of word, i,e. 32.
 * @param[in]   modulus              - modulus.
 * @param[in]   bitLen               - bit length of modulus, must be multiple of 32
 * @param[out]  H                    - R^2 mod modulus.
 * @param[out]  n0                   - modulus ^(-1) mod 2^w, here w is 32 actually.
 * @return      PKE_SUCCESS(success)     other:error
 * @note
  @verbatim
      -# 1.modulus must be odd.
      -# 2.please make sure word length of buffer H is equal to wordLen(word length of modulus),
 *        and n0 only need one word.
      -# 3.bitLen must not be bigger than OPERAND_MAX_BIT_LEN
  @endverbatim
 */
    unsigned int pke_pre_calc_mont_without_mgmr_microcode(unsigned int *modulus, unsigned int bitLen, unsigned int *H, unsigned int *n0);
#endif
    /**
 * @brief       calc H(R^2 mod modulus) and n0'( - modulus ^(-1) mod 2^w ) for modMul,modExp, and pointMul. etc
 *              here w is bit width of word, i,e. 32.
 * @param[in]   modulus              - modulus.
 * @param[in]   bitLen               - bit length of modulus
 * @param[out]  H                    - R^2 mod modulus.
 * @param[out]  n0                   - modulus ^(-1) mod 2^w, here w is 32 actually.
 * @return      PKE_SUCCESS(success)     other:error
 * @note
  @verbatim
      -# 1.modulus must be odd.
      -# 2.please make sure word length of buffer H is equal to wordLen(word length of modulus),
 *        and n0 only need one word.
      -# 3.bitLen must not be bigger than OPERAND_MAX_BIT_LEN
  @endverbatim
 */
    unsigned int pke_pre_calc_mont(unsigned int *modulus, unsigned int bitLen, unsigned int *H, unsigned int *n0);

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
    unsigned int pke_pre_calc_mont_no_output(unsigned int *modulus, unsigned int wordLen);

    /**
 * @brief       like function pke_pre_calc_mont(), but this one is for modexp
 * @param[in]   modulus              - modulus.
 * @param[in]   bitLen               - bit length of modulus.
 * @param[out]  H                    - R^2 mod modulus
 * @param[out]  n0                   - modulus ^(-1) mod 2^w, here w is 32 actually
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.modulus must be odd.
      -# 2.please make sure word length of buffer H is equal to wordLen(word length of modulus),and n0 only need one word.
      -# 3.bitLen must not be bigger than OPERAND_MAX_BIT_LEN
   @endverbatim
 */
    unsigned int pke_pre_calc_mont_for_modexp(unsigned int *modulus, unsigned int bitLen, unsigned int *H, unsigned int *n0);

    /**
 * @brief       like function pke_pre_calc_mont(), but this one is for modexp
 * @param[in]   modulus              - modulus.
 * @param[out]  modulus_h            - R^2 mod modulus
 * @param[out]  modulus_n0           - modulus ^(-1) mod 2^w, here w is 32 actually
 * @param[in]   bitLen               - bit length of modulus
 * @return      PKE_SUCCESS(success)     other:error
 * @note
  @verbatim
      -# 1.modulus must be odd.
      -# 2.bitLen must not be bigger than OPERAND_MAX_BIT_LEN
   @endverbatim
 */
    unsigned int pke_load_modulus_and_pre_monts(unsigned int *modulus, unsigned int *modulus_h, unsigned int *modulus_n0, unsigned int bitLen);

    /**
 * @brief       set modulus and pre-calculated mont parameters H(R^2 mod modulus) and n0'(- modulus ^(-1) mod 2^w) for hardware operation
 * @param[in]   modulus             - modulus.
 * @param[in]  modulus_h            - R^2 mod modulus
 * @param[in]  modulus_n0           - modulus ^(-1) mod 2^w, here w is 32 actually
 * @param[in]  bitLen               - bit length of modulus
 * @return      PKE_SUCCESS(success)     other:error
 * @note
  @verbatim
      -# 1.modulus must be odd.
      -# 2.bitLen must not be bigger than OPERAND_MAX_BIT_LEN
   @endverbatim
 */
    unsigned int pke_set_modulus_and_pre_monts(unsigned int *modulus, unsigned int *modulus_h, unsigned int *modulus_n0, unsigned int bitLen);
    /**
 * @brief       set modulus and pre-calculated mont parameters H(R^2 mod modulus) and n0'(- modulus ^(-1) mod 2^w) for hardware operation.
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
    unsigned int pke_modmul_internal(unsigned int *a, unsigned int *b, unsigned int *out, unsigned int wordLen);

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
    unsigned int pke_modmul(unsigned int *modulus, unsigned int *a, unsigned int *b, unsigned int *out, unsigned int wordLen);

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
    unsigned int pke_modexp(unsigned int *modulus, unsigned int *exponent, unsigned int *base, unsigned int *out, unsigned int mod_wordLen, unsigned int exp_wordLen);

    /**
 * @brief       check input before mod exponent
 * @param[in]   modulus              - modulus.
 * @param[in]   exponent             - exponent.
 * @param[in]   base                 - base number.
 * @param[out]  out                  - out = base^(exponent) mod modulus.
 * @param[in]   mod_wordLen          - word length of modulus and base number.
 * @param[in]   exp_wordLen          - word length of exponent.
 * @return      PKE_SUCCESS(input is valid, allow to calculate),PKE_FINISHED(mod exponent finished),other(error)
 * @note
  @verbatim
      -# 1.modulus must be odd
      -# 2.please make sure exp_wordLen <= mod_wordLen <= OPERAND_MAX_WORD_LEN
  @endverbatim
 */
    unsigned int pke_modexp_check_input(unsigned int *modulus, unsigned int *exponent, unsigned int *base, unsigned int *out, unsigned int mod_wordLen, unsigned int exp_wordLen);

    /**
 * @brief       od exponent(for high level use, operands are all U8 big-endian big number), this could be used for rsa encrypting,decrypting,signing,verifying.
 * @param[in]   modulus              - modulus.
 * @param[in]   exponent             - exponent.
 * @param[in]   base                 - base number.
 * @param[out]  out                  - out = base^(exponent) mod modulus.
 * @param[in]   mod_bitLen           - real bit length of modulus and base number.
 * @param[in]   exp_bitLen           - real bit length of exponent.
 * @param[in]   calc_pre_monts       - if it is 0, no need to calculate the pre-calculated mont arguments of modulus, otherwise calculate.
 * @return      PKE_SUCCESS(input is valid, allow to calculate),PKE_FINISHED(mod exponent finished),other(error)
 * @note
  @verbatim
      -# 1.modulus must be odd
      -# 2.please make sure exp_wordLen <= mod_wordLen <= OPERAND_MAX_WORD_LEN
      -# 3.this is for high level application or protocol to use RSA mod exponent directly. all operands of this API are U8 big-endian big number.
  @endverbatim
 */
    unsigned int pke_modexp_U8(unsigned char *modulus, unsigned char *exponent, unsigned char *base, unsigned char *out, unsigned int mod_bitLen, unsigned int exp_bitLen, unsigned int calc_pre_monts);

    /**
 * @brief       c = a mod b.
 * @param[in]   a           - nteger a.
 * @param[in]   aWordLen    - word length of integer.
 * @param[in]   b           - integer b, modulus.
 * @param[in]   b_h         - out = base^(exponent) mod modulus.
 * @param[in]   b_n0        - modulus ^(-1) mod 2^w, here w is 32 actually.
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
    unsigned int pke_mod(unsigned int *a, unsigned int aWordLen, unsigned int *b, unsigned int *b_h, unsigned int *b_n0, unsigned int bWordLen, unsigned int *c);
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
    unsigned int eccp_pointMul(eccp_curve_t *curve, unsigned int *k, unsigned int *Px, unsigned int *Py, unsigned int *Qx, unsigned int *Qy);

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
      -# 3.even if the input point P1 and P2 are valid, it will return error in the following 2 cases.
           (1). P1 = P2. return PKE_NO_MODINV.
           (2). P1 = -P2. return PKE_NO_MODINV. actually the output point is neutral point(point at infinity)
  @endverbatim
 */
    unsigned int eccp_pointAdd(eccp_curve_t *curve, unsigned int *P1x, unsigned int *P1y, unsigned int *P2x, unsigned int *P2y, unsigned int *Qx, unsigned int *Qy);

    /**
 * @brief       out = (a+b) mod modulus or out = (a-b) mod modulus
 * @param[in]   curve              - eccp_curve_t curve struct pointer
 * @param[in]   P1x                - x coordinate of point P1
 * @param[in]   P1y                - y coordinate of point P1
 * @param[in]   P2x                - x coordinate of point P2
 * @param[in]   P2y                - y coordinate of point P2
 * @param[out]   Qx                - x coordinate of point Q=P1+P2
 * @param[out]   Qy                - y coordinate of point Q=P1+P2
 * @return      PKE_SUCCESS(success)     other:error
 * @note
  @verbatim
      -# 1.please make sure input point P1 and P2 are both on the curve
      -# 2.please make sure bit length of the curve is not greater than ECCP_MAX_BIT_LEN
      -# 3.if P1 = -P2, it will return PKE_NO_MODINV. actually the output point is neutral point(point at infinity)
  @endverbatim
 */
    unsigned int eccp_pointAdd_safe(eccp_curve_t *curve, unsigned int *P1x, unsigned int *P1y, unsigned int *P2x, unsigned int *P2y, unsigned int *Qx, unsigned int *Qy);

//#define ECCP_POINT_DOUBLE   //not recommended to define
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


#ifdef SUPPORT_C25519
    /**
 * @brief       c25519 point mul(random point), Q=[k]P
 * @param[in]   curve              - c25519 curve struct pointer
 * @param[in]   k                  - scalar
 * @param[in]   Pu                 - u coordinate of point P
 * @param[out]  Qu                 - u coordinate of point Q
 * @return      PKE_SUCCESS(success)     other:error
 * @note
  @verbatim
      -# 1.please make sure input point P is on the curve
      -# 2.even if the input point P is valid, the output may be infinite point, in this case return error.
      -# 3.please make sure the curve is c25519
  @endverbatim
 */
    unsigned int x25519_pointMul(mont_curve_t *curve, unsigned int *k, unsigned int *Pu, unsigned int *Qu);

    /**
 * @brief       edwards25519 curve point mul(random point), Q=[k]P
 * @param[in]   curve                - edwards25519 curve struct pointer
 * @param[in]   k                    - scalar
 * @param[in]   Px                   - x coordinate of point P
 * @param[in]   Py                   - y coordinate of point P
 * @param[out]  Qx                   - x coordinate of point Q
 * @param[out]  Qy                   - y coordinate of point Q
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.please make sure input point P is on the curve
      -# 2.even if the input point P is valid, the output may be neutral point (0, 1), it is valid
      -# 3.please make sure the curve is edwards25519
      -# 4.k could not be zero now.
  @endverbatim
 */
    unsigned int ed25519_pointMul(edward_curve_t *curve, unsigned int *k, unsigned int *Px, unsigned int *Py, unsigned int *Qx, unsigned int *Qy);

    /**
 * @brief       edwards25519 point add, Q=P1+P2
 * @param[in]   curve                - edwards25519 curve struct pointer
 * @param[in]   P1x                  - x coordinate of point P1
 * @param[in]   P1y                  - y coordinate of point P1
 * @param[in]   P2x                  - x coordinate of point P2
 * @param[in]   P2y                  - y coordinate of point P2
 * @param[out]  Qx                   - x coordinate of point Q=P1+P2
 * @param[out]  Qy                   - y coordinate of point Q=P1+P2
 * @return      PKE_SUCCESS(success),     other:error
 * @note
  @verbatim
      -# 1.please make sure input point P1 and P2 are both on the curve
      -# 2.the output point may be neutral point (0, 1), it is valid
      -# 3.please make sure the curve is edwards25519
  @endverbatim
 */
    unsigned int ed25519_pointAdd(edward_curve_t *curve, unsigned int *P1x, unsigned int *P1y, unsigned int *P2x, unsigned int *P2y, unsigned int *Qx, unsigned int *Qy);

#endif


#ifdef __cplusplus
}
#endif

#endif
