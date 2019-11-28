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


int Tl_printf(const char *format, ...);




#if (UART_PRINTF_MODE || USB_PRINTF_MODE)
	#define	arrayPrint(arrayAddr,len)					\
	{													\
		Tl_printf("\n*********************************\n");		\
		unsigned char	i = 0;							\
		do{												\
			Tl_printf(" %x",((unsigned char *)arrayAddr)[i++]);						\
		}while(i<len);										\
		Tl_printf("\n*********************************\n");		\
	}

	#define	DEBUG(compileFlag,...)						\
			do{											\
				if(compileFlag) Tl_printf(__VA_ARGS__);					\
			}while(0)

	#define printf			Tl_printf
	#define	printfArray		arrayPrint
#else
	#define printf
	#define	printfArray
	#define	DEBUG(compileFlag,...)
#endif

#if (FLASH_PRINTF_MODE)

#define quick_printf ftl_write
#else
#define quick_printf
#endif


