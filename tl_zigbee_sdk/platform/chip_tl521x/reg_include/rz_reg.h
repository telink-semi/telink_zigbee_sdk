/********************************************************************************************************
 * @file    rz_reg.h
 *
 * @brief   This is the header file for tl521x
 *
 * @author  Driver Group
 * @date    2026
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
#ifndef RZ_REG_H_
#define RZ_REG_H_

#include "soc.h"
/*******************************      rz registers: 0x80140640       ******************************/
#define RZ_BASE_ADDR 0x80140640
#define reg_rz_ctrl0 REG_ADDR8(RZ_BASE_ADDR)
enum
{
    FLD_RZ_TX_CLR            = BIT(0),
};

#define reg_rz_ctrl1 REG_ADDR8(RZ_BASE_ADDR+0x01)
enum
{
    FLD_RZ_JITTER_L_EN       = BIT(0),
    FLD_RZ_JITTER_H_EN       = BIT(1),
    FLD_RZ_POLA              = BIT(2),
    FLD_RZ_BIT_MSB           = BIT(3),
    FLD_RZ_BIG_ENDIAN_MODE   = BIT(4),
    FLD_RZ_ALIGN_32BITS_MODE = BIT(5),
    FLD_RZ_GLOBAL_DATA_MODE  = BIT(6),
    FLD_RZ_ADDR_MODE         = BIT(7),
};
#define reg_rz_ctrl2 REG_ADDR8(RZ_BASE_ADDR+0x02)
enum
{
    FLD_RZ_JITTER_RANGE      = BIT_RNG(0, 2),
    FLD_RZ_FIFO_LVL          = BIT_RNG(3, 5),
    FLD_RZ_AUTO_TXCLR_EN     = BIT(7),
};

#define reg_rz_fifo_sts REG_ADDR8(RZ_BASE_ADDR + 0x3)

typedef enum
{
    FLD_RZ_TX_BUF_CNT = BIT_RNG(0, 3),
    FLD_RZ_TX_EMPTY   = BIT(4),
    FLD_RZ_TX_FULL    = BIT(5),
    FLD_RZ_EN         = BIT(6),
} rz_fifo_status_e;

#define reg_rz_t0h REG_ADDR16(RZ_BASE_ADDR + 0x4)
#define reg_rz_t0l REG_ADDR16(RZ_BASE_ADDR + 0x6)
#define reg_rz_t1h REG_ADDR16(RZ_BASE_ADDR + 0x8)
#define reg_rz_t1l REG_ADDR16(RZ_BASE_ADDR + 0xa)
#define reg_rz_tsrh REG_ADDR16(RZ_BASE_ADDR + 0xc)
#define reg_rz_tsrl REG_ADDR16(RZ_BASE_ADDR + 0xe)
#define reg_rz_pixel_num REG_ADDR16(RZ_BASE_ADDR + 0x10)
#define reg_rz_global_data_num REG_ADDR16(RZ_BASE_ADDR + 0x12)
#define reg_rz_data_bit_num_per_pixel REG_ADDR16(RZ_BASE_ADDR + 0x14)

#define reg_rz_mask REG_ADDR8(RZ_BASE_ADDR + 0x16)
typedef enum
{
    FLD_RZ_MASK_LVL     = BIT(0),
    FLD_RZ_MASK_TXDONE  = BIT(1),
    FLD_RZ_MASK_ERROR   = BIT(2),
    FLD_RZ_PEM_EVENT_EN = BIT(3),
} rz_mask_e;

#define reg_rz_int REG_ADDR8(RZ_BASE_ADDR + 0x17)
typedef enum
{
    FLD_RZ_CLR_INT_LVL    = BIT(0),
    FLD_RZ_CLR_INT_TXDONE = BIT(1),
    FLD_RZ_CLR_INT_ERROR  = BIT(2),
} rz_int_e;

#define reg_rz_tx_data REG_ADDR32(RZ_BASE_ADDR + 0x18)
#define reg_rz_tx_data_addr RZ_BASE_ADDR + 0x18

#endif /* RZ_REG_H_ */
