/********************************************************************************************************
 * @file    sm2.h
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
#ifndef SM2_H
#define SM2_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lib/include/pke/pke.h"
#include "lib/include/hash/hash.h"


#if (defined(PKE_HP) || defined(PKE_UHP))
#define SM2_HIGH_SPEED // only available for PKE_HP, PKE_UHP
#endif

/**
 * @brief           some sm2 length
 */
#define SM3_DIGEST_BYTE_LEN SM2_BYTE_LEN
#define SM2_MAX_ID_BYTE_LEN (0x1FFFU) // this is from ((1U<<13)-1U)

/**
 * @brief           SM2 return code
 */
#define SM2_SUCCESS               PKE_SUCCESS
#define SM2_BUFFER_NULL           (PKE_SUCCESS + 0x40U)
#define SM2_NOT_ON_CURVE          (PKE_SUCCESS + 0x41U)
#define SM2_EXCHANGE_ROLE_INVALID (PKE_SUCCESS + 0x42U)
#define SM2_INPUT_INVALID         (PKE_SUCCESS + 0x43U)
#define SM2_ZERO_ALL              (PKE_SUCCESS + 0x44U)
#define SM2_INTEGER_TOO_BIG       (PKE_SUCCESS + 0x45U)
#define SM2_VERIFY_FAILED         (PKE_SUCCESS + 0x46U)
#define SM2_DECRYPT_VERIFY_FAILED (PKE_SUCCESS + 0x47U)

    /**
 * @brief           SM2 key exchange role
 */
    typedef enum
    {
        SM2_Role_Sponsor = 0,
        SM2_Role_Responsor
    } sm2_exchange_role_e;

    /**
 * @brief           SM2 ciphertext order
 */
    typedef enum
    {
        SM2_C1C3C2 = 0,
        SM2_C1C2C3,
    } sm2_cipher_order_e;

    /**
 * @brief           APIs
 */

    /**
 * @brief           get SM2 Z value = SM3(bitLenofID||ID||a||b||Gx||Gy||px||Py).
 * @param[in]       ID                   - User ID
 * @param[in]       byteLenofID          - byte length of ID, must be less than 2^13
 * @param[in]       pubKey               - public key(0x04 + x + y), 65 bytes, big-endian
 * @param[out]      Z                    - Z value, SM3 digest, 32 bytes.
 * @return          0:success     other:error
 * @note            
 *        1.bit length of ID must be less than 2^16, thus byte length must be less than 2^13
 *        2.if ID is NULL, then replace it with sm2 default ID
 *        3.please make sure the pubKey is valid
 */
    unsigned int sm2_getZ(const unsigned char *ID, unsigned int byteLenofID, const unsigned char pubKey[65], unsigned char Z[32]);

    /**
 * @brief           get SM2 E value = SM3(Z||M) (one-off style)
 * @param[in]       M                    - Message
 * @param[in]       byteLen              - byte length of M
 * @param[in]       Z                    - Z value, 32 bytes
 * @param[out]      E                    - E value, 32 bytes
 * @return          0:success     other:error
 */
    unsigned int sm2_getE(const unsigned char *M, unsigned int byteLen, const unsigned char Z[32], unsigned char E[32]);

#if 1
#define SM2_GETE_BY_STEPS
#endif
#ifdef SM2_GETE_BY_STEPS
    /**
 * @brief           Step 1 of getting SM2 E value (stepwise style) - initialization
 * @param[in]       ctx                  - Hash context structure pointer
 * @param[in]       Z                    - Z value, 32 bytes
 * @return          SM2_SUCCESS on success, other values indicate error
 */
    unsigned int sm2_getE_init(hash_ctx_t *ctx, const unsigned char Z[32]);

    /**
 * @brief           Step 2 of getting SM2 E value (stepwise style) - update message
 * @param[in]       ctx                  - Hash context structure pointer
 * @param[in]       msg                  - Message to be hashed
 * @param[in]       msg_len              - Byte length of the message
 * @return          SM2_SUCCESS on success, other values indicate error
 * @note            Ensure all parameters are valid and ctx is initialized
 */
    unsigned int sm2_getE_update(hash_ctx_t *ctx, const unsigned char *msg, unsigned int msg_len);

    /**
 * @brief           Step 3 of getting SM2 E value (stepwise style) - finalize and get digest
 * @param[in]       ctx                  - Hash context structure pointer (must be valid and initialized)
 * @param[out]      E                    - Output hash digest (SM2 E value)
 * @return          SM2_SUCCESS on success, other values indicate error
 * @note            Ensure the digest buffer E is sufficient (32 bytes)
 */
    unsigned int sm2_getE_final(hash_ctx_t *ctx, unsigned char E[32]);
#endif
    /**
 * @brief           Generate SM2 public key from private key
 * @param[in]       priKey               - private key, 32 bytes, big-endian
 * @param[out]      pubKey               - public key(0x04 + x + y), 65 bytes, big-endian
 * @return          0:success     other:error
 */
    unsigned int sm2_get_pubkey_from_prikey(const unsigned char priKey[32], unsigned char pubKey[65]);

    /**
 * @brief           Generate SM2 random Key pair
 * @param[in]       priKey               - private key, 32 bytes, big-endian
 * @param[out]      pubKey               - public key(0x04 + x + y), 65 bytes, big-endian
 * @return          0:success     other:error
 */
    unsigned int sm2_getkey(unsigned char priKey[32], unsigned char pubKey[65]);

    /**
 * @brief           Generate SM2 Signature
 * @param[in]       E                    - input, E value, 32 bytes, big-endian
 * @param[in]       rand_k               - input, random big integer k in signing, 32bytes, big-endian,
 *                                         if you do not have this integer, please set this parameter to be NULL,
 *                                         it will be generated inside.
 * @param[in]       priKey               - private key, 32 bytes, big-endian
 * @param[out]      signature            - Signature r and s, 64 bytes, big-endian
 * @return          SM2_SUCCESS(success)     other:error
 * @note            
 *        1.if you do not have rand_k, please set the parameter to be NULL, it will be generated inside.
 */
    unsigned int sm2_sign(const unsigned char E[32], const unsigned char rand_k[32], const unsigned char priKey[32], unsigned char signature[64]);

    /**
 * @brief           Verify SM2 Signature
 * @param[in]       E                    - E value, 32 bytes, big-endian
 * @param[in]       pubKey               - public key(0x04 + x + y), 65 bytes, big-endian
 * @param[in]       signature            - Signature r and s, 64 bytes, big-endian
 * @return          SM2_SUCCESS(success)     other:error
 */
    unsigned int sm2_verify(const unsigned char E[32], const unsigned char pubKey[65], const unsigned char signature[64]);

    /**
 * @brief           SM2 Encryption
 * @param[in]       M                    - plaintext, m_len bytes, big-endian
 * @param[in]       m_len                - byte length of M
 * @param[in]       rand_k               - input, random big integer k in encrypting, 32 bytes, big-endian,
 *                                         if you do not have this integer, please set this parameter to be NULL,
 *                                         it will be generated inside.
 * @param[in]       pubKey               - public key, 65 bytes, big-endian
 * @param[in]       order                - either SM2_C1C3C2 or SM2_C1C2C3
 * @param[out]      C                    - ciphertext, c_len bytes, big-endian
 * @param[out]      c_len                - byte length of C, should be m_len+97 if success
 * @return          SM2_SUCCESS(success)     other:error
 * @note            
 *        1.M and C can be the same buffer.
 *        2.if you do not have rand_k, please set the parameter to be NULL, it will be generated inside.
 *        3.please make sure pubKey is valid
 */
    unsigned int sm2_encrypt(const unsigned char *M, unsigned int m_len, const unsigned char rand_k[32], const unsigned char pubKey[65], sm2_cipher_order_e order, unsigned char *C,
                             unsigned int *c_len);

    /**
 * @brief           SM2 Decryption
 * @param[in]       C                    - ciphertext, c_len bytes, big-endian
 * @param[in]       c_len                - byte length of C, make sure m_len>97
 * @param[in]       priKey               - private key, 32 bytes, big-endian
 * @param[in]       order                - either SM2_C1C3C2 or SM2_C1C2C3
 * @param[out]      M                    - plaintext, m_len bytes, big-endian
 * @param[out]      m_len                - byte length of M, should be c_len-97 if success
 * @return          SM2_SUCCESS(success)     other:error
 * @note            
 *        1.M and C can be the same buffer.
 */
    unsigned int sm2_decrypt(const unsigned char *C, unsigned int c_len, const unsigned char priKey[32], sm2_cipher_order_e order, unsigned char *M, unsigned int *m_len);

    /**
 * @brief           SM2 Key Exchange
 * @param[in]       role                 - Exchange role (SM2_Role_Sponsor or SM2_Role_Responsor)
 * @param[in]       dA                   - Local's permanent private key, 32 bytes
 * @param[in]       PB                   - Peer's permanent public key, 65 bytes
 * @param[in]       rA                   - Local's temporary private key, 32 bytes
 * @param[in]       RA                   - Local's temporary public key, 65 bytes
 * @param[in]       RB                   - Peer's temporary public key, 65 bytes
 * @param[in]       ZA                   - Local's Z value, 32 bytes
 * @param[in]       ZB                   - Peer's Z value, 32 bytes
 * @param[in]       k_len                - Byte length of output key (must be less than 2^32 - 1 bits)
 * @param[out]      KA                   - Output key, k_len bytes
 * @param[out]      S1                   - Sponsor's S1 or responsor's S2 (optional)
 * @param[out]      SA                   - Sponsor's SA or responsor's SB (optional)
 * @return          SM2_SUCCESS on success, other values indicate error
 * @note            
 *        1. Ensure all inputs are valid
 *        2. S1 and SA are optional parameters (set to NULL if not needed)
 *        3. For verification: If S1=SB and S2=SA, the exchange is successful
*/
    unsigned int sm2_exchangekey(sm2_exchange_role_e role, const unsigned char *dA, const unsigned char *PB, const unsigned char *rA, const unsigned char *RA,
                                 const unsigned char *RB, const unsigned char *ZA, const unsigned char *ZB, unsigned int k_len, unsigned char *KA, unsigned char *S1,
                                 unsigned char *SA);

#ifdef __cplusplus
}
#endif

#endif
