/********************************************************************************************************
 * @file    hci.c
 *
 * @brief   This is the source file for hci
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
#include "hci.h"
#if ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID
#include "drivers/usb/usb_common.h"
#endif

tl_hci_cmd_handler g_hci_cmd_handler = NULL;
#if ZBHCI_BLE_CONTROLLER
#include "ble.h"
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
#if (ZBHCI_USB_PRINT || ZBHCI_SWIRE_MODE)
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
u8 crc8Calculate(u16 type, u16 length, u8 *data)
{
    int n;
    u8 crc8;

    crc8  = (type   >> 0) & 0xff;
    crc8 ^= (type   >> 8) & 0xff;
    crc8 ^= (length >> 0) & 0xff;
    crc8 ^= (length >> 8) & 0xff;

    for (n = 0; n < length; n++) {
        crc8 ^= data[n];
    }

    return crc8;
}

#if ZBHCI_BLE_CONTROLLER
u8 hci_txMsg(u16 u16Type, u16 u16Length, u8 *pu8Data){
    u32 header = HCI_FLAG_EVENT_BT_STD;
    u8 reLength = u16Length + 7;
//	u8 sendData[72] = {0};
    u8* sendData = (u8*)ev_buf_allocate(reLength);
    if(!sendData){
        return ZBHCI_TX_BUFFERFULL;
    }
    u8* p = sendData;

    u8 crc8 = crc8Calculate(u16Type, u16Length, pu8Data);

    header |= HCI_EVT_VENDOR_USE;

    *p++ = 0x55;
    *p++ = (u16Type >> 8) & 0xff;
    *p++ = (u16Type >> 0) & 0xff;
    *p++ = (u16Length >> 8) & 0xff;
    *p++ = (u16Length >> 0) & 0xff;
    *p++ = crc8;
    for(u8 n = 0; n < u16Length; n++){
        *p++ = pu8Data[n];
    }
    *p++ = 0xAA;
    blc_hci_send_data(header, sendData, reLength);

    ev_buf_free(sendData);

    return ZBHCI_TX_SUCCESS;
}
#endif

/*
 * @brief:   data send by HCI
 *
 * */
zbhciTx_e zbhciTx(u16 u16Type, u16 u16Length, u8 *pu8Data)
{
#if (ZBHCI_USB_PRINT || ZBHCI_SWIRE_MODE)
    return usb_print_txMsg(u16Type, u16Length, pu8Data);
#elif ZBHCI_USB_CDC
    return usb_cdc_txMsg(u16Type, u16Length, pu8Data);
#elif ZBHCI_UART
    return uart_txMsg(u16Type, u16Length, pu8Data);
#elif ZBHCI_USB_HID
    return usb_hid_txMsg(u16Type, u16Length, pu8Data);
#elif ZBHCI_BLE_CONTROLLER
    return hci_txMsg(u16Type, u16Length, pu8Data);
#elif ZBHCI_SPI

#elif ZBHCI_I2C

#endif
    return ZBHCI_TX_FAILED;
}

void zbhciInit(tl_hci_cmd_handler cmd_cb)
{
    g_hci_cmd_handler = cmd_cb;

#if ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID
    //HW USB enable
#if !ZBHCI_SWIRE_MODE
    HW_USB_CFG();
#endif
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

#elif ZBHCI_SWIRE_MODE
    usb_print_init();
    return;
#endif
}

void zbhciTask(void)
{
#if ZBHCI_USB_PRINT || ZBHCI_SWIRE_MODE
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
