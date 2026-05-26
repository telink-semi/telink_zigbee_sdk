/********************************************************************************************************
 * @file    hash_kdf.h
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
#ifndef HASH_KDF_H
#define HASH_KDF_H

#include "hmac.h"

#ifdef __cplusplus
extern "C"
{
#endif


    //APIs

#ifdef SUPPORT_PBKDF2
    /**
 * @brief           pbkdf2 backup hmac ctx
 * @param[out]      ctx_bak              - hmac ctx
 * @param[in]       ctx                  - hmac ctx to be backup
 * @return          HASH_SUCCESS(success), other(error)
 */
    void pbkdf2_hmac_backup(hmac_ctx_t *ctx_bak, hmac_ctx_t *ctx);

    /**
 * @brief           pbkdf2 recover hmac ctx
 * @param[out]      ctx                  - hmac ctx to be recover
 * @param[in]       ctx_bak              - hmac ctx
 * @return          HASH_SUCCESS(success), other(error)
 */
    void pbkdf2_hmac_recover(hmac_ctx_t *ctx, hmac_ctx_t *ctx_bak);

    /**
 * @brief           pbkdf2 function(using hmac as PRF)
 * @param[in]       alg                  - specific hash algorithm
 * @param[in]       pwd                  - password, as the key of hmac
 * @param[in]       sp_key_idx           - index of secure port key(password)
 * @param[in]       pwd_bytes            - byte length of password, it could be 0
 * @param[in]       salt                 - salt
 * @param[in]       salt_bytes           - byte length of salt, it could be 0
 * @param[in]       iter                 - iteration times
 * @param[out]      out                  - derived key
 * @param[in]       out_bytes            - byte length of derived key
 * @return          HASH_SUCCESS(success), other(error)
 */
    unsigned int pbkdf2_hmac(hash_alg_e alg, const unsigned char *pwd, unsigned short sp_key_idx, unsigned int pwd_bytes, const unsigned char *salt, unsigned int salt_bytes,
                             unsigned int iter, unsigned char *out, unsigned int out_bytes);
#endif


#ifdef SUPPORT_ANSI_X9_63_KDF
#ifdef SUPPORT_HASH_NODE
    /**
 * @brief           out = ansi_x9_63_kdf(msg1||msg2||...||counter||... , out_bytes),        ---- if in is NULL
 *                  out = ansi_x9_63_kdf(msg1||msg2||...||counter||... , out_bytes) XOR in, ---- if in is not NULL
 * @param[in]       alg                  - specific hash algorithm
 * @param[in]       node                 - hash_node_t struct pointer
 * @param[in]       node_num             - number of hash_node_t, or number of message pieces
 * @param[in]       counter              - counter of 4 bytes in node array
 * @param[in]       in                   - input message, if no input message, please set this para as NULL
 * @param[out]      out                  - if in is NULL, out = kdf(...), otherwise out = kdf(...) XOR in.
 * @param[in]       out_bytes            - byte length of out, if in is not NULL, this is also byte length of in
 * @param[in]       check_whether_zero   - 0(not check whether kdf output is zero or not),
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure alg is valid
 *        2. node[counter_idx] holds counter of 4 bytes.
 *        3. if in is NULL, out = kdf(...), otherwise out = kdf(...) XOR in.
 *        4. for ansi x9.63, initial counter is 1; for PKCS#1 RSA MGF, initial counter is 0
 */
    unsigned int ansi_x9_63_kdf_node_with_xor_in(hash_alg_e alg, const hash_node_t *node, unsigned int node_num, unsigned char *counter, const unsigned char *in,
                                                 unsigned char *out, unsigned int out_bytes, unsigned int check_whether_zero);

    /**
 * @brief           key = ansi_x9_63_kdf(msg1||msg2||...||counter||..., key_bytes).
 * @param[in]       alg                  - specific hash algorithm
 * @param[in]       node                 - hash_node_t struct pointer
 * @param[in]       node_num             - number of hash_node_t, or number of message pieces
 * @param[in]       counter              - counter of 4 bytes in node array
 * @param[out]      key                  - key
 * @param[in]       key_bytes            - byte length of output key
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure alg is valid
 *        2. node[counter_idx] holds counter of 4 bytes.
 *        3. here key_bytes must be a multiple of alg digest byte length
 */
    unsigned int ansi_x9_63_kdf_internal(hash_alg_e alg, const hash_node_t *node, unsigned int node_num, unsigned char *counter, unsigned char *key, unsigned int key_bytes);

    /**
 * @brief           k1||k2 = ansi_x9_63_kdf(msg1||msg2||...||counter||... , k1_bytes + k2_bytes).
 * @param[in]       alg                  - specific hash algorithm
 * @param[in]       node                 - hash_node_t struct pointer
 * @param[in]       node_num             - number of hash_node_t, or number of message pieces
 * @param[in]       counter              - counter of 4 bytes in node array
 * @param[out]      k1                   - k1 part
 * @param[in]       k1_bytes             - byte length of k1
 * @param[out]      k2                   - k2 part
 * @param[in]       k2_bytes             - byte length of k2
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure alg is valid
 *        2. node[counter_idx] holds counter of 4 bytes.
 *        3. k1 can not be NULL, but k2 can
 */
    unsigned int ansi_x9_63_kdf_node(hash_alg_e alg, const hash_node_t *node, unsigned int node_num, unsigned char *counter, unsigned char *k1, unsigned int k1_bytes,
                                     unsigned char *k2, unsigned int k2_bytes);

    /**
 * @brief           k1||k2 = ansi_x9_63_kdf(Z||counter||shared_info , k1_bytes + k2_bytes).
 * @param[in]       alg                  - specific hash algorithm
 * @param[in]       Z                    - byte string
 * @param[in]       Z_bytes              - byte length of Z
 * @param[in]       shared_info          - shared info
 * @param[in]       shared_info_bytes    - byte length of shared info
 * @param[out]      k1                   - k1 part
 * @param[in]       k1_bytes             - byte length of k1
 * @param[out]      k2                   - k2 part
 * @param[in]       k2_bytes             - byte length of k2
 * @return          HASH_SUCCESS(success), other(error)
 * @note            
 *        1. please make sure alg is valid
 *        2. k1 can not be NULL, but k2 can
 */
    unsigned int ansi_x9_63_kdf(hash_alg_e alg, const unsigned char *Z, unsigned int Z_bytes, const unsigned char *shared_info, unsigned int shared_info_bytes, unsigned char *k1,
                                unsigned int k1_bytes, unsigned char *k2, unsigned int k2_bytes);
#endif
#endif


#ifdef __cplusplus
}
#endif


#endif
