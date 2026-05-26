/********************************************************************************************************
 * @file    pke.h
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
#ifndef PKE_H
#define PKE_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "pke_common.h"
#include "lib/include/crypto_common/eccp_curve.h"


/***************** PKE register *******************/
#ifndef SUPPORT_STATIC_ANALYSIS
#define rPKE_CTRL     (*((volatile unsigned int *)(PKE_BASE_ADDR)))
#define rPKE_CFG      (*((volatile unsigned int *)(PKE_BASE_ADDR + 0x04U)))
#define rPKE_MC_PTR   (*((volatile unsigned int *)(PKE_BASE_ADDR + 0x08U)))
#define rPKE_RISR     (*((volatile unsigned int *)(PKE_BASE_ADDR + 0x0CU)))
#define rPKE_IMCR     (*((volatile unsigned int *)(PKE_BASE_ADDR + 0x10U)))
#define rPKE_MISR     (*((volatile unsigned int *)(PKE_BASE_ADDR + 0x14U)))
#define rPKE_RT_CODE  (*((volatile unsigned int *)(PKE_BASE_ADDR + 0x24U)))
#define rPKE_EXE_CONF (*((volatile unsigned int *)(PKE_BASE_ADDR + 0x50U)))
#define rPKE_VERSION  (*((volatile unsigned int *)(PKE_BASE_ADDR + 0xFCU)))
#else

/**
 * @brief           Structure representing the PKE registers.
 */
typedef struct
{
    unsigned int PKE_CTRL[1];   // 0x00
    unsigned int PKE_CFG[1];    // 0x04
    unsigned int PKE_MC_PTR[1]; // 0x08
    unsigned int PKE_RISR[1];   // 0x0C
    unsigned int PKE_IMCR[1];   // 0x10
    unsigned int PKE_MISR[1];   // 0x14
    unsigned int REV_1[3];
    unsigned int PKE_RT_CODE[1]; // 0x24
    unsigned int REV_2[10];
    unsigned int PKE_EXE_CONF[1]; // 0x50
    unsigned int REV_3[42];
    unsigned int PKE_VERSION[1]; // 0xFC
} pke_reg_t;

#ifdef CONFIG_UNIT_TEST
extern volatile pke_reg_t *g_pke_reg;
#else
extern volatile pke_reg_t *const g_pke_reg;
#endif

#define rPKE_CTRL     (*((volatile unsigned int *)(g_pke_reg->PKE_CTRL)))
#define rPKE_CFG      (*((volatile unsigned int *)(g_pke_reg->PKE_CFG)))
#define rPKE_MC_PTR   (*((volatile unsigned int *)(g_pke_reg->PKE_MC_PTR)))
#define rPKE_RISR     (*((volatile unsigned int *)(g_pke_reg->PKE_RISR)))
#define rPKE_IMCR     (*((volatile unsigned int *)(g_pke_reg->PKE_IMCR)))
#define rPKE_MISR     (*((volatile unsigned int *)(g_pke_reg->PKE_MISR)))
#define rPKE_RT_CODE  (*((volatile unsigned int *)(g_pke_reg->PKE_RT_CODE)))
#define rPKE_EXE_CONF (*((volatile unsigned int *)(g_pke_reg->PKE_EXE_CONF)))
#define rPKE_VERSION  (*((volatile unsigned int *)(g_pke_reg->PKE_VERSION)))

#endif

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
#define OPERAND_MAX_WORD_LEN ((OPERAND_MAX_BIT_LEN + 31u) >> 5)

#define ECCP_MAX_BYTE_LEN    ((ECCP_MAX_BIT_LEN + 7u) >> 3)
#define ECCP_MAX_WORD_LEN    ((ECCP_MAX_BIT_LEN + 31u) >> 5)

#define C25519_BYTE_LEN      (256U / 8U)
#define C25519_WORD_LEN      (256U / 32U)

#define Ed25519_BYTE_LEN     C25519_BYTE_LEN
#define Ed25519_WORD_LEN     C25519_WORD_LEN

#define RSA_MAX_WORD_LEN     ((RSA_MAX_BIT_LEN + 31u) >> 5)
#define RSA_MAX_BYTE_LEN     ((RSA_MAX_BIT_LEN + 7u) >> 3)
#define RSA_MIN_BIT_LEN      (512U)

#define DH_MAX_WORD_LEN      ((DH_MAX_BIT_LEN + 31u) >> 5)
#define DH_MAX_BYTE_LEN      ((DH_MAX_BIT_LEN + 7u) >> 3)
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
#define PKE_SUCCESS (0U)
// #define PKE_STOP                              (1U)
#define PKE_NO_MODINV    (2U)
#define PKE_NOT_ON_CURVE (3U)
// #define PKE_INVALID_MC                        (4U)
#define PKE_ZERO_ALL        (10U) /**< for ECCP input check */
#define PKE_INTEGER_TOO_BIG (11U) /**< for ECCP input check */
#define PKE_INVALID_INPUT   (12U)
#define PKE_FINISHED        (13U)
#define PKE_POINTER_NULL    (14U)
#define PKE_ERROR           (15U)


#define eccp_pointAdd       eccp_pointadd
#define eccp_pointAdd_safe  eccp_pointadd_safe
#define eccp_pointMul       eccp_pointmul
#define eccp_pointMul_      eccp_pointmul_
#define eccp_pointVerify    eccp_pointverify
#define x25519_pointMul     x25519_pointmul


    // Maintenance API
    /**
 * @brief           to clear finished and interrupt tag
 * @return          None
 */
    void pke_clr_irq_status(void);

    /**
 * @brief           get pke irq status
 * @return          None
 */
    unsigned int pke_get_irq_status(void);

    /**
 * @brief           to start PKE calculation
 * @return          None
 */
    void pke_opr_start(void);

    /**
 * @brief           Computes out = (a + b) mod modulus.
 * @param[in]       modulus              - Modulus value.
 * @param[in]       a                    - Integer a, must be less than modulus.
 * @param[in]       b                    - Integer b, must be less than modulus.
 * @param[out]      out                  - Result of (a + b) mod modulus.
 * @param[in]       wlen                 - Word length of modulus, a, and b, must not be bigger than OPERAND_MAX_WORD_LEN.
 * @return          PKE_SUCCESS on success, other values indicate an error.
 * @note            
 *        1. a and b must be less than modulus.
 *        2. wlen must not be bigger than OPERAND_MAX_WORD_LEN.
 */
    unsigned int pke_mod_add(const unsigned int *modulus, const unsigned int *a, const unsigned int *b, unsigned int *out, unsigned int wlen);

    /**
 * @brief           Computes out = (a + b) mod modulus.
 * @param[in]       modulus              - Modulus value.
 * @param[in]       a                    - Integer a.
 * @param[in]       b                    - Integer b.
 * @param[out]      out                  - Result of (a + b) mod modulus.
 * @param[in]       wlen                 - Word length of modulus, a, and b.
 * @return          PKE_SUCCESS on success, other values indicate an error.
 * @note            
 *        1. a and b must be less than modulus.
 *        2. wlen must not be bigger than OPERAND_MAX_WORD_LEN.
 */
    unsigned int pke_mod_sub(const unsigned int *modulus, const unsigned int *a, const unsigned int *b, unsigned int *out, unsigned int wlen);

    /**
 * @brief           Computes out = a * b (mod modulus).
 * @param[in]       modulus              - Modulus value.
 * @param[in]       a                    - Integer a.
 * @param[in]       b                    - Integer b.
 * @param[out]      out                  - Result of a * b mod modulus.
 * @param[in]       wlen                 - Word length of modulus, a, and b.
 * @return          PKE_SUCCESS on success, other values indicate an error.
 * @note            
 *        1. Modulus must be odd.
 *        2. a, b must be less than modulus.
 *        3. wlen must not be bigger than OPERAND_MAX_WORD_LEN.
 */
    unsigned int pke_mod_mul(const unsigned int *modulus, const unsigned int *a, const unsigned int *b, unsigned int *out, unsigned int wlen);

    /**
 * @brief           Computes the modular inverse: ainv = a^(-1) mod modulus.
 * @param[in]       modulus              - Modulus value.
 * @param[in]       a                    - Integer a.
 * @param[out]      ainv                 - ainv = a^(-1) mod modulus.
 * @param[in]       mod_wlen             - Word length of modulus and ainv.
 * @param[in]       a_wlen               - Word length of a.
 * @return          PKE_SUCCESS on success, other values indicate an error or inverse does not exist.
 * @note            
 *        1. Please make sure a_wlen <= mod_wlen <= OPERAND_MAX_WORD_LEN and a < modulus.
  */
    unsigned int pke_mod_inv(const unsigned int *modulus, const unsigned int *a, unsigned int *ainv, unsigned int mod_wlen, unsigned int a_wlen);

    /**
 * @brief           ECCP curve point multiplication (random point), Q = [k]P.
 * @param[in]       curve                - Pointer to eccp_curve_t curve struct.
 * @param[in]       k                    - Scalar.
 * @param[in]       px                   - x coordinate of point P (unsigned char big-endian big number).
 * @param[in]       Py                   - y coordinate of point P (unsigned char big-endian big number).
 * @param[out]      qx                   - x coordinate of point Q (unsigned char big-endian big number).
 * @param[out]      qy                   - y coordinate of point Q (unsigned char big-endian big number).
 * @return          PKE_SUCCESS on success, other values indicate an error.
 * @note            
 *        1. Please make sure k is in [1, n-1], where n is the order of the ECCP curve.
 *        2. Please make sure the input point P is on the curve.
 *        3. Please make sure the bit length of the curve is not bigger than ECCP_MAX_BIT_LEN.
 *        4. Even if the input point P is valid, the output may be the infinite point, in which case it will return an error.
  */
    unsigned int pke_eccp_point_mul(const eccp_curve_t *curve, const unsigned int *k, const unsigned int *px, const unsigned int *Py, unsigned int *qx, unsigned int *qy);

    /**
 * @brief           ECCP curve point addition, Q = P1 + P2.
 * @param[in]       curve                - Pointer to eccp_curve_t curve struct.
 * @param[in]       x1                   - x coordinate of point P1 (unsigned char big-endian big number).
 * @param[in]       y1                   - y coordinate of point P1 (unsigned char big-endian big number).
 * @param[in]       x2                   - x coordinate of point P2 (unsigned char big-endian big number).
 * @param[in]       y2                   - y coordinate of point P2 (unsigned char big-endian big number).
 * @param[out]      qx                   - x coordinate of point Q = P1 + P2 (unsigned char big-endian big number).
 * @param[out]      qy                   - y coordinate of point Q = P1 + P2 (unsigned char big-endian big number).
 * @return          PKE_SUCCESS on success, other values indicate an error.
 * @note            
 *        1. Please make sure input points P1 and P2 are both on the curve.
 *        2. Please make sure the bit length of the curve is not greater than ECCP_MAX_BIT_LEN.
 *        3. Even if the input points P1 and P2 are valid, it will return an error in the following cases:
 *        3.1. P1 = P2. Return PKE_NO_MODINV.
 *        3.2. P1 = -P2. Return PKE_NO_MODINV. Actually, the output point is the neutral point (point at infinity).
  */
    unsigned int pke_eccp_point_add(const eccp_curve_t *curve, const unsigned int *x1, const unsigned int *y1, const unsigned int *x2, const unsigned int *y2, unsigned int *qx,
                                    unsigned int *qy);

    /**
 * @brief           Checks whether the input point P is on the ECCP curve or not.
 * @param[in]       curve                - Pointer to eccp_curve_t curve struct.
 * @param[in]       px                   - x coordinate of point P (unsigned char big-endian big number).
 * @param[in]       Py                   - y coordinate of point P (unsigned char big-endian big number).
 * @return          PKE_SUCCESS on success, point is on the curve), other values indicate an error or not on the curve.
 * @note            
 *        1. Please make sure the bit length of the curve is not bigger than ECCP_MAX_BIT_LEN.
  */
    unsigned int pke_eccp_point_verify(const eccp_curve_t *curve, const unsigned int *px, const unsigned int *Py);

#ifdef SUPPORT_C25519
    /**
 * @brief           C25519 point multiplication (random point), Q = [k]P.
 * @param[in]       curve                - Pointer to c25519 curve struct.
 * @param[in]       k                    - Scalar.
 * @param[in]       p                    - u coordinate of point P (unsigned char big-endian).
 * @param[out]      q                    - u coordinate of point Q (unsigned char big-endian).
 * @return          PKE_SUCCESS on success, other values indicate an error.
 * @note            
 *        1. Please make sure the input point P is on the curve.
 *        2. Even if the input point P is valid, the output may be the infinite point, in which case it will return an error.
 *        3. Please make sure the curve is C25519.
 */
    unsigned int pke_x25519_point_mul(const mont_curve_t *curve, const unsigned int *k, const unsigned int *p, unsigned int *q);

    /**
 * @brief           Edwards25519 curve point multiplication (random point), Q = [k]P.
 * @param[in]       curve                - Pointer to edwards25519 curve struct.
 * @param[in]       k                    - Scalar.
 * @param[in]       px                   - x coordinate of point P (unsigned char big-endian).
 * @param[in]       Py                   - y coordinate of point P (unsigned char big-endian).
 * @param[out]      qx                   - x coordinate of point Q (unsigned char big-endian).
 * @param[out]      qy                   - y coordinate of point Q (unsigned char big-endian).
 * @return          PKE_SUCCESS on success, other values indicate an error.
 * @note            
 *        1. Please make sure the input point P is on the curve.
 *        2. Even if the input point P is valid, the output may be the neutral point (0, 1), which is valid.
 *        3. Please make sure the curve is Edwards25519.
 *        4. k could not be zero.
  */
    unsigned int pke_ed25519_point_mul(const edward_curve_t *curve, const unsigned int *k, const unsigned int *px, const unsigned int *Py, unsigned int *qx, unsigned int *qy);

    /**
 * @brief           Edwards25519 point addition, Q = P1 + P2.
 * @param[in]       curve                - Pointer to edwards25519 curve struct.
 * @param[in]       x1                   - x coordinate of point P1 (unsigned char big-endian).
 * @param[in]       y1                   - y coordinate of point P1 (unsigned char big-endian).
 * @param[in]       x2                   - x coordinate of point P2 (unsigned char big-endian).
 * @param[in]       y2                   - y coordinate of point P2 (unsigned char big-endian).
 * @param[out]      qx                   - x coordinate of point Q = P1 + P2 (unsigned char big-endian).
 * @param[out]      qy                   - y coordinate of point Q = P1 + P2 (unsigned char big-endian).
 * @return          PKE_SUCCESS on success, other values indicate an error.
 * @note            
 *        1. Please make sure the input points P1 and P2 are both on the curve.
 *        2. The output point may be the neutral point (0, 1), which is valid.
 *        3. Please make sure the curve is Edwards25519.
  */
    unsigned int pke_ed25519_point_add(const edward_curve_t *curve, const unsigned int *x1, const unsigned int *y1, const unsigned int *x2, const unsigned int *y2,
                                       unsigned int *qx, unsigned int *qy);
#endif

    /**
 * @brief           Compute out = a - b.
 * @param[in]       a                    - Integer a.
 * @param[in]       b                    - Integer b.
 * @param[out]      out                  - Result of a - b.
 * @param[in]       wlen                 - Word length of a, b, and out.
 * @return          PKE_SUCCESS on success, other values indicate an error.
 * @note            
 *        1. Please make sure a > b.
 *        2. wlen must not be bigger than OPERAND_MAX_WORD_LEN.
 */
    unsigned int sub_u32(const unsigned int *a, const unsigned int *b, unsigned int *out, unsigned int wlen);

    /**
 * @brief           Perform a = a * (2^n) or a = a << n.
 * @param[in,out]   a                    - Input/big integer.
 * @param[in]       a_wlen               - Word length of a.
 * @param[in]       n                    - Exponent n.
 * @return          Result word length.
  */
    unsigned int big_mul_2n(unsigned int a[], int32_t a_wlen, unsigned char n);

    /**
 * @brief           Get J0.
 * @param[in]       n0                   - A unsigned int odd integer.
 * @return          J0, (-n0^(-1) mod 2^w), here w is 32 actually.
  */
    unsigned int get_j0(unsigned int n0);

    /**
 * @brief           Get PKE RAM A slot.
 * @param[in]       index                - Index of slot (0, 1, 2, ...).
 * @param[in]       step                 - Byte length of slot.
 * @return          Unsigned int pointer to the slot.
  */
    unsigned int *rPKE_A(unsigned int index, unsigned int step);

    /**
 * @brief           Get PKE RAM B slot.
 * @param[in]       index                - Index of slot (0, 1, 2, ...).
 * @param[in]       step                 - Byte length of slot.
 * @return          Pointer to the slot as unsigned int.
  */
    unsigned int *rPKE_B(unsigned int index, unsigned int step);

    /**
 * @brief           Get PKE IP version.
 * @return          PKE IP version.
  */
    unsigned int pke_get_version(void);

    /**
 * @brief           Get PKE driver version (software version).
 * @return          PKE driver version.
  */
    unsigned int pke_get_driver_version(void);

    /**
 * @brief           Clear finished and interrupt tag.
 * @return          None.
  */
    void pke_clear_interrupt(void);

    /**
 * @brief           Enable PKE interrupt.
 * @return          None.
  */
    void pke_enable_interrupt(void);

    /**
 * @brief           Disable PKE interrupt.
 * @return          None.
  */
    void pke_disable_interrupt(void);

    /**
 * @brief           Set operand width.
 * @param[in]       bitLen               - Bit length of operand.
 * @return          Uint bytes of hardware operand.
 * @note            Please make sure 0 < bitLen <= OPERAND_MAX_BIT_LEN.
  */
    unsigned int pke_set_operand_width(unsigned int bitLen);

    /**
 * @brief           Get current operand byte length.
 * @return          Current operand byte length.
  */
    unsigned int pke_get_operand_bytes(void);

    /**
 * @brief           Set operation micro code.
 * @param[in]       addr                 - Specific micro code address.
 * @return          None.
  */
    void pke_set_microcode(unsigned int addr);

    /**
 * @brief           Get execution configuration.
 * @return          Current execution configuration value.
  */
    unsigned int pke_get_exe_cfg(void);

    /**
 * @brief           Start PKE calculation.
 * @return          None.
  */
    void pke_start(void);

    /**
 * @brief           Get calculation return code.
 * @return          0 (success), other values indicate an error.
  */
    unsigned int pke_check_rt_code(void);

    /**
 * @brief           Wait until the operation is complete.
 * @return          None.
  */
    void pke_wait_till_done(void);

    /**
 * @brief           Set operation micro code, start hardware, wait until done, and return the result code.
 * @param[in]       micro_code           - Specific micro code.
 * @return          PKE_SUCCESS (success), other values indicate an error or inverse does not exist.
  */
    unsigned int pke_set_micro_code_start_wait_return_code(unsigned int micro_code);

    /**
 * @brief           Set execution configuration.
 * @param[in]       cfg                  - Specific configuration value.
 * @return          None.
  */
    void pke_set_exe_cfg(unsigned int cfg);

    /**
 * @brief           Compute the modular inverse: ainv = a^(-1) mod modulus.
 * @param[in]       modulus              - Modulus.
 * @param[in]       a                    - Integer a.
 * @param[out]      ainv                 - ainv = a^(-1) mod modulus.
 * @param[in]       mod_wlen             - Word length of modulus and ainv.
 * @param[in]       a_wlen               - Word length of a.
 * @return          PKE_SUCCESS (success), other values indicate an error or inverse does not exist.
 * @note            
 *        1. Please make sure a_wlen <= mod_wlen <= OPERAND_MAX_WORD_LEN and a < modulus.
  */
    unsigned int pke_modinv(const unsigned int *modulus, const unsigned int *a, unsigned int *ainv, unsigned int mod_wlen, unsigned int a_wlen);

    /**
 * @brief           Compute out = (a + b), (a - b), or (a * b) mod modulus.
 * @param[in]       a                    - Integer a.
 * @param[in]       b                    - Integer b.
 * @param[out]      out                  - Result of (a + b), (a - b), or (a * b) mod modulus.
 * @param[in]       micro_code           - Could be MICROCODE_MODADD, MICROCODE_MODSUB, or MICROCODE_MODMUL.
 * @return          PKE_SUCCESS (success), other values indicate an error.
 * @note            
 *        1. Please set hardware operand width to 256u before calling this function.
 *        2. Please make sure the modulus is set in A0 before calling this function.
 *        3. If micro_code is MICROCODE_MODMUL, please make sure the pre-calculated
 *           mont parameter H(R^2 mod modulus) is set in B0, and call micro code
 *           MICROCODE_MGMR_PRE_N0 before calling this function.
 *        4. Actually, micro_code could be MICROCODE_INTADD or MICROCODE_INTSUB, then out = (a + b) or (a - b)
 *           without modulus and pre-calculated mont parameters.
 *        5. All operands are of
 *           256 bits for SM2, SM9, etc.
 */
    unsigned int pke_mod_add_sub_mul_256bits_internal(const unsigned int *a, const unsigned int *b, unsigned int *out, unsigned int micro_code);

    /**
 * @brief           Compute out = (a + b) or (a - b) mod modulus.
 * @param[in]       modulus              - Modulus.
 * @param[in]       a                    - Integer a.
 * @param[in]       b                    - Integer b.
 * @param[out]      out                  - Result of (a + b) or (a - b) mod modulus.
 * @param[in]       micro_code           - Must be MICROCODE_MODADD or MICROCODE_MODSUB.
 * @return          PKE_SUCCESS (success), other values indicate an error.
 * @note            
 *        1. Please set hardware operand width to 256u before calling this function.
 *        2. Please make sure micro_code is MICROCODE_MODADD or MICROCODE_MODSUB.
 *        3. All operands are of 256 bits for SM2, SM9, etc.
  */
    unsigned int pke_modadd_modsub_256bits(const unsigned int *modulus, const unsigned int *a, const unsigned int *b, unsigned int *out, unsigned int micro_code);

    /**
 * @brief           Compute out = (a + b) mod modulus.
 * @param[in]       modulus              - Modulus.
 * @param[in]       a                    - Integer a.
 * @param[in]       b                    - Integer b.
 * @param[out]      out                  - Result of (a + b) mod modulus.
 * @param[in]       wlen                 - Word length of modulus, a, and b.
 * @return          PKE_SUCCESS (success), other values indicate an error.
 * @note            
 *        1. a and b must be less than modulus.
 *        2. wlen must not be bigger than OPERAND_MAX_WORD_LEN.
  */
    unsigned int pke_modadd(const unsigned int *modulus, const unsigned int *a, const unsigned int *b, unsigned int *out, unsigned int wlen);

    /**
 * @brief           Compute out = (a - b) mod modulus.
 * @param[in]       modulus              - Modulus.
 * @param[in]       a                    - Integer a.
 * @param[in]       b                    - Integer b.
 * @param[out]      out                  - Result of (a - b) mod modulus.
 * @param[in]       wlen                 - Word length of modulus, a, and b.
 * @return          PKE_SUCCESS (success), other values indicate an error.
 * @note            
 *        1. a and b must be less than modulus.
 *        2. wlen must not be bigger than OPERAND_MAX_WORD_LEN.
  */
    unsigned int pke_modsub(const unsigned int *modulus, const unsigned int *a, const unsigned int *b, unsigned int *out, unsigned int wlen);

    /**
 * @brief           Compute out = a + b.
 * @param[in]       a                    - Integer a.
 * @param[in]       b                    - Integer b.
 * @param[out]      out                  - Result of a + b.
 * @param[in]       wlen                 - Word length of a, b, and out.
 * @return          PKE_SUCCESS (success), other values indicate an error.
 * @note            
 *        1. a + b may overflow.
 *        2. wlen must not be bigger than OPERAND_MAX_WORD_LEN.
  */
    unsigned int pke_add(const unsigned int *a, const unsigned int *b, unsigned int *out, unsigned int wlen);

    /**
 * @brief           Compute out = a - b.
 * @param[in]       a                    - Integer a.
 * @param[in]       b                    - Integer b.
 * @param[out]      out                  - Result of a - b.
 * @param[in]       wlen                 - Word length of a, b, and out.
 * @return          PKE_SUCCESS (success), other values indicate an error.
 * @note            
 *        1. Please make sure a > b.
 *        2. wlen must not be bigger than OPERAND_MAX_WORD_LEN.
  */
    unsigned int pke_sub(const unsigned int *a, const unsigned int *b, unsigned int *out, unsigned int wlen);

    /**
 * @brief           Compute out = a * b.
 * @param[in]       a                    - Integer a.
 * @param[in]       a_wordLen            - Word length of a.
 * @param[in]       b                    - Integer b.
 * @param[in]       b_wordLen            - Word length of b.
 * @param[out]      out                  - Result of a * b.
 * @param[in]       out_wordLen          - Word length of out.
 * @return          PKE_SUCCESS (success), other values indicate an error.
 * @note            
 *        1. Please make sure the out buffer word length is bigger than ((2 * max_bit_len(a, b) + 0x1F) >> 5).
 *        2. Please make sure a_wordLen and b_wordLen are not bigger than OPERAND_MAX_WORD_LEN / 2.
  */
    unsigned int pke_mul_internal(const unsigned int *a, const unsigned int *b, unsigned int *out, unsigned int a_wordLen, unsigned int b_wordLen, unsigned int out_wordLen);

    /**
 * @brief           Compute out = a * b.
 * @param[in]       a                    - Integer a.
 * @param[in]       b                    - Integer b.
 * @param[out]      out                  - Result of a * b.
 * @param[in]       ab_wordLen           - Word length of a and b.
 * @return          PKE_SUCCESS (success), other values indicate an error.
 * @note            
 *        1. Please make sure the out buffer word length is bigger than ((2 * max_bit_len(a, b) + 0x1F) >> 5).
 *        2. Please make sure ab_wordLen is not bigger than OPERAND_MAX_WORD_LEN / 2.
  */
    unsigned int pke_mul(const unsigned int *a, const unsigned int *b, unsigned int *out, unsigned int ab_wordLen);

#ifndef PKE_CONFIG_ALL_MODEXP_PRE_CALC_WITH_MGMR_MICROCODE
    /**
 * @brief           Calculate H(R^2 mod modulus) and n0' (- modulus^(-1) mod 2^w) for modMul, modExp, pointMul, etc. Here w is the bit width of word, i.e., 32.
 * @param[in]       modulus              - Modulus.
 * @param[in]       bitLen               - Bit length of modulus (must be a multiple of 32).
 * @param[out]      H                    - R^2 mod modulus.
 * @param[out]      n0                   - Modulus^(-1) mod 2^w, where w is 32.
 * @return          PKE_SUCCESS (success), other values indicate an error.
 * @note            
 *        1. Modulus must be odd.
 *        2. Please make sure the word length of buffer H is equal to the word length of modulus, and n0 only needs one word.
 *        3. bitLen must not be bigger than OPERAND_MAX_BIT_LEN.
 */
    unsigned int pke_pre_calc_mont_without_mgmr_microcode(const unsigned int *modulus, unsigned int bitLen, unsigned int *H, unsigned int *n0);
#endif
    /**
 * @brief           Calculate H(R^2 mod modulus) and n0' (- modulus^(-1) mod 2^w) for modMul, modExp, pointMul, etc. Here w is the bit width of word, i.e., 32.
 * @param[in]       modulus              - Modulus.
 * @param[in]       bitLen               - Bit length of modulus.
 * @param[out]      H                    - R^2 mod modulus.
 * @param[out]      n0                   - - modulus^(-1) mod 2^w, where w is 32.
 * @return          PKE_SUCCESS (success), other values indicate an error.
 * @note            
 *        1. Modulus must be odd.
 *        2. Please make sure the word length of buffer H is equal to the word length of modulus, and n0 only needs one word.
 *        3. bitLen must not be bigger than OPERAND_MAX_BIT_LEN.
 */
    unsigned int pke_pre_calc_mont(const unsigned int *modulus, unsigned int bitLen, unsigned int *H, unsigned int *n0);

    /**
 * @brief           Like function pke_pre_calc_mont(), but this one is without output.
 * @param[in]       modulus              - Modulus.
 * @param[in]       wlen                 - Word length of modulus.
 * @return          PKE_SUCCESS (success), other values indicate an error.
 * @note            
 *        1. Modulus must be odd.
 *        2. wlen must not be bigger than OPERAND_MAX_WORD_LEN.
  */
    unsigned int pke_pre_calc_mont_no_output(const unsigned int *modulus, unsigned int wlen);

    /**
 * @brief           Like function pke_pre_calc_mont(), but this one is for modexp.
 * @param[in]       modulus              - Modulus.
 * @param[in]       bitLen               - Bit length of modulus.
 * @param[out]      H                    - R^2 mod modulus.
 * @param[out]      n0                   - - modulus^(-1) mod 2^w, where w is 32.
 * @return          PKE_SUCCESS (success), other values indicate an error.
 * @note            
 *        1. Modulus must be odd.
 *        2. Please make sure the word length of buffer H is equal to the word length of modulus, and n0 only needs one word.
 *        3. bitLen must not be bigger than OPERAND_MAX_BIT_LEN.
 */
    unsigned int pke_pre_calc_mont_for_modexp(const unsigned int *modulus, unsigned int bitLen, unsigned int *H, unsigned int *n0);

    /**
 * @brief           Load modulus and pre-calculated Mont parameters H(R^2 mod modulus) and n0'(- modulus^(-1) mod 2^w) for hardware operation.
 * @param[in]       modulus              - Modulus.
 * @param[in]       modulus_h            - R^2 mod modulus.
 * @param[in]       modulus_n0           - - modulus^(-1) mod 2^w, where w is 32.
 * @param[in]       bitLen               - Bit length of modulus.
 * @return          PKE_SUCCESS (success), other values indicate an error.
 * @note            
 *        1. Modulus must be odd.
 *        2. bitLen must not be bigger than OPERAND_MAX_BIT_LEN.
  */
    unsigned int pke_load_modulus_and_pre_monts(const unsigned int *modulus, const unsigned int *modulus_h, const unsigned int *modulus_n0, const unsigned int bitLen);

    /**
 * @brief           Load modulus and pre-calculated Mont parameters H(R^2 mod modulus) of 256 bits and n0'(- modulus^(-1) mod 2^w) for hardware operation.
 * @param[in]       modulus              - Modulus.
 * @param[in]       modulus_h            - R^2 mod modulus.
 * @param[in]       modulus_n0           - - modulus^(-1) mod 2^w, where w is 32.
 * @return          PKE_SUCCESS (success), other values indicate an error.
 * @note            
 *        1. Modulus must be odd.
 *        2. All operands are of 256 bits for SM2, SM9, etc.
 */
    unsigned int pke_load_modulus_and_pre_monts_256bits(const unsigned int *modulus, const unsigned int *modulus_h, const unsigned int *modulus_n0);

    /**
 * @brief           Set modulus and pre-calculated Mont parameters H(R^2 mod modulus) and n0'(- modulus^(-1) mod 2^w) for hardware operation.
 * @param[in]       modulus              - Modulus.
 * @param[in]       modulus_h            - R^2 mod modulus.
 * @param[in]       modulus_n0           - - modulus^(-1) mod 2^w, where w is 32.
 * @param[in]       bitLen               - Bit length of modulus.
 * @return          PKE_SUCCESS (success), other values indicate an error.
 * @note            
 *        1. Modulus must be odd.
 *        2. bitLen must not be bigger than OPERAND_MAX_BIT_LEN.
  */
    unsigned int pke_set_modulus_and_pre_monts(const unsigned int *modulus, const unsigned int *modulus_h, const unsigned int *modulus_n0, unsigned int bitLen);

    /**
 * @brief           Perform out = a * b (mod modulus).
 * @param[in]       a                    - Integer a.
 * @param[in]       b                    - Integer b.
 * @param[out]      out                  - Result of a * b mod modulus.
 * @param[in]       wlen                 - Word length of modulus, a, and b.
 * @return          PKE_SUCCESS (success), other values indicate an error.
 * @note            
 *        1. Modulus must be odd.
 *        2. a, b must be less than modulus.
 *        3. wlen must not be bigger than OPERAND_MAX_WORD_LEN.
 *        4. Before calling this function, please make sure the modulus and the pre-calculated Mont arguments of modulus are located in the right address.
  */
    unsigned int pke_modmul_internal(const unsigned int *a, const unsigned int *b, unsigned int *out, unsigned int wlen);

    /**
 * @brief           Perform out = a * b (mod modulus).
 * @param[in]       modulus              - Modulus.
 * @param[in]       a                    - Integer a.
 * @param[in]       b                    - Integer b.
 * @param[out]      out                  - Result of a * b mod modulus.
 * @param[in]       wlen                 - Word length of modulus, a, and b.
 * @return          PKE_SUCCESS (success), other values indicate an error.
 * @note            
 *        1. Modulus must be odd.
 *        2. a, b must be less than modulus.
 *        3. wlen must not be bigger than OPERAND_MAX_WORD_LEN.
  */
    unsigned int pke_modmul(const unsigned int *modulus, const unsigned int *a, const unsigned int *b, unsigned int *out, unsigned int wlen);

    /**
 * @brief           Mod exponentiation, this could be used for RSA encrypting, decrypting, signing, and verifying.
 * @param[in]       exponent             - exponent
 * @param[in]       base                 - base number
 * @param[out]      out                  - result of base^(exponent) mod modulus
 * @param[in]       mod_wordLen          - word length of modulus and base number
 * @param[in]       exp_wordLen          - word length of exponent
 * @return          PKE_SUCCESS (success), other values indicate an error
 * @note            
 *        1. Please set hardware operand width before calling this function.
 *        2. Before calling this function, please make sure the modulus and the pre-calculated Mont arguments of modulus are located in the right address.
 *        3. Modulus must be odd.
 *        4. Please make sure exp_wordLen <= mod_wordLen <= OPERAND_MAX_WORD_LEN.
 */
    unsigned int pke_modexp_internal(const unsigned int *exponent, const unsigned int *base, unsigned int *out, unsigned int mod_wordLen, unsigned int exp_wordLen);

    /**
 * @brief           Mod exponentiation, this could be used for RSA encrypting, decrypting, signing, and verifying.
 * @param[in]        modulus              - modulus
 * @param[in]        exponent             - exponent
 * @param[in]        base                 - base number
 * @param[out]       out                  - result of base^(exponent) mod modulus
 * @param[in]        mod_wordLen          - word length of modulus and base number
 * @param[in]        exp_wordLen          - word length of exponent
 * @return          PKE_SUCCESS (success), other values indicate an error
 * @note            
 *        1. Before calling this function, please make sure the pre-calculated Mont arguments of modulus are located in the right address.
 *        2. Modulus must be odd.
 *        3. Please make sure exp_wordLen <= mod_wordLen <= OPERAND_MAX_WORD_LEN.
*/
    unsigned int pke_modexp(const unsigned int *modulus, const unsigned int *exponent, const unsigned int *base, unsigned int *out, unsigned int mod_wordLen,
                            unsigned int exp_wordLen);

    /**
 * @brief           Check input before mod exponentiation.
 * @param[in]        modulus              - modulus
 * @param[in]        exponent             - exponent
 * @param[in]        base                 - base number
 * @param[out]       out                  - result of base^(exponent) mod modulus
 * @param[in]        mod_wordLen          - word length of modulus and base number
 * @param[in]        exp_wordLen          - word length of exponent
 * @return          PKE_SUCCESS (input is valid, allow to calculate)
 *        PKE_FINISHED (mod exponent finished)
 *        other values indicate an error
 * @note            
 *        1. Modulus must be odd.
 *        2. Please make sure exp_wordLen <= mod_wordLen <= OPERAND_MAX_WORD_LEN.
*/
    unsigned int pke_modexp_check_input(const unsigned int *modulus, const unsigned int *exponent, const unsigned int *base, unsigned int *out, unsigned int mod_wordLen,
                                        unsigned int exp_wordLen);

    /**
 * @brief           Mod exponentiation (for high-level use, operands are all unsigned char big-endian big numbers). This could be used for RSA encrypting, decrypting,
 *                  signing, and verifying.
 * @param[in]        modulus              - modulus (unsigned char big-endian big number)
 * @param[in]        exponent             - exponent (unsigned char big-endian big number)
 * @param[in]        base                 - base number (unsigned char big-endian big number)
 * @param[out]       out                  - result of base^(exponent) mod modulus (unsigned char big-endian big number)
 * @param[in]        mod_bitLen           - real bit length of modulus and base number
 * @param[in]        exp_bitLen           - real bit length of exponent
 * @param[in]        calc_pre_monts       - if it is 0, no need to calculate the pre-calculated Mont arguments of modulus; otherwise, calculate.
 * @return          PKE_SUCCESS (success), other values indicate an error
 * @note            
 *        1. This is for high-level application or protocol to use RSA mod exponent directly. 
 *        2. All operands of this API are unsigned char big-endian big numbers.
 *        3. Modulus must be odd.
 *        4. Please make sure exp_bitLen <= mod_bitLen <= OPERAND_MAX_BIT_LEN.
*/
    unsigned int pke_modexp_u8(const unsigned char *modulus, const unsigned char *exponent, const unsigned char *base, unsigned char *out, unsigned int mod_bitLen,
                               unsigned int exp_bitLen, unsigned int calc_pre_monts);

    /**
 * @brief           Compute c = a mod b.
 * @param[in]        a                    - integer a (unsigned char big-endian big number)
 * @param[in]        a_wlen               - word length of integer a
 * @param[in]        b                    - integer b, modulus (unsigned char big-endian big number)
 * @param[in]        b_h                  - H parameter of b (unsigned char big-endian big number)
 * @param[in]        b_n0                 - - modulus ^(-1) mod 2^w, here w is 32 actually
 * @param[in]        b_wlen               - word length of integer b and b_h
 * @param[out]       c                    - result of a mod b (unsigned char big-endian big number)
 * @return          PKE_SUCCESS (success), other values indicate an error
 * @note            
 *        1. b must be odd, and please make sure b_wlen is the real word length of b.
 *        2. Real bit length of a cannot be bigger than 2*(real bit length of b), so a_wlen cannot be bigger than 2*b_wlen.
 *        3. Please make sure a_wlen <= 2*OPERAND_MAX_WORD_LEN and b_wlen <= OPERAND_MAX_WORD_LEN.
*/
    unsigned int pke_mod(const unsigned int *a, unsigned int a_wlen, const unsigned int *b, const unsigned int *b_h, const unsigned int *b_n0, unsigned int b_wlen,
                         unsigned int *c);

    /**
 * @brief           ECCP curve point multiplication (random point), Q = [k]P.
 * @param[in]        curve                - pointer to eccp_curve_t curve struct
 * @param[in]        k                    - scalar
 * @param[in]        px                   - x coordinate of point P (unsigned char big-endian big number)
 * @param[in]        Py                   - y coordinate of point P (unsigned char big-endian big number)
 * @param[out]       qx                   - x coordinate of point Q (unsigned char big-endian big number)
 * @param[out]       qy                   - y coordinate of point Q (unsigned char big-endian big number)
 * @return          PKE_SUCCESS (success), other values indicate an error
 * @note            
 *        1. Please make sure k is in [1, n-1], where n is the order of the ECCP curve.
 *        2. Please make sure the input point P is on the curve.
 *        3. Please make sure the bit length of the curve is not bigger than ECCP_MAX_BIT_LEN.
 *        4. Even if the input point P is valid, the output may be the infinite point, in which case it will return an error.
*/
    unsigned int eccp_pointmul(const eccp_curve_t *curve, const unsigned int *k, const unsigned int *px, const unsigned int *Py, unsigned int *qx, unsigned int *qy);

    /**
 * @brief           ECCP curve point addition, Q = P1 + P2.
 * @param[in]        curve                - pointer to eccp_curve_t curve struct
 * @param[in]        x1                   - x coordinate of point P1 (unsigned char big-endian big number)
 * @param[in]        y1                   - y coordinate of point P1 (unsigned char big-endian big number)
 * @param[in]        x2                   - x coordinate of point P2 (unsigned char big-endian big number)
 * @param[in]        y2                   - y coordinate of point P2 (unsigned char big-endian big number)
 * @param[out]       qx                   - x coordinate of point Q = P1 + P2 (unsigned char big-endian big number)
 * @param[out]       qy                   - y coordinate of point Q = P1 + P2 (unsigned char big-endian big number)
 * @return          PKE_SUCCESS (success), other values indicate an error
 * @note            
 *        1. Please make sure input points P1 and P2 are both on the curve.
 *        2. Please make sure the bit length of the curve is not greater than ECCP_MAX_BIT_LEN.
 *        3. Even if the input points P1 and P2 are valid, it will return an error in the following cases:
 *        3.1. P1 = P2. Return PKE_NO_MODINV.
 *        3.2. P1 = -P2. Return PKE_NO_MODINV. Actually, the output point is the neutral point (point at infinity).
*/
    unsigned int eccp_pointadd(const eccp_curve_t *curve, const unsigned int *x1, const unsigned int *y1, const unsigned int *x2, const unsigned int *y2, unsigned int *qx,
                               unsigned int *qy);

    /**
 * @brief           ECCP curve point addition, Q = P1 + P2.
 * @param[in]        curve                - pointer to eccp_curve_t curve struct
 * @param[in]        x1                   - x coordinate of point P1 (unsigned char big-endian big number)
 * @param[in]        y1                   - y coordinate of point P1 (unsigned char big-endian big number)
 * @param[in]        x2                   - x coordinate of point P2 (unsigned char big-endian big number)
 * @param[in]        y2                   - y coordinate of point P2 (unsigned char big-endian big number)
 * @param[out]       qx                   - x coordinate of point Q = P1 + P2 (unsigned char big-endian big number)
 * @param[out]       qy                   - y coordinate of point Q = P1 + P2 (unsigned char big-endian big number)
 * @return          PKE_SUCCESS (success), other values indicate an error
 * @note            
 *        1. Please make sure input points P1 and P2 are both on the curve.
 *        2. Please make sure the bit length of the curve is not greater than ECCP_MAX_BIT_LEN.
 *        3. If P1 = -P2, it will return PKE_NO_MODINV. Actually, the output point is the neutral point (point at infinity).
*/
    unsigned int eccp_pointadd_safe(const eccp_curve_t *curve, const unsigned int *x1, const unsigned int *y1, const unsigned int *x2, const unsigned int *y2, unsigned int *qx,
                                    unsigned int *qy);

// #define ECCP_POINT_DOUBLE   //not recommended to define
#ifdef ECCP_POINT_DOUBLE
    /**
 * @brief           ECCP curve point double, Q=[2]P
 * @param[in]       curve                - eccp_curve_t curve struct pointer
 * @param[in]       px                   - x coordinate of point P
 * @param[in]       Py                   - y coordinate of point P
 * @param[out]      qx                   - x coordinate of point Q=[2]P
 * @param[out]      qy                   - y coordinate of point Q=[2]P
 * @return          PKE_SUCCESS (success), other values indicate an error
 * @note            
 *        1. please make sure the input point P is on the curve
 *        2. please make sure bit length of the curve is not bigger than ECCP_MAX_BIT_LEN
 */
    unsigned int eccp_pointDouble(eccp_curve_t *curve, unsigned int *px, unsigned int *Py, unsigned int *qx, unsigned int *qy);
#endif
    /**
 * @brief           Check whether the input point P is on the ECCP curve or not.
 * @param[in]        curve                - pointer to eccp_curve_t curve struct
 * @param[in]        px                   - x coordinate of point P (unsigned char big-endian big number)
 * @param[in]        Py                   - y coordinate of point P (unsigned char big-endian big number)
 * @return          PKE_SUCCESS (success, point is on the curve), other values indicate an error or not on the curve
 * @note            
 *        1. Please make sure the bit length of the curve is not bigger than ECCP_MAX_BIT_LEN.
 *        2. After calculation, A1 and A2 will be changed!
*/
    unsigned int eccp_pointverify(const eccp_curve_t *curve, const unsigned int *px, const unsigned int *Py);

    /**
 * @brief           Get ECCP public key from private key (the key pair could be used in SM2/ECDSA/ECDH, etc.).
 * @param[in]        curve                - pointer to eccp_curve_t curve struct
 * @param[in]        priKey               - private key (unsigned char big-endian)
 * @param[out]       pubKey               - public key (unsigned char big-endian)
 * @return          PKE_SUCCESS (success), other values indicate an error
 * @note            
 *        1. Please make sure the bit length of the curve is not bigger than ECCP_MAX_BIT_LEN.
*/
    unsigned int eccp_get_pubkey_from_prikey(const eccp_curve_t *curve, const unsigned char *priKey, unsigned char *pubKey);

    /**
 * @brief           Get ECCP key pair (the key pair could be used in SM2/ECDSA/ECDH).
 * @param[in]        curve                - pointer to eccp_curve_t curve struct
 * @param[out]       priKey               - private key (unsigned char big-endian)
 * @param[out]       pubKey               - public key (unsigned char big-endian)
 * @return          PKE_SUCCESS (success), other values indicate an error
 * @note            
 *        1. Please make sure the bit length of the curve is not bigger than ECCP_MAX_BIT_LEN.
*/
    unsigned int eccp_getkey(const eccp_curve_t *curve, unsigned char *priKey, unsigned char *pubKey);

#ifdef SUPPORT_C25519
    /**
 * @brief           C25519 point multiplication (random point), Q = [k]P.
 * @param[in]       curve                - pointer to c25519 curve struct
 * @param[in]        k                    - scalar
 * @param[in]        p                    - u coordinate of point P (unsigned char big-endian)
 * @param[out]       q                    - u coordinate of point Q (unsigned char big-endian)
 * @return          PKE_SUCCESS (success), other values indicate an error
 * @note            
 *        1. Please make sure the input point P is on the curve.
 *        2. Even if the input point P is valid, the output may be the infinite point, in which case it will return an error.
 *        3. Please make sure the curve is C25519.
*/
    unsigned int x25519_pointmul(const mont_curve_t *curve, const unsigned int *k, const unsigned int *p, unsigned int *q);

    /**
 * @brief           Decode X25519 or Ed25519 scalar for point multiplication.
 * @param[in]        k                    - scalar (unsigned char big-endian)
 * @param[out]       out                  - big scalar in little-endian (unsigned char little-endian)
 * @return          None
 * @note            
 *        1. This function is for X25519 or Ed25519.
*/
    void x25519_ed25519_decode_scalar(const unsigned char *k, unsigned char *out);

    /**
 * @brief           Decode an Ed25519 encoded point.
 * @param[in]        in_y                 - encoded Ed25519 point (unsigned char big-endian)
 * @param[out]       out_x                - x coordinate of the input point (unsigned char big-endian)
 * @param[out]       out_y                - y coordinate of the input point (unsigned char big-endian)
 * @return          PKE_SUCCESS (success), other values indicate an error
*/
    unsigned int ed25519_decode_point(const unsigned char in_y[32], unsigned char out_x[32], unsigned char out_y[32]);

    /**
 * @brief           Edwards25519 curve point multiplication (random point), Q = [k]P.
 * @param[in]        curve                - pointer to edwards25519 curve struct
 * @param[in]        k                    - scalar
 * @param[in]        px                   - x coordinate of point P (unsigned char big-endian)
 * @param[in]        Py                   - y coordinate of point P (unsigned char big-endian)
 * @param[out]       qx                   - x coordinate of point Q (unsigned char big-endian)
 * @param[out]       qy                   - y coordinate of point Q (unsigned char big-endian)
 * @return          PKE_SUCCESS (success), other values indicate an error
 * @note            
 *        1. Please make sure the input point P is on the curve.
 *        2. Even if the input point P is valid, the output may be the neutral point (0, 1), which is valid.
 *        3. Please make sure the curve is Edwards25519.
 *        4. k could not be zero.
 *        5. Please set hardware operand width to 256u before calling this function.
 *        6. Before calling this function, please ensure that the modulus and the
 *           pre-calculated montgomery arguments of the modulus are located in the correct addresses.
*/
    unsigned int ed25519_pointMul_internal(const edward_curve_t *curve, const unsigned int *k, const unsigned int *px, const unsigned int *Py, unsigned int *qx, unsigned int *qy);

    /**
 * @brief           Edwards25519 curve point multiplication (random point), Q = [k]P.
 * @param[in]        curve                - pointer to edwards25519 curve struct
 * @param[in]        k                    - scalar
 * @param[in]        px                   - x coordinate of point P (unsigned char big-endian)
 * @param[in]        Py                   - y coordinate of point P (unsigned char big-endian)
 * @param[out]       qx                   - x coordinate of point Q (unsigned char big-endian)
 * @param[out]       qy                   - y coordinate of point Q (unsigned char big-endian)
 * @return          PKE_SUCCESS (success), other values indicate an error
 * @note            
 *        1. Please make sure the input point P is on the curve.
 *        2. Even if the input point P is valid, the output may be the neutral point(0, 1), which is valid.
 *        3. Please make sure the curve is Edwards25519.
 *        4. k could not be zero.
*/
    unsigned int ed25519_pointMul(const edward_curve_t *curve, const unsigned int *k, const unsigned int *px, const unsigned int *Py, unsigned int *qx, unsigned int *qy);

    /**
 * @brief           Edwards25519 point addition, Q = P1 + P2.
 * @param[in]        curve                - pointer to edwards25519 curve struct
 * @param[in]        x1                   - x coordinate of point P1 (unsigned char big-endian)
 * @param[in]        y1                   - y coordinate of point P1 (unsigned char big-endian)
 * @param[in]        x2                   - x coordinate of point P2 (unsigned char big-endian)
 * @param[in]        y2                   - y coordinate of point P2 (unsigned char big-endian)
 * @param[out]       qx                   - x coordinate of point Q = P1 + P2 (unsigned char big-endian)
 * @param[out]       qy                   - y coordinate of point Q = P1 + P2 (unsigned char big-endian)
 * @return          PKE_SUCCESS (success), other values indicate an error
 * @note            
 *        1. Please make sure the input points P1 and P2 are both on the curve.
 *        2. The output point may be the neutral point (0, 1), which is valid.
 *        3. Please make sure the curve is Edwards25519.
*/
    unsigned int ed25519_pointAdd(const edward_curve_t *curve, const unsigned int *x1, const unsigned int *y1, const unsigned int *x2, const unsigned int *y2, unsigned int *qx,
                                  unsigned int *qy);

#endif

#ifdef __cplusplus
}
#endif

#endif
