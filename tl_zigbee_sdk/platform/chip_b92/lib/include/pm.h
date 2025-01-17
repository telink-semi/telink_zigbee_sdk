/********************************************************************************************************
 * @file    pm.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "gpio.h"
#include "clock.h"

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
 *                                          external
 *******************************************************************************************************/


/**
 * @brief these analog register can store data in deep sleep mode or deep sleep with SRAM retention mode.
 *        Reset these analog registers by watchdog, software reboot (sys_reboot()), RESET Pin, power cycle, 32k watchdog, vbus detect.
 */
/**
 * Customers cannot use analog register 0x35 because driver and chip functions are occupied, details are as follows:
 * [Bit0]: If this bit is 1, it means that reboot or power on has occurred. If this bit is 0, it means that sleep has occurred.
 * [Bit1~7]: These bits are used by the driver and cannot be used by the customer.
 */
#define PM_ANA_REG_WD_CLR_BUF0 0x35 // initial value 0xff.

enum
{
    POWERON_FLAG = BIT(0),
};

#define PM_ANA_REG_WD_CLR_BUF1 0x36 // initial value 0x00.
#define PM_ANA_REG_WD_CLR_BUF2 0x37 // initial value 0x00
#define PM_ANA_REG_WD_CLR_BUF3 0x38 // initial value 0x00
#define PM_ANA_REG_WD_CLR_BUF4 0x39 // initial value 0x00

/**
 * @brief analog register below can store information when MCU in deep sleep mode or deep sleep with SRAM retention mode.
 *        Reset these analog registers by power cycle, 32k watchdog, RESET Pin,vbus detect.
 */
/**
 * Customers cannot use analog register 0x3a because driver and chip functions are occupied, details are as follows:
 * [Bit0]: If this bit is 1, it means that reboot has occurred.
 * [Bit1]: If this bit is 1, it means that the software calls the function sys_reboot() when the crystal oscillator does not start up normally.
 * [Bit2]: If this bit is 1, it means that the pm_sleep_wakeup function failed to clear the pm wake flag bit when using the deep wake source, and the software called sys_reboot().
 * [Bit3]: If this bit is 1, it means that the software calls the function sys_reboot() when the PLL does not start up normally.
 * [Bit4~6]: These bits are used by the driver and cannot be used by the customer.
 * [Bit7]: The bootrom is used.
 */
#define PM_ANA_REG_POWER_ON_CLR_BUF0 0x3a // initial value 0x00

typedef enum
{
    REBOOT_FLAG              = BIT(0),
    XTAL_UNSTABLE            = BIT(1),
    PM_CLR_PLIC_REQUEST_FAIL = BIT(2),
    SW_REBOOT_REASON         = BIT_RNG(3, 6),
    BOOTROM_USED             = BIT(7),
} pm_poweron_clr_buf0_e;

//Since there are only 4 bits, this enumeration ranges from 0 to 15.
typedef enum
{
    WAIT_TIMEOUT = 0x00,
} pm_sw_reboot_reason_e;

#define PM_ANA_REG_POWER_ON_CLR_BUF1 0x3b // initial value 0x00
#define PM_ANA_REG_POWER_ON_CLR_BUF2 0x3c // initial value 0xff

/**
 * @brief   gpio wakeup level definition
 */
typedef enum
{
    WAKEUP_LEVEL_LOW  = 0,
    WAKEUP_LEVEL_HIGH = 1,
} pm_gpio_wakeup_level_e;

/**
 * @brief   wakeup tick type definition
 */
typedef enum
{
    PM_TICK_STIMER = 0, // 24M
    PM_TICK_32K    = 1,
} pm_wakeup_tick_type_e;

/**
 * @brief   suspend power whether to power down definition
 */
typedef enum
{
    PM_POWER_BASEBAND = BIT(0), //whether to power on the BASEBAND before suspend.
    PM_POWER_USB      = BIT(1), //whether to power on the USB before suspend.
} pm_suspend_power_cfg_e;

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
 * @brief   sleep mode.
 */
typedef enum
{
    //available mode for customer
    SUSPEND_MODE   = 0x00,
    DEEPSLEEP_MODE = 0x70,                 //when use deep mode pad wakeup(low or high level), if the high(low) level always in the pad,
                                           //system will not enter sleep and go to below of pm API, will reboot by core_6f = 0x20.
                                           //deep retention also had this issue, but not to reboot.
    DEEPSLEEP_MODE_RET_SRAM_LOW32K = 0x61, //for boot from sram
    DEEPSLEEP_MODE_RET_SRAM_LOW64K = 0x43, //for boot from sram
    DEEPSLEEP_MODE_RET_SRAM_LOW96K = 0x07, //for boot from sram
    //not available mode
    DEEPSLEEP_RETENTION_FLAG = 0x0F,
} pm_sleep_mode_e;

/**
 * @brief   available wake-up source for customer
 */
typedef enum
{
    PM_WAKEUP_PAD        = BIT(0),
    PM_WAKEUP_CORE       = BIT(1),
    PM_WAKEUP_TIMER      = BIT(2),
    PM_WAKEUP_COMPARATOR = BIT(3), /**<
                                            There are two things to note when using LPC wake up:
                                            1.After the LPC is configured, you need to wait 100 microseconds before go to sleep because the LPC need 1-2 32k tick to calculate the result.
                                              If you enter the sleep function at this time, you may not be able to sleep normally because the data in the result register is abnormal.

                                            2.When entering sleep, keep the input voltage and reference voltage difference must be greater than 30mV, otherwise can not enter sleep normally, crash occurs.
                                          */
                                   //   PM_WAKEUP_MDEC         = BIT(4),
    PM_WAKEUP_CTB = BIT(5),        //For internal testing only, this function is not available externally
    //   PM_WAKEUP_VAD          = BIT(6),
    //   PM_WAKEUP_SHUTDOWN     = BIT(7),
} pm_sleep_wakeup_src_e;

/**
 * @brief   wakeup status
 */
typedef enum
{
    WAKEUP_STATUS_PAD        = BIT(0),
    WAKEUP_STATUS_CORE       = BIT(1),
    WAKEUP_STATUS_TIMER      = BIT(2),
    WAKEUP_STATUS_COMPARATOR = BIT(3),
    //  WAKEUP_STATUS_MDEC              = BIT(4),
    WAKEUP_STATUS_CTB = BIT(5),           //For internal testing only, this function is not available externally
                                          //  WAKEUP_STATUS_VAD               = BIT(6),
    WAKEUP_STATUS_ALL = 0xff,

    STATUS_GPIO_ERR_NO_ENTER_PM = BIT(8), /**<Bit8 is used to determine whether the wake source is normal.*/
    STATUS_CLEAR_FAIL           = BIT(29),
    STATUS_ENTER_SUSPEND        = BIT(30),
} pm_wakeup_status_e;

/**
 * @brief       zb voltage definition.
 */
typedef enum
{
    ZB_0V8  = 0x00,
    ZB_0V85 = 0x20,
    ZB_0V9  = 0x40,
    ZB_0V95 = 0x60,
    ZB_1V   = 0x80,
    ZB_1V05 = 0xA0,
    ZB_1V1  = 0xC0,
    ZB_1V15 = 0xE0,
} pm_zb_voltage_e;

/**
 * @brief   mcu status
 */
typedef enum
{
    MCU_POWER_ON = BIT(0), /**< power on, vbus detect or reset pin */
    //BIT(1) RSVD
    MCU_SW_REBOOT_BACK           = BIT(2), /**< Clear the watchdog status flag in time, otherwise, the system reboot may be wrongly judged as the watchdog.*/
    MCU_DEEPRET_BACK             = BIT(3),
    MCU_DEEP_BACK                = BIT(4),
    MCU_HW_REBOOT_TIMER_WATCHDOG = BIT(5), /**< If determine whether is 32k watchdog/timer watchdog,
                                               can also use the interface wd_32k_get_status()/wd_get_status() to determine. */
    MCU_HW_REBOOT_32K_WATCHDOG   = BIT(6), /**< - When the 32k watchdog/timer watchdog status is set to 1, if it is not cleared:
                                               - power cyele/vbus detect/reset pin come back, the status is lost;
                                               - but software reboot(sys_reboot())/deep/deepretation/32k watchdog come back,the status remains;
                                               */

    MCU_STATUS_POWER_ON     = MCU_POWER_ON,
    MCU_STATUS_REBOOT_BACK  = MCU_SW_REBOOT_BACK,
    MCU_STATUS_DEEPRET_BACK = MCU_DEEPRET_BACK,
    MCU_STATUS_DEEP_BACK    = MCU_DEEP_BACK,
} pm_mcu_status;

/**
 * @brief   early wakeup time
 */
typedef struct
{
    unsigned short suspend_early_wakeup_time_us;  /**< suspend_early_wakeup_time_us = suspend_ret_r_delay_us + xtal_stable_time + early_time*/
    unsigned short deep_ret_early_wakeup_time_us; /**< deep_ret_early_wakeup_time_us = suspend_ret_r_delay_us + early_time*/
    unsigned short deep_early_wakeup_time_us;     /**< deep_early_wakeup_time_us = deep_r_delay_us + early_time*/
    unsigned short sleep_min_time_us;             /**< sleep_min_time_us = the maximum value of suspend and deep + code + margin*/
} pm_early_wakeup_time_us_s;

extern volatile pm_early_wakeup_time_us_s g_pm_early_wakeup_time_us;

/**
 * @brief   hardware delay time
 */
typedef struct
{
    unsigned short deep_r_delay_cycle;           /**< hardware delay time ,deep_ret_r_delay_us = deep_r_delay_cycle * 1/16k */
    unsigned short suspend_ret_r_delay_cycle;    /**< hardware delay time ,suspend_ret_r_delay_us = suspend_ret_r_delay_cycle * 1/16k */
    unsigned short deep_xtal_delay_cycle;        /**< hardware delay time ,deep_ret_xtal_delay_us = deep_xtal_delay_cycle * 1/16k */
    unsigned short suspend_ret_xtal_delay_cycle; /**< hardware delay time ,suspend_ret_xtal_delay_us = suspend_ret_xtal_delay_cycle * 1/16k */
} pm_r_delay_cycle_s;

extern volatile pm_r_delay_cycle_s g_pm_r_delay_cycle;

/**
 * @brief   deep sleep wakeup status
 */
typedef struct
{
    unsigned char is_pad_wakeup;
    unsigned char wakeup_src; //The pad pin occasionally wakes up abnormally in A0. The core wakeup flag will be incorrectly set in A0.
    unsigned char mcu_status;
    unsigned char rsvd;
} pm_status_info_s;

extern _attribute_aligned_(4) pm_status_info_s g_pm_status_info;
extern _attribute_data_retention_sec_ unsigned char g_pm_vbat_v;
extern unsigned char                                g_areg_aon_7f;

/**
 * @brief       This function serves to get deep retention flag.
 * @return      1 deep retention, 0 deep.
 */
static inline unsigned char pm_get_deep_retention_flag(void)
{
    return !(analog_read_reg8(0x7f) & BIT(0));
}

/**
 * @brief       This function serves to get wakeup source.
 * @return      wakeup source.
 */
static _always_inline pm_wakeup_status_e pm_get_wakeup_src(void)
{
    return (pm_wakeup_status_e)analog_read_reg8(0x64);
}

/**
 * @brief       This function serves to clear the wakeup bit.
 * @param[in]   status  - the interrupt status that needs to be cleared.
 * @return      none.
 */
static inline void pm_clr_irq_status(pm_wakeup_status_e status)
{
    analog_write_reg8(0x64, status); /*add by weihua.zhang, confirmed by jianzhi.chen*/
}

/**
 * @brief       This function serves to set the wakeup source.
 * @param[in]   wakeup_src  - wake up source select.
 * @return      none.
 */
static inline void pm_set_wakeup_src(pm_sleep_wakeup_src_e wakeup_src)
{
    analog_write_reg8(0x4b, wakeup_src);
}

/**
 * @brief       This function is used to set reboot reason.
 * @return      none.
 */
static _always_inline void pm_set_reboot_reason(pm_sw_reboot_reason_e reboot_reason)
{
    analog_write_reg8(PM_ANA_REG_POWER_ON_CLR_BUF0, REBOOT_FLAG | (reboot_reason << 3));
}

/**
 * @brief       This function configures a GPIO pin as the wakeup pin.
 * @param[in]   pin - the pins can be set to all GPIO except PB0/PC5 and GPIOG groups.
 * @param[in]   pol - the wakeup polarity of the pad pin(0: low-level wakeup, 1: high-level wakeup).
 * @param[in]   en  - enable or disable the wakeup function for the pan pin(1: enable, 0: disable).
 * @return      none.
 */
void pm_set_gpio_wakeup(gpio_pin_e pin, pm_gpio_wakeup_level_e pol, int en);

/**
 * @brief       This function configures pm wakeup time parameter.
 * @param[in]   param - deep/suspend/deep_retention r_delay time.(default value: suspend/deep_ret=3, deep=11)
 * @return      none.
 */
void pm_set_wakeup_time_param(pm_r_delay_cycle_s param);

/**
 * @brief       This function is used in applications where the crystal oscillator is relatively slow to start.
 *              When the start-up time is very slow, you can call this function to avoid restarting caused
 *              by insufficient crystal oscillator time (it is recommended to leave a certain margin when setting).
 * @param[in]   delay_us - This time setting is related to the parameter nopnum, which is about the execution time of the for loop
 *                          in the ramcode(default value: 200).
 * @param[in]   loopnum - The time for the crystal oscillator to stabilize is approximately: loopnum*40us(default value: 10).
 * @param[in]   nopnum - The number of for loops used to wait for the crystal oscillator to stabilize after suspend wakes up.
 *                       for(i = 0; i < nopnum; i++){ asm("tnop"); }(default value: Flash=250).
 * @return      none.
 */
void pm_set_xtal_stable_timer_param(unsigned int delay_us, unsigned int loopnum, unsigned int nopnum);
/**
 * @brief       this function servers to wait bbpll clock lock.
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_ void pm_wait_bbpll_done(void);

/**
 * @brief       This function serves to recover system timer.
 *              The code is placed in the ram code section, in order to shorten the time.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void pm_stimer_recover(void);

/**
 * @brief       This function serves to set the working mode of MCU based on 32k crystal,e.g. suspend mode, deep sleep mode, deep sleep with SRAM retention mode and shutdown mode.
 * @param[in]   sleep_mode          - sleep mode type select.
 * @param[in]   wakeup_src          - wake up source select.
 * @param[in]   wakeup_tick_type    - tick type select. Use 32K tick count for long-time sleep or 24M tick count for short-time sleep.
 * @param[in]   wakeup_tick         - The tick value at the time of wake-up.
                                      If the wakeup_tick_type is PM_TICK_STIMER, then wakeup_tick is converted to 24M. The range of tick that can be set is approximately:
                                      current tick value + (18352~0xe0000000), and the corresponding sleep time is approximately: 2ms~234.88s.It cannot go to sleep normally when it exceeds this range.
                                      If the wakeup_tick_type is PM_TICK_32K, then wakeup_tick is converted to 32K. The range of tick that can be set is approximately:
                                      64~0xffffffff, and the corresponding sleep time is approximately: 2ms~37hours.It cannot go to sleep normally when it exceeds this range.
 * @return      indicate whether the cpu is wake up successful.
 * @attention   Must ensure that all GPIOs cannot be floating status before going to sleep to prevent power leakage.
 */
_attribute_text_sec_ int pm_sleep_wakeup(pm_sleep_mode_e sleep_mode, pm_sleep_wakeup_src_e wakeup_src, pm_wakeup_tick_type_e wakeup_tick_type, unsigned int wakeup_tick);

/**
 * @brief       This function serves to set baseband/usb/npe power on/off before suspend sleep,If power
 *              on this module,the suspend current will increase;power down this module will save current,
 *              but you need to re-init this module after suspend wakeup.All module is power down default
 *              to save current.
 * @param[in]   value - whether to power on/off the baseband/usb/npe.
 * @param[in]   on_off - select power on or off, 0 - power off; other value - power on.
 * @return      none.
 */
void pm_set_suspend_power_cfg(pm_suspend_power_cfg_e value, unsigned char on_off);

/**
 * @brief       This function servers to set 3V3 LDO output voltage in active mode.
 * @param[in]   voltage - vddo3 setting gear, can be set from 0 to 7.
 * @return      none.
 */
void pm_set_active_vddo3(pm_vddo3_voltage_e voltage);

/**
 * @brief  This function serves to set the zb voltage.
 * @param  zb_voltage - enum variable of zb voltage.
 * @return none.
 */
static inline void pm_set_zb_voltage(pm_zb_voltage_e zb_voltage)
{
    analog_write_reg8(0x00, (analog_read_reg8(0x00) & 0x1F) | zb_voltage);
}

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
_attribute_ram_code_sec_optimize_o2_ void pm_wait_xtal_ready(unsigned char all_ramcode_en);

/**
 * @brief       This function serves to update wakeup status.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void pm_update_status_info(unsigned char clr_en);

/**
 * @brief       this function serves to clear all irq status.
 * @return      Indicates whether clearing irq status was successful.
 */
_attribute_ram_code_sec_optimize_o2_ unsigned char pm_clr_all_irq_status(void);

/********************************************************************************************************
 *                                          internal
 *******************************************************************************************************/
/********************************************************************************************************
 *              This is just for internal debug purpose, users are prohibited from calling.
 *******************************************************************************************************/
/**
 * @brief       When an error occurs, such as the crystal does not vibrate properly, the corresponding recording and reset operations are performed.
 * @param[in]   reboot_reason  - The bit to be configured in the power on buffer.
 * @param[in]   all_ramcode_en  - Whether all processing in this function is required to be ram code.
 * @return      none.
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void pm_sys_reboot_with_reason(pm_sw_reboot_reason_e reboot_reason, unsigned char all_ramcode_en);
