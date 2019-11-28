/********************************************************************************************************
 * @file     ev.c
 *
 * @brief    Interface of event file
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
#include "platform_includes.h"
#include "../common/utlist.h"
#include "ev.h"
#include "zb_task_queue.h"

#define EV_TIMER_SAFE_MARGIN(v)		(EV_TIMER_SAFE_MARGIN_US*v)

static ev_loop_ctrl_t ev_loop, *loop = &ev_loop;
sys_exception_cb_t g_sysExceptCallbak = NULL;

void ev_on_poll(ev_poll_e e, ev_poll_callback_t cb){
    loop->poll[e].valid = 1;
    loop->poll[e].cb = cb;
}

void ev_enable_poll(ev_poll_e e){
    loop->poll[e].valid = 1;
}

void ev_disable_poll(ev_poll_e e){
    loop->poll[e].valid = 0;
}

/* Process poll */
static void ev_poll(void) {
    int i;
    for(i = 0; i < EV_POLL_MAX; ++i){
        if(loop->poll[i].valid){
            loop->poll[i].cb();
        }
    }
}


static u32 inline ev_cal_timer_distant(u32 t, u32 now){
    if((u32)(now - t) < EV_TIMER_SAFE_MARGIN(CLOCK_SYS_CLOCK_1US))
        return 0;
    else
        return (u32)(t - now);
}

static int ev_cmp_timer(ev_time_event_t *e1, ev_time_event_t *e2, u32 now){
	//fixed, to avoid unsigned to signed convert overflow problem
	if(ev_cal_timer_distant(e1->t, now)<ev_cal_timer_distant(e2->t, now)){
		return -1;
	}else if(ev_cal_timer_distant(e1->t, now)> ev_cal_timer_distant(e2->t, now)){
		return 1;
	}else{
		return 0;
	}
}

void ev_reset_timer(ev_time_event_t *e){
    e->t = clock_time() + e->interval;    // be care of overflow
}

void ev_set_timer(ev_time_event_t *e, int t_us){
    e->interval = t_us * CLOCK_SYS_CLOCK_1US;
    ev_reset_timer(e);
}

static int inline ev_is_timer_expired(ev_time_event_t *e, u32 now){
    return ((u32)(now - e->t) < EV_TIMER_SAFE_MARGIN(CLOCK_SYS_CLOCK_1US));
}

int ev_timer_expired(ev_time_event_t *e){
    u32 now = clock_time();
    return ev_is_timer_expired(e, now);
}

static ev_time_event_t *ev_search_nearest_timer()
{
    ev_time_event_t *te = loop->timer_head;
    ev_time_event_t *nearest = 0;
    u32 now = clock_time();

    while(te) {
        if(!nearest || ev_cmp_timer(te, nearest, now) < 0){
            nearest = te;
        }
        te = te->next;
    }
    return nearest;
}

u8 ev_timer_exist(const ev_time_event_t * e){
    ev_time_event_t *te = loop->timer_head;
    while(te){
        if (te == e)
            return 1;
        te = te->next;
    }
    return 0;
}


void ev_start_timer(ev_time_event_t * e){
    u8 r = irq_disable();
    
    u32 now = clock_time();

    u32 t = now + e->interval;    // becare of overflow
    
    // add to timer list
    ev_time_event_t * out;
    LIST_EXIST(loop->timer_head, e, out);
    if(out){
        out->t = t;
    }else{
        e->t = t;
        LIST_ADD(loop->timer_head, e);
    }
    
	//**timer events congestion would happen, if a timer event re-started before it executed and happen that this event
	//is the nearest timer event, the next event timer would adjust to a new value however this value would mask other timer events
	//To avoid this happen, need to search nearest timer at the end of this function

	// check if e will be the nearest timer
	// "Use sorted list or skiplist to improve performance"
    if(!loop->timer_nearest || ev_cmp_timer(e, loop->timer_nearest, now) < 0){
        loop->timer_nearest = e;
    }

    irq_restore(r);
	loop->timer_nearest = ev_search_nearest_timer();//need to search nearest timer in case of timer event stuck
}

void ev_on_timer(ev_time_event_t * e, u32 t_us){
	if ( e == NULL ) {
		ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_TIMER_EVEVT);
	}
    e->interval = t_us * CLOCK_SYS_CLOCK_1US;
    ev_start_timer(e);
}

void ev_unon_timer(ev_time_event_t * e){
	if ( e == NULL ) {
		ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_TIMER_EVEVT);
	}
	u8 r = irq_disable();

	tl_zbTimerTaskChkAndDec(e);

    LIST_DELETE(loop->timer_head, e);
    loop->timer_nearest = ev_search_nearest_timer();

	irq_restore(r);
}


#define ZB_TIMER_EV_DBG_EN  0
#if ZB_TIMER_EV_DBG_EN
typedef struct{
	ev_timer_callback_t  cb;
	u8 *p;
}zb_timer_task_dbg_t;
zb_timer_task_dbg_t  g_zbTimerTaskDbg[256];
u32 g_zbTimerTaskDbgCnt = 0;
#endif

/* Process time events */
static void ev_process_timer(){

    u32 now = clock_time();
    if(!loop->timer_nearest || !ev_is_timer_expired(loop->timer_nearest, now))
        return;
    
    ev_time_event_t *te = loop->timer_head;
    ev_time_event_t *prev_head = te;
    while(te){			            
        if(ev_is_timer_expired(te, now)){            
            int t;
#ifndef WIN32
			//assert(te->cb > 0x100 && te->cb < 0x20000);
#endif
			t = te->cb(te->data);
#if ZB_TIMER_EV_DBG_EN
			g_zbTimerTaskDbg[g_zbTimerTaskDbgCnt&0xff].cb = te->cb;
			g_zbTimerTaskDbg[g_zbTimerTaskDbgCnt&0xff].p = te->data;
			g_zbTimerTaskDbgCnt++;
#endif
            if(t < 0){
                ev_unon_timer(te);        // delete timer
            }else if(0 == t){
                te->t = now + te->interval;    // becare of overflow
            }else{
                te->interval = t * CLOCK_SYS_CLOCK_1US;
                te->t = now + te->interval;    // becare of overflow
            }
            if(prev_head != loop->timer_head){
                // restart the whole from timer_head.  because the head could be changed within the loop
                te = loop->timer_head;
                prev_head = te;
            }else{
                te = te->next;
            }
        }else{
            te = te->next;
        }
    }
    // recalculate the nearest timer
    loop->timer_nearest = ev_search_nearest_timer();
    
}


int is_timer_expired(ev_time_event_t *e)
{
	u8 r = irq_disable();
    ev_time_event_t * out;
    LIST_EXIST(loop->timer_head, e, out);
	irq_restore(r);
    if (out)
    {
        return FALSE;
    }
    return TRUE;
}

u32 ev_nearestInterval(void){
	if(loop->timer_nearest){
		return ev_cal_timer_distant(loop->timer_nearest->t,clock_time());
	}
	else
		return 0xFFFFFFFF;
}

volatile u16 T_evtExcept[4] = {0};
u8 sys_exceptionPost(u16 line, u8 evt){
	T_evtExcept[0] = line;
	T_evtExcept[1] = evt;

	/* TODO: some information stored in NV */
	if(g_sysExceptCallbak){
		g_sysExceptCallbak();
	}

	return 0;
}

u8 sys_stackStatusCheck(void){
	extern u32 _end_bss_;
	u8 *stackEnd = (u8*)&_end_bss_;
	u8 stackOverflown = 0;
	for(s32 i = 0; i < 4; i++){
		if(stackEnd[i] != 0xff){
			/* stack overflown */
			stackOverflown = 1;
			break;
		}
	}

	if(stackOverflown){
		ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_STACK_OVERFLOWN);
	}

	return 0;
}

void sys_exceptHandlerRegister(sys_exception_cb_t cb){
	g_sysExceptCallbak = cb;
}

void ev_main(void){
    ev_process_timer();
    ev_poll();

#if(MODULE_WATCHDOG_ENABLE)
	wd_clear();
#endif

	//sys_stackStatusCheck();
}

volatile int T_timer_fix_cnt = 0;
void ev_synchronous_timer(void){
	u32 now = clock_time();
    T_timer_fix_cnt++;

	ev_time_event_t *te = loop->timer_head;
	while(te){
		if(te->t >= now)
		{
			te->t = te->t - now;
		}
		te = te->next;
	}
	reg_system_tick = 0;
}

