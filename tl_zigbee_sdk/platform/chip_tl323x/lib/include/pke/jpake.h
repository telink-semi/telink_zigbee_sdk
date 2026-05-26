/********************************************************************************************************
 * @file    jpake.h
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
#ifndef JPAKE_H
#define JPAKE_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "lib/include/pke/pke.h"
#include "lib/include/hash/hash.h"


#define JPAKE_MAX_WORD_LEN (2048 / 32)

    typedef struct
    {
        unsigned int *p;
        unsigned int *q;
        unsigned int *g;
        unsigned int *p_h; // less than p, so this occupies the same words as J-PAKE parameter p
        unsigned int *p_n0;
        unsigned int *q_h; // less than q, so this occupies the same words as J-PAKE parameter q
        unsigned int *q_n0;
        unsigned int  p_bitlen;
        unsigned int  qBitLen;
        hash_alg_e    hash_alg;
    } jpake_para_t;

    typedef jpake_para_t JPAKE_PARA;

    /**
 * @brief           ZKP of secret a
 */
    typedef struct
    {
        unsigned int *ga; // A = g^a mod p, less than p, so this occupies the same
                          // words as J-PAKE parameter p
        unsigned int *gv; // V = g^v mod p, less than p, so this occupies the same
                          // words as J-PAKE parameter p
        unsigned int *r;  // r = v - a*c mod q, less than q, so this occupies the same words as
                          // J-PAKE parameter q, and here c = hash(g, gv, ga, ID, otherinfo)
    } jpake_zkp_t;

    typedef jpake_zkp_t JPAKE_ZKP;

    typedef struct
    {
        unsigned char *ID;
        unsigned int   ID_bytes;
        unsigned char *other_info;
        unsigned int   other_info_bytes;
    } jpake_user_info_t;

    typedef jpake_user_info_t JPAKE_USER_INFO;

    typedef struct
    {
        unsigned int *gxa_gxc; // less than p, so this occupies the same words as
                               // J-PAKE parameter p
        unsigned int *xb_s;    // less than q, so this occupies the same words as J-PAKE
                               // parameter q
    } jpake_round2_ctx_t;

    typedef jpake_round2_ctx_t JPAKE_ROUND2_CTX;

    /**
 * @brief           JPAKE return code
 */
    enum JPAKE_RET_CODE
    {
        JPAKE_SUCCESS      = PKE_SUCCESS,
        JPAKE_POINTER_NULL = PKE_SUCCESS + 0x6000,
        JPAKE_VERIFY_ZKP_FAILURE,
        JPAKE_INVALID_INPUT,
        JPAKE_ZERO_ALL,
        JPAKE_INTEGER_TOO_BIG,
    };

    /**
 * @brief           J-PAKE round 1, get private key xa, xb, and ZKP(xa), ZKP(xb) for local user
 * @param[in]       jpake_para           - jpake_para_t struct pointer
 * @param[in]       local_zkp_owner_info - ID and other info of local user
 * @param[out]      xa                   - private key xa of local user(x1 of Alice, or x3 of Bob)
 * @param[out]      xb                   - private key xb of local user(x2 of Alice, or x4 of Bob)
 * @param[out]      xa_zkp               - ZKP(xa)
 * @param[out]      xb_zkp               - ZKP(xb)
 * @return          0:success     other:error
 * @note            
 *        1.xa is in [0,q-1], xb is in [1,q-1].
 */
    unsigned int jpake_round1_generate_xa_xb_and_local_two_zkps(jpake_para_t *jpake_para, jpake_user_info_t *local_zkp_owner_info, unsigned int *xa, unsigned int *xb,
                                                                jpake_zkp_t *xa_zkp, jpake_zkp_t *xb_zkp);

    /**
 * @brief           J-PAKE round 1, verify ZKP(xc), ZKP(xd) of peer user
 * @param[in]       jpake_para           - jpake_para_t struct pointer
 * @param[in]       peer_zkp_owner_info  - ID and other info of peer user
 * @param[in]       xc_zkp               - ZKP(xc) of peer user(ZKP(x1) of Alice, or ZKP(x3) of Bob)
 * @param[in]       xd_zkp               - ZKP(xd) of peer user(ZKP(x2) of Alice, or ZKP(x4) of Bob)
 * @return          0:success     other:error
 */
    unsigned int jpake_round1_verify_peer_two_zkps(jpake_para_t *jpake_para, jpake_user_info_t *peer_zkp_owner_info, jpake_zkp_t *xc_zkp, jpake_zkp_t *xd_zkp);

    /**
 * @brief           J-PAKE round 2, get local ZKP(xb*s) (ZKP(x2*s) of Alice, or ZKP(x4*s) of Bob)
 * @param[in]       jpake_para           - jpake_para_t struct pointer
 * @param[in]       local_zkp_owner_info - ID and other info of local user
 * @param[out]      round2_ctx           - jpake_round2_ctx_t struct pointer of local user
 * @param[in]       xb                   - private xb of local user(x2 of Alice, or x4 of Bob), less than q
 * @param[in]       gxa                  - public g^xa of local user(g^x1 of Alice, or g^x3 of Bob), less than p
 * @param[in]       gxc                  - public g^xc of peer user(g^x3 of Bob, or g^x1 of Alice), less than p
 * @param[in]       gxd                  - public g^xd of peer user(g^x4 of Bob, or g^x2 of Alice), less than p
 * @param[in]       s                    - shared secret of two sides
 * @param[in]       s_wlen               - word length of s
 * @param[out]      local_zkp_xb_s       - ZKP(xb*s) of local user(g is (g^xa)(g^xc)(g^xd) mod p)
 * @return          0:success     other:error
 */
    unsigned int jpake_round2_generate_local_zkp(jpake_para_t *jpake_para, jpake_user_info_t *local_zkp_owner_info, jpake_round2_ctx_t *round2_ctx, unsigned int *xb,
                                                 unsigned int *gxa, unsigned int *gxc, unsigned int *gxd, unsigned int *s, unsigned int s_wlen, jpake_zkp_t *local_zkp_xb_s);

    /**
 * @brief           J-PAKE round 2, verify peer ZKP(xb*s) (ZKP(x2*s) of Alice, or ZKP(x4*s) of Bob), and compute key
 * @param[in]       jpake_para           - jpake_para_t struct pointer
 * @param[in]       peer_zkp_owner_info  - ID and other info of peer user
 * @param[out]      round2_ctx           - jpake_round2_ctx_t struct pointer of local user
 * @param[in]       xb                   - xb of local user(x2 of Alice, or x4 of Bob)
 * @param[in]       gxb                  - g^xb of local user(g^x2 of Alice, or g^x4 of Bob)
 * @param[in]       gxd                  - g^xd of peer user(g^x4 of Bob, or g^x2 of Alice)
 * @param[in]       peer_zkp_xb_s        - ZKP(xb*s) of peer user(g is (g^xa)(g^xc)(g^xb) mod p)
 * @param[out]      key                  - output key
 * @return          0:success     other:error
 * @note            
 *        1.round2_ctx must be used by jpake_round2_generate_local_zkp() by local user
 */
    unsigned int jpake_round2_verify_peer_zkp_and_compute_key(jpake_para_t *jpake_para, jpake_user_info_t *peer_zkp_owner_info, jpake_round2_ctx_t *round2_ctx, unsigned int *xb,
                                                              unsigned int *gxb, unsigned int *gxd, jpake_zkp_t *peer_zkp_xb_s, unsigned int *key);

    /**
 * @brief           J-PAKE round 3, compute H(H(k))
 * @param[in]       jpake_para           - jpake_para_t struct pointer
 * @param[in]       h_key                - digest of the big number key after round 2
 * @param[out]      h_h_key              - H(H(k))
 * @return          0:success     other:error
 * @note            
 *        1.sponsor compute H(H(k)) and send it to responsor, responsor recompute and compare.
 */
    unsigned int jpake_round3_hash_hash_key(jpake_para_t *jpake_para, unsigned char *h_key, unsigned char *h_h_key);

    /**
 * @brief           J-PAKE round 3, compute H(k)
 * @param[in]       jpake_para           - jpake_para_t struct pointer
 * @param[in]       key                  - the big number key after round 2
 * @param[out]      h_key                - H(H(k))
 * @return          0:success     other:error
 * @note            
 *        1.if responsor compare H(H(k)) successfully, responsor compute H(k) and send it to sponsor, sponsor recompute and compare.
 */
    unsigned int jpake_round3_hash_key(jpake_para_t *jpake_para, unsigned int *key, unsigned char *h_key);

#ifdef __cplusplus
}
#endif

#endif
