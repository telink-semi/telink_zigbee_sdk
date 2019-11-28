/********************************************************************************************************
 * @file     mempool.c
 *
 * @brief    memory pool configuration interface file
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
#include "mempool.h"
#include "utility.h"

mem_pool_t* mempool_init(mem_pool_t* pool, void* mem, int itemsize, int itemcount)
{
    if (!pool || !mem) 	
		return (0);
	
	pool->free_list = (mem_block_t*)mem;

    u32 block_size = (u32)(MEMPOOL_ITEMSIZE_2_BLOCKSIZE(itemsize));
	mem_block_t* tmp = (mem_block_t*)mem;
	int i;
	for(i = 0; i < itemcount - 1; ++i){
		tmp = tmp->next_block = (mem_block_t*)(((u32)tmp) + block_size);
	}
	tmp->next_block = 0;
	return pool;
}

mem_block_t* mempool_header(char* pd){
	return (mem_block_t*)(pd - OFFSETOF(mem_block_t, data));
}

void* mempool_alloc(mem_pool_t* pool)
{
	if(!pool->free_list)
		return 0;
	mem_block_t* tmp = pool->free_list;
	pool->free_list = tmp->next_block;
	return &tmp->data;
}

void mempool_free(mem_pool_t* pool, void* p)
{
	mem_block_t* tmp = mempool_header((char*)p);
	tmp->next_block = pool->free_list;
	pool->free_list = tmp;
}
