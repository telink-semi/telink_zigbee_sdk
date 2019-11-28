/********************************************************************************************************
 * @file     pm.h
 *
 * @brief    power management configuration interface for tlsr8258
 *
 * @author   jian.zhang@telink-semi.com
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

#include "bsp.h"
#include "gpio.h"


static inline void usb_dp_pullup_en (int en)
{
	unsigned char dat = ReadAnalogReg(0x0b);
	if (en) {
		dat = dat | BIT(7);
	}
	else
	{
		dat = dat & 0x7f ;
	}

	WriteAnalogReg (0x0b, dat);
}





//analog register below can store information when MCU in deepsleep mode
//store your information in these ana_regs before deepsleep by calling analog_write function
//when MCU wakeup from deepsleep, read the information by by calling analog_read function

//these five below are stable
/* buffer clean at power on */
#define DEEP_ANA_REG0    0x3a//initial value =0x00
#define DEEP_ANA_REG1    0x3b//initial value =0x00
#define DEEP_ANA_REG2    0x3c//initial value =0x00



//these analog register below may have some problem when user enter deepsleep but ERR wakeup
//for example, when set a GPIO PAD high wakeup deepsleep, but this gpio is high before
//you call func cpu_sleep_wakeup, then deepsleep will be ERR wakeup, these analog register
//information loss.
/* buffer clean at watch dog */
#define DEEP_ANA_REG6    0x35//initial value =0x20
#define DEEP_ANA_REG7    0x36//initial value =0x00
#define DEEP_ANA_REG8    0x37//initial value =0x00
#define DEEP_ANA_REG9    0x38//initial value =0x00
#define DEEP_ANA_REG10   0x39//initial value =0xff


#define ADV_DEEP_FLG	 0x01
#define CONN_DEEP_FLG	 0x02
/* sram retention LDO List :ana_0x02<2:0>
0x00->0.6v
0x01->0.7v
0x02->0.8v
0x03->0.9v
0x04->1.0v
0x05->1.1v
0x06->1.2v
0x07->1.3v
*/
#define SRAM_RET_LDO     0x05     //set ret ldo = 1.1v to avoid sram data missing during low temperature

typedef enum {
	SUSPEND_MODE						= 0,
	DEEPSLEEP_MODE						= 0x80,
	DEEPSLEEP_MODE_RET_SRAM_LOW8K		= 0x61,
	DEEPSLEEP_MODE_RET_SRAM_HIGH8K 		= 0x52,
	DEEPSLEEP_MODE_RET_SRAM_LOW16K  	= 0x43,
	DEEPSLEEP_MODE_RET_SRAM_HIGH16K  	= 0x34,
	DEEPSLEEP_MODE_RET_SRAM_LOW32K  	= 0x07,

	DEEPSLEEP_RETENTION_FLAG			= 0x7F,
}SleepMode_TypeDef;



//set wakeup source
typedef enum {
	 PM_WAKEUP_PAD   = BIT(4),    		SUSPENDWAKEUP_SRC_PAD = BIT(4),    DEEPWAKEUP_SRC_PAD   = BIT(4),
	 PM_WAKEUP_CORE  = BIT(5),
	 PM_WAKEUP_TIMER = BIT(6),	  		SUSPENDWAKEUP_SRC_TIMER = BIT(6),  DEEPWAKEUP_SRC_TIMER = BIT(6),
	 PM_WAKEUP_COMP  = BIT(7),	  		SUSPENDWAKEUP_SRC_COMP  = BIT(7),  DEEPWAKEUP_SRC_COMP  = BIT(7),


	 PM_WAKEUP_CORE_GPIO   = BIT(5) | 0X0800,      SUSPENDWAKEUP_SRC_DIG_GPIO   = BIT(5) | 0X0800,
	 PM_WAKEUP_CORE_USB    = BIT(5) | 0X0400,      SUSPENDWAKEUP_SRC_DIG_USB    = BIT(5) | 0X0400,
	 PM_WAKEUP_CORE_QDEC   = BIT(5) | 0X1000,      SUSPENDWAKEUP_SRC_DIG_QDEC   = BIT(5) | 0X1000,
}SleepWakeupSrc_TypeDef;




//wakeup status from return value of "cpu_sleep_wakeup"
enum {
	 WAKEUP_STATUS_COMP   = BIT(0),  //wakeup by comparator
	 WAKEUP_STATUS_TIMER  = BIT(1),
	 WAKEUP_STATUS_CORE   = BIT(2),
	 WAKEUP_STATUS_PAD    = BIT(3),

	 STATUS_GPIO_ERR_NO_ENTER_PM  = BIT(7),
};

#define 	WAKEUP_STATUS_TIMER_CORE	( WAKEUP_STATUS_TIMER | WAKEUP_STATUS_CORE)
#define 	WAKEUP_STATUS_TIMER_PAD		( WAKEUP_STATUS_TIMER | WAKEUP_STATUS_PAD)

/* used to restore data during deep sleep mode or reset by software */
#define DATA_STORE_FLAG				0x55

#define	REG_DEEP_BACK_FLAG			DEEP_ANA_REG0//0x3A, power on reset clean
#define	REG_DEEP_FLAG				DEEP_ANA_REG6//0x35, watch dog reset clean
#define	REG_FRAMECOUNT				DEEP_ANA_REG7//0x36, watch dog reset clean, 4Bytes from 0x36 to 0x39

enum{
	BACK_FROM_REPOWER,
	BACK_FROM_DEEP,
	BACK_FROM_DEEP_RETENTION,
};

typedef struct{
	unsigned char back_mode;
	unsigned char is_pad_wakeup;
	unsigned char wakeup_src;
}pm_para_t;

extern pm_para_t	pmParam;



void cpu_stall_wakeup_by_timer0(unsigned int tick_stall);
void cpu_stall_wakeup_by_timer1(unsigned int tick_stall);
void cpu_stall_wakeup_by_timer2(unsigned int tick_stall);

typedef int (*suspend_handler_t)(void);
void bls_pm_registerFuncBeforeSuspend(suspend_handler_t func );




void cpu_wakeup_init(void);
void cpu_set_gpio_wakeup (GPIO_PinTypeDef pin, GPIO_LevelTypeDef pol, int en);

int cpu_sleep_wakeup (SleepMode_TypeDef sleep_mode, SleepWakeupSrc_TypeDef wakeup_src, unsigned int wakeup_tick);

int pm_long_sleep_wakeup (SleepMode_TypeDef sleep_mode, SleepWakeupSrc_TypeDef wakeup_src, unsigned int SleepDurationUs);



//only for debug below, will remove them later
void shutdown_gpio(void);  //for debug

unsigned int cpu_get_32k_tick_from_analog(void);

#define PM_Get32kTick			 cpu_get_32k_tick
#define pm_start				 sleep_start
