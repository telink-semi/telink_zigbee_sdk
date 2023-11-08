/********************************************************************************************************
 * @file    plic.h
 *
 * @brief   This is the header file for B91
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/** @page PLIC
 *
 *  Introduction
 *  ===============
 *  Platform-Level Interrupt Controller (PLIC)
 *
 *  API Reference
 *  ===============
 *  Header File: plic.h
 *
 *  PLIC features
 *  ===============
 *    - Compatible with RISC-V PLIC
 *    - For preemptive priority interrupt extension, when preempt is enabled, high priority interrupt can disturb low priority interrupt.
 *    - There are two modes of interrupt, vector and regular mode, and the driver defaults to vector mode(each external interrupt has its own entry address, which can speed up the response to interrupts).
 *
 *  How to use this driver
 *  ===============
 *    - Use the feature of preemptive priority interrupt:
 *        -# Enable preempt using the function plic_preempt_feature_en().
 *        -# Set interrupt priority and threshold using the function plic_set_priority() and plic_set_threshold(), interrupt occurs only when priority is greater than threshold.
 *            - default priority is 1.
 *            - default threshold is 0.
 *        -# Enable global interrupt using the function core_interrupt_enable().
 *        -# Enable plic interrupt source, for example stimer module plic_interrupt_enable(IRQ1_SYSTIMER).
 *        -# Enable the mask of the corresponding module, for example stimer_set_irq_mask().
 *    - Do not use the feature of preemptive priority interrupt:
 *        -# Enable global interrupt using the function core_interrupt_enable().
 *        -# Enable plic interrupt source, for example stimer module plic_interrupt_enable(IRQ1_SYSTIMER).
 *        -# Enable the mask of the corresponding module, for example stimer_set_irq_mask().
 *    - Interrupt service function
 *        -# Since the default interrupt service function is weak (defined in plic_isr.c), you need to re-implement your interrupt service function.
 *        -# Get the interrupt status flag bit and clear it, for example stimer_get_irq_status() determines whether corresponding interrupt occurs, and stimer_clr_irq_status() clears it.
 *        -# Your application code.
 *        -# Return from interrupt.
 *        -# The machine attribute was added to the interrupt function, and the compiler saw that it would insert code that modified the protection register, for example:
 *           void  entry_irq1(void) __attribute__ ((interrupt ("machine")));
 *
 */

#ifndef  INTERRUPT_H
#define  INTERRUPT_H
#include "reg_include/register.h"
#include "core.h"

#include "compiler.h"

/**
 * @brief interrupt preempt config.
 */
typedef struct
{
	unsigned char preempt_en;
	unsigned char threshold;
}preempt_config_t ;

/**
 * @brief declare the interrupt service function type.
 */
typedef void (*func_isr_t) (void);

/**
 * @brief global variable is used to indicate whether interrupt nesting is supported.
 */
extern _attribute_data_retention_sec_ volatile unsigned char g_plic_preempt_en;

/**
 * @brief interrupt source number.
 */
typedef enum{
	IRQ0_EXCEPTION ,
	IRQ1_SYSTIMER,
	IRQ2_ALG,
	IRQ3_TIMER1,
	IRQ4_TIMER0,
	IRQ5_DMA,
	IRQ6_BMC,
	IRQ7_USB_CTRL_EP_SETUP,
	IRQ8_USB_CTRL_EP_DATA,
	IRQ9_USB_CTRL_EP_STATUS,
	IRQ10_USB_CTRL_EP_SETINF,
	IRQ11_USB_ENDPOINT,
	IRQ12_ZB_DM,
	IRQ13_ZB_BLE,
	IRQ14_ZB_BT,
	IRQ15_ZB_RT,
	IRQ16_PWM,
	IRQ17_PKE,
	IRQ18_UART1,
	IRQ19_UART0,
	IRQ20_DFIFO,
	IRQ21_I2C,
	IRQ22_SPI_AHB,
	IRQ23_SPI_APB,
	IRQ24_USB_PWDN,
	IRQ25_GPIO,
	IRQ26_GPIO2RISC0,
	IRQ27_GPIO2RISC1,
	IRQ28_SOFT,

	IRQ29_NPE_BUS0,
	IRQ30_NPE_BUS1,
	IRQ31_NPE_BUS2,
	IRQ32_NPE_BUS3,
	IRQ33_NPE_BUS4,

	IRQ34_USB_250US,
	IRQ35_USB_RESET,
	IRQ36_NPE_BUS7,
	IRQ37_NPE_BUS8,

	IRQ42_NPE_BUS13=42,
	IRQ43_NPE_BUS14,
	IRQ44_NPE_BUS15,

	IRQ46_NPE_BUS17=46,

	IRQ50_NPE_BUS21=50,
	IRQ51_NPE_BUS22,
	IRQ52_NPE_BUS23,
	IRQ53_NPE_BUS24,
	IRQ54_NPE_BUS25,
	IRQ55_NPE_BUS26,
	IRQ56_NPE_BUS27,
	IRQ57_NPE_BUS28,
	IRQ58_NPE_BUS29,
	IRQ59_NPE_BUS30,
	IRQ60_NPE_BUS31,

	IRQ61_NPE_COMB,
	IRQ62_PM_TM,
	IRQ63_EOC,

} irq_source_e;

/**
 * @brief interrupt source priority. the larger the value, the higher the priority. default priority value is 1.
 * @note  an interrupt occurs only when priority is greater than threshold
 */
typedef enum{
	IRQ_PRI_LEV0, /**< 0 indicates that no interrupt is generated. */
	IRQ_PRI_LEV1,
	IRQ_PRI_LEV2,
	IRQ_PRI_LEV3,
}irq_priority_e;

/**
 * @brief target interrupt priority threshold. the larger the value, the higher the threshold. default threshold value is 0.
 */
typedef enum{
	IRQ_PRI_NUM0,
	IRQ_PRI_NUM1,
	IRQ_PRI_NUM2,
	IRQ_PRI_NUM3,
}irq_threshold_e;

/**
 * @brief    this function serves to set plic feature.
 * @param[in]   feature - preemptive priority interrupt feature and the vector mode.
 * @return  none
 */
static inline void plic_set_feature (feature_e feature)
{
	reg_irq_feature = feature; /* set preempt and vector mode */
}

/**
 * @brief    this function serves to enable preemptive priority interrupt feature.
 *              - these three types of interrupts occur simultaneously, they are handled under the following order:M-mode external interrupt(MEI) > M-mode software interrupt(MSI) > M-mode timer interrupt(MTI).
 *              - MEI, MSI and MTI do not occur simultaneously, they can be nested within each other.
 *              - only MEI interrupt sources can set different priority levels.
 * @return   none
 *
 */
static inline void plic_preempt_feature_en (void)
{
	reg_irq_feature |= FLD_FEATURE_PREEMPT_PRIORITY_INT_EN;
	g_plic_preempt_en=1;
}

/**
 * @brief    this function serves to disable preemptive priority interrupt feature.
 * @return  none
 */
static inline void plic_preempt_feature_dis (void)
{
	reg_irq_feature &=(~ FLD_FEATURE_PREEMPT_PRIORITY_INT_EN);
	g_plic_preempt_en=0;
}


/**
 * @brief    this function serves to set plic pending. its provide a way for software to trigger an interrupt without relying on external devices interrupt source.
 * @param[in]  src - interrupt source.
 * @return  none
 */
static inline void plic_set_pending (irq_source_e src)
{
	reg_irq_pending(src)=BIT(src%32);
}

/**
 * @brief    this function serves to set externals interrupt priority threshold,only active interrupts with priorities strictly greater than the threshold will cause interrupt, default threshold value is 0.
 * @param[in]   threshold -  threshold level.
 * @return  none
 */
static inline void plic_set_threshold (irq_threshold_e threshold)
{
/*the priority number in the threshold register will be saved to a preempted priority stack and the new priority number of the claimed interrupt will be written to the threshold register.
when the mcu sends an interrupt completion message to the PLIC (plic_interrupt_complete()), the PLIC will restore(Hardware automatic operation) the highest priority number in the preempted priority stack back to the priority threshold register.
it is possible that when hardware and software write threshold registers at the same time, there is a risk of software write failure, so turn off the global interrupt before software writes, and resume after writing*/
	unsigned int r = core_interrupt_disable();
	reg_irq_threshold=threshold;
	core_restore_interrupt(r);
}

/**
 * @brief    this function serves to set external interrupts preemptive priority level,The priority value 0 is reserved to mean "never interrupt".
 * the larger the priority value, the higher the interrupt priority, default priority value is 1.
 * @param[in]   src - interrupt source.
 * @param[in]   priority - priority level.
 * @return  none
 */
static inline void plic_set_priority (irq_source_e src, irq_priority_e priority)
{
	reg_irq_src_priority(src)=priority;
}


/**
 * @brief    this function serves to enable plic interrupt source.
 * @param[in]   src - interrupt source.
 * @return  none
 */
static inline void plic_interrupt_enable(irq_source_e  src)
{
	reg_irq_src(src) |= BIT(src%32);

}

/**
 * @brief    this function serves to disable plic interrupt source.
 * @param[in]   src - interrupt source.
 * @return  none
 */
static inline void plic_interrupt_disable(irq_source_e  src)
{
	reg_irq_src(src) &= (~ BIT(src%32));
}

/**
 * @brief    this function serves to send an interrupt complete message(by software) to the gateway to allow a new interrupt request when current interrupt done.
 * @param[in] src - interrupt source.
 * @return  none
 */
static inline void plic_interrupt_complete(irq_source_e  src)
{
	reg_irq_done = src;
}

/**
 * @brief    this function serves to get claimed interrupt id.
 * @return   it return the claimed interrupt source id in irq_source_e enum.
 * @note     vector interrupt the hardware will automatically claim an interrupt, in general, the software does not need to claim.
 *
 */
static inline  unsigned int plic_interrupt_claim(void)
{
	return reg_irq_done;
}




/**
 * @brief       this function serves to secure code by reconfigure interrupt threshold or mstatus.MIE to enter the critical section, when enter some function process such as flash.
 * @param[in]   preempt_en
 *                - 1 means interrupt priority larger than the given threshold can disturb function process.
 *                - 0 means can't disturb by interrupt, global interrupt(mstatus.MIE) will be disable.
 * @param[in]   threshold
 *                - plic interrupt threshold. when the interrupt priority > given threshold, the function process will be disturb by interrupt.
 * @return
 *                - return 0 when preempt_en = 1 and interrupt nesting is supported.
 *                - return the value of the mstatus.MIE bit in other cases.
 * @note    plic_enter_critical_sec and plic_exit_critical_sec must be used in pairs
*/
_attribute_ram_code_sec_noinline_ unsigned int plic_enter_critical_sec(unsigned char preempt_en ,unsigned char threshold);



/**
 * @brief    this function serves to restore interrupt threshold or mstatus.MIE to exit the critical section, when exit some function process such as flash.
 * @param[in]   preempt_en
 *                - 1 means it needs to restore the value of interrupt threshold.
 *                - 0 means it needs to restore mstatus.MIE.it must be the same as the preempt_en value passed by the plic_enter_critical_sec function.
 * @param[in]   r
 *                 - the value of mstatus.MIE or threshold to restore when exit critical section, it must be the value returned by the plic_enter_critical_sec function
 * @return  none
 * @note        when preempt_en = 1 and interrupt nesting is supported, the interrupt threshold will be set to 0 when this function returns.
 *
*/
_attribute_ram_code_sec_noinline_ void plic_exit_critical_sec(unsigned char preempt_en ,unsigned int r);

/**
 * @brief       this function serves to execute the interrupt service function. you can call this function when an interrupt occurs.
 * @param[in]   func - interrupt service function.
 * @param[in]   src - interrupt source.
 * @return      none
 */
_attribute_ram_code_sec_ void plic_isr(func_isr_t func, irq_source_e src);

#endif
