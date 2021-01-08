/********************************************************************************************************
 * @file	zcl_toucklink_security.c
 *
 * @brief	This is the source file for zcl_toucklink_security
 *
 * @author	Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
/**********************************************************************
 * INCLUDES
 */
#include "../zcl_include.h"


/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * LOCAL TYPES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */

void ll_reset(void)
{

}

void ll_zllAttrClr(void){
	g_touchlinkAttr.freeNwkAddressRangeBegin = 0x00;
	g_touchlinkAttr.freeNwkAddressRangeEnd = 0x00;
	g_touchlinkAttr.freeGroupAddressRangeBegin = 0x00;
	g_touchlinkAttr.freeGroupAddressRangeEnd = 0x00;
}

void ll_dataSendCnfCb(u8 st, u8 handle){

}

#ifdef ZB_SECURITY

void ll_calculate_AES_input(u8 *AES_key, TOUCHLINK_KEY_TYPE algorithm, u8 *transaction_id, u8 *response_id)
{
	u8 pos = 0;
	u8 *byte_0_3;
	u8 *byte_8_11;

	if(algorithm == DEVELOPMENT_KEY)
	{
		byte_0_3 = (u8 *)"PhLi";
		byte_8_11 = (u8 *)"CLSN";
	}
	else
	{
		byte_0_3 = transaction_id;
		byte_8_11 = response_id;
	}
	memcpy(AES_key, byte_0_3, 4);pos += 4;
	memcpy(AES_key + pos, transaction_id, 4);pos += 4;
	memcpy(AES_key + pos, byte_8_11, 4);pos += 4;
	memcpy(AES_key + pos, response_id, 4);
}

void ll_decrypt_network_key(u8 key_index, u8 *encrypted_network_key, u32 transactionID, u32 responseID)
{
	u8 transaction_ID[4];
	u8 response_ID[4];
	u8 AES_input[CCM_KEY_SIZE] _attribute_aligned_(4) ;
	u8 transport_key[CCM_KEY_SIZE] _attribute_aligned_(4) ;
	u8 network_key[CCM_KEY_SIZE] _attribute_aligned_(4) = {0};

	transaction_ID[0]	= U32_BYTE3(transactionID);
	transaction_ID[1]	= U32_BYTE2(transactionID);
	transaction_ID[2]	= U32_BYTE1(transactionID);
	transaction_ID[3]	= U32_BYTE0(transactionID);

	response_ID[0]	= U32_BYTE3(responseID);
	response_ID[1]	= U32_BYTE2(responseID);
	response_ID[2]	= U32_BYTE1(responseID);
	response_ID[3]	= U32_BYTE0(responseID);

	ll_calculate_AES_input(AES_input, key_index, transaction_ID, response_ID);

	if(key_index == DEVELOPMENT_KEY)
	{
		aes_decrypt(AES_input, encrypted_network_key, network_key);
	}
	else if(key_index == MASTER_KEY)
	{

		aes_encrypt(SS_IB().touchLinkKey, AES_input, transport_key);
		aes_decrypt(transport_key, encrypted_network_key, network_key);
	}
	else if(key_index == CERTIFICATION_KEY)
	{
		aes_encrypt(SS_IB().touchLinkKey, AES_input, transport_key);
		aes_decrypt(transport_key, encrypted_network_key, network_key);
	}

	memcpy(encrypted_network_key, network_key, CCM_KEY_SIZE);

	if (!APS_IB().aps_authenticated){
		APS_IB().aps_authenticated = 1;
	}
}

void ll_get_encrypted_network_key(u8 key_index, u32 transactionID, u32 responseID, u8 *encrypted_network_key)
{
	u8 transaction_ID[4];
	u8 response_ID[4];
	u8 AES_input[CCM_KEY_SIZE] _attribute_aligned_(4) ;
	u8 transport_key[CCM_KEY_SIZE] _attribute_aligned_(4) ;
	u8 network_key[CCM_KEY_SIZE] _attribute_aligned_(4) ;

	transaction_ID[0]	= U32_BYTE3(transactionID);
	transaction_ID[1]	= U32_BYTE2(transactionID);
	transaction_ID[2]	= U32_BYTE1(transactionID);
	transaction_ID[3]	= U32_BYTE0(transactionID);

	response_ID[0]	= U32_BYTE3(responseID);
	response_ID[1]	= U32_BYTE2(responseID);
	response_ID[2]	= U32_BYTE1(responseID);
	response_ID[3]	= U32_BYTE0(responseID);

	ll_calculate_AES_input(AES_input, key_index, transaction_ID, response_ID);

#if 1
	/* If factory-new, generate and store the network key */
	if(g_zbNwkCtx.is_factory_new){
		ss_nwkKeyGenerate(network_key);
	}
	else /* Not factory new. Use the persisted network key */
	{
		memcpy(network_key, SS_IB().nwkSecurMaterialSet[0].key, CCM_KEY_SIZE);
	}
#endif
	
	if(key_index == DEVELOPMENT_KEY)
	{
		aes_encrypt(AES_input, network_key, encrypted_network_key);
	}
	else if(key_index == MASTER_KEY)
	{
		aes_encrypt(SS_IB().touchLinkKey, AES_input, transport_key);
		aes_encrypt(transport_key, network_key, encrypted_network_key);
	}
	else if(key_index == CERTIFICATION_KEY)
	{
		aes_encrypt(SS_IB().touchLinkKey, AES_input, transport_key);
		aes_encrypt(transport_key, network_key, encrypted_network_key);
	}

	if (!APS_IB().aps_authenticated){
		APS_IB().aps_authenticated = 1;
	}
}

#endif
