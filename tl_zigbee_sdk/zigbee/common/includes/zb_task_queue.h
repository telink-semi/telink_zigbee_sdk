/********************************************************************************************************
 * @file	zb_task_queue.h
 *
 * @brief	This is the header file for zb_task_queue
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
#ifndef ZB_TASK_QUEUE_H
#define ZB_TASK_QUEUE_H



#define	TL_ZBTASKQ_USERUSE_SIZE				32
#define	TL_ZBTASKQ_STACKUSE_SIZE			16

enum{
	ZB_RET_OK,			/*!< status: success */
	ZB_RET_OVERFLOW,	/*!< status: array or buffer overflow */
}zb_sta_e;

enum{
	TL_Q_EV_TASK = 0,
	TL_Q_MAC2NWK,
	TL_Q_NWK2MAC,
	TL_Q_HIGH2NWK,
	TL_Q_NWK2HIGH,
	TL_Q_TYPE_MAX
};

/**
   Callback function typedef.
   Callback is function planned to execute by another function.
   Note that callback must be declared as reentrant for dscc.

   @param param - callback parameter - usually, but not always, ref to packet buf

   @return none.
 */
typedef void (*tl_zb_callback_t)(void *arg);


typedef struct tl_zb_task_s{
	 tl_zb_callback_t tlCb;
	 void *data;
}tl_zb_task_t;

typedef struct{
	tl_zb_task_t evt[TL_ZBTASKQ_USERUSE_SIZE];
	u8 wptr;
	u8 rptr;
}tl_zbtaskq_user_t;

typedef struct{
	tl_zb_task_t evt[TL_ZBTASKQ_STACKUSE_SIZE];
	u8 wptr;
	u8 rptr;
	u8 used;
	u8 resv;
}tl_zbtaskq_stack_t;

#define	TL_QUEUE_HAS_SPACE(wptr, rptr, size)		((wptr - rptr) < (size))

/**
   Initialize scheduler subsystem.
 */
void zb_sched_init(void);


void tl_zbTaskProcedure(void);

/**
  * @brief       get the valid task from task quenue list
  *
  * @param[in]   idx - the index of the task list
  *
  * @return      the pointer to the task list
  */
tl_zb_task_t *tl_zbTaskQPop(u8 idx, tl_zb_task_t *taskInfo);


/**
  * @brief       push task to task list
  *
  * @param[in]   idx - the index of the task list
  *
  * @param[in]   task - the task will be push to task list
  *
  * @return      the status
  */
u8 tl_zbTaskQPush(u8 idx, tl_zb_task_t *task);


#if 0
u8 my_tl_zbPrimitivePost(u8 layerQ, u8 primitive, void *arg, u32 line, char *file);
#define tl_zbPrimitivePost(layerQ, primitive, arg) my_tl_zbPrimitivePost(layerQ, primitive, arg, __LINE__, __FILE__)
#else
u8 tl_zbPrimitivePost(u8 layerQ, u8 primitive, void *arg);
#endif

/**
  * @brief       push a task to task list
  *
  * @param[in]   func - the callback of the event
  *
  * @param[in]   arg - the parameter to the callback
  *
  * @return      the status
  */
u8 tl_zbTaskPost(tl_zb_callback_t func, void *arg);
#define	TL_SCHEDULE_TASK	tl_zbTaskPost


u8 zb_isTaskDone(void);
u8 tl_zbUserTaskQNum(void);
void secondClockStop(void);


#endif /* ZB_TASK_QUEUE_H */
