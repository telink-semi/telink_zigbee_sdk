/********************************************************************************************************
 * @file    x25519.h
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
#ifndef X25519_H
#define X25519_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "lib/include/pke/pke.h"


    extern mont_curve_t c25519[1];

    //X25519 return code
    enum
    {
        X25519_SUCCESS      = PKE_SUCCESS,
        X25519_POINTER_NULL = PKE_SUCCESS + 0x70,
        X25519_ZERO_ALL,
        X25519_INVALID_INPUT,
        X25519_INVALID_OUTPUT,
    };

    //APIs
    /**
 * @brief     Get X25519 public key from private key
 * @param[in]  prikey           - Input private key, 32 bytes, little-endian format.
 * @param[out] pubkey           - Output public key, 32 bytes, little-endian format.
 * @return    X25519_SUCCESS(success); other(error)
 */
    unsigned int x25519_get_pubkey_from_prikey(unsigned char prikey[32], unsigned char pubkey[32]);

    /**
 * @brief     Get X25519 random key pair
 * @param[out] prikey           - Output private key, 32 bytes, little-endian format.
 * @param[out] pubkey           - Output public key, 32 bytes, little-endian format.
 * @return    X25519_SUCCESS(success); other(error)
 */
    unsigned int x25519_getkey(unsigned char prikey[32], unsigned char pubkey[32]);

    /**
 * @brief     X25519 key agreement
 * @param[in] local_prikey      - Input local private key, 32 bytes, little-endian format.
 * @param[in] peer_pubkey       - Input peer public key, 32 bytes, little-endian format.
 * @param[out] key              - Output derived key.
 * @param[in] keyByteLen        - Input byte length of the output key.
 * @param[in] kdf               - Input KDF function; set to NULL if not used.
 * @return     X25519_SUCCESS(success); other(error)
 */
    unsigned int x25519_compute_key(unsigned char local_prikey[32], unsigned char peer_pubkey[32], unsigned char *key, unsigned int keyByteLen, KDF_FUNC kdf);


#ifdef __cplusplus
}
#endif

#endif
