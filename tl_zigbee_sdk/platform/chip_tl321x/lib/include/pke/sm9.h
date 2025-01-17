/********************************************************************************************************
 * @file    sm9.h
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
#define SM9_MAX_ENC_K2_BYTE_LEN (0x40U)       //MAC key for SM9 enc/dec to get C3, here we set maximum length to be 64 bytes.


#define SM9_FP12_EXP_COMB_PARTS (1U)          //(2U)  //(3U)   //


//SM9 return code
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

    //SM9 encryption type choice
    typedef enum
    {
        SM9_ENC_KDF_STREAM_CIPHER = 0,
        SM9_ENC_KDF_BLOCK_CIPHER
    } sm9_enc_type_e;

    //available when choose SM9_ENC_KDF_BLOCK_CIPHER
    /*
typedef enum {
    SM9_ENC_NO_PADDING = 0,
    SM9_ENC_PKCS7_PADDING
} sm9_enc_padding_e;
*/
    typedef SKE_PADDING sm9_enc_padding_e;

    //SM9 key exchange role
    typedef enum
    {
        SM9_Role_Sponsor = 0,
        SM9_Role_Responsor
    } sm9_exchange_role_e;

    //APIs for user
    unsigned int sm9_pairing_calc(unsigned char P1[64], unsigned char P2[128], unsigned char g[32 * 12]);

    unsigned int sm9_fp12_exp(unsigned char g[32 * 12], unsigned char r[32], unsigned char out[32 * 12]);


    unsigned int sm9_sign_gen_mastPubKey_from_mastPriKey(unsigned char ks[32], unsigned char Ppub_s[128]);

    unsigned int sm9_sign_gen_mastKeyPair(unsigned char ks[32], unsigned char Ppub_s[128]);

    unsigned int sm9_sign_gen_userPriKey(unsigned char *IDA, unsigned int IDA_bytes, unsigned char hid, unsigned char ks[32], unsigned char dsA[64]);

    unsigned int sm9_sign(unsigned char *msg, unsigned int msg_bytes, unsigned char *fp12g, unsigned char Ppub_s[128], unsigned char dsA[64], unsigned char r[32], unsigned char h[32], unsigned char S[65]);

    unsigned int sm9_verify(unsigned char *msg, unsigned int msg_bytes, unsigned char *IDA, unsigned int IDA_bytes, unsigned char hid, unsigned char *fp12g, unsigned char Ppub_s[128], unsigned char h[32], unsigned char S[65]);


    unsigned int sm9_enc_gen_mastPubKey_from_mastPriKey(unsigned char ke[32], unsigned char Ppub_e[64]);

    unsigned int sm9_enc_gen_mastKeyPair(unsigned char ke[32], unsigned char Ppub_e[64]);

    unsigned int sm9_enc_gen_userPriKey(unsigned char *IDB, unsigned int IDB_bytes, unsigned char hid, unsigned char ke[32], unsigned char deB[128]);

    unsigned int sm9_wrap_key(unsigned char *IDB, unsigned int IDB_bytes, unsigned char hid, unsigned char *fp12g, unsigned char Ppub_e[64], unsigned char r[32], unsigned char C[64], unsigned int k_bytes, unsigned char *k);

    unsigned int sm9_unwrap_key(unsigned char *IDB, unsigned int IDB_bytes, unsigned char deB[128], unsigned char C[64], unsigned int k_bytes, unsigned char *k);

    unsigned int sm9_enc(unsigned char *IDB, unsigned int IDB_bytes, unsigned char hid, unsigned char *M, unsigned int M_bytes, unsigned char *fp12g, unsigned char Ppub_e[64], unsigned char r[32], sm9_enc_type_e enc_type, sm9_enc_padding_e padding_type, unsigned int K2_bytes, unsigned char *C, unsigned int *C_bytes);

    unsigned int sm9_dec(unsigned char *IDB, unsigned int IDB_bytes, unsigned char *C, unsigned int C_bytes, unsigned char deB[128], sm9_enc_type_e enc_type, sm9_enc_padding_e padding_type, unsigned int K2_bytes, unsigned char *M, unsigned int *M_bytes);


    unsigned int sm9_exckey_gen_mastPubKey_from_mastPriKey(unsigned char ke[32], unsigned char Ppub_e[64]);

    unsigned int sm9_exckey_gen_mastKeyPair(unsigned char ke[32], unsigned char Ppub_e[64]);

    unsigned int sm9_exckey_gen_userPriKey(unsigned char *IDA, unsigned int IDA_bytes, unsigned char hid, unsigned char ke[32], unsigned char deA[128]);

    unsigned int sm9_exckey_gen_tmpPubKey_from_tmpPriKey(unsigned char *IDB, unsigned int IDB_bytes, unsigned char hid, unsigned char Ppub_e[64], unsigned char rA[32], unsigned char RA[64]);

    unsigned int sm9_exckey_gen_tmpKeyPair(unsigned char *IDB, unsigned int IDB_bytes, unsigned char hid, unsigned char Ppub_e[64], unsigned char rA[32], unsigned char RA[64]);

    unsigned int sm9_exchangekey(sm9_exchange_role_e role,
                                 unsigned char      *IDA,
                                 unsigned int        IDA_bytes,
                                 unsigned char      *IDB,
                                 unsigned int        IDB_bytes,
                                 unsigned char      *fp12g,
                                 unsigned char       Ppub_e[64],
                                 unsigned char       deA[128],
                                 unsigned char       rA[32],
                                 unsigned char       RA[64],
                                 unsigned char       RB[64],
                                 unsigned int        k_bytes,
                                 unsigned char      *k,
                                 unsigned char       S1[32],
                                 unsigned char       SA[32]);


#ifdef __cplusplus
}
#endif

#endif
