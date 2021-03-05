/********************************************************************************************************
 * @file	ev_timer.c
 *
 * @brief	This is the source file for ev_timer
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
#include "../tl_common.h"
#include "ev_timer.h"

typedef struct{
	ev_timer_event_t *timer_head;		//timer events is sorted, use single linked list
	ev_timer_event_t *timer_nearest;	//the nearest fired timer

	ev_timer_event_pool_t timerEventPool;
}ev_timer_ctrl_t;

u8 g_ev_timer_maxNum = TIMER_EVENT_NUM;
ev_timer_ctrl_t ev_timer;

static u32 prevSysTick = 0;
static u32 remSysTick = 0;

void ev_timer_init(void)
{
	memset((u8 *)&ev_timer, 0, sizeof(ev_timer));
}

void ev_timer_setPrevSysTick(u32 tick)
{
	prevSysTick = tick;
}

ev_timer_event_t *ev_timer_nearestGet(void)
{
	return ev_timer.timer_nearest;
}

ev_timer_event_t *ev_timer_freeGet(void)
{
	if(ev_timer.timerEventPool.used_num >= g_ev_timer_maxNum){
		return NULL;
	}

	for(u8 i = 0; i < g_ev_timer_maxNum; i++){
		if(ev_timer.timerEventPool.evt[i].used == 0){
			ev_timer.timerEventPool.evt[i].used = 1;
			ev_timer.timerEventPool.used_num++;
			return &ev_timer.timerEventPool.evt[i];
		}
	}

	return NULL;
}

void ev_timer_poolDelUpdate(ev_timer_event_t *evt)
{
	if(((u32)evt >= (u32)&ev_timer.timerEventPool.evt[0]) &&
	   ((u32)evt <= (u32)&ev_timer.timerEventPool.evt[g_ev_timer_maxNum - 1]) &&
	   (evt->used)){
		evt->used = 0;
		ev_timer.timerEventPool.used_num--;
	}
}

void ev_timer_nearestUpdate(void)
{
	ev_timer_event_t *timerEvt = ev_timer.timer_head;

	ev_timer.timer_nearest = ev_timer.timer_head;

	while(timerEvt){
		if(timerEvt->timeout < ev_timer.timer_nearest->timeout){
			ev_timer.timer_nearest = timerEvt;
		}
		timerEvt = timerEvt->next;
	}
}

bool ev_timer_enough(void)
{
	if(ev_timer.timerEventPool.used_num < TIMER_EVENT_ENOUGH_NUM){
		return TRUE;
	}
	return FALSE;
}

bool ev_timer_exist(ev_timer_event_t *evt)
{
	ev_timer_event_t *timerEvt = ev_timer.timer_head;

    while(timerEvt){
        if(timerEvt == evt){
            return TRUE;
        }
        timerEvt = timerEvt->next;
    }
    return FALSE;
}

void ev_on_timer(ev_timer_event_t *evt, u32 timeout)
{
	if(!evt){
		ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_TIMER_EVEVT);
		return;
	}

	evt->period = timeout;

	u32 r = drv_disable_irq();

	ev_timer_event_t *out = NULL;

	LIST_EXIST(ev_timer.timer_head, evt, out);
	if(out){
		out->timeout = evt->period;
	}else{
		evt->timeout = evt->period;
		LIST_ADD(ev_timer.timer_head, evt);
	}

	ev_timer_nearestUpdate();

	drv_restore_irq(r);
}

void ev_unon_timer(ev_timer_event_t *evt)
{
	if(!evt){
		ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_TIMER_EVEVT);
		return;
	}

	ev_timer_event_t *out = NULL;
	LIST_EXIST(ev_timer.timer_head, evt, out);
	if(!out){
		return;
	}

	u32 r = drv_disable_irq();

	ev_timer_poolDelUpdate(evt);

	LIST_DELETE(ev_timer.timer_head, evt);

	ev_timer_nearestUpdate();

	drv_restore_irq(r);
}

ev_timer_event_t *ev_timer_add(ev_timer_callback_t func, void *arg, u32 timeout)
{
	ev_timer_event_t *timerEvt = ev_timer_freeGet();
	if(!timerEvt){
		ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_TIMER_EVEVT);
		return NULL;
	}

	timerEvt->cb = func;
	timerEvt->data = arg;

	ev_on_timer(timerEvt, timeout);

	return timerEvt;
}

ev_timer_event_t *ev_timer_taskPost(ev_timer_callback_t func, void *arg, u32 t_ms)
{
	ev_timer_event_t *timerEvt = NULL;

	u32 r = drv_disable_irq();

	timerEvt = ev_timer_add(func, arg, t_ms);

	drv_restore_irq(r);

	return timerEvt;
}

u8 ev_timer_taskCancel(ev_timer_event_t **evt)
{
	u8 ret = NO_TIMER_AVAIL;
	ev_timer_event_t *timerEvt = *evt;

	if(!timerEvt || !timerEvt->used){
		return ret;
	}

	ev_unon_timer(timerEvt);

	*evt = NULL;

	return SUCCESS;
}

void ev_timer_update(u32 updateTime)
{
	if(updateTime == 0){
		return;
	}

	u32 r = drv_disable_irq();

	ev_timer_event_t *timerEvt = ev_timer.timer_head;

	while(timerEvt){
		if(timerEvt->timeout > updateTime){
			timerEvt->timeout -= updateTime;
		}else{
			timerEvt->timeout = 0;
		}

		timerEvt = timerEvt->next;
	}

	drv_restore_irq(r);
}

void ev_timer_executeCB(void)
{
	ev_timer_event_t *timerEvt = ev_timer.timer_head;
	ev_timer_event_t *prev_head = timerEvt;

	while(timerEvt){
		if(timerEvt->timeout == 0){
			s32 t = timerEvt->cb(timerEvt->data);

			if(t < 0){
				ev_unon_timer(timerEvt);
			}else if(t == 0){
				timerEvt->timeout = timerEvt->period;
			}else{
				timerEvt->period = (u32)t;
				timerEvt->timeout = timerEvt->period;
			}

			if(prev_head != ev_timer.timer_head){
				timerEvt = ev_timer.timer_head;
				prev_head = timerEvt;
			}else{
				timerEvt = timerEvt->next;
			}
		}else{
			timerEvt = timerEvt->next;
		}
	}

	ev_timer_nearestUpdate();
}

void ev_timer_process(void)
{
	u32 updateTime = 0;
	u32 sysTicks = 0;
	u32 currSysTick = clock_time();

	if(currSysTick != prevSysTick){
		sysTicks = (u32)(currSysTick - prevSysTick);

		/* store current ticks. */
		prevSysTick = currSysTick;

		updateTime = sysTicks / (S_TIMER_CLOCK_1US * 1000);
		remSysTick += sysTicks % (S_TIMER_CLOCK_1US * 1000);

		updateTime += remSysTick / (S_TIMER_CLOCK_1US * 1000);
		remSysTick = remSysTick % (S_TIMER_CLOCK_1US * 1000);

		/* more than 1 ms. */
		if(updateTime){
			ev_timer_update(updateTime);
			ev_timer_executeCB();
		}
	}
}
