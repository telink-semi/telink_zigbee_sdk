/********************************************************************************************************
 * @file    aes.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/** @page AES
 *
 *  API Reference
 *  ===============
 *  Header File: aes.h
 *
 *  How to use this driver
 *  ===============
 *
 *  - Use the follow functions for encryption and decryption:
 *
 *  (#) When BT is not connected, API's are:
 *      (+) aes_encrypt()
 *      (+) aes_decrypt()
 *
 *  (#) When BT is connected, API's are:
 *      (+) aes_encrypt_bt_en()
 *      (+) aes_decrypt_bt_en()
 *
 *  - Two ways to get if the encryption or decryption process is done:
 *  (#) By polling, API is:
 *      (+) aes_wait_done()
 *
 *  (#) By interrupt mode should initialize interrupt first, then the interrupt service function
 *      will be triggered when the encryption or decryption process done.
 *
 *  - Use the function aes_set_em_base_addr() can modify the aes module em_base_address, but it's not recommended to call when using a BT-related SDK.
 *
 *
 */
#ifndef _AES_H_
#define _AES_H_

#include "compiler.h"
#include "./reg_include/aes_reg.h"

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/

#define AES_MAX_CNT 4

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/
/**
 * @brief AES mode.
 */
typedef enum
{
    AES_ENCRYPT_MODE = 0,
    AES_DECRYPT_MODE = 2,
} aes_mode_e;

// aes api error code
typedef enum
{
    AES_API_ERROR_TIMEOUT_NONE    = 0x00,
    AES_API_ERROR_TIMEOUT_ENCRYPT = 0x01,
    AES_API_ERROR_TIMEOUT_DECRYPT = 0x02,
} aes_api_error_code_e;

/**
 * aes error timeout(us),a large value is set by default,can set it by aes_set_error_timeout().
 */
extern unsigned int g_aes_error_timeout_us;

/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/
/**
 * @brief     This function serves to aes finite state machine reset(the configuration register is still there and does not need to be reconfigured).
 * @return    none.
 */
void aes_hw_fsm_reset(void);

/**
  * @brief     This function serves to set the aes timeout(us).
  * @param[in] timeout_us - the timeout(us).
  * @return    none.
  * @note      The default timeout (g_aes_error_timeout_us) is the larger value.If the timeout exceeds the feed dog time and triggers a watchdog restart,
  *            g_aes_error_timeout_us can be changed to a smaller value via this interface, depending on the application.
  *            g_aes_error_timeout_us the minimum time must meet the following conditions:
  *            1. at least 100us;
  *            2. maximum interrupt processing time;
  *            3. Consider the conflict time of aes encryption and decryption by ble/bt;
  */
void aes_set_error_timeout(unsigned int timeout_us);

/**
 * @brief     This function serves to record the api status.
 * @param[in] aes_api_status - aes_api_error_code_e.
 * @return    none.
 * @note      This function can be rewritten according to the application scenario,can read the parameters of the interface to obtain details about the timeout reason(aes_api_error_code_e),
 *            aes_hw_fsm_reset() must be called.
 */
__attribute__((weak)) void aes_timeout_handler(unsigned int aes_error_timeout_code);

/**
 * @brief      This function servers to perform aes_128 encryption for 16-Byte input data with specific 16-Byte key.
 * @param[in]  key       - the key of encrypt, big--endian.
 * @param[in]  plaintext - the plaintext of encrypt, big--endian.
 * @param[out] result    - the result of encrypt, big--endian.
 * @return     1: operation successful;
 *             DRV_API_TIMEOUT: timeout exit(g_aes_error_timeout_us refer to the note for aes_set_error_timeout,the solution processing is already done in aes_timeout_handler, so just re-invoke the interface);
 */
int aes_encrypt(unsigned char *key, unsigned char *plaintext, unsigned char *result);

/**
 * @brief      This function servers to perform aes_128 encryption for 16-Byte input data with specific 16-Byte key when BT is connected.
 * @param[in]  key       - the key of encrypt, big--endian.
 * @param[in]  plaintext - the plaintext of encrypt, big--endian.
 * @param[out] result    - the result of encrypt, big--endian.
 * @return     DRV_API_TIMEOUT: timeout exit(g_aes_error_timeout_us refer to the note for aes_set_error_timeout,the solution processing is already done in aes_timeout_handler, so just re-invoke the interface);
 * @note       Invoking this interface avoids the risk of AES conflicts when BT is connected.
 */
int aes_encrypt_bt_en(unsigned char *key, unsigned char *plaintext, unsigned char *result);

/**
 * @brief      This function servers to perform aes_128 decryption for 16-Byte input data with specific 16-Byte key.
 * @param[in]  key         - the key of decrypt, big--endian.
 * @param[in]  decrypttext - the text of decrypt, big--endian.
 * @param[out] result      - the result of decrypt, big--endian.
 * @return     1: operation successful;
 *             DRV_API_TIMEOUT: timeout exit(g_aes_error_timeout_us refer to the note for aes_set_error_timeout,the solution processing is already done in aes_timeout_handler, so just re-invoke the interface);
 */
int aes_decrypt(unsigned char *key, unsigned char *decrypttext, unsigned char *result);

/**
 * @brief      This function servers to perform aes_128 decryption for 16-Byte input data with specific 16-Byte key when BT is connected.
 * @param[in]  key         - the key of decrypt, big--endian.
 * @param[in]  decrypttext - the text of decrypt, big--endian.
 * @param[out] result      - the result of decrypt, big--endian.
 * @return     DRV_API_TIMEOUT: timeout exit(g_aes_error_timeout_us refer to the note for aes_set_error_timeout,the solution processing is already done in aes_timeout_handler, so just re-invoke the interface);
 * @note       Invoking this interface avoids the risk of AES conflicts when BT is connected.
 */
int aes_decrypt_bt_en(unsigned char *key, unsigned char *plaintext, unsigned char *result);

/**
 * @brief     This function refer to set the em base address.
 * @param[in] addr - The range of em base address that can be set is the address space of DLM and ILM, which can view the Memory Map of datasheets.
 *                   The current driver default setting is em_base_addr = 0xc0000000, if you call this function to modify the em base address,
 *                   you need to ensure that the _attribute_aes_data_sec_ section in the link file (AES-related functions will use this section)
 *                   is set in the following address range: [em_base_addr,em_base_addr+64KB] (chip design requirements)
 * @return    none.
 * @attention If you are using a BT-related SDK, you must follow the planning of BT's sdk to handle this address and not call this function
 */
void aes_set_em_base_addr(unsigned int addr);

/**
 * @brief     This function refer to set key and data for encryption/decryption. 
 * @param[in] key  - the key of encrypt/decrypt, big--endian.
 * @param[in] data - the data which to do encrypt/decrypt, big--endian. 
 * @return    none.
 * @note      The AES module register must be used by word and the key and data lengths must be 16 bytes.
 */
void aes_set_key_data(unsigned char *key, unsigned char *data);

/**
 * @brief      This function refer to encrypt/decrypt to get result. AES module register must be used by word.
 * @param[out] result - the result of encrypt/decrypt, big--endian.
 * @return     none.
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
 * @return    non-zero   -  the interrupt occurred.
 *            zero  -  the interrupt did not occur.
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
