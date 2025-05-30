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

#if defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
#include <stdio.h>
#endif

#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
int tl_printf(const char *format, ...);
#endif


#if (UART_PRINTF_MODE || USB_PRINTF_MODE)
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
    #define printf                                      tl_printf
#endif
    #define TRACE                                       printf

    #define DEBUG(compileFlag, ...)                     do{ \
                                                            if(compileFlag) TRACE(__VA_ARGS__); \
                                                        }while(0)

    #define DEBUG_ARRAY(compileFlag, arrayAddr, len)    do{ \
                                                            if (compileFlag) { \
                                                                TRACE("*********************************\n"); \
                                                                unsigned char i = 0; \
                                                                do { \
                                                                    TRACE(" %x", ((unsigned char *)arrayAddr)[i++]); \
                                                                } while(i < len); \
                                                                TRACE("\n*********************************\n"); \
                                                            } \
                                                        }while(0)
#else
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
    #define printf(...)
#endif
    #define TRACE(...)
    #define DEBUG(compileFlag, ...)
    #define DEBUG_ARRAY(compileFlag, arrayAddr, len)
#endif
