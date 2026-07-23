/********************************************************************************************************
 * @file    simulate_internal.h
 *
 * @brief   This is the header file for Bluetooth SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef TLKSTK_SIMULATE_SIMULATE_H_
#define TLKSTK_SIMULATE_SIMULATE_H_


#include "tl_common.h"
#include "common/compiler.h"
#include "common/bit.h"


/******************************* simulate general begin *********************************************************************/



/******************************* simulate general end *************************************************************************/








/******************************* simulate fixed task begin *********************************************************************/
_attribute_aligned_(4)
typedef struct {
    u8    fixed_task_sel; //fixed task selection index
    u8    u8_rsvd[3];

}simu_fixed_mng_t; //simulate fixed task management data type
extern simu_fixed_mng_t    g_fixedTskMng; //global variable for simulate fixed task management


#define SIMU_FIXED_TASK_FIFO_NUM                                    8  //consider 10mS interval at least for 80mS scheduler timing build

_attribute_aligned_(4)
typedef struct __attribute__((packed)) {
    u8    fixedTsk_en;
    u8    fixedTsk_idx;
    u8    u8_rsvd[2];

    u16   fixedTsk_allocate_us;
    u16   u16_rsvd;

    u32   fixedTsk_interval_tick;
    u32   fixedTsk_tick_mark; //simulate fixed task mark STIMER tick
    u32   fixedTsk_duration_us; //simulate fixed task duration

    sch_task_t    fixedTsk_sch_fifo[SIMU_FIXED_TASK_FIFO_NUM];
}simu_fixed_st_t;




void tlkstk_simi_setIntervalPriorityStep (u8 task_offset, u32 interval_us);

int tlkstk_simu_fixedTsk_interrupt_task(int);

u32 tlkstk_simu_fixedTsk_start(void);

void tlkstk_simu_fixedTsk_post(void);

int tlkstk_simu_fixedTsk_build(void);

/******************************* simulate fixed task begin *************************************************************************/









/******************************* simulate flexible task begin *********************************************************************/
_attribute_aligned_(4)
typedef struct {
    u8    u8_rsvd[4];

}simu_flex_mng_t; //simulate flexible task management data type
extern simu_flex_mng_t    g_simuFlexMng; //global variable for simulate flexible task management


#define SIMU_FLEXIBLE_TASK_FIFO_NUM                                    4  //consider at least 20mS interval for 80mS scheduler timing build

_attribute_aligned_(4)
typedef struct __attribute__((packed)) {
    u8    flexTsk_en;
    u8    u8_rsvd[3];

    u16   flexTsk_allocate_us;
    u16   u16_rsvd;

    u32   flexTsk_interval_tick;
    u32   flexTsk_tick_begin_mark; //simulate flexible task mark STIMER tick
    u32   flexTsk_tick_end_mark;

    sch_task_t    flexTsk_sch_fifo[SIMU_FLEXIBLE_TASK_FIFO_NUM];
}simu_flex_st_t;



int tlkstk_simu_flexTsk_interrupt_task(int);

void tlkstk_simu_flexTsk_running(void);

int tlkstk_simu_flexTsk_build(void);

void tlkstk_simu_flexTsk_post(void);
/******************************* simulate flexible task begin *************************************************************************/










/******************************* simulate insert task begin *********************************************************************/
_attribute_aligned_(4)
typedef struct {
    u8    insertTsk_en;
    u8    u8_rsvd[3];

}simu_insert_mng_t; //simulate insert task management data type
extern simu_insert_mng_t    g_insertTskMng; //global variable for simulate fixed task management



int tlkstk_simu_insertTask_interrupt_task(int);

void tlkstk_simu_insertTask_start(void);

void tlkstk_simu_insertTask_post(void);

/******************************* simulate insert task begin *************************************************************************/





#endif /* TLKSTK_SIMULATE_SIMULATE_H_ */
