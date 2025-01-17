/********************************************************************************************************
 * @file    qdec_reg.h
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
#ifndef QDEC_REG_H
#define QDEC_REG_H
#include "soc.h"
/*******************************      qdec registers: 0x140240      ******************************/


#define REG_QDEC_BASE    0x140240

#define reg_qdec_count0  REG_ADDR8(REG_QDEC_BASE + 0x00)

#define reg_qdec_dbntime REG_ADDR8(REG_QDEC_BASE + 0x01)

enum
{
    FLD_QDEC_DBNTIME  = BIT_RNG(0, 2),
    FLD_QDEC_POLA     = BIT(4),
    FLS_QDEC_SHUTTLE0 = BIT(5),
};

#define reg_qdec_channel_a REG_ADDR8(REG_QDEC_BASE + 0x02)

enum
{
    FLD_QDEC_CHANNEL_A = BIT_RNG(0, 2),
};

#define reg_qdec_channel_b REG_ADDR8(REG_QDEC_BASE + 0x03)

enum
{
    FLD_QDEC_CHANNEL_B = BIT_RNG(0, 2),
};

#define reg_qdec_mask REG_ADDR8(REG_QDEC_BASE + 0x04)

enum
{
    FLD_QDEC_MASK = BIT(0),
};

#define reg_qdec_int0 REG_ADDR8(REG_QDEC_BASE + 0x05)

enum
{
    FLD_QDEC_INT0 = BIT(0),
};

#define reg_qdec_reset REG_ADDR8(REG_QDEC_BASE + 0x06)

#define reg_qdec_mode  REG_ADDR8(REG_QDEC_BASE + 0x07)

enum
{
    FLD_QDEC_DOUBLE0 = BIT(0),
};

#define reg_qdec_load REG_ADDR8(REG_QDEC_BASE + 0x08)

enum
{
    FLD_QDEC_COUNT0_RELOAD = BIT(0),
};


#endif
