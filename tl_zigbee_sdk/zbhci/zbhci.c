/********************************************************************************************************
 * @file    zbhci.c
 *
 * @brief   This is the source file for zbhci
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
	extern zbhciTx_e uart_txMsg(u16 u16Type, u16 u16Length, u8 *pu8Data);
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
	return uart_txMsg(u16Type, u16Length, pu8Data);
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
