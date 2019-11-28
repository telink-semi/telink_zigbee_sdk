/********************************************************************************************************
 * @file     usb_ctrl.h
 *
 * @brief    This file provides set of functions to manage the USB interface
 *
 * @author   jian.zhang@telink-semi.com
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

enum {
    //  3000 ms
    USB_TIME_BEFORE_ALLOW_SUSPEND = (3000*1000),
};

enum {
    USB_IRQ_SETUP_REQ = 0,
    USB_IRQ_DATA_REQ,
};


// telink usb report ctrl command. used mixed with USB_AUD_PLAY_PAUSE...
enum{
    USB_REPORT_NO_EVENT     = 0xf0,
    USB_REPORT_RELEASE      = 0xff,
};

/**
 * @brief This function initializes the USB interface
 * @param   none
 * @return none
 */
void USB_Init();

/**
 * @brief This function handles the USB related irq.
 * @param   none
 * @return none
 */
void USB_IrqHandle(void);