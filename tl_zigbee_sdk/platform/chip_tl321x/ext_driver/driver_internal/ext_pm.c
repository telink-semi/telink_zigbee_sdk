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
#include "../../lib/include/analog.h"
#include "../../dma.h"
#include "../../gpio.h"
#include "../../lib/include/pm/pm.h"
#include "../../lib/include/pm/pm_internal.h"
#include "../../adc.h"
#include "../../timer.h"
#include "../../flash.h"
#include "../../lib/include/trng/trng.h"
#include "../../lib/include/sys.h"
#include "../../lib/include/plic.h"
#include "../../lib/include/stimer.h"
#include "../../lib/include/clock.h"
#include "../../lib/include/rf/rf_common.h"
#include "../ext_misc.h"
#include "../ext_pm.h"
#include "ext_lib.h"
#include "../mcu_boot.h"
//#include "config/user_config.h"
#include "usbhw.h"




_attribute_data_retention_  _attribute_aligned_(4) misc_para_t      blt_miscParam;

_attribute_data_retention_sec_  suspend_handler_t   func_before_suspend = 0;

_attribute_data_retention_sec_  cpu_pm_handler_t            cpu_sleep_wakeup;  //no need retention,  cause it will be set every wake_up
_attribute_data_retention_sec_  pm_tim_recover_handler_t    pm_tim_recover;
_attribute_data_retention_sec_  check_32k_clk_handler_t     pm_check_32k_clk_stable = 0;

_attribute_data_retention_sec_  unsigned char       pm_check_info = 0;
_attribute_data_retention_sec_  unsigned int        ota_program_bootAddr = MULTI_BOOT_ADDR_0x40000; //default 256K
_attribute_data_retention_sec_  unsigned int        ota_firmware_max_size = (MULTI_BOOT_ADDR_0x40000 - 0x1000);  //unit: Byte, - 4K is important
_attribute_data_retention_sec_  unsigned int        ota_program_offset = 0;//

/*  TL321x Flash protection
    BUT-46/BUT-40: Buteo Flash will be changed by mistake under power-on/off rapidly.
    Solution: Set PLC IRQ when awake. During power off, the PLC IRQ handler will be triggered.
    In IRQ handler reset and stall MCU and MSPI and make CCLK=24M to confirm no MSPI signal sent to flash.
    by PM@shenyan and Driver@xiaobin,kaixin at 2025.06. */
_attribute_data_retention_sec_  unsigned char       pm_flash_prot_by_lpc = 0;

extern drv_api_status_e efuse_read(unsigned char addr, unsigned char* buff, unsigned short len);
extern void efuse_check_protection_code(unsigned char sdk_version);
/**
 * @brief     This function servers to get protection code from EFUSE(byte 104).
 * @return    The protection code value.
 */
unsigned char efuse_get_protection_code(void)
{
    unsigned char protection_code;
    efuse_read(104, (u8*)&protection_code, 1);
    return protection_code;
}

//An internal user API. When user needs to use this, please extern this API.
void blt_flash_prot_method(unsigned char en)
{
    //en is used as enable/disable now. If other ICs need similar feature, en can be used as strategy.
    pm_flash_prot_by_lpc = en;
}

void bls_pm_registerFuncBeforeSuspend (suspend_handler_t func )
{
    func_before_suspend = func;
}


/**
 * @brief     this function servers to start reboot.
 * @param[in] none
 * @return    none
 */
_attribute_ram_code_ void mcu_reboot(void)
{
    mspi_stop_xip();

    if(blt_miscParam.pad32k_en){
        analog_write_reg8(SYS_DEEP_ANA_REG, analog_read_reg8(SYS_DEEP_ANA_REG) & (~SYS_NEED_REINIT_EXT32K)); //clear
    }

    #if (!WDT_REBOOT_RESET_ANA7F_WORK_AROUND)
    if(blt_miscParam.pad32k_en || blt_miscParam.pm_enter_en){
        analog_write_reg8(0x7f, 0x01);  // 0x7f reboot does not reset
    }
    #endif

    core_interrupt_disable ();
    REG_ADDR8(0x14082f) = 0x20;  //reboot
    while (1);
}

_attribute_text_sec_ _attribute_no_inline_ void start_reboot(void)
{
    DISABLE_BTB;
    mcu_reboot();
    ENABLE_BTB;
}

#if 0
/**
 * @brief       This function serves to initiate the cpu after power on or deepsleep mode.
 * @param[in]   none.
 * @return      none.
 */
static _attribute_no_inline_ void cpu_wakeup_no_deepretn_back_init(void)
{

    if(blt_miscParam.pad32k_en){
        clock_32k_init(CLK_32K_XTAL);
        clock_kick_32k_xtal(10);
    }else {
        clock_32k_init(CLK_32K_RC);
        clock_cal_32k_rc(); //6.68ms
    }



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
//static _attribute_no_inline_ void cpu_wakeup_no_deepretn_back_init(void)
//{
//  unsigned char sel_32k   = analog_read_reg8(0x4e)&0x7f;
//  unsigned char power_32k = analog_read_reg8(0x05)&0xfc;
//  //Set 32k clk src: external 32k crystal, only need init when deep+pad wakeup or 1st power on
//  if(blt_miscParam.pad32k_en){
//      analog_write_reg8(0x4e, sel_32k|(CLK_32K_XTAL<<7));
//      analog_write_reg8(0x05, power_32k|0x1);//32k xtal
//      g_clk_32k_src = CLK_32K_XTAL;
//      //in this case: ext 32k clk was closed, here need re-init.
//      //when deep_pad wakeup or first power on, it needs pwm acc 32k pad vibration time(dly 10ms)
//      if(!(analog_read_reg8(SYS_DEEP_ANA_REG) & SYS_NEED_REINIT_EXT32K)){
//          analog_write_reg8(SYS_DEEP_ANA_REG,  analog_read_reg8(SYS_DEEP_ANA_REG) | SYS_NEED_REINIT_EXT32K); //if initialized, the FLG is set to "1"
//          clock_kick_32k_xtal(10);
//      }
//      else{
//          delay_us(6000);
//      }
//  }
//  else{
//      analog_write_reg8(0x4e, sel_32k|(CLK_32K_RC<<7));
//      analog_write_reg8(0x05, power_32k|0x2);//32k rc
//      clock_cal_32k_rc();  //6.69 ms/6.7 ms
//  }
//
//
//  /*  0x23FFFF20 mspi_set_l: mutiboot address offset option, 0:0k;  1:128k;  2:256k;  4:512k
//   *  0x23FFFF21 mspi_set_h: program space size = (mspi_set_h+1)*4k
//   *
//   *  Normal mode       mspi_set_l            mspi_set_h          mspi_multi_boot in description
//   *  FW on 0x00000:      0x00                   0x00                             N/A
//   *  FW on 0x20000:      0x01                   0x1F                             N/A
//   *  FW on 0x40000:      0x02                   0x3F                             N/A
//   *  FW on 0x80000:      0x04                   0x7F                             N/A
//   *
//   *  Secure boot mode
//   *  FW on 0x00000:      0x00                   0x7F                         0x0000
//   *  FW on 0x20000:      0x01                   0x1F                         0x2020
//   *  FW on 0x40000:      0x02                   0x3F                         0x4040
//   *  FW on 0x80000:      0x04                   0x7F                         0x8080
//   */
//
//
//
//  //boot flag storage
//  //Only need read 1 byte. If read 2 bytes, in secure boot mode, erase err
//  unsigned char boot_flag = read_reg8(0x23FFFF20);
//  if (boot_flag)
//  {
//      ota_program_offset = 0;
//  }
//  else
//  {
//      ota_program_offset = ota_program_bootAddr;
//  }
//}
//static _always_inline void efuse_check_protection_code(void)
//{
//  unsigned char pCode;
//
//  if(efuse_read(104, &pCode, 1) == 0)
//  {
//      pCode = 0xff;
//  }
//
//  pCode &= 0x0f;
//
//  /* Jaguar: IOT + LE Audio ( pCode == 0 ) */
//  if(pCode == 0){
//      pm_check_info = 0xFF;
//  }
//
//  if(9 < pCode) //LE AUDIO:0   LE multi:9
//  {
////        reg_pwdn_en = 0x20;/*reboot*///driver_mark
//      while(1);
//  }
//}


/**
 * @brief       This function serves to initialize system.
 * @param[in]   vbat_v      - This parameter is used to determine whether the VBAT voltage can be greater than 3.6V.
 * @attention   If vbat_v is set to VBAT_MAX_VALUE_LESS_THAN_3V6, then gpio_v can only be set to GPIO_VOLTAGE_3V3.
 * @return      none
 * @note        For crystal oscillator with slow start-up or poor quality, after calling this function,
 *              a reboot will be triggered(whether a reboot has occurred can be judged by using PM_ANA_REG_POWER_ON_CLR_BUF0[bit1]).
 *              For the case where the crystal oscillator used is very slow to start-up, you can call the pm_set_xtal_stable_timer_param interface
 *              to adjust the waiting time for the crystal oscillator to start before calling the sys_init interface.
 *              When this time is adjusted to meet the crystal oscillator requirements, it will not reboot.
 */
_attribute_ram_code_sec_noinline_ void sys_init(power_mode_e power_mode, vbat_type_e vbat_v, cap_typedef_e cap)
{
#if 0
    /*
     * Reset function will be cleared by set "1",which is different from the previous configuration.
     * This setting turns off the TRNG and NPE modules in order to test power consumption.The current
     * decrease about 3mA when those two modules be turn off.changed by zhiwei,confirmed by kaixin.20200828.
     */
    reg_rst = 0xffffffff;
    reg_clk_en = 0xffffffff;

    reg_rst_1 = 0xffffffff;
    reg_clk_en_1 = 0xffffffff;
#else
    /*
     * Reset function will be cleared by set "1".
     * Turn off the following modules compared to the default values: sspi, brom.
     * Turn on the following modules compared to the default values: uart0, uart1, uart2, stimer, dma, algm.
     * Overall, the enabled modules here includes: dbgen, swires, stimer, dma, algm, machinetime, mcu, lm, trace, mspi, cclk.
     * uart0, uart1, uart2, efuse, pwm, timer.
     * among them, the uart, pwm, and timer modules do not have appropriate positions to enable them in the module interface, so they are also enabled here.
     */
    reg_rst    = 0x92390ed4;   //reset_0_dft:  0x96388080 -> 0x92380ed4
    reg_clk_en = 0x1a312ef4;   //clken_0_dft:  0x1e30a0a0 -> 0x1a312ef4

    reg_rst_1    = 0x00000204; //reset_1_dft:  0x00000004 -> 0x00000204
    reg_clk_en_1 = 0x00000244; //clken_1_dft:  0x00000044 -> 0x00000244
#endif
    /*
     * 1. Before calling the function pm_wait_xtal_ready, you need to ensure that the cclk is set to 24M,
     * otherwise a reboot may occur (for example, the following use case: when the flash is running a cclk>24M program,
     * without powering down the chip to load a ram program, at this time the nop timing judgment in the pm_wait_xtal_ready function is incorrect,
     * which may lead to a reboot)(add by weihua.zhang, confirmed by kaixin 20230609)
     * 2. Before calling the function crystal_manual_settle, you need to ensure that the cclk is set to 24M,
     * cause the crystal_manual_settle will power down xtal first then power it up.(add by jilong.liu at 20240513)
     */
    analog_write_reg8(areg_aon_0x05, analog_read_reg8(areg_aon_0x05) & ~(FLD_24M_RC_PD)); //power on 24M RC
    clock_set_all_clock_to_default();

    //A0: 0x00, A1: 0x01
    g_chip_version = read_reg8(0x14083d);

    //If the crystal oscillator uses an external capacitor, the internal capacitor must be turned off at the very beginning,
    //otherwise it will affect the start-up.(add by bingyu.li, confirmed by wenfeng.lou 20240621)
    if (cap != INTERNAL_CAP_XTAL24M) {
        rf_turn_off_internal_cap();
    }

    if (g_chip_version == CHIP_VERSION_A0) {
        /* 
         * For version A0, the RC 24M is high to 32M, register ana0x16<1:0> needs to be configured as 2b'01, the frequency is about 25MHz. 
         * XTAL will kick by RC, so trim it before.(updated by jilong.liu, confirmed by yangya at 20240827)
         */
        analog_write_reg8(0x16, (analog_read_reg8(0x16) & 0xfc) | 0x01);
    }

    //must to set xo_quick_settle with manual and wait it stable(added by jilong.liu, confirmed by wenfeng 20240320)
    crystal_manual_settle();

    g_areg_aon_7f = analog_read_reg8(areg_aon_0x7f);
    if (!(g_areg_aon_7f & FLD_BOOTFROMBROM)) {
        g_pm_status_info.mcu_status = MCU_DEEPRET_BACK;
    }

    pm_set_power_mode(power_mode);

    /*
     *                      poweron_dft:    0x7f -> 0x3f.
     *      pd_bit                      note
     * ---------------------------------------------------------------------------
     * <0>:pd_nvt_0p94  default:1,->1 power down 0p94 native transistor.
     * <1>:pd_nvt_1p8   default:1,->1 power down 1p8 native transistor.
     * <6>:mscn_pullup_res_enb  default:1,->0 enable 1M pullup resistor for mscn PAD.
     */
    /*
     * After waking up, it is not safe to power supply both the native LDO and the normal LDO together.
     * Therefore, this code will be processed in advance here to reduce the shared power supply time.(add by jilong.liu, 20240221)
     */
    analog_write_reg8(areg_aon_0x0b, (analog_read_reg8(areg_aon_0x0b) & (~FLD_MSCN_PULLUP_RES_ENB)) | (FLD_PD_NVT_0P94 | FLD_PD_NVT_1P8));

    /*
     *                      poweron_dft:    0xff -> 0xff.
     *      pd_bit                      note
     * ---------------------------------------------------------------------------
     * <0>:pd_bbpll_ldo  default:1,->0 Power up BBPLL LDO.
     * <3>:pd_vbat_sw  default:1,->vbat_v Power down of bypass switch(VBAT LDO).
     * <6>:spd_ldo_pd  default:1,->1 Power down of suspend LDO.
     * <7>:dig_ret_pd  default:1,->0 Power up of retention LDO.
     */
    /*
     * For tl321x, the retention ldo should turn on before use cause it need more time to be stable.
     * The follow chip version will not change this situation.
     * (modified by jilong.liu, confirmed by wenfeng 20240620)
     */
    analog_write_reg8(areg_aon_0x06, (analog_read_reg8(areg_aon_0x06) | FLD_PD_SPD_LDO | FLD_PD_VBAT_SW) & ~(FLD_PD_DIG_RET_LDO | vbat_v));

    g_pm_status_info.wakeup_src    = pm_get_wakeup_src();
    g_pm_status_info.is_pad_wakeup = (g_pm_status_info.wakeup_src & BIT(0));

    g_pm_vbat_v = vbat_v >> 3;

    if (g_chip_version == CHIP_VERSION_A0) {
        //For version A0, the theoretical value is low, and the current enumeration value is set after actual testing.
        pm_set_dig_ldo_voltage(DIG_LDO_TRIM_1P010V); //trim VDDDEC to 1V
        pm_set_1p25v(TRIM_1P25V_TO_1P25V);           //trim VDD1P25 to 1.25V
    }

    /*
    * Turn on xtal_24M clock to analog (includes stimer), this should setup before power up PLL and call pm_wait_xtal_ready.
    * Because the stimer is necessary for the pm_wait_xtal_ready.
    * (add by jilong.liu, confirmed by wenfeng.lou 20240513)
    */
    analog_write_reg8(areg_0x8c, 0x02);

    /*
     * Setting the Bandgap voltage to the highest level makes the absolute value of the adc vref voltage change with temperature smaller,
     * thus improving the problem of inaccurate sampling of the adc with temperature changes.(jira:BUT-72)(confirmed by bolong.zhang, peng.liu 20250325)
     */
    analog_write_reg8(areg_aon_0x00, (analog_read_reg8(areg_aon_0x00) & (~FLD_BG_TRIM_3V)) | 0x0e);

    //The xo_ready_ana signal fails, and the tick value of the clock is used to determine whether the crystal oscillator is stable.
    pm_wait_xtal_ready(0x00);

    //Before powering up the zb, you need to make sure that the input function of PA2 is disable, otherwise it may cause the RF module to not have clock.
    //See jira for details:BUT-53.(add by weihua.zhang, confirmed by peng.hao 20250101)
    reg_gpio_pa_ie &= (~0x04);
    //The RF power supply must be enabled only after the crystal oscillator has stabilized.(modified by chenxi.wang, confirmed by wenfeng 20250820)
    pm_set_dig_module_power_switch(FLD_PD_ZB_EN, PM_POWER_UP);

    if(g_pm_status_info.mcu_status == MCU_STATUS_DEEPRET_BACK)
    {
#if (_2P4G_DEMO_ || USE_DRIVER_PM)
        pm_stimer_recover();
#else
        stimer_set_run_upon_nxt_32k_enable();   //system tick set upon next 32k posedge.
        stimer_set_auto_enable_mode();

        unsigned int deepRet_32k_tick = clock_get_32k_tick();
        unsigned int deepRet_tick = pm_tim_recover(deepRet_32k_tick + 1); // pm_tim_recover_32k_rc


        stimer_set_tick(deepRet_tick+1);
        //wait command set delay done upon next 32k posedge.
        //if not using status bit, wait at least 1 32k cycle to set register r_run_upon_next_32k back to 0, or before next normal set
        stimer_wait_write_done();               //system timer set done status upon next 32k posedge
        stimer_set_run_upon_nxt_32k_disable();  //normal system tick update

        stimer_32k_tracking_enable();
#endif
    } else {
#if SYS_TIMER_AUTO_MODE
        stimer_enable(STIMER_AUTO_MODE_W_TRIG, 0x01);
        stimer_32k_tracking_enable(); //enable 32k cal
#else
        stimer_enable(STIMER_MANUAL_MODE, 0x01);
        stimer_32k_tracking_enable(); //enable 32k cal
#endif
        /*
        * When using the default ret ldo voltage setting (0.8V), the voltage will drop below 0.6V during deep ret sleep, causing the RAM to fail to function properly.
        * Raising the voltage can improve this situation, but it cannot completely solve the problem. It can only be used as a temporary solution.
        * (add by nanshun.ouyang, confirmed by wenfeng.lou 20260210)
        */
        pm_set_ret_ldo_voltage(RET_LDO_TRIM_0P95V);

        cpu_wakeup_no_deepretn_back_init(); // to save ramcode

        //check protection code
        u8 zbble_sdk_version = 1; //Buteo Zigbee BLE concurrent SDK version use 1.refer to the relate email
        efuse_check_protection_code(zbble_sdk_version); //0:driver sdk  0xff:sdk_version_ignore

        clock_cal_24m_rc();
    }
    // enable pke module by default
    //pke_reset()
    reg_rst1 &= ~FLD_RST1_PKE;
    reg_rst1 |= FLD_RST1_PKE;
    //pke_clk_en()
    reg_clk_en1 |= FLD_CLK1_PKE_EN;
}
#endif

unsigned int clock_get_digital_32k_tick(void)
{
    unsigned int timer_32k_tick;
    reg_system_st = FLD_SYSTEM_CLR_RD_DONE;//clr rd_done
    while((reg_system_st & FLD_SYSTEM_CLR_RD_DONE) != 0);//wait rd_done = 0;
    reg_system_ctrl &= ~FLD_SYSTEM_32K_WR_EN;   //1:32k write mode; 0:32k read mode
    while((reg_system_st & FLD_SYSTEM_CLR_RD_DONE) == 0);//wait rd_done = 1;
    timer_32k_tick = reg_system_timer_read_32k;
    reg_system_ctrl |= FLD_SYSTEM_32K_WR_EN;    //1:32k write mode; 0:32k read mode
    return timer_32k_tick;
}




_attribute_data_retention_ int tick_rc24mCal = BIT(31);


/**
 * @brief   internal oscillator calibration for environment change such as voltage, temperature
 *          to keep some critical PM or RF performance stable
 *          attention: this is a stack API, user can not call it
 * @param   none
 * @return  none
 */
void mcu_oscillator_crystal_calibration(void)
{
    if( clock_time_exceed(tick_rc24mCal, 59995*1000)){  //cal 24m every 60 second
        tick_rc24mCal = stimer_get_tick();
        clock_cal_24m_rc();  //469 us/474 us
    }
}


_attribute_data_retention_ pm_clock_drift_t pmbcd; //= {0, 0, 0, 0, 0, 0};
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

_attribute_ram_code_ void pm_ble_update_32k_rc_sleep_tick (unsigned int tick_32k, unsigned int tick)
{
//    pmbcd.rc32_rt = tick_32k - pmbcd.rc32_wakeup; //rc32_rt not used
    if (pmbcd.calib || pmbcd.ref_no > 20 || !pmbcd.ref_tick || ((tick_32k - pmbcd.ref_tick_32k) & 0xfffffff) > pmbcd.cal_max_interval)//3S
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
_attribute_ram_code_ unsigned int pm_ble_get_32k_rc_calib (void)
{
    while(!read_reg32(0x140214));   //Wait for the 32k clock calibration to complete.
    int tc = read_reg32(0x140214);

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











_attribute_ram_code_
unsigned int cpu_stall_WakeUp_By_RF_SystemTick(int WakeupSrc, unsigned short rf_mask, unsigned int tick)
{
    (void)WakeupSrc;(void)rf_mask;(void)tick;
    /*
   unsigned short rf_irq_mask_save =0;
   unsigned char systimer_irq_mask_save = 0;

   unsigned int r=core_interrupt_disable();

   rf_irq_mask_save = reg_rf_irq_mask;
   systimer_irq_mask_save = reg_system_irq_mask;


   unsigned int plic_save = 0;
   plic_save = reg_irq_src(0);

   plic_interrupt_enable(IRQ1_SYSTIMER);
   plic_interrupt_enable(IRQ14_ZB_BT);

    if( WakeupSrc & FLD_IRQ_ZB_RT_EN )
    {
        reg_rf_irq_mask = rf_mask;
    }

    if( WakeupSrc & FLD_IRQ_SYSTEM_TIMER )
    {
        stimer_set_irq_capture(tick);
        stimer_set_irq_mask(FLD_SYSTEM_IRQ);
    }

    core_entry_wfi_mode();//WFI instruction enables the processor to enter the wait-for-interrupt (WFI) mode

    reg_rf_irq_status = FLD_RF_IRQ_ALL;
    stimer_clr_irq_status(FLD_SYSTEM_IRQ);

    reg_irq_src(0) &= ~(BIT(IRQ1_SYSTIMER)|BIT(IRQ14_ZB_BT));
    reg_irq_src(0) |= plic_save;

    reg_rf_irq_mask = rf_irq_mask_save;
    reg_system_irq_mask = systimer_irq_mask_save;

    core_restore_interrupt(r);
*/
    return 0;
}

extern  check_32k_clk_handler_t     pm_check_32k_clk_stable;
extern  void check_32k_clk_stable(void);
void blc_pm_select_external_32k_crystal(void)
{
    cpu_sleep_wakeup        = cpu_sleep_wakeup_32k_xtal;
    pm_tim_recover          = pm_tim_recover_32k_xtal;  //
    blt_miscParam.pad32k_en     = 1; // set '1': 32k clk src use external 32k crystal
    pmbcd.cal_max_interval = 2000 * 31;  // 1/32768 ~= 30.5 ~= 31  = 2s
}


void blc_pm_select_internal_32k_crystal(void)
{
    cpu_sleep_wakeup        = cpu_sleep_wakeup_32k_rc;
    pm_tim_recover          = pm_tim_recover_32k_rc;

    blt_miscParam.pm_enter_en   = 1; // allow enter pm, 32k rc does not need to wait for 32k clk to be stable
    pmbcd.cal_max_interval = 3000 * 32;
}



