/********************************************************************************************************
 * @file    cache.c
 *
 * @brief   This is the source file for TL321X
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
#include "cache.h"
/**
 * @brief     This function serves to extract cache line from memory address.
 * @param[in] addr   -  address  (the mspi range : 0x20000000 - 0x24000000)
 * @return    none
 */
static cache_line_t cache_get_line(unsigned int addr){
    cache_line_t spec;
    // Extract Way
    spec.way = (addr >> 12) & 0xfffff;
    // Extract Index
    spec.index = (addr >> 5) & 0x7f;
    // Extract Word
    spec.word = addr & 0x1f;
    return spec;
}

/**
 * @brief     This function serves to invalid dcache by address(tag invalid).
 * @param[in] start - start addr (the mspi range : 0x20000000 - 0x24000000).
 * @param[in] end - end addr (the mspi range : 0x20000000 - 0x24000000).
 * @return    none
 * @note      1. The dcache size is 4k, and the (end - start) value is generally within 4k;
 *            2. Currently the corresponding cache inline is invalidate regardless of whether the address space is valid in the cache or not.
 */
void dcache_invalidate(unsigned int start, unsigned int end){
    cache_line_t start_addr = cache_get_line(start);
    cache_line_t end_addr = cache_get_line(end);
    start_addr.way =0;
    for(unsigned char i=start_addr.index;i<=end_addr.index;i++){
         start_addr.index =i;
         write_csr(NDS_MCCTLBEGINADDR,*(unsigned int*)&start_addr);
         write_csr(NDS_MCCTLCOMMAND,0x10);
    }
}

/**
 * @brief     This function serves to invalid dcache by index way(tag invalid).
 * @param[in] start - start addr (the mspi range : 0x20000000 - 0x24000000).
 * @param[in] end - end addr (the mspi range : 0x20000000 - 0x24000000).
 * @return    none
 * @note      1. The dcache size is 4k, and the (end - start) value is generally within 4k;
 *            2. Currently the corresponding cache inline is invalidate regardless of whether the address space is valid in the cache or not.
 */
void icache_invalidate(unsigned int start, unsigned int end){
    cache_line_t start_addr = cache_get_line(start);
    cache_line_t end_addr = cache_get_line(end);
    for(unsigned char j=0;j<2;j++){
        start_addr.way =j;
        for(unsigned char i=start_addr.index;i<=end_addr.index;i++){
             start_addr.index =i;
             write_csr(NDS_MCCTLBEGINADDR,*(unsigned int*)&start_addr);
             write_csr(NDS_MCCTLCOMMAND,0x18);
        }
    }
}

/*
 *  It should be noted that:
 *  1. the mspi access to the space belongs to the Write-through region and Write-no-allocate, write-through characteristics of the D25F write data to the cache
 *     at the same time will also write data to the next level of storage, equivalent to the clean function.
 */
#if(DCACHE_CLEAN_OR_FLUSH_IS_USED)
/**
 * @brief     This function serves to clean dcache by index way(dcache write back).
 * @param[in] start - start addr (the mspi range : 0x20000000 - 0x24000000).
 * @param[in] end - end addr (the mspi range : 0x20000000 - 0x24000000).
 * @return    none
 * @note      1. The dcache size is 4k, and the (end - start) value is generally within 4k;
 *            2. Currently the corresponding cache inline is clean regardless of whether the address space is valid in the cache or not.
 */
void dcache_clean(unsigned int start, unsigned int end){
    cache_line_t start_addr = cache_get_line(start);
    cache_line_t end_addr = cache_get_line(end);
    start_addr.way =0;
    for(unsigned char i=start_addr.index;i<=end_addr.index;i++){
         start_addr.index =i;
         write_csr(NDS_MCCTLBEGINADDR,*(unsigned int*)&start_addr);
         write_csr(NDS_MCCTLCOMMAND,0x11);
    }
}


/**
 * @brief     This function serves to flush dcache by index way(dcache write back and invalid).
 * @param[in] start - start addr (the mspi range : 0x20000000 - 0x24000000).
 * @param[in] end - end addr (the mspi range : 0x20000000 - 0x24000000).
 * @return    none
 * @note      1. The dcache size is 4k, and the (end - start) value is generally within 4k;
 *            2. Currently the corresponding cache inline is flush regardless of whether the address space is valid in the cache or not.
 */
void dcache_flush(unsigned int start, unsigned int end){
    cache_line_t start_addr = cache_get_line(start);
    cache_line_t end_addr = cache_get_line(end);
    start_addr.way =0;
    for(unsigned char i=start_addr.index;i<=end_addr.index;i++){
         start_addr.index =i;
         write_csr(NDS_MCCTLBEGINADDR,*(unsigned int*)&start_addr);
         write_csr(NDS_MCCTLCOMMAND,0x12);
    }
}
#endif
