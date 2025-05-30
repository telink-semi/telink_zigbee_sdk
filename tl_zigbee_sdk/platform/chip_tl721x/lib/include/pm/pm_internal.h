/********************************************************************************************************
 * @file    pm_internal.h
 *
 * @brief   This is the header file for TL721X
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
    #define WDT_REBOOT_RESET_ANA7F_WORK_AROUND 1
#endif

/********************************************************************************************************
 *              This is just for internal debug purpose, users are prohibited from calling.
 *******************************************************************************************************/

/*
 * @note    This is for internal stability debugging use only.
 */
#define PM_DEBUG 0
//1 PB4, 2 PB5
#define PM_SUSPEND_WHILE_DEBUG   0
#define PM_SUSPEND_WHILE_DEBUG_2 0
#define PM_MIN_CODE_DEBUG        0
#define PM_START_CODE_DEBUG      0
#define PM_XTAL_READY_DEBUG      0
#define PM_XTAL_ONCE_DEBUG       0
#define PM_XTAL_READY_TIME       0
#define PM_MANUAL_SETTLE_DEBUG   0

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
 * @brief   active mode VDDO3 output trim definition
 * @note    The voltage values of the following gears are all theoretical values, and there may be deviations between the actual and theoretical values.
 */
typedef enum
{
    PM_VDDO3_VOLTAGE_1V4_2V9 = 0x00, /**<LDO output 1.4V in 1.8V mode & 2.9V in 3.3V mode */
    PM_VDDO3_VOLTAGE_1V5_3V0 = 0x01, /**<LDO output 1.5V in 1.8V mode & 3.0V in 3.3V mode */
    PM_VDDO3_VOLTAGE_1V6_3V1 = 0x02, /**<LDO output 1.6V in 1.8V mode & 3.1V in 3.3V mode */
    PM_VDDO3_VOLTAGE_1V7_3V2 = 0x03, /**<LDO output 1.7V in 1.8V mode & 3.2V in 3.3V mode */
    PM_VDDO3_VOLTAGE_1V8_3V3 = 0x04, /**<LDO output 1.8V in 1.8V mode & 3.3V in 3.3V mode */
    PM_VDDO3_VOLTAGE_1V9_3V4 = 0x05, /**<LDO output 1.9V in 1.8V mode & 3.4V in 3.3V mode */
    PM_VDDO3_VOLTAGE_2V0_3V5 = 0x06, /**<LDO output 2.0V in 1.8V mode & 3.5V in 3.3V mode */
    PM_VDDO3_VOLTAGE_2V1_3V6 = 0x07, /**<LDO output 2.1V in 1.8V mode & 3.6V in 3.3V mode */
} pm_vddo3_voltage_e;

/**
 * @brief dcdc trim flash out
 * 
 */
typedef enum
{
    TRIM_VDDF_TO_1P64V = 0,
    TRIM_VDDF_TO_1P66V,
    TRIM_VDDF_TO_1P68V,
    TRIM_VDDF_TO_1P70V,
    TRIM_VDDF_TO_1P72V,
    TRIM_VDDF_TO_1P74V,
    TRIM_VDDF_TO_1P76V,
    TRIM_VDDF_TO_1P78V,
    TRIM_VDDF_TO_1P80V,
    TRIM_VDDF_TO_1P83V,
    TRIM_VDDF_TO_1P86V,
    TRIM_VDDF_TO_1P89V,
    TRIM_VDDF_TO_1P92V,
    TRIM_VDDF_TO_1P95V,
    TRIM_VDDF_TO_1P98V,
    TRIM_VDDF_TO_2P01V,
} pm_trim_vddf_e;

/**
 * @brief trim vddo 1p8 out
 * 
 */
typedef enum
{
    TRIM_VDDO1P8_TO_1P664V = 0,
    TRIM_VDDO1P8_TO_1P703V,
    TRIM_VDDO1P8_TO_1P744V,
    TRIM_VDDO1P8_TO_1P787V,
    TRIM_VDDO1P8_TO_1P832V,
    TRIM_VDDO1P8_TO_1P879V,
    TRIM_VDDO1P8_TO_1P929V,
    TRIM_VDDO1P8_TO_1P981V,
} pm_trim_vddo1p8_e;

/**
 * @brief trim vdd 0p94 out
 * 
 */
typedef enum
{
    TRIM_VDD0P94_TO_0P820V = 0,
    TRIM_VDD0P94_TO_0P835V,
    TRIM_VDD0P94_TO_0P850V,
    TRIM_VDD0P94_TO_0P865V,
    TRIM_VDD0P94_TO_0P880V,
    TRIM_VDD0P94_TO_08950V,
    TRIM_VDD0P94_TO_0P910V,
    TRIM_VDD0P94_TO_0P925V,
    TRIM_VDD0P94_TO_0P940V,
    TRIM_VDD0P94_TO_0P963V,
    TRIM_VDD0P94_TO_0P986V,
    TRIM_VDD0P94_TO_1P009V,
    TRIM_VDD0P94_TO_1P032V,
    TRIM_VDD0P94_TO_1P055V,
    TRIM_VDD0P94_TO_1P078V,
    TRIM_VDD0P94_TO_1P101V,
} pm_trim_vdd0p94_e;

/**
 * @brief trim dig ldo
 * 
 */
typedef enum
{
    DIG_LDO_TRIM_0P550V = 0,
    DIG_LDO_TRIM_0P575V,
    DIG_LDO_TRIM_0P600V,
    DIG_LDO_TRIM_0P625V,
    DIG_LDO_TRIM_0P650V,
    DIG_LDO_TRIM_0P675V,
    DIG_LDO_TRIM_0P700V,
    DIG_LDO_TRIM_0P725V,
    DIG_LDO_TRIM_0P750V,
    DIG_LDO_TRIM_0P775V,
    DIG_LDO_TRIM_0P800V,
    DIG_LDO_TRIM_0P825V,
    DIG_LDO_TRIM_0P850V,
    DIG_LDO_TRIM_0P875V,
    DIG_LDO_TRIM_0P900V,
    DIG_LDO_TRIM_0P925V,
} pm_dig_ldo_trim_e;

/**
 * @brief trim suspend LDO
 *
 */
typedef enum
{
    SPD_LDO_TRIM_0P55V = 0,
    SPD_LDO_TRIM_0P60V, //0.571V
    SPD_LDO_TRIM_0P65V,
    SPD_LDO_TRIM_0P70V,
    SPD_LDO_TRIM_0P75V,
    SPD_LDO_TRIM_0P80V,
    SPD_LDO_TRIM_0P85V,
    SPD_LDO_TRIM_0P90V,
} pm_spd_ldo_trim_e;

/**
 * @brief trim deep retention LDO
 *
 */
typedef enum
{
    RET_LDO_TRIM_0P55V = 0,
    RET_LDO_TRIM_0P60V, //0.589V
    RET_LDO_TRIM_0P65V,
    RET_LDO_TRIM_0P70V,
    RET_LDO_TRIM_0P75V,
    RET_LDO_TRIM_0P80V,
    RET_LDO_TRIM_0P85V,
    RET_LDO_TRIM_0P90V,
} pm_ret_ldo_trim_e;

/**
 * @brief trim deep retention LDO
 *
 */
typedef enum
{
    NTV_LDO_TRIM_1P785V_0P908 = 0x00, //1.817 0.648
    NTV_LDO_TRIM_1P812V_0P934 = 0x08, //1.860 0.662
    NTV_LDO_TRIM_1P825V_0P847 = 0x10, //1.883 0.670
    NTV_LDO_TRIM_1P734V_0P855 = 0x18, //1.895 0.676
} pm_ntv_ldo_trim_e;

/**
 * @brief trim dcore ldo
 * 
 */
typedef enum
{
    DCORE_LDO_TRIM_0P550V = 0,
    DCORE_LDO_TRIM_0P575V,
    DCORE_LDO_TRIM_0P600V,
    DCORE_LDO_TRIM_0P625V,
    DCORE_LDO_TRIM_0P650V,
    DCORE_LDO_TRIM_0P675V,
    DCORE_LDO_TRIM_0P700V,
    DCORE_LDO_TRIM_0P725V,
    DCORE_LDO_TRIM_0P750V,
    DCORE_LDO_TRIM_0P775V,
    DCORE_LDO_TRIM_0P800V,
    DCORE_LDO_TRIM_0P825V,
    DCORE_LDO_TRIM_0P850V,
    DCORE_LDO_TRIM_0P875V,
    DCORE_LDO_TRIM_0P900V,
    DCORE_LDO_TRIM_0P925V,
} pm_dcore_ldo_trim_e;

/**
 * @brief trim sram ldo
 * 
 */
typedef enum
{
    SRAM_LDO_TRIM_0P60V = 0,
    SRAM_LDO_TRIM_0P65V,
    SRAM_LDO_TRIM_0P70V,
    SRAM_LDO_TRIM_0P75V,
    SRAM_LDO_TRIM_0P80V,
    SRAM_LDO_TRIM_0P85V,
    SRAM_LDO_TRIM_0P90V,
    SRAM_LDO_TRIM_0P95V,
} pm_sram_ldo_trim_e;

typedef struct
{
    unsigned char dcdc_0p95v;
    unsigned char ldo_0p95v;
    unsigned char dcdc_1p05v;
    unsigned char ldo_1p05v;
} pm_cal_vdd0p94_t;

extern _attribute_data_retention_sec_ pm_cal_vdd0p94_t g_pm_cal_vdd0p94_info;

extern _attribute_data_retention_sec_ unsigned char  g_pm_cal_vddo1p8_info;
extern _attribute_data_retention_sec_ pm_cal_0p94v_e g_pm_vdd0p94_level;

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
 * @brief       This function serves to trim vddo1p8 out.
 * @param[in]   trim_vddo1p8 - vddo 1.8v out
 * @return      none
 * @note        The trim value is the theoretical voltage, and the actual output voltage is subject to testing.
 */
static _always_inline void pm_set_vddo1p8(pm_trim_vddo1p8_e trim_vddo1p8)
{
    /*
    *                       poweron_dft:    0x44
    *       bit                     note
    * ---------------------------------------------------------------------------
    * <2:0>:vbat_ldo1p8_trim_3v,    calibrate LDO_1p8_HP/LDO_1p8_LP LDO output.
    */
    analog_write_reg8(areg_aon_0x08, (analog_read_reg8(areg_aon_0x08) & 0xf8) | trim_vddo1p8);
}

/**
 * @brief       This function serves to trim dcdc/ldo out.
 * @param[in]   trim_vddf_ldo     - ldo trim vddf out
 * @param[in]   trim_vdd0p94_ldo  - ldo trim 0.94v out
 * @param[in]   trim_vddf_dcdc    - dcdc trim vddf out
 * @param[in]   trim_vdd0p94_dcdc - dcdc trim 0.94v out
 * @return      none
 * @note        The trim value is the theoretical voltage, and the actual output voltage is subject to testing.
 */
static _always_inline void pm_set_vddf_vdd0p94(pm_trim_vddf_e trim_vddf_ldo, pm_trim_vdd0p94_e trim_vdd0p94_ldo, pm_trim_vddf_e trim_vddf_dcdc, pm_trim_vdd0p94_e trim_vdd0p94_dcdc)
{
    /*
    *                       poweron_dft:    0x88
    *       bit                     note
    * ---------------------------------------------------------------------------
    * <3:0>:tr_dcdc_ldo_1p8,        calibrate VDDF 1.8V LDO output.
    * <7:4>:tr_dcdc_ldo_0p94,       calibrate 0.94V LDO output.
    */
    analog_write_reg8(0x09, (trim_vddf_ldo << 4) | trim_vdd0p94_ldo);

    /*
    *                       poweron_dft:    0x88
    *       bit                     note
    * ---------------------------------------------------------------------------
    * <3:0>:dcdc_trim_flash_out,    calibrate VDDF 1.8V DCDC output.
    * <7:4>:dcdc_trim_soc_out,      calibrate 0.94V DCDC output.
    */
    analog_write_reg8(0x0c, (trim_vdd0p94_dcdc << 4) | trim_vddf_dcdc);
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
 * @brief       This function serves to get dig ldo voltage
 * @param[in]   dig_ldo_trim - dig ldo trim voltage
 * @return      none
 */
static _always_inline pm_dig_ldo_trim_e pm_get_dig_ldo_voltage(void)
{
    return (analog_read_reg8(0x0f) >> 4);
}

/**
 * @brief       This function serves to get suspend LDO voltage
 * @return      suspend LDO trim voltage
 */
static _always_inline unsigned char pm_get_spd_ldo_voltage(void)
{
    return analog_read_reg8(0x0e) & 0xf8;
}

/**
 * @brief       This function serves to get deep retention LDO voltage
 * @return      deep retention LDO trim voltage
 */
static _always_inline unsigned char pm_get_ret_ldo_voltage(void)
{
    return analog_read_reg8(0x0f) & 0xf8;

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
 * @brief       This function serves to trim native LDO voltage
 * @param[in]   ntv_ldo_trim - native LDO trim voltage
 * @return      none
 */
static _always_inline void pm_set_ntv_ldo_voltage(pm_ntv_ldo_trim_e ntv_ldo_trim)
{
    analog_write_reg8(0x02, (analog_read_reg8(0x02) & 0xe7) | ntv_ldo_trim);
}

/**
 * @brief       This function serves to trim dcore ldo voltage.
 * @param[in]   dcore_ldo_trim - dcore ldo trim voltage.
 * @return      none.
 */
static _always_inline void pm_set_dcore_ldo_voltage(pm_dcore_ldo_trim_e dcore_ldo_trim)
{
    analog_write_reg8(0x12, (analog_read_reg8(0x12) & 0xf0) | dcore_ldo_trim);
}

/**
 * @brief       This function serves to trim sram ldo voltage
 * @param[in]   sram_ldo_trim - sram ldo trim voltage
 * @return      none
 */
static _always_inline void pm_set_sram_ldo_voltage(pm_sram_ldo_trim_e sram_ldo_trim)
{
    analog_write_reg8(0x0e, (analog_read_reg8(0x0e) & 0x8f) | (sram_ldo_trim << 4));
}

/**
 * @brief       This function servers to set 3V3 LDO output voltage in active mode.
 * @param[in]   voltage - vddo3 setting gear, can be set from 0 to 7.
 * @return      none.
 */
static _always_inline void pm_set_active_vddo3(pm_vddo3_voltage_e voltage)
{
    //There are three LDOs connected together inside the chip, which affect the output voltage of VDDO3.
    //Under different operating modes of the chip, different LDOs will be turned on, as follows: LDO for Active, LCLDO for SUSPEND and AOLDO for DEEP/DEEP_RET.
    //Among them, LCLDO and AOLDO have weak driving capabilities, while LDO has strong driving capabilities.
    //Currently, users of this interface only care about the voltage of VDDO3 in active mode so the interface only provides parameters for modifying LDO.
    analog_write_reg8(0x15, (analog_read_reg8(0x15) & 0xf8) | voltage);
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
 * @brief       this function servers to power up BBPLL
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void pm_bbpll_power_up(void);

/**
 * @brief       this function servers to wait BBPLL clock lock.
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void pm_wait_bbpll_done(void);

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

/**
 * @brief      This function serves to get vdd0p94 and vddo1p8 current value from analog register.
 * @return     none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void pm_update_vdd0p94_level(void);

/**
 * @brief      This function serves to update vdd0p94 and vddo1p8 calibration value.
 * @param[in]  vdd0p94_value - calibration value for vdd0p94(calibration value is 4bytes)
 * @param[in]  vddo1p8_value - calibration value for vddo1p8(calibration value is 1bytes)
 * @return     none
 */
void pm_update_vdd0p94_vddo1p8_cal_value(unsigned char *vdd0p94_value, unsigned char vddo1p8_value);
