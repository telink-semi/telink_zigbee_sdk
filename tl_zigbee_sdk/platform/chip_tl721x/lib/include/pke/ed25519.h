/********************************************************************************************************
 * @file    ed25519.h
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
#ifndef Ed25519_H
#define Ed25519_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "lib/include/pke/pke.h"

    //Ed25519 mode
    typedef enum
    {
        Ed25519_DEFAULT = 0,
        Ed25519_CTX,
        Ed25519_PH,
    } Ed25519_MODE;

    //Ed25519 return code
    enum
    {
        EdDSA_SUCCESS      = PKE_SUCCESS,
        EdDSA_POINTOR_NULL = PKE_SUCCESS + 0x80,
        EdDSA_INVALID_INPUT,
        EdDSA_VERIFY_FAIL,
    };

    //APIs

    /**
 * @brief      get Ed25519 public key from private key
 * @param[in]  prikey                - private key, 32 bytes, little-endian.
 * @param[out] pubkey                - public key, 32 bytes, little-endian.
 * @return      PKE_SUCCESS(success), other(error)
 */
    unsigned int ed25519_get_pubkey_from_prikey(unsigned char prikey[32], unsigned char pubkey[32]);

    /**
 * @brief      generate Ed25519 random key pair
 * @param[out] prikey                - private key, 32 bytes, little-endian.
 * @param[out] pubkey                - public key, 32 bytes, little-endian.
 * @return     PKE_SUCCESS(success), other(error)
 */
    unsigned int ed25519_getkey(unsigned char prikey[32], unsigned char pubkey[32]);

    /**
 * @brief      Ed25519 sign
 * @param[in]  mode                - Ed25519 signature mode
 * @param[in]  prikey              - private key, 32 bytes, little-endian
 * @param[in]  pubkey              - public key, 32 bytes, little-endian, if no pubkey, please set it to be NULL
 * @param[in]  ctx                 - 0-255 bytes
 * @param[in]  ctxByteLen          - byte length of ctx
 * @param[in]  M                   - message, M could be empty, in this case please set M to be NULL
 * @param[in]  MByteLen            - byte length of M, M could be empty, so it could be 0
 * @param[out] RS                  - signature
 * @return      PKE_SUCCESS(success), other(error)
 * @note
  @verbatim
      -# 1. if no public key, please set pubkey to be NULL, it will be generated inside
      -# 2. M could be empty(please set M to be NULL), so no need to check M and MByteLen
      -# 3. if mode is Ed25519_DEFAULT, ctx is not involved, no need to check ctx and ctxByteLen
      -# 4. if mode is Ed25519_CTX, ctx can not be empty(ctx length is from 1 to 255)
      -# 5. if mode is Ed25519_PH, ctx length is from 0 to 255, default length is 0, thus ctx could be empty
  @endverbatim
 */
    unsigned int ed25519_sign(Ed25519_MODE mode, unsigned char prikey[32], unsigned char pubkey[32], unsigned char *ctx, unsigned char ctxByteLen, unsigned char *M, unsigned int MByteLen, unsigned char RS[64]);

    /**
 * @brief       Ed25519 verify
 * @param[in]  mode                - Ed25519 signature mode
 * @param[in]  pubkey              - public key, 32 bytes, little-endian, if no pubkey, please set it to be NULL
 * @param[in]  ctx                 - 0-255 bytes
 * @param[in]  ctxByteLen          - byte length of ctx
 * @param[in]  M                   - message, M could be empty, in this case please set M to be NULL
 * @param[in]  MByteLen            - byte length of M, M could be empty, so it could be 0
 * @param[out] RS                  - signature
 * @return     PKE_SUCCESS(success), other(error)
 * @note
  @verbatim
      -# 1. M could be empty(please set M to be NULL), so no need to check M and MByteLen
      -# 2. if mode is Ed25519_DEFAULT, ctx is not involved, no need to check ctx and ctxByteLen
      -# 3. if mode is Ed25519_CTX, ctx can not be empty(ctx length is from 1 to 255)
      -# 4. if mode is Ed25519_PH, ctx length is from 0 to 255, default length is 0, thus ctx could be empty
  @endverbatim
 */
    unsigned int ed25519_verify(Ed25519_MODE mode, unsigned char pubkey[32], unsigned char *ctx, unsigned char ctxByteLen, unsigned char *M, unsigned int MByteLen, unsigned char RS[64]);

    /**
 * @brief     decode X25519 scalar for point multiplication
 * @param[in]  k                - null.
 * @param[out] out              - big scalar in little-endian
 * @param[in]  bytes            - byte length of k and out
 * @return     none
 */
    void x25519_decode_scalar(unsigned char *k, unsigned char *out, unsigned int bytes);

#ifdef __cplusplus
}
#endif

#endif
