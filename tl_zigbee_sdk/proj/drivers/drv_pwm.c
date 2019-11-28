/********************************************************************************************************
 * @file     drv_pwm.c
 *
 * @brief	 PWM driver interface file
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
#include "drv_pwm.h"
#include "platform_includes.h"

void drv_pwm_init(void){
#if defined(MCU_CORE_826x)
	pwm_Init(0);
#elif defined(MCU_CORE_HAWK)
#else

#endif
}

void drv_pwm_cfg(u32 pwmId, unsigned short cmp_tick, unsigned short cycle_tick){
#if defined(MCU_CORE_826x)
	pwm_Open(pwmId, NORMAL, 0, cmp_tick, cycle_tick, 0x2fff);
#elif defined(MCU_CORE_HAWK)
#else
	pwm_set_mode(pwmId, PWM_NORMAL_MODE);
	pwm_set_phase(pwmId, 0);   //no phase at pwm beginning
	pwm_set_cycle_and_duty(pwmId, cycle_tick, cmp_tick);
#endif
}
