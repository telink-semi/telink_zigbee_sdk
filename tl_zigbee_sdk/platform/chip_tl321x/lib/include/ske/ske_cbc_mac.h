/********************************************************************************************************
 * @file    ske_cbc_mac.h
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
#ifndef SKE_CBC_MAC_H
#define SKE_CBC_MAC_H



#include "ske.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    SKE_CTX ske_cbc_mac_ctx[1];
    unsigned char mac_bytes;
    unsigned char is_updated;
    unsigned char left_bytes;
    unsigned char block_buf[16];
    SKE_PADDING padding;
    SKE_MAC mac_action;
} SKE_CBC_MAC_CTX;


typedef struct {
    SKE_CTX ske_cbc_mac_ctx[1];
    unsigned char mac_bytes;
} SKE_CBC_MAC_DMA_CTX;




//APIs

unsigned int ske_lp_cbc_mac_init(SKE_CBC_MAC_CTX *ctx, SKE_ALG alg, SKE_MAC mac_action, SKE_PADDING padding,  unsigned char *key,
        unsigned short sp_key_idx, unsigned char mac_bytes);

unsigned int ske_lp_cbc_mac_update(SKE_CBC_MAC_CTX *ctx, unsigned char *msg, unsigned int msg_bytes);

unsigned int ske_lp_cbc_mac_final(SKE_CBC_MAC_CTX *ctx, unsigned char *mac);

unsigned int ske_lp_cbc_mac(SKE_ALG alg, SKE_MAC mac_action, SKE_PADDING padding,  unsigned char *key, unsigned short sp_key_idx,
        unsigned char *msg, unsigned int msg_bytes, unsigned char *mac, unsigned char mac_bytes);



#ifdef SKE_LP_DMA_FUNCTION
unsigned int ske_lp_dma_cbc_mac_init(SKE_CBC_MAC_DMA_CTX *ctx, SKE_ALG alg,  unsigned char *key, unsigned short sp_key_idx, unsigned char mac_bytes);

unsigned int ske_lp_dma_cbc_mac_update_blocks_excluding_last_block(SKE_CBC_MAC_DMA_CTX *ctx, unsigned int *msg, unsigned int msg_words,
        unsigned int *tmp_out, SKE_CALLBACK callback);

unsigned int ske_lp_dma_cbc_mac_update_including_last_block(SKE_CBC_MAC_DMA_CTX *ctx, unsigned int *msg, unsigned int msg_bytes,
        unsigned int *tmp_out, unsigned int *mac, SKE_CALLBACK callback);

unsigned int ske_lp_dma_cbc_mac(SKE_ALG alg,  unsigned char *key, unsigned short sp_key_idx, unsigned int *msg, unsigned int msg_bytes,
        unsigned int *tmp_out, unsigned int *mac, unsigned int mac_bytes, SKE_CALLBACK callback);
#endif







#ifdef __cplusplus
}
#endif

#endif
