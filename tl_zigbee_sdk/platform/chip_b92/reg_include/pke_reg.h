/********************************************************************************************************
 * @file    pke_reg.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef PKE_REG_H_
#define PKE_REG_H_
#include "soc.h"


/*******************************      pke registers: 0x110000      ******************************/

#define PKE_BASE_ADDR		     0x80110000

/***************** PKE register *******************/
#define PKE_CTRL           (*((volatile unsigned int *)(PKE_BASE_ADDR)))
#define PKE_CFG            (*((volatile unsigned int *)(PKE_BASE_ADDR+0x04)))
#define PKE_MC_PTR         (*((volatile unsigned int *)(PKE_BASE_ADDR+0x08)))
#define PKE_RISR           (*((volatile unsigned int *)(PKE_BASE_ADDR+0x0C)))
#define PKE_IMCR           (*((volatile unsigned int *)(PKE_BASE_ADDR+0x10)))
#define PKE_MISR           (*((volatile unsigned int *)(PKE_BASE_ADDR+0x14)))
#define PKE_RT_CODE        (*((volatile unsigned int *)(PKE_BASE_ADDR+0x24)))
#define PKE_EXE_CONF       (*((volatile unsigned int *)(PKE_BASE_ADDR+0x50)))   //
#define PKE_VERSION        (*((volatile unsigned int *)(PKE_BASE_ADDR+0xFC)))
#define PKE_A(a, step)     ((volatile unsigned int *)(PKE_BASE_ADDR+0x0400+(a)*(step)))
#define PKE_B(a, step)     ((volatile unsigned int *)(PKE_BASE_ADDR+0x1000+(a)*(step)))

#endif










