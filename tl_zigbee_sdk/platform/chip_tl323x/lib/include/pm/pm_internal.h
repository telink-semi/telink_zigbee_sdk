/********************************************************************************************************
 * @file    pm_internal.h
 *
 * @brief   This is the header file for tl323x
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
    #define WDT_REBOOT_RESET_ANA7F_WORK_AROUND 1
#endif

/********************************************************************************************************
 *              This is just for internal debug purpose, users are prohibited from calling.
 *******************************************************************************************************/

/*
 * @note    This is for internal stability debugging use only.
 */
#define PM_DEBUG                 0
//1 PB4, 2 PB5
#define PM_SUSPEND_WHILE_DEBUG   0
#define PM_SUSPEND_WHILE_DEBUG_2 0
#define PM_MIN_CODE_DEBUG        0
#define PM_START_CODE_DEBUG      0
#define PM_XTAL_READY_DEBUG      0
#define PM_XTAL_ONCE_DEBUG       0
#define PM_XTAL_READY_TIME       0
#define PM_MANUAL_SETTLE_DEBUG   0
#define PM_PLL_DONE_DEBUG        0

//system timer clock source is constant 24M, never change
//NOTICE:We think that the external 32k crystal clk is very accurate, does not need to read through TIMER_32K_LAT_CAL.
//register, the conversion error(use 32k:64 cycle, count 24M sys tmr's ticks), at least the introduction of 64ppm.
#define STIMER_CLOCK_16M 1
#define STIMER_CLOCK_24M 2
#define STIMER_CLOCK     STIMER_CLOCK_24M

#if (STIMER_CLOCK == STIMER_CLOCK_16M)
    #define CRYSTAL32768_TICK_PER_32CYCLE 15625
#elif (STIMER_CLOCK == STIMER_CLOCK_24M)
    #define CRYSTAL32768_TICK_PER_64CYCLE 46875
#endif


extern _attribute_data_retention_sec_ unsigned int  g_pm_tick_32k_calib;
extern _attribute_data_retention_sec_ unsigned int  g_pm_tick_cur;
extern _attribute_data_retention_sec_ unsigned int  g_pm_tick_32k_cur;
extern _attribute_data_retention_sec_ unsigned char g_pm_long_suspend;

/**
 * @brief for voltage calibration
 */
typedef struct
{
    unsigned char ldo_lp25;      /* for ldo */
    unsigned char dcdc_lp25;     /* for dcdc */
    unsigned char dig_ldo;
    unsigned char flash;

    unsigned char is_cal;
}pm_voltage_cal_t;
extern _attribute_data_retention_sec_ pm_voltage_cal_t g_pm_voltage_cal;

typedef struct
{
    unsigned char vdd1v8;

    unsigned char is_cal;
}pm_voltage_vdd1v8_cal_t;
extern _attribute_data_retention_sec_ pm_voltage_vdd1v8_cal_t g_pm_voltage_vdd1v8_cal;

/**
 * @brief peak_btr_trim<2:0>
 * 
 */
typedef enum                    /*     i_peak/mA       */
{                               /* L=6.8uH  |  L=3.3uH */
    PEAK_BTR_TRIM_001 = 1,      /* 399.60   |  765.42  */
    PEAK_BTR_TRIM_002,          /* 219.70   |  427.52  */
    PEAK_BTR_TRIM_003,          /* 155.10   |  300.19  */
    PEAK_BTR_TRIM_004,          /* 124.00   |  239.99  */ /**< peak_btr_trim (default) */
    PEAK_BTR_TRIM_005,          /* 103.20   |  198.29  */
    PEAK_BTR_TRIM_006,          /* 89.15    |  170.71  */
    PEAK_BTR_TRIM_007,          /* 79.12    |  149.33  */
} pm_peak_btr_trim_e;

/**
 * @brief trim dig ldo
 * 
 */
typedef enum
{
    DIG_LDO_TRIM_0P750V = 0,
    DIG_LDO_TRIM_0P775V,
    DIG_LDO_TRIM_0P800V,
    DIG_LDO_TRIM_0P825V,
    DIG_LDO_TRIM_0P850V,
    DIG_LDO_TRIM_0P875V,
    DIG_LDO_TRIM_0P900V,
    DIG_LDO_TRIM_0P925V,
    DIG_LDO_TRIM_0P950V,
    DIG_LDO_TRIM_0P975V,
    DIG_LDO_TRIM_0P1000V, /**< DIG LDO output 1.00V (default) */
    DIG_LDO_TRIM_0P1025V,
    DIG_LDO_TRIM_0P1050V,
    DIG_LDO_TRIM_0P1075V,
    DIG_LDO_TRIM_0P1100V,
    DIG_LDO_TRIM_0P1125V,
} pm_dig_ldo_trim_e;

/**
 * @brief trim suspend LDO
 *
 */
typedef enum
{
    SPD_LDO_TRIM_0P80V = 0x00, /**< suspend LDO output 0.80V */
    SPD_LDO_TRIM_0P85V = 0x01, /**< suspend LDO output 0.85V */
    SPD_LDO_TRIM_0P90V = 0x02, /**< suspend LDO output 0.90V */
    SPD_LDO_TRIM_0P95V = 0x03, /**< suspend LDO output 0.95V */
    SPD_LDO_TRIM_1P00V = 0x04, /**< suspend LDO output 1.00V (default) */
    SPD_LDO_TRIM_1P05V = 0x05, /**< suspend LDO output 1.05V */
    SPD_LDO_TRIM_1P10V = 0x06, /**< suspend LDO output 1.10V */
    SPD_LDO_TRIM_1P15V = 0x07, /**< suspend LDO output 1.15V */
} pm_spd_ldo_trim_e;

/**
 * @brief trim deep retention LDO
 *
 */
typedef enum
{
    RET_LDO_TRIM_0P60V = 0x00, /**< retention LDO output 0.60V */
    RET_LDO_TRIM_0P65V = 0x01, /**< retention LDO output 0.65V */
    RET_LDO_TRIM_0P70V = 0x02, /**< retention LDO output 0.70V */
    RET_LDO_TRIM_0P75V = 0x03, /**< retention LDO output 0.75V */
    RET_LDO_TRIM_0P80V = 0x04, /**< retention LDO output 0.80V (default) */
    RET_LDO_TRIM_0P85V = 0x05, /**< retention LDO output 0.85V */
    RET_LDO_TRIM_0P90V = 0x06, /**< retention LDO output 0.90V */
    RET_LDO_TRIM_0P95V = 0x07, /**< retention LDO output 0.95V */
} pm_ret_ldo_trim_e;

/**
 * @brief       This function serves to reboot system.
 * @return      none
 */
_always_inline void sys_reset_all(void)
{
#if (PM_DEBUG)
    while (1)
        ;
#endif
    reg_pwdn_en = 0x20;
}

/**
 * @brief      This function servers to get calibration value from EFUSE.
 * @param[in]  none
 * @return     DRV_API_SUCCESS - the calibration value update, DRV_API_FAILURE - the calibration value is not update.
 */
drv_api_status_e pm_efuse_calib_dig_ldo_voltage(void);

/**
 * @brief       This function serves to get dig ldo voltage
 * @param[in]   dig_ldo_trim - dig ldo trim voltage
 * @return      none
 */
static _always_inline pm_dig_ldo_trim_e pm_get_dig_ldo_voltage(void)
{
    return (analog_read_reg8(0x27) & 0x0f);
}

/**
 * @brief       This function serves to get suspend LDO voltage
 * @return      suspend LDO trim voltage
 */
static _always_inline unsigned char pm_get_spd_ldo_voltage(void)
{
    return analog_read_reg8(0x0e) & 0x07;
}

/**
 * @brief       This function serves to get deep retention LDO voltage
 * @return      deep retention LDO trim voltage
 */
static _always_inline unsigned char pm_get_ret_ldo_voltage(void)
{
    return analog_read_reg8(0x0f) & 0x07;

}

/**
 * @brief       This function serves to change the on time of PMOS switch
 *              and shorten it as the value of control bit increases.
 *              The smaller the inductance value is,the larger the value needs to be set
 * @param[in]   peak_btr_trim - peak_btr_current
 * @return      none
 */
static _always_inline void pm_set_peak_btr_current(pm_peak_btr_trim_e peak_btr_trim)
{
    analog_write_reg8(0x01, (analog_read_reg8(0x01) & 0xf8) | peak_btr_trim);
}

/**
 * @brief       This function serves to trim dig ldo voltage
 * @param[in]   dig_ldo_trim - dig ldo trim voltage
 * @return      none
 */
static _always_inline void pm_set_dig_ldo_voltage(pm_dig_ldo_trim_e dig_ldo_trim)
{
    analog_write_reg8(0x27, (analog_read_reg8(0x27) & 0xf0) | dig_ldo_trim);
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

#if (!PM_POWER_OPTIMIZATION)
/**
 * @brief   This function is used to enable native LDO.
 * @return  none.
 */
static _always_inline void pm_enable_native_ldo(void)
{
    analog_write_reg8(areg_aon_0x0b, (analog_read_reg8(areg_aon_0x0b) & ~(FLD_PD_NVT_1P25 | FLD_PD_NVT_1P8)));
}

/**
 * @brief   This function is used to disable native LDO.
 * @return  none.
 */
static _always_inline void pm_disable_native_ldo(void)
{
    analog_write_reg8(areg_aon_0x0b, (analog_read_reg8(areg_aon_0x0b) | (FLD_PD_NVT_1P25 | FLD_PD_NVT_1P8)));
}
#endif

/**
 * @brief   This function is used to enable suspend LDO.
 * @return  none.
 */
static _always_inline void pm_enable_spd_ldo(void)
{
#if (PM_POWER_OPTIMIZATION)
    g_areg_aon_06 &= ~(FLD_PD_SPD_LDO);
    analog_write_reg8(areg_aon_0x06, g_areg_aon_06);
#else
    analog_write_reg8(areg_aon_0x06, analog_read_reg8(areg_aon_0x06) & ~(FLD_PD_SPD_LDO));
#endif
}

/**
 * @brief   This function is used to disable suspend and retention LDO.
 * @return  none.
 */
static _always_inline void pm_disable_spd_ret_ldo(void)
{
#if (PM_POWER_OPTIMIZATION)
    g_areg_aon_06 |= (FLD_PD_SPD_LDO | FLD_PD_DIG_RET_LDO);
    analog_write_reg8(areg_aon_0x06, g_areg_aon_06);
#else
    analog_write_reg8(areg_aon_0x06, analog_read_reg8(areg_aon_0x06) | FLD_PD_SPD_LDO | FLD_PD_DIG_RET_LDO);
#endif
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
    analog_write_reg8(PM_ANA_REG_POWER_ON_CLR_BUF0, REBOOT_FLAG | (reboot_reason << 1));
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
    if (!g_24m_rc_is_used) {
        analog_write_reg8(areg_aon_0x05, analog_read_reg8(areg_aon_0x05) & ~(FLD_24M_RC_PD)); //power on 24M RC

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
    if (!g_24m_rc_is_used) {
        analog_write_reg8(areg_aon_0x05, analog_read_reg8(areg_aon_0x05) | FLD_24M_RC_PD); //power down 24M RC
    }
}

/**
 * @brief       This function servers to power up BBPLL.
 * @param[in]   all_ramcode_en  - Whether all processing in this function is required to be ram code. If this parameter is set to 1, it requires that:
 *              before calling this function, you have done the disable BTB, disable interrupt, mspi_stop_xip and other operations as the corresponding function configured to 0.
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void pm_bbpll_power_up(unsigned char all_ramcode_en);

/**
 * @brief       This function servers to power down BBPLL.
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void pm_bbpll_power_down(void);

/**
 * @brief       This function servers to wait BBPLL clock lock.
 * @param[in]   all_ramcode_en  - Whether all processing in this function is required to be ram code. If this parameter is set to 1, it requires that:
 *              before calling this function, you have done the disable BTB, disable interrupt, mspi_stop_xip and other operations as the corresponding function configured to 0.
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void pm_wait_bbpll_done(unsigned char all_ramcode_en);

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
_attribute_ram_code_sec_optimize_o2_noinline_ void pm_wait_xtal_ready(unsigned char all_ramcode_en);

/**
 * @brief       this function serves to clear all irq status.
 * @return      Indicates whether clearing irq status was successful.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ unsigned char pm_clr_all_irq_status(void);

/**
 * @brief       This function serves to recover system timer.
 *              The code is placed in the ram code section, in order to shorten the time.
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void pm_stimer_recover(void);
