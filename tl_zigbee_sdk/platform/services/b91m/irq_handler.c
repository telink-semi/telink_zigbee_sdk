/********************************************************************************************************
 * @file	irq_handler.c
 *
 * @brief	This is the source file of irq_handler for b91m
 *
 * @author	Zigbee Group
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
#include "tl_common.h"

extern void rf_rx_irq_handler(void);
extern void rf_tx_irq_handler(void);



volatile u8 T_DBG_testIrq[16] = {0};

void rf_irq_handler(void)
{
	T_DBG_testIrq[0]++;
	if(rf_get_irq_status(FLD_RF_IRQ_RX)){
		rf_clr_irq_status(FLD_RF_IRQ_RX);
		T_DBG_testIrq[1]++;
		rf_rx_irq_handler();
	}else if(rf_get_irq_status(FLD_RF_IRQ_TX)){
		rf_clr_irq_status(FLD_RF_IRQ_TX);
		T_DBG_testIrq[2]++;
		rf_tx_irq_handler();
	}else{
		T_DBG_testIrq[3]++;
		rf_clr_irq_status(0xffff);
	}
}

void timer0_irq_handler(void)
{
	if(timer_get_irq_status(TMR_STA_TMR0)){
		timer_clr_irq_status(TMR_STA_TMR0);
		drv_timer_irq0_handler();
	}
}

void timer1_irq_handler(void)
{
	if(timer_get_irq_status(TMR_STA_TMR1)){
		timer_clr_irq_status(TMR_STA_TMR1);
		drv_timer_irq1_handler();
	}
}

void stimer_irq_handler(void)
{
	if(stimer_get_irq_status(FLD_SYSTEM_IRQ)){
		stimer_clr_irq_status(FLD_SYSTEM_IRQ);
		T_DBG_testIrq[4]++;
		drv_timer_irq3_handler();
	}
}

void gpio_irq_handler(void)
{
	T_DBG_testIrq[5]++;
	gpio_clr_irq_status(FLD_GPIO_IRQ_CLR);
	drv_gpio_irq_handler();
}

void gpio_risc0_irq_handler(void)
{
	T_DBG_testIrq[6]++;
	gpio_clr_irq_status(FLD_GPIO_IRQ_GPIO2RISC0_CLR);
	drv_gpio_irq_risc0_handler();
}

void gpio_risc1_irq_handler(void)
{
	T_DBG_testIrq[7]++;
	gpio_clr_irq_status(FLD_GPIO_IRQ_GPIO2RISC1_CLR);
	drv_gpio_irq_risc1_handler();
}

void uart0_irq_handler(void)
{
	if(uart_get_irq_status(UART0, UART_TXDONE)){
		T_DBG_testIrq[8]++;
		drv_uart_tx_irq_handler();
	}

	if(uart_get_irq_status(UART0, UART_RXDONE)){
		T_DBG_testIrq[9]++;
		drv_uart_rx_irq_handler();
	}
}
