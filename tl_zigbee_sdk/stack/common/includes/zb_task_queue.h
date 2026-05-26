/********************************************************************************************************
 * @file    zb_task_queue.h
 *
 * @brief   This is the header file for zb_task_queue
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
#ifndef ZB_TASK_QUEUE_H
#define ZB_TASK_QUEUE_H


#if defined(MCU_CORE_8258) && ZB_ED_ROLE
#define	TL_ZBTASKQ_USERUSE_SIZE         16
#define	TL_ZBTASKQ_STACKUSE_SIZE        8
#else
#define	TL_ZBTASKQ_USERUSE_SIZE         32
#define	TL_ZBTASKQ_STACKUSE_SIZE        16
#endif

enum {
    ZB_RET_OK,          /*!< status: success */
    ZB_RET_OVERFLOW,    /*!< status: array or buffer overflow */
};

enum {
    TL_Q_EV_TASK = 0,
    TL_Q_MAC2NWK,
    TL_Q_NWK2MAC,
    TL_Q_HIGH2NWK,
    TL_Q_NWK2HIGH,
    TL_Q_TYPE_MAX
};

/**
 * @brief  Callback is function planned to execute by another function.
 *         Note that callback must be declared as reentrant for dscc.
 *
 * @param  param - callback parameter - usually, but not always, ref to packet buf
 *
 * @return none.
 */
typedef void (*tl_zb_callback_t)(void *arg);

typedef struct tl_zb_task_s {
    tl_zb_callback_t tlCb;
    void *data;
} tl_zb_task_t;

typedef struct {
    u8 wptr;
    u8 rptr;
    u8 rsv[2];
    tl_zb_task_t evt[TL_ZBTASKQ_USERUSE_SIZE];
} tl_zbtaskq_user_t;

typedef struct {
    tl_zb_task_t evt[TL_ZBTASKQ_STACKUSE_SIZE];
    u8 wptr;
    u8 rptr;
    u8 used;
    u8 resv;
} tl_zbtaskq_stack_t;

#define	TL_QUEUE_HAS_SPACE(wptr, rptr, size)    ((wptr - rptr) < (size))

extern u8 ZB_TASKQ_USERUSE_SIZE;
extern tl_zbtaskq_user_t taskQ_user;

/**
 * @brief  Initialize scheduler subsystem.
 */
void zb_sched_init(void);

/**
 * @brief  get the valid task from task queue list
 *
 * @param  idx      - the index of the task list
 * @param  taskInfo - task
 *
 * @return the pointer to the task list
 */
tl_zb_task_t *tl_zbTaskQPop(u8 idx, tl_zb_task_t *taskInfo);

/**
 * @brief  push task to task list
 *
 * @param  idx  - the index of the task list
 * @param  task - the task will be push to task list
 *
 * @return the status
 */
u8 tl_zbTaskQPush(u8 idx, tl_zb_task_t *task);

/**
 * @brief  push a task to task list
 *
 * @param  func - the callback of the event
 * @param  arg  - the parameter to the callback
 *
 * @return the status
 */
u8 tl_zbTaskPost(tl_zb_callback_t func, void *arg);
#define	TL_SCHEDULE_TASK(a, b)        tl_zbTaskPost(a, b)

void tl_zbTaskProc(void);

u8 zb_isTaskDone(void);
u8 tl_zbUserTaskQNum(void);

#endif /* ZB_TASK_QUEUE_H */
