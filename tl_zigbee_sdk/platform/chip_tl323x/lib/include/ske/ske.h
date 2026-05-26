/********************************************************************************************************
 * @file    ske.h
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
#ifndef SKE_H
#define SKE_H


#include "ske_basic.h"

#ifdef __cplusplus
extern "C"
{
#endif


    //APIs

    /**
 * @brief           Gets the key byte length for a specific SKE algorithm
 * @param[in]       ske_alg              - SKE algorithm
 * @return          Key byte length for the algorithm
 */
    unsigned char ske_lp_get_key_byte_len(ske_alg_e ske_alg);

    /**
 * @brief           Gets the block byte length for a specific SKE algorithm
 * @param[in]       ske_alg              - SKE algorithm
 * @return          Block byte length for the algorithm
 */
    unsigned char ske_lp_get_block_byte_len(ske_alg_e ske_alg);

    /**
 * @brief           Sets the Initialization Vector (IV) for SKE
 * @param[in]       iv                   - Pointer to the IV data
 * @param[in]       block_bytes          - Byte length of the IV
 * @return          None
 */
    void ske_lp_set_iv(const unsigned char *iv, unsigned int block_bytes);

    /**
 * @brief           Sets the key for SKE
 * @param[in]       alg                  - SKE algorithm
 * @param[in]       key                  - Pointer to the key data
 * @param[in]       key_bytes            - Byte length of the key
 * @param[in]       key_idx              - Key index (1 or 2)
 * @return          None
 */
    void ske_lp_set_key(ske_alg_e alg, const unsigned char *key, unsigned short key_bytes, unsigned short key_idx);

    /**
 * @brief           Initializes SKE configuration
 * @param[in]       ctx                  - Pointer to SKE context
 * @param[in]       alg                  - SKE algorithm
 * @param[in]       mode                 - SKE operation mode
 * @param[in]       crypto               - Encrypt or decrypt operation
 * @param[in]       key                  - Pointer to the key data
 * @param[in]       sp_key_idx           - Secure port key index
 * @param[in]       iv                   - Pointer to the IV data
 * @param[in]       dma_en               - DMA mode enable flag
 * @return          SKE_SUCCESS if successful, otherwise error code
 */
    unsigned int ske_lp_init_internal(ske_ctx_t *ctx, ske_alg_e alg, ske_mode_e mode, ske_crypto_e crypto, const unsigned char *key, unsigned short sp_key_idx,
                                      const unsigned char *iv, unsigned int dma_en);

    /**
 * @brief           Initializes SKE configuration (CPU style)
 * @param[in]       alg                  - SKE algorithm
 * @param[in]       mode                 - SKE operation mode
 * @param[in]       crypto               - Encrypt or decrypt operation
 * @param[in]       key                  - Pointer to the key data
 * @param[in]       sp_key_idx           - Secure port key index
 * @param[in]       iv                   - Pointer to the IV data
 * @return          SKE_SUCCESS if successful, otherwise error code
 */
    unsigned int ske_lp_init(ske_alg_e alg, ske_mode_e mode, ske_crypto_e crypto, const unsigned char *key, unsigned short sp_key_idx, unsigned char *iv);

    /**
 * @brief           Performs SKE encryption or decryption (CPU style)
 * @param[in]       in                   - Pointer to input data
 * @param[out]      out                  - Pointer to output data
 * @param[in]       bytes                - Byte length of input/output data
 * @return          SKE_SUCCESS if successful, otherwise error code
 */
    unsigned int ske_lp_update_blocks(unsigned char *in, unsigned char *out, unsigned int bytes);

    /**
 * @brief           Finalizes SKE operation
 * @return          SKE_SUCCESS if successful, otherwise error code
 */
    unsigned int ske_lp_final(void);

    /**
 * @brief           Performs SKE encryption or decryption (CPU style, one-off)
 * @param[in]       alg                  - SKE algorithm
 * @param[in]       mode                 - SKE operation mode (ECB, CBC, OFB, etc.)
 * @param[in]       crypto               - Encrypt or decrypt operation
 * @param[in]       key                  - Key in bytes
 * @param[in]       sp_key_idx           - Secure port key index
 * @param[in]       iv                   - IV in bytes (must be a block)
 * @param[in]       in                   - Pointer to input data (plaintext or ciphertext)
 * @param[out]      out                  - Pointer to output data (ciphertext or plaintext)
 * @param[in]       bytes                - Byte length of input/output data
 * @return          SKE_SUCCESS if successful, otherwise error code
 * @note            
 *        1. If mode is ECB, then there is no IV (iv can be NULL)
 *        2. Designed for ECB/CBC/CFB/OFB/CTR/XTS modes (input/output unit is a block)
 *        3. If key is from user input, ensure key is not NULL (sp_key_idx is unused)
 *        4. in and out can be the same buffer (output will overwrite input)
 *        5. bytes must be a multiple of block byte length
 */
    unsigned int ske_lp_crypto(ske_alg_e alg, ske_mode_e mode, ske_crypto_e crypto, const unsigned char *key, unsigned short sp_key_idx, unsigned char *iv, unsigned char *in,
                               unsigned char *out, unsigned int bytes);


#ifdef SKE_LP_DMA_FUNCTION
    /**
 * @brief           Initializes SKE configuration (DMA style)
 * @param[in]       alg                  - ske_lp algorithm
 * @param[in]       mode                 - ske_lp algorithm operation mode, like ECB,CBC,OFB,etc.
 * @param[in]       crypto               - encrypting or decrypting
 * @param[in]       key                  - key in bytes, must be a block
 * @param[in]       sp_key_idx           - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX],
 *                                         if the MSB(sp_key_idx) is 1, that means using low 128bit of the 256bit key
 * @param[in]       iv                   - iv in bytes
 * @return          SKE_SUCCESS if successful, otherwise error code
 * @note            
 *        1. if mode is ECB, then there is no iv, in this case iv could be NULL
 *        2. this function is designed for ECB/CBC/CFB/OFB/CTR/XTS modes, and input/output unit is a block
 *        3. if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
 *           otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX]
 */
    unsigned int ske_lp_dma_init(ske_alg_e alg, ske_mode_e mode, ske_crypto_e crypto, const unsigned char *key, unsigned short sp_key_idx, const unsigned char *iv);

    /**
 * @brief           Performs SKE encryption or decryption (DMA style)
 * @param[in]       in                   - Pointer to input data (plaintext or ciphertext)
 * @param[out]      out                  - Pointer to output data (ciphertext or plaintext)
 * @param[in]       words                - Word length of input/output data (must be multiples of block length)
 * @param[in]       callback             - Callback function pointer (can be NULL)
 * @return          SKE_SUCCESS if successful, otherwise error code
 * @note            
 *        1. Designed for ECB/CBC/CFB/OFB/CTR/XTS modes, input/output unit is a block
 *        2. in and out can be the same buffer (output will overwrite input)
 *        3. words must be a multiple of block word length
 */
    unsigned int ske_lp_dma_update_blocks(const unsigned int *in, unsigned int *out, unsigned int words, SKE_CALLBACK callback);

    /**
 * @brief           Finalizes SKE operation (DMA style)
 * @return          SKE_SUCCESS if successful, otherwise error code
 * @note            If encryption or decryption is done, please call this (optional)
 */
    unsigned int ske_lp_dma_final(void);

    /**
 * @brief           Performs SKE encryption or decryption (DMA style, one-off)
 * @param[in]       alg                  - SKE algorithm
 * @param[in]       mode                 - SKE operation mode (ECB, CBC, OFB, etc.)
 * @param[in]       crypto               - Encrypt or decrypt operation
 * @param[in]       key                  - Key in bytes
 * @param[in]       sp_key_idx           - Secure port key index
 * @param[in]       iv                   - IV in bytes (must be a block)
 * @param[in]       in                   - Pointer to input data (plaintext or ciphertext)
 * @param[out]      out                  - Pointer to output data (ciphertext or plaintext)
 * @param[in]       words                - Word length of input/output data
 * @param[in]       callback             - Callback function pointer (can be NULL)
 * @return          SKE_SUCCESS if successful, otherwise error code
 * @note            
 *        1. If mode is ECB, then there is no IV (iv can be NULL)
 *        2. Designed for ECB/CBC/CFB/OFB/CTR/XTS modes (input/output unit is a block)
 *        3. If key is from user input, ensure key is not NULL (sp_key_idx is unused)
 *        4. in and out can be the same buffer (output will overwrite input)
 *        5. words must be a multiple of block word length
 */
    unsigned int ske_lp_dma_crypto(ske_alg_e alg, ske_mode_e mode, ske_crypto_e crypto, const unsigned char *key, unsigned short sp_key_idx, const unsigned char *iv,
                                   const unsigned int *in, unsigned int *out, unsigned int words, SKE_CALLBACK callback);
#endif


#ifdef __cplusplus
}
#endif

#endif
