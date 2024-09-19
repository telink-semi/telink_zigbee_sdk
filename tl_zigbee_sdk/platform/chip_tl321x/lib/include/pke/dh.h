/********************************************************************************************************
 * @file    dh.h
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
#ifndef DH_H
#define DH_H

#ifdef __cplusplus
extern "C" {
#endif



#include "lib/include/pke/pke.h"


//DH return code
#define DH_SUCCESS                          PKE_SUCCESS
#define DH_POINTER_NULL                     (PKE_SUCCESS+0xA0U)
#define DH_INVALID_INPUT                    (PKE_SUCCESS+0xA1U)
#define DH_ZERO_ALL                         (PKE_SUCCESS+0xA2U)
#define DH_VALUE_ONE                        (PKE_SUCCESS+0xA3U)
#define DH_INTEGER_TOO_BIG                  (PKE_SUCCESS+0xA4U)


typedef struct
{
    unsigned int p_bits;
    unsigned int q_bits;
    unsigned int g_bits;
    unsigned int *p;
    unsigned int *p_h;
    unsigned int *p_n0;
    unsigned int *q;
    unsigned int *g;
}DH_PARA;






//APIs
/**
 * @brief       DH parameters pointer init, set pointers of (p, q, g)
 * @param[in]   dh_para      - DH_PARA struct pointer.
 * @param[in]   p_buf        - a U32 buffer holds p, the prime defining the GF(p).
 * @param[in]   p_bits       - bit length of p.
 * @param[in]   p_h_buf      - a U32 buffer holds pre-calculated mont parameters H(R^2 mod p).
 * @param[in]   P_n0_buf     - a U32 buffer holds pre-calculated mont parameters n0'(-modoulus^(-1) mod 2^w).
 * @param[in]   q_buf        - a U32 buffer holds q, a prime factor of p-1, aka order of g..
 * @param[in]   q_bits       - bit length of q.
 * @param[in]   g_buf        - a U32 buffer holds g, a generator of the q-order subgroup of GF(p)*.
 * @param[in]   g_bits       - bit length of g.
 * @return      0:success     other:error
 * @note
  @verbatim
      -# 1.p_h_buf holds H(R^2 mod p), to accelerate DH calculation, it occupies the same size
          memory as well as p_buf. if you do not have this, please set p_h_buf to NULL.
      -# 2.p_n0_buf holds n0'(-modoulus^(-1) mod 2^w), to accelerate DH calculation, it occupies
          (w+7)/8 bytes, here w is actually 32. if you do not have this, please set p_n0_buf to NULL.
  @endverbatim
 */
unsigned int dh_param_pointer_init(DH_PARA *dh_para, unsigned int *p_buf, unsigned int p_bits, unsigned int *p_h_buf, 
        unsigned int *p_n0_buf, unsigned int *q_buf, unsigned int q_bits, unsigned int *g_buf, unsigned int g_bits);

/**
 * @brief       DH parameters value init, set pointers of (p, q, g)
 * @param[in]   dh_para      - DH_PARA struct pointer.
 * @param[in]   p            - a prime defining the GF(p).
 * @param[in]   p_h          - the pre-calculated mont parameter (R^2 mod p).
 * @param[it]   p_n0         - the pre-calculated mont parameter (-modoulus^(-1) mod 2^w).
 * @param[in]   q            - a prime factor of p-1, aka order of g.
 * @param[in]   g            - a generator of the q-order subgroup of GF(p)*
 * @return      0:success     other:error
 * @note
  @verbatim
      -#1. please call dh_param_pointer_init() before calling this function.
      -#2. the input p occupies (dh_para->p_bits+7)/8 bytes;
           the input p_h occupies (dh_para->p_bits+7)/8 bytes, if you have this;
           the input p_n0 occupies (w+7)/8 bytes, if you have this, w is actually 32 here;
           the input q occupies (dh_para->q_bits+7)/8 bytes;
           the input g occupies (dh_para->g_bits+7)/8 bytes;
       -#3.if you do not have p_h, please set p_h to NULL,
           if you do not have p_n0, please set p_n0 to NULL.
  @endverbatim
 */
unsigned int dh_param_value_init( DH_PARA *dh_para,  unsigned char *p,  unsigned char *p_h,  unsigned char *p_n0,  unsigned char *q,  unsigned char *g);

/**
 * @brief       DH check public key, it must be in [2, p-2], and pubkey^q = 1 mod p
 * @param[in]   dh_para      - DH_PARA struct pointer.
 * @param[in]   p_minus_1    - p-1.
 * @param[in]   pubkey       - public key.
 * @return      0:success     other:error
 * @note
  @verbatim
      -#1. please call dh_param_value_init() before calling this function.
      -#2. the input p_minus_1 and pubkey both occupy (dh_para->p_bits+31)/32 words;
  @endverbatim
 */
unsigned int dh_check_public_key( DH_PARA *dh_para, unsigned int *p_minus_2, unsigned int *pubkey);

/**
 * @brief       DH generate public key from private key.
 * @param[in]   dh_para      - DH_PARA struct pointer.
 * @param[in]   prikey       - private key.
 * @param[in]   pubkey       - public key.
 * @return      0:success     other:error
 * @note
  @verbatim
      -#1. please call dh_param_value_init() before calling this function.
      -#2. the input prikey occupies (dh_para->q_bits+7)/8 bytes;
           the output pubkey occupies (dh_para->p_bits+7)/8 bytes;
  @endverbatim
 */
unsigned int dh_generate_pubkey_from_prikey( DH_PARA *dh_para, unsigned char *prikey, unsigned char *pubkey);

/**
 * @brief       DH generate key pair.
 * @param[in]   dh_para      - DH_PARA struct pointer.
 * @param[in]   prikey       - private key.
 * @param[in]   pubkey       - public key.
 * @return      0:success     other:error
 * @note
  @verbatim
      -#1. please call dh_param_value_init() before calling this function.
      -#2. the input prikey occupies (dh_para->q_bits+7)/8 bytes;
           the output pubkey occupies (dh_para->p_bits+7)/8 bytes;
  @endverbatim
 */
unsigned int dh_generate_key( DH_PARA *dh_para, unsigned char *prikey, unsigned char *pubkey);

/**
 * @brief       DH compute key
 * @param[in]   dh_para         - DH_PARA struct pointer.
 * @param[in]   local_prikey    - local private key, big-endian.
 * @param[in]   peer_pubkey     - peer public key, big-endian.
 * @param[out]  key             - output key.
 * @return      DH_SUCCESS(success)     other:error
 * @note
   @verbatim
      -# 1.local_prikey occupies (dh_para->q_bits+7)/8 bytes.
      -# 2. peer_pubkey and key occupy (dh_para->p_bits+7)/8 bytes.
   @endverbatim
 */
unsigned int dh_compute_key( DH_PARA *dh_para, unsigned char *local_prikey, unsigned char *peer_pubkey, unsigned char *key);




#ifdef DH_SEC

//DH return code(secure version)
#define DH_SUCCESS_S                        (0x9C9BC1E3U)
#define DH_ERROR_S                          (0xFDC28CB1U)


unsigned int dh_compute_key_s(DH_PARA *dh_para, unsigned char *local_prikey, unsigned char *peer_pubkey, unsigned char *key);

#endif




#ifdef __cplusplus
}
#endif

#endif

