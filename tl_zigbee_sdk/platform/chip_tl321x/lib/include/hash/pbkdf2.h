/********************************************************************************************************
 * @file    pbkdf2.h
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
#ifndef PBKDF2_H
#define PBKDF2_H



#include "hmac.h"



#ifdef __cplusplus
extern "C" {
#endif



//APIs
/**
 * @brief       pbkdf2 function(using hmac as PRF)
 * @param[in]   hash_alg         - specific hash algorithm.
 * @param[in]   pwd              - password, as the key of hmac.
 * @param[in]   sp_key_idx       - index of secure port key(password).
 * @param[in]   pwd_bytes        - byte length of password, it could be 0.
 * @param[in]   salt             - salt.
 * @param[in]   salt_bytes       - byte length of salt, it could be 0.
 * @param[in]   iter             - iteration times.
 * @param[out]  out              - derived key.
 * @param[in]   out_bytes        - byte length of derived key.
 * @return      0:success     other:error
 */
unsigned int pbkdf2_hmac(HASH_ALG hash_alg, unsigned char *pwd, unsigned short sp_key_idx, unsigned int pwd_bytes, unsigned char *salt, unsigned int salt_bytes,
        unsigned int iter, unsigned char *out, unsigned int out_bytes);


#ifdef __cplusplus
}
#endif


#endif
