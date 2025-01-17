/********************************************************************************************************
 * @file    ske_basic.h
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
#ifndef SKE_BASIC_H
#define SKE_BASIC_H

#include "dma.h"
#include "reg_include/ske_reg.h"


#ifdef __cplusplus
extern "C"
{
#endif


#define SUPPORT_SKE_DES
#if 0 //warning: hardware does not support 3DES directly, here the 4 MACRO should be defined or not defined simultaneously
    #define SUPPORT_SKE_TDES_128
    #define SUPPORT_SKE_TDES_192
    #define SUPPORT_SKE_TDES_EEE_128
    #define SUPPORT_SKE_TDES_EEE_192
#endif
#define SUPPORT_SKE_AES_128
#define SUPPORT_SKE_AES_192
#define SUPPORT_SKE_AES_256
    //#define SUPPORT_SKE_IRQ

#define SUPPORT_SKE_MODE_ECB
#define SUPPORT_SKE_MODE_CBC
#define SUPPORT_SKE_MODE_CFB
#define SUPPORT_SKE_MODE_OFB
#define SUPPORT_SKE_MODE_CTR
//#define SUPPORT_SKE_MODE_XTS        //warning: hardware does not support directly
#define SUPPORT_SKE_MODE_GCM
#ifdef SUPPORT_SKE_MODE_GCM
    #define SUPPORT_SKE_MODE_GMAC //GMAC is specialization of GCM mode
#endif
//#define SUPPORT_SKE_MODE_CMAC       //warning: hardware does not support directly
//#define SUPPORT_SKE_MODE_CBC_MAC    //warning: hardware does not support directly
#define SUPPORT_SKE_MODE_CCM
    //#define SUPPORT_SKE_AES_XCBC_MAC_96 //warning: hardware does not support directly


    //#define SKE_LP_REVERSE_BYTE_ORDER_IN_WORD

#define SKE_LP_DMA_FUNCTION

//these two macro is for SKE LP
#define SKE_LP_DMA_ENABLE  (1)
#define SKE_LP_DMA_DISABLE (0)


//#define SKE_SECURE_PORT_FUNCTION  //open avoid unused key_idx warnning
#ifdef SKE_SECURE_PORT_FUNCTION
    #define SKE_MAX_KEY_IDX (8) //if key is from secure port, the max key index(or the number of keys)
#endif

//some register offset
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
        unsigned int ctrl;        /* Offset: 0x000 (W1S) SKE Control Register */
        unsigned int cfg;         /* Offset: 0x004 (R/W) SKE Config Register */
        unsigned int sr1;         /* Offset: 0x008 (R)   SKE Status Register 1 */
        unsigned int sr2;         /* Offset: 0x00C (W0C) SKE Status Register 2 */
        unsigned int key1[8];     /* Offset: 0x010 (R/W) Key1 */
        unsigned int rev1[12];
        unsigned int ske_a_len_l; /* Offset: 0x060 (R/W) CCM/GCM mode AAD length low Register */
        unsigned int ske_a_len_h; /* Offset: 0x064 (R/W) CCM/GCM mode AAD length high Register */
        unsigned int ske_c_len_l; /* Offset: 0x068 (R/W) CCM/GCM/XTS mode plaintext/ciphertext length low Register */
        unsigned int ske_c_len_h; /* Offset: 0x06C (R/W) CCM/GCM/XTS mode plaintext/ciphertext length high Register */
        unsigned int iv[4];       /* Offset: 0x070 (R/W) Initial Vector */
        unsigned int m_din_cr;    /* Offset: 0x080 (R/W) SKE Input flag Register */
        unsigned int rev3[3];
        unsigned int m_din[4];    /* Offset: 0x090 (R/W) SKE Input Register */
        unsigned int rev4[4];
        unsigned int m_dout[4];   /* Offset: 0x0B0 (R)   SKE Output Register */
        unsigned int dma_sa_l;    /* Offset: 0x0C0 (R/W) DMA Source Address Low part register */
        unsigned int dma_da_l;    /* Offset: 0x0C4 (R/W) DMA Destination Address Low part register */
        unsigned int dma_rlen;    /* Offset: 0x0C8 (R/W) DMA Source Data Length register */
        unsigned int dma_wlen;    /* Offset: 0x0CC (R/W) DMA Destination Data Length register */
        unsigned int rev5[11];
        unsigned int ske_version; /* Offset: 0x0FC (R)   SKE Version Register */
    } ske_lp_reg_t;

    //SKE Operation Mode
    typedef enum
    {
        SKE_MODE_BYPASS = 0, // BYPASS Mode

#ifdef SUPPORT_SKE_MODE_ECB
        SKE_MODE_ECB = 1,    // ECB Mode
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
    } SKE_MODE;


    //SKE Crypto Action
    typedef enum
    {
        SKE_CRYPTO_ENCRYPT = 0, // encrypt
        SKE_CRYPTO_DECRYPT,     // decrypt
    } SKE_CRYPTO;

    //SKE MAC Action
    typedef enum
    {
        SKE_GENERATE_MAC = SKE_CRYPTO_ENCRYPT,
        SKE_VERIFY_MAC   = SKE_CRYPTO_DECRYPT,
    } SKE_MAC;

    //SKE Algorithm
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
    } SKE_ALG;


    //SKE return code
    enum SKE_RET_CODE
    {
        SKE_SUCCESS = 0,
        SKE_BUFFER_NULL,
        SKE_CONFIG_INVALID,
        SKE_INPUT_INVALID,
        SKE_ATTACK_ALARM,
        SKE_ERROR,
    };

    //SKE padding scheme
    typedef enum
    {
        SKE_NO_PADDING,
        SKE_ZERO_PADDING,
    } SKE_PADDING;

    //SKE block length
    typedef struct
    {
        unsigned char  block_bytes;
        unsigned char  block_words;
        unsigned short sp_key_idx;
    } SKE_CTX;

    //DMA_LL node
    typedef struct
    {
        unsigned int src_addr;
        unsigned int dst_addr;
        unsigned int next_llp;
        unsigned int last_len;
    } DMA_LL_node;

    //hash callback function type
    typedef void (*SKE_CALLBACK)(void);


    //APIs
    /**
 * @brief       get ske IP version.
 * @return      ske IP version
 */
    unsigned int ske_lp_get_version(void);

    /**
 * @brief       set ske_lp to be CPU mode.
 * @return      none
 * @note
  @verbatim
      -# 1. for ske_lp, make up_cfg high, to input key(for dma too).
  @endverbatim
 */
    void ske_lp_set_cpu_mode(void);

    /**
 * @brief       set ske_lp to be DMA mode.
 * @return      none
 * @note
  @verbatim
      -# 1. after calling this function, please make up_cfg low.
  @endverbatim
 */
    void ske_lp_set_dma_mode(void);

    /**
 * @brief       set the ske_lp endian.
 * @return      none
 * @note
  @verbatim
      -# 1. actually, this config works for only CPU mode now.
  @endverbatim
 */
    void ske_lp_set_endian_uint32(void);

    /**
 * @brief       set the ske_lp endian.
 * @param[in]   crypto  - SKE_CRYPTO_ENCRYPT or SKE_CRYPTO_DECRYPT.
 * @return      none
 * @note
  @verbatim
      -# 1. please make sure crypto is valid.
  @endverbatim
 */
    void ske_lp_set_crypto(SKE_CRYPTO crypto);

    /**
 * @brief       set ske_lp alg.
 * @param[in]   ske_alg  - ske_lp algorithm.
 * @return      none
 * @note
  @verbatim
      -# 1. please make sure crypto is valid.
  @endverbatim
 */
    void ske_lp_set_alg(SKE_ALG ske_alg);

    /**
 * @brief       set ske_lp alg operation mode.
 * @param[in]   mode  - soperation mode.
 * @return      none
 * @note
  @verbatim
      -# 1. please make sure mode is valid.
  @endverbatim
  @note
 */
    void ske_lp_set_mode(SKE_MODE mode);

    /**
 * @brief       set whether ske_lp current input data is the last data or not.
 * @param[in]   is_last_block  - 0:no, other:yes.
 * @return      none
 * @note
  @verbatim
      -# 1. just for CMAC/CCM/GCM/XTS mode
  @endverbatim
 */
    void ske_lp_set_last_block(unsigned int is_last_block);

    /**
 * @brief       set ske_lp current input data bit length.
 * @param[in]   bytes  - byte length of current input data.
 * @return      none
 * @note
  @verbatim
      -# 1. just for CMAC.
  @endverbatim
*/
    void ske_lp_set_last_block_len(unsigned int bytes);

    /**
 * @brief       ske_lp start to expand key or calc.
 * @return      none
 * @note
*/
    void ske_lp_start(void);

    /**
 * @brief       wait till ske_lp calculating is done.
 * @return      none
 * @note
*/
    unsigned int ske_lp_wait_till_done(void);

    /**
 * @brief       set ske_lp key.
 * @param[in]   key              - key in word buffer.
 * @param[in]   idx              - key index, only 1 and 2 are valid.
 * @param[in]   key_words        - word length of key.
 * @return      none
 * @note
  @verbatim
      -# 1. if idx is 1, set key1 register, else if idx is 2, set key2 register, please
        make sure idx is valid.
  @endverbatim
 */
    void ske_lp_set_key_uint32(unsigned int *key, unsigned int idx, unsigned int key_words);

    /**
 * @brief       set ske_lp iv.
 * @param[in]   iv               - iv in word buffer.
 * @param[in]   block_words      - word length of ske_lp block.
 * @return      none
 * @note
  @verbatim
      -# 1.please make sure the three parameters are valid.
  @endverbatim
 */
    void ske_lp_set_iv_uint32(unsigned int *iv, unsigned int block_words);

#if (defined(SUPPORT_SKE_MODE_GCM) || defined(SUPPORT_SKE_MODE_CCM))
    /**
 * @brief       set aad bits(just for ske_lp ccm/gcm mode).
 * @param[in]   aad_bytes            - iv in word buffer.
 * @return      none
 * @note
  @verbatim
      -# 1.this function is just for CCM/GCM mode.
  @endverbatim
 */
    void ske_lp_set_aad_len_uint32(unsigned int aad_bytes);
#endif

#if (defined(SUPPORT_SKE_MODE_GCM) || defined(SUPPORT_SKE_MODE_CCM) || defined(SUPPORT_SKE_MODE_XTS))
    /**
 * @brief       set plaintext/ciphertext bits(just for ske_lp ccm/gcm/xts mode).
 * @param[in]   c_bytes              - byte length of plaintext/ciphertext.
 * @return      none
 * @note
  @verbatim
      -# 1.this function is just for CCM/GCM/XTS mode.
  @endverbatim
 */
    void ske_lp_set_c_len_uint32(unsigned int c_bytes);
#endif

    /**
 * @brief       input one block.
 * @param[in]   in                   - plaintext or ciphertext in word buffer.
 * @param[in]   block_words          - word length of ske_lp block.
 * @return      none
 * @note
  @verbatim
      -# 1.in is a word buffer of only one block.
  @endverbatim
 */
    void ske_lp_simple_set_input_block(unsigned int *in, unsigned int block_words);

    /**
 * @brief       output one block.
 * @param[out]  out              - one block output of ske_lp in word buffer.
 * @param[in]   block_words      - word length of ske_lp block.
 * @return      none
 */
    void ske_lp_simple_get_output_block(unsigned int *out, unsigned int block_words);

    /**
 * @brief       ske_lp expand key.
 * @param[in]   dma_en               - for DMA mode(not 0) or not(0).
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.must be called after ske_lp_set_crypto() and ske_lp_set_alg(), and the key is set already.
  @endverbatim
 */
    unsigned int ske_lp_expand_key(unsigned int dma_en);

    /**
 * @brief       ske_lp AES128_ECB encrypting or decrypting one block(16bytes) (CPU style, one-off style).
 * @param[in]   crypto               - input, encrypting or decrypting.
 * @param[in]   key                  - input, key in 16 bytes.
 * @param[in]   in                   - input, plaintext or ciphertext.
 * @param[out]  out                  - output, ciphertext or plaintext.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.please make sure all parameter valid, include crypto is encrypto or decrypto.
      -# 2.please make sure key/in/out address is word aligned.
  @endverbatim
 */
    unsigned int ske_lp_aes128_ecb_one_block(SKE_CRYPTO crypto, unsigned int *key, unsigned int *in, unsigned int *out);

#ifdef SKE_LP_DMA_FUNCTION
    /**
 * @brief       wait till ske_lp dma calculating is done.
 * @param[in]   callback         - callback function pointer, this could be NULL, means doing nothing.
 * @return      0:success     other:error
 */
    unsigned int ske_lp_dma_calc_wait_till_done(SKE_CALLBACK callback);

    /**
 * @brief       basic ske_lp DMA operation.
 * @param[in]   ctx              - e value, 8 words, little-endian.
 * @param[in]   in               - random number k, 8 words, little-endian.
 * @param[out]  out              - private key, 8 words, little-endian.
 * @param[in]   in_words         - Signature r, 8 words, little-endian.
 * @param[in]   out_words        - Signature s, 8 words, little-endian.
 * @param[in]   callback         - callback function pointer, this could be NULL, means doing nothing.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.in_words & out_words must be multiples of block words.
      -# 2.it could be without output, namely, out can be NULL, out_words can be 0(for input AAD, or CBC_MAC/CMAC mode).
  @endverbatim
 */
    unsigned int ske_lp_dma_operate(SKE_CTX *ctx, unsigned int *in, unsigned int *out, unsigned int in_words, unsigned int out_words, SKE_CALLBACK callback);

    /**
 * @brief       clear the last (16-bytes) of the block in(16 bytes).
 * @param[in]   in               - one block buffer(128bits, for AES/SM4 GCM, CCM mode).
 * @param[in]   bytes            - real bytes of in, must be in[1,16].
 * @return      none
 * @note
  @verbatim
      -# 1.this function is for GCM,CCM mode of DMA.
  @endverbatim
 */
    void clear_block_tail(unsigned int in[4], unsigned int bytes);
#endif

    /**
 * @brief       clear the last (16-bytes) of the block in(16 bytes).
 * @param[in]   ctx              - SKE_CTX context pointer.
 * @param[in]   in               - one block buffer(128bits, for AES/SM4 GCM, CCM mode).
 * @param[in]   bytes            - real bytes of in, must be in[1,16].
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.please make sure the bytes is a multiple of block byte length ctx->block_bytes.
      -# 2.this function is called by CCM(input aad)/GCM(input aad)/CMAC/CBC-MAC mode.
  @endverbatim
 */
    unsigned int ske_lp_update_blocks_no_output(SKE_CTX *ctx, unsigned char *in, unsigned int bytes);

    /**
 * @brief       update ske_lp some blocks and get the same number of blocks.
 * @param[in]   ctx              - SKE_CTX context pointer.
 * @param[in]   in               - some blocks.
 * @param[out]  out              - the same number of blocks.
 * @param[in]   bytes            - byte length of in.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.please make sure the bytes is a multiple of block byte length ctx->block_bytes.
  @endverbatim
 */
    unsigned int ske_lp_update_blocks_internal(SKE_CTX *ctx, unsigned char *in, unsigned char *out, unsigned int bytes);


#ifdef SUPPORT_SKE_MODE_GMAC
    /**
 * @brief       for GMAC mode to input message blocks(just for AES/SM4, block size is 16 bytes).
 * @param[in]   in               - some blocks.
 * @param[in]   bytes            - byte length of in.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.please make sure the bytes is a multiple of block byte length 16.
      -# 2.this function is like ske_lp_update_blocks_internal(), but discard output.
  @endverbatim
 */
    unsigned int ske_lp_gmac_update_blocks_internal(unsigned char *in, unsigned int bytes);
#endif

#if (defined(SUPPORT_SKE_TDES_128))
    unsigned int tdes_ecb_update_one_block(unsigned int is_EEE, unsigned int key[6], SKE_CRYPTO crypto, unsigned int in[2], unsigned int out[2]);
#endif


#ifdef __cplusplus
}
#endif


#endif
