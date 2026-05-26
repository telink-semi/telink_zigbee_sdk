/********************************************************************************************************
 * @file    zb_common.h
 *
 * @brief   This is the header file for zb_common
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
#ifndef TLSTK_COMMON_H
#define TLSTK_COMMON_H

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_list.h"
#include "zb_buffer.h"
#include "zb_task_queue.h"
#include "zb_primitive_task.h"
#include "zb_mm.h"
#include "aes_ccm.h"

#ifndef _CODE_MAC_
#define _CODE_MAC_
#endif

/*
 * functions return type.
 * In general, function can return OK, BLOCKED or some error. Errors are negative.
 * Error can be "generic" or some additional error code.
 */
enum {
    /* Most common return types: ok, generic error, BLOCKED, thread exit indication. */
    RET_OK = 0,
    RET_ERROR,
    RET_BLOCKED,
    RET_EXIT,
    RET_BUSY,
    RET_EOF,
    RET_OUT_OF_RANGE,
    RET_EMPTY,
    RET_CANCELLED,
    RET_PENDING,
    RET_NO_MEMORY,
    RET_INVALID_PARAMETER,
    RET_OPERATION_FAILED,
    RET_BUFFER_TOO_SMALL,
    RET_END_OF_LIST,
    RET_ALREADY_EXISTS,
    RET_NOT_FOUND, //0x10
    RET_OVERFLOW,
    RET_TIMEOUT,
    RET_NOT_IMPLEMENTED,
    RET_NO_RESOURCES,
    RET_UNINITIALIZED,
    RET_NO_SERVER,
    RET_INVALID_STATE,
    RET_DOES_NOT_EXIST,
    RET_CONNECTION_FAILED,
    RET_CONNECTION_LOST,
    RET_CANCELLED_BY_USER,
    RET_UNAUTHORIZED,
    RET_CONFLICT,
    RET_COULD_NOT_OPEN_FILE,
    RET_NO_MATCH,
    RET_PROTOCOL_ERROR, //0x20
    RET_VERSION,
    RET_MALFORMED_ADDRESS,
    RET_COULD_NOT_READ_FILE,
    RET_FILE_NOT_FOUND,
    RET_DIRECTORY_NOT_FOUND,
    RET_CONVERSION_ERROR,
    RET_INCOMPATIBLE_TYPES,
    RET_INCOMPATIBLE_TYPES_IN_COMPARE,
    RET_INCOMPATIBLE_TYPES_IN_ASSIGNMENT,
    RET_INCOMPATIBLE_TYPES_IN_EXPRESSION,
    RET_ILLEGAL_COMPARE_OPERATION,
    RET_FILE_CORRUPTED,
    RET_PAGE_NOT_FOUND,
    RET_FILE_WRITE_ERROR,
    RET_FILE_READ_ERROR,
    RET_FILE_PARTIAL_WRITE, //0x30
    RET_TOO_MANY_OPEN_FILES,
    RET_ILLEGAL_REQUEST,
    RET_INVALID_BINDING,
    RET_INVALID_GROUP,
    RET_TABLE_FULL,
    RET_NO_ACK,
    RET_ACK_OK,
    RET_NO_BOUND_DEVICE, //0x38
    RET_BUF_FULL,
    RET_INVALID = 0xFF
};


//all channels: 11 ~ 26
#define ZB_TRANSCEIVER_ALL_CHANNELS_MASK        0x07FFF800 /* 0000.0111 1111.1111 1111.1000 0000.0000 */
#define ZB_TRANSCEIVER_2400_CHN_PAGE_MASK       0xF8000000

#define ZB_BEACON_INTERVAL_USEC                 15360 /* in microseconds */

#define	TL_SUPERFRAMETIME_TO_US(n)              (n * ZB_BEACON_INTERVAL_USEC)
#define	TL_TIMEUS_TO_SUPEFRAMETIME(t)           (t / ZB_BEACON_INTERVAL_USEC)

/*
 * Convert time from beacon intervals to milliseconds
 * Try to not cause overflow in 16-bit arithmetic (with some precision lost...)
 */
#define ZB_TIME_BEACON_INTERVAL_TO_MSEC(t)      (ZB_BEACON_INTERVAL_USEC / 100 * (t) / 10)

/*
 * Convert time from milliseconds to beacon intervals
 * Try to not cause overflow in 16-bit arithmetic (with some precision lost...)
 */
#define ZB_MILLISECONDS_TO_BEACON_INTERVAL(ms)  (((10l * (ms) + 3) / (ZB_BEACON_INTERVAL_USEC / 100)))

/*
 * One second timeout
 */
#define ZB_TIME_ONE_SECOND                      ZB_MILLISECONDS_TO_BEACON_INTERVAL(1000)

/*
 * Return 1 if the number is a power of 2, works only if x > 0
 */
#define MAGIC_IS_POWER_OF_TWO(x)                ( ((x) & ((x) - 1)) == 0 )

//APS
#define	APS_ACK_WAIT_DURATION                   (3 * ZB_TIME_ONE_SECOND)


#define SHORT_ADDR_LEN                          2 //!< Network/short address length
#define EXT_ADDR_LEN                            8 //!< Extended/long/IEEE address length
#define SRC_ID_LEN                              4
#define SEC_KEY_LEN                             16

#define	TL_SETSTRUCTCONTENT(s, v)               (memset((u8 *)&s, v, sizeof(s)))


#define ZB_IS_16BYTE_SECURITY_KEY_ZERO(key)     (!memcmp((key), g_null_securityKey, SEC_KEY_LEN))
#define ZB_IS_64BIT_ADDR_ZERO(addr)             (!memcmp((addr), g_zero_addr, EXT_ADDR_LEN))
#define ZB_IS_64BIT_ADDR_INVALID(addr)          (!memcmp((addr), g_invalid_addr, EXT_ADDR_LEN))
#define ZB_64BIT_ADDR_ZERO(addr)                (memset((addr), 0, EXT_ADDR_LEN))
#define ZB_64BIT_ADDR_COPY(dst, src)            (memcpy(dst, src, EXT_ADDR_LEN))
#define ZB_64BIT_ADDR_CMP(one, two)             ((bool)!memcmp((one), (two), EXT_ADDR_LEN))

#define ZB_EXTPANID_IS_ZERO                     ZB_IS_64BIT_ADDR_ZERO
#define ZB_EXTPANID_ZERO                        ZB_64BIT_ADDR_ZERO
#define ZB_EXTPANID_COPY                        ZB_64BIT_ADDR_COPY
#define ZB_EXTPANID_CMP                         ZB_64BIT_ADDR_CMP

#define ZB_IEEE_ADDR_IS_ZERO                    ZB_IS_64BIT_ADDR_ZERO
#define ZB_IEEE_ADDR_IS_INVALID                 ZB_IS_64BIT_ADDR_INVALID
#define ZB_IEEE_ADDR_ZERO                       ZB_64BIT_ADDR_ZERO
#define	ZB_IEEE_ADDR_INVALID(addr)              ZB_64BIT_ADDR_COPY(addr, g_invalid_addr)
#define ZB_IEEE_ADDR_COPY                       ZB_64BIT_ADDR_COPY
#define ZB_IEEE_ADDR_CMP                        ZB_64BIT_ADDR_CMP

#define ZB_SEC_KEY_IS_NULL                      ZB_IS_16BYTE_SECURITY_KEY_ZERO

#define ZB_MAC_FRAME_HEADER                     (9 + 2)
#define ZB_NWK_FRAME_HEADER                     (8 + NWK_MAX_SOURCE_ROUTE * 2 + 14 + 4)
#define ZB_APS_FRAME_HEADER                     10


#define zb_random()                             ((u16)drv_u32Rand())
#define ZB_RANDOM()                             zb_random()


typedef	void (*secondCb)(void);

void secondClockRun(secondCb cb[]);
void secondClockStop(void);

void secondClockRestart(void);

extern u32 g_secondCnt;

#endif /* ZB_COMMON_H */
