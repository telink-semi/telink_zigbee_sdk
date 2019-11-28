/********************************************************************************************************
 * @file     factory_reset.c
 *
 * @brief    Factory reset
 *
 * @author
 * @date     Dec. 1, 2017
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
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
#include "tl_common.h"
#include "factory_reset.h"
#include "zb_api.h"

u32 flash_adr_reset_cnt = CFG_FACTORY_RST_CNT;
static int adr_reset_cnt_idx = 0;




#if 1	//org mode

#define RESET_CNT_RECOUNT_FLAG          0
#define RESET_FLAG                      0x80

#define SERIALS_CNT                     (5)   // must less than 7

u8 factory_reset_serials[SERIALS_CNT * 2]   = { 0, 3,    // [0]:must 0
                                                0, 3,    // [2]:must 0
                                                0, 3,    // [4]:must 0
                                                3, 30,
                                                3, 30};

static int reset_cnt = 0;



void reset_cnt_clean(void)
{
	if(adr_reset_cnt_idx < 3840){
		return;
	}

	flash_erase(flash_adr_reset_cnt);
	adr_reset_cnt_idx = 0;
}

void write_reset_cnt(u8 cnt)
{
	flash_write(flash_adr_reset_cnt + adr_reset_cnt_idx, 1, (u8 *)(&cnt));
}

void clear_reset_cnt(void)
{
    write_reset_cnt(RESET_CNT_RECOUNT_FLAG);
}

void reset_cnt_get_idx(void)		//return 0 if unconfigured
{
	u8 *pf = (u8 *)flash_adr_reset_cnt;
	for(adr_reset_cnt_idx = 0; adr_reset_cnt_idx < 4096; adr_reset_cnt_idx++){
	    u8 restcnt_bit = pf[adr_reset_cnt_idx];
		if(restcnt_bit != RESET_CNT_RECOUNT_FLAG){ //end
        	if(((u8)(~(BIT(0)|BIT(1)|BIT(2)|BIT(3))) == restcnt_bit)  // the fourth not valid
        	 ||((u8)(~(BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5))) == restcnt_bit)){  // the fifth not valid
                clear_reset_cnt();
            }else{
			    break;
			}
		}
	}

    reset_cnt_clean();
}

u8 get_reset_cnt_bit(void)
{
	if(adr_reset_cnt_idx < 0){
	    reset_cnt_clean();
		return 0;
	}

	u8 reset_cnt;
	flash_read(flash_adr_reset_cnt + adr_reset_cnt_idx, 1, &reset_cnt);
	return reset_cnt;
}

void increase_reset_cnt(void)
{
	u8 restcnt_bit = get_reset_cnt_bit();

	foreach(i, 8){
        if(restcnt_bit & BIT(i)){
            if(i < 3){
                reset_cnt = i;
            }else if(i < 5){
                reset_cnt = 3;
            }else if(i < 7){
                reset_cnt = 4;
            }

            restcnt_bit &= ~(BIT(i));
            write_reset_cnt(restcnt_bit);
            break;
        }
	}
}

ev_time_event_t factoryRstTimer;

s32 zb_factoryResetCb(void *arg)
{
	zb_factoryReset();

	return -1;
}

void factory_reset(void)
{
    clear_reset_cnt();

    factoryRstTimer.cb = zb_factoryResetCb;
    factoryRstTimer.data = NULL;
	ev_on_timer(&factoryRstTimer, 2 * 1000 * 1000);
}

u8 factory_reset_handle(void)
{
    reset_cnt_get_idx();

    u8 restcnt_bit = get_reset_cnt_bit();

	if(restcnt_bit == RESET_FLAG){
		return 1;
	}else{
        increase_reset_cnt();
	}

	return 0;
}

u8 factory_reset_cnt_check(void)
{
    static u8 clear_st = 3;
    static u32 reset_check_time;

	if(0 == clear_st) return 0;

	if(3 == clear_st){
        clear_st--;
        reset_check_time = factory_reset_serials[reset_cnt*2];
    }

	if((2 == clear_st) && clock_time_exceed(0, reset_check_time*1000*1000)){
	    clear_st--;
	    reset_check_time = factory_reset_serials[reset_cnt*2 + 1];
	    if(3 == reset_cnt || 4 == reset_cnt){
            increase_reset_cnt();
        }
	}

	if((1 == clear_st) && clock_time_exceed(0, reset_check_time*1000*1000)){
	    clear_st = 0;
        clear_reset_cnt();
	}

	return 0;
}

#else
/****************************************
new factory reset:
user can change any one of factory_reset_serials, and also can change SERIALS_CNT
*****************************************/
#define SERIALS_CNT                     (10*2)   // should less than 100

u8 factory_reset_serials[SERIALS_CNT]   =     { 0, 3,
                                                0, 3,
                                                0, 3,
                                                0, 3,
                                                0, 3,
                                                0, 3,
                                                0, 3,
                                                0, 3,
                                                0, 3,
                                                0, 3,};

#define RESET_CNT_INVALID               0
#define RESET_TRIGGER_VAL               SERIALS_CNT

void reset_cnt_clean(void)
{
	if(adr_reset_cnt_idx < 3840){
		return;
	}

	flash_erase(flash_adr_reset_cnt);
	adr_reset_cnt_idx = 0;
}

void write_reset_cnt(u8 cnt)// reset cnt value from 1 to 254, 0 is invalid cnt
{
	reset_cnt_clean();
	flash_write(flash_adr_reset_cnt + adr_reset_cnt_idx, 1, (u8 *)(&cnt));
}

void clear_reset_cnt(void)
{
    write_reset_cnt(RESET_CNT_INVALID);
}

void reset_cnt_get_idx(void)		//return 0 if unconfigured
{
	u8 *pf = (u8 *)flash_adr_reset_cnt;
	for(adr_reset_cnt_idx = 0; adr_reset_cnt_idx < 4096; adr_reset_cnt_idx++){
	    u8 restcnt_bit = pf[adr_reset_cnt_idx];
		if(restcnt_bit != RESET_CNT_INVALID){ //end
			if(0xFF == restcnt_bit){
				//do nothing
			}else if((restcnt_bit > RESET_TRIGGER_VAL) || (restcnt_bit & BIT(0))){// invalid state
				clear_reset_cnt();
			}
			break;
		}
	}
}

u8 get_reset_cnt_bit(void)// reset cnt value from 1 to 254, 0 is invalid cnt
{
	u8 reset_cnt;
	flash_read(flash_adr_reset_cnt + adr_reset_cnt_idx, 1, &reset_cnt);
	return reset_cnt;
}

void increase_reset_cnt(void)
{
	u8 restcnt_bit = get_reset_cnt_bit();

	if(0xFF == restcnt_bit){
		restcnt_bit = 0;
	}else if(restcnt_bit){
		clear_reset_cnt();// clear current BYTE and then use next BYTE
		adr_reset_cnt_idx++;
	}

	restcnt_bit++;
	write_reset_cnt(restcnt_bit);
}

ev_time_event_t factoryRstTimer;

s32 zb_factoryResetCb(void *arg)
{
	zb_factoryReset();

	return -1;
}

void factory_reset(void)
{
    clear_reset_cnt();

    factoryRstTimer.cb = zb_factoryResetCb;
    factoryRstTimer.data = NULL;
	ev_on_timer(&factoryRstTimer, 2 * 1000 * 1000);
}

u8 factory_reset_handle(void)
{
    reset_cnt_get_idx();

    u8 restcnt_bit = get_reset_cnt_bit();

	if(restcnt_bit == RESET_TRIGGER_VAL){
		return 1;
	}else{
        increase_reset_cnt();
	}

	return 0;
}

u8 factory_reset_cnt_check(void)
{
    static u8 clear_st = 3;
    static u32 reset_check_time;

	if(0 == clear_st) return 0;

	if(3 == clear_st){
        clear_st--;
        reset_check_time = factory_reset_serials[get_reset_cnt_bit() - 1];
    }

	if((2 == clear_st) && clock_time_exceed(0, reset_check_time*1000*1000)){
	    clear_st--;
	    increase_reset_cnt();
	    reset_check_time = factory_reset_serials[get_reset_cnt_bit() - 1];
	}

	if((1 == clear_st) && clock_time_exceed(0, reset_check_time*1000*1000)){
	    clear_st = 0;
        clear_reset_cnt();
	}

	return 0;
}

#endif
