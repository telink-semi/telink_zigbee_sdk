/********************************************************************************************************
 * @file    ext_pm.c
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
//#include "config/user_config.h"
#include "../../lib/include/pm/pm.h"
#include "../../lib/include/stimer.h"
#include "../../lib/include/pm/pm_internal.h"
#include "../ext_pm.h"
#include "ext_lib.h"
#include  "../mcu_boot.h"
//#include "usbhw.h"




_attribute_data_retention_sec_  _attribute_aligned_(4) misc_para_t      blt_miscParam;

_attribute_data_retention_sec_  suspend_handler_t   func_before_suspend = 0;

_attribute_data_retention_sec_  cpu_pm_handler_t            cpu_sleep_wakeup;  //no need retention,  cause it will be set every wake_up
_attribute_data_retention_sec_  pm_tim_recover_handler_t    pm_tim_recover;

_attribute_data_retention_sec_  unsigned char       pm_check_info = 0;
_attribute_data_retention_sec_  unsigned int        ota_program_bootAddr = MULTI_BOOT_ADDR_0x40000; //default 256K
_attribute_data_retention_sec_  unsigned int        ota_firmware_max_size = (MULTI_BOOT_ADDR_0x40000 - 0x1000);  //unit: Byte, - 4K is important
_attribute_data_retention_sec_  unsigned int        ota_program_offset = 0;//




void bls_pm_registerFuncBeforeSuspend (suspend_handler_t func )
{
    func_before_suspend = func;
}


/**
 * @brief       This function serves to initiate the cpu after power on or deepsleep mode.
 * @param[in]   none.
 * @return      none.
 */
_attribute_no_inline_ void cpu_wakeup_no_deepretn_back_init(void)
{

    if(blt_miscParam.pad32k_en){
        clock_32k_init(CLK_32K_XTAL);
        clock_kick_32k_xtal(10);
    }else {
        clock_32k_init(CLK_32K_RC);
        clock_cal_32k_rc(); //6.68ms
    }

    clock_cal_24m_rc();

    /*
     * 0,64K,128K,256K,512K,1M,2M,4M,8M
     */
    //boot flag storage
    //Only need read 1 byte. If read 2 bytes, in secure boot mode, erase err
    unsigned short boot_flag = reg_mspi_xip_core_offset;
    if (boot_flag)
    {
        ota_program_offset = 0;
    }
    else
    {
        ota_program_offset = ota_program_bootAddr;
    }
}


/**
 * @brief   internal oscillator calibration for environment change such as voltage, temperature
 *          to keep some critical PM or RF performance stable
 *          attention: this is a stack API, user can not call it
 * @param   none
 * @return  none
 */
_attribute_data_retention_sec_ int tick_rc24mCal = BIT(31);
void mcu_oscillator_crystal_calibration(void)
{
    if( clock_time_exceed(tick_rc24mCal, 59995*1000)){  //cal 24m every 60 second
        tick_rc24mCal = stimer_get_tick();
        clock_cal_24m_rc();  //469 us/474 us
    }
}


_attribute_data_retention_sec_ pm_clock_drift_t pmbcd = {0}; //= {0, 0, 0, 0, 0, 0};
_attribute_ram_code_sec_optimize_o2_noinline_ void ext_pm_ble_update_32k_rc_sleep_tick (unsigned int * tick_32k, unsigned int * tick)
{
      pmbcd.rc32_rt = *tick_32k - pmbcd.rc32_wakeup; // rc32_rt not used

      pmbcd.ref_tick_32k_enter = *tick_32k;
      pmbcd.ref_tick_enter = *tick;

    if (pmbcd.calib || pmbcd.ref_no > 20 || !pmbcd.ref_tick || ((*tick_32k - pmbcd.ref_tick_32k) & 0xfffffff) > 32 * 2000)
    {
        pmbcd.calib = 0;
        pmbcd.ref_tick_32k = *tick_32k;
        pmbcd.ref_tick = (*tick) | 1;
        pmbcd.ref_no = 0;
    }
    else
    {
      *tick_32k = pmbcd.ref_tick_32k;
      *tick = pmbcd.ref_tick;
        pmbcd.ref_no++;
    }
}

/**
 * @brief       32k rc calibration clock compensation.
 * @return      32k calibration value after compensation.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ unsigned int ext_pm_ble_get_32k_rc_calib (void)
{
    while(!stimer_get_tracking_32k_value());
    int tc = stimer_get_tracking_32k_value();

    pmbcd.s0 = tc;
    tc = tc << 4;
    if (!pmbcd.tc)
    {
        pmbcd.tc = tc;
    }
    else
    {
        pmbcd.tc += (tc - pmbcd.tc) >> (4 - pmbcd.calib);
    }

    int offset = (pmbcd.offset * (pmbcd.tc >> 4)) >> 18;        //offset : tick per 256ms
    offset = (pmbcd.tc >> 4) + offset;
    return (unsigned int)offset;
}

#if 1
/**
 * @brief       When 32k rc sleeps, the calibration function is initialized.
 * @return      none.
 */
void pm_32k_rc_offset_init(void)
{
    pmbcd.offset = 0;
    pmbcd.tc = 0;
    pmbcd.ref_tick = 0;
}

_attribute_ram_code_sec_noinline_ void pm_ble_update_32k_rc_sleep_tick (unsigned int tick_32k, unsigned int tick)
{
    pmbcd.rc32_rt = tick_32k - pmbcd.rc32_wakeup; //rc32_rt not used
    if (pmbcd.calib || pmbcd.ref_no > 20 || !pmbcd.ref_tick || ((tick_32k - pmbcd.ref_tick_32k) & 0xfffffff) > 32 * 3000)//3S
    {
        pmbcd.calib = 0;
        pmbcd.ref_tick_32k = tick_32k;
        pmbcd.ref_tick = tick | 1;
        pmbcd.ref_no = 0;
    }
    else
    {
        pmbcd.ref_no++;
    }
}

_attribute_ram_code_sec_noinline_ void pm_ble_32k_rc_cal_reset(void)
{
    pmbcd.offset = 0;
    pmbcd.tc = 0;
    pmbcd.ref_tick = 0;
    pmbcd.offset_cal_tick = 0;
}

#if 1
_attribute_ram_code_sec_noinline_ void pm_ble_cal_32k_rc_offset (int offset_tick, int rc32_cnt)
{
    int offset = offset_tick * (256 * 31) / rc32_cnt;       //256mS / sleep_period
    int thres = rc32_cnt/9600;  //240*32=7680  300*32= 9600  400*32= 12800
    if(!thres){
        thres = 1;
    }
//  else if(thres > 8){
//      thres = 8;
//  }
    thres *= 0x100;

    if (offset > thres)
    {
        offset = thres;
    }
    else if (offset < -thres)
    {
        offset = -thres;
    }
    pmbcd.calib = 1;
    pmbcd.offset += (offset - pmbcd.offset) >> 4;
//  pmbcd.offset_dc += (offset_tick - pmbcd.offset_dc) >> 3;
    pmbcd.offset_cal_tick  = stimer_get_tick() | 1;
}
#else
_attribute_ram_code_sec_noinline_ void pm_ble_cal_32k_rc_offset (int offset_tick)
{
//  pmbcd.offset_cur = offset_tick;
    int offset = offset_tick * (240 * 31) / pmbcd.rc32;     //240ms / sleep_period
    if (offset > 0x100)
    {
        offset = 0x100;
    }
    else if (offset < -0x100)
    {
        offset = -0x100;
    }
    pmbcd.calib = 1;
    pmbcd.offset += (offset - pmbcd.offset) >> 4;
    pmbcd.offset_dc += (offset_tick - pmbcd.offset_dc) >> 3;
}
#endif
/**
 * @brief       32k rc calibration clock compensation.
 * @return      32k calibration value after compensation.
 */
_attribute_ram_code_sec_noinline_ unsigned int pm_ble_get_32k_rc_calib (void)
{
        while (!stimer_get_tracking_32k_value())
            ; //Wait for the 32k clock calibration to complete.

        int tc =  stimer_get_tracking_32k_value();
    pmbcd.s0 = tc;
    tc = tc << 4;
    if (!pmbcd.tc)
    {
        pmbcd.tc = tc;
    }
    else
    {
        pmbcd.tc += (tc - pmbcd.tc) >> (4 - pmbcd.calib);
    }

    int offset = (pmbcd.offset * (pmbcd.tc >> 4)) >> 18;        //offset : tick per 256ms
    offset = (pmbcd.tc >> 4) + offset;
    return (unsigned int)offset;
}

#else
/**
 * @brief       When 32k rc sleeps, the calibration function is initialized.
 * @return      none.
 */
void pm_32k_rc_offset_init(void)
{
    pmbcd.offset = 0;
    pmbcd.tc = 0;
    pmbcd.ref_tick = 0;
}

_attribute_ram_code_sec_noinline_ void pm_ble_32k_rc_cal_reset(void)
{
    pmbcd.offset = 0;
    pmbcd.tc = 0;
    pmbcd.ref_tick = 0;
    pmbcd.offset_cal_tick = 0;
}

_attribute_ram_code_sec_noinline_ void pm_ble_update_32k_rc_sleep_tick (unsigned int tick_32k, unsigned int tick)
{
    pmbcd.rc32_rt = tick_32k - pmbcd.rc32_wakeup; //rc32_rt not used
    if (pmbcd.calib || pmbcd.ref_no > 20 || !pmbcd.ref_tick || ((tick_32k - pmbcd.ref_tick_32k) & 0xfffffff) > 32 * 3000)//3S
    {
        pmbcd.calib = 0;
        pmbcd.ref_tick_32k = tick_32k;
        pmbcd.ref_tick = tick | 1;
        pmbcd.ref_no = 0;
    }
    else
    {
        pmbcd.ref_no++;
    }
}


_attribute_ram_code_sec_noinline_ void pm_ble_trigger_cal_32k_rc_update (void)
{
    pmbcd.calib = 1;
    pmbcd.offset_cal_tick  = stimer_get_tick() | 1;
}

//clk
_attribute_ram_code_sec_noinline_ void pm_ble_cal_32k_rc_offset (int offset_tick, int rc32_cnt)
{
   //todo
    (void) offset_tick;
    (void) rc32_cnt;
}
/**
 * @brief       32k rc calibration clock compensation.
 * @return      32k calibration value after compensation.
 */
_attribute_ram_code_sec_noinline_ unsigned int pm_ble_get_32k_rc_calib (void)
{
    while (!stimer_get_tracking_32k_value())
        ; //Wait for the 32k clock calibration to complete.

      return  stimer_get_tracking_32k_value();
  
}
#endif


extern _attribute_text_sec_ _attribute_no_inline_ int cpu_sleep_wakeup_32k_rc(pm_sleep_mode_e sleep_mode,  pm_sleep_wakeup_src_e wakeup_src, unsigned int  wakeup_tick);
void blc_pm_select_internal_32k_crystal(void)
{
    cpu_sleep_wakeup        = cpu_sleep_wakeup_32k_rc;
    pm_tim_recover          = pm_tim_recover_32k_rc;

    blt_miscParam.pm_enter_en   = 1; // allow enter pm, 32k rc does not need to wait for 32k clk to be stable
}

/**
 * @brief       32k rc calibration clock compensation.
 * @return      32k calibration value after compensation.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void ext_pm_ble_exit_pm(unsigned int tick_32k, unsigned int tick)
{
    pmbcd.rc32_wakeup = tick_32k;
    pmbcd.rc32 = tick_32k - pmbcd.ref_tick_32k;

    g_sleep_32k_rc_cnt = tick_32k - pmbcd.ref_tick_32k_enter;
    g_sleep_stimer_tick = tick -  pmbcd.ref_tick_enter ;
}


