/********************************************************************************************************
 * @file     drv_hw.h
 *
 * @brief	 hardware interface function header file.
 *
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
	SYSTEM_RETENTION_NONE,
	SYSTEM_RETENTION_EN,
}startup_state_e;


startup_state_e platform_init(void);

void enable_irq(void);
u32 disable_irq(void);
u32 restore_irq(u32 en);

u32 u32Rand(void);
void generateRandomData(unsigned char *pData, unsigned char len);
