/********************************************************************************************************
 * @file    tlPrintf.c
 *
 * @brief   This is the source file for tlPrintf
 *
 * @author  Driver & Zigbee Group
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
#include "../drivers/drv_putchar.h"
#if defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
    #include <stdarg.h>
#else
    typedef char *va_list;

    #define _INTSIZEOF(n)       ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))

    #define va_start(ap, v)     (ap = (va_list)&v + _INTSIZEOF(v))
    #define va_arg(ap, t)       (*(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
    #define va_end(ap)          (ap = (va_list)0)

    #define DECIMAL_OUTPUT      10
    #define OCTAL_OUTPUT        8
    #define HEX_OUTPUT          16
#endif



#if defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
__attribute__((used)) int _write(int fd, const unsigned char *buf, int size)
{
    (void)fd;
    int i;
    for (i = 0; i < size; i++) {
    	drv_putchar(buf[i]);
    }
    return i;
}
#else
static void put_s(char *s)
{
    while ((*s != '\0')) {
        drv_putchar(*s++);
    }
}

static void puti(unsigned int num, int base, int w)
{
    char re[] = "0123456789ABCDEF";
    char buf[50];
    int cnt = 0;

    char *addr = &buf[49];
    *addr = '\0';

    do {
        *--addr = re[num % base];
        num /= base;
        cnt++;
    } while (num != 0 && cnt < 49);

    for ( ; w > cnt; --w) {
        *--addr = '0';
    }

    put_s(addr);
}

int tl_printf(const char *format, ...)
{
    char span;
    unsigned long j;
    char *s;
    long m;
    int w;

    va_list arg_ptr;
    va_start(arg_ptr, format);

    while ((span = *(format++))) {
        if (span != '%') {
            drv_putchar(span);
        } else {
            span = *(format++);

            w = 0;
            for ( ; span >= '0' && span <= '9'; span = *(format++)) {
                w *= 10;
                w += span - '0';
            }

            if (span == 'c') {
                j = va_arg(arg_ptr, int);//get value of char
                drv_putchar(j);
            } else if (span == 'd') {
                m = va_arg(arg_ptr, int);//get value of char
                if (m < 0) {
                    drv_putchar('-');
                    m = -m;
                }
                puti(m, DECIMAL_OUTPUT, w);
            } else if (span == 's') {
                s = va_arg(arg_ptr, char *);//get string value
                put_s(s);
            } else if (span == 'o') {
                j = va_arg(arg_ptr, unsigned int);//get octal value
                puti(j, OCTAL_OUTPUT, w);
            } else if (span == 'x') {
                j = va_arg(arg_ptr, unsigned int);//get hex value
                puti(j, HEX_OUTPUT, w);
            } else if (span == 0) {
                break;
            } else {
                drv_putchar(span);
            }
        }
    }

    va_end(arg_ptr);

    return 0;
}
#endif
