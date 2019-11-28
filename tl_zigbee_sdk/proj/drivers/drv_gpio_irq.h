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

enum gpio_pol{
	poll_rising,
	poll_falling,
};

typedef enum{
	GPIO_IRQ_INVALID,
	GPIO_IRQ_MODE,
	GPIO_IRQ_RISC0_MODE,
	GPIO_IRQ_RISC1_MODE,
}gpio_irq_mode_t;

typedef void (*irq_callback)(void);

/****
* brief: initialize the gpio interrupt
* param[in] pin, the gpio interrupt pin
* param[in] polarity, the falling edge or the rising edge.
* param[in] gpio_irq_callback, the callback function
* @return, 0 success, -1 failed(fail to allocate the node buffer)
*/
int drv_gpio_irq_conf(gpio_irq_mode_t mode, unsigned int pin, enum gpio_pol polarity, irq_callback gpio_irq_callback);

/****
* brief: enable the gpio interrupt
* param[in] pin, the pin of node.
* @return
*/
void drv_gpio_irq_en(unsigned int pin);
void drv_gpio_irq_risc0_en(unsigned int pin);
void drv_gpio_irq_risc1_en(unsigned int pin);

/****
* brief: disable the gpio interrupt
* param[in] pin, the pin of node.
* @return
*/
void drv_gpio_irq_clr(unsigned int pin);

void gpio_irq_handler(gpio_irq_mode_t mode);
