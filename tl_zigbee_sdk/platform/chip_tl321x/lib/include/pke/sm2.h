/********************************************************************************************************
 * @file    sm2.h
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
#ifndef SM2_H
#define SM2_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lib/include/pke/pke.h"
#include "lib/include/hash/hash.h"


#if (defined(PKE_HP) || defined(PKE_UHP))
    #define SM2_HIGH_SPEED //only available for PKE_HP, PKE_UHP
#endif


//some sm2 length
#define SM3_DIGEST_BYTE_LEN SM2_BYTE_LEN
#define SM2_MAX_ID_BYTE_LEN (0x1FFFU) //((1U<<13)-1U)


//SM2 return code
#define SM2_SUCCESS               PKE_SUCCESS
#define SM2_BUFFER_NULL           (PKE_SUCCESS + 0x40U)
#define SM2_NOT_ON_CURVE          (PKE_SUCCESS + 0x41U)
#define SM2_EXCHANGE_ROLE_INVALID (PKE_SUCCESS + 0x42U)
#define SM2_INPUT_INVALID         (PKE_SUCCESS + 0x43U)
#define SM2_ZERO_ALL              (PKE_SUCCESS + 0x44U)
#define SM2_INTEGER_TOO_BIG       (PKE_SUCCESS + 0x45U)
#define SM2_VERIFY_FAILED         (PKE_SUCCESS + 0x46U)
#define SM2_DECRYPT_VERIFY_FAILED (PKE_SUCCESS + 0x47U)

    //SM2 key exchange role
    typedef enum
    {
        SM2_Role_Sponsor = 0,
        SM2_Role_Responsor
    } sm2_exchange_role_e;

    // SM2 ciphertext order
    typedef enum
    {
        SM2_C1C3C2 = 0,
        SM2_C1C2C3,
    } sm2_cipher_order_e;

    //APIs

    /**
 * @brief       get SM2 Z value = SM3(bitLenofID||ID||a||b||Gx||Gy||Px||Py).
 * @param[in]   ID           - User ID
 * @param[in]   byteLenofID  - byte length of ID, must be less than 2^13
 * @param[in]   pubKey       - public key(0x04 + x + y), 65 bytes, big-endian
 * @param[out]  Z            - Z value, SM3 digest, 32 bytes.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.bit length of ID must be less than 2^16, thus byte length must be less than 2^13
      -# 2.if ID is NULL, then replace it with sm2 default ID
      -# 3.please make sure the pubKey is valid
  @endverbatim
 */
    unsigned int sm2_getZ(unsigned char *ID, unsigned int byteLenofID, unsigned char pubKey[65], unsigned char Z[32]);

    /**
 * @brief       get SM2 E value = SM3(Z||M) (one-off style)
 * @param[in]   M             - Message
 * @param[in]   byteLen       - byte length of M
 * @param[in]   Z             - Z value, 32 bytes
 * @param[out]  E             - E value, 32 bytes
 * @return      0:success     other:error
 */
    unsigned int sm2_getE(unsigned char *M, unsigned int byteLen, unsigned char Z[32], unsigned char E[32]);

//#define SM2_GETE_BY_STEPS
#ifdef SM2_GETE_BY_STEPS

    /**
 * @brief       step 1 of getting SM2 E value(stepwise style), init
 * @param[in]   ctx     - input, HASH_CTX context pointer
 * @param[in]   Z       - input, Z value, 32 bytes
 * @return      SM2_SUCCESS(success), other(error)
 */
    unsigned int sm2_getE_init(HASH_CTX *ctx, unsigned char Z[32]);

    /**
 * @brief       step 2 of getting SM2 E value(stepwise style), update message
 * @param[in]   ctx           - input, HASH_CTX context pointer
 * @param[in]   msg           - input, message
 * @param[in]   msg_bytes     - input, byte length of the input message
 * @return      SM2_SUCCESS(success), other(error)
 *  @note
  @verbatim
      -# 1. please make sure the three parameters are valid, and ctx is initialized.
  @endverbatim
 */
    unsigned int sm2_getE_update(HASH_CTX *ctx, unsigned char *msg, unsigned int msg_bytes);

    /**
 * @brief       step 3 of getting SM2 E value(stepwise style), message update done, get the digest(SM2 E value)
 * @param[in]   ctx           - input, HASH_CTX context pointer
 * @param[in]   E             - input, message
 * @return      SM2_SUCCESS(success), other(error)
 *  @note
  @verbatim
      -# 1. please make sure the ctx is valid and initialized.
      -# 2. please make sure the digest buffer E is sufficient.
  @endverbatim
 */
    unsigned int sm2_getE_final(HASH_CTX *ctx, unsigned char E[32]);
#endif

    /**
 * @brief       Generate SM2 public key from private key
 * @param[in]   priKey           - private key, 32 bytes, big-endian
 * @param[out]  pubKey           - public key(0x04 + x + y), 65 bytes, big-endian
 * @return      0:success     other:error
 */
    unsigned int sm2_get_pubkey_from_prikey(unsigned char priKey[32], unsigned char pubKey[65]);

    /**
 * @brief       Generate SM2 random Key pair
 * @param[in]   priKey           - private key, 32 bytes, big-endian
 * @param[out]  pubKey           - public key(0x04 + x + y), 65 bytes, big-endian
 * @return      0:success     other:error
 */
    unsigned int sm2_getkey(unsigned char priKey[32], unsigned char pubKey[65]);

    /**
 * @brief       Generate SM2 Signature
 * @param[in]   E          - input, E value, 32 bytes, big-endian
 * @param[in]   rand_k     - input, random big integer k in signing, 32 bytes, big-endian,
 *                               if you do not have this integer, please set this parameter to be NULL,
 *                               it will be generated inside.
 * @param[in]   priKey     - private key, 32 bytes, big-endian
 * @param[out]  signature  - Signature r and s, 64 bytes, big-endian
 * @return      SM2_SUCCESS(success)     other:error
 * @note
  @verbatim
      -# 1.if you do not have rand_k, please set the parameter to be NULL, it will be generated inside.
  @endverbatim
 */
    unsigned int sm2_sign(unsigned char E[32], unsigned char rand_k[32], unsigned char priKey[32], unsigned char signature[64]);

    /**
 * @brief       Verify SM2 Signature
 * @param[in]   E          - E value, 32 bytes, big-endian
 * @param[in]   pubKey     - public key(0x04 + x + y), 65 bytes, big-endian
 * @param[in]   signature  - Signature r and s, 64 bytes, big-endian
 * @return      SM2_SUCCESS(success)     other:error
 */
    unsigned int sm2_verify(unsigned char E[32], unsigned char pubKey[65], unsigned char signature[64]);

    /**
 * @brief       SM2 Encryption
 * @param[in]   M              - plaintext, MByteLen bytes, big-endian
 * @param[in]   MByteLen       - byte length of M
 * @param[in]   rand_k     - input, random big integer k in encrypting, 32 bytes, big-endian,
 *                               if you do not have this integer, please set this parameter to be NULL,
 *                               it will be generated inside.
 * @param[in]   pubKey     - public key, 65 bytes, big-endian
 * @param[in]   order          - either SM2_C1C3C2 or SM2_C1C2C3
 * @param[out]   C              - ciphertext, CByteLen bytes, big-endian
 * @param[out]  CByteLen       - byte length of C, should be MByteLen+97 if success
 * @return      SM2_SUCCESS(success)     other:error
 * @note
  @verbatim
      -# 1.M and C can be the same buffer.
      -# 2.if you do not have rand_k, please set the parameter to be NULL, it will be generated inside.
      -# 3.please make sure pubKey is valid
  @endverbatim
 */
    unsigned int sm2_encrypt(unsigned char *M, unsigned int MByteLen, unsigned char rand_k[32], unsigned char pubKey[65], sm2_cipher_order_e order, unsigned char *C, unsigned int *CByteLen);

    /**
 * @brief       SM2 Decryption
 * @param[in]   C             - ciphertext, CByteLen bytes, big-endian
 * @param[in]   CByteLen      - byte length of C, make sure MByteLen>97
 * @param[in]   priKey        - private key, 32 bytes, big-endian
 * @param[in]   order          - either SM2_C1C3C2 or SM2_C1C2C3
 * @param[out]  M             - plaintext, MByteLen bytes, big-endian
 * @param[out]  MByteLen      - byte length of M, should be CByteLen-97 if success
 * @return      SM2_SUCCESS(success)     other:error
 * @note
  @verbatim
      -# 1.M and C can be the same buffer.
  @endverbatim
 */
    unsigned int sm2_decrypt(unsigned char *C, unsigned int CByteLen, unsigned char priKey[32], sm2_cipher_order_e order, unsigned char *M, unsigned int *MByteLen);

    /**
 * @brief       SM2 Key Exchange
 * @param[in]   role              - SM2_Role_Sponsor - sponsor, SM2_Role_Responsor - responsor
 * @param[in]   dA            - local's permanent private key7
 * @param[in]   PB            - peer's permanent public key
 * @param[in]   rA            - local's temporary private key
 * @param[in]   RA            - local's temporary public key
 * @param[in]   RB            - peer's temporary public key
 * @param[in]   ZA            - local's Z value
 * @param[in]   ZB            - peer's Z value
 * @param[in]   kByteLen      - byte length of output key, should be less than (2^32 - 1)bit
 * @param[out]  KA            - output key
 * @param[out]  S1            - sponsor's S1, or responsor's S2, this is optional
 * @param[out]  SA            - sponsor's SA, or responsor's SB, this is optional
 * @return      SM2_SUCCESS(success)     other:error
 * @note
  @verbatim
      -# 1.please make sure the inputs are valid
      -# 2.S1 and SA are optional, if you don't need, please set S1 and SA as NULL
      -# 3.in case that S1(S2) and SA(SB) exist, if S1=SB,S2=SA, then exchange success.
  @endverbatim
 */
    unsigned int sm2_exchangekey(sm2_exchange_role_e role,
                                 unsigned char      *dA,
                                 unsigned char      *PB,
                                 unsigned char      *rA,
                                 unsigned char      *RA,
                                 unsigned char      *RB,
                                 unsigned char      *ZA,
                                 unsigned char      *ZB,
                                 unsigned int        kByteLen,
                                 unsigned char      *KA,
                                 unsigned char      *S1,
                                 unsigned char      *SA);


#ifdef SM2_SEC

    //SM2 return code(secure version)
    #define SM2_SUCCESS_S (0x3E2FDB1AU)
    #define SM2_ERROR_S   (0xCBAD735EU)


    unsigned int sm2_sign_s(unsigned char E[32], unsigned char rand_k[32], unsigned char priKey[32], unsigned char signature[64]);

    unsigned int sm2_verify_s(unsigned char E[32], unsigned char pubKey[65], unsigned char signature[64]);

    unsigned int sm2_encrypt_s(unsigned char *M, unsigned int MByteLen, unsigned char rand_k[32], unsigned char pubKey[65], sm2_cipher_order_e order, unsigned char *C, unsigned int *CByteLen);

    unsigned int sm2_decrypt_s(unsigned char *C, unsigned int CByteLen, unsigned char priKey[32], sm2_cipher_order_e order, unsigned char *M, unsigned int *MByteLen);

    unsigned int sm2_exchangekey_s(sm2_exchange_role_e role,
                                   unsigned char      *dA,
                                   unsigned char      *PB,
                                   unsigned char      *rA,
                                   unsigned char      *RA,
                                   unsigned char      *RB,
                                   unsigned char      *ZA,
                                   unsigned char      *ZB,
                                   unsigned int        kByteLen,
                                   unsigned char      *KA,
                                   unsigned char      *S1,
                                   unsigned char      *SA);

#endif


#ifdef __cplusplus
}
#endif

#endif
