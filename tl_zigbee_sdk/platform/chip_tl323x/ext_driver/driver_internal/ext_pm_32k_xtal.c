/********************************************************************************************************
 * @file    ext_pm_32k_xtal.c
 *
 * @brief   This is the source file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#if 0 //32K xtal not support now
#include "compiler.h"


#include "../../lib/include/pm/pm.h"
#include "gpio.h"
#include "compiler.h"
#include "core.h"
#include "timer.h"
#include "lib/include/sys.h"
#include "clock.h"
#include "mspi.h"
#include "stimer.h"
#include "analog.h"
#include "../ext_pm.h"
#include "ext_lib.h"


#define CRYSTAL32768_TICK_PER_64CYCLE       46875

extern unsigned char    g_pm_pad_filter_en;
extern  unsigned int            g_pm_interrupt_status;
extern  unsigned int            g_pm_interrupt_status1;
extern  unsigned int            g_pm_interrupt_status2;

extern void pm_switch_ext32kpad_to_int32krc(void);

extern void  pm_sleep_start(pm_sleep_mode_e sleep_mode);
_attribute_ram_code_sec_noinline_ int cpu_sleep_wakeup_32k_xtal_ram(pm_sleep_mode_e sleep_mode,  pm_sleep_wakeup_src_e wakeup_src, unsigned int  wakeup_tick)
{
    ////////// disable IRQ //////////////////////////////////////////
    unsigned int r= core_interrupt_disable();
    int timer_wakeup_enable = (wakeup_src & PM_WAKEUP_TIMER);
    if(timer_wakeup_enable)
    {
        unsigned int span = (unsigned int)(wakeup_tick - stimer_get_tick ());
        if (span > 0xE0000000) //BIT(31)+BIT(30)+BIT(29)   7/8 cycle of 32bit, 178*7/8 = 156 S
        {
            core_restore_interrupt(r);
            return  analog_read_reg8(0x64) & 0x7f;
        }
        else if (span < g_pm_early_wakeup_time_us.sleep_min_time_us * SYSTEM_TIMER_TICK_1US)
        {
            unsigned int t = stimer_get_tick ();
            analog_write_reg8(0x64, 0xff);
            unsigned char st;
            do
            {
                st = analog_read_reg8(0x64) & 0x7f;
            }while ( ((unsigned int)stimer_get_tick () - t < span) && !st);
            core_restore_interrupt(r);
            return st;
        }
    }
    if(func_before_suspend){
        if (!func_before_suspend())
        {
            core_restore_interrupt(r);
            return WAKEUP_STATUS_PAD;
        }
    }

    //Turn off all interrupts immediately after entering the sleep function to prevent other interrupts, save the interrupt state before turning off, and restore it after waking up.
    //Turn on pm interrupt only before going to sleep,enable M-mode external interrupt first in this place.
    //modify by bingyu.li, confirmed by jianzhi.chen at 20230810.
    g_pm_interrupt_status = reg_irq_src0;
    g_pm_interrupt_status1 = reg_irq_src1;
    g_pm_interrupt_status2 = read_csr(NDS_MIE);
    reg_irq_src0 = 0;
    reg_irq_src1 = 0;
    core_mie_enable(FLD_MIE_MEIE);
    core_mie_disable(FLD_MIE_MSIE|FLD_MIE_MTIE);

    //The clock source of analog is pclk, that is, the speed of reading and writing analog registers is related to cclk and pclk, before cclk=24M pclk=24M hclk=24M,
    //when the clock is switched to 24M RC before sleep, pclk is still 24M, this approach is no problem, and the early wake-up time in the pm function is calculated according to this clock.
    //When cclk=96M, the execution speed of the code will become faster, and when cclk is switched to 24M RC, pclk=6M will cause the analog register time to become longer,
    //which will cause deviations in the calculation of the early wake-up time in the previous pm function.modify by junhui.hu, confirmed by jianzhi at 20210923.
    mspi_stop_xip();
    unsigned char cclk_reg = read_reg8(0x1401e8);
    write_reg8(0x1401e8, cclk_reg & 0x8f);              //change cclk to 24M rc clock
    unsigned char div_reg = read_reg8(0x1401d8);
    write_reg8(0x1401d8, div_reg & 0xf8);               //change clock division to 1:1:1
    unsigned char mspiclk_reg = read_reg8(0x1401c0);
    write_reg8(0x1401c0, (mspiclk_reg & 0x80) | 0x01);  //change mspiclk to 24M rc clock

#if SYS_TIMER_AUTO_MODE
    BM_CLR(reg_system_irq_mask,BIT(0));
    REG_ADDR8(0x140218) = 0x01;     //system tick only update upon 32k posedge, must set before enable 32k read update!!!
    BM_CLR(reg_system_ctrl,FLD_SYSTEM_32K_TRACK_EN);//disable 32k track
    g_pm_tick_32k_cur = clock_get_32k_tick();
    g_pm_tick_cur = stimer_get_tick();
    BM_SET(reg_system_st,FLD_SYSTEM_CMD_STOP);  //write 1, stop system timer when using auto mode
#else
    g_pm_tick_cur = stimer_get_tick() + 37 * SYSTEM_TIMER_TICK_1US;  //cpu_get_32k_tick will cost 30~40 us stimer = 24M
    BM_CLR(reg_system_irq_mask,BIT(0));
    BM_CLR(reg_system_ctrl,FLD_SYSTEM_TIMER_EN | FLD_SYSTEM_TIMER_AUTO | FLD_SYSTEM_32K_TRACK_EN);//disable 32k track and stimer
    g_pm_tick_32k_cur = clock_get_32k_tick ();
#endif


    /////////////////// set wakeup source /////////////////////////////////
    analog_write_reg8(0x4b, wakeup_src);
    analog_write_reg8(0x64, 0xff);              //clear all flag
    analog_write_reg8(0x7e, sleep_mode);        //sram retention

    unsigned int earlyWakeup_us;
    if(sleep_mode & DEEPSLEEP_RETENTION_FLAG)  //deepsleep with retention
    {
        extern void pm_retention_register_save(void);
        pm_retention_register_save();//after retention, flash/debug cipher encryption keys is lost, save it
        if(wakeup_src & PM_WAKEUP_COMPARATOR)
        {
            analog_write_reg8(0x4d,0xfe);//retention
        }
        else
        {
            analog_write_reg8(0x4d,0xff);//retention
        }

        analog_write_reg8(0x00, (analog_read_reg8(0x00) | 0xe0));   //<7-5>:ldo_main_trim,  default:100,->111 digital LDO output voltage trim: 1.15V
        analog_write_reg8(0x09, (analog_read_reg8(0x09) & 0x3f));       //<6>:pd_sw_dcore,  default:1,->0 power up the main dig ldo to dcore.
                                                                        //<7>:pd_sw_sram,   default:1,->0 power up the main dig ldo to sram.
        analog_write_reg8(0x06, (analog_read_reg8(0x06) | 0xf0) & 0x7f);//<4>:pd_ldo_dcore, default:0,->1 Power down of digital core ldo.
                                                                        //<5>:pd_ldo_sram,  default:0,->1 Power down of sram ldo.
                                                                        //<6>:spd_ldo_pd,   default:1,->1 Power down spd ldo.
                                                                        //<7>:dig_ret_pd,   default:1,->0 Power on retention ldo.

        g_pm_mspi_cfg = read_reg32(0x23FFFF20);

        earlyWakeup_us = g_pm_early_wakeup_time_us.deep_ret_early_wakeup_time_us;
    }
    else if(sleep_mode == DEEPSLEEP_MODE)  //deepsleep no retention
    {
        analog_write_reg8(SYS_DEEP_ANA_REG, analog_read_reg8(SYS_DEEP_ANA_REG) & (~SYS_NEED_REINIT_EXT32K)); //clear
        //0x4d:
        //1:auto power down:<0>lpc <1>dcore/sram LDO <2>UVLO ib <3>vbus switch <4>flash LDO
        //<6>power down sequence enable <7>enable isolation
        if(wakeup_src & PM_WAKEUP_COMPARATOR)
        {
            analog_write_reg8(0x4d,0xfe);//deep
        }
        else
        {
            analog_write_reg8(0x4d,0xff);//deep
        }
        analog_write_reg8(0x00, (analog_read_reg8(0x00) | 0xe0));   //<7-5>:ldo_main_trim,  default:100,->111 digital LDO output voltage trim: 1.15V
        analog_write_reg8(0x09, (analog_read_reg8(0x09) & 0x3f));   //<6>:pd_sw_dcore,  default:1,->0 power up the main dig ldo to dcore.
                                                                    //<7>:pd_sw_sram,   default:1,->0 power up the main dig ldo to sram.
        analog_write_reg8(0x06, (analog_read_reg8(0x06) | 0xf0));   //<4>:pd_ldo_dcore, default:0,->1 Power down of digital core ldo.
                                                                    //<5>:pd_ldo_sram,  default:0,->1 Power down of sram ldo.
                                                                    //<6>:spd_ldo_pd,   default:1,->1 Power down spd ldo.
                                                                    //<7>:dig_ret_pd,   default:1,->1 Power down retention ldo.

        earlyWakeup_us = g_pm_early_wakeup_time_us.deep_early_wakeup_time_us;
    }
    else  //suspend
    {
        //0x4d:
        //1:auto power down:<0>lpc <1>dcore/sram LDO <2>UVLO ib <3>vbus switch <4>flash LDO
        //<6>power down sequence enable <7>enable isolation
        if(wakeup_src & PM_WAKEUP_COMPARATOR)
        {
            analog_write_reg8(0x4d,0x7e);//suspend
        }
        else
        {
            analog_write_reg8(0x4d,0x7f);//suspend
        }
        analog_write_reg8(0x00, (analog_read_reg8(0x00) | 0xe0));   //<7-5>:ldo_main_trim,  default:100,->111 digital LDO output voltage trim: 1.15V
        analog_write_reg8(0x09, (analog_read_reg8(0x09) & 0x3f));       //<6>:pd_sw_dcore,  default:1,->0 power up the main dig ldo to dcore.
                                                                        //<7>:pd_sw_sram,   default:1,->0 power up the main dig ldo to sram.
        analog_write_reg8(0x06, (analog_read_reg8(0x06) | 0xf0) & 0xbf);//<4>:pd_ldo_dcore, default:0,->1 Power down of digital core ldo.
                                                                        //<5>:pd_ldo_sram,  default:0,->1 Power down of sram ldo.
                                                                        //<6>:spd_ldo_pd,   default:1,->0 Power up spd ldo.
                                                                        //<7>:dig_ret_pd,   default:1,->1 Power down retention ldo.

        earlyWakeup_us = g_pm_early_wakeup_time_us.suspend_early_wakeup_time_us;
    }


    if(sleep_mode == DEEPSLEEP_MODE && !timer_wakeup_enable){ //if deep mode and no tmr wakeup
        pm_switch_ext32kpad_to_int32krc();
    }
    //suspend mode or deep retention mode or timer wakeup source.
    //(we don't power down external 32k pad clock, even though three's no timer wakeup source in suspend or deep retention mode)
    else
    {
        analog_write_reg8(0x4c, 0xed);//if use timer wakeup, auto pad 32k power down should be disabled
    }


    if(sleep_mode == DEEPSLEEP_MODE)
    {
//      analog_write_reg8 (0x3d, g_pm_r_delay_cycle.deep_xtal_delay_cycle);//driver_mark
        analog_write_reg8 (0x3e, g_pm_r_delay_cycle.deep_r_delay_cycle);//(n):  if timer wake up : (n*2) 32k cycle; else pad wake up: (n*2-1) ~ (n*2)32k cycle
    }
    else
    {
//      analog_write_reg8 (0x3d, g_pm_r_delay_cycle.suspend_ret_xtal_delay_cycle);//driver_mark
        analog_write_reg8 (0x3e, g_pm_r_delay_cycle.suspend_ret_r_delay_cycle);//(n):  if timer wake up : (n*2) 32k cycle; else pad wake up: (n*2-1) ~ (n*2)32k cycle
    }


    //The variable pmbcd.ref_tick is added, replacing the original variable g_pm_tick_cur. Because pmbcd.ref_tick directly affects the value of
    //g_pm_long_suspend, g_pm_long_suspend can be assigned after pmbcd.ref_tick is updated.changed by weihua,confirmed by biao.li.20201204.
    if(timer_wakeup_enable)
    {
        unsigned int tick_reset;
        unsigned int tick_wakeup_reset;
        tick_wakeup_reset = (unsigned int)(wakeup_tick - (earlyWakeup_us * SYSTEM_TIMER_TICK_1US) - g_pm_tick_cur);

        g_pm_tick_32k_calib = CRYSTAL32768_TICK_PER_64CYCLE;
        if(tick_wakeup_reset > 0x03ff0000)      // 24M: 2.79S
        {
            tick_reset = g_pm_tick_32k_cur + tick_wakeup_reset / g_pm_tick_32k_calib * 64;
            g_pm_long_suspend = 1;
        }
        else
        {
            tick_reset = g_pm_tick_32k_cur + (tick_wakeup_reset * 64 + (CRYSTAL32768_TICK_PER_64CYCLE>>1)) / g_pm_tick_32k_calib;
            g_pm_long_suspend = 0;
        }
        clock_set_32k_tick(tick_reset);


    }
    if(analog_read_reg8(0x64)&0x1f){

    }
    else{
        if(sleep_mode & DEEPSLEEP_RETENTION_FLAG)
        {
            analog_write_reg8(0x7f, (0x44 | g_pm_pad_filter_en));
        }
        DBG_CHN0_LOW;
        pm_sleep_start(sleep_mode);
        DBG_CHN0_HIGH;
        analog_write_reg8(0x7f, (0x45 | g_pm_pad_filter_en));
    }

    if(sleep_mode == DEEPSLEEP_MODE){
       write_reg8 (0x1401ef, 0x20);  //reboot
    }
    analog_write_reg8(0x06, (analog_read_reg8(0x06) | 0xc0) & 0xcf);//<4>:pd_ldo_dcore, default:1,->0 Power up of digital core ldo.
                                                                    //<5>:pd_ldo_sram,  default:1,->0 Power up of sram ldo.
                                                                    //<6>:spd_ldo_pd,   default:0,->1 Power down spd ldo.
                                                                    //<7>:dig_ret_pd,   default:1,->1 Power down retention ldo.
    analog_write_reg8(0x09, (analog_read_reg8(0x09) | 0xc0));       //<6>:pd_sw_dcore,  default:0,->1 power down the main dig ldo to dcore.
                                                                    //<7>:pd_sw_sram,   default:0,->1 power down the main dig ldo to sram.
    analog_write_reg8(0x00, (analog_read_reg8(0x00) | 0xe0) & 0x9f);//<7-5>:ldo_main_trim,  default:111,->100 digital LDO output voltage trim: 1.0V
    reg_system_irq_mask |= BIT(0);
#if SYS_TIMER_AUTO_MODE
    REG_ADDR8(0x140218) = 0x02; //sys tick 16M set upon next 32k posedge
    reg_system_ctrl |= (FLD_SYSTEM_TIMER_AUTO | FLD_SYSTEM_32K_TRACK_EN);
    unsigned int now_tick_32k = clock_get_32k_tick();
    if(g_pm_long_suspend){
        g_pm_tick_cur = g_pm_tick_cur + (unsigned int)(now_tick_32k + 1 - g_pm_tick_32k_cur) / 64 * CRYSTAL32768_TICK_PER_64CYCLE;
    }else{
        g_pm_tick_cur = g_pm_tick_cur + (unsigned int)(now_tick_32k + 1 - g_pm_tick_32k_cur) * CRYSTAL32768_TICK_PER_64CYCLE / 64;
    }
    reg_system_tick = g_pm_tick_cur + 1;        // current clock

    //wait cmd set dly done upon next 32k posedge
    //if not using status bit, wait at least 1 32k cycle to set register r_run_upon_next_32k back to 0, or before next normal set
    while((reg_system_st & BIT(7)) == 0);   //system timer set done status upon next 32k posedge
    REG_ADDR8(0x140218) = 0;                //normal sys tick (16/sys) update

#else
    unsigned int now_tick_32k = clock_get_32k_tick();
    if(g_pm_long_suspend){
        g_pm_tick_cur = g_pm_tick_cur + (unsigned int)(now_tick_32k - g_pm_tick_32k_cur) / 64 * CRYSTAL32768_TICK_PER_64CYCLE;
    }else{
        g_pm_tick_cur = g_pm_tick_cur + (unsigned int)(now_tick_32k - g_pm_tick_32k_cur) * CRYSTAL32768_TICK_PER_64CYCLE / 64;
    }


    reg_system_tick = g_pm_tick_cur + 20 * SYSTEM_TIMER_TICK_1US;
    reg_system_ctrl |= (FLD_SYSTEM_TIMER_EN | FLD_SYSTEM_32K_TRACK_EN); //enable 32k cal and stimer
#endif
    write_reg8(0x1401d8, div_reg);      //restore div
    write_reg8(0x1401e8, cclk_reg);     //restore cclk
    write_reg8(0x1401c0, mspiclk_reg);  //restore mspiclk
    mspi_set_xip_en();

    if( (g_pm_status_info.wakeup_src & WAKEUP_STATUS_TIMER) && timer_wakeup_enable )    //wakeup from timer only
    {
            while ((unsigned int)(stimer_get_tick() - wakeup_tick) > BIT(30));
    }
    //Resume the interrupted state before sleep.Cannot be placed in the pm_sleep_start() interface to avoid failure to recover if this interface is not called.
    //changed by weihua, confirmed by jianzhi. 20231115
    reg_irq_src0 = g_pm_interrupt_status;
    reg_irq_src1 = g_pm_interrupt_status1;
    write_csr(NDS_MIE,g_pm_interrupt_status2);
    core_restore_interrupt(r);
    return ( g_pm_status_info.wakeup_src  ? (g_pm_status_info.wakeup_src | STATUS_ENTER_SUSPEND ) : STATUS_GPIO_ERR_NO_ENTER_PM );
}

_attribute_text_sec_ _attribute_no_inline_ int cpu_sleep_wakeup_32k_xtal(pm_sleep_mode_e sleep_mode,  pm_sleep_wakeup_src_e wakeup_src, unsigned int  wakeup_tick)
{
    int status = 0;
    __asm__("csrci  mmisc_ctl,8");  //disable BTB
    status = cpu_sleep_wakeup_32k_xtal_ram(sleep_mode, wakeup_src, wakeup_tick);
    __asm__("csrsi  mmisc_ctl,8");  //enable BTB
    return status;
}

_attribute_ram_code_sec_noinline_ unsigned int pm_tim_recover_32k_xtal(unsigned int now_tick_32k)
{
    unsigned int deepRet_tick;

    if(g_pm_long_suspend){
        deepRet_tick = g_pm_tick_cur + (unsigned int)(now_tick_32k + 1 - g_pm_tick_32k_cur) / 64 * CRYSTAL32768_TICK_PER_64CYCLE;
    }else{
        deepRet_tick = g_pm_tick_cur + (unsigned int)(now_tick_32k + 1 - g_pm_tick_32k_cur) * CRYSTAL32768_TICK_PER_64CYCLE / 64;
    }

    return deepRet_tick;
}



_attribute_data_retention_sec_ static unsigned int tick_check32kPad = 0;


_attribute_no_inline_ void check_32k_clk_stable(void)
{
    if(clock_time_exceed(tick_check32kPad, 10000)){ //every 10ms, check if 32k pad clk is stable
        tick_check32kPad = clock_time ();

        unsigned int last_32k_tick;
        unsigned int curr_32k_tick;

        //Check if 32k pad vibration and basically works stably
        last_32k_tick = clock_get_32k_tick ();
        delay_us(50); //for 32k tick accumulator, tick period: 30.5us, if stable: delta tick > 0
        curr_32k_tick = clock_get_32k_tick();

        if(last_32k_tick != curr_32k_tick){
            blt_miscParam.pm_enter_en = 1;//allow enter pm
            return;
        }
    }
    else{
        return;
    }

    // T > 2s , 32k pad clk still unstable: reboot MCU
    if(!blt_miscParam.pm_enter_en && clock_time_exceed(0, 2000000)){
        analog_write_reg8(SYS_DEEP_ANA_REG, analog_read_reg8(SYS_DEEP_ANA_REG) & (~SYS_NEED_REINIT_EXT32K)); //clr
        start_reboot(); //reboot the MCU
    }
}



#endif







