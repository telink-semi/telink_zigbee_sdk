/********************************************************************************************************
 * @file     usbvendor_i.h
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     Sep. 30, 2010
 *
 * @par      Copyright (c) Telink Semiconductor (Shanghai) Co., Ltd.
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

#include "tl_common.h"

#if (USB_VENDOR_ENABLE)
#include "../usbstd/usbstd.h"

static const u8 vendor_report_desc[] = {
	HID_DESCRIPTOR_VENDOR(1, 4, 0, 1, 64),
};

static inline u8 *usbvendor_get_report_desc(void){
	return (u8*)(vendor_report_desc);
}

static inline u16 usbvendor_get_report_desc_size(void){
	return (u16)(sizeof(vendor_report_desc));
}

#endif
