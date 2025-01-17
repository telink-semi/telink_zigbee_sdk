/********************************************************************************************************
 * @file    chacha20_poly1305_basic.h
 *
 * @brief   This is the header file for TL721X
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
#ifndef CHACHA20_POLY1305_BASE_H
#define CHACHA20_POLY1305_BASE_H

#include <string.h>
#include "reg_include/chacha20_poly1305_reg.h"
#include "dma.h"


#ifdef __cplusplus
extern "C"
{
#endif


    typedef struct
    {
        unsigned char cur_tag[17];
        unsigned int  cur_cnt;
    } CALC_PROCESS_CTX;

#define CHACHA20_POLY1305_DMA_FUNCTION

//these two macro is for CHACHA20_POLY1305
#define CHACHA20_POLY1305_DMA_ENABLE  (1)
#define CHACHA20_POLY1305_DMA_DISABLE (0)


//some register offset
#define CHACHA20_POLY1305_SOFT_RESET_OFFSET (0)
#define CHACHA20_POLY1305_CRYPTO_OFFSET     (2)
#define CHACHA20_POLY1305_DMA_OFFSET        (3)
#define CHACHA20_POLY1305_SEC_STAGE_OFFSET  (0)

//other
#define CHACHA20_BLOCK_BYTE_LEN   (64)
#define POLY1305_TAG_BYTE_LEN     (16)
#define POLY1305_CUR_TAG_BYTE_LEN (17)

    //chacha20_poly1305 return code
    enum CHACHA20_POLY1305_CODE
    {
        CHACHA20_POLY1305_SUCCESS = 0,
        CHACHA20_POLY1305_BUFFER_NULL,
        CHACHA20_POLY1305_CONFIG_INVALID,
        CHACHA20_POLY1305_INPUT_INVALID,
        CHACHA20_POLY1305_ATTACK_ALARM,
        CHACHA20_POLY1305_ERROR,
        CHACHA20_POLY1305_VERIFY_FAIL
    };

    //chacha20_poly1305 Crypto Action
    typedef enum
    {
        CHACHA20_POLY1305_CRYPTO_ENCRYPT = 0, // encrypt
        CHACHA20_POLY1305_CRYPTO_DECRYPT,     // decrypt
    } CHACHA20_POLY1305_CRYPTO;

    //chacha20_poly1305 Stage Section
    typedef enum
    {
        CHACHA20_POLY1305_STAGE_ALL = 0,
        CHACHA20_POLY1305_STAGE_INIT,
        CHACHA20_POLY1305_STAGE_MIDDLE,
        CHACHA20_POLY1305_STAGE_LAST,
    } CHACHA20_POLY1305_SEC_STAGE;


#if 0
//chacha20_poly1305 register struct
typedef struct {
    unsigned int ctrl;              /* Offset: 0x000 (W1S) chacha20_poly1305 Control Register */
    unsigned int cfg;               /* Offset: 0x004 (R/W) chacha20_poly1305 Config Register */
    unsigned int soft_rst_n;        /* Offset: 0x008 (W0)  chacha20_poly1305 Soft Reset Register*/
    unsigned int din_rdy;           /* Offset: 0x00C (R0)  chacha20_poly1305 Ready Data Register*/
    unsigned int risr;              /* Offset: 0x010 (W0C) chacha20_poly1305 Interrupt Source Register */
    unsigned int imcr;              /* Offset: 0x014 (R/W) chacha20_poly1305 Interrupt Enable Register */
    unsigned int status;            /* Offset: 0x018 (R)   chacha20_poly1305 Status Register */
    unsigned int err_code;          /* Offset: 0x01C (R)   chacha20_poly1305 Error Code Register */
    unsigned int last;              /* Offset: 0x020 (R/W) chacha20_poly1305 Last Data Register */
    unsigned int se_done;           /* Offset: 0x024 (R)   chacha20_poly1305 Secure Key Configured Register */
    unsigned int rev2[2];
    unsigned int key[8];            /* Offset: 0x030 (W)   chacha20_poly1305 Key Register */
    unsigned int iv[2];          /* Offset: 0x050 (W)   chacha20_poly1305 IV Register */
    unsigned int constant;          /* Offset: 0x058 (W)   chacha20_poly1305 Constant Register */
    unsigned int rev3[1];
    unsigned int length[5];          /* Offset: 0x060 (W)   chacha20_poly1305 Length Register*/
    unsigned int cnt;               /* Offset: 0x074 (W)   chacha20_poly1305 Counter Register */
    unsigned int rev4[2];
    unsigned int tag_in[5];         /* Offset: 0x080 (W)   chacha20_poly1305 Tag Input Register */
    unsigned int din;               /* Offset: 0x094 (W)   chacha20_poly1305 Data Input Register */
    unsigned int rev6[25];
    unsigned int version;           /* Offset: 0x0FC (R)   chacha20_poly1305 Version Register */
    unsigned int dout[4];           /* Offset: 0x100 (R)   chacha20_poly1305 Data Output Register */
    unsigned int tout[5];           /* Offset: 0x110 (R)   chacha20_poly1305 Tag Output register */
    unsigned int cout;              /* Offset: 0x124 (R)   chacha20_poly1305 Counter Output Register */
    unsigned int rev7[38];
    
    unsigned int dma_saddr_a;       /* Offset: 0x1C0 (R/W) DMA Source AAD Register */
    unsigned int dma_rlen_a;        /* Offset: 0x1C4 (R/W) DMA Reading AAD Byte Length Register */
    unsigned int rev[2];
    unsigned int dma_saddr_d;       /* Offset: 0x1D0 (R/W) DMA Source Payload Register */
    unsigned int dma_daddr_d;       /* Offset: 0x1D4 (R/W) DMA Destination Payload Register */
    unsigned int dma_len_d;         /* Offset: 0x1D8 (R/W) DMA Reading Payload Byte Length Register */
} chacha20_poly1305_reg_t;
#else
    #define rCHACHA20_POLY1305_CTRL        (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR)))                       /* Offset: 0x000 (W1S) chacha20_poly1305 Control Register */
    #define rCHACHA20_POLY1305_CFG         (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x004U)))              /* Offset: 0x004 (R/W) chacha20_poly1305 Config Register */
    #define rCHACHA20_POLY1305_SOFT_RST_N  (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x008U)))              /* Offset: 0x008 (W0)  chacha20_poly1305 Soft Reset Register*/
    #define rCHACHA20_POLY1305_DIN_RDY     (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x00CU)))              /* Offset: 0x00C (R0)  chacha20_poly1305 Ready Data Register*/
    #define rCHACHA20_POLY1305_RISR        (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x010U)))              /* Offset: 0x010 (W0C) chacha20_poly1305 Interrupt Source Register */
    #define rCHACHA20_POLY1305_IMCR        (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x014U)))              /* Offset: 0x014 (R/W) chacha20_poly1305 Interrupt Enable Register */
    #define rCHACHA20_POLY1305_STATUS      (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x018U)))              /* Offset: 0x018 (R)   chacha20_poly1305 Status Register */
    #define rCHACHA20_POLY1305_ERR_CODE    (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x01CU)))              /* Offset: 0x01C (R)   chacha20_poly1305 Error Code Register */
    #define rCHACHA20_POLY1305_LAST        (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x020U)))              /* Offset: 0x020 (R/W) chacha20_poly1305 Last Data Register */
    #define rCHACHA20_POLY1305_SE_DONE     (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x024U)))              /* Offset: 0x024 (R)   chacha20_poly1305 Secure Key Configured Register */
    #define rCHACHA20_POLY1305_KEY(i)      (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x030U + ((i) << 2)))) /* Offset: 0x030 (W)   chacha20_poly1305 Key Register, 8 words*/
    #define rCHACHA20_POLY1305_IV(i)       (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x050U + ((i) << 2)))) /* Offset: 0x050 (W)   chacha20_poly1305 IV Register, 2 words*/
    #define rCHACHA20_POLY1305_CONSTANT    (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x058U)))              /* Offset: 0x058 (W)   chacha20_poly1305 Constant Register */
    #define rCHACHA20_POLY1305_LENGTH(i)   (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x060U + ((i) << 2)))) /* Offset: 0x060 (W)   chacha20_poly1305 Length Register, 5 words */
    #define rCHACHA20_POLY1305_CNT         (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x074U)))              /* Offset: 0x074 (W)   chacha20_poly1305 Counter Register */
    #define rCHACHA20_POLY1305_TAG_IN(i)   (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x080U + ((i) << 2)))) /* Offset: 0x080 (W)   chacha20_poly1305 Tag Input Register, 5 words */
    #define rCHACHA20_POLY1305_DIN         (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x094U)))              /* Offset: 0x094 (W)   chacha20_poly1305 Data Input Register */
    #define rCHACHA20_POLY1305_VERSION     (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x0FCU)))              /* Offset: 0x0FC (R)   chacha20_poly1305 Version Register */
    #define rCHACHA20_POLY1305_DOUT(i)     (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x100U + ((i) << 2)))) /* Offset: 0x100 (R)   chacha20_poly1305 Data Output Register, 4 words */
    #define rCHACHA20_POLY1305_TOUT(i)     (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x110U + ((i) << 2)))) /* Offset: 0x110 (R)   chacha20_poly1305 Tag Output register, 5 words */
    #define rCHACHA20_POLY1305_COUT        (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x124U)))              /* Offset: 0x124 (R)   chacha20_poly1305 Counter Output Register */
    #define rCHACHA20_POLY1305_DMA_SADDR_A (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x1C0U)))              /* Offset: 0x1C0 (R/W) DMA Source AAD Register */
    #define rCHACHA20_POLY1305_DMA_RLEN_A  (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x1C4U)))              /* Offset: 0x1C4 (R/W) DMA Reading AAD Byte Length Register */
    #define rCHACHA20_POLY1305_DMA_SADDR_D (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x1D0U)))              /* Offset: 0x1D0 (R/W) DMA Source Payload Register */
    #define rCHACHA20_POLY1305_DMA_DADDR_D (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x1D4U)))              /* Offset: 0x1D4 (R/W) DMA Destination Payload Register */
    #define rCHACHA20_POLY1305_DMA_LEN_D   (*((volatile unsigned int *)(CHACHA20_POLY1305_BASE_ADDR + 0x1D8U)))              /* Offset: 0x1D8 (R/W) DMA Reading Payload Byte Length Register */
#endif


    //APIs
    /**
 * @brief       get chacha20_poly1305 IP version
 * @return      chacha20_poly1305 IP version
 */
    unsigned int chacha20_poly1305_get_version(void);

    /**
 * @brief       set chacha20_poly1305 to be CPU mode
 * @return      none
 */
    void chacha20_poly1305_set_cpu_mode(void);

    /**
 * @brief       set chacha20_poly1305 to be DMA mode
 * @return      none
 */
    void chacha20_poly1305_set_dma_mode(void);

    /**
 * @brief       chacha20_poly1305 soft reset
 * @return      none
 */
    void chacha20_poly1305_soft_reset(void);

    /**
 * @brief       chacha20_poly1305 set crypto mode
 * @param[in]   crypto               - dec or enc.
 * @return      none
 */
    void chacha20_poly1305_set_crypto(CHACHA20_POLY1305_CRYPTO crypto);

    /**
 * @brief       chacha20_poly1305 set sec_stage
 * @param[in]   stage               - data interleave stage.
 * @return      error code
 */
    void chacha20_poly1305_set_sec_stage(CHACHA20_POLY1305_SEC_STAGE stage);

    /**
 * @brief       chacha20_poly1305 start
 * @return      none
 */
    void chacha20_poly1305_start(void);

    /**
 * @brief       chacha20_poly1305 start
 * @return      none
 */
    void chacha20_poly1305_wait_till_done(void);

    /**
 * @brief       chacha20_poly1305 set key
 * @param[in]   key               - key in word buffer.
 * @return      none
 */
    void chacha20_poly1305_set_key(unsigned char key[32]);

    /**
 * @brief       chacha20_poly1305 set key
 * @param[in]   constant            - constant of nonce.
 * @return      none
 */
    void chacha20_poly1305_set_const(unsigned int constant);

    /**
 * @brief       chacha20_poly1305 set nonce
 * @param[in]   iv            - iv in word buffer.
 * @return      none
 */
    void chacha20_poly1305_set_iv(unsigned char iv[8]);

    /**
 * @brief       chacha20_poly1305 set length
 * @param[in]   aad_bytes            - byte length of aad.
 * @param[in]   payload_bytes        - byte length of payload.
 * @return      none
 */
    void chacha20_poly1305_set_length(unsigned long long aad_bytes, unsigned long long payload_bytes);

    /**
 * @brief       chacha20_poly1305 set length and set aad last 0
 * @param[in]   aad_bytes            - byte length of aad.
 * @param[in]   payload_bytes        - byte length of payload.
 * @return      none
 */
    void chacha20_poly1305_set_length_clean_aad_last(unsigned long long aad_bytes, unsigned long long payload_bytes);

    /**
 * @brief       chacha20_poly1305 set cnt
 * @param[in]   counter            - counter.
 * @return      none
 */
    void chacha20_poly1305_set_cnt(unsigned int counter);

    /**
 * @brief       chacha20_poly1305 set tag_in
 * @param[in]   tag_in         - tag in byte buffer.
 * @return      none
 */
    void chacha20_poly1305_set_tag_in(unsigned char tag_in[17]);

    /**
 * @brief       chacha20_poly1305 get current counter
 * @return      none
 */
    unsigned int chacha20_poly1305_get_current_cnt(void);

    /**
 * @brief       chacha20_poly1305 get current tag
 * @param[out]  tag         - current tag.
 * @return      none
 */
    void chacha20_poly1305_get_current_tag(unsigned char tag[17]);

    /**
 * @brief       chacha20_poly1305 get err code
 * @return      error(err code)
 */
    unsigned int chacha20_poly1305_get_error(void);

    /**
 * @brief       set whether chacha20_poly1305 next input data is the last word data or not (CPU mode)
 * @param[in]   is_last_word         -last word data.
 * @return      none
 */
    void chacha20_poly1305_set_last_word(unsigned int is_last_word);

    /**
 * @brief       wait till chacha20_poly1305 can config data
 * @return      none
 */
    void chacha20_poly1305_wait_ready_data_signal(void);

    /**
 * @brief       wait till core is idle
 * @return      none
 */
    void chacha20_poly1305_wait_core_idle(void);

    /**
 * @brief       set one word data
 * @param[in]   in         - pointer to data.
 * @return      none
 */
    void chacha20_poly1305_simple_set_input_word(unsigned int in[1]);

    /**
 * @brief       set 4 words input data as one block
 * @param[in]   in         - pointer to data.
 * @return      none
 */
    void chacha20_poly1305_set_input_block(unsigned int in[4]);

    /**
 * @brief       get 4 words output data
 * @return      none
 */
    void chacha20_poly1305_get_output_block(unsigned int out[4]);

    /**
 * @brief       update payload blocks and get the same number of blocks
 * @param[in]   in             - plaintext or ciphertext.
 * @param[out]  out            - ciphertext or plaintext.
 * @param[in]   bytes          - bytes length of input or output, must be multiples of 64.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.  the input data does not contain last word data.
  @endverbatim
 */
    unsigned int chacha20_poly1305_update_blocks(unsigned char *in, unsigned char *out, unsigned int bytes);

    /**
 * @brief       update payload blocks and get the same number of blocks
 * @param[in]   in             - plaintext or ciphertext.
 * @param[out]  out            - ciphertext or plaintext.
 * @param[in]   remainder      - bytes length of input or output, must be multiples of 64.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.  the input data must contain last word data.
  @endverbatim
 */
    unsigned int chacha20_poly1305_update_last_block(unsigned char *in, unsigned char *out, unsigned int remainder);

    /**
 * @brief       update chacha20_poly1305 some blocks without output
 * @param[in]   in             - data.
 * @param[in]   words          - word length of in.
 * @return      none
 * @note
  @verbatim
      -# 1.  use for all aad input and get current tag.
  @endverbatim
 */
    void chacha20_poly1305_set_input_word(unsigned int *in, unsigned int words);


#ifdef CHACHA20_POLY1305_DMA_FUNCTION
    /**
 * @brief       basic chacha20_poly1305 DMA set source address and byte length of aad
 * @param[in]   aad             - source address of aad.
 * @param[out]  aad_bytes       - byte length of aad.
 * @return      none
 */
    void chacha20_poly1305_dma_set_aad(unsigned int *aad, unsigned int aad_bytes);

    /**
 * @brief       basic chacha20_poly1305 DMA set source address and byte length of payload
 * @param[in]   payload             - source address of payload.
 * @param[out]  payload_bytes       - byte length of payload.
 * @return      none
 */
    void chacha20_poly1305_dma_set_payload(unsigned int *payload, unsigned int payload_bytes);

    /**
 * @brief       update payload blocks and get the same number of blocks (DMA mode)
 * @param[in]   in             - plaintext or ciphertext.
 * @param[out]  out            - ciphertext or plaintext.
 * @param[in]   bytes          - bytes length of input or output.
 * @return      0:success     other:error
 */
    unsigned int chacha20_poly1305_dma_operate(unsigned int *in, unsigned int *out, unsigned int bytes);
#endif


#ifdef __cplusplus
}
#endif


#endif
