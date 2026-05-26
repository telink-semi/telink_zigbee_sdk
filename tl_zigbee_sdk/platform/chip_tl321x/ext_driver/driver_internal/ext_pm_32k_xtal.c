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
#include "lib/include/pm/pm.h"
#include "lib/include/pm/pm_internal.h"
#include "lib/include/core.h"
#include "lib/include/mspi.h"
#include "lib/include/clock.h"
#include "lib/include/stimer.h"
#include "watchdog.h"

#include "ext_lib.h"

extern unsigned int                       g_pm_multi_addr;
extern unsigned char                      g_pm_pad_filter_en;
extern void  pm_sleep_start(pm_sleep_mode_e sleep_mode);
extern void pm_ble_update_32k_rc_sleep_tick (unsigned int tick_32k, unsigned int tick);
_attribute_ram_code_ int cpu_sleep_wakeup_32k_xtal_ram(pm_sleep_mode_e sleep_mode,  pm_sleep_wakeup_src_e wakeup_src, unsigned int  wakeup_tick)
{
    pm_wakeup_tick_type_e wakeup_tick_type;
    g_clk_32k_src = CLK_32K_XTAL;
    wakeup_tick_type =  PM_TICK_STIMER;
     /**
         * At present, the compensation value in the function is tested on the basis of the optimization level O2. If there are other optimization levels,
         * it needs to be retested and then see how the compensation time is handled.
         */

        ////////// disable IRQ //////////////////////////////////////////
        //If the time point of closing the total interrupt is later, the function may be interrupted by the interrupt,
        //cause the wake-up tick value to be calculated incorrectly, resulting in incorrect sleep time.
        //modify by weihua.zhang, confirmed by sihui.wang at 20220908.
        unsigned int r = core_interrupt_disable();

    #if PM_START_CODE_DEBUG
        gpio_set_low_level(GPIO_PB5);
    #endif

        ///////////////////       /////////////////////////////////
        int timer_wakeup_enable = (wakeup_src & PM_WAKEUP_TIMER);
        if (timer_wakeup_enable) {
            if (wakeup_tick_type == PM_TICK_STIMER) {
                unsigned int span = (unsigned int)(wakeup_tick - stimer_get_tick());
                if (span > 0xE0000000) //BIT(31)+BIT(30)+BIT(29)   7/8 cycle of 32bit, 178*7/8 = 156 S
                {
                    core_restore_interrupt(r);
                    return pm_get_wakeup_src() | STATUS_EXCEED_MAX;
                } else if (span < g_pm_early_wakeup_time_us.sleep_min_time_us * SYSTEM_TIMER_TICK_1US) {
                    unsigned int t = stimer_get_tick();
                    pm_clr_irq_status(FLD_WAKEUP_STATUS_ALL);
                    unsigned char st;
                    do {
                        st = pm_get_wakeup_src();
                    } while (((unsigned int)stimer_get_tick() - t < span) && !(st & WAKEUP_STATUS_INUSE_ALL));

    #if (PM_DEBUG)
                    /******************************************debug_pm_info 1 **********************************************/
                    debug_pm_info         = 1;
                    debug_min_wakeup_src  = st;
                    debug_min_stimer_tick = t;
    #endif

                    core_restore_interrupt(r);
                    return st | STATUS_EXCEED_MIN;
                }
            } else {
                //What is the minimum time us, converted to how many 32k ticks.
                //When the minimum time is 2ms, the difference between /31.25 and /31 is 0.5 ticks, so 31 can also be used.
                if (wakeup_tick < g_pm_early_wakeup_time_us.sleep_min_time_us / 31) {
                    unsigned int t = clock_get_32k_tick() - 1;
                    pm_clr_irq_status(FLD_WAKEUP_STATUS_ALL);
                    unsigned char st;
                    do {
                        st = pm_get_wakeup_src();
                    } while (((unsigned int)clock_get_32k_tick() - t < wakeup_tick) && !(st & WAKEUP_STATUS_INUSE_ALL));

    #if (PM_DEBUG)
                    /******************************************debug_pm_info 1 **********************************************/
                    debug_pm_info         = 1;
                    debug_min_wakeup_src  = st;
                    debug_min_stimer_tick = t;
    #endif

                    core_restore_interrupt(r);
                    return st | STATUS_EXCEED_MIN;
                }
            }
        }

        //Turn off all interrupts immediately after entering the sleep function to prevent other interrupts, save the interrupt state before turning off, and restore it after waking up.
        //Turn on pm interrupt only before going to sleep,enable M-mode external interrupt first in this place.
        //modify by bingyu.li, confirmed by jianzhi.chen at 20230810.
        plic_irqs_preprocess_for_wfi(0, FLD_MIE_MEIE);

    #if (PM_DEBUG)
        /******************************************debug_pm_info 2 **********************************************/
        debug_pm_info = 2;
    #endif

        ///////////////////    change clock    /////////////////////////////////
        //The clock source of analog is pclk, that is, the speed of reading and writing analog registers is related to cclk and pclk, before cclk=24M pclk=24M hclk=24M,
        //when the clock is switched to 24M RC before sleep, pclk is still 24M, this approach is no problem, and the early wake-up time in the pm function is calculated according to this clock.
        //When cclk=96M, the execution speed of the code will become faster, and when cclk is switched to 24M RC, pclk=6M will cause the analog register time to become longer,
        //which will cause deviations in the calculation of the early wake-up time in the previous pm function.modify by junhui.hu, confirmed by jianzhi at 20210923.
        mspi_stop_xip();

        clock_save_clock_config();
        clock_set_all_clock_to_default();

    #if (PM_DEBUG)
        /******************************************debug_pm_info 3 **********************************************/
        debug_pm_info = 3;
    #endif

        ///////////////////     get 32k calib      /////////////////////////////////
        if (CLK_32K_RC == g_clk_32k_src) {
            while (!stimer_get_tracking_32k_value()); //Wait for the 32k clock calibration to complete.

            g_pm_tick_32k_calib = stimer_get_tracking_32k_value();
        } else {
    #if (STIMER_CLOCK == STIMER_CLOCK_16M)
            g_pm_tick_32k_calib = CRYSTAL32768_TICK_PER_32CYCLE;
    #elif (STIMER_CLOCK == STIMER_CLOCK_24M)
            g_pm_tick_32k_calib = CRYSTAL32768_TICK_PER_64CYCLE;
    #endif
        }
        unsigned int tick_32k_halfCalib = g_pm_tick_32k_calib >> 1;

    #if (PM_DEBUG)
        analog_write_reg16(PM_ANA_REG_POWER_ON_CLR_BUF1, g_pm_tick_32k_calib);
        /******************************************debug_pm_info 4 **********************************************/
        debug_pm_info      = 4;
        debug_tick_32k_cur = analog_read_reg32(0x60);
    #endif

        /////////////////// stop system timer /////////////////////////////////
    #if SYS_TIMER_AUTO_MODE
        stimer_32k_tracking_disable();           //disable 32k track
        stimer_set_update_upon_nxt_32k_enable(); //system tick only update upon 32k posedge, must set before enable 32k read update!!!
        g_pm_tick_32k_cur = clock_get_32k_tick();
        g_pm_tick_cur     = stimer_get_tick();
        stimer_set_update_upon_nxt_32k_disable();
        stimer_disable(); //disable system timer
    #else
        #error-- Manual mode is only for internal testing, and 37 in the code may not be accurate
        stimer_32k_tracking_disable();                                  //disable 32k track
        g_pm_tick_cur = stimer_get_tick() + 37 * SYSTEM_TIMER_TICK_1US; //cpu_get_32k_tick will cost 30~40 us
        stimer_disable();                                               //disable system timer
        g_pm_tick_32k_cur = clock_get_32k_tick();
    #endif

    #if PM_START_CODE_DEBUG
        gpio_set_high_level(GPIO_PB5);
    #endif

    #if PM_MIN_CODE_DEBUG
        gpio_set_low_level(GPIO_PB5);
    #endif

    #if (PM_DEBUG)
        /******************************************debug_pm_info 5 **********************************************/
        debug_pm_info         = 5;
        debug_min_code_tick_1 = rdmcycle();
    #endif

        unsigned int earlyWakeup_us;
        if (sleep_mode & DEEPSLEEP_RETENTION_FLAG) //deep sleep with retention
        {
            earlyWakeup_us = g_pm_early_wakeup_time_us.deep_ret_early_wakeup_time_us;
        } else if (sleep_mode == DEEPSLEEP_MODE)   //deepsleep no retention
        {
            earlyWakeup_us = g_pm_early_wakeup_time_us.deep_early_wakeup_time_us;
        } else                                     //suspend
        {
            earlyWakeup_us = g_pm_early_wakeup_time_us.suspend_early_wakeup_time_us;
        }

#if PM_32k_RC_CALIBRATION_ALGORITHM_EN
    pm_ble_update_32k_rc_sleep_tick (g_pm_tick_32k_cur, g_pm_tick_cur);//Todo:need check
#endif

        //The variable pmcd.ref_tick is added, replacing the original variable g_pm_tick_cur. Because pmcd.ref_tick directly affects the value of
        //g_pm_long_suspend, g_pm_long_suspend can be assigned after pmcd.ref_tick is updated.changed by weihua,confirmed by biao.li.20201204.
        if (timer_wakeup_enable) {
            unsigned int tick_reset = 0;
            unsigned int tick_wakeup_reset;
            tick_wakeup_reset = wakeup_tick - earlyWakeup_us * SYSTEM_TIMER_TICK_1US;

            #if PM_32k_RC_CALIBRATION_ALGORITHM_EN

                #if (STIMER_CLOCK == STIMER_CLOCK_16M)
                   unsigned int max_span_ticks = 0x07ff0000; // 16M: 8.38S
                   unsigned int track_32kcnt = 32;
                #elif (STIMER_CLOCK == STIMER_CLOCK_24M)
                   unsigned int max_span_ticks = 0x03ff0000; // 24M: 2.79S
                   unsigned int track_32kcnt = 64;
                #else
                   #error "error STIMER_CLOCK"
                #endif

                if( (unsigned int)(tick_wakeup_reset - pmbcd.ref_tick) > max_span_ticks){      // 24M: 11.18S, 16M: 16.77S
                    tick_reset = g_pm_tick_32k_cur + (unsigned int)(tick_wakeup_reset - g_pm_tick_cur)/ g_pm_tick_32k_calib * track_32kcnt;
                    g_pm_long_suspend = 1;
                }
                else{
                    tick_reset = g_pm_tick_32k_cur + ((unsigned int)(tick_wakeup_reset - g_pm_tick_cur) * track_32kcnt + tick_32k_halfCalib) / g_pm_tick_32k_calib;
                    g_pm_long_suspend = 0;
                }
            #endif
            //The clock_ct_32k_tick interface needs to avoid encountering rising edges of 32k as much as possible.
            //Otherwise, if the intermediate data generated during the clock_set_32k_tick process has the same value as 32k tick, it will cause the wake-up source to be set.
            //The interval time between the clock_get_32k_tick() and clock_set_32k_tick() interfaces should be as short as possible
            //to avoid the clock_set_32k_tick() interface encountering a rising edge of 32k.
            //add by weihua.zhang at 20240827
            clock_set_32k_tick(tick_reset);

    #if (PM_DEBUG)
            analog_write_reg32(PM_ANA_REG_WD_CLR_BUF1, g_pm_tick_32k_cur);
            debug_ana_32k_tick   = analog_read_reg32(areg_aon_0x65);
            debug_ana_tick_reset = tick_reset;
            if (tick_reset != debug_ana_32k_tick) {
                stimer_enable_in_manual_mode();
                stimer_32k_tracking_enable(); //enable 32k cal
                gpio_set_high_level(GPIO_PE7);
                while (1);
            }
            /******************************************debug_pm_info 6 **********************************************/
            debug_pm_info         = 6;
            debug_min_code_tick_2 = rdmcycle();
    #endif
        }

        /////////////////// set wakeup source /////////////////////////////////
        pm_set_wakeup_src(wakeup_src);

    #if (PM_DEBUG)
        /******************************************debug_pm_info 7 **********************************************/
        debug_pm_info = 7;
    #endif

        /////////////////// auto power down /////////////////////////////////
        unsigned short auto_power_down = FLD_PD_32K_RC | FLD_PD_32K_XTAL | FLD_PD_24M_XTAL | FLD_PD_DCDC | FLD_PD_VBUS_LDO | FLD_PD_ANA_BBPLL_TEMP_LDO | FLD_PD_VBUS_SW | FLD_PD_SEQUENCE_EN;
        unsigned char  sleep_ldo_en    = 0;


        /*
         * afe_0x7e<2:0> sram_ret default(000)
         * afe_0x7e<6:4> sram_slp default(000)
         * Analog 0x7e will remain value after sleep, the sram_slp is use to control the sram voltage not only in deep retention sleep mode but also take effect in suspend sleep mode.
         * If enter suspend sleep mode after enter deep retention sleep mode(setup not retention all sram), then the SRAM will also not retention all sram during suspend sleep mode.
         * So for tl321x it needs to be rewritten when enter suspend sleep mode.
         * (added by jilong.liu, confirmed by jianzhi.chen at 20241225)
         */
        analog_write_reg8(areg_aon_0x7e, sleep_mode);

        if (sleep_mode & DEEPSLEEP_RETENTION_FLAG)                                       //deep sleep with retention
        {
            g_pm_multi_addr = reg_mspi_xip_core_size | (reg_mspi_xip_core_offset << 16); //after retention, multiple address offset is lost, save it

            auto_power_down |= FLD_PD_ISOLATION;
            sleep_ldo_en = FLD_PD_DIG_RET_LDO;
        } else if (sleep_mode == DEEPSLEEP_MODE) //deepsleep no retention
        {
            auto_power_down |= FLD_PD_ISOLATION;
            sleep_ldo_en = 0;
        } else //suspend
        {
            sleep_ldo_en = FLD_PD_SPD_LDO;
        }

        if (!(wakeup_src & PM_WAKEUP_COMPARATOR)) {
            auto_power_down |= FLD_PD_LPC;
        }

        if (((wakeup_src & PM_WAKEUP_PAD) && g_pm_pad_filter_en) || (wakeup_src & PM_WAKEUP_TIMER) || (wakeup_src & PM_WAKEUP_COMPARATOR)) {
            if (CLK_32K_RC == g_clk_32k_src) {
                auto_power_down &= ~FLD_PD_32K_RC; //disable auto power down 32KRC
            } else {
                //suspend mode or deep retention mode or timer wake up source.
                //(we don't power down external 32k pad clock, even though three's no timer wake up source in suspend or deep retention mode)
                auto_power_down &= ~FLD_PD_32K_XTAL; //if use timer wake up, auto pad 32k power down should be disabled
            }
        } else {
            if (CLK_32K_XTAL == g_clk_32k_src) {
                if (sleep_mode == DEEPSLEEP_MODE) {
                    //deep + no tmr wakeup(we need  32k clk to count dcdc dly and xtal dly, but this case, ext 32k clk need close, here we use 32k rc instead.
                    //switch 32k clk src: select internal 32k rc, if not do this, when deep+pad wakeup: there's no stable 32k clk(therefore, the pad wake-up time
                    //is a bit long, the need for external 32k crystal vibration time) to count DCDC dly and XTAL dly. High temperatures even make it impossible
                    //to vibrate, as the code for PWM excitation crystals has not yet been effectively executed. SO, we can switch 32k clk to the internal 32k rc.
                    clock_32k_init(CLK_32K_RC);
                } else {
                    auto_power_down &= ~FLD_PD_32K_XTAL; //if use timer wake up, auto pad 32k power down should be disabled
                }
            }
        }

        //default:1, 0 Power up ; 1 power down;
        //areg_aon_0x06 need to restore after wake up
        analog_write_reg8(areg_aon_0x06, (analog_read_reg8(areg_aon_0x06) | FLD_PD_BBPLL_LDO | FLD_PD_SPD_LDO | FLD_PD_DIG_RET_LDO) & (~sleep_ldo_en));
        sys_clk_config.bbpll_is_used = g_bbpll_is_used;
        g_bbpll_is_used              = 0;
        analog_write_reg16(areg_aon_0x4c, auto_power_down);

    #if (PM_DEBUG)
        /******************************************debug_pm_info 8 **********************************************/
        debug_pm_info = 8;
    #endif

        /////////////////// R DELAY AND XTAL DELAY /////////////////////////////////
        if (sleep_mode == DEEPSLEEP_MODE) {
            pm_set_delay_cycle(g_pm_r_delay_cycle.deep_xtal_delay_cycle, g_pm_r_delay_cycle.deep_r_delay_cycle);
        } else {
            pm_set_delay_cycle(g_pm_r_delay_cycle.suspend_ret_xtal_delay_cycle, g_pm_r_delay_cycle.suspend_ret_r_delay_cycle);
        }

    #if (PM_DEBUG)
        /******************************************debug_pm_info 9 **********************************************/
        debug_pm_info = 9;
    #endif

        //Clear the wake source status after setting the wake tick.The wake tick value is set by bit shift.
        //This process will generate an intermediate value, which may be the same as the current 32k tick value.
        //If the value is the same, the state of the timer wake source will be set.
        //changed by weihua, confirmed by jianzhi. 20240711.
        pm_clr_irq_status(FLD_WAKEUP_STATUS_ALL); //clear all flag

    #if (PM_DEBUG)
        debug_sleep_start_wakeup_src1 = pm_get_wakeup_src();
        /******************************************debug_pm_info 11 **********************************************/
        debug_pm_info = 11;
    #endif

        if (pm_get_wakeup_src() & WAKEUP_STATUS_INUSE_ALL) {
    #if (PM_DEBUG)
            debug_sleep_start_wakeup_src2 = pm_get_wakeup_src();
            debug_sleep_start_cur_tick    = analog_read_reg32(0x60);
            debug_sleep_start_set_tick    = analog_read_reg32(0x65);
    #endif

        } else {
            if (sleep_mode & DEEPSLEEP_RETENTION_FLAG) {
                g_areg_aon_7f = (g_areg_aon_7f & (~FLD_BOOTFROMBROM)) | g_pm_pad_filter_en;
            } else {
                g_areg_aon_7f = (g_areg_aon_7f | FLD_BOOTFROMBROM | g_pm_pad_filter_en);
            }
            analog_write_reg8(areg_aon_0x7f, g_areg_aon_7f);

            pm_sleep_start(sleep_mode);
        }

    #if (PM_DEBUG)
        /******************************************debug_pm_info 12 **********************************************/
        debug_pm_info = 12;
    #endif

        if (sleep_mode == DEEPSLEEP_MODE) {
            sys_reset_all(); //reboot
        }

    #if (PM_DEBUG)
        /******************************************debug_pm_info 13 **********************************************/
        debug_pm_info = 13;
    #endif
        u32 now_tick_stimer;
        stimer_enable(STIMER_AUTO_MODE_W_AND_NXT_32K_START, 0);

        unsigned int now_tick_32k = clock_get_32k_tick() + 1;

        now_tick_stimer = pm_tim_recover_32k_xtal(now_tick_32k);

        stimer_set_tick(now_tick_stimer+1);
        //wait command set delay done upon next 32k posedge.
        //if not using status bit, wait at least 1 32k cycle to set register r_run_upon_next_32k back to 0, or before next normal set
        stimer_wait_write_done();               //system timer set done status upon next 32k posedge

        stimer_set_run_upon_nxt_32k_disable();  //disable the function to trigger the operation at the next 32k rise edge
    #if (PM_DEBUG)
        /******************************************debug_pm_info 14 **********************************************/
        debug_pm_info = 14;
    #endif

        clock_restore_clock_config();

        mspi_set_xip_en();

    #if (PM_DEBUG)
        /******************************************debug_pm_info 15 **********************************************/
        debug_pm_info = 15;
    #endif

    #if PM_SUSPEND_WHILE_DEBUG_2
        gpio_set_high_level(GPIO_PB5);
    #endif

    #if PM_SUSPEND_WHILE_DEBUG
        gpio_set_low_level(GPIO_PB5);
    #endif
        if ((g_pm_status_info.wakeup_src & WAKEUP_STATUS_TIMER) && timer_wakeup_enable) //wakeup from timer only
        {
            if (wakeup_tick_type == PM_TICK_STIMER) {
                while ((unsigned int)(stimer_get_tick() - wakeup_tick) > BIT(30));
            } else {
                while ((unsigned int)(clock_get_32k_tick() - wakeup_tick - g_pm_tick_32k_cur + 1) > BIT(30));
            }
        }

    #if PM_SUSPEND_WHILE_DEBUG
        gpio_set_high_level(GPIO_PB5);
    #endif

    #if (PM_DEBUG)
        /******************************************debug_pm_info 16 **********************************************/
        debug_pm_info = 16;
    #endif

        //  DBG_CHN2_LOW;
        //Resume the interrupted state before sleep.Cannot be placed in the pm_sleep_start() interface to avoid failure to recover if this interface is not called.
        //changed by weihua, confirmed by jianzhi. 20231115
        plic_irqs_postprocess_for_wfi();
        core_restore_interrupt(r);

    #if (PM_DEBUG)
        /******************************************debug_pm_info 17 **********************************************/
        debug_pm_info = 17;
    #endif

        /**
         * Under normal circumstances, the wake up source cannot be zero. B85 had an exception that the wake up source was zero and never encountered it again.
         * STATUS_GPIO_ERR_NO_ENTER_PM indicates this case where the wake up source is zero. The name of this flag is not quite appropriate, but it has been used for a long time, so it is still used today.
         * added by bingyu.li, confirmed by sihui.wang at 20231018.
         */
        return (g_pm_status_info.wakeup_src ? (g_pm_status_info.wakeup_src | STATUS_ENTER_SUSPEND) : STATUS_GPIO_ERR_NO_ENTER_PM);

}

_attribute_text_sec_ _attribute_no_inline_ int cpu_sleep_wakeup_32k_xtal(pm_sleep_mode_e sleep_mode,  pm_sleep_wakeup_src_e wakeup_src, unsigned int  wakeup_tick)
{
    if(func_before_suspend){
        if (!func_before_suspend())
        {
            return WAKEUP_STATUS_PAD;
        }
    }

    int status = 0;
    DISABLE_BTB;
#if  1//!USE_DRIVER_PM  //debug
    status = cpu_sleep_wakeup_32k_xtal_ram(sleep_mode, wakeup_src, wakeup_tick);
#else  //debug
    extern  int pm_sleep_wakeup_ram(pm_sleep_mode_e sleep_mode,  pm_sleep_wakeup_src_e wakeup_src, pm_wakeup_tick_type_e wakeup_tick_type, unsigned int  wakeup_tick);
    status = pm_sleep_wakeup_ram(sleep_mode, wakeup_src, PM_TICK_STIMER, wakeup_tick);
#endif
    ENABLE_BTB;
    return status;
}

_attribute_ram_code_ unsigned int pm_tim_recover_32k_xtal(unsigned int now_tick_32k)
{

       unsigned int deepRet_tick;


    #if PM_32k_RC_CALIBRATION_ALGORITHM_EN
        if (g_pm_long_suspend) {
            deepRet_tick = pmbcd.ref_tick + (unsigned int)(now_tick_32k - pmbcd.ref_tick_32k) / 64 * CRYSTAL32768_TICK_PER_64CYCLE;
        } else {
            deepRet_tick = pmbcd.ref_tick + (unsigned int)(now_tick_32k - pmbcd.ref_tick_32k) * CRYSTAL32768_TICK_PER_64CYCLE / 64;
        }
    #else
        #error " next version supports"
    #endif
    g_sleep_32k_rc_cnt = now_tick_32k - g_pm_tick_32k_cur;
    g_sleep_stimer_tick = deepRet_tick - g_pm_tick_cur;

    #if PM_32k_RC_CALIBRATION_ALGORITHM_EN
       // pmbcd.rc32_wakeup = now_tick_32k;
       // pmbcd.rc32 = now_tick_32k - pmbcd.ref_tick_32k;
    #endif
    return deepRet_tick;
}

_attribute_data_retention_sec_ static unsigned int tick_check32kPad = 0;
_attribute_no_inline_ void check_32k_clk_stable(void)
{

if(clock_time_exceed(tick_check32kPad, 10000)){ //every 10ms, check if 32k pad clk is stable
    tick_check32kPad = stimer_get_tick ();

    unsigned int last_32k_tick;
    unsigned int curr_32k_tick;

    //Check if 32k pad vibration and basically works stably
    last_32k_tick = pm_get_32k_tick();
    delay_us(50); //for 32k tick accumulator, tick period: 30.5us, if stable: delta tick > 0
    curr_32k_tick = pm_get_32k_tick();

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







