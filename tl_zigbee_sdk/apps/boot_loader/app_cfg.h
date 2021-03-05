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

#define _USER_CONFIG_DEFINED_	1	// must define this macro to make others known
#define	__LOG_RT_ENABLE__		0

/* Board define */
#if defined(MCU_CORE_826x)
#if !PA_ENABLE
	#define BOARD					BOARD_826x_DONGLE
#else
	#define BOARD					BOARD_826x_DONGLE_PA
#endif
	/* system clock config */
	#define CLOCK_SYS_CLOCK_HZ  	32000000
#elif defined(MCU_CORE_8258)
	#define BOARD					BOARD_8258_DONGLE
	/* system clock config */
	#define CLOCK_SYS_CLOCK_HZ  	48000000
#elif defined(MCU_CORE_8278)
	#define BOARD					BOARD_8278_DONGLE//BOARD_8278_EVK
	/* system clock config */
	#define CLOCK_SYS_CLOCK_HZ  	48000000
#elif defined(MCU_CORE_B91)
	#define BOARD					BOARD_9518_EVK
	/* system clock config */
	#define CLOCK_SYS_CLOCK_HZ  	48000000
#else
	#error "MCU is undefined!"
#endif

/* board ID */
#define BOARD_826x_EVK			0
#define BOARD_826x_DONGLE		1
#define BOARD_8258_EVK			2
#define BOARD_8258_EVK_V1P2		3
#define BOARD_8258_DONGLE		4


#if	(BOARD == BOARD_826x_DONGLE)
	#include "board_826x_dongle.h"
#elif(BOARD == BOARD_8258_DONGLE)
	#include "board_8258_dongle.h"
#elif(BOARD == BOARD_8258_EVK)
	#include "board_8258_evk.h"
#elif(BOARD == BOARD_8258_EVK_V1P2)
	#include "board_8258_evk_v1p2.h"
#else
	#include "board_826x_evk.h"
#endif


typedef enum{
	EV_EVENT_MAX = 1,
} ev_event_e;

enum{
	EV_FIRED_EVENT_MAX = 1
};


typedef enum{
	EV_POLL_ED_DETECT,
	EV_POLL_FACTORY_RST,
	EV_POLL_HCI,
    EV_POLL_IDLE,
	EV_POLL_MAX,
}ev_poll_e;

#define	IRQ_USB_PWDN_ENABLE  	0

//////////// product  Information  //////////////////////////////
#define ID_VENDOR				0x248a			// for report
#define ID_PRODUCT_BASE			0x880C
#define STRING_VENDOR			L"Telink"
#define STRING_PRODUCT			L"BLE Remote KMA Dongle"
#define STRING_SERIAL			L"TLSR8258"

#define		BLE_REMOTE_SECURITY_ENABLE		1

    /* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
