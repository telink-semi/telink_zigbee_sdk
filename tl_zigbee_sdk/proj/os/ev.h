/********************************************************************************************************
 * @file	ev.h
 *
 * @brief	This is the header file for ev
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

#include "../tl_common.h"

enum {
	EV_FIRED_EVENT_MAX_MASK = EV_FIRED_EVENT_MAX - 1
};

enum {
	EV_TIMER_SAFE_MARGIN_US = 4000000	// in us,
};

enum {
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

typedef void (*sys_exception_cb_t)(void);

void sys_exceptHandlerRegister(sys_exception_cb_t cb);

u8 sys_exceptionPost(u16 line, u8 evt);
#define ZB_EXCEPTION_POST(evt)  sys_exceptionPost(__LINE__, evt)

//will be called in every main loop
void ev_main(void);



