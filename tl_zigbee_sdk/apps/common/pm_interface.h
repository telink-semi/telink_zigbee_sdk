/********************************************************************************************************
 * @file     pm_interface.h
 *
 * @brief    Power management header
 *
 * @author
 * @date     Feb. 1, 2017
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
#ifndef PM_INTERFACE_H_
#define PM_INTERFACE_H_

typedef struct{
	u32 wakeupPin;
	u8 wakeLevel;
}pm_pinCfg_t;

void pm_wakeupPinConfig(pm_pinCfg_t *pmCfg, int pinNum);

u8 pm_wakeupValid(pm_pinCfg_t *pmCfg, int pinNum);

void sys_enterLowPower(u8 mode);

void pm_suspendEnter(int wakeUpSrc, u32 ms);

void pm_deepSleepEnter(int wakeUpSrc, u32 ms);

u8 pm_interleaveSleepEnter(int wakeUpSrc, u32 ms);

#endif
