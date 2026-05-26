/********************************************************************************************************
 * @file    tlkapi_debug.c
 *
 * @brief   This is the source file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "tl_common.h"
#include "tlkapi_debug.h"

_attribute_ble_data_retention_ tlk_dbg_t tlkDbgCtl = {
    .dbg_en = 1,
};

void tlkapi_ble_debug_init(bool enable)
{
    tlkDbgCtl.dbg_en = enable;
}

/**
 * @brief       check if debug log busy
 * @param[in]   none
 * @return      1: debug log busy, some log pending in FIFO, not all finish;
 *              0: debug log empty
 */
bool tlkapi_debug_isBusy(void)
{
    return 0;
}

/**
 * @brief   	Send debug log to log FIFO, printf mode
 *              attention: here just send log to FIFO, can not output immediately,
 *              wait for "tlkapi debug_handler" to output log.
 * @param[in]   format - the string will be printed
 * @return
 */

int tlk_printf(const char *format, ...)
{
#if (CONSOLE_ENABLE)
    if (tlkDbgCtl.dbg_en) {
        va_list args;
        va_start(args, format);
        printf(format, args);
        va_end(args);
    }
#endif
    return 0;
}

#define SEND_U8S_MAX_NUM                        16

_attribute_ram_code_sec_noinline_
void tlkapi_send_str_u8s(char *str, int size, ...)
{
#if (CONSOLE_ENABLE)
    u8 d[SEND_U8S_MAX_NUM];
    size = min(SEND_U8S_MAX_NUM, size);
    va_list args;
    va_start(args, size);
    for (int i = 0; i < size; i++) {
        d[i] = va_arg(args, unsigned int);
    }
    tlkapi_send_str_data(str, d, size);
    va_end(args);
#endif
}


#define SEND_U32S_MAX_NUM                       8

_attribute_ram_code_sec_noinline_
void tlkapi_send_str_u32s(char *str, int size, ...)
{
#if (CONSOLE_ENABLE)
    u32 d[SEND_U32S_MAX_NUM];
    size = min(SEND_U32S_MAX_NUM, size);
    va_list args;
    va_start(args, size);
    for (int i = 0; i < size; i++) {
        d[i] = va_arg(args, unsigned int);
    }
    tlkapi_send_str_data(str, (u8 *)d, size * 4);
    va_end(args);
#endif
}

/**
 * @brief      Send debug log to log FIFO, character string and data mixed mode.
 *             attention: here just send log to FIFO, can not output immediately,
 *             wait for "tlkapi debug_handler" to output log.
 * @param[in]  str - character string
 * @param[in]  pData - pointer of data
 * @param[in]  len - length of data
 * @return     none
 */
_attribute_ram_code_sec_noinline_
void tlkapi_send_str_data(char *str, u8 *pData, u32 data_len)
{
#if (CONSOLE_ENABLE)
    if (tlkDbgCtl.dbg_en) {
        unsigned char hex[] = "0123456789abcdef";
        const u8 *b = pData;
        u8 i;

        extern int tlk_strlen(const char *str);
        int ns = str ? tlk_strlen(str) : 0;
        while (ns--) {
            tl_printf_putChar(*str++);
        }
        tl_printf_putChar(':');

        for (i = 0; i < data_len; i++) {
            tl_printf_putChar(' ');
            tl_printf_putChar(hex[b[i] >> 4]);
            tl_printf_putChar(hex[b[i] & 0xf]);
        }

        tl_printf_putChar('\r');
        tl_printf_putChar('\n');
    }
#endif
}
