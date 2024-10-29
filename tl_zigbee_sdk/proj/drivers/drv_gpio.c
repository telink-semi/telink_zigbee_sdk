/********************************************************************************************************
 * @file    drv_gpio.c
 *
 * @brief   This is the source file for drv_gpio
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

#include "../tl_common.h"


typedef struct gpio_conf_list
{
	struct gpio_conf_list 	*next;
	irq_callback 			gpio_irq_callback;
	u32 					pin;
	u8 						mode;		//drv_gpioIrqMode_e
	u8 						polarity;	//drv_gpioPoll_e
}gpio_conf_list;

#define GPIO_IRQ_HANDLER_Q_NUM		3

LIST(gpioIrqHandlerQ);

struct gpio_conf_list *list_head = NULL;

/****
* brief: initialize the gpio interrupt
* param[in] mode, the gpio interrupt mode
* param[in] pin, the gpio interrupt pin
* param[in] polarity, the falling edge or the rising edge.
* param[in] gpio_irq_callback, the callback function
* @return, 0 success, -1 failed(fail to allocate the node buffer)
*/
int drv_gpio_irq_config(drv_gpioIrqMode_e mode, u32 pin, drv_gpioPoll_e polarity, irq_callback gpio_irq_callback)
{
	bool searched = 0;
	if(listLength(gpioIrqHandlerQ)){
		gpio_conf_list *pList = listHead(gpioIrqHandlerQ);
		while(pList){
			if(pList->mode == (u8)mode && pList->pin == pin){
				if(pList->polarity != (u8)polarity){
					pList->polarity = (u8)polarity;
				}
				if(pList->gpio_irq_callback != gpio_irq_callback){
					pList->gpio_irq_callback = gpio_irq_callback;
				}

				searched = 1;
				break;
			}

			pList = pList->next;
		}
	}

	if(!searched){
		if(listLength(gpioIrqHandlerQ) >= GPIO_IRQ_HANDLER_Q_NUM){
			return -1;
		}

		gpio_conf_list *list_node = (gpio_conf_list *)ev_buf_allocate(sizeof(gpio_conf_list));
		if(!list_node){
			return -1;
		}

		memset((u8 *)list_node, 0, sizeof(gpio_conf_list));
		list_node->mode = (u8)mode;
		list_node->pin = pin;
		list_node->polarity = (u8)polarity;
		list_node->gpio_irq_callback = gpio_irq_callback;
		listAdd(gpioIrqHandlerQ, (void *)list_node);
	}

#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278) || \
	defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X)
	if(mode == GPIO_IRQ_MODE){
		drv_gpio_irq_set(pin, polarity);
	}else if(mode == GPIO_IRQ_RISC0_MODE){
		drv_gpio_irq_risc0_set(pin, polarity);
	}else if(mode == GPIO_IRQ_RISC1_MODE){
		drv_gpio_irq_risc1_set(pin, polarity);
	}
#elif defined(MCU_CORE_TL321X)
	drv_gpio_irq_set(pin, polarity);
#endif

	return 0;
}

/****
* brief: gpio interrupt handler
* param[in]
* @return
*/
static void drv_gpio_irq_process(drv_gpioIrqMode_e mode)
{
	if(listLength(gpioIrqHandlerQ)){
		gpio_conf_list *pList = listHead(gpioIrqHandlerQ);
		while(pList){
			if(pList->mode == (u8)mode){
				if(pList->gpio_irq_callback){
					pList->gpio_irq_callback();
					break;
				}
			}

			pList = pList->next;
		}
	}
}

#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278) || \
	defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X)
void drv_gpio_irq_handler(void)
{
	drv_gpio_irq_process(GPIO_IRQ_MODE);
}

void drv_gpio_irq_risc0_handler(void)
{
	drv_gpio_irq_process(GPIO_IRQ_RISC0_MODE);
}

void drv_gpio_irq_risc1_handler(void)
{
	drv_gpio_irq_process(GPIO_IRQ_RISC1_MODE);
}
#elif defined(MCU_CORE_TL321X)
void drv_gpio_irq_handler(gpio_irq_num_e irq)
{
	drv_gpio_irq_process(irq);
}
#endif

void drv_gpio_func_set(u32 pin)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_set_func(pin, AS_GPIO);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
	gpio_function_en(pin);
#endif
}

void drv_gpio_output_en(u32 pin, bool enable)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_set_output_en(pin, enable);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
	gpio_set_output(pin, enable);
#endif
}

void drv_gpio_input_en(u32 pin, bool enable)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_set_input_en(pin, enable);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
	gpio_set_input(pin, enable);
#endif
}

void drv_gpio_up_down_resistor(u32 pin, u8 res)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_setup_up_down_resistor(pin, res);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
	gpio_set_up_down_res(pin, res);
#endif
}

void drv_gpio_write(u32 pin, bool value)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_write(pin, value);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
	gpio_set_level(pin, value);
#endif
}

bool drv_gpio_read(u32 pin)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	return gpio_read(pin);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
	return gpio_get_level(pin);
#else
	return 0;
#endif
}

void drv_gpio_read_all(u8 *p)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_read_all(p);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X)
	gpio_get_level_all(p);
#endif
}

void drv_gpio_irq_set(u32 pin, drv_gpioPoll_e polarity)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_set_interrupt(pin, polarity);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X)
	gpio_set_irq(pin, polarity);
#elif defined(MCU_CORE_TL321X)
	if(listLength(gpioIrqHandlerQ)){
		gpio_conf_list *pList = listHead(gpioIrqHandlerQ);
		while(pList){
			if(pList->pin == pin){
				gpio_set_irq(pList->mode, pin, polarity);
				break;
			}
			pList = pList->next;
		}
	}
#endif
}

void drv_gpio_irq_risc0_set(u32 pin, drv_gpioPoll_e polarity)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_set_interrupt_risc0(pin, polarity);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X)
	gpio_set_gpio2risc0_irq(pin, polarity);
#endif
}

void drv_gpio_irq_risc1_set(u32 pin, drv_gpioPoll_e polarity)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_set_interrupt_risc1(pin, polarity);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X)
	gpio_set_gpio2risc1_irq(pin, polarity);
#endif
}

/****
* brief: enable the gpio interrupt
* param[in] pin, the pin of node.
* @return
*/
void drv_gpio_irq_en(u32 pin)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_en_interrupt(pin, 1);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92)
	plic_interrupt_enable(IRQ25_GPIO);
#elif defined(MCU_CORE_TL721X)
	plic_interrupt_enable(IRQ_GPIO);
#elif defined(MCU_CORE_TL321X)
	if(listLength(gpioIrqHandlerQ)){
		gpio_conf_list *pList = listHead(gpioIrqHandlerQ);
		while(pList){
			if(pList->pin == pin){
				gpio_set_irq_mask(BIT(pList->mode));
				plic_interrupt_enable(IRQ_GPIO_IRQ0 + pList->mode);
				break;
			}
			pList = pList->next;
		}
	}
#endif
}

void drv_gpio_irq_risc0_en(u32 pin)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_en_interrupt_risc0(pin, 1);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92)
	plic_interrupt_enable(IRQ26_GPIO2RISC0);
#elif defined(MCU_CORE_TL721X)
	plic_interrupt_enable(IRQ_GPIO2RISC0);
#endif
}

void drv_gpio_irq_risc1_en(u32 pin)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_en_interrupt_risc1(pin, 1);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92)
	plic_interrupt_enable(IRQ27_GPIO2RISC1);
#elif defined(MCU_CORE_TL721X)
	plic_interrupt_enable(IRQ_GPIO2RISC1);
#endif
}

void drv_gpio_irq_dis(u32 pin)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_en_interrupt(pin, 0);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92)
	plic_interrupt_disable(IRQ25_GPIO);
#elif defined(MCU_CORE_TL721X)
	plic_interrupt_disable(IRQ_GPIO);
#elif defined(MCU_CORE_TL321X)
	if(listLength(gpioIrqHandlerQ)){
		gpio_conf_list *pList = listHead(gpioIrqHandlerQ);
		while(pList){
			if(pList->pin == pin){
				gpio_clr_irq_mask(BIT(pList->mode));
				plic_interrupt_disable(IRQ_GPIO_IRQ0 + pList->mode);
				break;
			}
			pList = pList->next;
		}
	}
#endif
}

void drv_gpio_irq_risc0_dis(u32 pin)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_en_interrupt_risc0(pin, 0);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92)
	plic_interrupt_disable(IRQ26_GPIO2RISC0);
#elif defined(MCU_CORE_TL721X)
	plic_interrupt_disable(IRQ_GPIO2RISC0);
#endif
}

void drv_gpio_irq_risc1_dis(u32 pin)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_en_interrupt_risc1(pin, 0);
#elif defined(MCU_CORE_B91) || defined(MCU_CORE_B92)
	plic_interrupt_disable(IRQ27_GPIO2RISC1);
#elif defined(MCU_CORE_TL721X)
	plic_interrupt_disable(IRQ_GPIO2RISC1);
#endif
}
