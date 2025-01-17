/********************************************************************************************************
 * @file    ecdsa.h
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
#ifndef ECDSA_H
#define ECDSA_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "lib/include/pke/pke.h"


//ECDSA return code
#define ECDSA_SUCCESS         PKE_SUCCESS
#define ECDSA_POINTOR_NULL    (PKE_SUCCESS + 0x50U)
#define ECDSA_INVALID_INPUT   (PKE_SUCCESS + 0x51U)
#define ECDSA_ZERO_ALL        (PKE_SUCCESS + 0x52U)
#define ECDSA_INTEGER_TOO_BIG (PKE_SUCCESS + 0x53U)
#define ECDSA_VERIFY_FAILED   (PKE_SUCCESS + 0x54U)


    //APIs

    unsigned int ecdsa_sign(eccp_curve_t *curve, unsigned char *E, unsigned int EByteLen, unsigned char *rand_k, unsigned char *priKey, unsigned char *signature);

    unsigned int ecdsa_verify(eccp_curve_t *curve, unsigned char *E, unsigned int EByteLen, unsigned char *pubKey, unsigned char *signature);


#ifdef ECDSA_SEC

    //ECDSA return code(secure version)
    #define ECDSA_SUCCESS_S (0x7D5FEB14U)
    #define ECDSA_ERROR_S   (0xB4C0BC5AU)


    unsigned int ecdsa_sign_s(eccp_curve_t *curve, unsigned char *E, unsigned int EByteLen, unsigned char *rand_k, unsigned char *priKey, unsigned char *signature);

    unsigned int ecdsa_verify_s(eccp_curve_t *curve, unsigned char *E, unsigned int EByteLen, unsigned char *pubKey, unsigned char *signature);

#endif


#ifdef __cplusplus
}
#endif

#endif
