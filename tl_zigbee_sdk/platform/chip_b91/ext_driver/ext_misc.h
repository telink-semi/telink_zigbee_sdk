/********************************************************************************************************
 * @file     ext_misc.h
 *
 * @brief   This is the header file for B91
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#ifndef DRIVERS_B91_EXT_MISC_H_
#define DRIVERS_B91_EXT_MISC_H_

//#include "nds_intrinsic.h"

#include "../analog.h"
#include "../dma.h"
#include "../gpio.h"
#include "lib/include/pm.h"
#include "../timer.h"
#include "../flash.h"
#include "../mdec.h"
#include "lib/include/trng.h"
#include "lib/include/sys.h"
#include "lib/include/plic.h"
#include "../stimer.h"
#include "../clock.h"
#include "../uart.h"
#include "../mspi.h"
#include "types.h"
#include "compiler.h"

/******************************* stimer_start ******************************************************************/

#define reg_system_tick_irq				reg_system_irq_level

typedef enum {
	STIMER_IRQ_MASK     		=   BIT(0),
	STIMER_32K_CAL_IRQ_MASK     =   BIT(1),
}stimer_irq_mask_e;

typedef enum {
	FLD_IRQ_SYSTEM_TIMER     		=   BIT(0),
}system_timer_irq_mask_e;


typedef enum {
	STIMER_IRQ_CLR	     		=   BIT(0),
	STIMER_32K_CAL_IRQ_CLR     	=   BIT(1),
}stimer_irq_clr_e;


/**
 * @brief    This function serves to enable system timer interrupt.
 * @return  none
 */
static inline void systimer_irq_enable(void)
{
	reg_irq_src0 |= BIT(IRQ1_SYSTIMER);
	//plic_interrupt_enable(IRQ1_SYSTIMER);
}

/**
 * @brief    This function serves to disable system timer interrupt.
 * @return  none
 */
static inline void systimer_irq_disable(void)
{
	reg_irq_src0 &= ~BIT(IRQ1_SYSTIMER);
	//plic_interrupt_disable(IRQ1_SYSTIMER);
}

static inline void systimer_set_irq_mask(void)
{
	reg_system_irq_mask |= STIMER_IRQ_MASK;
}

static inline void systimer_clr_irq_mask(void)
{
	reg_system_irq_mask &= (~STIMER_IRQ_MASK);
}

static inline unsigned char systimer_get_irq_status(void)
{
	return reg_system_cal_irq & FLD_IRQ_SYSTEM_TIMER;
}

static inline void systimer_clr_irq_status(void)
{
	reg_system_cal_irq = STIMER_IRQ_CLR;
}

static inline void systimer_set_irq_capture(unsigned int tick)
{
	reg_system_irq_level = tick;
}

static inline unsigned int systimer_get_irq_capture(void)
{
	return reg_system_irq_level;
}

static inline int tick1_exceed_tick2(u32 tick1, u32 tick2)
{
	return (u32)(tick1 - tick2) < BIT(30);
}
/******************************* stimer_end ********************************************************************/



/******************************* aes_start ******************************************************************/
extern unsigned int aes_data_buff[8];
/******************************* aes_end ********************************************************************/



/******************************* core_start ******************************************************************/
#define  irq_disable		core_interrupt_disable
#define	 irq_enable			core_interrupt_enable
#define  irq_restore(en)	core_restore_interrupt(en)
/******************************* core_end ********************************************************************/



/******************************* analog_start ******************************************************************/
#define analog_write	analog_write_reg8
#define analog_read		analog_read_reg8

/******************************* analog_end ********************************************************************/



/******************************* clock_start ******************************************************************/
typedef enum{
	SYSCLK_16M    =    16,
	SYSCLK_24M    =    24,
	SYSCLK_32M    =    32,
	SYSCLK_48M    =    48,
	SYSCLK_64M    =    64,
}sys_clk_fre_t;

static inline unsigned char clock_get_system_clk()
{
	return sys_clk.cclk;
}
/******************************* clock_end ********************************************************************/



/******************************* trng_start ******************************************************************/
#define rand						trng_rand

/******************************* trng_end ********************************************************************/



/******************************* sys_start ******************************************************************/
#define sleep_us(x)					delay_us(x)
#define sleep_ms(x)					delay_ms(x)


/******************************* sys_end ********************************************************************/

/******************************* pa_start ******************************************************************/
#define PA_TYPE_OFF							0
#define PA_TYPE_TX_ON						1
#define PA_TYPE_RX_ON						2


typedef void (*rf_pa_callback_t)(int type);
extern rf_pa_callback_t  blc_rf_pa_cb;
/******************************* pa_end ********************************************************************/


/******************************* usb_end *********************************************************************/
#define reg_usb_irq	REG_ADDR8(0x100839)
/******************************* usb_end *********************************************************************/



/******************************* core_start ******************************************************************/
#define	SUPPORT_PFT_ARCH		1
/******************************* core_end ********************************************************************/



/******************************* uart_start ******************************************************************/
_attribute_ram_code_ void uart_receive_dma_set(dma_chn_e chn, unsigned char * addr,unsigned int rev_size);
/******************************* uart_end ********************************************************************/


#endif /* DRIVERS_B91_EXT_MISC_H_ */
