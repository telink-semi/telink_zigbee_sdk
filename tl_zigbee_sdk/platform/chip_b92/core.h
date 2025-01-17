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
#include "reg_include/core_reg.h"
#include <stdbool.h>

#define MCAUSE_INT   0x80000000UL
#define MCAUSE_CAUSE 0x7FFFFFFFUL


#define TRAP_U_ECALL 8
#define TRAP_M_ECALL 11


/* Machine CSR */
#define MSTATUS_MPP_MSK ((1ULL << (11)) | (1ULL << (12)))
/* Hart mode for machine/supervisor/user */
#define MSTATUS_MPP_USER (0ULL << (11))
#define MSTATUS_MPP_SV   (1ULL << (11))
#define MSTATUS_MPP_MC   ((1ULL << (11)) | (1ULL << (12)))

#ifdef STD_GCC
    #define DISABLE_BTB __asm__("csrci %0,8" ::"i"(mmisc_ctl))
    #define ENABLE_BTB  __asm__("csrsi %0,8" ::"i"(mmisc_ctl))
#else
    #define DISABLE_BTB __asm__("csrci mmisc_ctl,8")
    #define ENABLE_BTB  __asm__("csrsi mmisc_ctl,8")
#endif

/**
 * @brief Machine mode MHSP_CTL
 */
typedef enum
{
    MHSP_CTL_OVF_EN   = BIT(0), /**< Enable bit for the stack overflow protection and recording mechanism. */
    MHSP_CTL_UDF_EN   = BIT(1), /**< Enable bit for the stack underflow protection mechanism. */
    MHSP_CTL_SCHM_SEL = BIT(2), /**< 0-Stack overflow/underflow,1-Top-of-stack recording. */
    MHSP_CTL_U_EN     = BIT(3), /**< Enables the SP protection and recording mechanism in User mode. */
    MHSP_CTL_S_EN     = BIT(4), /**< Enables the SP protection and recording mechanism in Supervisor mode. */
    MHSP_CTL_M_EN     = BIT(5), /**< Enables the SP protection and recording mechanism in Machine mode. */
} mhsp_ctl_e;

typedef enum
{
    FLD_MSTATUS_MIE = BIT(3), //M-mode interrupt enable bit
} mstatus_e;

typedef enum
{
    FLD_MIE_MSIE = BIT(3),  //M-mode software interrupt enable bit.
    FLD_MIE_MTIE = BIT(7),  //M-mode timer interrupt enable bit
    FLD_MIE_MEIE = BIT(11), //M-mode external interrupt enable bit
} mie_e;

/**
 * @brief MEI, MSI, and MTI interrupt nesting priorities.
 * @note
 *        - By default, interrupt nesting is disabled.
 *        - When interrupt nesting is enabled
 *          - MEI, MSI and MTI occur simultaneously, they are handled under the following order:MEI > MSI > MTI.
 *          - Other cases as described in core_preempt_pri_e below.
 */
typedef enum
{
    CORE_PREEMPT_PRI_MODE0 = FLD_MIE_MSIE | FLD_MIE_MTIE, /**< MTI and MSI cannot interrupt MEI, MSI and MTI can be nested within each other. */
    CORE_PREEMPT_PRI_MODE1 = FLD_MIE_MTIE,                /**< MTI cannot interrupt MEI, MSI and MTI can be nested within each other, MSI and MEI can be nested within each other. */
    CORE_PREEMPT_PRI_MODE2 = FLD_MIE_MSIE,                /**< MSI cannot interrupt MEI, MSI and MTI can be nested within each other, MTI and MEI can be nested within each other. */
    CORE_PREEMPT_PRI_MODE3 = BIT(1),                      /**< MEI, MSI and MTI can be nested within each other(MIE register bit1 is an invalid bit). */
} core_preempt_pri_e;

#define read_csr(reg)         ({ unsigned long __tmp; \
  __asm__ volatile ("csrr %0, %1" : "=r"(__tmp) : "i" (reg)); \
  __tmp; })

#define write_csr(reg, val)   ({ __asm__ volatile("csrw %0, %1" ::"i"(reg), "rK"(val)); })

#define swap_csr(reg, val)    ({ unsigned long __tmp; \
  __asm__ volatile ("csrrw %0, %1, %2" : "=r"(__tmp) : "i" (reg), "rK"(val)); \
  __tmp; })

#define set_csr(reg, bit)     ({ unsigned long __tmp; \
  __asm__ volatile ("csrrs %0, %1, %2" : "=r"(__tmp) : "i" (reg), "rK"(bit)); \
  __tmp; })

#define clear_csr(reg, bit)   ({ unsigned long __tmp; \
  __asm__ volatile ("csrrc %0, %1, %2" : "=r"(__tmp) : "i" (reg), "rK"(bit)); \
  __tmp; })

#define fence_iorw            __asm__ volatile("fence" : : : "memory")

#define core_get_current_sp() ({ unsigned long __tmp; \
  __asm__ volatile ("mv %0, sp" : "=r"(__tmp)); \
  __tmp; })

/*
 * Inline nested interrupt entry/exit macros
 */
/* Save/Restore macro */
#define save_csr(r)    long __##r = read_csr(r);
#define restore_csr(r) write_csr(r, __##r);
/* Support PowerBrake (Performance Throttling) feature */


#define save_mxstatus()    save_csr(NDS_MXSTATUS)
#define restore_mxstatus() restore_csr(NDS_MXSTATUS)

/* Nested external IRQ entry macro : Save CSRs and enable global interrupt.
 * - If mei does not want to be interrupted by msi and mti, can do the following
 *     save_csr(NDS_MIE)                                 \
 *     save_csr(NDS_MEPC)                                \
 *     save_csr(NDS_MSTATUS)                             \
 *     save_mxstatus()                                   \
 *     clear_csr(NDS_MIE, FLD_MIE_MTIE | FLD_MIE_MSIE);  \
 *     set_csr(NDS_MSTATUS, FLD_MSTATUS_MIE);
 */
#define core_save_nested_context() \
    save_csr(NDS_MEPC)             \
        save_csr(NDS_MSTATUS)      \
            save_mxstatus()        \
                set_csr(NDS_MSTATUS, FLD_MSTATUS_MIE);

/* Nested external IRQ exit macro : Restore CSRs
 * - If closed mti and msi in mei, can restore with the following
 *     clear_csr(NDS_MSTATUS, FLD_MSTATUS_MIE);        \
 *     restore_csr(NDS_MSTATUS)                        \
 *     restore_csr(NDS_MEPC)                           \
 *     restore_mxstatus()                              \
 *     restore_csr(NDS_MIE);
 */
#define core_restore_nested_context()        \
    clear_csr(NDS_MSTATUS, FLD_MSTATUS_MIE); \
    restore_csr(NDS_MSTATUS)                 \
        restore_csr(NDS_MEPC)                \
            restore_mxstatus()

typedef enum
{
    FLD_FEATURE_PREEMPT_PRIORITY_INT_EN = BIT(0),
    FLD_FEATURE_VECTOR_MODE_EN          = BIT(1),
} feature_e;

/**
 * @brief Disable interrupts globally in the system.
 * @return  r - the MIE bit value of machine status register(MSTATUS).
 * @note  this function must be used when the system wants to disable all the interrupt.
 * @return     none
 */
_attribute_ram_code_sec_optimize_o2_ static inline unsigned int core_interrupt_disable(void)
{
    unsigned int r = read_csr(NDS_MSTATUS) & FLD_MSTATUS_MIE;
    if (r) {
        clear_csr(NDS_MSTATUS, FLD_MSTATUS_MIE); //global interrupts disable
        fence_iorw;                              /* Hardware may change this value, fence IO ensures that software changes are valid. */
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
_attribute_ram_code_sec_optimize_o2_ static inline unsigned int core_restore_interrupt(unsigned int en)
{
    if (en) {
        set_csr(NDS_MSTATUS, en); //global interrupts enable
        fence_iorw;               /* Hardware may change this value, fence IO ensures that software changes are valid. */
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
 * @brief      This function serves to disable MEI(Machine External Interrupt),MTI(Machine timer Interrupt),or MSI(Machine Software Interrupt).
 * @param[in]  mie_mask - MIE(Machine Interrupt Enable) register mask.
 * @return     mie register value before disable.
 * @note       core_mie_disable and core_mie_restore must be used in pairs.
 */
static inline unsigned int core_mie_disable(mie_e mie_mask)
{
    unsigned int r = read_csr(NDS_MIE);
    clear_csr(NDS_MIE, mie_mask);
    return r;
}

/**
 * @brief      This function serves to restore MIE register value.
 * @param[in]  mie_value - mie register value returned by core_mie_disable().
 * @return     none.
 * @note       core_mie_disable and core_mie_restore must be used in pairs.
 */
static inline void core_mie_restore(unsigned int mie_value)
{
    write_csr(NDS_MIE, mie_value);
}

/**
 * @brief This function serves to enable interrupts globally in the system, MEI(machine external interrupt) will also enable.
 * @return  none
 */
static inline void core_interrupt_enable(void)
{
    set_csr(NDS_MSTATUS, FLD_MSTATUS_MIE); //global interrupts enable
    fence_iorw;                            /* Hardware may change this value, fence IO ensures that software changes are valid. */
    core_mie_enable(FLD_MIE_MEIE);         //machine interrupt enable selectively
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

        if (hi == read_csr(NDS_MCYCLEH)) {
            return ((unsigned long long)hi << 32) | lo;
        }
    } while (1);
#else
    return read_csr(NDS_MCYCLE);
#endif
}

/**
 * @brief     This function serves to set timeout by us.
 * @param[in] ref  - reference tick of cclk .
 * @param[in] us   - count by us.
 * @return    true - timeout, false - not timeout
 */
static _always_inline bool core_cclk_time_exceed(unsigned long long ref, unsigned int us)
{
    return ((unsigned long long)(rdmcycle() - ref) > us * sys_clk.cclk);
}

/**
 * @brief       This function performs to set delay time by cclk tick.
 * @param[in]   core_cclk_tick - Number of ticks in cclk
 * @return      none
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void core_cclk_delay_tick(unsigned long long core_cclk_tick);

typedef bool (*condition_fp)(void);
typedef bool (*condition_fp_with_param)(unsigned int);
typedef void (*timeout_handler_fp)(unsigned int err_code);

/**
 * @brief       provides a unified timeout interface(for internal calls only).
 * @param[in]   condition - function pointer, timeout judgment condition, must be bool xxx(void).
 * @param[in]   timeout_us - timeout(us).
 * @param[in]   func -  function pointer, timeout exit handle,must be void xxx(unsigned int).
 * @param[in]   err_code - xxx_api_error_code_e/drv_api_error_code_e.
 * @return      1: timeout(the upper layer interface uses drv_api_status_e(DRV_API_TIMEOUT)) 0: pass
 * @note        the rules for parameter are as follows (for internal purposes only):
 *              wait_condition_fails_or_timeout_with_param:
 *              1.If the peripheral module has only one timeout judgment, can call this interface directly;
 *              2.If the peripheral module has multiple calls, for the code simplicity, can be redefined by macros (refer to I2C_WAIT);
 *              3.If the function is called by another .c file or by itself .h,
 *                define it in the.h file and state that it is for internal use (refer to analog.h),otherwise put it in.c;
 *              condition:
 *              1.What is the type of the function according to the type of the upper layer interface;
 *              2.For the new conditional interface,if the function is called by another .c file or by itself .h,
 *                define it in the.c file,declare in.h and state that it is for internal use (refer to analog.h),otherwise put it in.c;
 *              3.For the original interface in.h,if the application is likely to call, determine whether to add timeout precautions;
 *              timeout_us:
 *              1.If determine that the timeout is caused by own interface(1.must reboot 2.reset is solved,
 *                but it is called in many places, and function calls are too nested, and most functions have no return type),
 *                use g_drv_api_error_timeout_us,and the corresponding set interface drv_set_error_timeout;
 *              2.If the timeout may be caused by an external source or own unexpected exceptions need to be reset,
 *                define the timeout global variable in the corresponding module(g_xxx_error_timeout_us),
 *                and the corresponding interface xxx_set_error_timeout:
 *                - If timeout may be related to the other side of the communication,
 *                  add comment minimum time limit and solution(refer to i2c);
 *                - If the timeout may be caused own unexpected exceptions need to be reset,
 *                  the timeout solution has been added to the parameter func,just note the minimum time limit(refer to aes);
 *              func:
 *              1.If determine that the timeout is caused by own interface(1.must reboot 2.reset is solved, but it is called
 *                in many places, and function calls are too nested, and most functions have no return type), use timeout_handler;
 *              2.If the timeout may be caused by an external source or own unexpected exceptions need to be reset,
 *                define the timeout_handler interface in the corresponding module (xxx_timeout_handler):
 *                - If timeout may be related to the other side of the communication,
 *                  the driver only needs to record err_code using global variable g_xxx_error_timeout_code(refer to i2c);
 *                - If the timeout may be caused own unexpected exceptions need to be reset(one wrong reasons),
 *                  add the timeout solution to the interface,and (void)err_code(refer to trng);
 *                - If the timeout may be caused own unexpected exceptions need to be reset(many wrong reasons),
 *                  add the timeout solution to the interface,and (void)err_code(refer to aes);
 *              err_code:
 *              1.If determine that the timeout is caused by own interface(1.must reboot 2.reset is solved,
 *                but it is called in many places, and function calls are too nested, and most functions have no return type),
 *                use drv_api_error_code_e(self-add);
 *              2.If the timeout may be caused by an external source or own unexpected exceptions need to be reset:
 *                 - If timeout may be related to the other side of the communication,
 *                   define the enumeration xxx_api_error_timeout_code_e, the global variable g_xxx_error_timeout_code,
 *                   and the interface xxx_get_error_timeout_code(refer to i2c);
 *                 - If the timeout may be caused own unexpected exceptions need to be reset(one wrong reasons),
 *                   err_code parameter transfer is 0,no need to define the enumeration xxx_api_error_timeout_code_e,
 *                   the global variable g_xxx_error_timeout_code, and the interface xxx_get_error_timeout_code(refer to trng);
 *                 - If the timeout may be caused own unexpected exceptions need to be reset(many wrong reasons),
 *                   only define the enumeration xxx_api_error_timeout_code_e, no need to define the global variable
 *                   g_xxx_error_timeout_code, and the interface xxx_get_error_timeout_code(refer to aes);
 */
unsigned int wait_condition_fails_or_timeout(condition_fp condition, unsigned int timeout_us, timeout_handler_fp func, unsigned int err_code);

/**
 * @brief       provides a unified timeout interface(condition with parameter)(for internal calls only).
 * @param[in]   condition - function pointer, timeout judgment condition, must be bool xxx(unsigned int).
 * @param[in]   cdt_param  - condition parameter.
 * @param[in]   timeout_us - timeout(us).
 * @param[in]   func -  function pointer, timeout exit handle,must be void xxx(unsigned int).
 * @param[in]   err_code - xxx_api_error_code_e/drv_api_error_code_e.
 * @return      1: timeout(the upper layer interface uses drv_api_status_e(DRV_API_TIMEOUT)) 0: pass
 * @note        the rules for parameter are as follows (for internal purposes only):
 *              wait_condition_fails_or_timeout_with_param:
 *              1.If the peripheral module has only one timeout judgment, can call this interface directly;
 *              2.If the peripheral module has multiple calls, for the code simplicity, can be redefined by macros (refer to I2C_WAIT);
 *              3.If the function is called by another .c file or by itself .h,
 *                define it in the.h file and state that it is for internal use (refer to analog.h),otherwise put it in.c;
 *              condition:
 *              1.What is the type of the function according to the type of the upper layer interface;
 *              2.For the new conditional interface,if the function is called by another .c file or by itself .h,
 *                define it in the.c file,declare in.h and state that it is for internal use (refer to analog.h),otherwise put it in.c;
 *              3.For the original interface in.h,if the application is likely to call, determine whether to add timeout precautions;
 *              timeout_us:
 *              1.If determine that the timeout is caused by own interface(1.must reboot 2.reset is solved,
 *                but it is called in many places, and function calls are too nested, and most functions have no return type),
 *                use g_drv_api_error_timeout_us,and the corresponding set interface drv_set_error_timeout;
 *              2.If the timeout may be caused by an external source or own unexpected exceptions need to be reset,
 *                define the timeout global variable in the corresponding module(g_xxx_error_timeout_us),
 *                and the corresponding interface xxx_set_error_timeout:
 *                - If timeout may be related to the other side of the communication,
 *                  add comment minimum time limit and solution(refer to i2c);
 *                - If the timeout may be caused own unexpected exceptions need to be reset,
 *                  the timeout solution has been added to the parameter func,just note the minimum time limit(refer to aes);
 *              func:
 *              1.If determine that the timeout is caused by own interface(1.must reboot 2.reset is solved, but it is called
 *                in many places, and function calls are too nested, and most functions have no return type), use timeout_handler;
 *              2.If the timeout may be caused by an external source or own unexpected exceptions need to be reset,
 *                define the timeout_handler interface in the corresponding module (xxx_timeout_handler):
 *                - If timeout may be related to the other side of the communication,
 *                  the driver only needs to record err_code using global variable g_xxx_error_timeout_code(refer to i2c);
 *                - If the timeout may be caused own unexpected exceptions need to be reset(one wrong reasons),
 *                  add the timeout solution to the interface,and (void)err_code(refer to trng);
 *                - If the timeout may be caused own unexpected exceptions need to be reset(many wrong reasons),
 *                  add the timeout solution to the interface,and (void)err_code(refer to aes);
 *              err_code:
 *              1.If determine that the timeout is caused by own interface(1.must reboot 2.reset is solved,
 *                but it is called in many places, and function calls are too nested, and most functions have no return type),
 *                use drv_api_error_code_e(self-add);
 *              2.If the timeout may be caused by an external source or own unexpected exceptions need to be reset:
 *                 - If timeout may be related to the other side of the communication,
 *                   define the enumeration xxx_api_error_timeout_code_e, the global variable g_xxx_error_timeout_code,
 *                   and the interface xxx_get_error_timeout_code(refer to i2c);
 *                 - If the timeout may be caused own unexpected exceptions need to be reset(one wrong reasons),
 *                   err_code parameter transfer is 0,no need to define the enumeration xxx_api_error_timeout_code_e,
 *                   the global variable g_xxx_error_timeout_code, and the interface xxx_get_error_timeout_code(refer to trng);
 *                 - If the timeout may be caused own unexpected exceptions need to be reset(many wrong reasons),
 *                   only define the enumeration xxx_api_error_timeout_code_e, no need to define the global variable
 *                   g_xxx_error_timeout_code, and the interface xxx_get_error_timeout_code(refer to aes);
 */
unsigned int wait_condition_fails_or_timeout_with_param(condition_fp_with_param condition, unsigned int cdt_param, unsigned int timeout_us, timeout_handler_fp func, unsigned int err_code);
#endif
