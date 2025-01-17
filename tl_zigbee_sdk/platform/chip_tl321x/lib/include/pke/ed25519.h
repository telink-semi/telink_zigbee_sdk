/********************************************************************************************************
 * @file    ed25519.h
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
#ifndef Ed25519_H
#define Ed25519_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "lib/include/pke/pke.h"

    //extern  edward_curve_t ed25519[1];


    //Ed25519 mode
    typedef enum
    {
        Ed25519_DEFAULT = 0,
        Ed25519_CTX,
        Ed25519_PH,
        Ed25519_PH_WITH_PH_M,
    } Ed25519_MODE;

//Ed25519 return code
#define EdDSA_SUCCESS       PKE_SUCCESS
#define EdDSA_POINTOR_NULL  (PKE_SUCCESS + 0x80U)
#define EdDSA_INVALID_INPUT (PKE_SUCCESS + 0x81U)
#define EdDSA_VERIFY_FAIL   (PKE_SUCCESS + 0x82U)


    //APIs

    /**
 * @brief       get Ed25519 public key from private key
 * @param[in]   prikey            - private key, 32 bytes, little-endian
 * @param[out]  pubkey            - public key, 32 bytes, little-endian
 * @return      0:success     other:error
*/
    unsigned int ed25519_get_pubkey_from_prikey(unsigned char prikey[32], unsigned char pubkey[32]);

    /**
 * @brief       generate Ed25519 random key pair
 * @param[out]  prikey            - private key, 32 bytes, little-endian
 * @param[out]  pubkey            - public key, 32 bytes, little-endian
 * @return      0:success     other:error
 * @note
*/
    unsigned int ed25519_getkey(unsigned char prikey[32], unsigned char pubkey[32]);

    /**
 * @brief       Ed25519 sign
 * @param[in]   mode               - Ed25519 signature mode
 * @param[in]   prikey             - private key, 32 bytes, little-endian
 * @param[in]   pubkey             - public key, 32 bytes, little-endian, if no pubkey, please set it to be NULL
 * @param[in]   ctx                - 0-255 bytes
 * @param[in]   ctxByteLen         - byte length of ctx
 * @param[in]   M                  - message, requirements are determined by mode
 * @param[in]   MByteLen           - byte length of M, M could be empty, so it could be 0
 * @param[out]  RS                 - signature
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.if no public key, please set pubkey to be NULL, it will be generated inside
      -# 2.if mode is not Ed25519_PH_WITH_PH_M, M could be empty(please set M to be NULL),
           so no need to check M and MByteLen, otherwise, M is sha512 value, occupies 64 bytes,
           and MByteLen is not involved
      -# 3.if mode is Ed25519_DEFAULT, ctx is not involved, no need to check ctx and ctxByteLen
      -# 4.if mode is Ed25519_CTX, ctx can not be empty(ctx length is from 1 to 255)
      -# 5.if mode is Ed25519_PH or Ed25519_PH_WITH_PH_M, ctx length is from 0 to 255, default
           length is 0, thus ctx could be empty
  @endverbatim
*/
    unsigned int ed25519_sign(Ed25519_MODE mode, unsigned char prikey[32], unsigned char pubkey[32], unsigned char *ctx, unsigned char ctxByteLen, unsigned char *M, unsigned int MByteLen, unsigned char RS[64]);

    /**
 * @brief       Ed25519 verify
 * @param[in]   mode               - Ed25519 signature mode
 * @param[in]   pubkey             - public key, 32 bytes, little-endian
 * @param[in]   ctx                - 0-255 bytes
 * @param[in]   ctxByteLen         - byte length of ctx
 * @param[in]   M                  - message, requirements are determined by mode
 * @param[in]   MByteLen           - byte length of M, M could be empty, so it could be 0
 * @param[in]   RS                 - signature
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.if mode is not Ed25519_PH_WITH_PH_M, M could be empty(please set M to be NULL),
           so no need to check M and MByteLen, otherwise, M is sha512 value, occupies 64 bytes,
           and MByteLen is not involved
      -# 2.if mode is Ed25519_DEFAULT, ctx is not involved, no need to check ctx and ctxByteLen
      -# 3.if mode is Ed25519_CTX, ctx can not be empty(ctx length is from 1 to 255)
      -# 4.if mode is Ed25519_PH or Ed25519_PH_WITH_PH_M, ctx length is from 0 to 255, default
           length is 0, thus ctx could be empty
      -# 5.if mode is Ed25519_PH_WITH_PH_M, the M is sha512 value, big-endian, occupies 64 bytes,
           and MByteLen is not involved
  @endverbatim
*/
    unsigned int ed25519_verify(Ed25519_MODE mode, unsigned char pubkey[32], unsigned char *ctx, unsigned char ctxByteLen, unsigned char *M, unsigned int MByteLen, unsigned char RS[64]);


#ifdef __cplusplus
}
#endif

#endif
