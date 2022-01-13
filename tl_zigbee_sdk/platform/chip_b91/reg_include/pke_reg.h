/********************************************************************************************************
 * @file    pke_reg.h
 *
 * @brief   This is the header file for B91
 *
 * @author  Driver Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
 *******************************************************************************************************/

#pragma once

#include "../sys.h"



/*******************************      pke registers: 0x110000      ******************************/

#define REG_PKE_BASE			0x110000

#define reg_pke_ctrl			REG_ADDR32(REG_PKE_BASE+0x00)
enum{
	FLD_PKE_CTRL_START			= BIT(0),
	FLD_PKE_CTRL_STOP			= BIT(16),
};

#define reg_pke_conf			REG_ADDR32(REG_PKE_BASE+0x04)
typedef enum{
	FLD_PKE_CONF_IRQ_EN			= BIT(8),
	FLD_PKE_CONF_PARTIAL_RADIX	= BIT_RNG(16,23),
	FLD_PKE_CONF_BASE_RADIX		= BIT_RNG(24,26),
}pke_conf_e;

#define reg_pke_mc_ptr			REG_ADDR32(REG_PKE_BASE+0x10)

#define reg_pke_stat			REG_ADDR32(REG_PKE_BASE+0x20)
typedef enum{
	FLD_PKE_STAT_DONE			= BIT(0),
}pke_status_e;

#define reg_pke_rt_code			REG_ADDR32(REG_PKE_BASE+0x24)
enum{
	FLD_PKE_RT_CODE_STOP_LOG	= BIT_RNG(0,3),
};

#define reg_pke_exe_conf		REG_ADDR32(REG_PKE_BASE+0x50)
enum{
	FLD_PKE_EXE_CONF_IAFF_R0	= BIT(0),
	FLD_PKE_EXE_CONF_IMON_R0	= BIT(1),
	FLD_PKE_EXE_CONF_IAFF_R1	= BIT(2),
	FLD_PKE_EXE_CONF_IMON_R1	= BIT(3),
	FLD_PKE_EXE_CONF_OAFF		= BIT(4),
	FLD_PKE_EXE_CONF_OMON		= BIT(5),
	FLD_PKE_EXE_CONF_ME_SCA_EN	= BIT_RNG(8,9),
};




