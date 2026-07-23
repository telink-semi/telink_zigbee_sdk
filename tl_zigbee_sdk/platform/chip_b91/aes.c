/********************************************************************************************************
 * @file    aes.c
 *
 * @brief   This is the source file for B91
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
#include "aes.h"
#include "string.h"
#include "compiler.h"

/**********************************************************************************************************************
 *                                            local constants                                                       *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                              local macro                                                        *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                             local data type                                                     *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                              global variable                                                       *
 *********************************************************************************************************************/
_attribute_aes_data_sec_ unsigned int aes_data_buff[8];
unsigned int                          aes_base_addr = 0xc0000000;
static unsigned int                   embase_offset = 0; //the embase address offset with IRAM head address.
/**********************************************************************************************************************
 *                                              local variable                                                     *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                          local function prototype                                               *
 *********************************************************************************************************************/
/**
 * @brief     This function refer to wait aes encrypt/decrypt done.
 * @return    none.
 */
static inline void aes_wait_done(void);

/**********************************************************************************************************************
 *                                         global function implementation                                             *
 *********************************************************************************************************************/
/**
 * @brief     This function refer to set key and data for encryption/decryption. 
 * @param[in] key  - the key of encrypt/decrypt, big--endian.
 * @param[in] data - the data which to do encrypt/decrypt, big--endian. 
 * @return    none.
 * @note      The AES module register must be used by word and the key and data lengths must be 16 bytes.
 */
void aes_set_key_data(unsigned char *key, unsigned char *data)
{
    /*
        The reg_aes_ptr register is 32 bits, but only the lower 16 bits can be used. The actual access address is obtained by 
        reg_embase_addr (32bit) + reg_aes_ptr (16bit) which means the accessible space is only 64K.
        The reg_embase_addr can adjust by call aes_set_em_base_addr().
    */
    unsigned int temp;
    reg_embase_addr = aes_base_addr; //set the embase addr
    for (unsigned char i = 0; i < 4; i++) {
        temp             = key[16 - (4 * i) - 4] << 24 | key[16 - (4 * i) - 3] << 16 | key[16 - (4 * i) - 2] << 8 | key[16 - (4 * i) - 1];
        reg_aes_key(i)   = temp;
        temp             = data[16 - (4 * i) - 4] << 24 | data[16 - (4 * i) - 3] << 16 | data[16 - (4 * i) - 2] << 8 | data[16 - (4 * i) - 1];
        aes_data_buff[i] = temp;
    }

    reg_aes_ptr = (reg_aes_ptr & 0xffff0000) | ((unsigned int)(aes_data_buff - embase_offset) & 0xffff); //the aes data ptr is base on embase address.
}

/**
 * @brief      This function refer to encrypt/decrypt to get result. AES module register must be used by word.
 * @param[out] result - the result of encrypt/decrypt, big--endian.
 * @return     none.
 */
void aes_get_result(unsigned char *result)
{
    /* read out the result */
    unsigned char *ptr = (unsigned char *)&aes_data_buff[4];
    for (unsigned char i = 0; i < 16; i++) {
        result[i] = ptr[15 - i];
    }
}

/**
 * @brief      This function servers to perform aes_128 encryption for 16-Byte input data with specific 16-Byte key.
 * @param[in]  key       - the key of encrypt, big--endian.
 * @param[in]  plaintext - the plaintext of encrypt, big--endian.
 * @param[out] result    - the result of encrypt, big--endian.
 * @return     none
 */
int aes_encrypt(unsigned char *key, unsigned char *plaintext, unsigned char *result)
{
    aes_set_key_data(key, plaintext); //set the key

    aes_set_mode(AES_ENCRYPT_MODE);   //cipher mode

    aes_wait_done();

    aes_get_result(result);

    return 1;
}

/**
 * @brief      This function servers to perform aes_128 encryption for 16-Byte input data with specific 16-Byte key when BT is connected.
 * @param[in]  key       - the key of encrypt, big--endian.
 * @param[in]  plaintext - the plaintext of encrypt, big--endian.
 * @param[out] result    - the result of encrypt, big--endian.
 * @return     none
 * @note       Invoking this interface avoids the risk of AES conflicts when BT is connected.
 */
int aes_encrypt_bt_en(unsigned char *key, unsigned char *plaintext, unsigned char *result)
{
    int           i, aes_correct = 0;
    unsigned char temp_result[AES_MAX_CNT][16];

    for (i = 0; i < AES_MAX_CNT; i++) {
        aes_encrypt(key, plaintext, temp_result[i]);

        if (i > 0) {
            if (!memcmp(temp_result[i], temp_result[i - 1], 16)) {
                aes_correct = 1;
                break;
            } else {
                if (i >= 2) {
                    for (int j = 0; j < i - 1; j++) {
                        if (!memcmp(temp_result[i], temp_result[j], 16)) {
                            aes_correct = 1;
                            break;
                        }
                    }
                }
            }
        }

        if (aes_correct) {
            break;
        }
    }

    if (aes_correct) {
        if (i < AES_MAX_CNT) {
            memcpy(result, temp_result[i], 16);
            return 1;
        } else {
            return 0;
        }
    }
    return 0;
}

/**
 * @brief      This function servers to perform aes_128 decryption for 16-Byte input data with specific 16-Byte key.
 * @param[in]  key         - the key of decrypt, big--endian.
 * @param[in]  decrypttext - the text of decrypt, big--endian.
 * @param[out] result      - the result of decrypt, big--endian.
 * @return     none.
 */
int aes_decrypt(unsigned char *key, unsigned char *decrypttext, unsigned char *result)
{
    aes_set_key_data(key, decrypttext); //set the key

    aes_set_mode(AES_DECRYPT_MODE);     //decipher mode

    aes_wait_done();

    aes_get_result(result);

    return 1;
}

/**
 * @brief      This function servers to perform aes_128 decryption for 16-Byte input data with specific 16-Byte key when BT is connected.
 * @param[in]  key         - the key of decrypt, big--endian.
 * @param[in]  decrypttext - the text of decrypt, big--endian.
 * @param[out] result      - the result of decrypt, big--endian.
 * @return     none.
 * @note       Invoking this interface avoids the risk of AES conflicts when BT is connected.
 */
int aes_decrypt_bt_en(unsigned char *key, unsigned char *plaintext, unsigned char *result)
{
    int           i, aes_correct = 0;
    unsigned char temp_result[AES_MAX_CNT][16];

    for (i = 0; i < AES_MAX_CNT; i++) {
        aes_decrypt(key, plaintext, temp_result[i]);

        if (i > 0) {
            if (!memcmp(temp_result[i], temp_result[i - 1], 16)) {
                aes_correct = 1;
                break;
            } else {
                if (i >= 2) {
                    for (int j = 0; j < i - 1; j++) {
                        if (!memcmp(temp_result[i], temp_result[j], 16)) {
                            aes_correct = 1;
                            break;
                        }
                    }
                }
            }
        }

        if (aes_correct) {
            break;
        }
    }

    if (aes_correct) {
        memcpy(result, temp_result[i], 16);
        return 1;
    }
    return 0;
}

/**
 * @brief     This function refer to set the em base address.
 * @param[in] addr - The range of em base address that can be set is the address space of DLM and ILM, which can view the Memory Map of datasheets.
 *                   The current driver default setting is em_base_addr = 0xc0000000, if you call this function to modify the em base address,
 *                   you need to ensure that the _attribute_aes_data_sec_ section in the link file (AES-related functions will use this section)
 *                   is set in the following address range: [em_base_addr,em_base_addr+64KB] (chip design requirements)
 * @return    none.
 * @attention If you are using a BT-related SDK, you must follow the planning of BT's sdk to handle this address and not call this function
 */
void aes_set_em_base_addr(unsigned int addr)
{
    aes_base_addr = addr; //set the embase addr
    embase_offset = convert_ram_addr_bus2cpu(addr);
}

/**********************************************************************************************************************
  *                                         local function implementation                                             *
  *********************************************************************************************************************/
/**
 * @brief     This function refer to wait aes crypt done.
 * @return    none.
 */
static inline void aes_wait_done(void)
{
    while (FLD_AES_START == (reg_aes_mode & FLD_AES_START))
        ;
}
