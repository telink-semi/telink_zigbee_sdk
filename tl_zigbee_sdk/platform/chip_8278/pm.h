/********************************************************************************************************
 * @file    pm.h
 *
 * @brief   This is the header file for B87
 *
 * @author  Driver & Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
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

#pragma once

#include "bit.h"
#include "gpio.h"
#include "clock.h"


#define RAM_CRC_EN							1	//if use RAM_CRC func, retention ldo will turn down to 0.6V in A1, A0 is 0.8V.

#define EARLYWAKEUP_TIME					19
#define	tick_32k_tick_per_ms				32
#define PM_EMPTYRUN_TIME_US					25


/**
 * @brief analog register below can store information when MCU in deepsleep mode
 * 	      store your information in these ana_regs before deepsleep by calling analog_write function
 * 	      when MCU wakeup from deepsleep, read the information by by calling analog_read function
 * 	      Reset these analog registers only by power cycle
 */
#define DEEP_ANA_REG0                       0x3a //initial value =0x00
#define DEEP_ANA_REG1                       0x3b //initial value =0x00
#define DEEP_ANA_REG2                       0x3c //initial value =0x0f

/**
 * @brief these analog register can store data in deepsleep mode or deepsleep with SRAM retention mode.
 * 	      Reset these analog registers by watchdog, chip reset, RESET Pin, power cycle
 */
#define DEEP_ANA_REG6                       0x35 //initial value =0x20
#define DEEP_ANA_REG7                       0x36 //initial value =0x00
#define DEEP_ANA_REG8                       0x37 //initial value =0x00
#define DEEP_ANA_REG9                       0x38 //initial value =0x00
#define DEEP_ANA_REG10                      0x39 //initial value =0xff


#define SYS_NEED_REINIT_EXT32K			    BIT(0)
#define SYS_DEEP_SLEEP_FLAG					BIT(1)


//ana3b system used, user can not use
#define SYS_DEEP_ANA_REG 					DEEP_ANA_REG1
#define WAKEUP_STATUS_TIMER_CORE     	    ( WAKEUP_STATUS_TIMER | WAKEUP_STATUS_CORE)
#define WAKEUP_STATUS_TIMER_PAD		        ( WAKEUP_STATUS_TIMER | WAKEUP_STATUS_PAD)

/**
 * @brief sleep mode.
 * @note	After entering suspend mode,the registers of linklayer and modem return to default values,so the
 * 			functions in rf.h need to be reconfigured after suspend. (The registers with base address 0x400,
 * 			0xf00,0x1200 need to be reconfigured.)
 */
typedef enum {
	//available mode for customer
	SUSPEND_MODE						= 0x00,

	DEEPSLEEP_MODE						= 0x30,		//when use deep mode pad wakeup(low or high level), if the high(low) level always in
													//the pad, system will not enter sleep and go to below of pm API, will reboot by core_6f = 0x20
													//deep retention also had this issue, but not to reboot.
	DEEPSLEEP_MODE_RET_SRAM_LOW16K  	= 0x21,  //for boot from sram
	DEEPSLEEP_MODE_RET_SRAM_LOW32K  	= 0x03,  //for boot from sram

	//not available mode
	DEEPSLEEP_RETENTION_FLAG			= 0x0F,
}SleepMode_TypeDef;


/**
 * @brief   wakeup source
 */
typedef enum {
	 //available wake-up source for customer
	 PM_WAKEUP_PAD   		= BIT(3),
	 PM_WAKEUP_CORE  		= BIT(4),
	 PM_WAKEUP_TIMER 		= BIT(5),
	 PM_WAKEUP_COMPARATOR 	= BIT(6),
	 PM_WAKEUP_MDEC		 	= BIT(7),
}SleepWakeupSrc_TypeDef;

/**
 * @brief   wakeup status
 */
enum {
	 WAKEUP_STATUS_COMPARATOR  		= BIT(0),
	 WAKEUP_STATUS_TIMER  			= BIT(1),
	 WAKEUP_STATUS_CORE 			= BIT(2),
	 WAKEUP_STATUS_PAD    			= BIT(3),
	 WAKEUP_STATUS_MDEC    			= BIT(4),

	 STATUS_GPIO_ERR_NO_ENTER_PM  	= BIT(8),/**<Bit8 is used to determine whether the wake source is normal.*/
	 STATUS_ENTER_SUSPEND  			= BIT(30),
};

/**
 * @brief   mcu status
 */
typedef enum{
	MCU_STATUS_BOOT,
	MCU_STATUS_DEEPRET_BACK,
	MCU_STATUS_DEEP_BACK,
}pm_mcu_status;

/**
 * @brief   deepsleep wakeup status
 */
typedef struct{
	unsigned char is_pad_wakeup;
	unsigned char wakeup_src;
	unsigned char mcu_status;
}pm_para_t;

/**
 * @brief	early wakeup time
 */
typedef struct {
	unsigned short  suspend;	/*< suspend_early_wakeup_time_us >*/
	unsigned short  deep_ret;	/*< deep_ret_early_wakeup_time_us >*/
	unsigned short  deep;		/*< deep_early_wakeup_time_us >*/
	unsigned short  min;		/*< sleep_min_time_us >*/
}pm_early_wakeup_time_us_s;

extern volatile pm_early_wakeup_time_us_s g_pm_early_wakeup_time_us;

/**
 * @brief	hardware delay time
 */
typedef struct {
	unsigned short  deep_r_delay_cycle ;			/**< hardware delay time, deep_r_delay_us = (deep_r_delay_cycle+1) * 1/16k */
	unsigned short  suspend_ret_r_delay_cycle ;		/**< hardware delay time, suspend_ret_r_delay_us = (suspend_ret_r_delay_cycle+1) * 1/16k */
}pm_r_delay_cycle_s;

extern volatile pm_r_delay_cycle_s g_pm_r_delay_cycle;


typedef int (*suspend_handler_t)(void);
typedef unsigned int (*pm_tim_recover_handler_t)(unsigned int);
typedef int (*cpu_pm_handler_t)(SleepMode_TypeDef sleep_mode,  SleepWakeupSrc_TypeDef wakeup_src, unsigned int  wakeup_tick);

extern suspend_handler_t func_before_suspend;
extern pm_tim_recover_handler_t pm_tim_recover;
extern cpu_pm_handler_t cpu_sleep_wakeup;
extern pm_para_t pmParam;

extern unsigned int RAM_CRC_EN_16KRAM_TIME;
extern unsigned int RAM_CRC_EN_32KRAM_TIME;

void bls_pm_registerFuncBeforeSuspend(suspend_handler_t func);

/**
 * @brief      This function serves to change the timing of enable ram crc.
 * @param[in]  none.
 * @return     none.
 */
static inline void ram_crc_en_timing(unsigned int RAM_CRC_16K_Timing, unsigned int RAM_CRC_32K_Timing)
{
	RAM_CRC_EN_16KRAM_TIME = RAM_CRC_16K_Timing;
	RAM_CRC_EN_32KRAM_TIME = RAM_CRC_32K_Timing;
}

/**
 * @brief      This function serves to get mcu status.
 * @param[in]  none.
 * @return     pm_mcu_status
 */
static inline unsigned char pm_get_mcu_status(void)
{
	return pmParam.mcu_status;
}

/**
 * @brief      This function serves to determine whether mcu is waked up by pad.
 * @param[in]  none.
 * @return     1- yes , 0- no.
 */
static inline unsigned char pm_is_padWakeup(void)
{
	return pmParam.is_pad_wakeup;
}

/**
 * @brief      This function serves to get the source of wake-up.
 * @param[in]  none.
 * @return     wakeup source.
 */
static inline unsigned char pm_get_wakeup_src(void)
{
	return pmParam.wakeup_src;
}

/**
 * @brief   This function serves to wake up cpu from stall mode by timer0.
 * @param   tick - capture value of timer0.
 * @return  none.
 */
void cpu_stall_wakeup_by_timer0(unsigned int tick);

/**
 * @brief   This function serves to wake up cpu from stall mode by timer1.
 * @param   tick - capture value of timer1.
 * @return  none.
 */
void cpu_stall_wakeup_by_timer1(unsigned int tick);

/**
 * @brief   This function serves to wake up cpu from stall mode by timer2.
 * @param   tick - capture value of timer2.
 * @return  none.
 */
void cpu_stall_wakeup_by_timer2(unsigned int tick);

/**
 * @brief      This function configures a GPIO pin as the wakeup pin.
 * @param[in]  Pin - the pin needs to be configured as wakeup pin.If want to use PA5/PA6<dp/dm> as the suspend wakeup pad,
 * 				and the polarity is low wakeup,need to set this IO as GPIO or call API pm_set_suspend_power_cfg() to power
 * 				on the USB power, power on USB will increase the suspend current about 0.1uA;In deep/deep_ret andlow polarity
 * 				wakeup mode,you need call API pm_set_suspend_power_cfg() to power on USB,power on USB will not increase deep current.
 * @param[in]  Pol - the wakeup polarity of the pad pin(0: low-level wakeup, 1: high-level wakeup)
 * @param[in]  En  - enable or disable the wakeup function for the pan pin(1: Enable, 0: Disable)
 * @return     none
 */
void cpu_set_gpio_wakeup(GPIO_PinTypeDef pin, GPIO_LevelTypeDef pol, int en);

/**
 * @brief     This function servers to set the match value for MDEC wakeup.
 * @param[in] value - the MDEC match value for wakeup.
 * @return    none
 */
void cpu_set_mdec_value_wakeup(unsigned char value);


/**
 * @brief   This function serves to reboot chip.
 * @param   none.
 * @return  none.
 */

void start_reboot(void);

/**
 * @brief   This function serves to get the 32k tick.
 * @param   none
 * @return  tick of 32k .
 */
extern unsigned int pm_get_32k_tick(void);

/**
 * @brief   This function serves to initialize MCU
 * @param   power mode- set the power mode(LOD mode, DCDC mode, DCDC_LDO mode)
 * @param   xtal - set crystal for different application.
 * @return  none
 * @note	1. For crystal oscillators with very slow start-up or poor quality, after calling this function,
 * 				a reboot will be triggered (whether a reboot has occurred can be judged by using DEEP_ANA_REG0[bit1]).
 * 				For the case where the crystal oscillator used is very slow to start, you can call the pm_set_wakeup_time_param()
 * 				to adjust the waiting time for the crystal oscillator to start before calling the cpu_wakeup_init interface.
 * 				When this time is adjusted to meet the crystal oscillator requirements, it will not reboot.
 * 			2. When this function called after power on or deep sleep wakeup, it will cost about 6~7ms for perform 32k RC calibration. 
 * 				If do not want this logic, you can check the usage and precautions of cpu_wakeup_init_calib_32k_rc_cfg().
 */
void cpu_wakeup_init(POWER_MODE_TypeDef power_mode, cap_typedef_e cap);

/**
 * @brief   This function serves to recover system timer from tick of internal 32k RC.
 * @param   none.
 * @return  none.
 */
unsigned int pm_tim_recover_32k_rc(unsigned int now_tick_32k);

/**
 * @brief   This function serves to recover system timer from tick of external 32k crystal.
 * @param   none.
 * @return  none.
 */
unsigned int pm_tim_recover_32k_xtal(unsigned int now_tick_32k);

/**
 * @brief      This function serves to set the working mode of MCU based on 32k crystal,e.g. suspend mode, deepsleep mode, deepsleep with SRAM retention mode and shutdown mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select.
 * @param[in]  wakeup_tick - the time of short sleep, which means MCU can sleep for less than 234 seconds.
 * @return     indicate whether the cpu is wake up successful.
 */
int cpu_sleep_wakeup_32k_rc(SleepMode_TypeDef sleep_mode, SleepWakeupSrc_TypeDef wakeup_src, unsigned int wakeup_tick);

/**
 * @brief      This function serves to set the working mode of MCU based on 32k crystal,e.g. suspend mode, deepsleep mode, deepsleep with SRAM retention mode and shutdown mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select.
 * @param[in]  wakeup_tick - the time of short sleep, which means MCU can sleep for less than 234 seconds.
 * @return     indicate whether the cpu is wake up successful.
 */
int cpu_sleep_wakeup_32k_xtal(SleepMode_TypeDef sleep_mode, SleepWakeupSrc_TypeDef wakeup_src, unsigned int wakeup_tick);

/**
 * @brief      This function serves to set the working mode of MCU,e.g. suspend mode, deepsleep mode, deepsleep with SRAM retention mode and shutdown mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select.
 * @param[in]  wakeup_tick - the time of short sleep, which means MCU can sleep for more than 1 day.
 * @return     indicate whether the cpu is wake up successful.
 */
int pm_long_sleep_wakeup(SleepMode_TypeDef sleep_mode, SleepWakeupSrc_TypeDef wakeup_src, unsigned int wakeup_tick);

/**
 * @brief      This function serves to determine whether wake up source is internal 32k RC.
 * @param[in]  none.
 * @return     none.
 */
static inline void pm_select_internal_32k_rc(void)
{
	cpu_sleep_wakeup 	 	= cpu_sleep_wakeup_32k_rc;
	pm_tim_recover  	 	= pm_tim_recover_32k_rc;
}

/**
 * @brief      This function serves to determine whether wake up source is external 32k RC.
 * @param[in]  none.
 * @return     none.
 */
static inline void pm_select_external_32k_crystal(void)
{
	cpu_sleep_wakeup 	 	= cpu_sleep_wakeup_32k_xtal;
	pm_tim_recover		 	= pm_tim_recover_32k_xtal;
}


/**********************************  Internal APIs (not for user)***************************************************/
extern  unsigned char 		    tl_multi_addr_L;
extern  unsigned char 		    tl_multi_addr_H;
extern  unsigned char 		    tl_24mrc_cal;
extern 	unsigned int 			tick_32k_calib;
extern  unsigned int 			tick_cur;
extern  unsigned int 			tick_32k_cur;
extern  unsigned char       	pm_long_suspend;

void sleep_start(void);

unsigned int pm_get_info0(void);

unsigned int pm_get_info1(void);

void cpu_set_32k_tick(unsigned int tick);

