/********************************************************************************************************
 * @file    sm9.h
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
#ifndef SM9_H
#define SM9_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lib/include/pke/pke.h"
#include "lib/include/ske/ske.h"
#include "lib/include/hash/hash.h"


#define SM9_MAX_MSG_BYTE_LEN    (0xFFFFFF9FU) //(0xFFFFFFFF-0x40-0x20)
#define SM9_MAX_ENC_K2_BYTE_LEN (0x40U)       // MAC key for SM9 enc_t/dec to get C3, here we set maximum length to be 64 bytes.

#define SM9_FP12_EXP_COMB_PARTS (1U) //(2U)  //(3U)   //

/**
 * @brief           SM9 return code
 */
#define SM9_SUCCESS               PKE_SUCCESS
#define SM9_BUFFER_NULL           (PKE_SUCCESS + 0x90U)
#define SM9_NOT_ON_CURVE          (PKE_SUCCESS + 0x91U)
#define SM9_EXCHANGE_ROLE_INVALID (PKE_SUCCESS + 0x92U)
#define SM9_INPUT_INVALID         (PKE_SUCCESS + 0x93U)
#define SM9_ZERO_ALL              (PKE_SUCCESS + 0x94U)
#define SM9_INTEGER_TOO_BIG       (PKE_SUCCESS + 0x95U)
#define SM9_VERIFY_FAILED         (PKE_SUCCESS + 0x96U)
#define SM9_IN_OUT_SAME_BUFFER    (PKE_SUCCESS + 0x97U)
#define SM9_DECRY_VERIFY_FAILED   (PKE_SUCCESS + 0x98U)

    /**
 * @brief           SM9 encryption type choice
 */
    typedef enum
    {
        SM9_ENC_KDF_STREAM_CIPHER = 0,
        SM9_ENC_KDF_BLOCK_CIPHER
    } sm9_enc_type_e;

#if 0
// available when choose SM9_ENC_KDF_BLOCK_CIPHER
typedef enum {
    SM9_ENC_NO_PADDING = 0,
    SM9_ENC_PKCS7_PADDING
} sm9_enc_padding_e;
#else
    typedef ske_padding_e sm9_enc_padding_e;
#endif

    /**
 * @brief           SM9 key exchange role
 */
    typedef enum
    {
        SM9_Role_Sponsor = 0,
        SM9_Role_Responsor
    } sm9_exchange_role_e;

    /**
 * @brief           APIs for user
 */

    /**
 * @brief           g = e(P1, P2)
 * @param[in]       P1                   - Pointer to a 64-byte array containing the G1 point P1.
 * @param[in]       P2                   - Pointer to a 128-byte array containing the G2 point P2.
 * @param[out]      g                    - Pointer to a 384-byte array where the resulting pairing value will be stored.
 * @return          PKE_SUCCESS if the operation is successful, other values indicate an error.
 */
    unsigned int sm9_pairing_calc(const unsigned char P1[64], const unsigned char P2[128], unsigned char g[32 * 12]);

    /**
 * @brief           out = g^r
 *                  r can not be zero.
 * @param[in]       g                    - Pointer to a 384-byte array containing the FP12 element g.
 * @param[in]       r                    - Pointer to a 32-byte array containing the scalar r.
 * @param[out]      out                  - Pointer to a 384-byte array where the resulting FP12 element will be stored.
 * @return          PKE_SUCCESS if the operation is successful, SM9_BUFFER_NULL if any input buffer is null, other values indicate an error.
 */
    unsigned int sm9_fp12_exp(unsigned char g[32 * 12], unsigned char r[32], unsigned char out[32 * 12]);

    /**
 * @brief           Generates the sign master public key from the master private key.
 * @param[in]       ks                   - Pointer to a 32-byte array containing the master private key ks.
 * @param[out]      Ppub_s               - Pointer to a 128-byte array where the resulting master public key Ppub_s will be stored.
 * @return          PKE_SUCCESS if the operation is successful, other values indicate an error.
 */
    unsigned int sm9_sign_gen_mastPubKey_from_mastPriKey(const unsigned char ks[32], unsigned char Ppub_s[128]);

    /**
 * @brief           Generates a sign master key pair.
 * @param[out]      ks                   - Pointer to a 32-byte array where the generated master private key ks will be stored.
 * @param[out]      Ppub_s               - Pointer to a 128-byte array where the generated master public key Ppub_s will be stored.
 * @return          PKE_SUCCESS if the operation is successful, other values indicate an error.
 */
    unsigned int sm9_sign_gen_mastKeyPair(unsigned char ks[32], unsigned char Ppub_s[128]);

    /**
 * @brief           Generates a user sign private key.
 * @param[in]       IDA                  - Pointer to an array containing the user identifier IDA.
 * @param[in]       IDA_bytes            - Length of the user identifier IDA in bytes.
 * @param[in]       hid                  - Hash value used in the generation process.
 * @param[in]       ks                   - Pointer to a 32-byte array containing the master private key ks.
 * @param[out]      dsA                  - Pointer to a 64-byte array where the generated user private key dsA will be stored.
 * @return          PKE_SUCCESS if the operation is successful, other values indicate an error.
 */
    unsigned int sm9_sign_gen_userPriKey(const unsigned char *IDA, unsigned int IDA_bytes, unsigned char hid, const unsigned char ks[32], unsigned char dsA[64]);

    /**
 * @brief           Generates a signature for a given message.
 * @param[in]       msg                  - Pointer to an array containing the message to be signed.
 * @param[in]       msg_len              - Length of the message in bytes.
 * @param[in]       fp12g                - Pointer to an array containing the FP12 element g in byte form.
 * @param[in]       Ppub_s               - Pointer to a 128-byte array containing the master public key Ppub_s. Ensure that Ppub_s is valid.
 * @param[in]       dsA                  - Pointer to a 64-byte array containing the user private key dsA.
 * @param[in]       r                    - Pointer to a 32-byte array containing the 256-bit random number r.
 * @param[out]      h                    - Pointer to a 32-byte array where the hash value h of the message will be stored.
 * @param[out]      S                    - Pointer to a 65-byte array where the resulting signature S will be stored.
 * @return          PKE_SUCCESS if the operation is successful, other values indicate an error.
 * @note            
 *        1.please make sure Ppub_s is valid
 */
    unsigned int sm9_sign(const unsigned char *msg, unsigned int msg_len, const unsigned char *fp12g, const unsigned char Ppub_s[128], const unsigned char dsA[64],
                          const unsigned char r[32], unsigned char h[32], unsigned char S[65]);

    /**
 * @brief           SM9 verify the signature.
 * @param[in]       msg                  - Pointer to an array containing the message to be verified.
 * @param[in]       msg_len              - Length of the message in bytes.
 * @param[in]       IDA                  - Pointer to an array containing the user identifier IDA (user A is the signer).
 * @param[in]       IDA_bytes            - Length of the user identifier IDA in bytes.
 * @param[in]       hid                  - User private key generation function identity, published by KGC. Default value is 0x01 (one byte).
 * @param[in]       fp12g                - Pointer to an array containing the value of e(P1, Ppub_s) as a U8 big-endian. If set to NULL, it will be calculated within the function.
 * @param[in]       Ppub_s               - Pointer to a 128-byte array containing KGC's master public key Ppub_s.
 * @param[in]       h                    - Pointer to a 32-byte array containing the partial signature result h.
 * @param[in]       S                    - Pointer to a 65-byte array containing the partial signature result S.
 * @return          SM9_SUCCESS if the operation is successful, other values indicate an error.
 * @note            
 *        1. IDA represents only the signer.
 */
    unsigned int sm9_verify(const unsigned char *msg, unsigned int msg_len, const unsigned char *IDA, unsigned int IDA_bytes, unsigned char hid, const unsigned char *fp12g,
                            const unsigned char Ppub_s[128], const unsigned char h[32], const unsigned char S[65]);

    /**
 * @brief           Generate KGC's master public key from master private key for SM9 encryption system.
 * @param[in]       ke                   - Pointer to a 32-byte array containing the KGC's master private key, in big-endian format.
 * @param[out]      Ppub_e               - Pointer to a 64-byte array where the KGC's master public key (x||y) will be stored, in big-endian format.
 * @return          SM9_SUCCESS if the operation is successful, other values indicate an error.
 */
    unsigned int sm9_enc_gen_mastPubKey_from_mastPriKey(const unsigned char ke[32], unsigned char Ppub_e[64]);

    /**
 * @brief           Generate random KGC's master key pair for SM9 encryption system.
 * @param[out]      ke                   - Pointer to a 32-byte array where the KGC's master private key will be stored, in big-endian format.
 * @param[out]      Ppub_e               - Pointer to a 64-byte array where the KGC's master public key (x||y) will be stored, in big-endian format.
 * @return          SM9_SUCCESS if the operation is successful, other values indicate an error.
 */
    unsigned int sm9_enc_gen_mastKeyPair(unsigned char ke[32], unsigned char Ppub_e[64]);

    /**
 * @brief           Generate user's private key for SM9 encryption system.
 * @param[in]       IDB                  - Pointer to an array containing the user identifier IDB (user B).
 * @param[in]       IDB_bytes            - Length of the user identifier IDB in bytes.
 * @param[in]       hid                  - User private generation function identity, published by KGC. Default value is 0x03 (one byte).
 * @param[in]       ke                   - Pointer to a 32-byte array containing the KGC's master private key, in big-endian format.
 * @param[out]      deB                  - Pointer to a 128-byte array where the user B's private key will be stored, in big-endian format.
 * @return          SM9_SUCCESS if the operation is successful, other values indicate an error.
 */
    unsigned int sm9_enc_gen_userPriKey(const unsigned char *IDB, unsigned int IDB_bytes, unsigned char hid, const unsigned char ke[32], unsigned char deB[128]);

    /**
 * @brief           SM9 key encapsulation, generates key and its encapsulated cipher C.
 * @param[in]       IDB                  - Pointer to an array containing the user identifier IDB (user B is the receiver of the cipher C).
 * @param[in]       IDB_bytes            - Length of the user identifier IDB in bytes.
 * @param[in]       hid                  - User private generation function identity, published by KGC. Default value is 0x03 (one byte).
 * @param[in]       fp12g                - Pointer to the value of e(Ppub_e, P2). If set to NULL, it will be calculated within the function.
 * @param[in]       Ppub_e               - Pointer to a 64-byte array containing KGC's system encryption master public key (x||y), in big-endian format.
 * @param[in,out]   r                    - Pointer to a 32-byte array containing the random big integer r in wrapping, in big-endian format. If set to NULL, it will be generated inside the function.
 * @param[out]      C                    - Pointer to a 64-byte array where the encapsulated cipher C will be stored.
 * @param[in]       k_bytes              - Length of the output key in bytes.
 * @param[out]      k                    - Pointer to an array where the plaintext output key will be stored.
 * @return          SM9_SUCCESS if the operation is successful, other values indicate an error.
 */
    unsigned int sm9_wrap_key(const unsigned char *IDB, unsigned int IDB_bytes, unsigned char hid, const unsigned char *fp12g, const unsigned char Ppub_e[64],
                              const unsigned char r[32], unsigned char C[64], unsigned int k_bytes, unsigned char *k);

    /**
 * @brief           SM9 key decapsulation, generates key from encapsulated cipher C.
 * @param[in]       IDB                  - Pointer to an array containing the user identifier IDB.
 * @param[in]       IDB_bytes            - Length of the user identifier IDB in bytes.
 * @param[in]       deB                  - Pointer to a 128-byte array containing the private key of user B, in big-endian format.
 * @param[in]       C                    - Pointer to a 64-byte array containing the encapsulated cipher C.
 * @param[in]       k_bytes              - Length of the output key in bytes.
 * @param[out]      k                    - Pointer to an array where the plaintext output key will be stored.
 * @return          SM9_SUCCESS if the operation is successful, other values indicate an error.
 */
    unsigned int sm9_unwrap_key(const unsigned char *IDB, unsigned int IDB_bytes, const unsigned char deB[128], const unsigned char C[64], unsigned int k_bytes, unsigned char *k);

    /**
 * @brief           SM9 encrypt.
 * @param[in]       IDB                  - Pointer to an array containing the user identifier IDB (user B is the cipher receiver).
 * @param[in]       IDB_bytes            - Length of the user identifier IDB in bytes.
 * @param[in]       hid                  - User private generation function identity, published by KGC. Default value is 0x03, one byte.
 * @param[in]       M                    - Pointer to an array containing the message to be encrypted.
 * @param[in]       M_bytes              - Length of the message to be encrypted in bytes.
 * @param[in]       fp12g                - Pointer to the value of e(Ppub_e, P2). If set to NULL, it will be calculated within the function.
 * @param[in]       Ppub_e               - KGC's system encryption master public key.
 * @param[in]       r                    - Pointer to a 32-byte array containing the random big integer r in wrapping, in big-endian format. If you do not have this integer, please set this parameter to NULL, and it will be generated inside.
 * @param[in]       enc_type             - Type of encryption (SM9_ENC_KDF_STREAM_CIPHER or SM9_ENC_KDF_BLOCK_CIPHER).
 * @param[in]       padding_type         - Type of padding (SKE_NO_PADDING or SKE_PKCS_5_7_PADDING).
 * @param[in]       K2_bytes             - Length of the key K2 in MAC function in bytes.
 * @param[out]      C                    - Pointer to an array where the cipher will be stored.
 * @param[out]      C_bytes              - Length of the cipher in bytes.
 * @return          SM9_SUCCESS if the operation is successful, other values indicate an error.
 * @note            
 *        1. IDB only represents the decryptor.
 *        2. K2_bytes should be less than SM9_MAX_ENC_K2_BYTE_LEN.
 */
    unsigned int sm9_enc(const unsigned char *IDB, unsigned int IDB_bytes, unsigned char hid, const unsigned char *M, unsigned int M_bytes, const unsigned char *fp12g,
                         const unsigned char Ppub_e[64], const unsigned char r[32], sm9_enc_type_e enc_type, sm9_enc_padding_e padding_type, unsigned int K2_bytes,
                         unsigned char *C, unsigned int *C_bytes);

    /**
 * @brief           SM9 decrypt.
 * @param[in]       IDB                  - Pointer to an array containing the user identifier IDB (user B is the cipher receiver).
 * @param[in]       IDB_bytes            - Length of the user identifier IDB in bytes.
 * @param[in]       C                    - Pointer to an array containing the cipher.
 * @param[in]       C_bytes              - Length of the cipher in bytes.
 * @param[in]       deB                  - User B's private key.
 * @param[in]       enc_type             - Type of encryption (SM9_ENC_KDF_STREAM_CIPHER or SM9_ENC_KDF_BLOCK_CIPHER).
 * @param[in]       padding_type         - Type of padding (SKE_NO_PADDING or SKE_PKCS_5_7_PADDING).
 * @param[in]       K2_bytes             - Length of the key in MAC function in bytes.
 * @param[out]      M                    - Pointer to an array where the plaintext message will be stored.
 * @param[out]      M_bytes              - Length of the plaintext message in bytes.
 * @return          SM9_SUCCESS if the operation is successful, other values indicate an error.
 * @note            
 *        1. Ensure that all input parameters are correctly provided and valid.
 */
    unsigned int sm9_dec(const unsigned char *IDB, unsigned int IDB_bytes, const unsigned char *C, unsigned int C_bytes, const unsigned char deB[128], sm9_enc_type_e enc_type,
                         sm9_enc_padding_e padding_type, unsigned int K2_bytes, unsigned char *M, unsigned int *M_bytes);

    /**
 * @brief           Generate KGC's master public key from master private key for SM9 key-exchange system.
 * @param[in]       ke                   - Pointer to an array containing the KGC's master private key, 32 bytes, big-endian.
 * @param[out]      Ppub_e               - Pointer to an array where the KGC's master public key (x||y) will be stored, 64 bytes, big-endian.
 * @return          SM9_SUCCESS if the operation is successful, other values indicate an error.
 */
    unsigned int sm9_exckey_gen_mastPubKey_from_mastPriKey(const unsigned char ke[32], unsigned char Ppub_e[64]);

    /**
 * @brief           Generate random KGC's master key pair for SM9 key-exchange system.
 * @param[out]      ke                   - Pointer to an array where the KGC's master private key will be stored, 32 bytes, big-endian.
 * @param[out]      Ppub_e               - Pointer to an array where the KGC's master public key (x||y) will be stored, 64 bytes, big-endian.
 * @return          SM9_SUCCESS if the operation is successful, other values indicate an error.
 */
    unsigned int sm9_exckey_gen_mastKeyPair(unsigned char ke[32], unsigned char Ppub_e[64]);

    /**
 * @brief           Generate user's private key for SM9 key-exchange system.
 * @param[in]       IDA                  - Pointer to an array containing the identify of user A.
 * @param[in]       IDA_bytes            - Length of the user identifier IDA in bytes.
 * @param[in]       hid                  - User private key generation function identity, published by KGC. Default value is 0x02, one byte.
 * @param[in]       ke                   - Pointer to an array containing the KGC's master private key, 32 bytes, big-endian.
 * @param[out]      deA                  - Pointer to an array where the user A's private key will be stored, 128 bytes, big-endian.
 * @return          SM9_SUCCESS if the operation is successful, other values indicate an error.
 */
    unsigned int sm9_exckey_gen_userPriKey(const unsigned char *IDA, unsigned int IDA_bytes, unsigned char hid, const unsigned char ke[32], unsigned char deA[128]);

    /**
 * @brief           Generate user's temporary public key from private key for SM9 key-exchange system.
 * @param[in]       IDB                  - Pointer to an array containing the peer's identifier IDB
 * @param[in]       IDB_bytes            - Length of the peer identifier IDB in bytes
 * @param[in]       hid                  - User private key generation function identity, published by KGC. Default value is 0x02, one byte
 * @param[in]       Ppub_e               - Pointer to an array containing KGC's public key
 * @param[in]       rA                   - Pointer to an array containing local's temporary private key, 32 bytes, big-endian
 * @param[out]      RA                   - Pointer to an array where local's temporary public key (x||y) will be stored, 64 bytes, big-endian
 * @return          SM9_SUCCESS if the operation is successful, other values indicate an error
 * @note            
 *        1. Ensure all input parameters are correctly provided and valid
 *        2. rA must be in the range [1, n-1] where n is the curve order
 */
    unsigned int sm9_exckey_gen_tmpPubKey_from_tmpPriKey(const unsigned char *IDB, unsigned int IDB_bytes, unsigned char hid, const unsigned char Ppub_e[64],
                                                         const unsigned char rA[32], unsigned char RA[64]);
    /**
 * @brief           Generate user's temporary random key pair for SM9 key-exchange system.
 * @param[in]       IDB                  - Pointer to an array containing the peer's identifier IDB
 * @param[in]       IDB_bytes            - Length of the peer identifier IDB in bytes
 * @param[in]       hid                  - User private key generation function identity, published by KGC. Default value is 0x02, one byte
 * @param[in]       Ppub_e               - Pointer to an array containing KGC's public key
 * @param[out]      rA                   - Pointer to an array where local's temporary private key will be stored, 32 bytes, big-endian
 * @param[out]      RA                   - Pointer to an array where local's temporary public key (x||y) will be stored, 64 bytes, big-endian
 * @return          SM9_SUCCESS if the operation is successful, other values indicate an error
 * @note            
 *        1. Ensure all input parameters are correctly provided and valid
 *        2. The generated private key rA will be in the range [1, n-1] where n is the curve order
 */
    unsigned int sm9_exckey_gen_tmpKeyPair(const unsigned char *IDB, unsigned int IDB_bytes, unsigned char hid, const unsigned char Ppub_e[64], unsigned char rA[32],
                                           unsigned char RA[64]);

    /**
 * @brief           SM9 Key exchange
 * @param[in]       role                 - Local user's role (SM9_Role_Sponsor or SM9_Role_Responsor)
 * @param[in]       IDA                  - Pointer to an array containing local user's identity
 * @param[in]       IDA_bytes            - Length of local user's identity in bytes
 * @param[in]       IDB                  - Pointer to an array containing peer user's identity
 * @param[in]       IDB_bytes            - Length of peer user's identity in bytes
 * @param[in]       fp12g                - Pointer to the value of e(Ppub_e, P2). If set to NULL, it will be calculated within the function
 * @param[in]       Ppub_e               - Pointer to an array containing KGC's system encryption master public key
 * @param[in]       deA                  - Pointer to an array containing local user's private key
 * @param[in]       rA                   - Pointer to an array containing local user's temporary private key
 * @param[in]       RA                   - Pointer to an array containing local user's temporary public key
 * @param[in]       RB                   - Pointer to an array containing peer user's temporary public key
 * @param[in]       k_bytes              - Length of the output key in bytes
 * @param[out]      k                    - Pointer to an array where the output key will be stored
 * @param[out]      S1                   - Pointer to an array where sponsor's S1 or responsor's S2 will be stored
 * @param[out]      SA                   - Pointer to an array where sponsor's SA or responsor's SB will be stored
 * @return          SM9_SUCCESS if the operation is successful, other values indicate an error
 * @note            
 *        1. fp12g and Ppub_e cannot be NULL at the same time
 *        2. Ensure all input parameters are correctly provided and valid
 */
    unsigned int sm9_exchangekey(sm9_exchange_role_e role, const unsigned char *IDA, unsigned int IDA_bytes, const unsigned char *IDB, unsigned int IDB_bytes,
                                 const unsigned char *fp12g, const unsigned char Ppub_e[64], const unsigned char deA[128], const unsigned char rA[32], const unsigned char RA[64],
                                 const unsigned char RB[64], unsigned int k_bytes, unsigned char *k, unsigned char S1[32], unsigned char SA[32]);

#ifdef __cplusplus
}
#endif

#endif
