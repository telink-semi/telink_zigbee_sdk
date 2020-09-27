/********************************************************************************************************
 * @file     drv_gpio_irq.h
 *
 * @brief	 gpio interrupt driver interface file
 *
 * @author
 * @date     Oct. 8, 2016
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
#pragma once



typedef enum{
	poll_rising,
	poll_falling,

	RISING_EDGE = poll_rising,
	FALLING_EDGE = poll_falling,
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
