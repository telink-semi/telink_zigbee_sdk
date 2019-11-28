/********************************************************************************************************
 * @file     aes_ccm_826x.h
 *
 * @brief    aes driver API for tlsr826x
 *
 * @author
 * @date     Dec. 1, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#pragma  once

u8 AES_Encrypt(u8 *key, u8 *data, u8 *result);

u8 AES_Decrypt(u8 *key, u8 *data, u8 *result);

int aes_encrypt(unsigned char *Key, unsigned char *Data, unsigned char *Result);
int aes_decrypt(unsigned char *Key, unsigned char *Data, unsigned char *Result);
