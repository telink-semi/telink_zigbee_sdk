/********************************************************************************************************
 * @file     pm.c
 *
 * @brief    This is the source file for TLSR8278
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

#include "bsp.h"

#include "compiler.h"
#include "register.h"
#include "gpio.h"
#include "analog.h"
#include "clock.h"
#include "irq.h"
#include "rf_drv.h"
#include "pm.h"
#include "adc.h"

#include "flash.h"


#define FLASH_DEEP_EN				1   //if flash sleep,  when deep retention, must wakeup flash in cstartup


_attribute_data_retention_  unsigned char 		tl_multi_addr_L;
_attribute_data_retention_  unsigned char 		tl_multi_addr_H;
_attribute_data_retention_  unsigned char 		tl_24mrc_cal = 0x80;
_attribute_data_retention_  unsigned char       pm_long_suspend;


_attribute_data_retention_  _attribute_aligned_(4) misc_para_t 		blt_miscParam;
cpu_pm_handler_t 		  	cpu_sleep_wakeup;  //no need retention,  cause it will be set every wake_up
pm_tim_recover_handler_t    pm_tim_recover;

_attribute_data_retention_  unsigned char PA5_PA6_DEEPSLEEP_LOW_LEVEL_WAKEUP_EN = 1;
_attribute_data_retention_  unsigned int RAM_CRC_EN_16KRAM_TIME = 2500*16*1000;
_attribute_data_retention_  unsigned int RAM_CRC_EN_32KRAM_TIME = 6*16*1000*1000;
unsigned char SOFT_START_DLY = 0x08;
unsigned int EARLYWAKEUP_TIME_US_DEEP = PM_DCDC_DELAY_DURATION + 32 + (0x08*62);
/* wakeup source :is_deepretn_back,is_pad_wakeup,wakeup_src*/
_attribute_aligned_(4) pm_para_t pmParam;
/* enable status :conn_mark,ext_cap_en,pad32k_en,pm_enter_en */
_attribute_data_retention_	unsigned int 		tick_32k_calib;
_attribute_data_retention_	unsigned int 		tick_cur;
_attribute_data_retention_	unsigned int 		tick_32k_cur;
/* tick :tick_sysClk,tick_32k,recover_flag */
#if (PM_TIM_RECOVER_MODE)
	_attribute_data_retention_	_attribute_aligned_(4) pm_tim_recover_t	pm_timRecover;
#endif

_attribute_data_retention_	suspend_handler_t	func_before_suspend = 0;

void bls_pm_registerFuncBeforeSuspend (suspend_handler_t func )
{
	func_before_suspend = func;
}



/**
 * @brief     this function servers to get data(BIT0~BIT31) from EFUSE.
 * @param[in] none
 * @return    data(BIT0~BIT31)
 */
unsigned int  pm_get_info0(void)
{
	unsigned int efuse_info;
	write_reg8(0x74, 0x62);
	efuse_info= read_reg32(0x48);
	write_reg8(0x74, 0x00);
	return efuse_info ;
}

/**
 * @brief     this function servers to get data(BIT32~BIT63) from EFUSE.
 * @param[in] none
 * @return    data(BIT32~BIT63)
 */
unsigned int  pm_get_info1(void)
{
	unsigned int efuse_info;
	write_reg8(0x74, 0x62);
	efuse_info = read_reg32(0x4c);
	write_reg8(0x74, 0x00);
	return efuse_info ;
}

_attribute_ram_code_ _attribute_no_inline_   __attribute__((naked))  void  write_pm_start_reg(void)
{
	//write_reg8(0x6f,0x81);
	//for(volatile u8 i = 0; i<0x30; i++);
 asm volatile (
		 "tloadr	r3, [pc, #36];"
		 "tmovs		r2, #129;"
		 "tsub		sp, #4;"
		 "tnop;"
		 "tnop;"
		 "tstorerb	r2, [r3, #0];"
		 "tmov		r3, sp;"
		 "tadd		r3, #3;"
		 "tmovs		r2, #0;"
		 "tj		write_pm_start_reg+0x18+4;"
		 "tloadrb	r2, [r3, #0];"
		 "tadd		r2, #1;"
		 "tshftl	r2, r2, #24;"
		 "tshftr	r2, r2, #24;"
		 "tstorerb	r2, [r3, #0];"
		 "tloadrb	r2, [r3, #0];"
		 "tcmp		r2, #47	;"
		 "tjls		write_pm_start_reg+0x10+4;"
		 "tadd		sp, #4;"
		 "tjex		lr;"
		 ".word(0x0080006f);"
  );

}

/**
 * @brief     this function servers to wait bbpll clock lock.
 * @param[in] none
 * @return    none
 */
_attribute_ram_code_ void pm_wait_bbpll_done(void)
{
	unsigned char ana_81 = analog_read(0x81);
	analog_write(0x81, ana_81 | BIT(6));
	for(unsigned char j = 0; j < 3; j++)
	{
		for(volatile unsigned char i = 0; i < 30; i++){//20us
			asm("tnop");
			//if(BIT(5) != analog_read(0x88) & BIT(7)){}
		}
		if(BIT(5) == (analog_read(0x88) & BIT(5)))
		{
			analog_write(0x81, ana_81 & 0xbf);
			break;
		}
		else
		{
			if(j == 0){
				analog_write(0x01, 0x4e);
			}
			else if(j == 1){
				analog_write(0x01, 0x4b);
			}
			else{
				analog_write(0x81, ana_81 & 0xbf);
			}
		}
	}
}

/**
 * @brief     this function srevers to start sleep mode.
 * @param[in] none
 * @return    none
 */
_attribute_ram_code_ _attribute_no_inline_ void  sleep_start(void)
{

	analog_write(0x34,(0x80 | (ZB_POWER_DOWN ? BIT(0) : 0) | (PA5_PA6_DEEPSLEEP_LOW_LEVEL_WAKEUP_EN ? BIT(1) : 0) | (AUDIO_POWER_DOWN ? BIT(2) : 0)));
											//BIT<2:0> audio/USB/ZB  power down, save 50uA +

	analog_write(0xe4,0x10);  //<4>: power down vmid(audio enable)  avoid current leakage, save 2uA
	analog_write(0x82,0x85);  //power down signal of bbpll to avoid current leakage(confired by wenfeng 20191017)


//	DBG_CHN0_LOW;

	volatile unsigned int i;

/*************************************************************************
 * Sleep start and Flash Power down/on, can not insert any other code here
*************************************************************************/
#if FLASH_DEEP_EN
    write_reg8(0x00d, 0);
    write_reg8(0x00c, 0xb9);
    for(i = 0; i < 2; i++); //1440ns when 32M clk
    write_reg8(0x00d, 1);

    write_reg8(0x5a1, 0x00); //MSPI ie disable
#endif

    write_pm_start_reg();   //sleep  core_6f = 0x81;

    //Attention: If Execute code from flash, MCU will die cause flash not alive now

#if FLASH_DEEP_EN
    write_reg8(0x5a1, 0x0f); //MSPI ie enable

    write_reg8(0x00d, 0);
    write_reg8(0x00c, 0xab);
    for(i = 0; i < 2; i++); //1440ns when 32M clk
    write_reg8(0x00d, 1);
#endif
/**************************************************************************/


//    DBG_CHN0_HIGH;





	//must to set xo_quick_settle with manual and wait it stable(added by Yi Bao,confired by wenfeng 20191015) TODO
	unsigned char ana_2c = analog_read(0x2c);
	analog_write(0x2c, ana_2c | FLD_AUTO_PD_XTAL_QUICK_RST);   //0x2c<5>: xo_quick_rst
	analog_write(0x2c, ana_2c & ~FLD_AUTO_PD_XTAL_QUICK_RST);	//<5> 1b'1: xtal 24M quick settle count

	unsigned char ana_05 = analog_read(0x05);
	analog_write(0x05, ana_05 | 0x08);   //0x05<3>: 24M_xtl_pd
	analog_write(0x05, ana_05 & 0xf7);   //<3>1b'0: Power up 24MHz XTL oscillator
										 //<3>1b'1: Power down 24MHz XTL oscillator




	//XTAL oscillation Manual mode success, "PM_XTAL_MANUAL_MODE_DELAY" delay timing begin

//	DBG_CHN1_HIGH;

	analog_write(0x82,0x65);  //power up signal of bbpll. here should be wait 30us, but from here to change CLK may cost
							  //about 48us(cpu_get_32k_tick (void) may cost about 30~40 us), so here is not wait.

    analog_write(0xe4,0x00);//<4>: power on vmid(audio enable)
    analog_write(0x34,0x80);  //BIT<2:0> audio/USB/ZB  power on


#if(PM_XTAL_MANUAL_MODE_DELAY == 100)

#elif (PM_XTAL_MANUAL_MODE_DELAY == 200)
    for(i = 0; i < 300; i++){//200us
    	asm("tnop");
    }
#else

#endif


	//"PM_XTAL_MANUAL_MODE_DELAY" delay timing end

//    DBG_CHN1_LOW;

    //check 3 times for safety
	while( BIT(7) != (analog_read(0x88) & (BIT(7)))); //0x88<7>: xo_ready_ana
	while( BIT(7) != (analog_read(0x88) & (BIT(7)))); //0x88<7>: xo_ready_ana
	while( BIT(7) != (analog_read(0x88) & (BIT(7)))); //0x88<7>: xo_ready_ana

	pm_wait_bbpll_done();
}

/**
 * @brief     this function servers to start reboot.
 * @param[in] none
 * @return    none
 */
_attribute_ram_code_ void start_reboot(void)
{
	if(blt_miscParam.pad32k_en){
		analog_write(SYS_DEEP_ANA_REG, analog_read(SYS_DEEP_ANA_REG) & (~SYS_NEED_REINIT_EXT32K)); //clear
	}

	irq_disable ();
	REG_ADDR8(0x6f) = 0x20;  //reboot
	while (1);
}

/**
 * @brief   This function serves to get the 32k tick.
 * @param   none
 * @return  variable of 32k tick.
 */

_attribute_ram_code_ unsigned int pm_get_32k_tick(void)
{
    unsigned int t0 = 0;
    unsigned int t1 = 0;
    unsigned int n = 0;

    while (1) {

        t0 = t1;
        t1 = analog_read(0x43);
        t1 <<= 8;
        t1 += analog_read(0x42);
        t1 <<= 8;
        t1 += analog_read(0x41);
        t1 <<= 8;
        t1 += analog_read(0x40);
        if (n)
        {
            if ((unsigned int)(t1 - t0) < 2) {
                return t1;
            }
            else if ( (t0^t1) == 1 ) {
                return t0;
            }
        }
        n++;
    }
    return t1;
}

/**
 * @brief   This function serves to get the 32k tick.
 * @param   none
 * @return  variable of 32k tick.
 */
_attribute_ram_code_ _attribute_no_inline_ unsigned int cpu_get_32k_tick (void)
{
//	analog_write(0x44, analog_read(0x44) | BIT(1));				//clear timer wakeup flag(can not clear when read 32k)
	reg_system_irq_mask &= (~BIT(2));   						//disable system timer irq
	//system_timer and 32kshould_cal should be disable before suspend(added by Yi Bao, confired by junwen and zhidong 20191015)
	reg_system_ctrl &= (~(FLD_SYSTEM_32K_WR_EN|FLD_SYSTEM_TIMER_AUTO_EN|FLD_SYSTEM_TIMER_MANUAL_EN|FLD_SYSTEM_32K_CAL_EN));             // <0>: 1- write , 0- read
	while((reg_system_status & FLD_SYSTEM_32K_TIMER_UPDATE_RD));
	while(!(reg_system_status & FLD_SYSTEM_32K_TIMER_UPDATE_RD));
	reg_system_ctrl |= FLD_SYSTEM_TIMER_AUTO_EN;
 	return reg_system_32k_tick_rd;
}


/**
 * @brief  This function serves to set the 32k tick.
 * @param  none.
 * @return none.
 */
_attribute_ram_code_ void cpu_set_32k_tick(unsigned int tick)
{

	reg_system_ctrl |= FLD_SYSTEM_32K_WR_EN;  	// <0>: 1- write , 0- read
	while(reg_system_status & FLD_SYSTEM_32K_TIMER_BUSY);

	reg_system_32k_tick_wt = tick;

	reg_system_status = FLD_SYSTEM_32K_TIMER_SYCN_TRIG;

	asm("tnop");asm("tnop");asm("tnop");asm("tnop");
	asm("tnop");asm("tnop");asm("tnop");asm("tnop");
	asm("tnop");asm("tnop");asm("tnop");asm("tnop");
	asm("tnop");asm("tnop");asm("tnop");asm("tnop");
	while(reg_system_status & FLD_SYSTEM_32K_TIMER_SYCN_TRIG);
}
/**
 * @brief   This function serves to wake up cpu from stall mode by timer0.
 * @param[in]   tick - capture value of timer0.
 * @return  none.
 */
void cpu_stall_wakeup_by_timer0(unsigned int tick)
{
   /*Write 0x00 here may cause problem, it is removed to blt_sleep_wakeup*/
   //write_reg8(0x6f,0x00);//clear bit[0] suspend enable

    reg_tmr0_tick = 0;

    reg_tmr0_capt = tick;
    reg_tmr_ctrl8 |= FLD_TMR0_EN;//enable TIMER0,mode:using sclk
    reg_mcu_wakeup_mask |= FLD_IRQ_TMR0_EN;//timer0 mask enable
    reg_tmr_sta = FLD_TMR_STA_TMR0; // clean interrupt

    write_reg8(0x6f,0x80);//stall mcu
    asm("tnop");
    asm("tnop");

    reg_tmr_sta = FLD_TMR_STA_TMR0; // clean interrupt
    reg_tmr_ctrl8 &= ~FLD_TMR0_EN;//disable TIMER0
}

/**
 * @brief   This function serves to wake up cpu from stall mode by timer1.
 * @param   tick - capture value of timer1.
 * @return  none.
 */
void cpu_stall_wakeup_by_timer1(unsigned int tick)
{
   /*Write 0x00 here may cause problem, it is removed to blt_sleep_wakeup*/
   //write_reg8(0x6f,0x00);//clear bit[0] suspend enable

    reg_tmr1_tick = 0;

    reg_tmr1_capt = tick;
    reg_tmr_ctrl8 |= FLD_TMR1_EN;//enable TIMER1,mode:using sclk
    reg_mcu_wakeup_mask |= FLD_IRQ_TMR1_EN;//timer1 mask enable
    reg_tmr_sta = FLD_TMR_STA_TMR1; // clean interrupt

    write_reg8(0x6f,0x80);//stall mcu
    asm("tnop");
    asm("tnop");

    reg_tmr_sta = FLD_TMR_STA_TMR1; // clean interrupt
    reg_tmr_ctrl8 &= ~FLD_TMR1_EN;//disable TIMER1
}

/**
 * @brief   This function serves to wake up cpu from stall mode by timer2.
 * @param[in]   tick - capture value of timer2.
 * @return  none.
 */
void cpu_stall_wakeup_by_timer2(unsigned int tick)
{
   /*Write 0x00 here may cause problem, it is removed to blt_sleep_wakeup*/
   //write_reg8(0x6f,0x00);//clear bit[0] suspend enable

    reg_tmr2_tick = 0;

    reg_tmr2_capt = tick;
    reg_tmr_ctrl8 |= FLD_TMR2_EN;//enable TIMER1,mode:using sclk
    reg_mcu_wakeup_mask |= FLD_IRQ_TMR2_EN;//timer1 mask enable
    reg_tmr_sta = FLD_TMR_STA_TMR2; // clean interrupt

    write_reg8(0x6f,0x80);//stall mcu
    asm("tnop");
    asm("tnop");

    reg_tmr_sta = FLD_TMR_STA_TMR2; // clean interrupt
    reg_tmr_ctrl8 &= ~FLD_TMR2_EN;//disable TIMER1
}

#if 0
/**
 * @brief   This function serves to wake up cpu from stall mode by timer1 or RF TX done irq.
 * @param   WakeupSrc  - timer1.
 * @param   IntervalUs - capture value of timer1.
 * @param   sysclktick - tick value of per us based on system clock.
 * @return  none.
 */
_attribute_ram_code_ unsigned int cpu_stall(int WakeupSrc, unsigned int IntervalUs,unsigned int sysclktick)
{
    unsigned int wakeup_src = 0;

    if (IntervalUs) {
        reg_tmr1_tick = 0;
        reg_tmr1_capt = IntervalUs * sysclktick;
        reg_tmr_sta = FLD_TMR_STA_TMR1; // clean interrupt
        reg_tmr_ctrl8 &= (~FLD_TMR1_MODE); //select mode0: using sclk
        reg_tmr_ctrl8 |= FLD_TMR1_EN; //enable TIMER1
    }
    REG_ADDR32(0x78) |= WakeupSrc;  //FLD_IRQ_TMR1_EN | FLD_IRQ_ZB_RT_EN

    reg_irq_mask &= (~FLD_IRQ_ZB_RT_EN);
    reg_irq_mask &= (~FLD_IRQ_TMR1_EN);

    write_reg8(0x6f, 0x80); //stall mcu
    asm("tnop");
    asm("tnop");

    if (IntervalUs) {
        reg_tmr1_tick = 0;
        reg_tmr_ctrl8 &= (~FLD_TMR1_EN); //disable TIMER1
    }

    //store the wakeup source
    wakeup_src = REG_ADDR32(0x40);

    //clear the source
    reg_tmr_sta = FLD_TMR_STA_TMR1; // clear timer1 irq source
    write_reg16(0xf20, 0xffff); //clear rf irq status

    return wakeup_src;
}
#endif

/**
 * @brief   	This function serves to kick external crystal.
 * @param[in]   kick_ms - duration of kick.
 * @return  	none.
 */
static _attribute_no_inline_ void pwm_kick_32k_pad(unsigned int kick_ms)
{
	//2.set pc3 as pwm output
	write_reg8(0x66,  0x43); //sys clk use 16M crystal
	write_reg8(0x596, 0xf7); //pc3 as pwm func
	write_reg32(0x798,0x20001); //pwm1's high time or low time && pwm1's cycle time
	write_reg8(0x780, 0x02); //enable pwm1
	write_reg8(0x782, 0xf3); //pwm clk div

	//3.wait for PWM wake up Xtal

	sleep_us(kick_ms*1000);

	//4.Recover PC3 as Xtal pin
	write_reg8(0x66,   0x06); //default value
	write_reg8(0x596,  0xff); //default value
	write_reg32(0x798, 0x00); //default value
	write_reg8(0x780,  0x00); //default value
	write_reg8(0x782,  0x00); //default value
}

/**
 * @brief   	This function serves to delay 13ms based on 24M rc clk.
 * @param[in]   none.
 * @return  	none.
 */
__attribute__((optimize("-O0")))
_attribute_no_inline_ void soft_reboot_dly13ms_use24mRC(void)
{
	for(int i = 0; i < 15500; i++); //about 13ms when 24M rc clk
}

/**
 * @brief   	This function serves to kick external crystal.
 * @param[in]   times - the frequency of being kicked by pwm.
 * @return  	0-kick success; 1-kick fail.
 */
static _attribute_no_inline_ unsigned char pwm_kick_32k_pad_times(unsigned int times)
{
	if(times){

#if 0 //debug code
		write_reg8(0x73,0x07);
		write_reg8(0x586,0x1d);
#endif
		//1. select 32k xtal
		analog_write(0x2d, 0x95);//32k select:[7]:0 sel 32k rc,1:32k XTAL

		#if 0 //must close, reason is as follows:
			/*
			 * This problem occurs with suspend and deep and deep retention. When the power supply voltage is low, suspend/deep/deep retention cannot
			 * be reset within 12ms, otherwise softstart will work. However, there is not enough delay to wait for softstart to complete. This delay
			 * will be postponed to the code execution area and it will not be able to handle larger code, otherwise it will be dropped by 1.8v, load
			 * error, and finally stuck.(Root: DCDC dly depends on the 32k rc clock, so the 32k rc power supply can't be turned off here.)
			 */
			analog_write(0x05, 0x01);//Power down 32KHz RC,  Power up [32KHz crystal, 24MHz RC, 24MHz XTAL,DCDC, VBUS_LDO, baseband pll LDO]
		#else
			analog_write(0x05, 0x00);//Power up 32KHz RC,  Power up 32KHz crystal
		#endif

		//analog_write(0x03,0x4f); //<7:6>current select(default value, not need)

		int last_32k_tick;
		int curr_32k_tick;
		int i = 0;
		for(i = 0; i< times; i++){

			//After 10ms, the external 32k crystal clk is considered stable(when using PWM to accelerate the oscillation process)
			pwm_kick_32k_pad(10);//PWM kick external 32k pad (duration 10ms)

			//Check if 32k pad vibration and basically works stably
			last_32k_tick = pm_get_32k_tick();

			sleep_us(305);//for 32k tick accumulator, tick period: 30.5us, dly 10 ticks

			curr_32k_tick = pm_get_32k_tick();

			if(last_32k_tick != curr_32k_tick){ //pwm kick 32k pad success
				return 0;
			}
		}

		#if (0) //blt_sdk_main_loop: check if 32k pad stable, if not, reboot MCU

			if(i >= times){
				analog_write(SYS_DEEP_ANA_REG, analog_read(SYS_DEEP_ANA_REG) & (~SYS_NEED_REINIT_EXT32K)); //clr
				start_reboot(); //reboot the MCU
			}
		#endif
	}
	return 1;
}

/**
 * @brief      This function configures a GPIO pin as the wakeup pin.
 * @param[in]  Pin - the pin needs to be configured as wakeup pin
 * @param[in]  Pol - the wakeup polarity of the pad pin(0: low-level wakeup, 1: high-level wakeup)
 * @param[in]  En  - enable or disable the wakeup function for the pan pin(1: Enable, 0: Disable)
 * @return     none
 */
void cpu_set_gpio_wakeup (GPIO_PinTypeDef pin, GPIO_LevelTypeDef pol, int en)
{
	///////////////////////////////////////////////////////////
	// 		  PA[7:0]	    PB[7:0]		PC[7:0]		PD[7:0]
	// en: 	ana_21<7:0>	 ana_22<7:0>  ana_23<7:0>  ana_24<7:0>
	// pol:	ana_27<7:0>	 ana_28<7:0>  ana_29<7:0>  ana_2a<7:0>
    unsigned char mask = pin & 0xff;
	unsigned char areg;
	unsigned char val;

	////////////////////////// polarity ////////////////////////

	areg = ((pin>>8) + 0x21);
	val = analog_read(areg);
	if (pol) {
		val &= ~mask;
	}
	else {
		val |= mask;
	}
	analog_write (areg, val);

	/////////////////////////// enable /////////////////////
	areg = ((pin>>8) + 0x27);
	val = analog_read(areg);
	if (en) {
		val |= mask;
	}
	else {
		val &= ~mask;
	}
	analog_write (areg, val);
}

/**
 * @brief     This function servers to set the match value for MDEC wakeup.
 * @param[in] none
 * @return    none
 */
void cpu_set_mdec_value_wakeup(unsigned char value)
{
	//When use the MDEC as the wake_up source,should set the 0x17[3:0] to the match value(confirmed by jianzhi).
	analog_write(mdec_match_addr,((analog_read(mdec_match_addr) & (~0x0f)) | value));
}

/**
 * @brief   	This function serves to initiate the cpu after power on or deepsleep mode.
 * @param[in]   none.
 * @return  	none.
 */
static _attribute_no_inline_ void cpu_wakeup_no_deepretn_back_init(void)
{

	//Set 32k clk src: external 32k crystal, only need init when deep+pad wakeup or 1st power on
	if(blt_miscParam.pad32k_en){

		//in this case: ext 32k clk was closed, here need re-init.
		//when deep_pad wakeup or first power on, it needs pwm acc 32k pad vibration time(dly 10ms)
		{
			pwm_kick_32k_pad_times(10);   //kick success if the return value is 0, return 1 mean kick fail;if kick failed , can call
										  //void check_32k_clk_stable(void)(the API used in ble sdk) to solve it or solve it by self.

		}
	}
	else{
		//default 32k clk src: internal 32k rc, here can be optimized
		//analog_write(0x2d, 0x15); //32k select:[7]:0 sel 32k rc,1:32k pad
		//analog_write(0x05, 0x02); //Power down 32k crystal,  Power up [32KHz RC, 24MHz RC, 24MHz XTAL,DCDC, VBUS_LDO, baseband pll LDO]

		rc_32k_cal();  //6.69 ms/6.7 ms
	}

	//////////////////// get Efuse bit32~63 info ////////////////////
	unsigned int  efuse_32to63bit_info = pm_get_info1();

	/////////////  ADC vref calib  //////////////////
	if((efuse_32to63bit_info&0xff))//if there has cal_adc_ref value.
	{
		if(adc_vref_cfg.adc_calib_en)
		{
			unsigned char adc_ref_idx = efuse_32to63bit_info & 0xff; //ADC Ref: efuse bit32~bit39 8bits.
			adc_vref_cfg.adc_vref = 1047 + adc_ref_idx ; //ADC ref voltage: g_adc_vref (unit: mV)
		}
	}
}

/**
 * @brief   This function serves to initialize MCU
 * @param   none
 * @return  none
 */
_attribute_ram_code_
void cpu_wakeup_init(void)    //must on ramcode
{
	POWER_MODE_TypeDef power_mode = DCDC_MODE;	//default 2.1
	XTAL_TypeDef xtal = EXTERNAL_XTAL_24M;		//default
	write_reg8(0x60, 0x00); 	//poweron_dft: 0x7c -> 0x00
	write_reg8(0x61, 0x00); 	//poweron_dft: 0xff -> 0x00
	write_reg8(0x62, 0x08); 	//poweron_dft: 0xcf -> 0xc8
								//<7>:mcic1,R  	default:1
								//<6>:risc1,R  	default:1
								//<3>:trng  	default:1,

	write_reg8(0x63, 0xff); 	//poweron_dft: 0x83 -> 0xff
	write_reg8(0x64, 0xff); 	//poweron_dft: 0x00 -> 0x9f
								//<5>/<6> undefined ,default:0

	write_reg8(0x65, 0xf7); 	//poweron_dft: 0x30 -> 0x37
								//<0>:aif -> disable 32k for qdec
								//<5/6>:undefined,default:0

	//when load code twice without power down dut, dut will use crystal clock in here,
	//xo_quick_settle manual mode need to use in RC colck. TODO
	write_reg8(0x66, 0x06);		//poweron_dft :set RC clock as system clock.

	analog_write(0x06, 0xce);  	//poweron_dft: 0xcf -> 0xce
								//<0>: power on bbpll LDO
	                          	//<4>: power on otp LDO

	//xo_quict_settle with auto will cause the pragram crash in high temperature, must
	//be set to manual mode.(add by Yi Bao, confirmed by wenfeng 20190911)
	unsigned char  ana_2c = analog_read(0x2c);
	unsigned char  ana_05 = analog_read(0x05);
	analog_write(0x2c, ana_2c | 0x20);   //0x2c<5>: xo_quick_rst
	analog_write(0x2c, ana_2c & 0xdf);	//<5> 1b'1: xtal 24M quick settle count
	analog_write(0x05, ana_05 | 0x08);   //0x05<3>: 24M_xtl_pd
	analog_write(0x05, ana_05 & 0xf7);   //<3>1b'0: Power up 24MHz XTL oscillator
										 //<3>1b'1: Power down 24MHz XTL oscillator


#if (!XTAL_READY_CHECK_TIMING_OPTIMIZE)
	while( BIT(7) != (analog_read(0x88) & (BIT(7)))); //0x88<7>: xo_ready_ana
#endif

	analog_write(0x82, 0x65);  	//poweron_dft: 0x05 -> 0x25
								//<3:0>:bbpll setting
								//<5>: enable 48M clock to digital core
								//<6>: enable signal of 24M clock to sar

	analog_write(0x34, 0x80);	//poweron_dft: 0x07 -> 0x80
								//<0>: power on baseband
								//<1>: power on usb
								//<2>: power on audio
								//<7>: enable change power sequence clk
	//When using the BDT tool to download a program through USB,
	//if the dp pull-up is turned off, the device will be disconnected,so the dp pull-up is set to be keep
	//modify by kaixin(2019.12.27)
	analog_write(0x0b, (analog_read(0x0b) & 0x80) | (0x38));  //default: 0x7b
									//<7>:enable_b signal of  1.5K pullup resistor for DP PAD--keep
									//<6>:enable signal of 1M pullup resistor for mscn PAD,avoid current leakage 1->0
		                            //<5:4>: reference scale select 11->11
		                            //<1:0>: power on native 1.8v/1.4v 11->00
	if(0xff == read_reg8(0x7d)){
		analog_write(0x17, 0x02);   //poweron_dft: 0xf2,  <7:4>=0000,enable pullup for sensor to avoid current leakage
									//<7:4>=0000 : GPIO bug only need in A0, A1 should delete.
	}

	analog_write(0x8c, 0x02);  	//poweron_dft: 0x00,  <1> set 1: reg_xo_en_clk_ana_ana, to enable external 24M crystal

//	analog_write(0x00, 0x98);  	//poweron_dft: 0x18,  <7> MSB of ret_ldo_trim,  0: 0.8-1.15V; 1: 0.6-0.95V
//	analog_write(0x02, 0xa0);  	//poweron_dft: 0xa4,  <2:0> ret_ldo_trim,  set 0x00: 0.6V
	analog_write(0x00, analog_read(0x00) & 0x7f);  	//poweron_dft: 0x18,  <7> MSB of ret_ldo_trim,  0: 0.8-1.15V; 1: 0.6-0.95V
	analog_write(0x02, 0xa4);  	//poweron_dft: 0xa4,  <2:0> ret_ldo_trim,  set 0x04: 1.00V

	analog_write(0x01, 0x4d);//pragram can crash in high temperature, ana_01 and pm_wait_bbpll_done() is order to solve this problem.

	if(xtal == EXTERNAL_XTAL_48M){		//use external 48M crystal instead of external 24M crystal
		analog_write(0x2d, analog_read(0x2d) | BIT(6));  	//poweron_dft: 0x15, <6>: 0-24M XTAL , 1- 48M XTAL
	}

	if((0xff == read_reg8(0x7d)) && (DCDC_LDO_MODE == power_mode)){
		analog_write(0x0a, power_mode | BIT(2));    //poweron_dft: 0x40,  <1:0> 00-LDO, 01- LDO-DC, 11- DCDC
	}												//when use the LDO_1P4_DCDC_1P8, should use synchronize mode(0x0a<2> 1-synchronize mode ,
	else{											//0-asynchronize mode) to avoid the current abnormal in A0.
		analog_write(0x0a, power_mode);
	}

	if(LDO_MODE != power_mode)
	{
		analog_write(0x0c, 0xa4);		//poweron_dft: 0xc4 1.4v voltage turn dowm 100mv
	}

	analog_write(0x27, 0x00);   //poweron_dft: 0x00 PA gpio wakeup disable
	analog_write(0x28, 0x00);   //poweron_dft: 0x00 PB gpio wakeup disable
	analog_write(0x29, 0x00);   //poweron_dft: 0x00	PC gpio wakeup disable
	analog_write(0x2a, 0x00);   //poweron_dft: 0x00 PD gpio wakeup disable

	write_reg32(0xc40, 0x04040404); //poweron_dft: 0x00000000 -> 0x04040404
	write_reg32(0xc44, 0x04040404); //poweron_dft: 0x00000000 -> 0x04040404
	write_reg8(0xc48, 0x04);        //poweron_dft: 0x00 -> 0x04

#if (XTAL_READY_CHECK_TIMING_OPTIMIZE)
	//xo_ready check should be done after Xtal manual on_off, we put it here to save code running time, code running time between
	// Xtal manual on_off and xo_ready check can be used as Xtal be stable timimg
	while( BIT(7) != (analog_read(0x88) & (BIT(7)))); //0x88<7>: xo_ready_ana
#endif

	pm_wait_bbpll_done();
	
	if(analog_read(REG_DEEP_BACK_FLAG) == DATA_STORE_FLAG){
		if(!(analog_read(0x7f) & 0x01)){
			pmParam.back_mode = BACK_FROM_DEEP_RETENTION;
		}else{
			pmParam.back_mode = BACK_FROM_DEEP;
		}
	}else{
		pmParam.back_mode = BACK_FROM_REPOWER;
	}
	if(pmParam.back_mode){

		pmParam.wakeup_src = analog_read(0x44);
		if ( (analog_read(0x44) & WAKEUP_STATUS_TIMER_PAD ) == WAKEUP_STATUS_PAD)  //pad, no timer
		{
			pmParam.is_pad_wakeup = 1;
		}else{
			pmParam.is_pad_wakeup = 0;
		}
	}
	if(pmParam.back_mode == BACK_FROM_DEEP_RETENTION){
		unsigned int deepRet_32k_tick = cpu_get_32k_tick();

		unsigned int deepRet_tick = pm_tim_recover(deepRet_32k_tick);
		reg_system_tick = deepRet_tick;

		reg_system_ctrl |= FLD_SYSTEM_TIMER_MANUAL_EN|FLD_SYSTEM_32K_CAL_EN;		
	}
	else
	{
		reg_system_ctrl |= FLD_SYSTEM_TIMER_MANUAL_EN|FLD_SYSTEM_32K_CAL_EN;

		cpu_wakeup_no_deepretn_back_init(); // to save ramcode
	}

	reg_system_irq_mask |= BIT(2);   //enable system timer irq
	//core_c20/c21 power on default all enable, so we disable them first,  then if use, enable them by manual
	//note that: PWM/RF Tx/RF Rx/AES code/AES dcode dma may be affected by this, you must handle them when initialization
	reg_dma_chn_en = 0;
	reg_dma_chn_irq_msk = 0;
	reg_gpio_wakeup_irq |= (FLD_GPIO_CORE_WAKEUP_EN | FLD_GPIO_CORE_INTERRUPT_EN);
}



#if PM_LONG_SLEEP_WAKEUP_EN
//definitions for U64 operations
typedef struct {            /*The result of the multiplication with 64 bits.*/
    unsigned int High32;    /*The high 32 bits.*/
    unsigned int Low32;     /*The low 32 bits.*/
} U64_VALUE_t;

/** Get the high word and low word of a variable with 32 bits.*/
#define LOWORD(p)           ((p) & 0x0000FFFFU)
#define HIWORD(p)           (((p) >> 16) & 0x0000FFFFU)

#define MAXVALUEOF32BITS    (0xFFFFFFFFU)

/**
* @brief Calculate the multiplication between two factors,
*        and one factor is the high 32 bit of one 64 bit unsigned interger and the other factor is 32 bit unsigned interger.
* @param Factor64H           [IN]The high 32 bit of one 64 bit unsigned interger.
*                               - Type: unsigned int  \n
*                               - Range: [0,0xFFFFFFFF].\n
* @param Factor32            [IN]A 32 bit unsigned interger.
*                               - Type: unsigned int \n
*                               - Range: [0,0xFFFFFFFF].\n
* @param Result          [OUT]A pointer to the production between the two factors.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
*
* @return The overflow status of the multiplication.
*                               - Type: unsigned char \n
*                               - Range: (0, 1). \n
*
*/
__attribute__((section(".ram_code"))) static unsigned char TN_Cal64Multi32_H(
    const unsigned int Factor64H,                /*The high 32 bit of one 64 bit unsigned interger.*/
    const unsigned int Factor32,               /*A 32 bit unsigned interger.*/
    U64_VALUE_t * const Result)       /*A pointer to the production between the two factors.*/

{
    unsigned char IsOverFlow = 0;   /*The overflow state of the product.*/
    if (0 != Result) {    /*Check the output buffer.*/
        /*Initiate the output buffer.*/
        Result->High32 = 0U;
        Result->Low32 = 0U;

        if ((0U == Factor64H) || (0U == Factor32)) {/*Check whether one of the multiplier factor is zero.*/
            /*Set the product to zero when one of the multiplier factor is zero.*/
            Result->High32 = 0U;
            Result->Low32 = 0U;
        }
        else {
            if ((Factor64H * Factor32) / Factor32 == Factor64H) {/*Check whether the product is not overflow.*/
                /*Calculate the product when the product is not overflow*/
                Result->High32 = 0U;
                Result->Low32 = Factor64H * Factor32;
            }
            else {
                /*Set the overflow flag of the product and print the overflow message for debug.*/
                IsOverFlow = 1;
            }
        }
    }
    return IsOverFlow;
}

/**
* @brief Calculate the multiplication between two factors,
*        and one factor is the low 32 bit of one 64 bit unsigned interger and the other factor is 32 bit unsigned interger.
* @param Factor64L             [IN]The low 32 bit of one 64 bit unsigned interger
*                               - Type: unsigned int  \n
*                               - Range: [0,0xFFFFFFFF].\n
* @param Factor32               [IN]A 32 bit unsigned interger.
*                               - Type: unsigned int \n
*                               - Range: [0,0xFFFFFFFF].\n
* @param Result          [OUT]A pointer to the production between the two factors.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
*
* @return none
*/
__attribute__((section(".ram_code"))) static void TN_Cal64Multi32_L(
    const unsigned int Factor64L,            /*The low 32 bit of one 64 bit unsigned interger.*/
    const unsigned int Factor32,              /*An 32 bit unsigned interger.*/
    U64_VALUE_t * const Result)   /*A pointer to the production between the two factors.*/

    {
    unsigned int  A0    = 0U;   /*The low 16 bits of the multiplier factor with 64 bits.*/
    unsigned int  A1    = 0U;   /*The 16~31 bits of the multiplier factor with 64 bits.*/
    unsigned int  B0    = 0U;   /*The low 16 bits of the multiplier factor with 32 bits.*/
    unsigned int  B1    = 0U;   /*The 16~31 bits of the multiplier factor with 32 bits.*/
    unsigned int  A0B0  = 0U;   /*The product between A0 and B0.*/
    unsigned int  A1B0  = 0U;   /*The product between A1 and B0.*/
    unsigned int  A0B1  = 0U;   /*The product between A0 and B1.*/
    unsigned int  A1B1  = 0U;   /*The product between A1 and B1.*/
    if (0 != Result)   /*Check the output buffer.*/
    {
        /*Initiate the output buffer.*/
        Result->High32 = 0U;
        Result->Low32 = 0U;
        if((0U == Factor64L) || (0U == Factor32)) /*Check whether one of the multiplier factor with low 32 bits is zero.*/
        {
            /*Set the product to zero when one of the multiplier factor with 32 bits is zero.*/
            Result->High32 = 0U;
            Result->Low32 = 0U;
        }
        else
        {
            if((Factor32 * Factor64L )/Factor32   ==   Factor64L )/*Check whether the product between two multiplier factor with 32 bits is not oveeflow.*/
            {
                /*Calculate the product when the product is not overflow.*/
                Result->High32 = 0U;
                Result->Low32 = Factor64L  * Factor32;
            }
            else
            {
                /*Calculate the product when the product between two multiplier factor with 32 bits is not overflow.*/
                A0 = LOWORD( Factor64L );    /*Get the low 16 bits of the multiplier factor with 64 bits.*/
                A1 = HIWORD( Factor64L );    /*Get the 16~31 bits of the multiplier factor with 64 bits.*/
                B0 = LOWORD( Factor32 );       /*Get the low 16 bits of the multiplier factor with 32 bits.*/
                B1 = HIWORD( Factor32 );       /*Get the 16~31 bits of the multiplier factor with 32 bits.*/
                /*Calculate the product high 32 bit.*/
                /* Factor64L * Factor32 = (A0+A1*2^16)*(B0+B1*2^16) = A0*B0 + (A0*B1+A1*B0)*2^16 + A1*B1*2^32 */
                A0B0 = A0 * B0;
                A1B0 = A1 * B0 + HIWORD( A0B0 );
                A0B1 = A0 * B1 + LOWORD( A1B0 );
                A1B1 = A1 * B1 + HIWORD( A1B0 ) + HIWORD( A0B1 );
                /*Calculate the product low 32 bit.*/
                Result->High32 = A1B1;
                Result->Low32 = Factor64L * Factor32;
            }
        }
    }

}

/**
* @brief   Calculate the addition between the two 64 bit factors.
* @param Factor32             [IN]A pointer to one factor of the addition.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param Factor32             [IN]A pointer to another factor of the addition.
*                               - Type: U64_VALUE_t *\n
*                               - Range: N/A.\n
* @param Result              [OUT]A pointer to the result of the addition.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
*
* @return The overflow status of the addition.
*                               - Type: unsigned char \n
*                               - Range: (0, 1). \n
*
*/
__attribute__((section(".ram_code"))) static unsigned char TN_Cal64Add(
    const U64_VALUE_t * const Factor1,    /*One factor of the addition.*/
    const U64_VALUE_t * const Factor2,    /*Another factor of the addition.*/
    U64_VALUE_t * const Result)           /*The result of the addition.*/

{
    unsigned char IsOverFlow = 0;   /*The overflow state of the product.*/
    if (0 != Result) {  /*Check the output buffer.*/
        /*Initiate the output buffer.*/
        Result->High32 = 0U;
        Result->Low32 = 0U;
        if ((0 != Factor1) && (0 != Factor2)) {  /*Check the input buffer.*/
            if( (Factor1->Low32) > ((unsigned int)(0xFFFFFFFFu) - (Factor2->High32)) ) { /*Check whether the high 32 bits of the product is overflow.*/
                /*Set the overflow flag of the product and print the overflow message for debug.*/
                IsOverFlow = 1;
            }
            else {
                /*Calculate the final product and reset the overflow state.*/
                Result->High32 = Factor2->High32 + Factor1->Low32;
                Result->Low32 = Factor2->Low32;
                IsOverFlow = 0;
            }
        }
    }
    return IsOverFlow;
}

/**
* @brief Calculate the multiplication between two factors, and one factor is 64 bit unsigned interger while the other factor 32 bit.
* @param U64Factor           [IN]One of the multiplier factor with 64 bits.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param U32Factor           [IN]One of the multiplier factor with 32 bits.
*                               - Type: unsigned int \n
*                               - Range: [0,0xFFFFFFFF]\n
* @param Result              [OUT]A pointer to the multiplication result with 64 bits.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
*
* @return The overflow status of the multiplication.
*                               - Type: unsigned char \n
*                               - Range: (0, 1). \n
*
*/
__attribute__((section(".ram_code"))) static unsigned char TN_UINT64Multiply(
    const U64_VALUE_t * const U64Factor,      /*One of the multiplier factor with 64 bits.*/
    const unsigned int U32Factor,             /*One of the multiplier factor with 32 bits.*/
    U64_VALUE_t * const Result)               /*The multiplication result with 64 bits.*/

{
    unsigned char IsOverFlow = 0;       /*The overflow state of the U64Factor.*/
    U64_VALUE_t Product64H_32 = {0};    /*The U64Factor between the high 32 bits of the multiplier factor with 64 bits and the multiplier factor with 32 bits.*/
    U64_VALUE_t Product64L_32 = {0};    /*The U64Factor between the low 32 bits of the multiplier factor with 64 bits and the multiplier factor with 32 bits.*/
    /*Initiate the output buffer.*/
    if (0 != Result) { /*Check the output buffer.*/
        Result->High32 = 0U;
        Result->Low32 = 0U;
        if (0 != U64Factor) {  /*Check the input buffer.*/
            if ((0U == U32Factor)||((0U == U64Factor->High32) && (0U == U64Factor->Low32))) { /*Check whether one of the multiplier factor is zero.*/
                /*The result is zero when one of the multiplier factor is zero.*/
                Result->High32 = 0U;
                Result->Low32 = 0U;
                IsOverFlow = 0;
            }
            else {
                /*Calculate the multiplication between the high 32 bits of the multiplier factor with 64 bits and the multiplier factor with 32 bits.*/
                IsOverFlow = TN_Cal64Multi32_H(U64Factor->High32,U32Factor,&Product64H_32);
                if (0 == IsOverFlow) {/*Check whether the multiplication between the high 32 bits of the 64 bits multiplier factor and the 32 bits multiplier factor is not overflow.*/
                    /*Calculate multiplication between the low 32 bits of the two multiplier factors.*/
                    TN_Cal64Multi32_L(U64Factor->Low32,U32Factor,&Product64L_32);
                    /*Calculate the addition between the two multiplication results calculating by the upper two functions.*/
                    IsOverFlow = TN_Cal64Add(&Product64H_32,&Product64L_32,Result);
                }
            }
            /*The U64Factor is zero when it is overflow.*/
            if(1 == IsOverFlow) {
                Result->High32 = 0U;
                Result->Low32 = 0U;
            }
        }
    }
    return IsOverFlow;
}

/**
* @brief Calculate the remainder with 64 bits.
* @param MulRes             [IN]The multiplication result.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param Dividend           [IN]The dividend.
*                               - Type: U64_VALUE_t *\n
*                               - Range: N/A.\n
*
* @return The remainder
*                               - Type: U64_VALUE_t \n
*                               - Range: N/A.\n
*
*/
__attribute__((section(".ram_code"))) static U64_VALUE_t TN_CalRemaider(const U64_VALUE_t MulRes,const U64_VALUE_t Dividend)
{
    U64_VALUE_t Remaider = {0};                  /*The division remainder with 64 bit.*/
    if( MulRes.Low32 <= Dividend.Low32)/*Do not need to carry calculation*/
    {
        Remaider.Low32 = Dividend.Low32 - MulRes.Low32;
        Remaider.High32 = Dividend.High32 - MulRes.High32;
    }
    else    /*Need to carry calculation*/
    {
        Remaider.Low32 = MAXVALUEOF32BITS - MulRes.Low32 + 1U + Dividend.Low32;
        Remaider.High32 = Dividend.High32 - MulRes.High32 - 1U;
    }
    return Remaider;
}

/**
* @brief Two numbers with 64 bit are divided by 2 respectively until the high 32 bit of the second number is zero.
* @param Number1             [IN]A pointer to the first number with 64 bit.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param Number2             [IN]A pointer to the second number with 64 bit.
*                               - Type: unsigned char *\n
*                               - Range: N/A.\n
* @param Number1Div          [OUT]A pointer to the first divided number which has been divided by 2.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param Number2Div          [OUT]A pointer to the second number which has been divided by 2.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
*
* @return none
*
*
*/
__attribute__((section(".ram_code"))) static void TN_Div2NumBy2(
    const U64_VALUE_t * const Number1,    /*A pointer to the first number with 64 bit.*/
    const U64_VALUE_t * const Number2,    /*A pointer to the second number with 64 bit.*/
    U64_VALUE_t * const Number1Div,       /*A pointer to the first number which has been divided by 2.*/
    U64_VALUE_t * const Number2Div)       /*A pointer to the second number which has been divided by 2.*/
{
    if ((0 != Number1Div) && (0 != Number2Div) && (0 != Number1) && (0 != Number2)) {/*Check the output buffer.*/
        /*Initiate the output buffer.*/
        Number1Div->Low32 = Number1->Low32;
        Number1Div->High32 = Number1->High32;
        Number2Div->Low32 = Number2->Low32;
        Number2Div->High32 = Number2->High32;
        while (0U != Number2Div->High32) {
                /*The high 32 bit and low 32 bit of the first number are divided by 2.*/
                Number1Div->Low32 >>= 1U;
                if ((Number1Div->High32 % 2U) > 0U) {
                    Number1Div->Low32 += 0x80000000U;
                }
                Number1Div->High32 >>= 1U;
                /*The high 32 bit and low 32 bit of the second number are divided by 2.*/
                Number2Div->Low32 >>= 1U;
                if((Number2Div->High32 %2U) > 0U) {
                    Number2Div->Low32 += 0x80000000U;
                }
                Number2Div->High32 >>= 1U;
        }
    }
}

/**
* @brief Calculate the quotient between the 64 bit dividend and 32 bit divisor. The value of the dividend and divisor are treated as unsigned interger.
*        The division used in this function is the standard algorithm used for pen-and-paper division of multidigit numbers expressed in decimal notation.
* @param U64Dividend         [IN]The dividend with 64 bit.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param U32Divisor          [IN]The divisor with 32 bit.
*                               - Type: unsigned int \n
*                               - Range: N/A.\n
*
* @return none
*
*/
__attribute__((section(".ram_code"))) static unsigned int TN_CalQuto(
    const U64_VALUE_t * const U64Dividend,    /*The dividend with 64 bit.*/
    const unsigned int U32Divisor)                 /*The divisor with 32 bit.*/
{
    unsigned int i = 0U; /*The index for the FOR loop.*/
    unsigned int l_uFlag=0U;  /*The most significant bit of the dividend which has been shifted.*/
    U64_VALUE_t l_DivdShift={0};   /*The dividend which has been shifted.*/
    unsigned int l_Quto = 0U;         /*The quotient.*/
    if (0 != U64Dividend) {/*Check the input buffer.*/
        l_DivdShift.High32 = U64Dividend->High32;
        l_DivdShift.Low32 = U64Dividend->Low32;
        for (i = 0U; i < 32U; i++) {
            l_uFlag = (unsigned int)l_DivdShift.High32 >> 31U;    /*The 'uFlag' is 0xffffffff when the most significant bit of the 'l_DivdShift.High32' is one, otherwise 0x00000000.*/
            /*Left-shift dividend by 1 bit.*/
            l_DivdShift.High32 = (l_DivdShift.High32 << 1)|(l_DivdShift.Low32 >> 31);
            l_DivdShift.Low32 <<= 1;
            /*Left-shift quotient by 1 bit.*/
            l_Quto = l_Quto<<1;
            if((l_DivdShift.High32 >= U32Divisor) || (l_uFlag >0U)) { /*Check whether the dividend high 32 bit is greater than the divisor low 32 bit after left shifting.*/
                /*Calculate the quotient and remainder between the dividend high 32 bit and divisor low 32 bit.*/
                l_DivdShift.High32 -= U32Divisor;
                l_Quto++;
            }
        }
    }
    return l_Quto;
}

/**
* @brief Check the validation of the function input and output.
* @param i_pDividend            [IN]A pointer to the dividend with 64 bit.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param i_pDivisor             [IN]A pointer to the divisor with 64 bit.
*                               - Type: U64_VALUE_t *\n
*                               - Range: N/A.\n
* @param o_pQuto                [OUT]A pointer to the division quotient with 32 bit.
*                               - Type: unsigned int  *\n
*                               - Range: N/A.\n
* @param ErrorCode           [OUT]A pointer to the division error code.
*                               - Type: unsigned char  *\n
*                               - Range: N/A.\n
* @param DivStatus           [OUT]A pointer to the division successful status.
*                               - Type: unsigned char  *\n
*                               - Range: N/A.\n
*
* @return none
*
*/
__attribute__((section(".ram_code"))) static void TN_CheckUINT64AccurateDiv(
    const U64_VALUE_t * const i_pDividend,               /*A pointer to the dividend with 64 bit.*/
    const U64_VALUE_t * const i_pDivisor,                /*A pointer to the divisor with 64 bit.*/
    unsigned int * const o_pQuto,                             /*A pointer to the division quotient with 32 bit.*/
    unsigned char * const ErrorCode,    /*A pointer to the division error code.*/
    unsigned char * const o_pDivStatus)                     /*A pointer to the division successful status.*/
{
    if ((0 != o_pQuto) && (0 != ErrorCode) && (0 != o_pDivStatus)) { /*Check the output buffer.*/
        /*Initiate the output buffer.*/
        *ErrorCode = 0;
        *o_pQuto = 0U;
        *o_pDivStatus = 1;
        if ((0 == i_pDividend) || (0 == i_pDivisor)) { /*Check the input buffer.*/
            *ErrorCode = 0xff;
            *o_pQuto = 0U;
            *o_pDivStatus = 0;
        }
    }
}

/**
* @brief Calculate the 64 bit accurate division, and the quotient is rounded.
* @param i_pDividend            [IN]A pointer to the dividend with 64 bit.
*                               - Type: U64_VALUE_t  *\n
*                               - Range: N/A.\n
* @param i_pDivisor             [IN]A pointer to the divisor with 64 bit.
*                               - Type: U64_VALUE_t *\n
*                               - Range: N/A.\n
* @param o_pQuto                [OUT]A pointer to the division quotient with 32 bit.
*                               - Type: unsigned int  *\n
*                               - Range: N/A.\n
* @param ErrorCode           [OUT]A pointer to the division error code.
*                               - Type: unsigned char  *\n
*                               - Range: N/A.\n
* @param o_pDivStatus           [OUT]A pointer to the division successful status.
*                               - Type: unsigned char  *\n
*                               - Range: N/A.\n
*
* @return N/A
*
*/
__attribute__((section(".ram_code"))) static void TN_U64AccurateDiv(
    const U64_VALUE_t * const Dividend,               /*A pointer to the dividend with 64 bit.*/
    const U64_VALUE_t * const Divisor,                /*A pointer to the divisor with 64 bit.*/
    unsigned int * const Quto,
    U64_VALUE_t * const Remainder,                            /*A pointer to the division quotient with 32 bit.*/
    unsigned char * const ErrorCode,    /*A pointer to the division error code.*/
    unsigned char * const DivStatus)                      /*A pointer to the division successful status.*/
{
    U64_VALUE_t l_DividendDiv2 = {0};        /*The temporary buffer.*/
    U64_VALUE_t l_DivisorDiv2 = {0};         /*The temporary buffer.*/
    unsigned int l_QutoTemp = 0U;             /*The quotient has not been rounded.*/
    TN_CheckUINT64AccurateDiv(Dividend, Divisor, Quto, ErrorCode, DivStatus);
    if (0 == *ErrorCode) {
        if ((0U == Divisor->High32) && (0U == Divisor->Low32)) {/*Check whether the divisor is zero.*/
            /*The division result is zero and division status is not successful when the divisor is zero.*/
            *Quto = 0;
            *ErrorCode  = 1;
            *DivStatus =  0;
        }
        else {
            if ((0U == Divisor->High32) && (Dividend->High32 >= Divisor->Low32)) {/*Check whether the dividend high 32 bit is greater than the divisor low 32 bit.*/
                /*The division result is overflow when the dividend high 32 bit is greater than the divisor low 32 bit.*/
                *Quto = 0;
                *ErrorCode = 2;
                *DivStatus =  0;
            }
            else {
                if ((0U == Divisor->High32) && (0U == Dividend->High32)) { /*Check whether the high 32 bit of the divisor and dividend is zero.*/
                    /*Get the division result directly the high 32 bit of the divisor and dividend is zero.*/
                    l_QutoTemp = Dividend->Low32 / Divisor->Low32;
                }
                else {
                    /*The dividend and divisor are divided by 2 respectively until the high 32 bit of the divisor is zero.*/
                    TN_Div2NumBy2(Dividend,Divisor,& l_DividendDiv2,& l_DivisorDiv2);
                    /*Calculate the quotient and remainder of the 64 bit division by subtracting with borrow.*/
                    l_QutoTemp = TN_CalQuto(&l_DividendDiv2,l_DivisorDiv2.Low32);
                }

                U64_VALUE_t U64MulRes = {0};
                TN_UINT64Multiply(Divisor, l_QutoTemp, &U64MulRes);/*Calculate the multiplication result between the 'Divisor' and 'QutoNotRound'.*/
                /*Calculate the remainder.*/
                *Remainder = TN_CalRemaider(U64MulRes, *Dividend);
                *Quto = l_QutoTemp;
            }
        }
    }
}




/**
 * @brief      This function serves to set the working mode of MCU,e.g. suspend mode, deepsleep mode, deepsleep with SRAM retention mode and shutdown mode.
 * @param[in]  sleep_mode - sleep mode type select.
 * @param[in]  wakeup_src - wake up source select.
 * @param[in]  SleepDurationUs - the time of long sleep, which means MCU can sleep for up to one hour.
 * @return     indicate whether the cpu is wake up successful.
 */
int pm_long_sleep_wakeup (SleepMode_TypeDef sleep_mode, SleepWakeupSrc_TypeDef wakeup_src, unsigned int SleepDurationUs)
{
	unsigned int   t0 = clock_time();
	unsigned int   tl_tick_cur;
	unsigned int   tl_tick_32k_cur;
	int timer_wakeup_enable = (wakeup_src & PM_WAKEUP_TIMER);
#if(RAM_CRC_EN)
	unsigned char ana_00 = analog_read(0x00);
	unsigned char ram_crc_en = 0;
	unsigned short ram_crc_tick = 0;
#endif
	//NOTICE:We think that the external 32k crystal clock is very accurate, does not need to read 2 bytes through reg_750
	//the conversion offset is less than 64ppm
#if (!CLK_32K_XTAL_EN)
	unsigned short  tl_tick_32k_calib = REG_ADDR16(0x754);
//	unsigned short  tick_32k_halfCalib = tl_tick_32k_calib>>1;
#endif

	unsigned int span = SleepDurationUs;


	if(timer_wakeup_enable){
		if (span < EMPTYRUN_TIME_US) 		// 0 us base
		{
			analog_write (0x44, 0x1d);			//clear all status

			unsigned char st;
			do {
				st = analog_read (0x44) & 0x1d;
			} while ( ((unsigned int)clock_time () - t0 < (span * sys_tick_per_us )) && !st);
			return st;
		}
	}
#if(RAM_CRC_EN)
	if(wakeup_src == PM_WAKEUP_TIMER){
		if(((sleep_mode == DEEPSLEEP_MODE_RET_SRAM_LOW16K)&&(span > (RAM_CRC_EN_16KRAM_TIME/16))) \
				|| ((sleep_mode == DEEPSLEEP_MODE_RET_SRAM_LOW32K)&&(span > (RAM_CRC_EN_32KRAM_TIME/16)))){
			analog_write(0x00, ana_00 | BIT(7));  	//poweron_dft: 0x18,  <7> MSB of ret_ldo_trim,  0: 0.8-1.15V; 1: 0.6-0.95V
			analog_write(0x02, 0xa0);  	//poweron_dft: 0xa4,  <2:0> ret_ldo_trim,  set 0x00: 0.6V
			ram_crc_en = 1;
		}
	}
#endif

	////////// disable IRQ //////////////////////////////////////////
	unsigned char r = irq_disable ();

	//set 32k wakeup tick
	U64_VALUE_t TmpVal[4] = {
		{0, 0},
		{0, 0},
		{0, 0},
		{0, 0}
	};
	unsigned char ErrorCode = 0;
	unsigned char DivStatus = 0;
	//1. calculate the absolute System wakeup tick
	TmpVal[0].Low32 = SleepDurationUs;
	TN_UINT64Multiply(&TmpVal[0], sys_tick_per_us, &TmpVal[1]);
	TmpVal[2].Low32 = t0;
	TN_Cal64Add(&TmpVal[1], &TmpVal[2], &TmpVal[3]);
	unsigned int wakeup_tick = TmpVal[3].Low32;
	TmpVal[0].Low32 = 0;
	TmpVal[0].High32 = 0;
	TmpVal[1].Low32 = 0;
	TmpVal[1].High32 = 0;
	TmpVal[2].Low32 = 0;
	TmpVal[2].High32 = 0;
	TmpVal[3].Low32 = 0;
	TmpVal[3].High32 = 0;

	//2. calculate and set the 32K timer wakeup tick
#if(!CLK_32K_XTAL_EN)
	TmpVal[2].Low32 = tl_tick_32k_calib;
#else
	TmpVal[2].Low32 = TICK_16M_PER_32CYC_XTAL32P768K;
#endif
	tl_tick_cur = clock_time ();
	tl_tick_32k_cur = cpu_get_32k_tick ();
    unsigned int M = 0;
    unsigned int m = 0;
	unsigned int EARLYWAKEUP_TIME_US = EARLYWAKEUP_TIME_US_SUSPEND;
	if(sleep_mode == DEEPSLEEP_MODE){
		EARLYWAKEUP_TIME_US = EARLYWAKEUP_TIME_US_DEEP;
	}
	else if(sleep_mode & DEEPSLEEP_RETENTION_FLAG){
		EARLYWAKEUP_TIME_US = EARLYWAKEUP_TIME_US_DEEP_RET;
	}
	TmpVal[0].Low32 = SleepDurationUs - (((unsigned int)(tl_tick_cur - t0)) / sys_tick_per_us) - EARLYWAKEUP_TIME_US;
	TN_UINT64Multiply(&TmpVal[0], sys_tick_per_us, &TmpVal[1]);
	TN_U64AccurateDiv(&TmpVal[1], &TmpVal[2], &M, &TmpVal[3], &ErrorCode, &DivStatus);
#if(!CLK_32K_XTAL_EN)
	m = M*16 + (TmpVal[3].Low32)/(tl_tick_32k_calib/16);
#else
	m = M*32 + (TmpVal[3].Low32)/(TICK_16M_PER_32CYC_XTAL32P768K/32);
#endif


	/////////////////// set wakeup source /////////////////////////////////
	analog_write (0x26, wakeup_src);   //@@@@@@@ [3] wakeup pad sel,1: 16usfilter (need 32k is on) 0: non-filter
	write_reg8(0x6e, 0x00);

	analog_write (0x44, 0x1f);				//clear all flag

	unsigned char reg66 = read_reg8(0x066);			//
	write_reg8 (0x066, 0);				//change to 24M rc clock

	unsigned char bit567_ana2c;
//	unsigned char bit012_ana07;

	if(sleep_mode & 0x7f) { //deepsleep with retention
		bit567_ana2c = 0x40;  //ana_2c<7:5>: 010
		tl_multi_addr_L = read_reg8(0x63e);
		if(0 == tl_multi_addr_L)
		{
			tl_multi_addr_H = 0;
		}else if(0xf9 == tl_multi_addr_L)
		{
			tl_multi_addr_H = 0x0;
		}else if(0xfa == tl_multi_addr_L)
		{
			tl_multi_addr_H = 0x01;
		}else if(0xfc == tl_multi_addr_L)
		{
			tl_multi_addr_H = 0x03;
		}
	}
	else if(sleep_mode){  //deepsleep no retention
		bit567_ana2c = 0xc0;  //ana_2c<7:5>: 110
	}
	else{  //suspend
		bit567_ana2c = 0x80;  //ana_2c<7:5>: 100    //BIT<5>: iso_ana,  set 0 now
		analog_write(0x04,0x48);  //ana_04<6:4> suspend LDO output voltage trim  default:100-1.0v
	}
	analog_write(0x7e, sleep_mode);
	unsigned char shutdown = sleep_mode + 1;
	analog_write(0x2b, (sleep_mode ? 0xde : 0x5e)|((shutdown<sleep_mode)? 1:0));// turn off soft start switch
#if (!CLK_32K_XTAL_EN)
		analog_write(0x2c, bit567_ana2c | 0x1e | (!timer_wakeup_enable));
#else
		analog_write(0x2c, bit567_ana2c | 0x1d | ((!timer_wakeup_enable)<<1));
#endif
	analog_write(0x06, (analog_read(0x06) & 0x3f) | bit567_ana2c);

	if(sleep_mode & 0x7f ) {
#if(RAM_CRC_EN)
		if(ram_crc_en){
			if((sleep_mode == DEEPSLEEP_MODE_RET_SRAM_LOW16K)){//16k retention,check 16k sram crc
				write_reg8(0x40008,0x0f);					  // span > 2.5s or pad wakeup
				ram_crc_tick = (200 * 16);//170*16
			}
			else if((sleep_mode == DEEPSLEEP_MODE_RET_SRAM_LOW32K)){//32k retention,check 32k sram crc
				write_reg8(0x40008,0x1f);						   //span > 6s or pad wakeup
				ram_crc_tick = (350 * 16);
			}
			write_reg8(0x6f, 0x10);
			analog_write(0x7f, 0x02);
			m -= ram_crc_tick;
		}
#else
		analog_write(0x7f, 0x00);
#endif
	}
	else{
		write_reg8(0x602,0x08);
		analog_write(0x7f, 0x01);
	}



	//set DCDC delay duration
	if(sleep_mode == DEEPSLEEP_MODE){
		analog_write (0x1f, 0xff - PM_DCDC_DELAY_CYCLE - SOFT_START_DLY);//(0xff - n):  if timer wake up : ((n+1)*2) 32k cycle; else pad wake up: (((n+1)*2-1) ~ (n+1)*2 )32k cycle
	}else{
		analog_write (0x1f, 0xff - PM_DCDC_DELAY_CYCLE);//(0xff - n): if timer wake up : ((n+1)*2) 32k cycle; else pad wake up: (((n+1)*2-1) ~ (n+1)*2 )32k cycle
	}

	cpu_set_32k_tick(tl_tick_32k_cur + m);

	write_reg8(0x74c, 0x20);
	if(analog_read(0x44)&0x1f){

	}
	else{
		sleep_start();
	}

	/* long press pad to wake up from deep */  //reboot can not use at A0
	if(0xff != read_reg8(0x7d)){
		if(sleep_mode == DEEPSLEEP_MODE){ //
		   write_reg8 (0x6f, 0x20);  //reboot
		}
	}
	else if(sleep_mode&0x7f)
	{
		write_reg8 (0x602, 0x88);  //mcu run from the beginning
	}

  //recover system clock from 32k clock
	TmpVal[0].Low32 = 0;
	TmpVal[0].High32 = 0;
	TmpVal[1].Low32 = 0;
	TmpVal[1].High32 = 0;
	TmpVal[2].Low32 = 0;
	TmpVal[2].High32 = 0;
	TmpVal[3].Low32 = 0;
	TmpVal[3].High32 = 0;
	span = (unsigned int)(cpu_get_32k_tick () - tl_tick_32k_cur);
#if(!CLK_32K_XTAL_EN)
	M = span / 16;
	m = span % 16;
	TmpVal[0].Low32 = M;
	TN_UINT64Multiply(&TmpVal[0], tl_tick_32k_calib, &TmpVal[1]);
	TmpVal[0].Low32 = m*tl_tick_32k_calib/16;
#else
	M = span / 32;
	m = span % 32;
	TmpVal[0].Low32 = M;
	TN_UINT64Multiply(&TmpVal[0], TICK_16M_PER_32CYC_XTAL32P768K, &TmpVal[1]);
	TmpVal[0].Low32 = m*TICK_16M_PER_32CYC_XTAL32P768K/32;
#endif
	TN_Cal64Add(&TmpVal[1], &TmpVal[0], &TmpVal[2]);
	TmpVal[3].Low32 = tl_tick_cur;
	TmpVal[0].Low32 = 0;
	TmpVal[0].High32 = 0;
	TN_Cal64Add(&TmpVal[2], &TmpVal[3], &TmpVal[0]);
	tl_tick_cur = TmpVal[0].Low32;

	reg_system_tick = tl_tick_cur + 20*sys_tick_per_us;  //20us compensation
	reg_system_ctrl |= FLD_SYSTEM_32K_CAL_EN|FLD_SYSTEM_TIMER_MANUAL_EN;    //system timer auto mode

	reg_system_irq_mask |= BIT(2);   //enable system timer irq

	write_reg8 (0x066, reg66);		//restore system clock

	unsigned char anareg44 = analog_read(0x44);
	if(!anareg44){
		anareg44 = STATUS_GPIO_ERR_NO_ENTER_PM;
	}
	else if ( (anareg44 & WAKEUP_STATUS_TIMER) && timer_wakeup_enable )	//wakeup from timer only
	{
		while ((unsigned int)(clock_time () -  wakeup_tick) > BIT(30));
	}

	irq_restore(r);

	return anareg44;
}
#endif




