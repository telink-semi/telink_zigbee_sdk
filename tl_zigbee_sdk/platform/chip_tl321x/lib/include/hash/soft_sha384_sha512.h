/********************************************************************************************************
 * @file    soft_sha384_sha512.h
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
#ifndef SHA384_SHA512_H
#define SHA384_SHA512_H

#if 0
    #include "type.h"
#else
//typedef unsigned int   unsigned int;
//typedef unsigned short unsigned char;
//typedef unsigned char  unsigned char;
typedef signed char S8;
#endif


/* type to hold the SHA512 context */
typedef struct
{
    unsigned int count[4];
    unsigned int hash[16]; //*hash;//
    unsigned int wbuf[32];
} SHA512_Ctx;

/* type to hold the SHA384 context */
typedef SHA512_Ctx SHA384_Ctx;

/* type to hold the SHA512_224 context */
typedef SHA512_Ctx SHA512_224_Ctx;

/* type to hold the SHA512_256 context */
typedef SHA512_Ctx SHA512_256_Ctx;

/* type to hold the HMAC_SHA512 context */
typedef struct
{
    unsigned int K0[32];
    SHA512_Ctx   sha512_ctx[1];
} HMAC_SHA512_Ctx;

/* type to hold the HMAC_SHA384 context */
typedef HMAC_SHA512_Ctx HMAC_SHA384_Ctx;

/* type to hold the HMAC_SHA512_224 context */
typedef HMAC_SHA512_Ctx HMAC_SHA512_224_Ctx;

/* type to hold the HMAC_SHA512_256 context */
typedef HMAC_SHA512_Ctx HMAC_SHA512_256_Ctx;


//internal APIs
void SHA512_convert(unsigned int *a, unsigned char bytelen);
void SHA512_Copy_16_Words(unsigned int *dst, unsigned int *src);
#if 0
void SHA512_XOR(unsigned int a[2], unsigned int b[2], unsigned int c[2]);
void SHA512_ROTR(unsigned int x[2], unsigned int y[2], unsigned char n);
void SHA512_SHR(unsigned int x[2], unsigned int y[2], unsigned char n);
void SHA512_mod_add(unsigned int a[2], unsigned int b[2], unsigned int c[2]);
#endif
void          SHA512_byteLen_add(unsigned int a[4], unsigned int byteLen);
unsigned char SHA512_block_byteLen(SHA512_Ctx *ctx);
void          SHA512_block(SHA512_Ctx *ctx, unsigned char byteLen);


//APIs for user.
void SHA512_Init_with_iv_and_updated_length(SHA512_Ctx *ctx, unsigned char iv[64], unsigned int byte_length_h, unsigned int byte_length_l);
void SHA512_Init(SHA512_Ctx *ctx);
void SHA512_Process(SHA512_Ctx *ctx, unsigned char *message, unsigned int byteLen);
void SHA512_Done(SHA512_Ctx *ctx, unsigned char digest[64]);
void SHA512_Hash(unsigned char *message, unsigned int byteLen, unsigned char digest[64]);

void SHA384_Init_with_iv_and_updated_length(SHA384_Ctx *ctx, unsigned char iv[64], unsigned int byte_length_h, unsigned int byte_length_l);
void SHA384_Init(SHA384_Ctx *ctx);
void SHA384_Process(SHA384_Ctx *ctx, unsigned char *message, unsigned int byteLen);
void SHA384_Done(SHA384_Ctx *ctx, unsigned char digest[48]);
void SHA384_Hash(unsigned char *message, unsigned int byteLen, unsigned char digest[48]);

void SHA512_224_Init_with_iv_and_updated_length(SHA512_224_Ctx *ctx, unsigned char iv[64], unsigned int byte_length_h, unsigned int byte_length_l);
void SHA512_224_Init(SHA512_224_Ctx *ctx);
void SHA512_224_Process(SHA512_224_Ctx *ctx, unsigned char *message, unsigned int byteLen);
void SHA512_224_Done(SHA512_224_Ctx *ctx, unsigned char digest[28]);
void SHA512_224_Hash(unsigned char *message, unsigned int byteLen, unsigned char digest[28]);

void SHA512_256_Init_with_iv_and_updated_length(SHA512_256_Ctx *ctx, unsigned char iv[64], unsigned int byte_length_h, unsigned int byte_length_l);
void SHA512_256_Init(SHA512_256_Ctx *ctx);
void SHA512_256_Process(SHA512_256_Ctx *ctx, unsigned char *message, unsigned int byteLen);
void SHA512_256_Done(SHA512_256_Ctx *ctx, unsigned char digest[32]);
void SHA512_256_Hash(unsigned char *message, unsigned int byteLen, unsigned char digest[32]);


void HMAC_SHA384_Init(HMAC_SHA384_Ctx *ctx, unsigned char *key, unsigned int keyByteLen);
void HMAC_SHA384_Process(HMAC_SHA384_Ctx *ctx, unsigned char *input, unsigned int byteLen);
void HMAC_SHA384_Done(HMAC_SHA384_Ctx *ctx, unsigned char mac[48]);
void HMAC_SHA384(unsigned char *key, unsigned int keyByteLen, unsigned char *msg, unsigned int msgByteLen, unsigned char mac[48]);

void HMAC_SHA512_224_Init(HMAC_SHA512_224_Ctx *ctx, unsigned char *key, unsigned int keyByteLen);
void HMAC_SHA512_224_Process(HMAC_SHA512_224_Ctx *ctx, unsigned char *input, unsigned int byteLen);
void HMAC_SHA512_224_Done(HMAC_SHA512_224_Ctx *ctx, unsigned char mac[28]);
void HMAC_SHA512_224(unsigned char *key, unsigned int keyByteLen, unsigned char *msg, unsigned int msgByteLen, unsigned char mac[28]);

void HMAC_SHA512_256_Init(HMAC_SHA512_256_Ctx *ctx, unsigned char *key, unsigned int keyByteLen);
void HMAC_SHA512_256_Process(HMAC_SHA512_256_Ctx *ctx, unsigned char *input, unsigned int byteLen);
void HMAC_SHA512_256_Done(HMAC_SHA512_256_Ctx *ctx, unsigned char mac[32]);
void HMAC_SHA512_256(unsigned char *key, unsigned int keyByteLen, unsigned char *msg, unsigned int msgByteLen, unsigned char mac[32]);

void HMAC_SHA512_Init(HMAC_SHA512_Ctx *ctx, unsigned char *key, unsigned int keyByteLen);
void HMAC_SHA512_Process(HMAC_SHA512_Ctx *ctx, unsigned char *input, unsigned int byteLen);
void HMAC_SHA512_Done(HMAC_SHA512_Ctx *ctx, unsigned char mac[64]);
void HMAC_SHA512(unsigned char *key, unsigned int keyByteLen, unsigned char *msg, unsigned int msgByteLen, unsigned char mac[64]);


#endif
