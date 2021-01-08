/********************************************************************************************************
 * @file	tl_common.h
 *
 * @brief	This is the header file for tl_common
 *
 * @author	Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#pragma once


#include "app_cfg.h"
#include "platform.h"

#include "common/types.h"
#include "common/compiler.h"
#include "common/static_assert.h"
#include "common/assert.h"
#include "common/bit.h"
#include "common/utility.h"
#include "common/utlist.h"
#include "common/list.h"
#include "common/string.h"
#include "common/tlPrintf.h"
#include "common/mempool.h"

#include "os/ev_poll.h"
#include "os/ev_buffer.h"
#include "os/ev_queue.h"
#include "os/ev_timer.h"
#include "os/ev.h"

#include "drivers/drv_hw.h"
#include "drivers/drv_radio.h"
#include "drivers/drv_gpio.h"
#include "drivers/drv_adc.h"
#include "drivers/drv_flash.h"
#include "drivers/drv_i2c.h"
#include "drivers/drv_spi.h"
#include "drivers/drv_pwm.h"
#include "drivers/drv_uart.h"
#include "drivers/drv_pm.h"
#include "drivers/drv_timer.h"
#include "drivers/drv_keyboard.h"
#include "drivers/drv_nv.h"
#include "drivers/drv_putchar.h"
#include "drivers/drv_usb.h"

