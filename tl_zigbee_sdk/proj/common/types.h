/********************************************************************************************************
 * @file     types.h
 *
 * @brief    the file used to define the data types
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


typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef int16_t coffee_page_t;

typedef unsigned char u8 ;
typedef signed char s8;

typedef unsigned short u16;
typedef signed short s16;

typedef int s32;
typedef unsigned int u32;

typedef long long s64;
typedef unsigned long long u64;


typedef void (*fn_ptr)(u8*);

typedef u32  u24;
typedef s32	 s24;

// Generic Status return
typedef u8 status_t;
typedef u32 UTCTime;

typedef u32 arg_t;

enum{
  ZB_FALSE = 0,
  ZB_TRUE = 1
};

typedef u8 cId_t;

/**
 *  @brief Type definition for extended address
 */
typedef u8 addrExt_t[8];
typedef u8 extPANId_t[8];

typedef addrExt_t	extAddr_t;

typedef union {
	u16  shortAddr;
	addrExt_t extAddr;
}tl_zb_addr_t;

typedef union
{
	u32	srcId;
	addrExt_t gpdIeeeAddr;
}gpdId_t;

/**
 *  @brief Type definition for combined short/extended device address
 */
typedef struct
{
    union
    {
        u16         shortAddr;         //!< Short address
        addrExt_t   extAddr;           //!< Extended address
    } addr;
    u8  addrMode;                      //!< Address mode
} addr_t;

typedef struct
{
	u16 id;
    u8  mode;                      //!< Address mode
} pan_id_t;

#ifdef WIN32
#   ifndef FALSE
#        define FALSE 0
#    endif

#   ifndef TRUE
#        define TRUE 1
#    endif

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
#define NULL 	0
#endif

#ifndef __cplusplus

typedef u8 bool;

#ifndef FALSE
#define FALSE 	0
#endif
#ifndef TRUE
#define TRUE 	(!FALSE)
#endif

#define false     FALSE
#define true     TRUE

#elif defined WIN32

#ifndef FALSE
#define FALSE     0
#endif

#ifndef TRUE
#define TRUE      1
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
#define U8_MAX ((u8)0xff)
#define U31_MAX ((u32)0x7fffffff)
#define U15_MAX ((u16)0x7fff)
#define U7_MAX ((u8)0x7f)


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


#define ADDR_MODE_NONE       0        //!< Address not present
#define ADDR_MODE_SHORT      2        //!< Short address
#define ADDR_MODE_EXT        3        //!< Extended address




