/********************************************************************************************************
 * @file    charger_bin.h
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

#include "reg_include/charger_reg.h"
#include "analog.h"


extern unsigned int read_done;
extern unsigned int write_done;

/**
 * @brief	This function serves to reset the charger communication module.
 * @return	none
 */
static inline void charger_reset(void)
{
	reg_rst1 &= (~FLD_RST1_CHG);
	reg_rst1 |= FLD_RST1_CHG;
}

/**
 * @brief		This function serves to control the status of the charger communication module.
 * @param[in] 	command - control command.
 * @return		none
 */
static inline void charger_control(chg_irq_control_e command)
{
	reg_chg_control = ((reg_chg_control & 0xf0)|command);
}

/**
 * @brief		This function serves to master send a byte data.
 * @param[in] 	data - data to be sent.
 * @return		none.
 */
static inline void charger_write_data(unsigned char data)
{
	reg_chg_write_data = data;
}

/**
 * @brief		This function serves to master read a byte data.
 * @return		Data received.
 */
static inline unsigned char charger_read_data(void)
{
	return reg_chg_read_data;
}

/**
 * @brief		This function serves to clear the charger communication status.
 * @param[in] 	status - the irq status.
 * @return		none.
 */
static inline void charger_clr_irq_status(chg_irq_status_e status)
{
	reg_chg_irq = status;/*add by weihua.zhang, confirmed by jianzhi.chen*/
}

/**
 * @brief       This function serves to get the charger communication status.
 * @param[in]   status    - the irq status.
 * @retval      non-zero      -  the interrupt occurred.
 * @retval      zero  -  the interrupt did not occur.
 */
static inline unsigned char charger_get_irq_status(chg_irq_status_e status)
{
	return 	reg_chg_irq & status;
}

/**
 * @brief		This function serves to set the mask of the charger communication module.
 * @param[in] 	mask - the irq mask.
 * @return		none.
 */
static inline void charger_set_irq_mask(chg_irq_mask_e mask)
{
	reg_chg_mask |= mask;
}

/**
 * @brief		This function serves to clear the mask of the charger communication module.
 * @param[in] 	mask - the irq mask.
 * @return		none.
 */
static inline void charger_clr_irq_mask(chg_irq_mask_e mask)
{
	reg_chg_mask &= ~mask;
}

/**
 * @brief		This function serves to initialize the charger module's communication function.
 * @return		none.
 */
void charger_bin_init(void);

/**
 * @brief		This function serves to judge send data completion flag.
 * @return		none.
 */
void charger_bin_mst_wait_wr_idle(void);

/**
 * @brief		This function serves to judge receive data completion flag.
 * @return		0-fail, 1-success.
 */
unsigned char charger_bin_mst_wait_rd_idle(void);

/**
 * @brief		This function serves to write one byte of data to the slave.
 * @param[in] 	cmd - control command.
 * @param[in] 	data - data to be sent.
 * @return		none.
 */
void charger_bin_mst_wr_byte(unsigned char cmd, unsigned char data);

/**
 * @brief		This function serves to read one byte of data from slave.
 * @return		Data received.
 */
unsigned int charger_bin_mst_rd_byte(void);

