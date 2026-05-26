/********************************************************************************************************
 * @file    rsa.h
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
#ifndef RSA_H
#define RSA_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "lib/include/pke/pke.h"


#if defined(SUPPORT_RSASSA_PSS)
    #include "lib/include/hash/hash.h"
#endif


/**
 * @brief           RSA return code
 */
#define RSA_SUCCESS        PKE_SUCCESS
#define RSA_BUFFER_NULL    (PKE_SUCCESS + 0x30U)
#define RSA_INPUT_TOO_LONG (PKE_SUCCESS + 0x31U)
#define RSA_INPUT_INVALID  (PKE_SUCCESS + 0x32U)
#define RSA_ERROR          (PKE_SUCCESS + 0x33U)

    /**
 * @brief           APIs
 */

    /**
 * @brief           out = a^e mod n.
 * @param[in]       a                    - unsigned int big integer a, base number, make sure a < n.
 * @param[in]       e                    - unsigned int big integer e, exponent, make sure e < n.
 * @param[in]       n                    - unsigned int big integer n, modulus, make sure n is odd.
 * @param[out]      out                  - out = a^e mod n.
 * @param[in]       eBitLen              - real bit length of unsigned int big integer e.
 * @param[in]       nBitLen              - real bit length of unsigned int big integer n.
 * @return          0:success     other:error
 * @note            
 *        1.a, n, and out have the same word length:((nBitLen+31)>>5); and e word length is (eBitLen+31)>>5.
 */
    unsigned int RSA_ModExp(const unsigned int *a, const unsigned int *e, const unsigned int *n, unsigned int *out, unsigned int eBitLen, unsigned int nBitLen);

    /**
 * @brief           out = a^d mod n, here d represents RSA CRT private key (p,q,dp,dq,u).
 * @param[in]       a                    - unsigned int big integer a, base number, make sure a < n=pq.
 * @param[in]       p                    - unsigned int big integer p, prime number, one part of private key (p,q,dp,dq,u).
 * @param[in]       q                    - unsigned int big integer q, prime number, one part of private key (p,q,dp,dq,u).
 * @param[in]       dp                   - unsigned int big integer dp = e^(-1) mod (p-1), one part of private key (p,q,dp,dq,u).
 * @param[in]       dq                   - unsigned int big integer dq = e^(-1) mod (q-1), one part of private key (p,q,dp,dq,u).
 * @param[in]       u                    - unsigned int big integer u = q^(-1) mod p, one part of private key (p,q,dp,dq,u).
 * @param[in]       out                  - out = a^d mod n.
 * @param[out]      nBitLen              - real bit length of unsigned int big integer n=pq.
 * @return          0:success     other:error
 * @note            
 *        1.a and out have the same word length:((nBitLen+31)>>5); and p,p_h,q,q_h,dp,dq,u have the same word length:((nBitLen/2+31)>>5).
 */
    unsigned int RSA_CRTModExp(const unsigned int *a, const unsigned int *p, const unsigned int *q, const unsigned int *dp, const unsigned int *dq, const unsigned int *u,
                               unsigned int *out, unsigned int nBitLen);

    /**
 * @brief           generate RSA key (e,d,n).
 * @param[out]      e                    - unsigned int big integer, RSA public key e.
 * @param[out]      d                    - unsigned int big integer, RSA private key d.
 * @param[out]      n                    - unsigned int big integer, RSA public module n.
 * @param[in]       eBitLen              - real bit length of e.
 * @param[in]       nBitLen              - real bit length of n.
 * @return          0:success     other:error
 * @note            
 *        1.nBitLen can not be even.
 *        2.eBitLen must be larger than 1, and less than or equal to nBitLen.
 */
    unsigned int RSA_GetKey(unsigned int *e, unsigned int *d, unsigned int *n, unsigned int eBitLen, unsigned int nBitLen);

    /**
 * @brief           Generate RSA-CRT key (e,p,q,dp,dq,u,n)
 * @param[out]      e                    - unsigned int big integer, RSA public key e
 * @param[out]      p                    - unsigned int big integer, RSA private key p
 * @param[out]      q                    - unsigned int big integer, RSA private key q
 * @param[out]      dp                   - unsigned int big integer, RSA private key dp
 * @param[out]      dq                   - unsigned int big integer, RSA private key dq
 * @param[out]      u                    - unsigned int big integer, RSA private key u = q^(-1) mod p
 * @param[out]      n                    - unsigned int big integer, RSA public module n
 * @param[in]       eBitLen              - Real bit length of e
 * @param[in]       nBitLen              - Real bit length of n
 * @return          RSA_SUCCESS on success, other values indicate error
 * @note            
 *        1. nBitLen cannot be even
 *        2. eBitLen must be greater than 1, and less than or equal to nBitLen
 *        3. If eBitLen is 2,5,17, here makes e as 3,17,65537 respectively, otherwise e is random
*/
    unsigned int RSA_GetCRTKey(unsigned int *e, unsigned int *p, unsigned int *q, unsigned int *dp, unsigned int *dq, unsigned int *u, unsigned int *n, unsigned int eBitLen,
                               unsigned int nBitLen);

    typedef struct
    {
        unsigned char *p;
        unsigned char *q;
        unsigned char *dp;
        unsigned char *dq;
        unsigned char *u; //qinv
    } rsa_crt_private_key_t;

    // fix to old project
    typedef rsa_crt_private_key_t RSA_CRT_PRIVATE_KEY;

#if (defined(SUPPORT_RSASSA_PSS) || defined(SUPPORT_RSAES_OAEP))
    /**
 * @brief           RSA PKCS#1_v2.2 MGF1(a mask generation function based on a hash function)
 * @param[in]       hash_alg             - specific hash algorithm for MGF1.
 * @param[in]       seed                 - seed.
 * @param[in]       seed_bytes           - byte length of seed.
 * @param[in]       in                   - this is to XOR mask, and this could be NULL.
 * @param[out]      out                  - if in is NULL, this is mask directly, otherwise,this is (mask XOR in).
 * @param[in]       mask_bytes           - input, if in is NULL, this is byte length of out(mask), otherwise,
 *                                         this is byte length of in or out(mask XOR in).
 * @return          0:success     other:error
 * @note            
 *        1.out = mask XOR in, if in is NULL, out is mask directly.
 */
    unsigned int rsa_pkcs1_mgf1_with_xor_in(hash_alg_e hash_alg, const unsigned char *seed, unsigned int seed_bytes, const unsigned char *in, unsigned char *out,
                                            unsigned int mask_bytes);
#endif

#ifdef SUPPORT_RSASSA_PSS
    /**
 * @brief           RSA PKCS#1_v2.2 RSASSA-PSS-SIGN with message digest
 * @param[in]       msg_hash_alg         - specific hash algorithm for message or Hash(message)
 * @param[in]       mgf_hash_alg         - specific hash algorithm for MGF1
 * @param[in]       salt                 - salt
 * @param[in]       salt_bytes           - byte length of salt
 * @param[in]       msg_digest           - Hash(message), message is to be signed, here Hash is msg_hash_alg.
 * @param[in]       d                    - RSA private key d, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n_bits               - bit length of n
 * @param[out]      signature            - RSA signature, (n_bits+7)/8 bytes, big-endian.
 * @return          0:success     other:error
 * @note            
 *        1.it is recommended that msg_hash_alg and mgf_hash_alg are the same.
 *        2.if no salt prepared to input, please set salt to NULL, it will be generated inside.
 *        3.salt_bytes should be in [0, em_bytes-digest_bytes-2], em_bytes is
 *           (em_bits+7)/8, em_bits is (n_bits-1). it is recommended to use default value,
 *           digest length of hash algorithm msg_hash_alg or mgf_hash_alg.
 */
    unsigned int rsa_ssa_pss_sign_by_msg_digest(hash_alg_e msg_hash_alg, hash_alg_e mgf_hash_alg, const unsigned char *salt, unsigned int salt_bytes,
                                                const unsigned char *msg_digest, const unsigned char *d, const unsigned char *n, unsigned int n_bits, unsigned char *signature);

    /**
 * @brief           RSA PKCS#1_v2.2 RSASSA-PSS-SIGN with message digest
 * @param[in]       msg_hash_alg         - specific hash algorithm for message or Hash(message)
 * @param[in]       mgf_hash_alg         - specific hash algorithm for MGF1
 * @param[in]       salt                 - salt
 * @param[in]       salt_bytes           - byte length of salt
 * @param[in]       msg                  - message to be signed
 * @param[in]       msg_len              - byte length of message
 * @param[in]       d                    - RSA private key d, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n_bits               - bit length of n
 * @param[out]      signature            - RSA signature, (n_bits+7)/8 bytes, big-endian.
 * @return          0:success     other:error
 * @note            
 *        1.it is recommended that msg_hash_alg and mgf_hash_alg are the same.
 *        2.if no salt prepared to input, please set salt to NULL, it will be
 *           generated inside.
 *        3.salt_bytes should be in [0, em_bytes-digest_bytes-2], em_bytes is
 *           (em_bits+7)/8, em_bits is (n_bits-1). it is recommended to use default value,
 *           digest length of hash algorithm msg_hash_alg or mgf_hash_alg.
 */
    unsigned int rsa_ssa_pss_sign(hash_alg_e msg_hash_alg, hash_alg_e mgf_hash_alg, const unsigned char *salt, unsigned int salt_bytes, const unsigned char *msg,
                                  unsigned int msg_len, const unsigned char *d, const unsigned char *n, unsigned int n_bits, unsigned char *signature);

    /**
 * @brief           RSA PKCS#1_v2.2 RSASSA-PSS-SIGN with message digest(private key is CRT style)
 * @param[in]       msg_hash_alg         - specific hash algorithm for message or Hash(message)
 * @param[in]       mgf_hash_alg         - specific hash algorithm for MGF1
 * @param[in]       salt                 - salt
 * @param[in]       salt_bytes           - byte length of salt
 * @param[in]       msg_digest           - Hash(message), message is to be signed, here Hash is msg_hash_alg.
 * @param[in]       d                    - RSA-CRT private key (p,q,dp,dq,u), every field is (n_bits/2+7)/8 bytes, big-endian.
 * @param[in]       n_bits               - bit length of n
 * @param[out]      signature            - RSA signature, (n_bits+7)/8 bytes, big-endian.
 * @return          0:success     other:error
 * @note            
 *        1.it is recommended that msg_hash_alg and mgf_hash_alg are the same.
 *        2.if no salt prepared to input, please set salt to NULL, it will be
 *           generated inside.
 *        3.salt_bytes should be in [0, em_bytes-digest_bytes-2], em_bytes is
 *           (em_bits+7)/8, em_bits is (n_bits-1). it is recommended to use default value,
 *           digest length of hash algorithm msg_hash_alg or mgf_hash_alg.
 */
    unsigned int rsa_ssa_pss_crt_sign_by_msg_digest(hash_alg_e msg_hash_alg, hash_alg_e mgf_hash_alg, const unsigned char *salt, unsigned int salt_bytes,
                                                    const unsigned char *msg_digest, const rsa_crt_private_key_t *d, unsigned int n_bits, unsigned char *signature);

    /**
 * @brief           RSA PKCS#1_v2.2 RSASSA-PSS-SIGN with message(private key is CRT style)
 * @param[in]       msg_hash_alg         - specific hash algorithm for message or Hash(message)
 * @param[in]       mgf_hash_alg         - specific hash algorithm for MGF1
 * @param[in]       salt                 - salt
 * @param[in]       salt_bytes           - byte length of salt
 * @param[in]       msg                  - message to be signed
 * @param[in]       msg_len              - byte length of message
 * @param[in]       d                    - RSA-CRT private key (p,q,dp,dq,u), every field is (n_bits/2+7)/8 bytes, big-endian.
 * @param[in]       n_bits               - bit length of n
 * @param[out]      signature            - RSA signature, (n_bits+7)/8 bytes, big-endian.
 * @return          0:success     other:error
 * @note            
 *        1.it is recommended that msg_hash_alg and mgf_hash_alg are the same.
 *        2.if no salt prepared to input, please set salt to NULL, it will be
 *           generated inside.
 *        3.salt_bytes should be in [0, em_bytes-digest_bytes-2], em_bytes is
 *           (em_bits+7)/8, em_bits is (n_bits-1). it is recommended to use default value,
 *           digest length of hash algorithm msg_hash_alg or mgf_hash_alg.
 */
    unsigned int rsa_ssa_pss_crt_sign(hash_alg_e msg_hash_alg, hash_alg_e mgf_hash_alg, const unsigned char *salt, unsigned int salt_bytes, const unsigned char *msg,
                                      unsigned int msg_len, const rsa_crt_private_key_t *d, unsigned int n_bits, unsigned char *signature);

    /**
 * @brief           RSA PKCS#1_v2.2 RSASSA-PSS-VERIFY with message digest
 * @param[in]       msg_hash_alg         - specific hash algorithm for message or Hash(message)
 * @param[in]       mgf_hash_alg         - specific hash algorithm for MGF1
 * @param[in]       salt_bytes           - byte length of salt
 * @param[in]       msg_digest           - Hash(message), message is to be verified, here Hash is msg_hash_alg.
 * @param[in]       e                    - RSA public key e, (e_bits+7)/8 bytes, big-endian.
 * @param[in]       e_bits               - bit length of e
 * @param[in]       n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n_bits               - bit length of n
 * @param[out]      signature            - RSA signature, (n_bits+7)/8 bytes, big-endian.
 * @return          0:success     other:error
 * @note            
 *        1.it is recommended that msg_hash_alg and mgf_hash_alg are the same.
 *        2.salt_bytes should be in [0, em_bytes-digest_bytes-2], em_bytes is
 *           (em_bits+7)/8, em_bits is (n_bits-1). it is recommended to use default value,
 *           digest length of hash algorithm msg_hash_alg or mgf_hash_alg. if salt_bytes is
 *           not known, please set it to -1
 */
    unsigned int rsa_ssa_pss_verify_by_msg_digest(hash_alg_e msg_hash_alg, hash_alg_e mgf_hash_alg, int32_t salt_bytes, const unsigned char *msg_digest, const unsigned char *e,
                                                  unsigned int e_bits, const unsigned char *n, unsigned int n_bits, const unsigned char *signature);

    /**
 * @brief           RSA PKCS#1_v2.2 RSASSA-PSS-VERIFY with message
 * @param[in]       msg_hash_alg         - specific hash algorithm for message or Hash(message)
 * @param[in]       mgf_hash_alg         - specific hash algorithm for MGF1
 * @param[in]       salt_bytes           - byte length of salt
 * @param[in]       msg                  - message to be verified
 * @param[in]       msg_len              - byte length of message
 * @param[in]       e                    - RSA public key e, (e_bits+7)/8 bytes, big-endian.
 * @param[in]       e_bits               - bit length of e
 * @param[in]       n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n_bits               - bit length of n
 * @param[in]       signature            - RSA signature, (n_bits+7)/8 bytes, big-endian.
 * @return          0:success     other:error
 * @note            
 *        1.it is recommended that msg_hash_alg and mgf_hash_alg are the same.
 *        2.salt_bytes should be in [0, em_bytes-digest_bytes-2], em_bytes is
 *           (em_bits+7)/8, em_bits is (n_bits-1). it is recommended to use default value,
 *           digest length of hash algorithm msg_hash_alg or mgf_hash_alg. if salt_bytes is
 *           not known, please set it to -1
 */
    unsigned int rsa_ssa_pss_verify(hash_alg_e msg_hash_alg, hash_alg_e mgf_hash_alg, int32_t salt_bytes, const unsigned char *msg, unsigned int msg_len, const unsigned char *e,
                                    unsigned int e_bits, const unsigned char *n, unsigned int n_bits, const unsigned char *signature);
#endif

#ifdef SUPPORT_RSASSA_PKCS1_V1_5
    /**
 * @brief           RSA PKCS#1_v2.2 EMSA-PKCS1-v1_5 encoding with message digest
 * @param[in]       alg                  - specific hash algorithm for message or Hash(message)
 * @param[in]       msg_digest           - Hash(message), message is to be signed, here Hash is msg_hash_alg.
 * @param[out]      em                   - big integer to be signed, big-endian.
 * @param[in]       em_bytes             - byte length of em, should be bit length of RSA modulus n minus 1.
 * @return          PKE_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure hash alg is valid
 */
    unsigned int emsa_pkcs_v1_5_encode_by_msg_digest(hash_alg_e alg, const unsigned char *msg_digest, unsigned char *em, unsigned int em_bytes);

    /**
 * @brief           RSA PKCS#1_v2.2 EMSA-PKCS1-v1_5 encoding with message
 * @param[in]       alg                  - specific hash algorithm for message or Hash(message)
 * @param[in]       msg                  - message to be encoded
 * @param[in]       msg_len              - byte length of message
 * @param[out]      em                   - big integer to be signed, big-endian.
 * @param[in]       em_bytes             - byte length of em, should be bit length of RSA modulus n minus 1.
 * @return          PKE_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure hash alg is valid
 */
    unsigned int emsa_pkcs_v1_5_encode(hash_alg_e alg, const unsigned char *msg, unsigned int msg_len, unsigned char *em, unsigned int em_bytes);

    /**
 * @brief           RSA PKCS#1_v2.2 RSASSA-PKCS1-v1_5 with message digest
 * @param[in]       alg                  - specific hash algorithm for message or Hash(message)
 * @param[in]       msg_digest           - Hash(message), message is to be verified, here Hash is msg_hash_alg.
 * @param[in]       d                    - RSA private key d, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n_bits               - bit length of n
 * @param[out]      signature            - RSA signature, (n_bits+7)/8 bytes, big-endian.
 * @return          PKE_SUCCESS(success), other(error)
 */
    unsigned int rsa_ssa_pkcs1_v1_5_sign_by_msg_digest(hash_alg_e alg, const unsigned char *msg_digest, const unsigned char *d, const unsigned char *n, unsigned int n_bits,
                                                       unsigned char *signature);

    /**
 * @brief           RSA PKCS#1_v2.2 RSASSA-PKCS1-v1_5 with message digest
 * @param[in]       alg                  - specific hash algorithm for message or Hash(message)
 * @param[in]       msg_digest           - Hash(message), message is to be verified, here Hash is msg_hash_alg.
 * @param[in]       d                    - RSA private key d, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n_bits               - bit length of n
 * @param[out]      signature            - RSA signature, (n_bits+7)/8 bytes, big-endian.
 * @return          PKE_SUCCESS(success), other(error)
 */
    unsigned int rsa_ssa_pkcs1_v1_5_crt_sign_by_msg_digest(hash_alg_e alg, const unsigned char *msg_digest, const rsa_crt_private_key_t *d, const unsigned char *n,
                                                           unsigned int n_bits, unsigned char *signature);

    /**
 * @brief           RSA PKCS#1_v2.2 RSASSA-PKCS1-v1_5 with message
 * @param[in]       alg                  - specific hash algorithm for message or Hash(message)
 * @param[in]       msg                  - message to be signed
 * @param[in]       msg_len              - byte length of message
 * @param[in]       d                    - RSA private key d, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n_bits               - bit length of n
 * @param[out]      signature            - RSA signature, (n_bits+7)/8 bytes, big-endian.
 * @return          PKE_SUCCESS(success), other(error)
 * @note            
 *           - msg_len should in [0, em_bytes-t_bytes-11], em_bytes is
 *           (em_bits+7)/8, t_bytes=hash_bytes+hashAlgID_bytes, hashAlgID_bytes refer to
 *           get_pkcs1_v1_5_t_prefix().
 */
    unsigned int rsa_ssa_pkcs1_v1_5_sign(hash_alg_e alg, const unsigned char *msg, unsigned int msg_len, const unsigned char *d, const unsigned char *n, unsigned int n_bits,
                                         unsigned char *signature);

    /**
 * @brief           RSA PKCS#1_v2.2 RSASSA-PKCS1-v1_5 with message (private key is CRT style)
 * @param[in]       alg                  - specific hash algorithm for message or Hash(message)
 * @param[in]       msg                  - message to be signed
 * @param[in]       msg_len              - byte length of message
 * @param[in]       d                    - RSA-CRT private key (p,q,dp,dq,u), every field is (n_bits/2+7)/8 bytes, big-endian.
 * @param[in]       n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n_bits               - bit length of n
 * @param[out]      signature            - RSA signature, (n_bits+7)/8 bytes, big-endian.
 * @return          PKE_SUCCESS(success), other(error)
 * @note            
 *        1. msg_len should in [0, em_bytes-t_bytes-11], em_bytes is
 *           (em_bits+7)/8, t_bytes=hash_bytes+hashAlgID_bytes, hashAlgID_bytes refer to
 *           get_pkcs1_v1_5_t_prefix().
 */
    unsigned int rsa_ssa_pkcs1_v1_5_crt_sign(hash_alg_e alg, const unsigned char *msg, unsigned int msg_len, const rsa_crt_private_key_t *d, const unsigned char *n,
                                             unsigned int n_bits, unsigned char *signature);

    /**
 * @brief           RSA PKCS#1_v2.2 RSASSA-PKCS1-v1_5 with message digest
 * @param[in]       alg                  - specific hash algorithm for message or Hash(message)
 * @param[in]       msg_digest           - Hash(message), message is to be signed, here Hash is msg_hash_alg.
 * @param[in]       e                    - RSA public key e, (e_bits+7)/8 bytes, big-endian.
 * @param[in]       e_bits               - bit length of e
 * @param[in]       n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n_bits               - bit length of n
 * @param[in]       signature            - RSA signature, (n_bits+7)/8 bytes, big-endian.
 * @return          PKE_SUCCESS(success), other(error)
 * @note            
 *        1. msg_len should in [0, em_bytes-t_bytes-11], em_bytes is
 *           (em_bits+7)/8, t_bytes=hash_bytes+hashAlgID_bytes, hashAlgID_bytes refer to
 *           get_pkcs1_v1_5_t_prefix().
 */
    unsigned int rsa_ssa_pkcs1_v1_5_verify_by_msg_digest(hash_alg_e alg, const unsigned char *msg_digest, const unsigned char *e, unsigned int e_bits, const unsigned char *n,
                                                         unsigned int n_bits, const unsigned char *signature);

    /**
 * @brief           RSA PKCS#1_v2.2 RSASSA-PKCS1-v1_5 with message
 * @param[in]       alg                  - specific hash algorithm for message or Hash(message)
 * @param[in]       msg                  - message to be verified
 * @param[in]       msg_len              - byte length of message
 * @param[in]       e                    - RSA public key e, (e_bits+7)/8 bytes, big-endian.
 * @param[in]       e_bits               - bit length of e
 * @param[in]       n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n_bits               - bit length of n
 * @param[in]       signature            - RSA signature, (n_bits+7)/8 bytes, big-endian.
 * @return          PKE_SUCCESS(success), other(error)
 * @note            
 *        1. msg_len should in [0, em_bytes-t_bytes-11], em_bytes is
 *           (em_bits+7)/8, t_bytes=hash_bytes+hashAlgID_bytes, hashAlgID_bytes refer to
 *           get_pkcs1_v1_5_t_prefix().
 */
    unsigned int rsa_ssa_pkcs1_v1_5_verify(hash_alg_e alg, const unsigned char *msg, unsigned int msg_len, const unsigned char *e, unsigned int e_bits, const unsigned char *n,
                                           unsigned int n_bits, const unsigned char *signature);
#endif

#ifdef SUPPORT_RSAES_OAEP
    /**
 * @brief           RSA PKCS#1_v2.2 RSAES-OAEP encode with label digest
 * @param[in]       label_hash_alg       - specific hash algorithm for message or Hash(label)
 * @param[in]       mgf_hash_alg         - specific hash algorithm for MGF1
 * @param[in]       label_digest         - label digest of label_hash_alg
 * @param[in]       seed                 - Seed, occupy digest bytes of label_hash_alg (can be NULL to generate inside)
 * @param[in]       msg                  - message to be encode
 * @param[in]       msg_len              - byte length of message
 * @param[out]      em                   - big integer to be encrypt, big-endian.
 * @param[in]       em_bytes             - byte length of em, should be bit length of RSA modulus n
 * @return          RSA_SUCCESS(success), other(error)
 * @note            
 *        1. it is recommended that label_hash_alg and mgf_hash_alg are the same.
 *        2. if no seed is prepared to input, please set seed to NULL, it will be generated inside.
 *        3. msg_len should be in [0, em_bytes-2*hLen-2], where em_bytes is
 *           (n_bits+7)/8 and hLen is digest length of hash algorithm label_hash_alg. It
 *           is recommended to use the default value, which is the digest length of the
 *           hash algorithm label_hash_alg or mgf_hash_alg.
*/
    unsigned int eme_oaep_encode_by_label_digest(hash_alg_e label_hash_alg, hash_alg_e mgf_hash_alg, const unsigned char *label_digest, const unsigned char *seed,
                                                 const unsigned char *msg, unsigned int msg_len, unsigned char *em, unsigned int em_bytes);

    /**
 * @brief           RSA PKCS#1_v2.2 RSAES-OAEP encode
 * @param[in]       label_hash_alg       - Specific hash algorithm for message or Hash(label)
 * @param[in]       mgf_hash_alg         - Specific hash algorithm for MGF1
 * @param[in]       label                - Label (can be NULL if no label is provided)
 * @param[in]       label_bytes          - Byte length of label (0 if label is NULL)
 * @param[in]       seed                 - Seed, occupy digest bytes of label_hash_alg (can be NULL to generate inside)
 * @param[in]       msg                  - Message to be encoded
 * @param[in]       msg_len              - Byte length of message
 * @param[out]      em                   - big integer to be encrypt, big-endian.
 * @param[in]       em_bytes             - byte length of em, should be bit length of RSA modulus n
 * @return          RSA_SUCCESS on success, other values indicate error
 * @note            
 *        1. It is recommended that label_hash_alg and mgf_hash_alg are the same
 *        2. If no label is prepared to input, please set label to NULL
 *        3. If no seed is prepared to input, please set seed to NULL, it will be generated inside
 *        4. msg_len should be in [1, em_bytes-2*hLen-2], where em_bytes is (n_bits+7)/8 and hLen is
 *           digest length of hash algorithm msg_hash_alg. It is recommended to use the default value,
 *           which is the digest length of the hash algorithm msg_hash_alg or mgf_hash_alg
 */
    unsigned int eme_oaep_encode(hash_alg_e label_hash_alg, hash_alg_e mgf_hash_alg, const unsigned char *label, unsigned int label_bytes, const unsigned char *seed,
                                 const unsigned char *msg, unsigned int msg_len, unsigned char *em, unsigned int em_bytes);

    /**
 * @brief           RSA PKCS#1_v2.2 RSAES-OAEP decode with label digest
 * @param[in]       label_hash_alg       - specific hash algorithm for message or Hash(label)
 * @param[in]       mgf_hash_alg         - specific hash algorithm for MGF1
 * @param[in]       label_digest         - label digest of label_hash_alg
 * @param[out]      msg                  - message to be decrypted
 * @param[out]      msg_len              - byte length of the message pointer
 * @param[in]       em                   - big integer to be parsed, big-endian.
 * @param[in]       em_bytes             - byte length of em, should be bit length of RSA modulus n
 * @return          RSA_SUCCESS(success), other(error)
 * @note            
 *        1. it is recommended that label_hash_alg and mgf_hash_alg are the same.
*/
    unsigned int eme_oaep_decode_by_label_digest(hash_alg_e label_hash_alg, hash_alg_e mgf_hash_alg, const unsigned char *label_digest, unsigned char *msg, unsigned int *msg_len,
                                                 const unsigned char *em, unsigned int em_bytes);

    /**
 * @brief           RSA PKCS#1_v2.2 RSAES-OAEP decode
 * @param[in]       label_hash_alg       - specific hash algorithm for message or Hash(label)
 * @param[in]       mgf_hash_alg         - specific hash algorithm for MGF1
 * @param[in]       label                - label
 * @param[in]       label_bytes          - byte length of label
 * @param[out]      msg                  - message to be decrypted
 * @param[out]      msg_len              - byte length of the message pointer
 * @param[in]       em                   - big integer to be parsed, big-endian.
 * @param[in]       em_bytes             - byte length of em, should be bit length of RSA modulus n
 * @return          RSA_SUCCESS(success), other(error)
 * @note            
 *        1. it is recommended that label_hash_alg and mgf_hash_alg are the same.
*/
    unsigned int eme_oaep_decode(hash_alg_e label_hash_alg, hash_alg_e mgf_hash_alg, const unsigned char *label, unsigned int label_bytes, unsigned char *msg,
                                 unsigned int *msg_len, const unsigned char *em, unsigned int em_bytes);

    /**
 * @brief           RSA PKCS#1_v2.2 RSAES-OAEP-ENCRYPT with label digest
 * @param[in]       label_hash_alg       - specific hash algorithm for message or Hash(label)
 * @param[in]       mgf_hash_alg         - specific hash algorithm for MGF1
 * @param[in]       label_digest         - label digest of label_hash_alg
 * @param[in]       seed                 - Seed, occupy digest bytes of label_hash_alg (can be NULL to generate inside)
 * @param[in]       msg                  - message to be encrypt
 * @param[in]       msg_len              - byte length of message
 * @param[in]       e                    - RSA public key e, (e_bits+7)/8 bytes, big-endian.
 * @param[in]       e_bits               - bit length of e
 * @param[in]       n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n_bits               - bit length of n
 * @param[out]      cipher               - RSA cipher, (n_bits+7)/8 bytes, big-endian.
 * @return          RSA_SUCCESS(success), other(error)
 * @note            
 *        1. it is recommended that label_hash_alg and mgf_hash_alg are the same.
 *        2. if no seed is prepared to input, please set seed to NULL, it will be
 *           generated inside.
 *        3. msg_len should be in [1, em_bytes-2*hLen-2], where em_bytes is
 *           (n_bits+7)/8 and hLen is digest length of hash algorithm label_hash_alg. It
 *           is recommended to use the default value, which is the digest length of the
 *           hash algorithm label_hash_alg or mgf_hash_alg.
*/
    unsigned int rsa_es_oaep_enc_by_label_digest(hash_alg_e label_hash_alg, hash_alg_e mgf_hash_alg, const unsigned char *label_digest, const unsigned char *seed,
                                                 const unsigned char *msg, unsigned int msg_len, const unsigned char *e, unsigned int e_bits, const unsigned char *n,
                                                 unsigned int n_bits, unsigned char *cipher);

    /**
 * @brief           RSA PKCS#1_v2.2 RSAES-OAEP-ENCRYPT with message
 * @param[in]       label_hash_alg       - specific hash algorithm for message or Hash(label)
 * @param[in]       mgf_hash_alg         - specific hash algorithm for MGF1
 * @param[in]       label                - label (can be NULL if no label is provided)
 * @param[in]       label_bytes          - byte length of label (0 if label is NULL)
 * @param[in]       seed                 - Seed, occupy digest bytes of label_hash_alg (can be NULL to generate inside)
 * @param[in]       msg                  - message to be encrypt
 * @param[in]       msg_len              - byte length of message
 * @param[in]       e                    - RSA public key e, (e_bits+7)/8 bytes, big-endian.
 * @param[in]       e_bits               - bit length of e
 * @param[in]       n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n_bits               - bit length of n
 * @param[out]      cipher               - RSA cipher, (n_bits+7)/8 bytes, big-endian.
 * @return          RSA_SUCCESS(success), other(error)
 * @note            
 *        1. it is recommended that label_hash_alg and mgf_hash_alg are the same.
 *        2. if no label is prepared to input, please set label to NULL.
 *        3. if no seed is prepared to input, please set seed to NULL, it will be
 *           generated inside.
 *        4. msg_len should be in [0, em_bytes-2*hLen-2], where em_bytes is
 *           (n_bits+7)/8 and hLen is digest length of hash algorithm label_hash_alg. It
 *           is recommended to use the default value, which is the digest length of the
 *           hash algorithm label_hash_alg or mgf_hash_alg.
*/
    unsigned int rsa_es_oaep_enc(hash_alg_e label_hash_alg, hash_alg_e mgf_hash_alg, const unsigned char *label, unsigned int label_bytes, const unsigned char *seed,
                                 const unsigned char *msg, unsigned int msg_len, const unsigned char *e, unsigned int e_bits, const unsigned char *n, unsigned int n_bits,
                                 unsigned char *cipher);

    /**
 * @brief           RSA PKCS#1_v2.2 RSAES-OAEP-DECRYPT with label digest
 * @param[in]       label_hash_alg       - specific hash algorithm for message or Hash(label)
 * @param[in]       mgf_hash_alg         - specific hash algorithm for MGF1
 * @param[in]       label_digest         - label digest of label_hash_alg
 * @param[out]      msg                  - message to be decrypted
 * @param[out]      msg_len              - byte length of the message pointer
 * @param[in]       d                    - RSA private key d, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n_bits               - bit length of n
 * @param[in]       cipher               - RSA cipher, (n_bits+7)/8 bytes, big-endian.
 * @return          RSA_SUCCESS(success), other(error)
 * @note            
 *        1. it is recommended that label_hash_alg and mgf_hash_alg are the same.
*/
    unsigned int rsa_es_oaep_dec_by_label_digest(hash_alg_e label_hash_alg, hash_alg_e mgf_hash_alg, const unsigned char *label_digest, unsigned char *msg, unsigned int *msg_len,
                                                 const unsigned char *d, const unsigned char *n, unsigned int n_bits, const unsigned char *cipher);

    /**
 * @brief           RSA PKCS#1_v2.2 RSAES-OAEP-DECRYPT with message
 * @param[in]       label_hash_alg       - specific hash algorithm for message or Hash(label)
 * @param[in]       mgf_hash_alg         - specific hash algorithm for MGF1
 * @param[in]       label                - label
 * @param[in]       label_bytes          - byte length of label
 * @param[out]      msg                  - message to be decrypted
 * @param[out]      msg_len              - byte length of the message pointer
 * @param[in]       d                    - RSA private key d, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n_bits               - bit length of n
 * @param[in]       cipher               - RSA cipher, (n_bits+7)/8 bytes, big-endian.
 * @return          RSA_SUCCESS(success), other(error)
 * @note            
 *        1. it is recommended that label_hash_alg and mgf_hash_alg are the same.
*/
    unsigned int rsa_es_oaep_dec(hash_alg_e label_hash_alg, hash_alg_e mgf_hash_alg, const unsigned char *label, unsigned int label_bytes, unsigned char *msg,
                                 unsigned int *msg_len, const unsigned char *d, const unsigned char *n, unsigned int n_bits, const unsigned char *cipher);

    /**
 * @brief           RSA PKCS#1_v2.2 RSAES-OAEP-DECRYPT with label digest (private key is CRT style)
 * @param[in]       label_hash_alg       - specific hash algorithm for message or Hash(label)
 * @param[in]       mgf_hash_alg         - specific hash algorithm for MGF1
 * @param[in]       label_digest         - label digest of label_hash_alg
 * @param[out]      msg                  - message to be decrypted
 * @param[out]      msg_len              - byte length of the message pointer
 * @param[in]       d                    - RSA-CRT private key (p, q, dp, dq, u), every field is (n_bits/2+7)/8 bytes, big-endian.
 * @param[in]       n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n_bits               - bit length of n
 * @param[in]       cipher               - RSA cipher, (n_bits+7)/8 bytes, big-endian.
 * @return          RSA_SUCCESS(success), other(error)
 * @note            
 *        1. it is recommended that label_hash_alg and mgf_hash_alg are the same.
*/
    unsigned int rsa_es_oaep_crt_dec_by_label_digest(hash_alg_e label_hash_alg, hash_alg_e mgf_hash_alg, const unsigned char *label_digest, unsigned char *msg,
                                                     unsigned int *msg_len, const rsa_crt_private_key_t *d, const unsigned char *n, unsigned int n_bits,
                                                     const unsigned char *cipher);

    /**
 * @brief           RSA PKCS#1_v2.2 RSAES-OAEP-DECRYPT with message (private key is CRT style)
 * @param[in]       label_hash_alg       - specific hash algorithm for message or Hash(label)
 * @param[in]       mgf_hash_alg         - specific hash algorithm for MGF1
 * @param[in]       label                - label
 * @param[in]       label_bytes          - byte length of label
 * @param[out]      msg                  - message to be decrypted
 * @param[out]      msg_len              - byte length of the message pointer
 * @param[in]       d                    - RSA-CRT private key (p, q, dp, dq, u), every field is (n_bits/2+7)/8 bytes, big-endian.
 * @param[in]       n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n_bits               - bit length of n
 * @param[in]       cipher               - RSA cipher, (n_bits+7)/8 bytes, big-endian.
 * @return          RSA_SUCCESS(success), other(error)
 * @note            
 *        1. it is recommended that label_hash_alg and mgf_hash_alg are the same.
*/
    unsigned int rsa_es_oaep_crt_dec(hash_alg_e label_hash_alg, hash_alg_e mgf_hash_alg, const unsigned char *label, unsigned int label_bytes, unsigned char *msg,
                                     unsigned int *msg_len, const rsa_crt_private_key_t *d, const unsigned char *n, unsigned int n_bits, const unsigned char *cipher);
#endif

#ifdef SUPPORT_RSAES_PKCS1_V1_5
    /**
 * @brief           EME-PKCS1-v1_5 encoding
 * @param[in]       msg                  - message to be encoded
 * @param[in]       msg_len              - byte length of message
 * @param[in]       ps                   - padding octets (set to NULL if not provided)
 * @param[out]      em                   - big integer to be encrypted, big-endian.
 * @param[in]       em_bytes             - byte length of em, should be bit length of RSA modulus n
 * @return          PKE_SUCCESS(success), other(error)
 * @note            
 *        1. if you do not have ps, please set the parameter to be NULL; it will be
 *           generated inside. otherwise, please make sure ps is nonzero octets and it
 *           occupies em_bytes-3-msg_len.
 *        2. em_bytes is (em_bits+7)/8
 */
    unsigned int eme_pkcs1_v1_5_encode(const unsigned char *msg, unsigned int msg_len, const unsigned char *ps, unsigned char *em, unsigned int em_bytes);

    /**
 * @brief           EME-PKCS1-v1_5 decoding
 * @param[out]      msg                  - message to be decoded
 * @param[out]      msg_len              - byte length of message
 * @param[in]       em                   - big integer to be parsed, big-endian.
 * @param[in]       em_bytes             - byte length of em, should be bit length of RSA modulus n
 * @return          PKE_SUCCESS(success), other(error)
 * @note            
 *        1. em_bytes is (em_bits+7)/8
 */
    unsigned int eme_pkcs1_v1_5_decode(unsigned char *msg, unsigned int *msg_len, const unsigned char *em, unsigned int em_bytes);

    /**
 * @brief           RSA PKCS#1_v2.2 RSAES_PKCS1_V1_5 ENCRYPT with message
 * @param[in]       msg                  - message to be encrypted
 * @param[in]       msg_len              - byte length of message
 * @param[in]       ps                   - padding octets (set to NULL if not provided)
 * @param[in]       e                    - RSA public key e, (e_bits+7)/8 bytes, big-endian.
 * @param[in]       e_bits               - bit length of e
 * @param[in]       n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n_bits               - bit length of n
 * @param[out]      cipher               - RSA cipher, (n_bits+7)/8 bytes, big-endian.
 * @return          PKE_SUCCESS(success), other(error)
 * @note            
 *        1. if you do not have ps, please set the parameter to be NULL; it will be
 *           generated inside. otherwise, please make sure ps is nonzero octets and it
 *           occupies em_bytes-3-msg_len.
 *        2. em_bytes is (em_bits+7)/8
 */
    unsigned int rsa_es_pkcs1_v1_5_enc(const unsigned char *msg, unsigned int msg_len, const unsigned char *ps, const unsigned char *e, unsigned int e_bits, const unsigned char *n,
                                       unsigned int n_bits, unsigned char *cipher);

    /**
 * @brief           RSA PKCS#1_v2.2 RSAES_PKCS1_V1_5 DECRYPT with message
 * @param[out]      msg                  - message to be decrypted
 * @param[out]      msg_len              - byte length of message
 * @param[in]       d                    - RSA private key d, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]       n_bits               - bit length of n
 * @param[in]       cipher               - RSA cipher, (n_bits+7)/8 bytes, big-endian.
 * @return          PKE_SUCCESS(success), other(error)
 * @note            
 */
    unsigned int rsa_es_pkcs1_v1_5_dec(unsigned char *msg, unsigned int *msg_len, const unsigned char *d, const unsigned char *n, unsigned int n_bits, const unsigned char *cipher);

    /**
 * @brief           RSAES_PKCS1_V1_5 DECRYPT with message (private key is CRT style)
 * @param[out]      msg                  - message to be decrypted
 * @param[out]      msg_len              - byte length of message
 * @param[in]       d                    - RSA-CRT private key (p,q,dp,dq,u), every field is (n_bits/2+7)/8 bytes, big-endian.
 * @param[in]       n_bits               - bit length of n
 * @param[in]       cipher               - RSA cipher, (n_bits+7)/8 bytes, big-endian.
 * @return          PKE_SUCCESS(success), other(error)
 */
    unsigned int rsa_es_pkcs1_v1_5_crt_dec(unsigned char *msg, unsigned int *msg_len, const rsa_crt_private_key_t *d, unsigned int n_bits, const unsigned char *cipher);
#endif

#ifdef __cplusplus
}
#endif

#endif
