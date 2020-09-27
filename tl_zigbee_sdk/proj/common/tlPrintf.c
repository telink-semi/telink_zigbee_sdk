/********************************************************************************************************
 * @file     tlPrintf.c
 *
 * @brief    Printf function file
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

#include "../drivers/drv_putchar.h"
#if defined(MCU_CORE_B91)
	#include <stdarg.h>
#else
	typedef char *va_list;

	#define _INTSIZEOF(n)   	( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )

	#define va_start(ap,v)  	( ap = (va_list)&v + _INTSIZEOF(v) )
	#define va_arg(ap,t)    	( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
	#define va_end(ap)      	( ap = (va_list)0 )

	#define	DECIMAL_OUTPUT		10
	#define	OCTAL_OUTPUT		8
	#define	HEX_OUTPUT			16
#endif

static void put_s(char *s){
	while((*s != '\0')){
		drv_putchar(*s++);
	}
}

static void puti(unsigned int num, int base){
	char re[]="0123456789ABCDEF";

	char buf[50];

	char *addr = &buf[49];

	*addr = '\0';

	do{
		*--addr = re[num%base];
		num/=base;
	}while(num!=0);

	put_s(addr);
}

#if defined(MCU_CORE_B91)
__attribute__((used)) int _write(int fd, const unsigned char *buf, int size){
    int i;
    for(i = 0; i < size; i++){
    	drv_putchar(buf[i]);
    }
    return i;
}
#else
int Tl_printf(const char *format, ...){
	char span;
	unsigned long j;
	char *s;
	long m;

	va_list arg_ptr;
	va_start(arg_ptr, format);

	while((span = *(format++))){
		if(span != '%'){
			drv_putchar(span);
		}else{
			span = *(format++);
			if(span == 'c'){
				j = va_arg(arg_ptr,int);//get value of char
				drv_putchar(j);
			}else if(span == 'd'){
				m = va_arg(arg_ptr,int);//get value of char
				if(m<0){
					drv_putchar('-');
					m = -m;
				}
				puti(m,DECIMAL_OUTPUT);
			}else if(span == 's'){
				s = va_arg(arg_ptr,char *);//get string value
				put_s(s);
			}else if(span == 'o'){
				j = va_arg(arg_ptr,unsigned int);//get octal value
				puti(j,OCTAL_OUTPUT);
			}else if(span == 'x'){
					j = va_arg(arg_ptr,unsigned int);//get hex value
					puti(j,HEX_OUTPUT);
			}else if(span == 0){
				break;
			}else{
				drv_putchar(span);
			}
		}

	}
	va_end(arg_ptr);

	return 0;
}
#endif

