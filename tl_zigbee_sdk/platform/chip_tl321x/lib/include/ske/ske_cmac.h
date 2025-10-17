/********************************************************************************************************
 * @file    ske_cmac.h
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
#ifndef SKE_CMAC_H
#define SKE_CMAC_H



#include "ske.h"


#ifdef __cplusplus
extern "C" {
#endif



typedef struct {
    unsigned char k1[16];
    unsigned char k2[16];
    SKE_CTX ske_cmac_ctx[1];
    unsigned char mac_bytes;
    unsigned char left_bytes;
    unsigned char block_buf[16];
    SKE_MAC mac_action;
} SKE_CMAC_CTX;

typedef struct {
    unsigned char k1[16];
    unsigned char k2[16];
    SKE_CTX ske_cmac_ctx[1];
    unsigned char mac_bytes;
} SKE_CMAC_DMA_CTX;




//APIs

unsigned int ske_lp_cmac_init(SKE_CMAC_CTX *ctx, SKE_ALG alg, SKE_MAC mac_action,  unsigned char *key, unsigned short sp_key_idx,
        unsigned char mac_bytes);

unsigned int ske_lp_cmac_update(SKE_CMAC_CTX *ctx, unsigned char *msg, unsigned int msg_bytes);

unsigned int ske_lp_cmac_final(SKE_CMAC_CTX *ctx,unsigned char *mac);

unsigned int ske_lp_cmac(SKE_ALG alg, SKE_MAC mac_action,  unsigned char *key, unsigned short sp_key_idx, unsigned char *msg, unsigned int msg_bytes,
        unsigned char *mac, unsigned char mac_bytes);


#ifdef SKE_LP_DMA_FUNCTION

unsigned int ske_lp_dma_cmac_init(SKE_CMAC_DMA_CTX *ctx, SKE_ALG alg,  unsigned char *key, unsigned short sp_key_idx, unsigned char mac_bytes);

unsigned int ske_lp_dma_cmac_update_blocks_excluding_last_block(SKE_CMAC_DMA_CTX *ctx, unsigned int *msg,
        unsigned int msg_words, unsigned int *tmp_out, SKE_CALLBACK callback);

unsigned int ske_lp_dma_cmac_update_including_last_block(SKE_CMAC_DMA_CTX *ctx, unsigned int *msg, unsigned int msg_bytes,
        unsigned int *tmp_out, unsigned int *mac, SKE_CALLBACK callback);

unsigned int ske_lp_dma_cmac(SKE_ALG alg,  unsigned char *key, unsigned short sp_key_idx, unsigned int *msg, unsigned int msg_bytes,
         unsigned int *tmp_out, unsigned int *mac, unsigned char mac_bytes, SKE_CALLBACK callback);

#endif







#ifdef __cplusplus
}
#endif

#endif
