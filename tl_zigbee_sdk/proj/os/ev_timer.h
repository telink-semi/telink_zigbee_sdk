/********************************************************************************************************
 * @file	ev_timer.h
 *
 * @brief	This is the header file for ev_timer
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


/**
 *  @brief Definition for timer event
 */
#define TIMER_EVENT_NUM		24

/**
 *  @brief Type definition for timer callback function
 */
typedef int (*ev_timer_callback_t)(void *data);
 
/**
 *  @brief Type definition for timer event
 */
typedef struct ev_timer_event_t {
	struct ev_timer_event_t *next;  //!< Used internal

    ev_timer_callback_t     cb;     //!< Callback function when expire, this must be specified
    void *data;         			//!< Callback function arguments.
    u32 timeout;             		//!< In millisecond
    u32 period;      				//!< Used internal
	u8 resv[3];
    u8 used;
} ev_timer_event_t;

typedef struct ev_timer_event_pool_s {
	ev_timer_event_t evt[TIMER_EVENT_NUM];
    u8 used_num;
} ev_timer_event_pool_t;


bool ev_timer_enough(void);
void ev_timer_update(u32 updateTime);
void ev_timer_setPrevSysTick(u32 tick);

/**
 * @brief       EV timer pool initialization
 *
 * @param[in]   None
 *
 * @return      None
 */
void ev_timer_init(void);

/**
 * @brief       Process EV timer events
 *
 * @param[in]   None
 *
 * @return      None
 */
void ev_timer_process(void);

/**
 * @brief       Get the nearest EV timer events
 *
 * @param[in]   None
 *
 * @return      None
 */
ev_timer_event_t *ev_timer_nearestGet(void);

/**
  * @brief       Check whether a specified timer exist or not
  *
  * @param[in]   evt  - The specified timer event
  *
  * @return      True indicating the timer is already exist. <BR>
  *              False indicating the timer is not exist. <BR>
  */
bool ev_timer_exist(ev_timer_event_t *evt);

/**
  * @brief       Set a new timer
  *
  * @param[in]   evt  		- The timer event including the callback function
  * @param[in]   timeout 	- Timeout in millisecond
  *
  * @return      None
  */
void ev_on_timer(ev_timer_event_t *evt, u32 timeout);

/**
  * @brief       Cancel an existed timer
  *
  * @param[in]   e  - The existed timer event
  *
  * @return      None
  */
void ev_unon_timer(ev_timer_event_t *evt);

/**
  * @brief       push timer task to task list
  *
  * @param[in]   func - the callback of the timer event
  *
  * @param[in]   arg - the parameter to the callback
  *
  * @param		 cycle - the timer interval
  *
  * @return      the status
  */
ev_timer_event_t *ev_timer_taskPost(ev_timer_callback_t func, void *arg, u32 t_ms);
#define TL_ZB_TIMER_SCHEDULE(cb, arg, timeout) 	(ev_timer_taskPost((cb), (arg), (timeout)))

/**
  * @brief       cancel timer task from task list
  *
  * @param[in]   te - the pointer to the the timer event pointer
  *
  * @return      the status
  */
u8 ev_timer_taskCancel(ev_timer_event_t **evt);
#define TL_ZB_TIMER_CANCEL(evt)	(ev_timer_taskCancel(evt))
