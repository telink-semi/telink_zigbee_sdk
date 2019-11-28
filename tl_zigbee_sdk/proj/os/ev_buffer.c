/********************************************************************************************************
 * @file     ev_buffer.c
 *
 * @brief    Implementation of Telink EV Buffer Module
 *
 * @author   jian.zhang@telink-semi.com
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
#include "ev_buffer.h"
#include "ev.h"
#include "platform_includes.h"
#include "user_config.h"
#include "../common/mempool.h"
#include "../common/utility.h"
#include "../common/string.h"
#include "../common/assert.h"


#ifdef WIN32
#include <malloc.h>
#endif


#if (MODULE_BUFM_ENABLE)

#define DEFAULT_BUFFER_GROUP_NUM                 3


/**************************** Private Variable Definitions *******************/

typedef struct {
	ev_bufItem_t *qHead;
	u16 size;
	u8 availBufNum;
	u8 reserved;
} ev_buf_groups_t;


typedef struct bufm_vars {
    ev_buf_groups_t bufGroups[DEFAULT_BUFFER_GROUP_NUM];
} ev_buf_vars_t;


ev_buf_vars_t ev_buf_vs;
ev_buf_vars_t *ev_buf_v = &ev_buf_vs;


MEMPOOL_DECLARE(size_0_pool, size_0_mem, BUFFER_GROUP_0, BUFFER_NUM_IN_GROUP0);
MEMPOOL_DECLARE(size_1_pool, size_1_mem, BUFFER_GROUP_1, BUFFER_NUM_IN_GROUP1);
MEMPOOL_DECLARE(size_2_pool, size_2_mem, BUFFER_GROUP_2, BUFFER_NUM_IN_GROUP2);

/*********************************************************************
 * @fn      ev_buf_isExisted
 *
 * @brief   Return whether the buffer is in the available buffer 
 *
 * @param   pd - the pointer of a data, which is previously allocated
 *
 * @return  Pointer of bufferItem
 */
u8 ev_buf_isExisted(u8 index, mem_block_t* block)
{
    mem_pool_t *pool = (mem_pool_t*)ev_buf_v->bufGroups[index].qHead;
    mem_block_t* curBlock = pool->free_list;

    while (curBlock) {
        if (block == curBlock) {
            return 1;
        } 
        curBlock = curBlock->next_block;
    }
    return 0;
}

u8* ev_buf_retriveMempoolHeader(u8* pd)
{
    return pd - (OFFSETOF(ev_bufItem_t, data) - OFFSETOF(mem_block_t, data));
}


/*********************************************************************
 * @fn      ev_buf_reset
 *
 * @brief   Reset the EV Buffer module
 *
 * @param   None
 *
 * @return  None
 */
//volatile u8 wise_var = 1;
void ev_buf_reset(void)
{
    int i;

    u16 size[DEFAULT_BUFFER_GROUP_NUM] = {BUFFER_GROUP_0, BUFFER_GROUP_1, BUFFER_GROUP_2};
    mem_pool_t *memPool[DEFAULT_BUFFER_GROUP_NUM] = {&size_0_pool, &size_1_pool, &size_2_pool};
    u8 *mem[DEFAULT_BUFFER_GROUP_NUM] = {size_0_mem, size_1_mem, size_2_mem};
    u8 buffCnt[DEFAULT_BUFFER_GROUP_NUM] = {BUFFER_NUM_IN_GROUP0, BUFFER_NUM_IN_GROUP1, BUFFER_NUM_IN_GROUP2};

    memset((u8 *)ev_buf_v, 0, sizeof(ev_buf_vars_t));

    /* reinitialize available buffer */
    for(i = 0; i < DEFAULT_BUFFER_GROUP_NUM; i++){
        ev_buf_v->bufGroups[i].availBufNum = buffCnt[i];
        ev_buf_v->bufGroups[i].qHead = (ev_bufItem_t*)mempool_init(memPool[i], mem[i], size[i], buffCnt[i]);
        ev_buf_v->bufGroups[i].size = size[i];
    }  
}

/*********************************************************************
 * @fn      ev_buf_init
 *
 * @brief   Initialize the EV Buffer module
 *
 * @param   None
 *
 * @return  None
 */
void ev_buf_init(void)
{
    ev_buf_reset();
}

/*********************************************************************
 * @fn      ev_buf_allocate
 *
 * @brief   Allocate an available buffer according to the requested size
 *          The allocated buffer will have only three kind of size, defined 
 *          in @ref EV_BUFFER_CONSTANT 
 *
 * @param   size - requested size
 *
 * @return  Pointer to an allocated buffer.
 *          NULL means the there is no available buffer.
 */

#if EV_BUFFER_DEBUG
u8 *my_ev_buf_allocate(u16 size, u16 line)
#else
u8 *ev_buf_allocate(u16 size)
#endif
{
    if((size == 0) || (size > MAX_BUFFER_SIZE)){
        /* the size parameter is wrong */
        return NULL;
    }
    u8 r = irq_disable();
    u8 index = U8_MAX;
    ev_bufItem_t *pNewBuf;
    /* find related the buffer blocks */
    for(u8 i = 0; i < DEFAULT_BUFFER_GROUP_NUM; i++){
        if(size <= ev_buf_v->bufGroups[i].size - OFFSETOF(ev_bufItem_t, data)){
            index = i;
            break;
        }
    }
    if((index == U8_MAX ) || (!ev_buf_v->bufGroups[index].availBufNum)){
        /* no available buffer */
    	irq_restore(r);
        return NULL;
    }
    u8 *temp = (u8*)mempool_alloc((mem_pool_t*)ev_buf_v->bufGroups[index].qHead);
    if(!temp){
    	irq_restore(r);
    	return NULL;
    }
    ev_buf_v->bufGroups[index].availBufNum--;
    pNewBuf = (ev_bufItem_t*)(temp - 4);
    pNewBuf->groupIndex = index;
#if EV_BUFFER_DEBUG
    pNewBuf->line = line;
    pNewBuf->flag = 0xfe;
#endif
    irq_restore(r);
    return pNewBuf->data;
}


/*********************************************************************
 * @fn      ev_buf_free
 *
 * @brief   Free the specified buffer 
 *
 * @param   pBuf - the pointer to the specified buffer to free.
 *
 * @return  status
 */
#if EV_BUFFER_DEBUG
volatile u32 T_DBG_evFreeBuf = 0;
volatile u16 T_DBG_evFreeBufLine = 0;
buf_sts_t my_ev_buf_free(u8* pBuf, u16 line)
#else
buf_sts_t ev_buf_free(u8* pBuf)
#endif
{
    u8 r = irq_disable();

    if(!is_ev_buf(pBuf)){
#if EV_BUFFER_DEBUG
    	T_DBG_evFreeBuf = (u32)pBuf;
    	T_DBG_evFreeBufLine = line;
#endif

    	ZB_EXCEPTION_POST(SYS_EXCEPTTION_EV_BUFFER_EXCEPTION_FREE_OVERFLON);
    }

    ev_bufItem_t *pDelBuf = ev_buf_getHead(pBuf);
    /* check whether the buffer is duplicated release */
    if(ev_buf_isExisted(pDelBuf->groupIndex, (mem_block_t*)pDelBuf)){

#if EV_BUFFER_DEBUG
    	T_DBG_evFreeBuf = (u32)pBuf;
    	T_DBG_evFreeBufLine = line;
#endif

    	ZB_EXCEPTION_POST(SYS_EXCEPTTION_EV_BUFFER_EXCEPTION_FREE_MULIT);

        irq_restore(r);
        return BUFFER_DUPLICATE_FREE;
    }

    mempool_free((mem_pool_t*)ev_buf_v->bufGroups[pDelBuf->groupIndex].qHead, ev_buf_retriveMempoolHeader(pBuf));
    ev_buf_v->bufGroups[pDelBuf->groupIndex].availBufNum++;    

#if EV_BUFFER_DEBUG
    pDelBuf->line = line;
    pDelBuf->flag = 0xff;
#endif

    irq_restore(r);
    return BUFFER_SUCC;
}



/*********************************************************************
 * @fn      ev_buf_getHead
 *
 * @brief   Get the header pointer of a buffer item 
 *
 * @param   pd - the pointer of a data, which is previously allocated
 *
 * @return  Pointer of bufferItem
 */
ev_bufItem_t* ev_buf_getHead(u8* pd)
{
    return (ev_bufItem_t*)(pd - OFFSETOF(ev_bufItem_t, data));
}

/*********************************************************************
 * @fn      ev_buf_getTail
 *
 * @brief   Get the pointer from a EV BUFFER tail.
 *
 * @param   pd - the pointer of a data, which is previously allocated
 * @param   offsetToTail - The offset to Tail
 *
 * @return  Pointer of the specified memory
 */
u8* ev_buf_getTail(u8* pd, int offsetToTail)
{
	u32 index;
	u16 size[DEFAULT_BUFFER_GROUP_NUM] = {BUFFER_GROUP_0, BUFFER_GROUP_1, BUFFER_GROUP_2};

	memcpy((u8*)&index, pd - 4, 4);
	assert((index < 3) && (index >= 0));
	return (u8*)(pd - 8 + size[index] - offsetToTail);
}


u8 is_ev_buf(void *arg){
	 if( ((u32)arg >= (u32)(size_0_mem) && (u32)arg <= ((u32)(size_0_mem) + sizeof(size_0_mem))) ||
		  ((u32)arg >= (u32)(size_1_mem) && (u32)arg <= ((u32)(size_1_mem) + sizeof(size_1_mem))) ||
		  ((u32)arg >= (u32)(size_2_mem) && (u32)arg <= ((u32)(size_2_mem) + sizeof(size_2_mem))) ){
		 return 1;
	 }
	 return 0;
}

u16 ev_buf_getFreeMaxSize(void)
{
	u16 size = 0;

	for(u8 i = 0; i < DEFAULT_BUFFER_GROUP_NUM; i++){
		if(ev_buf_v->bufGroups[i].availBufNum){
			if((ev_buf_v->bufGroups[i].size - OFFSETOF(ev_bufItem_t, data)) > size){
				size = ev_buf_v->bufGroups[i].size - OFFSETOF(ev_bufItem_t, data);
			}
		}
	}

	return size;
}

#endif  /* MODULE_BUFM_ENABLE */






