/********************************************************************************************************
 * @file     assert.h
 *
 * @brief    assert file
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

#include "user_config.h"			//   for  __DEBUG__

#if (DEBUG_MODE)
#define assert(exp) do{ \
                        if(!(exp)){ \
                            DEBUG_OUT("File:%s,Line:%d\n",__FILE__,__LINE__); \
                            while(1){;} \
                        } \
                    }while(0)
#define trace_route do{ \
                        DEBUG_OUT("File:%s,Line:%d\n",__FILE__,__LINE__); \
                    }while(0)
#else
#define assert(ignore)        ((void) 0)
#define trace_route
#endif

////////////////////  To do compiler warning  //////////////////
// http://stackoverflow.com/questions/5966594/how-can-i-use-pragma-message-so-that-the-message-points-to-the-filelineno
// http://gcc.gnu.org/ml/gcc-help/2010-10/msg00196.html
// http://stackoverflow.com/questions/3030099/c-c-pragma-in-define-macro

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

#ifdef __GNUC__
#define COMPILE_MESSAGE(x) _Pragma (#x)
#endif

#if (__SHOW_TODO__)
#ifdef __GNUC__
#define TODO(x) COMPILE_MESSAGE(message ("--TODO-- " #x))
#else
#define TODO(x) __pragma(message("--TODO-- "_STRINGIFY(x) " ::function: " __FUNCTION__ "@"STRINGIFY(__LINE__)))
#endif
#else
#define TODO(x)
#endif

#if (__SHOW_WARN__)
#ifdef __GNUC__
#define WARN(x) COMPILE_MESSAGE(message ("--WARN-- " #x))
#else
#define WARN(x) __pragma(message("--WARN-- "_STRINGIFY(x) " ::function: " __FUNCTION__ "@"STRINGIFY(__LINE__)))
#endif
#else
#define WARN(x)
#endif

#if (__SHOW_WARN__)
#ifdef __GNUC__
#define NOTE(x) COMPILE_MESSAGE(message ("--NOTE-- " #x))
#else
#define NOTE(x) __pragma(message("--NOTE-- "_STRINGIFY(x) " ::function: " __FUNCTION__ "@"STRINGIFY(__LINE__)))
#endif
#else
#define NOTE(x)
#endif

