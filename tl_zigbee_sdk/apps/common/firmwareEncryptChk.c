/********************************************************************************************************
 * @file     firmwareEncryptChk.c
 *
 * @brief
 *
 * @author
 * @date     Feb. 1, 2017
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
#include "tl_common.h"
#include "firmware_encrypt.h"
#include "firmwareEncryptChk.h"


#if defined(MCU_CORE_826x)
int flash_read_mid_uid_with_check( unsigned int *flash_mid ,unsigned char *flash_uid)
{
	unsigned char no_uid[16]={0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01};
	int i,f_cnt=0;
	unsigned int mid;
	flash_read_mid((unsigned char*)&mid);
	mid = mid & 0xffff;
	*flash_mid  = mid;

	if((mid == 0x4051) || (mid == 0x60C8)|| (mid == 0x6085) || (mid==0x40c8)){
		flash_read_uid(0x4b,(unsigned char *)flash_uid);
	}else{
	  return 0;
	}
	for(i = 0; i < 16; i++){
		if(flash_uid[i] == no_uid[i]){
			f_cnt++;
		}
	}
	if(f_cnt==16) {
		return 0;
	}else{
		return 1;
	}
}
#endif


/**
 *  @brief Only support for 8258, if you want to this function, please contact to us.
 */
u8 firmwareCheckWithUID(void)
{
#if UID_ENABLE
	u8 flash_mid[16] = {0};
	u8 flash_uid[16] = {0};
	int flag = flash_read_mid_uid_with_check(flash_mid, flash_uid);
	if(flag == 0){
		return 1;
	}
	u8 ciphertext[16] = {0};
	firmware_encrypt_based_on_uid(flash_uid, ciphertext);

	u8 code[16] = {0};
	flash_read(CFG_FIRMWARE_ENCRYPTION, 16, code);

	if(memcmp(ciphertext, code, 16)){
		return 1;
	}
#endif

	return 0;
}
