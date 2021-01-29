/********************************************************************************************************
 * @file	zbhci.c
 *
 * @brief	This is the source file for zbhci
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
#include "tl_common.h"

#if ZBHCI_EN
#include "zbhci.h"
#if ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID
#include "drivers/usb/usb_common.h"
#endif

/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */
#if ZBHCI_USB_PRINT
	extern void usb_print_init(void);
	extern u8 usb_print_txMsg(u16 u16Type, u16 u16Length, u8 *pu8Data);
	extern void usb_print_task(void);
#elif ZBHCI_USB_CDC
	extern void usb_cdc_init(void);
	extern zbhciTx_e usb_cdc_txMsg(u16 u16Type, u16 u16Length, u8 *pu8Data);
#elif ZBHCI_USB_HID
	extern void usb_hid_init(void);
	extern zbhciTx_e usb_hid_txMsg(u16 u16Type, u16 u16Length, u8 *pu8Data);
#elif ZBHCI_UART
	extern void hci_uart_init(void);
	extern void uart_txMsg(u16 u16Type, u16 u16Length, u8 *pu8Data);
#endif


/*
 * @brief:   crc8 calculate
 *
 * */
u8 crc8Calculate(u16 type, u16 length, u8 *data){
	int n;
	u8 crc8;

	crc8  = (type   >> 0) & 0xff;
	crc8 ^= (type   >> 8) & 0xff;
	crc8 ^= (length >> 0) & 0xff;
	crc8 ^= (length >> 8) & 0xff;

	for(n = 0; n < length; n++)	{
		crc8 ^= data[n];
	}

	return crc8;
}


/*
 * @brief:   data send by HCI
 *
 * */
zbhciTx_e zbhciTx(u16 u16Type, u16 u16Length, u8 *pu8Data){
#if ZBHCI_USB_PRINT
	return usb_print_txMsg(u16Type, u16Length, pu8Data);
#elif ZBHCI_USB_CDC
	return usb_cdc_txMsg(u16Type, u16Length, pu8Data);
#elif ZBHCI_UART
    uart_txMsg(u16Type, u16Length, pu8Data);
    return ZBHCI_TX_SUCCESS;
#elif ZBHCI_USB_HID
    return usb_hid_txMsg(u16Type, u16Length, pu8Data);
#elif ZBHCI_SPI

#elif ZBHCI_I2C

#endif
    return ZBHCI_TX_FAILED;
}

void zbhciInit(void){
#if ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID
	//HW USB enable
	HW_USB_CFG();
#if ZBHCI_USB_PRINT
	usb_print_init();
	return;
#elif ZBHCI_USB_CDC
	usb_cdc_init();
#elif ZBHCI_USB_HID
    usb_hid_init();
#endif
    usb_init();
#elif ZBHCI_UART
    hci_uart_init();
#elif ZBHCI_SPI

#elif ZBHCI_I2C

#endif
}

void zbhciTask(void){
#if ZBHCI_USB_PRINT
	usb_print_task();
#elif ZBHCI_USB_CDC || ZBHCI_USB_HID
	usb_handle_irq();
#elif ZBHCI_UART
	/* process messages in the uart ISR, and we must check the uart RX state in main loop. */
	drv_uart_exceptionProcess();
#elif ZBHCI_SPI

#elif ZBHCI_I2C

#endif
}

#endif	/* ZBHCI_EN */
