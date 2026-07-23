/********************************************************************************************************
 * @file    lpc.c
 *
 * @brief   This is the source file for TL321X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "lpc.h"
#include "lib/include/plic.h"
#include "lib/include/pm/pm.h"
#include "gpio.h"
#include "lib/include/stimer.h"
/**
 * @brief       This function selects input reference voltage for low power comparator.
 * @param[in]   mode    - lower power comparator working mode includes normal mode and low power mode.
 * @param[in]   ref     - selected input reference voltage.
 * @return      none.
 */
void lpc_set_input_ref(lpc_mode_e mode, lpc_reference_e ref)
{
    if (mode == LPC_LOWPOWER) {
        //switch uvlo vref
        analog_write_reg8(0x0b, analog_read_reg8(0x0b) & 0xf7);
        analog_write_reg8(0x0d, analog_read_reg8(0x0d) | 0x80);
    } else if (mode == LPC_NORMAL) {
        //switch bg vref
        analog_write_reg8(0x0b, analog_read_reg8(0x0b) | 0x08);
        analog_write_reg8(0x0d, analog_read_reg8(0x0d) & 0x7f);
    }

    analog_write_reg8(0x0d, (analog_read_reg8(0x0d) & 0x8f) | (ref << 4));
}
/**
 * @brief       This function is used to initialize GPIO voltage detection.
 * @param[in]   mode    - lower power comparator working mode includes normal mode and low power mode.
 * @param[in]   pin     - selected input channel.Input derived from external PortB(PB<1>~PB<7>).
 * @param[in]   ref     - selected input reference voltage.
 * @param[in]   divider - selected scaling coefficient.(%25,%50,%75,%100)
 * @return      none.
 */
void lpc_gpio_vol_detect_init(lpc_mode_e mode, lpc_input_channel_e pin, lpc_reference_e ref,lpc_scaling_e divider)
{
    lpc_set_input_chn(pin);
    lpc_set_input_ref(mode,ref);
    lpc_set_scaling_coeff(divider);
}
/**
 * @brief       This function serves to protect the flash during the chip power-down process.
 * @param[in]   pin  - selected input channel.Input derived from external PortB(PB<1>~PB<7>).
 * @return      none.
 * @note       -# In order to improve the robustness of the chip during high-speed operation, the low power comparator (LPC) is used to
 *              protect the flash during power-down of the chip when the main frequency CCLK is running above 48MHz (excluding 48MHz).
 *              When this feature is enabled, there are the following limitations:
 *               -# The chip power supply voltage is limited to 2.1V to 4.2V.
 *               -# One of PB[1:7] must be reserved for this feature.
 *               -# Interrupt preemption must be enabled and the LPC interrupt will be set as the only highest priority interrupt that can interrupt any process.
 *               -# LPC interrupt priority(IRQ_PM_LVL) > flash operation priority > other interrupt priority.
 *               -# lpc_flash_prot_config() and core_interrupt_enable() should be called as early as possible to activate flash power-down protection.This maximizes the duration of flash protection.
 */
void lpc_flash_prot_config(lpc_input_channel_e chn)
{
    /**
     * The priority of flash operations has been set to the IRQ_PRI_LEV2 by default in flash.c.​
     */
    plic_set_priority(IRQ_PM_LVL,3);//Setting the interrupt priority of LPC(PM) to highest IRQ_PRI_LEV3.
    plic_preempt_feature_en(CORE_PREEMPT_PRI_MODE0);//enable preemptive priority interrupt feature.

    gpio_function_en(GPIO_GROUPB | (1<<chn));
    gpio_set_up_down_res(GPIO_GROUPB | (1<<chn), GPIO_PIN_UP_DOWN_FLOAT);//must be configured as float to prevent being pulled down in gpio_shutdown
    gpio_output_en(GPIO_GROUPB | (1<<chn));    //enable output
    gpio_input_dis(GPIO_GROUPB | (1<<chn));    //disable input
    gpio_set_level(GPIO_GROUPB | (1<<chn), 1);


    lpc_set_input_chn(chn);
    lpc_set_input_ref(LPC_NORMAL, LPC_REF_974MV);
    lpc_set_scaling_coeff(LPC_SCALING_PER50);
    lpc_power_on();
    delay_us(64);
    pm_set_wakeup_src(PM_WAKEUP_COMPARATOR);
    pm_clr_irq_status(FLD_WAKEUP_STATUS_COMPARATOR);
    plic_interrupt_enable(IRQ_PM_LVL);
}
