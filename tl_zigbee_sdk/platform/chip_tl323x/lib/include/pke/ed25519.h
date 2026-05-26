/********************************************************************************************************
 * @file    ed25519.h
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
#ifndef Ed25519_H
#define Ed25519_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "lib/include/pke/pke.h"

    /**
 * @brief           Ed25519 mode
 */
    typedef enum
    {
        Ed25519_DEFAULT = 0,
        Ed25519_CTX,
        Ed25519_PH,
        Ed25519_PH_WITH_PH_M,
    } ed25519_mode_e;

    // fix to old project
    typedef ed25519_mode_e Ed25519_MODE;

/**
 * @brief           Ed25519 return code
 */
#define EdDSA_SUCCESS       PKE_SUCCESS
#define EdDSA_POINTER_NULL  (PKE_SUCCESS + 0x80U)
#define EdDSA_INVALID_INPUT (PKE_SUCCESS + 0x81U)
#define EdDSA_VERIFY_FAIL   (PKE_SUCCESS + 0x82U)

    /**
 * @brief           APIs
 */

    /**
 * @brief           Decode X25519 scalar for point multiplication
 * @param[in]       k                    - scalar
 * @param[out]      out                  - big scalar in little-endian format
 * @param[in]       bytes                - Byte length of k and out
 */
    void x25519_decode_scalar(const unsigned char *k, unsigned char *out, unsigned int bytes);

    /**
 * @brief           Get Ed25519 public key from private key
 * @param[in]       prikey               - Private key, 32 bytes, little-endian
 * @param[out]      pubkey               - Public key, 32 bytes, little-endian
 * @return          EdDSA_SUCCESS on success, other values indicate error
  */
    unsigned int ed25519_get_pubkey_from_prikey(const unsigned char prikey[32], unsigned char pubkey[32]);

    /**
 * @brief           generate Ed25519 random key pair
 * @param[out]      prikey               - private key, 32 bytes, little-endian
 * @param[out]      pubkey               - public key, 32 bytes, little-endian
 * @return          0:success     other:error
 * @note            
 */
    unsigned int ed25519_getkey(unsigned char prikey[32], unsigned char pubkey[32]);

    /**
 * @brief           Ed25519 sign function
 * @param[in]       mode                 - Ed25519 signature mode
 * @param[in]       prikey               - Private key, 32 bytes, little-endian
 * @param[in]       pubkey               - Public key, 32 bytes, little-endian (NULL if not available)
 * @param[in]       ctx                  - Context data, 0-255 bytes
 * @param[in]       ctx_len              - Byte length of ctx
 * @param[in]       M                    - Message, requirements depend on mode
 * @param[in]       m_len                - Byte length of M, requirements depend on mode
 * @param[out]      RS                   - signature
 * @return          EdDSA_SUCCESS on success, other values indicate error
 * @note            
 *        1. If pubkey is NULL, it will be generated internally
 *        2. If mode is not Ed25519_PH_WITH_PH_M, M can be NULL (no need to check M and m_len)
 *        3. For Ed25519_PH_WITH_PH_M, M is SHA512 digest (64 bytes) and m_len is not used
 *        4. For Ed25519_DEFAULT, ctx is not involved
 *        5. For Ed25519_CTX, ctx cannot be empty (length 1-255)
 *        6. For Ed25519_PH/Ed25519_PH_WITH_PH_M, ctx length is 0-255 (default 0, can be empty)
 */
    unsigned int ed25519_sign(ed25519_mode_e mode, const unsigned char prikey[32], const unsigned char pubkey[32], const unsigned char *ctx, unsigned char ctx_len,
                              const unsigned char *M, unsigned int m_len, unsigned char RS[64]);

    /**
 * @brief           Ed25519 verify function
 * @param[in]       mode                 - Ed25519 signature mode
 * @param[in]       pubkey               - Public key, 32 bytes, little-endian
 * @param[in]       ctx                  - Context data, 0-255 bytes
 * @param[in]       ctx_len              - Byte length of ctx
 * @param[in]       M                    - Message, requirements depend on mode
 * @param[in]       m_len                - Byte length of M (can be 0 if M is empty)
 * @param[in]       RS                   - signature
 * @return          EdDSA_SUCCESS on success, other values indicate error
 * @note            
 *        1. If mode is not Ed25519_PH_WITH_PH_M, M can be NULL (no need to check M and m_len)
 *        2. For Ed25519_PH_WITH_PH_M, M is SHA512 value (64 bytes, big-endian) and m_len is not used
 *        3. For Ed25519_DEFAULT, ctx is not involved
 *        4. For Ed25519_CTX, ctx cannot be empty (length 1-255)
 *        5. For Ed25519_PH/Ed25519_PH_WITH_PH_M, ctx length is 0-255 (default 0, can be empty)
 */
    unsigned int ed25519_verify(ed25519_mode_e mode, const unsigned char pubkey[32], const unsigned char *ctx, unsigned char ctx_len, const unsigned char *M, unsigned int m_len,
                                const unsigned char RS[64]);

#ifdef __cplusplus
}
#endif

#endif
