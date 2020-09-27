/********************************************************************************************************
 * @file     rf_irq_handler.c
 *
 * @brief    Interface of hardware timer
 *
 * @author
 * @date     July 8, 2020
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
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
		T_DBG_testIrq[1]++;
		rf_rx_irq_handler();
	}else if(rf_get_irq_status(FLD_RF_IRQ_TX)){
		T_DBG_testIrq[2]++;
		rf_tx_irq_handler();
	}else{
		T_DBG_testIrq[3]++;
		rf_clr_irq_status(0xffff);
	}
}

void timer0_irq_handler(void)
{
	if(reg_tmr_sta & FLD_TMR_STA_TMR0){
		drv_timer_irq0_handler();
	}
}

void timer1_irq_handler(void)
{
	if(reg_tmr_sta & FLD_TMR_STA_TMR1){
		drv_timer_irq1_handler();
	}
}

void stimer_irq_handler(void)
{
	if(stimer_get_irq_status(FLD_SYSTEM_IRQ)){
		T_DBG_testIrq[4]++;
		drv_timer_irq3_handler();
	}
}


void gpio_irq_handler(void)
{
	T_DBG_testIrq[5]++;
	reg_gpio_irq_clr = FLD_GPIO_IRQ_CLR;
	drv_gpio_irq_handler();
}

void gpio_risc0_irq_handler(void)
{
	T_DBG_testIrq[6]++;
	reg_gpio_irq_clr = FLD_GPIO_IRQ_GPIO2RISC0_CLR;
	drv_gpio_irq_risc0_handler();
}

void gpio_risc1_irq_handler(void)
{
	T_DBG_testIrq[7]++;
	reg_gpio_irq_clr = FLD_GPIO_IRQ_GPIO2RISC1_CLR;
	drv_gpio_irq_risc1_handler();
}
