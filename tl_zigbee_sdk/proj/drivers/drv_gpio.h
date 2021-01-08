/********************************************************************************************************
 * @file	drv_gpio.h
 *
 * @brief	This is the header file for drv_gpio
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
#pragma once



typedef enum{
	RISING_EDGE,
	FALLING_EDGE,
	HIGH_LEVEL,	/* Only for b91 platform. */
	LOW_LEVEL,	/* Only for b91 platform. */
}drv_gpioPoll_e;

typedef enum{
	GPIO_IRQ_INVALID,
	GPIO_IRQ_MODE,
	GPIO_IRQ_RISC0_MODE,
	GPIO_IRQ_RISC1_MODE,
}drv_gpioIrqMode_t;

typedef void (*irq_callback)(void);

/****
* brief: initialize the gpio interrupt
* param[in] pin, the gpio interrupt pin
* param[in] polarity, the falling edge or the rising edge.
* param[in] gpio_irq_callback, the callback function
* @return, 0 success, -1 failed(fail to allocate the node buffer)
*/
int drv_gpio_irq_config(drv_gpioIrqMode_t mode, u32 pin, drv_gpioPoll_e polarity, irq_callback gpio_irq_callback);

void drv_gpio_irq_handler(void);
void drv_gpio_irq_risc0_handler(void);
void drv_gpio_irq_risc1_handler(void);

void drv_gpio_irq_set(u32 pin, drv_gpioPoll_e polarity);
void drv_gpio_irq_risc0_set(u32 pin, drv_gpioPoll_e polarity);
void drv_gpio_irq_risc1_set(u32 pin, drv_gpioPoll_e polarity);

void drv_gpio_irq_en(u32 pin);
void drv_gpio_irq_risc0_en(u32 pin);
void drv_gpio_irq_risc1_en(u32 pin);

void drv_gpio_irq_dis(u32 pin);
void drv_gpio_irq_risc0_dis(u32 pin);
void drv_gpio_irq_risc1_dis(u32 pin);

void drv_gpio_func_set(u32 pin);
void drv_gpio_output_en(u32 pin, bool enable);
void drv_gpio_input_en(u32 pin, bool enable);
void drv_gpio_up_down_resistor(u32 pin, u8 res);
void drv_gpio_write(u32 pin, bool value);
bool drv_gpio_read(u32 pin);
void drv_gpio_read_all(u8 *p);
