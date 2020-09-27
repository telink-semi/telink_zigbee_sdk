/********************************************************************************************************
 * @file     drv_hw.c
 *
 * @brief	 hardware interface function file
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



/*
 * system clock configuration
 */
#if defined(MCU_CORE_826x)
	#if(CLOCK_SYS_CLOCK_HZ == 32000000)
		#define SYS_CLOCK_VALUE		SYS_CLK_32M_PLL
	#elif(CLOCK_SYS_CLOCK_HZ == 16000000)
		#define SYS_CLOCK_VALUE		SYS_CLK_16M_PLL
	#else
		#error please config system clock
	#endif
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	#if(CLOCK_SYS_CLOCK_HZ == 24000000)
		#define SYS_CLOCK_VALUE		SYS_CLK_24M_Crystal
	#elif(CLOCK_SYS_CLOCK_HZ == 16000000)
		#define SYS_CLOCK_VALUE		SYS_CLK_16M_Crystal
	#elif(CLOCK_SYS_CLOCK_HZ == 48000000)
		#define SYS_CLOCK_VALUE		SYS_CLK_48M_Crystal
	#else
		#error please config system clock
	#endif
#elif defined(MCU_CORE_B91)
	#if(CLOCK_SYS_CLOCK_HZ == 48000000)
		#define CLOCK_INIT			CCLK_48M_HCLK_48M_PCLK_24M
	#else
		#error please config system clock
	#endif
#endif



void enable_irq(void)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	irq_enable();
#elif defined(MCU_CORE_B91)
	core_interrupt_enable();
#endif
}

u32 disable_irq(void)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	return (u32)irq_disable();
#elif defined(MCU_CORE_B91)
	return core_interrupt_disable();
#endif
}

u32 restore_irq(u32 en)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	irq_restore((u8)en);
	return 0;
#elif defined(MCU_CORE_B91)
	return core_restore_interrupt(en);
#endif
}

void randInit(void)
{
#if defined(MCU_CORE_8258)
		rng_init();
#elif defined(MCU_CORE_8278)
		random_generator_init();
#elif defined(MCU_CORE_B91)
		trng_init();
#endif
}

u32 u32Rand(void)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	return rand();
#elif defined(MCU_CORE_B91)
	return trng_rand();
#endif
}

void generateRandomData(u8 *pData, u8 len)
{
	u32 randNums = 0;
	/* if len is odd */
	for(u8 i = 0; i < len; i++){
		if((i & 3) == 0){
			randNums = u32Rand();
		}

		pData[i] = randNums & 0xff;
		randNums >>= 8;
	}
}

/****
* brief: platform initialization function
*
* param[in] null
*
* @return	  1: startup with ram retention;
*             0: no ram retention.
*/
startup_state_e platform_init(void)
{
	startup_state_e ret = SYSTEM_RETENTION_NONE;

	platform_wakeup_init();

#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	clock_init(SYS_CLOCK_VALUE);
#elif defined(MCU_CORE_B91)
	CLOCK_INIT;
#endif

#if defined(MCU_CORE_8258) || defined(MCU_CORE_8278) || defined(MCU_CORE_B91)
	ret = (pmParam.back_mode == BACK_FROM_DEEP_RETENTION) ? SYSTEM_RETENTION_EN : SYSTEM_RETENTION_NONE;
#endif

	if(ret == SYSTEM_RETENTION_NONE){
		randInit();
	}

	gpio_init(TRUE);

	ZB_RADIO_INIT();

	ZB_TIMER_INIT();

	return ret;
}



