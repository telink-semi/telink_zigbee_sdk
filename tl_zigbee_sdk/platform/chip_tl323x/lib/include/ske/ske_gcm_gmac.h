/********************************************************************************************************
 * @file    ske_gcm_gmac.h
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
#ifndef SKE_GCM_H
#define SKE_GCM_H


#include "ske.h"


#ifdef __cplusplus
extern "C"
{
#endif


#define SKE_LP_GCM_CPU_UPDATE_AAD_BY_STEP


#define SKE_LP_GCM_MAX_BYTES (16)

    typedef struct
    {
        unsigned char buf[16];
        ske_ctx_t     ske_gcm_ctx[1];
        ske_crypto_e  crypto;
        unsigned int  aad_bytes;
        unsigned int  c_bytes;
        unsigned int  mac_bytes;
        unsigned int  current_bytes;
    } ske_gcm_ctx_t;

    typedef ske_gcm_ctx_t SKE_GCM_CTX;

    typedef struct
    {
        ske_gcm_ctx_t gcm_ctx[1];
        ske_mac_e     mac_action;
    } ske_gmac_ctx_t;

    typedef ske_gmac_ctx_t SKE_GMAC_CTX;

    /**
 * @brief           APIs
 */

    /**
 * @brief           ske_lp gcm mode init config(CPU style)
 * @param[in]       ctx                  - ske_gcm_ctx_t context pointer
 * @param[in]       alg                  - ske_lp algorithm
 * @param[in]       crypto               - encrypting or decrypting
 * @param[in]       key                  - key in bytes, key of AES(128/192/256) or SM4
 * @param[in]       sp_key_idx           - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX]
 * @param[in]       iv                   - iv in bytes
 * @param[in]       iv_bytes             - byte length of iv, now only 12 bytes is supported
 * @param[in]       aad_bytes            - byte length of aad, it could be any value, including 0
 * @param[in]       c_bytes              - byte length of plaintext/ciphertext, it could be any value, including 0
 * @param[in]       mac_bytes            - byte length of mac, must be in [0,16]
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function is for CPU style
 *        2. only AES(128/192/256) and SM4 are supported for GCM mode
 *        3. if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
 *           otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX]
 *        4. iv must be 12 bytes here
 *        5. aad_bytes and c_bytes could not be zero at the same tim
 */
    unsigned int ske_lp_gcm_init(ske_gcm_ctx_t *ctx, ske_alg_e alg, ske_crypto_e crypto, const unsigned char *key, unsigned short sp_key_idx, unsigned char *iv,
                                 unsigned int iv_bytes, unsigned int aad_bytes, unsigned int c_bytes, unsigned int mac_bytes);

    /**
 * @brief           ske_lp gcm mode input aad
 * @param[in]       ctx                  - ske_gcm_ctx_t context pointer
 * @param[in]       aad                  - aad
 * @param[in]       bytes                - real byte length of aad
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function must be called after calling ske_lp_gcm_init()
 *        2. if there is no aad, this function could be omitted
 *        3. if the whole aad is too long, you could divide it into some sections by byte, then call
 *           this function to input the sections respectively. for example, if the whole aad byte
 *           length is 65, it could be divided into 3 sections with byte length 10,47,8 respectively
 */
    unsigned int ske_lp_gcm_update_aad(ske_gcm_ctx_t *ctx, const unsigned char *aad, unsigned int bytes);

    /**
 * @brief           ske_lp gcm mode input aad(one-off style)
 * @param[in]       ctx                  - ske_gcm_ctx_t context pointer
 * @param[in]       aad                  - aad, its length is ctx->aad_bytes, please make sure
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function must be called after calling ske_lp_gcm_init()
 *        2. if there is no aad, this function could be omitted
 *        3. please make sure aad is integrality
 */
    unsigned int ske_lp_gcm_aad(ske_gcm_ctx_t *ctx, const unsigned char *aad);

    /**
 * @brief           ske_lp gcm mode input plaintext/ciphertext
 * @param[in]       ctx                  - ske_gcm_ctx_t context pointer
 * @param[in]       in                   - plaintext or ciphertext
 * @param[out]      out                  - ciphertext or plaintext
 * @param[in]       bytes                - byte length of input or output
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function must be called after calling ske_lp_gcm_aad()
 *        2. if there is no plaintext/ciphertext, this function could be omitted
 *        3. to save memory, in and out could be the same buffer, in this case, the output will
 *           cover the input.
 *        4. if the whole plaintext/ciphertext is too long, you could divide it by block(16 bytes),
 *           and if the whole plaintext/ciphertext byte length is not a multiple of 16, please make
 *           sure the last section contains the tail, then call this function to input the sections
 *           respectively. for example, if the whole plaintext/ciphertext byte length is 65, it
 *           could be divided into 3 sections with byte length 48,16,1 respectively
 */
    unsigned int ske_lp_gcm_update_blocks(ske_gcm_ctx_t *ctx, unsigned char *in, unsigned char *out, unsigned int bytes);

    /**
 * @brief           ske_lp gcm mode finish
 * @param[in]       ctx                  - ske_gcm_ctx_t context pointer
 * @param[in]       mac                  - input(for decryption), output(for encryption)
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function must be called after calling ske_lp_gcm_update()
 *        2. mac_bytes could be 0, but not bigger than SKE_LP_GCM_MAX_BYTES
 *        3. for encryption, mac is output; and for decryption, mac is input, if returns SKE_SUCCESS
 *           that means certification passed, otherwise not
 */
    unsigned int ske_lp_gcm_final(ske_gcm_ctx_t *ctx, unsigned char *mac);

    /**
 * @brief           ske_lp gcm mode encrypt/decrypt(one-off style)
 * @param[in]       alg                  - ske_lp algorithm
 * @param[in]       crypto               - encrypting or decrypting
 * @param[in]       key                  - key in bytes, key of AES(128/192/256) or SM4
 * @param[in]       sp_key_idx           - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX],
 * @param[in]       iv                   - iv in bytes
 * @param[in]       iv_bytes             - byte length of iv, now only 12 bytes supported
 * @param[in]       aad                  - aad, please make sure aad here is integral
 * @param[in]       aad_bytes            - byte length of aad, it could be any value, including 0
 * @param[in]       in                   - plaintext or ciphertext
 * @param[out]      out                  - ciphertext or plaintext
 * @param[in]       c_bytes              - byte length of plaintext/ciphertext, it could be any value,
 * @param[in]       mac                  - input(for decryption), output(for encryption)
 * @param[in]       mac_bytes            - byte length of mac.
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function is for CPU style
 *        2. only AES(128/192/256) and SM4 are supported for GCM mode
 *        3. if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
 *           otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX]
 *        4. iv must be 12 bytes here
 *        5. aad_bytes and c_bytes could not be zero at the same time
 *        6. to save memory, in and out could be the same buffer, in this case, the output will
 *           cover the input.
 *        7. mac_bytes could be 0, but not bigger than SKE_LP_GCM_MAX_BYTES
 *        8. for encryption, mac is output; and for decryption, mac is input, if returns SKE_SUCCESS
 *           that means certification passed, otherwise not
 */
    unsigned int ske_lp_gcm_crypto(ske_alg_e alg, ske_crypto_e crypto, const unsigned char *key, unsigned short sp_key_idx, unsigned char *iv, unsigned int iv_bytes,
                                   const unsigned char *aad, unsigned int aad_bytes, unsigned char *in, unsigned char *out, unsigned int c_bytes, unsigned char *mac,
                                   unsigned int mac_bytes);


#ifdef SKE_LP_DMA_FUNCTION
    /**
 * @brief           ske_lp dma gcm mode init config
 * @param[in]       ctx                  - ske_gcm_ctx_t context pointer
 * @param[in]       alg                  - ske_lp algorithm
 * @param[in]       crypto               - encrypting or decrypting
 * @param[in]       key                  - key in bytes, key of AES(128/192/256) or SM4
 * @param[in]       sp_key_idx           - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX],
 * @param[in]       iv                   - iv in bytes
 * @param[in]       iv_bytes             - byte length of iv, now only 12 bytes is supported
 * @param[in]       aad_bytes            - byte length of aad, it could be any value, including 0
 * @param[in]       c_bytes              - byte length of plaintext/ciphertext, it could be any value, including 0
 * @param[in]       mac_bytes            - byte length of mac, must be in [0,16]
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function is for DMA style
 *        2. only AES(128/192/256) and SM4 are supported for GCM mode
 *        3. if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
 *           otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX]
 *        4. iv must be 12 bytes here
 *        5. aad_bytes and c_bytes could not be zero at the same tim
 */
    unsigned int ske_lp_dma_gcm_init(ske_gcm_ctx_t *ctx, ske_alg_e alg, ske_crypto_e crypto, const unsigned char *key, unsigned short sp_key_idx, const unsigned char *iv,
                                     unsigned int iv_bytes, unsigned int aad_bytes, unsigned int c_bytes, unsigned int mac_bytes);

    /**
 * @brief           ske_lp dma gcm mode input aad
 * @param[in]       ctx                  - ske_gcm_ctx_t context pointer
 * @param[in]       aad                  - aad
 * @param[in]       bytes                - real byte length of aad
 * @param[in]       callback             - callback function pointer, this could be NULL, means doing nothing
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function must be called after calling ske_lp_dma_gcm_init()
 *        2. the whole aad must be some blocks, if not, please pad it with 0
 *        3. if the whole aad is too long, you could divide it by block(16 bytes), and if the whole aad byte
 *           length is not a multiple of 16, please make sure the last section contains the tail, then call
 *           this function to input the sections respectively. for example, if the whole aad byte length is 65,
 *           it could be divided into 3 sections with byte length 48,16,1 respectively
 */
    unsigned int ske_lp_dma_gcm_update_aad_blocks(ske_gcm_ctx_t *ctx, const unsigned int *aad, unsigned int bytes, SKE_CALLBACK callback);

    /**
 * @brief           ske_lp dma gcm mode input plaintext/ciphertext, get ciphertext/plaintext+mac
 * @param[in]       ctx                  - ske_gcm_ctx_t context pointer
 * @param[in]       in                   - plaintext/ciphertext
 * @param[out]      out                  - ciphertext/plaintext or ciphertext/plaintext+mac
 * @param[in]       in_bytes             - byte length of in
 * @param[in]       callback             - callback function pointer, this could be NULL, means doing nothing
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function must be called after calling ske_lp_dma_gcm_update_aad_blocks()
 *        2. the whole plaintext/ciphertext must be some blocks, if not, please pad it with 0
 *        3. if the whole plaintext/ciphertext is too long, you could divide it by block(16 bytes), and if
 *           the whole plaintext/ciphertext byte length is not a multiple of 16, please make sure the last
 *           section contains the tail, then call this function to input the sections respectively. for
 *           example, if the whole plaintext/ciphertext byte length is 65, it could be divided into 3 sections
 *           with byte length 48,16,1 respectively.
 *        4. the output will be some blocks too, it has the same length as the input, and with padding 0 if
 *           necessary
 *        5. if input contains the tail, then output will be ciphertext/plaintext+mac
 *        6. to save memory, in and out could be the same buffer, in this case, the output will
 *           cover the input
 */
    unsigned int ske_lp_dma_gcm_update_blocks(ske_gcm_ctx_t *ctx, unsigned int *in, unsigned int *out, unsigned int in_bytes, SKE_CALLBACK callback);

    /**
 * @brief           ske_lp dma gcm mode finish
 * @param[in]       ctx                  - ske_gcm_ctx_t context pointer
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function is optional
 */
    unsigned int ske_lp_dma_gcm_final(ske_gcm_ctx_t *ctx);

    /**
 * @brief           ske_lp dma gcm mode encrypt/decrypt(one-off style)
 * @param[in]       alg                  - ske_lp algorithm
 * @param[in]       crypto               - encrypting or decrypting
 * @param[in]       key                  - key in bytes, key of AES(128/192/256) or SM4
 * @param[in]       sp_key_idx           - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX],
 * @param[in]       iv                   - iv in bytes
 * @param[in]       iv_bytes             - byte length of iv, now only 12 bytes supported
 * @param[in]       aad                  - aad, please make sure aad here is integral
 * @param[in]       aad_bytes            - byte length of aad, it could be any value, including 0
 * @param[in]       in                   - plaintext/ciphertext
 * @param[out]      out                  - ciphertext/plaintext+mac
 * @param[in]       c_bytes              - byte length of plaintext/ciphertext, it could be any value, including 0
 * @param[in]       mac_bytes            - byte length of mac, must be in [0,16]
 * @param[in]       callback             - callback function pointer, this could be NULL, means doing nothing
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function is for DMA style
 *        2. only AES(128/192/256) and SM4 are supported for GCM mode
 *        3. if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
 *           otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX]
 *        4. iv must be 12 bytes here
 *        5. aad_bytes and c_bytes could not be zero at the same time
 *        6. if aad exists, it must be some blocks, if not, please pad it with 0
 *        7. plaintext/ciphertext must be some blocks, if not, please pad it with 0
 *        8. the output ciphertext/plaintext has the same number of blocks as the input plaintext/ciphertext,
 *           and followed by one block, it is mac with padding 0 if necessary, so is the second last block if
 *           necessary(ciphertext/plaintext)
 *        9. please make sure aad and in(plaintext/ciphertext) both are integral
 *           10. to save memory, in and out could be the same buffer, in this case, the output will
 *           cover the input
 */
    unsigned int ske_lp_dma_gcm_crypto(ske_alg_e alg, ske_crypto_e crypto, const unsigned char *key, unsigned short sp_key_idx, const unsigned char *iv, unsigned int iv_bytes,
                                       const unsigned int *aad, unsigned int aad_bytes, unsigned int *in, unsigned int *out, unsigned int c_bytes, unsigned int mac_bytes,
                                       SKE_CALLBACK callback);
#endif


#ifdef SUPPORT_SKE_MODE_GMAC
    /**
 * @brief           ske_lp gmac mode init config(CPU style)
 * @param[in]       ctx                  - ske_gmac_ctx_t context pointer
 * @param[in]       alg                  - ske_lp algorithm
 * @param[in]       mac_action           - must be SKE_GENERATE_MAC or SKE_VERIFY_MAC
 * @param[in]       key                  - key in bytes, key of AES(128/192/256) or SM4
 * @param[in]       sp_key_idx           - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX],
 * @param[in]       iv                   - iv in bytes
 * @param[in]       iv_bytes             - byte length of iv, now only 12 bytes supported
 * @param[in]       aad_bytes            - byte length of aad, it could be any value, including 0
 * @param[in]       c_bytes              - byte length of message, it could be any value, including 0
 * @param[in]       mac_bytes            - byte length of mac, must be in [1,16]
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function is for CPU style
 *        2. only AES(128/192/256) and SM4 are supported for GMAC mode
 *        3. if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
 *           otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX]
 *        4. iv must be 12 bytes here
 *        5. aad_bytes and c_bytes could not be zero at the same tim
 */
    unsigned int ske_lp_gmac_init(ske_gmac_ctx_t *ctx, ske_alg_e alg, ske_mac_e mac_action, const unsigned char *key, unsigned short sp_key_idx, unsigned char *iv,
                                  unsigned int iv_bytes, unsigned int aad_bytes, unsigned int c_bytes, unsigned int mac_bytes);

    /**
 * @brief           ske_lp gmac mode input aad
 * @param[in]       ctx                  - ske_gmac_ctx_t context pointer
 * @param[in]       aad                  - aad
 * @param[in]       bytes                - real byte length of aad
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function must be called after calling ske_lp_gmac_init()
 *        2. if there is no aad, this function could be omitted
 *        3. if the whole aad is too long, you could divide it into some sections by byte, then call
 *           this function to input the sections respectively. for example, if the whole aad byte
 *           length is 65, it could be divided into 3 sections with byte length 10,47,8 respectively
 */
    unsigned int ske_lp_gmac_update_aad(ske_gmac_ctx_t *ctx, const unsigned char *aad, unsigned int bytes);

    /**
 * @brief           ske_lp gmac mode input aad(one-off style)
 * @param[in]       ctx                  - ske_gmac_ctx_t context pointer
 * @param[in]       aad                  - aad, its length is ctx->gcm_ctx->aad_bytes, please make sure
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function must be called after calling ske_lp_gmac_init()
 *        2. if there is no aad, this function could be omitted
 */
    unsigned int ske_lp_gmac_aad(ske_gmac_ctx_t *ctx, unsigned char *aad);

    /**
 * @brief           ske_lp gmac mode input message
 * @param[in]       ctx                  - ske_gmac_ctx_t context pointer
 * @param[in]       in                   - message
 * @param[in]       bytes                - byte length of message
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function must be called after calling ske_lp_gmac_aad()
 *        2. if there is no message, this function could be omitted
 *        3. if the whole message is too long, you could divide it into some sections by byte, then call
 *           this function to input the sections respectively. for example, if the whole message byte
 *           length is 65, it could be divided into 3 sections with byte length 15,49,1 respectively
 */
    unsigned int ske_lp_gmac_update(ske_gmac_ctx_t *ctx, unsigned char *in, unsigned int bytes);

    /**
 * @brief           ske_lp gmac mode finish
 * @param[in]       ctx                  - ske_gmac_ctx_t context pointer
 * @param[in]       mac                  - input(for generating mac), output(for verifying mac)
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function must be called after calling ske_lp_gmac_update_blocks()
 *        2. mac_bytes must be in [1, SKE_LP_GCM_MAX_BYTES]
 *        3. if ctx->mac_action is SKE_GENERATE_MAC, mac is output. and if ctx->mac_action is SKE_VERIFY_MAC,
 *           mac is input, return value SKE_SUCCESS means the mac is valid, otherwise mac is invalid
 */
    unsigned int ske_lp_gmac_final(ske_gmac_ctx_t *ctx, unsigned char *mac);

    /**
 * @brief           ske_lp gmac mode(one-off style)
 * @param[in]       alg                  - ske_lp algorithm
 * @param[in]       mac_action           - must be SKE_GENERATE_MAC or SKE_VERIFY_MAC
 * @param[in]       key                  - key in bytes, key of AES(128/192/256) or SM4
 * @param[in]       sp_key_idx           - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX],
 * @param[in]       iv                   - iv in bytes
 * @param[in]       iv_bytes             - byte length of iv, now only 12 bytes supported
 * @param[in]       aad                  - aad, please make sure aad here is integral
 * @param[in]       aad_bytes            - byte length of aad, it could be any value, including 0
 * @param[in]       in                   - message
 * @param[in]       c_bytes              - byte length of message, it could be any value,
 * @param[in]       mac                  - input(for generating mac), output(for verifying mac)
 * @param[in]       mac_bytes            - byte length of mac.
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function is for CPU style
 *        2. only AES(128/192/256) and SM4 are supported for GCM mode
 *        3. if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
 *           otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX]
 *        4. iv must be 12 bytes here
 *        5. aad_bytes and c_bytes could not be zero at the same time
 *        6. mac_bytes must be in [1, SKE_LP_GCM_MAX_BYTES]
 *        7. if mac_action is SKE_GENERATE_MAC, mac is output. and if mac_action is SKE_VERIFY_MAC,
 *           mac is input, return value SKE_SUCCESS means the mac is valid, otherwise mac is invalid
 */
    unsigned int ske_lp_gmac_crypto(ske_alg_e alg, ske_mac_e mac_action, const unsigned char *key, unsigned short sp_key_idx, unsigned char *iv, unsigned int iv_bytes,
                                    const unsigned char *aad, unsigned int aad_bytes, unsigned char *in, unsigned int c_bytes, unsigned char *mac, unsigned int mac_bytes);
#endif


#ifdef __cplusplus
}
#endif

#endif
