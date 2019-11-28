/********************************************************************************************************
 * @file     platform_includes.h
 *
 * @brief    This file includes all the modules drivers API
 *
 * @author
 * @date     May. 5, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#pragma once


#if defined (MCU_CORE_826x)

#include "./chip_826x/includes/register_826x.h"
#include "./chip_826x/includes/bsp.h"
#include "./chip_826x/includes/clock_826x.h"
#include "./chip_826x/includes/pm_826x.h"
#include "./chip_826x/includes/rf_drv_826x.h"
#include "./chip_826x/includes/adc_826x.h"
#include "./chip_826x/includes/gpio_826x.h"
#include "./chip_826x/includes/gpio_default_826x.h"
#include "./chip_826x/includes/pwm_826x.h"
#include "./chip_826x/includes/aes_ccm_826x.h"
#include "./chip_826x/includes/uart_826x.h"
#include "./chip_826x/includes/flash_826x.h"
#include "./chip_826x/includes/i2c_826x.h"
#include "./chip_826x/includes/spi_826x.h"
#include "./chip_826x/includes/putchar.h"
#endif


#if defined (MCU_CORE_8258)
#include "./chip_8258/driver_8258.h"
#endif


#if defined (MCU_CORE_HAWK)
#include "./chip_hawk/adc.h"
#include "./chip_hawk/aes_128.h"
#include "./chip_hawk/bsp.h"
#include "./chip_hawk/emitest.h"
#include "./chip_hawk/flash.h"
#include "./chip_hawk/gpio.h"
#include "./chip_hawk/gpio_default.h"
#include "./chip_hawk/i2c.h"
#include "./chip_hawk/irq.h"
#include "./chip_hawk/mspi.h"
#include "./chip_hawk/pga.h"
#include "./chip_hawk/pm.h"
#include "./chip_hawk/pwm.h"
#include "./chip_hawk/register_5316.h"
#include "./chip_hawk/rf.h"
#include "./chip_hawk/rf_drv_5316.h"
#include "./chip_hawk/spi.h"
#include "./chip_hawk/timer.h"
#include "./chip_hawk/uart.h"
#endif
