/********************************************************************************************************
 * @file    ecdsa.h
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
#ifndef ECDSA_H
#define ECDSA_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "pke.h"

    //ECDSA return code
    enum ECDSA_RET_CODE
    {
        ECDSA_SUCCESS      = PKE_SUCCESS,
        ECDSA_POINTOR_NULL = PKE_SUCCESS + 0x50,
        ECDSA_INVALID_INPUT,
        ECDSA_ZERO_ALL,
        ECDSA_INTEGER_TOO_BIG,
        ECDSA_VERIFY_FAILED,
    };

    //APIs
    /**
 * @brief       Generate ECDSA Signature in U32 little-endian big integer style
 * @param[in]   curve         - ecc curve struct pointer, please make sure it is valid.
 * @param[in]   E             - derived from hash value.
 * @param[in]   EByteLen      - byte length of E.
 * @param[in]   rand_k        - internal random integer k.
 * @param[out]  priKey        - private key.
 * @param[out]  signature     - signature s.
 * @return      0:success     other:error
 */
    unsigned int ecdsa_sign(eccp_curve_t *curve, unsigned char *E, unsigned int EByteLen, unsigned char *rand_k, unsigned char *priKey, unsigned char *signature);

    /**
 * @brief       Generate ECDSA Signature in byte string style
 * @param[in]   curve               - ecc curve struct pointer, please make sure it is valid.
 * @param[in]   E                   - hash value, big-endian.
 * @param[in]   EByteLen            - byte length of E.
 * @param[in]   pubKey              - private key, big-endian.
 * @param[out]  signature           - signature r and s, big-endian.
 * @return      0:success     other:error
 */
    unsigned int ecdsa_verify(eccp_curve_t *curve, unsigned char *E, unsigned int EByteLen, unsigned char *pubKey, unsigned char *signature);


#ifdef __cplusplus
}
#endif

#endif
