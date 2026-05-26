/********************************************************************************************************
 * @file    ecdh.h
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
#ifndef ECDH_H
#define ECDH_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "lib/include/pke/pke.h"


/**
 * @brief           ECDH return code
 */
#define ECDH_SUCCESS         PKE_SUCCESS
#define ECDH_POINTER_NULL    (PKE_SUCCESS + 0x60U)
#define ECDH_INVALID_INPUT   (PKE_SUCCESS + 0x61U)
#define ECDH_ZERO_ALL        (PKE_SUCCESS + 0x62U)
#define ECDH_INTEGER_TOO_BIG (PKE_SUCCESS + 0x63U)

    /**
 * @brief           APIs
 */

    /**
 * @brief           ECDH compute key
 * @param[in]       curve                - ecc curve struct pointer, please make sure it is valid.
 * @param[in]       local_prikey         - local private key, big-endian.
 * @param[in]       peer_pubkey          - peer public key, big-endian.
 * @param[out]      key                  - output key.
 * @param[in]       key_len              - byte length of output key.
 * @param[in]       kdf                  - KDF function to get key.
 * @return          ECDH_SUCCESS(success)     other:error
 */
    unsigned int ecdh_compute_key(const eccp_curve_t *curve, const unsigned char *local_prikey, const unsigned char *peer_pubkey, unsigned char *key, unsigned int key_len,
                                  KDF_FUNC kdf);
#ifdef __cplusplus
}
#endif

#endif
