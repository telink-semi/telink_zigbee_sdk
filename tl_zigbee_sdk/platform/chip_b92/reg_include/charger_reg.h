/********************************************************************************************************
 * @file    charger_reg.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#pragma once

#include "soc.h"
/*******************************      charger registers: 0x1402c0      ******************************/

#define REG_CHG_BASE        0x1402c0

#define reg_chg_write_data			REG_ADDR8(0x1402c0 + 0x00)

#define reg_chg_control				REG_ADDR8(0x1402c0 + 0x01)
typedef enum{
	FLD_CHG_CONTROL_WR_CMD      =  BIT(0),
	FLD_CHG_CONTROL_RD_CMD      =  BIT(1),
	FLD_CHG_CONTROL_CMD       	=  BIT(2),
	FLD_CHG_CONTROL_MANUAL_EN  =  BIT(3),
	FLD_CHG_CONTROL_WR_MANUAL  =  BIT(4),
	FLD_CHG_CONTROL_TX_EN       =  BIT(5),
	FLD_CHG_CONTROL_RX_EN       =  BIT(6),
	FLD_CHG_CONTROL_SOFT_RST    =  BIT(7),
}chg_irq_control_e;;

#define reg_chg_mask  				REG_ADDR8(0x1402c0 + 0x04)
typedef enum{
	FLD_CHG_MASK_PEDGE_RD       =  BIT(0),
	FLD_CHG_MASK_PEDGE_WR       =  BIT(1),
	FLD_CHG_MASK_NEDGE_RD       =  BIT(2),
	FLD_CHG_MASK_NEDGE_WR       =  BIT(3),
	FLD_CHG_MASK_RD_DONE        =  BIT(4),
	FLD_CHG_MASK_WR_DONE        =  BIT(5),
}chg_irq_mask_e;;

#define reg_chg_irq       			REG_ADDR8(0x1402c0 + 0x05)
typedef enum{
	FLD_CHG_PEDGE_RD_IRQ       =  BIT(0),
	FLD_CHG_PEDGE_WR_IRQ       =  BIT(1),
	FLD_CHG_NEDGE_RD_IRQ       =  BIT(2),
	FLD_CHG_NEDGE_WR_IRQ       =  BIT(3),
	FLD_CHG_RD_DONE_IRQ        =  BIT(4),
	FLD_CHG_WR_DONE_IRQ        =  BIT(5),
}chg_irq_status_e;

#define reg_chg_read_data       	REG_ADDR8(0x1402c0 + 0x07)


