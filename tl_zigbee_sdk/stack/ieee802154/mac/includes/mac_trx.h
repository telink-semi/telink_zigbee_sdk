/********************************************************************************************************
 * @file    mac_trx.h
 *
 * @brief   This is the header file for mac_trx
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
#ifndef MAC_TRX_H
#define MAC_TRX_H


#if (ZB_COORDINATOR_ROLE || ZB_ROUTER_ROLE)
#define TX_QUEUE_BN                     32
#else
#define TX_QUEUE_BN                     16
#endif

/**
 * @brief MAC header frame control
 */
#define MAC_FCF_ACK_REQ_BIT             0x20
#define MAC_FCF_INTRA_PAN_BIT           0x40
#define MAC_FCF_DST_ADDR_BIT            0x0C
#define MAC_FCF_SRC_ADDR_BIT            0xC0
#define MAC_FCF_FRAME_TYPE              0x07

/**
 * @brief Frame control field bit masks
 */
#define MAC_FCF_FRAME_TYPE_MASK         0x0007
#define MAC_FCF_SEC_ENABLED_MASK        0x0008
#define MAC_FCF_FRAME_PENDING_MASK      0x0010
#define MAC_FCF_ACK_REQUEST_MASK        0x0020
#define MAC_FCF_INTRA_PAN_MASK          0x0040
#define MAC_FCF_DST_ADDR_MODE_MASK      0x0C00
#define MAC_FCF_FRAME_VERSION_MASK      0x3000
#define MAC_FCF_SRC_ADDR_MODE_MASK      0xC000

/**
 * @brief MAC frame field lengths in bytes
 */
#define MAC_FCF_FIELD_LEN               2       /* frame control field */
#define MAC_SEQ_NUM_FIELD_LEN           1       /* sequence number */
#define MAC_PAN_ID_FIELD_LEN            2       /* PAN ID  */
#define MAC_EXT_ADDR_FIELD_LEN          8       /* Extended address */
#define MAC_SHORT_ADDR_FIELD_LEN        2       /* Short address */
#define MAC_FCS_FIELD_LEN               2       /* FCS field */

/**
 * @brief Frame type
 */
#define MAC_FRAME_TYPE_BEACON           0
#define MAC_FRAME_TYPE_DATA             1
#define MAC_FRAME_TYPE_ACK              2
#define MAC_FRAME_TYPE_COMMAND          3
#define MAC_FRAME_TYPE_MAX_VALID        MAC_FRAME_TYPE_COMMAND

/**
 * @brief Frame control field bit positions
 */
#define MAC_FCF_FRAME_TYPE_POS          0
#define MAC_FCF_SEC_ENABLED_POS         3
#define MAC_FCF_FRAME_PENDING_POS       4
#define MAC_FCF_ACK_REQUEST_POS         5
#define MAC_FCF_INTRA_PAN_POS           6
#define MAC_FCF_DST_ADDR_MODE_POS       10
#define MAC_FCF_FRAME_VERSION_POS       12
#define MAC_FCF_SRC_ADDR_MODE_POS       14

//BIT(1) 1- FFD, 0- RFD
#define	MAC_CAP_GET_DEVICE_TYPE(c)      ((c >> 1) & 1)
#define	MAC_CAP_GET_RXON_WHEN_IDLE(c)	((c >> 3) & 1)

typedef struct {
    u16 shortAddr;
    addrExt_t ieeeAddr;
} access_addr_t;

typedef enum {
    RX_BUSY = BIT(0),
    TX_BUSY = BIT(1)
} rf_busy_st_e;

typedef enum {
    MAC_TX_IDLE,
    MAC_TX_CSMA,
    MAC_TX_UNDERWAY,
    MAC_TX_WAIT_ACK,
    MAC_TX_RETRY,
    MAC_TX_TIME_OUT,
    MAC_TX_DONE,
} mac_txState_e;

/**
 * @brief Definition of MAC generic frame type, used in both TX and RX
 */
typedef struct {
    u8 fAck:4;          /*!< Used in TX  */
    u8 fFramePending:4; /*!< Used in Poll */
    u8 psduLen;
    u8 cnfStatus;
    u8 seqNum;
    u8 *buf;
    u8 *txData;
    void *pendingList;
} mac_genFrame_t;

typedef	mac_genFrame_t tx_data_queue;


extern u8 MAC_TX_QUEUE_SIZE;
extern tx_data_queue g_txQueue[];
extern volatile u8 rf_busyFlag;


/**
 * @brief  Initialize the RF transceiver and state
 *
 * @param  none
 *
 * @return none
 */
void mac_trxInit(void);

/**
 * @brief  MAC packet filter
 *
 * @param  macPld   - mac payload
 * @param  len      - the length of the mac payload
 * @param  needDrop - 1: this frame should be drop
 * @param  ackPkt   - 1: this frame is an ACK frame
 *
 * @return the address of the source address
 */
u8 *zb_macDataFilter(u8 *macPld, u8 len, u8 *needDrop, u8 *ackPkt);

/**
 * @brief  Handle the received data
 *
 * @param  rxBuf     - the address of zb buffer
 * @param  data      - the address of the valid data buffer
 * @param  len       - data length
 * @param  ackPkt    - this frame is an ACK packet
 * @param  timestamp - the timestamp of this packet
 * @param  rssi      - RSSI of this packet
 *
 * @return none
 */
void zb_macDataRecvHandler(u8 *rxBuf, u8 *data, u8 len, u8 ackPkt, u32 timestamp, s8 rssi);

/**
 * @brief  TX done callback
 *
 * @param  none
 *
 * @return none
 */
void zb_macDataSendHandler(void);

/**
 * @brief  Get MAC header length
 *
 * @param  mHdr
 *
 * @return none
 */
u8 tl_zbMacHdrSize(tl_zb_mac_mhr_t *mHdr);

/**
 * @brief  Get MAC state
 *
 * @param  none
 *
 * @return 1: Busy; 0: Idle
 */
bool tl_zbMacStateBusy(void);

/**
 * @brief  Set MAC channel
 *
 * @param  chn - channel
 *
 * @return none
 */
void tl_zbMacChannelSet(u8 chn);

#define ZB_TRANSCEIVER_SET_CHANNEL(chn)         tl_zbMacChannelSet(chn)

#endif /* MAC_TRX_H */
