/********************************************************************************************************
 * @file    hash_reg.h
 *
 * @brief   This is the header file for tl323x
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef HASH_REG_H
#define HASH_REG_H
#include "soc.h"

#define HASH_BASE_ADDR  (0x80102000U)

#define reg_hash_fifo   (0x80100800U + 0x30)
#define reg_hash_dma_en REG_ADDR8(0x80100800U + 0x34)
#define reg_hash_thres  REG_ADDR8(0x80101800U + 0x35)

#endif
