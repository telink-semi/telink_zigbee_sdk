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
 *        -# Enable plic interrupt source, for example stimer module plic_interrupt_enable(IRQ_SYSTIMER).
 *        -# Enable the mask of the corresponding module, for example stimer_set_irq_mask().
 *    - Do not use the feature of preemptive priority interrupt:
 *        -# Enable global interrupt using the function core_interrupt_enable().
 *        -# Enable plic interrupt source, for example stimer module plic_interrupt_enable(IRQ_SYSTIMER).
 *        -# Enable the mask of the corresponding module, for example stimer_set_irq_mask().
 *    - Interrupt service routine
 *        -# Defining interrupt service routine, recommended name see @ref irq_source_index.
 *        -# Register interrupt service routine using the PLIC_ISR_REGISTER macro, for example: PLIC_ISR_REGISTER(stimer_irq_handler, IRQ_SYSTIMER).
 *          - stimer_irq_handler - stimer interrupt service routine.
 *          - IRQ_SYSTIMER - stimer interrupt number.
 *        -# In interrupt service routine:
 *          -# Get the interrupt status flag bit and clear it, for example stimer_get_irq_status() determines whether corresponding interrupt occurs, and stimer_clr_irq_status() clears it.
 *          -# Your application code.
 *          -# Return from interrupt.
 *        -# For example:
 *              @code
 *               _attribute_ram_code_sec_ void stimer_irq_handler(void)
 *               {
 *                   if(stimer_get_irq_status(FLD_SYSTEM_IRQ))
 *                   {
 *                       stimer_clr_irq_status(FLD_SYSTEM_IRQ);
 *                       stimer_set_irq_capture(stimer_get_tick() + SYSTEM_TIMER_TICK_1S);
 *                       gpio_toggle(LED2);
 *                   }
 *               }
 *               PLIC_ISR_REGISTER(stimer_irq_handler, IRQ_SYSTIMER)
 *              @endcode
 *    - Saving and disabling global interrupts and restoring global interrupts example.
 *        -# Saving and disabling global interrupts example:
 *            @code
 *            void global_interrupts_save_and_disable(void)
 *            {
 *                g_mstatus_value = core_interrupt_disable();
 *                g_mie_value     = core_mie_disable(FLD_MIE_MSIE | FLD_MIE_MTIE | FLD_MIE_MEIE);
 *                plic_all_interrupt_save_and_disable();
 *            }
 *            @endcode
 *        -# Restoring global interrupts example:
 *            @code
 *            void global_interrupts_restore(void)
 *            {
 *                plic_all_interrupt_restore();
 *                core_mie_restore(g_mie_value);
 *                core_restore_interrupt(g_mstatus_value);
 *            }
 *            @endcode
 *    - WFI Example.
 *      # Before entering WFI, set the wakeup source and turn off unneeded interrupts:
 *          @code
 *          plic_irqs_preprocess_for_wfi(1, FLD_MIE_MEIE); // disable all interrupts except MEIE.
 *          plic_interrupt_enable(IRQ_SYSTIMER); // stimer wakeup.
 *          core_entry_wfi_mode(); // enter wfi.
 *          @endcode
 *      # After WFI wakeup, clear the corresponding interrupt flag bit and restore other interrupts:
 *          @code
 *          core_entry_wfi_mode();
 *          stimer_clr_irq_status(FLD_SYSTEM_IRQ); // clear stimer irq status.
 *          plic_irqs_postprocess_for_wfi(); // restore interrupts.
 *          @endcode
 */

#ifndef INTERRUPT_H
#define INTERRUPT_H
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
} preempt_config_t;

/**
 * @brief declare the interrupt service function type.
 */
typedef void (*func_isr_t)(void);

/**
 * @brief global variable is used to indicate whether interrupt nesting is supported.
 */
extern _attribute_data_retention_sec_ volatile unsigned char g_plic_preempt_en;

/**
 * @defgroup irq_source_index Interrupt source index definition
 * @{
 * @brief Defines the number of each interrupt in the system and its corresponding handler function.
 */
#define IRQ_SYSTIMER           1  /* STIMER_IRQ: system timer interrupt, - stimer_irq_handler */
#define IRQ_ALG                2  /* ALGM_IRQ: analog register master interface interrupt, - analog_irq_handler */
#define IRQ_TIMER1             3  /* TIMER1_IRQ, - timer1_irq_handler */
#define IRQ_TIMER0             4  /* TIMER0_IRQ, - timer0_irq_handler */
#define IRQ_DMA                5  /* DMA_IRQ_IRQ, - dma_irq_handler */
#define IRQ_BMC                6  /* BMC_IRQ: ahb bus matrix controller interrupt, - bmc_irq_handler */
#define IRQ_USB_CTRL_EP_SETUP  7  /* USB_SETUP_IRQ: USB setup interrupt, - usb_ctrl_ep_setup_irq_handler */
#define IRQ_USB_CTRL_EP_DATA   8  /* USB_DATA_IRQ: USB data interrupt, - usb_ctrl_ep_data_irq_handler */
#define IRQ_USB_CTRL_EP_STATUS 9  /* USB_STATUS_IRQ: USB status interrupt, - usb_ctrl_ep_status_irq_handler */
#define IRQ_USB_CTRL_EP_SETINF 10 /* USB_SETINF_IRQ: USB set interface interrupt, - usb_ctrl_ep_setinf_irq_handler */
#define IRQ_USB_ENDPOINT       11 /* USB_EDP_IRQ: USB edp (1-8) interrupt, - usb_endpoint_irq_handler */
#define IRQ_ZB_DM              12 /* ZB_DM_IRQ, - rf_dm_irq_handler */
#define IRQ_ZB_BLE             13 /* ZB_BLE_IRQ, - rf_ble_irq_handler */
#define IRQ_ZB_BT              14 /* ZB_BT_IRQ: BR/EDR sub-system interrupt, - rf_bt_irq_handler */
#define IRQ_ZB_RT              15 /* ZB_BLE_TL_IRQ: BLE (TL) sub-system interrupt, - rf_irq_handler */
#define IRQ_PWM                16 /* PWM_IRQ, - pwm_irq_handler */
#define IRQ_PKE                17 /* PKE_IRQ, - pke_irq_handler */
#define IRQ_UART1              18 /* UART1_IRQ, - uart1_irq_handler */
#define IRQ_UART0              19 /* UART0_IRQ, - uart0_irq_handler */
#define IRQ_DFIFO              20 /* DFIFO_IRQ: audio dma fifo interrupt, - audio_irq_handler */
#define IRQ_I2C                21 /* I2C_IRQ, - i2c_irq_handler */
#define IRQ_SPI_AHB            22 /* HSPI_IRQ, - hspi_irq_handler */
#define IRQ_SPI_APB            23 /* PSPI_IRQ, - pspi_irq_handler */
#define IRQ_USB_PWDN           24 /* USB_PWDN_IRQ: USB suspend interrupt, - usb_pwdn_irq_handler */
#define IRQ_GPIO               25 /* GPIO_IRQ, - gpio_irq_handler */
#define IRQ_GPIO2RISC0         26 /* GPIO2RISC[0]_IRQ, - gpio_risc0_irq_handler */
#define IRQ_GPIO2RISC1         27 /* GPIO2RISC[1]_IRQ, - gpio_risc1_irq_handler */
#define IRQ_SOFT               28 /* SOFT_IRQ: software interrupt, - soft_irq_handler */
#define IRQ_NPE_BUS0           29 /* NPE_BUS0_IRQ, - npe_bus0_irq_handler */
#define IRQ_NPE_BUS1           30 /* NPE_BUS1_IRQ, - npe_bus1_irq_handler */
#define IRQ_NPE_BUS2           31 /* NPE_BUS2_IRQ, - npe_bus2_irq_handler */
#define IRQ_NPE_BUS3           32 /* NPE_BUS3_IRQ, - npe_bus3_irq_handler */
#define IRQ_NPE_BUS4           33 /* NPE_BUS4_IRQ, - npe_bus4_irq_handler */
#define IRQ_USB_250US          34 /* USB_250US_SOF_IRQ: USB 250us or SOF interrupt, - usb_250us_irq_handler */
#define IRQ_USB_RESET          35 /* USB_RESET_IRQ: USB reset interrupt, - usb_reset_irq_handler */
#define IRQ_NPE_BUS7           36 /* NPE_BUS7_IRQ, - npe_bus7_irq_handler */
#define IRQ_NPE_BUS8           37 /* NPE_BUS8_IRQ, - npe_bus8_irq_handler */

#define IRQ_NPE_BUS13          42 /* NPE_BUS13_IRQ, - npe_bus13_irq_handler */
#define IRQ_NPE_BUS14          43 /* NPE_BUS14_IRQ, - npe_bus14_irq_handler */
#define IRQ_NPE_BUS15          44 /* NPE_BUS15_IRQ, - npe_bus15_irq_handler */

#define IRQ_NPE_BUS17          46 /* NPE_BUS17_IRQ, - npe_bus17_irq_handler */

#define IRQ_NPE_BUS21          50 /* NPE_BUS21_IRQ, - npe_bus21_irq_handler */
#define IRQ_NPE_BUS22          51 /* NPE_BUS22_IRQ, - npe_bus22_irq_handler */
#define IRQ_NPE_BUS23          52 /* NPE_BUS23_IRQ, - npe_bus23_irq_handler */
#define IRQ_NPE_BUS24          53 /* NPE_BUS24_IRQ, - npe_bus24_irq_handler */
#define IRQ_NPE_BUS25          54 /* NPE_BUS25_IRQ, - npe_bus25_irq_handler */
#define IRQ_NPE_BUS26          55 /* NPE_BUS26_IRQ, - npe_bus26_irq_handler */
#define IRQ_NPE_BUS27          56 /* NPE_BUS27_IRQ, - npe_bus27_irq_handler */
#define IRQ_NPE_BUS28          57 /* NPE_BUS28_IRQ, - npe_bus28_irq_handler */
#define IRQ_NPE_BUS29          58 /* NPE_BUS29_IRQ, - npe_bus29_irq_handler */
#define IRQ_NPE_BUS30          59 /* NPE_BUS30_IRQ, - npe_bus30_irq_handler */
#define IRQ_NPE_BUS31          60 /* NPE_BUS31_IRQ, - npe_bus31_irq_handler */
#define IRQ_NPE_COMB           61 /* NPE_COMB_IRQ, - npe_comb_irq_handler */
#define IRQ_PM_TM              62 /* PM_IRQ, - pm_irq_handler */
#define IRQ_EOC                63 /* EOC_IRQ, - eoc_irq_handler */

/**
 * @brief The following macros are for compatibility with previous definitions and will be deprecated in the future.
 */
#define IRQ1_SYSTIMER            IRQ_SYSTIMER           /* STIMER_IRQ: system timer interrupt, - stimer_irq_handler */
#define IRQ2_ALG                 IRQ_ALG                /* ALGM_IRQ: analog register master interface interrupt, - analog_irq_handler */
#define IRQ3_TIMER1              IRQ_TIMER1             /* TIMER1_IRQ, - timer1_irq_handler */
#define IRQ4_TIMER0              IRQ_TIMER0             /* TIMER0_IRQ, - timer0_irq_handler */
#define IRQ5_DMA                 IRQ_DMA                /* DMA_IRQ_IRQ, - dma_irq_handler */
#define IRQ6_BMC                 IRQ_BMC                /* BMC_IRQ: ahb bus matrix controller interrupt, - bmc_irq_handler */
#define IRQ7_USB_CTRL_EP_SETUP   IRQ_USB_CTRL_EP_SETUP  /* USB_SETUP_IRQ: USB setup interrupt, - usb_ctrl_ep_setup_irq_handler */
#define IRQ8_USB_CTRL_EP_DATA    IRQ_USB_CTRL_EP_DATA   /* USB_DATA_IRQ: USB data interrupt, - usb_ctrl_ep_data_irq_handler */
#define IRQ9_USB_CTRL_EP_STATUS  IRQ_USB_CTRL_EP_STATUS /* USB_STATUS_IRQ: USB status interrupt, - usb_ctrl_ep_status_irq_handler */
#define IRQ10_USB_CTRL_EP_SETINF IRQ_USB_CTRL_EP_SETINF /* USB_SETINF_IRQ: USB set interface interrupt, - usb_ctrl_ep_setinf_irq_handler */
#define IRQ11_USB_ENDPOINT       IRQ_USB_ENDPOINT       /* USB_EDP_IRQ: USB edp (1-8) interrupt, - usb_endpoint_irq_handler */
#define IRQ12_ZB_DM              IRQ_ZB_DM              /* ZB_DM_IRQ, - rf_dm_irq_handler */
#define IRQ13_ZB_BLE             IRQ_ZB_BLE             /* ZB_BLE_IRQ, - rf_ble_irq_handler */
#define IRQ14_ZB_BT              IRQ_ZB_BT              /* ZB_BT_IRQ: BR/EDR sub-system interrupt, - rf_bt_irq_handler */
#define IRQ15_ZB_RT              IRQ_ZB_RT              /* ZB_BLE_TL_IRQ: BLE (TL) sub-system interrupt, - rf_irq_handler */
#define IRQ16_PWM                IRQ_PWM                /* PWM_IRQ, - pwm_irq_handler */
#define IRQ17_PKE                IRQ_PKE                /* PKE_IRQ, - pke_irq_handler */
#define IRQ18_UART1              IRQ_UART1              /* UART1_IRQ, - uart1_irq_handler */
#define IRQ19_UART0              IRQ_UART0              /* UART0_IRQ, - uart0_irq_handler */
#define IRQ20_DFIFO              IRQ_DFIFO              /* DFIFO_IRQ: audio dma fifo interrupt, - audio_irq_handler */
#define IRQ21_I2C                IRQ_I2C                /* I2C_IRQ, - i2c_irq_handler */
#define IRQ22_SPI_AHB            IRQ_SPI_AHB            /* HSPI_IRQ, - hspi_irq_handler */
#define IRQ23_SPI_APB            IRQ_SPI_APB            /* PSPI_IRQ, - pspi_irq_handler */
#define IRQ24_USB_PWDN           IRQ_USB_PWDN           /* USB_PWDN_IRQ: USB suspend interrupt, - usb_pwdn_irq_handler */
#define IRQ25_GPIO               IRQ_GPIO               /* GPIO_IRQ, - gpio_irq_handler */
#define IRQ26_GPIO2RISC0         IRQ_GPIO2RISC0         /* GPIO2RISC[0]_IRQ, - gpio_risc0_irq_handler */
#define IRQ27_GPIO2RISC1         IRQ_GPIO2RISC1         /* GPIO2RISC[1]_IRQ, - gpio_risc1_irq_handler */
#define IRQ28_SOFT               IRQ_SOFT               /* SOFT_IRQ: software interrupt, - soft_irq_handler */
#define IRQ29_NPE_BUS0           IRQ_NPE_BUS0           /* NPE_BUS0_IRQ, - npe_bus0_irq_handler */
#define IRQ30_NPE_BUS1           IRQ_NPE_BUS1           /* NPE_BUS1_IRQ, - npe_bus1_irq_handler */
#define IRQ31_NPE_BUS2           IRQ_NPE_BUS2           /* NPE_BUS2_IRQ, - npe_bus2_irq_handler */
#define IRQ32_NPE_BUS3           IRQ_NPE_BUS3           /* NPE_BUS3_IRQ, - npe_bus3_irq_handler */
#define IRQ33_NPE_BUS4           IRQ_NPE_BUS4           /* NPE_BUS4_IRQ, - npe_bus4_irq_handler */
#define IRQ34_USB_250US          IRQ_USB_250US          /* USB_250US_SOF_IRQ: USB 250us or SOF interrupt, - usb_250us_irq_handler */
#define IRQ35_USB_RESET          IRQ_USB_RESET          /* USB_RESET_IRQ: USB reset interrupt, - usb_reset_irq_handler */
#define IRQ36_NPE_BUS7           IRQ_NPE_BUS7           /* NPE_BUS7_IRQ, - npe_bus7_irq_handler */
#define IRQ37_NPE_BUS8           IRQ_NPE_BUS8           /* NPE_BUS8_IRQ, - npe_bus8_irq_handler */

#define IRQ42_NPE_BUS13          IRQ_NPE_BUS13          /* NPE_BUS13_IRQ, - npe_bus13_irq_handler */
#define IRQ43_NPE_BUS14          IRQ_NPE_BUS14          /* NPE_BUS14_IRQ, - npe_bus14_irq_handler */
#define IRQ44_NPE_BUS15          IRQ_NPE_BUS15          /* NPE_BUS15_IRQ, - npe_bus15_irq_handler */

#define IRQ46_NPE_BUS17          IRQ_NPE_BUS17          /* NPE_BUS17_IRQ, - npe_bus17_irq_handler */

#define IRQ50_NPE_BUS21          IRQ_NPE_BUS21          /* NPE_BUS21_IRQ, - npe_bus21_irq_handler */
#define IRQ51_NPE_BUS22          IRQ_NPE_BUS22          /* NPE_BUS22_IRQ, - npe_bus22_irq_handler */
#define IRQ52_NPE_BUS23          IRQ_NPE_BUS23          /* NPE_BUS23_IRQ, - npe_bus23_irq_handler */
#define IRQ53_NPE_BUS24          IRQ_NPE_BUS24          /* NPE_BUS24_IRQ, - npe_bus24_irq_handler */
#define IRQ54_NPE_BUS25          IRQ_NPE_BUS25          /* NPE_BUS25_IRQ, - npe_bus25_irq_handler */
#define IRQ55_NPE_BUS26          IRQ_NPE_BUS26          /* NPE_BUS26_IRQ, - npe_bus26_irq_handler */
#define IRQ56_NPE_BUS27          IRQ_NPE_BUS27          /* NPE_BUS27_IRQ, - npe_bus27_irq_handler */
#define IRQ57_NPE_BUS28          IRQ_NPE_BUS28          /* NPE_BUS28_IRQ, - npe_bus28_irq_handler */
#define IRQ58_NPE_BUS29          IRQ_NPE_BUS29          /* NPE_BUS29_IRQ, - npe_bus29_irq_handler */
#define IRQ59_NPE_BUS30          IRQ_NPE_BUS30          /* NPE_BUS30_IRQ, - npe_bus30_irq_handler */
#define IRQ60_NPE_BUS31          IRQ_NPE_BUS31          /* NPE_BUS31_IRQ, - npe_bus31_irq_handler */
#define IRQ61_NPE_COMB           IRQ_NPE_COMB           /* NPE_COMB_IRQ, - npe_comb_irq_handler */
#define IRQ62_PM_TM              IRQ_PM_TM              /* PM_IRQ, - pm_irq_handler */
#define IRQ63_EOC                IRQ_EOC                /* EOC_IRQ, - eoc_irq_handler */
/**
 * @}
 */

/**
 * @brief This macro generates a unique name for an interrupt service routine (ISR) entry point based on the input parameter. \n
 * The generated name is in the form "entry_irq" followed by the value of "irq_num". \n
 * For example, ISR_ENTRY_NAME(1) means entry_irq1.
 * @param[in] irq_num - The number of the interrupt see @ref irq_source_index.
 */
#define ISR_ENTRY_NAME(irq_num) entry_irq##irq_num

/**
 * @brief Interrupt service routine register, this macro defines an entry_irq function based on irq_num. \n
 *        The machine attribute was added to the interrupt function, and the compiler saw that it would insert code that modified the protection register, for example:
 *        @code {.c}
 *           _attribute_ram_code_sec_noinline_ entry_irq1(void) __attribute__ ((interrupt ("machine") , aligned(4)));
 *        @endcode
 * @param[in] isr - Interrupt service routine pointer.
 * @param[in] irq_num - The number of the interrupt see @ref irq_source_index.
 * @note
 *      - The same interruption number can only be registered once.
 *      - The code in the interrupt service handler should be as simple as possible, and if you can do it without function calls, you can reduce the number of registers on the stack, \n
 *        thus reducing the code size and speeding up the interrupt response.
 */
#define PLIC_ISR_REGISTER(isr, irq_num)                                                                                     \
    _attribute_ram_code_sec_noinline_ void ISR_ENTRY_NAME(irq_num)(void) __attribute__((interrupt("machine"), aligned(4))); \
    void                                   ISR_ENTRY_NAME(irq_num)(void)                                                    \
    {                                                                                                                       \
        plic_isr(isr, irq_num);                                                                                             \
    }

/**
 * @brief interrupt source priority. the larger the value, the higher the priority. default priority value is 1.
 * @note  an interrupt occurs only when priority is greater than threshold
 */
typedef enum
{
    IRQ_PRI_LEV0, /**< 0 indicates that no interrupt is generated. */
    IRQ_PRI_LEV1,
    IRQ_PRI_LEV2,
    IRQ_PRI_LEV3,
} irq_priority_e;

/**
 * @brief target interrupt priority threshold. the larger the value, the higher the threshold. default threshold value is 0.
 */
typedef enum
{
    IRQ_PRI_NUM0,
    IRQ_PRI_NUM1,
    IRQ_PRI_NUM2,
    IRQ_PRI_NUM3,
} irq_threshold_e;

/**
 * @brief    this function serves to set plic feature.
 * @param[in]   feature - preemptive priority interrupt feature and the vector mode.
 * @return  none
 */
static inline void plic_set_feature(feature_e feature)
{
    reg_irq_feature = feature; /* set preempt and vector mode */
}

/**
 * @brief    This function serves to enable preemptive priority interrupt feature.
 *              - These three types of interrupts occur simultaneously, they are handled under the following order:M-mode external interrupt(MEI) > M-mode software interrupt(MSI) > M-mode timer interrupt(MTI).
 *              - MEI, MSI and MTI do not occur simultaneously, they can be nested within each other. This function is used to enable interrupt nesting and configure the interrupt nesting priority at which MEI can be interrupted by MSI and MTI.
 *              - The priority of the MEI interrupts is managed by the PLIC and is configured using the plic_set_priority() interface.
 * @param[in]   preempt_pri - MEI, MSI, and MTI interrupt priorities that do not occur simultaneously, configure the interrupt nesting priority at which MEI can be interrupted by MSI and MTI.
 * @return   none
 */
static inline void plic_preempt_feature_en(core_preempt_pri_e preempt_pri)
{
    reg_irq_feature |= FLD_FEATURE_PREEMPT_PRIORITY_INT_EN;
    g_plic_preempt_en = preempt_pri;
}

/**
 * @brief    this function serves to disable preemptive priority interrupt feature.
 * @return  none
 */
static inline void plic_preempt_feature_dis(void)
{
    reg_irq_feature &= (~FLD_FEATURE_PREEMPT_PRIORITY_INT_EN);
    g_plic_preempt_en = 0;
}

/**
 * @brief    this function serves to set plic pending. its provide a way for software to trigger an interrupt without relying on external devices interrupt source.
 * @param[in]  src - interrupt source see @ref irq_source_index.
 * @return  none
 */
static inline void plic_set_pending(unsigned int src)
{
    reg_irq_pending(src) = BIT(src % 32);
}

/**
 * @brief    This function serves to set external interrupt priority threshold,only active interrupts with priorities strictly greater than the threshold will cause interrupt, the default threshold value is 0.
 * @param[in]   threshold -  Threshold level.
 * @return  none
 * @note    This interface cannot be called from the interrupt service routine. \n
 *          If this interface is called in an interrupt service routine, the hardware has threshold in-stack and out-stack behaviors, \n
 *          the out-stack overwrites the set threshold value.
 */
static inline void plic_set_threshold(irq_threshold_e threshold)
{
    /*the priority number in the threshold register will be saved to a preempted priority stack and the new priority number of the claimed interrupt will be written to the threshold register.
when the mcu sends an interrupt completion message to the PLIC (plic_interrupt_complete()), the PLIC will restore(Hardware automatic operation) the highest priority number in the preempted priority stack back to the priority threshold register.
it is possible that when hardware and software write threshold registers at the same time, there is a risk of software write failure, so turn off the global interrupt before software writes, and resume after writing*/
    unsigned int r    = core_interrupt_disable();
    reg_irq_threshold = threshold;
    fence_iorw; /* Hardware may change this value, fence IO ensures that software changes are valid. */
    core_restore_interrupt(r);
}

/**
 * @brief    this function serves to set external interrupts preemptive priority level,The priority value 0 is reserved to mean "never interrupt".
 * the larger the priority value, the higher the interrupt priority, default priority value is 1.
 * @param[in]   src - interrupt source see @ref irq_source_index.
 * @param[in]   priority - priority level.
 * @return  none
 * @note    This interface cannot be called from the interrupt service routine. \n
 *          If this interface is called in an interrupt service routine, the priority = 0 is changed at this point, and if it is done before the claim, \n
 *          claim accesses the PLIC and gets no value and the interrupt is considered not to have occurred.
 */
static inline void plic_set_priority(unsigned int src, irq_priority_e priority)
{
    reg_irq_src_priority(src) = priority;
}

/**
 * @brief    this function serves to enable plic interrupt source.
 * @param[in]   src - interrupt source see @ref irq_source_index.
 * @return  none
 * @note    This interface cannot be called from the interrupt service routine.
 */
static inline void plic_interrupt_enable(unsigned int src)
{
    reg_irq_src(src) |= BIT(src % 32);
}

/**
 * @brief    this function serves to disable plic interrupt source.
 * @param[in]   src - interrupt source see @ref irq_source_index.
 * @return  none
 * @note    This interface cannot be called from the interrupt service routine. \n
 *          If this interface is called in an interrupt service routine, the CPU claims the ID and then closes the interrupt, complete will not be completed and the threshold value will not be updated, \n
 *          all subsequent interrupts of the same priority to be stuck.
 */
static inline void plic_interrupt_disable(unsigned int src)
{
    reg_irq_src(src) &= (~BIT(src % 32));
}

/**
 * @brief    This function serves to send a complete interrupt message(by software) to the gateway to allow a new interrupt request when current interrupt is done.
 * @param[in] src - Interrupt source see @ref irq_source_index.
 * @return  none
 * @note
 *          - plic_interrupt_claim() and plic_interrupt_complete() must be used in pairs.
 */
static inline void plic_interrupt_complete(unsigned int src)
{
    reg_irq_done = src;
}

/**
 * @brief    This function serves to get claimed interrupt id.
 * @return   It returns the claimed interrupt source id see @ref irq_source_index.
 * @note
 *          - For vector interrupt, the hardware will automatically claim an interrupt, in general, the software does not need to claim.
 *          - plic_interrupt_claim() and plic_interrupt_complete() must be used in pairs.
 *          - If the corresponding interrupt has never been enabled, claim does not get the corresponding interrupt ID, and the interface returns 0.
 */
static inline unsigned int plic_interrupt_claim(void)
{
    /**
     *   - Each claim can only claim one IP corresponding to the ID, more than one IP, need to claim multiple times.
     *   - The claim will be successful only after the IP signal reaches the EIP (if IE is closed all the time, the IP will not reach the EIP).
     */
    return reg_irq_done;
}

/**
 * @brief       This function serves to secure code by reconfiguring interrupt threshold or mstatus.MIE to enter the critical section, such as calling some flash interface to enter some function process.
 * @param[in]   preempt_en
 *                - 1 means interrupt priority larger than the given threshold which can disturb function process.
 *                - 0 means it can't be disturbed by interrupt, global interrupt(mstatus.MIE) will be disabled.
 * @param[in]   threshold
 *                - PLIC interrupt threshold. When the interrupt priority is greater than the maximum of the given threshold and the threshold before calling the interface, the function process will be disturbed by interrupt.
 * @return
 *                - When preempt_en = 1 and interrupt nesting is supported, the interrupt threshold before calling the interface is returned.
 *                - Return the value of the mstatus.MIE bit in other cases.
 * @note    plic_enter_critical_sec and plic_exit_critical_sec must be used in pairs.
 */
_attribute_ram_code_sec_noinline_ unsigned int plic_enter_critical_sec(unsigned char preempt_en, unsigned char threshold);


/**
 * @brief    This function serves to restore interrupt threshold or mstatus.MIE to exit the critical section, such as calling some flash interface to exit some function.
 * @param[in]   preempt_en
 *                - 1 means it needs to restore the value of interrupt threshold.
 *                - 0 means it needs to restore mstatus.MIE.It must be the same as the preempt_en value passed by the plic_enter_critical_sec function.
 * @param[in]   r
 *                 - The value of mstatus.MIE or threshold to restore when exit critical section, it must be the value returned by the plic_enter_critical_sec function.
 * @return  none
 */
_attribute_ram_code_sec_noinline_ void plic_exit_critical_sec(unsigned char preempt_en, unsigned int r);

/**
 * @brief       this function serves to execute the interrupt service function. you can call this function when an interrupt occurs.
 * @param[in]   func - interrupt service function.
 * @param[in]   src - interrupt source see @ref irq_source_index.
 * @return      none
 */
_attribute_ram_code_sec_ void plic_isr(func_isr_t func, unsigned int src);

/**
 * @brief   This function serves to clear all PLIC request When global interrupt is disabled.
 * @return
 *          - 1 indicates that all PLIC interrupt requests were cleared successfully.
 *          - 0 indicates failure to clear all PLIC interrupt requests, failure of interrupt clearing indicates that the interrupt source keeps triggering in one of two ways:
                -# The corresponding interrupt status is not cleared.
                -# The level that triggers the interrupt is always present.
 * @note
 *          - When global interrupt is disabled, the application needs to call this interface to ensure that all PLIC interrupt requests have been processed.
 *          - When global interrupt is enabled, the application does not need to call this interface, the hardware and interrupt service routine handle interrupt requests.
 */
_attribute_ram_code_sec_ int plic_clr_all_request(void);

/**
 * @brief      This function serves to save and disable all PLIC interrupt sources.
 * @return     none
 * @note       plic_all_interrupt_save_and_disable and plic_all_interrupt_restore must be used in pairs.
 */
void plic_all_interrupt_save_and_disable(void);

/**
 * @brief      This function serves to restore and all PLIC interrupt sources.
 * @return     none
 * @note       plic_all_interrupt_save_and_disable and plic_all_interrupt_restore must be used in pairs.
 */
void plic_all_interrupt_restore(void);

/**
 * @brief      This function serves to save and disable interrupts, including the MIE bit of the MSTATUS register(depends on the first parameter,regardless of whether the global interrupt is enabled or not.), \n
 *             MSIE, MTIE, and MEIE bits of the MIE register, and all PLIC interrupt sources.
 * @param[in]  flag - Global interrupt disable flag, 1: disable, 0: not disable.
 * @param[in]  mie - MSIE, MTIE, and MEIE Which one wakes up WFI.
 * @note
 *  - When the core is awoken by the taken interrupt and global interrupts enable, it will resume and start to execute from the corresponding interrupt service routine.
 *  - When the core is awoken by the pending interrupt and global interrupts disable, it will resume and start to execute from the instruction after the WFI instruction.
 *  - plic_irqs_preprocess_for_wfi and plic_irqs_postprocess_for_wfi must be used in pairs.
 */
void plic_irqs_preprocess_for_wfi(unsigned char flag, mie_e mie);

/**
 * @brief      This function serves to restore interrupts, including the MIE bit of the MSTATUS register, \n
 *             MSIE, MTIE, and MEIE bits of the MIE register, and all PLIC interrupt sources.
 * @return     none
 * @note
 *  - Make sure that the status flag of the corresponding interrupt is cleared before calling this interface.
 *  - This function will enable PLIC interrupt so this interface cannot be called from the interrupt service routine.
 *  - plic_irqs_preprocess_for_wfi and plic_irqs_postprocess_for_wfi must be used in pairs.
 */
void plic_irqs_postprocess_for_wfi(void);

#endif
