/********************************************************************************************************
 * @file	ev_queue.h
 *
 * @brief	This is the header file for ev_queue
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

/** @addtogroup  EV_QUEUE EV Queue
 *  @{
 */
 
/** @defgroup EV_QUEUE_TYPE EV QUEUE Types
 *  @{
 */

/**        
 *  @brief Definition for priority calculate function
 */
typedef u32 (*ev_priFunc_t)(arg_t);

/**        
 *  @brief Structure of an item in the EV Queue
 */
typedef struct queue_item {
    struct queue_item *next;        //!<  Pointer to the next element
    u8 data[1];                     //!<  Pointer to the data
} queue_item_t;


/**
 *  @brief Definition for the EV Queue structure
 */
typedef struct priQueue {
    queue_item_t *head;             //!<  Pointer to the head item of the queue
    queue_item_t *tail;             //!<  Pointer to the tail item of the queue
    ev_priFunc_t priFunc;           //!<  Priority function, NULL means not use priority
    u32 curNum;                     //!<  Current number of entries in the queue
} ev_queue_t;

/**
 *  @brief Definition for the status of the EV Queue Module
 */
typedef enum ev_queue_sts_e {
    // SUCCESS should always be ZERO
	QUEUE_SUCC = 0,
    QUEUE_INVALID_PARAMETER = 1,    //!< Invalid parameter of the API
    QUEUE_EMPTY,                    //!< Queue is empty
    QUEUE_NOT_FOUND,                //!< Queue is not found
    QUEUE_OVERFLOW,                 //!< Queue is overflowed
} ev_queue_sts_t;

/**  @} end of group EV_QUEUE_TYPE */


/** @defgroup EV_QUEUE_FUNCTIONS EV Queue API
 *  @brief Function declaration of EV Queue module
 *  @{
 */

 /**
   * @brief       Initialize the EV queue 
   *
   * @param[in]   q        - The queue need to use
   * @param[in]   priFunc  - Pointer to the function that calculates a priority
   *                         NULL means not use the priority feature
   *
   * @return      Status
   */
ev_queue_sts_t ev_queue_init( ev_queue_t *q, ev_priFunc_t priFunc);


 /**
   * @brief       Push a data part of an element into a queue
   *
   * @param[in]   q        - The queue that a new element need to push to
   * @param[in]   payload  - The payload of the new element  
   *
   * @return      Status
   */
ev_queue_sts_t ev_queue_push( ev_queue_t* q, u8* payload );


/**
  * @brief       Pop data part of the element from the specified queue.
  *
  * @param[in]   q - The queue that element need to pop from
  *
  * @return      Pointer to data part of the @ev_bufItem_t
  */
u8* ev_queue_pop( ev_queue_t* q );


  /**
   * @brief       Delete an element from the queue
   *
   * @param[in]   q        - The specified queue
   * @param[in]   payload  - Pointer to data part of the buffer that is to be deleted
   *
   * @return      Status
   */
ev_queue_sts_t ev_queue_delete( ev_queue_t* q, u8* payload );


  /**
   * @brief       Free a queue. This also deallocates all buffers in the queue.
   *
   * @param[in]   q - The specified queue to free
   *
   * @return      Status
   */
ev_queue_sts_t ev_queue_freeQ( ev_queue_t* q);


  /**
   * @brief       Push a raw data into a queue
   *              The element must compatible with the format @ref queue_item_t
   *
   * @param[in]   q       - The specified queue
   * @param[in]   element - Pointer to the new element  
   *
   * @return      Status
   */
ev_queue_sts_t ev_queue_rawPush( ev_queue_t* q, queue_item_t* element );


 /**
   * @brief       Delete an element from the queue
   *              The element must compatible with the format @ref queue_item_t
   *
   * @param[in]   q       - The specified queue
   * @param[in]   element - Pointer to data part of the bufferItem that is to be deleted
   *
   * @return      Status
   */
ev_queue_sts_t ev_queue_rawDelete(ev_queue_t* q, queue_item_t* element);


 /**
  * @brief       Pop data part of the element from the speified queue. 
  *
  * @param[in]   q - The specified queue
  *
  * @return      Pointer to first element in the queue
  */
queue_item_t* ev_queue_rawPop(ev_queue_t* q);

/**  @} end of group EV_QUEUE_FUNCTIONS */

/**  @} end of group EV_QUEUE */

/**  @} end of group TELINK_COMMON_MODULE */

