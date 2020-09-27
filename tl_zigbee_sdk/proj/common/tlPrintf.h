/********************************************************************************************************
 * @file     tlPrintf.h
 *
 * @brief    Header file, extend printf interface which called by external usage
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


