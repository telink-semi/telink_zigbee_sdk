/********************************************************************************************************
 * @file	ev_buffer.h
 *
 * @brief	This is the header file for ev_buffer
 *
 * @author	Zigbee Group
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




/** @addtogroup  TELINK_COMMON_MODULE TELINK Common Module
 *  @{
 */
#define EV_BUFFER_DEBUG						0

/** @addtogroup  EV_BUFFER EV Buffer
 *  @{
 */
 
/** @defgroup EV_BUFFER_CONSTANT EV Buffer Constants
 *  @{
 */


/** @addtogroup ev_buffer_groups EV Buffer Groups 
 * Definition the length of each buffer group
 * @{
 */
#define BUFFER_GROUP_0                   	24
#define BUFFER_GROUP_1                   	60
#define BUFFER_GROUP_2                   	150
#define BUFFER_GROUP_3                   	1024
#define MAX_BUFFER_SIZE                  	BUFFER_GROUP_3

/** @} end of group ev_buffer_groups */

/**
 * @brief Default buffer number in each group
 */
#define BUFFER_NUM_IN_GROUP0             	8
#define BUFFER_NUM_IN_GROUP1             	8
#define BUFFER_NUM_IN_GROUP2             	8
#define BUFFER_NUM_IN_GROUP3             	1

/** @addtogroup ev_buffer_typical_size EV Buffer Typical Application Size
 * Definition default buffer size for different typical usage
 * @{
 */
#define SMALL_BUFFER                     	(BUFFER_GROUP_0 - 8)
#define LARGE_BUFFER                     	(BUFFER_GROUP_3 - 8)


/** @} end of group ev_buffer_typical_size */

/** @} end of group EV_BUFFER_CONSTANT */


/** @defgroup EV_BUFFER_TYPE EV Buffer Types
 *  @{
 */

/**
 *  @brief Definition of a buffer item, it is internal used.
 */
typedef struct ev_bufItem {
    struct ev_bufItem *next;
#if EV_BUFFER_DEBUG
    u8  groupIndex;
    u8  flag;
    u16 line;
#else
    u32 groupIndex;
#endif
    u8  data[1];
} ev_bufItem_t;

/**
 *  @brief Definition error code of EV buffer operation
 */
typedef enum buf_sts_e {
    // SUCCESS always be ZERO
	BUFFER_SUCC,
    BUFFER_INVALID_PARAMETER = 1,  //!< Invalid parameter passed to the buffer API
    BUFFER_DUPLICATE_FREE          //!< The same buffer is freed more than once
} buf_sts_t;

/**  @} end of group EV_BUFFER_TYPE */


/** @defgroup EV_BUFFER_FUNCTIONS EV Buffer API
 *  @brief Function declaration of EV Buffer module
 *  @{
 */

/**
 * @brief       Reset the EV Buffer module
 *
 * @param       None
 *
 * @return      None
 */
void ev_buf_reset(void);

 /**
  * @brief       Initialize the EV Buffer module
  *
  * @param       None
  *
  * @return      None
  */
void ev_buf_init(void);

/**
 * @brief       Allocate an available buffer according to the requested size
 *              The allocated buffer will have only three kind of size, defined
 *              in @ref EV_BUFFER_CONSTANT
 *
 * @param       size - The requested size
 *
 * @return      Pointer to an allocated buffer.
 *              NULL means the there is no available buffer.
 */
#if EV_BUFFER_DEBUG
u8 *my_ev_buf_allocate(u16 size, u16 line);
#define ev_buf_allocate(size)	my_ev_buf_allocate(size, __LINE__)
#else
u8 *ev_buf_allocate(u16 size);
#endif

/**
 * @brief       Free the specified buffer
 *
 * @param       pBuf - the pointer to the specified buffer to free.
 *
 * @return      Status.
 */
#if EV_BUFFER_DEBUG
buf_sts_t my_ev_buf_free(u8 *pBuf, u16 line);
#define ev_buf_free(pBuf)	my_ev_buf_free(pBuf, __LINE__)
#else
buf_sts_t ev_buf_free(u8 *pBuf);
#endif

/**
 * @brief       Get the header of a buffer item
 *
 * @param       pd - the pointer of a data, which is previously allocated
 *
 * @return      Pointer of bufferItem
 */
ev_bufItem_t *ev_buf_getHead(u8 *pd);

/**
 * @brief       judge if the buffer is ev buffer
 *
 * @param       the buffer adrress
 *
 * @return      1: if ev buffer; 0: others
 */
u8 is_ev_buf(void *arg);

u16 ev_buf_getFreeMaxSize(void);

u8 *long_ev_buf_get(void);
/**  @} end of group EV_BUFFER_FUNCTIONS */

/**  @} end of group EV_BUFFER */

/**  @} end of group TELINK_COMMON_MODULE */


