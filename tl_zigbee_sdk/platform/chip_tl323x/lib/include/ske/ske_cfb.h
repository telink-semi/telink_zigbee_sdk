/********************************************************************************************************
 * @file    ske_cfb.h
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
#ifndef SKE_CFB_H
#define SKE_CFB_H


#include "ske.h"


#ifdef __cplusplus
extern "C"
{
#endif


    /**
 * @brief           APIs
 */

    /**
 * @brief           Initializes CFB mode configuration
 * @param[in]       alg                  - SKE algorithm
 * @param[in]       crypto               - Encrypt or decrypt operation
 * @param[in]       key                  - Key data
 * @param[in]       sp_key_idx           - Secure port key index
 * @param[in]       iv                   - Initialization vector
 * @return          SKE_SUCCESS if successful, otherwise error code
 */
    unsigned int ske_lp_cfb_init(ske_alg_e alg, ske_crypto_e crypto, const unsigned char *key, unsigned short sp_key_idx, unsigned char *iv);

    /**
 * @brief           Updates multiple blocks in CFB mode
 * @param[in]       in                   - Input data
 * @param[out]      out                  - Output data
 * @param[in]       bytes                - Data length in bytes
 * @return          SKE_SUCCESS if successful, otherwise error code
 */
    unsigned int ske_lp_cfb_update_blocks(unsigned char *in, unsigned char *out, unsigned int bytes);

    /**
 * @brief           Finalizes CFB mode operation
 * @return          SKE_SUCCESS if successful, otherwise error code
 */
    unsigned int ske_lp_cfb_final(void);

    /**
 * @brief           Performs one-shot encryption/decryption in CFB mode
 * @param[in]       alg                  - SKE algorithm
 * @param[in]       crypto               - Encrypt or decrypt operation
 * @param[in]       key                  - Key data
 * @param[in]       sp_key_idx           - Secure port key index
 * @param[in]       iv                   - Initialization vector
 * @param[in]       in                   - Input data
 * @param[out]      out                  - Output data
 * @param[in]       bytes                - Data length in bytes
 * @return          SKE_SUCCESS if successful, otherwise error code
 */
    unsigned int ske_lp_cfb_crypto(ske_alg_e alg, ske_crypto_e crypto, const unsigned char *key, unsigned short sp_key_idx, unsigned char *iv, unsigned char *in,
                                   unsigned char *out, unsigned int bytes);


#ifdef SKE_LP_DMA_FUNCTION
    /**
 * @brief           Initializes CFB mode configuration for DMA
 * @param[in]       alg                  - SKE algorithm
 * @param[in]       crypto               - Encrypt or decrypt operation
 * @param[in]       key                  - Key data
 * @param[in]       sp_key_idx           - Secure port key index
 * @param[in]       iv                   - Initialization vector
 * @return          SKE_SUCCESS if successful, otherwise error code
 */
    unsigned int ske_lp_dma_cfb_init(ske_alg_e alg, ske_crypto_e crypto, const unsigned char *key, unsigned short sp_key_idx, unsigned char *iv);

    /**
 * @brief           Updates multiple blocks in CFB mode using DMA
 * @param[in]       in                   - Input data
 * @param[out]      out                  - Output data
 * @param[in]       words                - Data length in words
 * @param[in]       callback             - Callback function pointer
 * @return          SKE_SUCCESS if successful, otherwise error code
 */
    unsigned int ske_lp_dma_cfb_update_blocks(unsigned int *in, unsigned int *out, unsigned int words, SKE_CALLBACK callback);

    /**
 * @brief           Finalizes CFB mode operation for DMA
 * @return          SKE_SUCCESS if successful, otherwise error code
 */
    unsigned int ske_lp_dma_cfb_final(void);

    /**
 * @brief           Performs one-shot encryption/decryption in CFB mode using DMA
 * @param[in]       alg                  - SKE algorithm
 * @param[in]       crypto               - Encrypt or decrypt operation
 * @param[in]       key                  - Key data
 * @param[in]       sp_key_idx           - Secure port key index
 * @param[in]       iv                   - Initialization vector
 * @param[in]       in                   - Input data
 * @param[out]      out                  - Output data
 * @param[in]       words                - Data length in words
 * @param[in]       callback             - Callback function pointer
 * @return          SKE_SUCCESS if successful, otherwise error code
 */
    unsigned int ske_lp_dma_cfb_crypto(ske_alg_e alg, ske_crypto_e crypto, const unsigned char *key, unsigned short sp_key_idx, unsigned char *iv, unsigned int *in,
                                       unsigned int *out, unsigned int words, SKE_CALLBACK callback);
#endif


#ifdef __cplusplus
}
#endif

#endif
