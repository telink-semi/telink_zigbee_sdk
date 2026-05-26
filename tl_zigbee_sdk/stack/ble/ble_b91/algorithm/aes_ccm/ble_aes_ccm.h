/********************************************************************************************************
 * @file     aes_ccm.h
 *
 * @brief    This is the header file for b91 BLE SDK
 *
 * @author	 BLE GROUP
 * @date         12,2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
 *******************************************************************************************************/

#pragma  once

#include "stack/ble/ble_b91/ble_format.h"

#define AES_BLOCK_SIZE     16


//#define		SUCCESS			0
enum {
    BLE_AES_SUCC = SUCCESS,
    BLE_AES_NO_BUF,
    BLE_AES_FAIL,
};


typedef struct {
	u32		pkt;
	u8		dir;
	u8		iv[8];
} ble_crypt_nonce_t;


typedef struct {
	u32					enc_pno;
	u32					dec_pno;
	u8					sk[16];			//session key
	ble_crypt_nonce_t	nonce;
	u8					st;
	u8					enable;			//1: slave enable; 2: master enable
	u8					mic_fail;
} ble_crypt_para_t;

struct CCM_FLAGS_TAG_BLE {
    union {
        struct {
            u8 L : 3;
            u8 M : 3;
            u8 aData :1;
            u8 reserved :1;            
        } bf;
        u8 val;
    };
};

typedef struct CCM_FLAGS_TAG_BLE ble_ccm_flags_t;


typedef struct {
    union {
        u8 A[AES_BLOCK_SIZE];
        u8 B[AES_BLOCK_SIZE];
    } bf;
    
    u8 tmpResult[AES_BLOCK_SIZE];
    u8 newAstr[AES_BLOCK_SIZE];
} ble_aes_enc_t;


enum{
	CRYPT_NONCE_TYPE_ACL = 0,
	CRYPT_NONCE_TYPE_CIS = 1,
	CRYPT_NONCE_TYPE_BIS = 2,
};

typedef union {
	struct{
		u8 enEncFlg:1; //enable encryption
		u8 noneType:2; //ACL, CIS, BIS
		u8 decMicFail:1;//Decryption status
		u8 role:1;     //ll_ccm_enc: Master role must use 1, Slave role must use 0;
        			   //ll_ccm_dec: Master role must use 0, Slave role must use 1;
		u8 rsvd:3;     //Rsvd
	};
	u8 cryptBitsInfo;
}cryptBitsInfo_t;

typedef struct {
	u64					txPayloadCnt; //Packet counter for Tx
	u64					rxPayloadCnt; //Packet counter for Rx
	u8					sk[16];   	  //Session key
	ble_crypt_nonce_t	ccmNonce;     //CCM nonce format
	cryptBitsInfo_t     cryptBitsInfo;//To save Ram
	u16                 rsvd;         //For align
	llPhysChnPdu_t*     pllPhysChnPdu;//LL physical channel PDU
} leCryptCtrl_t;


/**
 * @brief   	this function is used to encrypt the plaintext
 * @param[in]	*key - aes key: 128 bit key for the encryption of the data, little--endian.
 * @param[in]	*plaintext - 128 bit data block that is requested to be encrypted, little--endian.
 * @param[out]	*result - 128 bit encrypted data block, little--endian.
 * @return  	none.
 * @Note		Input data requires strict Word alignment
 */
void aes_ll_encryption(u8* key, u8* plaintext, u8 *encrypted_data);


/**
 * @brief   	this function is used to initialize the aes_ccm initial value
 * @param[in]   ltk - encryption key, LTK
 * @param[in]   skdm -
 * @param[in]   skds -
 * @param[in]   ivm -
 * @param[in]   ivs -
 * @param[in]   pd - Reference structure ble_crypt_para_t
 * @return  	none
 */
void aes_ll_ccm_encryption_init (u8 *ltk, u8 *skdm, u8 *skds, u8 *ivm, u8 *ivs, ble_crypt_para_t *pd);


/**
 * @brief   	this function is used to encrypt the aes_ccm value
 * @param[in]   pkt - plaint_text
 * @param[in]   master - ll_ccm_enc: Master role must use 1, Slave role must use 0;
                         ll_ccm_dec: Master role must use 0, Slave role must use 1;
 * @param[in]   pd - Reference structure ble_crypt_para_t
 * @return  	none
 */
void aes_ll_ccm_encryption(u8 *pkt, int master, ble_crypt_para_t *pd);


/**
 * @brief   	this function is used to encrypt the aes_ccm value, version2
 * @param[in]   pd - Reference structure leCryptCtrl_t
 * @return  	none
 */
void aes_ll_ccm_encryption_v2(leCryptCtrl_t *pd);


/**
 * @brief   	this function is used to decrypt the aes_ccm value
 * @param[in]   pkt - plaint_text
 * @param[in]   master - ll_ccm_enc: Master role must use 1, Slave role must use 0;
                         ll_ccm_dec: Master role must use 0, Slave role must use 1;
 * @param[in]   pd - Reference structure ble_crypt_para_t
 * @return  	0: decryption succeeded; 1: decryption failed
 */
int  aes_ll_ccm_decryption(u8 *pkt, int master, ble_crypt_para_t *pd);


/**
 * @brief   	this function is used to decrypt the aes_ccm value, version2
 * @param[in]   pd - Reference structure leCryptCtrl_t
 * @return  	0: decryption succeeded; 1: decryption failed
 */
int  aes_ll_ccm_decryption_v2(leCryptCtrl_t *pd);






