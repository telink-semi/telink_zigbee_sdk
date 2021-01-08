/********************************************************************************************************
 * @file	plic.c
 *
 * @brief	This is the source file for B91
 *
 * @author	Driver Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#include "plic.h"
 unsigned char g_plic_preempt_en=0;

/**
 * @brief    This function serves to config plic when enter some function process such as flash.
 * @param[in]   preempt_en - 1 can disturb by interrupt, 0 can't disturb by interrupt.
 * @param[in]   threshold  - interrupt threshold.when the interrupt priority> interrupt threshold,the function process will be disturb by interrupt.
 * @return  none
*/
_attribute_ram_code_sec_noinline_ unsigned int plic_enter_critical_sec(unsigned char preempt_en ,unsigned char threshold)
{
	unsigned int r;
	if(g_plic_preempt_en&&preempt_en)
	{
		plic_set_threshold(threshold);
		r=0;
	}
	else
	{
	   r = core_interrupt_disable();
	}
	return r ;
}

/**
 * @brief    This function serves to config plic when exit some function process such as flash.
 * @param[in]   preempt_en - 1 can disturb by interrupt, 0 can disturb by interrupt.
 * @param[in]    r         - the value of mie register to restore.
 * @return  none
*/
_attribute_ram_code_sec_noinline_ void  plic_exit_critical_sec(unsigned char preempt_en ,unsigned int r)
{
	if (g_plic_preempt_en&&preempt_en)
	{
		plic_set_threshold(0);
	}
	else
	{
		core_restore_interrupt(r);
	}
}

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

 __attribute__((section(".ram_code"))) void default_irq_handler(void)
{

}
void stimer_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void analog_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void timer1_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void timer0_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void dma_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void bmc_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void usb_ctrl_ep_setup_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void usb_ctrl_ep_data_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void usb_ctrl_ep_status_irq_handler(void)  __attribute__((weak, alias("default_irq_handler")));
void usb_ctrl_ep_setinf_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void usb_endpoint_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void rf_dm_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void rf_ble_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void rf_bt_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void rf_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void pwm_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void pke_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void uart1_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void uart0_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void audio_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void i2c_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void hspi_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void pspi_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void usb_pwdn_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio_risc0_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio_risc1_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void soft_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void npe_bus0_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus1_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus2_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus3_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus4_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void usb_250us_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void usb_reset_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void npe_bus7_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus8_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void npe_bus13_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus14_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void npe_bus15_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus17_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void npe_bus21_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus22_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus23_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus24_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus25_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus26_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus27_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus28_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void npe_bus29_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus30_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus31_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_comb_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void pm_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void eoc_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

typedef void (*func_isr_t) (void);
_attribute_ram_code_sec_ void plic_isr(func_isr_t func, irq_source_e src)
{
	  if(g_plic_preempt_en)
		{
		    core_save_nested_context();//save csr and  Enable interrupt enable
			func ();//irq handler
			core_restore_nested_context();//  restore csr and disable interrupt enable
			plic_interrupt_complete(src);//complete interrupt
			fence_iorw;//fence instruction
		}
		else
		{
			func ();//irq handler
			plic_interrupt_complete(src);//complete interrupt
		}
}

/**
 * @brief  exception handler.this defines an exception handler to handle all the platform pre-defined exceptions.
 * @return none
 */
_attribute_ram_code_sec_ __attribute__((weak)) void except_handler()
{
	while(1){
		/* Unhandled Trap */
		for(volatile unsigned int i = 0; i < 0xffff; i++)
		{
			__asm__("nop");
		}
	}
}
_attribute_ram_code_sec_noinline_  void trap_entry(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void trap_entry(void)
{
	except_handler();
}

/**
 * @brief system timer interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq1(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq1(void)
{
	plic_isr(stimer_irq_handler,IRQ1_SYSTIMER);
}

/**
 * @brief analog interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq2(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq2(void)
{
	plic_isr(analog_irq_handler,IRQ2_ALG);
}


/**
 * @brief timer1 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq3(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq3(void)
{
	plic_isr(timer1_irq_handler,IRQ3_TIMER1);
}

/**
 * @brief timer0 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq4(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq4(void)
{
	plic_isr(timer0_irq_handler,IRQ4_TIMER0);
}


/**
 * @brief dma interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq5(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq5(void)
{
	plic_isr(dma_irq_handler,IRQ5_DMA);
}

/**
 * @brief bmc interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq6(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq6(void)
{
	plic_isr(bmc_irq_handler,IRQ6_BMC);
}


/**
 * @brief usb control endpoint setup interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq7(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq7(void)
{
	plic_isr(usb_ctrl_ep_setup_irq_handler,IRQ7_USB_CTRL_EP_SETUP);
}

/**
 * @brief usb control endpoint data interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq8(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq8(void)
{
	plic_isr(usb_ctrl_ep_data_irq_handler,IRQ8_USB_CTRL_EP_DATA);
}

/**
 * @brief usb control endpoint status interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq9(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq9(void)
{
	plic_isr(usb_ctrl_ep_status_irq_handler,IRQ9_USB_CTRL_EP_STATUS);
}


/**
 * @brief usb control endpoint setinf interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq10(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq10(void)
{
	plic_isr(usb_ctrl_ep_setinf_irq_handler,IRQ10_USB_CTRL_EP_SETINF);
}

/**
 * @brief usb endpoint interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq11(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq11(void)
{
	plic_isr(usb_endpoint_irq_handler,IRQ11_USB_ENDPOINT);
}

/**
 * @brief rf dual mode interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq12(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq12(void)
{
	plic_isr(rf_dm_irq_handler,IRQ12_ZB_DM);
}

/**
 * @brief rf ble interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq13(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq13(void)
{
	plic_isr(rf_ble_irq_handler,IRQ13_ZB_BLE);
}


/**
 * @brief rf BT  interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq14(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq14(void)
{
	plic_isr(rf_bt_irq_handler,IRQ14_ZB_BT);
}

/**
 * @brief telink rf interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq15(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq15(void)
{
	plic_isr(rf_irq_handler,IRQ15_ZB_RT);
}



/**
 * @brief pwm interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq16(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq16(void)
{
	plic_isr(pwm_irq_handler,IRQ16_PWM);
}

/**
 * @brief pke interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq17(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq17(void)
{
	plic_isr(pke_irq_handler,IRQ17_PKE);
}



/**
 * @brief uart1 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq18(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq18(void)
{
	plic_isr(uart1_irq_handler,IRQ18_UART1);
}



/**
 * @brief uart0 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq19(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq19(void)
{
	plic_isr(uart0_irq_handler,IRQ19_UART0);
}


/**
 * @brief audio interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq20(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq20(void)
{
	plic_isr(audio_irq_handler,IRQ20_DFIFO);
}

/**
 * @brief i2c interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq21(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq21(void)
{
	plic_isr(i2c_irq_handler,IRQ21_I2C);
}


/**
 * @brief hspi interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq22(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq22(void)
{
	plic_isr(hspi_irq_handler,IRQ22_SPI_AHB);
}


/**
 * @brief pspi interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq23(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq23(void)
{
	plic_isr(pspi_irq_handler,IRQ23_SPI_APB);

}

/**
 * @brief usb power down interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq24(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq24(void)
{
	plic_isr(usb_pwdn_irq_handler,IRQ24_USB_PWDN);
}

/**
 * @brief gpio interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq25(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq25(void)
{
	plic_isr(gpio_irq_handler,IRQ25_GPIO);
}

/**
 * @brief gpio_risc0 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq26(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq26(void)
{
	plic_isr(gpio_risc0_irq_handler,IRQ26_GPIO2RISC0);
}


/**
 * @brief gpio_risc1 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq27(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq27(void)
{
	plic_isr(gpio_risc1_irq_handler,IRQ27_GPIO2RISC1);
}

/**
 * @brief soft interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq28(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq28(void)
{
	plic_isr(soft_irq_handler,IRQ28_SOFT);
}

/**
 * @brief npe bus0 interrupt handler.
 * @return none
 */

_attribute_ram_code_sec_noinline_ void  entry_irq29(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq29(void)
{
	plic_isr(npe_bus0_irq_handler,IRQ29_NPE_BUS0);
}
/**
 * @brief npe bus1 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq30(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq30(void)
{
	plic_isr(npe_bus1_irq_handler,IRQ30_NPE_BUS1);
}
/**
 * @brief npe bus2 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq31(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq31(void)
{
	plic_isr(npe_bus2_irq_handler,IRQ31_NPE_BUS2);
}
/**
 * @brief npe bus3 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq32(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq32(void)
{
	plic_isr(npe_bus3_irq_handler,IRQ32_NPE_BUS3);
}

/**
 * @brief npe bus4 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq33(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq33(void)
{
	plic_isr(npe_bus4_irq_handler,IRQ33_NPE_BUS4);
}
/**
 * @brief usb 250us interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq34(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq34(void)
{
	plic_isr(usb_250us_irq_handler,IRQ34_USB_250US);
}
/**
 * @brief usb reset interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq35(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq35(void)
{
	plic_isr(usb_reset_irq_handler,IRQ35_USB_RESET);
}
/**
 * @brief npe bus7 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq36(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq36(void)
{
	plic_isr(npe_bus7_irq_handler,IRQ36_NPE_BUS7);
}
/**
 * @brief npe bus8 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq37(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq37(void)
{
	plic_isr(npe_bus8_irq_handler,IRQ37_NPE_BUS8);
}


_attribute_ram_code_sec_noinline_ void  entry_irq38(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq38(void)
{

}
_attribute_ram_code_sec_noinline_ void  entry_irq39(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq39(void)
{

}
_attribute_ram_code_sec_noinline_ void  entry_irq40(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq40(void)
{

}
_attribute_ram_code_sec_noinline_ void  entry_irq41(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq41(void)
{

}
/**
 * @brief npe bus13 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq42(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq42(void)
{
	plic_isr(npe_bus13_irq_handler,IRQ42_NPE_BUS13);
}
/**
 * @brief npe bus14 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq43(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq43(void)
{
	plic_isr(npe_bus14_irq_handler,IRQ43_NPE_BUS14);
}

/**
 * @brief npe bus15 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq44(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq44(void)
{
	plic_isr(npe_bus15_irq_handler,IRQ44_NPE_BUS15);
}
_attribute_ram_code_sec_noinline_ void  entry_irq45(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq45(void)
{

}
/**
 * @brief npe bus17 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq46(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq46(void)
{
	plic_isr(npe_bus17_irq_handler,IRQ46_NPE_BUS17);
}


_attribute_ram_code_sec_noinline_ void  entry_irq47(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq47(void)
{

}

_attribute_ram_code_sec_noinline_ void  entry_irq48(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq48(void)
{

}

_attribute_ram_code_sec_noinline_ void  entry_irq49(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq49(void)
{

}
/**
 * @brief npe bus21 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq50(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq50(void)
{
	plic_isr(npe_bus21_irq_handler,IRQ50_NPE_BUS21);
}
/**
 * @brief npe bus22 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq51(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq51(void)
{
	plic_isr(npe_bus22_irq_handler,IRQ51_NPE_BUS22);
}
/**
 * @brief npe bus23 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq52(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq52(void)
{
	plic_isr(npe_bus23_irq_handler,IRQ52_NPE_BUS23);
}
/**
 * @brief npe bus24 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq53(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq53(void)
{
	plic_isr(npe_bus24_irq_handler,IRQ53_NPE_BUS24);
}
/**
 * @brief npe bus25 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq54(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq54(void)
{
	plic_isr(npe_bus25_irq_handler,IRQ54_NPE_BUS25);
}
/**
 * @brief npe bus26 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq55(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq55(void)
{
	plic_isr(npe_bus26_irq_handler,IRQ55_NPE_BUS26);
}
/**
 * @brief npe bus27 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq56(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq56(void)
{
	plic_isr(npe_bus27_irq_handler,IRQ56_NPE_BUS27);
}
/**
 * @brief npe bus28 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq57(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq57(void)
{
	plic_isr(npe_bus28_irq_handler,IRQ57_NPE_BUS28);
}
/**
 * @brief npe bus29 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq58(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq58(void)
{
	plic_isr(npe_bus29_irq_handler,IRQ58_NPE_BUS29);
}
/**
 * @brief npe bus30 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq59(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq59(void)
{
	plic_isr(npe_bus30_irq_handler,IRQ59_NPE_BUS30);
}
/**
 * @brief npe bus31 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq60(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq60(void)
{
	plic_isr(npe_bus31_irq_handler,IRQ60_NPE_BUS31);
}
/**
 * @brief npe comb interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq61(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq61(void)
{
	plic_isr(npe_comb_irq_handler,IRQ61_NPE_COMB);
}
/**
 * @brief pm interrupt handler.An interrupt will be generated after wake-up
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq62(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq62(void)
{
	plic_isr(pm_irq_handler,IRQ62_PM_TM);
}
/**
 * @brief eoc interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq63(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq63(void)
{
	plic_isr(eoc_irq_handler,IRQ63_EOC);
}



/// @} DRIVERS
