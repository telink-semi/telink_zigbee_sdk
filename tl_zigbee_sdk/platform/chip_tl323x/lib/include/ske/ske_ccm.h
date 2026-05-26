/********************************************************************************************************
 * @file    ske_ccm.h
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
#ifndef SKE_CCM_H
#define SKE_CCM_H


#include "ske.h"


#ifdef __cplusplus
extern "C"
{
#endif


#define SKE_LP_CCM_CPU_UPDATE_AAD_BY_STEP

    typedef struct
    {
        unsigned char buf[16];
        ske_ctx_t     ske_ccm_ctx[1];
        ske_crypto_e  crypto;
        unsigned int  aad_bytes;
        unsigned int  c_bytes;
        unsigned int  current_bytes;
        unsigned char b1_aad_start_offset;
        unsigned char b1_aad_end_offset;
        unsigned char M;
        unsigned char L;
    } ske_ccm_ctx_t;

    typedef ske_ccm_ctx_t SKE_CCM_CTX;

    /**
 * @brief           APIs
 */

    /**
 * @brief           ske_lp ccm mode init config
 * @param[in]       ctx                  - ske_ccm_ctx_t context pointer
 * @param[in]       alg                  - ske_lp algorithm
 * @param[in]       crypto               - encrypting or decrypting
 * @param[in]       key                  - key in bytes, key of AES(128/192/256) or SM4
 * @param[in]       sp_key_idx           - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX]
 * @param[in]       nonce                - nonce in bytes, its byte length is 15-L
 * @param[in]       M                    - bytes of authentication field(bytes of mac)
 * @param[in]       L                    - bytes of length field(message byte length is less than 256^L)
 * @param[in]       aad_bytes            - byte length of aad, it could be any value, including 0
 * @param[in]       c_bytes              - byte length of plaintext/ciphertext, it could be any value, including 0
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function is for CPU style
 *        2. only AES(128/192/256) and SM4 are supported for CCM mode
 *        3. if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
 *           otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX]
 *        4. valid M is {4,6,8,10,12,14,16}, and valid L is {2,3,4,5,6,7,8},
 *        5. aad_bytes and c_bytes could not be zero at the same time due to hardware implementation
 */
    unsigned int ske_lp_ccm_init(ske_ccm_ctx_t *ctx, ske_alg_e alg, ske_crypto_e crypto, const unsigned char *key, unsigned short sp_key_idx, unsigned char *nonce, unsigned char M,
                                 unsigned char L, unsigned int aad_bytes, unsigned int c_bytes);

    /**
 * @brief           ske_lp ccm mode input aad(multiple step style)
 * @param[in]       ctx                  - ske_gcm_ctx_t context pointer
 * @param[in]       aad                  - aad
 * @param[in]       bytes                - byte length of aad
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function must be called after calling ske_lp_ccm_init()
 *        2. if there is no aad, this function could be omitted
 *        3. if the whole aad is too long, you could divide it into some sections by byte, then call
 *           this function to input the sections respectively. for example, if the whole aad byte
 *           length is 65, it could be divided into 3 sections with byte length 10,47,8 respectively
 */
    unsigned int ske_lp_ccm_update_aad(ske_ccm_ctx_t *ctx, const unsigned char *aad, unsigned int bytes);

    /**
 * @brief           ske_lp ccm mode input aad(one-off style)
 * @param[in]       ctx                  - ske_gcm_ctx_t context pointer
 * @param[in]       aad                  - aad, its length is ctx->aad_bytes, please make sure
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function must be called after calling ske_lp_ccm_init()
 *        2. if there is no aad, this function could be omitted
 */
    unsigned int ske_lp_ccm_aad(ske_ccm_ctx_t *ctx, const unsigned char *aad);

    /**
 * @brief           ske_lp ccm mode input plaintext/ciphertext
 * @param[in]       ctx                  - ske_ccm_ctx_t context pointer
 * @param[in]       in                   - plaintext or ciphertext
 * @param[out]      out                  - ciphertext or plaintext
 * @param[in]       bytes                - byte length of input or output
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function must be called after inputting aad
 *        2. if there is no plaintext/ciphertext, this function could be omitted
 *        3. to save memory, in and out could be the same buffer, in this case, the output will
 *           cover the input.
 *        4. if the whole plaintext/ciphertext is too long, you could divide it by block(16 bytes),
 *           and if the whole plaintext/ciphertext byte length is not a multiple of 16, please make
 *           sure the last section contains the tail, then call this function to input the sections
 *           respectively. for example, if the whole plaintext/ciphertext byte length is 65, it
 *           could be divided into 3 sections with byte length 32,16,17 respectively
 */
    unsigned int ske_lp_ccm_update_blocks(ske_ccm_ctx_t *ctx, unsigned char *in, unsigned char *out, unsigned int bytes);

    /**
 * @brief           ske_lp ccm mode finish
 * @param[in]       ctx                  - ske_ccm_ctx_t context pointer
 * @param[in]       mac                  - input(for decryption), output(for encryption)
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function must be called after calling ske_lp_ccm_update_blocks()
 *        2. byte length of mac is ctx->M
 *        3. for encryption, mac is output; and for decryption, mac is input, if returns SKE_SUCCESS
 *           that means certification passed, otherwise not
 */
    unsigned int ske_lp_ccm_final(ske_ccm_ctx_t *ctx, unsigned char *mac);

    /**
 * @brief           ske_lp ccm mode encrypt/decrypt(one-off style)
 * @param[in]       alg                  - ske_lp algorithm
 * @param[in]       crypto               - encrypting or decrypting
 * @param[in]       key                  - key in bytes, key of AES(128/192/256) or SM4
 * @param[in]       sp_key_idx           - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX]
 * @param[in]       nonce                - nonce in bytes, its byte length is 15-L
 * @param[in]       M                    - bytes of authentication field(bytes of mac)
 * @param[in]       L                    - bytes of length field(message byte length is less than 256^L)
 * @param[in]       aad                  - aad, please make sure aad here is integral
 * @param[in]       aad_bytes            - byte length of aad, it could be any value, including 0
 * @param[in]       in                   - plaintext or ciphertext
 * @param[out]      out                  - ciphertext or plaintext
 * @param[in]       c_bytes              - byte length of plaintext/ciphertext, it could be any value, including 0
 * @param[in]       mac                  - input(for decryption), output(for encryption)
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function is for CPU style
 *        2. only AES(128/192/256) and SM4 are supported for GCM mode
 *        3. if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
 *           otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX]
 *        4. valid M is {4,6,8,10,12,14,16}, and valid L is {2,3,4,5,6,7,8},
 *        5. aad_bytes and c_bytes could not be zero at the same time due to hardware implementation
 *        6. to save memory, in and out could be the same buffer, in this case, the output will
 *           cover the input.
 *        7. byte length of mac is M
 *        8. for encryption, mac is output; and for decryption, mac is input, if returns SKE_SUCCESS
 *           that means certification passed, otherwise not
 */
    unsigned int ske_lp_ccm_crypto(ske_alg_e alg, ske_crypto_e crypto, const unsigned char *key, unsigned short sp_key_idx, unsigned char *nonce, unsigned char M, unsigned char L,
                                   const unsigned char *aad, unsigned int aad_bytes, unsigned char *in, unsigned char *out, unsigned int c_bytes, unsigned char *mac);


#ifdef SKE_LP_DMA_FUNCTION
    /**
 * @brief           ske_lp dma ccm mode init config
 * @param[in]       ctx                  - ske_ccm_ctx_t context pointer
 * @param[in]       alg                  - ske_lp algorithm
 * @param[in]       crypto               - encrypting or decrypting
 * @param[in]       key                  - key in bytes, key of AES(128/192/256) or SM4
 * @param[in]       sp_key_idx           - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX]
 * @param[in]       nonce                - nonce in bytes, its byte length is 15-L
 * @param[in]       M                    - bytes of authentication field(bytes of mac)
 * @param[in]       L                    - bytes of length field(message byte length is less than 256^L)
 * @param[in]       aad_bytes            - byte length of aad, it could be any value, including 0
 * @param[in]       c_bytes              - byte length of plaintext/ciphertext, it could be any value, including 0
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function is for DMA style
 *        2. only AES(128/192/256) and SM4 are supported for CCM mode
 *        3. if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
 *           otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX]
 *        4. valid M is {4,6,8,10,12,14,16}, and valid L is {2,3,4,5,6,7,8},
 *        5. aad_bytes and c_bytes could not be zero at the same time due to hardware implementation
 */
    unsigned int ske_lp_dma_ccm_init(ske_ccm_ctx_t *ctx, ske_alg_e alg, ske_crypto_e crypto, const unsigned char *key, unsigned short sp_key_idx, const unsigned char *nonce,
                                     unsigned char M, unsigned char L, unsigned int aad_bytes, unsigned int c_bytes);

    /**
 * @brief           Get B0 block for CCM mode
 * @param[in]       nonce                - Nonce value
 * @param[in]       M                    - Bytes of authentication field
 * @param[in]       L                    - Bytes of length field
 * @param[in]       aad_bytes            - Byte length of additional authenticated data
 * @param[in]       c_bytes              - Byte length of plaintext/ciphertext
 * @param[out]      out                  - Output buffer for B0 block
 * @note            Please make sure all parameters are valid
 */
    void ske_lp_ccm_get_B0(const unsigned char *nonce, unsigned char M, unsigned char L, unsigned int aad_bytes, unsigned int c_bytes, unsigned char out[16]);

    /**
 * @brief           ske_lp dma ccm mode update B0(multiple steps style)
 * @param[in]       ctx                  - ske_ccm_ctx_t context pointer
 * @param[in]       B0                   - ske_lp algorithm
 * @param[in]       callback             - callback function pointer, this could be NULL, means do nothing
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this is for multiple steps style
 *        2. this function must be called after calling ske_lp_dma_ccm_init(
 */
    unsigned int ske_lp_dma_ccm_update_B0_block(ske_ccm_ctx_t *ctx, unsigned int B0[4], SKE_CALLBACK callback);

    /**
 * @brief           Get B1 block (if aad exists)
 * @param[in]       aad                  - Additional Authenticated Data
 * @param[in]       aad_bytes            - Byte length of aad
 * @param[out]      aad_offset           - Necessary byte length of add head to build B1 block
 * @param[out]      out                  - Output buffer for B1 block
 * @note            
 *        1. Please make sure all parameters are valid
 *        2. *aad_offset will be the necessary byte length of add head, to build B1 block
 */
    void ske_lp_ccm_get_B1(const unsigned char *aad, unsigned int aad_bytes, unsigned int *aad_offset, unsigned char out[16]);

    /**
 * @brief           ske_lp dma ccm mode update aad(multiple steps style)
 * @param[in]       ctx                  - ske_ccm_ctx_t context pointer
 * @param[in]       aad                  - aad
 * @param[in]       bytes                - byte length of aad
 * @param[in]       callback             - callback function pointer, this could be NULL, means do nothing
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this is for multiple steps style
 *        2. if there is no aad, this function could be omitted
 *        3. this function must be called after calling ske_lp_dma_ccm_update_B0_block()
 *        4. the whole aad here must be with prefix(in B1), and padding the tail with 0 if necessary(to
 *           make it occupies a multiple of 16 bytes). and now whole aad byte length is prefix byte length
 *           (ctx->b1_aad_start_offset) + original aad byte length.
 *        5. if the whole aad with prefix is too long, you could divide it by block(16 bytes), and if the
 *           whole aad byte length is not a multiple of 16, please make sure the last section contains the
 *           tail, then call this function to input the sections respectively. for example, if the whole
 *           aad byte length is 65, it could be divided into 3 sections with byte length 48,16,1 respectively
 */
    unsigned int ske_lp_dma_ccm_update_aad_blocks(ske_ccm_ctx_t *ctx, unsigned int *aad, unsigned int bytes, SKE_CALLBACK callback);

    /**
 * @brief           ske_lp dma ccm mode input plaintext/ciphertext, get ciphertext/plaintext or            ciphertext/plaintext+mac(multiple steps style)
 * @param[in]       ctx                  - ske_ccm_ctx_t context pointer
 * @param[in]       in                   - plaintext/ciphertext
 * @param[out]      out                  - ciphertext/plaintext or ciphertext/plaintext+mac
 * @param[in]       in_bytes             - byte length of in
 * @param[in]       callback             - callback function pointer, this could be NULL, means do nothing
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function must be called after calling ske_lp_dma_ccm_update_aad_blocks()
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
    unsigned int ske_lp_dma_ccm_update_blocks(ske_ccm_ctx_t *ctx, unsigned int *in, unsigned int *out, unsigned int in_bytes, SKE_CALLBACK callback);

    /**
 * @brief           ske_lp dma ccm mode finish
 * @param[in]       ctx                  - ske_ccm_ctx_t context pointer
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function is optional
 */
    unsigned int ske_lp_dma_ccm_final(ske_ccm_ctx_t *ctx);

    /**
 * @brief           ske_lp dma ccm mode encrypt/decrypt(one-off style)
 * @param[in]       alg                  - ske_lp algorithm
 * @param[in]       crypto               - encrypting or decrypting
 * @param[in]       key                  - key in bytes, key of AES(128/192/256) or SM4
 * @param[in]       sp_key_idx           - index of secure port key, (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX]
 * @param[in]       nonce                - nonce in bytes, its byte length is 15-L
 * @param[in]       M                    - bytes of authentication field(bytes of mac)
 * @param[in]       L                    - bytes of length field(message byte length is less than 256^L)
 * @param[in]       aad_bytes            - byte length of aad, it could be any value, including 0
 * @param[in]       in                   - B0+aad+plaintext/ciphertext
 * @param[out]      out                  - ciphertext/plaintext+mac
 * @param[in]       c_bytes              - byte length of plaintext/ciphertext, it could be any value, including 0
 * @param[in]       callback             - callback function pointer, this could be NULL, means do nothing
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. this function is for DMA style
 *        2. only AES(128/192/256) and SM4 are supported for GCM mode
 *        3. if key is from user input, please make sure key is not NULL(now sp_key_idx is useless),
 *           otherwise, key is from secure port, and (sp_key_idx & 0x7FFF) must be in [1,SKE_MAX_KEY_IDX]
 *        4. valid M is {4,6,8,10,12,14,16}, and valid L is {2,3,4,5,6,7,8},
 *        5. aad_bytes and c_bytes could not be zero at the same time due to hardware implementation
 *        6. if aad exists, it must be some blocks, if not, please pad it with 0
 *        7. plaintext/ciphertext must be some blocks, if not, please pad it with 0
 *        8. the output ciphertext/plaintext has the same number of blocks as the input plaintext/ciphertext,
 *           and followed by one block, it is mac with padding 0 if necessary, so is the second last block if
 *           necessary(ciphertext/plaintext)
 *        9. please make sure B0+aad+plaintext/ciphertext is integral
 *           10. to save memory, in and out could be the same buffer, in this case, the output will
 *           cover the input
 */
    unsigned int ske_lp_dma_ccm_crypto(ske_alg_e alg, ske_crypto_e crypto, const unsigned char *key, unsigned short sp_key_idx, const unsigned char *nonce, unsigned char M,
                                       unsigned char L, unsigned int aad_bytes, unsigned int *in, unsigned int *out, unsigned int c_bytes, SKE_CALLBACK callback);
#endif


#ifdef __cplusplus
}
#endif

#endif
