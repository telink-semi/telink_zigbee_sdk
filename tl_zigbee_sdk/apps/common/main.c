/********************************************************************************************************
 * @file     main.c
 *
 * @brief	 Common main entry
 *
 * @author
 * @date     Dec. 1, 2017
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#include "user_config.h"
#include "zb_common.h"


extern void user_init();

/* system clock configuration
 *
 * platform:
 * 		826x
 * 		8258
 * 		HAWK
 *
 *  */
#if defined(MCU_CORE_826x)
	#if(CLOCK_SYS_CLOCK_HZ == 32000000)
		SYS_CLK_TYPEDEF g_sysClk = SYS_CLK_32M_PLL;
	#elif(CLOCK_SYS_CLOCK_HZ == 16000000)
		SYS_CLK_TYPEDEF g_sysClk = SYS_CLK_16M_PLL;
	#else
		#error please config system clock
	#endif
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	#if(CLOCK_SYS_CLOCK_HZ == 24000000)
		SYS_CLK_TYPEDEF g_sysClk = SYS_CLK_24M_Crystal;
	#elif(CLOCK_SYS_CLOCK_HZ == 16000000)
		SYS_CLK_TYPEDEF g_sysClk = SYS_CLK_16M_Crystal;
	#elif(CLOCK_SYS_CLOCK_HZ == 48000000)
		SYS_CLK_TYPEDEF g_sysClk = SYS_CLK_48M_Crystal;
	#else
		#error please config system clock
	#endif
#elif defined(MCU_CORE_HAWK)
	#if(CLOCK_SYS_CLOCK_HZ == 16000000)
		SYS_CLK_TYPEDEF g_sysClk = SYS_CLK_16M_PLL;
	#else
		#error please config system clock
	#endif
#endif

enum{
	SYSTEM_RETENTION_NONE,
	SYSTEM_RETENTION_EN,
};

/*
 * platform initiation:
 * 		clock_init: 	system clock selection
 * 		ZB_RADIO_INIT: 	RF module
 * 		gpio_init:		gpio configuration
 * 		ZB_TIMER_INIT:  enable a timer for zigbee mac cca-csma
 *
 * */
static u8 platform_init(void){
	u8 ret = SYSTEM_RETENTION_NONE;

#if defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	extern void bss_section_clear(void);
	extern void data_section_load();
	if((analog_read(0x7f) & 0x01)){
		/* Boot from brom. */
		bss_section_clear();
		data_section_load();
	}else{
		/* Recovery from deep retention. */
		ret = SYSTEM_RETENTION_EN;
	}
#endif

	platform_wakeup_init();

	clock_init(g_sysClk);

	if(ret == SYSTEM_RETENTION_NONE){
#if defined(MCU_CORE_8258)
		rng_init();
#elif defined(MCU_CORE_8278)
		random_generator_init();
#endif

#if defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
		internalFlashSizeCheck();
#endif
	}

	gpio_init();

	ZB_RADIO_INIT();

	ZB_TIMER_INIT();

	return ret;
}


/*
 * main:
 * */
int main(void){
	u8 isRetention = platform_init();

	os_init(isRetention);

	user_init();

    irq_enable();

#if (MODULE_WATCHDOG_ENABLE)
    wd_setintervalms(600);
    wd_start();
#endif

#if 0
	extern void moduleTest_start(void);
	moduleTest_start();
#endif

	while(1){
    	tl_zbTaskProcedure();

#if(MODULE_WATCHDOG_ENABLE)
		wd_clear();
#endif
		ev_main();
	}
}


