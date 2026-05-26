/********************************************************************************************************
 * @file    hci_tr.c
 *
 * @brief   This is the source file for BLE SDK
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
#include "hci_tr.h"
#include "hci_tr_def.h"
#include "hci_tr_h4.h"
#include "stack/ble/ble_multi/controller/ble_controller.h"


#if HCI_TR_EN


void HCI_Handler(void);

/**
 * @brief : HCI transport initialization.
 * @param : none.
 * @param : none.
 */
void HCI_TransportInit(void)
{
    #if HCI_TR_MODE == HCI_TR_H4
    HCI_Tr_H4Init(&bltHci_rxfifo);
    #endif
}

    /**
 * @brief : HCI transport main loop.
 * @param : none.
 * @param : none.
 */
    #if (TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM)
_attribute_ram_code_
    #endif
    void
    HCI_TransportPoll(void)
{
    #if HCI_TR_MODE == HCI_TR_H4
    HCI_Tr_H4RxHandler();
    HCI_Handler();
#endif
}


    #if (TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM)
_attribute_ram_code_
    #endif
    void
    HCI_RxHandler(void)
{
#if HCI_TR_MODE == HCI_TR_H4
    if (bltHci_rxfifo.wptr == bltHci_rxfifo.rptr) {
        return; //have no data
    }

    u8 *p = bltHci_rxfifo.p + (bltHci_rxfifo.rptr & bltHci_rxfifo.mask) * bltHci_rxfifo.size;

    if (p) {
        blc_hci_handler(&p[0], 0); //the second parameter is not used.
        bltHci_rxfifo.rptr++;
        return;
    }

    #endif
}

    #if (TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM)
_attribute_ram_code_
    #endif
    void
    HCI_TxHandler(void)
{
    #if HCI_TR_MODE == HCI_TR_H4

    static u8 uartTxBuf[4 + HCI_TR_TX_BUF_SIZE] = {0}; //[!!important]

    if (!ext_hci_getTxCompleteDone()) {
        return;
    }

    u8 *pBuf = uartTxBuf;

    u8        *p = NULL;
    hci_type_t type;
    if (bltHci_outIsofifo.wptr != bltHci_outIsofifo.rptr) { //Priority of HCI ISO DATA higher than HCI ACL data

        type = HCI_TYPE_ISO_DATA;
        p    = bltHci_outIsofifo.p + (bltHci_outIsofifo.rptr & bltHci_outIsofifo.mask) * bltHci_outIsofifo.size;
    } else if (bltHci_txfifo.wptr != bltHci_txfifo.rptr) {
        type = HCI_TYPE_ACL_DATA;
        p    = bltHci_txfifo.p + (bltHci_txfifo.rptr & bltHci_txfifo.mask) * bltHci_txfifo.size;
    }


    if (p) {
        u32 len = 0;
        BSTREAM_TO_UINT16(len, p);

        #if (TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM)
        smemcpy(pBuf, p, len);
        #else
        memcpy(pBuf, p, len);
        #endif

        ASSERT(len <= HCI_TX_FIFO_SIZE, HCI_TR_ERR_TR_TX_BUF);
        if (ext_hci_uartSendData(pBuf, len)) {
            if (type == HCI_TYPE_ACL_DATA) {
                bltHci_txfifo.rptr++;
            } else {
                bltHci_outIsofifo.rptr++;
            }

            return;
        }
    }
    #endif
}

    #if (TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM)
_attribute_ram_code_
    #endif
    void
    HCI_Handler(void)
{
    #if HCI_TR_MODE == HCI_TR_H4
    HCI_RxHandler();
    HCI_TxHandler();
#endif
}

#endif /* End of HCI_TR_EN */
