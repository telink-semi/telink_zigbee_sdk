/********************************************************************************************************
 * @file	types.h
 *
 * @brief	This is the header file for types
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

#if 0
typedef signed char int8_t;
typedef unsigned char uint8_t;

typedef signed short int16_t;
typedef unsigned short uint16_t;

typedef signed int int32_t;
typedef unsigned int uint32_t;

typedef int16_t coffee_page_t;
#endif

typedef unsigned char u8 ;
typedef signed char s8;

typedef unsigned short u16;
typedef signed short s16;

typedef unsigned int u32;
typedef signed int s32;


typedef long long s64;
typedef unsigned long long u64;

typedef void (*fn_ptr)(u8*);

typedef u32  u24;
typedef s32	 s24;

// Generic Status return
typedef u8 status_t;
typedef u32 UTCTime;

typedef u32 arg_t;

enum {
  ZB_FALSE = 0,
  ZB_TRUE = 1
};

typedef u8 cId_t;

/**
 *  @brief Type definition for extended address
 */
typedef u8 addrExt_t[8];
typedef u8 extPANId_t[8];

typedef addrExt_t extAddr_t;

typedef union {
	u16  shortAddr;
	addrExt_t extAddr;
}tl_zb_addr_t;

typedef union {
	u32	srcId;
	addrExt_t gpdIeeeAddr;
}gpdId_t;

/**
 *  @brief Type definition for combined short/extended device address
 */
typedef struct {
    union {
        u16         shortAddr;         //!< Short address
        addrExt_t   extAddr;           //!< Extended address
    } addr;
    u8  addrMode;                      //!< Address mode
} addr_t;

typedef struct {
	u16 id;
    u8  mode;                      //!< Address mode
} pan_id_t;

#ifdef WIN32
#ifndef FALSE
#define FALSE	0
#endif
#ifndef TRUE
#define TRUE	1
#endif

#define PACK_1
#define CODE
#define __no_init
#define GENERIC

#define NULL_OK
#define INP
#define OUTP
#define UNUSED
#define ONLY
#define READONLY
#define SHARED
#define KEEP
#endif

#ifndef NULL
#define NULL	0
#endif

#ifndef __cplusplus

typedef u8 bool;

#ifndef FALSE
#define FALSE 	0
#endif
#ifndef TRUE
#define TRUE 	(!FALSE)
#endif

#define false	FALSE
#define true	TRUE

#elif defined WIN32

#ifndef FALSE
#define FALSE	0
#endif
#ifndef TRUE
#define TRUE	1
#endif

#endif

// There is no way to directly recognise whether a typedef is defined
// http://stackoverflow.com/questions/3517174/how-to-check-if-a-datatype-is-defined-with-typedef
#ifdef __GNUC__
typedef	u16	wchar_t;		
#endif

#ifndef WIN32
typedef u32 size_t;
#endif

#define U32_MAX ((u32)0xffffffff)
#define U16_MAX ((u16)0xffff)
#define U8_MAX  ((u8)0xff)
#define U31_MAX ((u32)0x7fffffff)
#define U15_MAX ((u16)0x7fff)
#define U7_MAX  ((u8)0x7f)


/*** Generic Status Return Values ***/
#define SUCCESS                   0x00
#define FAILURE                   0x01
#define INVALIDPARAMETER          0x02
#define INVALID_TASK              0x03
#define MSG_BUFFER_NOT_AVAIL      0x04
#define INVALID_MSG_POINTER       0x05
#define INVALID_EVENT_ID          0x06
#define INVALID_INTERRUPT_ID      0x07
#define NO_TIMER_AVAIL            0x08
#define NV_ITEM_UNINIT            0x09
#define NV_OPER_FAILED            0x0A
#define INVALID_MEM_SIZE          0x0B
#define NV_BAD_ITEM_LEN           0x0C


#define ADDR_MODE_NONE				0        //!< Address not present
#define ADDR_MODE_SHORT      		2        //!< Short address
#define ADDR_MODE_EXT       		3        //!< Extended address




