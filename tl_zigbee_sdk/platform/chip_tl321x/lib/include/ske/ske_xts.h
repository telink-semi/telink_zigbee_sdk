/********************************************************************************************************
 * @file    ske_xts.h
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
#ifndef SKE_XTS_H
#define SKE_XTS_H



#include "ske.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct{
    unsigned char t[16];
    SKE_CRYPTO crypto;
    SKE_CTX ske_xts_ctx[1];
    unsigned int c_bytes;
    unsigned int current_bytes;
} SKE_XTS_CTX;

//APIs

unsigned int ske_lp_xts_init(SKE_XTS_CTX *ctx, SKE_ALG alg, SKE_CRYPTO crypto, unsigned char *key, unsigned short sp_key_idx,
        unsigned char *i, unsigned int c_bytes);

unsigned int ske_lp_xts_update_blocks(SKE_XTS_CTX *ctx, unsigned char *in, unsigned char *out, unsigned int bytes);

unsigned int ske_lp_xts_update_including_last_2_blocks(SKE_XTS_CTX *ctx, unsigned char *in, unsigned char *out, unsigned int bytes);

unsigned int ske_lp_xts_final(SKE_XTS_CTX *ctx);

unsigned int ske_lp_xts_crypto(SKE_ALG alg, SKE_CRYPTO crypto, unsigned char *key, unsigned short sp_key_idx, unsigned char *i,
        unsigned char *in, unsigned char *out, unsigned int bytes);







#ifdef __cplusplus
}
#endif

#endif
