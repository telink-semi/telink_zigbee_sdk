/********************************************************************************************************
 * @file    charger.c
 *
 * @brief   This is the source file for B91
 *
 * @author  Driver Group
 * @date    2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "charger.h"
#include "analog.h"

/**
 * @brief      The function of this API is to set charger mode.
 * @param[in]  manual_mode_e - charger model.
 * @return     none
 */
void charger_set_mode(charger_mode_e charger_mode)
{
    analog_write_reg8(reg_charger_mode, ((analog_read_reg8(reg_charger_mode) & (~AUTO_MANUAL_SEL)) & (~MANUAL_MODE)) | charger_mode);
}

/**
 * @brief      The function of this API is to adjust constant current charging current.
 * @param[in]  cc_current - enumeration of current gear.
 * @return     none
 */
void charger_set_cc_current(charger_cc_current_e cc_current)
{
    analog_write_reg8(reg_charger_ichg, ((analog_read_reg8(reg_charger_ichg) & (~ICHG_TRIM)) | (cc_current & ICHG_TRIM)));
}

/**
 * @brief      The function of this API is to adjust charge in advance charging current.
 * @param[in]  tc_current - enumeration of current gear.
 * @return     none
 */
void charger_set_tc_current(charger_tc_current_e tc_current)
{
    analog_write_reg8(reg_charger_ichg, ((analog_read_reg8(reg_charger_ichg) & (~ICHG_TRIM)) | (tc_current & ICHG_TRIM)));
}

/**
 * @brief      The function of this API is to adjust constant voltage charging voltage.
 * @param[in]  cv_voltage - enumeration of voltage gear.
 * @return     none
 */
void charger_set_cv_voltage(charger_cv_voltage_e cv_voltage)
{
    analog_write_reg8(reg_charger_vchg, ((analog_read_reg8(reg_charger_vchg) & (~VCHG_TRIM)) | (cv_voltage & VCHG_TRIM)));
}

/**
 * @brief      The function of this API is to check whether the usb is on or off,
 *             due to the jitter of manual usb insertion and removal, through the experimental test, the jitter range is as follows:
 *             the max of the usb insertion: 3ms    the max of the usb removal:1ms
 * @return     1:vbus power on 0:vbus power down;
 */
_Bool charger_get_vbus_status()
{
    return analog_read_reg8(reg_charger_status) & CHG_USB_ON_OFF;
}

/*******************************************************************************************************************
 *                the functions for internal use in charger,in order to test                                       *
 ******************************************************************************************************************/
/**
 * @brief      The function of this API is to get charge status.
 * @return     none
 */
charger_status_e charger_get_status()
{
    return analog_read_reg8(reg_charger_status);
}
