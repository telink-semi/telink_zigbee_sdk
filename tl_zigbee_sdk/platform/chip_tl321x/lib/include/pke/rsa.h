/********************************************************************************************************
 * @file    rsa.h
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


//RSA return code
#define RSA_SUCCESS        PKE_SUCCESS
#define RSA_BUFFER_NULL    (PKE_SUCCESS + 0x30U)
#define RSA_INPUT_TOO_LONG (PKE_SUCCESS + 0x31U)
#define RSA_INPUT_INVALID  (PKE_SUCCESS + 0x32U)


    //APIs

    /**
 * @brief       out = a^e mod n.
 * @param[in]   a            - unsigned int big integer a, base number, make sure a < n.
 * @param[in]   e            - unsigned int big integer e, exponent, make sure e < n.
 * @param[in]   n            - unsigned int big integer n, modulus, make sure n is odd.
 * @param[out]  out          - out = a^e mod n.
 * @param[in]   eBitLen      - real bit length of unsigned int big integer e.
 * @param[in]   nBitLen      - real bit length of unsigned int big integer n.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.a, n, and out have the same word length:((nBitLen+31)>>5); and e word length is (eBitLen+31)>>5.
  @endverbatim
 */
    unsigned int RSA_ModExp(unsigned int *a, unsigned int *e, unsigned int *n, unsigned int *out, unsigned int eBitLen, unsigned int nBitLen);

    /**
 * @brief        out = a^d mod n, here d represents RSA CRT private key (p,q,dp,dq,u).
 * @param[in]   a            - unsigned int big integer a, base number, make sure a < n=pq.
 * @param[in]   p            - unsigned int big integer p, prime number, one part of private key (p,q,dp,dq,u).
 * @param[in]   q            - unsigned int big integer q, prime number, one part of private key (p,q,dp,dq,u).
 * @param[in]   dp           - unsigned int big integer dp = e^(-1) mod (p-1), one part of private key (p,q,dp,dq,u).
 * @param[in]   dq           - unsigned int big integer dq = e^(-1) mod (q-1), one part of private key (p,q,dp,dq,u).
 * @param[in]   u            - unsigned int big integer u = q^(-1) mod p, one part of private key (p,q,dp,dq,u).
 * @param[in]   out          - out = a^d mod n.
 * @param[out]  nBitLen      - real bit length of unsigned int big integer n=pq.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# a and out have the same word length:((nBitLen+31)>>5); and p,p_h,q,q_h,dp,dq,u
         have the same word length:((nBitLen/2+31)>>5).
  @endverbatim
 */
    unsigned int RSA_CRTModExp(unsigned int *a, unsigned int *p, unsigned int *q, unsigned int *dp, unsigned int *dq, unsigned int *u, unsigned int *out, unsigned int nBitLen);

    /**
 * @brief       generate RSA key (e,d,n).
 * @param[out]  e                - unsigned int big integer, RSA public key e.
 * @param[out]  d                - unsigned int big integer, RSA private key d.
 * @param[out]  n                - unsigned int big integer, RSA public module n.
 * @param[in]   eBitLen          - real bit length of e.
 * @param[in]   nBitLen          - real bit length of n.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.nBitLen can not be even.
      -# 2.eBitLen must be larger than 1, and less than or equal to nBitLen.
  @endverbatim
 */
    unsigned int RSA_GetKey(unsigned int *e, unsigned int *d, unsigned int *n, unsigned int eBitLen, unsigned int nBitLen);

    /**
 * @brief       generate RSA-CRT key (e,p,q,dp,dq,u,n).
 * @param[out]  e                - unsigned int big integer, RSA public key e.
 * @param[out]  p                - unsigned int big integer, RSA private key p.
 * @param[out]  q                - unsigned int big integer, RSA private key q.
 * @param[out]  dp               - unsigned int big integer, RSA private key dp.
 * @param[out]  dq               - unsigned int big integer, RSA private key dq.
 * @param[out]  u                - unsigned int big integer, RSA private key u = q^(-1) mod p.
 * @param[out]  n                - unsigned int big integer, RSA public module n.
 * @param[in]   eBitLen          - real bit length of e.
 * @param[in]   nBitLen          - real bit length of n.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.nBitLen can not be even.
      -# 2.eBitLen must be larger than 1, and less than or equal to nBitLen.
  @endverbatim
 */
    unsigned int RSA_GetCRTKey(unsigned int *e, unsigned int *p, unsigned int *q, unsigned int *dp, unsigned int *dq, unsigned int *u, unsigned int *n, unsigned int eBitLen, unsigned int nBitLen);


#ifdef RSA_SEC

    //RSA return code(secure version)
    #define RSA_SUCCESS_S (0x3AEBA318U)
    #define RSA_ERROR_S   (0x45DF3DAEU)


    unsigned int RSA_ModExp_with_pub(unsigned int *a, unsigned int *e, unsigned int *d, unsigned int *n, unsigned int *out, unsigned int eBitLen, unsigned int nBitLen);

    unsigned int RSA_CRTModExp_with_pub(unsigned int *a, unsigned int *p, unsigned int *q, unsigned int *dp, unsigned int *dq, unsigned int *u, unsigned int *e, unsigned int *out, unsigned int eBitLen, unsigned int nBitLen);

#endif


    typedef struct
    {
        unsigned char *p;
        unsigned char *q;
        unsigned char *dp;
        unsigned char *dq;
        unsigned char *u; //qinv
    } RSA_CRT_PRIVATE_KEY;


#if (defined(SUPPORT_RSASSA_PSS) || defined(SUPPORT_RSAES_OAEP))
    void rsa_pkcs1_mgf1_counter_add(unsigned char *counter, unsigned int bytes, unsigned char b);

    /**
 * @brief       RSA PKCS#1_v2.2 MGF1(a mask generation function based on a hash function)
 * @param[in]   hash_alg     - specific hash algorithm for MGF1.
 * @param[in]   seed         - seed.
 * @param[in]   seed_bytes   - byte length of seed.
 * @param[in]   in           - this is to XOR mask, and this could be NULL.
 * @param[out]  out          - if in is NULL, this is mask directly, otherwise,this is (mask XOR in).
 * @param[in]   mask_bytes   - input, if in is NULL, this is byte length of out(mask), otherwise,
 *                             this is byte length of in or out(mask XOR in).
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.out = mask XOR in, if in is NULL, out is mask directly.
  @endverbatim
 */
    unsigned int rsa_pkcs1_mgf1_with_xor_in(HASH_ALG hash_alg, unsigned char *seed, unsigned int seed_bytes, unsigned char *in, unsigned char *out, unsigned int mask_bytes);
#endif


#ifdef SUPPORT_RSASSA_PSS
    /**
 * @brief       RSA PKCS#1_v2.2 RSASSA-PSS-SIGN with message digest
 * @param[in]   msg_hash_alg         - specific hash algorithm for message or Hash(message)
 * @param[in]   mgf_hash_alg         - specific hash algorithm for MGF1
 * @param[in]   salt                 - salt
 * @param[in]   salt_bytes           - byte length of salt
 * @param[in]   msg_digest           - Hash(message), message is to be signed, here Hash is msg_hash_alg.
 * @param[in]   d                    - RSA private key d, (n_bits+7)/8 bytes, big-endian.
 * @param[in]   n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]   n_bits               - bit length of n
 * @param[out]  signature            - RSA signature, (n_bits+7)/8 bytes, big-endian.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.it is recommended that msg_hash_alg and mgf_hash_alg are the same.
      -# 2.if no salt prepared to input, please set salt to NULL, it will be generated inside.
      -# 3.salt_bytes should be in [0, em_bytes-digest_bytes-2], em_bytes is (em_bits+7)/8, em_bits is (n_bits-1).
           it is recommended to use default value, digest length of hash algorithm msg_hash_alg or mgf_hash_alg.
  @endverbatim
 */
    unsigned int rsa_ssa_pss_sign_by_msg_digest(HASH_ALG msg_hash_alg, HASH_ALG mgf_hash_alg, unsigned char *salt, unsigned int salt_bytes, unsigned char *msg_digest, unsigned char *d, unsigned char *n, unsigned int n_bits, unsigned char *signature);

    /**
 * @brief       RSA PKCS#1_v2.2 RSASSA-PSS-SIGN with message digest
 * @param[in]   msg_hash_alg         - specific hash algorithm for message or Hash(message)
 * @param[in]   mgf_hash_alg         - specific hash algorithm for MGF1
 * @param[in]   salt                 - salt
 * @param[in]   salt_bytes           - byte length of salt
 * @param[in]   msg                  - message to be signed
 * @param[in]   msg_digest           - byte length of message
 * @param[in]   d                    - RSA private key d, (n_bits+7)/8 bytes, big-endian.
 * @param[in]   n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]   n_bits               - bit length of n
 * @param[out]  signature            - RSA signature, (n_bits+7)/8 bytes, big-endian.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.it is recommended that msg_hash_alg and mgf_hash_alg are the same.
      -# 2.if no salt prepared to input, please set salt to NULL, it will be generated inside.
      -# 3.salt_bytes should be in [0, em_bytes-digest_bytes-2], em_bytes is (em_bits+7)/8, em_bits is (n_bits-1).
           it is recommended to use default value, digest length of hash algorithm msg_hash_alg or mgf_hash_alg.
  @endverbatim
 */
    unsigned int rsa_ssa_pss_sign(HASH_ALG msg_hash_alg, HASH_ALG mgf_hash_alg, unsigned char *salt, unsigned int salt_bytes, unsigned char *msg, unsigned int msg_bytes, unsigned char *d, unsigned char *n, unsigned int n_bits, unsigned char *signature);

    /**
 * @brief       RSA PKCS#1_v2.2 RSASSA-PSS-SIGN with message digest(private key is CRT style)
 * @param[in]   msg_hash_alg         - specific hash algorithm for message or Hash(message)
 * @param[in]   mgf_hash_alg         - specific hash algorithm for MGF1
 * @param[in]   salt                 - salt
 * @param[in]   salt_bytes           - byte length of salt
 * @param[in]   msg_digest           - Hash(message), message is to be signed, here Hash is msg_hash_alg.
 * @param[in]   d                    - RSA-CRT private key (p,q,dp,dq,u), every field is (n_bits/2+7)/8 bytes, big-endian.
 * @param[out]  signature            - RSA signature, (n_bits+7)/8 bytes, big-endian.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.it is recommended that msg_hash_alg and mgf_hash_alg are the same.
      -# 2.if no salt prepared to input, please set salt to NULL, it will be generated inside.
      -# 3.salt_bytes should be in [0, em_bytes-digest_bytes-2], em_bytes is (em_bits+7)/8, em_bits is (n_bits-1).
           it is recommended to use default value, digest length of hash algorithm msg_hash_alg or mgf_hash_alg.
  @endverbatim
 */
    unsigned int rsa_ssa_pss_crt_sign_by_msg_digest(HASH_ALG msg_hash_alg, HASH_ALG mgf_hash_alg, unsigned char *salt, unsigned int salt_bytes, unsigned char *msg_digest, RSA_CRT_PRIVATE_KEY *d, unsigned int n_bits, unsigned char *signature);

    /**
 * @brief       RSA PKCS#1_v2.2 RSASSA-PSS-SIGN with message(private key is CRT style)
 * @param[in]   msg_hash_alg         - specific hash algorithm for message or Hash(message)
 * @param[in]   mgf_hash_alg         - specific hash algorithm for MGF1
 * @param[in]   salt                 - salt
 * @param[in]   salt_bytes           - byte length of salt
 * @param[in]   msg                  - message to be signed
 * @param[in]   msg_digest           - byte length of message
 * @param[in]   d                    - RSA-CRT private key (p,q,dp,dq,u), every field is (n_bits/2+7)/8 bytes, big-endian.
 * @param[in]   n_bits               - bit length of n
 * @param[out]  signature            - RSA signature, (n_bits+7)/8 bytes, big-endian.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.it is recommended that msg_hash_alg and mgf_hash_alg are the same.
      -# 2.if no salt prepared to input, please set salt to NULL, it will be generated inside.
      -# 3.salt_bytes should be in [0, em_bytes-digest_bytes-2], em_bytes is (em_bits+7)/8, em_bits is (n_bits-1).
           it is recommended to use default value, digest length of hash algorithm msg_hash_alg or mgf_hash_alg.
  @endverbatim
 */
    unsigned int rsa_ssa_pss_crt_sign(HASH_ALG msg_hash_alg, HASH_ALG mgf_hash_alg, unsigned char *salt, unsigned int salt_bytes, unsigned char *msg, unsigned int msg_bytes, RSA_CRT_PRIVATE_KEY *d, unsigned int n_bits, unsigned char *signature);

    /**
 * @brief       RSA PKCS#1_v2.2 RSASSA-PSS-VERIFY with message digest
 * @param[in]   msg_hash_alg         - specific hash algorithm for message or Hash(message)
 * @param[in]   mgf_hash_alg         - specific hash algorithm for MGF1
 * @param[in]   salt_bytes           - byte length of salt
 * @param[in]   msg_digest           - Hash(message), message is to be verified, here Hash is msg_hash_alg.
 * @param[in]   e                    - RSA public key e, (e_bits+7)/8 bytes, big-endian.
 * @param[in]   e_bits               - bit length of e
 * @param[in]   n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]   n_bits               - bit length of n
 * @param[out]  signature            - RSA signature, (n_bits+7)/8 bytes, big-endian.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.it is recommended that msg_hash_alg and mgf_hash_alg are the same.
      -# 2.salt_bytes should be in [0, em_bytes-digest_bytes-2], em_bytes is (em_bits+7)/8, em_bits is (n_bits-1).
           it is recommended to use default value, digest length of hash algorithm msg_hash_alg or mgf_hash_alg.
           if salt_bytes is not known, please set it to -1
  @endverbatim
 */
    unsigned int rsa_ssa_pss_verify_by_msg_digest(HASH_ALG msg_hash_alg, HASH_ALG mgf_hash_alg, int32_t salt_bytes, unsigned char *msg_digest, unsigned char *e, unsigned int e_bits, unsigned char *n, unsigned int n_bits, unsigned char *signature);

    /**
 * @brief       RSA PKCS#1_v2.2 RSASSA-PSS-VERIFY with message
 * @param[in]   msg_hash_alg         - specific hash algorithm for message or Hash(message)
 * @param[in]   mgf_hash_alg         - specific hash algorithm for MGF1
 * @param[in]   salt_bytes           - byte length of salt
 * @param[in]   msg                  - message to be verified
 * @param[in]   msg_digest           - byte length of message
 * @param[in]   e                    - RSA public key e, (e_bits+7)/8 bytes, big-endian.
 * @param[in]   e_bits               - bit length of e
 * @param[in]   n                    - RSA modulus n, (n_bits+7)/8 bytes, big-endian.
 * @param[in]   n_bits               - bit length of n
 * @param[in]   signature            - RSA signature, (n_bits+7)/8 bytes, big-endian.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.it is recommended that msg_hash_alg and mgf_hash_alg are the same.
      -# 2.salt_bytes should be in [0, em_bytes-digest_bytes-2], em_bytes is (em_bits+7)/8, em_bits is (n_bits-1).
           it is recommended to use default value, digest length of hash algorithm msg_hash_alg or mgf_hash_alg.
           if salt_bytes is not known, please set it to -1
  @endverbatim
 */
    unsigned int rsa_ssa_pss_verify(HASH_ALG msg_hash_alg, HASH_ALG mgf_hash_alg, int32_t salt_bytes, unsigned char *msg, unsigned int msg_bytes, unsigned char *e, unsigned int e_bits, unsigned char *n, unsigned int n_bits, unsigned char *signature);
#endif


#ifdef __cplusplus
}
#endif

#endif
