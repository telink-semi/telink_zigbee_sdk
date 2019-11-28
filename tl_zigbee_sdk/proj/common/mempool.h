/********************************************************************************************************
 * @file     mempool.h
 *
 * @brief    memory pool configuration interface header file
 *
 * @author
 * @date     Oct. 8, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#pragma once

#include "types.h"

typedef struct mem_block_t
{
	struct mem_block_t*	next_block;
    char        	data[4];		// must 4 or 8 aligned, padding
}mem_block_t;

typedef struct mem_pool_t
{
    mem_block_t* 	free_list;
}mem_pool_t;

#define MEMPOOL_ALIGNMENT 	4
#define MEMPOOL_ITEMSIZE_2_BLOCKSIZE(s)		((s + (MEMPOOL_ALIGNMENT - 1)) & ~(MEMPOOL_ALIGNMENT-1))

#define MEMPOOL_DECLARE(pool_name, pool_mem, itemsize, itemcount)	\
	mem_pool_t pool_name;											\
	u8 pool_mem[MEMPOOL_ITEMSIZE_2_BLOCKSIZE(itemsize) * itemcount] _attribute_aligned_(4);

mem_pool_t* mempool_init(mem_pool_t* pool, void* mem, int itemsize, int itemcount);
void* mempool_alloc(mem_pool_t* pool);
void mempool_free(mem_pool_t* pool, void* p);
mem_block_t* mempool_header(char* pd);


