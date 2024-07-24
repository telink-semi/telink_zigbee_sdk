/********************************************************************************************************
 * @file    chacha20_poly1305_reg.h
 *
 * @brief   This is the header file for TL721X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef CHACHA20_POLY1505_REG_H
#define CHACHA20_POLY1505_REG_H
#include "soc.h"

#define CHACHA20_POLY1305_BASE_ADDR     (0x80105000)

#define reg_chacha20_fifo               (0x80100800 + 0x10)
#define reg_chacha20_dma_en             REG_ADDR8(0x100800 + 0x14)
#define reg_chacha20_thres              REG_ADDR8(0x100800 + 0x15)

#endif