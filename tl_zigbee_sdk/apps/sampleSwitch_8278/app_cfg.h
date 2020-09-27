/********************************************************************************************************
 * @file     app_config.h
 *
 * @brief    application configuration
 *
 * @author
 * @date     Dec. 1, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define _USER_CONFIG_DEFINED_		1//must define this macro to make others known

/**********************************************************************
 * Version configuration
 */
#include "version_cfg.h"


/**********************************************************************
 * Product  Information
 */
/* Debug mode config */
#define	UART_PRINTF_MODE			0
#define USB_PRINTF_MODE         	0

/* PM */
#define PM_ENABLE					1

/* PA */
#define PA_ENABLE					0

/* BDB */
#define TOUCHLINK_SUPPORT			0
#define FIND_AND_BIND_SUPPORT		0

/* Board ID */
#define BOARD_8278_EVK				0
#define BOARD_8278_DONGLE			1

/* Board define */
#ifdef MCU_CORE_8278
	#define BOARD					BOARD_8278_DONGLE//BOARD_8278_EVK
	#define CLOCK_SYS_CLOCK_HZ  	48000000
#endif

/* Board include */
#if (BOARD == BOARD_8278_EVK)
	#include "board_8278_evk.h"
#elif (BOARD == BOARD_8278_DONGLE)
	#include "board_8278_dongle.h"
#else
	#error
#endif


/* Watch dog module */
#define MODULE_WATCHDOG_ENABLE						0

/* UART module */
#define	MODULE_UART_ENABLE							0

#if (ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID || ZBHCI_UART)
	#define ZBHCI_EN								1
#endif


/**********************************************************************
 * ZCL cluster support setting
 */
#define ZCL_ON_OFF_SUPPORT							1
#define ZCL_LEVEL_CTRL_SUPPORT						1
#define ZCL_LIGHT_COLOR_CONTROL_SUPPORT				1
#define ZCL_GROUP_SUPPORT							1
#define ZCL_OTA_SUPPORT								1
#if TOUCHLINK_SUPPORT
#define ZCL_ZLL_COMMISSIONING_SUPPORT				1
#endif


/**********************************************************************
 * Stack configuration
 */
#include "stack_cfg.h"


/**********************************************************************
 * EV configuration
 */
typedef enum{
	EV_EVENT_MAX = 1,
}ev_event_e;

enum{
	EV_FIRED_EVENT_MAX = 1
};

typedef enum{
	EV_POLL_ED_DETECT,
	EV_POLL_PM,
	EV_POLL_HCI,
    EV_POLL_IDLE,
	EV_POLL_MAX,
}ev_poll_e;

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
