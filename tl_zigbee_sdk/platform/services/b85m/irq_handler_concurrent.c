/********************************************************************************************************
 * @file    irq_handler.c
 *
 * @brief   This is the source file of irq_handler for b85m
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "tl_common.h"
#include "zigbee_ble_switch.h"

extern void rf_rx_irq_handler(void);
extern void rf_tx_irq_handler(void);
extern void switch_to_ble_context(void);
extern bool zb_rfSwitchAllow(void);
extern void irq_blt_sdk_handler(void);

volatile u8 T_DBG_irqTest[16] = {0};
volatile u8 T_irqExceptCnt[2] = {0};

//called by irq in cstartup.s
_attribute_ram_code_ void irq_handler(void)
{
    u32 src = reg_irq_src;
    u32 msk = reg_irq_mask;

    u16 src_rf = rf_irq_src_get();

    if (CURRENT_SLOT_GET() == DUALMODE_SLOT_ZIGBEE) {
        /* first check the BLE sync event(system timer interrupt, just only be used by ble stack),
         * and then check if the zigbee phy layer is busy
         * if it isn't busy, clear rf interrupt status and switch to BLE directly
         * if it's busy, need to wait till the zigbee tx event is finish */
        if (src & FLD_IRQ_SYSTEM_TIMER) {
            /*
             * when ble is idle/adv, the mask "FLD_IRQ_SYSTEM_TIMER" is disable,
             * but the timer capture value is still valid,
             * so the status will be triggered once the tick counter reach this capture value
             *
             */
            T_DBG_irqTest[0]++;
            if (msk & FLD_IRQ_SYSTEM_TIMER) {
                if (zb_rfSwitchAllow()) {
                    src_rf = 0;    //clear it to skip the rf_tx_irq_handler/rf_rx_irq_handler

                    /* need switch to ble mode */
                    switch_to_ble_context();

                    irq_blt_sdk_handler();  //process rf and system timer interrupt

                    T_irqExceptCnt[0]++;
                } else {
                    T_irqExceptCnt[1]++;
                }
            } else {
                reg_irq_src = FLD_IRQ_SYSTEM_TIMER;
            }
        }

        if (src_rf & FLD_RF_IRQ_TX) {
            rf_irq_clr_src(FLD_RF_IRQ_TX);
            T_DBG_irqTest[10]++;
            rf_tx_irq_handler();
        }

        if (src_rf & FLD_RF_IRQ_RX) {
            rf_irq_clr_src(FLD_RF_IRQ_RX);
            T_DBG_irqTest[11]++;
            rf_rx_irq_handler();
        }
    } else {
    	T_DBG_irqTest[12]++;
        irq_blt_sdk_handler();
    }

    if ((src & FLD_IRQ_TMR0_EN)) {
        reg_irq_src = FLD_IRQ_TMR0_EN;
        reg_tmr_sta = FLD_TMR_STA_TMR0;
        T_DBG_irqTest[2]++;
        drv_timer_irq0_handler();
    }

    if ((src & FLD_IRQ_TMR1_EN)) {
        reg_irq_src = FLD_IRQ_TMR1_EN;
        reg_tmr_sta = FLD_TMR_STA_TMR1;
        T_DBG_irqTest[3]++;
        drv_timer_irq1_handler();
    }

    if ((src & FLD_IRQ_GPIO_EN) == FLD_IRQ_GPIO_EN) {
        reg_irq_src = FLD_IRQ_GPIO_EN;
        T_DBG_irqTest[5]++;
        drv_gpio_irq_handler();
    }

    if ((src & FLD_IRQ_GPIO_RISC0_EN) == FLD_IRQ_GPIO_RISC0_EN) {
        reg_irq_src = FLD_IRQ_GPIO_RISC0_EN;
        T_DBG_irqTest[6]++;
        drv_gpio_irq_risc0_handler();
    }

    if ((src & FLD_IRQ_GPIO_RISC1_EN) == FLD_IRQ_GPIO_RISC1_EN) {
        reg_irq_src = FLD_IRQ_GPIO_RISC1_EN;
        T_DBG_irqTest[7]++;
        drv_gpio_irq_risc1_handler();
    }

    u16 dma_irq_source = dma_chn_irq_status_get();

    if (dma_irq_source & FLD_DMA_CHN_UART_RX) {
        dma_chn_irq_status_clr(FLD_DMA_CHN_UART_RX);
        T_DBG_irqTest[8]++;
        drv_uart_rx_irq_handler();
    } else if (dma_irq_source & FLD_DMA_CHN_UART_TX) {
        dma_chn_irq_status_clr(FLD_DMA_CHN_UART_TX);
        T_DBG_irqTest[9]++;
        drv_uart_tx_irq_handler();
    } else {
        dma_chn_irq_status_clr(~(FLD_DMA_CHN_UART_TX | FLD_DMA_CHN_UART_RX));
    }
}
