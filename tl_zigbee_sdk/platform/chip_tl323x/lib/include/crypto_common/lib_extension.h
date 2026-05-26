/********************************************************************************************************
 * @file    lib_extension.h
 *
 * @brief   This is the header file for tl323x
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#ifndef LIB_EXTENSION_H
#define LIB_EXTENSION_H

#include "lib/include/crypto_common/common_config.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
 * @brief           Address remap.
 * @param[in]       addr_h               - address high 32-bit (unsigned int)
 * @param[in]       addr_l               - address low 32-bit (unsigned int)
 * @param[in]       is_dma_read_addr     - whether the address is DMA input or output (0 for output, non-zero for input)
 * @return          None
 * @note            
 */
    unsigned int lib_ske_secure_port_config(unsigned int alg, unsigned short sp_key_idx);

    /**
 * @brief           Configure secure port for Lib Hash
 * @param[in]       sp_key_idx           - secure port key ID
 * @return          None
 * @note            
 */
    unsigned int lib_hash_secure_port_config(unsigned short sp_key_idx);

    /**
 * @brief           Address remap.
 * @param[in]       addr_h               - address high 32-bit (unsigned int)
 * @param[in]       addr_l               - address low 32-bit (unsigned int)
 * @param[in]       is_dma_read_addr     - whether the address is DMA input or output (0 for output, non-zero for input)
 * @return          None
 * @note            
 */
    unsigned char *lib_addr_arch32_lock_remap(unsigned int addr_h, unsigned int addr_l, unsigned int is_dma_read_addr);

    /**
 * @brief           Address unlock and remap
 * @return          None
 * @note            
 */
    void lib_addr_arch32_unlock_remap(void);

    /**
     * @brief       Locks access to hardware registers for a specific IP block.
     * @param[in]   ip_base_addr         - Base address of the IP block to lock.
     * @return      None
     * @note        This is a placeholder function and currently does not implement any locking mechanism.
     */
    static inline void lib_register_lock(volatile const void *ip_base_addr)
    {
        (void)ip_base_addr;
    }

    /**
    * @brief        Unlocks access to hardware registers for a specific IP block.
    * @param[in]    ip_base_addr         - Base address of the IP block to unlock.
    * @return       None
    * @note         This is a placeholder function and currently does not implement any unlocking mechanism.
    */
    static inline void lib_register_unlock(volatile const void *ip_base_addr)
    {
        (void)ip_base_addr;
    }

#ifdef __cplusplus
}
#endif

#endif
