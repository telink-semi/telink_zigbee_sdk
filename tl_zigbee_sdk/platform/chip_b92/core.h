/********************************************************************************************************
 * @file    core.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef CORE_H
#define CORE_H
#include "lib/include/sys.h"
#include "nds_intrinsic.h"

#define DISABLE_BTB __asm__("csrci mmisc_ctl,8")
#define ENABLE_BTB  __asm__("csrsi mmisc_ctl,8")

/**
 * @brief Machine mode MHSP_CTL
 */
typedef enum {
    MHSP_CTL_OVF_EN     = BIT(0), /**< Enable bit for the stack overflow protection and recording mechanism. */
    MHSP_CTL_UDF_EN     = BIT(1), /**< Enable bit for the stack underflow protection mechanism. */
    MHSP_CTL_SCHM_SEL   = BIT(2), /**< 0-Stack overflow/underflow,1-Top-of-stack recording. */
    MHSP_CTL_U_EN       = BIT(3), /**< Enables the SP protection and recording mechanism in User mode. */
    MHSP_CTL_S_EN       = BIT(4), /**< Enables the SP protection and recording mechanism in Supervisor mode. */
    MHSP_CTL_M_EN       = BIT(5), /**< Enables the SP protection and recording mechanism in Machine mode. */
} mhsp_ctl_e;

typedef enum
{
	FLD_MSTATUS_MIE = BIT(3),//M-mode interrupt enable bit
}mstatus_e;
typedef enum
{
	FLD_MIE_MSIE     = BIT(3),//M-mode software interrupt enable bit.
	FLD_MIE_MTIE     = BIT(7),//M-mode timer interrupt enable bit
	FLD_MIE_MEIE     = BIT(11),//M-mode external interrupt enable bit
}mie_e;

/**
 * @brief MEI, MSI, and MTI interrupt nesting priorities.
 * @note
 *        - By default, interrupt nesting is disabled.
 *        - When interrupt nesting is enabled
 *          - MEI, MSI and MTI occur simultaneously, they are handled under the following order:MEI > MSI > MTI.
 *          - Other cases as described in core_preempt_pri_e below.
 */
typedef enum {
    CORE_PREEMPT_PRI_MODE0 = FLD_MIE_MSIE | FLD_MIE_MTIE, /**< MTI and MSI cannot interrupt MEI, MSI and MTI can be nested within each other. */
    CORE_PREEMPT_PRI_MODE1 = FLD_MIE_MTIE,                /**< MTI cannot interrupt MEI, MSI and MTI can be nested within each other, MSI and MEI can be nested within each other. */
    CORE_PREEMPT_PRI_MODE2 = FLD_MIE_MSIE,                /**< MSI cannot interrupt MEI, MSI and MTI can be nested within each other, MTI and MEI can be nested within each other. */
    CORE_PREEMPT_PRI_MODE3 = BIT(1),                      /**< MEI, MSI and MTI can be nested within each other(MIE register bit1 is an invalid bit). */
}core_preempt_pri_e;

#define  read_csr(reg)		         __nds__csrr(reg)
#define  write_csr(reg, val)	      __nds__csrw(val, reg)
#define  swap_csr(reg, val)	          __nds__csrrw(val, reg)
#define set_csr(reg, bit)	         __nds__csrrs(bit, reg)
#define clear_csr(reg, bit)	         __nds__csrrc(bit, reg)

/*
 * Inline nested interrupt entry/exit macros
 */
/* Save/Restore macro */
#define save_csr(r)             long __##r = read_csr(r);
#define restore_csr(r)           write_csr(r, __##r);
/* Support PowerBrake (Performance Throttling) feature */


#define save_mxstatus()         save_csr(NDS_MXSTATUS)
#define restore_mxstatus()      restore_csr(NDS_MXSTATUS)

/* Nested external IRQ entry macro : Save CSRs and enable global interrupt.
 * - If mei does not want to be interrupted by msi and mti, can do the following
 *     save_csr(NDS_MIE)                                 \
 *     save_csr(NDS_MEPC)                                \
 *     save_csr(NDS_MSTATUS)                             \
 *     save_mxstatus()                                   \
 *     clear_csr(NDS_MIE, FLD_MIE_MTIE | FLD_MIE_MSIE);  \
 *     set_csr(NDS_MSTATUS, FLD_MSTATUS_MIE);
 */
#define core_save_nested_context()                              \
	 save_csr(NDS_MEPC)                              \
	 save_csr(NDS_MSTATUS)                           \
	 save_mxstatus()                                 \
	 set_csr(NDS_MSTATUS, FLD_MSTATUS_MIE);

/* Nested external IRQ exit macro : Restore CSRs
 * - If closed mti and msi in mei, can restore with the following
 *     clear_csr(NDS_MSTATUS, FLD_MSTATUS_MIE);        \
 *     restore_csr(NDS_MSTATUS)                        \
 *     restore_csr(NDS_MEPC)                           \
 *     restore_mxstatus()                              \
 *     restore_csr(NDS_MIE);
 */
#define core_restore_nested_context()                               \
	 clear_csr(NDS_MSTATUS, FLD_MSTATUS_MIE);            \
	 restore_csr(NDS_MSTATUS)                        \
	 restore_csr(NDS_MEPC)                           \
	 restore_mxstatus()

#define fence_iorw	      	__nds__fence(FENCE_IORW,FENCE_IORW)

typedef enum{
	FLD_FEATURE_PREEMPT_PRIORITY_INT_EN = BIT(0),
	FLD_FEATURE_VECTOR_MODE_EN 			= BIT(1),
}
feature_e;


/**
 * @brief Disable interrupts globally in the system.
 * @return  r - the MIE bit value of machine status register(MSTATUS).
 * @note  this function must be used when the system wants to disable all the interrupt.
 * @return     none
 */
_attribute_ram_code_sec_optimize_o2_ static inline unsigned int core_interrupt_disable(void){
	unsigned int r = read_csr (NDS_MSTATUS)&FLD_MSTATUS_MIE;
	if(r)
	{
		clear_csr(NDS_MSTATUS,FLD_MSTATUS_MIE);//global interrupts disable
        fence_iorw; /* Hardware may change this value, fence IO ensures that software changes are valid. */
	}
	return r;
}

/**
 * @brief restore interrupts globally in the system.
 * @param[in]  en - the MIE bit value of machine status register(MSTATUS).
 * this  parameter must be the return value of the core_interrupt_disable function.
 * @return     0
 * @note this function must be used when the system wants to restore all the interrupt.
 */
_attribute_ram_code_sec_optimize_o2_ static inline unsigned int core_restore_interrupt(unsigned int en){
	if(en)
	{
		set_csr(NDS_MSTATUS,en);//global interrupts enable
        fence_iorw; /* Hardware may change this value, fence IO ensures that software changes are valid. */
	}
	return 0;
}

/**
 * @brief This function serves to enable MEI(Machine External Interrupt),MTI(Machine timer Interrupt),or MSI(Machine Software Interrupt).
 * @param[in] mie_mask - MIE(Machine Interrupt Enable) register mask.
 * @return  none
 */
static _always_inline void core_mie_enable(mie_e mie_mask)
{
    set_csr(NDS_MIE, mie_mask);
}

/**
 * @brief This function serves to disable MEI(Machine External Interrupt),MTI(Machine timer Interrupt),or MSI(Machine Software Interrupt).
 * @param[in] mie_mask - MIE(Machine Interrupt Enable) register mask.
 * @return  none
 */
static _always_inline void core_mie_disable(mie_e mie_mask)
{
    clear_csr(NDS_MIE, mie_mask);
}

/**
 * @brief This function serves to enable interrupts globally in the system, MEI(machine external interrupt) will also enable.
 * @return  none
 */
static inline void core_interrupt_enable(void)
{
    set_csr(NDS_MSTATUS, FLD_MSTATUS_MIE);//global interrupts enable
    fence_iorw; /* Hardware may change this value, fence IO ensures that software changes are valid. */
    core_mie_enable(FLD_MIE_MEIE);//machine interrupt enable selectively
}

/**
 * @brief This function serves to set mhsp control registers.
 * @param[in]  ctl - the value of mhsp_ctl, refer to mhsp_ctl_e for the definition of MHSP_CTL
 * @return     none
 */
static inline void core_set_mhsp_ctr(mhsp_ctl_e ctl)
{
    write_csr(NDS_MHSP_CTL, (unsigned int)ctl);
}

/**
 * @brief This function serves to set hsp bound registers.
 *  ---Stack overflow---
 When the SP overflow detection mechanism is properly selected and enabled, any updated value to
 the SP register (via any instruction) is compared with the msp_bound register If the updated value
 to the SP register is smaller than the msp_bound register, a stack overflow exception is generated
 a stack overflow exception is generated.
 The stack overflow exception has an exception code of 32 in the mcause register
 ---Top-of-stack recording---
 When the top of stack recording mechanism is properly selected and enabled, any updated value to
 the SP register on any instruction is compared with the msp_bound register. If the updated value
 to the SP register is smaller than the msp_bound register, the msp_bound register is updated with this updated value
 * @param[in]  bound - the value of  hsp_bound .
 * @return     none
 */
static inline void core_set_msp_bound(unsigned int bound)
{
    write_csr(NDS_MSP_BOUND, bound);
}

/**
 * @brief This function serves to set hsp base registers.
 When the SP underflow detection mechanism is properly selected and enabled, any updated value to
 the SP register (via any instruction) is compared with the msp_base register. If the updated value to
 the SP register is greater than the msp_base register, a stack underflow exception is generated. The
 stack underflow exception has an exception code of 33 in the mcause register.
 * @param[in]  base - the value of hsp_base .
 * @return     none
 */
static inline void core_set_msp_base(unsigned int base)
{
    write_csr(NDS_MSP_BASE, base);
}

/**
 * @brief This function serves to get hsp bound registers.
 * ---Top-of-stack recording---
 When the top of stack recording mechanism is properly selected and enabled, any updated value to
 the SP register on any instruction is compared with the msp_bound register. If the updated value
 to the SP register is smaller than the msp_bound register, the msp_bound register is updated with this updated value,you can read this value.
 * @return     none
 */
static inline unsigned int core_get_msp_bound(void)
{
    return read_csr(NDS_MSP_BOUND);
}

/**
 * @brief This function serves to get hsp base registers.there is no recording.
 * @return     none
 */
static inline unsigned int core_get_msp_base(void)
{
    return read_csr(NDS_MSP_BASE);
}

/**
 * @brief This function serves to get current sp(Stack pointer).
 * @return     none
 */
static inline unsigned int core_get_current_sp(void)
{
    return __nds__get_current_sp();
}

/**
 * @brief This function serves to get mcause(Machine Cause) value.
 * This register indicates the cause of trap, reset, NMI or the interrupt source ID of a vector interrupt.
   This register is updated when a trap, reset, NMI or vector interrupt occurs
 * @return     none
 */
static inline unsigned int core_get_mcause(void)
{
	return read_csr(NDS_MCAUSE);
}

/**
 * @brief This function serves to get mepc(Machine Exception Program Counter) value.
 * When entering an exception, the hardware will automatically update the value of the mepc register
 * to the value of the instruction pc currently encountered with the exception
 * @return     none
 */
static inline unsigned int core_get_mepc(void)
{
	return read_csr(NDS_MEPC);
}

/**
 * @brief    This function serves to enable mcu entry WFI(Wait-For-Interrupt) mode similar to stall mcu.
 * @return   none
 * @note:    there are two awoke modes by interrupt:
 *             - When global interrupts are enabled using the interface core_interrupt_enable() (mstatus.MIE is enabled)
 *               - Before entering WFI, make sure the following conditions are met:
 *                 -# Enable MEI, MSI or MTI using interface core_mie_enable(), so that can wake up mcu after core enters WFI.
 *                 -# If there is an interrupt that has already been triggered, the corresponding interrupt flag bit has been cleared.
 *                 -# Interrupt enable for wakeup source using interface plic_interrupt_enable(), interrupt disable for non-wakeup sources using interface plic_interrupt_disable().
 *               - After exiting WFI, the processor is awoken by a taken interrupt, it will resume and start to execute from the corresponding interrupt service routine, the processing steps in ISR as follows:
*                  -# Clear the corresponding interrupt flag bit in the corresponding interrupt service routine.
 *                 -# Your application code.
 *             - When global interrupts are disabled using the interface core_interrupt_disable() (mstatus.MIE is disabled)
 *               - Before entering WFI, make sure the following conditions are met:
 *                 -# Enable MEI, MSI or MTI using interface core_mie_enable(), so that can wake up mcu after core enters WFI.
 *                 -# If there is an interrupt that has already been triggered, the corresponding interrupt flag bit has been cleared.
 *                 -# Interrupt enable for wakeup source using interface plic_interrupt_enable(), interrupt disable for non-wakeup sources using interface plic_interrupt_disable().
 *                 -# Clear all current requests from the PLIC using the plic_clr_all_request() interface.
 *               - After exiting WFI, the processor is awoken by a pending interrupt, it will resume and start to execute from the instruction after the WFI instruction, the processing steps after WFI instruction as follows:
 *                 -# Getting the wakeup source using interface plic_interrupt_claim().
 *                 -# Take stimer for example, using the interfaces stimer_get_irq_status() and stimer_clr_irq_status() to get and clear the corresponding interrupt flag bit according to the interrupt source.
 *                 -# Your application code.
 *                 -# Using interface plic_interrupt_complete() to notify PLIC that the corresponding interrupt processing is complete.
 */
static inline void core_entry_wfi_mode(void)
{
    /* Interrupts disabled by the mie CSR will not be able to wake up the processor.
      However,the processor can be awoken by these interrupts regardless the value of the global interrupt enable bit (mstatus.MIE)*/
    __asm__ __volatile__("wfi");
}

/**
 * @brief     This function serves to get current pc.
 * @return    current pc
 */
static inline unsigned int core_get_current_pc(void)
{
    unsigned int current_pc = 0;
    __asm__("auipc %0, 0" : "=r"(current_pc)::"a0");
    return current_pc;
}

/**
 * @brief     This function performs to get cclk tick.
 * @return    cclk timer tick value.
 */
__attribute__((always_inline)) static inline unsigned long long rdmcycle(void)
{
#if __riscv_xlen == 32
	do {
		unsigned long hi = read_csr(NDS_MCYCLEH);
		unsigned long lo = read_csr(NDS_MCYCLE);

		if (hi == read_csr(NDS_MCYCLEH))
			return ((unsigned long long)hi << 32) | lo;
	} while(1);
#else
	return read_csr(NDS_MCYCLE);
#endif
}

/**
 * @brief       This function performs to set delay time by cclk tick.
 * @param[in]   core_cclk_tick - Number of ticks in cclk
 * @return      none
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void core_cclk_delay_tick(unsigned long long core_cclk_tick);

#endif
