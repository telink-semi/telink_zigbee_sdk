/********************************************************************************************************
 * @file    analog_afe3v_reg.h
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
#ifndef ANALOG_AFE3V_REG_H
#define ANALOG_AFE3V_REG_H
#include "soc.h"

#define areg_aon_0x05           0x05
enum{
    FLD_32K_RC_PD               = BIT(0),//power down 32KHz RC oscillator
    FLD_32K_XTAL_PD             = BIT(1),//power down 32k crystal
    FLD_24M_RC_PD               = BIT(2),//power down of 24MHz RC oscillator
    FLD_24M_XTAL_PD             = BIT(3),//power down of 24MHz XTL oscillator 
    FLD_PD_PL_ALL_3V            = BIT(4),//power down of power logic, 4.2V VBUS_LDO and DCDC
    FLD_PD_PL_DCDC_LDO_3V       = BIT(5),//power down of DCDC & LDO module
    FLD_PD_PL_VBAT_LDO_3V       = BIT(6),//power down of VBAT_LDO(4.2V)
    FLD_ANA_LDO_PD_3V           = BIT(7),//power down ana LDO
};

#define areg_aon_0x06           0x06
enum{
    FLD_PD_BBPLL_LDO            = BIT(0),//power down bbpll LDO
    FLD_PD_LC_COMP_3V           = BIT(1),//power down of low current comparator
    FLD_PD_TEMP_SENSOR_3V       = BIT(2),//power down of temp sensor
    FLD_PD_VBAT_SW              = BIT(3),//power down of bypass switch(VBAT LDO)
    //RSVD
    //RSVD
    FLD_PD_SPD_LDO              = BIT(6),//power down of spd ldo
    FLD_PD_DIG_RET_LDO          = BIT(7),//power down of retention ldo
};

#define areg_aon_0x08           0x08
enum{
    FLD_PD_VDD_RAM              = BIT(6),//power down of supply to ram
    //RSVD
    FLD_PD_VDD_DCORE            = BIT(7),//power down the supply to dcore
};

#define areg_aon_0x09           0x09
#define areg_aon_0x0a           0x0a
#define areg_aon_0x0b           0x0b
enum{
    FLD_PD_NVT_0P94             = BIT(0),
    FLD_PD_NVT_1P8              = BIT(1),
    FLD_MSCN_PULLUP_RES_ENB     = BIT(6),
};

#define areg_aon_0x0c           0x0c
#define areg_aon_0x0e           0x0e
#define areg_aon_0x0f           0x0f

/**
 * Customers cannot use analog register 0x35 because driver and chip functions are occupied, details are as follows:
 * [Bit0]: If this bit is 1, it means that reboot or power on has occurred. If this bit is 0, it means that sleep has occurred.
 * [Bit1~7]: These bits are used by the driver and cannot be used by the customer.
 */
#define PM_ANA_REG_WD_CLR_BUF0          0x35 // initial value 0xff.
enum{
    POWERON_FLAG                 = BIT(0),
};

/**
 * Customers cannot use analog register 0x3a because driver and chip functions are occupied, details are as follows:
 * [Bit0]: If this bit is 1, it means that reboot has occurred.
 * REBOOT_MANUAL, it means that manually invoke the reboot interface.
 * REBOOT_XTAL_UNSTABLE, it means that the software calls the function sys_reboot() when the crystal oscillator does not start up normally.
 * REBOOT_PM_CLR_PLIC_REQUEST_FAIL, it means that the pm_sleep_wakeup function failed to clear the PM wake flag bit when using the deep wake source, and the software called sys_reboot().
 * Bit0 has been used and can no longer be used, any combination of other bits can be used.
 */
#define PM_ANA_REG_POWER_ON_CLR_BUF0    0x3a // initial value 0x00.
typedef enum{
    REBOOT_FLAG                   = BIT(0),
    SW_REBOOT_REASON              = BIT_RNG(1,7),
}pm_poweron_clr_buf0_e;

//Since there are only 7 bits, this enumeration ranges from 0 to 127.
typedef enum{
    APPLIC_INTERFACE              = 0x00,
    XTAL_UNSTABLE                 = 0x01,
    PM_CLR_PLIC_REQUEST_FAIL      = 0x02,
    WAIT_TIMEOUT                  = 0x03,
}pm_sw_reboot_reason_e;

#define areg_aon_0x3d           0x3d

#define areg_aon_0x3e           0x3e

#define areg_aon_0x4b           0x4b

#define areg_aon_0x4c           0x4c
enum{
    FLD_PD_32K_RC               = BIT(0),
    FLD_PD_32K_XTAL             = BIT(1),
//    FLD_PD_RSVD_BIT2            = BIT(2),
    FLD_PD_24M_XTAL             = BIT(3),
    FLD_PD_LOGIC                = BIT(4),
    FLD_PD_DCDC                 = BIT(5),
    FLD_PD_VBUS_LDO             = BIT(6),
    FLD_PD_ANA_BBPLL_TEMP_LDO   = BIT(7),
    FLD_PD_LPC                  = BIT(8),
//    FLD_PD_RSVD_BIT9            = BIT(9),
    FLD_PD_UVLO_IB              = BIT(10),////Cannot be turned off during sleep.
    FLD_PD_VBUS_SW              = BIT(11),
//    FLD_PD_RSVD_BIT12           = BIT(12),
//    FLD_PD_RSVD_BIT13           = BIT(13),
    FLD_PD_SEQUENCE_EN          = BIT(14),
    FLD_PD_ISOLATION            = BIT(15),
};

#define areg_aon_0x4e           0x4e
enum{
    FLD_XO_ISEL_PMU             = BIT_RNG(2,5),//xo current sel
    FLD_CLK32K_SEL              = BIT(7),//0: sel 32k RC 1:sel 32K Xtal
};

#define areg_aon_0x4f           0x4f
enum{
    FLD_RC_32K_CAP_SEL          = BIT(6),//0:sel cap from pm_top,1: sel cap from calibration module
    FLD_RC_24M_CAP_SEL          = BIT(7),//0:sel cap from pm_top,1: sel cap from calibration module
};

#define areg_aon_0x51           0x51
#define areg_aon_0x52           0x52

#define areg_aon_0x64           0x64
typedef enum {
    FLD_WAKEUP_STATUS_PAD           = BIT(0),
    FLD_WAKEUP_STATUS_CORE          = BIT(1),
    FLD_WAKEUP_STATUS_TIMER         = BIT(2),
    FLD_WAKEUP_STATUS_COMPARATOR    = BIT(3),

    //To clear all wake sources, the parameter of this interface is usually FLD_WAKEUP_STATUS_ALL
    //instead of FLD_WAKEUP_STATUS_INUSE_ALL.
    FLD_WAKEUP_STATUS_ALL           = 0xff,

    //After the wake source is obtained, &WAKEUP_STATUS_INUSE_ALL is needed to determine
    //whether the wake source in use has been cleared, because some of the wake sources
    //that are not in use may have been set up.
    FLD_WAKEUP_STATUS_INUSE_ALL     = 0x0f,
}pm_wakeup_status_e;

#define areg_aon_0x65           0x65

#define areg_aon_0x69           0x69

#define areg_aon_0x79           0x79

#define areg_aon_0x7d           0x7d
typedef enum{
    FLD_PD_ZB_EN                = BIT(0),//power down baseband
    FLD_PD_USB_EN               = BIT(1),//power down usb
    FLD_PD_AUDIO_EN             = BIT(2),//power down audio
    //RSVD
    FLD_PG_CLK_EN               = BIT(7),//enable change power sequence clk
}pm_pd_module_e;

#define areg_aon_0x7e           0x7e

#define areg_aon_0x7f           0x7f
enum{
    FLD_BOOTFROMBROM            = BIT(0),
    FLD_PAD_FILTER_EN           = BIT(5),
};


#endif
