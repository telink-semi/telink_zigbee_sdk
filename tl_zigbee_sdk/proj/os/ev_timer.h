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


/** @addtogroup  TELINK_COMMON_MODULE TELINK Common Module
 *  @{
 */

/** @addtogroup  EV_TIMER EV Timer
 *  @{
 */
 
/** @defgroup EV_TIMER_TYPE EV Timer Types
 *  @{
 */

/**
 *  @brief Type definition for timer callback function
 */
typedef int (*ev_timer_callback_t)(void *data);
 
/**
 *  @brief Definition for timer event
 */
#define TIMER_EVENT_NUM		24

typedef struct ev_time_event_t {
    ev_timer_callback_t     cb;            //!< Callback function when expire, this must be specified

    u32                     t;             //!< Used internal

    u32                     interval;      //!< Used internal

    void                    *data;         //!< Callback function arguments.

    struct ev_time_event_t  *next;         //!< Used internal

    u8                      used;
	u8						resv;
	u8						allig[2];
} ev_time_event_t;

typedef struct ev_time_event_pool_s {
   ev_time_event_t evt[TIMER_EVENT_NUM];
   s32 used_num;
} ev_time_event_pool_t;

extern u8 TIMER_EVENT_SIZE;

/** @} end of group EV_TIMER_TYPE */

/** @defgroup EV_TIMER_FUNCTION EV Timer API
 *  @{
 */

/**
  * @brief       Set a new timer
  *
  * @param[in]   e  - The timer event including the callback function
  * @param[in]   us - Interval time in microsecond
  *
  * @return      None
  */
void ev_on_timer(ev_time_event_t *e, u32 t_us);

/**
  * @brief       Cancel an existed timer
  *
  * @param[in]   e  - The existed timer event
  *
  * @return      None
  */
void ev_unon_timer(ev_time_event_t *e);

/**
  * @brief       Check whether a specified timer exist or not
  *
  * @param[in]   e  - The specified timer event
  *
  * @return      True indicating the timer is already exist. <BR>
  *              False indicating the timer is not exist. <BR>
  */
bool ev_timer_exist(const ev_time_event_t *e);

/**
  * @brief       Get the nearest fired timer interval
  *
  * @param[in]   None
  *
  * @return      Value of the interval.
  */
u32 ev_nearestInterval(void);

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
ev_time_event_t *ev_timerTaskPost(ev_timer_callback_t func, void *arg, u32 t_us);
#define TL_ZB_TIMER_SCHEDULE 	ev_timerTaskPost

/**
  * @brief       cancel timer task from task list
  *
  * @param[in]   te - the pointer to the the timer event pointer
  *
  * @return      the status
  */
u8 ev_timerTaskCancel(ev_time_event_t **te);
#define TL_ZB_TIMER_CANCEL 		ev_timerTaskCancel

bool ev_timerTaskIdle(void);
bool ev_timerTaskQEnough(void);
void ev_timerTaskQInit(void);

/**  @} end of group EV_TIMER_FUNCTION */

/**  @} end of group EV_TIMER */

/**  @} end of group TELINK_COMMON_MODULE */
