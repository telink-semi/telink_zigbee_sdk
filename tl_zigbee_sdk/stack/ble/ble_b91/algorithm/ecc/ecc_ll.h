/********************************************************************************************************
 * @file     ecc_ll.h
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

#ifndef ECC_LL_H_
#define ECC_LL_H_


extern const u8 blt_ecc_dbg_priv_key[32];
extern const u8 blt_ecc_dbg_pub_key[64];


/**
* @brief		This function is used to register the random number function needed for ECC calculation
* @param		none
* @return		none
*/
void 			blt_ecc_init(void);

/**
* @brief		This function is used to generate an ECDH public-private key pairs
* @param[out]  pub[64]:  output ecdh public key
* @param[out]  priv[64]: output ecdh private key
* @param[in]   use_dbg_key: 0: Non-debug key , others: debug key
* @return		1:  success
*              0: failure
*/
int 			blt_ecc_gen_key_pair(unsigned char pub[64], unsigned char priv[32], bool use_dbg_key);

/**
* @brief		This function is used to calculate DHKEY based on the peer public key and own private key
* @param[in]   peer_pub_key[64]: peer public key
* @param[in]   own_priv_key[32]: own private key
* @param[out]  out_dhkey[32]: dhkey key
* @return		1:  success
*              0: failure
*/
int 			blt_ecc_gen_dhkey(const unsigned char peer_pub[64], const unsigned char own_priv[32], unsigned char out_dhkey[32]);




#endif /* ECC_LL_H_ */




