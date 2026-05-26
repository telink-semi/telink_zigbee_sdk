/********************************************************************************************************
 * @file    ext_aes.c
 *
 * @brief   This is the source file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#include "tl_common.h"



/******************************** Test case for HW AES (little or big --endian )*********************************************
//Refer to Core4.0 Spec <<BLUETOOTH SPECIFICATION Version 4.0 [Vol 6], Sample Data, Page2255
u8 KEY[16] = {0xBF, 0x01, 0xFB, 0x9D, 0x4E, 0xF3, 0xBC, 0x36, 0xD8, 0x74, 0xF5, 0x39, 0x41, 0x38, 0x68, 0x4C}; //(LSO to MSO)
u8 SKD[16] = {0x13, 0x02, 0xF1, 0xE0, 0xDF, 0xCE, 0xBD, 0xAC, 0x79, 0x68, 0x57, 0x46, 0x35, 0x24, 0x13, 0x02}; //(LSO to MSO)
u8 SK[16]  = {0x66, 0xC6, 0xC2, 0x27, 0x8E, 0x3B, 0x8E, 0x05, 0x3E, 0x7E, 0xA3, 0x26, 0x52, 0x1B, 0xAD, 0x99}; //(LSO to MSO)
u8 ASK[16] = { 0};

aes_encryption_le(KEY, SKD, ASK); //little-endian

if(smemcmp(ASK, SK, 16) == 0){
    printf("aes_encryption_le: little-endian\n");
}

swapN(KEY, 16);
swapN(SKD, 16);
aes_encrypt(KEY, SKD, ASK); //big-endian
swapN(ASK, 16);

if(smemcmp(ASK, SK, 16) == 0){
    printf("aes_encrypt: big-endian\n");
}

while(1);
******************************************************************************************************************************/


/* Different process for different MCU: ******************************************/

_attribute_ram_code_sec_noinline_
static void flip_16byte_order(u8 *dst, const u8 *src)
{
    for (int i = 0; i < 16; i++){
        dst[15 - i] = src[i];
    }
}

/**
 * @brief       this function is used to encrypt the plaintext by hw aes
 * @param[in]   *key - aes key: 128 bit key for the encryption of the data, little--endian.
 * @param[in]   *plaintext - 128 bit data block that is requested to be encrypted, little--endian.
 * @param[out]  *encrypted_data - 128 bit encrypted data block, little--endian.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_
void aes_encryption_le(u8* key, u8* plaintext, u8 *encrypted_data)
{
    u8 key_r[16], plaintext_r[16], enc_data_r[16];

    flip_16byte_order(key_r, key);
    flip_16byte_order(plaintext_r, plaintext);
    aes_encryption_be(key_r, plaintext_r, enc_data_r);
    flip_16byte_order(encrypted_data, enc_data_r);
}

/**
 * @brief       this function is used to encrypt the plaintext by hw aes
 * @param[in]   *key - aes key: 128 bit key for the encryption of the data, big--endian.
 * @param[in]   *plaintext - 128 bit data block that is requested to be encrypted, big--endian.
 * @param[out]  *encrypted_data - 128 bit encrypted data block, big--endian.
 * @return      none.
 */
//_attribute_ram_code_sec_noinline_
//void aes_encryption_be(u8* key, u8* plaintext, u8 *encrypted_data)
//{
//    u32 r = irq_disable();
//    ske_lp_aes128_ecb_one_block(SKE_CRYPTO_ENCRYPT, (unsigned int *)key, (unsigned int *)plaintext, (unsigned int *)encrypted_data);
//    irq_restore(r);
//}
/*
Sample data:
u8 test_irk[16]  = {0x71, 0x4a ,0x57 ,0x3d,  0xf6 ,0x88, 0x69 ,0x0c,  0x57, 0x98, 0x50, 0x51, 0x82 ,0xf5 ,0x2a, 0xa0};
u8 test_mac[6]  = {0x3b, 0x3f, 0xfb, 0xeb, 0x1e, 0x78};

u8 result = aes_resolve_irk_rpa(test_irk, test_mac);

// Test value by qihang.mou 2024/04/11
PLL_240M_CCLK_60M_HCLK_30M_PCLK_30M_MSPI_48M aes_encryption_be use 6us, aes_resolve_irk_rpa use 14.6us.
PLL_240M_CCLK_40M_HCLK_40M_PCLK_40M_MSPI_40M aes_encryption_be use 3us, aes_resolve_irk_rpa 7.3us.
 */

/**
 * @brief       this function is used to resolve address by irk
 * @param[in]   *key - irk key: 128 bit key for the encryption of the data, little--endian.
 * @param[in]   *addr - 48-bit the bluetooth address, little--endian.
 * @return      1: TRUE: Bluetooth address resolution succeeded
 *              0: FALSE: bluetooth address resolution failed.
 */
_attribute_ram_code_sec_noinline_
bool aes_resolve_irk_rpa(u8 *key, u8 *addr)
{
    u32 plaintext[4];
    plaintext[0] = ((addr[3] << 0) | (addr[4] << 8) | (addr[5] << 16) );  //prand 3 byte
    plaintext[1] = plaintext[2] = plaintext[3] = 0;

    u8 encrypted_data[16];
    aes_encryption_le(key, (u8*)plaintext, encrypted_data);

    return (encrypted_data[0] == addr[0]) && (encrypted_data[1] == addr[1]) && (encrypted_data[2] == addr[2]);
}
/**********************************************************************************/



#if(HW_AES_CCM_ALG_EN)
_attribute_ram_code_sec_noinline_
void blt_ll_setAesCcmPara(u8 role, u8 *sk, u8 *iv, u8 aad, u64 enc_pno, u64 dec_pno, u8 lastTxLenFlag)
{

    reg_rwbtcntl |= FLD_CRYPT_SOFT_RST; //reset AES_ccm

    unsigned char *ptr = sk + 12;//blc_crypt_para.sk + 12;
    for (int i=0; i<4; i++){
        reg_tlk_sk(i) = ((ptr[3]) | (ptr[2]<<8) | (ptr[1]<<16) | (ptr[0]<<24));
        ptr -= 4;
    }

    reg_rf_tlk_iv0 = iv[0] | (iv[1]<<8)| (iv[2]<<16)|(iv[3]<<24);
    reg_rf_tlk_iv1 = iv[4] | (iv[5]<<8)| (iv[6]<<16)|(iv[7]<<24);
    reg_rf_tlk_aad = aad;// for ACL connection
    reg_rf_tx_ccm_pkt_cnt0_31 = enc_pno&0xffffffff;
    reg_rf_rx_ccm_pkt_cnt0_31 = dec_pno&0xffffffff;

    reg_rf_tx_mode2 |= FLD_TLK_MST_SLV;

    if(role == 1){//slave
        reg_rf_tx_mode2 &= (~FLD_TLK_MST_SLV);
        reg_ccm_control = lastTxLenFlag;

    }
    reg_rf_tx_mode2 |= FLD_TLK_CRYPT_ENABLE;
}
#endif



