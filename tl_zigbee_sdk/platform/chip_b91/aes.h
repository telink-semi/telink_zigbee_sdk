/********************************************************************************************************
 * @file    aes.h
 *
 * @brief   This is the header file for B91
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/**	@page AES
 *
 *	Introduction
 *	===============
 *	B91 supports hardware AES function.
 *
 *	API Reference
 *	===============
 *	Header File: aes.h
 */
#ifndef _AES_H_
#define _AES_H_

#include "compiler.h"
#include "./reg_include/aes_reg.h"

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/

#define AES_MAX_CNT   4

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/
/**
 * @brief AES mode.
 */
typedef enum{
	AES_ENCRYPT_MODE	=  0,
	AES_DECRYPT_MODE	=  2,
}aes_mode_e;
/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/
 /* @brief     This function refer to encrypt. AES module register must be used by word. , all data need big endian.
 * @param[in] key       - the key of encrypt.
 * @param[in] plaintext - the plaintext of encrypt.
 * @param[in] result    - the result of encrypt.
 * @return    none
 */
int aes_encrypt(unsigned char *key, unsigned char* plaintext, unsigned char *result);

/**
 * @brief     This function refer to encrypt when BT is connected. AES module register must be used by word, all data need big endian.
 * @param[in] key       - the key of encrypt.
 * @param[in] plaintext - the plaintext of encrypt.
 * @param[in] result    - the result of encrypt.
 * @return    none
 * @note      Invoking this interface avoids the risk of AES conflicts when BT is connected.
 */
int aes_encrypt_bt_en(unsigned char* key, unsigned char* plaintext, unsigned char *result);

/**
 * @brief     This function refer to decrypt. AES module register must be used by word., all data need big endian.
 * @param[in] key         - the key of decrypt.
 * @param[in] decrypttext - the decrypttext of decrypt.
 * @param[in] result      - the result of decrypt.
 * @return    none.
 */
int aes_decrypt(unsigned char *key, unsigned char* decrypttext, unsigned char *result);

/**
 * @brief     This function refer to decrypt when BT is connected. AES module register must be used by word.all data need big endian.
 * @param[in] key         - the key of decrypt.
 * @param[in] decrypttext - the text of decrypt.
 * @param[in] result      - the result of decrypt.
 * @return    none.
 * @note      Invoking this interface avoids the risk of AES conflicts when BT is connected.
 */
int aes_decrypt_bt_en(unsigned char* key, unsigned char* plaintext, unsigned char *result);

/**
 * @brief     This function refer to set the em base address.
 * @param[in] addr - The range of em base address that can be set is the address space of DLM and ILM, which can view the Memory Map of datasheets.
 * 					 The current driver default setting is em_base_addr = 0xc0000000, if you call this function to modify the em base address,
 * 					 you need to ensure that the _attribute_aes_data_sec_ section in the link file (AES-related functions will use this section)
 * 					 is set in the following address range: [em_base_addr,em_base_addr+64KB] (chip design requirements)
 * @return    none.
 * @attention If you are using a BT-related SDK, you must follow the planning of BT's sdk to handle this address and not call this function
 */
void aes_set_em_base_addr(unsigned int addr);

/**
 * @brief     This function refer to encrypt/decrypt to set key and data. AES module register must be used by word.
 * 				All data need Little endian.
 * @param[in] key  - the key of encrypt/decrypt.
 * @param[in] data - the data which to do encrypt/decrypt. The address is 32 bits, but only the lower 16 bits are used.
 * @return    none.
 * @note	  reg_embase_addr (32bit) +reg_aes_ptr (16bit) is the actual access address.
 * 			  reg_aes_ptr is only 16bit, so access space is only 64K. Adjusting reg_embase_addr changes the initial address of 64K.
 */
void aes_set_key_data(unsigned char *key, unsigned char* data);

/**
 * @brief     This function refer to encrypt/decrypt to get result. AES module register must be used by word.
 * @param[in] result - the result of encrypt/decrypt. Little endian
 * @return    none.
 */
void aes_get_result(unsigned char *result);

/**
 * @brief     This function refer to set aes mode.
 * @param[in] mode - the irq mask.
 * @return    none.
 */
static inline void aes_set_mode(aes_mode_e mode)
{
	reg_aes_mode = (FLD_AES_START | mode);
}

/**
 * @brief     This function refer to set aes irq mask.
 * @param[in] mask - the irq mask.
 * @return    none.
 */
static inline void aes_set_irq_mask(aes_irq_e mask)
{
	reg_aes_irq_mask |= mask;
}

/**
 * @brief     This function refer to clr aes irq mask.
 * @param[in] mask - the irq mask.
 * @return    none.
 */
static inline void aes_clr_irq_mask(aes_irq_e mask)
{
	reg_aes_irq_mask &= (~mask);
}

/**
 * @brief     This function refer to get aes irq status.
 * @param[in] status - the irq status to get.
 * @return    none.
 */
static inline int aes_get_irq_status(aes_irq_e status)
{
	return (reg_aes_irq_status & status);
}

/**
 * @brief     This function refer to clr aes irq status.
 * @param[in] status - the irq status to clear.
 * @return    none.
 */
static inline void aes_clr_irq_status(aes_irq_e status)
{
	reg_aes_clr_irq_status = (status);
}

#endif /* _AES_H_ */
