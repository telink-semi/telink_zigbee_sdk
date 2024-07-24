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
#include "analog.h"

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
#define PM_SUSPEND_WHILE_DEBUG          0
#define PM_MIN_CODE_DEBUG               0
#define PM_START_CODE_DEBUG             0

/**
 * @brief   active mode VDDO3 output trim definition
 * @note    The voltage values of the following gears are all theoretical values, and there may be deviations between the actual and theoretical values.
 */
typedef enum {
    PM_VDDO3_VOLTAGE_1V4_2V9    = 0x00, /**<LDO output 1.4V in 1.8V mode & 2.9V in 3.3V mode */
    PM_VDDO3_VOLTAGE_1V5_3V0    = 0x01, /**<LDO output 1.5V in 1.8V mode & 3.0V in 3.3V mode */
    PM_VDDO3_VOLTAGE_1V6_3V1    = 0x02, /**<LDO output 1.6V in 1.8V mode & 3.1V in 3.3V mode */
    PM_VDDO3_VOLTAGE_1V7_3V2    = 0x03, /**<LDO output 1.7V in 1.8V mode & 3.2V in 3.3V mode */
    PM_VDDO3_VOLTAGE_1V8_3V3    = 0x04, /**<LDO output 1.8V in 1.8V mode & 3.3V in 3.3V mode */
    PM_VDDO3_VOLTAGE_1V9_3V4    = 0x05, /**<LDO output 1.9V in 1.8V mode & 3.4V in 3.3V mode */
    PM_VDDO3_VOLTAGE_2V0_3V5    = 0x06, /**<LDO output 2.0V in 1.8V mode & 3.5V in 3.3V mode */
    PM_VDDO3_VOLTAGE_2V1_3V6    = 0x07, /**<LDO output 2.1V in 1.8V mode & 3.6V in 3.3V mode */
}pm_vddo3_voltage_e;

/**
 * @brief dcdc trim flash out
 * 
 */
typedef enum{
    TRIM_1P8V_TO_1P64V = 0,
    TRIM_1P8V_TO_1P66V,
    TRIM_1P8V_TO_1P68V,
    TRIM_1P8V_TO_1P70V,
    TRIM_1P8V_TO_1P72V,
    TRIM_1P8V_TO_1P74V,
    TRIM_1P8V_TO_1P76V,
    TRIM_1P8V_TO_1P78V,
    TRIM_1P8V_TO_1P80V,
    TRIM_1P8V_TO_1P83V,
    TRIM_1P8V_TO_1P86V,
    TRIM_1P8V_TO_1P89V,
    TRIM_1P8V_TO_1P92V,
    TRIM_1P8V_TO_1P95V,
    TRIM_1P8V_TO_1P98V,
    TRIM_1P8V_TO_2P01V,
}pm_trim_1p8v_e;

/**
 * @brief dcdc trim soc out
 * 
 */
typedef enum{
    TRIM_0P94V_TO_0P820V = 0,
    TRIM_0P94V_TO_0P835V,
    TRIM_0P94V_TO_0P850V,
    TRIM_0P94V_TO_0P865V,
    TRIM_0P94V_TO_0P880V,
    TRIM_0P94V_TO_08950V,
    TRIM_0P94V_TO_0P910V,
    TRIM_0P94V_TO_0P925V,
    TRIM_0P94V_TO_0P940V,
    TRIM_0P94V_TO_0P963V,
    TRIM_0P94V_TO_0P986V,
    TRIM_0P94V_TO_1P009V,
    TRIM_0P94V_TO_1P032V,
    TRIM_0P94V_TO_1P055V,
    TRIM_0P94V_TO_1P078V,
    TRIM_0P94V_TO_1P101V,
}pm_trim_0p94v_e;

/**
 * @brief trim dig ldo
 * 
 */
typedef enum {
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
 * @brief trim suspend ldo
 * 
 */
typedef enum {
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
typedef enum {
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
typedef enum {
    NTV_LDO_TRIM_1P785V_0P908 = 0x00,//1.817 0.648
    NTV_LDO_TRIM_1P812V_0P934 = 0x08,//1.860 0.662
    NTV_LDO_TRIM_1P825V_0P847 = 0x10,//1.883 0.670
    NTV_LDO_TRIM_1P734V_0P855 = 0x18,//1.895 0.676
} pm_ntv_ldo_trim_e;

/**
 * @brief trim dcore ldo
 * 
 */
typedef enum {
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
typedef enum {
    SRAM_LDO_TRIM_0P60V = 0,
    SRAM_LDO_TRIM_0P65V,
    SRAM_LDO_TRIM_0P70V,
    SRAM_LDO_TRIM_0P75V,
    SRAM_LDO_TRIM_0P80V,
    SRAM_LDO_TRIM_0P85V,
    SRAM_LDO_TRIM_0P90V,
    SRAM_LDO_TRIM_0P95V,
} pm_sram_ldo_trim_e;

/**
 * @brief power config
 *
 */
typedef enum{
    CORE_0P9V_SRAM_0P9V_BB_0P9V = 0x00, /**< dig ldo mode  1.05V-LDO/DCDC 0.9V_CORE 0.9V_SRAM 0.9V BB*/
    CORE_0P8V_SRAM_0P8V_BB_0P8V = 0x01, /**< dig ldo mode  0.94V-LDO/DCDC 0.8V_CORE 0.8V_SRAM 0.8V BB*/
    CORE_0P7V_SRAM_0P8V_BB_0P7V = 0x02, /**< multi ldo mode  0.94V-LDO/DCDC 0.7V_CORE 0.8V_SRAM 0.7V BB*/
}pm_power_cfg_e;


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
 * @brief       This function servers to wait bbpll clock lock.
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
 * @brief       This function serves to trim dcdc/ldo out.
 * @param[in]   trim_1p8vldo    - ldo trim 1.8v out
 * @param[in]   trim_0p94vldo   - ldo trim 0.94v out
 * @param[in]   trim_1p8vdcdc   - dcdc trim 1.8v out
 * @param[in]   trim_0p94vdcdc  - dcdc trim 0.94v out
 * @return      none
 * @note        The trim value is the theoretical voltage, and the actual output voltage is subject to testing.
 */
static _always_inline void pm_set_1p8v_0p94v(pm_trim_1p8v_e trim_1p8vldo, pm_trim_0p94v_e trim_0p94vldo, pm_trim_1p8v_e trim_1p8vdcdc, pm_trim_0p94v_e trim_0p94vdcdc)
{
    /*
    *                       poweron_dft:    0x88
    *       bit                     note
    * ---------------------------------------------------------------------------
    * <3:0>:tr_dcdc_ldo_1p8,        calibrate 1.8V LDO output.
    * <7:4>:tr_dcdc_ldo_0p94,       calibrate 0.94V LDO output.
    */
    analog_write_reg8(0x09, (trim_1p8vldo << 4) | trim_0p94vldo);

    /*
    *                       poweron_dft:    0x88
    *       bit                     note
    * ---------------------------------------------------------------------------
    * <3:0>:dcdc_trim_flash_out,    calibrate 1.8V DCDC output.
    * <7:4>:dcdc_trim_soc_out,      calibrate 0.94V DCDC output.
    */
    analog_write_reg8(0x0c, (trim_0p94vdcdc << 4) | trim_1p8vdcdc);
}

/**
 * @brief       This function serves to trim dig ldo voltage
 * @param[in]   dig_ldo_trim - dig ldo trim voltage
 * @return      none
 */
static _always_inline void pm_set_dig_ldo_voltage(pm_dig_ldo_trim_e dig_ldo_trim)
{
    analog_write_reg8(0x0f, (analog_read_reg8(0x0f) & 0x0f) | (dig_ldo_trim << 4));
}

/**
 * @brief       This function serves to trim spd ldo voltage
 * @param[in]   spd_ldo_trim - spd ldo trim voltage
 * @return      none
 */
static inline void pm_set_spd_ldo_voltage(pm_spd_ldo_trim_e spd_ldo_trim)
{
    analog_write_reg8(0x0e, (analog_read_reg8(0x0e) & 0xf8) | spd_ldo_trim);
}

/**
 * @brief       This function serves to trim deep retention LDO voltage
 * @param[in]   ret_ldo_trim - deep retention LDO trim voltage
 * @return      none
 */
static inline void pm_set_ret_ldo_voltage(pm_ret_ldo_trim_e ret_ldo_trim)
{
    analog_write_reg8(0x0f, (analog_read_reg8(0x0f) & 0xf8) | ret_ldo_trim);
}

/**
 * @brief       This function serves to trim native LDO voltage
 * @param[in]   ntv_ldo_trim - native LDO trim voltage
 * @return      none
 */
static inline void pm_set_ntv_ldo_voltage(pm_ntv_ldo_trim_e ntv_ldo_trim)
{
    analog_write_reg8(0x02, (analog_read_reg8(0x02) & 0xe7) | ntv_ldo_trim);
}

/**
 * @brief       This function serves to trim dcore ldo voltage.
 * @param[in]   dcore_ldo_trim - dcore ldo trim voltage.
 * @return      none.
 */
static inline void pm_set_dcore_ldo_voltage(pm_dcore_ldo_trim_e dcore_ldo_trim)
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
static inline void pm_set_active_vddo3(pm_vddo3_voltage_e voltage)
{
    //There are three LDOs connected together inside the chip, which affect the output voltage of VDDO3.
    //Under different operating modes of the chip, different LDOs will be turned on, as follows: LDO for Active, LCLDO for SUSPEND and AOLDO for DEEP/DEEP_RET.
    //Among them, LCLDO and AOLDO have weak driving capabilities, while LDO has strong driving capabilities.
    //Currently, users of this interface only care about the voltage of VDDO3 in active mode so the interface only provides parameters for modifying LDO.
    analog_write_reg8(0x15, (analog_read_reg8(0x15) & 0xf8) | voltage);
}

/**
 * @brief       The function of this interface is to configure the voltage of the sleep-related LDO.
 * @return      none.
 */
static inline void pm_set_sleep_ldo_voltage(void)
{
    analog_write_reg8(areg_aon_0x0b, analog_read_reg8(areg_aon_0x0b) | (FLD_PD_NVT_0P94 | FLD_PD_NVT_1P8));
    analog_write_reg8(areg_aon_0x06, analog_read_reg8(areg_aon_0x06) | (FLD_PD_SPD_LDO|FLD_PD_DIG_RET_LDO));
//  pm_set_spd_ldo_voltage(SPD_LDO_TRIM_0P55V);
//  pm_set_ret_ldo_voltage(RET_LDO_TRIM_0P55V);
//  pm_set_ntv_ldo_voltage(NTV_LDO_TRIM_1P734V_0P855);
}

/**
 * @brief       This function serves to configure power supply.
 * @param[in]   power_cfg - power config
 * @return      none
 * @note        There are three modes can be configure, different configurations correspond to different system power supply modes.
 *              The configured power supply modules are CORE voltage, BASEBAND voltage and SRAM voltage. The configuration requirements are:
 *              1. The working voltage of Baseband should be maintained at the same voltage as the CORE
 *              2. The working voltage of SRAM should not be lower than the CORE voltage and not lower than 0.8V
 */
_attribute_ram_code_sec_noinline_ void pm_power_supply_config(pm_power_cfg_e power_cfg);

