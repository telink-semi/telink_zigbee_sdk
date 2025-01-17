/********************************************************************************************************
 * @file    x25519.h
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
#ifndef X25519_H
#define X25519_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "lib/include/pke/pke.h"


//extern mont_curve_t c25519[1];


//X25519 return code
#define X25519_SUCCESS        PKE_SUCCESS
#define X25519_POINTER_NULL   (PKE_SUCCESS + 0x70U)
#define X25519_ZERO_ALL       (PKE_SUCCESS + 0x71U)
#define X25519_INVALID_INPUT  (PKE_SUCCESS + 0x72U)
#define X25519_INVALID_OUTPUT (PKE_SUCCESS + 0x73U)


    //APIs

    /**
 * @brief       get X25519 public key from private key.
 * @param[in]   prikey         - private key, 32 bytes, little-endian.
 * @param[in]   pubkey         - public key, 32 bytes, little-endian.
 * @return      X25519_SUCCESS(success)     other:error
 */
    unsigned int x25519_get_pubkey_from_prikey(unsigned char prikey[32], unsigned char pubkey[32]);

    /**
 * @brief       get x25519 random key pair
 * @param[in]   prikey         - private key, 32 bytes, little-endian.
 * @param[in]   pubkey         - public key, 32 bytes, little-endian.
 * @return      X25519_SUCCESS(success)     other:error
 */
    unsigned int x25519_getkey(unsigned char prikey[32], unsigned char pubkey[32]);

    /**
 * @brief       X25519 key agreement.
 * @param[in]   local_prikey     - local private key, 32 bytes, little-endian
 * @param[in]   peer_pubkey      - peer Public key, 32 bytes, little-endian
 * @param[out]  key              - derived key
 * @param[in]   keyByteLen       - byte length of output key
 * @param[in]   kdf              - KDF function
 * @return      X25519_SUCCESS(success)    other:error
 * @note
  @verbatim
      -# 1.if no KDF function, please set kdf to be NULL
  @endverbatim
 */
    unsigned int x25519_compute_key(unsigned char local_prikey[32], unsigned char peer_pubkey[32], unsigned char *key, unsigned int keyByteLen, KDF_FUNC kdf);


#ifdef __cplusplus
}
#endif

#endif
