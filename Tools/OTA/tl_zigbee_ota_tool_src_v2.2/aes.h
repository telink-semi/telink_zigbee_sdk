#include "types.h"

void _rijndaelSetKey(unsigned char *k, unsigned char aes_sw_k0[4][4], unsigned char aes_sw_k10[4][4]);
void _rijndaelEncrypt(unsigned char *a, unsigned char aes_sw_k0[4][4]);
void _rijndaelDecrypt(unsigned char *a, unsigned char aes_sw_k10[4][4]);

//AES 128, ECB mode, all of the parameters are Big Endian.
void aes_encrypt(unsigned char *Key, unsigned char *Data, unsigned char *Result);
void aes_decrypt(unsigned char *Key, unsigned char *Data, unsigned char *Result);


void aes_att_encryption(u8 *key, u8 *plaintext, u8 *result);
void aes_att_decryption(u8 *key, u8 *plaintext, u8 *result);

