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
#include "drv_gpio_irq.h"
#include "../common/list.h"
#include "../common/string.h"
#include "../os/ev_buffer.h"
#include "platform_includes.h"


typedef struct gpio_conf_list
{
	struct gpio_conf_list 	*next;
	irq_callback 			gpio_irq_callback;
	unsigned int 			pin;
	gpio_irq_mode_t 		mode;
	enum gpio_pol 			polarity;
} gpio_conf_list;

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
int drv_gpio_irq_conf(gpio_irq_mode_t mode, unsigned int pin, enum gpio_pol polarity, irq_callback gpio_irq_callback)
{
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

	if(mode == GPIO_IRQ_MODE){
		reg_irq_src = FLD_IRQ_GPIO_EN;
		gpio_set_interrupt(pin, polarity);
		BM_SET(reg_gpio_wakeup_irq,BIT(3));		//enable gpio irq
		irq_set_mask(FLD_IRQ_GPIO_EN);			//enable gpio interrupt mask
	}else if(mode == GPIO_IRQ_RISC0_MODE){
		reg_irq_src = FLD_IRQ_GPIO_RISC0_EN;
		gpio_set_interrupt_risc0(pin, polarity);
		irq_set_mask(FLD_IRQ_GPIO_RISC0_EN);
	}else if(mode == GPIO_IRQ_RISC1_MODE){
		reg_irq_src = FLD_IRQ_GPIO_RISC1_EN;
		gpio_set_interrupt_risc1(pin, polarity);
		irq_set_mask(FLD_IRQ_GPIO_RISC1_EN);
	}

	return 0;
}

/****
* brief: enable the gpio interrupt
* param[in] pin, the pin of node.
* @return
*/
void drv_gpio_irq_en(unsigned int pin)
{
	gpio_en_interrupt(pin, 1);
}

void drv_gpio_irq_risc0_en(unsigned int pin)
{
	gpio_en_interrupt_risc0(pin, 1);
}

void drv_gpio_irq_risc1_en(unsigned int pin)
{
	gpio_en_interrupt_risc1(pin, 1);
}

/****
* brief: disable the gpio interrupt
* param[in] pin, the pin of node.
* @return
*/
void drv_gpio_irq_clr(unsigned int pin)
{
#if	defined (MCU_CORE_8258)
	gpio_en_interrupt(pin,0);
#else
	gpio_clr_interrupt(pin);
#endif
}


/****
* brief: gpio interrupt handler
* param[in]
* @return
*/
void gpio_irq_handler(gpio_irq_mode_t mode)
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

/****
* brief: gpio interrupt handler
* param[in]
* @return
*/
extern void ev_synchronous_timer(void);
void timer_synchronous_irq_callback(){
	u8 r = irq_disable();
	ev_synchronous_timer();
	irq_restore(r);
}


/****
* brief: configure the timer synchronous gpio and  polarity
* param[in] pin, the pin of node.
* param[in] polarity, the falling edge or the rising edge.
* @return
*/
void gpio_timer_synchronous_set(unsigned int pin,enum gpio_pol polarity)
{
	drv_gpio_irq_conf(GPIO_IRQ_MODE, pin, polarity, timer_synchronous_irq_callback);
}
