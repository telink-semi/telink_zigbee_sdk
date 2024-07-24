/********************************************************************************************************
 * @file    drv_putchar.h
 *
 * @brief   This is the header file for drv_putchar
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
#elif defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X)
	#ifndef	BAUDRATE
		#define BAUDRATE					1000000//1M
	#endif
		#define	BIT_INTERVAL	 			((24*1000*1000) / BAUDRATE)
#endif

#if UART_PRINTF_MODE
	#ifdef DEBUG_INFO_TX_PIN
		#if defined(MCU_CORE_TL721X)
			#define TX_PIN_OUTPUT_REG			reg_gpio_out_set_clear(DEBUG_INFO_TX_PIN)
		#else//826x/8258/8278/b91/b92
			#define TX_PIN_OUTPUT_REG			reg_gpio_out(DEBUG_INFO_TX_PIN)
		#endif

		#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
			#define DEBUG_TX_PIN_INIT()		do{	\
												gpio_set_func(DEBUG_INFO_TX_PIN, AS_GPIO);							\
												gpio_set_output_en(DEBUG_INFO_TX_PIN, 1);							\
												gpio_setup_up_down_resistor(DEBUG_INFO_TX_PIN, PM_PIN_PULLUP_1M); 	\
												gpio_write(DEBUG_INFO_TX_PIN, 1);									\
											}while(0)
		#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X)
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



