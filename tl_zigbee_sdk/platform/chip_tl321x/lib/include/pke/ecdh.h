/********************************************************************************************************
 * @file    ecdh.h
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
#ifndef ECDH_H
#define ECDH_H

#ifdef __cplusplus
extern "C" {
#endif



#include "lib/include/pke/pke.h"


//ECDH return code
#define ECDH_SUCCESS                          PKE_SUCCESS
#define ECDH_POINTOR_NULL                     (PKE_SUCCESS+0x60U)
#define ECDH_INVALID_INPUT                    (PKE_SUCCESS+0x61U)
#define ECDH_ZERO_ALL                         (PKE_SUCCESS+0x62U)
#define ECDH_INTEGER_TOO_BIG                  (PKE_SUCCESS+0x63U)



//APIs

unsigned int ecdh_compute_key( eccp_curve_t *curve, unsigned char *local_prikey, unsigned char *peer_pubkey, unsigned char *key,
        unsigned int keyByteLen, KDF_FUNC kdf);




#ifdef ECDH_SEC

//ECDH return code(secure version)
#define ECDH_SUCCESS_S                        (0x8B9BC1E1U)
#define ECDH_ERROR_S                          (0xCBC192A3U)


unsigned int ecdh_compute_key_s(eccp_curve_t *curve, unsigned char *local_prikey, unsigned char *peer_pubkey, unsigned char *key,
        unsigned int keyByteLen, KDF_FUNC kdf);

#endif




#ifdef __cplusplus
}
#endif

#endif
