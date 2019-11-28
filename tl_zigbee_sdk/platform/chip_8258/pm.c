
#include "compiler.h"
#include "bsp.h"
#include "register.h"
#include "gpio.h"
#include "analog.h"
#include "clock.h"
#include "irq.h"
#include "rf_drv.h"
#include "pm.h"

extern void start_suspend(void);

#define 	BLT_RESET_WAKEUP_TIME_1000		1
#define 	BLT_RESET_WAKEUP_TIME_1500		0
#define 	BLT_RESET_WAKEUP_TIME_2000		0
#define 	BLT_RESET_WAKEUP_TIME_3000		0
#define 	BLT_RESET_WAKEUP_TIME_4000		0

#define TICK_16M_PER_32CYC_XTAL32P768K		15625  // 16M tick per 32 cycle of 32k

#if (BLT_RESET_WAKEUP_TIME_1000)
	#define PM_XTAL_DELAY_DURATION    	   500
	#define PM_DCDC_DELAY_DURATION        1000
	#define EARLYWAKEUP_TIME_US           1300
	#define EMPTYRUN_TIME_US       	      1600
#elif (BLT_RESET_WAKEUP_TIME_1500)
	#define PM_XTAL_DELAY_DURATION    	   750
	#define PM_DCDC_DELAY_DURATION        1500
	#define EARLYWAKEUP_TIME_US           1800
	#define EMPTYRUN_TIME_US       	      2100
#elif (BLT_RESET_WAKEUP_TIME_2000)
	#define PM_XTAL_DELAY_DURATION    	  1300
	#define PM_DCDC_DELAY_DURATION        2000
	#define EARLYWAKEUP_TIME_US           2300
	#define EMPTYRUN_TIME_US       	      2700
#elif(BLT_RESET_WAKEUP_TIME_3000)
	#define PM_XTAL_DELAY_DURATION    	  1000
	#define PM_DCDC_DELAY_DURATION        3000
	#define EARLYWAKEUP_TIME_US           3300
	#define EMPTYRUN_TIME_US       	      3800
#elif(BLT_RESET_WAKEUP_TIME_4000)
	#define PM_XTAL_DELAY_DURATION    	  2000
	#define PM_DCDC_DELAY_DURATION        4000
	#define EARLYWAKEUP_TIME_US           4300
	#define EMPTYRUN_TIME_US       	      4800

#else
#endif




pm_para_t pmParam;


_attribute_data_retention_	unsigned short tick_32k_calib;
_attribute_data_retention_	unsigned int tick_cur;
_attribute_data_retention_	unsigned int tick_32k_cur;
_attribute_data_retention_  unsigned int Multi_address_offset;


suspend_handler_t	func_before_suspend = 0;

void	bls_pm_registerFuncBeforeSuspend (suspend_handler_t func )
{
	func_before_suspend = func;
}
#define FLASH_DEEP_EN				1
#define FLASH_STILL_ON				1

_attribute_ram_code_ _attribute_no_inline_ void  sleep_start(void)
{
	WriteAnalogReg(0x34,0x87);  //BIT<2:0> audio/USB/ZB  power down, save 50uA +

	volatile unsigned int i;

#if FLASH_DEEP_EN
    WRITE_REG8(0x00d, 0);
    WRITE_REG8(0x00c, 0xb9);
    for(i = 0; i < 2; i++); //1440ns when 32M clk
    WRITE_REG8(0x00d, 1);

    WRITE_REG8(0x5a1, 0x00); //MSPI ie disable
#endif


#if FLASH_STILL_ON
    WriteAnalogReg(0x0b, 0x38);   //BIT(1)  BIT(0)  enable
#if MCU_8258_A0
    WriteAnalogReg(0x0c, 0xcc);   //suspend wakeup recover to  0xc4
#endif
    WriteAnalogReg(0x82, 0x0c);   //suspend wakeup recover to  0x64
#endif

    //WRITE_REG8(0x06f, 0x81);
    unsigned int T_reg = READ_REG8(0x60d);
    T_reg = (T_reg << 8) + 0x40058;

    unsigned int T_regValue = READ_REG32(T_reg);
    WRITE_REG32(T_reg, 0x06c006c0);

    start_suspend();
    WRITE_REG32(T_reg, T_regValue);

    for(i = 0; i < 0x30; i++);


	WriteAnalogReg(0x82, 0x64);
#if MCU_8258_A0
	WriteAnalogReg(0x0c, 0xc4);
#endif
	WriteAnalogReg(0x0b, 0x3b);

#if FLASH_DEEP_EN
    WRITE_REG8(0x5a1, 0x0f); //MSPI ie enable

    WRITE_REG8(0x00d, 0);
    WRITE_REG8(0x00c, 0xab);
    for(i = 0; i < 2; i++); //1440ns when 32M clk
    WRITE_REG8(0x00d, 1);
#endif


	WriteAnalogReg(0x34,0x80);  //BIT<2:0> audio/USB/ZB  power on
}

_attribute_ram_code_ unsigned int cpu_get_32k_tick_from_analog(void)
{
    unsigned int t0 = 0;
    unsigned int t1 = 0;
    unsigned int n = 0;

    while (1) {
        //start read
        t0 = t1;
        t1 = ReadAnalogReg(0x43);
        t1 <<= 8;
        t1 += ReadAnalogReg(0x42);
        t1 <<= 8;
        t1 += ReadAnalogReg(0x41);
        t1 <<= 8;
        t1 += ReadAnalogReg(0x40);
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

_attribute_ram_code_
_attribute_no_inline_ unsigned int cpu_get_32k_tick (void)
{
	unsigned int		t0 = 0, t1 = 0, n;

	n = 0;
	REG_ADDR8(0x74c) = 0x28;							//system timer manual mode, interrupt disable
	while (1)
	{
		REG_ADDR8(0x74f) = BIT(3);							//start read

		//16 tnop is enough
		CLOCK_DLY_8_CYC;
		CLOCK_DLY_8_CYC;

		while (REG_ADDR8(0x74f) & BIT(3));
		t0 = t1;
		t1 = REG_ADDR32(0x754);
		if (n)
		{
			if ((unsigned int)(t1 - t0) < 2)
			{
				return t1;
			}
			else if ( (t0^t1) == 1 )	// -1
			{
				return t0;
			}
		}
		n++;
	}
	return t1;
}

const TBLCMDSET  tbl_cpu_wakeup_init[] = {
	{0x60, 0x00,		TCMD_UNDER_BOTH | TCMD_WRITE},	//open all the clk,disable all the rst
	{0x61, 0x00,		TCMD_UNDER_BOTH | TCMD_WRITE},	//open all the clk,disable all the rst
	{0x62, 0x00,		TCMD_UNDER_BOTH | TCMD_WRITE},	//open all the clk,disable all the rst
	{0x63, 0xff,		TCMD_UNDER_BOTH | TCMD_WRITE},	//open all the clk,disable all the rst
	{0x64, 0xff,		TCMD_UNDER_BOTH | TCMD_WRITE},   //open all the clk,disable all the rst
	{0x65, 0xff,		TCMD_UNDER_BOTH | TCMD_WRITE},	//open all the clk,disable all the rst

	{0x82, 0x64,  		TCMD_UNDER_BOTH | TCMD_WAREG},
	{0x34, 0x80,		TCMD_UNDER_BOTH | TCMD_WAREG},
	{0x0b, 0x3b,  		TCMD_UNDER_BOTH | TCMD_WAREG},
#if MCU_8258_A0
	{0x0c, 0xc4,  		TCMD_UNDER_BOTH | TCMD_WAREG}, //by junwei
#endif
	{0x8c, 0x02,  		TCMD_UNDER_BOTH | TCMD_WAREG},

	//clear gpio pad wake up enable
	{0x27, 0x00,  		TCMD_UNDER_BOTH | TCMD_WAREG},
	{0x28, 0x00,  		TCMD_UNDER_BOTH | TCMD_WAREG},
	{0x29, 0x00,  		TCMD_UNDER_BOTH | TCMD_WAREG},
	{0x2a, 0x00,  		TCMD_UNDER_BOTH | TCMD_WAREG},


	{0xc40, 0x04,		TCMD_UNDER_BOTH | TCMD_WRITE},
	{0xc41, 0x04,		TCMD_UNDER_BOTH | TCMD_WRITE},
	{0xc42, 0x04,		TCMD_UNDER_BOTH | TCMD_WRITE},
	{0xc43, 0x04,		TCMD_UNDER_BOTH | TCMD_WRITE},
	{0xc44, 0x04,		TCMD_UNDER_BOTH | TCMD_WRITE},
	{0xc45, 0x04,		TCMD_UNDER_BOTH | TCMD_WRITE},
	{0xc46, 0x04,		TCMD_UNDER_BOTH | TCMD_WRITE},
	{0xc47, 0x04,		TCMD_UNDER_BOTH | TCMD_WRITE},
	{0xc48, 0x04,		TCMD_UNDER_BOTH | TCMD_WRITE},
};

void rc_32k_cal (void)
{
    analog_write(0x30, 0x60);
    analog_write(0xc6, 0xf6);
    analog_write(0xc6, 0xf7);
    while((analog_read(0xcf) & BIT(6)) == 0);
	unsigned char res1 = analog_read(0xc9); //read 32k res[13:6]
	analog_write(0x32, res1);  //write 32k res[13:6] into manual register
	unsigned char res2 = analog_read(0xca); //read 32k res[5:0]
	analog_write(0x31, res2);  //write 32k res[5:0] into manual register
	analog_write(0xc6, 0xf6);
	analog_write(0x30, 0x20);//manual on
}

void cpu_stall_wakeup_by_timer0(unsigned int tick_stall)
{
   /*Write 0x00 here may cause problem, it is removed to blt_sleep_wakeup*/
   //write_reg8(0x6f,0x00);//clear bit[0] suspend enable

    reg_tmr0_tick = 0;

    reg_tmr0_capt = tick_stall;
    reg_tmr_ctrl8 |= FLD_TMR0_EN;//enable TIMER1,mode:using sclk
    reg_mcu_wakeup_mask |= FLD_IRQ_TMR0_EN;//timer1 mask enable
    reg_tmr_sta = FLD_TMR_STA_TMR0; // clean interrupt

    write_reg8(0x6f,0x80);//stall mcu
    asm("tnop");
    asm("tnop");

    reg_tmr_sta = FLD_TMR_STA_TMR0; // clean interrupt
    reg_tmr_ctrl8 &= ~FLD_TMR0_EN;//disable TIMER1
}


void cpu_stall_wakeup_by_timer1(unsigned int tick_stall)
{
   /*Write 0x00 here may cause problem, it is removed to blt_sleep_wakeup*/
   //write_reg8(0x6f,0x00);//clear bit[0] suspend enable

    reg_tmr1_tick = 0;

    reg_tmr1_capt = tick_stall;
    reg_tmr_ctrl8 |= FLD_TMR1_EN;//enable TIMER1,mode:using sclk
    reg_mcu_wakeup_mask |= FLD_IRQ_TMR1_EN;//timer1 mask enable
    reg_tmr_sta = FLD_TMR_STA_TMR1; // clean interrupt

    write_reg8(0x6f,0x80);//stall mcu
    asm("tnop");
    asm("tnop");

    reg_tmr_sta = FLD_TMR_STA_TMR1; // clean interrupt
    reg_tmr_ctrl8 &= ~FLD_TMR1_EN;//disable TIMER1
}

void cpu_stall_wakeup_by_timer2(unsigned int tick_stall)
{
   /*Write 0x00 here may cause problem, it is removed to blt_sleep_wakeup*/
   //write_reg8(0x6f,0x00);//clear bit[0] suspend enable

    reg_tmr2_tick = 0;

    reg_tmr2_capt = tick_stall;
    reg_tmr_ctrl8 |= FLD_TMR2_EN;//enable TIMER1,mode:using sclk
    reg_mcu_wakeup_mask |= FLD_IRQ_TMR2_EN;//timer1 mask enable
    reg_tmr_sta = FLD_TMR_STA_TMR2; // clean interrupt

    write_reg8(0x6f,0x80);//stall mcu
    asm("tnop");
    asm("tnop");

    reg_tmr_sta = FLD_TMR_STA_TMR2; // clean interrupt
    reg_tmr_ctrl8 &= ~FLD_TMR2_EN;//disable TIMER1
}



#define	CAL_24M_TIMES			3



_attribute_ram_code_ void cpu_wakeup_init(void)    //must on ramcode
{
#if 0
	LoadTblCmdSet (tbl_cpu_wakeup_init, sizeof (tbl_cpu_wakeup_init)/sizeof (TBLCMDSET));
#else
	write_reg8(0x60, 0x00);
	write_reg8(0x61, 0x00);
	write_reg8(0x62, 0x00);
	write_reg8(0x63, 0xff);
	write_reg8(0x64, 0xff);
	write_reg8(0x65, 0xff);

	analog_write(0x82, 0x64);  //poweron_dft 0x0c, BIT<6>:clk_24M_tosar_en;  BIT<5>:clk_48M_todig_en;  BIT<3>:pd_dccc_doubler, 0 for power on
	analog_write(0x34, 0x80);
	analog_write(0x0b, 0x38);  //BIT<2> is removed, so 0x38 equals 0x3c
#if MCU_8258_A0
	analog_write(0x0c, 0xc4);  //power on flash power supply, must do this before any flash read when deep back,
#endif						   //so data copy in cstartup.S must be handled,  will do it later

	analog_write(0x8c, 0x02);  //poweron_dft: 0x00,  <1> set 1: reg_xo_en_clk_ana_ana, to enable external 24M crystal

	analog_write(0x02, 0xa2); //poweron_dft: 0xa4, <2:0> ret_ldo_trim,  set 0x02: 0.8V

	analog_write(0x27, 0x00);
	analog_write(0x28, 0x00);
	analog_write(0x29, 0x00);
	analog_write(0x2a, 0x00);

	write_reg32(0xc40, 0x04040404);
	write_reg32(0xc44, 0x04040404);
	write_reg8(0xc48, 0x04);

#endif

	write_reg16(0x750, 8000);    			//set 32KTimer cal Result reg a value
//	write_reg8(0x74f, 0x01);    			//enable system timer and 32KTimer's cal

	if(analog_read(REG_DEEP_BACK_FLAG) == DATA_STORE_FLAG){
		if(!(analog_read(0x7f) & 0x01)){
			pmParam.back_mode = BACK_FROM_DEEP_RETENTION;
		}else{
			pmParam.back_mode = BACK_FROM_DEEP;
		}
	}else{
		pmParam.back_mode = BACK_FROM_REPOWER;
	}

	/* If back from deep or deep retention mode, check wakeup source. */
	if(pmParam.back_mode){
		pmParam.wakeup_src = analog_read(0x44);

		if((pmParam.wakeup_src & WAKEUP_STATUS_TIMER_PAD ) == WAKEUP_STATUS_PAD)  //pad, no timer
		{
			pmParam.is_pad_wakeup = 1;
		}else{
			pmParam.is_pad_wakeup = 0;
		}
	}

	if(pmParam.back_mode == BACK_FROM_DEEP_RETENTION){
		unsigned int deepRet_32k_tick = cpu_get_32k_tick();
		unsigned int span = (unsigned int)(deepRet_32k_tick - tick_32k_cur) / 16 * tick_32k_calib;

		reg_system_tick = tick_cur + span;
		REG_ADDR8(0x74c) = 0x00;
		REG_ADDR8(0x74c) = 0x92;
		REG_ADDR8(0x74f) = BIT(0);    //enable system tick
	}else{
		REG_ADDR8(0x74f) = BIT(0);    //enable system tick

		rc_32k_cal();  //6.69 ms/6.7 ms
		//rc_24m_cal();  //469 us/474 us

//		pm_register_tick32kGet_callback(pm_get_tick_32k_filter);
	}


	//core_c20/c21 power on default all enable, so we disable them first,  then if use, enable them by manual
	//note that: PWM/RF Tx/RF Rx/AES code/AES dcode dma may be affected by this, you must handle them when initialization
	reg_dma_chn_en = 0;
	reg_dma_chn_irq_msk = 0;



	//boot flag storage
	unsigned char boot_flag = read_reg8(0x63e);
	if (boot_flag)
	{
		write_reg8(0x40004, 0xA5);   	//mark
		write_reg8(0x40005, boot_flag);
	}
}

#define AREG_PAD_WAKEUP_EN(i)				((i>>8) + 0x27)
#define AREG_PAD_WAKEUP_POL(i)				((i>>8) + 0x21)

_attribute_ram_code_ void cpu_set_gpio_wakeup (GPIO_PinTypeDef pin, GPIO_LevelTypeDef pol, int en)
{
	///////////////////////////////////////////////////////////
	// 		  PA[7:0]	    PB[7:0]		PC[7:0]		PD[7:0]
	// en: 	ana_21<7:0>	 ana_22<7:0>  ana_23<7:0>  ana_24<7:0>
	// pol:	ana_27<7:0>	 ana_28<7:0>  ana_29<7:0>  ana_2a<7:0>
    unsigned char mask = pin & 0xff;
	unsigned char areg;
	unsigned char val;

	////////////////////////// polarity ////////////////////////
	areg = AREG_PAD_WAKEUP_POL(pin);
	val = ReadAnalogReg(areg);
	if (pol) {
		val &= ~mask;
	}
	else {
		val |= mask;
	}
	WriteAnalogReg (areg, val);

	/////////////////////////// enable /////////////////////
	areg = AREG_PAD_WAKEUP_EN(pin);
	val = ReadAnalogReg(areg);
	if (en) {
		val |= mask;
	}
	else {
		val &= ~mask;
	}
	WriteAnalogReg (areg, val);
}


#define		SYSTEM_TICK_ALWAYS_ON		0
#define	    TickPerUs   				16

int cpu_sleep_wakeup (SleepMode_TypeDef sleep_mode, SleepWakeupSrc_TypeDef wakeup_src, unsigned int  wakeup_tick)
{
	int timer_wakeup_enable = (wakeup_src & PM_WAKEUP_TIMER);

	tick_32k_calib = REG_ADDR16(0x750);
	unsigned short  tick_32k_halfCalib = tick_32k_calib>>1;


	int long_suspend = 0;
	unsigned int span = (unsigned int)(wakeup_tick - clock_time ());


	if(timer_wakeup_enable){
		if (span > 0xE0000000)  //BIT(31)+BIT(30)+BIT(19)   7/8 cylce of 32bit
		{
			return  analog_read (0x44) & 0x0f;
		}
		else if (span < EMPTYRUN_TIME_US * sys_tick_per_us) // 0 us base
		{
			unsigned int t = clock_time ();
			analog_write (0x44, 0x0f);			//clear all status

			unsigned char st;
			do {
				st = analog_read (0x44) & 0x0f;
			} while ( ((unsigned int)clock_time () - t < span) && !st);
			return st;

		}
		else
		{
			if( span > 0x0ff00000 ){  //BIT(28) = 0x10000000   16M:16S
				long_suspend = 1;
			}
		}
	}


	////////// disable IRQ //////////////////////////////////////////
	unsigned char r = irq_disable ();


	tick_cur = clock_time ();
	tick_32k_cur = cpu_get_32k_tick ();
	unsigned int tick_wakeup_reset = wakeup_tick - EARLYWAKEUP_TIME_US * sys_tick_per_us;


	/////////////////// set wakeup source /////////////////////////////////
	analog_write (0x26, wakeup_src);   //@@@@@@@ [3] wakeup pad sel,1: 16usfilter (need 32k is on) 0: non-filter
	write_reg8(0x6e, wakeup_src & PM_WAKEUP_CORE ? 0x08 : 0x00);

	analog_write (0x44, 0x0f);				//clear all flag


	///////////////////////// change to 24M RC clock before suspend /////////////
	unsigned char reg66 = read_reg8(0x066);			//
	write_reg8 (0x066, 0);				//change to 24M rc clock

	unsigned char bit567_ana2c;
	unsigned char bit012_ana07;

	if(sleep_mode & DEEPSLEEP_RETENTION_FLAG) { //deepsleep with retention
		bit567_ana2c = 0x40;  //ana_2c<7:5>: 010
		bit012_ana07 = 0x01;  //ana_07<2:0>:001
		analog_write(0x02, (analog_read(0x02) & 0xf8) | SRAM_RET_LDO);  //ana_02<2:0>:010-> 0.8v ret ldo
		analog_write(0x7e, sleep_mode);
		Multi_address_offset = read_reg8(0x63e);
	}
	else if(sleep_mode){  //deepsleep no retention
		bit567_ana2c = 0xc0;  //ana_2c<7:5>: 110
		bit012_ana07 = 0x05;  //ana_07<2:0>: 101
		analog_write(0x02, 0xa4); //set default
		analog_write(0x7e, 0); //set default
	}
	else{  //suspend
		bit567_ana2c = 0x80;  //ana_2c<7:5>: 100    //BIT<5>: iso_ana,  set 0 now
		bit012_ana07 = 0x04;  //ana_07<2:0>: 100
		analog_write(0x04,0x48);  //ana_04<6:4> suspend LDO output voltage trim  default:100-1.0v
		analog_write(0x7e, 0x00);
	}

	analog_write(0x2b, (sleep_mode ? 0xde : 0x5e) );// turn off soft start switch
	analog_write(0x2c, bit567_ana2c | 0x1e | (!timer_wakeup_enable));
	analog_write(0x07, (analog_read(0x07) & 0xf8) | bit012_ana07);

	if(sleep_mode & DEEPSLEEP_RETENTION_FLAG ) {
		analog_write(0x7f, 0x00);
	}
	else{
		WRITE_REG8(0x602,0x08);
		analog_write(0x7f, 0x01);
	}

	/* Before enter deep mode, store a flag in power on reset analog register. */
	if(sleep_mode){
		analog_write(REG_DEEP_BACK_FLAG, DATA_STORE_FLAG);
	}else{
		analog_write(REG_DEEP_BACK_FLAG, 0);
	}

	//set xtal delay duration
	span = (PM_XTAL_DELAY_DURATION * (sys_tick_per_us>>1) * 16 + tick_32k_halfCalib)/ tick_32k_calib;
	unsigned char rst_cycle = 0x7f - span;
	analog_write (0x20, rst_cycle);


	//set DCDC delay duration
    span = (PM_DCDC_DELAY_DURATION * (sys_tick_per_us>>1) * 16 + tick_32k_halfCalib)/ tick_32k_calib;
	rst_cycle = 0xff - span;
	analog_write (0x1f, rst_cycle);

#if 1
	unsigned int tick_reset;
	if(long_suspend){
		tick_reset = tick_32k_cur + (unsigned int)(tick_wakeup_reset - tick_cur)/ tick_32k_calib * 16;
	}
	else{
		tick_reset = tick_32k_cur + ((unsigned int)(tick_wakeup_reset - tick_cur) * 16 + tick_32k_halfCalib) / tick_32k_calib;
	}


	REG_ADDR8(0x74c) = 0x2c;
	REG_ADDR32(0x754) = tick_reset;
	REG_ADDR8(0x74f) = BIT(3);									//start write
	//16 tnop is enough
	CLOCK_DLY_8_CYC;
	CLOCK_DLY_8_CYC;
	while (REG_ADDR8(0x74f) & BIT(3));

#endif

	write_reg8(0x74c, 0x20);
	if(analog_read(0x44)&0x0f){

	}
	else{
		sleep_start();
	}

	if(sleep_mode == DEEPSLEEP_MODE){
		write_reg8 (0x6f, 0x20);  //reboot
	}else if(sleep_mode & DEEPSLEEP_RETENTION_FLAG){
		write_reg8 (0x602, 0x88);  //mcu run from the beginning
	}

	if(long_suspend){
		tick_cur += (unsigned int)(cpu_get_32k_tick () - tick_32k_cur) / 16 * tick_32k_calib;
	}
	else{
		tick_cur += (unsigned int)(cpu_get_32k_tick () - tick_32k_cur) * tick_32k_calib / 16;		// current clock
	}

	write_reg8 (0x066, reg66);		//restore system clock
	reg_system_tick = tick_cur + 480;  //30us compensation
	REG_ADDR8(0x74c) = 0x00;
	REG_ADDR8(0x74c) = 0x92;  //reg_system_tick_mode |= FLD_SYSTEM_TICK_IRQ_EN;
	REG_ADDR8(0x74f) = BIT(0);

	unsigned char anareg44 = analog_read(0x44);
	if(!anareg44){ //GPIO ERR can not enter suspend
		anareg44 = STATUS_GPIO_ERR_NO_ENTER_PM;
	}
	else if ( (anareg44 & WAKEUP_STATUS_TIMER) && timer_wakeup_enable )	//wakeup from timer only
	{
		while ((unsigned int)(clock_time () -  wakeup_tick) > BIT(30));
	}

	irq_restore(r);

	return anareg44;
}

#if 1

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
static unsigned char TN_Cal64Multi32_H(
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
static void TN_Cal64Multi32_L(
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
static unsigned char TN_Cal64Add(
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
static unsigned char TN_UINT64Multiply(
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
static U64_VALUE_t TN_CalRemaider(const U64_VALUE_t MulRes,const U64_VALUE_t Dividend)
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
static void TN_Div2NumBy2(
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
static unsigned int TN_CalQuto(
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
static void TN_CheckUINT64AccurateDiv(
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
static void TN_U64AccurateDiv(
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
	//NOTICE:We think that the external 32k crystal clock is very accurate, does not need to read 2 bytes through reg_750
	//the conversion offset is less than 64ppm
#if (!CLK_32K_XTAL_EN)
	unsigned short  tl_tick_32k_calib = REG_ADDR16(0x750);
	unsigned short  tick_32k_halfCalib = tl_tick_32k_calib>>1;
#endif

	unsigned int span = SleepDurationUs;

	if(timer_wakeup_enable){
		if (span < EMPTYRUN_TIME_US) // 0 us base
		{
			analog_write (0x44, 0x0f);			//clear all status

			unsigned char st;
			do {
				st = analog_read (0x44) & 0x0f;
			} while ( ((unsigned int)clock_time() - t0 < (span * sys_tick_per_us)) && !st);
			return st;
		}
	}


	////////// disable IRQ //////////////////////////////////////////
	unsigned char r = irq_disable();

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
	tl_tick_cur = clock_time();
	tl_tick_32k_cur = cpu_get_32k_tick();
    unsigned int M = 0;
    unsigned int m = 0;
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
	WRITE_REG8(0x6e, 0x00);

	analog_write (0x44, 0x0f);				//clear all flag

#if(SYS_CLK_48MRC_EN)
	///////////////////////// change to 24M RC clock before suspend /////////////
	unsigned char ana_04 = analog_read(0x04);
	analog_write(0x04, ana_04&0xfc);
	unsigned char ana_33 = analog_read(0x33);
	analog_write(0x33, tl_24mrc_cal);
#endif
	unsigned char reg66 = READ_REG8(0x066);			//
	WRITE_REG8 (0x066, 0);				//change to 24M rc clock

	unsigned char bit567_ana2c;
	unsigned char bit012_ana07;

	if(sleep_mode & 0x7f) { //deepsleep with retention
		bit567_ana2c = 0x40;  //ana_2c<7:5>: 010
		bit012_ana07 = 0x01;  //ana_07<2:0>:001
		analog_write(0x02, (analog_read(0x02) & 0xf8) | 0x05);  //ana_02<2:0>:010-> 0.8v ret ldo
		analog_write(0x7e, sleep_mode);
		Multi_address_offset = READ_REG8(0x63e);
	}
	else if(sleep_mode){  //deepsleep no retention
		bit567_ana2c = 0xc0;  //ana_2c<7:5>: 110
		bit012_ana07 = 0x05;  //ana_07<2:0>: 101
		analog_write(0x02, 0xa4); //set default
		analog_write(0x7e, 0); //set default
	}
	else{  //suspend
		bit567_ana2c = 0x80;  //ana_2c<7:5>: 100    //BIT<5>: iso_ana,  set 0 now
		bit012_ana07 = 0x04;  //ana_07<2:0>: 100
		analog_write(0x04,0x48);  //ana_04<6:4> suspend LDO output voltage trim  default:100-1.0v
		analog_write(0x7e, 0x00);
	}

	unsigned char shutdown = sleep_mode + 1;
	analog_write(0x2b, (sleep_mode ? 0xde : 0x5e)|((shutdown<sleep_mode)? 1:0));// turn off soft start switch
#if (!CLK_32K_XTAL_EN)
		analog_write(0x2c, bit567_ana2c | 0x1e | (!timer_wakeup_enable));
#else
		analog_write(0x2c, bit567_ana2c | 0x1d | ((!timer_wakeup_enable)<<1));
#endif
	analog_write(0x07, (analog_read(0x07) & 0xf8) | bit012_ana07);

	if(sleep_mode & 0x7f ) {
		analog_write(0x7f, 0x00);
	}
	else{
		WRITE_REG8(0x602,0x08);
		analog_write(0x7f, 0x01);
	}

	/* Before enter deep mode, store a flag in power on reset analog register. */
	if(sleep_mode){
		analog_write(REG_DEEP_BACK_FLAG, DATA_STORE_FLAG);
	}else{
		analog_write(REG_DEEP_BACK_FLAG, 0);
	}

#if(!CLK_32K_XTAL_EN)
	//set xtal delay duration
	span = (PM_XTAL_DELAY_DURATION * (sys_tick_per_us>>1) * 16 + tick_32k_halfCalib)/ tl_tick_32k_calib;
	unsigned char rst_cycle = 0x7f - span;
	analog_write (0x20, rst_cycle);

	//set DCDC delay duration
    span = (PM_DCDC_DELAY_DURATION * (sys_tick_per_us>>1) * 16 + tick_32k_halfCalib)/ tl_tick_32k_calib;
	rst_cycle = 0xff - span;
	analog_write (0x1f, rst_cycle);
#else
	//set xtal delay duration
	span = (PM_XTAL_DELAY_DURATION * (sys_tick_per_us>>1) * 32 + (TICK_16M_PER_32CYC_XTAL32P768K>>1))/ TICK_16M_PER_32CYC_XTAL32P768K;
	unsigned char rst_cycle = 0x7f - span;
	analog_write (0x20, rst_cycle);

	//set DCDC delay duration
    span = (PM_DCDC_DELAY_DURATION * (sys_tick_per_us>>1) * 32 + (TICK_16M_PER_32CYC_XTAL32P768K>>1))/ TICK_16M_PER_32CYC_XTAL32P768K;
	rst_cycle = 0xff - span;
	analog_write (0x1f, rst_cycle);
#endif

	REG_ADDR8(0x74c) = 0x2c;
	REG_ADDR32(0x754) = tl_tick_32k_cur + m;
	REG_ADDR8(0x74f) = BIT(3);									//start write
	//16 tnop is enough
	CLOCK_DLY_8_CYC;
	CLOCK_DLY_8_CYC;
	while (REG_ADDR8(0x74f) & BIT(3));

	WRITE_REG8(0x74c, 0x20);
	if(analog_read(0x44)&0x0f){

	}
	else{
		sleep_start();
	}

	/* long press pad to wake up from deep */
	if(sleep_mode == DEEPSLEEP_MODE){
		WRITE_REG8 (0x6f, 0x20);  //reboot
	}
	else if(sleep_mode&0x7f)
	{
		WRITE_REG8 (0x602, 0x88);  //mcu run from the beginning
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
	span = (unsigned int)(cpu_get_32k_tick() - tl_tick_32k_cur);
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
	REG_ADDR8(0x74c) = 0x00;
	//caution: at least 5~6 cycle confirmed by guangjun: system timer clock domain(16M) and system clock(24M) is inconsistent,
	//set the system timer related registers need clock synchronization, to ensure correct writing, otherwise there is a risk of crash.
	CLOCK_DLY_6_CYC;
	REG_ADDR8(0x74c) = 0x92;
	//caution: at least 3~4 cycle confirmed by guangjun: system timer clock domain(16M) and system clock(24M) is inconsistent,
	//set the system timer related registers need clock synchronization, to ensure correct writing, otherwise there is a risk of crash.
	CLOCK_DLY_4_CYC;
	REG_ADDR8(0x74f) = BIT(0);

#if(SYS_CLK_48MRC_EN)
	analog_write(0x33, ana_33);
	analog_write(0x04, ana_04);
#endif

	WRITE_REG8 (0x066, reg66);		//restore system clock

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


//just for pm test
void shutdown_gpio(void)
{
	//output disable
	reg_gpio_pa_oen = 0xff;
	reg_gpio_pb_oen = 0xff;
	reg_gpio_pc_oen = 0xff;
	reg_gpio_pd_oen = 0xff;

	//dataO = 0
	reg_gpio_pa_out = 0x00;
	reg_gpio_pb_out = 0x00;
	reg_gpio_pc_out = 0x00;
	reg_gpio_pd_out = 0x00;

	//ie = 0
	//SWS   581<7>
	//DM DP  581<6:5>
	reg_gpio_pa_ie = 0x80;
	analog_write(anaReg_gpioPB_ie, 0);
	analog_write(anaReg_gpioPC_ie, 0);
	reg_gpio_pd_ie = 0x00;
}



