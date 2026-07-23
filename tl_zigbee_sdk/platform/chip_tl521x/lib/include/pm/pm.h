/********************************************************************************************************
 * @file    pm.h
 *
 * @brief   This is the header file for tl521x
 *
 * @author  Driver Group
 * @date    2026
 *
 * @par     Copyright (c) 2026, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "lib/include/clock.h"

#define PM_POWER_OPTIMIZATION  1

/**
 * @brief these analog register can store data in deep sleep mode or deep sleep with SRAM retention mode.
 *        Reset these analog registers by watchdog, software reboot (sys_reboot()), RESET Pin, power cycle, 32k watchdog, vbus detect.
 */
#define PM_ANA_REG_WD_CLR_BUF1 0x36 // initial value 0x00.
#define PM_ANA_REG_WD_CLR_BUF2 0x37 // initial value 0x00.
#define PM_ANA_REG_WD_CLR_BUF3 0x38 // initial value 0x00.
#define PM_ANA_REG_WD_CLR_BUF4 0x39 // initial value 0x00.

/**
 * @brief analog register below can store information when MCU in deep sleep mode or deep sleep with SRAM retention mode.
 *        Reset these analog registers by power cycle, 32k watchdog, RESET Pin,vbus detect.
 */
#define PM_ANA_REG_POWER_ON_CLR_BUF1 0x3b // initial value 0x00.
#define PM_ANA_REG_POWER_ON_CLR_BUF2 0x3c // initial value 0xff.

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
 * @brief   sleep mode.
 */
typedef enum
{
    //available mode for customer
    SUSPEND_MODE                    = 0x00,
    DEEPSLEEP_MODE                  = 0xc0, //when use deep mode pad wakeup(low or high level), if the high(low) level always in the pad,
                                            //system will not enter sleep and go to below of pm API, will reboot by core_6f = 0x20.
                                            //deep retention also had this issue, but not to reboot.
    DEEPSLEEP_MODE_RET_SRAM_LOW32K  = 0x01, //for boot from sram
    DEEPSLEEP_MODE_RET_SRAM_LOW64K  = 0x03, //for boot from sram
    DEEPSLEEP_MODE_RET_SRAM_LOW128K = 0x07, //for boot from sram
    DEEPSLEEP_MODE_RET_SRAM_LOW192K = 0x0f, //for boot from sram
    DEEPSLEEP_MODE_RET_SRAM_LOW256K = 0x1f, //for boot from sram
    SHUTDOWN_MODE                   = 0xe0,

    //not available mode
    DEEPSLEEP_RETENTION_FLAG        = 0x1f,
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
    PM_WAKEUP_SHUTDOWN   = BIT(7),
} pm_sleep_wakeup_src_e;

/**
 * @brief   wakeup status
 */
typedef enum
{
    WAKEUP_STATUS_PAD        = FLD_WAKEUP_STATUS_PAD,
    WAKEUP_STATUS_CORE       = FLD_WAKEUP_STATUS_CORE,
    WAKEUP_STATUS_TIMER      = FLD_WAKEUP_STATUS_TIMER,
    WAKEUP_STATUS_COMPARATOR = FLD_WAKEUP_STATUS_COMPARATOR,
    WAKEUP_STATUS_ALL        = FLD_WAKEUP_STATUS_ALL,
    WAKEUP_STATUS_INUSE_ALL  = FLD_WAKEUP_STATUS_INUSE_ALL,

    STATUS_GPIO_ERR_NO_ENTER_PM = BIT(8), /**<Bit8 is used to determine whether the wake source is normal.*/
    STATUS_EXCEED_MAX           = BIT(27),
    STATUS_EXCEED_MIN           = BIT(28),
    STATUS_CLEAR_FAIL           = BIT(29),
    STATUS_ENTER_SUSPEND        = BIT(30),
} pm_suspend_wakeup_status_e;

/**
 * @brief   mcu status
 */
typedef enum
{
    MCU_POWER_ON                 = BIT(0), /**< power on, vbus detect or reset pin */
    //BIT(1) RSVD
    MCU_SW_REBOOT_BACK           = BIT(2), /**< Clear the watchdog status flag in time, otherwise, the system reboot may be wrongly judged as the watchdog.*/
    MCU_DEEPRET_BACK             = BIT(3),
    MCU_DEEP_BACK                = BIT(4),
    MCU_HW_REBOOT_TIMER_WATCHDOG = BIT(5), /**< If determine whether is 32k watchdog/timer watchdog,can also use the interface wd_32k_get_status()/wd_get_status() to determine. */
    MCU_HW_REBOOT_32K_WATCHDOG   = BIT(6), /**< - When the 32k watchdog/timer watchdog status is set to 1, if it is not cleared:
                                              - power cyele/vbus detect/reset pin come back, the status is lost;
                                              - but software reboot(sys_reboot())/deep/deepretation/32k watchdog come back,the status remains;
                                              */

    MCU_STATUS_POWER_ON          = MCU_POWER_ON,
    MCU_STATUS_REBOOT_BACK       = MCU_SW_REBOOT_BACK,
    MCU_STATUS_DEEPRET_BACK      = MCU_DEEPRET_BACK,
    MCU_STATUS_DEEP_BACK         = MCU_DEEP_BACK,
} pm_mcu_status;

/**
 * @brief power sel
 * 
 */
typedef enum
{
    PM_POWER_UP   = 0,
    PM_POWER_DOWN = 1,
} pm_power_sel_e;

/**
 * @brief dig voltage mode
 * 
 */
typedef enum
{
    DIG_VOL_1V_MODE  = 0,
    DIG_VOL_1V1_MODE,
} pm_dig_vol_mode_e;

/**
 * @brief   early wake up time
 */
typedef struct
{
    unsigned short suspend_early_wakeup_time_us;  /**< suspend_early_wakeup_time_us = deep_ret_r_delay_us + xtal_stable_time + early_time*/
    unsigned short deep_ret_early_wakeup_time_us; /**< deep_ret_early_wakeup_time_us = deep_ret_r_delay_us + early_time*/
    unsigned short deep_early_wakeup_time_us;     /**< deep_early_wakeup_time_us = suspend_ret_r_delay_us*/
    unsigned short sleep_min_time_us;             /**< sleep_min_time_us = suspend_early_wakeup_time_us + 200*/
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

extern unsigned char                                g_areg_aon_7f;
extern unsigned char                                g_areg_aon_35;
extern unsigned char                                g_areg_aon_3a;
extern _attribute_data_retention_sec_ unsigned char g_pm_vbat_v;
extern _attribute_data_retention_sec_ unsigned char g_pm_active_power_cfg;
extern _attribute_data_retention_sec_ unsigned char g_areg_aon_0a;
#if (PM_POWER_OPTIMIZATION)
extern _attribute_data_retention_sec_ volatile unsigned char g_areg_aon_06;
extern _attribute_data_retention_sec_ volatile unsigned char g_areg_aon_05;
extern _attribute_data_retention_sec_ volatile unsigned char g_areg_0x100;
extern _attribute_data_retention_sec_ volatile unsigned char g_areg_0x101;
extern _attribute_data_retention_sec_ volatile unsigned char g_areg_0x102;
#endif

/**
 * @brief       This function serves to get wakeup source.
 * @return      wakeup source.
 * @note        After the wake source is obtained, &WAKEUP_STATUS_INUSE_ALL is needed to determine
 *              whether the wake source in use has been cleared, because some of the wake sources
 *              that are not in use may have been set up.
 */
static _always_inline pm_wakeup_status_e pm_get_wakeup_src(void)
{
    return ((pm_wakeup_status_e)analog_read_reg8(areg_aon_0x64));
}

/**
 * @brief       This function serves to clear the wakeup bit.
 * @param[in]   status  - the interrupt status that needs to be cleared.
 * @return      none.
 * @note        To clear all wake sources, the parameter of this interface is usually FLD_WAKEUP_STATUS_ALL
 *              instead of FLD_WAKEUP_STATUS_INUSE_ALL.
 */
static _always_inline void pm_clr_irq_status(pm_wakeup_status_e status)
{
    analog_write_reg8(areg_aon_0x64, status);
}

/**
 * @brief       This function serves to set the wakeup source.
 * @param[in]   wakeup_src  - wake up source select.
 * @return      none.
 */
static _always_inline void pm_set_wakeup_src(pm_sleep_wakeup_src_e wakeup_src)
{
    analog_write_reg8(areg_aon_0x4b, wakeup_src);
}

/**
 * @brief       This function serves to enable core wakeup.
 * @return      none.
 */
static inline void pm_set_core_wakeup_mask(soc_core_wakeup_mask_e core_wakeup_mask)
{
    reg_wakeup_en |= core_wakeup_mask;
}

/**
 * @brief       This function serves to disable core wakeup.
 * @return      none.
 */
static inline void pm_clr_core_wakeup_mask(soc_core_wakeup_mask_e core_wakeup_mask)
{
    reg_wakeup_en &= ~core_wakeup_mask;
}

/**
 * @brief       This function serves to set vbat type.
 * @param[in]   vbat_v  - This parameter is used to determine whether the VBAT voltage can be greater than 3.6V.
 *                      - Please refer to vbat_type_e for specific usage precautions.
 * @return      none
 * @note        When the VBAT voltage is greater than 3.6V, use VBAT_MAX_VALUE_GREATER_THAN_3V6.
 *              When the VBAT voltage is less than 2.2V, use VBAT_MAX_VALUE_LESS_THAN_3V6.
 */
static _always_inline void pm_set_vbat_type(vbat_type_e vbat_v)
{
    /*
     *      pd_bit                      note
     * ---------------------------------------------------------------------------
     * <3>:pd_vbat_sw  default:1,->vbat_v Power down of bypass switch(VBAT LDO).
     */
    g_areg_aon_06 = (g_areg_aon_06 | FLD_PD_VBAT_SW) & ~(vbat_v);
    analog_write_reg8(areg_aon_0x06, g_areg_aon_06);
}

/**
 * @brief       This function configures a GPIO pin as the wakeup pin.
 * @param[in]   pin - the pins can be set to all GPIO except GPIOF groups.
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
 * @param[in]   delay_us - The time wait for xtal stable and flash restore to the active working state in the ramcode
 *                          when wakeup from suspend sleep (default value: 200).
 * @param[in]   loopnum - The time for the crystal oscillator to stabilize is approximately: loopnum*40us(default value: loopnum=10).
 * @return      none.
 * @note        Those parameters will be lost after reboot or deep sleep, so it required to be reconfigured.
 */
void pm_set_xtal_stable_timer_param(unsigned int delay_us, unsigned int loopnum);

/**
 * @brief       This function is used to configure whether to turn on the power switches of
 *              the baseband/USB/AUDIO during initialization. The default configuration is to enable all of them.
 *              If these modules will not be used throughout the entire operation process,
 *              then this interface needs to be configured to be disabled, which will save power consumption.
 * @param[in]   value - whether to power on/off the baseband/USB/AUDIO.
 * @param[in]   on_off - select power on or off.
 * @return      none.
 */
void pm_set_active_power_cfg(pm_pd_module_e value, pm_power_sel_e on_off);

/**
 * @brief       This function serves to set baseband/usb/npe power on/off before suspend sleep,If power
 *              on this module,the suspend current will increase;power down this module will save current,
 *              but you need to re-init this module after suspend wakeup.All module is power down default
 *              to save current.
 * @param[in]   value - whether to power on/off the baseband/usb/npe.
 * @param[in]   on_off - select power on or off, 0 - power off; other value - power on.
 * @return      none.
 */
void pm_set_suspend_power_cfg(pm_pd_module_e value, unsigned char on_off);

/**
 * @brief       This function serves to set the working mode of MCU based on 32k crystal,
 *              e.g. suspend mode, deep sleep mode, deep sleep with SRAM retention mode and shutdown mode.
 * @param[in]   sleep_mode          - sleep mode type select.
 * @param[in]   wakeup_src          - wake up source select.
 * @param[in]   wakeup_tick_type    - tick type select. Use 32K tick count for long-time sleep or 24M tick count for short-time sleep.
 * @param[in]   wakeup_tick         - The tick value at the time of wake-up.
                                      If wakeup_tick_type is pm_tick_timer and
                                      if system timer is 24M, the scale range that can be set is about:
                                      The current tick value + (48000 ~ 0xe0000000) ranges from 2ms ~ 156.59 seconds.
                                      If the system timer is 16M, the scale range that can be set is about:
                                      The current tick value + (32000 ~ 0xe0000000) ranges from 2ms ~ 234.88 seconds.
                                      If the wakeup_tick_type is PM_TICK_32K, then wakeup_tick is converted to 32K.
                                      The range of tick that can be set is approximately: 64~0xffffffff,
                                      and the corresponding sleep time is approximately: 2ms~37hours.
                                      When it exceeds this range, it cannot sleep properly.
 * @note        There are two things to note when using LPC wake up:
 *              1.After the LPC is configured, you need to wait 100 seconds before you can go to sleep.
 *                After the LPC is opened, 1-2 32k tick is needed to calculate the result.
 *                Before this, the data in the result register is random. If you enter the sleep function at this time,
 *                you may not be able to sleep normally because the data in the result register is abnormal.
 *              2.When entering sleep, keep the input voltage and reference voltage difference must be greater than 30mV,
 *                otherwise can not enter sleep normally, crash occurs.
 * @return      indicate whether the cpu is wake up successful.
 * @attention   Must ensure that all GPIOs cannot be floating status before going to sleep to prevent power leakage.
 */
#if PM_POWER_OPTIMIZATION
_attribute_ram_code_sec_optimize_o2_noinline_ int pm_sleep_wakeup(pm_sleep_mode_e sleep_mode, pm_sleep_wakeup_src_e wakeup_src, pm_wakeup_tick_type_e wakeup_tick_type, unsigned int wakeup_tick);
#else
_attribute_text_sec_ int pm_sleep_wakeup(pm_sleep_mode_e sleep_mode, pm_sleep_wakeup_src_e wakeup_src, pm_wakeup_tick_type_e wakeup_tick_type, unsigned int wakeup_tick);
#endif

/**
 * @brief       This function is used to obtain the cause of software reboot.
 * @return      reboot enum element of pm_poweron_clr_buf0_e.
 * @note        -# the interface pm_update_status_info() must be called before this interface can be invoked;
 */
pm_sw_reboot_reason_e pm_get_sw_reboot_event(void);

/**
 * @brief       This function serves to update wakeup status.
 * @param[in]   clr_en  - Whether to set the value of the status register to a fixed value.
 *                        If the interface is called twice, the first time it is not modified, clr_en=0;
 *                        if the interface is called once, it is modified, clr_en=1.
 * @return      none.
 * @note        After calling this interface, it is necessary to clear the flag of the timer watchdog or the 32k watchdog.
 *              Otherwise, if the flag remains set, it may affect the next judgment.
 *              After calling this interface, other states are set to fixed values.
 *              Therefore, this interface cannot be called twice,
 *              and if it is called twice, the state will be fixed to one state, not the correct state.
 */
_attribute_ram_code_sec_noinline_ void pm_update_status_info(unsigned char clr_en);

/**
 * @brief       This function serves to set dig ldo mode.
 * @param[in]   vol             - dig ldo mode.
 * @param[in]   dma_timeout_us - wait dma all chn complete timeout.
 * @return      DRV_API_SUCCESS - successful;
 *              DRV_API_INVALID_PARAM - select the not support mode;
 *              DRV_API_TIMEOUT - wait for dma all chn idle timeout to exit;
 * @note        1.If the voltage goes up, after calling the interface first, then adjust the frequency;
 *                If the voltage goes down, adjust the frequency first, then call the interface;
 *              2.When adjusting this voltage, no access ram operation is allowed, so it will wait for dma idle in this interface,
 *                modifying dma_timeout_us won't work if there are dma chains working all the time, and needs to be turned off by the upper layers themselves depending on the situation.
 *              3.When adjusting this voltage, no access ram operation is allowed, disable swire.
 *              4.If the check configuration fails, reboot.
 */
_attribute_ram_code_sec_noinline_ drv_api_status_e pm_set_dvdd(pm_dig_vol_mode_e vol, unsigned int dma_timeout_us);

/**
 * @brief      This function servers to get calibration value from EFUSE for retention LDO voltage.
 * @param[in]  none
 * @return     DRV_API_SUCCESS - the calibration value update, DRV_API_INVALID_PARAM - the calibration value is not valid,
 *             DRV_API_TIMEOUT - efuse read timeout.
 */
drv_api_status_e pm_efuse_calib_ret_ldo_voltage(void);

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
