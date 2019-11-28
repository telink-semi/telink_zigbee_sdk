/********************************************************************************************************
 * @file     usbhw_i.h 
 *
 * @brief    This is the header file for TLSR8258
 *
 * @author	 public@telink-semi.com;
 * @date     May 8, 2018
 *
 * @par      Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
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
 * @par      History:
 * 			 1.initial release(DEC. 26 2018)
 *
 * @version  A001
 *
 *******************************************************************************************************/

#pragma once

//#include "bsp.h"
#include "drivers.h"
#include "../../../tl_common.h"


static inline void usbhw_set_printer_threshold(unsigned char th) {
	reg_usb_ep8_send_thre = th;
}

static inline unsigned int usbhw_get_ctrl_ep_irq(void) {
	return reg_ctrl_ep_irq_sta;
}

static inline void usbhw_clr_ctrl_ep_irq(int irq) {
#ifdef WIN32
	BM_CLR(reg_ctrl_ep_irq_sta, irq);
#else
	reg_ctrl_ep_irq_sta = irq;
#endif
}
static inline void usbhw_write_ctrl_ep_ctrl(unsigned char data) {
	reg_ctrl_ep_ctrl = data;
}

// Reset the buffer pointer
static inline void usbhw_reset_ctrl_ep_ptr(void) {
	reg_ctrl_ep_ptr = 0;
}

#if 0
#define usbhw_read_ctrl_ep_data()	(reg_ctrl_ep_dat)
#else
static inline unsigned char usbhw_read_ctrl_ep_data(void) {
#ifdef WIN32
	return 0;// usb_sim_ctrl_ep_buffer[usb_sim_ctrl_ep_ptr++];
#else
	return reg_ctrl_ep_dat;
#endif
}
#endif

static inline void usbhw_write_ctrl_ep_data(unsigned char data) {
	reg_ctrl_ep_dat = data;
#ifdef WIN32
	printf("%02x,", data);
#endif
}

static inline int usbhw_is_ctrl_ep_busy() {
	return reg_ctrl_ep_irq_sta & FLD_USB_EP_BUSY;
}

static inline unsigned char usbhw_read_ep_data(unsigned int ep) {
	return reg_usb_ep_dat(ep & 0x07);
}

static inline void usbhw_write_ep_data(unsigned int ep, unsigned char data) {
	reg_usb_ep_dat(ep & 0x07) = data;
#ifdef WIN32
	printf("%02x,", data);
#endif
}

static inline unsigned int usbhw_is_ep_busy(unsigned int ep) {
	return reg_usb_ep_ctrl(ep & 0x07) & FLD_USB_EP_BUSY;
}

static inline void usbhw_data_ep_ack(unsigned int ep) {
	reg_usb_ep_ctrl(ep & 0x07) = FLD_USB_EP_BUSY;
}

static inline void usbhw_data_ep_stall(unsigned int ep) {
	reg_usb_ep_ctrl(ep & 0x07) = FLD_USB_EP_STALL;
}

static inline void usbhw_reset_ep_ptr(unsigned int ep) {
	reg_usb_ep_ptr(ep & 0x07) = 0;
}

static inline unsigned int usbhw_get_eps_irq(void) {
	return reg_usb_irq;
}

static inline void usbhw_clr_eps_irq(int irq) {
	reg_usb_irq = irq;
}
