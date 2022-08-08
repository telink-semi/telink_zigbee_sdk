/********************************************************************************************************
 * @file    mac_phy.h
 *
 * @brief   This is the header file for mac_phy
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

#ifndef MAC_PHY_H
#define MAC_PHY_H


/*********************************************************************
 * CONSTANTS
 */
#define RF_PKT_BUFF_LEN				(144)//(128+32) //(128+16)

#define CCA_THRESHOLD       		(-70)
#define RSSI_PASS_THRESHOLD			(-30)

/*********************************************************************
 * ENUMS
 */
enum{
    RF_SUCC,
    RF_CSMA_FAILURE,
};

enum{
    RF_RX_OFF,
    RF_RX_ON,
};

enum{
    CCA_ED_ONLY,
    CCA_CS_ONLY,
    CCA_ED_AND_CS,
    CCA_ED_OR_CS,
};

enum{
    RF_STATE_TX = 0,
    RF_STATE_RX,
    RF_STATE_ED,
    RF_STATE_OFF,
};

/*
 * Definition for RF Setting identifier.
 */
enum{
	SUCC,
    RF_BLACK_LIST_FULL,
    RF_BLACK_LIST_NO_MEMBER,
};

/*
 * Definition for RF Setting identifier.
 */
enum{
    /* General ID */
    RF_ID_CHANNEL               = 0x00,
    RF_ID_TX_POWER,
    RF_ID_RX_ONOFF,
    RF_ID_GENERAL_END           = 0x9F,
    
    /* Protocol Specific ID*/
    RF_ID_ZIGBEE                = 0xA0,
    RF_ID_MAC_SHORT_ADDR,
    RF_ID_MAC_EXT_ADDR,
    RF_ID_MAC_PANID,
    RF_ID_MAC_FRAME_PENDING,
    RF_ID_MAC_PAN_COORD,
    RF_ID_MAC_RECV_START,
    RF_ID_BECON_RECV,

    RF_ID_BLE                   = 0xB0,
};

/*********************************************************************
 * TYPES
 */
typedef enum{
    PHY_CCA_IDLE       = 0x04,
    PHY_CCA_TRX_OFF    = 0x03,
    PHY_CCA_BUSY       = 0x00,
}phy_ccaSts_t;

/**
 *  @brief  Definition for Telink RX packet format
 */
typedef struct{
    u32 dmaLen;
    u8  rssi;
    u8  rseverd[7];
    u8  payloadLen;
    u8  payload[1];
}rf_recvPkt_t;

/**
 *  @brief  Definition for Telink TX packet format
 */
typedef struct{
    u32 dmaLen;
    u8  rfLen;
    u8  payload[1];
}rf_sendPkt_t;

/*
 * Definition for RF Rx buffer format
 */
typedef struct{
    u8	*psdu;
    u32 timeStamp;
    s8  rssi;
    u8  psduLen;
}rx_buf_t;


extern u8 g_zb_txPowerSet;


/*********************************************************************
 * Public Functions
 */


/*********************************************************************
 * @fn      rf_reset
 *
 * @brief   Reset RF module.
 *
 * @param   none
 *
 * @return  none
 */
void rf_reset(void);

#define mac_phyReconfig()	rf_reset();

/*********************************************************************
 * @fn      rf_init
 *
 * @brief   Initialize RF module and do the RF power up.
 *
 * @param   none
 *
 * @return  none
 */
void rf_init(void);

/*********************************************************************
 * @fn      rf_setRxBuf
 *
 * @brief   Set RX buffer.
 *
 * @param   pBuf - RX buffer
 *
 * @return  none
 */
void rf_setRxBuf(u8 *pBuf);

/*********************************************************************
 * @fn      rf_TrxStateGet
 *
 * @brief   Get current TRX state.
 *
 * @param   none
 *
 * @return  state
 */
u8 rf_TrxStateGet(void);

/*********************************************************************
 * @fn      rf_setTrxState
 *
 * @brief   Go to specified state and set related register.
 *
 * @param   state - Specified state
 *
 * @return  none
 */
void rf_setTrxState(u8 state);

/*********************************************************************
 * @fn      rf_setChannel
 *
 * @brief   Set specified channel to RF module.
 *
 * @param   ch - 11~26
 *
 * @return  none
 */
void rf_setChannel(u8 chn);

/*********************************************************************
 * @fn      rf_getChannel
 *
 * @brief   Get specified channel.
 *
 * @param   none
 *
 * @return  chn
 */
u8 rf_getChannel(void);

/*********************************************************************
 * @fn      rf_setTxPower
 *
 * @brief   Set specified transmitted power.
 *
 * @param   txPower - Specified power
 *
 * @return  none
 */
void rf_setTxPower(u8 txPower);

/*********************************************************************
 * @fn      rf_getLqi
 *
 * @brief   Get calculated Link Quality value from specified rssi
 *
 * @param   rssi
 *
 * @return  lqi result
 */
u8 rf_getLqi(s8 rssi);

/*********************************************************************
 * @fn      rf_lqi2cost
 *
 * @brief   Get calculated cost value
 *
 * @param   lqi
 *
 * @return  cost
 */
u8 rf_lqi2cost(u8 lqi);

/*********************************************************************
 * @fn      rf_startED
 *
 * @brief   Start ED detect
 *
 * @param   none
 *
 * @return  none
 */
void rf_startED(void);

/*********************************************************************
 * @fn      rf_stopED
 *
 * @brief   Stop Energy Detect
 *
 * @param   none
 *
 * @return  ED result
 */
u8 rf_stopED(void);

/*********************************************************************
 * @fn      rf_performCCA
 *
 * @brief   Perform CCA
 *
 * @param   none
 *
 * @return  phy_ccaSts_t
 */
u8 rf_performCCA(void);

void rf802154_tx_ready(u8 *buf, u8 len);
void rf802154_tx(void);

/*********************************************************************
 * @fn      rf_paInit
 *
 * @brief   Initialize PA.
 *
 * @param   none
 *
 * @param   none
 *
 * @return  none
 */
void rf_paInit(u32 TXEN_pin, u32 RXEN_pin);
void rf_paShutDown(void);

void rf_ptaInit(u32 ZB_priorityPin, u32 WLAN_activePin);

_attribute_ram_code_ u32 mac_currentTickGet(void);

#endif  /* MAC_PHY_H */
