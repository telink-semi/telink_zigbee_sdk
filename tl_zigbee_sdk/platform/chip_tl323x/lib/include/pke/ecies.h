/********************************************************************************************************
 * @file    ecies.h
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
#ifndef ECIES_H
#define ECIES_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lib/include/pke/pke.h"
#include "lib/include/hash/hash.h"

#if 0
#define ECIES_SUPPORT_EC_POINT_COMPRESSED
#endif

    /**************************************************************************************
 *                                 KDF
 **************************************************************************************/
    /**
 * @brief           Enumeration for different Key Derivation Function (KDF) types.
 */
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
    } kdf_type_e;

    /**
 * @brief           Structure for Key Derivation Function (KDF) parameters.
 */
    typedef struct kdf_t kdf_base_t;

    struct kdf_t
    {
        unsigned char *input; // KDF input parameter, DH shared secret value.
        unsigned int   input_bytes;
        unsigned char *out1; // KDF output, actually output = out1||out2, out2 could be NULL.
        unsigned int   out1_bytes;
        unsigned char *out2;
        unsigned int   out2_bytes;
        unsigned int (*kdf_fun_imp)(kdf_base_t *self);
        kdf_type_e kdf_type;
    };

    /**
 * @brief           out1||out2 =
 *                  hash(input||counter||shared_info)||hash(input||counter++||shared_info)...
 *                  counter is 4 bytes, with initial value 0x00000001
 */
    typedef struct
    {
        kdf_base_t           base;
        const unsigned char *shared_info; // optional, another input parameter of KDF.
        unsigned int         shared_info_bytes;
        hash_alg_e           hash_alg; // hash algorithm used in KDF.
    } kdf_ansi_x963_2001_ctx_t;

    /**
 * @brief           ANSI-X963 KDF CTX init.
 * @param[in]       kdf_ctx              - ctx to be initialized.
 * @param[in]       shared_info          - optional, shared secret value.
 * @param[in]       shared_info_bytes    - byte length of shared_info.
 * @param[out]      hash_alg             - hash algorithm used in KDF.
 * @note            
 *        1.after this initialization, before kdf calculation, the following fields of (kdf_base_t *)kdf_ctx must be set by hand.
 *           input, input_bytes, out1, out1_bytes.out2 and out2_bytes could be ignored if the whole kdf output is in
 *           out1, otherwise these two also must be initialized.
 */
    void ansi_x963_2001_kdf_init(kdf_ansi_x963_2001_ctx_t *kdf_ctx, const unsigned char *shared_info, unsigned int shared_info_bytes, hash_alg_e hash_alg);

/**************************************************************************************
 *                                 enc_t **************************************************************************************/
#define ECIES_BLOCK_ENC_K_MAX_BYTE_LEN (64) // the name is just for static analysis.

    /**
 * @brief           Enumeration for different encryption types.
 */
    typedef enum
    {
        XOR_ENC,
        // AES_ENC
    } enc_type_e;

    typedef struct enc_t enc_base_t;

    /**
 * @brief           Structure for encryption parameters and functions.
 */
    struct enc_t
    {
        unsigned char       *key;
        unsigned int         key_bytes;
        const unsigned char *input;
        unsigned int         input_bytes;
        unsigned char       *output;
        unsigned int         output_bytes;
        unsigned int (*enc_fun_imp)(enc_base_t *self);
        unsigned int (*dec_fun_imp)(enc_base_t *self);
        enc_type_e enc_type_e;
    };

    /**
 * @brief           Structure for XOR encryption context.
 */
    typedef struct
    {
        enc_base_t base;
    } xor_enc_ctx_t;

    /**
 * @brief           XOR Encryption enc_t CTX init.
 * @param[in]       enc_ctx              - ctx to be initialized
 * @param[in]       input                - internal plaintext or ciphertext
 * @param[in]       input_bytes          - byte length of key and input
 * @return          0:success     other:error
 * @note            
 *        1.in XOR encryption, the encryption key length is equal to the message length
 *        2.after this initialization, before encryption or decryption, the
 *           following fields of (enc_base_t *)enc_ctx must be set by hand. key, output
 */
    void xor_enc_init(xor_enc_ctx_t *enc_ctx, const unsigned char *input, unsigned int input_bytes);

/**************************************************************************************
 *                                 MAC **************************************************************************************/
/**
 * @brief           the name is just for static analysis.
 */
#define ECIES_MAC_K_MAX_BYTE_LEN (128u)
/**
 * @brief           10 for ANSI-9.63-2001, and 14 for ANSI-9.63-2011-r2017
 */
#define ECIES_MAC_K_ANSI_X963_MIN_BYTE_LEN (10u)
#define ECIES_MAC_MAX_BYTE_LEN             (64u)

    typedef struct mac_t mac_base_t;

    /**
 * @brief           Structure for MAC operations.
 */
    struct mac_t
    {
        // in this structure, msg is dynamically calculated ciphertext,
        // don't have appendix. eg: out = mac(key, msg || appendix)
        unsigned char       *key;
        unsigned int         key_bytes;
        const unsigned char *msg;
        unsigned int         msg_len;
        unsigned char       *mac;
        unsigned int         mac_bytes;
        // In ECIES, set mac input = ciphertext || appendix,
        // The ciphertext is calculating and
        // the value of the appendix is determined in advance
        const unsigned char *appendix;
        unsigned int         appendix_bytes;
        unsigned int (*mac_imp)(mac_base_t *self);
    };

    /**
 * @brief           Context for HMAC operations.
 */
    typedef struct
    {
        mac_base_t base;
        hash_alg_e hash_alg;
    } ecies_hmac_ctx_t;

    /**
 * @brief           HMAC MAC CTX init.
 * @param[in]       mac_ctx              - ctx to be initialized
 * @param[in]       key_buffer           - hmac key buffer, to store hmac key
 * @param[in]       key_bytes            - hmac key byte length
 * @param[out]      appendix             - appendix followed by cipher
 * @param[in]       appendix_bytes       - byte length of appendix
 * @param[in]       hash_alg             - hash algorithm used in hmac
 * @return          0:success     other:error
 * @note            
 *        1.after this initialization, before mac calculation, the following fields of (mac_base_t *)mac_ctx must be set by hand, msg, msg_len, mac
 */
    void ecies_hmac_init(ecies_hmac_ctx_t *mac_ctx, unsigned char *key, unsigned int key_bytes, const unsigned char *appendix, unsigned int appendix_bytes, hash_alg_e hash_alg);

    typedef struct
    {
        mac_base_t base;
    } cmac_ctx_t;

    // fix to old project
    typedef ecies_hmac_ctx_t         E_HMAC_CTX;
    typedef cmac_ctx_t               E_CMAC_CTX;
    typedef kdf_ansi_x963_2001_ctx_t E_KDF_ANSI_X963_2001_CTX;

    /**************************************************************************************
 *                                 ECIES  STANDARD **************************************************************************************/
    /**
 * @brief           Enumeration for different ECIES standards.
 */
    typedef enum
    {
        ANSI_X963   = 0x00000001,
        IEEE_1363a  = 0x00000002,
        ISO_18033_2 = 0x00000004,
        SECG_SEC_1  = 0x00000008,
    } ecies_type_e;

    /**
 * @brief           Enumeration for the order of encryption and MAC in ECIES.
 */
    typedef enum
    {
        ENC_MAC_ORDER = 1,
        MAC_ENC_ORDER,
    } ecies_enc_mac_key_order_e;

    typedef struct ecies_standard_t ecies_std_t;

    /**
 * @brief           Structure for ECIES standard parameters and functions.
 */
    struct ecies_standard_t
    {
        unsigned int              type_flag;
        ecies_enc_mac_key_order_e enc_mac_key_order;
        unsigned int (*get_ec_point_len)(const eccp_curve_t *curve, unsigned char *cipher, unsigned int cipher_len, unsigned int *point_bytes);
        unsigned int (*point_decompress)(const eccp_curve_t *curve, unsigned char *encode, unsigned int *x, unsigned int *y);
        unsigned int (*point_compress)(const eccp_curve_t *curve, unsigned char *x, unsigned char *y, ec_point_form_e point_form, unsigned char *result, unsigned int *r_bytes);
    };

    // fix to old project
    typedef ecies_type_e              ECIES_TYPE;
    typedef ecies_enc_mac_key_order_e ECIES_ENC_MAC_KEY_ORDER;

/**
 * @brief           ECIES return code
 */
#define ECIES_SUCCESS         PKE_SUCCESS
#define ECIES_POINTER_NULL    (PKE_SUCCESS + 0x100U)
#define ECIES_INVALID_INPUT   (PKE_SUCCESS + 0x101U)
#define ECIES_ZERO_ALL        (PKE_SUCCESS + 0x102U)
#define ECIES_INTEGER_TOO_BIG (PKE_SUCCESS + 0x103U)
#define ECIES_ERROR           (PKE_SUCCESS + 0x104U)

    /*************************************************************************************
 *                       ECIES structure Implementation
 *************************************************************************************/
    /**
 * @brief           Initialize an ECIES context with ANSI X9.63 parameters.
 * @param[in,out]   ctx                  - Pointer to the ECIES context structure to be initialized.
 * @param[in]       enc_mac_key_order    - Order of encryption and MAC key usage, as defined by the `ecies_enc_mac_key_order_e`
 *                                         enumeration.
 * @note            
 *        1. Ensure that the provided `ctx` is not NULL before calling this function.
 *        2. The `enc_mac_key_order` parameter specifies whether the encryption key or
 *           the MAC key should be derived first, following the ANSI X9.63 standard.
 */
    void ecies_ansi_x963_ctx_init(ecies_std_t *ctx, ecies_enc_mac_key_order_e enc_mac_key_order);

    /**
 * @brief           Elliptic Curve Integrated Encryption Scheme (ECIES) ANSI-X963-2001
 * @param[in]       curve                - ecc curve struct pointer, please make sure it is valid
 * @param[in]       msg                  - original message, plaintext.
 * @param[in]       msg_len              - byte length of msg.
 * @param[in]       shared_info1         - optional, shared information, for KDF.
 * @param[in]       shared_info1_len     - byte length of shared_info1.
 * @param[in]       shared_info2         - optional, shared information, for MAC.
 * @param[in]       shared_info2_len     - byte length of shared_info2.
 * @param[in]       sender_tmp_pri_key   - sender's ephemeral private key, big-endian. if you do not have this, please set this
 *                                         parameter to be NULL, it will be generated inside.
 * @param[in]       receiver_pub_key     - receiver's public key, big-endian.
 * @param[in]       point_form           - curve point representation.
 * @param[in]       kdf_hash_alg         - specific hash algorithm for KDF.
 * @param[in]       mac_hash_alg         - specific hash algorithm for MAC.
 * @param[in]       mac_k_len            - key length of the MAC.
 * @param[out]      cipher               - encryption result, ciphertext.
 * @param[out]      cipher_len           - byte length of cipher.
 * @return          ECIES_SUCCESS(success)     other:error
 * @note            
 *        1. if no shared_info1 needs to be provided, set the shared_info1 to
 *           NULL and the shared_info1_len to 0
 *        2. if no shared_info2 needs to be provided, set the shared_info2 to
 *           NULL and the shared_info2_len to 0
 *        3. if you do not have local_tmp_pri_key, please set the parameter to be
 *           NULL, it will be generated inside. this is recommended.
 *        4. the result ciphertext consists of three parts. the 1st part is a
 *           point, the 2nd part is internal ciphertext, the 3rd part is mac.
 */
    unsigned int ansi_x963_2001_ecies_encrypt(const eccp_curve_t *curve, const unsigned char *msg, unsigned int msg_len, const unsigned char *shared_info1,
                                              unsigned int shared_info1_len, const unsigned char *shared_info2, unsigned int shared_info2_len, unsigned char *sender_tmp_pri_key,
                                              unsigned char *receiver_pub_key, ec_point_form_e point_form, hash_alg_e kdf_hash_alg, hash_alg_e mac_hash_alg, unsigned int mac_k_len,
                                              unsigned char *cipher, unsigned int *cipher_len);

    /**
 * @brief           Elliptic Curve Integrated Encryption Scheme (ECIES) Decrypt ANSI-X963-2001
 * @param[in]       curve                - ecc curve struct pointer, please make sure it is valid
 * @param[in]       cipher               - original message, cipher.
 * @param[in]       cipher_len           - byte length of cipher.
 * @param[in]       receiver_pri_key     - optional, shared information, for KDF.
 * @param[in]       shared_info1         - byte length of shared_info1.
 * @param[in]       shared_info1_len     - optional, shared information, for MAC.
 * @param[in]       shared_info2         - byte length of shared_info2.
 * @param[in]       shared_info2_len     - byte length of shared_info2.
 * @param[in]       kdf_hash_alg         - specific hash algorithm for KDF.
 * @param[in]       mac_hash_alg         - specific hash algorithm for KDF.
 * @param[in]       mac_k_len            - specific key length for MAC.
 * @param[out]      msg                  - decryption result.
 * @param[out]      msg_len              - byte length of msg.
 * @return          ECIES_SUCCESS(success)     other:error
 * @note            
 *        1. if no shared_info1 needs to be provided, set the shared_info1 to
 *           NULL and the shared_info1_len to 0
 *        2. if no shared_info2 needs to be provided, set the shared_info2 to
 *           NULL and the shared_info2_len to 0
 *        3. the input cipher consists of three parts. the 1st part is a point,
 *           the 2nd part is internal ciphertext, the 3rd part is mac.
 */
    unsigned int ansi_x963_2001_ecies_decrypt(const eccp_curve_t *curve, unsigned char *cipher, unsigned int cipher_len, unsigned char *receiver_pri_key,
                                              const unsigned char *shared_info1, unsigned int shared_info1_len, const unsigned char *shared_info2, unsigned int shared_info2_len,
                                              hash_alg_e kdf_hash_alg, hash_alg_e mac_hash_alg, unsigned int mac_k_len, unsigned char *msg, unsigned int *msg_len);

#ifdef __cplusplus
}
#endif

#endif
