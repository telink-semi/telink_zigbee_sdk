/********************************************************************************************************
 * @file	plic_isr.c
 *
 * @brief	This is the source file for tl321x
 *
 * @author	Driver Group
 * @date	2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
/*
 * All interrupt entry functions and weak definitions of related processing functions are defined here,If you dont use the relevant interrupt function,
 *  you can delete it to save ram_code space.
 ****************************************************************************************
 */
#include "tl_common.h"

/**********************************************************************************
 * The interrupt sources used in the SDK.
 * If you want to use other interrupts, you'll need to find the corresponding registered
 * function in the commented code below and put it here.
 **********************************************************************************/
//interrupt handler register
#if (__PROJECT_TL_BOOT_LOADER__)
extern void uart0_irq_handler(void);
extern void dma_irq_handler(void);

PLIC_ISR_REGISTER(uart0_irq_handler, IRQ_UART0)
PLIC_ISR_REGISTER(dma_irq_handler, IRQ_DMA)
#else
extern void uart0_irq_handler(void);
extern void dma_irq_handler(void);
extern void rf_irq_handler(void);
extern void timer1_irq_handler(void);
extern void timer0_irq_handler(void);
extern void stimer_irq_handler(void);
extern void gpio0_irq_handler(void);
extern void gpio1_irq_handler(void);
extern void gpio2_irq_handler(void);

PLIC_ISR_REGISTER(uart0_irq_handler, IRQ_UART0)
PLIC_ISR_REGISTER(dma_irq_handler, IRQ_DMA)
PLIC_ISR_REGISTER(rf_irq_handler, IRQ_ZB_RT)
PLIC_ISR_REGISTER(timer1_irq_handler, IRQ_TIMER1)
PLIC_ISR_REGISTER(timer0_irq_handler, IRQ_TIMER0)
PLIC_ISR_REGISTER(stimer_irq_handler, IRQ_SYSTIMER)
PLIC_ISR_REGISTER(gpio0_irq_handler, IRQ_GPIO_IRQ0)
PLIC_ISR_REGISTER(gpio1_irq_handler, IRQ_GPIO_IRQ1)
PLIC_ISR_REGISTER(gpio2_irq_handler, IRQ_GPIO_IRQ2)
#endif

/**
 * @brief The value of the mtval register when the exception is entered.
 */
static volatile long exception_mtval;

/**
 * @brief The value of the mepc register when the exception is entered.
 */
static volatile long exception_mepc;

/**
 * @brief The value of the mstatus register when the exception is entered.
 */
static volatile long exception_mstatus;

/**
 * @brief The value of the mcause register when the exception is entered.
 */
static volatile long exception_mcause;

/**
 * @brief The value of the mdcause register when the exception is entered.
 */
static volatile long exception_mdcause;

/**
 * @brief     Default mtime irq handler.
 * @return    none
 */
__attribute__((weak)) void mtime_irq_handler(void)
{
}

/**
 * @brief     Default swi irq handler.
 * @return    none
 */
__attribute__((weak)) void mswi_irq_handler(void)
{
}

/**
 * @brief     Default exception irq handler.
 * @return      none
 */
__attribute__((weak)) void except_handler(void)
{
    /* unhandled trap */
    exception_mtval = read_csr(NDS_MTVAL);
    exception_mepc = read_csr(NDS_MEPC);
    exception_mstatus = read_csr(NDS_MSTATUS);
    exception_mcause = read_csr(NDS_MCAUSE);
    exception_mdcause = read_csr(NDS_MDCAUSE);

    while (1)
    {
        __asm__("nop");
    }
}

/**
 * @brief       This function service to handle all the platform pre-defined interrupt or exception.
 * @return      none
 */
_attribute_ram_code_sec_noinline_ void trap_entry(void) __attribute__((interrupt("machine"), aligned(4)));
__attribute__((weak)) void trap_entry(void)
{
    long mcause = read_csr(NDS_MCAUSE);
    long mepc = 0;
    long mstatus = 0;
    long mxstatus = 0;

    if(g_plic_preempt_en)
    {
        mepc = read_csr(NDS_MEPC);
        mstatus = read_csr(NDS_MSTATUS);
        mxstatus = read_csr(NDS_MXSTATUS);
    }

    if((mcause & 0x80000000UL) && ((mcause & 0x7FFFFFFFUL) == 7)) /* machine timer interrupt */
    {
        if(g_plic_preempt_en)
        {
            /* before enable global interrupt,disable the timer interrupt to prevent re-entry */
            core_mie_disable(FLD_MIE_MTIE);
            set_csr(NDS_MSTATUS, FLD_MSTATUS_MIE);
        }

        mtime_irq_handler();

        if(g_plic_preempt_en)
        {
            clear_csr(NDS_MSTATUS, FLD_MSTATUS_MIE);
            /* re-enable the timer interrupt. */
            core_mie_enable(FLD_MIE_MTIE);
        }
    }
    else if((mcause & 0x80000000UL) && ((mcause & 0x7FFFFFFFUL) == 3)) /* machine software interrupt */
    {
        plic_sw_interrupt_claim();
        /* if support interrupt nest,enable global interrupt */
        if(g_plic_preempt_en)
        {
            set_csr(NDS_MSTATUS, FLD_MSTATUS_MIE);
        }

        mswi_irq_handler();

        if(g_plic_preempt_en)
        {
            clear_csr(NDS_MSTATUS, FLD_MSTATUS_MIE);
        }

        plic_sw_interrupt_complete();
    }
    else /* unhandled Trap */
    {
        except_handler();
    }

    if(g_plic_preempt_en)
    {
        write_csr(NDS_MSTATUS, mstatus);
        write_csr(NDS_MEPC, mepc);
        write_csr(NDS_MXSTATUS, mxstatus);
    }
}
