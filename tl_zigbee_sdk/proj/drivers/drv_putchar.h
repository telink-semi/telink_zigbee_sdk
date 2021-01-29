/********************************************************************************************************
 * @file	drv_putchar.h
 *
 * @brief	This is the header file for drv_putchar
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


#if defined(MCU_CORE_826x)
	#ifndef	BAUDRATE
		#define BAUDRATE					2000000//2M
	#endif
		#define	BIT_INTERVAL	 			(CLOCK_SYS_CLOCK_HZ / BAUDRATE)
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278) || defined(MCU_CORE_B91)
	#ifndef	BAUDRATE
		#define BAUDRATE					1000000//1M
	#endif
		#define	BIT_INTERVAL	 			((16*1000*1000) / BAUDRATE)
#endif

#if UART_PRINTF_MODE
	#ifdef DEBUG_INFO_TX_PIN
		#define TX_PIN_OUTPUT_REG			reg_gpio_out(DEBUG_INFO_TX_PIN)

		#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
			#define DEBUG_TX_PIN_INIT()		do{	\
												gpio_set_func(DEBUG_INFO_TX_PIN, AS_GPIO);							\
												gpio_set_output_en(DEBUG_INFO_TX_PIN, 1);							\
												gpio_setup_up_down_resistor(DEBUG_INFO_TX_PIN, PM_PIN_PULLUP_1M); 	\
												gpio_write(DEBUG_INFO_TX_PIN, 1);									\
											}while(0)
		#elif defined(MCU_CORE_B91)
			#define DEBUG_TX_PIN_INIT()		do{	\
												gpio_function_en(DEBUG_INFO_TX_PIN);								\
												gpio_set_output(DEBUG_INFO_TX_PIN, 1);								\
												gpio_set_up_down_res(DEBUG_INFO_TX_PIN, GPIO_PIN_PULLUP_1M);		\
												gpio_set_high_level(DEBUG_INFO_TX_PIN);								\
											}while(0)
		#endif
	#else
		#error	"DEBUG_INFO_TX_PIN is undefined!"
	#endif
#endif

void drv_putchar(unsigned char byte);



