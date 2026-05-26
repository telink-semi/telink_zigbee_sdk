/********************************************************************************************************
 * @file     ext_pm.c
 *
 * @brief   This is the source file for B91
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#include "../analog.h"
#include "../stimer.h"
#include "ext_pm.h"


_attribute_data_retention_  _attribute_aligned_(4) misc_para_t 		blt_miscParam;

_attribute_data_retention_sec_	suspend_handler_t	func_before_suspend = 0;
_attribute_data_retention_sec_	check_32k_clk_handler_t   	pm_check_32k_clk_stable = 0;
_attribute_data_retention_sec_	pm_get_32k_clk_handler_t  	pm_get_32k_tick = 0;

_attribute_data_retention_ static unsigned int tick_check32kPad = 0;


_attribute_no_inline_ void check_32k_clk_stable(void)
{
	if(clock_time_exceed(tick_check32kPad, 10000)){ //every 10ms, check if 32k pad clk is stable
		tick_check32kPad = stimer_get_tick();

		unsigned int last_32k_tick;
		unsigned int curr_32k_tick;

		//Check if 32k pad vibration and basically works stably
		last_32k_tick = clock_get_32k_tick();
		delay_us(50); //for 32k tick accumulator, tick period: 30.5us, if stable: delta tick > 0
		curr_32k_tick = clock_get_32k_tick();

		if(last_32k_tick != curr_32k_tick){
			blt_miscParam.pm_enter_en = 1;//allow enter pm
			return;
		}
	}
	else{
		return;
	}

	// T > 2s , 32k pad clk still unstable: reboot MCU
	if(!blt_miscParam.pm_enter_en && clock_time_exceed(0, 2000000)){
//		analog_write_reg8(SYS_DEEP_ANA_REG, analog_read_reg8(SYS_DEEP_ANA_REG) & (~SYS_NEED_REINIT_EXT32K)); //clr
		sys_reboot(); //reboot the MCU
	}
}
