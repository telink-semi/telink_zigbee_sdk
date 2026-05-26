/********************************************************************************************************
 * @file    soft_sha384_sha512.h
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
#ifndef SHA384_SHA512_H
#define SHA384_SHA512_H

/* type to hold the SHA512 context */
typedef struct
{
    unsigned int count[4];
    unsigned int hash[16]; //*hash;//
    unsigned int wbuf[32];
} sha512_ctx_t;

/* type to hold the SHA384 context */
typedef sha512_ctx_t sha384_ctx_t;

/* type to hold the SHA512_224 context */
typedef sha512_ctx_t sha512_224_ctx_t;

/* type to hold the SHA512_256 context */
typedef sha512_ctx_t sha512_256_ctx_t;

/* type to hold the hmac_sha512 context */
typedef struct
{
    unsigned int K0[32];
    sha512_ctx_t sha512_ctx_t[1];
} hmac_sha512_ctx_t;

/* type to hold the hmac_sha384 context */
typedef hmac_sha512_ctx_t hmac_sha384_ctx_t;

/* type to hold the hmac_sha512_224 context */
typedef hmac_sha512_ctx_t hmac_sha512_224_ctx_t;

/* type to hold the hmac_sha512_256 context */
typedef hmac_sha512_ctx_t hmac_sha512_256_ctx_t;

// internal APIs
/**
 * @brief           Reverses the byte order in an array of unsigned integers (changes endian).
 * @param[in,out]   a                    - Pointer to the array of unsigned integers whose byte order will be reversed.
 * @param[in]       bytelen              - Length of the input array in bytes. Must be a multiple of 4.
 * @return          None
 * @note            This function reverses the byte order in each unsigned integer within the array, changing the endianness. The `bytelen` must be a multiple of 4.
*/
void sha512_convert(unsigned int *a, unsigned char bytelen);

/**
 * @brief           Copies 16 words from the source array to the destination array.
 * @param[out]      dst                  - Pointer to the destination array where the words will be copied.
 * @param[in]       src                  - Pointer to the source array from which the words will be copied.
 * @return          None
 * @note            This function copies 16 words (unsigned integers) from the source array to the destination array.
*/
void sha512_copy_16_words(unsigned int *dst, const unsigned int *src);

/**
 * @brief           Adds `byteLen` to the 128-bit integer represented by the array `a`.
 * @param[in,out]   a                    - Pointer to the array of unsigned integers representing the 128-bit integer. `a[0]` is the high 32 bits, and `a[1]` is the low 32 bits.
 * @param[in]       byteLen              - The value to be added to the 128-bit integer.
 * @return          None
 * @note            This function adds `byteLen` to the 128-bit integer represented by the array `a`, where `a[0]` is the high 32 bits and `a[1]` is the low 32 bits. This is typically used for updating the total byte length.
*/
void sha512_bytelen_add(unsigned int a[4], unsigned int byteLen);

/**
 * @brief           Retrieves the byte length of the current message stored in the block buffer.
 * @param[in]       ctx                  - Pointer to the SHA512 context structure.
 * @note            This function returns the byte length of the current message stored in the block buffer within the SHA512 context.
*/
unsigned char SHA512_block_byteLen(sha512_ctx_t *ctx);


/**
 * @brief           Updates and computes the new internal output for a message block stored in the block buffer.
 * @param[in,out]   ctx                  - Pointer to the SHA512 context structure.
 * @param[in]       byteLen              - The byte length of the current message block stored in the block buffer.
 * @return          None
 * @note            This function updates and computes the new internal output for a message block stored in the block buffer within the SHA512 context.
*/
void sha512_block(sha512_ctx_t *ctx, unsigned char byteLen);

// APIs for user.
/**
 * @brief           Initializes the SHA512 context with an input IV and updates the message length.
 * @param[in,out]   ctx                  - Pointer to the SHA512 context structure.
 * @param[in]       iv                   - Input initialization vector (IV) of 64 bytes.
 * @param[in]       byte_length_h        - High 32 bits of the message length in bytes. Must be a multiple of SHA512 block length (i.e., 128 bytes).
 * @param[in]       byte_length_l        - Low 32 bits of the message length in bytes.
 * @return          None
 * @note            This function initializes the SHA512 context with the provided IV and updates the message length, ensuring that the byte length is a multiple of the SHA512 block length (128 bytes).
*/
void sha512_init_with_iv_and_updated_length(sha512_ctx_t *ctx, unsigned char iv[64], unsigned int byte_length_h, unsigned int byte_length_l);

/**
 * @brief           Initializes the SHA512 context.
 * @param[in,out]   ctx                  - Pointer to the SHA512 context structure.
 * @return          None
*/
void sha512_init(sha512_ctx_t *ctx);

/**
 * @brief           Processes a message block in the SHA512 context.
 * @param[in,out]   ctx                  - Pointer to the SHA512 context structure.
 * @param[in]       message              - Pointer to the message block to be processed.
 * @param[in]       byteLen              - Length of the message block in bytes.
 * @return          None
*/
void sha512_process(sha512_ctx_t *ctx, const unsigned char *message, unsigned int byteLen);

/**
 * @brief           Completes the SHA512 hash computation and outputs the digest.
 * @param[in,out]   ctx                  - Pointer to the SHA512 context structure.
 * @param[out]      digest               - Output buffer for the resulting 64-byte digest.
 * @return          None
*/
void sha512_done(sha512_ctx_t *ctx, unsigned char digest[64]);

/**
 * @brief           Computes the SHA512 hash of a message in one step.
 * @param[in]       message              - Pointer to the message to be hashed.
 * @param[in]       byteLen              - Length of the message in bytes.
 * @param[out]      digest               - Output buffer for the resulting 64-byte digest.
 * @return          None
*/
void sha512_hash(const unsigned char *message, unsigned int byteLen, unsigned char digest[64]);

/**
 * @brief           Initializes the SHA384 context with an input IV and updates the message length.
 * @param[in,out]   ctx                  - Pointer to the SHA384 context structure.
 * @param[in]       iv                   - Input initialization vector (IV) of 64 bytes.
 * @param[in]       byte_length_h        - High 32 bits of the message length in bytes. Must be a multiple of SHA384 block length (i.e., 128 bytes).
 * @param[in]       byte_length_l        - Low 32 bits of the message length in bytes.
 * @return          None
 * @note            This function initializes the SHA384 context with the provided IV and updates the message length, ensuring that the byte length is a multiple of the SHA384 block length (128 bytes).
*/
void sha384_init_with_iv_and_updated_length(sha384_ctx_t *ctx, unsigned char iv[64], unsigned int byte_length_h, unsigned int byte_length_l);

/**
 * @brief           Initializes the SHA384 context.
 * @param[in,out]   ctx                  - Pointer to the SHA384 context structure.
 * @return          None
*/
void sha384_init(sha384_ctx_t *ctx);

/**
 * @brief           Processes a message block in the SHA384 context.
 * @param[in,out]   ctx                  - Pointer to the SHA384 context structure.
 * @param[in]       message              - Pointer to the message block to be processed.
 * @param[in]       byteLen              - Length of the message block in bytes.
 * @return          None
*/
void sha384_process(sha384_ctx_t *ctx, const unsigned char *message, unsigned int byteLen);

/**
 * @brief           Completes the SHA384 hash computation and outputs the digest.
 * @param[in,out]   ctx                  - Pointer to the SHA384 context structure.
 * @param[out]      digest               - Output buffer for the resulting 48-byte digest.
 * @return          None
*/
void sha384_done(sha384_ctx_t *ctx, unsigned char digest[48]);

/**
 * @brief           Computes the SHA384 hash of a message in one step.
 * @param[in]       message              - Pointer to the message to be hashed.
 * @param[in]       byteLen              - Length of the message in bytes.
 * @param[out]      digest               - Output buffer for the resulting 48-byte digest.
 * @return          None
*/
void sha384_hash(const unsigned char *message, unsigned int byteLen, unsigned char digest[48]);

/**
 * @brief           Initializes the SHA512-224 context with an input IV and updates the message length.
 * @param[in,out]   ctx                  - Pointer to the SHA512-224 context structure.
 * @param[in]       iv                   - Input initialization vector (IV) of 64 bytes.
 * @param[in]       byte_length_h        - High 32 bits of the message length in bytes.
 * @param[in]       byte_length_l        - Low 32 bits of the message length in bytes.
 * @return          None
*/
void sha512_224_init_with_iv_and_updated_length(sha512_224_ctx_t *ctx, unsigned char iv[64], unsigned int byte_length_h, unsigned int byte_length_l);

/**
 * @brief           Initializes the SHA512-224 context.
 * @param[in,out]   ctx                  - Pointer to the SHA512-224 context structure.
 * @return          None
*/
void sha512_224_init(sha512_224_ctx_t *ctx);

/**
 * @brief           Processes a message block in the SHA512-224 context.
 * @param[in,out]   ctx                  - Pointer to the SHA512-224 context structure.
 * @param[in]       message              - Pointer to the message block to be processed.
 * @param[in]       byteLen              - Length of the message block in bytes.
 * @return          None
*/
void sha512_224_process(sha512_224_ctx_t *ctx, const unsigned char *message, unsigned int byteLen);

/**
 * @brief           Completes the SHA512-224 hash computation and outputs the digest.
 * @param[in,out]   ctx                  - Pointer to the SHA512-224 context structure.
 * @param[out]      digest               - Output buffer for the resulting 28-byte digest.
 * @return          None
*/
void sha512_224_done(sha512_224_ctx_t *ctx, unsigned char digest[28]);

/**
 * @brief           Computes the SHA512-224 hash of a message in one step.
 * @param[in]       message              - Pointer to the message to be hashed.
 * @param[in]       byteLen              - Length of the message in bytes.
 * @param[out]      digest               - Output buffer for the resulting 28-byte digest.
 * @return          None
*/
void sha512_224_hash(const unsigned char *message, unsigned int byteLen, unsigned char digest[28]);

/**
 * @brief           Initializes the SHA512-256 context with an input IV and updates the message length.
 * @param[in,out]   ctx                  - Pointer to the SHA512-256 context structure.
 * @param[in]       iv                   - Input initialization vector (IV) of 64 bytes.
 * @param[in]       byte_length_h        - High 32 bits of the message length in bytes.
 * @param[in]       byte_length_l        - Low 32 bits of the message length in bytes.
 * @return          None
*/
void sha512_256_init_with_iv_and_updated_length(sha512_256_ctx_t *ctx, unsigned char iv[64], unsigned int byte_length_h, unsigned int byte_length_l);

/**
 * @brief           Initializes the SHA512-256 context.
 * @param[in,out]   ctx                  - Pointer to the SHA512-256 context structure.
 * @return          None
*/
void sha512_256_init(sha512_256_ctx_t *ctx);

/**
 * @brief           Processes a message block in the SHA512-256 context.
 * @param[in,out]   ctx                  - Pointer to the SHA512-256 context structure.
 * @param[in]       message              - Pointer to the message block to be processed.
 * @param[in]       byteLen              - Length of the message block in bytes.
 * @return          None
*/
void sha512_256_process(sha512_256_ctx_t *ctx, const unsigned char *message, unsigned int byteLen);

/**
 * @brief           Completes the SHA512-256 hash computation and outputs the digest.
 * @param[in,out]   ctx                  - Pointer to the SHA512-256 context structure.
 * @param[out]      digest               - Output buffer for the resulting 32-byte digest.
 * @return          None
*/
void sha512_256_done(sha512_256_ctx_t *ctx, unsigned char digest[32]);

/**
 * @brief           Computes the SHA512-256 hash of a message in one step.
 * @param[in]       message              - Pointer to the message to be hashed.
 * @param[in]       byteLen              - Length of the message in bytes.
 * @param[out]      digest               - Output buffer for the resulting 32-byte digest.
 * @return          None
*/
void sha512_256_hash(const unsigned char *message, unsigned int byteLen, unsigned char digest[32]);

/**
 * @brief           Initializes the HMAC-SHA384 context with a key.
 * @param[in,out]   ctx                  - Pointer to the HMAC-SHA384 context structure.
 * @param[in]       key                  - Pointer to the key for HMAC computation.
 * @param[in]       key_len              - Length of the key in bytes.
 * @return          None
*/
void hmac_sha384_init(hmac_sha384_ctx_t *ctx, const unsigned char *key, unsigned int key_len);

/**
 * @brief           Processes a message block in the HMAC-SHA384 context.
 * @param[in,out]   ctx                  - Pointer to the HMAC-SHA384 context structure.
 * @param[in]       input                - Pointer to the message block to be processed.
 * @param[in]       byteLen              - Length of the message block in bytes.
 * @return          None
*/
void hmac_sha384_process(hmac_sha384_ctx_t *ctx, const unsigned char *input, unsigned int byteLen);

/**
 * @brief           Completes the HMAC-SHA384 computation and outputs the MAC.
 * @param[in,out]   ctx                  - Pointer to the HMAC-SHA384 context structure.
 * @param[out]      mac                  - Output buffer for the resulting 48-byte MAC.
 * @return          None
*/
void hmac_sha384_done(hmac_sha384_ctx_t *ctx, unsigned char mac[48]);

/**
 * @brief           Computes the HMAC-SHA384 of a message in one step.
 * @param[in]       key                  - Pointer to the key for HMAC computation.
 * @param[in]       key_len              - Length of the key in bytes.
 * @param[in]       msg                  - Pointer to the message to be hashed.
 * @param[in]       msg_len              - Length of the message in bytes.
 * @param[out]      mac                  - Output buffer for the resulting 48-byte MAC.
 * @return          None
*/
void hmac_sha384(unsigned char *key, unsigned int key_len, unsigned char *msg, unsigned int msg_len, unsigned char mac[48]);

/**
 * @brief           Initializes the HMAC-SHA512-224 context with a key.
 * @param[in,out]   ctx                  - Pointer to the HMAC-SHA512-224 context structure.
 * @param[in]       key                  - Pointer to the key for HMAC computation.
 * @param[in]       key_len              - Length of the key in bytes.
 * @return          None
*/
void hmac_sha512_224_init(hmac_sha512_224_ctx_t *ctx, const unsigned char *key, unsigned int key_len);

/**
 * @brief           Processes a message block in the HMAC-SHA512-224 context.
 * @param[in,out]   ctx                  - Pointer to the HMAC-SHA512-224 context structure.
 * @param[in]       input                - Pointer to the message block to be processed.
 * @param[in]       byteLen              - Length of the message block in bytes.
 * @return          None
*/
void hmac_sha512_224_process(hmac_sha512_224_ctx_t *ctx, const unsigned char *input, unsigned int byteLen);

/**
 * @brief           Completes the HMAC-SHA512-224 computation and outputs the MAC.
 * @param[in,out]   ctx                  - Pointer to the HMAC-SHA512-224 context structure.
 * @param[out]      mac                  - Output buffer for the resulting 28-byte MAC.
 * @return          None
*/
void hmac_sha512_224_done(hmac_sha512_224_ctx_t *ctx, unsigned char mac[28]);

/**
 * @brief           Computes the HMAC-SHA512-224 of a message in one step.
 * @param[in]       key                  - Pointer to the key for HMAC computation.
 * @param[in]       key_len              - Length of the key in bytes.
 * @param[in]       msg                  - Pointer to the message to be hashed.
 * @param[in]       msg_len              - Length of the message in bytes.
 * @param[out]      mac                  - Output buffer for the resulting 28-byte MAC.
 * @return          None
*/
void hmac_sha512_224(unsigned char *key, unsigned int key_len, unsigned char *msg, unsigned int msg_len, unsigned char mac[28]);

/**
 * @brief           Initializes the HMAC-SHA512-256 context with a key.
 * @param[in,out]   ctx                  - Pointer to the HMAC-SHA512-256 context structure.
 * @param[in]       key                  - Pointer to the key for HMAC computation.
 * @param[in]       key_len              - Length of the key in bytes.
 * @return          None
*/
void hmac_sha512_256_init(hmac_sha512_256_ctx_t *ctx, const unsigned char *key, unsigned int key_len);

/**
 * @brief           Processes a message block in the HMAC-SHA512-256 context.
 * @param[in,out]   ctx                  - Pointer to the HMAC-SHA512-256 context structure.
 * @param[in]       input                - Pointer to the message block to be processed.
 * @param[in]       byteLen              - Length of the message block in bytes.
 * @return          None
*/
void hmac_sha512_256_process(hmac_sha512_256_ctx_t *ctx, const unsigned char *input, unsigned int byteLen);

/**
 * @brief           Completes the HMAC-SHA512-256 computation and outputs the MAC.
 * @param[in,out]   ctx                  - Pointer to the HMAC-SHA512-256 context structure.
 * @param[out]      mac                  - Output buffer for the resulting 32-byte MAC.
 * @return          None
*/
void hmac_sha512_256_done(hmac_sha512_256_ctx_t *ctx, unsigned char mac[32]);

/**
 * @brief           Computes the HMAC-SHA512-256 of a message in one step.
 * @param[in]       key                  - Pointer to the key for HMAC computation.
 * @param[in]       key_len              - Length of the key in bytes.
 * @param[in]       msg                  - Pointer to the message to be hashed.
 * @param[in]       msg_len              - Length of the message in bytes.
 * @param[out]      mac                  - Output buffer for the resulting 32-byte MAC.
 * @return          None
*/
void hmac_sha512_256(unsigned char *key, unsigned int key_len, unsigned char *msg, unsigned int msg_len, unsigned char mac[32]);

/**
 * @brief           Initializes the HMAC-SHA512 context with a key.
 * @param[in,out]   ctx                  - Pointer to the HMAC-SHA512 context structure.
 * @param[in]       key                  - Pointer to the key for HMAC computation.
 * @param[in]       key_len              - Length of the key in bytes.
 * @return          None
*/
void hmac_sha512_init(hmac_sha512_ctx_t *ctx, const unsigned char *key, unsigned int key_len);

/**
 * @brief           Processes a message block in the HMAC-SHA512 context.
 * @param[in,out]   ctx                  - Pointer to the HMAC-SHA512 context structure.
 * @param[in]       input                - Pointer to the message block to be processed.
 * @param[in]       byteLen              - Length of the message block in bytes.
 * @return          None
*/
void hmac_sha512_process(hmac_sha512_ctx_t *ctx, const unsigned char *input, unsigned int byteLen);

/**
 * @brief           Completes the HMAC-SHA512 computation and outputs the MAC.
 * @param[in,out]   ctx                  - Pointer to the HMAC-SHA512 context structure.
 * @param[out]      mac                  - Output buffer for the resulting 64-byte MAC.
 * @return          None
*/
void hmac_sha512_done(hmac_sha512_ctx_t *ctx, unsigned char mac[64]);

/**
 * @brief           Computes the HMAC-SHA512 of a message in one step.
 * @param[in]       key                  - Pointer to the key for HMAC computation.
 * @param[in]       key_len              - Length of the key in bytes.
 * @param[in]       msg                  - Pointer to the message to be hashed.
 * @param[in]       msg_len              - Length of the message in bytes.
 * @param[out]      mac                  - Output buffer for the resulting 64-byte MAC.
 * @return          None
*/
void hmac_sha512(unsigned char *key, unsigned int key_len, unsigned char *msg, unsigned int msg_len, unsigned char mac[64]);

#endif
