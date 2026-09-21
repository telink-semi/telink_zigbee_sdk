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
#include "types.h"
#include "tlPrintf.h"


static tl_putCharFn_t tl_putChar = NULL;

void tl_printf_register(tl_putCharFn_t fn)
{
    tl_putChar = fn;
}

void tl_printf_putChar(const unsigned char byte)
{
    if (tl_putChar) {
        tl_putChar(byte);
    }
}

#if defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || \
    defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL323X) || defined(MCU_CORE_TL521X)
__attribute__((used)) int _write(int fd, const unsigned char *buf, int size)
{
    (void)fd;
    int i = 0;

    if (tl_putChar == NULL) {
        return 0;
    }

    for (i = 0; i < size; i++) {
        tl_putChar(buf[i]);
    }

    return i;
}

#elif defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
static void put_s(char *s)
{
    while ((*s != '\0')) {
        tl_putChar(*s++);
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

    if (tl_putChar == NULL) {
        return 0;
    }

    va_list arg_ptr;
    va_start(arg_ptr, format);

    while ((span = *(format++))) {
        if (span != '%') {
            tl_putChar(span);
        } else {
            span = *(format++);

            w = 0;
            for ( ; span >= '0' && span <= '9'; span = *(format++)) {
                w *= 10;
                w += span - '0';
            }

            if (span == 'c') {
                j = va_arg(arg_ptr, int);//get value of char
                tl_putChar(j);
            } else if (span == 'd') {
                m = va_arg(arg_ptr, int);//get value of char
                if (m < 0) {
                    tl_putChar('-');
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
                tl_putChar(span);
            }
        }
    }

    va_end(arg_ptr);

    return 0;
}
#endif
