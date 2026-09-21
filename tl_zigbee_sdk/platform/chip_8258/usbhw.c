/********************************************************************************************************
 * @file    usbhw.c
 *
 * @brief   This is the source file for B85
 *
 * @author  Driver & Zigbee Group
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
#include "usbhw.h"

/**
 * @brief      This function disables the manual interrupt
 *             (Endpoint8 is the alias of endpoint0)
 * @param[in]  m - the irq mode needs to set
 * @return     none
 */
void usbhw_disable_manual_interrupt(int m)
{
    BM_SET(reg_ctrl_ep_irq_mode, m);
}

/**
 * @brief      This function enable the manual interrupt
 * @param[in]  m - the irq mode needs to set
 * @return     none
 */
void usbhw_enable_manual_interrupt(int m)
{
    BM_CLR(reg_ctrl_ep_irq_mode, m);
}

/**
 * @brief      This function sends a bulk of data to host via the specified endpoint
 * @param[in]  ep - the number of the endpoint
 * @param[in]  data - pointer to the data need to send
 * @param[in]  len - length in byte of the data need to send
 * @return     none
 */
void usbhw_write_ep(unsigned int ep, unsigned char *data, int len)
{
    reg_usb_ep_ptr(ep) = 0;

    for (int i = 0; i < len; ++i) {
        reg_usb_ep_dat(ep) = data[i];
    }
    reg_usb_ep_ctrl(ep) = FLD_EP_DAT_ACK; // ACK
}

/**
 * @brief      This function sends two bytes data to host via the control endpoint
 *             (handy help function)
 * @param[in]  v - the two bytes data need to send
 * @return     none
 */
void usbhw_write_ctrl_ep_u16(unsigned short v)
{
    usbhw_write_ctrl_ep_data(v & 0xff);
    usbhw_write_ctrl_ep_data(v >> 8);
}

/**
 * @brief   This function reads two bytes data from host via the control endpoint
 * @param   none
 * @return  the two bytes data read from the control endpoint
 */
unsigned short usbhw_read_ctrl_ep_u16(void)
{
    unsigned short v = usbhw_read_ctrl_ep_data();
    return (usbhw_read_ctrl_ep_data() << 8) | v;
} 

