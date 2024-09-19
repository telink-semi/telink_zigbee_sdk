/********************************************************************************************************
 * @file    usbhw.h
 *
 * @brief   This is the header file for B92
 *
 * @author  Driver Group
 * @date    2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/**	@page USBHW
 *
 *	Introduction
 *	===============
 *	USB hard ware .
 *
 *	API Reference
 *	===============
 *	Header File: usbhw.h
 */

#pragma once

#include "analog.h"
#include "gpio.h"
#include "reg_include/register.h"
#include <stdbool.h>

/* For compatibility, usb_set_pin_en() is equivalent to usb_set_pin(1), configure the usb pin and enable the dp_through_swire function.*/
#define usb_set_pin_en()    usb_set_pin(1)

typedef enum {
	USB_EDP8_IN  = 8, // default buff len = 64
	USB_EDP1_IN  = 1, // default buff len = 8
	USB_EDP2_IN  = 2, // default buff len = 8
	USB_EDP3_IN  = 3, // default buff len = 16
	USB_EDP4_IN  = 4, // default buff len = 64
	USB_EDP5_OUT = 5, // default buff len = 64
	USB_EDP6_OUT = 6, // default buff len = 16
	USB_EDP7_IN  = 7, // default buff len = 16

	USB_EDP_PRINTER_IN = USB_EDP8_IN,
	USB_EDP_KEYBOARD_IN = USB_EDP1_IN,
	USB_EDP_MOUSE = USB_EDP2_IN,
	USB_EDP3_UNUSED_IN = USB_EDP3_IN,
	USB_EDP_AUDIO_IN = USB_EDP4_IN,
	USB_EDP_PRINTER_OUT = USB_EDP5_OUT,
	USB_EDP_SPEAKER = USB_EDP6_OUT,
	USB_EDP_MIC = USB_EDP7_IN,
	USB_EDP_MS_IN = USB_EDP_PRINTER_IN,
	USB_EDP_MS_OUT = USB_EDP5_OUT,
	USB_EDP_SOMATIC_IN = USB_EDP4_IN,
	USB_EDP_SOMATIC_OUT = USB_EDP5_OUT,
	USB_EDP_CDC_IN = USB_EDP4_IN,
	USB_EDP_CDC_OUT = USB_EDP5_OUT,
}usb_ep_index;

// #defined in the standard spec
enum {
	USB_HID_AUDIO       	= 2,
	USB_HID_MOUSE       	= 1,
	USB_HID_KB_MEDIA    	= 3,// media
	USB_HID_KB_SYS      	= 4,// system : power,sleep,wakeup
	USB_HID_SOMATIC			= 5,// somatic sensor,  may have many report ids
};



typedef enum
{
	USB_IRQ_RESET_MASK   =BIT(0),
	USB_IRQ_250US_MASK   =BIT(1),
	USB_IRQ_SUSPEND_MASK =BIT(2),
}usb_irq_mask_e;

typedef enum
{
	USB_IRQ_RESET_STATUS    =BIT(0),
	USB_IRQ_250US_STATUS    =BIT(1),
	USB_IRQ_SUSPEND_STATUS  =BIT(2),
	USB_IRQ_EP_SETUP_STATUS	=BIT(4),
	USB_IRQ_EP_DATA_STATUS  =BIT(5),
	USB_IRQ_EP_STA_STATUS   =BIT(6),
	USB_IRQ_EP_INTF_STATUS  =BIT(7),
}usb_irq_status_e;



/**
 * @brief     This function servers to set ed8 to fifo mode.
 * @return    none.
 */
static inline void usbhw_set_ep8_fifo_mode(void) {
	BM_SET(reg_usb_ep8_fifo_mode,FLD_USB_ENP8_FIFO_MODE);
}

/**
 * @brief     This function servers to reset the pointer of control Endpoint.
 * @return    none.
 */
static inline void usbhw_reset_ctrl_ep_ptr(void) {
	reg_ctrl_ep_ptr = 0;
}



/**
 * @brief     This function servers to set the value of control Endpoint.
 * @param[in] data - the value of control Endpoint
 * @return    none.
 */
static inline void usbhw_write_ctrl_ep_ctrl(unsigned char data) {
	reg_ctrl_ep_ctrl = data;
}

/**
 * @brief     This function servers to read the data of control Endpoint.
 * @return    the value of control Endpoint data
 */
static inline unsigned char usbhw_read_ctrl_ep_data(void) {
	return reg_ctrl_ep_dat;
}

/**
 * @brief     This function servers to write the data of control Endpoint.
 * @param[in] data -  the data of control Endpoint to write
 * @return    none
 */
static inline void usbhw_write_ctrl_ep_data(unsigned char data) {
	reg_ctrl_ep_dat = data;
}

/**
 * @brief     This function servers to determine whether control Endpoint is busy.
 * @return    1: busy; 0: not busy.
 */
static inline int usbhw_is_ctrl_ep_busy(void) {
	return reg_ctrl_ep_irq_sta & FLD_USB_EP_BUSY;
}

/**
 * @brief     This function servers to reset the pointer of Endpoint.
 * @param[in] ep - select the Endpoint
 * @return    none.
 */
static inline void usbhw_reset_ep_ptr(usb_ep_index ep) {
	reg_usb_ep_ptr(ep)= 0;
	reg_usb_ep_ptrh(ep)=0;
}

/**
 * @brief     This function servers to get the irq status of control Endpoint.
 * @return    none.
 */
static inline unsigned int usbhw_get_ctrl_ep_irq(void) {
	return reg_ctrl_ep_irq_sta;
}

/**
 * @brief     This function servers to clear the irq status of control Endpoint.
 * @param[in] ep - selected  the Endpoint
 * @return    none.
 */
static inline void usbhw_clr_ctrl_ep_irq(int ep) {

	reg_ctrl_ep_irq_sta = ep;

}
/**
 * @brief     This function servers to get the pointer of Endpoint.
 * @param[in] ep - select the Endpoint
 * @return    none.
 */
static inline unsigned short usbhw_get_ep_ptr(usb_ep_index ep) {
   return (reg_usb_ep_ptrh(ep)<<8)|reg_usb_ep_ptr(ep) ;
}

/**
 * @brief     This function servers to set the buffer address of data endpoint.
 * @param[in] ep - select the data endpoint.
 * @param[in] addr - data endpoint buffer address.
 * @return    none.
 */
static inline void usbhw_set_ep_addr(usb_ep_index ep,unsigned short addr)
{
	reg_usb_ep_buf_addr(ep) = addr&0xff;
	reg_usb_ep_buf_addrh(ep)=(addr>>8)&0xff;
}

/**
 * @brief     This function servers to set the irq mask of Endpoint.
 * @return    none.
 */
static inline void usbhw_set_eps_irq_mask(usb_ep_irq_e mask)
{
	reg_usb_ep_irq_mask|=mask;

}

/**
 * @brief     This function servers to clr the irq mask of Endpoint.
 * @return    none.
 */
static inline void usbhw_clr_eps_irq_mask(usb_ep_irq_e mask)
{
	reg_usb_ep_irq_mask &=(~mask);

}

/**
 * @brief     This function servers to get the irq status of Endpoint.
 * @return    none.
 */
static inline unsigned int usbhw_get_eps_irq(void) {
	return reg_usb_ep_irq_status;
}

/**
 * @brief     This function servers to clear the irq status of Endpoint.
 * @param[in] ep - selected  the Endpoint
 * @return    none.
 */
static inline void usbhw_clr_eps_irq(int ep) {
	reg_usb_ep_irq_status = ep;
}

/**
 * @brief     This function servers to set usb irq mask.
 * @param[in]  mask -the  irq mask of usb.
 * @return    none.
 */
static inline void usbhw_set_irq_mask( usb_irq_mask_e mask)
{
	reg_usb_irq_mask|=mask;
}

/**
 * @brief     This function servers to clr usb irq mask.
 * @param[in]  mask -the  irq mask of usb.
 * @return    none.
 */
static inline void usbhw_clr_irq_mask( usb_irq_mask_e mask)
{
	reg_usb_irq_mask &= (~mask);
}

/**
 * @brief     This function servers to get usb irq status.
 * @param[in]  status    - the  irq status of usb.
 * @retval     non-zero      - the interrupt occurred.
 * @retval     zero  - the interrupt did not occur.
 */
static inline unsigned char  usbhw_get_irq_status(usb_irq_status_e status)
{
	return reg_ctrl_ep_irq_sta&status;
}

/**
 * @brief     This function servers to clr usb irq status.
 * @param[in]  status -the  irq status of usb.
 * @return    none.
 */
static inline void usbhw_clr_irq_status(usb_irq_status_e status)
{
	reg_ctrl_ep_irq_sta=status;
}


/**
 * @brief     This function servers to enable Endpoint.
 * @param[in] ep - selected  the Endpoint
 * @return    none.
 */
static inline void  usbhw_set_eps_en(usb_ep_en_e ep)
{
	reg_usb_edp_en= ep;
}

/**
 * @brief     This function servers to enable Endpoint.
 * @param[in] ep - selected  the Endpoint
 * @return    none.
 */
static inline void  usbhw_set_eps_dis(usb_ep_en_e ep)
{
	reg_usb_edp_en &=(~ ep);
}

/**
 * @brief     This function servers to read the data of Endpoint.
 * @param[in] ep - selected the Endpoint
 * @return    the value of Endpoint
 */
static inline unsigned char usbhw_read_ep_data(unsigned int ep) {
	return reg_usb_ep_dat(ep);
}

/**
 * @brief     This function servers to write the data of Endpoint.
 * @param[in] ep   -  selected the Endpoint
 * @param[in] data -  the value of Endpoint
 * @return    none
 */
static inline void usbhw_write_ep_data(unsigned int ep, unsigned char data) {
	reg_usb_ep_dat(ep) = data;
}

/**
 * @brief     This function servers to determine whether Endpoint is busy.
 * @param[in] ep -  selected the Endpoint
 * @return    1: busy; 0: not busy.
 */
static inline unsigned int usbhw_is_ep_busy(unsigned int ep) {
	return reg_usb_ep_ctrl(ep) & FLD_USB_EP_BUSY;
}

/**
 * @brief     This function servers to set the specified data EndPoint to ack.
 * @param[in] ep -  select the data EndPoint.
 * @return    none.
 */
static inline void usbhw_data_ep_ack(unsigned int ep) {
	reg_usb_ep_ctrl(ep) = FLD_USB_EP_BUSY;
}

/**
 * @brief     This function servers to set the specified data EndPoint to stall.
 * @param[in] ep -  select the data EndPoint.
 * @return    none.
 */
static inline void usbhw_data_ep_stall(unsigned int ep) {
	reg_usb_ep_ctrl(ep) = FLD_USB_EP_STALL;
}


/**
 * @brief     This function servers to set the threshold of printer.
 * @param[in] th - set the threshold for printer
 * @return    none.
 */
static inline void usbhw_set_printer_threshold(unsigned char th) {
	reg_usb_ep8_send_thres = th;
}

/**
 * @brief     This function servers to set in endpoint buffer max size(except 7 aiso mode).
 * @param[in] max_size - in endpoint max size.
 * @return    none.
 */
static inline void usbhw_set_eps_max_size(unsigned int max_size)
{
    reg_usb_ep_max_size = max_size >> 3;
}

/**
 * @brief      This function disables the manual interrupt
 *             (Endpoint8 is the alias of endpoint0)
 * @param[in]  m - the irq mode needs to set
 * @return     none
 */
void usbhw_disable_manual_interrupt(int m);

/**
 * @brief      This function enable the manual interrupt
 * @param[in]  m - the irq mode needs to set
 * @return     none
 */
void usbhw_enable_manual_interrupt(int m);

/**
 * @brief      This function sends a bulk of data to host via the specified endpoint
 * @param[in]  ep - the number of the endpoint
 * @param[in]  data - pointer to the data need to send
 * @param[in]  len - length in byte of the data need to send
 * @return     none
 */
void usbhw_write_ep(unsigned int ep, unsigned char * data, int len);

/**
 * @brief      This function sends two bytes data to host via the control endpoint
 *             (handy help function)
 * @param[in]  v - the two bytes data need to send
 * @return     none
 */
void usbhw_write_ctrl_ep_u16(unsigned short v);

/**
 * @brief   This function reads two bytes data from host via the control endpoint
 * @return  the two bytes data read from the control endpoint
 */
unsigned short usbhw_read_ctrl_ep_u16(void);

/**
 * @brief      This function enables or disables the internal pull-up resistor of DP pin of USB interface
 * @param[in]  en - enables or disables the internal pull-up resistor(1: enable 0: disable)
 * @return     none
 */
static inline void usb_dp_pullup_en (int en)
{
	unsigned char dat = analog_read_reg8(0x0b);
	if (en) {
		dat = dat | BIT(7);
	}
	else
	{
		dat = dat & 0x7f ;
	}

	analog_write_reg8(0x0b, dat);
}

/**
 * @brief      This function serves to power on or down USB module
 * @param[in]  en - 1: power on 0: power down
 * @return     none
 */
static inline void usb_power_on(unsigned char en)
{
	if(en)
	{
		analog_write_reg8(0x7d,analog_read_reg8(0x7d)&0xfd);
	}
	else
	{
		analog_write_reg8(0x7d,analog_read_reg8(0x7d)|0x02);
	}
}

/**
 * @brief      This function serves to set dp_through_swire function.
 * @param[in]  dp_through_swire - 1: swire_usb_en 0: swire_usb_dis
 * @return     none.
 */
void dp_through_swire_en(bool dp_through_swire);

/**
 * @brief      This function serves to set GPIO MUX function as DP and DM pin of USB.
 * @param[in]  dp_through_swire - 1: swire_usb_en 0: swire_usb_dis
 * @return     none.
 * @note       1. Configure usb_set_pin(0) , there are some risks, please refer to the startup.S file about DP_THROUGH_SWIRE_DIS
 *                for detailed description (by default dp_through_swire is disabled). Configure usb_set_pin(1) to enable dp_through_swire again.
 *             2. When dp_through_swire is enabled, Swire and USB applications do not affect each other.
 */
void usb_set_pin(bool dp_through_swire);

/**
 * @brief		get vbus detect status.
 * @return		vbus detect status.
 * @note        When using the vbus (not vbat) power supply, the vbus detect status remains at 1. Conversely, it is 0.
 */
static inline unsigned char usb_get_vbus_detect_status(void)
{
	return (analog_read_reg8(0x69) & 0x40);
}
