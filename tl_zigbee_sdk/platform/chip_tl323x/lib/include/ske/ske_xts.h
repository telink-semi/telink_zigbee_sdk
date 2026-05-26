/********************************************************************************************************
 * @file    ske_xts.h
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
#ifndef SKE_XTS_H
#define SKE_XTS_H



#include "ske.h"


#ifdef __cplusplus
extern "C" {
#endif


    typedef struct
    {
        unsigned char t[16];
        ske_crypto_e  crypto;
        ske_ctx_t     ske_xts_ctx[1];
        unsigned int  c_bytes;
        unsigned int  current_bytes;
    } ske_xts_ctx_t;

    typedef ske_xts_ctx_t SKE_XTS_CTX;

    /**
 * @brief           APIs
 */

    /**
 * @brief           Initializes SKE XTS mode configuration
 * @param[in]       ctx                  - ske_xts_ctx_t context pointer
 * @param[in]       alg                  - SKE algorithm
 * @param[in]       crypto               - Encrypt or decrypt operation
 * @param[in]       key                  - Key in bytes, key = key1||key2
 * @param[in]       sp_key_idx           - Index of secure port key
 * @param[in]       i                    - I value, same length as block length
 * @param[in]       c_bytes              - Byte length of plaintext/ciphertext
 * @return          SKE_SUCCESS if successful, otherwise error code
 * @note            
 *        1. If key is from user input, ensure key is not NULL
 *        2. Key consists of key1 and key2
 *        3. c_bytes cannot be less than block byte length
 */
    unsigned int ske_lp_xts_init(ske_xts_ctx_t *ctx, ske_alg_e alg, ske_crypto_e crypto, const unsigned char *key, unsigned short sp_key_idx, unsigned char *i,
                                 unsigned int c_bytes);

    /**
 * @brief           Updates multiple blocks in SKE XTS mode
 * @param[in]       ctx                  - ske_xts_ctx_t context pointer
 * @param[in]       in                   - Input data
 * @param[out]      out                  - Output data
 * @param[in]       bytes                - Byte length of input/output data
 * @return          SKE_SUCCESS if successful, otherwise error code
 * @note            
 *        1. in and out can be the same buffer
 *        2. bytes must be a multiple of block byte length
 *        3. For partial blocks, use ske_lp_xts_update_including_last_2_blocks()
 */
    unsigned int ske_lp_xts_update_blocks(ske_xts_ctx_t *ctx, unsigned char *in, unsigned char *out, unsigned int bytes);

    /**
 * @brief           Updates including last 2 blocks in SKE XTS mode
 * @param[in]       ctx                  - ske_xts_ctx_t context pointer
 * @param[in]       in                   - Input data
 * @param[out]      out                  - Output data
 * @param[in]       bytes                - Byte length of input/output data
 * @return          SKE_SUCCESS if successful, otherwise error code
 * @note            
 *        1. in and out can be the same buffer
 *        2. Handles cases where ctx->c_bytes % 16 is not 0
 */
    unsigned int ske_lp_xts_update_including_last_2_blocks(ske_xts_ctx_t *ctx, unsigned char *in, unsigned char *out, unsigned int bytes);

    /**
 * @brief           Finalizes SKE XTS mode operation
 * @param[in]       ctx                  - ske_xts_ctx_t context pointer
 * @return          SKE_SUCCESS if successful, otherwise error code
 * @note            This is the last step of XTS calling, and it is optional
 */
    unsigned int ske_lp_xts_final(ske_xts_ctx_t *ctx);

    /**
 * @brief           Performs SKE XTS mode encryption or decryption
 * @param[in]       alg                  - SKE algorithm
 * @param[in]       crypto               - Encrypt or decrypt operation
 * @param[in]       key                  - Key in bytes, key = key1||key2
 * @param[in]       sp_key_idx           - Index of secure port key
 * @param[in]       i                    - I value, same length as block length
 * @param[in]       in                   - Input data
 * @param[out]      out                  - Output data
 * @param[in]       c_bytes              - Byte length of input/output data
 * @return          SKE_SUCCESS if successful, otherwise error code
 * @note            
 *        1. If key is from user input, ensure key is not NULL
 *        2. Key consists of key1 and key2
 *        3. c_bytes cannot be less than block byte length
 */
    unsigned int ske_lp_xts_crypto(ske_alg_e alg, ske_crypto_e crypto, const unsigned char *key, unsigned short sp_key_idx, unsigned char *i, unsigned char *in, unsigned char *out,
                                   unsigned int bytes);


#ifdef __cplusplus
}
#endif

#endif
