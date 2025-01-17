/********************************************************************************************************
 * @file    sys.h
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
#include "reg_include/stimer_reg.h"
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
#define CLOCK_DLY_24_CYC \
    CLOCK_DLY_6_CYC;     \
    CLOCK_DLY_6_CYC;     \
    CLOCK_DLY_6_CYC;     \
    CLOCK_DLY_6_CYC
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
 */
typedef enum
{
    LDO_1P4_LDO_2P0   = 0x00, /**< 1.4V-LDO & 2.0V-LDO mode */
    DCDC_1P4_LDO_2P0  = 0x01, /**< 1.4V-DCDC & 2.0V-LDO mode */
    DCDC_1P4_DCDC_2P0 = 0x03, /**< 1.4V-DCDC & 2.0V-DCDC mode */
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
 * @enum        gpio_voltage_e
 * @brief       This is the configuration of GPIO voltage.
 * @attention   If the enumeration uses GPIO_VOLTAGE_1V8, the following usage considerations need to be noted:
 *              -# Pay attention to check the sampling range of ADC, the maximum detection voltage of the ADC input cannot be higher than 1.8V.
 *                 If a voltage higher than this needs to be detected, external access to the divider circuit is required.
 *              -# ADC_VBAT_SAMPLE can not be used.
 *                 Users can use external voltage divider instead, the details refer to the Driver SDK Developer Handbook for this chip.
 *              -# Since 1.8V IO does not comply with USB electrical layer regulations, GPIO cannot be configured to 1.8V when using USB.
 *              -# When using VBUS 5V to power the chip, only after register configuration will GPIO be 1.8V.
 *                 So from power-on to register configuration, the default output GPIO(PC5) is still 3.3V. reboot will also be 3.3V for a while.
 *                 (the other default output GPIO(PG1/PG2/PG3/PG5) are not affected by this.)
 */
typedef enum
{
    GPIO_VOLTAGE_3V3 = 0x00, /**< the GPIO voltage is set to 3.3V. */
    GPIO_VOLTAGE_1V8 = 0x01, /**< the GPIO voltage is set to 1.8V. */
} gpio_voltage_e;

/**
 * @brief command table for special registers
 */
typedef struct tbl_cmd_set_t
{
    unsigned int  adr;
    unsigned char dat;
    unsigned char cmd;
} tbl_cmd_set_t;

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
 * @param[in]   power_mode  - power mode(LDO/DCDC/LDO_DCDC)
 * @param[in]   vbat_v      - This parameter is used to determine whether the VBAT voltage can be greater than 3.6V.
 * @param[in]   gpio_v      - This is the configuration of GPIO voltage.
 *                            For some chip models the GPIO voltage is fixed 3.3V or fixed 1.8V,
 *                            For other GPIO models the voltage is configurable:
 *                            Requires hardware configuration: 3v3 (CFG_VIO connects to VSS) or 1V8 (CFG_VIO connects to VDDO3/AVDD3)),
 *                            please configure this parameter correctly according to the chip data sheet and the corresponding board design.
 * @param[in]   cap     - This parameter is used to determine whether to close the internal capacitor.
 * @attention   If vbat_v is set to VBAT_MAX_VALUE_LESS_THAN_3V6, then gpio_v can only be set to GPIO_VOLTAGE_3V3.
 * @return      none
 * @note        For crystal oscillator with slow start-up or poor quality, after calling this function, 
 *              a reboot will be triggered(whether a reboot has occurred can be judged by using PM_ANA_REG_POWER_ON_CLR_BUF0[bit1]).
 *              For the case where the crystal oscillator used is very slow to start-up, you can call the pm_set_xtal_stable_timer_param interface 
 *              to adjust the waiting time for the crystal oscillator to start before calling the sys_init interface.
 *              When this time is adjusted to meet the crystal oscillator requirements, it will not reboot.
 */
void sys_init(power_mode_e power_mode, vbat_type_e vbat_v, gpio_voltage_e gpio_v, cap_typedef_e cap);
/**
 * @brief      This function performs a series of operations of writing digital or analog registers
 *             according to a command table
 * @param[in]  pt    - pointer to a command table containing several writing commands
 * @param[in]  size  - number of commands in the table
 * @return     number of commands are carried out
 */

int write_reg_table(const tbl_cmd_set_t *pt, int size);

/**
 * @brief      This function servers to get calibration value from EFUSE.
 * @param[in]  gpio_type - select the type of GPIO.
 * @return     1 - the calibration value update, 0 - the calibration value is not update.
 */
unsigned char efuse_calib_adc_vref(gpio_voltage_e gpio_type);

/**
 * @brief     this function servers to manual set crystal.
 * @return    none.
 * @note      This function can only used when cclk is 24M RC cause the function execution process will power down the 24M crystal.
 */
_attribute_ram_code_sec_optimize_o2_ void crystal_manual_settle(void);

/**
* @brief      This function servers to get chip id from EFUSE.
* @param[in]  chip_id_buff - store chip id. Chip ID is 16 bytes.
* @return     1 - operation completed,  0 - operation failed.
* @note       Only A3 and later are written as chip id values.
*/
unsigned char efuse_get_chip_id(unsigned char *chip_id_buff);

/**
 * @brief        This function retrieves specific functionality bits from the EFUSE.
 * @return       result:
 *               - bit 0: JTAG function (0 = enabled, 1 = disabled)
 *               - bit 1: SWS function (0 = enabled, 1 = disabled)
 *               - bit 2: Mode selector (1 = secure boot mode, 0 = normal mode)
 * @note         If there's an error reading the EFUSE, returns 0xFF.
 */
unsigned char efuse_get_chip_status(void);

#endif
