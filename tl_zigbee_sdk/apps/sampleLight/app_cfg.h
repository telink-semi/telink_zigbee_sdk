/********************************************************************************************************
 * @file    app_cfg.h
 *
 * @brief   This is the header file for app_cfg
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
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

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

/**********************************************************************
 * App configuration
 */
/* Debug mode */
#define	UART_PRINTF_MODE                        0
#define USB_PRINTF_MODE                         0

/* HCI interface */
#define	ZBHCI_UART                              0

/* BDB */
#define TOUCHLINK_SUPPORT                       1
#define FIND_AND_BIND_SUPPORT                   0

/* Voltage detect module */
/* If VOLTAGE_DETECT_ENABLE is set,
 * 1) if MCU_CORE_826x is defined, the DRV_ADC_VBAT_MODE mode is used by default,
 * and there is no need to configure the detection IO port;
 * 2) if MCU_CORE_8258 or MCU_CORE_8278 is defined, the DRV_ADC_VBAT_MODE mode is used by default,
 * we need to configure the detection IO port, and the IO must be in a floating state.
 * 3) if MCU_CORE_B91 is defined, the DRV_ADC_BASE_MODE mode is used by default,
 * we need to configure the detection IO port, and the IO must be connected to the target under test,
 * such as VCC.
 */
#define VOLTAGE_DETECT_ENABLE                   0

/* Flash protect module */
/* Only the firmware area will be locked, the NV data area will not be locked.
 * For details, please refer to drv_flash.c file.
 */
#define FLASH_PROTECT_ENABLE                    1

/* Watch dog module */
#define MODULE_WATCHDOG_ENABLE                  0

/* UART module */
#if ZBHCI_UART
#define	MODULE_UART_ENABLE                      1
#endif

#if (ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID || ZBHCI_UART)
#define ZBHCI_EN                                1
#endif

/**********************************************************************
 * Board definitions
 */
/* Board ID */
#define BOARD_826x_EVK                          0
#define BOARD_826x_DONGLE                       1
#define BOARD_8258_EVK                          2//DEPRECATED
#define BOARD_8258_EVK_V1P2                     3//C1T139A30_V1.2
#define BOARD_8258_DONGLE                       4
#define BOARD_8278_EVK                          5
#define BOARD_8278_DONGLE                       6
#define BOARD_B91_EVK                           7
#define BOARD_B91_DONGLE                        8
#define BOARD_B92_EVK                           9
#define BOARD_B92_DONGLE                        10
#define BOARD_TL721X_EVK                        11
#define BOARD_TL721X_DONGLE                     12
#define BOARD_TL321X_EVK                        13
#define BOARD_TL321X_DONGLE                     14
//Module
#define BOARD_ML7218D_MERCURY                   15//ML7218D-MERCURY-M0-PE11-V1.3
#define BOARD_ML7218A_GAIA                      16//ML7218A_GAIA-M0-PE11-V1.3

/* Board define */
#if defined(MCU_CORE_826x)
    #define BOARD                               BOARD_826x_DONGLE//BOARD_826x_EVK
    #define CLOCK_SYS_CLOCK_HZ                  32000000
#elif defined(MCU_CORE_8258)
    #define BOARD                               BOARD_8258_DONGLE//BOARD_8258_EVK_V1P2
    #define CLOCK_SYS_CLOCK_HZ                  48000000
#elif defined(MCU_CORE_8278)
    #define BOARD                               BOARD_8278_DONGLE//BOARD_8278_EVK
    #define CLOCK_SYS_CLOCK_HZ                  48000000
#elif defined(MCU_CORE_B91)
    #define BOARD                               BOARD_B91_DONGLE//BOARD_B91_EVK
    #define CLOCK_SYS_CLOCK_HZ                  48000000
#elif defined(MCU_CORE_B92)
    #define BOARD                               BOARD_B92_DONGLE//BOARD_B92_EVK
    #define CLOCK_SYS_CLOCK_HZ                  48000000
#elif defined(MCU_CORE_TL721X)
    #define BOARD                               BOARD_TL721X_DONGLE//BOARD_TL721X_EVK
    #define CLOCK_SYS_CLOCK_HZ                  120000000
#elif defined(MCU_CORE_TL321X)
    #define BOARD                               BOARD_TL321X_DONGLE//BOARD_TL321X_EVK
    #define CLOCK_SYS_CLOCK_HZ                  48000000
#else
    #error "MCU is undefined!"
#endif

/**********************************************************************
 * Version configuration
 */
#include "version_cfg.h"

/**********************************************************************
 * Board configuration
 */
#if (BOARD == BOARD_826x_EVK)
    #include "board_826x_evk.h"
#elif (BOARD == BOARD_826x_DONGLE)
    #include "board_826x_dongle.h"
#elif (BOARD == BOARD_8258_DONGLE)
    #include "board_8258_dongle.h"
#elif (BOARD == BOARD_8258_EVK)
    #include "board_8258_evk.h"//DEPRECATED
#elif (BOARD == BOARD_8258_EVK_V1P2)
    #include "board_8258_evk_v1p2.h"
#elif (BOARD == BOARD_8278_EVK)
    #include "board_8278_evk.h"
#elif (BOARD == BOARD_8278_DONGLE)
    #include "board_8278_dongle.h"
#elif (BOARD == BOARD_B91_EVK)
    #include "board_b91_evk.h"
#elif (BOARD == BOARD_B91_DONGLE)
    #include "board_b91_dongle.h"
#elif (BOARD == BOARD_B92_EVK)
    #include "board_b92_evk.h"
#elif (BOARD == BOARD_B92_DONGLE)
    #include "board_b92_dongle.h"
#elif (BOARD == BOARD_TL721X_EVK)
    #include "board_tl721x_evk.h"
#elif (BOARD == BOARD_TL721X_DONGLE)
    #include "board_tl721x_dongle.h"
#elif (BOARD == BOARD_TL321X_EVK)
    #include "board_tl321x_evk.h"
#elif (BOARD == BOARD_TL321X_DONGLE)
    #include "board_tl321x_dongle.h"
#elif (BOARD == BOARD_ML7218D_MERCURY)
    #include "board_ml7218d_mercury.h"
#elif (BOARD == BOARD_ML7218A_GAIA)
    #include "board_ml7218a_gaia.h"
#endif

/**********************************************************************
 * Stack configuration
 */
#include "stack_cfg.h"

/**********************************************************************
 * ZCL cluster configuration
 */
#define ZCL_ON_OFF_SUPPORT                      1
#if (COLOR_RGB_SUPPORT || COLOR_CCT_SUPPORT)
#define ZCL_LEVEL_CTRL_SUPPORT                  1
#define ZCL_LIGHT_COLOR_CONTROL_SUPPORT         1
#elif (BRIGHTNESS_SUPPORT)
#define ZCL_LEVEL_CTRL_SUPPORT                  1
#endif
#define ZCL_GROUP_SUPPORT                       1
#define ZCL_SCENE_SUPPORT                       1
#define ZCL_OTA_SUPPORT                         1
#define ZCL_GP_SUPPORT                          1
#define ZCL_WWAH_SUPPORT                        0
#if TOUCHLINK_SUPPORT
#define ZCL_ZLL_COMMISSIONING_SUPPORT           1
#endif
//test
#define AF_TEST_ENABLE                          0

/**********************************************************************
 * EV configuration
 */
typedef enum {
    EV_POLL_ED_DETECT,
    EV_POLL_HCI,
    EV_POLL_IDLE,
    EV_POLL_MAX,
} ev_poll_e;

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
