/********************************************************************************************************
 * @file    sys.h
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
/** @page SYS
 *
 *  Introduction
 *  ===============
 *  Clock init and system timer delay.
 *
 *  API Reference
 *  ===============
 *  Header File: sys.h
 */

#ifndef SYS_H_
#define SYS_H_
#include "reg_include/register.h"
#include "compiler.h"

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/
/**
 * @brief instruction delay
 */

#define _ASM_NOP_       __asm__ __volatile__("nop")

#define CLOCK_DLY_1_CYC _ASM_NOP_
#define CLOCK_DLY_2_CYC \
    _ASM_NOP_;          \
    _ASM_NOP_
#define CLOCK_DLY_3_CYC \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_
#define CLOCK_DLY_4_CYC \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_
#define CLOCK_DLY_5_CYC \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_
#define CLOCK_DLY_6_CYC \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_
#define CLOCK_DLY_7_CYC \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_
#define CLOCK_DLY_8_CYC \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_
#define CLOCK_DLY_9_CYC \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_;          \
    _ASM_NOP_
#define CLOCK_DLY_10_CYC \
    _ASM_NOP_;           \
    _ASM_NOP_;           \
    _ASM_NOP_;           \
    _ASM_NOP_;           \
    _ASM_NOP_;           \
    _ASM_NOP_;           \
    _ASM_NOP_;           \
    _ASM_NOP_;           \
    _ASM_NOP_;           \
    _ASM_NOP_
#define CLOCK_DLY_64_CYC \
    CLOCK_DLY_10_CYC;    \
    CLOCK_DLY_10_CYC;    \
    CLOCK_DLY_10_CYC;    \
    CLOCK_DLY_10_CYC;    \
    CLOCK_DLY_10_CYC;    \
    CLOCK_DLY_10_CYC;    \
    CLOCK_DLY_4_CYC

/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/

/**
 * @brief:  External 24M crystal using internal or external capacitors
 * @note:   If the software configuration and hardware board does not match,
 *          it may lead to the following problems:
 *          crystal clock frequency is not allowed,  slow crystal vibration caused by the chip reset, etc.
 */
typedef enum
{
    INTERNAL_CAP_XTAL24M = 0, /**<    Use the chip's internal crystal capacitors,
                                 <p>  hardware boards can not have 24M crystal matching capacitors */
    EXTERNAL_CAP_XTAL24M = 1, /**<    Use an external crystal capacitor,
                                <p>  the hardware board needs to have a matching capacitor for the 24M crystal,
                                <p>  the program will turn off the chip's internal capacitor */
} cap_typedef_e;

/**
 * @brief   Power type for different application
 * @note    Chip version A0 don't support DCDC_1P25_LDO_1P8 mode, only A1 can use.
 *          Chip version A0/A1 don't support DCDC_1P25_DCDC_1P8 mode, only A2 can use.
 */
typedef enum
{
    LDO_1P25_LDO_1P8  = 0x00, /**< 1.25V-LDO & 1.8V-LDO mode */
    DCDC_1P25_LDO_1P8 = 0x01, /**< 1.25V-DCDC & 1.8V-LDO mode */
    //DCDC_1P25_DCDC_1P8   = 0x03, /**< 1.25V-DCDC & 1.8V-DCDC mode */
} power_mode_e;

/**
 * @brief   This enumeration is used to select whether VBAT can be greater than 3.6V.
 */
typedef enum
{
    VBAT_MAX_VALUE_GREATER_THAN_3V6 = 0x00,   /**  VBAT must be greater than 2.2V. VBAT may be greater than 3.6V.
                                                <p>  In this configuration the bypass is closed
                                                <p>  and the VBAT voltage passes through the 3V3 LDO to supply power to the chip.
                                                <p>  The voltage of the GPIO pin (VOH) is the voltage after VBAT passes through the LDO (V_ldo),
                                                <p>  and the maximum value is about 3.3V floating 10% (V_ldoh).
                                                <p>  When VBAT > V_ldoh, <p>VOH = V_ldo = V_ldoh(no load).
                                                <p>  When VBAT < V_ldoh, <p>VOH = V_ldo = VBAT(no load) */
    VBAT_MAX_VALUE_LESS_THAN_3V6    = BIT(3), /**  VBAT must be below 3.6V. VBAT may be below 2.2V.
                                                <p>  In this configuration bypass is turned on.vbat is directly supplying power to the chip
                                                <p>  VOH(the output voltage of GPIO)= VBAT */
} vbat_type_e;

/**
 * @brief command table for special registers
 */
typedef struct tbl_cmd_set_t
{
    unsigned int  adr;
    unsigned char dat;
    unsigned char cmd;
} tbl_cmd_set_t;

/**
 * @brief   chip version.
 * @note    this value should confirm when chip reversion.
 */
typedef enum
{
    CHIP_VERSION_A0 = 0x00,
    CHIP_VERSION_A1 = 0x01,
    CHIP_VERSION_A2 = 0x81,
} sys_chip_version_e;

/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/

extern unsigned int g_chip_version;

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/
/**
 * @brief      This function reboot mcu.
 * @return     none
 */
_attribute_text_sec_ void sys_reboot(void);

/**
 * @brief      This function reboot mcu.
 * @return     none
 */
_attribute_ram_code_sec_noinline_ void sys_reboot_ram(void);

/**
 * @brief       This function serves to initialize system.
 * @param[in]   power_mode  - power mode(LDO/DCDC_LDO)
 * @param[in]   vbat_v      - This parameter is used to determine whether the VBAT voltage can be greater than 3.6V.
 * @attention   If vbat_v is set to VBAT_MAX_VALUE_LESS_THAN_3V6, then gpio_v can only be set to GPIO_VOLTAGE_3V3.
 * @return      none
 * @note        -# For crystal oscillator with slow start-up or poor quality, after calling this function,
 *                 a reboot will be triggered(whether a reboot has occurred can be judged by using pm_update_status_info() and pm_get_sw_reboot_event()).
 *                 For the case where the crystal oscillator used is very slow to start-up, you can call the pm_set_xtal_stable_timer_param interface
 *                 to adjust the waiting time for the crystal oscillator to start before calling the sys_init interface.
 *                 When this time is adjusted to meet the crystal oscillator requirements, it will not reboot.
 *              -# Before calling this interface, you need to ensure that the input function of PA2 is disable.
 *                 and in order to prevent errors, the PA2 input function is disabled on this interface.(BUT-53)
 */
_attribute_ram_code_sec_noinline_ void sys_init(power_mode_e power_mode, vbat_type_e vbat_v, cap_typedef_e cap);

/**
 * @brief     this function servers to manual set crystal.
 * @return    none.
 * @note      This function can only used when cclk is 24M RC cause the function execution process will power down the 24M crystal.
 */
_attribute_ram_code_sec_noinline_ void crystal_manual_settle(void);

#endif
