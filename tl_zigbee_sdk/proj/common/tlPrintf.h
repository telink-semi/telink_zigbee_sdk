/********************************************************************************************************
 * @file	tlPrintf.h
 *
 * @brief	This is the header file for tlPrintf
 *
 * @author	Driver & Zigbee Group
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

#if defined(MCU_CORE_B91)
#include <stdio.h>
#endif

#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
int Tl_printf(const char *format, ...);
#endif


#if (UART_PRINTF_MODE || USB_PRINTF_MODE)
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	#define printf										Tl_printf
#endif

	#define TRACE										printf

	#define	DEBUG(compileFlag, ...)						do{	\
															if(compileFlag) TRACE(__VA_ARGS__);		\
														}while(0)

	#define DEBUG_ARRAY(compileFlag, arrayAddr, len)	do{	\
															if(compileFlag){											\
																TRACE("*********************************\n");			\
																unsigned char i = 0;									\
																do{														\
																	TRACE(" %x", ((unsigned char *)arrayAddr)[i++]);	\
																}while(i < len);										\
																TRACE("\n*********************************\n");			\
															}															\
														}while(0)
#else
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	#define printf
#endif

	#define TRACE
	#define	DEBUG(compileFlag, ...)
	#define DEBUG_ARRAY(compileFlag, arrayAddr, len)
#endif


