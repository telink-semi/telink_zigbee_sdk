/********************************************************************************************************
 * @file    ske_ccm.h
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
#ifndef SKE_CCM_H
#define SKE_CCM_H


#include "ske.h"


#ifdef __cplusplus
extern "C" {
#endif



#define SKE_LP_CCM_CPU_UPDATE_AAD_BY_STEP




typedef struct{
    unsigned char buf[16];
    SKE_CTX ske_ccm_ctx[1];
    SKE_CRYPTO crypto;
    unsigned int aad_bytes;
    unsigned int c_bytes;
    unsigned int current_bytes;
    unsigned char b1_aad_start_offset;
    unsigned char b1_aad_end_offset;
    unsigned char M;
    unsigned char L;
} SKE_CCM_CTX;




//APIs
/**
 * @brief       ske_lp ccm mode init config.
 * @param[in]   ctx              - SKE_CCM_CTX context pointer.
 * @param[in]   alg              - ske_lp algorithm.
 * @param[in]   crypto           - encrypting or decrypting.
 * @param[in]   key              - TDES key, 24 bytes.
 * @param[in]   sp_key_idx       - key in bytes, key of AES(128/192/256) or SM4.
 * @param[in]   nonce            - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX].
 * @param[in]   M                - nonce in bytes, its byte length is 15-L.
 * @param[in]   L                - bytes of length field(message byte length is less than 256^L).
 * @param[in]   aad_bytes        - byte length of aad, it could be any value, including 0.
 * @param[in]   c_bytes          - byte length of plaintext/ciphertext, it could be any value, including 0.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.this function is for CPU style.
      -# 2.only AES(128/192/256) and SM4 are supported for CCM mode.
      -# 3.if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
       otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX].
      -# 4.only AES(128/192/256) and SM4 are supported for CCM mode.
      -# 5.aad_bytes and c_bytes could not be zero at the same time due to hardware implementation.
  @endverbatim
 */
unsigned int ske_lp_ccm_init(SKE_CCM_CTX *ctx, SKE_ALG alg, SKE_CRYPTO crypto, unsigned char *key, unsigned short sp_key_idx,
        unsigned char *nonce, unsigned char M, unsigned char L, unsigned int aad_bytes, unsigned int c_bytes);
/**
 * @brief       ske_lp ccm mode input aad(multiple step style).
 * @param[in]   ctx              - SKE_GCM_CTX context pointer.
 * @param[in]   aad              - aad.
 * @param[in]   bytes            - byte length of aad.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.this function must be called after calling ske_lp_ccm_init().
      -# 2.if there is no aad, this function could be omitted.
      -# 3.if the whole aad is too long, you could divide it into some sections by byte, then call
        this function to input the sections respectively. for example, if the whole aad byte
        length is 65, it could be divided into 3 sections with byte length 10,47,8 respectively.
  @endverbatim
 */
unsigned int ske_lp_ccm_update_aad(SKE_CCM_CTX *ctx, unsigned char *aad, unsigned int bytes);

/**
 * @brief       ske_lp ccm mode input aad(one-off style).
 * @param[in]   ctx              - SKE_GCM_CTX context pointer.
 * @param[in]   aad              - aad, its length is ctx->aad_bytes, please make sure
                                   aad here is integral.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.this function must be called after calling ske_lp_ccm_init().
      -# 2.if there is no aad, this function could be omitted.
  @endverbatim
 */
unsigned int ske_lp_ccm_aad(SKE_CCM_CTX *ctx, unsigned char *aad);

/**
 * @brief       ske_lp ccm mode input plaintext/ciphertext.
 * @param[in]   ctx              - SKE_CCM_CTX context pointer.
 * @param[in]   in               - plaintext or ciphertext.
 * @param[out]  out              - ciphertext or plaintext.
 * @param[in]   bytes            - byte length of input or output.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.this function must be called after inputting aad.
      -# 2.if there is no plaintext/ciphertext, this function could be omitted.
      -# 3.to save memory, in and out could be the same buffer, in this case, the output will
       cover the input.
      -# 4.if the whole plaintext/ciphertext is too long, you could divide it by block(16 bytes),
          and if the whole plaintext/ciphertext byte length is not a multiple of 16, please make
          sure the last section contains the tail, then call this function to input the sections
          respectively. for example, if the whole plaintext/ciphertext byte length is 65, it
          could be divided into 3 sections with byte length 32,16,17 respectively..
  @endverbatim
 */
unsigned int ske_lp_ccm_update_blocks(SKE_CCM_CTX *ctx, unsigned char *in, unsigned char *out, unsigned int bytes);

/**
 * @brief       ske_lp ccm mode input plaintext/ciphertext.
 * @param[in]   ctx              - SKE_CCM_CTX context pointer.
 * @param[in]   mac              - (for decryption), output(for encryption).
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.this function must be called after calling ske_lp_ccm_update_blocks().
      -# 2.byte length of mac is ctx->M.
      -# 3.for encryption, mac is output; and for decryption, mac is input, if returns SKE_SUCCESS
 *        that means certification passed, otherwise not.
      -# 4.if the whole plaintext/ciphertext is too long, you could divide it by block(16 bytes),
          and if the whole plaintext/ciphertext byte length is not a multiple of 16, please make
          sure the last section contains the tail, then call this function to input the sections
          respectively. for example, if the whole plaintext/ciphertext byte length is 65, it
          could be divided into 3 sections with byte length 32,16,17 respectively.
  @endverbatim
 */
unsigned int ske_lp_ccm_final(SKE_CCM_CTX *ctx, unsigned char *mac);

/**
 * @brief       ske_lp ccm mode encrypt/decrypt(one-off style).
 * @param[in]   alg              - ske_lp algorithm.
 * @param[in]   crypto           - encrypting or decrypting.
 * @param[in]   key              - key in bytes, key of AES(128/192/256) or SM4.
 * @param[in]   sp_key_idx       - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX].
 * @param[in]   nonce            - nonce in bytes, its byte length is 15-L.
 * @param[in]   M                - bytes of authentication field(bytes of mac).
 * @param[in]   L                -  bytes of length field(message byte length is less than 256^L).
 * @param[in]   aad              - aad, please make sure aad here is integral.
 * @param[in]   aad_bytes        - byte length of aad, it could be any value, including 0.
 * @param[in]   in               - plaintext or ciphertext.
 * @param[out]  out              - ciphertext or plaintext.
 * @param[in]   c_bytes          - byte length of plaintext/ciphertext, it could be any value, including 0.
 * @param[in]   mac              - (for decryption), output(for encryption).
 *
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.this function is for CPU style.
      -# 2.only AES(128/192/256) and SM4 are supported for GCM mode.
      -# 3.if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
         otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX].
      -# 4.valid M is {4,6,8,10,12,14,16}, and valid L is {2,3,4,5,6,7,8}.
      -# 5.aad_bytes and c_bytes could not be zero at the same time due to hardware implementation.
      -# 6.to save memory, in and out could be the same buffer, in this case, the output will
        cover the input.
      -# 7.byte length of mac is M.
      -# 8.for encryption, mac is output; and for decryption, mac is input, if returns SKE_SUCCESS
 *        that means certification passed, otherwise not.
  @endverbatim
 */
unsigned int ske_lp_ccm_crypto(SKE_ALG alg, SKE_CRYPTO crypto, unsigned char *key, unsigned short sp_key_idx, unsigned char *nonce,
        unsigned char M, unsigned char L, unsigned char *aad, unsigned int aad_bytes, unsigned char *in, unsigned char *out, unsigned int c_bytes,
        unsigned char *mac);


#ifdef SKE_LP_DMA_FUNCTION
/**
 * @brief       ske_lp dma ccm mode init config.
 * @param[in]   ctx              - SKE_CCM_CTX context pointer.
 * @param[in]   alg              - ske_lp algorithm.
 * @param[in]   crypto           - encrypting or decrypting.
 * @param[in]   key              - key in bytes, key of AES(128/192/256) or SM4.
 * @param[in]   sp_key_idx       - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX].
 * @param[in]   nonce            - nonce in bytes, its byte length is 15-L.
 * @param[in]   M                - bytes of authentication field(bytes of mac).
 * @param[in]   L                -  bytes of length field(message byte length is less than 256^L).
 * @param[in]   aad_bytes        - byte length of aad, it could be any value, including 0.
 * @param[in]   c_bytes          - byte length of plaintext/ciphertext, it could be any value, including 0.
 *
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.this function is for DMA style.
      -# 2.only AES(128/192/256) and SM4 are supported for CCM mode.
      -# 3.if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
         otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX].
      -# valid M is {4,6,8,10,12,14,16}, and valid L is {2,3,4,5,6,7,8}.
      -# 5.aad_bytes and c_bytes could not be zero at the same time due to hardware implementation.
  @endverbatim
 */
unsigned int ske_lp_dma_ccm_init(SKE_CCM_CTX *ctx, SKE_ALG alg, SKE_CRYPTO crypto, unsigned char *key, unsigned short sp_key_idx,
        unsigned char *nonce, unsigned char M, unsigned char L, unsigned int aad_bytes, unsigned int c_bytes);

/**
 * @brief       to get B0 block, please make sure all parameters are valid.
 * @param[in]   nonce           - pointer to the nonce value.
 * @param[in]   M               - number of message authentication bits (tag length).
 * @param[in]   L               - length of the length field in bytes.
 * @param[in]   aad_bytes       - number of bytes in the Additional Authenticated Data (AAD).
 * @param[in]   c_bytes         - number of bytes in the ciphertext.
 * @param[out]  out             - pointer to the output B0 block
 * @return      0:success     other:error
 */
void ske_lp_ccm_get_B0(unsigned char *nonce, unsigned char M, unsigned char L, unsigned int aad_bytes, unsigned int c_bytes, unsigned char out[16]);

/**
 * @brief       ske_lp dma ccm mode update B0(multiple steps style).
 * @param[in]   ctx              - SKE_CCM_CTX context pointer.
 * @param[in]   B0               - ske_lp algorithm.
 * @param[in]   callback         - callback function pointer, this could be NULL, means do nothing.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.this is for multiple steps style.
      -# 2.this function must be called after calling ske_lp_dma_ccm_init().
  @endverbatim
 */
unsigned int ske_lp_dma_ccm_update_B0_block(SKE_CCM_CTX *ctx, unsigned int B0[4], SKE_CALLBACK callback);

/**
 * @brief       to get B1 block(if aad exists), please make sure all parameters are valid.
 * @param[in]   aad              -
 * @param[in]   aad_bytes        -
 * @param[in]   aad_offset       -
 * @param[in]   out          -
 * @return      none
 * @note
  @verbatim
      -# 1. *aad_offset will be the necessary byte length of add head, to build B1 block.
      -# 2.this function is common part for ske_lp_ccm_init() and ske_lp_dma_ccm_init().
  @endverbatim
 */
void ske_lp_ccm_get_B1(unsigned char *aad, unsigned int aad_bytes, unsigned int *aad_offset, unsigned char out[16]);

/**
 * @brief       ske_lp dma ccm mode update aad(multiple steps style).
 * @param[in]   ctx              - SKE_CCM_CTX context pointer.
 * @param[in]   aad              - aad.
 * @param[in]   bytes            - byte length of aad.
 * @param[in]   callback         - callback function pointer, this could be NULL, means do nothing.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.this is for multiple steps style.
      -# 2.if there is no aad, this function could be omitted.
      -# 3.this function must be called after calling ske_lp_dma_ccm_update_B0_block().
      -# 4.the whole aad here must be with prefix(in B1), and padding the tail with 0 if necessary(to
        make it occupies a multiple of 16 bytes). and now whole aad byte length is prefix byte length
        (ctx->b1_aad_start_offset) + original aad byte length.
      -# 5.if the whole aad with prefix is too long, you could divide it by block(16 bytes), and if the
         whole aad byte length is not a multiple of 16, please make sure the last section contains the
         tail, then call this function to input the sections respectively. for example, if the whole
         aad byte length is 65, it could be divided into 3 sections with byte length 48,16,1 respectively.
  @endverbatim
 */
unsigned int ske_lp_dma_ccm_update_aad_blocks(SKE_CCM_CTX *ctx, unsigned int *aad, unsigned int bytes, SKE_CALLBACK callback);

/**
 * @brief       ske_lp dma ccm mode input plaintext/ciphertext, get ciphertext/plaintext or
 *              ciphertext/plaintext+mac(multiple steps style).
 * @param[in]   ctx              - SKE_CCM_CTX context pointer.
 * @param[in]   in               - plaintext/ciphertext.
 * @param[in]   out              - ciphertext/plaintext or ciphertext/plaintext+macs.
 * @param[in]   in_bytes         - byte length of in.
 * @param[in]   callback         - callback function pointer, this could be NULL, means do nothing.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.this function must be called after calling ske_lp_dma_ccm_update_aad_blocks().
      -# 2.the whole plaintext/ciphertext must be some blocks, if not, please pad it with 0.
      -# 3.f the whole plaintext/ciphertext is too long, you could divide it by block(16 bytes), and if
        the whole plaintext/ciphertext byte length is not a multiple of 16, please make sure the last
        section contains the tail, then call this function to input the sections respectively. for
        example, if the whole plaintext/ciphertext byte length is 65, it could be divided into 3 sections
        with byte length 48,16,1 respectively.
      -# 4.the output will be some blocks too, it has the same length as the input, and with padding 0 if
         necessary.
      -# 5.if input contains the tail, then output will be ciphertext/plaintext+mac.
      -# 6.to save memory, in and out could be the same buffer, in this case, the output will
       cover the input.
  @endverbatim
 */
unsigned int ske_lp_dma_ccm_update_blocks(SKE_CCM_CTX *ctx, unsigned int *in, unsigned int *out, unsigned int in_bytes,
        SKE_CALLBACK callback);

/**
 * @brief       ske_lp dma ccm mode finish.
 * @param[in]   ctx              - SKE_CCM_CTX context pointer.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.this function is optional.
  @endverbatim
 */
unsigned int ske_lp_dma_ccm_final(SKE_CCM_CTX *ctx);

/**
 * @brief       ske_lp ccm mode encrypt/decrypt(one-off style).
 * @param[in]   alg              - ske_lp algorithm.
 * @param[in]   crypto           - encrypting or decrypting.
 * @param[in]   key              - key in bytes, key of AES(128/192/256) or SM4.
 * @param[in]   sp_key_idx       - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX].
 * @param[in]   nonce            - nonce in bytes, its byte length is 15-L.
 * @param[in]   M                - bytes of authentication field(bytes of mac).
 * @param[in]   L                -  bytes of length field(message byte length is less than 256^L)
 * @param[in]   aad_bytes        - byte length of aad, it could be any value, including 0.
 * @param[in]   in               - plaintext or ciphertext.
 * @param[out]  out              - ciphertext or plaintext.
 * @param[in]   c_bytes          - byte length of plaintext/ciphertext, it could be any value, including 0.
 * @param[in]   callback         - callback function pointer, this could be NULL, means do nothing.
 *
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.this function is for DMA style.
      -# 2.only AES(128/192/256) and SM4 are supported for GCM mode.
      -# 3.if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
         otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX].
      -# 4.valid M is {4,6,8,10,12,14,16}, and valid L is {2,3,4,5,6,7,8}.
      -# 5.aad_bytes and c_bytes could not be zero at the same time due to hardware implementation.
      -# 6.if aad exists, it must be some blocks, if not, please pad it with 0
        cover the input.
      -# 7.the output ciphertext/plaintext has the same number of blocks as the input plaintext/ciphertext,
         and followed by one block, it is mac with padding 0 if necessary, so is the second last block if
         necessary(ciphertext/plaintext).
      -# 8.for encryption, mac is output; and for decryption, mac is input, if returns SKE_SUCCESS
         that means certification passed, otherwise not.
      -# 9.please make sure B0+aad+plaintext/ciphertext is integral.
      -# 10.to save memory, in and out could be the same buffer, in this case, the output will
        cover the input.
  @endverbatim
 */
unsigned int ske_lp_dma_ccm_crypto(SKE_ALG alg, SKE_CRYPTO crypto, unsigned char *key, unsigned short sp_key_idx, unsigned char *nonce,
        unsigned char M, unsigned char L, unsigned int aad_bytes, unsigned int *in, unsigned int *out, unsigned int c_bytes, SKE_CALLBACK callback);
#endif






#ifdef __cplusplus
}
#endif

#endif
