/********************************************************************************************************
 * @file    ske_basic.h
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
#ifndef SKE_BASIC_H
#define SKE_BASIC_H

#include "dma.h"
#include "reg_include/ske_reg.h"

#ifdef __cplusplus
extern "C"
{
#endif



//#define SUPPORT_SKE_DES
#if 0 /**< warning: hardware does not support 3DES directly, here the 4 MACRO should be defined or not defined simultaneously */
#define SUPPORT_SKE_TDES_128
#define SUPPORT_SKE_TDES_192
#define SUPPORT_SKE_TDES_EEE_128
#define SUPPORT_SKE_TDES_EEE_192
#endif
#define SUPPORT_SKE_AES_128
    //#define SUPPORT_SKE_AES_192
    //#define SUPPORT_SKE_AES_256
    //#define SUPPORT_SKE_SM4

    /**< #define SUPPORT_SKE_IRQ */

#define SUPPORT_SKE_MODE_ECB
#define SUPPORT_SKE_MODE_CBC
#define SUPPORT_SKE_MODE_CFB
#define SUPPORT_SKE_MODE_OFB
#define SUPPORT_SKE_MODE_CTR
//#define SUPPORT_SKE_MODE_XTS /**< warning: hardware does not support directly */
#define SUPPORT_SKE_MODE_GCM
#ifdef SUPPORT_SKE_MODE_GCM
#define SUPPORT_SKE_MODE_GMAC /**< GMAC is specialization of GCM mode */
#endif
#define SUPPORT_SKE_MODE_CMAC /**< warning: hardware does not support directly */
//#define SUPPORT_SKE_MODE_CBC_MAC /**< warning: hardware does not support directly */
#define SUPPORT_SKE_MODE_CCM
    //#define SUPPORT_SKE_AES_XCBC_MAC_96 /**< warning: hardware does not support directly */


    /**< #define SKE_LP_REVERSE_BYTE_ORDER_IN_WORD */

#define SKE_LP_DMA_FUNCTION

/**< these two macro is for SKE LP */
#define SKE_LP_DMA_ENABLE  (1)
#define SKE_LP_DMA_DISABLE (0)


/**< #define SKE_SECURE_PORT_FUNCTION */
#ifdef SKE_SECURE_PORT_FUNCTION
#define SKE_MAX_KEY_IDX (8) // if key is from secure port, the max key index(or the number of keys)
#endif

/**< some register offset */
#define SKE_LP_REVERSE_BYTE_ORDER_IN_WORD_OFFSET (24)
#define SKE_LP_MODE_OFFSET                       (28)
#define SKE_LP_CRYPTO_OFFSET                     (11)
#define SKE_LP_IRQ_OFFSET                        (17)
#define SKE_LP_UP_CFG_OFFSET                     (12)
#define SKE_LP_DMA_OFFSET                        (16)
#define SKE_LP_LAST_DATA_OFFSET                  (16)

    //SKE register struct
    typedef struct
    {
        unsigned int ctrl;    /* Offset: 0x000 (W1S) SKE Control Register */
        unsigned int cfg;     /* Offset: 0x004 (R/W) SKE Config Register */
        unsigned int sr1;     /* Offset: 0x008 (R)   SKE Status Register 1 */
        unsigned int sr2;     /* Offset: 0x00C (W0C) SKE Status Register 2 */
        unsigned int key1[8]; /* Offset: 0x010 (R/W) Key1 */
        unsigned int rev1[12];
        unsigned int ske_a_len_l; /* Offset: 0x060 (R/W) CCM/GCM mode AAD length low Register */
        unsigned int ske_a_len_h; /* Offset: 0x064 (R/W) CCM/GCM mode AAD length high Register */
        unsigned int ske_c_len_l; /* Offset: 0x068 (R/W) CCM/GCM/XTS mode plaintext/ciphertext length low Register */
        unsigned int ske_c_len_h; /* Offset: 0x06C (R/W) CCM/GCM/XTS mode plaintext/ciphertext length high Register */
        unsigned int iv[4];       /* Offset: 0x070 (R/W) Initial Vector */
        unsigned int m_din_cr;    /* Offset: 0x080 (R/W) SKE Input flag Register */
        unsigned int rev3[3];
        unsigned int m_din[4]; /* Offset: 0x090 (R/W) SKE Input Register */
        unsigned int rev4[4];
        unsigned int m_dout[4]; /* Offset: 0x0B0 (R)   SKE Output Register */
        unsigned int dma_sa_l;  /* Offset: 0x0C0 (R/W) DMA Source Address Low part register */
        unsigned int dma_da_l;  /* Offset: 0x0C4 (R/W) DMA Destination Address Low part register */
        unsigned int dma_rlen;  /* Offset: 0x0C8 (R/W) DMA Source Data Length register */
        unsigned int dma_wlen;  /* Offset: 0x0CC (R/W) DMA Destination Data Length register */
        unsigned int rev5[11];
        unsigned int ske_version; /* Offset: 0x0FC (R)   SKE Version Register */
    } ske_lp_reg_t;

    /**
 * @brief           SKE Operation Mode
 */
    typedef enum
    {
        SKE_MODE_BYPASS = 0, // BYPASS Mode

#ifdef SUPPORT_SKE_MODE_ECB
        SKE_MODE_ECB = 1, // ECB Mode
#endif

#ifdef SUPPORT_SKE_MODE_XTS
        SKE_MODE_XTS = 2, // XTS Mode
#endif

#ifdef SUPPORT_SKE_MODE_CBC
        SKE_MODE_CBC = 3, // CBC Mode
#endif

#ifdef SUPPORT_SKE_MODE_CFB
        SKE_MODE_CFB = 4, // CFB Mode
#endif

#ifdef SUPPORT_SKE_MODE_OFB
        SKE_MODE_OFB = 5, // OFB Mode
#endif

#ifdef SUPPORT_SKE_MODE_CTR
        SKE_MODE_CTR = 6, // CTR Mode
#endif

#ifdef SUPPORT_SKE_MODE_CMAC
        SKE_MODE_CMAC = 7, // CMAC Mode
#endif

#ifdef SUPPORT_SKE_MODE_CBC_MAC
        SKE_MODE_CBC_MAC = 8, // CBC-MAC Mode
#endif

#ifdef SUPPORT_SKE_MODE_GCM
        SKE_MODE_GCM = 9, // GCM Mode
#endif

#ifdef SUPPORT_SKE_MODE_CCM
        SKE_MODE_CCM = 10, // CCM Mode
#endif

#ifdef SUPPORT_SKE_MODE_GMAC
        SKE_MODE_GMAC = 18, // GMAC Mode
#endif
    } ske_mode_e;
    typedef ske_mode_e SKE_MODE;

    /**
 * @brief           SKE Crypto Action
 */
    typedef enum
    {
        SKE_CRYPTO_ENCRYPT = 0, // encrypt
        SKE_CRYPTO_DECRYPT,     // decrypt
    } ske_crypto_e;

    typedef ske_crypto_e SKE_CRYPTO;

    /**
 * @brief           SKE MAC Action
 */
    typedef enum
    {
        SKE_GENERATE_MAC = SKE_CRYPTO_ENCRYPT,
        SKE_VERIFY_MAC   = SKE_CRYPTO_DECRYPT,
    } ske_mac_e;

    typedef ske_mac_e SKE_MAC;

    /**
 * @brief           SKE Algorithm
 */
    typedef enum
    {
#ifdef SUPPORT_SKE_DES
        SKE_ALG_DES = 0, // DES
#endif

#ifdef SUPPORT_SKE_TDES_128
        SKE_ALG_TDES_128 = 1, // TDES 128 bits key
#endif

#ifdef SUPPORT_SKE_TDES_192
        SKE_ALG_TDES_192 = 2, // TDES 192 bits key
#endif

#ifdef SUPPORT_SKE_TDES_EEE_128
        SKE_ALG_TDES_EEE_128 = 3, // TDES_EEE 128 bits key
#endif

#ifdef SUPPORT_SKE_TDES_EEE_192
        SKE_ALG_TDES_EEE_192 = 4, // TDES_EEE 192 bits key
#endif

#ifdef SUPPORT_SKE_AES_128
        SKE_ALG_AES_128 = 5, // AES 128 bits key
#endif

#ifdef SUPPORT_SKE_AES_192
        SKE_ALG_AES_192 = 6, // AES 192 bits key
#endif

#ifdef SUPPORT_SKE_AES_256
        SKE_ALG_AES_256 = 7, // AES 256 bits key
#endif

#ifdef SUPPORT_SKE_SM4
        SKE_ALG_SM4 = 8, // SM4
#endif
    } ske_alg_e;
    typedef ske_alg_e SKE_ALG;

    /**
 * @brief           SKE return code
 */
    enum ske_ret_code_e
    {
        SKE_SUCCESS = 0,
        SKE_BUFFER_NULL,
        SKE_CONFIG_INVALID,
        SKE_INPUT_INVALID,
        SKE_ATTACK_ALARM,
        SKE_ERROR,
    };

    /**
 * @brief           SKE padding scheme
 */
    typedef enum
    {
        SKE_NO_PADDING,
        SKE_ZERO_PADDING,
        SKE_PKCS_5_7_PADDING,   // not support in this ske driver version
        SKE_ISO_7816_4_PADDING, // not support in this ske driver version
    } ske_padding_e;

    typedef ske_padding_e SKE_PADDING;

    /**
 * @brief           SKE block length
 */
    typedef struct
    {
        unsigned char block_bytes;
        unsigned char block_words;
    } ske_ctx_t;

    typedef ske_ctx_t SKE_CTX;

    /**
 * @brief           DMA_LL node
 */
    typedef struct
    {
        unsigned int src_addr;
        unsigned int dst_addr;
        unsigned int next_llp;
        unsigned int last_len;
    } dma_ll_node_t;

    /**
 * @brief           hash callback function type
 */
    typedef void (*SKE_CALLBACK)(void);


    /**
 * @brief           APIs
 */
    unsigned int ske_lp_get_version(void);

    /**
 * @brief           Set SKE to CPU mode
 */
    void ske_lp_set_cpu_mode(void);

    /**
 * @brief           Set SKE to DMA mode
 * @return          none
 */
    void ske_lp_set_dma_mode(void);

    /**
 * @brief           set the ske_lp endian
 * @return          none
 * @note            
 *        1. actually, this config works for only CPU mode no
 */
    void ske_lp_set_endian_uint32(void);

    /**
 * @brief           set ske_lp encrypting or decrypting
 * @param[in]       crypto               - SKE_CRYPTO_ENCRYPT or SKE_CRYPTO_DECRYPT
 * @return          none
 * @note            
 *        1. please make sure crypto is valid
 */
    void ske_lp_set_crypto(ske_crypto_e crypto);

    /**
 * @brief           set ske_lp alg
 * @param[in]       ske_alg              - ske_lp algorithm
 * @return          none
 * @note            
 *        1. please make sure ske_alg is valid
 */
    void ske_lp_set_alg(ske_alg_e ske_alg);

    /**
 * @brief           set ske_lp alg operation mode
 * @param[in]       mode                 - operation mode
 * @return          none
 * @note            
 *        1. please make sure mode is valid
 */
    void ske_lp_set_mode(ske_mode_e mode);

    /**
 * @brief           set whether ske_lp current input data is the last data or not
 * @param[in]       is_last_block        - 0:no, other:yes
 * @return          none
 * @note            
 *        1. just for CMAC/CCM/GCM/XTS mod
 */
    void ske_lp_set_last_block(unsigned int is_last_block);

    /**
 * @brief           ske_lp start to expand key or calc
 * @return          none
 */
    void ske_lp_start(void);

    /**
 * @brief           wait till ske_lp calculating is done
 * @return          none
 */
    unsigned int ske_lp_wait_till_done(void);

    /**
 * @brief           set ske_lp key
 * @param[in]       key                  - key in word buffer
 * @param[in]       idx                  - key index, only 1 and 2 are valid
 * @param[in]       key_words            - word length of key
 * @return          none
 * @note            
 *        1. if idx is 1, set key1 register, else if idx is 2, set key2 register, please
 *           make sure idx is valid
 */
    void ske_lp_set_key_uint32(unsigned int *key, unsigned int idx, unsigned int key_words);

    /**
 * @brief           set ske_lp iv
 * @param[in]       iv                   - iv in word buffer
 * @param[in]       block_words          - word length of ske_lp block
 * @return          none
 * @note            
 *        1. please make sure the three parameters are valid
 */
    void ske_lp_set_iv_uint32(const unsigned int *iv, unsigned int block_words);

#if (defined(SUPPORT_SKE_MODE_GCM) || defined(SUPPORT_SKE_MODE_CCM))
    /**
 * @brief           set aad bits(just for ske_lp ccm/gcm mode)
 * @param[in]       aad_bytes            - byte length of aad
 * @return          none
 * @note            
 *        1. this function is just for CCM/GCM mod
 */
    void ske_lp_set_aad_len_uint32(unsigned int aad_bytes);
#endif

#if (defined(SUPPORT_SKE_MODE_GCM) || defined(SUPPORT_SKE_MODE_CCM) || defined(SUPPORT_SKE_MODE_XTS))
    /**
 * @brief           set plaintext/ciphertext bits(just for ske_lp ccm/gcm/xts mode)
 * @param[in]       c_bytes              - byte length of plaintext/ciphertext
 * @return          none
 * @note            
 *        1. this function is just for CCM/GCM/XTS mod
 */
    void ske_lp_set_c_len_uint32(unsigned int c_bytes);
#endif

    /**
 * @brief           input one block
 * @param[in]       in                   - plaintext or ciphertext in word buffer
 * @param[in]       block_words          - word length of ske_lp block
 * @return          none
 * @note            
 *        1. in is a word buffer of only one block
 */
    void ske_lp_simple_set_input_block(const unsigned int *in, unsigned int block_words);

    /**
 * @brief           output one block
 * @param[out]      out                  - one block output of ske_lp in word buffer
 * @param[in]       block_words          - word length of ske_lp block
 * @return          none
 */
    void ske_lp_simple_get_output_block(unsigned int *out, unsigned int block_words);

    /**
 * @brief           ske_lp expand key
 * @param[in]       dma_en               - for DMA mode(not 0) or not(0)
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. must be called after ske_lp_set_crypto() and ske_lp_set_alg(), and the key is set already
 */
    unsigned int ske_lp_expand_key(unsigned int dma_en);

#ifdef SKE_LP_DMA_FUNCTION
    /**
 * @brief           wait till ske_lp dma calculating is done
 * @param[in]       callback             - callback function pointer, this could be NULL, means doing nothing
 * @return          SKE_SUCCESS(success), other(error)
 */
    unsigned int ske_lp_dma_calc_wait_till_done(SKE_CALLBACK callback);

    /**
 * @brief           basic ske_lp DMA operation
 * @param[in]       ctx                  - ske_ctx_t context pointer
 * @param[in]       in                   - plaintext or ciphertext
 * @param[out]      out                  - ciphertext or plaintext
 * @param[in]       in_words             - word length of in, must be multiples of block word length
 * @param[in]       out_words            - word length of out, must be multiples of block word length
 * @param[in]       callback             - callback function pointer, this could be NULL, means doing nothing
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. in_words & out_words must be multiples of block words.
 *        2. it could be without output, namely, out can be NULL, out_words can be 0(for input AAD, or CBC_MAC/CMAC mode
 */
    unsigned int ske_lp_dma_operate(ske_ctx_t *ctx, const unsigned int *in, unsigned int *out, unsigned int in_words, unsigned int out_words, SKE_CALLBACK callback);

    /**
 * @brief           clear the last (16-bytes) of the block in(16 bytes)
 * @param[in]       in                   - one block buffer(128bits, for AES/SM4 GCM, CCM mode)
 * @param[in]       bytes                - real bytes of in, must be in[1,16]
 * @return          none
 * @note            
 *        1. this function is for GCM,CCM mode of DMA
 */
    void clear_block_tail(unsigned int in[4], unsigned int bytes);
#endif

    /**
 * @brief           update ske_lp some blocks without output
 * @param[in]       ctx                  - ske_ctx_t context pointer
 * @param[in]       in                   - some blocks
 * @param[in]       bytes                - byte length of in
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the bytes is a multiple of block byte length ctx->block_bytes
 *        2. this function is called by CCM(input aad)/GCM(input aad)/CMAC/CBC-MAC mod
 */
    unsigned int ske_lp_update_blocks_no_output(ske_ctx_t *ctx, const unsigned char *in, unsigned int bytes);

    /**
 * @brief           update ske_lp some blocks and get the same number of blocks
 * @param[in]       ctx                  - ske_ctx_t context pointer
 * @param[in]       in                   - some blocks
 * @param[out]      out                  - the same number of blocks;
 * @param[in]       bytes                - byte length of in
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the bytes is a multiple of block byte length ctx->block_byte
 */
    unsigned int ske_lp_update_blocks_internal(ske_ctx_t *ctx, unsigned char *in, unsigned char *out, unsigned int bytes);


#ifdef SUPPORT_SKE_MODE_GMAC
    /**
 * @brief           for GMAC mode to input message blocks(just for AES/SM4, block size is 16 bytes)
 * @param[in]       in                   - some blocks
 * @param[in]       bytes                - byte length of in
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure the bytes is a multiple of block byte length 16
 *        2. this function is like ske_lp_update_blocks_internal(), but discard output
 */
    unsigned int ske_lp_gmac_update_blocks_internal(unsigned char *in, unsigned int bytes);
#endif

#if (defined(SUPPORT_SKE_TDES_128))
    /**
 * @brief           TDES input one block and output one block
 * @param[in]       is_EEE               - is tdes SKE_ALG_TDES_EEE_128/SKE_ALG_TDES_EEE_192 or not
 * @param[in]       key                  - TDES key, 24 bytes
 * @param[in]       crypto               - SKE_CRYPTO_ENCRYPT or SKE_CRYPTO_DECRYPT
 * @param[in]       in                   - one block
 * @param[out]      out                  - one block
 * @return          SKE_SUCCESS(success), other(error)
 * @note            
 *        1. hw DES must be set already
 *        2. hw ECB must be set already
 */
    unsigned int tdes_ecb_update_one_block(unsigned int is_EEE, unsigned int key[6], ske_crypto_e crypto, unsigned int in[2], unsigned int out[2]);
#endif

    /*
 * @brief       ske_lp AES128_ECB encrypting or decrypting one block(16bytes) (CPU style, one-off style)
 * @param[in]   key                   - key
 * @param[in]   in                    - one block.
 * @param[out]  out                   - one block.
 * @return      SKE_SUCCESS(success), other(error)
 * @note
 *       1.please make sure all parameter valid, include crypto is encryption or decryption
 *       2.please make sure key/iv/in/out address is word aligned.
 */
    unsigned int ske_lp_aes128_ecb_one_block(ske_crypto_e crypto, unsigned int *key, unsigned int *in, unsigned int *out);

    /*
 * @brief          ske_lp AES128_ECB encrypting one block(16bytes) (CPU style, one-off style)
 * @param[in]    key                   - key
 * @param[in]    plaintext             - one block plaintext
 * @param[out]   encrypted_data        - one block ciphertext
 * @return       SKE_SUCCESS(success), other(error)
 */
    void aes_encryption_be(unsigned char *key, unsigned char *plaintext, unsigned char *encrypted_data);


    /*
 * @brief          ske_lp AES128_ECB encrypting one block(16bytes) (CPU style, one-off style)
 * @param[in]    key                   - key
 * @param[in]    ciphertext            - one block ciphertext
 * @param[out]   decrypted_data        - one block plaintext
 * @return       SKE_SUCCESS(success), other(error)
 */
    void aes_decryption_be(unsigned char *key, unsigned char *ciphertext, unsigned char *decrypted_data);

#ifdef __cplusplus
}
#endif


#endif
