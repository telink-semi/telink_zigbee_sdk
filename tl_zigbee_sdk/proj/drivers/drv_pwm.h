/********************************************************************************************************
 * @file     drv_pwm.h
 *
 * @brief	 PWM driver configuration header file
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
#include "../common/compiler.h"

void drv_pwm_init(void);

void drv_pwm_cfg(u32 pwmId, unsigned short cmp_tick, unsigned short cycle_tick);

#ifdef MCU_CORE_8258
#define drv_pwm_start(pwmId)		pwm_start(pwmId)
#define drv_pwm_stop(pwmId)			pwm_stop(pwmId)
#define drv_pwm_invert(pwmId)		pwm_revert(pwmId)
#define drv_pwm_n_invert(pwmId)		pwm_n_revert(pwmId)
#else
#define drv_pwm_start(pwmId)		pwm_Start(pwmId)
#define drv_pwm_stop(pwmId)			pwm_Stop(pwmId)
#define drv_pwm_invert(pwmId)		pwm_Invert(pwmId)
#define drv_pwm_n_invert(pwmId)		pwm_INVInvert(pwmId)
#endif

