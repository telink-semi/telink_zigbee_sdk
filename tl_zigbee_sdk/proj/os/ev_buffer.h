/********************************************************************************************************
 * @file    ev_buffer.h
 *
 * @brief   This is the header file for ev_buffer
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
#pragma once


#define EV_BUFFER_DEBUG         0


/**
 * @brief Definition the length of each buffer group
 */
#define BUFFER_GROUP_0          24
#define BUFFER_GROUP_1          60
#define BUFFER_GROUP_2          152
#define BUFFER_GROUP_3          512
#define MAX_BUFFER_SIZE         (BUFFER_GROUP_3 - OFFSETOF(ev_bufItem_t, data))

/**
 * @brief Default buffer number in each group
 */
#if defined(MCU_CORE_8258) && (ZB_ED_ROLE == 1)
#define BUFFER_NUM_IN_GROUP0    4
#define BUFFER_NUM_IN_GROUP1    4
#define BUFFER_NUM_IN_GROUP2    4
#define BUFFER_NUM_IN_GROUP3    2
#else
#define BUFFER_NUM_IN_GROUP0    8
#define BUFFER_NUM_IN_GROUP1    8
#define BUFFER_NUM_IN_GROUP2    8
#define BUFFER_NUM_IN_GROUP3    2
#endif

/**
 * @brief Definition default buffer size for different typical usage
 */
#define SMALL_BUFFER            (BUFFER_GROUP_0 - 8)
#define LARGE_BUFFER            (BUFFER_GROUP_3 - 8)

/**
 * @brief Definition of a buffer item, it is internal used.
 */
typedef struct ev_bufItem {
    struct ev_bufItem *next;
#if EV_BUFFER_DEBUG
    u8 groupIndex;
    u8 flag;
    u16 line;
#else
    u32 groupIndex;
#endif
    u8 data[1];
} ev_bufItem_t;

/**
 * @brief Definition error code of EV buffer operation
 */
typedef enum buf_sts_e {
    BUFFER_SUCC,
    BUFFER_INVALID_PARAMETER = 1, //!< Invalid parameter passed to the buffer API
    BUFFER_DUPLICATE_FREE         //!< The same buffer is freed more than once
} buf_sts_t;


/**
 * @brief  Reset the EV Buffer module
 *
 * @param  none
 *
 * @return none
 */
void ev_buf_reset(void);

/**
 * @brief  Initialize the EV Buffer module
 *
 * @param  none
 *
 * @return none
 */
void ev_buf_init(void);

/**
 * @brief  Allocate an available buffer according to the requested size
 *         The allocated buffer will have only three kind of size, defined
 *         in @ref EV_BUFFER_CONSTANT
 *
 * @param  size - The requested size
 *
 * @return Pointer to an allocated buffer.
 *         NULL means the there is no available buffer.
 */
#if EV_BUFFER_DEBUG
u8 *my_ev_buf_allocate(u16 size, u16 line);
#define ev_buf_allocate(size)   my_ev_buf_allocate(size, __LINE__)
#else
u8 *ev_buf_allocate(u16 size);
#endif

/**
 * @brief  Free the specified buffer
 *
 * @param  pBuf - the pointer to the specified buffer to free.
 *
 * @return status
 */
#if EV_BUFFER_DEBUG
buf_sts_t my_ev_buf_free(u8 *pBuf, u16 line);
#define ev_buf_free(pBuf)       my_ev_buf_free(pBuf, __LINE__)
#else
buf_sts_t ev_buf_free(u8 *pBuf);
#endif

/**
 * @brief  Get the header of a buffer item
 *
 * @param  pd - the pointer of a data, which is previously allocated
 *
 * @return Pointer of bufferItem
 */
ev_bufItem_t *ev_buf_getHead(u8 *pd);

/**
 * @brief  Check if the buffer is ev buffer
 *
 * @param  arg - the buffer address
 *
 * @return 1 - if ev buffer; 0 - others
 */
u8 is_ev_buf(void *arg);

u16 ev_buf_getFreeMaxSize(void);

u8 *long_ev_buf_get(void);
