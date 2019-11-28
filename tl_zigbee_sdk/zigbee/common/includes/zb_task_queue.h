/********************************************************************************************************
 * @file     zb_task_queue.h
 *
 * @brief    task queue for zigbee stack
 *
 * @author
 * @date     May. 27, 2017
 *
 * @par      Copyright (c) 2017, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef EV_SCHEDULER_H
#define EV_SCHEDULER_H 1

#include "tl_common.h"


enum{
	ZB_RET_OK,			/*!< status: success */
	ZB_RET_OVERFLOW,	/*!< status: array or buffer overflow */
}zb_sta_e;

/**
   Callback function typedef.
   Callback is function planned to execute by another function.
   Note that callback must be declared as reentrant for dscc.

   @param param - callback parameter - usually, but not always, ref to packet buf

   @return none.
 */
typedef void (* zb_callback_t)(u8 param) ;
typedef void (* tl_zb_callback_t)(void *arg) ;

typedef void (* zb_timer_callback_t)(void *arg) ;

enum{
	TL_Q_EV_TASK = 0,
	TL_Q_MAC2NWK,
	TL_Q_NWK2MAC,
	TL_Q_HIGH2NWK,
	TL_Q_NWK2HIGH,
	TL_Q_TYPE_MAX
};

typedef struct tl_zb_task_s{
	 tl_zb_callback_t tlCb;
	 void *data;
}tl_zb_task_t;


#define	TL_ZBTASKQ_USERUSE_SIZE				32
#define	TL_ZBTASKQ_STACKUSE_SIZE			16

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

#define		TL_QUEUE_HAS_SPACE(wptr,rptr,size)		((wptr - rptr) < (size))


extern u8 TIMER_EVENT_SIZE;

/**
   Initialize scheduler subsystem.
 */
extern void zb_sched_init() ;


void tl_zbTaskProcedure(void) ;

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
extern ev_time_event_t *tl_zbTimerTaskPost(ev_timer_callback_t func, void *arg, u32 t_us);
#define TL_ZB_TIMER_SCHEDULE 	tl_zbTimerTaskPost

#define		TL_SUPERFRAMETIME_TO_US(n)				(n*15360)
#define		TL_TIMEUS_TO_SUPEFRAMETIME(t)			(t/15360)
/**
  * @brief       cancel timer task from task list
  *
  * @param[in]   te - the pointer to the the timer event pointer
  *
  * @return      the status
  */
u8 tl_zbTimerTaskCancel(ev_time_event_t **te);
#define TL_ZB_TIMER_CANCEL 		tl_zbTimerTaskCancel

void tl_zbTimerTaskChkAndDec(ev_time_event_t *e);

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

u8 zb_timerTaskIdle(void);
u8 zb_isTimerTaskQEnough(void);
u8 zb_isTaskDone(void);
bool tl_stackBusy(void);

void secondClockStop(void);

u8 tl_zbUserTaskQNum(void);

#endif /* EV_SCHEDULER_H */
