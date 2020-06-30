/********************************************************************************************************
 * @file     app_config.h
 *
 * @brief    Application configuration
 *
 * @author
 * @date     Jan. 4, 2018
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

#define _USER_CONFIG_DEFINED_		1// must define this macro to make others known
#define	__LOG_RT_ENABLE__			0

//////////// product  Information  //////////////////////////////
#define TOUCHLINK_SUPPORT			0
#define FIND_AND_BIND_SUPPORT		0

/* debug mode config */
#define	UART_PRINTF_MODE			0
#define USB_PRINTF_MODE         	0

#define PM_ENABLE					1
#define PA_ENABLE					0


/* board ID */
#define BOARD_8258_DONGLE			0
#define BOARD_8258_EVK				1
#define BOARD_8258_EVK_V1P2			2

/* board define */
#ifdef MCU_CORE_8258
	#define BOARD					BOARD_8258_DONGLE//BOARD_8258_EVK
	#define CLOCK_SYS_CLOCK_HZ  	48000000
#endif

#if (BOARD == BOARD_8258_DONGLE)
	#include "board_8258_dongle.h"
#elif (BOARD == BOARD_8258_EVK)
	#include "board_8258_evk.h"
#elif (BOARD == BOARD_8258_EVK_V1P2)
	#include "board_8258_evk_v1p2.h"
#else
	#error
#endif


#define MODULE_BUFM_ENABLE        1
#define MODULE_PRIQ_ENABLE        1
#define EV_POST_TASK_ENABLE       1

/* interrupt */
#define IRQ_TIMER1_ENABLE         1
#define	IRQ_GPIO_ENABLE			  0

/* module selection */
#define MODULE_WATCHDOG_ENABLE	0
#define	MODULE_UART_ENABLE		0

/* Rf mode: 250K */
#define	RF_MODE_250K		1


/**
 *  @brief ZCL cluster support setting
 */
#define ZCL_IAS_ZONE_SUPPORT						1
#define ZCL_POLL_CTRL_SUPPORT						1
#define ZCL_OTA_SUPPORT								1


///////////////////  Zigbee Profile Configuration /////////////////////////////////
#include "stack_cfg.h"

typedef enum{
	EV_EVENT_MAX = 1,
} ev_event_e;

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
