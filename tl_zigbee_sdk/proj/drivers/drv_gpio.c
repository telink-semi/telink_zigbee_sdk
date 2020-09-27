/********************************************************************************************************
 * @file     drv_gpio_irq.c
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

#include "../tl_common.h"


typedef struct gpio_conf_list
{
	struct gpio_conf_list 	*next;
	irq_callback 			gpio_irq_callback;
	u32 					pin;
	drv_gpioIrqMode_t 		mode;
	drv_gpioPoll_e 			polarity;
}gpio_conf_list;

#define GPIO_IRQ_HANDLER_Q_NUM		3

LIST(gpioIrqHandlerQ);

struct gpio_conf_list *list_head = NULL;

/****
* brief: initialize the gpio interrupt
* param[in] pin, the gpio interrupt pin
* param[in] polarity, the falling edge or the rising edge.
* param[in] gpio_irq_callback, the callback function
* @return, 0 success, -1 failed(fail to allocate the node buffer)
*/
int drv_gpio_irq_config(drv_gpioIrqMode_t mode, u32 pin, drv_gpioPoll_e polarity, irq_callback gpio_irq_callback)
{
	bool searched = 0;
	if(listLength(gpioIrqHandlerQ)){
		gpio_conf_list *pList = listHead(gpioIrqHandlerQ);
		while(pList){
			if(pList->mode == mode && pList->pin == pin){
				if(pList->polarity != polarity){
					pList->polarity = polarity;
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

		gpio_conf_list *list_node = (gpio_conf_list*)ev_buf_allocate(sizeof(gpio_conf_list));
		if(!list_node){
			return -1;
		}

		memset((u8 *)list_node, 0, sizeof(gpio_conf_list));
		list_node->mode = mode;
		list_node->pin = pin;
		list_node->polarity = polarity;
		list_node->gpio_irq_callback = gpio_irq_callback;
		listAdd(gpioIrqHandlerQ, (void *)list_node);
	}

	if(mode == GPIO_IRQ_MODE){
		drv_gpio_irq_set(pin, polarity);
	}else if(mode == GPIO_IRQ_RISC0_MODE){
		drv_gpio_irq_risc0_set(pin, polarity);
	}else if(mode == GPIO_IRQ_RISC1_MODE){
		drv_gpio_irq_risc1_set(pin, polarity);
	}

	return 0;
}

/****
* brief: gpio interrupt handler
* param[in]
* @return
*/
static void drv_gpio_irq_process(drv_gpioIrqMode_t mode)
{
	if(listLength(gpioIrqHandlerQ)){
		gpio_conf_list *pList = listHead(gpioIrqHandlerQ);
		while(pList){
			if(pList->mode == mode){
				if(pList->gpio_irq_callback){
					pList->gpio_irq_callback();
					break;
				}
			}

			pList = pList->next;
		}
	}
}

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

void drv_gpio_func_set(u32 pin)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_set_func(pin, AS_GPIO);
#elif defined(MCU_CORE_B91)
	gpio_function_en(pin);
#endif
}

void drv_gpio_output_en(u32 pin, bool enable)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_set_output_en(pin, enable);
#elif defined(MCU_CORE_B91)
	gpio_set_output_en(pin, enable);
#endif
}

void drv_gpio_input_en(u32 pin, bool enable)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_set_input_en(pin, enable);
#elif defined(MCU_CORE_B91)
	gpio_set_input_en(pin, enable);
#endif
}

void drv_gpio_up_down_resistor(u32 pin, u8 res)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_setup_up_down_resistor(pin, res);
#elif defined(MCU_CORE_B91)
	gpio_set_up_down_res(pin, res);
#endif
}

void drv_gpio_irq_set(u32 pin, drv_gpioPoll_e polarity)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_set_interrupt(pin, polarity);
#elif defined(MCU_CORE_B91)
	gpio_set_irq(pin, polarity);
#endif
}

void drv_gpio_irq_risc0_set(u32 pin, drv_gpioPoll_e polarity)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_set_interrupt_risc0(pin, polarity);
#elif defined(MCU_CORE_B91)
	gpio_set_gpio2risc0_irq(pin, polarity);
#endif
}

void drv_gpio_irq_risc1_set(u32 pin, drv_gpioPoll_e polarity)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_set_interrupt_risc1(pin, polarity);
#elif defined(MCU_CORE_B91)
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
#elif defined(MCU_CORE_B91)
	gpio_irq_en(pin);
	plic_interrupt_enable(IRQ25_GPIO);
#endif
}

void drv_gpio_irq_risc0_en(u32 pin)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_en_interrupt_risc0(pin, 1);
#elif defined(MCU_CORE_B91)
	gpio_gpio2risc0_irq_en(pin);
	plic_interrupt_enable(IRQ26_GPIO2RISC0);
#endif
}

void drv_gpio_irq_risc1_en(u32 pin)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_en_interrupt_risc1(pin, 1);
#elif defined(MCU_CORE_B91)
	gpio_gpio2risc1_irq_en(pin);
	plic_interrupt_enable(IRQ27_GPIO2RISC1);
#endif
}

void drv_gpio_irq_dis(u32 pin)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_en_interrupt(pin, 0);
#elif defined(MCU_CORE_B91)
	gpio_irq_dis(pin);
	plic_interrupt_disable(IRQ25_GPIO);
#endif
}

void drv_gpio_irq_risc0_dis(u32 pin)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_en_interrupt_risc0(pin, 0);
#elif defined(MCU_CORE_B91)
	gpio_gpio2risc0_irq_dis(pin);
	plic_interrupt_disable(IRQ26_GPIO2RISC0);
#endif
}

void drv_gpio_irq_risc1_dis(u32 pin)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	gpio_en_interrupt_risc1(pin, 0);
#elif defined(MCU_CORE_B91)
	gpio_gpio2risc1_irq_dis(pin);
	plic_interrupt_disable(IRQ27_GPIO2RISC1);
#endif
}


#if 0
/****
* brief: gpio interrupt handler
* param[in]
* @return
*/
extern void ev_synchronous_timer(void);
void timer_synchronous_irq_callback()
{
	u32 r = disable_irq();
	ev_synchronous_timer();
	restore_irq(r);
}


/****
* brief: configure the timer synchronous gpio and  polarity
* param[in] pin, the pin of node.
* param[in] polarity, the falling edge or the rising edge.
* @return
*/
void gpio_timer_synchronous_set(u32 pin, drv_gpioPoll_e polarity)
{
	drv_gpio_irq_config(GPIO_IRQ_MODE, pin, polarity, timer_synchronous_irq_callback);
}
#endif
