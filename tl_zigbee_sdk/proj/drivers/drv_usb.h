/********************************************************************************************************
 * @file    drv_usb.h
 *
 * @brief   This is the header file for drv_usb
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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

#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)

static inline u8 usb_is_irq_reset(void)
{
    return reg_irq_src & FLD_IRQ_USB_RST_EN;
}

static inline void usb_clear_irq_reset(void)
{
    reg_irq_src = FLD_IRQ_USB_RST_EN;
}

#elif defined(MCU_CORE_B91) || defined(MCU_CORE_TL721X) || \
      defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL521X)

static inline u8 usb_is_irq_reset(void)
{
    return reg_usb_irq_mask & USB_IRQ_RESET_STATUS;
}

static inline void usb_clear_irq_reset(void)
{
    reg_usb_irq_mask |= USB_IRQ_RESET_STATUS;
}

#endif

void drv_usb_init(void);
