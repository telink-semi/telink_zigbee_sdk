/********************************************************************************************************
 * @file	mempool.c
 *
 * @brief	This is the source file for mempool
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
#include "mempool.h"
#include "utility.h"

mem_pool_t *mempool_init(mem_pool_t *pool, void *mem, int itemsize, int itemcount)
{
    if(!pool || !mem)
		return (0);
	
	pool->free_list = (mem_block_t *)mem;

	unsigned int block_size = (unsigned int)(MEMPOOL_ITEMSIZE_2_BLOCKSIZE(itemsize));
	mem_block_t *tmp = (mem_block_t *)mem;
	int i;
	for(i = 0; i < itemcount - 1; ++i){
		tmp = tmp->next_block = (mem_block_t *)(((unsigned int)tmp) + block_size);
	}
	tmp->next_block = 0;
	return pool;
}

mem_block_t *mempool_header(char *pd)
{
	return (mem_block_t *)(pd - OFFSETOF(mem_block_t, data));
}

void *mempool_alloc(mem_pool_t *pool)
{
	if(!pool->free_list)
		return 0;

	mem_block_t *tmp = pool->free_list;
	pool->free_list = tmp->next_block;
	return &tmp->data;
}

void mempool_free(mem_pool_t *pool, void *p)
{
	mem_block_t *tmp = mempool_header((char *)p);
	tmp->next_block = pool->free_list;
	pool->free_list = tmp;
}
