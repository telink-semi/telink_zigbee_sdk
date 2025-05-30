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

/*
 * Enable UART to upgrade image.
 */
#define UART_ENABLE                             1

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


/**********************************************************************
 * Board definitions
 */
/* Board ID */
#define BOARD_826x_EVK                          0
#define BOARD_826x_DONGLE                       1
#define BOARD_826x_DONGLE_PA                    2
#define BOARD_8258_EVK                          3//DEPRECATED
#define BOARD_8258_EVK_V1P2                     4//C1T139A30_V1.2
#define BOARD_8258_DONGLE                       5
#define BOARD_8278_EVK                          6
#define BOARD_8278_DONGLE                       7
#define BOARD_B91_EVK                           8
#define BOARD_B91_DONGLE                        9
#define BOARD_B92_EVK                           10
#define BOARD_B92_DONGLE                        11
#define BOARD_TL721X_EVK                        12
#define BOARD_TL721X_DONGLE                     13
#define BOARD_TL321X_EVK                        14
#define BOARD_TL321X_DONGLE                     15

/* Board define */
#if defined(MCU_CORE_826x)
    #define BOARD                               BOARD_826x_DONGLE
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
#endif


/**********************************************************************
 * EV configuration
 */
typedef enum {
    EV_POLL_UART_PROC,
    EV_POLL_KEY_PRESS,
    EV_POLL_MAX,
} ev_poll_e;

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
