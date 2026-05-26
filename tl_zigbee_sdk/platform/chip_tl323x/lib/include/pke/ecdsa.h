/********************************************************************************************************
 * @file    ecdsa.h
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
#ifndef ECDSA_H
#define ECDSA_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "lib/include/pke/pke.h"


/**
 * @brief           ECDSA return code
 */
#define ECDSA_SUCCESS         PKE_SUCCESS
#define ECDSA_POINTER_NULL    (PKE_SUCCESS + 0x50U)
#define ECDSA_INVALID_INPUT   (PKE_SUCCESS + 0x51U)
#define ECDSA_ZERO_ALL        (PKE_SUCCESS + 0x52U)
#define ECDSA_INTEGER_TOO_BIG (PKE_SUCCESS + 0x53U)
#define ECDSA_VERIFY_FAILED   (PKE_SUCCESS + 0x54U)

    /**
 * @brief           APIs
 */

    /**
 * @brief           Generate ECDSA Signature in byte string style
 * @param[in]       curve                - ecc curve struct pointer, please make sure it is valid.
 * @param[in]       E                    - derived from hash value.
 * @param[in]       e_len                - byte length of E.
 * @param[in]       rand_k               - random big integer k in signing, big-endian.
 * @param[out]      priKey               - private key, big-endian.
 * @param[out]      signature            - signature r and s, big-endian.
 * @return          0:success     other:error
 */
    unsigned int ecdsa_sign(const eccp_curve_t *curve, const unsigned char *E, unsigned int e_len, const unsigned char *rand_k, const unsigned char *priKey,
                            unsigned char *signature);

    /**
 * @brief           Verify ECDSA Signature in byte string style
 * @param[in]       curve                - ecc curve struct pointer, please make sure it is valid.
 * @param[in]       E                    - hash value, big-endian.
 * @param[in]       e_len                - byte length of E.
 * @param[in]       pubKey               - private key, big-endian.
 * @param[out]      signature            - signature r and s, big-endian.
 * @return          ECDSA_SUCCESS(success)     other:error
 */
    unsigned int ecdsa_verify(const eccp_curve_t *curve, const unsigned char *E, unsigned int e_len, const unsigned char *pubKey, const unsigned char *signature);
#ifdef __cplusplus
}
#endif

#endif
