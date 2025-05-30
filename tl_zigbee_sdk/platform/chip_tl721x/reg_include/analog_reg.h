/********************************************************************************************************
 * @file    analog_reg.h
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
#ifndef ANALOG_REG_H
#define ANALOG_REG_H
#include "soc.h"
/*******************************      alg  registers: 140180      ******************************/
#define ALG_BASE_ADDR 0x140180
#define reg_ana_addr  REG_ADDR8(ALG_BASE_ADDR)
#define reg_ana_ctrl  REG_ADDR8(ALG_BASE_ADDR + 0x02)

enum
{
    FLD_ANA_TX_EN         = BIT(0),
    FLD_ANA_RX_EN         = BIT(1),
    FLD_ANA_MASKX_TX_DONE = BIT(2),
    FLD_ANA_MASKX_RX_DONE = BIT(3),
    FLD_ANA_CONTIU_ACC    = BIT(4),
    FLD_ANA_RW            = BIT(5), /**< 1:write,0:read */
    FLD_ANA_CYC           = BIT(6),
    FLD_ANA_BUSY          = BIT(7),
};

#define reg_ana_len          REG_ADDR8(ALG_BASE_ADDR + 0x03)
#define ANALOG_DATA_REG_ADDR (ALG_BASE_ADDR + 0x04)
#define reg_ana_data(n)      REG_ADDR8(ANALOG_DATA_REG_ADDR + (n))
#define reg_ana_addr_data16  REG_ADDR16(ANALOG_DATA_REG_ADDR)
#define reg_ana_addr_data32  REG_ADDR32(ANALOG_DATA_REG_ADDR)


#define reg_ana_buf_cnt      REG_ADDR8(ALG_BASE_ADDR + 0x08)

enum
{
    FLD_ANA_RX_BUFCNT = BIT_RNG(0, 3),
    FLD_ANA_TX_BUFCNT = BIT_RNG(4, 7),
};

#define reg_ana_sta REG_ADDR8(ALG_BASE_ADDR + 0x09)

enum
{
    FLD_ANA_RX_DONE = BIT(7),
};

#define reg_ana_irq_sta REG_ADDR8(ALG_BASE_ADDR + 0x0a)

enum
{
    FLD_ANA_TXBUFF_IRQ = BIT(0),
    FLD_ANA_RXBUFF_IRQ = BIT(1),
};

#define reg_ana_dma_ctl REG_ADDR8(ALG_BASE_ADDR + 0x0b)

enum
{
    FLD_ANA_CYC1           = BIT(0),
    FLD_ANA_DMA_EN         = BIT(1),
    FLD_ANA_AUTO_RXCLR_EN  = BIT(2),
    FLD_ANA_NDMA_RXDONE_EN = BIT(3),
    FLD_ANA_DIV_MOD        = BIT_RNG(4, 5),

};
#endif
