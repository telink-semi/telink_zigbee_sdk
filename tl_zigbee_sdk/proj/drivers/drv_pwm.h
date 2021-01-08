/********************************************************************************************************
 * @file	drv_pwm.h
 *
 * @brief	This is the header file for drv_pwm
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

#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	#define PWM_CLOCK_SOURCE			CLOCK_SYS_CLOCK_HZ
#elif defined(MCU_CORE_B91)
	#define PWM_CLOCK_SOURCE			(sys_clk.pclk * 1000 * 1000)
#endif


#if defined(MCU_CORE_826x)
	#define drv_pwm_start(pwmId)		pwm_Start(pwmId)
	#define drv_pwm_stop(pwmId)			pwm_Stop(pwmId)
	#define drv_pwm_invert(pwmId)		pwm_Invert(pwmId)
	#define drv_pwm_n_invert(pwmId)		pwm_INVInvert(pwmId)
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	#define drv_pwm_start(pwmId)		pwm_start(pwmId)
	#define drv_pwm_stop(pwmId)			pwm_stop(pwmId)
	#define drv_pwm_invert(pwmId)		pwm_revert(pwmId)
	#define drv_pwm_n_invert(pwmId)		pwm_n_revert(pwmId)
#elif defined(MCU_CORE_B91)
	#define drv_pwm_start(pwmId)		pwm_start(pwmId)
	#define drv_pwm_stop(pwmId)			pwm_stop(pwmId)
	#define drv_pwm_invert(pwmId)		pwm_invert_en(pwmId)
	#define drv_pwm_n_invert(pwmId)		pwm_n_invert_en(pwmId)
#endif

void drv_pwm_init(void);

void drv_pwm_cfg(u8 pwmId, u16 cmp_tick, u16 cycle_tick);



