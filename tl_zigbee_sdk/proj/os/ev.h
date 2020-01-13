/********************************************************************************************************
 * @file     ev.h
 *
 * @brief    Interface of event header file
 *
 * @author
 * @date     Oct. 8, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#pragma once

#include "../common/types.h"
#include "ev_poll.h"
#include "ev_timer.h"


enum{EV_FIRED_EVENT_MAX_MASK = EV_FIRED_EVENT_MAX - 1};

typedef void (*sys_exception_cb_t)(void);

typedef struct ev_loop_ctrl_t{
    ev_poll_t                poll[EV_POLL_MAX];
	/*
	Time events is sorted, use single linked list
	*/
    ev_time_event_t        	*timer_head;
    ev_time_event_t     	*timer_nearest;        // find the nearest fired timer,
} ev_loop_ctrl_t;

enum {
	EV_TIMER_SAFE_MARGIN_US = 4000000	// in us,
};

enum{
	SYS_EXCEPTTION_COMMON_MEM_ACCESS = 0,
	SYS_EXCEPTTION_COMMON_TIMER_EVEVT,
	SYS_EXCEPTTION_COMMON_BUFFER_OVERFLOWN,
	SYS_EXCEPTTION_COMMON_STACK_OVERFLOWN,
	SYS_EXCEPTTION_COMMON_PARAM_ERROR,
	SYS_EXCEPTTION_COMMON_TASK_POST,

	SYS_EXCEPTTION_ZB_BUFFER_OVERFLOWN = 0x10,
	SYS_EXCEPTTION_ZB_BUFFER_EXCEPTION_FREE_OVERFLON,
	SYS_EXCEPTTION_ZB_BUFFER_EXCEPTION_FREE_MULIT,
	SYS_EXCEPTTION_ZB_BUFFER_EXCEPTION,
	SYS_EXCEPTTION_ZB_TIMER_TASK,
	SYS_EXCEPTTION_ZB_TASK_POST,

	SYS_EXCEPTTION_ZB_MAC_TX_IRQ = 0x20,
	SYS_EXCEPTTION_ZB_MAC_TX_TIMER,
	SYS_EXCEPTTION_ZB_MAC_CSMA,
	SYS_EXCEPTTION_ZB_MAC_TRX_TASK,

	SYS_EXCEPTTION_ZB_NWK_ADDR_IDX = 0x30,
	SYS_EXCEPTTION_ZB_NWK_BRC_INFO,
	SYS_EXCEPTTION_ZB_NWK_GET_ENTRY,
	SYS_EXCEPTTION_ZB_NWK_NEIGHBOR_TABLE,
	SYS_EXCEPTTION_ZB_NWK_ROUTE_TABLE,
	SYS_EXCEPTTION_ZB_NWK_LINK_STA_MEM_ALLOC_FAIL,

	SYS_EXCEPTTION_ZB_SS_KEY_INDEX = 0x40,

	SYS_EXCEPTTION_ZB_APS_PARAM = 0x50,
	SYS_EXCEPTTION_ZB_APS_FRAGMENT_TRANS,
	SYS_EXCEPTTION_ZB_APS_FRAGMENT_RCV,

	SYS_EXCEPTTION_ZB_ZCL_ENTRY = 0x60,

	SYS_EXCEPTTION_EV_BUFFER_OVERFLOWN = 0x70,
	SYS_EXCEPTTION_EV_BUFFER_EXCEPTION_FREE_OVERFLON,
	SYS_EXCEPTTION_EV_BUFFER_EXCEPTION_FREE_MULIT,
	SYS_EXCEPTTION_EV_TASK_POST,
};

void sys_exceptHandlerRegister(sys_exception_cb_t cb);
u8 sys_exceptionPost(u16 line, u8 evt);
#define ZB_EXCEPTION_POST(evt)  sys_exceptionPost(__LINE__, evt)

//will be called in every main loop
void ev_main(void);



