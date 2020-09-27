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

#include "app_cfg.h"
#include "zb_common.h"

extern void user_init();


/*
 * main:
 * */
int main(void){
	u8 isRetention = platform_init();

	os_init(isRetention);

#if 0
	extern void moduleTest_start(void);
	moduleTest_start();
#endif

	user_init();

	enable_irq();

#if (MODULE_WATCHDOG_ENABLE)
    wd_setintervalms(600);
    wd_start();
#endif

	while(1){
    	tl_zbTaskProcedure();

#if(MODULE_WATCHDOG_ENABLE)
		wd_clear();
#endif
		ev_main();
	}

	return 0;
}


