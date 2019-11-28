/********************************************************************************************************
 * @file     zbhci.c
 *
 * @brief	 Zigbee Host communication interface which responsible for receive data from lower layer and uplayer. And resolve
 * 			the received message
 *
 * @author
 * @date     June. 10, 2017
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
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

/**********************************************************************
 * INCLUDES
 */
#include "../include/zb_common.h"
#include "zbhci.h"


#if ZBHCI_EN
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
	extern u8 usb_printTxMsg(u16 u16Type, u16 u16Length, u8 *pu8Data);
	extern void usbPrintTask(void);

#elif ZBHCI_USB_CDC
	extern void zbhciRxCb(u8 *buf,u8 len);
	extern void usb_cdc_init(void);
	zbhciTx_e usb_cdc_txMsg(u16 u16Type, u16 u16Length, u8 *pu8Data);

#elif ZBHCI_UART
	extern void hci_uart_init(void);
	extern void uart_txMsg(u16 u16Type, u16 u16Length, u8 *pu8Data);
#endif


/*
 * @breif:   crc8 calculate
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
 * @breif:   data send by HCI
 *
 * */
zbhciTx_e zbhciTx(u16 u16Type, u16 u16Length, u8 *pu8Data){
#if ZBHCI_USB_PRINT
	return usb_printTxMsg(u16Type, u16Length, pu8Data);
#elif ZBHCI_USB_CDC
	return usb_cdc_txMsg(u16Type,u16Length,pu8Data);
#elif ZBHCI_UART
    uart_txMsg(u16Type, u16Length, pu8Data);
    return ZBHCI_TX_SUCCESS;
#elif ZBHCI_SPI

#elif ZBHCI_I2C

#endif
    return ZBHCI_TX_FAILED;
}

void zbhciInit(void){
#if ZBHCI_USB_PRINT
	usb_print_init();
#elif ZBHCI_USB_CDC
	usb_cdc_init();
#elif ZBHCI_UART
    hci_uart_init();
#elif ZBHCI_USB_HID
    usb_hid_init();
#elif ZBHCI_SPI

#elif ZBHCI_I2C

#endif
}

void zbhciTask(void){
#if ZBHCI_USB_PRINT
	usbPrintTask();
#elif ZBHCI_USB_CDC
	usbRwTask();
#elif ZBHCI_USB_HID
	usbHidTask();
#elif ZBHCI_UART
	/* process messages in the uart ISR, and we must check the uart RX state in main loop. */
	uart_exceptionProcess();
#elif ZBHCI_SPI

#elif ZBHCI_I2C

#endif
}

#endif	/* ZBHCI_EN */
