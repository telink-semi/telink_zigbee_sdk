/********************************************************************************************************
 * @file	mempool.h
 *
 * @brief	This is the header file for mempool
 *
 * @author	Driver & Zigbee Group
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
#pragma once


typedef struct mem_block_t{
	struct mem_block_t	*next_block;
    char        		data[4];		// must 4 or 8 aligned, padding
}mem_block_t;

typedef struct mem_pool_t{
    mem_block_t	*free_list;
}mem_pool_t;

#define MEMPOOL_ALIGNMENT 	4
#define MEMPOOL_ITEMSIZE_2_BLOCKSIZE(s)		((s + (MEMPOOL_ALIGNMENT - 1)) & ~(MEMPOOL_ALIGNMENT-1))

#define MEMPOOL_DECLARE(pool_name, pool_mem, itemsize, itemcount)	\
	mem_pool_t pool_name;											\
	u8 pool_mem[MEMPOOL_ITEMSIZE_2_BLOCKSIZE(itemsize) * itemcount] _attribute_aligned_(4);

mem_pool_t *mempool_init(mem_pool_t *pool, void *mem, int itemsize, int itemcount);
void *mempool_alloc(mem_pool_t *pool);
void mempool_free(mem_pool_t *pool, void *p);
mem_block_t* mempool_header(char *pd);


