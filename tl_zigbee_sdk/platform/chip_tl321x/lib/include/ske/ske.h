/********************************************************************************************************
 * @file    ske.h
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
#ifndef SKE_H
#define SKE_H


#include "ske_basic.h"

#ifdef __cplusplus
extern "C" {
#endif




//APIs
/**
 * @brief       a=a+b.
 * @param[in]   a            - big integer a in bytes, big-endian.
 * @param[in]   a_bytes      - byte length of a.
 * @param[in]   b            - b.
 * @return      none
 * @note
  @verbatim
      -# 1.for CTR/CCM counter addition(big-endian).
  @endverbatim
 */
void ske_lp_big_endian_add_uint8(unsigned char *a, unsigned int a_bytes, unsigned char b);

/**
 * @brief       get key byte length for specific ske_lp alg.
 * @param[in]   a                   - ske_lp algorithm.
 * @param[in]   a_words             - word length of buffer a.
 * @param[in]   b                   - b
 * @return      key byte length for ske_lp alg
 * * @note
  @verbatim
      -# 1. please make sure the inputs are valid.
  @endverbatim
 */
void ske_lp_little_endian_add_uint32(unsigned int *a, unsigned int a_words, unsigned int b);

/**
 * @brief       get key byte length for specific ske_lp alg.
 * @param[in]   ske_alg              - ske_lp algorithm.
 * @return      key byte length for ske_lp alg
 * * @note
  @verbatim
      -# 1. please make sure the inputs are valid.
  @endverbatim
 */
unsigned char ske_lp_get_key_byte_len(SKE_ALG ske_alg);

/**
 * @brief       get block byte length for specific ske_lp alg.
 * @param[in]   ske_alg              - ske_lp algorithm.
 * @return      block byte length for ske_lp alg
 * @note
  @verbatim
      -# 1. please make sure ske_alg is valid.
  @endverbatim
 */
unsigned char ske_lp_get_block_byte_len(SKE_ALG ske_alg);

/**
 * @brief       set ske_lp iv.
 * @param[in]   iv               - initial vector.
 * @param[in]   block_bytes      - byte length of current ske_lp block.
 * @return      none
 * @note
  @verbatim
      -# 1. please make sure ske_alg is valid.
  @endverbatim
 */
void ske_lp_set_iv(unsigned char *iv, unsigned int block_bytes);

/**
 * @brief       ske_lp setting key
 * @param[in]   alg              - ske_lp algorithm.
 * @param[in]   key              - key in word buffer.
 * @param[in]   key_bytes        - key bytes.
 * @param[in]   key_idx          - key id.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.this function is common for CPU/DMA/DMA-LL.
      -# 2.if mode is ECB, then there is no iv, in this case iv could be NULL.
      -# 3.if mode is CMAC/CBC-MAC, the iv must be a block of all zero.
      -# 4.if key is from user input, please make sure the argument key is not NULL(now sp_key_idx is useless),
        otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX].
  @endverbatim
 */
void ske_lp_set_key(SKE_ALG alg, const unsigned char *key, unsigned short key_bytes, unsigned short key_idx);


unsigned int ske_lp_update_blocks_internal_(SKE_CTX *ctx, unsigned char *in, unsigned char *out, unsigned int bytes);

unsigned int ske_lp_update_blocks_no_output_(SKE_CTX *ctx, unsigned char *in, unsigned int bytes);

/**
 * @brief       ske_lp init config
 * @param[in]   ctx              - SKE_CTX context pointer.
 * @param[in]   alg              - ske_lp algorithm.
 * @param[in]   mode             - ske_lp algorithm operation mode, like ECB,CBC,OFB,etc.
 * @param[in]   crypto           - encrypting or decrypting.
 * @param[in]   key              - key in bytes.
 * @param[in]   sp_key_idx       - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX],
 *                                 if the MSB(sp_key_idx) is 1, that means using low 128bit of the 256bit key.
 * @param[in]   iv               - iv in bytes, must be a block.
 * @param[in]   dma_en           - for DMA mode(not 0) or not(0).
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.this function is common for CPU/DMA/DMA-LL.
      -# 2.if mode is ECB, then there is no iv, in this case iv could be NULL.
      -# 3.if mode is CMAC/CBC-MAC, the iv must be a block of all zero.
      -# 4.if key is from user input, please make sure the argument key is not NULL(now sp_key_idx is useless),
        otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX].
  @endverbatim
 */
unsigned int ske_lp_init_internal(SKE_CTX *ctx, SKE_ALG alg, SKE_MODE mode, SKE_CRYPTO crypto, const unsigned char *key,
        unsigned short sp_key_idx, unsigned char *iv , unsigned int dma_en);

/**
 * @brief       ske_lp init config(CPU style)
 * @param[in]   alg              - ske_lp algorithm.
 * @param[in]   mode             - ske_lp algorithm operation mode, like ECB,CBC,OFB,etc.
 * @param[in]   crypto           - encrypting or decrypting.
 * @param[in]   key              - key in bytes.
 * @param[in]   sp_key_idx       - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX],
 *                                 if the MSB(sp_key_idx) is 1, that means using low 128bit of the 256bit key.
 * @param[in]   iv               - iv in bytes, must be a block.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.if mode is ECB, then there is no iv, in this case iv could be NULL.
      -# 2.this function is designed for ECB/CBC/CFB/OFB/CTR/XTS modes, and input/output unit must be a block.
      -# 3.if key is from user input, please make sure the argument key is not NULL(now sp_key_idx is useless),
        otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX].
  @endverbatim
 */
unsigned int ske_lp_init(SKE_ALG alg, SKE_MODE mode, SKE_CRYPTO crypto, unsigned char *key, unsigned short sp_key_idx, unsigned char *iv);

/**
 * @brief       ske 3des encryption or decryption(CPU style)
 * @param[in]   in               - plaintext or ciphertext.
 * @param[out]  out              - ciphertext or plaintext.
 * @param[in]   bytes            - byte length of input or output.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.this function is designed for ECB/CBC/CFB/OFB/CTR modes, and input/output unit must be a block.
      -# 2.to save memory, in and out could be the same buffer, in this case, the output will
        cover the input.
      -# 3.bytes must be a multiple of block byte length.
  @endverbatim
 */
unsigned int ske_lp_update_blocks(unsigned char *in, unsigned char *out, unsigned int bytes);

/**
 * @brief       ske_lp finish.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.if encryption or decryption is done, please call this(optional).
  @endverbatim
 */
unsigned int ske_lp_final(void);

/**
 * @brief       ske_lp encrypting or decrypting(CPU style, one-off style)
 * @param[in]   alg              - ske_lp algorithm.
 * @param[in]   mode             - ske_lp algorithm operation mode, like ECB,CBC,OFB,etc.
 * @param[in]   crypto           - encrypting or decrypting.
 * @param[in]   key              - key in bytes.
 * @param[in]   sp_key_idx       - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX],
 *                                 if the MSB(sp_key_idx) is 1, that means using low 128bit of the 256bit key.
 * @param[in]   iv               - iv in bytes, must be a block.
 * @param[in]   in               - plaintext or ciphertext.
 * @param[in]   out              - ciphertext or plaintext.
 * @param[in]   bytes            - byte length of input or output.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.if mode is ECB, then there is no iv, in this case iv could be NULL.
      -# 2.this function is designed for ECB/CBC/CFB/OFB/CTR/XTS modes, and input/output unit is a block.
      -# 3.if key is from user input, please make sure the argument key is not NULL(now sp_key_idx is useless),
        otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX].
      -# 4.ito save memory, in and out could be the same buffer, in this case, the output will
        cover the input.
      -# 5.bytes must be a multiple of block byte length.
  @endverbatim
 */
unsigned int ske_lp_crypto(SKE_ALG alg, SKE_MODE mode, SKE_CRYPTO crypto, unsigned char *key, unsigned short sp_key_idx, unsigned char *iv,
        unsigned char *in, unsigned char *out, unsigned int bytes);


#ifdef SKE_LP_DMA_FUNCTION
/**
 * @brief       ske_lp init config(DMA style)
 * @param[in]   alg              - ske_lp algorithm.
 * @param[in]   mode             - ske_lp algorithm operation mode, like ECB,CBC,OFB,etc.
 * @param[in]   crypto           - encrypting or decrypting.
 * @param[in]   key              - key in bytes.
 * @param[in]   sp_key_idx       - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX],
 *                                 if the MSB(sp_key_idx) is 1, that means using low 128bit of the 256bit key.
 * @param[in]   iv               - iv in bytes, must be a block.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.if mode is ECB, then there is no iv, in this case iv could be NULL.
      -# 2.this function is designed for ECB/CBC/CFB/OFB/CTR/XTS modes, and input/output unit is a block.
      -# 3.if key is from user input, please make sure the argument key is not NULL(now sp_key_idx is useless),
        otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX].
  @endverbatim
 */
unsigned int ske_lp_dma_init(SKE_ALG alg, SKE_MODE mode, SKE_CRYPTO crypto, unsigned char *key, unsigned short sp_key_idx, unsigned char *iv);

/**
 * @brief       ske_lp encryption or decryption(DMA style)
 * @param[in]   in               - plaintext or ciphertext.
 * @param[out]  out              - ciphertext or plaintext.
 * @param[in]   words            - word length of input or output, must be multiples of block length.
 * @param[in]   callback         - callback function pointer, this could be NULL, means doing nothing.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.this function is designed for ECB/CBC/CFB/OFB/CTR/XTS modes, and input/output unit is a block.
      -# 2.to save memory, in and out could be the same buffer, in this case, the output will
        cover the input.
      -# 3.words must be a multiple of block word length.
  @endverbatim
 */
unsigned int ske_lp_dma_update_blocks(unsigned int *in, unsigned int *out, unsigned int words, SKE_CALLBACK callback);

/**
 * @brief       ske_lp finish(DMA style)
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.if encryption or decryption is done, please call this(optional).
  @endverbatim
 */
unsigned int ske_lp_dma_final(void);

/**
 * @brief       ske_lp encryption or decryption(DMA style)
 * @param[in]   alg              - ske_lp algorithm.
 * @param[in]   mode             - ske_lp algorithm operation mode, like ECB,CBC,OFB,etc.
 * @param[in]   crypto           - encrypting or decrypting.
 * @param[in]   key              - key in bytes.
 * @param[in]   sp_key_idx       - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX],
 *                                 if the MSB(sp_key_idx) is 1, that means using low 128bit of the 256bit key.
 * @param[in]   iv               - iv in bytes, must be a block.
 * @param[in]   in               - plaintext or ciphertext.
 * @param[out]  out              - ciphertext or plaintext.
 * @param[in]   words            - word length of input or output.
 * @param[in]   callback         - callback function pointer, this could be NULL, means do nothing.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.if mode is ECB, then there is no iv, in this case iv could be NULL.
      -# 2.this function is designed for ECB/CBC/CFB/OFB/CTR/XTS modes, and input/output unit is a block.
      -# 3.if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
        otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX].
      -# 4.to save memory, in and out could be the same buffer, in this case, the output will
        cover the input.
      -# 5.words must be a multiple of block word length.
  @endverbatim
 */
unsigned int ske_lp_dma_crypto(SKE_ALG alg, SKE_MODE mode, SKE_CRYPTO crypto, unsigned char *key, unsigned short sp_key_idx, unsigned char *iv,
        unsigned int *in, unsigned int *out, unsigned int words, SKE_CALLBACK callback);
#endif




#ifdef __cplusplus
}
#endif

#endif
