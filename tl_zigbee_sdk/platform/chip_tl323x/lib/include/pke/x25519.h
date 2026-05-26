/********************************************************************************************************
 * @file    x25519.h
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
#ifndef X25519_H
#define X25519_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "lib/include/pke/pke.h"


// X25519 return code
#define X25519_SUCCESS        PKE_SUCCESS
#define X25519_POINTER_NULL   (PKE_SUCCESS + 0x70U)
#define X25519_ZERO_ALL       (PKE_SUCCESS + 0x71U)
#define X25519_INVALID_INPUT  (PKE_SUCCESS + 0x72U)
#define X25519_INVALID_OUTPUT (PKE_SUCCESS + 0x73U)

    /**
 * @brief           APIs for user
 */

    /**
 * @brief           Generate X25519 public key from a private key.
 *                  This function generates the corresponding public key for a given private key
 *                  using scalar multiplication.
 * @param[in]       prikey               - Private key (32 bytes in little-endian format).
 * @param[out]      pubkey               - Public key (32 bytes in little-endian format).
 * @return          X25519_SUCCESS on success, error code otherwise.
 */
    unsigned int x25519_get_pubkey_from_prikey(const unsigned char prikey[32], unsigned char pubkey[32]);

    /**
 * @brief           Generate a random key pair for X25519.
 *                  This function generates a new private key and computes the corresponding
 *                  public key.
 * @param[out]      prikey               - Generated private key (32 bytes in little-endian format).
 * @param[out]      pubkey               - Corresponding public key (32 bytes in little-endian format).
 * @return          X25519_SUCCESS on success, error code otherwise.
 */
    unsigned int x25519_getkey(unsigned char prikey[32], unsigned char pubkey[32]);

    /**
 * @brief           Perform X25519 key agreement.
 *                  This function computes the shared secret between local and peer keys using
 *                  scalar multiplication, and optionally applies a Key Derivation Function (KDF)
 *                  to derive the final key material.
 * @param[in]       local_prikey         - Local private key (32 bytes in little-endian format).
 * @param[in]       peer_pubkey          - Peer public key (32 bytes in little-endian format).
 * @param[out]      key                  - Derived key.
 * @param[in]       key_len              - Byte length of the output key.
 * @param[in]       kdf                  - Key Derivation Function. Set to NULL if no KDF is used.
 * @return          X25519_SUCCESS on success, error code otherwise.
 */
    unsigned int x25519_compute_key(const unsigned char local_prikey[32], const unsigned char peer_pubkey[32], unsigned char *key, unsigned int key_len, KDF_FUNC kdf);

#ifdef __cplusplus
}
#endif

#endif
