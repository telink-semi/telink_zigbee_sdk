/********************************************************************************************************
 * @file    pm_internal.h
 *
 * @brief   This is the header file for B92
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
#ifndef PM_INTERNAL_H_
#define PM_INTERNAL_H_


/**
 * @brief   active mode vdd_dcore output trim definition
 * @note    The measured values deviate significantly from the corresponding gear values.
 *          The note indicates that these are the measured values of a chip for reference only.
 */
typedef enum
{
    PM_VDD_DCORE_VOLTAGE_0V800 = 0x00,  /**<  measured 0V950  */
    PM_VDD_DCORE_VOLTAGE_0V825 = 0x02,  /**<  measured 0V975  */
    PM_VDD_DCORE_VOLTAGE_0V850 = 0x04,  /**<  measured 1V000  */
    PM_VDD_DCORE_VOLTAGE_0V875 = 0x06,  /**<  measured 1V025  */
    PM_VDD_DCORE_VOLTAGE_0V900 = 0x08,  /**<  measured 1V050  */
    PM_VDD_DCORE_VOLTAGE_0V925 = 0x0a,  /**<  measured 1V075  */
    PM_VDD_DCORE_VOLTAGE_0V950 = 0x0c,  /**<  measured 1V100  */
    PM_VDD_DCORE_VOLTAGE_0V975 = 0x0e,  /**<  measured 1V125  */
    PM_VDD_DCORE_VOLTAGE_1V000 = 0x10,  /**<  measured 1V150  */
    PM_VDD_DCORE_VOLTAGE_1V025 = 0x12,  /**<  measured 1V175  */
    PM_VDD_DCORE_VOLTAGE_1V050 = 0x14,  /**<  measured 1V200  */
    PM_VDD_DCORE_VOLTAGE_1V075 = 0x16,  /**<  measured 1V225  */
    PM_VDD_DCORE_VOLTAGE_1V100 = 0x18,  /**<  measured 1V250  */
    PM_VDD_DCORE_VOLTAGE_1V125 = 0x1a,  /**<  measured 1V275  */
    PM_VDD_DCORE_VOLTAGE_1V150 = 0x1c,  /**<  measured 1V300  */
    PM_VDD_DCORE_VOLTAGE_1V175 = 0x1e,  /**<  measured 1V350  */
} pm_vdd_dcore_voltage_e;


/**
 * @brief       This function servers to set vdd_dcore output voltage in active mode.
 * @param[in]   voltage - vdd_dcore setting.
 * @return      none.
 */
static inline void pm_set_vdd_dcore(pm_vdd_dcore_voltage_e voltage)
{
    analog_write_reg8(0x1d, (analog_read_reg8(0x1d) & 0xe0) | voltage);
}

#endif
