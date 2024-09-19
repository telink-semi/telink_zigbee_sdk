/********************************************************************************************************
 * @file    pm_internal.h
 *
 * @brief   This is the header file for TL321X
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#include "reg_include/register.h"
#include "compiler.h"
#include "lib/include/analog.h"
#include "lib/include/pm/pm.h"

/********************************************************************************************************
 *                                          internal
 *******************************************************************************************************/

/********************************************************************************************************
 *              This is currently included in the H file for compatibility with other SDKs.
 *******************************************************************************************************/

//When the watchdog comes back, the Eagle chip does not clear 0x7f[0]. To avoid this problem, this macro definition is added.
#ifndef WDT_REBOOT_RESET_ANA7F_WORK_AROUND
#define WDT_REBOOT_RESET_ANA7F_WORK_AROUND  1
#endif

/********************************************************************************************************
 *              This is just for internal debug purpose, users are prohibited from calling.
 *******************************************************************************************************/

/*
 * @note    This is for internal stability debugging use only.
 */
#define PM_DEBUG                        0
//1 PB4, 2 PB5
#define PM_SUSPEND_WHILE_DEBUG          0
#define PM_SUSPEND_WHILE_DEBUG_2        0
#define PM_MIN_CODE_DEBUG               0
#define PM_START_CODE_DEBUG             0
#define PM_XTAL_READY_DEBUG             0

//TODO:The A2 chip changes the default values of some analog registers to commonly configured values,
//which saves the time of configuring registers during initialization.
#define PM_A0_1_ANA_DEFAULT             1

//system timer clock source is constant 24M, never change
//NOTICE:We think that the external 32k crystal clk is very accurate, does not need to read through TIMER_32K_LAT_CAL.
//register, the conversion error(use 32k:64 cycle, count 24M sys tmr's ticks), at least the introduction of 64ppm.
#define STIMER_CLOCK_16M                    1
#define STIMER_CLOCK_24M                    2
#define STIMER_CLOCK                    STIMER_CLOCK_24M

#if(STIMER_CLOCK == STIMER_CLOCK_16M)
#define CRYSTAL32768_TICK_PER_32CYCLE   15625
#elif(STIMER_CLOCK == STIMER_CLOCK_24M)
#define CRYSTAL32768_TICK_PER_64CYCLE   46875
#endif


extern _attribute_data_retention_sec_ unsigned int  g_pm_tick_32k_calib;
extern _attribute_data_retention_sec_ unsigned int  g_pm_tick_cur;
extern _attribute_data_retention_sec_ unsigned int  g_pm_tick_32k_cur;
extern _attribute_data_retention_sec_ unsigned char g_pm_long_suspend;

/**
 * @brief dcdc trim soc out
 * 
 */
typedef enum{
    TRIM_1P25V_TO_1P050V = 0x0,
    TRIM_1P25V_TO_1P065V,
    TRIM_1P25V_TO_1P080V,
    TRIM_1P25V_TO_1P095V,
    TRIM_1P25V_TO_1P110V,
    TRIM_1P25V_TO_1P125V,
    TRIM_1P25V_TO_1P140V,
    TRIM_1P25V_TO_1P155V,
    TRIM_1P25V_TO_1P170V,
    TRIM_1P25V_TO_1P19V,
    TRIM_1P25V_TO_1P21V,
    TRIM_1P25V_TO_1P23V,
    TRIM_1P25V_TO_1P25V,
    TRIM_1P25V_TO_1P27V,
    TRIM_1P25V_TO_1P29V,
    TRIM_1P25V_TO_1P31V,
}pm_trim_1p25v_e;

/**
 * @brief trim dig ldo
 * 
 */
typedef enum {
    DIG_LDO_TRIM_0P735V = 0,
    DIG_LDO_TRIM_0P760V,
    DIG_LDO_TRIM_0P785V,
    DIG_LDO_TRIM_0P810V,
    DIG_LDO_TRIM_0P835V,
    DIG_LDO_TRIM_0P860V,
    DIG_LDO_TRIM_0P885V,
    DIG_LDO_TRIM_0P910V,
    DIG_LDO_TRIM_0P935V,
    DIG_LDO_TRIM_0P960V,
    DIG_LDO_TRIM_0P985V,
    DIG_LDO_TRIM_1P010V,
    DIG_LDO_TRIM_1P035V,
    DIG_LDO_TRIM_1P060V,
    DIG_LDO_TRIM_1P085V,
    DIG_LDO_TRIM_1P100V,
} pm_dig_ldo_trim_e;

/**
 * @brief trim suspend LDO
 *
 */
typedef enum {
    SPD_LDO_TRIM_0P75V = 0,
    SPD_LDO_TRIM_0P80V,
    SPD_LDO_TRIM_0P85V,
    SPD_LDO_TRIM_0P90V,
    SPD_LDO_TRIM_0P95V,
    SPD_LDO_TRIM_1P00V,
    SPD_LDO_TRIM_1P05V,
    SPD_LDO_TRIM_1P10V,
} pm_spd_ldo_trim_e;

/**
 * @brief trim deep retention LDO
 *
 */
typedef enum {
    RET_LDO_TRIM_0P55V = 0,
    RET_LDO_TRIM_0P60V,
    RET_LDO_TRIM_0P65V,
    RET_LDO_TRIM_0P70V,
    RET_LDO_TRIM_0P75V,
    RET_LDO_TRIM_0P80V,
    RET_LDO_TRIM_0P85V,
    RET_LDO_TRIM_0P90V,
} pm_ret_ldo_trim_e;

/**
 * @brief       This function serves to reboot system.
 * @return      none
 */
_always_inline void sys_reset_all(void)
{
#if(PM_DEBUG)
    while(1);
#endif
    reg_pwdn_en = 0x20;
}

/**
 * @brief       This function serves to trim dig ldo voltage
 * @param[in]   dig_ldo_trim - dig ldo trim voltage
 * @return      none
 */
static _always_inline void pm_set_dig_ldo_voltage(pm_dig_ldo_trim_e dig_ldo_trim)
{
    analog_write_reg8(areg_aon_0x0f, (analog_read_reg8(areg_aon_0x0f) & 0x0f) | (dig_ldo_trim << 4));
}

/**
 * @brief       This function serves to trim suspend LDO voltage
 * @param[in]   spd_ldo_trim - suspend LDO trim voltage
 * @return      none
 */
static _always_inline void pm_set_spd_ldo_voltage(pm_spd_ldo_trim_e spd_ldo_trim)
{
    analog_write_reg8(areg_aon_0x0e, (analog_read_reg8(areg_aon_0x0e) & 0xf8) | spd_ldo_trim);
}

/**
 * @brief       This function serves to trim deep retention LDO voltage
 * @param[in]   ret_ldo_trim - deep retention LDO trim voltage
 * @return      none
 */
static _always_inline void pm_set_ret_ldo_voltage(pm_ret_ldo_trim_e ret_ldo_trim)
{
    analog_write_reg8(areg_aon_0x0f, (analog_read_reg8(areg_aon_0x0f) & 0xf8) | ret_ldo_trim);
}

/**
 * @brief       This function serves to trim ldo 1.25v out.     
 * @param[in]   trim_1p25vldo - ldo trim 1.25v out
 */
static _always_inline void pm_set_1p25v(pm_trim_1p25v_e trim_1p25vldo)
{
    analog_write_reg8(areg_aon_0x09, (analog_read_reg8(areg_aon_0x09) & 0xf0) | trim_1p25vldo);
}

/**
 * @brief   This function is used to enable native LDO.
 * @return  none.
 */
static _always_inline void pm_enable_native_ldo(void)
{
    analog_write_reg8(areg_aon_0x0b, (analog_read_reg8(areg_aon_0x0b) & ~(FLD_PD_NVT_0P94 | FLD_PD_NVT_1P8)));
}

/**
 * @brief   This function is used to disable native LDO.
 * @return  none.
 */
static _always_inline void pm_disable_native_ldo(void)
{
    analog_write_reg8(areg_aon_0x0b, (analog_read_reg8(areg_aon_0x0b) | (FLD_PD_NVT_0P94 | FLD_PD_NVT_1P8)));
}

/**
 * @brief   This function is used to enable suspend LDO.
 * @return  none.
 */
static _always_inline void pm_enable_spd_ldo(void)
{
    analog_write_reg8(areg_aon_0x06, analog_read_reg8(areg_aon_0x06) & ~(FLD_PD_SPD_LDO));
}

/**
 * @brief   This function is used to disable suspend LDO.
 * @return  none.
 */
static _always_inline void pm_disable_spd_ldo(void)
{
    analog_write_reg8(areg_aon_0x06, analog_read_reg8(areg_aon_0x06) | FLD_PD_SPD_LDO);
}

/**
 * @brief   This function is used to disable suspend LDO and enable ret ldo.
 * @return  none.
 */
static _always_inline void pm_disable_spd_ldo_enable_ret_ldo(void)
{
    //<6>:spd_ldo_pd,   default:1,->1 Power down spd ldo.
    //<7>:dig_ret_pd,   default:1,->0 Power on retention ldo.
    analog_write_reg8(areg_aon_0x06, (analog_read_reg8(areg_aon_0x06) & 0x3f) | FLD_PD_SPD_LDO);
}

/**
 * @brief       This function configures the values of xtal_delay cycle and r_delay cycle.
 * @param[in]   xtal_delay - xtal_delay cycle.
 * @param[in]   r_delay - r_delay cycle.
 * @return      none.
 */
static _always_inline void pm_set_delay_cycle(unsigned char xtal_delay, unsigned char r_delay)
{
    //(n):  if timer wake up : (n*2) 32k cycle; else pad wake up: (n*2-1) ~ (n*2)32k cycle
    analog_write_reg8(areg_aon_0x3d, xtal_delay);
    analog_write_reg8(areg_aon_0x3e, r_delay);
}

/**
 * @brief       This function is used to set reboot reason.
 * @return      none.
 */
static _always_inline void pm_set_reboot_reason(pm_sw_reboot_reason_e reboot_reason)
{
    analog_write_reg8(PM_ANA_REG_POWER_ON_CLR_BUF0, REBOOT_FLAG | (reboot_reason<<1));
}

/**
 * @brief       This function is used to put the chip into low power mode.
 * @return      none.
 */
static _always_inline void pm_trigger_sleep(void)
{
    //0x80 is to enter low power mode immediately. 0x81 is to wait for D25F to enter wfi mode before entering low power,this way is more secure.
    //Once in the WFI mode, memory transactions that are started before the execution of WFI are guaranteed to have been completed,
    //all transient states of memory handling are flushed and no new memory accesses will take place.
    //only suspend requires this process, after waking up to resume the scene.
    //(add by bingyu.li, confirmed by jianzhi.chen 20230810)
    write_reg8(0x14082f, 0x81); //stall mcu trig
    __asm__ __volatile__("wfi");
}

/**
 * @brief       This function is used to power up 24m rc.
 *              [DRIV-1966]The power consumption of 24m rc is 400uA in DCDC mode.
 * @return      none.
 */
static _always_inline void pm_24mrc_power_up(void)
{
    if(!g_24m_rc_is_used)
    {
        analog_write_reg8(areg_aon_0x05, analog_read_reg8(areg_aon_0x05) & ~(FLD_24M_RC_PD));//power on 24M RC

        /*
         * the calibration of 24m RC should wait for 1us if just power it up.
         * (added by jilong.liu, confirmed by yangya at 20240805)
        */
        core_cclk_delay_tick((unsigned long long)(2 * sys_clk.cclk));
    }
}

/**
 * @brief       This function is used to power down 24m rc.
 *              [DRIV-1966]The power consumption of 24m rc is 400uA in DCDC mode.
 * @return      none.
 * @note        In the following case, please make sure the 24m rc can not be power down.
 *              1. Doing clock switch
 *              2. XTAL start up
 *              3. Doing digital module power switch
 *              4. Enter sleep.
 */
static _always_inline void pm_24mrc_power_down_if_unused(void)
{
    if(!g_24m_rc_is_used)
    {
        analog_write_reg8(areg_aon_0x05, analog_read_reg8(areg_aon_0x05) | FLD_24M_RC_PD);//power down 24M RC
    }
}

/**
 * @brief       this function servers to power up BBPLL
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void pm_bbpll_power_up(void);

/**
 * @brief       this function servers to wait BBPLL clock lock.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void pm_wait_bbpll_done(void);

/**
 * @brief       This function is used to determine the stability of the crystal oscillator.
 *              To judge the stability of the crystal oscillator, xo_ready_ana is invalid, and use an alternative solution to judge.
 *              Alternative principle: Because the clock source of the stimer is the crystal oscillator,
 *              if the crystal oscillator does not vibrate, the tick value of the stimer does not increase or increases very slowly (when there is interference).
 *              So first use 24M RC to run the program and wait for a fixed time, calculate the number of ticks that the stimer should increase during this time,
 *              and then read the tick value actually increased by the stimer.
 *              When it reaches 50% of the calculated value, it proves that the crystal oscillator has started.
 *              If it is not reached for a long time, the system will reboot.
 * @param[in]   all_ramcode_en  - Whether all processing in this function is required to be ram code. If this parameter is set to 1, it requires that:
 *              before calling this function, you have done the disable BTB, disable interrupt, mspi_stop_xip and other operations as the corresponding function configured to 0.
 * @attention   This function can only be called with the 24M clock configuration
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void pm_wait_xtal_ready(unsigned char all_ramcode_en);

/**
 * @brief       this function serves to clear all irq status.
 * @return      Indicates whether clearing irq status was successful.
 */
_attribute_ram_code_sec_noinline_ unsigned char pm_clr_all_irq_status(void);

/**
 * @brief       This function serves to recover system timer.
 *              The code is placed in the ram code section, in order to shorten the time.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void pm_stimer_recover(void);

