/********************************************************************************************************
 * @file     tl_common.h
 *
 * @brief    the common header file collection
 *
 * @author
 * @date     Oct. 8, 2016
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

#include "user_config.h"
#include "platform_includes.h"

#include "common/types.h"
#include "common/bit.h"
#include "common/utility.h"
#include "common/utlist.h"
#include "common/list.h"
#include "common/static_assert.h"
#include "common/assert.h"
#include "common/string.h"
#include "common/tlPrintf.h"
#include "common/compiler.h"
#include "common/mempool.h"

#include "os/timer.h"
#include "os/ev_poll.h"
#include "os/ev_buffer.h"
#include "os/ev_queue.h"
#include "os/ev_timer.h"
#include "os/ev.h"

#include "drivers/drv_gpio_irq.h"
#include "drivers/drv_adc.h"
#include "drivers/drv_flash.h"
#include "drivers/drv_i2c.h"
#include "drivers/drv_spi.h"
#include "drivers/drv_pwm.h"
#include "drivers/drv_uart.h"
#include "drivers/drv_adc.h"
#include "drivers/drv_pm.h"
#include "drivers/keyboard.h"
#include "drivers/tl_putchar.h"
#include "drivers/nv.h"



