/********************************************************************************************************
 * @file    tlPrintf.h
 *
 * @brief   This is the header file for tlPrintf
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
#pragma once

#if defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || \
    defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL323X) || defined(MCU_CORE_TL521X)
#include <stdio.h>
#include <stdarg.h>
#elif defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
#define OCTAL_OUTPUT            8
#define DECIMAL_OUTPUT          10
#define HEX_OUTPUT              16

#define _INTSIZEOF(n)           ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))

typedef char *va_list;

#define va_start(ap, v)         (ap = (va_list)&v + _INTSIZEOF(v))
#define va_arg(ap, t)           (*(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
#define va_end(ap)              (ap = (va_list)0)

int tl_printf(const char *format, ...);
#define printf                  tl_printf
#endif

typedef void (*tl_putCharFn_t)(const unsigned char byte);

void tl_printf_register(tl_putCharFn_t fn);
void tl_printf_putChar(const unsigned char byte);
