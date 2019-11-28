/********************************************************************************************************
 * @file     sampleLightCtrl.h
 *
 * @brief
 *
 * @author
 * @date     Dec. 1, 2016
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

#ifndef _SAMPLE_LIGHT_CTRL_H_
#define _SAMPLE_LIGHT_CTRL_H_


/**********************************************************************
 * CONSTANT
 */
#define PMW_MAX_TICK		            (CLOCK_SYS_CLOCK_HZ / 1000)/* 1000Hz PWM */
#define PWM_FULL_DUTYCYCLE				100


/**********************************************************************
 * FUNCTIONS
 */
void hwLight_init(void);
void hwLight_onOffUpdate(u8 onOff);
void hwLight_levelUpdate(u8 level);
void hwLight_colorUpdate_colorTemperature(u16 colorTemperatureMireds, u8 level);
void hwLight_colorUpdate_HSV2RGB(u8 hue, u8 saturation, u8 level);

void light_adjust(void);
void light_fresh(void);
void light_applyUpdate(u8 *curLevel, u16 *curLevel256, s32 *stepLevel256, u16 *remainingTime, u8 minLevel, u8 maxLevel, bool wrap);
void light_applyUpdate_16(u16 *curLevel, u32 *curLevel256, s32 *stepLevel256, u16 *remainingTime, u16 minLevel, u16 maxLevel, bool wrap);

void light_blink_start(u8 times, u16 ledOnTime, u16 ledOffTime);
void light_blink_stop(void);

#endif	/* _SAMPLE_LIGHT_CTRL_H_ */
