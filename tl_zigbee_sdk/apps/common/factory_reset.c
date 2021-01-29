/********************************************************************************************************
 * @file	factory_reset.c
 *
 * @brief	This is the source file for factory_reset
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
#include "tl_common.h"
#include "factory_reset.h"
#include "zb_api.h"

#define FACTORY_RESET_POWER_CNT_THRESHOLD		10	//times
#define FACTORY_RESET_TIMEOUT					2	//second

ev_timer_event_t *factoryRst_timerEvt = NULL;
u8 factoryRst_powerCnt = 0;
bool factoryRst_exist = FALSE;

nv_sts_t factoryRst_powerCntSave(void){
	nv_sts_t st = NV_SUCC;
#if NV_ENABLE
	st = nv_flashWriteNew(1, NV_MODULE_APP, NV_ITEM_APP_POWER_CNT, 1, &factoryRst_powerCnt);
#else
	st = NV_ENABLE_PROTECT_ERROR;
#endif
	return st;
}

nv_sts_t factoryRst_powerCntRestore(void){
	nv_sts_t st = NV_SUCC;
#if NV_ENABLE
	st = nv_flashReadNew(1, NV_MODULE_APP, NV_ITEM_APP_POWER_CNT, 1, &factoryRst_powerCnt);
#else
	st = NV_ENABLE_PROTECT_ERROR;
#endif
	return st;
}

static s32 factoryRst_timerCb(void *arg){
	if(factoryRst_powerCnt >= FACTORY_RESET_POWER_CNT_THRESHOLD){
		/* here is just a mark, wait for device announce and then perform factory reset. */
		factoryRst_exist = TRUE;
	}

	factoryRst_powerCnt = 0;
	factoryRst_powerCntSave();

	factoryRst_timerEvt = NULL;
	return -1;
}

void factroyRst_handler(void){
	if(factoryRst_exist){
		factoryRst_exist = FALSE;
		zb_factoryReset();
	}
}

void factroyRst_init(void){
	factoryRst_powerCntRestore();
	factoryRst_powerCnt++;
	factoryRst_powerCntSave();

	if(factoryRst_timerEvt){
		TL_ZB_TIMER_CANCEL(&factoryRst_timerEvt);
	}
	factoryRst_timerEvt = TL_ZB_TIMER_SCHEDULE(factoryRst_timerCb, NULL, FACTORY_RESET_TIMEOUT * 1000);
}

