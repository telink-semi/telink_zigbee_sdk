/********************************************************************************************************
 * @file	drv_hw.c
 *
 * @brief	This is the source file for drv_hw
 *
 * @author	Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

#define BATTERY_SAFETY_THRESHOLD	2200   //2.2v

//system ticks per US
u32 sysTimerPerUs;



static void randInit(void)
{
#if defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	random_generator_init();
#elif defined(MCU_CORE_B91)
	trng_init();
#endif
}

/*********************************************************************
 * @fn      internalFlashSizeCheck
 *
 * @brief   This function is provided to get and update to the correct flash address
 * 			where are stored the right MAC address and pre-configured parameters.
 * 			NOTE: It should be called before ZB_RADIO_INIT().
 *
 * @param   None
 *
 * @return  None
 */
static void internalFlashSizeCheck(void){
#if defined(MCU_CORE_8258) || defined(MCU_CORE_8278) || defined(MCU_CORE_B91)
	u32 mid = flash_read_mid();
	u8 *pMid = (u8 *)&mid;

	if( (pMid[2] < FLASH_SIZE_512K) ||
		((g_u32MacFlashAddr == FLASH_ADDR_0F_MAC_ADDR_1M) && (pMid[2] < FLASH_SIZE_1M)) ){
		/* Flash space not matched. */
		while(1);
	}

	if( (g_u32MacFlashAddr == FLASH_ADDR_OF_MAC_ADDR_512K) && (pMid[2] >= FLASH_SIZE_1M) ){
		g_u32MacFlashAddr = FLASH_ADDR_0F_MAC_ADDR_1M;
		g_u32CfgFlashAddr = FLASH_ADDR_OF_F_CFG_INFO_1M;
	}
#endif
}

static void voltage_detect_init(void)
{
	drv_adc_init();

#if defined(MCU_CORE_826x)
	drv_adc_mode_pin_set(DRV_ADC_VBAT_MODE, NOINPUT);
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	drv_adc_mode_pin_set(DRV_ADC_VBAT_MODE, GPIO_PB3);
#elif defined(MCU_CORE_B91)
	drv_adc_mode_pin_set(DRV_ADC_BASE_MODE, ADC_GPIO_PB0);
#endif

	drv_adc_enable(1);
}

void voltage_detect(void)
{
#if defined(MCU_CORE_B91)
	//EVK board and dongle do not support.
	return;
#endif

	u16 voltage = drv_get_adc_data();
	u32 curTick = clock_time();

	//printf("VDD: %d\n", voltage);

	while(voltage < BATTERY_SAFETY_THRESHOLD){
		if(clock_time_exceed(curTick, 1000 * 1000)){
#if PM_ENABLE
			drv_pm_sleep(PM_SLEEP_MODE_DEEPSLEEP, 0, 0);
#else
			SYSTEM_RESET();
#endif
		}
		voltage = drv_get_adc_data();
	}
}

static startup_state_e platform_wakeup_init(void)
{
	startup_state_e state = SYSTEM_RETENTION_NONE;

#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258)
	cpu_wakeup_init();
#elif defined(MCU_CORE_8278)
	cpu_wakeup_init(LDO_MODE, EXTERNAL_XTAL_24M);
#elif defined(MCU_CORE_B91)
	sys_init(LDO_1P4_LDO_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6);
#endif

#if defined(MCU_CORE_826x)
	//826x not support ram retention.
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	state = (pm_get_mcu_status() == MCU_STATUS_DEEPRET_BACK) ? SYSTEM_RETENTION_EN : SYSTEM_RETENTION_NONE;
#elif defined(MCU_CORE_B91)
	state = (g_pm_status_info.mcu_status == MCU_STATUS_DEEPRET_BACK) ? SYSTEM_RETENTION_EN : SYSTEM_RETENTION_NONE;
#endif

	return state;
}

/****************************************************************************************************
* @brief 		platform initialization function
*
* @param[in] 	none
*
* @return	  	1: startup with ram retention;
*             	0: no ram retention.
*/
startup_state_e drv_platform_init(void)
{
	startup_state_e state = platform_wakeup_init();

#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	clock_init(SYS_CLOCK_VALUE);
#elif defined(MCU_CORE_B91)
	CLOCK_INIT;
#endif

	/* Get system ticks per US, must be after the clock is initialized. */
#if defined(MCU_CORE_826x)
	sysTimerPerUs = tickPerUs;
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	sysTimerPerUs = sys_tick_per_us;
#elif defined(MCU_CORE_B91)
	sysTimerPerUs = SYSTEM_TIMER_TICK_1US;
#endif

	gpio_init(TRUE);

#if UART_PRINTF_MODE
	DEBUG_TX_PIN_INIT();
#endif

	if(state == SYSTEM_RETENTION_NONE){
		randInit();
		internalFlashSizeCheck();
#if PM_ENABLE
		PM_CLOCK_INIT();
#endif
	}else{
#if PM_ENABLE
		drv_pm_wakeupTimeUpdate();
#endif
	}

#if VOLTAGE_DETECT_ENABLE
	voltage_detect_init();
#endif

	ZB_RADIO_INIT();

	ZB_TIMER_INIT();

	return state;
}

void drv_enable_irq(void)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	irq_enable();
#elif defined(MCU_CORE_B91)
	core_interrupt_enable();
#endif
}

u32 drv_disable_irq(void)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	return (u32)irq_disable();
#elif defined(MCU_CORE_B91)
	return core_interrupt_disable();
#endif
}

u32 drv_restore_irq(u32 en)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	irq_restore((u8)en);
	return 0;
#elif defined(MCU_CORE_B91)
	return core_restore_interrupt(en);
#endif
}

void drv_wd_setInterval(u32 ms)
{
	wd_set_interval_ms(ms);
}

void drv_wd_start(void)
{
	wd_start();
}

void drv_wd_clear(void)
{
	wd_clear();
}

u32 drv_u32Rand(void)
{
#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
	return rand();
#elif defined(MCU_CORE_B91)
	return trng_rand();
#endif
}

void drv_generateRandomData(u8 *pData, u8 len)
{
	u32 randNums = 0;
	/* if len is odd */
	for(u8 i = 0; i < len; i++){
		if((i & 3) == 0){
			randNums = drv_u32Rand();
		}

		pData[i] = randNums & 0xff;
		randNums >>= 8;
	}
}
