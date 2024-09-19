/********************************************************************************************************
 * @file    plic_sw.h
 *
 * @brief   This is the header file for TL321X
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
/** @page PLIC_SW
 *
 *  Introduction
 *  ===============
 *   Software Platform-Level Interrupt Controller(PLIC_SW)
 *
 *  API Reference
 *  ===============
 *  Header File: plic_sw.h
 *
 *  PLIC_SW features
 *  ===============
 *    - Number of interrupt : 1.
 *    - Software-programmable interrupt generation.
 *
 *  How to use this driver
 *  ===============
 *    -# Enable global interrupt using the function core_interrupt_enable().
 *    -# Enable MSI(Machine Software Interrupt) using the function core_mie_enable()
 *    -# Enable plic_sw interrupt using the function plic_sw_interrupt_enable().
 *    -# Trigger a software interrupt using the function plic_sw_set_pending().
 *    -# Re-implement the mswi_irq_handler() function and write your application code in it.
 * 
 */

#ifndef __PLIC_SW_H_
#define __PLIC_SW_H_
#include "reg_include/register.h"

/**
 * @brief       This function serves to set software interrupt pending. its means trigger a software interrupt.
 * @return      none
 */
static inline void plic_sw_set_pending(void)
{
    reg_irq_sw_pending = FLD_PLIC_SW_SRC;
}

/**
 * @brief    This function serves to enable software interrupt.
 * @return  none
 */
static inline void plic_sw_interrupt_enable(void)
{
    reg_irq_sw_src |= FLD_PLIC_SW_SRC;
}

/**
 * @brief    This function serves to disable software interrupt.
 * @return   none
 */
static inline void plic_sw_interrupt_disable(void)
{
    reg_irq_sw_src &= (~FLD_PLIC_SW_SRC);
}

/**
 * @brief    This function serves to claim software interrupt.
 * @return   It returns the software interrupt source id.
 * @note     The claim signal needs to be generated manually in the interrupt service routine.
 */
static inline unsigned int plic_sw_interrupt_claim(void)
{
    return reg_irq_sw_done;
}

/**
 * @brief    This function serves to send an interrupt complete message to allow a new software interrupt request when current software interrupt done.
 * @return   none
 * @note     Software interrupts need to be completed before exiting the interrupt service routine to ensure that software interrupts are fully released..
 */
static inline void plic_sw_interrupt_complete(void)
{
    reg_irq_sw_done = FLD_PLIC_SW_ID;
}

#endif /* end of __PLIC_SW_H_ */
