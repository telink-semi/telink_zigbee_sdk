/********************************************************************************************************
 * @file    analog.c
 *
 * @brief   This is the source file for B85
 *
 * @author  Driver & Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
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
#include "analog.h"
#include "register.h"
#include "compiler.h"
#include "irq.h"

/**
 * @brief      This function serves to wait for analog register ready.
 * @param[in]  none.
 * @return     none.
 */
static inline void analog_wait(void)
{
    while(reg_ana_ctrl & FLD_ANA_BUSY){}
}

/**
 * @brief      This function serves to analog register read.
 * @param[in]  addr - address need to be read.
 * @return     the result of read.
 */
_attribute_ram_code_sec_noinline_ unsigned char analog_read(unsigned char addr)
{
    unsigned char r = irq_disable();

    reg_ana_addr = addr;
    reg_ana_ctrl = (FLD_ANA_CYC0);
    analog_wait();
    unsigned char data = reg_ana_data;
    reg_ana_ctrl = 0;
    irq_restore(r);
    return data;
}

/**
 * @brief      This function serves to analog register write.
 * @param[in]  addr - address need to be write.
 * @param[in]  v - the value need to be write.
 * @return     none.
 */
_attribute_ram_code_sec_noinline_ void analog_write(unsigned char addr, unsigned char v)
{
    unsigned char r = irq_disable();

    reg_ana_addr = addr;
    reg_ana_data = v;
    reg_ana_ctrl = (FLD_ANA_CYC0 | FLD_ANA_RW);
    analog_wait();
    reg_ana_ctrl = 0;
    irq_restore(r);
}

/**
 * @brief      This function serves to analog register read.
 * @param[in]  addr - address need to be read.
 * @param[in]  *buff - the value need to be read.
 * @param[in]  len - the length of read value.
 * @return     none.
 */
void analog_read_buff(unsigned char addr, unsigned char *buff, int len)
{
    unsigned char r = irq_disable();

    reg_ana_ctrl = 0;		// issue clock
    reg_ana_addr = addr;
    while (len--) {
        reg_ana_ctrl = FLD_ANA_CYC0 | FLD_ANA_LS_CON;
        analog_wait();

        *buff++ = reg_ana_data;
    }
    reg_ana_ctrl = 0; 		// finish

    irq_restore(r);
}

/**
 * @brief      This function serves to analog register write.
 * @param[in]  addr - address need to be write.
 * @param[in]  *v - the value need to be write.
 * @param[in]  len - the length of write value.
 * @return     none.
 */
void analog_write_buff(unsigned char addr, unsigned char *buff, int len)
{
    unsigned char r = irq_disable();

    reg_ana_addr = addr;
    while (len--) {
        reg_ana_data = *buff++;

        reg_ana_ctrl = FLD_ANA_CYC0 | FLD_ANA_LS_CON | FLD_ANA_RW; 	// multi write
        analog_wait();
    }
    reg_ana_ctrl = 0; 		// finish
    irq_restore(r);
}
