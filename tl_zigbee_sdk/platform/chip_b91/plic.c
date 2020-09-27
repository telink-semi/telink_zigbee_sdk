/********************************************************************************************************
 * @file	plic.c
 *
 * @brief	This is the source file for B91
 *
 * @author	D.M.H
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

#include "compiler.h"
#include "core.h"
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
__attribute__((section(".ram_code"))) void default_irq_handler(void){ }

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
			asm("nop");
		}
	}
}

_attribute_ram_code_sec_noinline_ void trap_entry(void) __attribute__ ((interrupt ("machine") , aligned(4)));
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
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		stimer_irq_handler();
		plic_interrupt_complete(IRQ1_SYSTIMER);
		core_restore_nested_context();
	}
	else
	{
		stimer_irq_handler();
		plic_interrupt_complete(IRQ1_SYSTIMER);
	}

}

/**
 * @brief analog interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq2(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq2(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		analog_irq_handler();
		plic_interrupt_complete(IRQ2_ALG);
		core_restore_nested_context();
	}
	else
	{
		analog_irq_handler();
		plic_interrupt_complete(IRQ2_ALG);
	}

}


/**
 * @brief timer1 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq3(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq3(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		timer1_irq_handler();
		plic_interrupt_complete(IRQ3_TIMER1);
		core_restore_nested_context();
	}
	else
	{
		timer1_irq_handler();
		plic_interrupt_complete(IRQ3_TIMER1);
	}
}

/**
 * @brief timer0 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq4(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq4(void)
{

	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		timer0_irq_handler();
		plic_interrupt_complete(IRQ4_TIMER0);
		core_restore_nested_context();
	}
	else
	{
		timer0_irq_handler();
		plic_interrupt_complete(IRQ4_TIMER0);
	}
}


/**
 * @brief dma interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq5(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq5(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		dma_irq_handler();
		plic_interrupt_complete(IRQ5_DMA);
		core_restore_nested_context();
	}
	else
	{
		dma_irq_handler();
		plic_interrupt_complete(IRQ5_DMA);
	}

}

/**
 * @brief bmc interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq6(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq6(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		bmc_irq_handler();
		plic_interrupt_complete(IRQ6_BMC);
		core_restore_nested_context();
	}
	else
	{
		bmc_irq_handler();
		plic_interrupt_complete(IRQ6_BMC);
	}

}


/**
 * @brief usb control endpoint setup interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq7(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq7(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		usb_ctrl_ep_setup_irq_handler();
		plic_interrupt_complete(IRQ7_USB_CTRL_EP_SETUP);
		core_restore_nested_context();
	}
	else
	{
		usb_ctrl_ep_setup_irq_handler();
		plic_interrupt_complete(IRQ7_USB_CTRL_EP_SETUP);
	}


}

/**
 * @brief usb control endpoint data interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq8(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq8(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		usb_ctrl_ep_data_irq_handler();
		plic_interrupt_complete(IRQ8_USB_CTRL_EP_DATA);
		core_restore_nested_context();
	}
	else
	{
		usb_ctrl_ep_data_irq_handler();
		plic_interrupt_complete(IRQ8_USB_CTRL_EP_DATA);
	}

}

/**
 * @brief usb control endpoint status interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq9(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq9(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		usb_ctrl_ep_status_irq_handler();
		plic_interrupt_complete(IRQ9_USB_CTRL_EP_STATUS);
		core_restore_nested_context();
	}
	else
	{
		usb_ctrl_ep_status_irq_handler();
		plic_interrupt_complete(IRQ9_USB_CTRL_EP_STATUS);
	}
}


/**
 * @brief usb control endpoint setinf interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq10(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq10(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		usb_ctrl_ep_setinf_irq_handler();
		plic_interrupt_complete(IRQ10_USB_CTRL_EP_SETINF);
		core_restore_nested_context();
	}
	else
	{
		usb_ctrl_ep_setinf_irq_handler();
		plic_interrupt_complete(IRQ10_USB_CTRL_EP_SETINF);
	}
}

/**
 * @brief usb endpoint interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq11(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq11(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		usb_endpoint_irq_handler ();
		plic_interrupt_complete(IRQ11_USB_ENDPOINT);
		core_restore_nested_context();
	}
	else
	{
		usb_endpoint_irq_handler ();
		plic_interrupt_complete(IRQ11_USB_ENDPOINT);
	}

}

/**
 * @brief rf dual mode interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq12(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq12(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		rf_dm_irq_handler();
		plic_interrupt_complete(IRQ12_ZB_DM);
		core_restore_nested_context();
	}
	else
	{
		rf_dm_irq_handler();
		plic_interrupt_complete(IRQ12_ZB_DM);
	}

}

/**
 * @brief rf ble interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq13(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq13(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		rf_ble_irq_handler();
		plic_interrupt_complete(IRQ13_ZB_BLE);
		core_restore_nested_context();
	}
	else
	{
		rf_ble_irq_handler();
		plic_interrupt_complete(IRQ13_ZB_BLE);
	}

}


/**
 * @brief rf BT  interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq14(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq14(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		rf_bt_irq_handler();
		plic_interrupt_complete(IRQ14_ZB_BT);
		core_restore_nested_context();
	}
	else
	{
		rf_bt_irq_handler();
		plic_interrupt_complete(IRQ14_ZB_BT);
	}

}

/**
 * @brief telink rf interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq15(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq15(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		rf_irq_handler();
		plic_interrupt_complete(IRQ15_ZB_RT);
		core_restore_nested_context();
	}
	else
	{
		rf_irq_handler();
		plic_interrupt_complete(IRQ15_ZB_RT);
	}

}



/**
 * @brief pwm interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq16(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq16(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		pwm_irq_handler();
		plic_interrupt_complete(IRQ16_PWM);
		core_restore_nested_context();
	}
	else
	{
		pwm_irq_handler();
		plic_interrupt_complete(IRQ16_PWM);
	}

}

/**
 * @brief pke interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq17(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq17(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		pke_irq_handler();
		plic_interrupt_complete(IRQ17_PKE);
		core_restore_nested_context();
	}
	else
	{
		pke_irq_handler();
		plic_interrupt_complete(IRQ17_PKE);
	}

}



/**
 * @brief uart1 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq18(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq18(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		uart1_irq_handler();
		plic_interrupt_complete(IRQ18_UART1);
		core_restore_nested_context();
	}
	else
	{
		uart1_irq_handler();
		plic_interrupt_complete(IRQ18_UART1);
	}

}



/**
 * @brief uart0 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq19(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq19(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		uart0_irq_handler();
		plic_interrupt_complete(IRQ19_UART0);
		core_restore_nested_context();
	}
	else
	{
		uart0_irq_handler();
		plic_interrupt_complete(IRQ19_UART0);
	}

}


/**
 * @brief audio interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq20(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq20(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		audio_irq_handler();
		plic_interrupt_complete(IRQ20_DFIFO);
		core_restore_nested_context();
	}
	else
	{
		audio_irq_handler();
		plic_interrupt_complete(IRQ20_DFIFO);
	}

}

/**
 * @brief i2c interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq21(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq21(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		i2c_irq_handler();
		plic_interrupt_complete(IRQ21_I2C);
		core_restore_nested_context();
	}
	else
	{
		i2c_irq_handler();
		plic_interrupt_complete(IRQ21_I2C);
	}

}


/**
 * @brief hspi interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq22(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq22(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		hspi_irq_handler();
		plic_interrupt_complete(IRQ22_SPI_AHB);
		core_restore_nested_context();
	}
	else
	{
		hspi_irq_handler();
		plic_interrupt_complete(IRQ22_SPI_AHB);
	}

}


/**
 * @brief pspi interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq23(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq23(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		pspi_irq_handler();
		plic_interrupt_complete(IRQ23_SPI_APB);
		core_restore_nested_context();
	}
	else
	{
		pspi_irq_handler();
		plic_interrupt_complete(IRQ23_SPI_APB);
	}
}


/**
 * @brief usb power down interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq24(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq24(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		usb_pwdn_irq_handler();
		plic_interrupt_complete(IRQ24_USB_PWDN);
		core_restore_nested_context();
	}
	else
	{
		usb_pwdn_irq_handler();
		plic_interrupt_complete(IRQ24_USB_PWDN);
	}

}

/**
 * @brief gpio interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq25(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq25(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		gpio_irq_handler();
		plic_interrupt_complete(IRQ25_GPIO);
		core_restore_nested_context();
	}
	else
	{
		gpio_irq_handler();
		plic_interrupt_complete(IRQ25_GPIO);
	}

}

/**
 * @brief gpio_risc0 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq26(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq26(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		gpio_risc0_irq_handler();
		plic_interrupt_complete(IRQ26_GPIO2RISC0);
		core_restore_nested_context();
	}
	else
	{
		gpio_risc0_irq_handler();
		plic_interrupt_complete(IRQ26_GPIO2RISC0);
	}

}


/**
 * @brief gpio_risc1 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq27(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq27(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		gpio_risc1_irq_handler();
		plic_interrupt_complete(IRQ27_GPIO2RISC1);
		core_restore_nested_context();
	}
	else
	{
		gpio_risc1_irq_handler();
		plic_interrupt_complete(IRQ27_GPIO2RISC1);
	}

}

/**
 * @brief soft interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq28(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq28(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		soft_irq_handler();
		plic_interrupt_complete(IRQ28_SOFT);
		core_restore_nested_context();
	}
	else
	{
		soft_irq_handler();
		plic_interrupt_complete(IRQ28_SOFT);
	}

}

/**
 * @brief npe bus0 interrupt handler.
 * @return none
 */

_attribute_ram_code_sec_noinline_ void  entry_irq29(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq29(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus0_irq_handler();
		plic_interrupt_complete(IRQ29_NPE_BUS0);
		core_restore_nested_context();
	}
	else
	{
		npe_bus0_irq_handler();
		plic_interrupt_complete(IRQ29_NPE_BUS0);
	}

}
/**
 * @brief npe bus1 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq30(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq30(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus1_irq_handler();
		plic_interrupt_complete(IRQ30_NPE_BUS1);
		core_restore_nested_context();
	}
	else
	{
		npe_bus1_irq_handler();
		plic_interrupt_complete(IRQ30_NPE_BUS1);
	}

}
/**
 * @brief npe bus2 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq31(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq31(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus2_irq_handler();
		plic_interrupt_complete(IRQ31_NPE_BUS2);
		core_restore_nested_context();
	}
	else
	{
		npe_bus2_irq_handler();
		plic_interrupt_complete(IRQ31_NPE_BUS2);
	}
}
/**
 * @brief npe bus3 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq32(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq32(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus3_irq_handler();
		plic_interrupt_complete(IRQ32_NPE_BUS3);
		core_restore_nested_context();
	}
	else
	{
		npe_bus3_irq_handler();
		plic_interrupt_complete(IRQ32_NPE_BUS3);
	}
}

/**
 * @brief npe bus4 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq33(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq33(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus4_irq_handler();
		plic_interrupt_complete(IRQ33_NPE_BUS4);
		core_restore_nested_context();
	}
	else
	{
		npe_bus4_irq_handler();
		plic_interrupt_complete(IRQ33_NPE_BUS4);
	}
}
/**
 * @brief usb 250us interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq34(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq34(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		usb_250us_irq_handler();
		plic_interrupt_complete(IRQ34_USB_250US);
		core_restore_nested_context();
	}
	else
	{
		usb_250us_irq_handler();
		plic_interrupt_complete(IRQ34_USB_250US);
	}

}
/**
 * @brief usb reset interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq35(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq35(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		usb_reset_irq_handler();
		plic_interrupt_complete(IRQ35_USB_RESET);
		core_restore_nested_context();
	}
	else
	{
		usb_reset_irq_handler();
		plic_interrupt_complete(IRQ35_USB_RESET);
	}

}
/**
 * @brief npe bus7 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq36(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq36(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus7_irq_handler();
		plic_interrupt_complete(IRQ36_NPE_BUS7);
		core_restore_nested_context();
	}
	else
	{
		npe_bus7_irq_handler();
		plic_interrupt_complete(IRQ36_NPE_BUS7);
	}

}
/**
 * @brief npe bus8 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq37(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq37(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus8_irq_handler();
		plic_interrupt_complete(IRQ37_NPE_BUS8);
		core_restore_nested_context();
	}
	else
	{
		npe_bus8_irq_handler();
		plic_interrupt_complete(IRQ37_NPE_BUS8);
	}
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
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus13_irq_handler();
		plic_interrupt_complete(IRQ42_NPE_BUS13);
		core_restore_nested_context();
	}
	else
	{
		npe_bus13_irq_handler();
		plic_interrupt_complete(IRQ42_NPE_BUS13);
	}

}
/**
 * @brief npe bus14 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq43(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq43(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus14_irq_handler();
		plic_interrupt_complete(IRQ43_NPE_BUS14);
		core_restore_nested_context();
	}
	else
	{
		npe_bus14_irq_handler();
		plic_interrupt_complete(IRQ43_NPE_BUS14);
	}

}

/**
 * @brief npe bus15 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq44(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq44(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus15_irq_handler();
		plic_interrupt_complete(IRQ44_NPE_BUS15);
		core_restore_nested_context();
	}
	else
	{
		npe_bus15_irq_handler();
		plic_interrupt_complete(IRQ44_NPE_BUS15);
	}

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
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus17_irq_handler();
		plic_interrupt_complete(IRQ46_NPE_BUS17);
		core_restore_nested_context();
	}
	else
	{
		npe_bus17_irq_handler();
		plic_interrupt_complete(IRQ46_NPE_BUS17);
	}

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
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus21_irq_handler();
		plic_interrupt_complete(IRQ50_NPE_BUS21);
		core_restore_nested_context();
	}
	else
	{
		npe_bus21_irq_handler();
		plic_interrupt_complete(IRQ50_NPE_BUS21);
	}

}
/**
 * @brief npe bus22 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq51(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq51(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus22_irq_handler();
		plic_interrupt_complete(IRQ51_NPE_BUS22);
		core_restore_nested_context();
	}
	else
	{
		npe_bus22_irq_handler();
		plic_interrupt_complete(IRQ51_NPE_BUS22);
	}

}
/**
 * @brief npe bus23 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq52(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq52(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus23_irq_handler();
		plic_interrupt_complete(IRQ52_NPE_BUS23);
		core_restore_nested_context();
	}
	else
	{
		npe_bus23_irq_handler();
		plic_interrupt_complete(IRQ52_NPE_BUS23);
	}

}
/**
 * @brief npe bus24 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq53(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq53(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus24_irq_handler();
		plic_interrupt_complete(IRQ53_NPE_BUS24);
		core_restore_nested_context();
	}
	else
	{
		npe_bus24_irq_handler();
		plic_interrupt_complete(IRQ53_NPE_BUS24);
	}

}
/**
 * @brief npe bus25 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq54(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq54(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus25_irq_handler();
		plic_interrupt_complete(IRQ54_NPE_BUS25);
		core_restore_nested_context();
	}
	else
	{
		npe_bus25_irq_handler();
		plic_interrupt_complete(IRQ54_NPE_BUS25);
	}

}
/**
 * @brief npe bus26 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq55(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq55(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus26_irq_handler();
		plic_interrupt_complete(IRQ55_NPE_BUS26);
		core_restore_nested_context();
	}
	else
	{
		npe_bus26_irq_handler();
		plic_interrupt_complete(IRQ55_NPE_BUS26);
	}

}
/**
 * @brief npe bus27 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq56(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq56(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus27_irq_handler();
		plic_interrupt_complete(IRQ56_NPE_BUS27);
		core_restore_nested_context();
	}
	else
	{
		npe_bus27_irq_handler();
		plic_interrupt_complete(IRQ56_NPE_BUS27);
	}

}
/**
 * @brief npe bus28 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq57(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq57(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus28_irq_handler();
		plic_interrupt_complete(IRQ57_NPE_BUS28);
		core_restore_nested_context();
	}
	else
	{
		npe_bus28_irq_handler();
		plic_interrupt_complete(IRQ57_NPE_BUS28);
	}

}
/**
 * @brief npe bus29 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq58(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq58(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus29_irq_handler();
		plic_interrupt_complete(IRQ58_NPE_BUS29);
		core_restore_nested_context();
	}
	else
	{
		npe_bus29_irq_handler();
		plic_interrupt_complete(IRQ58_NPE_BUS29);
	}

}
/**
 * @brief npe bus30 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq59(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq59(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus30_irq_handler();
		plic_interrupt_complete(IRQ59_NPE_BUS30);
		core_restore_nested_context();
	}
	else
	{
		npe_bus30_irq_handler();
		plic_interrupt_complete(IRQ59_NPE_BUS30);
	}

}
/**
 * @brief npe bus31 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq60(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq60(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_bus31_irq_handler();
		plic_interrupt_complete(IRQ60_NPE_BUS31);
		core_restore_nested_context();
	}
	else
	{
		npe_bus31_irq_handler();
		plic_interrupt_complete(IRQ60_NPE_BUS31);
	}

}
/**
 * @brief npe comb interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq61(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq61(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		npe_comb_irq_handler();
		plic_interrupt_complete(IRQ61_NPE_COMB);
		core_restore_nested_context();
	}
	else
	{
		npe_comb_irq_handler();
		plic_interrupt_complete(IRQ61_NPE_COMB);
	}

}
/**
 * @brief pm interrupt handler.An interrupt will be generated after wake-up
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq62(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq62(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		pm_irq_handler();
		plic_interrupt_complete(IRQ62_PM_TM);
		core_restore_nested_context();
	}
	else
	{
		pm_irq_handler();
		plic_interrupt_complete(IRQ62_PM_TM);
	}

}
/**
 * @brief eoc interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq63(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq63(void)
{
	if(g_plic_preempt_en)
	{
		core_save_nested_context();
		eoc_irq_handler();
		plic_interrupt_complete(IRQ63_EOC);
		core_restore_nested_context();
	}
	else
	{
		eoc_irq_handler();
		plic_interrupt_complete(IRQ63_EOC);
	}

}



/// @} DRIVERS
