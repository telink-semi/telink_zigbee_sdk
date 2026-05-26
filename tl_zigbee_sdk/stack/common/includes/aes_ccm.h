/********************************************************************************************************
 * @file    aes_ccm.h
 *
 * @brief   This is the header file for aes_ccm
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
#ifndef AES_CCM_H
#define AES_CCM_H

#define AES_BLOCK_SIZE     16
#define CCM_ASTR_MAX_SIZE  64

enum {
    AES_SUCC = 0x00,
    AES_NO_BUF,
    AES_FAIL,
};

struct CCM_FLAGS_TAG {
    union {
        struct {
            u8 L:3;
            u8 M:3;
            u8 aData:1;
            u8 reserved:1;
        } bf;
        u8 val;
    };
};

typedef struct CCM_FLAGS_TAG ccm_flags_t;

enum AES_OPT {
    AES_ENCRYPTION = 0,
    AES_DECRYPTION,
};

typedef struct {
    union {
        u8 A[AES_BLOCK_SIZE];
        u8 B[AES_BLOCK_SIZE];
    } bf;
    
    u8 tmpResult[AES_BLOCK_SIZE];
    u8 newAstr[CCM_ASTR_MAX_SIZE];
} aes_enc_t;

u8 aes_ccmAuthTran(u8 micLen, u8 *key, u8 *iv, u8 *mStr, u16 mStrLen, u8 *aStr, u8 aStrLen, u8 *result);
u8 aes_ccmEncTran(u8 micLen, u8 *key, u8 *iv, u8 *mStr, u16 mStrLen, u8 *aStr, u8 aStrLen, u8 *mic);
u8 aes_ccmDecTran(u8 micLen, u8 *key, u8 *iv, u8 *mStr, u16 mStrLen, u8 *aStr, u8 aStrLen, u8 *mic);
u8 aes_ccmDecAuthTran(u8 micLen, u8 *key, u8 *iv, u8 *mStr, u16 mStrLen, u8 *aStr, u8 aStrLen, u8 *mic);
u8 aes_cryptHashFunc(u8 *input, u8 inputLen, u8 *output);

#endif /* AES_CCM_H */
