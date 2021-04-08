/********************************************************************************************************
 * @file	irq_handler.c
 *
 * @brief	This is the source file of irq_handler for b85m
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

#if (__PROJECT_TL_SNIFFER__)
extern void irq_sniffer_handler(void);
#endif
extern void rf_rx_irq_handler(void);
extern void rf_tx_irq_handler(void);

volatile u8 T_DBG_irqTest[16] = {0};

//called by irq in cstartup.s
_attribute_ram_code_ void irq_handler(void){
#if (__PROJECT_TL_BOOT_LOADER__)

#elif (__PROJECT_TL_SNIFFER__)
	irq_sniffer_handler();
#else
	u16 src_rf = rf_irq_src_get();
	if(src_rf & FLD_RF_IRQ_TX){
		rf_irq_clr_src(FLD_RF_IRQ_TX);
		T_DBG_irqTest[0]++;
		rf_tx_irq_handler();
	}

	if(src_rf & FLD_RF_IRQ_RX){
		rf_irq_clr_src(FLD_RF_IRQ_RX);
		T_DBG_irqTest[1]++;
		rf_rx_irq_handler();
	}

	u32 src = irq_get_src();

	if((src & FLD_IRQ_TMR0_EN)){
		reg_irq_src = FLD_IRQ_TMR0_EN;
		reg_tmr_sta = FLD_TMR_STA_TMR0;
		T_DBG_irqTest[2]++;
		drv_timer_irq0_handler();
	}

	if((src & FLD_IRQ_TMR1_EN)){
		reg_irq_src = FLD_IRQ_TMR1_EN;
		reg_tmr_sta = FLD_TMR_STA_TMR1;
		T_DBG_irqTest[3]++;
		drv_timer_irq1_handler();
	}

    if((src & FLD_IRQ_SYSTEM_TIMER)){
    	reg_irq_src = FLD_IRQ_SYSTEM_TIMER;
    	T_DBG_irqTest[4]++;
    	drv_timer_irq3_handler();
    }

	if((src & FLD_IRQ_GPIO_EN)==FLD_IRQ_GPIO_EN){
		reg_irq_src = FLD_IRQ_GPIO_EN;
		T_DBG_irqTest[5]++;
		drv_gpio_irq_handler();
	}

	if((src & FLD_IRQ_GPIO_RISC0_EN)==FLD_IRQ_GPIO_RISC0_EN){
		reg_irq_src = FLD_IRQ_GPIO_RISC0_EN;
		T_DBG_irqTest[6]++;
		drv_gpio_irq_risc0_handler();
	}

	if((src & FLD_IRQ_GPIO_RISC1_EN)==FLD_IRQ_GPIO_RISC1_EN){
		reg_irq_src = FLD_IRQ_GPIO_RISC1_EN;
		T_DBG_irqTest[7]++;
		drv_gpio_irq_risc1_handler();
	}

    u16 dma_irq_source = dma_chn_irq_status_get();

	if(dma_irq_source & FLD_DMA_CHN_UART_RX){
		dma_chn_irq_status_clr(FLD_DMA_CHN_UART_RX);
		T_DBG_irqTest[8]++;
		drv_uart_rx_irq_handler();
	}else if(dma_irq_source & FLD_DMA_CHN_UART_TX){
		dma_chn_irq_status_clr(FLD_DMA_CHN_UART_TX);
		T_DBG_irqTest[9]++;
		drv_uart_tx_irq_handler();
	}else{
		dma_chn_irq_status_clr(~(FLD_DMA_CHN_UART_TX | FLD_DMA_CHN_UART_RX));
	}
#endif
}

