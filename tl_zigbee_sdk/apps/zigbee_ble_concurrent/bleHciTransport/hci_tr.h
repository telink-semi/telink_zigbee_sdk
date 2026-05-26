/********************************************************************************************************
 * @file    hci_tr.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef HCI_TR_H_
#define HCI_TR_H_

#include "common/types.h"
#include "common/compiler.h"

#include "stack/ble/ble_multi/hci/hci.h"

#ifndef HCI_TR_EN
    #define HCI_TR_EN 0
#endif


#if HCI_TR_EN

    /*! HCI transport layer protocol selection. */
    #define HCI_TR_H4   0
    #define HCI_TR_H5   1
    #define HCI_TR_USB  2
    #define HCI_TR_MODE HCI_TR_H4


    /*! HCI ACL data packet max size define. */
    #ifndef HCI_TR_RX_BUF_SIZE
        #define HCI_TR_RX_BUF_SIZE (760) // must be larger than IAL length
    #endif
    #ifndef HCI_TR_TX_BUF_SIZE
        #define HCI_TR_TX_BUF_SIZE (760)
    #endif


    #ifndef HCI_TR_RX_PIN
        #error "please define UART RX Pin for HCI."
    #endif

    #ifndef HCI_TR_TX_PIN
        #error "please define UART TX Pin for HCI."
    #endif

    #ifndef HCI_TR_BAUDRATE
        #define HCI_TR_BAUDRATE 115200
    #endif


    #ifndef HCI_UART_SoftwareRxDone_EN //If the HCI_TR_BAUDRATE is greater than 1m(1000000), you are advised to enable it
        #define HCI_UART_SoftwareRxDone_EN 0
    #endif

    #if (HCI_UART_SoftwareRxDone_EN)
        #ifndef HCI_TR_RTS_PIN
            #error "please define UART RTS Pin for HCI."
        #endif

        #ifndef HCI_TR_CTS_PIN
            #error "please define UART CTS Pin for HCI."
        #endif
    #endif

typedef void (*HciH5PacketHandler_t)(u8 *pPacket, u32 len);

/*! HCI transport layer API. */

/**
 * @brief : HCI transport initialization.
 * @param : none.
 * @param : none.
 */
void HCI_TransportInit(void);

/**
 * @brief : HCI transport main loop.
 * @param : none.
 * @param : none.
 */
void HCI_TransportPoll(void);

#endif /* HCI_TR_EN */

#endif /* HCI_TR_H_ */
