/********************************************************************************************************
 * @file    hci_tr_h4.c
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
#include "hci_tr_h4.h"
#include "hci_tr_def.h"
#include "platform.h"
#include "stack/ble/ble_multi/controller/ble_controller.h"


#if HCI_TR_EN

    // #define HCI_Tr_H4TimerEnable()  hciH4TrCB.flushTimer = clock_time() | 1
    #define HCI_Tr_H4TimerEnable()                                                                         \
        do {                                                                                               \
            if (hciH4TrCB.flushTimer == 0) {  \
                hciH4TrCB.flushTimer = clock_time() | 1;                                                   \
            }                                                                                              \
        } while (0)


    #define HCI_Tr_H4TimerDisable() hciH4TrCB.flushTimer = 0
    #define HCI_Tr_H4FlushHandler()                                                                        \
        do {                                                                                               \
            if (hciH4TrCB.flushTimer && clock_time_exceed(hciH4TrCB.flushTimer, HCI_H4_FLUSH_TO * 1000)) { \
                hciH4TrCB.flushTimer = 0;                                                                  \
                hciH4TrCB.backupCnt  = 0;                                                                  \
            }                                                                                              \
        } while (0)

u8 h4TrBackupBuf[HCI_H4_TR_RX_BUF_SIZE];
u8 h4TrRxBuf[HCI_H4_TR_RX_BUF_SIZE * HCI_H4_TR_RX_BUF_NUM];

static hci_fifo_t h4TrRxFifo = {
    HCI_H4_TR_RX_BUF_SIZE,
    HCI_H4_TR_RX_BUF_NUM,
    HCI_H4_TR_RX_BUF_NUM - 1,
    0,
    0,
    &h4TrRxBuf[0],
};

/*!  HCI Transmit main control block */
typedef struct
{
    hci_fifo_t *pHciRxFifo; /*!< Point to HCI rx fifo. */
    hci_fifo_t *pRxFifo;    /*!< Point to H4 rx fifo.  */
    u8         *pBackUpBuf;
    u32         flushTimer;
    u8          backupCnt;
    u8          align[3];
} HciH4TrCb_t;

static HciH4TrCb_t  hciH4TrCB;
void                HCI_Tr_H4UartRxIRQHandler(unsigned int *param);
ext_hci_InitTypeDef hci_h4_uart;

/**
 * @brief  : HCI Transport initialization.
 * @param  : pFifo    Pointer point to hci rx FIFO.
 * @return : None.
 */
void HCI_Tr_H4Init(hci_fifo_t *pHciRxFifo)
{
    hciH4TrCB.pHciRxFifo = pHciRxFifo;
    hciH4TrCB.pRxFifo    = &h4TrRxFifo;
    hciH4TrCB.pBackUpBuf = h4TrBackupBuf;
    hciH4TrCB.backupCnt  = 0;
    hciH4TrCB.flushTimer = 0;

    //UART HARDWARE INIT

    memset(&hci_h4_uart, 0, sizeof(ext_hci_InitTypeDef));
    hci_h4_uart.baudrate       = HCI_TR_BAUDRATE;
    hci_h4_uart.tx_Pin         = HCI_TR_TX_PIN; //
    hci_h4_uart.rx_Pin         = HCI_TR_RX_PIN; //
    hci_h4_uart.HwFlowCtl      = 0;
    hci_h4_uart.cts_Pin        = 0;
    hci_h4_uart.rts_Pin        = 0;
    hci_h4_uart.RxCpltCallback = HCI_Tr_H4UartRxIRQHandler;
    hci_h4_uart.TxCpltCallback = NULL;
    ext_hci_uartInit(&hci_h4_uart);

    ext_hci_uartReceData((hciH4TrCB.pRxFifo->p + 4), hciH4TrCB.pRxFifo->size - 4);
}

bool HCI_RxHaveData(void)
{
    if (hciH4TrCB.pRxFifo->wptr != hciH4TrCB.pRxFifo->rptr) {
        return TRUE;
    }

    return FALSE;
}

/**
 * @brief  : HCI Transmit backup handler
 * @param  : pPacket    Pointer point to data buffer.
 * @param  : len        The length of data.
 * @return : Returns the number of bytes processed.
 */
    #if (TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM)
_attribute_ram_code_
    #endif
    int
    HCI_Tr_H4BackUpHandler(u8 *pPacket, u32 len)
{
    u8 *pBuf      = hciH4TrCB.pBackUpBuf;
    u8  backupLen = hciH4TrCB.backupCnt;

    if ((backupLen == 0) && (len <= HCI_H4_TR_RX_BUF_SIZE)) {
    #if (TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM)
        smemcpy(pBuf, pPacket, len);
    #else
        memcpy(pBuf, pPacket, len);
    #endif
        hciH4TrCB.backupCnt = len;

        HCI_Tr_H4TimerEnable();
        return len;
    }

    u8 hciType = 0;
    BSTREAM_TO_UINT8(hciType, pBuf);

    u16         paramLen   = 0;
    hci_fifo_t *pHciRxFifo = hciH4TrCB.pHciRxFifo;

    switch (hciType) {
    case HCI_TR_TYPE_CMD:
        if (backupLen >= 1 + HCI_CMD_HEAD_LEN) {
            pBuf += 2; //skip opcode
            BSTREAM_TO_UINT8(paramLen, pBuf);

            u16 cpyLen = 1 + HCI_CMD_HEAD_LEN + paramLen - hciH4TrCB.backupCnt;
    #if (TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM)
            smemcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, min(cpyLen, len));
    #else
            memcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, min(cpyLen, len));
    #endif
            hciH4TrCB.backupCnt += min(cpyLen, len);

            if (hciH4TrCB.backupCnt == 1 + HCI_CMD_HEAD_LEN + paramLen) {
                u8 *p = pHciRxFifo->p + (pHciRxFifo->wptr & (pHciRxFifo->num - 1)) * pHciRxFifo->size;
    #if (TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM)
                smemcpy(p, hciH4TrCB.pBackUpBuf, hciH4TrCB.backupCnt);
    #else
                memcpy(p, hciH4TrCB.pBackUpBuf, hciH4TrCB.backupCnt);
    #endif
                pHciRxFifo->wptr++;

                hciH4TrCB.backupCnt = 0;
                HCI_Tr_H4TimerDisable();
            } else {
                HCI_Tr_H4TimerEnable();
            }
            return min(cpyLen, len);
        } else {
            if (backupLen + len >= 1 + HCI_CMD_HEAD_LEN) {
                u16 cnt    = 0;
                u16 cpyLen = 1 + HCI_CMD_HEAD_LEN - backupLen;
    #if (TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM)
                smemcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, cpyLen);
    #else
                memcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, cpyLen);
    #endif
                hciH4TrCB.backupCnt += cpyLen;
                pPacket += cpyLen;
                len -= cpyLen;
                cnt += cpyLen;

                pBuf += 2; //skip opcode
                BSTREAM_TO_UINT8(paramLen, pBuf);
                cpyLen = 1 + HCI_CMD_HEAD_LEN + paramLen - hciH4TrCB.backupCnt;
    #if (TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM)
                smemcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, min(cpyLen, len));
    #else
                memcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, min(cpyLen, len));
    #endif
                hciH4TrCB.backupCnt += min(cpyLen, len);
                cnt += min(cpyLen, len);

                if (hciH4TrCB.backupCnt == 1 + HCI_CMD_HEAD_LEN + paramLen) {
                    u8 *p = pHciRxFifo->p + (pHciRxFifo->wptr & (pHciRxFifo->num - 1)) * pHciRxFifo->size;
    #if (TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM)
                    smemcpy(p, hciH4TrCB.pBackUpBuf, hciH4TrCB.backupCnt);
    #else
                    memcpy(p, hciH4TrCB.pBackUpBuf, hciH4TrCB.backupCnt);
    #endif
                    pHciRxFifo->wptr++;

                    hciH4TrCB.backupCnt = 0;
                    HCI_Tr_H4TimerDisable();
                } else {
                    HCI_Tr_H4TimerEnable();
                }
                return cnt;
            } else {
    #if (TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM)
                smemcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, len);
    #else
                memcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, len);
    #endif
                hciH4TrCB.backupCnt += len;
                HCI_Tr_H4TimerEnable();
                return len;
            }
        }
        break;

    case HCI_TR_TYPE_ACL:
    case HCI_TR_TYPE_ISO:
        if (backupLen >= 1 + HCI_ACL_HEAD_LEN) {
            pBuf += 2; //skip connHandle
            BSTREAM_TO_UINT16(paramLen, pBuf);
            u16 cpyLen = 1 + HCI_ACL_HEAD_LEN + paramLen - backupLen;
    #if (TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM)
            smemcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, min(cpyLen, len));
    #else
            memcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, min(cpyLen, len));
    #endif
            hciH4TrCB.backupCnt += min(cpyLen, len);

            if (hciH4TrCB.backupCnt == 1 + HCI_ACL_HEAD_LEN + paramLen) {
                HCI_Tr_H4TimerDisable();

                u8 *p = pHciRxFifo->p + (pHciRxFifo->wptr & (pHciRxFifo->num - 1)) * pHciRxFifo->size;
    #if (TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM)
                smemcpy(p, hciH4TrCB.pBackUpBuf, hciH4TrCB.backupCnt);
    #else
                memcpy(p, hciH4TrCB.pBackUpBuf, hciH4TrCB.backupCnt);
    #endif
                pHciRxFifo->wptr++;

                hciH4TrCB.backupCnt = 0;
            } else {
                HCI_Tr_H4TimerEnable();
            }
            return min(cpyLen, len);
        } else {
            if (backupLen + len >= 1 + HCI_ACL_HEAD_LEN) {
                u16 cnt    = 0;
                u16 cpyLen = 1 + HCI_ACL_HEAD_LEN - backupLen;
    #if (TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM)
                smemcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, cpyLen);
    #else
                memcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, cpyLen);
    #endif
                hciH4TrCB.backupCnt += cpyLen;
                pPacket += cpyLen;
                len -= cpyLen;
                cnt += cpyLen;

                pBuf += 2; //skip connHandle
                BSTREAM_TO_UINT16(paramLen, pBuf);
                cpyLen = 1 + HCI_ACL_HEAD_LEN + paramLen - hciH4TrCB.backupCnt;
    #if (TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM)
                smemcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, min(cpyLen, len));
    #else
                memcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, min(cpyLen, len));
    #endif
                hciH4TrCB.backupCnt += min(cpyLen, len);
                cnt += min(cpyLen, len);

                if (hciH4TrCB.backupCnt == 1 + HCI_ACL_HEAD_LEN + paramLen) {
                    u8 *p = pHciRxFifo->p + (pHciRxFifo->wptr & (pHciRxFifo->num - 1)) * pHciRxFifo->size;
    #if (TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM)
                    smemcpy(p, hciH4TrCB.pBackUpBuf, hciH4TrCB.backupCnt);
    #else
                    memcpy(p, hciH4TrCB.pBackUpBuf, hciH4TrCB.backupCnt);
    #endif
                    pHciRxFifo->wptr++;

                    hciH4TrCB.backupCnt = 0;
                    HCI_Tr_H4TimerDisable();
                } else {
                    HCI_Tr_H4TimerEnable();
                }
                return cnt;
            } else {
    #if (TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM)
                smemcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, len);
    #else
                memcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, len);
    #endif
                hciH4TrCB.backupCnt += len;
                HCI_Tr_H4TimerEnable();
                return len;
            }
        }
        break;

    case HCI_TR_TYPE_EVENT:
    case HCI_TR_TYPE_SCO:
    default:
//        tlkapi_send_string_data(DBG_HCI_TR, "[I] Rx invalid data 2", pPacket, len);
        return len;
    }

    /* we will never be here */
    return 0;
}

    /**
 * @brief  : HCI Transmit RX handler
 * @param  : none.
 * @return : none.
 */
    #if (TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM)
_attribute_ram_code_
    #endif
    void
    HCI_Tr_H4RxHandler(void)
{
    HCI_Tr_H4FlushHandler();

    hci_fifo_t *pRxFifo = hciH4TrCB.pRxFifo;
    if (pRxFifo->wptr == pRxFifo->rptr) {
        return; //have no data
    }

    u8 *pPacket = pRxFifo->p + (pRxFifo->rptr & pRxFifo->mask) * pRxFifo->size;
    u32 len     = 0;
    BSTREAM_TO_UINT32(len, pPacket); //DMA mode length

    hci_fifo_t *pHciRxFifo = hciH4TrCB.pHciRxFifo;

    while (len) {
        if (hciH4TrCB.backupCnt) {
            u32 res = HCI_Tr_H4BackUpHandler(pPacket, len);
            pPacket += res;
            len -= res;

            if (len == 0) {
                continue;
            }
        }

        u8 *pBuf    = pPacket;
        u8  hciType = 0;
        BSTREAM_TO_UINT8(hciType, pBuf);
        u16 paramLen = 0;
        u16 pktLen   = 0;

        switch (hciType) {
        case HCI_TR_TYPE_CMD:
            if (len < 1 + HCI_CMD_HEAD_LEN) {
                HCI_Tr_H4BackUpHandler(pPacket, len);

                len = 0;
                continue;
            }

            pBuf += 2; //skip opcode
            BSTREAM_TO_UINT8(paramLen, pBuf);
            pktLen = 1 + HCI_CMD_HEAD_LEN + paramLen;
            break;

        case HCI_TR_TYPE_ACL:
        case HCI_TR_TYPE_ISO:
            if (len < 1 + HCI_ACL_HEAD_LEN) {
                HCI_Tr_H4BackUpHandler(pPacket, len);

                len = 0;
                continue;
            }
            pBuf += 2; //skip connHandle
            BSTREAM_TO_UINT16(paramLen, pBuf);

            if (hciType == HCI_TR_TYPE_ISO) {
                paramLen &= 0x03ff;
            }
            pktLen = 1 + HCI_ACL_HEAD_LEN + paramLen;
            break;

        case HCI_TR_TYPE_EVENT:
            if (len < 1 + HCI_EVT_HEAD_LEN) {
                len = 0;
                continue;
            }
            pBuf += 1; //skip evtCode
            BSTREAM_TO_UINT8(paramLen, pBuf);
            pktLen = 1 + HCI_EVT_HEAD_LEN + paramLen;
            if(len < pktLen )
            {
                len = 0;
                continue;
            }
            len -= pktLen;
            pktLen = 0;
            continue;

        case HCI_TR_TYPE_SCO:
            if (len < 1 + HCI_SCO_HEAD_LEN) {
                len = 0;
                continue;
            }
            pBuf += 2; //skip connHandler
            BSTREAM_TO_UINT8(paramLen, pBuf);
            pktLen = 1 + HCI_SCO_HEAD_LEN + paramLen;
            if(len < pktLen )
            {
                len = 0;
                continue;
            }
            len -= pktLen;
            pktLen = 0;
            continue;

        default:
            len = 0;
            continue;
        }

        if (pktLen && len >= pktLen) {
            if ((u8)(pHciRxFifo->wptr - pHciRxFifo->rptr) >= pHciRxFifo->num) {
//                tlkapi_send_string_data(DBG_HCI_TR, "[I] HCI RX FIFO overflow", 0, 0);
                return; //have no memory.
            }
            /* Copy packet to HCI Rx buffer */
            u8 *p = pHciRxFifo->p + (pHciRxFifo->wptr & (pHciRxFifo->num - 1)) * pHciRxFifo->size;
    #if (TIFS_VARIATION_WORKAROUND_MLP_CODE_IN_RAM)
            smemcpy(p, pPacket, pktLen);
    #else
            memcpy(p, pPacket, pktLen);
    #endif
            pHciRxFifo->wptr++;
//            tlkapi_send_string_data(1, "[I] ACL to HCI RX FIFO", p, pktLen);
            if(len < pktLen )
            {
                len = 0;
                continue;
            }
            len -= pktLen;
            pPacket += pktLen;
        } else if (pktLen && len < pktLen) {
            HCI_Tr_H4BackUpHandler(pPacket, len);

            len = 0;
            continue;
        }
    } /* while(len) */


    if (len == 0) {
        pRxFifo->rptr++;
    }
}

_attribute_ram_code_ void HCI_Tr_H4UartRxIRQHandler(unsigned int *param)
{
    (void)*param;
    /* Get the length of Rx data */
    u32         rxLen   = 0;
    hci_fifo_t *pRxFifo = hciH4TrCB.pRxFifo;
    u8         *p       = pRxFifo->p + (pRxFifo->wptr & pRxFifo->mask) * pRxFifo->size;

    if ((u8)(pRxFifo->wptr - pRxFifo->rptr) >= pRxFifo->num) {
        ext_hci_uartReceData((p + 4), pRxFifo->size - 4); //[!!important]
//        tlkapi_send_string_data(DBG_HCI_TR, "[I] UART Rx overflow", 0, 0);

        return;                                           //have no memory.
    }
    BSTREAM_TO_UINT32(rxLen, p);
    if (rxLen>0 && rxLen<(HCI_H4_TR_RX_BUF_SIZE-4)) {
        pRxFifo->wptr++;
        p = pRxFifo->p + (pRxFifo->wptr & pRxFifo->mask) * pRxFifo->size;
    }
    ext_hci_uartReceData((p + 4), pRxFifo->size - 4); //[!!important]
}


#endif /* end of HCI_TR_EN */
