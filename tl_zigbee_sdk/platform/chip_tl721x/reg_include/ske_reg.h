/********************************************************************************************************
 * @file    ske_reg.h
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
#ifndef SKE_REG_H
#define SKE_REG_H
#include "soc.h"

#define SKE_LP_BASE_ADDR (0x80104000)

#define reg_ske_fifo     (0x80100800 + 0x20)
#define reg_ske_dma_en   REG_ADDR8(0x100800 + 0x24)
#define reg_ske_thres    REG_ADDR8(0x100800 + 0x25)

#endif
