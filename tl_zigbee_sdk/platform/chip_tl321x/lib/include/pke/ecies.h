/********************************************************************************************************
 * @file    ecies.h
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
#ifndef ECIES_H
#define ECIES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lib/include/pke/pke.h"
#include "lib/include/hash/hash.h"



//#define ECIES_SUPPORT_EC_POINT_COMPRESSED



/**************************************************************************************
 *                                 KDF                                                *
 **************************************************************************************/

typedef enum
{
    X963_KDF,
    // KDF1,
    // KDF2,
    // IKEV2_KDF,
    // TLS_KDF,
    // NIST_ONE_KDF_HASH,
    // NIST_ONE_KDF_HMAC,
    // NIST_TWO_STEP_KDF,
}E_KDF_TYPE;

typedef struct E_KDF E_KDF_BASE;
struct E_KDF
{
    unsigned char *input;                 //KDF input parameter, DH shared secret value
    unsigned int input_bytes;
    unsigned char *out1;                  //KDF output, actually output = out1||out2, out2 could be NULL
    unsigned int out1_bytes;
    unsigned char *out2;
    unsigned int out2_bytes;
    unsigned int (*kdf_fun_imp)(E_KDF_BASE *self);
    E_KDF_TYPE kdf_type;
};

//out1||out2 = hash(input||counter||shared_info)||hash(input||counter++||shared_info)...
//counter is 4 bytes, with initial value 0x00000001
typedef struct
{
    E_KDF_BASE base;
     unsigned char *shared_info;           //optional, another input parameter of KDF
    unsigned int shared_info_bytes;
    HASH_ALG hash_alg;              //hash algorithm used in KDF
}E_KDF_ANSI_X963_2001_CTX;

/**
 * @brief       ANSI-X963 KDF CTX init.
 * @param[in]   kdf_ctx             - ctx to be initialized.
 * @param[in]   shared_info         - optional, shared secret value.
 * @param[in]   shared_info_bytes   - byte length of shared_info.
 * @param[out]  hash_alg            - hash algorithm used in KDF.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.after this initialization, before kdf calculation, the following fields of
 *       (E_KDF_BASE *)kdf_ctx must be set by hand.
 *       input, input_bytes, out1, out1_bytes.
 *       out2 and out2_bytes could be ignored if the whole kdf output is in out1, otherwise
 *       these two also must be initialized.
  @endverbatim
 */
void ansi_x963_2001_kdf_init(E_KDF_ANSI_X963_2001_CTX *kdf_ctx,  unsigned char *shared_info,
        unsigned int shared_info_bytes, HASH_ALG hash_alg);


/**************************************************************************************
 *                                 ENC                                                *
 **************************************************************************************/
#define ECIES_BLOCK_ENC_KEY_MAX_BYTE_LEN   (64)

typedef enum{
    XOR_ENC,
    //AES_ENC
}ENC_TYPE;

typedef struct ENC E_ENC_BASE;

struct ENC
{
    unsigned char  *key;
    unsigned int key_bytes;
     unsigned char  *input;
    unsigned int input_bytes;
    unsigned char  *output;
    unsigned int output_bytes;
    unsigned int (*enc_fun_imp)(E_ENC_BASE *self);
    unsigned int (*dec_fun_imp)(E_ENC_BASE *self);
    ENC_TYPE enc_type;
};

typedef struct
{
    E_ENC_BASE base;
}E_XOR_ENC_CTX;

/**
 * @brief       XOR Encryption ENC CTX init.
 * @param[in]   enc_ctx      - ctx to be initialized
 * @param[in]   input        - internal plaintext or ciphertext
 * @param[in]   input_bytes  - byte length of key and input
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.in XOR encryption, the encryption key length is equal to the message length
      -# 2.after this initialization, before encryption or decryption, the following fields of
           (E_ENC_BASE *)enc_ctx must be set by hand.
           key, output
  @endverbatim
 */
void e_xor_enc_init(E_XOR_ENC_CTX *enc_ctx,  unsigned char *input, unsigned int input_bytes);



/**************************************************************************************
 *                                 MAC                                                *
 **************************************************************************************/
#define ECIES_MAC_KEY_MAX_BYTE_LEN             (128)
#define ECIES_MAC_KEY_ANSI_X963_MIN_BYTE_LEN   (10)  //10 for ANSI-9.63-2001, and 14 for ANSI-9.63-2011-r2017
#define ECIES_MAC_MAX_BYTE_LEN                 (64)

typedef struct E_MAC E_MAC_BASE;

struct E_MAC
{
    //in this structure, msg is dynamically calculated ciphertext,
    //don't have appendix. eg: out = mac(key, msg || appendix);
    unsigned char  *key;
    unsigned int key_bytes;
     unsigned char  *msg;
    unsigned int msg_bytes;
    unsigned char  *mac;
    unsigned int mac_bytes;
    //In ECIES, set mac input = ciphertext || appendix,
    //The ciphertext is calculating and
    //the value of the appendix is determined in advance
     unsigned char  *appendix;
    unsigned int appendix_bytes;
    unsigned int (*mac_imp)(E_MAC_BASE *self);
};

typedef struct
{
    E_MAC_BASE base;
    HASH_ALG   hash_alg;
}E_HMAC_CTX;

/**
 * @brief       HMAC MAC CTX init.
 * @param[in]   mac_ctx          - ctx to be initialized
 * @param[in]   key_buffer       - hmac key buffer, to store hmac key
 * @param[in]   key_bytes        - hmac key byte length
 * @param[out]  appendix         - appendix followed by cipher
 * @param[in]   appendix_bytes   - byte length of appendix
 * @param[in]   hash_alg         - hash algorithm used in hmac
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.after this initialization, before mac calculation, the following fields of
          (E_MAC_BASE *)mac_ctx must be set by hand.
          msg, msg_bytes, mac
  @endverbatim
 */
void e_hmac_init(E_HMAC_CTX *mac_ctx, unsigned char *key, unsigned int key_bytes,  unsigned char *appendix,
        unsigned int appendix_bytes, HASH_ALG hash_alg);


typedef struct
{
    E_MAC_BASE base;
}E_CMAC_CTX;


/**************************************************************************************
 *                                 ECIES  STANDARD                                    *
 **************************************************************************************/
typedef enum
{
    ANSI_X963   = 0x00000001,
    IEEE_1363a  = 0x00000002,
    ISO_18033_2 = 0x00000004,
    SECG_SEC_1  = 0x00000008,
}ECIES_TYPE;


typedef enum
{
    ENC_MAC_ORDER   = 1,
    MAC_ENC_ORDER,
}ECIES_ENC_MAC_KEY_ORDER;


typedef struct ECIES_STANDARD ECIES_STD;

struct ECIES_STANDARD
{
    unsigned int type_flag;
    ECIES_ENC_MAC_KEY_ORDER enc_mac_key_order;
    unsigned int (* get_point_len_from_ciphertext)( eccp_curve_t *curve, unsigned char *cipher, unsigned int cipher_bytes, unsigned int *point_bytes);
    unsigned int (* point_decompress)( eccp_curve_t *curve, unsigned char *encode, unsigned int *x, unsigned int *y);
    unsigned int (* point_compress)( eccp_curve_t *curve, unsigned char *x, unsigned char *y, EC_POINT_FORM point_form, unsigned char *result, unsigned int *r_bytes);
};



//ECIES return code
#define ECIES_SUCCESS            PKE_SUCCESS
#define ECIES_POINTOR_NULL      (PKE_SUCCESS+0x100U)
#define ECIES_INVALID_INPUT     (PKE_SUCCESS+0x101U)
#define ECIES_ZERO_ALL          (PKE_SUCCESS+0x102U)
#define ECIES_INTEGER_TOO_BIG   (PKE_SUCCESS+0x103U)
#define ECIES_ERROR             (PKE_SUCCESS+0x104U)




void ecies_ansi_x963_ctx_init(ECIES_STD *ctx, ECIES_ENC_MAC_KEY_ORDER enc_mac_key_order);


/**
 * @brief       Elliptic Curve Integrated Encryption Scheme (ECIES) ANSI-X963-2001
 * @param[in]   curve                 - ecc curve struct pointer, please make sure it is valid
 * @param[in]   msg                   - original message, plaintext.
 * @param[in]   msg_bytes             - byte length of msg.
 * @param[in]   shared_info1          - optional, shared information, for KDF.
 * @param[in]   shared_info1_bytes    - byte length of shared_info1.
 * @param[in]   shared_info2          - optional, shared information, for MAC.
 * @param[in]   shared_info2_bytes    - byte length of shared_info2.
 * @param[in]   sender_tmp_pri_key    - sender's ephemeral private key, big-endian.
                                        if you do not have this, please set this parameter to be NULL,
                                        it will be generated inside.
 * @param[in]   receiver_pub_key      - reveiver's public key, big-endian.
 * @param[in]   conversion_form       - curve point representation.
 * @param[in]   kdf_hash_alg          - specific hash algorithm for KDF.
 * @param[in]   mac_hash_alg          - specific hash algorithm for MAC.
 * @param[in]   mac_k_bytes           - key length of the MAC.
 * @param[out]  cipher                - encryption result, ciphertext.
 * @param[out]  cipher_bytes          - byte length of cipher.
 * @return      ECIES_SUCCESS(success)     other:error
 * @note
   @verbatim
      -# 1. if no shared_info1 needs to be provided, set the shared_info1 to NULL and the shared_info1_len to 0
      -# 2. if no shared_info2 needs to be provided, set the shared_info2 to NULL and the shared_info2_len to 0
      -# 3. if you do not have local_tmp_pri_key, please set the parameter to be NULL, it will be generated inside.
            this is recommended.
      -# 4. the result ciphertext consists of three parts. the 1st part is a point, the 2nd part is
            internal ciphertext, the 3rd part is mac.
   @endverbatim
 */
unsigned int ansi_x963_2001_ecies_encrypt( eccp_curve_t *curve,  unsigned char *msg, unsigned int msg_bytes,
         unsigned char *shared_info1, unsigned int shared_info1_bytes,  unsigned char *shared_info2,
        unsigned int shared_info2_bytes, unsigned char *sender_tmp_pri_key, unsigned char *receiver_pub_key,
        EC_POINT_FORM point_form, HASH_ALG kdf_hash_alg, HASH_ALG mac_hash_alg,
        unsigned int mac_k_bytes, unsigned char *cipher, unsigned int *cipher_bytes);

/**
 * @brief       Elliptic Curve Integrated Encryption Scheme (ECIES) Decrypt ANSI-X963-2001
 * @param[in]   curve                 - ecc curve struct pointer, please make sure it is valid
 * @param[in]   cipher                - original message, plaintext.
 * @param[in]   cipher_len            - byte length of msg.
 * @param[in]   receiver_pri_key      - optional, shared information, for KDF.
 * @param[in]   shared_info1          - byte length of shared_info1.
 * @param[in]   shared_info1_bytes    - optional, shared information, for MAC.
 * @param[in]   shared_info2          - byte length of shared_info2.
 * @param[in]   shared_info2_bytes    - sender's ephemeral private key, big-endian.
 * @param[in]   kdf_hash_alg          - reveiver's public key, big-endian.
 * @param[in]   mac_hash_alg          - curve point representation.
 * @param[in]   mac_k_bytes           - specific hash algorithm for KDF.
 * @param[out]  msg                   - decryption result.
 * @param[out]  msg_bytes             - byte length of msg.
 * @return      ECIES_SUCCESS(success)     other:error
 * @note
   @verbatim
      -# 1. if no shared_info1 needs to be provided, set the shared_info1 to NULL and the shared_info1_len to 0
      -# 2. if no shared_info2 needs to be provided, set the shared_info2 to NULL and the shared_info2_len to 0
      -# 3. the input cipher consists of three parts. the 1st part is a point, the 2nd part is
            internal ciphertext, the 3rd part is mac.
   @endverbatim
 */
unsigned int ansi_x963_2001_ecies_decrypt( eccp_curve_t *curve, unsigned char *cipher, unsigned int cipher_bytes,
        unsigned char *receiver_pri_key,  unsigned char *shared_info1, unsigned int shared_info1_bytes,
         unsigned char *shared_info2, unsigned int shared_info2_bytes, HASH_ALG kdf_hash_alg,
        HASH_ALG mac_hash_alg, unsigned int mac_k_bytes, unsigned char *msg, unsigned int *msg_bytes);





#ifdef __cplusplus
}
#endif

#endif
