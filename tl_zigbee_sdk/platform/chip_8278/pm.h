/********************************************************************************************************
 * @file     pm.h
 *
 * @brief    This is the header file for TLSR8278
 *
 * @author	 Driver Group
 * @date     May 8, 2018
 *
 * @par      Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
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
 * @par      History:
 * 			 1.initial release(DEC. 26 2018)
 *
 * @version  A001
 *
 *******************************************************************************************************/

#pragma once

#include "bsp.h"
#include "gpio.h"

#define PM_LONG_SUSPEND_EN					1

#ifndef PM_TIM_RECOVER_MODE
#define PM_TIM_RECOVER_MODE			    	0
#endif


#define XTAL_READY_CHECK_TIMING_OPTIMIZE	1

#define RAM_CRC_EN							1  	//if use RAM_CRC func, retention ldo will turn down to 0.6V in A1, A0 is 0.8V.

//when timer wakeup,the DCDC delay time is accurate,but other wake-up sources wake up,
//this time is ((PM_DCDC_DELAY_CYCLE+1)*2-1)*32us ~ (PM_DCDC_DELAY_CYCLE+1)*2*32us
#define PM_DCDC_DELAY_DURATION     			187   // delay_time_us = (PM_DCDC_DELAY_CYCLE+1)*2*32us
												  // 2 * 1/16k = 125 uS, 3 * 1/16k = 187.5 uS  4*1/16k = 250 uS

#define PM_XTAL_MANUAL_MODE_DELAY		    200  //150  200

#if(PM_DCDC_DELAY_DURATION == 62)
#define PM_DCDC_DELAY_CYCLE		0
#elif(PM_DCDC_DELAY_DURATION == 125)
#define PM_DCDC_DELAY_CYCLE		1
#elif(PM_DCDC_DELAY_DURATION == 187)
#define PM_DCDC_DELAY_CYCLE		2
#elif(PM_DCDC_DELAY_DURATION == 250)
#define PM_DCDC_DELAY_CYCLE		3
#endif

#define EARLYWAKEUP_TIME_US_SUSPEND 		(PM_DCDC_DELAY_DURATION + PM_XTAL_MANUAL_MODE_DELAY + 200)  //100: code running time margin//154  //175
#define EARLYWAKEUP_TIME_US_DEEP_RET    	(PM_DCDC_DELAY_DURATION + 64)//(PM_DCDC_DELAY_DURATION + 32)
//#define EARLYWAKEUP_TIME_US_DEEP	    	(PM_DCDC_DELAY_DURATION + 32 + ((SOFT_START_DLY)*62))
#define EMPTYRUN_TIME_US       	    		(EARLYWAKEUP_TIME_US_SUSPEND + 200)

#define EARLYWAKEUP_TIME			19
#define	tick_32k_tick_per_ms		32
#define PM_EMPTYRUN_TIME_US			25



#define PM_LONG_SLEEP_WAKEUP_EN			    1 //if user need to make MCU sleep for a long time that is more than 268s, this macro need to be enabled and use "pm_long_sleep_wakeup" function

/**
 * @brief analog register below can store infomation when MCU in deepsleep mode
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


/* used to restore data during deep sleep mode or reset by software */
#define DATA_STORE_FLAG				0x55

#define	REG_DEEP_BACK_FLAG			DEEP_ANA_REG0//0x3A, power on reset clean
#define	REG_DEEP_FLAG				DEEP_ANA_REG6//0x35, watch dog reset clean
#define	REG_FRAMECOUNT				DEEP_ANA_REG7//0x36, watch dog reset clean, 4Bytes from 0x36 to 0x39


//ana3b system used, user can not use
#define SYS_DEEP_ANA_REG 					DEEP_ANA_REG1
#define WAKEUP_STATUS_TIMER_CORE     	    ( WAKEUP_STATUS_TIMER | WAKEUP_STATUS_CORE)
#define WAKEUP_STATUS_TIMER_PAD		        ( WAKEUP_STATUS_TIMER | WAKEUP_STATUS_PAD)

#define	ZB_POWER_DOWN						1 //weather to power down the RF before suspend
#define	AUDIO_POWER_DOWN					1 //weather to power down the AUDIO before suspend
#define	USB_POWER_DOWN						1 //weather to power down the USB before suspend

/**
 * @brief sleep mode.
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
	 //not available wake-up source for customer
	 PM_TIM_RECOVER_START   = BIT(14),
	 PM_TIM_RECOVER_END     = BIT(15),
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

	 STATUS_GPIO_ERR_NO_ENTER_PM  	= BIT(7),
	 STATUS_ENTER_SUSPEND  			= BIT(30),
};

/**
 * @brief   deepsleep wakeup by external xtal
 */
typedef struct{
	unsigned char ext_cap_en;    //24xtal  cap
	unsigned char pad32k_en;
	unsigned char pm_enter_en;
	unsigned char rsvd;
}misc_para_t;

extern  _attribute_aligned_(4) misc_para_t 				blt_miscParam;

enum{
	BACK_FROM_REPOWER,
	BACK_FROM_DEEP,
	BACK_FROM_DEEP_RETENTION,
};


/**
 * @brief   deepsleep wakeup status
 */
typedef struct{
	unsigned char back_mode;
	unsigned char is_pad_wakeup;
	unsigned char wakeup_src;
	unsigned char rsvd;
}pm_para_t;

extern _attribute_aligned_(4) pm_para_t	pmParam;

#if (PM_TIM_RECOVER_MODE)

typedef struct{
	unsigned int   tick_sysClk;
	unsigned int   tick_32k;
	unsigned int   recover_flag;
}pm_tim_recover_t;

extern _attribute_aligned_(4) pm_tim_recover_t			pm_timRecover;
#endif


typedef int (*suspend_handler_t)(void);
extern  suspend_handler_t 		 func_before_suspend;

/**
 * @brief      This function serves to enable dp and dm deep gpio low level wakeup. if enable, current will
 * 						add about 0.1uA
 * @param[in]  none.
 * @return     none.
 */
extern unsigned char PA5_PA6_DEEPSLEEP_LOW_LEVEL_WAKEUP_EN;
static inline void deepsleep_dp_dm_gpio_low_wake_enable(void)
{
	PA5_PA6_DEEPSLEEP_LOW_LEVEL_WAKEUP_EN = 0;
}
/**
 * @brief      This function serves to disable dp and dm deep gpio low level wakeup.
 * @param[in]  none.
 * @return     none.
 */
static inline void deepsleep_dp_dm_gpio_low_wake_disable(void)
{
	PA5_PA6_DEEPSLEEP_LOW_LEVEL_WAKEUP_EN = 1;
}

/**
 * @brief      This function serves to change the timing of enable ram crc.
 * @param[in]  none.
 * @return     none.
 */
extern unsigned int RAM_CRC_EN_16KRAM_TIME;
extern unsigned int RAM_CRC_EN_32KRAM_TIME;
static inline void ram_crc_en_timing(unsigned int RAM_CRC_16K_Timing, unsigned int RAM_CRC_32K_Timing)
{
	RAM_CRC_EN_16KRAM_TIME = RAM_CRC_16K_Timing;
	RAM_CRC_EN_32KRAM_TIME = RAM_CRC_32K_Timing;
}

/**
 * @brief      This function serves to change the timing of soft start delay.
 * @param[in]  none.
 * @return     none.
 */
extern unsigned char SOFT_START_DLY;
extern unsigned int EARLYWAKEUP_TIME_US_DEEP;
static inline void soft_start_dly_time(unsigned char soft_start_time)
{
	SOFT_START_DLY = soft_start_time;
	EARLYWAKEUP_TIME_US_DEEP = PM_DCDC_DELAY_DURATION + 32 + ((SOFT_START_DLY)*62);
}

/**
 * @brief     this function servers to wait bbpll clock lock
 * @param[in] none
 * @return    none
 */
void pm_wait_bbpll_done(void);

void bls_pm_registerFuncBeforeSuspend (suspend_handler_t func );


/**
 * @brief      This function serves to determine whether mcu is waked up from deep retention.
 * @param[in]  none.
 * @return     1- yes , 0- no.
 */
static inline int pm_is_MCU_deepRetentionWakeup(void)
{
	if(pmParam.back_mode == BACK_FROM_DEEP_RETENTION){
		return 1;
	}
	return 0;
}

/**
 * @brief      This function serves to determine whether mcu is waked up by pad.
 * @param[in]  none.
 * @return     1- yes , 0- no.
 */
static inline int pm_is_deepPadWakeup(void)
{
	return pmParam.is_pad_wakeup;
}

/**
 * @brief      This function serves to get the source of wake-up.
 * @param[in]  none.
 * @return     wakeup source.
 */
static inline int pm_get_wakeup_src(void)
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
 * @brief   This function serves to wake up cpu from stall mode by timer1 or RF TX done irq.
 * @param   WakeupSrc  - timer1.
 * @param   IntervalUs - capture value of timer1.
 * @param   sysclktick - tick value of per us based on system clock.
 * @return  none.
 */
unsigned int cpu_stall(int WakeupSrc, unsigned int IntervalUs,unsigned int sysclktick);

/**
 * @brief      This function configures a GPIO pin as the wakeup pin.
 * @param[in]  pin - the pin needs to be configured as wakeup pin
 * @param[in]  pol - the wakeup polarity of the pad pin(0: low-level wakeup, 1: high-level wakeup)
 * @param[in]  en  - enable or disable the wakeup function for the pan pin(1: Enable, 0: Disable)
 * @return     none
 */
void cpu_set_gpio_wakeup (GPIO_PinTypeDef pin, GPIO_LevelTypeDef pol, int en);

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
 * @param   none
 * @return  none
 */
void cpu_wakeup_init(void);

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


typedef unsigned int (*pm_tim_recover_handler_t)(unsigned int);

extern  pm_tim_recover_handler_t pm_tim_recover;


/**
 * @brief      This function serves to set the working mode of MCU based on 32k crystal,e.g. suspend mode, deepsleep mode, deepsleep with SRAM retention mode and shutdown mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select.
 * @param[in]  wakeup_tick - the time of short sleep, which means MCU can sleep for less than 5 minutes.
 * @return     indicate whether the cpu is wake up successful.
 */
int  cpu_sleep_wakeup_32k_rc(SleepMode_TypeDef sleep_mode,  SleepWakeupSrc_TypeDef wakeup_src, unsigned int  wakeup_tick);

/**
 * @brief      This function serves to set the working mode of MCU based on 32k crystal,e.g. suspend mode, deepsleep mode, deepsleep with SRAM retention mode and shutdown mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select.
 * @param[in]  wakeup_tick - the time of short sleep, which means MCU can sleep for less than 5 minutes.
 * @return     indicate whether the cpu is wake up successful.
 */
int  cpu_sleep_wakeup_32k_xtal(SleepMode_TypeDef sleep_mode,  SleepWakeupSrc_TypeDef wakeup_src, unsigned int  wakeup_tick);

typedef int (*cpu_pm_handler_t)(SleepMode_TypeDef sleep_mode,  SleepWakeupSrc_TypeDef wakeup_src, unsigned int  wakeup_tick);

extern 	cpu_pm_handler_t  		 cpu_sleep_wakeup;

/**
 * @brief      This function serves to determine whether wake up source is internal 32k RC.
 * @param[in]  none.
 * @return     none.
 */
static inline void blc_pm_select_internal_32k_crystal(void)
{
	cpu_sleep_wakeup 	 	= cpu_sleep_wakeup_32k_rc;
	pm_tim_recover  	 	= pm_tim_recover_32k_rc;

	blt_miscParam.pm_enter_en 	= 1; // allow enter pm, 32k rc does not need to wait for 32k clk to be stable
}

/**
 * @brief      This function serves to determine whether wake up source is external 32k RC.
 * @param[in]  none.
 * @return     none.
 */
static inline void blc_pm_select_external_32k_crystal(void)
{
	cpu_sleep_wakeup 	 	= cpu_sleep_wakeup_32k_xtal;
	pm_tim_recover		 	= pm_tim_recover_32k_xtal;

	blt_miscParam.pad32k_en 	= 1; // set '1': 32k clk src use external 32k crystal
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

unsigned int  pm_get_info0(void);

unsigned int  pm_get_info1(void);

unsigned int cpu_get_32k_tick(void);

void cpu_set_32k_tick(unsigned int tick);

void soft_reboot_dly13ms_use24mRC(void);

void check_32k_clk_stable(void);

#if PM_LONG_SLEEP_WAKEUP_EN
/**
 * @brief      This function servers to wake up the cpu from sleep mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select.
 * @param[in]  SleepDurationUs - the time of sleep.
 * @return     indicate whether the cpu is wake up successful.
 */
int pm_long_sleep_wakeup (SleepMode_TypeDef sleep_mode, SleepWakeupSrc_TypeDef wakeup_src, unsigned int  SleepDurationUs);
#endif







#define DEBUG_GPIO_ENABLE							0

#if(DEBUG_GPIO_ENABLE)
	//define debug GPIO here according to your hardware

	#define GPIO_CHN0							GPIO_PB4
	#define GPIO_CHN1							GPIO_PB5
	#define GPIO_CHN2							GPIO_PB6
	#define GPIO_CHN3							GPIO_PB7
	#define GPIO_CHN4							0
	#define GPIO_CHN5							0
	#define GPIO_CHN6							0



	#define DBG_CHN0_LOW		gpio_write(GPIO_CHN0, 0)
	#define DBG_CHN0_HIGH		gpio_write(GPIO_CHN0, 1)
	#define DBG_CHN0_TOGGLE		gpio_toggle(GPIO_CHN0)
	#define DBG_CHN1_LOW		gpio_write(GPIO_CHN1, 0)
	#define DBG_CHN1_HIGH		gpio_write(GPIO_CHN1, 1)
	#define DBG_CHN1_TOGGLE		gpio_toggle(GPIO_CHN1)
	#define DBG_CHN2_LOW		gpio_write(GPIO_CHN2, 0)
	#define DBG_CHN2_HIGH		gpio_write(GPIO_CHN2, 1)
	#define DBG_CHN2_TOGGLE		gpio_toggle(GPIO_CHN2)
	#define DBG_CHN3_LOW		gpio_write(GPIO_CHN3, 0)
	#define DBG_CHN3_HIGH		gpio_write(GPIO_CHN3, 1)
	#define DBG_CHN3_TOGGLE		gpio_toggle(GPIO_CHN3)
	#define DBG_CHN4_LOW		gpio_write(GPIO_CHN4, 0)
	#define DBG_CHN4_HIGH		gpio_write(GPIO_CHN4, 1)
	#define DBG_CHN4_TOGGLE		gpio_toggle(GPIO_CHN4)
	#define DBG_CHN5_LOW		gpio_write(GPIO_CHN5, 0)
	#define DBG_CHN5_HIGH		gpio_write(GPIO_CHN5, 1)
	#define DBG_CHN5_TOGGLE		gpio_toggle(GPIO_CHN5)
	#define DBG_CHN6_LOW		gpio_write(GPIO_CHN6, 0)
	#define DBG_CHN6_HIGH		gpio_write(GPIO_CHN6, 1)
	#define DBG_CHN6_TOGGLE		gpio_toggle(GPIO_CHN6)
#else
	#define DBG_CHN0_LOW
	#define DBG_CHN0_HIGH
	#define DBG_CHN0_TOGGLE
	#define DBG_CHN1_LOW
	#define DBG_CHN1_HIGH
	#define DBG_CHN1_TOGGLE
	#define DBG_CHN2_LOW
	#define DBG_CHN2_HIGH
	#define DBG_CHN2_TOGGLE
	#define DBG_CHN3_LOW
	#define DBG_CHN3_HIGH
	#define DBG_CHN3_TOGGLE
	#define DBG_CHN4_LOW
	#define DBG_CHN4_HIGH
	#define DBG_CHN4_TOGGLE
	#define DBG_CHN5_LOW
	#define DBG_CHN5_HIGH
	#define DBG_CHN5_TOGGLE
	#define DBG_CHN6_LOW
	#define DBG_CHN6_HIGH
	#define DBG_CHN6_TOGGLE
	#define DBG_CHN7_LOW
	#define DBG_CHN7_HIGH
	#define DBG_CHN7_TOGGLE
#endif  //end of DEBUG_GPIO_ENABLE
