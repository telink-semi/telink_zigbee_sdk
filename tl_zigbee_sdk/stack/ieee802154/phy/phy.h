/********************************************************************************************************
 * @file    phy.h
 *
 * @brief   This is the header file for phy
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
#ifndef PHY_H
#define PHY_H


#define RF_PKT_BUFF_LEN         (144)//(128+32) //(128+16)

#define CCA_THRESHOLD           (-70)

/**
 * @brief PHY RF state
 */
enum {
    RF_STATE_TX = 0,
    RF_STATE_RX,
    RF_STATE_ED,
    RF_STATE_OFF,
};

/**
 * @brief PHY CCA state
 */
typedef enum {
    PHY_CCA_IDLE       = 0x04,
    PHY_CCA_TRX_OFF    = 0x03,
    PHY_CCA_BUSY       = 0x00,
} phy_ccaSts_t;

/**
 * @brief Definition for RF RX buffer format
 */
typedef struct {
    u8 *psdu;
    u32 timeStamp;
    s8 rssi;
    u8 psduLen;
} rx_buf_t;


extern u8 g_zb_txPowerSet;


/**
 * @fn     rf_reset
 *
 * @brief  Reset RF module.
 *
 * @param  none
 *
 * @return none
 */
void rf_reset(void);

#define mac_phyReconfig()       rf_reset()

/**
 * @fn     rf_init
 *
 * @brief  Initialize RF module and do the RF power up.
 *
 * @param  none
 *
 * @return none
 */
void rf_init(void);

/**
 * @fn     rf_setRxBuf
 *
 * @brief  Set RX buffer.
 *
 * @param  pBuf - RX buffer
 *
 * @return none
 */
void rf_setRxBuf(u8 *pBuf);

/**
 * @fn     rf_getTrxState
 *
 * @brief  Get current TRX state.
 *
 * @param  none
 *
 * @return state
 */
u8 rf_getTrxState(void);

/**
 * @fn     rf_setTrxState
 *
 * @brief  Go to specified state and set related register.
 *
 * @param  state - Specified state
 *
 * @return none
 */
void rf_setTrxState(u8 state);

/**
 * @fn     rf_setChannel
 *
 * @brief  Set specified channel to RF module.
 *
 * @param  chn - 11 ~ 26
 *
 * @return none
 */
void rf_setChannel(u8 chn);

/**
 * @fn     rf_getChannel
 *
 * @brief  Get specified channel.
 *
 * @param  none
 *
 * @return chn
 */
u8 rf_getChannel(void);

/**
 * @fn     rf_setTxPower
 *
 * @brief  Set specified transmitted power.
 *
 * @param  txPower - Specified power
 *
 * @return none
 */
void rf_setTxPower(u8 txPower);

/**
 * @fn     rf_getLqi
 *
 * @brief  Get calculated Link Quality value from specified rssi
 *
 * @param  rssi
 *
 * @return lqi result
 */
u8 rf_getLqi(s8 rssi);

/**
 * @fn     rf_lqi2cost
 *
 * @brief  Get calculated cost value
 *
 * @param  lqi
 *
 * @return cost
 */
u8 rf_lqi2cost(u8 lqi);

/**
 * @fn     rf_startEDScan
 *
 * @brief  Start ED detect
 *
 * @param  none
 *
 * @return none
 */
void rf_startEDScan(void);

/**
 * @fn     rf_stopEDScan
 *
 * @brief  Stop Energy Detect
 *
 * @param  none
 *
 * @return result
 */
u8 rf_stopEDScan(void);

/**
 * @fn     rf_performCCA
 *
 * @brief  Perform CCA
 *
 * @param  none
 *
 * @return phy_ccaSts_t
 */
u8 rf_performCCA(void);

/**
 * @fn     rf802154_tx_ready
 *
 * @brief  Prepare TX data
 *
 * @param  buf - data buffer
 * @param  len - length of data
 *
 * @return none
 */
void rf802154_tx_ready(u8 *buf, u8 len);

/**
 * @fn     rf802154_tx
 *
 * @brief  Data transmit
 *
 * @param  none
 *
 * @return none
 */
void rf802154_tx(void);

/**
 * @fn     rf_paInit
 *
 * @brief  Initialize PA.
 *
 * @param  tx_pin
 * @param  rx_pin
 *
 * @return none
 */
void rf_paInit(u32 tx_pin, u32 rx_pin);

/**
 * @fn     rf_paShutDown
 *
 * @brief  Turn off PA.
 *
 * @param  none
 *
 * @return none
 */
void rf_paShutDown(void);

/**
 * @fn     rf_ptaInit
 *
 * @brief  Initialize PTA.
 *
 * @param  priority_pin
 * @param  active_pin
 *
 * @return none
 */
void rf_ptaInit(u32 priority_pin, u32 active_pin);

/**
 * @fn     mac_currentTickGet
 *
 * @brief  Get current tick
 *
 * @param  none
 *
 * @return tick counter
 */
u32 mac_currentTickGet(void);

/**
 * @fn     tl_stackBusy
 *
 * @brief  Check if RF busy
 *
 * @param  none
 *
 * @return TRUE or FALSE
 */
bool tl_stackBusy(void);

bool zb_rfSwitchAllow(void);
bool zb_rfTxDoing(void);
void restore_zb_rf_context(void);

#endif /* PHY_H */
