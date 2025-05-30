/********************************************************************************************************
 * @file    cache.h
 *
 * @brief   This is the header file for TL321X
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
/** @page Cache
 *
 *  Introduction
 *  ===============
 *  It should be noted that:
 *  1. mspi is connected to the flash by default, the chip does not support pointer write, can only be written by way of the interface,
 *     can read through the pointer, then you will only use dcache_invalidate, dcache_clean/dcache_flush is not open.
 *  2. For example, mspi is connected to the psram, then it can support If the mspi is connected to psram, it can support pointer write,
 *     open the macro(DCACHE_CLEAN_OR_FLUSH_IS_USED) to open dcache_clean/dcache_flush, and refer to the use of the two interfaces.
 *
 *  API Reference
 *  ===============
 *  Header File: cache.h
 */
#include "lib/include/core.h"

#define  DCACHE_CLEAN_OR_FLUSH_IS_USED      0

// Structure to hold the Way, Index, and Word
typedef struct {
    unsigned int word : 5;
    unsigned int index : 8;
    unsigned int way : 1;
} cache_line_t;

/**
 * @brief     This function serves to get dcache line size (32 bytes).
 * @return    none
 */
static inline unsigned int dcache_get_linesize(void){
    return  (read_csr(NDS_MDCM_CFG)& FLD_CACHE_LINE_SZ)>>6 ;
}

/**
 * @brief     This function serves to get dcache size (4k bytes).
 * @return    none
 */
static inline unsigned int dcache_get_size(void){
//Total cache size = Cache lines per way × Ways × Line size
    unsigned int dcache_cfg=read_csr(NDS_MDCM_CFG);
    return  (dcache_cfg& FLD_CACHE_ISET)*(((dcache_cfg&FLD_CACHE_XWAY)>>3)+1)*((dcache_cfg & FLD_CACHE_LINE_SZ)>>6);
}

/**
 * @brief     This function serves to enable dcache.
 * @return    none
 */
static inline void dcache_enable(void){
    set_csr(NDS_MCACHE_CTL,BIT(1));
}

/**
 * @brief     This function serves to disable dcache.
 * @return    none
 */
static inline void dcache_disable(void){
    clear_csr(NDS_MCACHE_CTL,BIT(1));
}

/**
 * @brief     This function serves to invalid all dcache(tag invalid).
 * @return    none
 * @note      1. mspi access to the space belongs to the Write-through region, write-through characteristics of the D25F write data to the cache
 *            at the same time will also write data to the next level of storage, equivalent to the clean function.
 *            2. the mspi is connected to flash, can not pointer write, only through the interface way to write, can pointer read,
 *            generally only  make dcache invalid.
 */
static inline void dcache_invalidate_all(void){
    write_csr(NDS_MCCTLCOMMAND,0x17);
}

//icache
/**
 * @brief     This function serves to get icache line size (32 bytes).
 * @return    none
 */
static inline unsigned int icache_get_linesize(void){
    return  (read_csr(NDS_MICM_CFG)& FLD_CACHE_LINE_SZ)>>6 ;
}

/**
 * @brief     This function serves to get dcache size (2 way *4k bytes).
 * @return    none
 */
static inline unsigned int icache_get_size(void){
    //Total cache size = Cache lines per way × Ways × Line size
    unsigned int dcache_cfg=read_csr(NDS_MICM_CFG);
    return  (dcache_cfg& FLD_CACHE_ISET)*(((dcache_cfg&FLD_CACHE_XWAY)>>3)+1)*((dcache_cfg & FLD_CACHE_LINE_SZ)>>6);
}

/**
 * @brief     This function serves to enable icache.
 * @return    none
 */
static inline void icache_enable(void){
    set_csr(NDS_MCACHE_CTL,BIT(0));
}

/**
 * @brief     This function serves to disable dcache.
 * @return    none
 */
static inline void icache_disable(void){

    clear_csr(NDS_MCACHE_CTL,BIT(0));
}



/**
 * @brief     This function serves to invalid all dcache(tag invalid).
 * @return    none
 */
static inline void icache_invalidate_all(void){
      __asm__ __volatile__("fence.i");
}


/**
 * @brief     This function serves to invalid dcache by address(tag invalid).
 * @param[in] start - start addr (the mspi range : 0x20000000 - 0x24000000).
 * @param[in] end - end addr (the mspi range : 0x20000000 - 0x24000000).
 * @return    none
 * @note      1. The dcache size is 4k, and the (end - start) value is generally within 4k;
 *            2. Currently the corresponding cache inline is invalidate regardless of whether the address space is valid in the cache or not.
 */
void dcache_invalidate(unsigned int start, unsigned int end);

/**
 * @brief     This function serves to invalid dcache by index way(tag invalid).
 * @param[in] start - start addr (the mspi range : 0x20000000 - 0x24000000).
 * @param[in] end - end addr (the mspi range : 0x20000000 - 0x24000000).
 * @return    none
 * @note      1. The dcache size is 4k, and the (end - start) value is generally within 4k;
 *            2. Currently the corresponding cache inline is invalidate regardless of whether the address space is valid in the cache or not.
 */
void icache_invalidate(unsigned int start, unsigned int end);


/*
 *  It should be noted that:
 *  1. the mspi access to the space belongs to the Write-through region and Write-no-allocate, write-through characteristics of the D25F write data to the cache
 *     at the same time will also write data to the next level of storage, equivalent to the clean function.
 */
#if(DCACHE_CLEAN_OR_FLUSH_IS_USED)
/**
 * @brief     This function serves to clean all dcache(dcache write back).
 * @return    none
 * @note      1. mspi access to the space belongs to the Write-through region, write-through characteristics of the D25F write data to the cache
 *            at the same time will also write data to the next level of storage, equivalent to the clean function.
 *            2. the mspi is connected to flash, can not pointer write, only through the interface way to write, can pointer read,
 *            generally only  make dcache invalid.
 */
static inline void dcache_clean_all(void){
    write_csr(NDS_MCCTLCOMMAND,0x07);
}


/**
 * @brief     This function serves to flush all icache(dcache write back and invalid).
 * @return    none
 * @note      1. mspi access to the space belongs to the Write-through region, write-through characteristics of the D25F write data to the cache
 *            at the same time will also write data to the next level of storage, equivalent to the clean function.
 *            2. the mspi is connected to flash, can not pointer write, only through the interface way to write, can pointer read,
 *            generally only  make dcache invalid.
 */
static inline void dcache_flush_all(void){
    write_csr(NDS_MCCTLCOMMAND,0x06);
}


/**
 * @brief     This function serves to clean dcache by index way(dcache write back).
 * @param[in] start - start addr (the mspi range : 0x20000000 - 0x24000000).
 * @param[in] end - end addr (the mspi range : 0x20000000 - 0x24000000).
 * @return    none
 * @note      1. The dcache size is 4k, and the (end - start) value is generally within 4k;
 *            2. the mspi access to the space belongs to the Write-through region, write-through characteristics of the D25F write data to the cache
 *            at the same time will also write data to the next level of storage, equivalent to the clean function.
 *            3. the mspi is connected to flash, can not pointer write, only through the interface way to write, can pointer read,
 *            generally only  make dcache invalid.
 *            4. Currently the corresponding cache inline is clean regardless of whether the address space is valid in the cache or not.
 */
void dcache_clean(unsigned int start, unsigned int end);


/**
 * @brief     This function serves to flush dcache by index way(dcache write back and invalid).
 * @param[in] start - start addr (the mspi range : 0x20000000 - 0x24000000).
 * @param[in] end - end addr (the mspi range : 0x20000000 - 0x24000000).
 * @return    none
 * @note      1. The dcache size is 4k, and the (end - start) value is generally within 4k;
 *            2. the mspi access to the space belongs to the Write-through region, write-through characteristics of the D25F write data to the cache
 *            at the same time will also write data to the next level of storage, equivalent to the clean function.
 *            3. the mspi is connected to flash, can not pointer write, only through the interface way to write, can pointer read,
 *            generally only  make dcache invalid.
 *            4. Currently the corresponding cache inline is flush regardless of whether the address space is valid in the cache or not.
 */
void dcache_flush(unsigned int start, unsigned int end);
#endif
