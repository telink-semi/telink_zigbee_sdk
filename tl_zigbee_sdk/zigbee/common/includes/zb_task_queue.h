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
